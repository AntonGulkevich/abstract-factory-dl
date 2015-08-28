/*
 * analogin.h
 *
 *  Created on: 30 марта 2015 г.
 *      Author: Youbrin_A
 */

#ifndef ANALOGIN_H_
#define ANALOGIN_H_
#include <cmath>

#include "../types.h"
#include "../interfaces/iinputvalue.h"


class AnalogIn : public IInputValue
{

public:

	enum class GAIN_COEFFICIENTS
	{
		K_x01,
		K_x02,
		K_x05,
		K_x1,
		Count
	};

	enum class TYPE
	{
		Disabled,
		SCT,
		Logometr,
		Selsin,
		NPT,
		ControlNPT,
		ControlSCT,
		ControlSelsin,
		Calibration,
		Count
	};

	enum class ANGLE_TYPE
	{
		NoAngleType,
		SCT8,
		SCT20,
		SCT36,
		ControlSCT,
		Logometr,
		Selsin,
		ControlSelsin,
		Count,
	};

protected:


	struct ANGLE_CONST
	{
		float MinLevelV;
		GAIN_COEFFICIENTS GainCoeff;
		TYPE Type;
		float MaxScaleV;
	};


	// Адреса параметров в ПЛИС
	const UINT SYNC_OFFSET = 0;
	const UINT DELAY_OFFSET = 0x100;
	const UINT DELAYINVERSE_OFFSET = 0xE00;
	const UINT GAIN_OFFSET = 0x200;
	const UINT TYPE_OFFSET = 0x300;
	const UINT MIN_LEVEL_OFFSET = 0x400;
	const UINT NO_SIGNAL_OFFSET = 0x500;
	const UINT VALUE_OFFSET = 0x600;
	const UINT REG1_OFFSET = 0x700;
	const UINT REG2_OFFSET = 0x800;
	const UINT REG3_OFFSET = 0x900;

	const UINT FAIL_LOW_LEVEL_OFFSET = 0xB00;
	const UINT FAIL_NO_SIGNAL_OFFSET = 0xC00;
	const UINT FAIL_OVERSCALE_OFFSET = 0xD00;
	const UINT MEASURE_COUNT_OFFSET = 0xF00;



	// Константы усилений
	const float maxScaleV[(UINT)GAIN_COEFFICIENTS::Count] = { 100.0f, 50.0f, 20.0f, 10.0f };
	const UINT maxScaleADC = (1 << 13)-1;
	// Константы для угловых датчиков
	const ANGLE_CONST angleConsts[(UINT)ANGLE_TYPE::Count] =
	{
			{0, GAIN_COEFFICIENTS::K_x01, TYPE::Disabled, 1.0f },
			{ 6, GAIN_COEFFICIENTS::K_x05, TYPE::SCT, 20.0f },
			{ 13, GAIN_COEFFICIENTS::K_x02, TYPE::SCT, 50.0f },
			{ 20, GAIN_COEFFICIENTS::K_x01, TYPE::SCT, 100.0f },
			{ 6, GAIN_COEFFICIENTS::K_x01, TYPE::ControlSCT, 20.0f },
			{ 7, GAIN_COEFFICIENTS::K_x02, TYPE::Logometr, 50.0f },
			{ 30, GAIN_COEFFICIENTS::K_x01, TYPE::Selsin,100.0f },
			{ 6, GAIN_COEFFICIENTS::K_x01, TYPE::ControlSelsin, 20.0f },
	};


	DWORD _baseAddress;
	DWORD _inputOffset;

	WORD reg1;
	WORD reg2;
	WORD reg3;

	ANGLE_TYPE _angleType = ANGLE_TYPE::NoAngleType;

public:
	AnalogIn(DWORD baseAddress, DWORD inputOffset);
	virtual ~AnalogIn();

	// Установка номера синхронизации
	void SetSyncNumber(UINT sync) { UCU_IOWR_32DIRECT(_baseAddress, SYNC_OFFSET + _inputOffset, sync); }
	// Установка задержки измерения в мкс
	void SetDelay(UINT delay) { UCU_IOWR_32DIRECT(_baseAddress, DELAY_OFFSET + _inputOffset, delay / 20); UCU_IOWR_32DIRECT(_baseAddress, DELAYINVERSE_OFFSET + _inputOffset, delay / 20); }
	// Установка коэффициента усиления
	void SetGainCoefficient(GAIN_COEFFICIENTS coeff);
	// Установка коэффициента усиления
	void SetType(TYPE type) { UCU_IOWR_32DIRECT(_baseAddress, TYPE_OFFSET + _inputOffset, (UINT)type); }
	// Установка порога низкого уровня сигнала (доля от максимальной шкалы 0..1)
	// Устанавливается для угловых типов, вызывать после установки типа
	void SetMinlevel(float voltage)
	{
		UINT minLevel = roundf(voltage * voltage * maxScaleADC  * maxScaleADC / angleConsts[(UINT)_angleType].MaxScaleV / angleConsts[(UINT)_angleType].MaxScaleV);
		UCU_IOWR_32DIRECT(_baseAddress, MIN_LEVEL_OFFSET + _inputOffset, (UINT)minLevel);
	}
	// Установка коэффициента усиления
	void SetAngleType(ANGLE_TYPE type);

	TYPE GetType() { return (TYPE)UCU_IORD_32DIRECT(_baseAddress, TYPE_OFFSET + _inputOffset); }
	ANGLE_TYPE GetAngleType() { return _angleType; }
	UINT GetDelay() { return UCU_IORD_32DIRECT(_baseAddress, DELAY_OFFSET + _inputOffset) * 20; }
	UINT GetSyncNum() { return  UCU_IORD_32DIRECT(_baseAddress, SYNC_OFFSET + _inputOffset); }
	GAIN_COEFFICIENTS GetGainKoefficient() { return  (GAIN_COEFFICIENTS)UCU_IORD_32DIRECT(_baseAddress, GAIN_OFFSET + _inputOffset); }


	virtual void UpdateValue();

	WORD GetRegister1() { return reg1; }
	WORD GetRegister2() { return reg2; }
	WORD GetRegister3() { return reg3; }

	BYTE GetMeasureCount() { return UCU_IORD_32DIRECT(_baseAddress, MEASURE_COUNT_OFFSET + _inputOffset); }


};

#endif /* ANALOGIN_H_ */
