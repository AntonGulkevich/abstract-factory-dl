/*
 * manager.h
 *
 *  Created on: 26 янв. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef MANAGER_H_
#define MANAGER_H_
#include "types.h"
#include <vector>

class IDispatchFlags;
class PerfomanceCounter;

class FlagManager
{
private:
	class Dispatcher
	{
	public:
		DISPATH_RESULT flagState;
		IDispatchFlags* Object;
		Dispatcher(IDispatchFlags* obj):
			flagState(drNotDispatched), Object(obj) {}

		DISPATH_RESULT Process(Flags id);
		void ResetState() { flagState = drNotDispatched; }
	};

	bool flags[(int)PRIORITY::Count][(int)Flags::Count];
	void ResetFlag(const Flags id, PRIORITY priority) { flags[(int)priority][(int)id] = false; for (UINT i = 0; i < dispatchers[(int)priority][(int)id].size(); i++) dispatchers[(int)priority][(int)id][i]->ResetState(); }

	std::vector<Dispatcher*> dispatchers[(int)PRIORITY::Count][(int)Flags::Count];
	PerfomanceCounter* _pc;

public:
	FlagManager(PerfomanceCounter* pc);
	virtual ~FlagManager();

	void SetFlag(Flags id) { for(int i = 0; i < (int)PRIORITY::Count; i++) flags[i][(int)id] = true; }
	// Для отладки, в работе не используется
	bool GetFlag(Flags id) { return flags[0][(int)id] || flags[1][(int)id] || flags[2][(int)id]; }
	void RegFlag(Flags id, PRIORITY priority, IDispatchFlags* obj);

	void Process();
};

#endif /* MANAGER_H_ */
