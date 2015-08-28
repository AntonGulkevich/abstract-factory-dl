/*
 * charincin.h
 *
 *  Created on: 03 ���� 2015 �.
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
	ChArincIn* _patternChannel; // ������ � �������� �������� ����������� �����
	DWORD _dataMask; // ����� ������� ������ ��� ���������� �����

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
