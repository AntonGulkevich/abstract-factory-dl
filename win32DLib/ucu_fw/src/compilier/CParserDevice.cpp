
#include "cparserdevice.h"

#include "../application/cpattern.h"
#include "../application/cconfiguration.h"
#include "../utilities/commod.h"
#include "../utilities/console.h"
#include "../driversio/charincin.h"
#include "../driversio/charincout.h"
#include "../driversio/charincwordin.h"
#include "../driversio/charincwordout.h"

short CParserDevice::scannerContrTable[44][27] = 
{//		\r	%	(	)	*	,	/	^	+	=	0	.	Sym	"	!	&	-	Dig	<	>	|	\	;	{	}	All Space
	{	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	12,	13,	15,	17,	24,	26,	28,	29,	31,	33,	35, -1,	37,	38,	39,	-1, 1},  // State 0
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 1
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 2
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 3
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 4
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 5
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 6
	{	0,	0,	0,	0,	41,	0,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 7
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 8
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 9
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	11,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 10
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 11
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	12,	13,	0,	0,	0,	0,	0,	12,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 12
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	14,	0,	0,	0,	0,	0,	0,	14,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 13
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	14,	0,	0,	0,	0,	0,	0,	14,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 14
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	16,	0,	16,	0,	0,	0,	0,	16,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 15
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	16,	0,	16,	0,	0,	0,	0,	16,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 16
	{	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	19,	18,	18,	18,	18,	18,	18,	18,	20,	18,	18,	18,	18, 18},  // State 17
	{	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	19,	18,	18,	18,	18,	18,	18,	18,	20,	18,	18,	18,	18, 18},  // State 18
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 19
	{	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	22,	21,	21,	21,	21,	21,	21,	21,	23,	21,	21,	21,	21, 21},  // State 20
	{	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	19,	18,	18,	18,	18,	18,	18,	18,	20,	18,	18,	18,	18, 18},  // State 21
	{	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	19,	18,	18,	18,	18,	18,	18,	18,	20,	18,	18,	18,	18,	18},  // State 22
	{	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	22,	21,	21,	21,	21,	21,	21,	21,	23,	21,	21,	21,	21, 21},  // State 23
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	25,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 24
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 25
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	27,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 26
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 27
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 28
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	30,	13,	0,	0,	0,	0,	0,	30,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 29
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	30,	13,	0,	0,	0,	0,	0,	30,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 30
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 31
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 32
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 33
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 34
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	36,	0,	0,	0,	0,	0,	0},  // State 35
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 36
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 37
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 38
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},  // State 39
	{	0,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40, 40},  // State 40
	{	41,	41,	41,	41,	42,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41, 41},  // State 41
	{	41,	41,	41,	41,	41,	41,	43,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41,	41, 41},  // State 42
	{	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0}  // State 43


};

///////////////////////////////////////
// Generated Code
///////////////////////////////////////
#include "CParserDevice_cpp.inc"

////////////////////////////////////////
// End of Generated Code
////////////////////////////////////////


CParserDevice::CParserDevice(BYTE lDevice, CConfiguration* config, CPattern& lPattern, Console* console, Commod* commod) : CParser(console, commod),
		_config(config), curPattern(lPattern)
{ 
	device = lDevice; 
	curSection = sGeneral;
	parsingNC = false; 
	parsingNCLabel = false;
	curChannel = NULL;
}

int CParserDevice::ReduceRule(short rule)
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

short CParserDevice::Reverse(UINT label)
{
	int label1 = label % 10;
	int label10 = (label % 100 - label1) / 10;
	int label100 = (label - label10 - label1) / 100;
	if ((label1 > 7) || (label10 > 7) || (label100 > 3))
		return -1;
	return REVERSE((label1 + label10 * 8 + label100 * 64));
}

void CParserDevice::Clear()
{
	CParser::Clear();
	parsingNC = false;
	exprStack.clear();
	arrayBuff.clear();
	curChannel = NULL;
	curLabel = -1;
	curWordNum = -1;
	curRegType = ioError;
}

int CParserDevice::DefineSeparator(char* str)
{
	if (strlen(str) == 1)
	{
		if (str[0] == '-') return SYMBOL_MINUS;
		if (str[0] == '!') return SYMBOL_EXCLAM;
		if (str[0] == '%') return SYMBOL_PERCENT;
		if (str[0] == '&') return SYMBOL_AMP;
		if (str[0] == '(') return SYMBOL_LPARAN;
		if (str[0] == ')') return SYMBOL_RPARAN;
		if (str[0] == '*') return SYMBOL_TIMES;
		if (str[0] == ',') return SYMBOL_COMMA;
		if (str[0] == '/') return SYMBOL_DIV;
		if (str[0] == ';') return SYMBOL_SEMI;	
		if (str[0] == '^') return SYMBOL_CARET;	
		if (str[0] == '{') return SYMBOL_LBRACE;	
		if (str[0] == '|') return SYMBOL_PIPE;	
		if (str[0] == '}') return SYMBOL_RBRACE;
		if (str[0] == '+') return SYMBOL_PLUS;
		if (str[0] == '<') return SYMBOL_LT;	
		if (str[0] == '=') return SYMBOL_EQ;	
		if (str[0] == '>') return SYMBOL_GT;	
	} else
	if (strlen(str) == 2)
	{
		if (str[0] == '!' && str[1] == '=') return SYMBOL_EXCLAMEQ;
		if (str[0] == '&' && str[1] == '&') return SYMBOL_AMPAMP;
		if (str[0] == '|' && str[1] == '|') return SYMBOL_PIPEPIPE;
		if (str[0] == '<' && str[1] == '=') return SYMBOL_LTEQ;
		if (str[0] == '=' && str[1] == '=') return SYMBOL_EQEQ;
		if (str[0] == '>' && str[1] == '=') return SYMBOL_GTEQ;
	}
	return -1;
}

BYTE CParserDevice::GetSymbolType(char ch)
{
	// Разделители
	if (ch == ' ' || ch == '\t')
		return 26;
	if (ch == '\n' || ch == '\r')
		return 0;
	if (ch == '%') return 1;
	if (ch == '(') return 2;
	if (ch == ')') return 3;
	if (ch == '*') 
		return 4;
	if (ch == ',') return 5;
	if (ch == '/') return 6;
	if (ch == '^') return 7;
	if (ch == '+') return 8;
	if (ch == '=') return 9;
	if (ch == '0') return 10;
	if (ch == '.') return 11;
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_'))
		return 12;
	if (ch == '"') return 13;
	if (ch == '!') return 14;
	if (ch == '&') return 15;
	if (ch == '-') return 16;
	if (ch >= '1' && ch <= '9')
		return 17;
	if (ch == '>') return 18;
	if (ch == '<') return 19;
	if (ch == '|') return 20;
	if (ch == '\\') return 21;
	if (ch == ';') return 22;
	if (ch == '{') return 23;
	if (ch == '}') return 24;

	return 25;
}


CParser::TOKEN CParserDevice::ReduceSym(short state)
{
	TOKEN result;
	result.tc = tcError;
	TOKENCLASS tc;
	int data;

	switch(state)
	{
	case 43:
		curCh = ' '; // Коментарий является разделителем
		result.tc = tcNoPrint;
		PopSeparator();
		break;
	case 0:
	case 1:
	case 40:
		result.tc = tcNoPrint;
		PopSeparator();
		break;
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
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
		data = PopSeparator();
		result = AddToken(tcSeparator, (SYMBOLS)data);
		break;
	case 12:
	case 29:
	case 30:
		result = AddToken(tcDec, PopDec());
		break;
	case 13:
	case 14:
		result = AddToken(tcFloat, PopFloat());
		break;
	case 15:
	case 16:
		data = PopId(tc);
		result = AddToken(tc, data);
		break;
	case 19:
		result = AddToken(tcString, PopString());
		break;
	}
	return result;
}


