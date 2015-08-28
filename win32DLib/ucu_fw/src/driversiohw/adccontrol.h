/*
 * adccontrol.h
 *
 *  Created on: 03 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef ADCCONTROL_H_
#define ADCCONTROL_H_
#include "../types.h"
#include "../interfaces/iusingirq.h"
#include "../interfaces/iusingflags.h"
#include "analogin.h"

class ADCControl
{
public:
	enum class Reference
	{
		Auto = 0x8,
		Ref1 = 0x1,
		Ref2 = 0x2,
		Ref3 = 0x4,
		Off = 0,
	};

	struct OscillographData
	{
		DWORD marker;
		WORD channel1[1000];
		WORD channel2[1000];
		WORD channel3[1000];
		WORD sync[1000];
	};

	union Calibrations
	{
		struct
		{
			short a1g1;
			short a1g2;
			short a1g3;
			short a1g4;

			short a2g1;
			short a2g2;
			short a2g3;
			short a2g4;

			short a3g1;
			short a3g2;
			short a3g3;
			short a3g4;

		};
		struct
		{
			short a1[4];
			short a2[4];
			short a3[4];
		};
		short params[12];
	};

private:
	DWORD _baseAddress;
	IO_GROUP_STATE _state;
	OscillographData _oscillographData;

public:

	ADCControl(DWORD baseAddress);
	virtual ~ADCControl();

	UINT GetStatus() { return UCU_IORD_32DIRECT(_baseAddress, 0x38); }
	UINT GetSynchronisationCorrect() { return UCU_IORD_32DIRECT(_baseAddress, 0x44);  }
	// Регистр проведенных измерений, если была попытка измерить
	UINT GetMeasureTry() { return UCU_IORD_32DIRECT(_baseAddress, 0x40);  }
	// Регистр проведенных измерений, если померялось и записалось в память
	UINT GetMeasureWritten() { return UCU_IORD_32DIRECT(_baseAddress, 0x4C);  }
	// Регистр проведенных измерений, если померялось и записалось в память
	DWORD GetCnvst() { return UCU_IORD_32DIRECT(_baseAddress, 0x74);  }
	DWORD GetClkEn() { return UCU_IORD_32DIRECT(_baseAddress, 0x78);  }
	DWORD GetWrChn() { return UCU_IORD_32DIRECT(_baseAddress, 0x7C);  }
	void UpdateState();
	IO_GROUP_STATE GetState() { return _state; }
	void SetReference(Reference ref) { UCU_IOWR_32DIRECT(_baseAddress, 0x30, (UINT)ref); }
	void SetInverseOff(bool isInverseOff) { UCU_IOWR_32DIRECT(_baseAddress, 0x70, isInverseOff ? 1 : 0); }
	bool IsInverseOff() { return UCU_IORD_32DIRECT(_baseAddress, 0x70); }

	// Установка параметров калибровок
	void SetCalibration(Calibrations& calibrations);
	// разрешение прерывания 20мс
	void EnableInternalIrq() { UCU_IOWR_32DIRECT(_baseAddress, 0x50, 1); }
	bool IsInternalIrqEnabled() { return (UCU_IORD_32DIRECT(_baseAddress, 0x50) & 1) != 0; }
	void ResetIrq() { UCU_IORD_32DIRECT(_baseAddress, 0x38); }

	void EnableOscillograph(UINT channelNumber, AnalogIn::GAIN_COEFFICIENTS gain);
	void DisableOscillograph()  {  UCU_IOWR_32DIRECT(_baseAddress, 0x54, 0); }
	OscillographData* GetOscillographData();

};

#endif /* ADCCONTROL_H_ */
