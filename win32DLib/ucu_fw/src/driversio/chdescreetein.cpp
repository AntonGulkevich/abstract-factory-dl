/*
 * chpotentiometr.cpp
 *
 *  Created on: 01 июн€ 2015 г.
 *      Author: Youbrin_A
 */

#include "chdescreetein.h"
#include "../driversiohw/descreeteinput.h"

ChDescreeteIn::ChDescreeteIn(CPattern* const pattern, UINT number) : IChannelIn(pattern)
{
	_channel = NULL;
	_ioType = IOTYPES::ioCheck;
	_number = number;

	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
		registers_t[i].id = REGISTER_ID::NULLID;

	registers_t[(UINT)REGISTER_ID::rNAME] = {REGISTER_ID::rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rVALUE] = {REGISTER_ID::rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rSTATE] = {REGISTER_ID::rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false};
	CreateRegisters();

}

void ChDescreeteIn::InitRegisters()
{
	UpdateDataToHW();
}


void ChDescreeteIn::UpdateDataToHW()
{

}


void ChDescreeteIn::UpdateHWToData()
{
	registers_t[(UINT)REGISTER_ID::rVALUE].reg->SetValue(_channel->GetValue());
	registers_t[(UINT)REGISTER_ID::rSTATE].reg->SetValue((UINT)_channel->GetState().dword);
	// —брос пользовательского отказа
	ResetCheckAlarm();

}