CParserDevice::SYMBOLS CParserDevice::GetTokenSymbol(CParser::TOKEN t)
{
	switch(t.tc)
	{
	case CParser::tcString:
		return SYMBOL_STRINGLITERAL;
		break;
	case CParser::tcDec:
		return SYMBOL_DECLITERAL;
	case CParser::tcFloat:
		return SYMBOL_FLOATLITERAL;
		break;
	case CParser::tcSeparator:
		return (SYMBOLS)t.data;
		break;
	case CParser::tcId:
	case CParser::tcRegister:
	case CParser::tcFunction:
		return SYMBOL_ID;
		break;
	case CParser::tcKeyWord:
		return GetIdKeyWord(t.data);
		break;
	case CParser::tcNoTerminal:
		return (SYMBOLS)t.data;
		break;
    default:
        return SYMBOL_ERROR; 
	}
	return SYMBOL_ERROR;
}


int CParserDevice::AddAction(RULES rule)
{
	if (parsingNCLabel)
	{
		if ((rule == RULE_LABELS___LABELS_LABEL_PROG) ||
			(rule == RULE_LABELS___LABEL_PROG))
		{
			exprStack.clear();
			parsingNCLabel = false;
		}
		return 0;
	}
	if (parsingNC)  // Если идет разбор неподключенного в(ы)хода, то все пропускаем до следующего в(ы)хода
	{
		if ((rule == RULE_NUMS___NUMS_NUM_PROG) ||
			(rule == RULE_NUMS___NUM_PROG) || 
			(rule == RULE_NUMSARINC___NUMSARINC_NUM_LABELS) ||
			(rule == RULE_NUMSARINC___NUM_PROG_LABELS))
		{
			exprStack.clear();
			parsingNC = false;
			inParams = false;
		}
		return 0;
	}
	
	CRegister* reg;
	IChannel* io;
#ifdef _PARSER_TRACE	
	char text[50];
#endif
	EXPRESSION e; // Переменная для работы со стеком выражения
	switch(rule)
	{
	case RULE_CONFIG___GENSEC_INPUTSEC_OUTPUTSEC_END:
		break;
	case RULE_GENSEC___GENERAL_PROG: 
		curSection = sInputs;
		ioSecStr = "входа";
		inParams = false;
		curIOType = ioDigitalIn;
		break;
	case RULE_INPUTSEC___INPUTS_INTYPES: 
		curSection = sOutputs;
		ioSecStr = "выхода";
		curIOType = ioDigitalOut;
		break;
	case RULE_INTYPES___INTYPES_INTYPE:
	case RULE_INTYPES___INTYPE: 
		curIO = 0xFF;
		curLabel = -1;
		curWordNum = -1;
		break;
	case RULE_INTYPE___INNAME_NUMS: 
		curIOType = ioDigitalIn;
		break;
	case RULE_INTYPE___DIGITALIN_NUMSARINC: 
		break;
	case RULE_INNAME___POTENTIOMETR: 
		inParams ? curRegType = ioPotentiometr: curIOType = ioPotentiometr;
		break;
	case RULE_INNAME___ANALOG: 
		inParams ? curRegType = ioAnalog: curIOType = ioAnalog;
		break;
	case RULE_INNAME___RSIN: 
		inParams ? curRegType = ioRSIn: curIOType = ioRSIn;
		break;
	case RULE_INNAME___FREQUENCY: 
		//inParams ? curRegType = ioFrequency: curIOType = ioFrequency;
		inParams ? curRegType = ioError: curIOType = ioError;
		break;
	case RULE_INNAME___CHECK: 
		inParams ? curRegType = ioCheck: curIOType = ioCheck;
		break;
	case RULE_INNAME___LOGICALIN: 
		inParams ? curRegType = ioLogicalIn: curIOType = ioLogicalIn;
		break;
	case RULE_NUMS___NUMS_NUM_PROG: 
	case RULE_NUMS___NUM_PROG: 
	case RULE_NUMSARINC___NUMSARINC_NUM_LABELS: 
	case RULE_NUMSARINC___NUM_PROG_LABELS: 
	{
		inParams = false;
		IChannel::CheckConfigurationResult result;
		if (curSection == sOutputs)
		{
			result = curPattern.GetOutput(curIOType, curIO)->CheckConfiguration();
		}
		else
		{
			result = curPattern.GetInput(curIOType, curIO)->CheckConfiguration();
		}
		if (result == IChannel::CheckConfigurationResult::Error)
		{
            WriteCriticalWarning(params[0],
			"Для %s %d типа %s заполнены не все обязательные параметры", ioSecStr, curIO, GetTypeStr(curIOType));
			existWarning = 1;
		}
        curChannel = NULL;
	}
		break;
	case RULE_LABELS___LABELS_LABEL_PROG:
	case RULE_LABELS___LABEL_PROG: 
	{
		IChannel::CheckConfigurationResult result;
		if (curSection == sOutputs)
		{
			result = ((ChArincOut*)curPattern.GetOutput(ioDigitalOut, curIO))->GetLabel((BYTE)curWordNum)->CheckConfiguration();
		}
		else
		{
			result = ((ChArincIn*)curPattern.GetInput(ioDigitalIn, curIO))->GetLabel((BYTE)curWordNum)->CheckConfiguration();
		}
		if (result == IChannel::CheckConfigurationResult::Error)
		{
            WriteCriticalWarning(params[0],
			"Для метки %o ARINC %s %d заполнены не все обязательные параметры", REVERSE(curLabel), ioSecStr, curIO);
			existWarning = 1;
		}
        curChannel = NULL;
	}		
		break;
	case RULE_NUM___DECLITERAL: 
	{
		inParams = true;
		auto result = curPattern.ValidIO(curIOType, params[0].dec);
/*		if (curIOType == ioLogicalIn && result == -2)  // Логический вход конфигурируется 2 раза, как вход и как выход
			result = 0;*/
		switch(result)
		{
		case CPattern::VALIDIO_RESULT::OK:
			curIO = params[0].dec;
			if(curSection == sOutputs)
				curChannel = curPattern.GetChannel(curIOType, curIO);
			break;
		case CPattern::VALIDIO_RESULT::NC:
            WriteWarning(params[0],
			"Номер %s %d для типа %s помечен как неподключенный и был удален", ioSecStr, params[0].dec, GetTypeStr(curIOType));
			parsingNC = true;
			break;
		case CPattern::VALIDIO_RESULT::NotExist:
            WriteError(params[0],
			"Номер %s %d для типа %s несуществует или не подключен", ioSecStr, params[0].dec, GetTypeStr(curIOType));
			return -1;
		case CPattern::VALIDIO_RESULT::AlreadyConfigured:
			WriteError(params[0],
            "Номер %s %d для типа %s уже сконфигурирован", ioSecStr, params[0].dec, GetTypeStr(curIOType));
			return -1;
		}
	}
		break;
	case RULE_LABEL___DECLITERAL: 
	{
		short label = Reverse(params[0].dec);
		if (label == -1)
		{
            WriteError(params[0], "Метка %d не корректна", params[0].dec);
			return -1;
		}
		if (curSection == sOutputs)
		{
			auto addResult = curPattern.AddOut(ioArincWordOut, curIO, label);
			if (addResult == CPattern::ADDIO_RESULT::AlreadyExist)
			{
                WriteError(params[0],
				"Метка %d для ARINC выхода %d уже сконфигурирована", params[0].dec, curIO);
				return -1;
			}
			else if (addResult == CPattern::ADDIO_RESULT::Error)
			{
				WriteError(params[0],
				"Не удалось добавить метку %d для ARINC выхода %d", params[0].dec, curIO);
				return -1;
			}else
				curWordNum = label;
		}
		else
		{
			auto addResult = curPattern.AddIn(ioArincWordIn, curIO, label);
			if (addResult == CPattern::ADDIO_RESULT::AlreadyExist)
			{
                WriteError(params[0],
				"Метка %d для ARINC входа %d уже сконфигурирована", params[0].dec, curIO);
				return -1;
			}else if (addResult == CPattern::ADDIO_RESULT::Error)
			{
				WriteError(params[0],
				"Не удалось добавить метку %d для ARINC входа %d", params[0].dec, curIO);
				return -1;
			}else
				curWordNum = label;
		}
		curLabel = (BYTE)label;
		if (curSection == sOutputs)
		{
			curChannel = curPattern.GetChannelLabel(curIO, label);
		}
	}
		break;

	case RULE_OUTPUTSEC___OUTPUTS_OUTTYPES: 
		break;
	case RULE_OUTTYPES___OUTTYPES_OUTTYPE: 
	case RULE_OUTTYPES___OUTTYPE: 
		curIO = 0xFF;
		curLabel = -1;
		curWordNum = -1;
		break;
	case RULE_OUTTYPE___OUTNAME_NUMS: 
	case RULE_OUTTYPE___DIGITALOUT_NUMSARINC: 
		curIOType = ioDigitalOut;
		break;
	case RULE_OUTNAME___ANGLE: 
		inParams ? curRegType = ioSCT: curIOType = ioSCT;
		break;
	case RULE_OUTNAME___NPT: 
		inParams ? curRegType = ioNPT: curIOType = ioNPT;
		break;
	case RULE_OUTNAME___COMMAND: 
		inParams ? curRegType = ioCommand: curIOType = ioCommand;
		break;
	case RULE_OUTNAME___LOGICALOUT: 
		inParams ? curRegType = ioLogicalOut: curIOType = ioLogicalOut;
		break;
	case RULE_OUTNAME___RSOUT: 
		inParams ? curRegType = ioRSOut: curIOType = ioRSOut;
		break;
	case RULE_PROG___PROG_PROGSTEP: break;
	case RULE_PROG___PROGSTEP: break;
	case RULE_PROGSTEP___OP_ASSIGN: 
		break;
	case RULE_PROGSTEP___OP_IF: 
		break;
	case RULE_OP_ASSIGN___ID_EQ_EXPR_SEMI: 
		switch(curSection)
		{
		case sGeneral:
			if (params[0].tc != CParser::tcRegister)
			{
                WriteCriticalWarning(params[0],
				"Использование переменных/функций в секции GENERAL невозможно");
				existWarning = 1;
			} else
			if ((reg = curPattern.GetRegister((REGISTER_ID)params[0].data)) == NULL)
			{
                WriteCriticalWarning(params[0],
				"Регистр %s для секции GENERAL несуществует", registers[params[0].data]);
				existWarning = 1;
			}
			else if (curPattern.GetRegisterT((REGISTER_ID)params[0].data)->type != rtFloat && curPattern.GetRegisterT((REGISTER_ID)params[0].data)->type != rtDec)
			{
                WriteCriticalWarning(params[0],
				"Регистр %s для секции GENERAL не является числовым типом", registers[params[0].data]);
				existWarning = 1;
			}
			else if ((curPattern.GetRegisterT((REGISTER_ID)params[0].data)->min_value > exprStack.back().value) || 
				(curPattern.GetRegisterT((REGISTER_ID)params[0].data)->max_value < exprStack.back().value))
			{
                WriteCriticalWarning(params[0],
				"Регистру %s для секции GENERAL присваивается значение вне допустимых пределов", registers[params[0].data]);
			}
			else
				reg->SetValue(exprStack.back().value);
			break;
		case sInputs:
			if (params[0].tc != CParser::tcRegister)
			{
                WriteCriticalWarning(params[0],
				"Использование переменных/функций в секции INPUTS невозможно");
				existWarning = 1;
			} else
			if ((curIOType == ioDigitalIn) && (curLabel != -1))
			{
				io = ((ChArincIn*)curPattern.GetInput(ioDigitalIn, curIO))->GetLabel((BYTE)curWordNum);
				if ((reg = io->GetRegister((REGISTER_ID)params[0].data)) == NULL)
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для метки %o номера входа %d типа DIGITALIN несуществует", registers[params[0].data], REVERSE(curLabel), curIO);
					existWarning = 1;
				}
				else if (io->GetRegisterT((REGISTER_ID)params[0].data)->type != rtFloat && io->GetRegisterT((REGISTER_ID)params[0].data)->type != rtDec)
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для метки %o номера входа %d типа DIGITALIN не является числовым типом", registers[params[0].data], REVERSE(curLabel), curIO);
					existWarning = 1;
				}
				else if ((io->GetRegisterT((REGISTER_ID)params[0].data)->min_value > exprStack.back().value) ||
					(io->GetRegisterT((REGISTER_ID)params[0].data)->max_value < exprStack.back().value))
				{
                    WriteCriticalWarning(params[0],
					"Регистру %s для метки %o номера входа %d типа DIGITALIN присваивается значение вне допустимых пределов", registers[params[0].data], REVERSE(curLabel), curIO);
					existWarning = 1;
				}
				else
					reg->SetValue(exprStack.back().value);
			}
			else 
			{
				io = curPattern.GetInput(curIOType, curIO);
				if ((reg = io->GetRegister((REGISTER_ID)params[0].data)) == NULL) 
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для номера входа %d типа %s несуществует", registers[params[0].data], curIO, GetTypeStr(curIOType));
					existWarning = 1;
				}
				else if (io->GetRegisterT((REGISTER_ID)params[0].data)->type != rtFloat && io->GetRegisterT((REGISTER_ID)params[0].data)->type != rtDec)
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для номера входа %d типа %s является типом string", registers[params[0].data], curIO, GetTypeStr(curIOType));
					existWarning = 1;
				}
				else if ((io->GetRegisterT((REGISTER_ID)params[0].data)->min_value > exprStack.back().value) ||
					(io->GetRegisterT((REGISTER_ID)params[0].data)->max_value < exprStack.back().value))
				{
                    WriteCriticalWarning(params[0],
					"Регистру %s для номера входа %d типа %s присваивается значение вне допустимых пределов", registers[params[0].data], curIO, GetTypeStr(curIOType));
					existWarning = 1;
				}
				else
					reg->SetValue(exprStack.back().value);
			}
			exprStack.pop_back();
