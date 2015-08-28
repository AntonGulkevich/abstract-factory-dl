/*
 * calibrate.cpp
 *
 *  Created on: 16 июля 2015 г.
 *      Author: Youbrin_A
 */

#include "calibrate.h"

Calibrate::Calibrate(DWORD baseAddress) : _baseAddress(baseAddress)
{
	UCU_IOWR_32DIRECT(_baseAddress, TYPE_OFFSET + _inputOffset, 8);

}

