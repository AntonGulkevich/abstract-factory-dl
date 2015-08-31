/*
 * diagnostic.cpp
 *
 *  Created on: 07 апр. 2015 г.
 *      Author: Youbrin_A
 */

#include "diagnostic.h"

#include <string.h>


#include "../drivermanager.h"
#include "../driversiomanager.h"
#include "../driversiohw/analogin.h"
#include "../driversiohw/nptcontrol.h"
#include "../driversiohw/potentiometr.h"
#include "../driversiohw/sctout.h"
#include "../driversiohw/nptout.h"
#include "../driversiohw/descreeteinput.h"
#include "../driversiohw/descreeteoutput.h"
#include "../driversiohw/arincin.h"
#include "../driversiohw/arincout.h"
#include "../driversiohw/adccontrol.h"
#include "../driversiohw/calibrate.h"
#include "../utilities/m_math.h"
#include "../driversiohw/rsin.h"
#include "../driversiohw/rsout.h"
#include "../workmanager.h"
#include "../utilities/failmanager.h"
#include "../application/cconfiguration.h"
#include "../application/cpattern.h"
#include "../driversio/charincin.h"
#include "../driversio/charincwordin.h"
#include "../driversio/charincout.h"
#include "../driversio/charincwordout.h"
#include "../driversio/cregister.h"
#include "../utilities/console.h"
#include "../utilities/calibrationmanager.h"

#include "commod.h"
#include "version.h"

using version::datetime;

Diagnostic::Diagnostic(WorkManager* workManager) :
		_driversIO(workManager->GetDriversIO()), _drivers(workManager->GetDrivers()), _commod(workManager->GetCommod()), _workManager(workManager),
		_packetNumAnalog(0), _packetNumDigital(0)
{
	_mode = DiagnosticMode::Disabled;
	_diagFlashState = false;
	_oscillFlashState = false;

	_drivers->GetUSB()->MaskIrq(UsbTr::DataXReceived | UsbTr::DiagnosticAnalogReceived | UsbTr::DiagnosticDigitalReceived);
	RegFlag(Flags::SendDiagnostic, PRIORITY::BackGround);
	RegFlag(Flags::CommandPCReceived, PRIORITY::BackGround);
	RegFlag(Flags::DataPCReceived, PRIORITY::BackGround);

}

Diagnostic::~Diagnostic()
{
}


void Diagnostic::FillDiagVersion()
{
	_diagVersion.marker = 0x70A1EA5F;
	_diagVersion.PLDDate = _drivers->GetUSB()->GetPLDDate();
	_diagVersion.FWDate = datetime();
	_diagVersion.blockNumber = _workManager->GetCalibrationManager()->GetBlockNumber();
	_diagVersion.FWVersion = (VERSION_NUM << 24) | (SUBVERSION_NUM << 16) | PBV;
	_diagVersion.FWCS = 0;
}

