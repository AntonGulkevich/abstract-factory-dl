#ifndef ICPARCER
#define ICPARCER

#include "../types.h"

#define _PARSER_FULLDIAGNOSTIC

class Console;
class Commod;

struct ICParser
{
	// Список возможных типов токенов
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
	// Результаты работы Парсера
	enum PARSE_RESULT
	{
		prOk,		// Конфигурация успешно разобрана
		prError		// При разборе конфигурации есть фатальные ошибки
	};
	// Описание структуры токена
	typedef struct _TOKEN
	{
		TOKENCLASS tc;	// Класс токена
		BYTE data;
		UINT dec;		// Данные токена
		float flo;
		char* str;
#ifdef _PARSER_FULLDIAGNOSTIC
		BYTE symNum;	// номер символа в строке
		WORD strNum;	// номер строки
#endif

	} TOKEN;
	virtual ~ICParser(){};
	virtual void Clear() = 0;
	virtual int LoadConfiguration() = 0;
};
#endif
