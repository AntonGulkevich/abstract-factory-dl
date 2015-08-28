/*
 * ichannelin.cpp
 *
 *  Created on: 01 июня 2015 г.
 *      Author: Youbrin_A
 */

#include "ichannelout.h"

#include "../application/cpattern.h"
#include "../application/cprocessor.h"
#include "../driversio/charincin.h"
#include "../driversio/charincout.h"


IChannelOut::IChannelOut(CPattern* const pattern): IChannel(pattern), _proc(new CProcessor(this))
{


}

IChannel* IChannelOut::GetPatternChannel(IOTYPES ioType, UINT num)
{
	if (IS_IN(ioType))
		return _pattern->GetInput(ioType, num);
	else if (IS_OUT(ioType))
		return _pattern->GetOutput(ioType, num);
	return NULL;
}

IChannel* IChannelOut::GetPatternChannelLabel(IOTYPES ioType, UINT num, BYTE label, BYTE id)
{
	if (IS_IN(ioType))
		return (IChannel*)((ChArincIn*)_pattern->GetInput(ioType, num))->GetLabel(label);  // Добавить чтение по ID
	else if (IS_OUT(ioType))
		return (IChannel*)((ChArincOut*)_pattern->GetOutput(ioType, num))->GetLabel(label);  // Добавить чтение по ID;
	return NULL;
}
