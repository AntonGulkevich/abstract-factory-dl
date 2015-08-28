/*
 * iusingflags.cpp
 *
 *  Created on: 25 февр. 2015 г.
 *      Author: Youbrin_A
 */

#include "idispatchflags.h"
#include "../flagmanager.h"

FlagManager* IDispatchFlags::_pFlagManager;

void IDispatchFlags::RegFlag(Flags id, PRIORITY p)
{
	_pFlagManager->RegFlag(id, p, this);
}
