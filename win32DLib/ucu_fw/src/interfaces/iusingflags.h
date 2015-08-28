/*
 * iusingflags.h
 *
 *  Created on: 25 февр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef IUSINGFLAGS_H_
#define IUSINGFLAGS_H_
#include "../types.h"

class FlagManager;

class IUsingFlags
{

private:
static FlagManager* _pFlagManager;


public:

	static void SetManager( FlagManager* flagManager ) { _pFlagManager = flagManager; }

	IUsingFlags(){}
	virtual ~IUsingFlags() {}

	void SetFlag(Flags id);
	bool GetFlag(Flags id);

};

#endif /* IUSINGFLAGS_H_ */
