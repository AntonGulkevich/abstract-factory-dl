#ifndef CCHANNEL_H_HEADER_INCLUDED_B9E8CA69
#define CCHANNEL_H_HEADER_INCLUDED_B9E8CA69

#include <vector>

#include "../driversio/ichannelout.h"

const short STACKPTR = 32767;

class CPattern;
class IChannel;

//##ModelId=4617524C01FE
class CProcessor
{
public:
    // ������ ������ ��������������
    enum ACTIONS
	{
		acAdd,
		acSub,
		acMul,
		acIntMul,
		acDiv,
		acMinus,
		acNot,
		acGtEq,
		acGt,
		acLtEq,
		acLt,
		acEqEq,
		acNotEq,
		acAmp,
		acCaret,
		acPipe,
		acAmpAmp,
		acPipePipe,
		acAssign,
		acAssignConst,
		acAssignRegister,
		acAssignRegisterConst,
		acAddStack,
		acJz,
		acJmp,
		acFunction,
		acReadRegister,
		acReadRegisterLabel,
        acReadSelfRegisterLabel,
		acReadSelfRegister,
		acLoadBinParamVarVar,
		acLoadBinParamVarConst,
		acLoadBinParamVarStack,
		acLoadBinParamConstVar,
		acLoadBinParamConstConst,
		acLoadBinParamConstStack,
		acLoadBinParamStackVar,
		acLoadBinParamStackConst,
		acLoadBinParamStackStack,
		acLoadParamStack,
		acLoadParamVar,
		acSaveParamStack,
		acSaveParamVar,
		acAddStackVar,
		acAddStackConst,

	};

	
	union ACTION_TRIADA
	{
		struct 
		{
			ACTIONS act;
			float param1;
			float param2;
		};
		struct
		{
			ACTIONS act;
			DWORD ptr1;	// ����� ������� ���������
			DWORD ptr2;
		} ptrs;
	};


	CProcessor(IChannelOut* channel);
	virtual ~CProcessor();	
    // ������� ��������� ����������
    void Clear();
	void Convert();
	/*void Save();
	void Load();*/
	void Init();
#ifndef _NO_COMPILIER
	// ���������� �������� � ��������������
	int AddAction(ACTIONS id);
	// ���������� ��������� � ��������������
	BYTE AddConst(float num);
	// �������� ���������� � ��������������
	BYTE AddVar(BYTE id);
	// �������� �������� � ��������������
	BYTE AddParam(short id);
	// �������� ����� � ��������������
	UINT AddLabel()	{ labelsV.push_back(-1); return (UINT)(labelsV.size() - 1);} 
	// ���������� ����� ������ ��� �����
	void SetLabel(int labelNum);
	// ��������� ������������ ���������� ���������� ��� �������
	BYTE TestFunctionParam(int fId);
	// ����� ���������� � ������ �� ������
	short ExistVar(BYTE id);

    void Analys();
    void Compile();
    void GetDependentChannels(std::vector<IChannel*>& channels);
    bool IsCalculated()     { return calculated; } // ��� ���������
    void ResetCalculated()  { calculated = false; }

#endif

	void Instance();
	void SetConstant()	{ constant = true; }

    static DWORD GetBit(DWORD val, UINT bit);
    static DWORD SetBit(DWORD val, UINT bit, UINT bitval);
    static DWORD GetField(DWORD val, UINT startbit, UINT count);
    static DWORD SetField(DWORD val, UINT startbit, UINT count, DWORD valfield);
	static UCU_FLOAT FromBNR(UINT val, UINT count, UCU_FLOAT range);
	static UCU_FLOAT FromRTM(UINT val, UINT count, UCU_FLOAT range);
	static int ToBNR(UCU_FLOAT val, UINT count, UCU_FLOAT range);
	static int ToRTM(UCU_FLOAT val, UINT count, UCU_FLOAT range);
	UINT ToBCD(UCU_FLOAT val, UINT fillBits);

private:

	IChannelOut* _channel;

    // ��������� �������
	//void DoAction(ACTION_TRIADA id);
	// ��������� �������
	//void DoFunction(ACTION_TRIADA id);
	//void CalcExpr(ACTIONS id);
	// ���������� � ����
	void exprPush(UCU_FLOAT val)	{ expr[exprPtr++] = val; }
	UCU_FLOAT exprPop()	{ return expr[--exprPtr]; }
#ifndef _NO_COMPILIER
	// ������ ������ � ��������������
	std::vector<int> actionsV;
	// ������ ����������
	std::vector<int> paramsV;
	// ���� ���������� ���������
	// ������ ��������
	std::vector<float> constsV;
	// ������ ����� � ��������������
	std::vector<int> labelsV;
#endif

    UCU_FLOAT* expr;
    WORD exprPtr;
    WORD exprSize;
    // ������ ���������� ��������������
    std::vector<UCU_FLOAT> varsV;
    
    std::vector<ACTION_TRIADA> actionsTr;
    // ������ ���������� �������� �� ������
    //std::vector<IO_CACHE> ios;

	// ��������� �� ������� �������� � ������
	WORD paramPtr;
	float result;
	// ��������� �� ������� ������� � ��������������
	WORD actionPtr;

    bool calculated;
	bool constant;
	bool cleared;
};



#endif /* CCHANNEL_H_HEADER_INCLUDED_B9E8CA69 */
