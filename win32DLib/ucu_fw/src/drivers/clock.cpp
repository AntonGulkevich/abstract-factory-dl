#include "clock.h"
#include "i2c.h"
#include <ctime>

Clock::Clock(I2C* i2c) : _i2c(i2c), isAvaible(false), _currentTime(0)
{
	RegFlag(Flags::Timer1Sec, PRIORITY::BackGround);
	isAvaible = false;
}

void Clock::Init()
{
    isAvaible = _i2c->IsDeviceAvailable();
    ReadChipTime();
}

void Clock::WriteChipTime()
{
	_i2c->Write(&chipTime, 0, sizeof(chipTime));
	ReadChipTime();
}

void Clock::ReadChipTime()
{
	_i2c->Read(&chipTime, 0, sizeof(chipTime));
	ChipTimeToSecond();
}

void Clock::ChipTimeToSecond()
{
	tm time;

	time.tm_sec = chipTime.oneSec + chipTime.decSec * 10;
	time.tm_min = chipTime.oneMin + chipTime.decMin * 10;
	time.tm_hour = chipTime.oneHours + chipTime.decHours * 10;
	time.tm_mday = chipTime.oneDay + chipTime.decDay * 10;
	time.tm_mon = chipTime.oneMonth + chipTime.decMonth * 10 - 1;
	time.tm_year = chipTime.oneYear + chipTime.decYear * 10 + 100;
	_currentTime = mktime(&time);
}

void Clock::SetTime(TIME_STRUCT& time)
{
    chipTime.oneSec = 0xf&(time.sec%10);
    chipTime.decSec = 0x7&(time.sec/10);
    chipTime.on = 0;
    chipTime.oneMin = 0xf&(time.min%10);
    chipTime.decMin = 0x7&(time.min/10);
    chipTime.reserv0 = 0;
    chipTime.oneHours = 0xf&(time.hour%10);
    chipTime.decHours = 0x3&(time.hour/10);
    chipTime.size = 0;
    chipTime.reserv1 = 0;
    chipTime.reserv2 = 0;
    chipTime.reserv3 = 0;
    chipTime.reserv4 = 0;
    chipTime.rs = 0;
    chipTime.reserv5 = 0;
    chipTime.rs_on = 1;
    chipTime.reserv6 = 0;
    chipTime.out = 0;
    WriteChipTime();
}

void Clock::SetDate(DATE_STRUCT& date)
{
    chipTime.on = 0;
    chipTime.reserv0 = 0;
    chipTime.size = 0;
    chipTime.reserv1 = 0;
    chipTime.day = 0x7&(0);
    chipTime.reserv2 = 0;
    chipTime.oneDay = 0xf&(date.day%10);
    chipTime.decDay = 0x3&(date.day/10);
    chipTime.reserv3 = 0;
    chipTime.oneMonth = 0xf&(date.month)%10;
    chipTime.decMonth = 0x1&((date.month)/10);
    chipTime.reserv4 = 0;
    chipTime.oneYear = 0xf&((date.year)%10);
    chipTime.decYear = 0xf&(((date.year)%100)/10);
    chipTime.rs = 0;
    chipTime.reserv5 = 0;
    chipTime.rs_on = 1;
    chipTime.reserv6 = 0;
    chipTime.out = 0;
    WriteChipTime();
}

void Clock::WriteData(void* buff, DWORD address, UINT count)
{
    if (address + count > 56)
        return;

	_i2c->Write(buff, address + 8, count);
}

void Clock::ReadData(void* buff, DWORD address, UINT count)
{
    if (address + count > 56)
        return;

	_i2c->Read(buff, address + 8, count);
}

void Clock::OnPowerDown()
{
	ReadChipTime();
	WriteData(&chipTime, 4, sizeof(chipTime));
	//TRACEUCU("Сбой по питанию\r\n");
}

bool Clock::IsHotStart()
{
	CHIPTIME lct;
	ReadChipTime();
	ReadData(&lct, 4, sizeof(lct));
//	TRACEUCU("Время выключения %u%u:%u%u:%u%u", lct.decHours, lct.oneHours, lct.decMin, lct.oneMin, lct.decSec, lct.oneSec);
	DWORD cur = (chipTime.decHours *10 + chipTime.oneHours) * 3600 + (chipTime.decMin * 10 + chipTime.oneMin) * 60 + (chipTime.decSec * 10 + chipTime.oneSec);
	DWORD last = (lct.decHours *10 + lct.oneHours) * 3600 + (lct.decMin * 10 + lct.oneMin) * 60 + (lct.decSec * 10 + lct.oneSec);
	return ((cur - last) < 10);

}

void Clock::GetDateStr(char* buff, const CHIPTIME* ct)
{
    if (ct == NULL)
    {
        ReadChipTime();
        ct = &chipTime;
    }
	bool dateOk = false;
	for (UINT i = 0; i < 8; i++)
		if (ct->byte[i] != 0xFF && ct->byte[i] != 0)
		{
			dateOk = true;
		}
	if (dateOk)
		sprintf(buff, "%02u.%02u.%04u", ct->oneDay + (ct->decDay << 3) + (ct->decDay << 1),
			ct->oneMonth + (ct->decMonth << 3) + (ct->decMonth << 1),
			2000 + ct->oneYear + (ct->decYear << 3) + (ct->decYear << 1));
	else
		sprintf(buff, "не установлена");

}

void Clock::GetTimeStr(char* buff, const CHIPTIME* ct)
{
    if (ct == NULL)
    {
        ReadChipTime();
        ct = &chipTime;
    }
	bool dateOk = false;
	for (UINT i = 0; i < 8; i++)
		if (ct->byte[i] != 0xFF && ct->byte[i] != 0)
		{
			dateOk = true;
		}
	if (dateOk)
		sprintf(buff, "%02u:%02u:%02u", ct->oneHours + (ct->decHours << 3) + (ct->decHours << 1),
			ct->oneMin + (ct->decMin << 3) + (ct->decMin << 1),
			ct->oneSec + (ct->decSec << 3) + (ct->decSec << 1));
	else
		sprintf(buff, "не установлено");

}

void Clock::GetDateTimeStr(char* buff, const CHIPTIME* ct)
{
    if (ct == NULL)
    {
        ReadChipTime();
        ct = &chipTime;
    }
	bool dateOk = false;
	for (UINT i = 0; i < 8; i++)
		if (ct->byte[i] != 0xFF && ct->byte[i] != 0)
		{
			dateOk = true;
		}
	if (dateOk)
		sprintf(buff, "%02u.%02u.%04u %02u:%02u:%02u", ct->oneDay + (ct->decDay << 3) + (ct->decDay << 1),
			ct->oneMonth + (ct->decMonth << 3) + (ct->decMonth << 1),
			2000 + ct->oneYear + (ct->decYear << 3) + (ct->decYear << 1),
			ct->oneHours + (ct->decHours << 3) + (ct->decHours << 1),
			ct->oneMin + (ct->decMin << 3) + (ct->decMin << 1),
			ct->oneSec + (ct->decSec << 3) + (ct->decSec << 1));
	else
		sprintf(buff, "не установлены");


}

DISPATH_RESULT Clock::ProcessFlag(Flags id)
{
	switch(id)
	{
	case Flags::Timer1Sec:
		_currentTime++;
		return DISPATH_RESULT::drDispatched;
	default:
		break;
	}
	return DISPATH_RESULT::drNotDispatched;
}

