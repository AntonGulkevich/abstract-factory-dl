#include "CParser.h"

#include <stdio.h>

#include "../drivers/gpio.h"
#include "../utilities/console.h"
#include "../utilities/commod.h"
#ifdef CPARSER_H

#include <stdarg.h>

//#define _PARSER_TRACE
//#define _TRACE_SCANNER


const char* registers[] =
{
	"SPEED",
	"NAME",
	"VERSION",
	"VALUE",
	"STATE",
	"MATRIX",
	"TYPE",
	"DELAY",
	"SYNCNUM",
	"PERIODMAX",
	"PERIODMIN",
	"CONTROL",
	"ID",
    "DELTA",
    "FILTRBASE",
	"MAXLEVEL",
	"MINLEVEL",
	"UNIT",
	"VALUE1",
	"VALUE2",
	"VALUE3",
	"GROUP",
	"TARMIN",
	"TARMAX",
	"PASSPORT",
	"INTERVAL",
	"DEADZONE",
	"CORRECTIONDELTA",
	"CORRECTIONSIGN",
	"SUBGROUP",
	"FILTRTYPE",
	"RTCNUM",
	"RTCVAL",
	"PARITYTYPE",
	"STOPBIT",
	"DATABASENUM",
	"DATAARRAY",
	"RATE",
	"RATEBASE",
	"SETTINGSNUM",
	"BITSCOUNT",
	"RANGE",
	"REVERSE",
	"NOTFILTREDVALUE",
	"FILTRREADY",
	""
};

const char* functions[] =
{
	"TOBNR",
	"FROMBNR",
	"TOBCD",
	"FROMBCD",
	"LINEAR",
	"SHOWALARM",
	"SETBIT",
	"GETBIT",
	"SETFIELD",
	"GETFIELD",
	"GETUCUSTATE",
	"RESETRATE",
	"RESETRATECHANNEL",
	"ISRATEREADY",
	"RESETFILTR",
	"TORTM",
	"FROMRTM",
	"AERODYNAMICCORR",
	"SUPPRESSALARM",
	"SIN",
	"COS",
	"ISFILTRREADY",
	"TOBNRS",
	"FROMBNRS",
	"TORTMS",
	"FROMRTMS",
	"TRUNC",
	"FRAC",
	""
};


const char* CParser::keyWords[] =
{
	"ANGLE",
	"DIGITALIN",
	"CHECK",
	"COMMAND",
	"else",
	"END",
	"FREQUENCY",
	"GENERAL",
	"if",
	"INPUTS",
	"LOGICALIN",
	"NPT",
	"OUTPUTS",
	"POTENTIOMETR",
	"RSIN",
	"DEVIN",
	"DEVOUT",
	"IN",
	"OUT",
	"COUNT",
	"CONNECTIONS",
	"NC",
	"ANALOG",
	"DIGITALOUT",
	"LOGICALOUT",
	"DEVCONSTIN",
	"DEVCONSTOUT",
	"RSOUT",
	""
};

CParser::CParser(Console* console, Commod* commod) : _console(console), _commod(commod)
{
	curCh = 0;
	stackPtr = 0;
	breakCount = 0;
	curIO = 0xFF;
}

CParser::~CParser()
{
	for(UINT i = 0; i < str.size(); i++)
		delete[] str[i];
}


short CParser::IndexOfList(const char* llist[])
{
	for (int i = 0; strlen(llist[i]) != 0; i++)
		if (strcmp(llist[i], stack) == 0)
			return i;
	return -1;
}

short CParser::IndexOfId(char* str)
{
	std::string lStr(str);
	for (UINT i = 0; i < id.size(); i++)
		if (id[i] == lStr)
			return i;
	return -1;
}

void CParser::Push(char ch)
{
	if (stackPtr < 1023)
		stack[stackPtr++] = ch;
	else {
		if (stack[0] == '/' && stack[1] == '*')
		{
			stack[2] = stack[1022];
			stack[3] = stack[1023];
			stackPtr = 4;
		}else
			throw;
	}
}


int CParser::PopSeparator()
{
	if(stackPtr > 1023)
		throw;

	stack[stackPtr] = 0;
	int res = DefineSeparator(stack);
	stackPtr = 0;
	return res;
}

