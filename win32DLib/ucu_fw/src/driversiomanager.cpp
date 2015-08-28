/*
 * driversiomanager.cpp
 *
 *  Created on: 10 апр. 2015 г.
 *      Author: Youbrin_A
 */

#include "driversiomanager.h"
#include "driversiohw/analogin.h"
#include "driversiohw/potentiometr.h"
#include "interfaces/iinputvalue.h"
#include "xparameters.h"
#include "driversiohw/adccontrol.h"
#include "driversiohw/arincin.h"
#include "driversiohw/descreeteinput.h"
#include "driversiohw/descreeteoutput.h"
#include "driversiohw/arincout.h"
#include "driversiohw/arincwordout.h"
#include "driversiohw/arincwordin.h"
#include "driversiohw/calibrate.h"
#include "driversiohw/nptcontrol.h"

DriversIOManager::DriversIOManager()
{
	_irqCount = 0;
	_timerCount = 0;
	_state.dword = 0;
	_state.iostAnalog20msFail = 1;
	_adcControl = new ADCControl(XPAR_MAIN_ADC_HUB_0_BASEADDR + 0x1000);
	_adcControl->EnableInternalIrq();
	RegIRQ((DWORD)XPAR_FABRIC_MAIN_ADC_HUB_0_IRQ_INTR);

	_calibrate = new Calibrate((DWORD)XPAR_MAIN_ADC_HUB_0_BASEADDR);
	for(UINT i = 0; i < analogInCount; i++)
	{
		_analogIns[i] = new AnalogIn((DWORD)XPAR_MAIN_ADC_HUB_0_BASEADDR, i*4);
		_inputs.push_back(_analogIns[i]);
	}
	for(UINT i = 0; i < SCTOutCount; i++)
	{
		_SCTControls[i] = new AnalogIn((DWORD)XPAR_MAIN_ADC_HUB_0_BASEADDR, (i+13)*4);
		_inputs.push_back(_SCTControls[i]);
	}
	for(UINT i = 0; i < NPTOutCount; i++)
	{
		_NPTControls[i] = new NptControl((DWORD)XPAR_MAIN_ADC_HUB_0_BASEADDR, (i+19)*4);
		_inputs.push_back(_NPTControls[i]);
	}
	for(UINT i = 0; i < potentiometrCount; i++)
	{
		_potentiometrs[i] = new Potentiometr((DWORD)XPAR_POT_TOP_0_BASEADDR, i*4);
		_inputs.push_back(_potentiometrs[i]);
	}
	for(UINT i = 0; i < descreeteInputCount; i++)
	{
		_descreeteInputs[i] = new DescreeteInput((DWORD)XPAR_EX_MAX_0_BASEADDR, i*4);
		_inputs.push_back(_descreeteInputs[i]);
	}
	for(UINT i = 0; i < SCTOutCount; i++)
	{
		_SCTOuts[i] = new SCTOut((DWORD)XPAR_RESOLV_TOP_0_BASEADDR, i*4, _SCTControls[i]);
		_outputs.push_back(_SCTOuts[i]);
	}
	for(UINT i = 0; i < NPTOutCount; i++)
	{
		_NPTOuts[i] = new NPTOut((DWORD)XPAR_CURRENT_DAC_TOP_0_BASEADDR, i*4, _NPTControls[i]);
		_outputs.push_back(_NPTOuts[i]);
	}
	for(UINT i = 0; i < descreeteOutputCount; i++)
	{
		_descreeteOutputs[i] = new DescreeteOutput((DWORD)XPAR_EX_MAX_0_BASEADDR, i);
		_outputs.push_back(_descreeteOutputs[i]);
	}
	_arincIns[0] = new ArincIn(XPAR_ARINC_IN_TOP_0_BASEADDR);
	_arincIns[1] = new ArincIn(XPAR_ARINC_IN_TOP_1_BASEADDR);
	_arincIns[2] = new ArincIn(XPAR_ARINC_IN_TOP_2_BASEADDR);
	_arincIns[3] = new ArincIn(XPAR_ARINC_IN_TOP_3_BASEADDR);
	_arincIns[4] = new ArincIn(XPAR_ARINC_IN_TOP_4_BASEADDR);
	_arincIns[5] = new ArincIn(XPAR_ARINC_IN_TOP_5_BASEADDR);

	_arincOuts[0] = new ArincOut(XPAR_ARINC_OUT_TOP_0_BASEADDR, XPAR_TEST_ARINC_0_BASEADDR, 0);
	_arincOuts[1] = new ArincOut(XPAR_ARINC_OUT_TOP_1_BASEADDR, XPAR_TEST_ARINC_0_BASEADDR, 1);
	_arincOuts[2] = new ArincOut(XPAR_ARINC_OUT_TOP_2_BASEADDR, XPAR_TEST_ARINC_0_BASEADDR, 2);
	_arincOuts[3] = new ArincOut(XPAR_ARINC_OUT_TOP_3_BASEADDR, XPAR_TEST_ARINC_0_BASEADDR, 3);

	// Добавить ссылку на контрольный аринк

	_rsIns[0] = new RSIn(XPAR_INFO_RS_0_BASEADDR);
	_rsIns[1] = new RSIn(XPAR_RS485_TOP_0_BASEADDR);

	_rsOuts[0] = new RSOut(XPAR_INFO_RS_0_BASEADDR);
	_rsOuts[1] = new RSOut(XPAR_RS485_TOP_0_BASEADDR);

	ArincWordOut::SetArincTablePtr(new ArincOutTable(XPAR_ARINC_TABLE_0_BASEADDR));
	Potentiometr::SetBaseAddress(XPAR_POT_TOP_0_BASEADDR);
	Potentiometr::SetRef(Potentiometr::RefType::Variable);
	GetADCControl()->SetReference(ADCControl::Reference::Auto);
	//GetADCControl()->SetReference(ADCControl::Reference::Off);

}

