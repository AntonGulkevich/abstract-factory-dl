#ifndef CPARSER_H
#define CPARSER_H

#include <cstring>
#include <string>
#include <vector>
#include "../types.h"

// ������� �����. ����� ����������� ������������ ���
// 
// 
// 
#define _FAST_PARSER

#define WRITEERROR      manager.utils.GetTerminal().WriteError
#define WRITEWARNING    manager.utils.GetTerminal().WriteWarning
#define WRITEOK         manager.utils.GetTerminal().WriteOk

class Console;
class Commod;



class CParser
{
public:
	// ������ ��������� ����� �������
	enum TOKENCLASS
	{
		tcId,
		tcString,
		tcFloat,
		tcDec,
		tcSeparator,
		tcKeyWord,
		tcRegister,
		tcFunction,
		tcNoTerminal,
		tcNoPrint,
		tcEnd,
		tcError
	};
	
	// �������� ��������� ������
	typedef struct _TOKEN
	{
		TOKENCLASS tc;	// ����� ������
		BYTE data;
		UINT dec;		// ������ ������
		float flo;
		char* str;
#ifdef _UCU_EMULATION
		BYTE symNum;	// ����� ������� � ������
		WORD strNum;	// ����� ������
#endif

	} TOKEN;

	enum TYPESTACK
	{
		tsVariable,
		tsStack,
		tsConstant,
		tsFuncParam,
		tsLabel
	};
	// ���������� ������ �������
	enum PARSE_RESULT
	{
		prOk,		// ������������ ������� ���������
		prError		// ��� ������� ������������ ���� ��������� ������
	};

	// �����������
	CParser(Console* console, Commod* commod);
	// ����������
	virtual ~CParser();
	// ������� ������� �� ����������� ������� -- (����������� �� ��� ����������)
	virtual void Clear();


	int LoadConfiguration();

	static const char* GetTypeStr(IOTYPES type);
	static const char* GetTypeStrRus(IOTYPES type);


	static const char* keyWords[];

protected:
	Console* _console;
	Commod* _commod;

	std::string& GetIdName(BYTE index)	{ return id[index]; }
	//std::string& GetStrName(BYTE index)	{ return str[index]; }

	virtual BYTE GetSymbolType(char ch) = 0;
	virtual short GetNextParserState(short state, TOKEN t) = 0;
	virtual short GetNextScannerState(short state, char ch) = 0;
	virtual TOKEN ReduceSym(short state) = 0;
	virtual int ReduceRule(short rule) = 0;
	
	int StartParsing();
    void WriteError(TOKEN t, const char* format, ...);
    void WriteCriticalWarning(TOKEN t, const char* format, ...);
    void WriteWarning(TOKEN t, const char* format, ...);

    TOKEN GetNextToken();
	short IndexOfList(const char* llist[]);
	short IndexOfId(char* str);
	void Push(char ch);
	virtual int DefineSeparator(char* str) = 0;
	int PopSeparator();
	int PopString();
	int PopDec();
	int PopFloat();
	int PopId(TOKENCLASS &tc);
	TOKEN AddToken(TOKENCLASS tc, BYTE data);
	void PrintTokenFlow();

	std::vector<std::string> id;
	float numFloat;
	UINT numDec;
	
	std::vector<char*> str;

	char stack[1024];
	WORD stackPtr;

#ifdef _UCU_EMULATION
	WORD curStr;
	BYTE curSym;
#endif
	
	int Parse();
	int WriteResult(int res);
	const char* GetIdName(TOKEN t);

	std::vector<TOKEN> tokenStack;
	TOKEN params[10];  // ��������� ��� �������� � ������� AddAction()
	char curCh;

	IOTYPES curIOType;		// ������� ��� ����� ������
	BYTE curIO;			// ������� ����� ����� ������
	int existWarning; // ���� ���� ����������� �������� �� 1
	BYTE device;  // 0 �� ������ ������������
    BYTE breakCount;  // �������� ���������� � ����������




};



#endif /* CPARSER_H*/

