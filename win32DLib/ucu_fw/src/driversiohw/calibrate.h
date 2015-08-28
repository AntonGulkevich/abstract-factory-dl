/*
 * calibrate.h
 *
 *  Created on: 16 июля 2015 г.
 *      Author: Youbrin_A
 */

#ifndef CALIBRATE_H_
#define CALIBRATE_H_

#include "analogin.h"

class Calibrate
{
private:
	DWORD _baseAddress;
	const DWORD _inputOffset = 22 * 4;
	// Адреса параметров в ПЛИС
	const UINT SYNC_OFFSET = 0;
	const UINT DELAY_OFFSET = 0x100;
	const UINT DELAYINVERSE_OFFSET = 0xE00;
	const UINT GAIN_OFFSET = 0x200;
	const UINT TYPE_OFFSET = 0x300;
	const UINT REG1_OFFSET = 0x700;
	const UINT REG2_OFFSET = 0x800;
	const UINT REG3_OFFSET = 0x900;
	const UINT REG_REF_OFFSET = 0xA00;
	const UINT MEASURE_COUNT_OFFSET = 0xF00;

public:

	void SetGainCoefficient(AnalogIn::GAIN_COEFFICIENTS coeff) { UCU_IOWR_32DIRECT(_baseAddress, GAIN_OFFSET + _inputOffset, (UINT)coeff); }


	Calibrate(DWORD baseAddress);
	virtual ~Calibrate() {}


	WORD GetRegister1() { return UCU_IORD_32DIRECT(_baseAddress, REG1_OFFSET + _inputOffset); }
	WORD GetRegister2() { return UCU_IORD_32DIRECT(_baseAddress, REG2_OFFSET + _inputOffset); }
	WORD GetRegister3() { return UCU_IORD_32DIRECT(_baseAddress, REG3_OFFSET + _inputOffset); }

	WORD GetReference() { return UCU_IORD_32DIRECT(_baseAddress, REG_REF_OFFSET + _inputOffset); }

	BYTE GetMeasureCount() { return UCU_IORD_32DIRECT(_baseAddress, MEASURE_COUNT_OFFSET + _inputOffset); }
};

#endif /* CALIBRATE_H_ */