DriversIOManager::~DriversIOManager()
{

}

void DriversIOManager::UpdateGloabalState()
{
	_state.iostAnalog20msFail = 0;
	_state.iostAnalogADCFail = GetADCControl()->GetState().iostAnalogADCFail;
	_state.iostSync1Fail = GetADCControl()->GetState().iostSync1Fail;
	_state.iostSync2Fail = GetADCControl()->GetState().iostSync2Fail;
	_state.iostSync3Fail = GetADCControl()->GetState().iostSync3Fail;
	_state.iostSync4Fail = GetADCControl()->GetState().iostSync4Fail;
	_state.iostSync5Fail = GetADCControl()->GetState().iostSync5Fail;
	_state.iostSync6Fail = GetADCControl()->GetState().iostSync6Fail;
	_state.iostSync7Fail = GetADCControl()->GetState().iostSync7Fail;
	_state.iostArincIn1Fail = GetArincIn(0)->GetState().iostArincChNoSignal;
	_state.iostArincIn2Fail = GetArincIn(1)->GetState().iostArincChNoSignal;
	_state.iostArincIn3Fail = GetArincIn(2)->GetState().iostArincChNoSignal;
	_state.iostArincIn4Fail = GetArincIn(3)->GetState().iostArincChNoSignal;
	_state.iostArincIn5Fail = GetArincIn(4)->GetState().iostArincChNoSignal;
	_state.iostArincIn6Fail = GetArincIn(5)->GetState().iostArincChNoSignal;
	_state.iostArincOut1Fail = GetArincOut(0)->GetState().iostArincChNoSignal;
	_state.iostArincOut2Fail = GetArincOut(1)->GetState().iostArincChNoSignal;
	_state.iostArincOut3Fail = GetArincOut(2)->GetState().iostArincChNoSignal;
	_state.iostArincOut4Fail = GetArincOut(3)->GetState().iostArincChNoSignal;
	_state.iostSDADCRefFail = 0; // TODO Выяснить как формировать

}


