/*
 * keys.cpp
 *
 *  Created on: 25 ����. 2015 �.
 *      Author: Youbrin_A
 */

#include "keys.h"

Keys::~Keys()
{
	// TODO Auto-generated destructor stub
}

Keys::KEYS Keys::UpdateState()
{
	_prevKeyState.Value = _keyState.Value;
	_keyState.Value = UCU_IORD_32DIRECT(_baseAddress, 0x8);
	return _keyState;
}