#ifdef _PARSER_TRACE
			TRACEUCU("Регистру %s присвоено значение %g", registers[params[0].data], exprStack.back().value);
#endif		
			break;
		case sOutputs:
			switch(params[0].tc)
			{
			case CParser::tcRegister:
			{
				io = curPattern.GetOutput(curIOType, curIO);
				if ((curIOType == ioDigitalOut) && (curLabel != -1))
				{
					io = ((ChArincOut*)io)->GetLabel((BYTE)curWordNum);
					if ((reg = io->GetRegister((REGISTER_ID)params[0].data)) == NULL)  // Если неверный регистр, ошибка
					{
                        WriteCriticalWarning(params[0],
						"Регистр %s для метки %o номера выхода %d типа DIGITALOUT несуществует", registers[params[0].data], REVERSE(curLabel), curIO);
						existWarning = 1;
					}
					// Если тип не числовой, ошибка
					else if (io->GetRegisterT((REGISTER_ID)params[0].data)->type != rtFloat && io->GetRegisterT((REGISTER_ID)params[0].data)->type != rtDec)
					{
                        WriteCriticalWarning(params[0],
						"Регистр %s для метки %o номера выхода %d типа DIGITALOUT не является числовым типом", registers[params[0].data], REVERSE(curLabel), curIO);
						existWarning = 1;
					}
					// Если тип присваемого значения константа, проверяем допустимые пределы
					else if (exprStack.back().type == tsConstant)
					{
						if ((io->GetRegisterT((REGISTER_ID)params[0].data)->min_value > exprStack.back().value) ||
							(io->GetRegisterT((REGISTER_ID)params[0].data)->max_value < exprStack.back().value))
						{
                            WriteCriticalWarning(params[0],
							"Регистру %s для метки %o номера выхода %d типа DIGITALOUT присваивается значение вне доспустимых пределов", registers[params[0].data], REVERSE(curLabel), curIO);
							existWarning = 1;
						}
						else
						{
							// Все успешно -> Добавление команды
							reg->SetValue(exprStack.back().value);
							if (io->GetRegisterT((REGISTER_ID)params[0].data)->rw == rwVariable && curChannel != NULL)
							{
								curChannel->AddParam(-curChannel->AddConst(exprStack.back().value));
								curChannel->AddParam(params[0].data);
								reg->SetValue(io->GetRegisterT((REGISTER_ID)params[0].data)->default_value);
								curChannel->AddAction(CProcessor::acAssignRegister);
							}
						}
					}
					// Если тип регистра не переменная, (тип присваемого значения уже не константа) то ошибка
					else if (io->GetRegisterT((REGISTER_ID)params[0].data)->rw != rwVariable)
					{
                        WriteCriticalWarning(params[0],
						"Константному регистру %s для метки %o номера выхода %d типа DIGITAL присваивается значение переменной", registers[params[0].data], REVERSE(curLabel), curIO);
						existWarning = 1;
					}
					// Тип регистра переменная, значение переменная --> добавление команды
					else if (curChannel != NULL)
					{
						curChannel->AddParam((short)exprStack.back().value);
						curChannel->AddParam(params[0].data);
						reg->SetValue(io->GetRegisterT((REGISTER_ID)params[0].data)->default_value);
						curChannel->AddAction(CProcessor::acAssignRegister);
					}
				}
				// Если секция не метка ARINC
				else
				{
					// Если неверный регистр, ошибка
					if ((reg = io->GetRegister((REGISTER_ID)params[0].data)) == NULL)
					{
                        WriteCriticalWarning(params[0],
						"Регистр %s для номера выхода %d типа %s несуществует", registers[params[0].data], curIO, GetTypeStr(curIOType));
						existWarning = 1;
					}
					else if (io->GetRegisterT((REGISTER_ID)params[0].data)->type != rtFloat && io->GetRegisterT((REGISTER_ID)params[0].data)->type != rtDec)
					{
                        WriteCriticalWarning(params[0],
						"Регистр %s для номера выхода %d типа %s не является числовым типом", registers[params[0].data], curIO, GetTypeStr(curIOType));
						existWarning = 1;
					}
					else if (exprStack.back().type == tsConstant)
					{
						if ((io->GetRegisterT((REGISTER_ID)params[0].data)->min_value > exprStack.back().value) ||
							(io->GetRegisterT((REGISTER_ID)params[0].data)->max_value < exprStack.back().value))
						{
                            WriteCriticalWarning(params[0],
							"Регистру %s для номера выхода %d типа %s присваивается значение вне доспустимых пределов", registers[params[0].data], curIO, GetTypeStr(curIOType));
							existWarning = 1;
						}
						else
						{
							reg->SetValue(exprStack.back().value);
							if (io->GetRegisterT((REGISTER_ID)params[0].data)->rw == rwVariable && curChannel != NULL)
							{
								curChannel->AddParam(-curChannel->AddConst(exprStack.back().value));
								curChannel->AddParam(params[0].data);
								reg->SetValue(io->GetRegisterT((REGISTER_ID)params[0].data)->default_value);
								curChannel->AddAction(CProcessor::acAssignRegister);
							}
						}
					}
					else
						if (io->GetRegisterT((REGISTER_ID)params[0].data)->rw != rwVariable)
						{
                            WriteCriticalWarning(params[0],
							"Константному регистру %s для номера выхода %d типа %s присваивается значение переменной", registers[params[0].data], curIO, GetTypeStr(curIOType));
							existWarning = 1;
						}
						else if (curChannel != NULL)
						{
							curChannel->AddParam((short)exprStack.back().value);
							curChannel->AddParam(params[0].data);
							reg->SetValue(io->GetRegisterT((REGISTER_ID)params[0].data)->default_value);
							curChannel->AddAction(CProcessor::acAssignRegister);
						}
				}
			}

				break;
			case CParser::tcId:
				if (curChannel != NULL)
				{
					if (exprStack.back().type == tsConstant)
						curChannel->AddParam(-curChannel->AddConst(exprStack.back().value));
					else 
						curChannel->AddParam((short)exprStack.back().value);
					curChannel->AddParam(curChannel->AddVar(params[0].data));
					curChannel->AddAction(CProcessor::acAssign);
				}
				break;
			case CParser::tcFunction:
                WriteCriticalWarning(params[0], "Функции невозможно присвоить значение");
				existWarning = 1;
				break;
            default:
                break;
			}
			exprStack.pop_back();
			break;
		}		
		break;
	case RULE_OP_ASSIGN___ID_EQ_STRINGLITERAL_SEMI: 
		if (params[0].tc != CParser::tcRegister)
		{
            WriteCriticalWarning(params[0],
			"Использование переменных типа string невозможно");
			existWarning = 1;
		}
		switch(curSection)
		{
		case sGeneral:
			if ((reg = curPattern.GetRegister((REGISTER_ID)params[0].data)) == NULL)
			{
                WriteCriticalWarning(params[0],
				"Регистр %s для секции GENERAL несуществует", registers[params[0].data]);
				existWarning = 1;
			}
			else if (curPattern.GetRegisterT((REGISTER_ID)params[0].data)->type != rtString)
			{
                WriteCriticalWarning(params[0],
				"Регистр %s для секции GENERAL не является строковым типом", registers[params[0].data]);
				existWarning = 1;
			}
			else
				reg->SetValue(params[2].str);
			break;
		case sInputs:
		case sOutputs:
		{
			IChannel* ioWord = NULL;
			if (curSection == sInputs)
			{
				io = curPattern.GetInput(curIOType, curIO);
				if (curIOType == ioDigitalIn)
					ioWord = ((ChArincIn*)io)->GetLabel((BYTE)curWordNum);
			}
			else
			{
				io = curPattern.GetOutput(curIOType, curIO);
				if (curIOType == ioDigitalOut)
					ioWord = ((ChArincOut*)io)->GetLabel((BYTE)curWordNum);
			}
			if (((curIOType == ioDigitalIn) || (curIOType == ioDigitalOut))&& (curLabel != -1))
			{
				if ((reg = ioWord->GetRegister((REGISTER_ID)params[0].data)) == NULL)
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для метки %o номера %s %d типа DIGITAL несуществует", registers[params[0].data], REVERSE(curLabel), ioSecStr, curIO);
					existWarning = 1;
				}
				else if (ioWord->GetRegisterT((REGISTER_ID)params[0].data)->type != rtString)
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для метки %o номера %s %d типа DIGITAL не является строковым типом", registers[params[0].data], REVERSE(curLabel), ioSecStr, curIO);
					existWarning = 1;
				}
				else
					reg->SetValue(params[2].str);
			}
			else 
			{
				if ((reg = io->GetRegister((REGISTER_ID)params[0].data)) == NULL)
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для номера %s %d типа %s несуществует", registers[params[0].data], ioSecStr, curIO, GetTypeStr(curIOType));
					existWarning = 1;
				}
				else if (io->GetRegisterT((REGISTER_ID)params[0].data)->type != rtString)
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для номера %s %d типа %s не является строковым типом", registers[params[0].data], ioSecStr, curIO, GetTypeStr(curIOType));
					existWarning = 1;
				}
				else
					reg->SetValue(params[2].str);
			}
		}
			break;
		}