void Diagnostic::FillDiagDataAnalog()
{
	_diagDataAnalog.marker = 0xD1A9DA7A;
	_diagDataAnalog.packetNum = _packetNumAnalog++;

	_diagDataAnalog.mainAdcFail = _driversIO->GetADCControl()->GetState().dword;
	_diagDataAnalog.mainAdcMeasureTry = _driversIO->GetADCControl()->GetMeasureTry();
	_diagDataAnalog.mainAdcMeasureWritten = _driversIO->GetADCControl()->GetMeasureWritten();
	_diagDataAnalog.mainAdcMeasureCount[22] = _driversIO->GetCalibrate()->GetMeasureCount();
	_diagDataAnalog.mainAdcMeasureCount[23] = 0;
	_diagDataAnalog.mainAdcCnvst = _driversIO->GetADCControl()->GetCnvst();
	_diagDataAnalog.mainAdcClken = _driversIO->GetADCControl()->GetClkEn();
	_diagDataAnalog.mainAdcWrchn = _driversIO->GetADCControl()->GetWrChn();

	for(UINT i = 0; i < DriversIOManager::analogInCount; i++)
	{
		auto ch = _driversIO->GetAnalogIn(i);
		_diagDataAnalog.mainAdcMeasureCount[i] = ch->GetMeasureCount();
		_diagDataAnalog.adcValue[i] = ch->GetValue();
		_diagDataAnalog.adcChannel1[i] = ch->GetRegister1();
		_diagDataAnalog.adcChannel2[i] = ch->GetRegister2();
		_diagDataAnalog.adcChannel3[i] = ch->GetRegister3();
		_diagDataAnalog.adcFails[i] = ch->GetState().dword;
	}
	for(UINT i = 0; i < DriversIOManager::SCTOutCount; i++)
	{
		auto ch = _driversIO->GetSCTControl(i);
		_diagDataAnalog.mainAdcMeasureCount[i+DriversIOManager::analogInCount] = ch->GetMeasureCount();
		_diagDataAnalog.adcSCTControlValue[i] = ch->GetValue();
		_diagDataAnalog.adcSCTControlChannel1[i] = ch->GetRegister1();
		_diagDataAnalog.adcSCTControlChannel2[i] = ch->GetRegister2();
	}
	for(UINT i = 0; i < DriversIOManager::NPTOutCount; i++)
	{
		auto ch = _driversIO->GetNPTControl(i);
		_diagDataAnalog.mainAdcMeasureCount[i+DriversIOManager::analogInCount + DriversIOManager::SCTOutCount] = ch->GetMeasureCount();
		_diagDataAnalog.adcNPTControlValue[i] = ch->GetValue();
		_diagDataAnalog.adcNPTControlChannel3[i] = ch->GetRegister3();
	}
	for(UINT i = 0; i < DriversIOManager::potentiometrCount; i++)
	{
		auto ch = _driversIO->GetPotentiometr(i);
		_diagDataAnalog.potValue[i] = ch->GetValue();
		_diagDataAnalog.potChannel[i] = ch->GetRegister();
		_diagDataAnalog.potFails[i] = ch->GetState().dword;
	}
	for(UINT i = 0; i < DriversIOManager::SCTOutCount; i++)
	{
		auto ch = _driversIO->GetSCTOut(i);
		_diagDataAnalog.SCTOutValue[i] = ch->GetValue();
		_diagDataAnalog.SCTOutChannel1[i] = ch->GetRegisterSin();
		_diagDataAnalog.SCTOutChannel2[i] = ch->GetRegisterCos();
		_diagDataAnalog.SCTOutFails[i] = ch->GetState().dword;
	}
	for(UINT i = 0; i < DriversIOManager::NPTOutCount; i++)
	{
		auto ch = _driversIO->GetNPTOut(i);
		_diagDataAnalog.NPTOutValue[i] = ch->GetValue();
		_diagDataAnalog.NPTOutChannel1[i] = ch->GetRegister();
		_diagDataAnalog.NPTOutFails[i] = ch->GetState().dword;
	}
	_diagDataAnalog.calibrateChannels[0] = _driversIO->GetCalibrate()->GetRegister1();
	_diagDataAnalog.calibrateChannels[1] = _driversIO->GetCalibrate()->GetRegister2();
	_diagDataAnalog.calibrateChannels[2] = _driversIO->GetCalibrate()->GetRegister3();
	_diagDataAnalog.calibrateChannels[3] = _driversIO->GetCalibrate()->GetReference();

	DWORD di = 0;
	for(UINT i = 0; i < DriversIOManager::descreeteInputCount; i++)
	{
		auto ch = _driversIO->GetDescreeteIn(i);
		di |= ((DWORD)ch->GetValue()) << i;
	}
	_diagDataAnalog.descreeteInput = di;
	DWORD dout = 0;
	for(UINT i = 0; i < DriversIOManager::descreeteOutputCount; i++)
	{
		auto ch = _driversIO->GetDescreeteOut(i);
		dout |= ((DWORD)ch->GetValue() & 1) << i;
	}
	_diagDataAnalog.descreeteOutput = dout;
	_diagDataAnalog.endMarker = 0xC215BA76;

}


