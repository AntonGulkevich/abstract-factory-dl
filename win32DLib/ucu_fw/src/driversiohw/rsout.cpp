/*
 * rsin.cpp
 *
 *  Created on: 19 мая 2015 г.
 *      Author: Youbrin_A
 */

#include "rsout.h"

#include <string.h>

RSOut::RSOut(DWORD baseAddress) : _baseAddress(baseAddress)
{
	_sendCount = 0;
	_diagSendCount = 0;
	_workMode.dword = 0;
	_workMode.workMode = (UINT)WorkMode::Fifo;
	_workMode.isWork = 1;
	UCU_IOWR_32DIRECT(_baseAddress, IRQENABLE_OFFSET, 0);
}

void RSOut::UpdateValue()
{
	Status st;
	st.dword = GetStatus().dword;
	if (st.fifofull == 1)
	{
		// Обработка отказа
		ClearBuffer();
	}
	else if (_sendCount > 0)
	{
		if (_workMode.workMode == (UINT)WorkMode::Fifo)
		{
			for(UINT i = 0; i < _sendCount; i++)
				UCU_IOWR_32DIRECT(_baseAddress, FIFO_OFFSET, _outputBuffer[i]);
		}
		else
		{
			for(UINT i = 0; i < _sendCount; i++)
				UCU_IOWR_32DIRECT(_baseAddress, DATA_OFFSET, _outputBuffer[i]);
			UpdateBuffer();
		}
		_sendCount = 0;
	}

}

void RSOut::SetBuffer(BYTE* buff, UINT count)
{
	if (count > 1024)
		count = 1024;
	_sendCount = _diagSendCount = count;
	memcpy(_outputBuffer, buff, count);
}

