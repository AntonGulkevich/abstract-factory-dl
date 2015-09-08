#ifndef ICPARCER
#define ICPARCER

#include "../types.h"

class Console;
class Commod;

class ICParser
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
	virtual ~ICParser(){};
	virtual void Clear() = 0;
	virtual int LoadConfiguration() = 0;
};
#endif
