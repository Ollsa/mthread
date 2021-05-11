#include "mthread.h"
#include <iostream>
#include <thread>
#include <vector>
#include <stdint.h>
#include <string>
#include <random>
#include "FIFO.h"
#include <algorithm>
#include <queue>
#include <mutex>

unsigned int crcTable[256];
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

void crcInit(void)
{
	uint32_t polynomial = 0xEDB88320;
	for (uint32_t i = 0; i < 256; i++)
	{
		uint32_t c = i;
		for (size_t j = 0; j < 8; j++)
		{
			if (c & 1) {
				c = polynomial ^ (c >> 1);
			}
			else {
				c >>= 1;
			}
		}
		crcTable[i] = c;
	}
};

void calculateCRC32(std::queue<Node> *fifo)
{
	unsigned int c = 0;
	std::string _string;

	while (fifo->size() > 0) {
		mt2.lock();
		Node node = fifo->front();
		fifo->pop();
		mt2.unlock();
		_string = node.data;
		if (!_string.empty())
		{
			uint32_t c = 0 ^ 0xFFFFFFFF;
			for (unsigned int j = 0; j < _string.size(); j++)
			{
				unsigned int data = _string[j];
				for (size_t i = 0; i < 4; ++i)
				{
					c = crcTable[(c ^ *(((unsigned char*)&data) + i)) & 0xFF] ^ (c >> 8);
				}
			}
		}
		std::cout << "CRC: " << std::this_thread::get_id() << ":" << (c ^ 0xFFFFFFFF) << std::endl;
	}
};

int main(int argc, char* argv[])
{
	unsigned int n = std::stoi(argv[1]);
	unsigned int m = std::stoi(argv[2]);
	unsigned int size = std::stoi(argv[3]);
	
	unsigned int countRandom = std::thread::hardware_concurrency() / 2;
	unsigned int countCRC = std::thread::hardware_concurrency() - countRandom;

	std::queue<Node> fifo;
	std::vector<int> resultCRC;
	std::vector<std::thread> vThreadRand(countRandom);
	std::vector<std::thread> vThreadCRC(countCRC);
	std::vector<unsigned int>crc;

	crcInit();
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