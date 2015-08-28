/*
 * manager.cpp
 *
 *  Created on: 26 џэт. 2015 у.
 *      Author: Youbrin_A
 */

#include "flagmanager.h"

#include <string.h>

#include "drivers/keys.h"
#include "drivers/perfomancecounter.h"
#include "interfaces/idispatchflags.h"
#include "interfaces/iusingflags.h"

DISPATH_RESULT FlagManager::Dispatcher::Process(Flags id)
{
	if (flagState == drNotDispatched)
		return flagState = Object->ProcessFlag(id);
	else
		return drAlreadyDispatched;
}

FlagManager::FlagManager(PerfomanceCounter* pc) : _pc(pc)
{
	IDispatchFlags::SetManager(this);
	IUsingFlags::SetManager(this);
	memset(flags, 0, sizeof(flags));

}

FlagManager::~FlagManager()
{

}

void FlagManager::Process()
{
	for (int p = 0; p < (int)PRIORITY::Count; p++)
		for (int f = 0; f < (int)Flags::Count; f++)
			if (flags[p][f])
			{
				_pc->ProcessStart();
				for (UINT i = 0; i < dispatchers[p][f].size(); i++)
				{
					if (dispatchers[p][f][i]->Process((Flags)f) == drDispatched)
					{
						_pc->ProcessStop();
						return;
					}
				}
				ResetFlag((Flags)f, (PRIORITY)p);
				_pc->ProcessStop();
			}
}


void FlagManager::RegFlag(Flags id, PRIORITY priority, IDispatchFlags* obj)
{
	dispatchers[(int)priority][(int)id].push_back(new Dispatcher(obj));
}

