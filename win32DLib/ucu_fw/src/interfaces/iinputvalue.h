/*
 * iinputvalue.h
 *
 *  Created on: 10 ���. 2015 �.
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
