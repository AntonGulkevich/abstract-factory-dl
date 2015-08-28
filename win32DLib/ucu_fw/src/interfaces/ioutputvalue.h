/*
 * ioutputvalue.h
 *
 *  Created on: 13 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef IOUTPUTVALUE_H_
#define IOUTPUTVALUE_H_
#include "../types.h"

class IOutputValue {
protected:
	float _value;
	IOSTATE _state;

public:
	void SetValue(float value)  { _value = value; }
	float GetValue()  {return _value; }
	IOSTATE GetState()  {return _state; }
	void virtual UpdateValue() = 0;
	IOutputValue();
	virtual ~IOutputValue();
};

#endif /* IOUTPUTVALUE_H_ */