#ifdef _PARSER_TRACE
		TRACEUCU("Регистру %s присвоено значение \"%s\"", registers[params[0].data], params[2].str);
#endif
		break;
	case RULE_OP_ASSIGN___ID_EQ_ARRAY_SEMI:
		if (params[0].tc != CParser::tcRegister)
		{
            WriteCriticalWarning(params[0],
			"Использование переменных типа array невозможно");
			existWarning = 1;
		}
		switch(curSection)
		{
		case sGeneral:
			if ((reg = curPattern.GetRegister((REGISTER_ID)params[0].data)) == NULL)
			{
                WriteCriticalWarning(params[0],
				"Регистр %s для секции GENERAL несуществует", registers[params[0].data]);
				existWarning = 1;
			}
			else if (curPattern.GetRegisterT((REGISTER_ID)params[0].data)->type != rtArray)
			{
                WriteCriticalWarning(params[0],
				"Регистр %s для секции GENERAL не является типом array", registers[params[0].data]);
				existWarning = 1;
			}
			else if (arrayBuff.size() > curPattern.GetRegisterT((REGISTER_ID)params[0].data)->max_value)
			{
                WriteCriticalWarning(params[0],
				"Для регистра %s, секции GENERAL много элементов массива", registers[params[0].data]);
				existWarning = 1;
			}
			else if (arrayBuff.size() < curPattern.GetRegisterT((REGISTER_ID)params[0].data)->min_value)
			{
                WriteCriticalWarning(params[0],
				"Для регистра %s, секции GENERAL мало элементов массива", registers[params[0].data]);
				existWarning = 1;
			}
			else
			{
				float* arr = new float[arrayBuff.size()];
				for(UINT i = 0; i < arrayBuff.size(); i++)
					arr[i] = arrayBuff[i];
				reg->SetValueArray(arr, arrayBuff.size());
			}
			break;
		case sInputs:
		case sOutputs:
		{
			IChannel* ioWord = NULL;
			if (curSection == sInputs)
			{
				io = curPattern.GetInput(curIOType, curIO);
				if (curIOType == ioDigitalIn)
					ioWord = ((ChArincIn*)io)->GetLabel((BYTE)curWordNum);
			}
			else
			{
				io = curPattern.GetOutput(curIOType, curIO);
				if (curIOType == ioDigitalOut)
					ioWord = ((ChArincOut*)io)->GetLabel((BYTE)curWordNum);
			}
			if (((curIOType == ioDigitalIn) || (curIOType == ioDigitalOut))&& (curLabel != -1))
			{
				if ((reg = ioWord->GetRegister((REGISTER_ID)params[0].data)) == NULL)
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для метки %o номера %s %d типа DIGITAL несуществует", registers[params[0].data], REVERSE(curLabel), ioSecStr, curIO);
					existWarning = 1;
				}
				else if (ioWord->GetRegisterT((REGISTER_ID)params[0].data)->type != rtArray)
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для метки %o номера %s %d типа DIGITAL не является типом array", registers[params[0].data], REVERSE(curLabel), ioSecStr, curIO);
					existWarning = 1;
				}
				else if (arrayBuff.size() > ioWord->GetRegisterT((REGISTER_ID)params[0].data)->max_value)
				{
                    WriteCriticalWarning(params[0],
					"Для регистра %s для метки %o номера %s %d типа DIGITAL много элементов массива", registers[params[0].data], REVERSE(curLabel), ioSecStr, curIO);
					existWarning = 1;
				}
				else if (arrayBuff.size() < ioWord->GetRegisterT((REGISTER_ID)params[0].data)->min_value)
				{
                    WriteCriticalWarning(params[0],
					"Для регистра %s для метки %o номера %s %d типа DIGITAL мало элементов массива", registers[params[0].data], REVERSE(curLabel), ioSecStr, curIO);
					existWarning = 1;
				}
				else
				{
					float* arr = new float[arrayBuff.size()];
					for(UINT i = 0; i < arrayBuff.size(); i++)
						arr[i] = arrayBuff[i];
					reg->SetValueArray(arr, arrayBuff.size());					 
					delete[] arr;
				}
			}
			else 
			{
				if ((reg = io->GetRegister((REGISTER_ID)params[0].data)) == NULL)
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для номера %s %d типа %s несуществует", registers[params[0].data], ioSecStr, curIO, GetTypeStr(curIOType));
					existWarning = 1;
				}
				else if (io->GetRegisterT((REGISTER_ID)params[0].data)->type != rtArray)
				{
                    WriteCriticalWarning(params[0],
					"Регистр %s для номера %s %d типа %s не является типом array", registers[params[0].data], ioSecStr, curIO, GetTypeStr(curIOType));
					existWarning = 1;
				}
				else if (arrayBuff.size() > io->GetRegisterT((REGISTER_ID)params[0].data)->max_value)
				{
                    WriteCriticalWarning(params[0],
					"Для регистра %s для номера %s %d типа %s много элементов массива", registers[params[0].data], ioSecStr, curIO, GetTypeStr(curIOType));
					existWarning = 1;
				}
				else if (arrayBuff.size() < io->GetRegisterT((REGISTER_ID)params[0].data)->min_value)
				{
                    WriteCriticalWarning(params[0],
					"Для регистра %s для номера %s %d типа %s мало элементов массива", registers[params[0].data], ioSecStr, curIO, GetTypeStr(curIOType));
					existWarning = 1;
				}
				else
				{
					float* arr = new float[arrayBuff.size()];
					for(UINT i = 0; i < arrayBuff.size(); i++)
						arr[i] = arrayBuff[i];
					reg->SetValueArray(arr, arrayBuff.size());		
					delete[] arr;
				}
			}
			arrayBuff.clear();
		}
			break;
		}
