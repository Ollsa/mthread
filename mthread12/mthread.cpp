#include "mthread.h"
#include <iostream>
#include <thread>
#include <vector>
#include <stdint.h>
#include <string>
#include <random>
#include "FIFO.h"
#include "CRC.h"
#include <algorithm>
#include <queue>
#include <mutex>

std::mutex mt1;
std::mutex mt2;

std::string doRandomNumbers(std::queue<Node> *fifo, unsigned int max_count, unsigned int data_size)
{
	std::string str;
	std::random_device rd;
	while (fifo->size() < max_count) {
		for (unsigned int i = 0; i < data_size; i++)
		{
			unsigned char number = rd();
			str.push_back(number);
		}
		mt1.lock();
		auto id = std::this_thread::get_id();
		uint32_t* ptr = (uint32_t*)&id;
		unsigned int prId = *ptr;
		fifo->push({ str, prId });
		mt1.unlock();
		str.clear();
	}
	return str;
};

void calculateCRC32(std::queue<Node> *fifo)
{
	unsigned int crc = 0;

	while (fifo->size() > 0) {
		mt2.lock();
		Node node = fifo->front();
		fifo->pop();
		mt2.unlock();
		if (!node.data.empty())
		{
			crc = CRC::Calculate(&node.data, sizeof(node.data), CRC::CRC_32());
		}
		std::cout << "CRC: " << std::this_thread::get_id() << ":" << (crc) << std::endl;
	}
};

int main(int argc, char* argv[])
{
	unsigned int size = std::stoi(argv[3]);
	
	unsigned int countRandom = std::thread::hardware_concurrency() / 2;
	unsigned int countCRC = std::thread::hardware_concurrency() - countRandom;

	std::queue<Node> fifo;
	std::vector<std::thread> vThreadRand(countRandom);
	std::vector<std::thread> vThreadCRC(countCRC);

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	for (unsigned int i = 0; i < countRandom; i++)
	{
		vThreadRand.at(i) = std::thread
		([&]()
			{  
				doRandomNumbers(&fifo, 25, size);
			}
		);
	}

	for (unsigned int i = 0; i < countCRC; i++)
	{
		vThreadCRC.at(i) = std::thread
		([&]()
			{
				calculateCRC32(&fifo);
			}
		);
	}
	
	for (unsigned int i = 0; i < vThreadRand.size(); i++)
	{
		vThreadRand.at(i).join();
	}

	for (unsigned int i = 0; i < vThreadCRC.size(); i++)
	{
		vThreadCRC.at(i).join();
	}

	return 0;
}