#include "commod.h"
#include <string.h>

#include "../drivers/gpio.h"
#include "../drivers/i2c.h"

void Commod::Init()
{
	WORD lFat[128];
	memset(lFat, 0xFF, 256);
	_i2c->Read(lFat, 0, 256);
	cFat.clear();
	for (int i = 0; (lFat[i] != 0xFFFF && i < 128); i+=2)
	{
		FAT f;
		f.address = (UINT)(lFat[i] << 8);
		f.size = lFat[i + 1];
		cFat.push_back(f);
	}
}

int Commod::OpenDevice(UINT index)
{
	if (index >= cFat.size())
		return -1;
	opened = index;
	_i2c->Read(_currentDevice, cFat[opened].address, cFat[opened].size);
	_currentDevice[cFat[opened].size] = 0;
	ptr = 0;
	return opened;
}

char Commod::GetChar()
{
	if (opened == -1)
		return 0;
	return _currentDevice[ptr++];
}


void Commod::WriteConfig(char* buffer, UINT size)
{
	for(UINT i = 0; i < size; i+=256)
	{
		Gpio::SVSet();
		_i2c->Write(&buffer[i], i, 256);
	}
}

void Commod::ReadConfig(char* buffer)
{
	UINT size = GetConfigSize();
	if (size > 0xFFFF)
		size = 0xFFFF;
	for(UINT i = 0; i < size; i+=256)
	{
		Gpio::SVSet();
		_i2c->Read(&buffer[i], i, 256);
	}
}

