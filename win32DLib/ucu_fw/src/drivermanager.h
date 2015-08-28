/*
 * drivermanager.h
 *
 *  Created on: 27 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef DRIVERMANAGER_H_
#define DRIVERMANAGER_H_

#include "drivers/clock.h"
#include "drivers/display.h"
#include "drivers/i2c.h"
#include "drivers/irqmanager.h"
#include "drivers/keys.h"
#include "drivers/qspi.h"
#include "drivers/usbtr.h"
#include "drivers/gpio.h"
#include "drivers/perfomancecounter.h"
#include "drivers/termometr.h"
#include "flagmanager.h"
#include "interfaces/iusingirq.h"
#include "utilities/fwloader.h"
#include "xparameters.h"

class DriverManager : IUsingIrq
{
private:
	PerfomanceCounter _perfomanceCounter;
	Gpio _gpio;
	IrqManager _irqManager;
	FlagManager _flagManager;
	Display _display;
	UsbTr _usb;
	Qspi _flash;
	Keys _keys;
	FWLoader _fwLoader;
	I2C _i2cCommod;
	I2C _i2cRTC;
	I2C _i2cEEPROM;
	Clock _clock;
	Termometr _termometr;



	UINT _svSetValue;
public:

	DriverManager();
	virtual ~DriverManager() {}

	virtual void OnIrq(DWORD number);

	void EnableEthernet(bool isEnable) { _gpio.SetOutput(0, isEnable ? 1 : 0); } // Enable Ethernet
	void EnableUsb(bool isEnable) { _gpio.SetOutput(7, isEnable ? 1 : 0); } // Enable Ethernet
	void UsbFromPLD(bool fromPLD) { _gpio.SetOutput(13, fromPLD ? 1 : 0); } // Enable Ethernet

	void EnableAllIrq() { _irqManager.EnableAll(); }

	Gpio* GetGpio() { return &_gpio; }
	FlagManager* GetFlagManager() { return &_flagManager; }
	Display* GetDisplay() { return &_display; }
	UsbTr* GetUSB() { return &_usb; }
	Keys* GetKeys() { return &_keys; }
	FWLoader* GetFWLoader() { return &_fwLoader; }
	I2C* GetI2CCommod() { return &_i2cCommod; }

	Clock* GetClock() { return &_clock; }
	Termometr* GetTermometr() { return &_termometr; }
	PerfomanceCounter* GetPerfomance() { return &_perfomanceCounter; }


	void WriteEEPROM(void* buff, UINT address, UINT count);
	void ReadEEPROM(void* buff, UINT address, UINT count);
};

#endif /* DRIVERMANAGER_H_ */
