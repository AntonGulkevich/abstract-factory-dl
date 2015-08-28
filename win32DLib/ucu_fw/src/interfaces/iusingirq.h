/*
 * iusingirq.h
 *
 *  Created on: 26.02.2013
 *      Author: Plaksin_A
 */

#ifndef CUSINGIRQ_H_H_
#define CUSINGIRQ_H_H_

#include "../types.h"

class IrqManager;

class IUsingIrq
{
private:
static IrqManager* pIrqManager;

public:
	IUsingIrq() {}
	virtual ~IUsingIrq() {}
	static void SetManager( IrqManager* irqManager ) { pIrqManager = irqManager; }
	// Обработка прерывания
	virtual void OnIrq(DWORD number) = 0;
	void RegIRQ(DWORD irq, DWORD param=0) { RegIRQ(irq, this, param); }
	void RegIRQ(DWORD irq, IUsingIrq* pobj, DWORD param=0);
	void EnableIRQ(DWORD irq);
	void DisableIRQ(DWORD irq);
};

#endif /* CUSINGIRQ_H_H_ */
