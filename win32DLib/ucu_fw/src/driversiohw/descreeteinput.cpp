/*
 * DescreeteInput.cpp
 *
 *  Created on: 20 ���. 2015 �.
 *      Author: Youbrin_A
 */

#include "descreeteinput.h"

void DescreeteInput::UpdateValue()
{
	_value = UCU_IORD_32DIRECT(_baseAddress, VALUE_OFFSET + _inputOffset);
	_state.dword = 0;
}



