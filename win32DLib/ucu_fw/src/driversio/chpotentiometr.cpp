/*
 * chpotentiometr.cpp
 *
 *  Created on: 01 июня 2015 г.
 *      Author: Youbrin_A
 */

#include "chpotentiometr.h"
#include "../driversiohw/potentiometr.h"

ChPotentiometr::ChPotentiometr(CPattern* const pattern, UINT number) : IChannelIn(pattern)
{
	_channel = NULL;
	_ioType = IOTYPES::ioPotentiometr;
	_number = number;

	_calibration = new ChannelCalibration(this);
	_filter = new ChannelFilter(this);
	_rate = new ChannelRate(this);
	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
		registers_t[i].id = REGISTER_ID::NULLID;

	registers_t[(UINT)REGISTER_ID::rNAME] = {REGISTER_ID::rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rVALUE] = {REGISTER_ID::rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rSTATE] = {REGISTER_ID::rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rMAXLEVEL] = {REGISTER_ID::rMAXLEVEL, rwConstant, rtDec, -200.0f, 200, 100.0f, false};
	registers_t[(UINT)REGISTER_ID::rMINLEVEL] = {REGISTER_ID::rMINLEVEL, rwConstant, rtDec, -200.0f, 200, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rGROUP] = {REGISTER_ID::rGROUP, rwConstant, rtDec, 0.0f, 99.0f, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rSUBGROUP] = {REGISTER_ID::rSUBGROUP, rwConstant, rtDec, 1.0f, 9.0f, 1.0f, false};
	registers_t[(UINT)REGISTER_ID::rUNIT] = {REGISTER_ID::rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, false};
	_calibration->CreateClibrationRegisters();
	_filter->CreateFilterRegisters();
	_rate->CreateRateRegisters();
	CreateRegisters();

}

void ChPotentiometr::InitRegisters()
{
	_channel->SetMaximumSignalLevel(registers_t[(UINT)REGISTER_ID::rMAXLEVEL].reg->GetValueInt());
	_channel->SetMinimumSignalLevel(registers_t[(UINT)REGISTER_ID::rMINLEVEL].reg->GetValueInt());
	_calibration->CreateKoeffTable();
	_filter->CreateFilterBuffer();
	_rate->CreateRateBuffer();
	UpdateDataToHW();
}

void ChPotentiometr::UpdateDataToHW()
{
	// Сброс пользовательского отказа
	ResetCheckAlarm();

}


void ChPotentiometr::UpdateHWToData()
{
	registers_t[(UINT)REGISTER_ID::rVALUE].reg->SetValue(_channel->GetValue());
	registers_t[(UINT)REGISTER_ID::rSTATE].reg->SetValue((UINT)_channel->GetState().dword);
	//Расчет фильтра
	_filter->UpdateValue();
	// Расчет скорости
	_rate->UpdateValue();
}