void DriversIOManager::UpdateInputData()
{
	_adcControl->UpdateState();
	for(UINT i = 0; i < _inputs.size(); i++)
	{
		_inputs[i]->UpdateValue();
	}
	for(UINT i = 0; i < arincInCount; i++)
		_arincIns[i]->UpdateValue();
	for(UINT i = 0; i < _arincWordsIn.size(); i++)
		_arincWordsIn[i]->UpdateValue();
	for(UINT i = 0; i < rsInCount; i++)
		_rsIns[i]->UpdateValue();
	UpdateGloabalState();
}

void DriversIOManager::UpdateOutputData()
{
	for(UINT i = 0; i < _outputs.size(); i++)
	{
		_outputs[i]->UpdateValue();
	}
	ArincOut::UpdateTactNumber();
	for(UINT i = 0; i < arincOutCount; i++)
		_arincOuts[i]->UpdateValue();
	for(UINT i = 0; i < _arincWordsOut.size(); i++)
		_arincWordsOut[i]->UpdateValue();
	for(UINT i = 0; i < rsOutCount; i++)
		_rsOuts[i]->UpdateValue();

}


DISPATH_RESULT DriversIOManager::ProcessFlag(Flags id)
{
	switch(id)
	{
	case Flags::Irq20ms:
		return drDispatched;
	default:
		return drNotDispatched;
	}
	return drNotDispatched;
}

void DriversIOManager::OnIrq(DWORD id)
{
	_irqCount++;
	_adcControl->ResetIrq();
	UpdateInputData();
	SetFlag(Flags::Irq20ms);
	_timerCount+=20;
	if (_timerCount % 80 == 0)
		SetFlag(Flags::Timer80ms);
	if (_timerCount % 500 == 0)
	{
		SetFlag(Flags::Timer500ms);
	}
	if (_timerCount % 1000 == 0)
	{
		SetFlag(Flags::Timer1Sec);
		_timerCount = 0;
	}
}

bool DriversIOManager::IsIOExist(IOTYPES ioType, UINT num)
{
	switch (ioType) {
	case ioDigitalIn:
		return num < arincInCount;
	case ioDigitalOut:
		return num < arincOutCount;
	case ioNPT:
		return num < NPTOutCount;
	case ioPotentiometr:
		return num < potentiometrCount;
	case ioRSIn:
		return num < rsInCount;
    case ioRSOut:
        return num < rsOutCount;
	case ioCommand:
		return num < descreeteOutputCount;
	case ioCheck:
		return num < descreeteInputCount;
	case ioSCT:
		return num < SCTOutCount;
	case ioAnalog:
		return num < analogInCount;
	default:
		break;
	}
	return false;
}

IInputValue* DriversIOManager::GetInput(IOTYPES ioType, UINT num)
{
	switch (ioType) {
	case ioDigitalIn:
		return GetArincIn(num);
	case ioPotentiometr:
		return GetPotentiometr(num);
	case ioRSIn:
		return GetRSIn(num);
	case ioCheck:
		return GetDescreeteIn(num);
	case ioAnalog:
		return GetAnalogIn(num);
	case ioDigitalOut:
	case ioNPT:
    case ioRSOut:
	case ioCommand:
	case ioSCT:
	default:
		break;
	}
	return UCUNULL;
}

IOutputValue* DriversIOManager::GetOutput(IOTYPES ioType, UINT num)
{
	switch (ioType)
	{
	case ioDigitalOut:
		return GetArincOut(num);
	case ioNPT:
		return GetNPTOut(num);
    case ioRSOut:
    	return GetRSOut(num);
	case ioCommand:
		return GetDescreeteOut(num);
	case ioSCT:
		return GetSCTOut(num);
	case ioDigitalIn:
	case ioPotentiometr:
	case ioRSIn:
	case ioCheck:
	case ioAnalog:
	default:
		break;
	}
	return UCUNULL;
}


