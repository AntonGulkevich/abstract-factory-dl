#ifndef IDRIVERSIOMANAGER
#define IDRIVERSIOMANAGER
#include "../types.h"

class ArincWordIn;
class ArincWordOut;
class ArincOut;
class DescreeteOutput;
class ArincIn;
class DescreeteInput;
class Potentiometr;
class Calibrate;
class RSFieldIn;
class RSFieldOut;

struct IDriversIOManager
{
	virtual ~IDriversIOManager() {};
	virtual ADCControl* GetADCControl() const = 0;
	virtual Calibrate* GetCalibrate() const = 0;
	virtual AnalogIn* GetAnalogIn(UINT index) = 0;
	virtual AnalogIn* GetSCTControl(UINT index) = 0;
	virtual NptControl* GetNPTControl(UINT index) = 0;
	virtual Potentiometr* GetPotentiometr(UINT index) = 0;
	virtual DescreeteInput* GetDescreeteIn(UINT index) = 0;
	virtual ArincIn* GetArincIn(UINT index) = 0;
	virtual RSIn* GetRSIn(UINT index) = 0;
	virtual SCTOut* GetSCTOut(UINT index) = 0;
	virtual NPTOut* GetNPTOut(UINT index) = 0;
	virtual DescreeteOutput* GetDescreeteOut(UINT index) = 0;
	virtual ArincOut* GetArincOut(UINT index) = 0;
	virtual RSOut* GetRSOut(UINT index) = 0;
	virtual void AddArincWord(ArincWordOut* word) = 0;
	virtual void AddArincWord(ArincWordIn* word) = 0;
	virtual void AddRSField(RSFieldIn* word) = 0;
	virtual void AddRSField(RSFieldOut* word) = 0;

	virtual void UpdateInputData() = 0;
	virtual void UpdateOutputData() = 0;

	virtual IInputValue* GetInput(IOTYPES ioType, UINT num) = 0;
	virtual IOutputValue* GetOutput(IOTYPES ioType, UINT num) = 0;

	virtual void OnIrq(DWORD id) = 0;
	virtual DISPATH_RESULT ProcessFlag(Flags id) = 0;

	virtual bool IsIOExist(IOTYPES ioType, UINT num) const = 0;

	virtual IO_GROUP_STATE GetState() = 0;
	virtual bool IsIrqNormal() = 0;
	virtual UINT GetCurrentIrqNum() const = 0;
};

#endif

