#include "cparserconfig.h"

#include "../application/cpattern.h"
#include "../application/cconfiguration.h"
#include "../driversio/cregister.h"
#include "../driversiomanager.h"

short CParserConfig::scannerContrTable[17][12] = 
{//		\r	0	.	Sym	"	Dig	/	*	=	;	All Space
	{	1,	2,	3,	4,	6,	10,	11,	-1,	15,	16,	-1,	1},  // State 0
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 1
	{	0,	2,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 2
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 3
	{	0,	5,	0,	5,	0,	5,	0,	0,	0,	0,	0,	0},  // State 4
	{	0,	5,	0,	5,	0,	5,	0,	0,	0,	0,	0,	0},  // State 5
	{	7,	7,	7,	7,	8,	7,	9,	7,	7,	7,	7,	7},  // State 6
	{	7,	7,	7,	7,	8,	7,	7,	7,	7,	7,	7,	7},  // State 7
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 8	
	{	7,	7,	7,	7,	7,	7,	7,	7,	7,	7,	7,	7},  // State 9
	{	0,	10,	0,	0,	0,	10,	0,	0,	0,	0,	0,	0},  // State 10
	{	-1,	-1,	-1,	-1,	-1,	-1,	14,	12,	-1,	-1,	-1,	-1},  // State 11
	{	12,	12,	12,	12,	12,	12,	12,	13,	12,	12,	12,	12},  // State 12
	{	12,	12,	12,	12,	12,	12,	0,	12,	12,	12,	12,	12},  // State 13
	{	0,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14},  // State 14
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 15	
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0}  // State 16	
};

int CParserConfig::DefineSeparator(char* str)
{
	if (strlen(str) == 1)
	{
		if (str[0] == '.') return SYMBOL_POINT;
		if (str[0] == '=') return SYMBOL_EQ;
		if (str[0] == ';') return SYMBOL_SEMI;
	}
	return -1;
}

BYTE CParserConfig::GetSymbolType(char ch)
{
	// Разделители
	if (ch == ' ' || ch == '\t')
		return 11;
	if (ch == '\n' || ch == '\r')
		return 0;
	if (ch == '0') return 1;
	if (ch == '.') return 2;
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_'))
		return 3;
	if (ch == '"') return 4;
	if (ch >= '1' && ch <= '9')
		return 5;
	if (ch == '/') return 6;
	if (ch == '*') 
		return 7;
	if (ch == '=') return 8;
	if (ch == ';') return 9;

	return 10;
}

CParser::TOKEN CParserConfig::ReduceSym(short state)
{
	TOKEN result;
	result.tc = tcError;
	TOKENCLASS tc;
	int data;

	switch(state)
	{
	case 13:
		curCh = ' ';
		PopSeparator();
		result.tc = tcNoPrint;
		break;
	case 0:
	case 1:
	case 14:
		PopSeparator();
		result.tc = tcNoPrint;
		break;
	case 3:
	case 15:
	case 16:
		data = PopSeparator();
		result = AddToken(tcSeparator, (SYMBOLS)data);
		break;
	case 2:
	case 10:
		result = AddToken(tcDec, PopDec());
		break;
	case 5:
	case 4:
		data = PopId(tc);
		result = AddToken(tc, data);
		break;
	case 8:
		result = AddToken(tcString, PopString());
		break;
	}
	return result;
}


#include "CParserConfig_cpp.inc" 

int CParserConfig::ReduceRule(short rule)
{
	for (int i = (int)reduceRuleTable[rule][0] - 1; i >= 0; i--)
	{
		params[i] = tokenStack.back();
		tokenStack.pop_back();
	}
	if (AddAction((RULES)rule) == -1)
		return -rule;
	params[0].data = (SYMBOLS)reduceRuleTable[rule][1];
	params[0].tc = tcNoTerminal;
	tokenStack.push_back(params[0]);
	return 0;
}


