/*
 * rsin.cpp
 *
 *  Created on: 19 мая 2015 г.
 *      Author: Youbrin_A
 */

#include "rsin.h"

RSIn::RSIn(DWORD baseAddress) : _baseAddress(baseAddress)
{
	_receivedCount = 0;
	_markerMode.dword = 0;
	SetWorkMode(WorkMode::Normal);
	UCU_IOWR_32DIRECT(_baseAddress, IRQENABLE_OFFSET, 0);
	ClearBuffer();
}

void RSIn::UpdateValue()
{
	Status st;
	st.dword = GetStatus().dword;
	switch (_workMode) {
		case WorkMode::Normal:
			_receivedCount = GetCount();
			break;
		case WorkMode::Time:
			if (st.time == 1)
				_receivedCount = GetCountPT();
			else
				_receivedCount = 0;
			break;
		case WorkMode::Packet:
			if (st.packet == 1)
				_receivedCount = GetCountPT();
			else
				_receivedCount = 0;
			break;
		default:
			break;
	}
	for(UINT i = 0; i < _receivedCount; i++)
		_inputBuffer[i] = UCU_IORD_32DIRECT(_baseAddress, DATA_OFFSET);
	ResetStatus();

}


