/*
 * failmanager.cpp
 *
 *  Created on: 04 июля 2015 г.
 *      Author: Youbrin_A
 */

#include "failmanager.h"

#include <string.h>

#include "../application/cconfiguration.h"
#include "../application/cpattern.h"
#include "../drivers/display.h"
#include "../driversiomanager.h"


FailManager::FailManager(Display* display, CConfiguration* config) : _display(display), _config(config)
{
	_currentFail = 0;
	_totalFail = 0;
}


void FailManager::PrepareFails()
{
	// Очистка
	memset(_fails, 0xFF, sizeof(_fails));
	//Групповые отказы
	UINT countFail = 0;
	auto state = _config->GetState();
	STATUS s;
	s.first = sfSync;
	s.second = ssEmpty;
	s.fourth = sfrNoSignal;
	s.in = true;

	// Проверка синхронизации
	if (state.iostSync1Fail == 1) 	{  	s.third = 1; 	_fails[countFail++].status = s; 	}
	if (state.iostSync2Fail == 1) 	{  	s.third = 2; 	_fails[countFail++].status = s; 	}
	if (state.iostSync3Fail == 1) 	{  	s.third = 3; 	_fails[countFail++].status = s; 	}
	if (state.iostSync4Fail == 1) 	{  	s.third = 4; 	_fails[countFail++].status = s; 	}

	//s.in = false;  // В старой версии отказ синхронизции отображается как вход
	if (state.iostSync5Fail == 1) 	{  	s.third = 5; 	_fails[countFail++].status = s; 	}
	if (state.iostSync6Fail == 1) 	{  	s.third = 6; 	_fails[countFail++].status = s; 	}
	if (state.iostSync7Fail == 1) 	{  	s.third = 7; 	_fails[countFail++].status = s; 	}

	// отказ АЦП
    if (state.iostAnalogADCFail)
    {
        s.first = sfGroup;
        s.second = ssADC;
        s.third = 0;
        s.fourth = sfrADCFail;
        s.in = true;
        _fails[countFail++].status = s;
    } else if (state.iostSDADCRefFail)
	{
		s.first = sfGroup;
		s.second = ssSDADC;
		s.third = 0;
		s.fourth = sfrEmpty;
		s.in = true;
		_fails[countFail++].status = s;
	}

    // Отказы по ARINC каналам
	s.group.value = 0x0208;
	s.in = true;
	for(UINT i = 0; i < DriversIOManager::arincInCount; i++)
	{
		if (_config->GetArincIn(i) != NULL)
		{
			s.third = i+1;
			s.fourth = sfrEmpty;
			IOSTATE state;
			state.dword = _config->GetArincIn(i)->GetRegister(REGISTER_ID::rSTATE)->GetValueUInt();
			if (state.iostArincChNoSignal)
				s.group.fourth = sfrNoSignal;
			else if (state.iostArincBreak)
				s.group.fourth = sfrBreak;
			else if (state.iostArincWNoWord)
				s.group.fourth = sfrArincNoWord;
			if (s.fourth != sfrEmpty)
				_fails[countFail++].status = s;
		}
	}
	s.in = false;
	for(UINT i = 0; i < DriversIOManager::arincOutCount; i++)
	{
		if (_config->GetArincOut(i) != NULL)
		{
			s.third = i+1;
			s.fourth = sfrEmpty;
			IOSTATE state;
			state.dword = _config->GetArincOut(i)->GetRegister(REGISTER_ID::rSTATE)->GetValueUInt();
			if (state.iostArincChNoSignal)
				s.group.fourth = sfrNoSignal;
			else if (state.iostArincBreak)
				s.group.fourth = sfrBreak;
			if (s.fourth != sfrEmpty)
				_fails[countFail++].status = s;
		}
	}

    // Отказы по каналам


	for(UINT c = 0; c < _config->GetAllChannels().size(); c++)
	{
		auto ch = _config->GetAllChannels()[c];
		if (ch->GetType() == ioLogicalIn || ch->GetType() == ioLogicalOut || ch->GetType() == ioArincWordOut)
			continue;
		IOSTATE lstate;
		lstate.dword = ch->GetRegister(REGISTER_ID::rSTATE)->GetValueUInt();
		//Console::TraceLine("%d, %s, %d %08X", ch->GetPatternIndex(), CParser::GetTypeStr(ch->GetType()), ch->GetNumber(), lstate.dword);

		if(lstate.iostAnalogSyncFail || lstate.iostArincChNoSignal || ch->GetSuppressAlarm())
			continue;

		STATUS s;
		UINT gr = ch->GetRegister(REGISTER_ID::rGROUP)->GetValueUInt();
		s.group.value = ((gr / 10) << 8) + (gr %10);
		s.group.sub = ch->GetRegister(REGISTER_ID::rSUBGROUP)->GetValueUInt();
		s.in = IS_IN(ch->GetType());
		s.fourth = sfrEmpty;

		if(lstate.iostAnalogNoSignal == 1)
			s.group.fourth = sfrNoSignal;
		else if(lstate.iostAnalogBreak == 1)
			s.group.fourth = sfrBreak;
		else if(lstate.iostAnalogFail == 1)
			s.group.fourth = sfrBadAccuracy;
		else if(lstate.iostAnalogHighLevel == 1)
			s.group.fourth = sfrHighLevel;
		else if(lstate.iostAnalogLowLevel == 1)
			s.group.fourth = sfrLowLevel;
		else if(lstate.iostUserFail == 1)
			s.group.fourth = (STATUS_FOURTH)ch->GetCheckAlarmCode();
		else if(lstate.iostArincWNoWord == 1)
			s.group.fourth = sfrArincNoWord;
		else if(lstate.iostArincWOutLimit == 1)
			s.group.fourth = sfrArincOutOfLimit;
		else if(lstate.iostAnalogDACFail == 1)
			s.group.fourth = sfrDACFail;
		else if(lstate.iostAnalogDACOverload == 1)
			s.group.fourth = sfrDACOverLoad;


		if (s.fourth != sfrEmpty)
		{
			_fails[countFail].chData = ch->GetChannelIdentificator();
			_fails[countFail].chData.channelHWNum = _config->GetChannelHWNum(ch);
			_fails[countFail++].status = s;
		}

	}


    _totalFail = countFail;
    if (_currentFail >= _totalFail)
    	_currentFail = 0;


}

void FailManager::ShowAlarms()
{
	if (!_config->IsInitialized())
		_display->RunningB();
	else
	{
		PrepareFails();
		if (_totalFail == 0)
			_display->RunningZero();
		else
		{
			// Показать отказ
			BYTE text[4];
			text[3] = _display->GetByteByLetter(_fails[_currentFail].status.first);
			text[2] = _display->GetByteByLetter(_fails[_currentFail].status.second);
			text[1] = _display->GetByteByLetter(_fails[_currentFail].status.third);
			text[0] = _display->GetByteByLetter(_fails[_currentFail].status.fourth);
			_display->SetValue(text);
			if (_fails[_currentFail].status.in)
				_display->SetPoint(false, false, false, false);
			else
				_display->SetPoint(true, true, false, false);
			_display->Show();
			_currentFail++;
			if (_currentFail >= _totalFail)
				_currentFail = 0;
		}
	}
}
