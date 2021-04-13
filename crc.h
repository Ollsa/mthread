#pragma once
#include <vector>
class CRC
{
	unsigned int crcTable[256];
private:
public:
	void crcInit(void);
	std::vector<unsigned int> calculateCRC32(std::vector<std::vector<unsigned int>>input_data_block);
};