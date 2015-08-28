/*
 * chanalogin.cpp
 *
 *  Created on: 01 июня 2015 г.
 *      Author: Youbrin_A
 */

#include "chanalogin.h"
#include "../driversiohw/analogin.h"

ChAnalogIn::ChAnalogIn(CPattern* const pattern, UINT number) : IChannelIn(pattern)
{
	_channel = NULL;
	_ioType = IOTYPES::ioAnalog;
	_number = number;
	_calibration = new ChannelCalibration(this);
	_filter = new ChannelFilter(this);
	_rate = new ChannelRate(this);

	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
		registers_t[i].id = REGISTER_ID::NULLID;

	registers_t[(UINT)REGISTER_ID::rNAME] = {REGISTER_ID::rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rVALUE] = {REGISTER_ID::rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rSTATE] = {REGISTER_ID::rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rTYPE] = {REGISTER_ID::rTYPE, rwConstant, rtDec, 0.0f, 5.0f, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rDELAY] = {REGISTER_ID::rDELAY, rwUser, rtDec, 0.0f, 2499.9998f, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rSYNCNUM] = {REGISTER_ID::rSYNCNUM, rwConstant, rtDec, 0.0f, 4, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rDELTA] = {REGISTER_ID::rDELTA, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rMAXLEVEL] = {REGISTER_ID::rMAXLEVEL, rwConstant, rtDec, -100.0f, 100, 100.0f, false};
	registers_t[(UINT)REGISTER_ID::rMINLEVEL] = {REGISTER_ID::rMINLEVEL, rwConstant, rtDec, -100.0f, 100, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rGROUP] = {REGISTER_ID::rGROUP, rwConstant, rtDec, 0.0f, 99.0f, 0.0f, true};
	registers_t[(UINT)REGISTER_ID::rSUBGROUP] = {REGISTER_ID::rSUBGROUP, rwConstant, rtDec, 1.0f, 9.0f, 1.0f, false};
	registers_t[(UINT)REGISTER_ID::rUNIT] = {REGISTER_ID::rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, false};
	registers_t[(UINT)REGISTER_ID::rDEADZONE] = {REGISTER_ID::rDEADZONE, rwConstant, rtFloat, -360, 0, 0, false};
	registers_t[(UINT)REGISTER_ID::rCORRECTIONDELTA] = {REGISTER_ID::rCORRECTIONDELTA, rwUser, rtDec, 0, 360, 0, false};
	registers_t[(UINT)REGISTER_ID::rCORRECTIONSIGN] = {REGISTER_ID::rCORRECTIONSIGN, rwUser, rtDec, -1, 1, 1, false};
	_calibration->CreateClibrationRegisters();
	_filter->CreateFilterRegisters();
	_rate->CreateRateRegisters();
	CreateRegisters();

}

ChAnalogIn::~ChAnalogIn()
{
	delete _calibration;
}

void ChAnalogIn::InitRegisters()
{
	_channel->SetSyncNumber(registers_t[(UINT)REGISTER_ID::rSYNCNUM].reg->GetValueUInt());
	_channel->SetDelay(registers_t[(UINT)REGISTER_ID::rDELAY].reg->GetValueUInt());
	// Чтобы не проверять потом
	if (registers_t[(UINT)REGISTER_ID::rCORRECTIONSIGN].reg->GetValueInt() != -1 &&
		registers_t[(UINT)REGISTER_ID::rCORRECTIONSIGN].reg->GetValueInt() != 1)
	{
		bool isFilled = registers_t[(UINT)REGISTER_ID::rCORRECTIONSIGN].reg->IsFilled();
		registers_t[(UINT)REGISTER_ID::rCORRECTIONSIGN].reg->SetValue(1);
		registers_t[(UINT)REGISTER_ID::rCORRECTIONSIGN].reg->SetFilled(isFilled);
	}
	// Задать minlevel для угловых, чтобы проверять низкий уровень сигнала
	int minLevel = registers_t[(UINT)REGISTER_ID::rMINLEVEL].reg->GetValueInt();
	int maxLevel = registers_t[(UINT)REGISTER_ID::rMAXLEVEL].reg->GetValueInt();
	switch(registers_t[(UINT)REGISTER_ID::rTYPE].reg->GetValueUInt())
	{
	case 0: // СКТ 8В
		_channel->SetAngleType(AnalogIn::ANGLE_TYPE::SCT8);
		break;
	case 1: // СКТ 20В
		_channel->SetAngleType(AnalogIn::ANGLE_TYPE::SCT20);
		break;
	case 2: // СКТ 36В
		_channel->SetAngleType(AnalogIn::ANGLE_TYPE::SCT36);
		break;
	case 3: // Сельсин 36В-60В
		_channel->SetAngleType(AnalogIn::ANGLE_TYPE::Selsin);
		break;
	case 4: // Логометр 27В
		_channel->SetAngleType(AnalogIn::ANGLE_TYPE::Logometr);
		break;
	case 5: // НПТ
	{
		float level;
		if (m_abs(minLevel) > m_abs(maxLevel))
			level = m_abs(minLevel);
		else
			level = m_abs(maxLevel);
		if (level > 50)
			_channel->SetGainCoefficient(AnalogIn::GAIN_COEFFICIENTS::K_x01);
		if (level > 20)
			_channel->SetGainCoefficient(AnalogIn::GAIN_COEFFICIENTS::K_x02);
		if (level > 10)
			_channel->SetGainCoefficient(AnalogIn::GAIN_COEFFICIENTS::K_x05);
		_channel->SetGainCoefficient(AnalogIn::GAIN_COEFFICIENTS::K_x1);
		_channel->SetType(AnalogIn::TYPE::NPT);
	}
		break;
	}
	if (registers_t[(UINT)REGISTER_ID::rMINLEVEL].reg->IsFilled() && _channel->GetAngleType() != AnalogIn::ANGLE_TYPE::NoAngleType)
		_channel->SetMinlevel(minLevel);
	_calibration->CreateKoeffTable();
	_filter->CreateFilterBuffer();
	_rate->CreateRateBuffer();
	UpdateDataToHW();
}

void ChAnalogIn::UpdateDataToHW()
{
}


void ChAnalogIn::UpdateHWToData()
{
	float value = _channel->GetValue();

	// Насмерть убитая зона
	if(_channel->GetAngleType() != AnalogIn::ANGLE_TYPE::NoAngleType)
	{
		value += registers_t[(UINT)REGISTER_ID::rCORRECTIONDELTA].reg->GetValueFloat();
	  	value *= registers_t[(UINT)REGISTER_ID::rCORRECTIONSIGN].reg->GetValueFloat();
		value -= ((int)(value / 360) * 360) - 360;
		if (value >= (360.0f + registers_t[(UINT)REGISTER_ID::rDEADZONE].reg->GetValueFloat()))
			value -= 360.0f;
	}
	registers_t[(UINT)REGISTER_ID::rVALUE].reg->SetValue(value);
	IOSTATE st;
	st.dword = _channel->GetState().dword;

	//Расчет фильтра
	_filter->UpdateValue();
	// Расчет скорости
	_rate->UpdateValue();
	// Установка статуса фильтра, имхо бред, надо пользоваться регистром FiltrReady
	st.iostFiltrNotReady = _filter->IsFilterReady() ? 0 : 1;
	registers_t[(UINT)REGISTER_ID::rSTATE].reg->SetValue((UINT)st.dword);
	// Сброс пользовательского отказа
	ResetCheckAlarm();
}