void Diagnostic::FillDiagDataDigital()
{
	_diagDataDigital.marker = 0xD1A9D191;
	_diagDataDigital.packetNum = _packetNumDigital++;
	_diagDataDigital.currentTime = _drivers->GetClock()->GetCurrentTime();
	UINT count = 0;
	UINT wc = 0;
	//ARINC IN
	for(UINT i = 0;i < DriversIOManager::arincInCount; i++)
	{
		auto ch = _driversIO->GetArincIn(i);
		wc = ch->GetWordBufferCount();
		_diagDataDigital.data[count++] = wc;
		for(UINT w = 0; w < wc; w++)
		{
			_diagDataDigital.data[count+w*2] = ch->GetWordBuffer()[w].time/ 10; // Для совместимости с трассми ППК
			_diagDataDigital.data[count+w*2+1] = ch->GetWordBuffer()[w].value;
		}
		count += wc * 2;
	}

	// ARINC OUT
	for(UINT i = 0;i < DriversIOManager::arincOutCount; i++)
	{
		auto ch = _driversIO->GetArincOut(i);
		wc = ch->GetWordBufferCount();
		_diagDataDigital.data[count++] = wc;
		for(UINT w = 0; w < wc; w++)
		{
			_diagDataDigital.data[count+w*2] = ch->GetWordBuffer()[w].time/ 10; // Для совместимости с трассми ППК;
			_diagDataDigital.data[count+w*2+1] = ch->GetWordBuffer()[w].value;
		}
		count += wc * 2;
		ch->ResetWordBuffer();
	}
	// RS вх
	for(UINT i = 0;i < DriversIOManager::rsInCount; i++)
	{
		auto ch = _driversIO->GetRSIn(i);
		wc = (ch->GetBufferSize() + 3) >> 2;
		_diagDataDigital.data[count++] = ch->GetBufferSize();
		memcpy(&_diagDataDigital.data[count], ch->GetBuffer(), ch->GetBufferSize());
		count += wc;
	}

	//RSвых
	for(UINT i = 0;i < DriversIOManager::rsOutCount; i++)
	{
		auto ch = _driversIO->GetRSOut(i);
		wc = (ch->GetBufferSize() + 3) >> 2;
		_diagDataDigital.data[count++] = ch->GetBufferSize();
		memcpy(&_diagDataDigital.data[count], ch->GetBuffer(), ch->GetBufferSize());
		count += wc;
		ch->ResetDiagBuffer();
	}

	// Маркер конца
	_diagDataDigital.data[count++] = 0xC2FAAD15;

	_diagDataDigital.count = count + 3; // 3 - маркер + пакет + время
}

void Diagnostic::FillCommandResponse(Commands cmd)
{
	_commandResponse.marker = 0xABABABAB;
	_commandResponse.command = (UINT)cmd;
	_commandResponse.allOkMarker = 0x3F344A20;
}

void Diagnostic::FillConfigurationData()
{
	ConfigurationData cdata;
	cdata.marker = 0xAC8D0440;
	UINT channelsCount = 0;
	if (_workManager->GetConfiguration()->IsInitialized())
	{
		for(UINT p = 0; p < _workManager->GetConfiguration()->GetPatternCount(); p++)
		{
			for(UINT c = 0; c < _workManager->GetConfiguration()->GetPattern(p)->GetAllInputs().size(); c++)
			{
				auto ch = _workManager->GetConfiguration()->GetPattern(p)->GetAllInputs()[c];
				cdata.data[channelsCount] = ch->GetChannelIdentificator();
				cdata.data[channelsCount].channelHWNum = _workManager->GetConfiguration()->GetChannelHWNum(ch);
				channelsCount++;
			}
			for(UINT c = 0; c < _workManager->GetConfiguration()->GetPattern(p)->GetAllOutputs().size(); c++)
			{
				auto ch = _workManager->GetConfiguration()->GetPattern(p)->GetAllOutputs()[c];
				cdata.data[channelsCount] = ch->GetChannelIdentificator();
				cdata.data[channelsCount].channelHWNum = _workManager->GetConfiguration()->GetChannelHWNum(ch);
				channelsCount++;
			}
		}
	}
	cdata.channelsCount = channelsCount;
	cdata.count = cdata.channelsCount * sizeof(IChannel::ChannelData) + 4 + 4; // Количество каналов + маркер + количество
	_drivers->GetUSB()->WriteData(&cdata, cdata.count);

}

