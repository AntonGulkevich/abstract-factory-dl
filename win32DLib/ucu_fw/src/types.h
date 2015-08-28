// �������� ����� ������ ������������ � ���
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
    gtCourse = 1,   //  01  ����
    gtRoll = 2,     // 02   ����
    gtPitch = 3,    // 03   ������
    gtDTK = 4,      // 04   �������� ������� ����
    gtFAS = 5,      // 05   ��� (������� ��������� ���������)
    gtBearing = 6,  // 06   ������ ���
    gtCourseAngle = 7, // 07    �������� ���� ������������ (���)
    gtAS = 8,       // 08   ���� ����� (��)
    gtAltitude = 9,    //  11  ��� (��������������)
    gtRelAltitude = 10, // 12   ����
    gtAltitudeAbs = 11, //  ���� ���.
//    gtAltitudeAbsReserv = 12, //14  ���� ���.
    gtAltitudeEchelon = 12, //15    ��������
    gtVertSpeed = 13, //16  Vy ���.
    gtTrueSpeed = 14, // 20 V���
    gtRelSpeed = 15,//  V ����
    gtMACH  = 16, //    ���
    gtDeltaK = 17, //31 �
    gtDeltaG = 18, //  �
    gtWorkTime = 19, // 33   ����
	gtGroundSpeed = 20, // ������� ��������
	gtWx = 21, // ���������� ������������ ������� ��������
	gtWy = 22, // ������������ ������������ ������� ��������
	gtWz = 23, // ������� ������������ ������� ��������
	gtSost = 24, // ���������� ����������
	gtLBU = 25, // ���
	gtRange = 26, // ���������� ��
	gtImpCode = 27, // ���������� ���
	gtBipolCode = 28, // ���������� ���
	gtRollTrue = 29, //	���� ��������
	gtCourseGyro = 30, //	���� ���������������
	gtAngleDelta = 31, // 	�������� ���� �����������
	gtLatitudeDelta = 32, //	�������� ��������
	gtExternalCourse = 33, //	���� �������
	gtState = 99, // ����� ���������
};

enum UNITS
{
    uUndefined = 0,
    uNone = 1, // ������������
    uKm = 2, // ���������
    uDegree = 3, // �������
    uMeter = 4, // �����
    uKmph = 5, // ��\�
    uFeet = 6, // ����
    uDDM = 7,  // ���
    uSec = 8,   // �������
	uGrad = 9, // ������� �������
	uMile = 10, // ����
	uKt = 11, // ���� � ��� (����)
	uMps = 12, // ����� � �������
	uMach = 13, // ���
	uDegph =14, // ����/�
    uPoints = 15, // �����
	uFeetPerMin = 16, // ���� � ������
	uVolts = 17 // ������

};

// ������ ��������������� �������
enum FUNCTION_ID
{
	fTOBNR,//(value, ���������� ���, �������� �������� �������) val
	fFROMBNR,//(value, ���������� ���, �������� �������� �������) val
	fTOBCD,//(????)
	fFROMBCD,//(value) val
	fLINEAR,//(value, ��� ������ (� ������� �������� ������� ������.), ����� ������
	fSHOWALARM,// (����� ������, ��� ������, ��� ������ (0..F) void
	fSETBIT, // (val, num, 01);   val
	fGETBIT, // (val, num); 01
	fSETFIELD, // (val, num, count, val2);   val
	fGETFIELD, // (val, num, count); val
	fGETUCUSTATE, // �������� ��������� ��� ��� ����������
	fRESETRATE,		// �������� ������ �������� ��������� ���������
	fRESETRATECHANNEL,	// �������� ������ �������� ��������� ��������� � ��������� ������
	fISRATEREADY,	// ��������� ������ �������� ��������� ���������
	fRESETFILTR,	// �������� ������
	fTORTM,		//(value, ���������� ���, �������� �������� �������) val
	fFROMRTM,	//(value, ���������� ���, �������� �������� �������) val
	fAERODYNAMICCORR,  // (������, ��������)  ��������
	fSUPPRESSALARM,  // (����� ������, ��� ������, (1 - �������� / 0 - ���������) void
	fSIN,
	fCOS,
	fISFILTRREADY,   // ���������� 1 ���� ������ �����
	fTOBNRS,
	fFROMBNRS,
	fTORTMS,
	fFROMRTMS,
	fTRUNC,
	fFRAC,

};

