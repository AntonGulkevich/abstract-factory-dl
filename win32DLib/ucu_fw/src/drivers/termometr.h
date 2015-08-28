/*
 * termometr.h
 *
 *  Created on: 21 мая 2015 г.
 *      Author: Youbrin_A
 */

#ifndef TERMOMETR_H_
#define TERMOMETR_H_
#include "../types.h"
//#include <xadcps.h>

class Termometr
{

private:
	DWORD _deviceId;
	//XAdcPs XAdcInst;      /* XADC driver instance */

	bool _isAvaible;

public:
	Termometr(DWORD deviceId);
	virtual ~Termometr() {}

	//float GetCurrentTemperature() { auto TempRawData = XAdcPs_GetAdcData(&XAdcInst, XADCPS_CH_TEMP); return XAdcPs_RawToTemperature(TempRawData); }
	float GetCurrentTemperature() {};
};

#endif /* TERMOMETR_H_ */
