/*
 * charincout.h
 *
 *  Created on: 01 θώνÿ 2015 γ.
 *      Author: Youbrin_A
 */

#ifndef CHARINCOUT_H_
#define CHARINCOUT_H_
#include "ichannelout.h"
#include "../driversiohw/arinctypes.h"

class ArincOut;
class ChArincWordOut;

class ChArincOut : public IChannelOut
{
private:
	ArincOut* _channel;
	ChArincWordOut* _word[256];
	const Arinc::Rate _arincRates[4] = { Arinc::Rate::R12_5, Arinc::Rate::R12_5,  Arinc::Rate::R50,  Arinc::Rate::R100 };
	bool _isDataExits;

public:
	ChArincOut(CPattern* const pattern, UINT number);
	virtual ~ChArincOut() {}

	void AssignChannel(IOutputValue* ch)  { _channel = (ArincOut*)ch; }

	virtual void InitRegisters();
	virtual void UpdateDataToHW();
	virtual void UpdateHWToData();

	bool IsLabelExist(BYTE label) { return _word[label] != NULL; }
	void AddLabel(BYTE label);
	ChArincWordOut* GetLabel(BYTE label) { return _word[label]; }
	bool IsDataExists() { return _isDataExits; }
};

#endif /* CHARINCOUT_H_ */
