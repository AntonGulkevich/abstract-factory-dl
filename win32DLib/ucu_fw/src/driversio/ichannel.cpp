/*
 * ichannelin.cpp
 *
 *  Created on: 01 июня 2015 г.
 *      Author: Youbrin_A
 */

#include "ichannel.h"
#include "../application/cpattern.h"

IChannel::IChannel(CPattern* const pattern) : _pattern(pattern)
{
	_isConfigured = false;
	_calibration = NULL;
	ResetCheckAlarm();

}

IChannel::~IChannel() {
	// TODO Auto-generated destructor stub
}


void IChannel::CreateRegisters()
{
	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
	{
		if (registers_t[i].id != REGISTER_ID::NULLID)
			registers_t[i].reg = new CRegister();
		else
			registers_t[i].reg = NULL;
	}
}


IChannel::CheckConfigurationResult IChannel::CheckConfiguration()
{
	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
	{
		if (registers_t[i].id != REGISTER_ID::NULLID)
		{
			if (!registers_t[i].reg->IsFilled())
			{
				// Заполнение UNIT по номеру группы ??
				if (registers_t[i].required)
					return CheckConfigurationResult::Error;
				else // Инициализация значением по умолчанию
				{
					registers_t[i].reg->SetValue(registers_t[i].default_value);
					registers_t[i].reg->SetFilled(false);
				}
			}
		}
	}
	_isConfigured = true;
	return CheckConfigurationResult::OK;
}

UINT IChannel::GetPatternIndex()
{
	return _pattern->GetSelfNum();
}

UINT IChannel::GetPatternDeviceNum()
{
	return _pattern->GetDeviceNum();
}

void IChannel::SetCheckAlarmCode(UINT code)
{
	_userAlarmCode = code;
	auto reg = GetRegister(REGISTER_ID::rSTATE);
	if (reg != NULL)
	{
		IOSTATE st;
		st.dword = reg->GetValueUInt();
		st.iostUserFail = 1;
		reg->SetValue((UINT)st.dword);
	}
}

IChannel::ChannelData IChannel::GetChannelIdentificator()
{
	ChannelData cd;
	cd.channelPattern = _pattern == NULL ? 0xFF : _pattern->GetDeviceNum();
	cd.channelType = _ioType;
	cd.channelNum = _number;
	cd.channelHWNum = 0xFF;
	return cd;
}
