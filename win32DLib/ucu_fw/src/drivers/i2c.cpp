/*
 * i2c.cpp
 *
 *  Created on: 20 апр. 2015 г.
 *      Author: Youbrin_A
 */

#include "i2c.h"

#include "perfomancecounter.h"
#include "gpio.h"

//void status_handler(void* sender, u32 StatusEvent)
//{
//	if (StatusEvent == 0)
//		for(;;);
//}



I2C::I2C(UINT iicDdeviceId, UINT slaveDeviceId, UINT rate, UINT addressSize) : _iicDeviceId(iicDdeviceId), _slaveDeviceId(slaveDeviceId), _addressSize(addressSize)
{
	//XIicPs_Config *ConfigPtr;	/* Pointer to configuration data */
	//ConfigPtr = XIicPs_LookupConfig(_iicDeviceId);
	//XIicPs_CfgInitialize(&_iicInstance, ConfigPtr, ConfigPtr->BaseAddress);
	//if (XIicPs_SelfTest(&_iicInstance) == 0)
	//{
	//	//XIicPs_SetStatusHandler(&_iicInstance, this, status_handler);
	//	XIicPs_SetSClk(&_iicInstance, rate);
	//	if (XIicPs_BusIsBusy(&_iicInstance))
	//		XIicPs_Abort(&_iicInstance);
	//	// Проверка наличия КМ
	//	FillAddress(0);
	//	_isDeviceAvailable = XIicPs_MasterSendPolled(&_iicInstance, _addressBuff, _addressSize, _slaveDeviceId) == 0;
	//} else
	//	_isDeviceAvailable = false;
}

void I2C::FillAddress(UINT address)
{
	//if (_addressSize == 1)
	//{
	//	_addressBuff[0] = address & 0xFF;
	//	_addressBuff[2] = 0;
	//}
	//else if (_addressSize == 2)
	//{
	//	_addressBuff[0] = (address >> 8) & 0xFF;
	//	_addressBuff[1] = address & 0xFF;
	//	_addressBuff[2] = ((address >> 16) & 1);
	//}

}

void I2C::Read(void* buffer, DWORD address, UINT size)
{
	//if (_isDeviceAvailable)
	//{
	//	while (XIicPs_BusIsBusy(&_iicInstance));
	//	FillAddress(address);
	//	XIicPs_MasterSendPolled(&_iicInstance, _addressBuff, _addressSize, _slaveDeviceId | _addressBuff[2]);
	//	while (XIicPs_BusIsBusy(&_iicInstance));
	//	XIicPs_MasterRecvPolled(&_iicInstance, reinterpret_cast<u8*>(buffer), size, _slaveDeviceId  | _addressBuff[2]);
	//	while (XIicPs_BusIsBusy(&_iicInstance));
	//	Gpio::SVSet();
	//}
}

void I2C::Write(void* buffer, DWORD address, UINT size)
{
	//if (_isDeviceAvailable)
	//{
	//	while (XIicPs_BusIsBusy(&_iicInstance)) {}
	//	u8 addr[size + _addressSize];
	//	FillAddress(address);
	//	memcpy(addr, _addressBuff, _addressSize);
	//	memcpy(&addr[_addressSize], buffer, size);
	//	XIicPs_MasterSendPolled(&_iicInstance, addr, sizeof(addr), _slaveDeviceId  | _addressBuff[2]);
	//	PerfomanceCounter::WaitMs(10); // В DataSheet Write Cycle Time
	//}
}
