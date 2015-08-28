/*
 * analogin.cpp
 *
 *  Created on: 30 марта 2015 г.
 *      Author: Youbrin_A
 */

#include "analogin.h"

AnalogIn::AnalogIn(DWORD baseAddress, DWORD inputOffset) :
	_baseAddress(baseAddress), _inputOffset(inputOffset)
{
	_state.dword = 0;

}

AnalogIn::~AnalogIn()
{
	// TODO Auto-generated destructor stub
}

void AnalogIn::UpdateValue()
{
	reg1 = UCU_IORD_32DIRECT(_baseAddress, REG1_OFFSET + _inputOffset);
	reg2 = UCU_IORD_32DIRECT(_baseAddress, REG2_OFFSET + _inputOffset);
	reg3 = UCU_IORD_32DIRECT(_baseAddress, REG3_OFFSET + _inputOffset);

	_value = UCU_IORD_32DIRECTF(_baseAddress, VALUE_OFFSET + _inputOffset);
	if (GetType() == TYPE::NPT)
	{
		_state.iostAnalogNoSignal = 0;
		_state.iostAnalogLowLevel = 0;
		_state.iostAnalogHighLevel = (UCU_IORD_32DIRECT(_baseAddress, FAIL_OVERSCALE_OFFSET + _inputOffset) & 0x3) != 0 ? 1 : 0;
	}
	else
	{
		_state.iostAnalogLowLevel = UCU_IORD_32DIRECT(_baseAddress, FAIL_LOW_LEVEL_OFFSET + _inputOffset);
		_state.iostAnalogNoSignal = UCU_IORD_32DIRECT(_baseAddress, FAIL_NO_SIGNAL_OFFSET + _inputOffset);
		_state.iostAnalogHighLevel = (UCU_IORD_32DIRECT(_baseAddress, FAIL_OVERSCALE_OFFSET + _inputOffset) & 0x7) != 0 ? 1 : 0;
	}

	_state.iostAnalogSyncFail = ((UCU_IORD_32DIRECT(_baseAddress, 0x1040) >> (_inputOffset/4)) & 1) == 0 ? 1 : 0;
	_state.iostAnalogFail = GetMeasureCount();
			// ((UCU_IORD_32DIRECT(_baseAddress, 0x104C) >> (_inputOffset/4)) & 1) == 0 ? 1 : 0;
}


void AnalogIn::SetGainCoefficient(GAIN_COEFFICIENTS coeff)
{
	UCU_IOWR_32DIRECT(_baseAddress, GAIN_OFFSET + _inputOffset, (UINT)coeff);
	UINT noSignalLevel = roundf(125 * 100.0f / maxScaleV[(UINT)coeff]);
	UCU_IOWR_32DIRECT(_baseAddress, NO_SIGNAL_OFFSET + _inputOffset, noSignalLevel);

}

void AnalogIn::SetAngleType(ANGLE_TYPE type)
{
	_angleType = type;
	SetGainCoefficient(angleConsts[(UINT)type].GainCoeff);
	SetType(angleConsts[(UINT)type].Type);
	SetMinlevel(angleConsts[(UINT)type].MinLevelV);
}