void Diagnostic::FillRegisterData(UINT patternNum, IOTYPES type, UINT num)
{
	ChannelRegistersData cdata;
	cdata.marker = RegistersRequest;
	cdata.irqNum = _driversIO->GetCurrentIrqNum();
	cdata.channelPattern = patternNum;
	cdata.channelType = type;
	cdata.channelNum = num;
	cdata.count = 0;
	UINT size = 0;
	if (patternNum == ConfigurationId)
	{
		cdata.marker = ConfigurationId;
		cdata.count = 2;
		auto regT = _workManager->GetConfiguration()->GetRegisterT(REGISTER_ID::rNAME);
		auto reg = _workManager->GetConfiguration()->GetRegister(REGISTER_ID::rNAME);
		RegisterData* data = reinterpret_cast<RegisterData*>(&cdata.data[size]);
		data->id = (BYTE)regT->id;
		data->type = (BYTE)regT->type;
		data->isFilled = reg->IsFilled() ? 1 : 0;
		data->registerType = (BYTE)regT->rw;
		data->isEmulated = (BYTE)reg->GetEmulated() ? 1 : 0;
		data->size = strlen(reg->GetValueStr());
		memcpy(data->data, reg->GetValueStr(), data->size);
		BYTE size4 = (data->size +3) & 0xFC;
		for(UINT i = data->size; i < size4; i++)
			data->data[i] = 0;
		data->size = size4;
		size += data->size + 8;

		regT = _workManager->GetConfiguration()->GetRegisterT(REGISTER_ID::rDATABASENUM);
		reg = _workManager->GetConfiguration()->GetRegister(REGISTER_ID::rDATABASENUM);
		data = reinterpret_cast<RegisterData*>(&cdata.data[size]);
		data->id = (BYTE)regT->id;
		data->type = (BYTE)regT->type;
		data->isFilled = reg->IsFilled() ? 1 : 0;
		data->registerType = (BYTE)regT->rw;
		data->isEmulated = (BYTE)reg->GetEmulated() ? 1 : 0;
		data->size = 4;
		int val = reg->GetValueInt();
		memcpy(data->data, &val, data->size);
		size += data->size + 8;
	}
	else
	{
		auto pattern = _workManager->GetConfiguration()->GetPatternByDeviceNum(patternNum);
		if (pattern != NULL)
		{
			IChannel* ch = (IS_IN(type) ? (IChannel*)pattern->GetInput(type, num) :
										  (IChannel*)pattern->GetOutput(type, num));
			if (ch != NULL)
			{
				for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
					if (ch->GetRegisterT((REGISTER_ID)i)->id != REGISTER_ID::NULLID)
					{
						auto data = reinterpret_cast<RegisterData*>(&cdata.data[size]);
						data->id = (BYTE)ch->GetRegisterT((REGISTER_ID)i)->id;
						data->type = (BYTE)ch->GetRegisterT((REGISTER_ID)i)->type;
						data->isFilled =  ch->GetRegister((REGISTER_ID)i)->IsFilled() ? 1 : 0;
						data->registerType = (BYTE)ch->GetRegisterT((REGISTER_ID)i)->rw;
						data->isEmulated = (BYTE)ch->GetRegister((REGISTER_ID)i)->GetEmulated() ? 1 : 0;
						switch(data->type)
						{
						case rtDec:
						{
							data->size = 4;
							auto val = ch->GetRegister((REGISTER_ID)i)->GetValueInt();
							memcpy(data->data, &val, data->size);
						}
							break;
						case rtFloat:
						{
							data->size = 4;
							auto val = ch->GetRegister((REGISTER_ID)i)->GetValueFloat();
							memcpy(data->data, &val, data->size);
						}
							break;
						case rtString:
						{
							data->size = strlen(ch->GetRegister((REGISTER_ID)i)->GetValueStr());
							memcpy(data->data, ch->GetRegister((REGISTER_ID)i)->GetValueStr(), data->size);
							BYTE size4 = (data->size +3) & 0xFC;
							for(UINT i = data->size; i < size4; i++)
								data->data[i] = 0;
							data->size = size4;
						}
							break;
						case rtArray:
						{
							data->size = ch->GetRegister((REGISTER_ID)i)->GetArraySize() * 4;
							memcpy(data->data, ch->GetRegister((REGISTER_ID)i)->GetArray(), data->size);
						}
							break;

						default:
							data->size = 0;
							break;
						}
						size += data->size + 8;
						cdata.count++;
					}

			}

		}
	}
	_drivers->GetUSB()->WriteData(&cdata, size + 4 + 4 + 4 + 4); // Маркер + количество + идентификатор канала + номер прерывания
}

