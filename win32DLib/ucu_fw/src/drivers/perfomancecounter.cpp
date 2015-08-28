/*
 * perfomancecounter.cpp
 *
 *  Created on: 06 июля 2015 г.
 *      Author: Youbrin_A
 */

#include "perfomancecounter.h"

DWORD PerfomanceCounter::_baseAddress = 0;

PerfomanceCounter::PerfomanceCounter(DWORD baseAddress)
{
	_baseAddress = baseAddress;
	Start();
	Reset();
}

PerfomanceCounter::~PerfomanceCounter() {
	// TODO Auto-generated destructor stub
}

