#include "mthread.h"
#include <iostream>
#include <thread>
#include <vector>
#include <stdint.h>
#include <string>
#include <random>
#include "FIFO.h"

#include <algorithm>
unsigned int crcTable[256];

std::string doRandomNumbers(FIFO* fifo, unsigned int max_count, unsigned int data_size)
{
	std::string str;
	std::random_device rd;

	while (fifo->getSize() < max_count) {
		for (unsigned int i = 0; i < data_size; i++)
		{
			unsigned char number = rd();
			str.push_back(number);
		}
		fifo->push(str);
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

unsigned int calculateCRC32(FIFO* fifo)
{
	unsigned int c = 0;
	std::string _string;
	//std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	while (fifo->getSize() > 0) {
		 _string = fifo->pop();
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
	}
	//std::cout << std::this_thread::get_id() << std::endl;
	return c ^ 0xFFFFFFFF;
};

int main(int argc, char* argv[])
{
	unsigned int n = std::stoi(argv[1]);
	unsigned int m = std::stoi(argv[2]);
	unsigned int size = std::stoi(argv[3]);
	
	unsigned int count = std::thread::hardware_concurrency();

	FIFO fifo;
	FIFO fifo_crc;
	std::string str;
	
	std::vector<int> resultCRC;
	std::vector<std::thread> vThreadCRC(count/2);
	std::vector<std::thread> vThreadRand(count/2);
	std::vector<unsigned int>crc;

	crcInit();

	for (unsigned int i = 0; i < (count/2); i++)
	{
		vThreadRand.at(i) = std::thread
		([&]()
			{  
				doRandomNumbers(&fifo, 25, size);
			}
		);
		
	}

	for (unsigned int i = 0; i < (count / 2); i++)
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

	/*//Проверка CRC
	if (!resultCRC.empty())
	{
		for (unsigned int i = 0; i < resultCRC.size(); i++)
		{
			if (!resultCRC.at(i).empty())
			{
				if (resultCRC.at(i) != resultCRC.at(i + 1))
				{
					std::cout << i << " Unequal CRC values " << std::endl;
				}
			}
			else
			{
				std::cout << i << " Empty CRC values for all blocks  " << std::endl;
			}
		}
	}
	else
	{
		std::cout << " Empty CRC values for all blocks  " << std::endl;
	}*/
	//fifo.print();
	return 0;
}