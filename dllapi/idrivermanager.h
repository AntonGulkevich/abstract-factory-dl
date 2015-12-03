#ifndef IDRIVERMANAGER
#define IDRIVERMANAGER

#include "../types.h"

class PerfomanceCounter;
class Termometr;
class Clock;
class I2C;
class FWLoader;
class Keys;
class UsbTr;
class Display;
class FlagManager;
class Gpio;

struct IDriverManager
{
	virtual ~IDriverManager(){};
	virtual void OnIrq(DWORD number) = 0;
	virtual void EnableEthernet(bool isEnable) = 0;
	virtual void EnableUsb(bool isEnable) = 0;
	virtual void UsbFromPLD(bool fromPLD) = 0;
	virtual void EnableAllIrq() = 0;
	virtual Gpio* GetGpio() = 0;
	virtual FlagManager* GetFlagManager() = 0;
	virtual Display* GetDisplay() = 0;
	virtual UsbTr* GetUSB() = 0;
	virtual Keys* GetKeys() = 0;
	virtual FWLoader* GetFWLoader() = 0;
	virtual I2C* GetI2CCommod() = 0;
	virtual Clock* GetClock() = 0;
	virtual Termometr* GetTermometr() = 0;
	virtual PerfomanceCounter* GetPerfomance() = 0;
	virtual void WriteEEPROM(void* buff, UINT address, UINT count) = 0;
	virtual void ReadEEPROM(void* buff, UINT address, UINT count) = 0;
#ifdef EMULATION
	virtual const char * getI2cCommodFileName() = 0;
	virtual void setI2cCommodFileName(const char* commodFileName_) = 0;
	//virtual void setI2cCommodFileName(const std::string &commodFileName_) = 0;
#endif
};

#endif

