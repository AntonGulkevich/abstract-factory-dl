#ifndef IICHANNEL
#define  IICHANNEL

#include "../types.h"

class IIChannel
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
		bool required;		// если параметр не обязателен, то required = 0xFF, если обязателен, то = 0 (выдается warning)
		CRegister* reg;
	} REGISTER_TYPE;

	struct ChannelData
	{
		BYTE channelPattern;
		BYTE channelType;
		BYTE channelNum;
		BYTE channelHWNum;
	};
	virtual ~IIChannel(){};
	virtual UINT GetNumber() const = 0;
	virtual IOTYPES GetType() const = 0;
	virtual UINT GetPatternIndex() const  = 0;
	virtual UINT GetPatternDeviceNum() const = 0;
	virtual CRegister* GetRegister(REGISTER_ID id) = 0;
	virtual const REGISTER_TYPE* GetRegisterT(REGISTER_ID id) = 0;
	virtual CheckConfigurationResult CheckConfiguration() = 0;
	virtual bool IsNumberType(IOTYPES type, UINT number) const = 0;
	virtual bool IsConfigured() const = 0;
	virtual bool IsHasCalibration() const = 0;
	virtual bool IsValueFiltered() const = 0;
	virtual bool IsRateCalculating() const = 0;
	virtual ChannelCalibration* GetChCalibration() const = 0;
	virtual ChannelFilter* GetChFilter() const = 0;
	virtual ChannelRate* GetChRate() const = 0;

	// Информация для отображения отказов
	virtual void ResetCheckAlarm() = 0;
	virtual void SetCheckAlarmCode(UINT code) = 0;
	virtual UINT GetCheckAlarmCode() const = 0;
	virtual void SetSuppressAlarm(bool isSupress) = 0;
	virtual bool GetSuppressAlarm() const = 0;

	virtual ChannelData GetChannelIdentificator() = 0;
};
#endif