void Diagnostic::FillChannelsSettings()
{
	ChannelsSettings data;
	data.marker = Markers::ChannelSettings;
	for(UINT i = 0; i < DriversIOManager::analogInCount; i++)
	{
		data.analog[i].type = (UINT)_driversIO->GetAnalogIn(i)->GetType();
		data.analog[i].analogType = data.analog[i].type == (UINT)AnalogIn::TYPE::NPT ? (UINT)_driversIO->GetAnalogIn(i)->GetGainKoefficient() : (UINT)_driversIO->GetAnalogIn(i)->GetAngleType();
		data.analog[i].delay = _driversIO->GetAnalogIn(i)->GetDelay();
		data.analog[i].syncNum =  _driversIO->GetAnalogIn(i)->GetSyncNum();
	}
	for(UINT i = 0;i < DriversIOManager::SCTOutCount; i++)
	{
		data.sct[i].type = (WORD)_driversIO->GetSCTOut(i)->GetType();
		data.sct[i].phase = (WORD)_driversIO->GetSCTOut(i)->GetPhaseShift();
	}
	for(UINT i = 0;i < DriversIOManager::arincInCount; i++)
	{
		data.arincIn[i].rate = (UINT)_driversIO->GetArincIn(i)->GetRate();
		data.arincIn[i].parity = (UINT)_driversIO->GetArincIn(i)->GetParity();
	}
	for(UINT i = 0;i < DriversIOManager::arincOutCount; i++)
	{
		data.arincOut[i].rate = (UINT)_driversIO->GetArincOut(i)->GetRate();
		data.arincOut[i].parity = (UINT)_driversIO->GetArincOut(i)->GetParity();
	}
	for(UINT i = 0;i < DriversIOManager::rsInCount; i++)
	{
		data.rsIn[i].rate = (UINT)_driversIO->GetRSIn(i)->GetBaudRate() / 100;
		data.rsIn[i].parity = (UINT)_driversIO->GetRSIn(i)->GetParityType();
		data.rsIn[i].stopBits = _driversIO->GetRSIn(i)->GetStopBits();
		data.rsIn[i].dataBits = _driversIO->GetRSIn(i)->GetNumberBits();
	}
	for(UINT i = 0;i < DriversIOManager::rsOutCount; i++)
	{
		data.rsOut[i].rate = (UINT)_driversIO->GetRSOut(i)->GetBaudRate() / 100;
		data.rsOut[i].parity = (UINT)_driversIO->GetRSOut(i)->GetParityType();
		data.rsOut[i].stopBits = _driversIO->GetRSOut(i)->GetStopBits();
		data.rsOut[i].dataBits = _driversIO->GetRSOut(i)->GetNumberBits();
	}
	data.isInverseOff = _driversIO->GetADCControl()->IsInverseOff() ? 1 : 0;
	_drivers->GetUSB()->WriteData(&data, sizeof(ChannelsSettings));
}

