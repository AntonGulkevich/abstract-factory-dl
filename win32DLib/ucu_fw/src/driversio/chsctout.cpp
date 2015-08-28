/*
 * csctout.cpp
 *
 *  Created on: 01 июня 2015 г.
 *      Author: Youbrin_A
 */

#include "chsctout.h"
#include "../driversiohw/sctout.h"

ChSCTOut::ChSCTOut(CPattern* const pattern, UINT number) : IChannelOut(pattern)
{
	_channel = NULL;
	_ioType = IOTYPES::ioSCT;
	_number = number;
	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
		registers_t[i].id = REGISTER_ID::NULLID;


	registers_t[(UINT)REGISTER_ID::rNAME] = {REGISTER_ID::rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rVALUE] = {REGISTER_ID::rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rCONTROL] = {REGISTER_ID::rCONTROL, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rSTATE] = {REGISTER_ID::rSTATE, rwConstant, rtDec, 0.0f, 0xFFFFF, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rGROUP] = {REGISTER_ID::rGROUP, rwConstant, rtDec, 0.0f, 99.0f, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rSUBGROUP] = {REGISTER_ID::rSUBGROUP, rwConstant, rtDec, 1.0f, 9.0f, 1.0f, false};
	registers_t[(UINT)REGISTER_ID::rUNIT] = {REGISTER_ID::rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rDELTA] = {REGISTER_ID::rDELTA, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rDELAY] = {REGISTER_ID::rDELAY, rwUser, rtDec, 0.0f, 360.0f, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rTYPE] = {REGISTER_ID::rTYPE, rwConstant, rtDec, 0.0f, 1.0f, 0.0f, true};
//Коэффициент ослабления амплитуды???
	//registers_t[(UINT)REGISTER_ID::rMAXLEVEL] = {REGISTER_ID::rMAXLEVEL, rwConstant, rtFloat, 0.0f, 1.0f, 0.9782f, false};

	CreateRegisters();
}

void ChSCTOut::InitRegisters()
{
	_channel->SetType((SCTOut::SCTType)registers_t[(UINT)REGISTER_ID::rTYPE].reg->GetValueUInt());
	_channel->SetPhaseShift(registers_t[(UINT)REGISTER_ID::rDELAY].reg->GetValueUInt());
	UpdateDataToHW();
}

void ChSCTOut::UpdateDataToHW()
{
	float value = registers_t[(UINT)REGISTER_ID::rVALUE].reg->GetValueFloat() + registers_t[(UINT)REGISTER_ID::rDELTA].reg->GetValueFloat();
	_channel->SetValue(value);
}


void ChSCTOut::UpdateHWToData()
{
	registers_t[(UINT)REGISTER_ID::rSTATE].reg->SetValue((UINT)_channel->GetState().dword);
	// Сброс пользовательского отказа
	ResetCheckAlarm();

}
