#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <thread>
#include <random>
#include <vector>
#include <queue>
#include "CRC.h"
#include "Fifo.h"
#include "mthread.h"
#include "RandomNumber.h"

/// <summary>
/// Task for calculation CRC32 or all elements fifo 
/// </summary>
/// <param name="fifo"> Queue with data for calculating CRC32</param>
void calculateCrc32Task(Fifo *fifo)
{
	unsigned int crc = 0;

	//todo//while () {
	
	//todo	Если другой процесс положил данные после того как изменился размер
		while (fifo->getSize() > 0) {
			Node node = fifo->getNode();
			if (!node.data.empty())
			{
				crc = CRC::Calculate(&node.data, sizeof(node.data), CRC::CRC_32());
			}
			std::cout << "CRC: " << std::this_thread::get_id() << ":" << (crc) << std::endl;
		}
	//}
};

/// <summary>
/// Task for generating queue whith random data blocks
/// </summary>
/// <param name="fifo">Queue for saving random blocks</param>
/// <param name="max_count">Length of queue</param>
/// <param name="data_size">Length of random blocks</param>
void randomNumbersTask(Fifo* fifo, const unsigned int max_count, unsigned int data_size)
{
	while (fifo->getSize() < max_count) {
		std::unique_ptr<RandomNumber> rNumber = std::make_unique<RandomNumber>(data_size);
		auto id = std::this_thread::get_id();
		uint32_t* ptr = (uint32_t*)&id;
		unsigned int prId = *ptr;
		fifo->setNode(rNumber.get()->getRandomStr(), prId);
	}
};

/// <summary>
/// Clean up all resources 
/// </summary>
void cleanUpResources(Fifo* fifo)
{
	delete fifo;
}

/// <summary>
/// Handler for Control+C
/// </summary>
/// <param name="signum"></param>
void signalHandler(int signum)
{
	std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
	//cleanUpResources();
	//exit(signum);
}

int main(int argc, char* argv[])
{
	//todo
	unsigned int fifoSize = std::stoi(argv[2]);
	unsigned int dataBlockSize = std::stoi(argv[3]);
	
	unsigned int countRandomTask = std::thread::hardware_concurrency() / 2;
	if (countRandomTask <= 0) { countRandomTask = 1; };
	
	signed int countCrcTask = std::thread::hardware_concurrency() - countRandomTask;
	if (countCrcTask <= 0) { countCrcTask = 1; };

	std::vector<std::thread> vThreadRand(countRandomTask);
	std::vector<std::thread> vThreadCrc(countCrcTask);

	signal(SIGINT, signalHandler);

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	//todo;
	Fifo* fifo = new Fifo(fifoSize);

	try
	{
		for (unsigned int i = 0U; i < countRandomTask; i++)
		{
			vThreadRand.push_back(std::thread
			([&]()
				{
					randomNumbersTask(fifo, 11L, dataBlockSize);
				}
			));
		}

		for (unsigned int i = 0U; i < countCrcTask; i++)
		{
			vThreadCrc.push_back(std::thread
			([&]()
				{
					calculateCrc32Task(fifo);
				}
			));
		}

		for (unsigned int i = 0; i < vThreadRand.size(); i++)
		{
			if (vThreadRand.at(i).joinable())
			{
				vThreadRand.at(i).join();
			}
			/// что делать, если не joinable????
		}

		for (unsigned int i = 0; i < vThreadCrc.size(); i++)
		{
			if (vThreadCrc.at(i).joinable())
			{
				vThreadCrc.at(i).join();
			}
		}
		cleanUpResources(fifo);
	}
	catch (std::out_of_range e)
	{
		std::cerr << "We caught an exception of type out_of_range: "<<  e.what() << std::endl;
		cleanUpResources(fifo);
	}
	catch (std::length_error e)
	{
		std::cerr << "We caught an exception of type std::length_error: " << e.what() << std::endl;
		cleanUpResources(fifo);
	}
	catch (...)
	{
		std::cerr << "We caught unhandled exception " << std::endl;
		cleanUpResources(fifo);
	}
	return 0;
}
