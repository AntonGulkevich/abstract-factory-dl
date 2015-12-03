#ifndef IPERFOMANCECOUNTER_H_
#define IPERFOMANCECOUNTER_H_
#include "../types.h"

struct IPerfomanceCounter
{
	virtual ~IPerfomanceCounter(){};

	virtual UINT GetValue() const = 0;
	virtual void Start() const = 0;
	virtual void Stop() const = 0;
	virtual void ProcessStart() = 0;
	virtual void ProcessStop() = 0;
	virtual void LogicStart() = 0;
	virtual void LogicStop() = 0;
	virtual void DiagStart() = 0;
	virtual void DiagStop() = 0;
	virtual void IrqStart() = 0;
	virtual void IrqStop() = 0;
	virtual void Reset() = 0;
	virtual float GetLoadBg() const = 0;
	virtual float GetLoadLogic() const = 0;
	virtual float GetLoadDiag() const = 0;
	virtual float GetLoadIrq() const = 0;
};

#endif /* PERFOMANCECOUNTER_H_ */
