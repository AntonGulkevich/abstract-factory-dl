/*
 * IDispatchFlags.h
 *
 *  Created on: 25 февр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef IDISPATCHFLAGS_H_
#define IDISPATCHFLAGS_H_
#include "../types.h"

class FlagManager;

class IDispatchFlags
{
private:
static FlagManager* _pFlagManager;


public:

	static void SetManager( FlagManager* flagManager ) { _pFlagManager = flagManager; }

	IDispatchFlags(){}
	virtual ~IDispatchFlags() {}

	void RegFlag(Flags id, PRIORITY p);
	virtual DISPATH_RESULT ProcessFlag(Flags id) = 0;

};

#endif /* IDISPATCHFLAGS_H_ */
