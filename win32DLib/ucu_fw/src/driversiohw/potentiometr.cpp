/*
 * potentiometr.cpp
 *
 *  Created on: 10 апр. 2015 г.
 *      Author: Youbrin_A
 */

#include "potentiometr.h"

DWORD Potentiometr::_staticBaseAddress = 0;

Potentiometr::Potentiometr(DWORD baseAddress, DWORD inputOffset) :
_baseAddress(baseAddress), _inputOffset(inputOffset)
{
	_state.dword = 0;
	SetMinimumSignalLevel(0);
	SetMaximumSignalLevel(100);

}

Potentiometr::~Potentiometr()
{
	// TODO Auto-generated destructor stub
}


void Potentiometr::UpdateValue()
{
	_value = UCU_IORD_32DIRECTF(_baseAddress, _inputOffset + valueOffset);
	_state.iostAnalogHighLevel = UCU_IORD_32DIRECT(_baseAddress, _inputOffset + failHighLevelOffset);
	_state.iostAnalogLowLevel = UCU_IORD_32DIRECT(_baseAddress, _inputOffset + failLowLevelOffset);
	_state.iostPotentADCFail = ((UCU_IORD_32DIRECT(_baseAddress, 0x204) >> (_inputOffset/4)) & 1) == 0 ? 1 : 0;
}
