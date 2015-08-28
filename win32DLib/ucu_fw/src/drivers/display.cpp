#include <cstring>
#include "../types.h"
#include "../utilities/m_math.h"
#include "display.h"

#ifdef _UCU_EMULATION
#ifndef _PC_Compilier
#include "../../ucudlg.h"
#endif
#endif
BYTE Display::digitTable[16] =
{
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
};


void Display::Init()
{
	curZero = 0;
	points[0] =points[1] =points[2] =points[3] = false;
	Hide();
}

#ifdef _UCU_EMULATION

void Display::Show()
{
#ifndef _PC_Compilier
	for(int i = 0; i < 4; i++)
	{
		mainDlg->lamel[i] = value[3 - i];
		mainDlg->lamelPoints[i] = points[i];
	}
	CDC *dc = mainDlg->st_lamel.GetWindowDC();
	mainDlg->DrawLamel(dc);
#endif
}

void Display::Hide()
{
#ifndef _PC_Compilier
	for(int i = 0; i < 4; i++)
	{
		mainDlg->lamel[3 - i] = 0;
		mainDlg->lamelPoints[i] = false;
	}
	CDC *dc = mainDlg->st_lamel.GetWindowDC();
	mainDlg->DrawLamel(dc);
#endif
}

#else

void Display::Show()
{
    WORD val = 0;
	for(UINT i = 0; i < 4; ++i)
	{
		UCU_IOWR_32DIRECT(baseAddress, 0x20 + (i * 4), value[i]);
        if (points[i])
	       val |= 1 << i;

	}
	UCU_IOWR_32DIRECT(baseAddress, 4, val);
}

void Display::Hide()
{
	UCU_IOWR_32DIRECT(baseAddress, 4, 0);
    for(UINT i = 0; i < 4; ++i)
    	UCU_IOWR_32DIRECT(baseAddress, 0x20 + (i * 4), 0);

}

#endif

void Display::SetPoint(bool p1, bool p2, bool p3, bool p4)
{
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	points[3] = p4;
}

void Display::SetTextByValue(int lValue, bool sign)
{
    bool res = false;
	if (lValue >= 16000)
    {
         lValue = 15999;
         res = true;
    } else if (lValue <= -1000)
    {
         lValue = -999;
         res = true;
    }

    value[3] = (lValue < 0 && sign) ? 0x40 : 0x00;
	lValue = m_abs(lValue % 16000);
	if (value[3] == 0)
		value[3] = GetByteByDigit((m_abs(lValue) / 1000) % 16);
	value[0] = GetByteByDigit(lValue % 10);
	value[1] = GetByteByDigit((lValue / 10) % 10);
	value[2] = GetByteByDigit((lValue / 100) % 10);
    points[3] = res;
	Show();
}

void Display::SetTextByText(const char* text, bool clearPoint)
{
	for(int i = 0; i < 4; i++)
		value[3 - i] = GetByteByLetter(text[i]);
	if (clearPoint)
		SetPoint(false, false, false, false);
	Show();
}

void Display::RunningZero()
{
	BYTE text[4];
	memset(text, 0, 4);
	text[curZero] = GetByteByDigit(0);
	SetValue(text);
	Show();
	curZero++;
	if (curZero == 4)
		curZero = 0;
}

void Display::RunningB()
{
	BYTE text[4];
	memset(text, 0, 4);
	text[curZero] = GetByteByDigit(0xb);
	SetValue(text);
	Show();
	curZero++;
	if (curZero == 4)
		curZero = 0;
}

void Display::SetValue(BYTE* text)
{
	for(int i = 0; i < 4; i++)
		value[i] = text[i];
	points[0] = points[1] = points[2] = points[3] = false;
}

BYTE Display::GetByteByDigit(BYTE digit)
{
	if (digit > 15)
		return 0x08;
	else
		return digitTable[digit];
}

BYTE Display::GetByteByLetter(char ch)
{
	switch (ch)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		return GetByteByDigit((BYTE)ch);
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return GetByteByDigit((BYTE)(ch - '0'));
	case 'A':
	case 'a':
	case 'А': // рус
	case 'а': // рус
		return 0x77;
	case 'b':
		return 0x7C;
	case 'Б':
		return 0x7D;
	case 'C':
	case 'С': // рус
		return 0x39;
	case 'c':
	case 'с': // рус
		return 0x58;
	case 'd':
		return 0x5E;
	case 'Г':
		return 0x31;
	case 'г':
	case 'r':
		return 0x50;
	case 's':
	case 'S':
		return 0x6D;
	case 'T':
	case 't':
		return 0x78;
	case 'E':
	case 'e':
	case 'Е': // рус
	case 'е': // рус
		return 0x79;
	case 'F':
	case 'f':
		return 0x71;
	case 'З': // рус
	case 'з': // рус
		return 0x4F;
	case 'L':
	case 'l':
		return 0x38;
	case 'H':
	case 'Н':
	case 'н':
		return 0x76;
	case 'O':
	case 'О': // рус
		return 0x3F;
	case 'o':
	case 'о': // рус
		return 0x5C;
	case 'P':
	case 'p':
	case 'Р': // рус
	case 'р': // рус
		return 0x73;
	case 'П':
		return 0x37;
	case 'п':
	case 'n':
		return 0x54;
	case 'У': //рус
	case 'у': // рус
	case 'Y':
	case 'y':
		return 0x6E;
	case 'U':
		return 0x3E;
	case 'u':
		return 0x1C;
	case 'ч':
	case 'Ч':
		return 0x66;
	case '-':
		return 0x40;
	case '_':
    case (char)0xFF:
		return 0x08;
	case '>':
		return 0x70;
	case '<':
		return 0x46;
	case 16:
		return 0x0E;
	case '+':
		return 0x49;
	case 'h':
		return 0x74;
	default:
		return 0x00;

	}
}
