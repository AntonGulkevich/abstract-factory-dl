/*
 * chlogicalin.h
 *
 *  Created on: 01 ���� 2015 �.
 *      Author: Youbrin_A
 */

#ifndef CHLOGICALIN_H_
#define CHLOGICALIN_H_


#include "ichannelin.h"
class ChLogicalOut;

class ChLogicalIn : public IChannelIn
{
private:
	ChLogicalOut* _channel; // �������� ������ ��� LogicalIn

public:
	ChLogicalIn(CPattern* const pattern, UINT number);

	virtual ~ChLogicalIn() {}

	void AssignChannel(IInputValue* ch)  {  _channel = (ChLogicalOut*)ch;}

	virtual void InitRegisters();
	virtual void UpdateDataToHW();
	virtual void UpdateHWToData();

};

#endif /* CHLOGICALIN_H_ */