void Diagnostic::UnpackChannelsSettings(ChannelsSettings* data)
{
	for(UINT i = 0;i < DriversIOManager::analogInCount; i++)
		if (data->analog[i].type == (UINT)AnalogIn::TYPE::NPT)
		{
			_driversIO->GetAnalogIn(i)->SetType(AnalogIn::TYPE::NPT);
			_driversIO->GetAnalogIn(i)->SetSyncNumber(0);
			_driversIO->GetAnalogIn(i)->SetGainCoefficient((AnalogIn::GAIN_COEFFICIENTS)data->analog[i].analogType);
		}
		else if(data->analog[i].type == (UINT)AnalogIn::TYPE::Selsin)
		{
			_driversIO->GetAnalogIn(i)->SetAngleType(AnalogIn::ANGLE_TYPE::Selsin);
			_driversIO->GetAnalogIn(i)->SetDelay(data->analog[i].delay);
			_driversIO->GetAnalogIn(i)->SetSyncNumber(data->analog[i].syncNum);
		}
		else if(data->analog[i].type == (UINT)AnalogIn::TYPE::Logometr)
		{
			_driversIO->GetAnalogIn(i)->SetAngleType(AnalogIn::ANGLE_TYPE::Logometr);
			_driversIO->GetAnalogIn(i)->SetDelay(data->analog[i].delay);
			_driversIO->GetAnalogIn(i)->SetSyncNumber(data->analog[i].syncNum);
		}
		else if(data->analog[i].type == (UINT)AnalogIn::TYPE::SCT)
		{
			_driversIO->GetAnalogIn(i)->SetAngleType((AnalogIn::ANGLE_TYPE)data->analog[i].analogType);
			_driversIO->GetAnalogIn(i)->SetDelay(data->analog[i].delay);
			_driversIO->GetAnalogIn(i)->SetSyncNumber(data->analog[i].syncNum);
		}
	_driversIO->GetCalibrate()->SetGainCoefficient((AnalogIn::GAIN_COEFFICIENTS)data->calibrate.analogType);
	for(UINT i = 0;i < DriversIOManager::SCTOutCount; i++)
	{
		_driversIO->GetSCTOut(i)->SetType((SCTOut::SCTType)data->sct[i].type);
		_driversIO->GetSCTOut(i)->SetPhaseShift(data->sct[i].phase);
	}
	for(UINT i = 0;i < DriversIOManager::arincInCount; i++)
	{
		_driversIO->GetArincIn(i)->SetRate((Arinc::Rate)data->arincIn[i].rate);
		_driversIO->GetArincIn(i)->SetParity((Arinc::ParityIn)data->arincIn[i].parity);
	}
	for(UINT i = 0;i < DriversIOManager::arincOutCount; i++)
	{
		_driversIO->GetArincOut(i)->SetRate((Arinc::Rate)data->arincOut[i].rate);
		_driversIO->GetArincOut(i)->SetParity((Arinc::ParityOut)data->arincOut[i].parity);
		_driversIO->GetArincOut(i)->ClearFifo();
	}
	for(UINT i = 0;i < DriversIOManager::rsInCount; i++)
	{
		_driversIO->GetRSIn(i)->SetBaudRate((RS::BaudRate)(data->rsIn[i].rate));
		_driversIO->GetRSIn(i)->SetParityType((RS::ParityType)data->rsIn[i].parity);
		_driversIO->GetRSIn(i)->SetStopBits(data->rsIn[i].stopBits);
		_driversIO->GetRSIn(i)->SetNumberBits(data->rsIn[i].dataBits);
	}
	for(UINT i = 0;i < DriversIOManager::rsOutCount; i++)
	{
		_driversIO->GetRSOut(i)->SetBaudRate((RS::BaudRate)(data->rsOut[i].rate));
		_driversIO->GetRSOut(i)->SetParityType((RS::ParityType)data->rsOut[i].parity);
		_driversIO->GetRSOut(i)->SetStopBits(data->rsOut[i].stopBits);
		_driversIO->GetRSOut(i)->SetNumberBits(data->rsOut[i].dataBits);
	}
	_driversIO->GetADCControl()->SetInverseOff(data->isInverseOff == 1);
}

void Diagnostic::FillDynamicData()
{
	DynamicData data;
	data.marker = 0xb3897c68;
	data.datetime = _drivers->GetClock()->GetCurrentTime();
	data.perfomanceIrq = _drivers->GetPerfomance()->GetLoadIrq();
	data.perfomanceBg = _drivers->GetPerfomance()->GetLoadBg();
	data.perfomanceLogic = _drivers->GetPerfomance()->GetLoadLogic();
	data.perfomanceDiag = _drivers->GetPerfomance()->GetLoadDiag();
#ifndef EMULATION
	data.temperature = _drivers->GetTermometr()->GetCurrentTemperature();
#endif
	_drivers->GetUSB()->WriteData(&data, sizeof(DynamicData));
}

void Diagnostic::FillFailsData()
{
	FailsData data;
	data.marker = Markers::Fails;
	data.count = _workManager->GetFailManager()->GetFailsCount();
	data.currentFailIndex = _workManager->GetFailManager()->GetCurrentFail();
	for(UINT i = 0; i < data.count; i++)
	{
		data.fails[i].chData = _workManager->GetFailManager()->GetFails()[i].chData;
		auto st = _workManager->GetFailManager()->GetFails()[i].status;
		data.fails[i].status = (st.first % 0xFF) | ((st.second % 0xFF) << 8) | ((st.third % 0xFF) << 16) | ((st.fourth % 0xFF) << 24);
	}
	_drivers->GetUSB()->WriteData(&data, data.count * 8 + 4 + 4 + 4);
}