int CParser::PopString()
{
	if(stackPtr > 1023 || stackPtr < 1)
		throw;

	stack[stackPtr - 1] = 0;
	WORD size = strlen(&stack[1])+1;
	char* lStr = new char[size];
	strcpy(lStr,&stack[1]);
	lStr[size-1] = 0;
	str.push_back(lStr);
	stackPtr = 0;
	return 0;
}

int CParser::PopDec()
{
	int value = 0;

	if(stackPtr > 1023)
		throw;
	stack[stackPtr] = 0;
	sscanf(stack, "%d", &value);
	stackPtr = 0;
	numDec = (UINT)value;
	return 0;
}

int CParser::PopFloat()
{
	if(stackPtr > 1023)
		throw;
	stack[stackPtr] = 0;

	float value = 0;
	sscanf(stack, "%f", &value);
	stackPtr = 0;
	numFloat = value;
	return 0;
}

int CParser::PopId(TOKENCLASS &tc)
{
	if(stackPtr > 1023)
		throw;
     stack[stackPtr] = 0;
     int index;

     if ((index = IndexOfList(keyWords)) != -1)   // �������� �����
          tc = tcKeyWord;
     else if ((index = IndexOfList(registers)) != -1)     // �������
          tc = tcRegister;
     else if ((index = IndexOfList(functions)) != -1)     // �������
          tc = tcFunction;
     else
     {               // �������������
          tc = tcId;
          if ((index = IndexOfId(stack)) == -1)
          {
			  id.push_back(std::string(stack));
               index = id.size() - 1;
          }     
     }
     stackPtr = 0;
     return index;
}

CParser::TOKEN CParser::AddToken(TOKENCLASS tc, BYTE data)
{
	TOKEN t;
	t.tc = tc;
	t.data = data;
	t.dec = numDec;
	t.flo = numFloat;
	if (tc == tcString)
		t.str = str.back();
	else
		t.str = NULL;
#ifdef _UCU_EMULATION		
	t.strNum = curStr + 1;
	t.symNum = curSym;
#endif
	return t;
}

void CParser::PrintTokenFlow()
{
#ifdef _TRACE_SCANNER
	char text[100];
	for (UINT i = 0; i < tokens.size(); i++)
	{
		switch (tokens[i].tc)
		{
		case tcId:
			sprintf(text, "%s", id[tokens[i].data].c_str());
			break;
		case tcDec:
			sprintf(text, "N%d", tokens[i].data);
			break;
		case tcFloat:
			sprintf(text, "F%d", tokens[i].data);
			break;
		case tcString:
			sprintf(text, "\"%s\"", str[tokens[i].data].c_str());
			break;
		case tcKeyWord:
			sprintf(text, "%s", keyWords[tokens[i].data]);
			break;
		case tcRegister:
			sprintf(text, "%s", registers[tokens[i].data]);
			break;
		case tcFunction:
			sprintf(text, "%s()", functions[tokens[i].data]);
			break;
		case tcSeparator:
			sprintf(text, "S%d", tokens[i].data);
			break;
		default:
			_console->WriteLine("��������� �����");
		}
		_console->WriteLine(text);
	}
#endif
}


void CParser::Clear()
{
// �������
	stackPtr = 0;
	id.clear();
#ifdef _UCU_EMULATION		
	curStr = 0;
	curSym = 0;
#endif
// ������
	curIO = 0xFF;
	curIOType = ioDigitalIn;
	tokenStack.clear();
	existWarning = 0;

}

int CParser::LoadConfiguration()
{
    try
    {
        breakCount = 0;
    	Clear();
    	if (StartParsing() != -1)
    		return WriteResult(Parse());

    } catch(...)
    {
        _console->WriteLine("���������� ��� ������� ������������");
    }
    return -1;
}

int CParser::StartParsing()
{
	if (_commod->OpenDevice(device) == -1)
	{
		_console->Format("���������� ������� ���������� %d\r\n", device);
		return -1;
	}
	curCh = _commod->GetChar();
#ifdef _UCU_EMULATION	
	curSym = 1;
	curStr = 0;
#endif
    return 0;
}

