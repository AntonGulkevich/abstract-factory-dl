#include "iusingirq.h"
#include "../drivers/irqmanager.h"

IrqManager* IUsingIrq::pIrqManager;

void IUsingIrq::RegIRQ(DWORD irq, IUsingIrq* pobj, DWORD param)
{
	pIrqManager->RegIRQ(irq, pobj, param);
}
void IUsingIrq::EnableIRQ(DWORD irq)
{
	pIrqManager->EnableIRQ(irq);
}
void IUsingIrq::DisableIRQ(DWORD irq)
{
	pIrqManager->DisableIRQ(irq);
}
