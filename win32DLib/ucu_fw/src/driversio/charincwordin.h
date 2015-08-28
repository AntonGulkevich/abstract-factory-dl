/*
 * charincin.h
 *
 *  Created on: 03 июня 2015 г.
 *      Author: Youbrin_A
 */

#ifndef CHARINCWORDIN_H_
#define CHARINCWORDIN_H_

#include "ichannelin.h"

class ArincWordIn;
class ChArincIn;

class ChArincWordIn : public IChannelIn
{
private:
	ArincWordIn* _channel;
	ChArincIn* _patternChannel; // Канала в паттерне которому принадлежит метка
	DWORD _dataMask; // Маска обрезки данных при распаковке слова

public:
	ChArincWordIn(CPattern* const pattern, ChArincIn* patternChannel, UINT number);
	virtual ~ChArincWordIn() {}

	void AssignChannel(IInputValue* ch)  { _channel = (ArincWordIn*)ch; }

	virtual void InitRegisters();
	virtual void UpdateDataToHW();
	virtual void UpdateHWToData();

	ChArincIn* GetChannel() { return _patternChannel; }
	bool IsTimeouted();


};

#endif /* CHARINCWORDIN_H_ */
