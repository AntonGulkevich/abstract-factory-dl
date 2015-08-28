/*
 * chdescreeteout.h
 *
 *  Created on: 01 θώνÿ 2015 γ.
 *      Author: Youbrin_A
 */

#ifndef CHDESCREETOUT_H_
#define CHDESCREETOUT_H_
#include "ichannelout.h"

class DescreeteOutput;

class ChDescreeteOut : public IChannelOut
{
private:
	DescreeteOutput* _channel;

public:
	ChDescreeteOut(CPattern* const pattern, UINT number);
	virtual ~ChDescreeteOut() {}

	void AssignChannel(IOutputValue* ch)  { _channel = (DescreeteOutput*)ch; }

	virtual void InitRegisters();
	virtual void UpdateDataToHW();
	virtual void UpdateHWToData();
};

#endif /* CHDESCREETOUT_H_ */