CParser::TOKEN CParser::GetNextToken()
{
	TOKEN res;
	if (_commod->Eof())
	{
		res.tc = tcEnd;
		return res;
	}
	do
	{
		short state = 0;
		while (GetNextScannerState(state, curCh))
		{			
			Push(curCh);            
			state = GetNextScannerState(state, curCh);
			if (_commod->Eof() || (state == -1))
				break;
			curCh = _commod->GetChar();
		#ifdef _UCU_EMULATION		
			curSym++;
			if (curCh == '\n')
			{
				curStr++;
				curSym = 0;
			}
		#endif
		}
		res = ReduceSym(state);
		Gpio::SVSet();
		if (res.tc == tcNoPrint && _commod->Eof())
			res.tc = tcEnd;
	}
	while(res.tc == tcNoPrint);

	if(res.tc == tcError && !_commod->Eof())
	{
		_console->Write("������ � ������ ������ '");
		char ch2[2];
		ch2[0] = curCh;
		ch2[1] = 0;
		_console->Write(ch2);
		_console->WriteLine("'");
	}    
    /*if (manager.drivers.GetKeys().CheckLoadBreak())
        manager.GetConfig().BreakLoad();*/
	return res;
}

int CParser::Parse()
{
	short state = 0;
	int k = 0; // ������ ��� �������� �� �����
	int existWarning = 0;
	TOKEN t;
	TOKEN curT = GetNextToken();
	while (curT.tc != tcError && curT.tc != tcEnd)
	{
		if (k > 0)
		{
			t = tokenStack[tokenStack.size() - k];
			k--;
		}
		else
			t = curT;
		state = GetNextParserState(state, t);
		if (state < 0)
		{
			_console->Write("\x1B[0;7;31m");
			_console->Write("Error: ");
			_console->Write("\x1B[0;0;30;47m");
			_console->Write("����������� ����� ");
			switch(t.tc)
			{
			case CParser::tcDec:
				_console->FormatLine("%d �� ������� ������", t.dec);
				break;
			case CParser::tcFloat:
				_console->FormatLine("%f �� ������� ������", t.flo);
				break;
			case CParser::tcNoTerminal:
				_console->FormatLine("���������� %d �� ������� ������", t.data);
				break;
			case CParser::tcSeparator:
				_console->FormatLine("����������� %d �� ������� ������", t.data);
				break;
			default:
				_console->FormatLine("%s �� ������� ������", GetIdName(t));
			}
#ifdef _UCU_EMULATION   			
            TRACEUCU("������ %d, ������ %d\r\n", t.strNum, t.symNum);
#endif
			return prError;
		}
		if (state < 1000)  // ��������� �����
		{
			if (k == 0)
			{
				tokenStack.push_back(t);
				curT = GetNextToken();
			}
		}
		else if (state < 2000) // ������� �������
		{
			state -= 1000;
			int err = ReduceRule(state); 
			if (err < 0)
			{
#ifdef _UCU_EMULATION	
				t = curT;
				TRACEUCU("������ %d, ������ %d\r\n", t.strNum, t.symNum);
#endif
				return prError;
			}
			// ����� ������� ������� ������������� state 0 � �������� ������ ������ ��������� ����
			state = 0;
			k = tokenStack.size();
		}
		else if (state < 3000) // ��������� �� ���������
		{
			state -= 2000;
			t = curT;
		}
		
	}
    _commod->Close();
	if (existWarning || curT.tc == tcError)
		return prError;
	else
	{
		if (tokenStack.back().tc != tcKeyWord || tokenStack.back().data != 5)
		{
			_console->Write("\x1B[0;7;31m");
			_console->Write("Error: ");
			_console->Write("\x1B[0;0;30;47m");
			_console->Write("�� ������� �������� ����� END, ����������� ����� �����");
			return prError;
		} else
			return prOk;
	}
}

int CParser::WriteResult(int res)
{
	switch(res)
	{
	case prOk:
		if (existWarning)
		{
			_console->Write("\x1B[1;7;31;47m");
			_console->Write("Critical Warning: ");
			_console->Write("\x1B[0;0;30;47m");
			_console->FormatLine("������������ ���������� %d ��������� c ��������", device);
			return -1;
		}
		else
		{
			_console->Format("������������ ���������� %d ������� ���������\r\n", device);
			return 0;
		}
		break;
	case prError:
		_console->Write("\x1B[0;7;31m");
		_console->Write("Error: ");
		_console->Write("\x1B[0;0;30;47m");
		_console->Format("������ �������� ������������ ���������� %d\r\n", device);
		return -1;
	default:
		return -1;
	}
}

