/*
 * chnptout.h
 *
 *  Created on: 01 θώνÿ 2015 γ.
 *      Author: Youbrin_A
 */

#ifndef CHNPTOUT_H_
#define CHNPTOUT_H_
#include "ichannelout.h"

class NPTOut;

class ChNPTOut : public IChannelOut
{
private:
	NPTOut* _channel;

public:
	ChNPTOut(CPattern* const pattern, UINT number);
	virtual ~ChNPTOut() {}

	void AssignChannel(IOutputValue* ch)  { _channel = (NPTOut*)ch; }

	virtual void InitRegisters();
	virtual void UpdateDataToHW();
	virtual void UpdateHWToData();
};

#endif /* CHNPTOUT_H_ */
