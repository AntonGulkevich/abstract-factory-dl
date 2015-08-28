/*
 * arincout.cpp
 *
 *  Created on: 04 мая 2015 г.
 *      Author: Youbrin_A
 */

#include "arincout.h"

#include "../xparameters.h"

UINT ArincOut::_tactNum = 0;

ArincOut::ArincOut(DWORD baseAddress, DWORD baseAddressControl, UINT selfNum) : _baseAddress(baseAddress), _baseAddressControl(baseAddressControl), _selfNumber(selfNum)
{
	// TODO Auto-generated constructor stub
	_controlRegister.dword = 0;
	_wordCount = 0;
	_wordWaitmks = 0;
	_channelTimeout = 0;
	_mode = Mode::Normal;
}

void ArincOut::SendWord(Arinc::WORD429 data)
{
	if ((GetFifoStatus() & (UINT)Arinc::FifoStatus::FifoFull) == 0)
	{
		if (_mode == Mode::Normal)
		{
			UCU_IOWR_32DIRECT(_baseAddress, DATA_OFFSET, data.time);
			UCU_IOWR_32DIRECT(_baseAddress, DATA_OFFSET, data.value);
		}
	}
	else
	{
		// Обработка отказа переполнения
	}
	// Диагностика
	if (_wordCount > diagBufferLength-1)
	{
		_wordCount = 0;
		_wordWaitmks = 0;
	}
	if (_wordWaitmks > 0)
	{
		data.time = _wordWaitmks / 10;  // Для совместимости с ППК
		_wordWaitmks = 0;//-wordPauses[_controlRegister.rate];
	}
	else
	{
		data.time = wordPauses[(UINT)_controlRegister.rate];
		_wordWaitmks -= wordPauses[(UINT)_controlRegister.rate];
	}
	_wordBuffer[_wordCount] = data;
	_wordCount++;
}


void ArincOut::UpdateValue()
{
	_wordWaitmks += 20000;
	_state.iostArincChNoSignal = _channelTimeout != 0 && IsSignalTimeOut() ? 1 : 0;
}


