/*
 * nptcontrol.cpp
 *
 *  Created on: 07 ���� 2015 �.
 *      Author: Youbrin_A
 */

#include "nptcontrol.h"


void NptControl::UpdateValue()
{
	if (!IsRefShifted())
		AnalogIn::UpdateValue();
}
