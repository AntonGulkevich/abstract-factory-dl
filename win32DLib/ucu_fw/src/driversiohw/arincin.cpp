/*
 * arincin.cpp
 *
 *  Created on: 21 апр. 2015 г.
 *      Author: Youbrin_A
 */

#include "arincin.h"
#include <string.h>

ArincIn::ArincIn(DWORD baseAddress) : _baseAddress(baseAddress)
{
	memset(_values, 0, sizeof(_values));
	memset(_valuesId, 0, sizeof(_valuesId));
	memset(_wordBuffer, 0, sizeof(_wordBuffer));
	_wordCount = 0;
	_controlRegister.dword = 0;
	_timeOut = 0;
	_state.dword = 0;
}


void ArincIn::UpdateValue()
{
	if (GetFifoStatus() & (int)Arinc::FifoStatus::FifoEmpty)
	{
		_wordCount = 0;
		_timeOut += 20;
	}
	else
	{
		_timeOut = 0;
		_wordCount = GetFifoCount();
		for(UINT i = 0; i < _wordCount && i < 256; i++)
		{
			_wordBuffer[i].time = GetTopWordTime();
			_wordBuffer[i].value = GetTopWordValue();
			_values[_wordBuffer[i].value & 0xFF] =_wordBuffer[i].value;
			_valuesId[_wordBuffer[i].value & 0x3FF] =_wordBuffer[i].value;
			_timeOuts[_wordBuffer[i].value & 0xFF] = 0;
			_timeOutsId[_wordBuffer[i].value & 0x3FF] = 0;
		}
	}
	_state.iostArincChNoSignal = _timeOut > 2000 ? 1 : 0;
	for(int i = 0; i < 1024; i++)
		_timeOutsId[i] += 20;
	for(int i = 0; i < 256; i++)
		_timeOuts[i] += 20;

}
