#pragma once
#include <random>
#include <string>

struct rNumberStruct {
	std::string randomStr;
	unsigned int size;
};

class RandomNumber
{
private:
	/// ???
	//std::unique_ptr<rNumberStruct> rNumber = ...;
	rNumberStruct rNumber;

public:
	RandomNumber(unsigned int blockSize)
	{
		rNumber.size = blockSize;
		std::random_device rd;
		for (unsigned int i = 0; i < blockSize; i++)
		{
			unsigned char number = rd();
			rNumber.randomStr.push_back(number);
		}
	};

	~RandomNumber() {};

	std::string getRandomStr()
	{
		return rNumber.randomStr;
	};

	unsigned int getRandomDataSize()
	{
		return rNumber.size;
	};

	std::string setRandomDataSize(unsigned int _size)
	{
		rNumber.size = _size;
	};
};

