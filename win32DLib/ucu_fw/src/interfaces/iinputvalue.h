/*
 * iinputvalue.h
 *
 *  Created on: 10 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef IINPUTVALUE_H_
#define IINPUTVALUE_H_
#include "../types.h"

class IInputValue
{
protected:
	float _value;
	IOSTATE _state;

public:
	float GetValue()  {return _value; }
	IOSTATE GetState()  {return _state; }

	void virtual UpdateValue() = 0;
	IInputValue() { _value = 0; _state.dword = 0; }
	virtual ~IInputValue() {}
};

#endif /* IINPUTVALUE_H_ */
