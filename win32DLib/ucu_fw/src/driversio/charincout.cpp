/*
 * chnptout.cpp
 *
 *  Created on: 01 июн€ 2015 г.
 *      Author: Youbrin_A
 */

#include "charincout.h"

#include "../driversio/charincwordout.h"
#include "../driversiohw/arincout.h"

ChArincOut::ChArincOut(CPattern* const pattern, UINT number) : IChannelOut(pattern)
{
	_channel = NULL;
	_ioType = IOTYPES::ioDigitalOut;
	_number = number;
	_isDataExits = false;

	for(UINT i = 0; i < 256; i++)
		_word[i] = NULL;
	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
		registers_t[i].id = REGISTER_ID::NULLID;



	registers_t[(UINT)REGISTER_ID::rNAME] = {REGISTER_ID::rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rVALUE] = {REGISTER_ID::rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rCONTROL] = {REGISTER_ID::rCONTROL, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rSTATE] = {REGISTER_ID::rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rSPEED] = {REGISTER_ID::rSPEED, rwConstant, rtDec, 0.0f, 3, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rPARITYTYPE] = {REGISTER_ID::rPARITYTYPE, rwConstant, rtDec, 0.0f, 5.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rREVERSE] = {REGISTER_ID::rREVERSE, rwConstant, rtDec, 0.0f, 1.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rTYPE] = {REGISTER_ID::rTYPE, rwConstant, rtDec, 0.0f, 1.0f, 0.0f, false};


	CreateRegisters();
}

void ChArincOut::InitRegisters()
{
	UINT maxPeriod = 0;
	for(int i = 0; i < 256; i++)
	{
		if (IsLabelExist(i))
		{
			if (maxPeriod < GetLabel(i)->GetPeriod())
				maxPeriod = GetLabel(i)->GetPeriod();
		}

	}
	maxPeriod = maxPeriod < 20 ? 40 : maxPeriod*3;
	_channel->SetTimeout(maxPeriod);
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
		_channel->SetParity(Arinc::ParityOut::NoChange);
		break;
	case 1: //PT_EVEN
		_channel->SetParity(Arinc::ParityOut::EvenControl);
		break;
	case 0: //PT_ODD
		_channel->SetParity(Arinc::ParityOut::OddControl);
		break;
	}

	_channel->SetReverse(registers_t[(UINT)REGISTER_ID::rREVERSE].reg->GetValueInt() == 1);

	_channel->SetMode((ArincOut::Mode)registers_t[(UINT)REGISTER_ID::rTYPE].reg->GetValueUInt());
	//_channel->SetMode(ArincOut::Mode::Table);
	if (_channel->GetMode() == ArincOut::Mode::Table)
		for(int i = 0; i < 256; i++)
			if (IsLabelExist(i))
				GetLabel(i)->SetTableMode();
	UpdateDataToHW();
}

void ChArincOut::UpdateDataToHW()
{
	_isDataExits = false;
	for(int i = 0; i < 256; i++)
		if (IsLabelExist(i) && GetLabel(i)->IsOnline())
			_isDataExits = true;

}


void ChArincOut::UpdateHWToData()
{
	UINT state = (UINT)_channel->GetState().dword;
	registers_t[(UINT)REGISTER_ID::rSTATE].reg->SetValue(_isDataExits ? state : 0u);
	// —брос пользовательского отказа
	ResetCheckAlarm();
}

void ChArincOut::AddLabel(BYTE label)
{
	if (_word != NULL)
		delete _word[label];
	_word[label] = new ChArincWordOut(_pattern, this, label);
}
