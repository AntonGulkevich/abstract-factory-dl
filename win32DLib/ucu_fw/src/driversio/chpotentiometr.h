/*
 * chpotentiometr.h
 *
 *  Created on: 01 ���� 2015 �.
 *      Author: Youbrin_A
 */

#ifndef CHPOTENTIOMETR_H_
#define CHPOTENTIOMETR_H_


#include "channelcalibration.h"
#include "ichannelin.h"

class Potentiometr;

class ChPotentiometr : public IChannelIn
{
private:
	Potentiometr* _channel;

public:
	ChPotentiometr(CPattern* const pattern, UINT number);

	virtual ~ChPotentiometr() {}

	void AssignChannel(IInputValue* ch)  { _channel = (Potentiometr*)ch; }

	virtual void InitRegisters();
	virtual void UpdateDataToHW();
	virtual void UpdateHWToData();
};

#endif /* CHPOTENTIOMETR_H_ */
