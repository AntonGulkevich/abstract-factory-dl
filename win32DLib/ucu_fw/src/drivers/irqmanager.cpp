/*
 * cirqmngr.cpp
 */
#include <stdio.h>
#include "irqmanager.h"
#include "../types.h"
#include "../interfaces/iusingirq.h"
#include "perfomancecounter.h"

PerfomanceCounter* IrqManager::_pc = NULL;

IrqManager::IrqManager(PerfomanceCounter* pc)
{
//	_pc = pc;
//	IUsingIrq::SetManager(this);
//	memset(IrqManager::contexts, 0, sizeof(CIrqContext) * interruptsCount);
//	int Status;
//	Xil_ExceptionInit();
//
//	XScuGic_DeviceInitialize(XPAR_SCUGIC_0_DEVICE_ID);
//	/*
//	 * Initialize the interrupt controller driver so that it is ready to
//	 * use.
//	 */
//	_instance.Config = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);
//	if (NULL == _instance.Config) {
//		return;
//	}
//
//	Status = XScuGic_CfgInitialize(&_instance, _instance.Config,
//			_instance.Config->CpuBaseAddress);
//	if (Status != XST_SUCCESS) {
//		return;
//	}
//
//	_baseAddress = _instance.Config->CpuBaseAddress;
//	/*
//	 * Connect the interrupt controller interrupt handler to the hardware
//	 * interrupt handling logic in the processor.
//	 */
//	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
//				(Xil_ExceptionHandler)XScuGic_InterruptHandler,
//				&_instance);
//
//	// —брос всех аппаратных прерываний
///*	for(int i = 0; i < 32; i++)
//		XScuGic_WriteReg(_baseAddress, XSCUGIC_EOI_OFFSET, i);*/
//	/*
//	 * Enable interrupts in the Processor.
//	 */
//	Xil_ExceptionEnable();
//
}

IrqManager::CIrqContext IrqManager::contexts[interruptsCount];
void IrqManager::OnIRQ(void * ptr)
{
	_pc->IrqStart();
	CIrqContext* tmp = reinterpret_cast<CIrqContext*>(ptr);
	tmp->obj->OnIrq(tmp->param);
	_pc->IrqStop();
}

void IrqManager::RegIRQ(DWORD irq, IUsingIrq* pobj, DWORD param)
{
//	if (irq < interruptsCount)
//	{
//		contexts[irq].irqId = irq;
//		contexts[irq].obj = pobj;
//		contexts[irq].param = param;
//
//		XScuGic_Connect(&_instance, irq,
//						(Xil_ExceptionHandler)IrqManager::OnIRQ,
//						(void *)&contexts[irq]);
//	}
//	else
//		throw;
}

//void IrqManager::RegIRQ(DWORD irq, Xil_ExceptionHandler func, void* context)
//{
//	XScuGic_Connect(&_instance, irq,
//					(Xil_ExceptionHandler)func,
//					context);
//}

void IrqManager::EnableIRQ(DWORD irq)
{
	//XScuGic_Enable(&_instance, irq);
}

void IrqManager::DisableIRQ(DWORD irq)
{
	//XScuGic_Disable(&_instance, irq);
}

void IrqManager::DisableAll()
{
	for(UINT i = 0; i < interruptsCount; i++)
		if (contexts[i].obj != 0)
			DisableIRQ(i);
}
void IrqManager::EnableAll()
{
	for(UINT i = 0; i < interruptsCount; i++)
		if (contexts[i].obj != 0)
			EnableIRQ(i);
}

void IrqManager::ClearPeripheralIrq(DWORD IntID)
{
	/*
	 * Write to the EOI register, we are all done here.
	 * Let this function return, the boot code will restore the stack.
	 */
	//XScuGic_WriteReg(_baseAddress, XSCUGIC_EOI_OFFSET, IntID);

}
