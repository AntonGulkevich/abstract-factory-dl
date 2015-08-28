/*
 * console.cpp
 *
 *  Created on: 01 θώνÿ 2015 γ.
 *      Author: Youbrin_A
 */

#include "console.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdio.h>

#include <string.h>

#include "../types.h"
#include "../drivers/usbtr.h"

UsbTr* Console::_usb = NULL;

void Console::SetUsbTr(UsbTr* usb)
{
	_usb = usb;
}

void Console::SendData(const char* str, bool isNewLine)
{
	if (_usb != NULL)
	{
		UINT len = strlen(str);
		if (len > 1024)
			len = 1024;
		if (isNewLine)
			len +=2;
		UINT marker = 0x0C04501E;
		BYTE buff[1024 + 8];

		memcpy(buff, &marker, 4);
		memcpy(&buff[4], &len, 4);
		memcpy(&buff[8], str, len - (isNewLine ? 2 : 0));
		if (isNewLine)
			memcpy(&buff[len+8-2], "\r\n", 2);
		_usb->WriteData(buff, sizeof(buff));
	}

}

void Console::Write(const char* str)
{
	SendData(str, false);
}

void Console::WriteLine(const char* str)
{
	SendData(str, true);
}

void Console::Format(const char* str, ...)
{
    char buff[2048];
    va_list list;

    va_start(list, str);
    vsprintf((char*)buff, str, list);
    va_end(list);
    SendData(buff, false);
}

void Console::FormatLine(const char* str, ...)
{
    char buff[2048];
    va_list list;

    va_start(list, str);
    vsprintf((char*)buff, str, list);
    va_end(list);
    SendData(buff, true);
}

void Console::Trace(const char* str, ...)
{
    char buff[2048];
    va_list list;

    va_start(list, str);
    vsprintf((char*)buff, str, list);
    va_end(list);
    SendData(buff, false);
}

void Console::TraceLine(const char* str, ...)
{
    char buff[2048];
    va_list list;

    va_start(list, str);
    vsprintf((char*)buff, str, list);
    va_end(list);
    SendData(buff, true);
}