DISPATH_RESULT Diagnostic::ProcessFlag(Flags id)
{
	_drivers->GetPerfomance()->DiagStart();
	DISPATH_RESULT result = drDispatched;
	switch(id)
	{
	case Flags::SendDiagnostic:
		switch(_mode)
		{
		case DiagnosticMode::NormalDiagnostic:
			FillDiagDataAnalog();
			_drivers->GetUSB()->WriteData(GetDiagDataAnalog(), sizeof(Diagnostic::DiagDataAnalog));
			FillDiagDataDigital();
			_drivers->GetUSB()->WriteData(GetDiagDataDigital(), GetDiagDataDigital()->count*4);
			_drivers->GetGpio()->SetLED(Gpio::LEDS::L3_2, _diagFlashState);
			_diagFlashState = !_diagFlashState;
			break;
		case DiagnosticMode::Oscillograph:
			_drivers->GetUSB()->WriteData(_driversIO->GetADCControl()->GetOscillographData(), sizeof(ADCControl::OscillographData));
			_drivers->GetGpio()->SetLED(Gpio::LEDS::L3_3, _oscillFlashState);
			_oscillFlashState = !_oscillFlashState;
			break;
		default:
			result = drNotDispatched;
			break;
		}
		break;
	case Flags::CommandPCReceived:
	{
		Commands cmd = (Commands)_drivers->GetUSB()->GetCommand();
		switch(cmd)
		{
		case Commands::VersionRequest:
			FillDiagVersion();
			_drivers->GetUSB()->WriteData(&_diagVersion, sizeof(VersionData));
			break;
		case Commands::LoadFW:
			FillCommandResponse(cmd);
			_drivers->GetUSB()->WriteData(&_commandResponse, sizeof(CommandResponse));
			_drivers->GetFWLoader()->LoadFW();
			break;
		case Commands::SaveCM:
		{
			UINT size = _commod->GetConfigSize();
			if (size > 0 && size < 64*1024)
			{
				char* buffer = new char[size+4];
				DWORD* marker = reinterpret_cast<DWORD*>(buffer);
				*marker = 0xC077770D;
				_commod->ReadConfig(&buffer[4]);
				_drivers->GetUSB()->WriteData(buffer, size+4);
				delete[] buffer;
			} else
				_drivers->GetUSB()->SendAnswer(UsbTr::PACKET_RESULT::Break);
		}
		break;
		case Commands::DiagnosticEnable:
			_mode = DiagnosticMode::NormalDiagnostic;
			_driversIO->GetADCControl()->DisableOscillograph();
			break;
		case Commands::DiagnosticDisable:
			_mode = DiagnosticMode::Disabled;
			_driversIO->GetADCControl()->DisableOscillograph();
			break;
		case Commands::Configuration:
			FillConfigurationData();
			break;
		case Commands::GetChannelSettings:
			FillChannelsSettings();
			break;
		case Commands::GetDynamicInfo:
			FillDynamicData();
			break;
		case Commands::GetFails:
			FillFailsData();
			break;
		case Commands::Reboot:
			_drivers->GetDisplay()->SetTextByText("ПРЗГ");
			for(;;);
			break;
		case Commands::StartCalibrate:
			_workManager->GetCalibrationManager()->StartCalibrationIn();
			break;
		case Commands::GetCalibrate:
		{
			BYTE data[sizeof(DriversIOManager::Calibration) + 4]; // Маркер + номер блока
			*reinterpret_cast<DWORD*>(&data) = Markers::Calibrate;
			memcpy(&data[4], &_workManager->GetCalibrationManager()->GetCalibration(), sizeof(DriversIOManager::Calibration));
			_drivers->GetUSB()->WriteData(data, sizeof(DriversIOManager::Calibration) + 4);
		}
			break;
		default:
			result = drNotDispatched;
			break;
		}
	}
		break;
	case Flags::DataPCReceived:
	{
		UINT size = _drivers->GetUSB()->GetPacketSize();
		BYTE data[64*1024];
		_drivers->GetUSB()->ReceiveData(data);
		DWORD marker = *reinterpret_cast<DWORD*>(data);
		switch(marker)
		{
		case ChannelSettings:
			UnpackChannelsSettings(reinterpret_cast<ChannelsSettings*>(data));
			break;
		case EnableOscillograph:
		{
			DWORD channelNum = *reinterpret_cast<DWORD*>(&data[4])-1;
			DWORD koeff = *reinterpret_cast<DWORD*>(&data[8]);
			_driversIO->GetADCControl()->EnableOscillograph(channelNum, (AnalogIn::GAIN_COEFFICIENTS)koeff);
			_mode = DiagnosticMode::Oscillograph;
		}
		break;
		case RegistersRequest:
		{
			if (_workManager->GetConfiguration()->IsInitialized())
			{
				DWORD count = (*reinterpret_cast<DWORD*>(&data[4]) & 0xF);
				for(UINT i = 0; i < count; i++)
				{
					DWORD patternNum = *reinterpret_cast<DWORD*>(&data[8+i*12]);
					IOTYPES channelType = *reinterpret_cast<IOTYPES*>(&data[12+i*12]);
					DWORD channelNum = *reinterpret_cast<DWORD*>(&data[16+i*12]);
					FillRegisterData(patternNum, channelType, channelNum);
				}
			}
		}
		break;
		case SetRegister:
		{
			if (_workManager->GetConfiguration()->IsInitialized())
			{
					DWORD patternNum = data[4];
					IOTYPES channelType = (IOTYPES)data[5];
					DWORD channelNum = data[6];
					DWORD regId = data[7];
					DWORD isEmulated = *reinterpret_cast<DWORD*>(&data[8]);
					auto pattern = _workManager->GetConfiguration()->GetPatternByDeviceNum(patternNum);
					if (pattern != NULL)
					{
						IChannel* ch = (IS_IN(channelType) ? (IChannel*)pattern->GetInput(channelType, channelNum) :
													  (IChannel*)pattern->GetOutput(channelType, channelNum));
						if (ch != NULL && regId < (UINT)REGISTER_ID::COUNTREGISTERS && ch->GetRegister((REGISTER_ID)regId) != NULL)
						{
							ch->GetRegister((REGISTER_ID)regId)->SetEmulated(isEmulated == 1);
							if (isEmulated == 1)
							{
								float val  = *reinterpret_cast<float*>(&data[12]);
								ch->GetRegister((REGISTER_ID)regId)->SetEmulatedValue(val);
							}

						}
					}

			}
		}
			break;
		case LoadCM:
		{
			CRC32 crc;
			crc.ProcessCRC(&data[4], size-8);
			DWORD crcR = *reinterpret_cast<DWORD*>(&data[size-4]);
			if (crcR != crc.GetCRC32())
				_drivers->GetUSB()->SendAnswer(UsbTr::PACKET_RESULT::WrongCrc);
			else
			{
				_drivers->GetGpio()->SetCMWriteProtect(false);
				_commod->WriteConfig(reinterpret_cast<char*>(&data[4]), size-8);
				_drivers->GetUSB()->SendAnswer(UsbTr::PACKET_RESULT::Correct);
				_drivers->GetGpio()->SetCMWriteProtect(true);
				_commod->Init();
			}
		}
		break;
		case SetTime:
		{
			Clock::DATE_STRUCT dt;
			Clock::TIME_STRUCT tm;
			tm.sec = data[4];
			tm.min = data[5];
			tm.hour = data[6];
			dt.day = data[7];
			dt.month = data[8];
			dt.year = data[9];
			_drivers->GetClock()->SetDate(dt);
			_drivers->GetClock()->SetTime(tm);
		}
		break;
		case Calibrate:
			if (size-4 == sizeof(DriversIOManager::Calibration))
			{
				if (_workManager->GetCalibrationManager()->SetCalibration(*reinterpret_cast<DriversIOManager::Calibration*>(&data[4])))
				{
					_drivers->GetUSB()->SendAnswer(UsbTr::PACKET_RESULT::Correct);
					break;
				}
			}
			_drivers->GetUSB()->SendAnswer(UsbTr::PACKET_RESULT::WrongCrc);
			break;
		case DataByAddressRequest:
		{
			DWORD address =  *reinterpret_cast<DWORD*>(&data[size-4]);
			DWORD data[2];
			data[0] =DataByAddressRequest;
			data[1] = UCU_IORD_32DIRECT(address, 0);
			_drivers->GetUSB()->WriteData(&data, 8);
		}
			break;
		default:
			result = drNotDispatched;
			break;
		}
	}
		break;
	default:
		result = drNotDispatched;
		break;
	}
	_drivers->GetPerfomance()->DiagStop();
	return result;
}

