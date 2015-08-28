/*
 * chnotconnectedio.cpp
 *
 *  Created on: 05 θώνÿ 2015 γ.
 *      Author: Youbrin_A
 */

#include "chnotconnectedio.h"

ChNotConnectedIO::ChNotConnectedIO(CPattern* pattern, IOTYPES type, UINT num) : IChannel(pattern)
{
	_ioType = type;
	_number = num;

}


