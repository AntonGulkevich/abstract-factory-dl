/*
 * cirqmngr.h
 */

#ifndef CIRQMNGR_H_
#define CIRQMNGR_H_

#include "../types.h"
//#include "xscugic.h"

class IUsingIrq;
class PerfomanceCounter;

class IrqManager
{
private:
	DWORD _baseAddress;
	static PerfomanceCounter* _pc;

public:
	static void OnIRQ(void* ptr);


	IrqManager(PerfomanceCounter* pc);
	void RegIRQ(DWORD irq, IUsingIrq* pobj, DWORD param=0);
	//void RegIRQ(DWORD irq, Xil_ExceptionHandler func, void* context = NULL);
	void EnableIRQ(DWORD irq);
	void DisableIRQ(DWORD irq);

	void DisableAll();
	void EnableAll();

	void ClearPeripheralIrq(DWORD IntID);

	static void SetStartStopMeasure(void (*start)(), void (*stop)())
	{
		startMeasure = start;
		stopMeasure = stop;
	};
private:
	const static UINT interruptsCount = 100;
	struct CIrqContext
	{
		CIrqContext():irqId(0), obj(0),param(0) {}

		UINT irqId;
		IUsingIrq* obj;
		DWORD param;
	};
	static CIrqContext contexts[interruptsCount];

	static void (*startMeasure) ();
	static void (*stopMeasure) ();

	//XScuGic _instance;
};

#endif /* CIRQMNGR_H_ */
