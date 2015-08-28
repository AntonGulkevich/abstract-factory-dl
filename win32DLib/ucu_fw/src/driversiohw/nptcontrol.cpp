/*
 * nptcontrol.cpp
 *
 *  Created on: 07 июля 2015 г.
 *      Author: Youbrin_A
 */

#include "nptcontrol.h"


void NptControl::UpdateValue()
{
	if (!IsRefShifted())
		AnalogIn::UpdateValue();
}
