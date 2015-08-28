/*
 * chlogicalin.cpp
 *
 *  Created on: 01 июня 2015 г.
 *      Author: Youbrin_A
 */

#include "chlogicalin.h"
#include "chlogicalout.h"

ChLogicalIn::ChLogicalIn(CPattern* const pattern, UINT number) : IChannelIn(pattern)
{
	_channel = NULL;
	_ioType = IOTYPES::ioLogicalIn;
	_number = number;

	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
		registers_t[i].id = REGISTER_ID::NULLID;
	_filter = new ChannelFilter(this);
	_rate = new ChannelRate(this);

	registers_t[(UINT)REGISTER_ID::rNAME] = {REGISTER_ID::rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rVALUE] = {REGISTER_ID::rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rVALUE1] = {REGISTER_ID::rVALUE1, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rVALUE2] = {REGISTER_ID::rVALUE2, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rVALUE3] = {REGISTER_ID::rVALUE3, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rSTATE] = {REGISTER_ID::rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rUNIT] = {REGISTER_ID::rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rDATAARRAY] = {REGISTER_ID::rDATAARRAY, rwConstant, rtArray, 1, 200, 1, false},
	_filter->CreateFilterRegisters();
	_rate->CreateRateRegisters();
	CreateRegisters();

}

void ChLogicalIn::InitRegisters()
{
	// Для совместимости со старой версией
	// Если используется фильтр на выходе, то отключить фильтр на входе, чтобы не было двойной фильтрации
	if (_channel->GetRegister(REGISTER_ID::rFILTRTYPE)->IsFilled())
		GetRegister(REGISTER_ID::rFILTRTYPE)->SetValue(0);
	if (_channel->GetRegister(REGISTER_ID::rRATEBASE)->IsFilled())
		GetRegister(REGISTER_ID::rRATEBASE)->SetFilled(false);
	_filter->CreateFilterBuffer();
	_rate->CreateRateBuffer();
	UpdateDataToHW();
}

void ChLogicalIn::UpdateDataToHW()
{
}


void ChLogicalIn::UpdateHWToData()
{
	GetRegister(REGISTER_ID::rVALUE)->SetValue(_channel->GetRegister(REGISTER_ID::rVALUE)->GetValueFloat());
	GetRegister(REGISTER_ID::rSTATE)->SetValue(_channel->GetRegister(REGISTER_ID::rSTATE)->GetValueFloat());
	GetRegister(REGISTER_ID::rRATE)->SetValue(_channel->GetRegister(REGISTER_ID::rRATE)->GetValueFloat());
	//Расчет фильтра
	_filter->UpdateValue();
	// Расчет скорости
	_rate->UpdateValue();
	// Сброс пользовательского отказа
	ResetCheckAlarm();

}

