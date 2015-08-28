#ifndef COMMOD_H
#define COMMOD_H

#include <vector>

#include "../types.h"

class I2C;

// Управление списком кофигураций загруженых в КМ
class Commod
{
private:
	I2C* _i2c;

public:
	enum
	{
		COMMOD_START_CONFIG_NUM = 64*1024 - 4
	};
	Commod(I2C* i2c) : _i2c(i2c)	{ opened = -1; ptr = 0; }
	virtual ~Commod()	{};
	void Init();
	int OpenDevice(UINT index);
	int OpenDevice(char* name)	{ return 0; }
	UINT GetDeviceCount() { return cFat.size(); }
	char GetChar();
	bool Eof() { return opened == -1 || ptr >= cFat[opened].size; }
	void Close()	{ opened = -1; }
	void ShowConfig();	// Показать конфигурацию в терминал
	bool IsConfigurationAvaible() { return cFat.size() > 0; }

	void WriteConfig(char* buffer, UINT size);
	void ReadConfig(char* buffer);
	UINT GetConfigSize() { return cFat.size() == 0 ? 0 : cFat[cFat.size()-1].address + cFat[cFat.size()-1].size; }

private:
	WORD GetCurArchiveConfig();
	struct FAT
	{
		UINT address;
		DWORD size;
	};

	std::vector<FAT> cFat;
	char _currentDevice[65536];
	short opened;  // -1 ниодно устройство не открыто
	UINT ptr;	// Указатель на позицию в текущем файле

};



#endif /* COMMOD_H */