#ifdef _PARSER_TRACE
		TRACEUCU("Регистру %s присвоено значение \"%s\"", registers[params[0].data], params[2].str);
#endif
		break;
		break;
	case RULE_OP_IF___IF_IFEXPR_THENIF_ELSEIF: 
		break;
	case RULE_OP_IF___IF_IFEXPR_THENIF: 
		if (curChannel != NULL)
			curChannel->SetLabel((BYTE)exprStack.back().value);
		exprStack.pop_back();
		break;
	case RULE_THENIF___LBRACE_PROG_RBRACE: 
		switch(curSection)
		{
		case sGeneral:
		case sInputs:
			WriteCriticalWarning(params[0],
            "В данной секции использование ветвлений невозможно");
			existWarning = 1;
			break;
		case sOutputs:
			{
				if (curChannel != NULL)
				{
					curChannel->AddAction(CProcessor::acJmp);
					BYTE labelNum = (BYTE)exprStack[exprStack.size() - 1].value;
					exprStack[exprStack.size() - 1].value = curChannel->AddLabel();
					curChannel->AddParam((short)exprStack[exprStack.size() - 1].value);
					curChannel->SetLabel(labelNum);
				}
			}
			break;
		}
		break;
	case RULE_ELSEIF___ELSE_LBRACE_PROG_RBRACE: 
		switch(curSection)
		{
		case sGeneral:
		case sInputs:
            WriteCriticalWarning(params[0],
			"В данной секции использование ветвлений невозможно");
			existWarning = 1;
			break;
		case sOutputs:
			if (curChannel != NULL)
				curChannel->SetLabel((BYTE)exprStack.back().value);
			exprStack.pop_back();
			break;
		}
		break;
	case RULE_IFEXPR___LPARAN_EXPR_RPARAN: 
		switch(curSection)
		{
		case sGeneral:
		case sInputs:
            WriteCriticalWarning(params[0],
			"В данной секции использование ветвлений невозможно");
			existWarning = 1;
			break;
		case sOutputs:
			if (curChannel != NULL)
			{
				if (exprStack.back().type == tsConstant)
					curChannel->AddParam(-curChannel->AddConst(exprStack.back().value));
				else
					curChannel->AddParam((short)exprStack.back().value);
				exprStack.pop_back();
				curChannel->AddAction(CProcessor::acJz);
				e.value = curChannel->AddLabel();
				e.type = tsLabel;
				exprStack.push_back(e);
				curChannel->AddParam((short)exprStack[exprStack.size() - 1].value);
			}
			break;
		}
		break;
	case RULE_FUNCTION___ID_LPARAN_FUNCPARAMS_RPARAN:
	case RULE_FUNCTION___ID_LPARAN_RPARAN: 
	{
		if (params[0].tc != CParser::tcFunction)
		{
            WriteCriticalWarning(params[0],
			"Неизвестная функция %s", GetIdName(params[0]));
			existWarning = 1;			
		}
		int fc;
		for (fc = 0; ((!exprStack.empty()) && (exprStack.back().type == tsFuncParam)); exprStack.pop_back())
			fc++;
		if (curChannel != NULL)
		{
			curChannel->AddParam(params[0].data);
			int trueFC = curChannel->TestFunctionParam(params[0].data);
			if (trueFC == fc)
				curChannel->AddParam(fc);	
			else if (params[0].tc == CParser::tcFunction)
			{			
				WriteCriticalWarning(params[0],
				"Функция %s требует %d параметров. Представлено %d", functions[params[0].data], trueFC, fc);
				existWarning = 1;
			}
		}
	}
		break;
	case RULE_FUNCPARAMS___FUNCPARAMS_COMMA_FUNCPARAM:
	case RULE_FUNCPARAMS___FUNCPARAM:
		break;
	case RULE_FUNCPARAM___DIGITALIN:
	case RULE_FUNCPARAM___DIGITALOUT:
		if (rule == RULE_FUNCPARAM___DIGITALOUT)
			curRegType = ioDigitalOut;
		else
			curRegType = ioDigitalIn;
		// break не надо
	case RULE_FUNCPARAM___INNAME:
	case RULE_FUNCPARAM___OUTNAME:
		switch(curSection)
		{
		case sGeneral:
		case sInputs:
            WriteCriticalWarning(params[0],
			"В данной секции использование функций невозможно");
			existWarning = 1;
			break;
		case sOutputs:
			if (curChannel != NULL)
			{
				e.value = curChannel->AddParam(-curChannel->AddConst((float)curRegType));
				e.type = tsFuncParam;
				exprStack.push_back(e);
				curChannel->AddAction(CProcessor::acAddStack);
			}
			break;
		}
		break;
	case RULE_FUNCPARAM___EXPR: 
		switch(curSection)
		{
		case sGeneral:
		case sInputs:
            WriteCriticalWarning(params[0],
			"В данной секции использование функций невозможно");
			existWarning = 1;
			break;
		case sOutputs:
			if (curChannel != NULL)
			{
				if (exprStack.back().type == tsConstant)
					exprStack.back().value = curChannel->AddParam(-curChannel->AddConst(exprStack.back().value));
				else
					exprStack.back().value = curChannel->AddParam((short)exprStack.back().value);
				exprStack.back().type = tsFuncParam;
				curChannel->AddAction(CProcessor::acAddStack);
			}
			break;
		}
		break;
	case RULE_ARRAY___LBRACE_ARRAYITEMS_RBRACE:
		break;
	case RULE_ARRAYITEMS___ARRAYITEMS_COMMA_EXPR:
	case RULE_ARRAYITEMS___EXPR:
		if (exprStack.back().type == tsConstant)
		{
			arrayBuff.push_back(exprStack.back().value);
			exprStack.pop_back();
		} else
		{
            WriteCriticalWarning(params[0],
			"Элемент массива должен быть константой");
		}
		break;
	case RULE_REGISTERLABEL___ID_LPARAN_DIGITALIN_COMMA_DECLITERAL_COMMA_DECLITERAL_RPARAN:
	case RULE_REGISTERLABEL___ID_LPARAN_DIGITALOUT_COMMA_DECLITERAL_COMMA_DECLITERAL_RPARAN:
	case RULE_REGISTERLABEL___ID_LPARAN_DIGITALIN_COMMA_DECLITERAL_COMMA_DECLITERAL_COMMA_DECLITERAL_RPARAN:
	case RULE_REGISTERLABEL___ID_LPARAN_DIGITALOUT_COMMA_DECLITERAL_COMMA_DECLITERAL_COMMA_DECLITERAL_RPARAN:
	{
		short id = 0;
		if (rule == RULE_REGISTERLABEL___ID_LPARAN_DIGITALIN_COMMA_DECLITERAL_COMMA_DECLITERAL_COMMA_DECLITERAL_RPARAN ||
			rule == RULE_REGISTERLABEL___ID_LPARAN_DIGITALOUT_COMMA_DECLITERAL_COMMA_DECLITERAL_COMMA_DECLITERAL_RPARAN)
			id = params[8].dec;
		if (rule == RULE_REGISTERLABEL___ID_LPARAN_DIGITALOUT_COMMA_DECLITERAL_COMMA_DECLITERAL_RPARAN ||
			rule == RULE_REGISTERLABEL___ID_LPARAN_DIGITALOUT_COMMA_DECLITERAL_COMMA_DECLITERAL_COMMA_DECLITERAL_RPARAN)
			curRegType = ioDigitalOut;
		else
			curRegType = ioDigitalIn;
		switch(curSection)
		{
		case sInputs:
		case sGeneral:
            WriteCriticalWarning(params[0],
			"В данной секции использование регистров возможно только для записи");
			existWarning = 1;
			break;			
		case sOutputs:
		{
			if (params[0].tc != CParser::tcRegister)
			{
                WriteCriticalWarning(params[0],
				"%s не является регистром", GetIdName(params[0]));
				existWarning = 1;				
			}
			auto result = curPattern.ValidIO(curRegType, params[4].dec);
			switch(result)
			{
			case CPattern::VALIDIO_RESULT::NC:
                WriteWarning(params[0],
				"Попытка чтения регистра %s с входа/выхода %d типа DIGITAL помеченного\r\nкак неподключенный, каскад %s,%d был удален!", 
					GetIdName(params[0]), params[4].dec, GetTypeStr(curIOType), curIO);
				parsingNCLabel = true;
				break;
			case CPattern::VALIDIO_RESULT::NotExist:
                WriteCriticalWarning(params[0],
				"Попытка чтения регистра %s с несуществующего входа/выхода %d типа DIGITAL", GetIdName(params[0]), params[4].dec);
				existWarning = 1;
				break;
			case CPattern::VALIDIO_RESULT::OK:
                WriteWarning(params[0],
				"Попытка чтения регистра %s с несконфигурированного входа/выхода %d типа DIGITAL", GetIdName(params[0]), params[4].dec);
				/// break не надо
			case CPattern::VALIDIO_RESULT::AlreadyConfigured:
				if (_config->TestConnection(curPattern.GetSelfNum(), curRegType, params[4].dec) == -1)
				{	
                    WriteCriticalWarning(params[0],
					"Попытка чтения регистра %s с неподключенного %s %d типа DIGITAL", GetIdName(params[0]), (IS_IN(curRegType) ? "входа" : "выхода"), params[4].dec);
					existWarning = 1;
				} 
				if (Reverse(params[6].dec) == -1)
				{
                    WriteCriticalWarning(params[0],
					"Попытка чтения некорректной метки %d", params[6].dec);
					existWarning = 1;
				} 
				else if (curRegType == ioDigitalIn)
				{
					auto ch = (ChArincIn*)curPattern.GetInput(ioDigitalIn, params[4].dec);
					/// !!!!!! Сюда надо добавить чтение по ID
					if (!ch->IsLabelExist((BYTE)Reverse(params[6].dec)))
					{
                        WriteCriticalWarning(params[0],
						"Попытка чтения регистра %s с несуществующей метки %d входа %d типа DIGITAL", GetIdName(params[0]), params[6].dec, params[4].dec);
						existWarning = 1;
					} else
					{
						io = ch->GetLabel((BYTE)Reverse(params[6].dec));
						if (io->GetRegister((REGISTER_ID)params[0].data) == NULL)
						{
                            WriteCriticalWarning(params[0],
							"Попытка чтения несуществующего регистра %s со входа %d типа DIGITAL метка", GetIdName(params[0]), params[4].dec, params[6].dec);
							existWarning = 1;
						}
						if (curChannel != NULL)
						{
							curChannel->AddParam((short)params[4].dec);
							curChannel->AddParam((short)Reverse((WORD)params[6].dec));
							curChannel->AddParam((short)params[0].data);
							curChannel->AddParam(id);
							curChannel->AddParam(ioDigitalIn);
						}
					}
				}
				else if (curRegType == ioDigitalOut)
				{
					/// !!!!!! Сюда надо добавить чтение по ID
					auto ch = (ChArincOut*)curPattern.GetInput(ioDigitalIn, params[4].dec);
					if (!ch->IsLabelExist((BYTE)Reverse(params[6].dec)))
					{
                        WriteCriticalWarning(params[0],
						"Попытка чтения регистра %s с несуществующей метки %d выхода %d типа DIGITAL", GetIdName(params[0]), params[6].dec, params[4].dec);
						existWarning = 1;
					} else
					{
						io = ch->GetLabel((BYTE)Reverse(params[6].dec));
						if (io->GetRegister((REGISTER_ID)params[0].data) == NULL)
						{
                            WriteCriticalWarning(params[0],
							"Попытка чтения несуществующего регистра %s с выхода %d типа DIGITAL метка %d", GetIdName(params[0]), params[4].dec, params[6].dec);
							existWarning = 1;
						}
						if (curChannel != NULL)
						{
							curChannel->AddParam((short)params[4].dec);
							curChannel->AddParam((short)Reverse((WORD)params[6].dec));
							curChannel->AddParam((short)params[0].data);
							curChannel->AddParam(id);
							curChannel->AddParam(ioDigitalOut);
						}
					}
				}
				break;
			}
		}
			break;
		}
	}
		break;
	case RULE_REGISTER___ID_LPARAN_DIGITALIN_COMMA_DECLITERAL_RPARAN:
	case RULE_REGISTER___ID_LPARAN_DIGITALOUT_COMMA_DECLITERAL_RPARAN:
		if (rule == RULE_REGISTER___ID_LPARAN_DIGITALIN_COMMA_DECLITERAL_RPARAN)
			curRegType = ioDigitalIn;
		else
			curRegType = ioDigitalOut;
		//break не нужен
	case RULE_REGISTER___ID_LPARAN_INNAME_COMMA_DECLITERAL_RPARAN:
	case RULE_REGISTER___ID_LPARAN_OUTNAME_COMMA_DECLITERAL_RPARAN:
	{
		switch(curSection)
		{
		case sGeneral:
		case sInputs:
            WriteCriticalWarning(params[0],
			"В данной секции использование регистров возможно только для записи");
			existWarning = 1;
			break;
		case sOutputs:
		{
			if (params[0].tc != CParser::tcRegister)
			{
                WriteCriticalWarning(params[0],
				"%s не является регистром", GetIdName(params[0]));
				existWarning = 1;				
			}
			auto result = curPattern.ValidIO(curRegType, params[4].dec);
			switch(result)
			{
			case CPattern::VALIDIO_RESULT::NC:
				if (curLabel == -1)
				{
                    WriteWarning(params[0],
					"Попытка чтения регистра %s с входа/выхода %d типа %s помеченного\r\nкак неподключенный, каскад %s,%d был удален!", 
						GetIdName(params[0]), params[4].dec, GetTypeStr(curRegType), GetTypeStr(curIOType), curIO);
					parsingNC = true;
				} else
				{
                    WriteWarning(params[0],
					"Попытка чтения регистра %s с входа/выхода %d типа %s помеченного\r\nкак неподключенный, каскад %s,%d,%03o был удален!", 
						GetIdName(params[0]), params[4].dec, GetTypeStr(curRegType), GetTypeStr(curIOType), curIO, REVERSE(curLabel));
					parsingNCLabel = true;					
				}
				break;
			case CPattern::VALIDIO_RESULT::NotExist:
				WriteCriticalWarning(params[0],
                "Попытка чтения регистра %s с несуществующего входа/выхода %d типа %s", GetIdName(params[0]), params[4].dec, GetTypeStr(curRegType));
				existWarning = 1;
				break;
			case CPattern::VALIDIO_RESULT::OK:
                WriteWarning(params[0],
				"Попытка чтения регистра %s с несконфигурированного входа/выхода %d типа %s", GetIdName(params[0]), params[4].dec, GetTypeStr(curRegType));
				// break не надо
			case CPattern::VALIDIO_RESULT::AlreadyConfigured:
				if (_config->TestConnection(curPattern.GetSelfNum(), curRegType, params[4].dec) == -1)
				{
                    WriteCriticalWarning(params[0],
					"Попытка чтения регистра %s с неподключенного %s %d типа %s", GetIdName(params[0]), (IS_IN(curRegType) ? "входа" : "выхода"), params[4].dec, GetTypeStr(curRegType));
					existWarning = 1;
				} 
				if (IS_IN(curRegType) && curPattern.GetInput(curRegType, params[4].dec)->GetRegister((REGISTER_ID)params[0].data) == NULL)
				{
                    WriteCriticalWarning(params[0],
					"Попытка чтения несуществующего регистра %s со входа %d типа %s", GetIdName(params[0]), params[4].dec, GetTypeStr(curRegType));
					existWarning = 1;	
				} 
				else if (IS_OUT(curRegType) && curPattern.GetOutput(curRegType, params[4].dec)->GetRegister((REGISTER_ID)params[0].data) == NULL)
				{
                    WriteCriticalWarning(params[0],
					"Попытка чтения несуществующего регистра %s со выхода %d типа %s", GetIdName(params[0]), params[4].dec, GetTypeStr(curRegType));
					existWarning = 1;	
				} else if (curChannel != NULL)
				{
					curChannel->AddParam((short)curRegType);
					curChannel->AddParam((short)params[4].dec);
					curChannel->AddParam(params[0].data);
				}
			}
			curRegType = ioError;
		}
			break;
		}
	}
		break;

	case RULE_EXPR___OP_AND: 
		break;
	case RULE_OP_AND___OP_BINOR: 
		break;
	case RULE_OP_BINOR___OP_BINXOR: 
		break;
	case RULE_OP_BINXOR___OP_BINAND: 
		break;
	case RULE_OP_BINAND___OP_EQUATE: 
		break;
	case RULE_OP_EQUATE___OP_COMPARE: 
		break;
	case RULE_OP_COMPARE___OP_ADD: 
		break;
	case RULE_OP_ADD___OP_MULT: 
		break;
	case RULE_OP_MULT___OP_MULT_DIV_OP_UNARY: 
	case RULE_OP_MULT___OP_MULT_PERCENT_OP_UNARY: 
		if (exprStack[exprStack.size() - 1].value == 0)
		{
            WriteCriticalWarning(params[0],
			"Деление на 0");
			existWarning = 1;			
		}
		else
			AddBinOpParams(rule);
		break;
	case RULE_EXPR___EXPR_PIPEPIPE_OP_AND: 
	case RULE_OP_AND___OP_AND_AMPAMP_OP_BINOR: 
	case RULE_OP_BINOR___OP_BINOR_PIPE_OP_BINXOR: 
	case RULE_OP_BINXOR___OP_BINXOR_CARET_OP_BINAND: 
	case RULE_OP_BINAND___OP_BINAND_AMP_OP_EQUATE: 
	case RULE_OP_EQUATE___OP_EQUATE_EQEQ_OP_COMPARE: 
	case RULE_OP_EQUATE___OP_EQUATE_EXCLAMEQ_OP_COMPARE: 
	case RULE_OP_COMPARE___OP_COMPARE_LT_OP_ADD: 
	case RULE_OP_COMPARE___OP_COMPARE_GT_OP_ADD: 
	case RULE_OP_COMPARE___OP_COMPARE_LTEQ_OP_ADD: 
	case RULE_OP_COMPARE___OP_COMPARE_GTEQ_OP_ADD: 
	case RULE_OP_ADD___OP_ADD_PLUS_OP_MULT: 
	case RULE_OP_ADD___OP_ADD_MINUS_OP_MULT: 
	case RULE_OP_MULT___OP_MULT_TIMES_OP_UNARY: 
		AddBinOpParams(rule);
		break;
	case RULE_OP_MULT___OP_UNARY: 
		break;
	case RULE_OP_UNARY___EXCLAM_OP_UNARY: 
		switch(curSection)
		{
		case sGeneral:
		case sInputs:
			exprStack.back().value = !exprStack.back().value;
			break;
		case sOutputs:
			if (exprStack.back().type == tsConstant)
				exprStack.back().value = !exprStack.back().value;
			else if (curChannel != NULL)
			{
				curChannel->AddParam((short)exprStack.back().value);	
				exprStack.back().value = (float)curChannel->AddAction(CProcessor::acNot);
			}
			break;
		}
		break;
	case RULE_OP_UNARY___MINUS_OP_UNARY: 
		switch(curSection)
		{
		case sGeneral:
		case sInputs:
			exprStack.back().value = -exprStack.back().value;
			break;
		case sOutputs:
			if (exprStack.back().type == tsConstant)
				exprStack.back().value = -exprStack.back().value;
			else if (curChannel != NULL)
			{
				curChannel->AddParam((short)exprStack.back().value);	
				exprStack.back().value = (float)curChannel->AddAction(CProcessor::acMinus);
			}
			break;
		}
		break;
	case RULE_OP_UNARY___VALUE: 
		break;
	case RULE_VALUE___DECLITERAL: 
		e.type = tsConstant;
		e.value = (float)params[0].dec;
		exprStack.push_back(e);
		break;
	case RULE_VALUE___FLOATLITERAL: 
		e.type = tsConstant;
		e.value = params[0].flo;
		exprStack.push_back(e);
		break;
	case RULE_VALUE___FUNCTION: 
		e.type = tsStack;
		if (curChannel != NULL)
			e.value = (float)curChannel->AddAction(CProcessor::acFunction);
		exprStack.push_back(e);
		break;
	case RULE_VALUE___REGISTER: 
		e.type = tsStack;
		if (curChannel != NULL)
			e.value = (float)curChannel->AddAction(CProcessor::acReadRegister);
		exprStack.push_back(e);
		break;
	case RULE_VALUE___REGISTERLABEL: 
		e.type = tsStack;
		if (curChannel != NULL)
			e.value = (float)curChannel->AddAction(CProcessor::acReadRegisterLabel);
		exprStack.push_back(e);
		break;
	case RULE_VALUE___ID: 
		if (curSection == sOutputs)
		{
			e.type = tsVariable;
			if (curChannel != NULL)
			{
				if (curChannel->ExistVar(params[0].data) == -1)
				{
					WriteCriticalWarning(params[0],
					"Переменная %s не определена", GetIdName(params[0]));
					existWarning = 1;			
				}
				e.value = curChannel->AddVar(params[0].data);
			}
			exprStack.push_back(e);
		} else
		{
            WriteError(params[0],
			"Использование переменных в секции INPUT невозможно", GetIdName(params[0]));
			return -1;
		}

		break;
	case RULE_VALUE___LPARAN_EXPR_RPARAN: 
		break;
	default:
		return -1;
	}
	return 0;
}


