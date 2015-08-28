/*
 * chnotconnectedio.h
 *
 *  Created on: 05 θώνÿ 2015 γ.
 *      Author: Youbrin_A
 */

#ifndef CHNOTCONNECTEDIO_H_
#define CHNOTCONNECTEDIO_H_
#include "ichannel.h"

class ChNotConnectedIO : public IChannel
{
public:
	ChNotConnectedIO(CPattern* pattern, IOTYPES type, UINT num);
	virtual ~ChNotConnectedIO() {}

	virtual void InitRegisters() {}
	virtual void UpdateDataToHW() {}
	virtual void UpdateHWToData() {}
};

#endif /* CHNOTCONNECTEDIO_H_ */
