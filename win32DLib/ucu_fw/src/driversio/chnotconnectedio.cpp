/*
 * chnotconnectedio.cpp
 *
 *  Created on: 05 ���� 2015 �.
 *      Author: Youbrin_A
 */

#include "chnotconnectedio.h"

ChNotConnectedIO::ChNotConnectedIO(CPattern* pattern, IOTYPES type, UINT num) : IChannel(pattern)
{
	_ioType = type;
	_number = num;

}