// ������ ��������������� ���������
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
		UINT iostAnalogNoSignal        : 1; // ������� ��� �������
		UINT iostAnalogLowLevel        : 1; // ������� ������ ��
		UINT iostAnalogHighLevel       : 1; // ������� ����������
		UINT iostAnalogBreak           : 1; // ������� �����

		UINT iostAnalogSyncFail        : 1; // ����� �������������
		UINT iostAnalogFail		     : 1; // ����� ���
		UINT iostArincChNoSignal       : 1; // �����
		UINT iostArincWOutLimit        : 1; // ���������

		UINT iostArincWNoWord          : 1; // ���������
		UINT iostArincBadParity        : 1; // ���������
		UINT iostAnalogDACFail         : 1; // ����� ���
		UINT iostAnalogDACOverload     : 1; // ���������� ���

		UINT iostPotentADCFail         : 1; // ������� ������������
		UINT iostFactoryFail           : 1; // ���������� �����
		UINT iostFiltrNotReady         : 1; // ������ �� ������ �����
		UINT iostAnalogRefFail         : 1; // ��������� ����� ���

		UINT iostAnalogRefOverload     : 1; // ���������� ����� ���
		UINT iostUserFail			   : 1; // ���������������� �����
		UINT iostArincBreak		       : 1; // ����� ������ �����
		UINT iostCommandFail           : 1; // ����� ��

		UINT iostAnalogSingleDACFail   : 1; // ��������� ����� ���������� �� ������������
	};
	DWORD dword;
};

union IO_GROUP_STATE {
    struct {
      UINT iostAnalog20msFail         : 1; // ������� ���������� 20 ��
      UINT iostAnalogADCFail         : 1; // ����� ���
	  UINT iostSDADCRefFail          : 1; // ����� �����
	  UINT iostSync1Fail	         : 1; // ����� �������������
	  UINT iostSync2Fail	         : 1; // ����� �������������
	  UINT iostSync3Fail	         : 1; // ����� �������������
	  UINT iostSync4Fail	         : 1; // ����� �������������
	  UINT iostSync5Fail	         : 1; // ����� �������������
	  UINT iostSync6Fail	         : 1; // ����� �������������
	  UINT iostSync7Fail	         : 1; // ����� �������������
	  UINT iostArincIn1Fail			 : 1; // ����� ������ ARINC
	  UINT iostArincIn2Fail			 : 1; // ����� ������ ARINC
	  UINT iostArincIn3Fail			 : 1; // ����� ������ ARINC
	  UINT iostArincIn4Fail			 : 1; // ����� ������ ARINC
	  UINT iostArincOut1Fail			 : 1; // ����� ������ ARINC
	  UINT iostArincOut2Fail			 : 1; // ����� ������ ARINC
	  UINT iostArincOut3Fail			 : 1; // ����� ������ ARINC
	  UINT iostArincOut4Fail			 : 1; // ����� ������ ARINC
	  UINT iostArincIn5Fail			 : 1; // ����� ������ ARINC
	  UINT iostArincIn6Fail			 : 1; // ����� ������ ARINC
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


// ������ ������ �� ����� ��
enum class Flags
{
	Irq20ms, // 20 �� ����������
	SendDiagnostic, // �������� �����������
	Timer80ms, // 80 ��
	Timer500ms, // 500 ��
	CommandPCReceived, // ������� �� �� ��������
	DataPCReceived, // ������ �� �� ��������
	Timer1Sec, // ����
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