void CParserDevice::AddBinOpParams(RULES rule)
{
	float p1,p2;
	p1 = p2 = 1;
	switch(curSection)
	{
	case sGeneral:
	case sInputs:
		BinOpsByRule(rule, exprStack[exprStack.size() - 2].value, exprStack[exprStack.size() - 1].value);
		break;
	case sOutputs:
		if ((exprStack[exprStack.size() - 2].type == tsConstant) && (exprStack[exprStack.size() - 1].type == tsConstant))
			BinOpsByRule(rule, exprStack[exprStack.size() - 2].value, exprStack[exprStack.size() - 1].value);
		else if (curChannel != NULL)
		{
			if (exprStack.back().type == tsConstant)
			{
				curChannel->AddParam((short)exprStack[exprStack.size() - 2].value);
				curChannel->AddParam(-curChannel->AddConst(exprStack.back().value));
				exprStack[exprStack.size() - 2].type = tsStack;
			}
			else if (exprStack[exprStack.size() - 2].type == tsConstant)
			{
				curChannel->AddParam(-curChannel->AddConst(exprStack[exprStack.size() - 2].value));
				curChannel->AddParam((short)exprStack.back().value);
				exprStack[exprStack.size() - 2].type = tsStack;
			}
			else
			{
				curChannel->AddParam((short)exprStack[exprStack.size() - 2].value);
				curChannel->AddParam((short)exprStack.back().value);
			}
			exprStack[exprStack.size() - 2].value = (float)curChannel->AddAction(BinOpsByRule(rule, p1, p2));
		}
		break;
	}
	exprStack.pop_back();
}