int CParserConfig::AddAction(RULES rule)
{
	CRegister* reg;
	switch(rule)
	{
	case RULE_CONFIG___GENSEC_DEVICESSEC_CONNECTIONSSEC_END:
		break;
	case RULE_GENSEC___GENERAL_PROG:
		break;
	case RULE_DEVICESSEC___COUNT_EQ_DECLITERAL_SEMI:
		// Создание COUNT паттернов
		count = params[2].dec;
		break;
	case RULE_CONNECTIONSSEC___CONNECTIONS_CONNECTIONS_:
		break;
	case RULE_CONNECTIONS____CONNECTIONS__CONNECTION:
		break;
	case RULE_CONNECTIONS____CONNECTION:
		break;
	case RULE_CONNECTION___DESTINATION_EQ_NC_SEMI:
		if (dest.dt == dtPattern)
		{
			if(config->GetPattern(IndexOfDevice(dest.patternNum))->AddNCIO(dest.type, dest.pinNum) == CPattern::ADDIO_RESULT::AlreadyExist)
			{
                WriteCriticalWarning(params[0],
					"Вход %s.%d устройства %d уже помечен как неподключенный", GetTypeStr(dest.type), dest.pinNum+1, dest.patternNum);
			}
		}
		break;
	case RULE_CONNECTION___SOURSE_EQ_NC_SEMI:
		if (src.dt == dtPattern)
		{
			if(config->GetPattern(IndexOfDevice(src.patternNum))->AddNCIO(src.type, src.pinNum) == CPattern::ADDIO_RESULT::AlreadyExist)
			{
                WriteCriticalWarning(params[0],
					"Выход %s.%d устройства %d уже помечен как неподключенный", GetTypeStr(src.type), src.pinNum+1, src.patternNum);
			}
		}
		break;
	case RULE_CONNECTION___DESTINATION_EQ_SOURSE_SEMI:
		if (dest.type != src.type && !(dest.type == ioLogicalIn && src.type == ioLogicalOut))
		{
            WriteCriticalWarning(params[0],
			"Тип приемника %s, тип источника %s", GetTypeStr(dest.type), GetTypeStr(src.type));
			existWarning = 1;
		}
		if (!((dest.dt == dtOut && !_driversIO->IsIOExist(dest.type, dest.pinNum)) ||
			(src.dt == dtIn && !_driversIO->IsIOExist(src.type, src.pinNum)) ||
			(dest.dt == dtPatternConst && config->GetPattern(IndexOfDevice(dest.patternNum))->GetInput(dest.type, dest.pinNum) == NULL) ||
			(src.dt == dtPatternConst && config->GetPattern(IndexOfDevice(src.patternNum))->GetOutput(src.type, src.pinNum) == NULL)))
		{
			if (config->AddConnection(IndexOfDevice(src.patternNum), src.type, src.pinNum, IndexOfDevice(dest.patternNum), dest.type, dest.pinNum) == -1)
			{
                WriteCriticalWarning(params[0],
				"Приемник %d.%s.%d или источник %d.%s.%d уже подключен", dest.patternNum, GetTypeStr(dest.type), (dest.patternNum == 255) ? dest.pinNum + 1: dest.pinNum, src.patternNum, GetTypeStr(src.type), (src.patternNum == 255) ? src.pinNum + 1: src.pinNum);
				existWarning = 1;
			}
			if (dest.dt == dtPattern)
			{
				if(config->GetPattern(IndexOfDevice(dest.patternNum))->AddIn(dest.type, dest.pinNum) == CPattern::ADDIO_RESULT::AlreadyExist)
				{
                    WriteError(params[0],
					"Вход %s.%d устройства %d уже существует", GetTypeStr(dest.type), dest.pinNum+1, dest.patternNum);
					return -1;
				}
			}
			if (src.dt == dtPattern)
			{
				if(config->GetPattern(IndexOfDevice(src.patternNum))->AddOut(src.type, src.pinNum) == CPattern::ADDIO_RESULT::AlreadyExist)
				{
                    WriteError(params[0],
					"Выход %s.%d устройства %d уже существует", GetTypeStr(src.type), src.pinNum+1, src.patternNum);
					return -1;
				}
			}
			if (src.dt == dtIn && dest.dt == dtOut)
			{
                WriteError(params[0],
				"Невозможно соединить вход и выход напрямую");
				return -1;
			}		
		}
		break;
	case RULE_SOURSE___IN_POINT_INNAME_POINT_DECLITERAL:
		src.dt = dtIn;
		src.patternNum = 0xFF;
		src.type = curIOType;
		src.pinNum = params[4].dec-1;
		if (!_driversIO->IsIOExist(src.type, src.pinNum))
		{
            WriteCriticalWarning(params[0],
			"Вход %s.%d несуществует", GetTypeStr(src.type), (BYTE)(src.pinNum + 1));
			existWarning = 1;
		}
		break;
	case RULE_SOURSE___DEVOUT_POINT_DECLITERAL_POINT_OUTNAME_POINT_DECLITERAL:
		src.dt = dtPattern;
		src.patternNum = params[2].dec;
		src.type = curIOType;
		src.pinNum = params[6].dec;
		if (IndexOfDevice(src.patternNum) == -1)
		{
			if (deviceNums.size() >= count)
			{			
                WriteError(params[0],
				"Используется больше устройств, чем объявлено в COUNT");
				return -1;	
			}
			else
			{
				config->AddPattern();
				config->GetPattern((BYTE)deviceNums.size())->SetDeviceNum(src.patternNum);
				deviceNums.push_back(src.patternNum);
			}
		}
		break;
	case RULE_SOURSE___DEVCONSTOUT_POINT_DECLITERAL_POINT_OUTNAME_POINT_DECLITERAL:
		src.dt = dtPatternConst;
		src.patternNum = 0xF0 - params[2].dec;
		src.type = curIOType;
		src.pinNum = params[6].dec - 1;
		if (IndexOfDevice(src.patternNum) == -1)
		{
			if (config->GetConstPatterns().CreatePattern(0xF0 - src.patternNum))
			{
				deviceNums.push_back(src.patternNum);
				count++;
			}
			else
			{
                WriteError(params[0],
				"Библиотечное устройство %d не найдено",  0xF0 - src.patternNum);
				return -1;	
			}
		}
		if (config->GetPattern(IndexOfDevice(src.patternNum))->GetOutput(src.type, src.pinNum) == NULL)
		{
            WriteCriticalWarning(params[0],
			"Выход %s.%d библиотечного устройства %d несуществует", GetTypeStr(src.type), (BYTE)(src.pinNum + 1), 0xF0 - src.patternNum);
			existWarning = 1;
		}
		break;
	case RULE_DESTINATION___OUT_POINT_OUTNAME_POINT_DECLITERAL:
		dest.dt = dtOut;
		dest.patternNum = 0xFF;
		dest.type = curIOType;
		dest.pinNum = params[4].dec - 1;
		if (!_driversIO->IsIOExist(dest.type, dest.pinNum))
		{
            WriteCriticalWarning(params[0],
			"Выход %s.%d несуществует", GetTypeStr(dest.type), (BYTE)(dest.pinNum + 1));
			existWarning = 1;
		}
		break;
	case RULE_DESTINATION___DEVIN_POINT_DECLITERAL_POINT_INNAME_POINT_DECLITERAL:
		dest.dt = dtPattern;
		dest.patternNum = params[2].dec;
		dest.type = curIOType;
		dest.pinNum = params[6].dec;
		if (IndexOfDevice(dest.patternNum) == -1)
		{
			if (deviceNums.size() >= count)
			{			
                WriteError(params[0],
				"Используется больше устройств, чем объявлено в COUNT");
				return -1;	
			}
			else
			{
				config->AddPattern();
				config->GetPattern((BYTE)deviceNums.size())->SetDeviceNum(dest.patternNum);
				deviceNums.push_back(dest.patternNum);
			}
		}
		break;
	case RULE_DESTINATION___DEVCONSTIN_POINT_DECLITERAL_POINT_INNAME_POINT_DECLITERAL:
		dest.dt = dtPatternConst;
		dest.patternNum = 0xF0 - params[2].dec;
		dest.type = curIOType;
		dest.pinNum = params[6].dec - 1;
		if (IndexOfDevice(dest.patternNum) == -1)
		{
			if (config->GetConstPatterns().CreatePattern(0xF0 - dest.patternNum))
			{
				deviceNums.push_back(dest.patternNum);
				count++;
			}
			else
			{
                WriteError(params[0],
				"Библиотечное устройство %d не найдено",  0xF0 - dest.patternNum);
				return -1;	
			}
		}
		if (config->GetPattern(IndexOfDevice(dest.patternNum))->GetInput(dest.type, dest.pinNum) == NULL)
		{
            WriteCriticalWarning(params[0],
			"Вход %s.%d библиотечного устройства %d несуществует", GetTypeStr(dest.type), (BYTE)(dest.pinNum + 1), 0xF0 - dest.patternNum);
			existWarning = 1;
		}	
		break;
	case RULE_INNAME___POTENTIOMETR:
		curIOType = ioPotentiometr;
		break;
	case RULE_INNAME___ANALOG:
		curIOType = ioAnalog;
		break;
	case RULE_INNAME___RSIN:
		curIOType = ioRSIn;
		break;
	case RULE_INNAME___FREQUENCY:
		curIOType = ioError;
		break;
	case RULE_INNAME___CHECK:
		curIOType = ioCheck;
		break;
	case RULE_INNAME___LOGICALIN:
		curIOType = ioLogicalIn;
		break;
	case RULE_INNAME___DIGITALIN:
		curIOType = ioDigitalIn;
		break;
	case RULE_OUTNAME___ANGLE:
		curIOType = ioSCT;
		break;
	case RULE_OUTNAME___NPT:
		curIOType = ioNPT;
		break;
	case RULE_OUTNAME___COMMAND:
		curIOType = ioCommand;
		break;
	case RULE_OUTNAME___LOGICALOUT:
		curIOType = ioLogicalOut;
		break;
	case RULE_OUTNAME___DIGITALOUT:
		curIOType = ioDigitalOut;
		break;
	case RULE_OUTNAME___RSOUT:
		curIOType = ioRSOut;
		break;
	case RULE_PROG___PROG_PROGSTEP:
		break;
	case RULE_PROG___PROGSTEP:
		break;
	case RULE_PROGSTEP___ID_EQ_DECLITERAL_SEMI:
		if ((reg = config->GetRegister((REGISTER_ID)params[0].data)) == NULL)
        {
            WriteCriticalWarning(params[0],
			"Регистр %s для секции GENERAL не существует", registers[params[0].data]);
            existWarning = 1;
        }
		else if (config->GetRegisterT((REGISTER_ID)params[0].data)->type == rtString)
        {
            WriteCriticalWarning(params[0],
			"Регистр %s для секции GENERAL является типом string", registers[params[0].data]);
            existWarning = 1;
        }
		else
			reg->SetValue(params[2].dec);

		break;
	case RULE_PROGSTEP___ID_EQ_STRINGLITERAL_SEMI:
		if ((reg = config->GetRegister((REGISTER_ID)params[0].data)) == NULL)
        {
            WriteCriticalWarning(params[0],
			"Регистр %s для секции GENERAL не существует", registers[params[0].data]);
            existWarning = 1;
        }
		else if (config->GetRegisterT((REGISTER_ID)params[0].data)->type != rtString)
        {
            WriteCriticalWarning(params[0],
			"Регистр %s для секции GENERAL является типом", registers[params[0].data]);
            existWarning = 1;
        }
		else
			reg->SetValue(params[2].str);
		break;
	}
	return 0;
}

 CParserConfig::SYMBOLS CParserConfig::GetIdKeyWord(int id)
{
	switch(id)
	{
	case 0:
		return SYMBOL_ANGLE;
	case 1:
		return SYMBOL_DIGITALIN;
	case 2:
		return SYMBOL_CHECK;
	case 3:
		return SYMBOL_COMMAND;
	case 5:
		return SYMBOL_END;
	case 6:
		return SYMBOL_FREQUENCY;
	case 7:
		return SYMBOL_GENERAL;
	case 10:
		return SYMBOL_LOGICALIN;
	case 11:
		return SYMBOL_NPT;
	case 13:
		return SYMBOL_POTENTIOMETR;
	case 14:
		return SYMBOL_RSIN;
	case 15:
		return SYMBOL_DEVIN;
	case 16:
		return SYMBOL_DEVOUT;
	case 17:
		return SYMBOL_IN;
	case 18:
		return SYMBOL_OUT;
	case 19:
		return SYMBOL_COUNT;
	case 20:
		return SYMBOL_CONNECTIONS;
	case 21:
		return SYMBOL_NC;
	case 22:
		return SYMBOL_ANALOG;
	case 23:
		return SYMBOL_DIGITALOUT;
	case 24:
		return SYMBOL_LOGICALOUT;
	case 25:
		return SYMBOL_DEVCONSTIN;
	case 26:
		return SYMBOL_DEVCONSTOUT;
	case 27:
		return SYMBOL_RSOUT;

	}
return SYMBOL_ERROR;
}

