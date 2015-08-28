/*
 * pldmemorycontrol.h
 *
 *  Created on: 12 рту. 2015 у.
 *      Author: Youbrin_A
 */

#ifndef PLDMEMORYCONTROL_H_
#define PLDMEMORYCONTROL_H_
#include "types.h"

class PldMemoryControl
{
private:
	static const int AddressCount = 70;

public:
	PldMemoryControl();
	virtual ~PldMemoryControl();

	static DWORD Addresses[AddressCount][2];

	DWORD UcuRead(UINT baseAddress, UINT offset);
	float UcuReadF(UINT baseAddress, UINT offset);
	void UcuWrite(UINT baseAddress, UINT offset, DWORD data);
	void UcuWriteF(UINT baseAddress, UINT offset, float data);

	bool TestAddress(DWORD address);

};

extern PldMemoryControl memControl;

#endif /* PLDMEMORYCONTROL_H_ */
