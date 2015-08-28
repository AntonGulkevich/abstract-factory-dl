/*
 * gpio.h
 *
 *  Created on: 20 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef GPIO_H_
#define GPIO_H_
//#include "xgpiops.h"
#include "../types.h"

class Gpio
{
private:
	UINT _deviceId;
	//static XGpioPs _gpio;	/* The driver instance for GPIO Device. */
	static int svSetValue;

public:

	enum class LEDS
	{
		L3_1 = 9, // HL2
		L3_2 = 10,
		L3_3 = 11, // HL4

		L4_1 = 12,
		L4_2 = 41,
		L4_3 = 42,
		L4_4 = 43,

		L7_1 = 32, // HL9
		L7_2 = 33,
		L7_3 = 34,
		L7_4 = 28, // HL12
		L7_5 = 29,
		L7_6 = 30,
		L7_7 = 31,
	};

	Gpio(UINT deviceId) : _deviceId(deviceId) {}
	virtual ~Gpio() {}

	void Init();

	UINT GetInput(UINT number);
	void SetOutput(UINT number, UINT value);

	void SetLED(LEDS led, bool isOn) { SetOutput((UINT)led, isOn ? 0 : 1); }
	void SetCMWriteProtect(bool isOn) { SetOutput(35, isOn ? 1 : 0); }

	static void SVSet()
	{
		svSetValue = 1 - svSetValue;
		/* XGpioPs_SetDirectionPin(&_gpio, 8, 1);
		 XGpioPs_SetOutputEnablePin(&_gpio, 8, 1);
		 XGpioPs_WritePin(&_gpio, 8, svSetValue);*/
		/*XGpioPs_SetDirectionPin(&_gpio, 9, 1);
		XGpioPs_SetOutputEnablePin(&_gpio, 9, 1);
		XGpioPs_WritePin(&_gpio, 9, svSetValue);*/
	}


	/*	Gpio gpio(XPAR_PS7_GPIO_0_DEVICE_ID);
		gpio.Init();
		gpio.SetOutput(0, 1);  // Enable Ethernet
		gpio.SetOutput(7, 1);  // Enable USB
		gpio.SetOutput(13, 1); // USB выход от PC - 0, USB выход от PLD
		gpio.SetOutput(8, 1);  // WatchDog*/
};

#endif /* GPIO_H_ */
