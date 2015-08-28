/*
 * descreeteinput.h
 *
 *  Created on: 20 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef DESCREETEINPUT_H_
#define DESCREETEINPUT_H_
#include "../interfaces/iinputvalue.h"
#include "../types.h"

class DescreeteInput : public IInputValue
{
private:
	DWORD _baseAddress;
	UINT _inputOffset;

	const UINT VALUE_OFFSET = 0x80;

public:
	DescreeteInput(DWORD baseAddress, UINT inputOffset) : _baseAddress(baseAddress), _inputOffset(inputOffset) {}
	virtual ~DescreeteInput() {}

	void UpdateValue();

};

#endif /* DESCREETEINPUT_H_ */
