/*
 * chlogicalin.h
 *
 *  Created on: 01 июн€ 2015 г.
 *      Author: Youbrin_A
 */

#ifndef CHLOGICALIN_H_
#define CHLOGICALIN_H_


#include "ichannelin.h"
class ChLogicalOut;

class ChLogicalIn : public IChannelIn
{
private:
	ChLogicalOut* _channel; // явл€етс€ входом дл€ LogicalIn

public:
	ChLogicalIn(CPattern* const pattern, UINT number);

	virtual ~ChLogicalIn() {}

	void AssignChannel(IInputValue* ch)  {  _channel = (ChLogicalOut*)ch;}

	virtual void InitRegisters();
	virtual void UpdateDataToHW();
	virtual void UpdateHWToData();

};

#endif /* CHLOGICALIN_H_ */
