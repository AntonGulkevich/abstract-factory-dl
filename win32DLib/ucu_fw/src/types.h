// Описание типов данных используемых в УБС
#ifndef TYPES_H_
#define TYPES_H_
#include <cstring>
//#define _NO_COMPILIER

typedef unsigned int        UINT;
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
typedef unsigned long long  QWORD;
typedef float               UCU_FLOAT;



#define VERSION_NUM 5
#define SUBVERSION_NUM 1
#define PBV 15
#define UCUNULL NULL

#ifndef EMULATION
#define EMULATION


#endif


//#define PLD_MEMORY_CONTROL

#ifdef PLD_MEMORY_CONTROL

#include "pldmemorycontrol.h"

#define UCU_IORD_32DIRECT(base, offset) memControl.UcuRead(base, offset)
#define UCU_IOWR_32DIRECT(base, offset, data) memControl.UcuWrite(base, offset, data)

#define UCU_IORD_32DIRECTF(base, offset) memControl.UcuReadF(base, offset)
#define UCU_IOWR_32DIRECTF(base, offset, data) memControl.UcuWriteF(base, offset, data)

#else

#define UCU_IORD_32DIRECT(base, offset) (*(volatile DWORD*)((base) + (offset)))
#define UCU_IOWR_32DIRECT(base, offset, data) (*(volatile DWORD*)((base) + (offset))) = data

#define UCU_IORD_32DIRECTF(base, offset) (*(volatile float*)((base) + (offset)))
#define UCU_IOWR_32DIRECTF(base, offset, data) (*(volatile float*)((base) + (offset))) = data


#endif


const float MAX_FLOAT =1E+29f;
const float MIN_FLOAT = -1E+29f;
//#define _NO_COMPILIER

enum IOTYPES
{
    ioDigitalIn = 0x00,
	ioAnalog = 0x01,
	ioPotentiometr = 0x02,
	ioRSIn = 0x03,
	ioCheck = 0x04,
	ioLogicalIn = 0x0F,
	ioSCT = 0x10,
	ioNPT = 0x11,
	ioCommand = 0x12,
	ioDigitalOut = 0x13,
    ioRSOut = 0x14,
	ioLogicalOut = 0x1F,
    ioArincWordIn = 0x0E,
    ioArincWordOut = 0x1E,
	ioError = 0xFF,

};



enum KEYS
{
    kNone = 0,
    kKey1 = 1,
    kKey2 = 2,
    kKey3 = 4,
    kKey4 = 8,
    kEnter = 1,
    kDown = 2,
    kUp = 4,
    kEsc = 8
};

enum COLOR
{
    clBlack,
    clRed,
    clGreen,
    clYellow,
    clBlue,
    clMagenta,
    clCyan,
    clWhite
};

enum GROUP_TYPE
{
    gtUnknown = 0,
    gtCourse = 1,   //  01  Курс
    gtRoll = 2,     // 02   Крен
    gtPitch = 3,    // 03   Тангаж
    gtDTK = 4,      // 04   Заданный путевой угол
    gtFAS = 5,      // 05   УПЗ (угловое положение закрылков)
    gtBearing = 6,  // 06   Пеленг ВОР
    gtCourseAngle = 7, // 07    Курсовой угол радиостанции (КУР)
    gtAS = 8,       // 08   Угол сноса (УС)
    gtAltitude = 9,    //  11  Нрв (радиовысотомер)
    gtRelAltitude = 10, // 12   Нотн
    gtAltitudeAbs = 11, //  Набс осн.
//    gtAltitudeAbsReserv = 12, //14  Набс рез.
    gtAltitudeEchelon = 12, //15    Нэшелона
    gtVertSpeed = 13, //16  Vy выч.
    gtTrueSpeed = 14, // 20 Vист
    gtRelSpeed = 15,//  V приб
    gtMACH  = 16, //    Мах
    gtDeltaK = 17, //31 К
    gtDeltaG = 18, //  Г
    gtWorkTime = 19, // 33   Тнар
	gtGroundSpeed = 20, // Путевая скорость
	gtWx = 21, // продольная составляющая путевой скорости
	gtWy = 22, // вертикальная составляющая путевой скорости
	gtWz = 23, // боковая составляющая путевой скорости
	gtSost = 24, // оставшееся расстояние
	gtLBU = 25, // ЛБУ
	gtRange = 26, // Расстояние до
	gtImpCode = 27, // Импульсный код
	gtBipolCode = 28, // Биполярный код
	gtRollTrue = 29, //	Крен заданный
	gtCourseGyro = 30, //	Курс гироскопический
	gtAngleDelta = 31, // 	Поправка угла визирования
	gtLatitudeDelta = 32, //	Поправка широтная
	gtExternalCourse = 33, //	Курс внешний
	gtState = 99, // Слово состояния
};

