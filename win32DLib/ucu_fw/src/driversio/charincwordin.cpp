/*
 * charincin.cpp
 *
 *  Created on: 03 июн€ 2015 г.
 *      Author: Youbrin_A
 */

#include "charincwordin.h"
#include "../driversiohw/arincwordin.h"

ChArincWordIn::ChArincWordIn(CPattern* const pattern, ChArincIn* patternChannel, UINT number) : IChannelIn(pattern), _patternChannel(patternChannel)
{
	_channel = NULL;
	_ioType = IOTYPES::ioArincWordIn;
	_number = number;
	_dataMask = 0xFFFFFF00;

	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
		registers_t[i].id = REGISTER_ID::NULLID;

	registers_t[(UINT)REGISTER_ID::rNAME] = {REGISTER_ID::rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	registers_t[(UINT)REGISTER_ID::rVALUE] = {REGISTER_ID::rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rSTATE] = {REGISTER_ID::rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rMATRIX] = {REGISTER_ID::rMATRIX, rwVariable, rtDec, 0.0f, 4, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rPERIODMAX] = {REGISTER_ID::rPERIODMAX, rwConstant, rtFloat, 0.0f, 20000.0f, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rPERIODMIN] = {REGISTER_ID::rPERIODMIN, rwConstant, rtFloat, 0.0f, 20000.0f, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rID] = 	{REGISTER_ID::rID, rwConstant, rtDec, 0.0f, 3, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rGROUP] = 	{REGISTER_ID::rGROUP, rwConstant, rtDec, 0.0f, 99.0f, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rSUBGROUP] = 	{REGISTER_ID::rSUBGROUP, rwConstant, rtDec, 1.0f, 9.0f, 1.0f, false};
	registers_t[(UINT)REGISTER_ID::rUNIT] = 	{REGISTER_ID::rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rCONTROL] = 	{REGISTER_ID::rCONTROL, rwVariable, rtDec, 0.0f, 1.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rDELTA] = 	{REGISTER_ID::rDELTA, rwConstant, rtDec, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rBITSCOUNT] = 	{REGISTER_ID::rBITSCOUNT, rwConstant, rtDec, 1, 21, 20.0f, false};
	registers_t[(UINT)REGISTER_ID::rRANGE] = 	{REGISTER_ID::rRANGE, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, (float)(1<<20), false};

	CreateRegisters();

}

void ChArincWordIn::InitRegisters()
{
	UpdateDataToHW();
}

void ChArincWordIn::UpdateDataToHW()
{
	_dataMask = _channel->IsDataParity() ? 0xFFFFFF00 : 0x7FFFFF00;
	if (registers_t[(UINT)REGISTER_ID::rID].reg->IsFilled())
		_dataMask &= 0xFFFFFC00;
}


void ChArincWordIn::UpdateHWToData()
{
	UINT val = ftou__(_channel->GetValue());
	registers_t[(UINT)REGISTER_ID::rVALUE].reg->SetValue((UINT)(val & _dataMask));
	registers_t[(UINT)REGISTER_ID::rMATRIX].reg->SetValue((val >> 29) & 0x3);
	registers_t[(UINT)REGISTER_ID::rSTATE].reg->SetValue((UINT)_channel->GetState().dword);

	// —брос пользовательского отказа
	ResetCheckAlarm();

}

bool ChArincWordIn::IsTimeouted()
{
	return _channel->GetState().iostArincWNoWord == 1;
}
