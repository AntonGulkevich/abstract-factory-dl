#ifndef APPLICATION_CPATTERN_H_HEADER_INCLUDED_B9C120EB
#define APPLICATION_CPATTERN_H_HEADER_INCLUDED_B9C120EB

#include <vector>

#include "cconfiguration.h"
#include "../driversio/ichannelin.h"
#include "../driversio/ichannelout.h"

class DriversIOManager;
class Diagnostic;

const UINT PATTERN_COUNTR = 1;
#define IS_IN(data) ((data & 0xF0) == 0)
#define IS_OUT(data) ((data & 0xF0) == 0x10)
#define IS_LOGICAL(data)  ((data & 0xF) == 0xF)


// Класс хранящий конфигурацию одного устройства.
class CPattern
{
public:
	enum class ADDIO_RESULT
	{
		OK,
		AlreadyExist,
		Error
	};

	enum class VALIDIO_RESULT
	{
		OK,
		NotExist,
		AlreadyConfigured,
		NC,
	};


	CPattern(DriversIOManager* driversIO, CConfiguration* config, UINT lSelfNum = 0xFF, bool lConstant = false);
	virtual ~CPattern();
	void SetDeviceNum(BYTE num)	{ deviceNum = num;	}
	BYTE GetDeviceNum()	{  return deviceNum;	}
	BYTE GetSelfNum()	{  return selfNum;	}
	UINT GetPINSCount()	{ return (UINT)(ins.size() + outs.size()); }
	ADDIO_RESULT AddIn(IOTYPES type, BYTE num, BYTE label = 0xFF);
	ADDIO_RESULT AddOut(IOTYPES type, BYTE num, BYTE label = 0xFF);
	ADDIO_RESULT AddNCIO(IOTYPES type, BYTE num);
	IChannelIn* GetInput(IOTYPES type, UINT num);
	IChannelOut* GetOutput(IOTYPES type, UINT num);
	bool IsNC(IOTYPES type, BYTE num);
	CProcessor* GetChannel(IOTYPES type, BYTE num);
	CProcessor* GetChannelLabel(BYTE num, BYTE label);

	int InitDevice();
	void ClearDevice();
    void CompileDevice();
    void InstanceDevice();
	bool IsConstant()	{ return constant; }
   

	VALIDIO_RESULT ValidIO(IOTYPES type, UINT index);		// inOut - 0 входы, 1 - выходы
	// Возвращает указатель на регистр по ID
	CRegister* GetRegister(REGISTER_ID id) { return (id != REGISTER_ID::NULLID) ? registers_t[(UINT)id].reg : NULL; }
	// Возвращает указатель на параметры регистров по ID
	const IChannel::REGISTER_TYPE* GetRegisterT(REGISTER_ID id) { return (id != REGISTER_ID::NULLID) ? &registers_t[(UINT)id] : NULL; }
	CConfiguration* GetConfig() { return _config;}
private:
	friend void CConfiguration::PrepareLogicSequence();
	friend Diagnostic;

	int IndexOfIns(IOTYPES type, BYTE num);
	int IndexOfOuts(IOTYPES type, BYTE num);

	std::vector<IChannelIn*>& GetAllInputs() { return ins; }
	std::vector<IChannelOut*>& GetAllOutputs() { return outs; }

	struct PIN
	{
		IOTYPES type;
		BYTE num;
	};
	struct POUT
	{
		IOTYPES type;
		BYTE num;
		//CProcessor* proc;
	};
	IChannel::REGISTER_TYPE registers_t[(UINT)REGISTER_ID::COUNTREGISTERS];

	BYTE deviceNum; // Номер устройства в базе
	BYTE selfNum;	// Собственный номер в списке
	std::vector<IChannelIn*> ins;
	std::vector<IChannelOut*> outs;
	std::vector<IChannel*> ncio;
    bool constant;
    CConfiguration* _config;
    DriversIOManager* _driversIO;

};



#endif /* APPLICATION_CPATTERN_H_HEADER_INCLUDED_B9C120EB */
