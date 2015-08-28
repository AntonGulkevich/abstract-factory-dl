/*
 * ichannel.h
 *
 *  Created on: 01 ���� 2015 �.
 *      Author: Youbrin_A
 */

#ifndef ICHANNEL_H_
#define ICHANNEL_H_
#include "../types.h"
#include "channelcalibration.h"
#include "channelfilter.h"
#include "channelrate.h"
#include "cregister.h"

class CPattern;

class IChannel
{
public:

	enum class CheckConfigurationResult
	{
		OK,
		Error,
	};

	typedef struct _REGISTER_TYPE
	{
		REGISTER_ID id;
		RW rw;
		RTYPE type;
		float min_value;
		float max_value;
		float default_value;
		bool required;		// ���� �������� �� ����������, �� required = 0xFF, ���� ����������, �� = 0 (�������� warning)
		CRegister* reg;
	} REGISTER_TYPE;

	struct ChannelData
	{
		BYTE channelPattern;
		BYTE channelType;
		BYTE channelNum;
		BYTE channelHWNum;
	};


protected:
	REGISTER_TYPE registers_t[(UINT)REGISTER_ID::COUNTREGISTERS];
	IOTYPES _ioType;
	UINT _number;
	CPattern* const _pattern;

	bool _isConfigured; // ������������ ��������� � ������ � ������
	ChannelCalibration* _calibration; // ����� ����� ������������
	ChannelFilter* _filter; // ����� ����� ����������� ��������
	ChannelRate* _rate; // ����� ����� ������� ��������

	void CreateRegisters();
	friend ChannelCalibration;
	friend ChannelFilter;
	friend ChannelRate;

	// ���������� ��� ����������� �������
	UINT _userAlarmCode;
	bool _isSupressAlarm;

public:

	IChannel(CPattern* const pattern);
	virtual ~IChannel();
	UINT GetNumber() { return _number; }
	IOTYPES GetType() { return _ioType; }
	UINT GetPatternIndex(); // ����� ��� �������� ���������� � ������� ����������
	UINT GetPatternDeviceNum(); // ����� ��� �����������

	// ���������� ��������� �� ������� �� ID
	CRegister* GetRegister(REGISTER_ID id) { return (id != REGISTER_ID::NULLID) ? registers_t[(UINT)id].reg : NULL; }
	// ���������� ��������� �� ��������� ��������� �� ID
	const REGISTER_TYPE* GetRegisterT(REGISTER_ID id) { return (id != REGISTER_ID::NULLID) ? &registers_t[(UINT)id] : NULL; }
	// �������� ������������ ������������
	virtual CheckConfigurationResult CheckConfiguration();

	virtual void InitRegisters() = 0;
	virtual void UpdateDataToHW() = 0;
	virtual void UpdateHWToData() = 0;

	bool IsNumberType(IOTYPES type, UINT number) { return _ioType == type && _number == number; }
	bool IsConfigured() { return _isConfigured; }
	bool IsHasCalibration() { return _calibration != NULL; }
	bool IsValueFiltered() { return _filter != NULL; }
	bool IsRateCalculating() { return _rate != NULL; }
	ChannelCalibration* GetChCalibration() { return _calibration; }
	ChannelFilter* GetChFilter() { return _filter; }
	ChannelRate* GetChRate() { return _rate; }

	// ���������� ��� ����������� �������
	void ResetCheckAlarm() { _userAlarmCode = 0xFFFFFFFF; _isSupressAlarm = false; }
	void SetCheckAlarmCode(UINT code);
	UINT GetCheckAlarmCode() { return _userAlarmCode; }
	void SetSuppressAlarm(bool isSupress) { _isSupressAlarm = isSupress; }
	bool GetSuppressAlarm() { return _isSupressAlarm; }

	virtual ChannelData GetChannelIdentificator();
};


#endif /* ICHANNEL_H_ */
