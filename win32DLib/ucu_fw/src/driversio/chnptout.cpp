/*
 * chnptout.cpp
 *
 *  Created on: 01 июн€ 2015 г.
 *      Author: Youbrin_A
 */

#include "chnptout.h"
#include "../driversiohw/nptout.h"

ChNPTOut::ChNPTOut(CPattern* const pattern, UINT number) : IChannelOut(pattern)
{
	_channel = NULL;
	_ioType = IOTYPES::ioNPT;
	_number = number;
	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
		registers_t[i].id = REGISTER_ID::NULLID;


	registers_t[(UINT)REGISTER_ID::rNAME] = {REGISTER_ID::rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rVALUE] = {REGISTER_ID::rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rCONTROL] = {REGISTER_ID::rCONTROL, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rSTATE] = {REGISTER_ID::rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rGROUP] = {REGISTER_ID::rGROUP, rwConstant, rtDec, 0.0f, 99.0f, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rSUBGROUP] = {REGISTER_ID::rSUBGROUP, rwConstant, rtDec, 1.0f, 9.0f, 1.0f, false};
	registers_t[(UINT)REGISTER_ID::rUNIT] = {REGISTER_ID::rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rDELTA] = {REGISTER_ID::rDELTA, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	CreateRegisters();
}

void ChNPTOut::InitRegisters()
{
	UpdateDataToHW();
}

void ChNPTOut::UpdateDataToHW()
{
	_channel->SetValue(registers_t[(UINT)REGISTER_ID::rVALUE].reg->GetValueFloat() + registers_t[(UINT)REGISTER_ID::rDELTA].reg->GetValueFloat());
}


void ChNPTOut::UpdateHWToData()
{
	registers_t[(UINT)REGISTER_ID::rSTATE].reg->SetValue((UINT)_channel->GetState().dword);
	// —брос пользовательского отказа
	ResetCheckAlarm();

}
