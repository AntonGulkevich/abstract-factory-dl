/*
 * termometr.cpp
 *
 *  Created on: 21 мая 2015 г.
 *      Author: Youbrin_A
 */

#include "termometr.h"

Termometr::Termometr(DWORD deviceId) : _deviceId(deviceId)
{
//	auto ConfigPtr = XAdcPs_LookupConfig(_deviceId);
//	if (ConfigPtr == NULL)
//	{
//		_isAvaible = false;
//		return;
//	}
//	XAdcPs_CfgInitialize(&XAdcInst, ConfigPtr, ConfigPtr->BaseAddress);
//
//	auto Status = XAdcPs_SelfTest(&XAdcInst);
//	if (Status != XST_SUCCESS)
//	{
//		_isAvaible = false;
//		return;
//	}
//	XAdcPs_SetSequencerMode(&XAdcInst, XADCPS_SEQ_MODE_SAFE);
//
}


