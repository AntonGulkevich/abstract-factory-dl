#ifndef CPARSERDEVICE_H
#define CPARSERDEVICE_H
#include <vector>

#include "cparser.h"
#include "../application/cprocessor.h"

class CPattern;
class CConfiguration;

// Класс разбирающий описание устройств.
class CParserDevice : public CParser
{
public:
	CParserDevice(BYTE lDevice, CConfiguration* config, CPattern& lPattern, Console* console, Commod* commod);
	virtual ~CParserDevice()	{}
	void Clear();
private:
	struct EXPRESSION
	{
		float value;
		TYPESTACK type;
	};
    enum SECTION
    {
        sGeneral,
        sInputs,
        sOutputs,
    };

	static short scannerContrTable[44][27];
	int DefineSeparator(char* str);
	BYTE GetSymbolType(char ch);
	short GetNextScannerState(short state, char ch)	{return scannerContrTable[state][GetSymbolType(ch)];}
	TOKEN ReduceSym(short state);

	
	/// Generated code
#include "CParserDevice_h.inc" 

	SYMBOLS GetIdKeyWord(int id);
	SYMBOLS GetTokenSymbol(TOKEN t);
#ifdef _FAST_PARSER
	short GetNextParserState(short state, TOKEN t)	{ return contrTable[state][GetTokenSymbol(t)]; }
#else
	short GetNextParserState(short state, TOKEN t);
#endif
	int AddAction(RULES rule);
	CProcessor::ACTIONS BinOpsByRule(RULES rule, float &p1, float p2);
	void AddBinOpParams(RULES rule);  // Добавляет 2 параметра для операции, проверяя их на константы
	int ReduceRule(short rule);
	short Reverse(UINT label);

	std::vector<EXPRESSION> exprStack;
	std::vector<float> arrayBuff;
    SECTION curSection;  // Текущая секция
	CProcessor* curChannel;
	CConfiguration* _config;
	CPattern& curPattern;
	const char* ioSecStr;   // строка (входа или выхода)
	short curLabel;		// Текущая метка для ARINC
	short curWordNum;	// Текущая номер метка для ARINC
	IOTYPES curRegType;		// Текущий тип входа выхода
	bool inParams;
	bool parsingNC;  // В данный момент разбирается неподключенный вход
	bool parsingNCLabel;  // В данный момент разбирается неподключенный выход (метка)
};



#endif /* CPARSERDEVICE_H */
