/*
 * driversiomanager.h
 *
 *  Created on: 10 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef DRIVERSIOMANAGER_H_
#define DRIVERSIOMANAGER_H_
#include <cwctype>

#include "interfaces/ioutputvalue.h"
#include <vector>

#include "driversiohw/adccontrol.h"



#include "driversiohw/rsin.h"
#include "driversiohw/rsout.h"
#include "driversiohw/sctout.h"
#include "driversiohw/nptout.h"

#include "interfaces/idispatchflags.h"
#include "interfaces/iusingflags.h"
#include "interfaces/iusingirq.h"


#include "types.h"

class AnalogIn;
class Potentiometr;
class IInputValue;
class ADCControl;
class SCTOut;
class NPTOut;
class DescreeteInput;
class DescreeteOutput;
class ArincIn;
class ArincOut;
class ArincWordOut;
class ArincWordIn;
class NptControl;
class Calibrate;

class DriversIOManager : public IDispatchFlags, public IUsingIrq, public IUsingFlags
{

public:
	const static UINT analogInCount = 13;
	const static UINT potentiometrCount = 6;
	const static UINT descreeteInputCount = 29;
	const static UINT arincInCount = 6;
	const static UINT rsInCount = 2;

	const static UINT SCTOutCount = 6;
	const static UINT NPTOutCount = 3;
	const static UINT descreeteOutputCount = 8;
	const static UINT arincOutCount = 4;
	const static UINT rsOutCount = 2;

	struct Calibration
	{
		DWORD version;
		DWORD blockNumber;
		ADCControl::Calibrations analogIn;
		NPTOut::Calibrations NPT[NPTOutCount];
		SCTOut::Calibrations SCT[SCTOutCount];
		DWORD cs;
	};

private:
	ADCControl* _adcControl;

	AnalogIn* _analogIns[analogInCount];
	AnalogIn* _SCTControls[SCTOutCount];
	NptControl* _NPTControls[NPTOutCount];
	Potentiometr* _potentiometrs[potentiometrCount];
	DescreeteInput* _descreeteInputs[descreeteInputCount];
	ArincIn* _arincIns[arincInCount];
	RSIn* _rsIns[rsInCount];


	SCTOut* _SCTOuts[SCTOutCount];
	NPTOut* _NPTOuts[NPTOutCount];
	DescreeteOutput* _descreeteOutputs[descreeteOutputCount];
	ArincOut* _arincOuts[arincOutCount];
	RSOut* _rsOuts[rsOutCount];

	Calibrate* _calibrate;

	std::vector<ArincWordOut*> _arincWordsOut;
	std::vector<ArincWordIn*> _arincWordsIn;

	UINT _timerCount;


	std::vector<IInputValue*> _inputs;
	std::vector<IOutputValue*> _outputs;

    IO_GROUP_STATE _state;

    // Контроль наличия прерываний
    UINT _prevTimerCount;
    UINT _irqCount;

    void UpdateGloabalState();

public:
	DriversIOManager();
	virtual ~DriversIOManager();

	ADCControl* GetADCControl() { return _adcControl; }
	Calibrate* GetCalibrate() { return _calibrate; }
	AnalogIn* GetAnalogIn(UINT index) { return index < analogInCount ? _analogIns[index] : UCUNULL; }
	AnalogIn* GetSCTControl(UINT index) { return index < SCTOutCount ? _SCTControls[index] : UCUNULL; }
	NptControl* GetNPTControl(UINT index) { return index < NPTOutCount ? _NPTControls[index] : UCUNULL; }
	Potentiometr* GetPotentiometr(UINT index) { return index < potentiometrCount ? _potentiometrs[index] : UCUNULL; }
	DescreeteInput* GetDescreeteIn(UINT index) { return index < descreeteInputCount ? _descreeteInputs[index] : UCUNULL; }
	ArincIn* GetArincIn(UINT index) { return index < arincInCount ? _arincIns[index] : UCUNULL; }
	RSIn* GetRSIn(UINT index) { return index < rsInCount ? _rsIns[index] : UCUNULL;  }

	SCTOut* GetSCTOut(UINT index) { return index < SCTOutCount ? _SCTOuts[index] : UCUNULL; }
	NPTOut* GetNPTOut(UINT index) { return index < NPTOutCount ? _NPTOuts[index] : UCUNULL; }
	DescreeteOutput* GetDescreeteOut(UINT index) { return index < descreeteOutputCount ? _descreeteOutputs[index] : UCUNULL; }
	ArincOut* GetArincOut(UINT index) { return index < arincOutCount ? _arincOuts[index] : UCUNULL; }
	RSOut* GetRSOut(UINT index) { return index < rsOutCount ? _rsOuts[index] : UCUNULL;  }

	void AddArincWord(ArincWordOut* word) { _arincWordsOut.push_back(word); }
	void AddArincWord(ArincWordIn* word) { _arincWordsIn.push_back(word); }

	void UpdateInputData();
	void UpdateOutputData();

	IInputValue* GetInput(IOTYPES ioType, UINT num);
	IOutputValue* GetOutput(IOTYPES ioType, UINT num);

	void OnIrq(DWORD id);
	DISPATH_RESULT ProcessFlag(Flags id);

	bool IsIOExist(IOTYPES ioType, UINT num);

	IO_GROUP_STATE GetState() { return _state; }
	bool IsIrqNormal() { bool diff = _prevTimerCount != _timerCount; _prevTimerCount = _timerCount;  return diff; }
	UINT GetCurrentIrqNum() { return _irqCount; }
};

#endif /* DRIVERSIOMANAGER_H_ */
