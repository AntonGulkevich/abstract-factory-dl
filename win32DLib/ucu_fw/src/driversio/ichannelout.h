/*
 * ichannelout.h
 *
 *  Created on: 01 θώνÿ 2015 γ.
 *      Author: Youbrin_A
 */

#ifndef ICHANNELOUT_H_
#define ICHANNELOUT_H_

#include "ichannel.h"
#include "../interfaces/ioutputvalue.h"

class CProcessor;

class IChannelOut : public IChannel
{

private:
	CProcessor* _proc;
public:
	IChannelOut(CPattern* const pattern);
	virtual ~IChannelOut() {}

	virtual void AssignChannel(IOutputValue* ch) = 0;
	CProcessor* GetProcessor() { return _proc; }

	IChannel* GetPatternChannel(IOTYPES ioType, UINT num);
	IChannel* GetPatternChannelLabel(IOTYPES ioType, UINT num, BYTE label, BYTE id);
	CPattern* GetPattern() { return _pattern; }
};

#endif /* ICHANNELOUT_H_ */