enum UNITS
{
    uUndefined = 0,
    uNone = 1, // Безразмерная
    uKm = 2, // Километры
    uDegree = 3, // Градусы
    uMeter = 4, // Метры
    uKmph = 5, // Км\ч
    uFeet = 6, // Футы
    uDDM = 7,  // ДДМ
    uSec = 8,   // Секунды
	uGrad = 9, // Градусы Цельсия
	uMile = 10, // Мили
	uKt = 11, // Мили в час (узлы)
	uMps = 12, // Метры в секунду
	uMach = 13, // Мах
	uDegph =14, // Град/ч
    uPoints = 15, // Точки
	uFeetPerMin = 16, // Футы в минуту
	uVolts = 17 // Вольты

};

// Список идентификаторов функций
enum FUNCTION_ID
{
	fTOBNR,//(value, количество бит, значение старшего разряда) val
	fFROMBNR,//(value, количество бит, значение старшего разряда) val
	fTOBCD,//(????)
	fFROMBCD,//(value) val
	fLINEAR,//(value, тип канала (в котором хранится таблица линеар.), номер канала
	fSHOWALARM,// (номер канала, тип канала, код отказа (0..F) void
	fSETBIT, // (val, num, 01);   val
	fGETBIT, // (val, num); 01
	fSETFIELD, // (val, num, count, val2);   val
	fGETFIELD, // (val, num, count); val
	fGETUCUSTATE, // Получить состояние УБС без параметром
	fRESETRATE,		// Сбросить расчет скорости изменения параметра
	fRESETRATECHANNEL,	// Сбросить расчет скорости изменения параметра в указанном канале
	fISRATEREADY,	// Состояние буфера Скорости изменения параметра
	fRESETFILTR,	// Сбросить фильтр
	fTORTM,		//(value, количество бит, значение старшего разряда) val
	fFROMRTM,	//(value, количество бит, значение старшего разряда) val
	fAERODYNAMICCORR,  // (высота, скорость)  поправка
	fSUPPRESSALARM,  // (номер канала, тип канала, (1 - подавить / 0 - разрешить) void
	fSIN,
	fCOS,
	fISFILTRREADY,   // возвращает 1 если фильтр готов
	fTOBNRS,
	fFROMBNRS,
	fTORTMS,
	fFROMRTMS,
	fTRUNC,
	fFRAC,

};

// Список идентификаторов регистров
enum class REGISTER_ID
{
	rSPEED, // 0
	rNAME,
	rVERSION,
	rVALUE,
	rSTATE,
	rMATRIX, // 5
	rTYPE,
	rDELAY,
	rSYNCNUM,
	rPERIODMAX,
	rPERIODMIN, //10
	rCONTROL,
	rID,
	rDELTA,
	rFILTRBASE,
	rMAXLEVEL, // 15
	rMINLEVEL,
	rUNIT,
	rVALUE1,
	rVALUE2,
	rVALUE3,//20
	rGROUP,
	rTARMIN,
	rTARMAX,
	rPASSPORT,
	rINTERVAL,
	rDEADZONE,
	rCORRECTIONDELTA,
	rCORRECTIONSIGN,
	rSUBGROUP,
	rFILTRTYPE,
	rRTCNUM,
	rRTCVAL,
	rPARITYTYPE,
	rSTOPBIT,
	rDATABASENUM,
	rDATAARRAY,
	rRATE,
	rRATEBASE,
	rSETTINGSNUM,
	rBITSCOUNT,
	rRANGE,
	rREVERSE,
	rNOTFILTREDVALUE,
	rFILTRREADY,
	rRATEREADY,
	COUNTREGISTERS,
	NULLID
};

union IOSTATE
{
	struct
	{
		UINT iostAnalogNoSignal        : 1; // Входные нет сигнала
		UINT iostAnalogLowLevel        : 1; // Входные низкий ур
		UINT iostAnalogHighLevel       : 1; // Входной превышение
		UINT iostAnalogBreak           : 1; // Входные обрыв

