/*
 * iusingflags.cpp
 *
 *  Created on: 25 ����. 2015 �.
 *      Author: Youbrin_A
 */

#include "iusingflags.h"
#include "../flagmanager.h"

FlagManager* IUsingFlags::_pFlagManager;

void IUsingFlags::SetFlag(Flags id)
{
	_pFlagManager->SetFlag(id);
}

bool IUsingFlags::GetFlag(Flags id)
{
	return _pFlagManager->GetFlag(id);
}
