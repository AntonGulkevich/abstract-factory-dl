/*
 * adccontrol.cpp
 *
 *  Created on: 03 апр. 2015 г.
 *      Author: Youbrin_A
 */

#include "adccontrol.h"

ADCControl::ADCControl(DWORD baseAddress) : _baseAddress(baseAddress)
{
	_state.dword = 0;
	SetInverseOff(false);
}

ADCControl::~ADCControl() {
	// TODO Auto-generated destructor stub
}


void ADCControl::SetCalibration(Calibrations& calibrations)
{
	for(int i = 0; i < 12; i++)
		UCU_IOWR_32DIRECT(_baseAddress, 0x0 + i*4,  calibrations.params[i]);
}

void ADCControl::UpdateState()
{
	_state.iostAnalogADCFail = (UCU_IORD_32DIRECT(_baseAddress, 0x34) == 0) ? 1 : 0;
	_state.iostSync1Fail = (GetSynchronisationCorrect() & 1) == 0 ? 1 : 0;
	_state.iostSync2Fail = (GetSynchronisationCorrect() & 2) == 0 ? 1 : 0;
	_state.iostSync3Fail = (GetSynchronisationCorrect() & 4) == 0 ? 1 : 0;
	_state.iostSync4Fail = (GetSynchronisationCorrect() & 8) == 0 ? 1 : 0;
	_state.iostSync5Fail = (GetSynchronisationCorrect() & 16) == 0 ? 1 : 0;
	_state.iostSync6Fail = (GetSynchronisationCorrect() & 32) == 0 ? 1 : 0;
	_state.iostSync7Fail = (GetSynchronisationCorrect() & 64) == 0 ? 1 : 0;
}

void ADCControl::EnableOscillograph(UINT channelNumber, AnalogIn::GAIN_COEFFICIENTS gain)
{
	UCU_IOWR_32DIRECT(_baseAddress, 0x5C, (UINT)gain);
	UCU_IOWR_32DIRECT(_baseAddress, 0x60, channelNumber);
	UCU_IOWR_32DIRECT(_baseAddress, 0x54, 1);
}


ADCControl::OscillographData* ADCControl::GetOscillographData()
{
	_oscillographData.marker = 0x608FDEB4;
	for(int i = 0; i < 1000; i++)
	{
		_oscillographData.channel1[i] = UCU_IORD_32DIRECT(_baseAddress, 0x1000 + i*4);
		_oscillographData.channel2[i] = UCU_IORD_32DIRECT(_baseAddress, 0x2000 + i*4);
		_oscillographData.channel3[i] = UCU_IORD_32DIRECT(_baseAddress, 0x3000 + i*4);
		_oscillographData.sync[i] = UCU_IORD_32DIRECT(_baseAddress, 0x4000 + i*4);
	}
	return &_oscillographData;
}
