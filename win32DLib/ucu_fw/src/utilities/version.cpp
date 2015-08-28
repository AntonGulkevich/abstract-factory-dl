/**	\file
 *	\brief This file contains definition of \c version namespace.
 */
#include "version.h"

#include <stdio.h>
#include <cstring>

namespace version
{

#ifdef SYS_RELEASE_BUILD
const char* string()
{
	return "__VERSION_STRING__";
}

const char* hash()
{
	return "__VERSION_HASH__";
}

const char* date()
{
	return "__VERSION_BUILD_DATE__";
}

const char* build()
{
	return "__VERSION_BUILD_NUMBER__";
}
#else
const char* string()
{
	return "none";
}

const char* hash()
{
	return "none";
}

const char dateStr[] =
{
	BUILD_DAY_CH0, BUILD_DAY_CH1, '.',
	BUILD_MONTH_CH0, BUILD_MONTH_CH1, '.',
	BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3, ' ',
    BUILD_HOUR_CH0, BUILD_HOUR_CH1, ':',
    BUILD_MIN_CH0, BUILD_MIN_CH1

};

const char* date()
{
	return dateStr;
}

unsigned int datetime()
{
	unsigned int year = 2000;
	unsigned int min;
	unsigned int hour;
	unsigned int mon;
	unsigned int day;
	DateTime dt;
	sscanf(dateStr, "%02u.%02u.%04u %02u:%02u", &day, &mon, &year, &hour, &min);
	dt.sec = 0;
	dt.min = min;
	dt.hour = hour;
	dt.mon = mon;
	dt.day = day;
	dt.year = year - 2000;
	return dt.dword;
}

const char* build()
{
	return "dev";
}

#endif

}