const char* CParser::GetTypeStr(IOTYPES type)
{
	switch(type)
	{
	case ioDigitalIn:
		return "DIGITALIN";
	case ioDigitalOut:
		return "DIGITALOUT";
	case ioNPT:
		return "NPT";
	case ioPotentiometr:
		return "POTENTIOMETR";
/*	case ioFrequency:
		return "FREQUENCY";*/
	case ioRSIn:
		return "RSIN";
    case ioRSOut:
        return "RSOUT";
	case ioCommand:
		return "COMMAND";
	case ioCheck:
		return "CHECK";
	case ioSCT:
		return "ANGLE";
	case ioAnalog:
		return "ANALOG";
	case ioLogicalIn:
		return "LOGICALIN";
	case ioLogicalOut:
		return "LOGICALOUT";
	case ioArincWordIn:
		return "����� ARINC �����";
	case ioArincWordOut:
		return "����� ARINC ������";
	default:
		return "";
	}
}

const char* CParser::GetTypeStrRus(IOTYPES type)
{
	switch(type)
	{
	case ioDigitalIn:
		return "������� ����";
	case ioDigitalOut:
		return "������� �����";
	case ioNPT:
		return "���";
	case ioPotentiometr:
		return "������������";
	/*case ioFrequency:
		return "����������";*/
	case ioRSIn:
		return "RS ����";
    case ioRSOut:
        return "RS �����";
	case ioCommand:
		return "��";
	case ioCheck:
		return "��";
	case ioSCT:
		return "�������";
	case ioAnalog:
		return "����������";
	case ioLogicalIn:
		return "���������� �����";
	case ioLogicalOut:
		return "���������� �����";
	case ioArincWordIn:
		return "����� ARINC �����";
	case ioArincWordOut:
		return "����� ARINC ������";
	default:
		return "";
	}
}

const char* CParser::GetIdName(CParser::TOKEN t)
{
	switch(t.tc)
	{
	case CParser::tcRegister:
		return registers[t.data];
	case CParser::tcFunction:
		return functions[t.data];
	case CParser::tcId:
		return GetIdName(t.data).c_str();
	case CParser::tcString:
		return t.str;
	case CParser::tcKeyWord:
		return keyWords[t.data];
	default:
		return "%%�����%%";
	}
}

void CParser::WriteError(TOKEN t, const char* format, ...)
{
    //_console->Write("\x1B[0;7;31m");
    _console->Write("Error: ");
    //_console->Write("\x1B[0;0;30;47m");
	char buff[256];
	va_list list;
	va_start(list, format);
	vsprintf((char*)buff, format, list);
	va_end(list);
#ifdef _UCU_EMULATION	
	_console->Write(buff);
	console->FormatLine(", ��. %d, ���. %d", t.strNum, t.symNum);
#else
	_console->WriteLine(buff);
#endif

}
void CParser::WriteCriticalWarning(TOKEN t, const char* format, ...)
{
    //_console->Write("\x1B[1;7;31;47m");
    _console->Write("Critical Warning: ");
    //_console->Write("\x1B[0;0;30;47m");
	char buff[256];
	va_list list;
	va_start(list, format);
	vsprintf((char*)buff, format, list);
	va_end(list);
#ifdef _UCU_EMULATION	
	_console->Write(buff);
	console->FormatLine(", ��. %d, ���. %d", t.strNum, t.symNum);
#else
	_console->WriteLine(buff);
#endif

}

void CParser::WriteWarning(TOKEN t, const char* format, ...)
{
    //_console->Write("\x1B[1;7;33;47m");
    _console->Write("Warning: ");
    //_console->Write("\x1B[0;0;30;47m");
	char buff[256];
	va_list list;
	va_start(list, format);
	vsprintf((char*)buff, format, list);
	va_end(list);
#ifdef _UCU_EMULATION	
	_console->Write(buff);
	console->FormatLine(", ��. %d, ���. %d", t.strNum, t.symNum);
#else
	_console->WriteLine(buff);
#endif

}


#endif

