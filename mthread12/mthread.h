#pragma once

struct Parameter
{
	unsigned int fifoSize;
	unsigned int dataBlockSize;
	Parameter(unsigned int _fifoSize, unsigned int _dataBlockSize) : fifoSize(_fifoSize), dataBlockSize(_dataBlockSize) {};
};
