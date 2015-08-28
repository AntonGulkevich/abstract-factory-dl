#ifndef CPARSERCONFIG_H
#define CPARSERCONFIG_H
#include "CParser.h"

class CConfiguration;
class Console;
class Commod;
class DriversIOManager;

// Класс разбирающий таблицу соединений
class CParserConfig : public CParser
{
public:
	CParserConfig(CConfiguration* conf, Console* console, Commod* commod, DriversIOManager* driversIO) :
		CParser(console, commod), config(conf), _driversIO(driversIO) 	{ device = 0; curSection = sGeneral; count = 0; }
	virtual ~CParserConfig()	{}
	void Clear();
private:
	enum DEVICETYPE
	{
		dtIn,
		dtOut,
		dtPattern,
		dtPatternConst
	};

    enum SECTION
    {
        sGeneral,
        sDevices,
        sConnections
    };

	struct PIN
	{
		DEVICETYPE dt;
		IOTYPES type;
		BYTE patternNum;  // если dt != dtPattern то параметр не учитывается
		BYTE pinNum;
	};

	static short scannerContrTable[17][12];

	BYTE GetSymbolType(char ch);
	int DefineSeparator(char* str);
	short GetNextScannerState(short state, char ch)	{return scannerContrTable[state][GetSymbolType(ch)];}
	TOKEN ReduceSym(short state);

	#include "CParserConfig_h.inc" 

	int ReduceRule(short rule);
	int AddAction(RULES rule);
	int IndexOfDevice(short num);
	SYMBOLS GetIdKeyWord(int id);
	SYMBOLS GetTokenSymbol(TOKEN t);
#ifdef _FAST_PARSER
	short GetNextParserState(short state, TOKEN t)	{ return contrTable[state][GetTokenSymbol(t)]; }
#else
	short GetNextParserState(short state, TOKEN t);
#endif
	std::vector<int> deviceNums;
    SECTION curSection;  // Текущая секция
	BYTE count; // Количество паттернов
	PIN dest;
	PIN src;

	CConfiguration* config;
	DriversIOManager* _driversIO;

};



#endif /* UTILITIES_CPARSERCONFIG_H_HEADER_INCLUDED_B9BBE4A2 */
