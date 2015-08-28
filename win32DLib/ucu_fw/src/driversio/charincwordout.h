/*
 * charincin.h
 *
 *  Created on: 03 июня 2015 г.
 *      Author: Youbrin_A
 */

#ifndef CHARINCWORDOUT_H_
#define CHARINCWORDOUT_H_

#include "ichannelout.h"

class ArincWordOut;
class ChArincOut;

class ChArincWordOut : public IChannelOut
{
private:
	ArincWordOut* _channel;
	ChArincOut* _patternChannel; // Канала в паттерне которому принадлежит метка
	UINT _period;

public:
	ChArincWordOut(CPattern* const pattern, ChArincOut* patternChannel, UINT number);
	virtual ~ChArincWordOut() {}

	void AssignChannel(IOutputValue* ch)  { _channel = (ArincWordOut*)ch; }

	virtual void InitRegisters();
	virtual void UpdateDataToHW();
	virtual void UpdateHWToData();

	UINT GetPeriod() { return _period; }
	void SetTableMode();
	bool IsOnline() { return GetRegister(REGISTER_ID::rCONTROL)->GetValueUInt() == 0; }

	ChArincOut* GetChannel() { return _patternChannel; }
};

#endif /* CHARINCWORDOUT_H_ */
