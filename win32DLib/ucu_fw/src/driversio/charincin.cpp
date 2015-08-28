/*
 * charincin.cpp
 *
 *  Created on: 03 июн€ 2015 г.
 *      Author: Youbrin_A
 */

#include "charincin.h"
#include "../driversiohw/arincin.h"
#include "charincwordin.h"

ChArincIn::ChArincIn(CPattern* const pattern, UINT number) : IChannelIn(pattern)
{
	_channel = NULL;
	_ioType = IOTYPES::ioDigitalIn;
	_number = number;
	_isDataExist = false;

	for(UINT i = 0; i < 256; i++)
		_word[i] = NULL;
	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
		registers_t[i].id = REGISTER_ID::NULLID;

	registers_t[(UINT)REGISTER_ID::rNAME] = {REGISTER_ID::rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	registers_t[(UINT)REGISTER_ID::rVALUE] = {REGISTER_ID::rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rSTATE] = {REGISTER_ID::rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rSPEED] = {REGISTER_ID::rSPEED, rwConstant, rtDec, 0.0f, 3, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rPARITYTYPE] = {REGISTER_ID::rPARITYTYPE, rwConstant, rtDec, 0.0f, 2.0f, 0.0f, false};
	CreateRegisters();

}

void ChArincIn::InitRegisters()
{
	UINT rate = registers_t[(UINT)REGISTER_ID::rSPEED].reg->GetValueInt();
	switch(rate)
	{
	case 0:
	case 1:
		_channel->SetRate(Arinc::Rate::R12_5);
		break;
	case 2:
		_channel->SetRate(Arinc::Rate::R50);
		break;
	default:
		_channel->SetRate(Arinc::Rate::R100);
		break;
	}
	UINT parity = registers_t[(UINT)REGISTER_ID::rPARITYTYPE].reg->GetValueInt();
	switch(parity)
	{
	case 2: //PT_USER
		_channel->SetParity(Arinc::ParityIn::NoControl);
		break;
	case 1: //PT_EVEN
		_channel->SetParity(Arinc::ParityIn::EvenControl);
		break;
	case 0: //PT_ODD
		_channel->SetParity(Arinc::ParityIn::OddControl);
		break;
	}
	_isDataExist = false;
	for(int i = 0; i < 256; i++)
		if (IsLabelExist(i))
			_isDataExist = true;
	UpdateDataToHW();
}

void ChArincIn::UpdateDataToHW()
{
}


void ChArincIn::UpdateHWToData()
{
	auto state = _channel->GetState();
	if (!_isDataExist)
		state.iostAnalogNoSignal = 0;
/*	state.iostArincWNoWord = 0;
	for(int i = 0; i < 256; i++)
		if (IsLabelExist(i) && GetLabel(i)->IsTimeouted() )
		{
			state.iostArincWNoWord = 1;
			break;
		}*/

	registers_t[(UINT)REGISTER_ID::rSTATE].reg->SetValue((UINT)state.dword);
	// —брос пользовательского отказа
	ResetCheckAlarm();

}

void ChArincIn::AddLabel(BYTE label)
{
	if (_word != NULL)
		delete _word[label];
	_word[label] = new ChArincWordIn(_pattern, this, label);
}
