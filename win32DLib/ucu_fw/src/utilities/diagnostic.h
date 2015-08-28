/*
 * diagnostic.h
 *
 *  Created on: 07 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef DIAGNOSTIC_H_
#define DIAGNOSTIC_H_
#include "../driversio/ichannel.h"
#include "../types.h"
#include "../interfaces/idispatchflags.h"


class DriverManager;
class DriversIOManager;
class Commod;
class WorkManager;

class Diagnostic : public IDispatchFlags
{


public:

	enum class DiagnosticMode
	{
		Disabled,
		NormalDiagnostic,
		Oscillograph,
	};

	enum Commands
	{
		VersionRequest = 1,
		LoadFW = 2,
		SaveCM = 4,  // Выгрузка КМ в ПК
		DiagnosticEnable = 5, // Включить диагностику
		DiagnosticDisable = 6, // Выключить диагностику
		Configuration = 7, // Запрос конфигурации
		GetChannelSettings = 8, // Запрос настроек каналов
		GetFails = 9, // Запрос отказов
		GetDynamicInfo = 10, // Запрос динамической информации
		Reboot = 11, // Перезагрузка блока
		StartCalibrate = 12, // Калибровать блок
		GetCalibrate = 13, // Получить текущие калибровки
	};

	enum Markers
	{
		ChannelSettings = 0x1C33C293,
		EnableOscillograph = 0x608FDEB4,
		LoadCM = 0xC077770D,
		SetTime = 0x5E11177E,
		RegistersRequest = 0xbaa1c357,
		SetRegister = 0x556b25d8,
		ConfigurationId = 0x169cee22,
		Calibrate = 0xdd3126d7,
		Fails = 0xe3a5d338,
		DataByAddressRequest = 0xd66e0eb1,
	};

	struct VersionData
	{
		UINT marker;
		DWORD FWDate;
		DWORD PLDDate;
		DWORD blockNumber;
		DWORD FWVersion;
		DWORD FWCS;
	};

	struct DiagDataAnalog
	{
		UINT marker;
		UINT packetNum;
		float adcValue[13];
		float adcSCTControlValue[6];
		float adcNPTControlValue[3];
		WORD adcChannel1[13];
		WORD adcSCTControlChannel1[6];
		WORD adcChannel2[13];
		WORD adcSCTControlChannel2[6];
		WORD adcChannel3[13];
		WORD adcNPTControlChannel3[3];
		DWORD adcFails[13];
		float potValue[6];
		DWORD potChannel[6];
		DWORD potFails[6];
		float SCTOutValue[6];
		WORD SCTOutChannel1[6];
		WORD SCTOutChannel2[6];
		DWORD SCTOutFails[6];
		float NPTOutValue[3];
		WORD NPTOutChannel1[3];
		WORD reserv;  // Для выравнивания по 4
		DWORD NPTOutFails[3];
		WORD calibrateChannels[4];
		DWORD descreeteInput;
		DWORD descreeteOutput;
		DWORD mainAdcFail;
		DWORD mainAdcMeasureTry;
		DWORD mainAdcMeasureWritten;
		BYTE mainAdcMeasureCount[24]; // количество прочитанных каналов для каждого датчика (при нормальной работе должен быть равен 4).
		BYTE mainAdcCnvst;
		BYTE mainAdcClken;
		BYTE mainAdcWrchn;
		BYTE reserv1;
		DWORD endMarker;
	};

	// Динамический пакет
	// Набор COUNT + DATA * COUNT
	// count в конце, количество DWORD включая маркер (отправляемый размер)
	struct DiagDataDigital
	{
		UINT marker;
		UINT packetNum;
		UINT currentTime;
		DWORD data[6 * 257 + 4 * 257 + 4 * 129 + 4 * 129 +1];
		UINT count;
	};

	struct AnalogChannelSetting
	{
		UINT type;
		UINT syncNum;
		UINT delay;
		UINT analogType;
	};

	struct ArincChannelSettings
	{
		WORD rate;
		WORD parity;
	};

	struct RsChannelSettings
	{
		DWORD rate;
		BYTE parity;
		BYTE stopBits;
		BYTE dataBits;
		BYTE reserv;
	};

	struct SCTChannelSettings
	{
		WORD type;
		WORD phase;
	};

	struct ChannelsSettings
	{
		UINT marker;
		AnalogChannelSetting analog[13];
		AnalogChannelSetting calibrate;
		SCTChannelSettings sct[6];
		ArincChannelSettings arincIn[6];
		ArincChannelSettings arincOut[4];
		RsChannelSettings rsIn[2];
		RsChannelSettings rsOut[2];
		UINT isInverseOff;
	};

	struct CommandResponse
	{
		UINT marker;
		UINT command;
		UINT allOkMarker;
	};


	struct ConfigurationData
	{
		UINT marker;
		UINT channelsCount;
		IChannel::ChannelData data[13 + 6 + 22 + 2 + 6 + 3 + 2 + 6 * 256 + 4 * 256]; // Список всех возможных каналов в конфигурации
		UINT count;
	};

	struct RegisterData
	{
		BYTE id;
		BYTE type;
		BYTE size;
		BYTE isFilled;
		BYTE registerType;
		BYTE isEmulated;
		WORD reserv;
		BYTE data[256];
	};

	struct ChannelRegistersData
	{
		UINT marker;
		UINT irqNum;
		BYTE channelPattern;
		BYTE channelType;
		BYTE channelNum;
		BYTE count;
		BYTE data[sizeof(RegisterData) * (UINT)REGISTER_ID::COUNTREGISTERS];
	};

	struct DynamicData // Время загрузка
	{
		UINT marker;
		UINT datetime;
		float perfomanceIrq;
		float perfomanceBg;
		float perfomanceLogic;
		float perfomanceDiag;
		float temperature;
	};


	struct FailInfo
	{
		IChannel::ChannelData chData;
		UINT status;
	};

	struct FailsData
	{
		UINT marker;
		UINT count;
		UINT currentFailIndex;
		FailInfo fails[256];
	};



private:
	DriversIOManager* _driversIO;
	DriverManager* _drivers;
	Commod* _commod;
	WorkManager* _workManager;

	DiagDataAnalog _diagDataAnalog;
	DiagDataDigital _diagDataDigital;
	VersionData _diagVersion;
	CommandResponse _commandResponse;
	UINT _packetNumAnalog;
	UINT _packetNumDigital;

	DiagnosticMode _mode;
	bool _diagFlashState;
	bool _oscillFlashState;

	void UnpackChannelsSettings(ChannelsSettings* data);


public:
	Diagnostic(WorkManager* workManager);
	virtual ~Diagnostic();

	UINT GetCurrentPacket() { return _packetNumAnalog; }

	void FillDiagDataAnalog();
	DiagDataAnalog* GetDiagDataAnalog() { return &_diagDataAnalog; }
	void FillDiagDataDigital();
	DiagDataDigital* GetDiagDataDigital() { return &_diagDataDigital; }
	void FillDiagVersion();
	void FillCommandResponse(Commands cmd);
	void FillConfigurationData();
	void FillRegisterData(UINT patternNum, IOTYPES type, UINT num);
	void FillChannelsSettings();
	void FillDynamicData();
	void FillFailsData();


	DISPATH_RESULT ProcessFlag(Flags id);

};

#endif /* DIAGNOSTIC_H_ */