CProcessor::ACTIONS CParserDevice::BinOpsByRule(RULES rule, float &p1, float p2)
{
	switch(rule)
	{
	case RULE_EXPR___EXPR_PIPEPIPE_OP_AND:
		p1 = p1 || p2;
		return CProcessor::acPipePipe;
	case RULE_OP_AND___OP_AND_AMPAMP_OP_BINOR:
		p1 = p1 && p2;
		return CProcessor::acAmpAmp;
	case RULE_OP_BINOR___OP_BINOR_PIPE_OP_BINXOR:
		p1 = (float)((int)p1 | (int)p2);
		return CProcessor::acPipe;
	case RULE_OP_BINXOR___OP_BINXOR_CARET_OP_BINAND:
		p1 = (float)((int)p1 ^ (int)p2);
		return CProcessor::acCaret;
	case RULE_OP_BINAND___OP_BINAND_AMP_OP_EQUATE:
		p1 = (float)((int)p1 & (int)p2);
		return CProcessor::acAmp;
	case RULE_OP_EQUATE___OP_EQUATE_EQEQ_OP_COMPARE:
		p1 = p1 == p2;
		return CProcessor::acEqEq;
	case RULE_OP_EQUATE___OP_EQUATE_EXCLAMEQ_OP_COMPARE:
		p1 = p1 != p2;
		return CProcessor::acNotEq;
	case RULE_OP_COMPARE___OP_COMPARE_LT_OP_ADD:
		p1 = p1 < p2;
		return CProcessor::acLt;
	case RULE_OP_COMPARE___OP_COMPARE_GT_OP_ADD:
		p1 = p1 > p2;
		return CProcessor::acGt;
	case RULE_OP_COMPARE___OP_COMPARE_LTEQ_OP_ADD:
		p1 = p1 <= p2;
		return CProcessor::acLtEq;
	case RULE_OP_COMPARE___OP_COMPARE_GTEQ_OP_ADD:
		p1 = p1 >= p2;
		return CProcessor::acGtEq;
	case RULE_OP_ADD___OP_ADD_PLUS_OP_MULT:
		p1 = p1 + p2;
		return CProcessor::acAdd;
	case RULE_OP_ADD___OP_ADD_MINUS_OP_MULT:
		p1 = p1 - p2;
		return CProcessor::acSub;
	case RULE_OP_MULT___OP_MULT_TIMES_OP_UNARY:
		p1 = p1 * p2;
		return CProcessor::acMul;
	case RULE_OP_MULT___OP_MULT_DIV_OP_UNARY:
		p1 = p1 / p2;
		return CProcessor::acDiv;
	case RULE_OP_MULT___OP_MULT_PERCENT_OP_UNARY:
		p1 = (float)((int)p1 % (int)p2);
		return CProcessor::acIntMul;
	default:
		break;
	};
	return CProcessor::acAssign;
}
// Возвращает Id ключевого слова, по номеру в массиве char* keyWords
CParserDevice::SYMBOLS CParserDevice::GetIdKeyWord(int id)
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
	case 4:
		return SYMBOL_ELSE;
	case 5:
		return SYMBOL_END;
	case 6:
		return SYMBOL_FREQUENCY;
	case 7:
		return SYMBOL_GENERAL;
	case 8:
		return SYMBOL_IF;
	case 9:
		return SYMBOL_INPUTS;
	case 10:
		return SYMBOL_LOGICALIN;
	case 11:
		return SYMBOL_NPT;
	case 12:
		return SYMBOL_OUTPUTS;
	case 13:
		return SYMBOL_POTENTIOMETR;
	case 14:
		return SYMBOL_RSIN;
	case 22:
		return SYMBOL_ANALOG;
	case 23:
		return SYMBOL_DIGITALOUT;
	case 24:
		return SYMBOL_LOGICALOUT;
	case 27:
		return SYMBOL_RSOUT;
	}
return SYMBOL_ERROR;
}

#ifndef _FAST_PARSER
short CParserDevice::GetNextParserState(short state, TOKEN t)
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


