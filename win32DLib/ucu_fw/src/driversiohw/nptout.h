/*
 * sktout.h
 *
 *  Created on: 13 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef NPTOUT_H_
#define NPTOUT_H_

#include "../types.h"
#include "../interfaces/ioutputvalue.h"

class NptControl;

class NPTOut : public IOutputValue
{

public:

	struct Calibrations
	{
		float scale;
		int offset;
	};

private:
	DWORD _baseAddress;
	DWORD _outputOffset;
	NptControl* _control;

	const DWORD typeOffset = 0x0;
	const DWORD valueOffset = 0x30;
	const DWORD registerOffset = 0x80;
	const DWORD failOffset = 0x00;
	const DWORD failSingleOffset = 0x60;
	const DWORD overloadOffset = 0x70;

public:
	void UpdateValue();


	NPTOut(DWORD baseAddress, DWORD outputOffset, NptControl* control);
	virtual ~NPTOut();
	// Настройки контроля
	// Допустимое отклонение от контрольного
	void SetControlDeviation(float value) {UCU_IOWR_32DIRECTF(_baseAddress, _outputOffset + valueOffset, value); }


	// Функции для диагностики
	float GetPLDValue() { return UCU_IORD_32DIRECTF(_baseAddress, _outputOffset + valueOffset); }
	WORD GetRegister() { return UCU_IORD_32DIRECT(_baseAddress, _outputOffset + registerOffset); }

	// Функции калибровки
	void SetCalibration(Calibrations& calib);
};

#endif /* NPTOUT_H_ */
