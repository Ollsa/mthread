#include "crc.h"
#include <thread>
#include <mutex>

void CRC::crcInit(void)
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

std::vector<unsigned int> CRC::calculateCRC32(std::vector<std::vector<unsigned int>>input_data_block)
{
	std::vector<unsigned int> result;

	std::this_thread::sleep_for(std::chrono::milliseconds(50000));

	//m.lock();
	if (!input_data_block.empty())
	{
		for (unsigned int i = 0; i < input_data_block.size(); i++)
		{
			uint32_t c = 0 ^ 0xFFFFFFFF;
			if (!input_data_block.at(i).empty())
				for (unsigned int j = 0; j < input_data_block.at(i).size(); j++)
				{
					unsigned int data = input_data_block.at(i).at(j);
					for (size_t i = 0; i < 4; ++i)
					{
						c = crcTable[(c ^ *(((unsigned char*)&data) + i)) & 0xFF] ^ (c >> 8);
					}
				}
			result.push_back(c ^ 0xFFFFFFFF);
		}
	}
	//m.unlock();
	return result;
};