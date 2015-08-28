/*
 * chsctout.h
 *
 *  Created on: 01 θώνÿ 2015 γ.
 *      Author: Youbrin_A
 */

#ifndef CHSCTOUT_H_
#define CHSCTOUT_H_
#include "ichannelout.h"

class SCTOut;

class ChSCTOut : public IChannelOut
{
private:
	SCTOut* _channel;

public:
	ChSCTOut(CPattern* const pattern, UINT number);
	virtual ~ChSCTOut() {}

	void AssignChannel(IOutputValue* ch)  { _channel = (SCTOut*)ch; }

	virtual void InitRegisters();
	virtual void UpdateDataToHW();
	virtual void UpdateHWToData();
};

#endif /* CHSCTOUT_H_ */
