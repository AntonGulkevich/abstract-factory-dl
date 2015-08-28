/*
 * ichannelin.h
 *
 *  Created on: 01 θώνÿ 2015 γ.
 *      Author: Youbrin_A
 */

#ifndef ICHANNELIN_H_
#define ICHANNELIN_H_

#include "ichannel.h"
#include "../interfaces/iinputvalue.h"

class IChannelIn : public IChannel
{
public:
	IChannelIn(CPattern* const pattern) : IChannel(pattern) {}
	virtual ~IChannelIn() {}

	virtual void AssignChannel(IInputValue* ch) = 0;
};

#endif /* ICHANNELIN_H_ */
