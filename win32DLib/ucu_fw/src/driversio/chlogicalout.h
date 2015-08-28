/*
 * chlogicalout.h
 *
 *  Created on: 01 ���� 2015 �.
 *      Author: Youbrin_A
 */

#ifndef CHLOGICALOUT_H_
#define CHLOGICALOUT_H_
#include "../interfaces/iinputvalue.h"
#include "ichannelout.h"

class ChLogicalOut : public IChannelOut, public IInputValue // �������� ���������� ������ ��� LogicalIn
{
private:

public:
	ChLogicalOut(CPattern* const pattern, UINT number);
	virtual ~ChLogicalOut() {}

	void AssignChannel(IOutputValue* ch)  { }

	virtual void InitRegisters();
	virtual void UpdateDataToHW();
	virtual void UpdateHWToData();

	void UpdateValue();
};

#endif /* CHLOGICALOUT_H_ */