		UINT iostAnalogSyncFail        : 1; // Отказ синхронизации
		UINT iostAnalogFail		     : 1; // Отказ АЦП
		UINT iostArincChNoSignal       : 1; // Аринк
		UINT iostArincWOutLimit        : 1; // АринкВорд

		UINT iostArincWNoWord          : 1; // АринкВорд
		UINT iostArincBadParity        : 1; // АринкВорд
		UINT iostAnalogDACFail         : 1; // Отказ ЦАП
		UINT iostAnalogDACOverload     : 1; // Перегрузка ЦАП

		UINT iostPotentADCFail         : 1; // Входной потенциометр
		UINT iostFactoryFail           : 1; // Аппаратный отказ
		UINT iostFiltrNotReady         : 1; // Фильтр не набрал буфер
		UINT iostAnalogRefFail         : 1; // Единичный опоры ЦАП

		UINT iostAnalogRefOverload     : 1; // Постоянный опоры ЦАП
		UINT iostUserFail			   : 1; // Пользовательский отказ
		UINT iostArincBreak		       : 1; // Обрыв канала аринк
		UINT iostCommandFail           : 1; // Отказ РК

		UINT iostAnalogSingleDACFail   : 1; // Одиночный отказ отклонение от контрольного
	};
	DWORD dword;
};

union IO_GROUP_STATE {
    struct {
      UINT iostAnalog20msFail         : 1; // Входные аналоговые 20 мс
      UINT iostAnalogADCFail         : 1; // Отказ АЦП
	  UINT iostSDADCRefFail          : 1; // Отказ опоры
	  UINT iostSync1Fail	         : 1; // отказ синхронизации
	  UINT iostSync2Fail	         : 1; // отказ синхронизации
	  UINT iostSync3Fail	         : 1; // отказ синхронизации
	  UINT iostSync4Fail	         : 1; // отказ синхронизации
	  UINT iostSync5Fail	         : 1; // отказ синхронизации
	  UINT iostSync6Fail	         : 1; // отказ синхронизации
	  UINT iostSync7Fail	         : 1; // отказ синхронизации
	  UINT iostArincIn1Fail			 : 1; // Отказ канала ARINC
	  UINT iostArincIn2Fail			 : 1; // Отказ канала ARINC
	  UINT iostArincIn3Fail			 : 1; // Отказ канала ARINC
	  UINT iostArincIn4Fail			 : 1; // Отказ канала ARINC
	  UINT iostArincOut1Fail			 : 1; // Отказ канала ARINC
	  UINT iostArincOut2Fail			 : 1; // Отказ канала ARINC
	  UINT iostArincOut3Fail			 : 1; // Отказ канала ARINC
	  UINT iostArincOut4Fail			 : 1; // Отказ канала ARINC
	  UINT iostArincIn5Fail			 : 1; // Отказ канала ARINC
	  UINT iostArincIn6Fail			 : 1; // Отказ канала ARINC
	};
    DWORD dword;
};

enum RW
{
	rwError,
	rwConstant,
	rwVariable,
	rwUser
};
enum RTYPE
{
	rtString,
	rtDec,
	rtFloat,
	rtArray
};
enum class FILTR_TYPE
{
	ftNone,
	ftBessel,
    ftWindow
};


// Список флагов по всему ПО
enum class Flags
{
	Irq20ms, // 20 мс прерывание
	SendDiagnostic, // Отправка диагностики
	Timer80ms, // 80 мс
	Timer500ms, // 500 мс
	CommandPCReceived, // Команда от ПК получена
	DataPCReceived, // Данные от ПК получены
	Timer1Sec, // Часы
	Count,

};

enum DISPATH_RESULT
{
	drNotDispatched = 0,
	drDispatched = 1,
	drAlreadyDispatched = 2
};

enum class PRIORITY
{
	High,
	Normal,
	BackGround,
	Count,
};

extern const char* registers[];
extern const char* functions[];

#define REVERSE(X)  (((X&1) << 7)   | ((X&2) << 5)   | \
                    ((X&4) << 3)    | ((X&8) << 1)   | \
                    ((X&16) >> 1)  | ((X&32) >> 3) | \
                    ((X&64) >> 5)  | ((X&128) >> 7))

#endif /* TYPES_H_ */
