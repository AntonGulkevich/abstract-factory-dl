/*
 * drivermanager.cpp
 *
 *  Created on: 27 апр. 2015 г.
 *      Author: Youbrin_A
 */

#include "drivermanager.h"
#include "drivers/gpio.h"
#include "utilities/console.h"

DriverManager::DriverManager() :
	_perfomanceCounter(XPAR_TOP_USB_0_BASEADDR),
	_gpio(XPAR_PS7_GPIO_0_DEVICE_ID),
	_irqManager(&_perfomanceCounter),
	_flagManager(&_perfomanceCounter),
	_display(XPAR_EX_MAX_0_BASEADDR),
	_usb(XPAR_TOP_USB_0_BASEADDR, XPAR_FABRIC_TOP_USB_0_PKT_IRQ_INTR),
	_flash(XPAR_PS7_QSPI_0_DEVICE_ID),
	_keys(XPAR_EX_MAX_0_BASEADDR),
	_fwLoader(&_flash, &_display, &_usb),
	_i2cCommod(XPAR_PS7_I2C_1_DEVICE_ID, 0x50, 400000, 2),
	_i2cRTC(XPAR_PS7_I2C_0_DEVICE_ID, 0x68, 100000, 1),
	_i2cEEPROM(XPAR_PS7_I2C_0_DEVICE_ID, 0x54, 100000, 2),
	_clock(&_i2cRTC),
	_termometr(XPAR_XADCPS_0_DEVICE_ID)
{
	_svSetValue = 0;
	_gpio.Init();
	EnableEthernet(true);
	EnableUsb(true);
	UsbFromPLD(true);

	_display.Init();
	_flash.Init();
	_usb.Init();
	_clock.Init();

	RegIRQ(XPS_IRQ_INT_ID);

}

void DriverManager::OnIrq(DWORD number)
{
	_gpio.SetLED(Gpio::LEDS::L7_1, true);
	_clock.OnPowerDown();
	Console::TraceLine("Сбой по питанию");
	_irqManager.ClearPeripheralIrq(XPS_IRQ_INT_ID);
	for(;;);
}

void DriverManager::WriteEEPROM(void* buff, UINT address, UINT count)
{
	const UINT pageSize = 256;
	// Постраничная запись, надо ли?
	if (count < pageSize)
		_i2cEEPROM.Write(buff, address, count);
	else
	{
		UINT startAddress = address % pageSize;
		UINT offset = 0;
		if (startAddress > 0)
		{
			_i2cEEPROM.Write(buff, address, pageSize - startAddress);
			offset = pageSize - startAddress;
		}
		for(; (int)offset < (int)(count-pageSize); offset += pageSize)
			_i2cEEPROM.Write(&reinterpret_cast<BYTE*>(buff)[offset], address + offset, pageSize);
		if ((int)(offset - count) > 0)
			_i2cEEPROM.Write(&reinterpret_cast<BYTE*>(buff)[offset], address + offset, count-offset);
	}
}

void DriverManager::ReadEEPROM(void* buff, UINT address, UINT count)
{
	const UINT pageSize = 256;
	if (count < pageSize)
		_i2cEEPROM.Read(buff, address, count);
	else
	{
		UINT startAddress = address % pageSize;
		UINT offset = 0;
		if (startAddress > 0)
		{
			_i2cEEPROM.Read(buff, address, pageSize - startAddress);
			offset = pageSize - startAddress;
		}
		for(; (int)offset < (int)(count-pageSize); offset += pageSize)
			_i2cEEPROM.Read(&reinterpret_cast<BYTE*>(buff)[offset], address + offset, pageSize);
		if ((int)(offset - count) > 0)
			_i2cEEPROM.Read(&reinterpret_cast<BYTE*>(buff)[offset], address + offset, count-offset);
	}

}