CParserConfig::SYMBOLS CParserConfig::GetTokenSymbol(CParser::TOKEN t)
{
	switch(t.tc)
	{
	case CParser::tcString:
		return SYMBOL_STRINGLITERAL;
		break;
	case CParser::tcDec:
		return SYMBOL_DECLITERAL;
	case CParser::tcSeparator:
		return (SYMBOLS)t.data;
		break;
	case CParser::tcKeyWord:
		return GetIdKeyWord(t.data);
		break;
	case CParser::tcNoTerminal:
		return (SYMBOLS)t.data;
		break;
	case CParser::tcRegister:
		return SYMBOL_ID;
    default:
        return SYMBOL_ERROR; 
	}
	return SYMBOL_ERROR;
}

void CParserConfig::Clear()
{
	CParser::Clear();
	deviceNums.clear();
}

int CParserConfig::IndexOfDevice(short num)
{
	for (UINT i = 0; i < deviceNums.size(); i++)
		if (deviceNums[i] == num)
			return i;
	return -1;
}

#ifndef _FAST_PARSER
short CParserConfig::GetNextParserState(short state, TOKEN t)
{
	UINT sum = contrTable[0];
	UINT i; 
		
	UINT num = state * symbolCount + GetTokenSymbol(t) + 1;
	for (i = 2; sum < num; i+=2)
		if (i >= sizeof(contrTable))
			return contrTable[i - 1];
		else
			sum += contrTable[i];
	return contrTable[i - 1];
}

#endif

