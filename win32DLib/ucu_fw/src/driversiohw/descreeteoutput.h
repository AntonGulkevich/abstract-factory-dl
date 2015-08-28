/*
 * DescreeteOutput.h
 *
 *  Created on: 20 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef DESCREETEOUTPUT_H_
#define DESCREETEOUTPUT_H_
#include "../interfaces/ioutputvalue.h"
#include "../types.h"

class DescreeteOutput : public IOutputValue
{
private:
	DWORD _baseAddress;
	UINT _outputNum;
	UINT _mask;

	const UINT VALUE_OFFSET = 0x00;
	const UINT STATE57_OFFSET = 0x10;
	const UINT STATE04_OFFSET = 0x0C;
	static UINT allValues;


public:
	DescreeteOutput(DWORD baseAddress, UINT outputNum) : _baseAddress(baseAddress), _outputNum(outputNum)
	{
		_mask = ~(1 << outputNum);
		_state.dword = 0;
		_value = 0;
		UpdateValue();
	}
	virtual ~DescreeteOutput() {}

	void UpdateValue();
};

#endif /* DESCREETEOUTPUT_H_ */
