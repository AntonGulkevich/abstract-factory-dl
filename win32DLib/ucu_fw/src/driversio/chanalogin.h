/*
 * chanalogin.h
 *
 *  Created on: 01 θώνÿ 2015 γ.
 *      Author: Youbrin_A
 */

#ifndef CHANALOGIN_H_
#define CHANALOGIN_H_


#include "channelcalibration.h"
#include "ichannelin.h"

class AnalogIn;

class ChAnalogIn : public IChannelIn
{
private:
	AnalogIn* _channel;

public:
	ChAnalogIn(CPattern* const pattern, UINT number);

	virtual ~ChAnalogIn();

	void AssignChannel(IInputValue* ch)  { _channel = (AnalogIn*)ch; }

	virtual void InitRegisters();
	virtual void UpdateDataToHW();
	virtual void UpdateHWToData();
};

#endif /* CHANALOGIN_H_ */
