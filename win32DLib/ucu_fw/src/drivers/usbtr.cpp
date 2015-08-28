/*
 * usbtr.cpp
 *
 *  Created on: 25 марта 2015 г.
 *      Author: Youbrin_A
 */

#include "usbtr.h"

#include "../utilities/console.h"

DWORD UsbTr::_baseAddress = 0;

UsbTr::UsbTr(DWORD baseAddress, DWORD irqId) : _irqId(irqId), _command(0)
{
	_baseAddress = baseAddress;
	_isInputDiagAnalogPacketCount = 0;
	_isInputDiagDigitalPacketCount = 0;

}

UsbTr::~UsbTr()
{

}

#define USB_DATA UCU_IORD_32DIRECT(_baseAddress, 0x4)

void UsbTr::Init()
{
	RegIRQ(_irqId);
}

void UsbTr::OnIrq(DWORD number)
{
	auto status = GetStatus();
	if ((status & STATUS_MASK::DiagnosticAnalogReceived) != 0 && (status & STATUS_MASK::TimeoutError) == 0)
		_isInputDiagAnalogPacketCount++;
	if ((status & STATUS_MASK::DiagnosticDigitalReceived) != 0 && (status & STATUS_MASK::TimeoutError) == 0)
		_isInputDiagDigitalPacketCount++;

	if ((status & STATUS_MASK::DataXReceived) == STATUS_MASK::DataXReceived)
	{
		if (GetPacketSize() == 2)
		{
			ReceiveData(&_command);
			SetFlag(Flags::CommandPCReceived);
		}
		else
			SetFlag(Flags::DataPCReceived);
	}
	ResetStatus(status);
}


void UsbTr::Receive64kData(void* data)
{
	BYTE* ldata = reinterpret_cast<BYTE*>(data);
	const UINT PACKET_SIZE = 64*1024;
	for(UINT i = 0; i < PACKET_SIZE; i++)
	{
		ldata[i] = UCU_IORD_32DIRECT(_baseAddress, dataOffset);
	}
}

UINT UsbTr::ReceiveData(void* data)
{
	UINT size = GetPacketSize();
	ResetReadBuffer();
	BYTE* ldata = reinterpret_cast<BYTE*>(data);
	for(UINT i = 0; i < size; i++)
	{
		ldata[i] = UCU_IORD_32DIRECT(_baseAddress, dataOffset);
	}
	return size;
}


void UsbTr::WriteData(const void* data, UINT size)
{
	while(IsWriteBufferBusy()) {}


	if (size == 0)
		SendAnswer(NoWriteData);
	else
	{
		UINT wordCount = (size + 3) >> 2;
		UCU_IOWR_32DIRECT(_baseAddress, 0x24, wordCount-1);
		const DWORD* ldata = reinterpret_cast<const DWORD*>(data);
		for(UINT i = 0; i < wordCount; i++)
		{
			UCU_IOWR_32DIRECT(_baseAddress, 0x1C, ldata[i]);
		}
		SendAnswer(WriteBufferReady);
	}

}

UsbTr::STATUS_MASK UsbTr::WaitForStatus(UINT mask)
{
	DWORD status;
	status = 0;
	UINT tactCount = 0;
	while((status & mask) == 0)
	{
		status = GetStatus();
		tactCount++;
		if ((tactCount & 0xFFFF) == 0)
			Gpio::SVSet();
	}
	return (STATUS_MASK)status;
}

UsbTr::STATUS_MASK UsbTr::WaitForStatusWithTry(UINT mask, int tryCount)
{
	DWORD status;
	status = 0;

	UINT tactCount = 0;
	while (tryCount > 0)
	{
		while((status & mask) == 0)
			status = GetStatus();
		if ((status & STATUS_MASK::SomeError) != 0)
		{
			tryCount--;
			ResetStatus(STATUS_MASK::SomeError);
			SendAnswer(PACKET_RESULT::ReTry);
		}
		else
			tryCount = 0;
		tactCount++;
		if ((tactCount & 0xFFF) == 0)
			Gpio::SVSet();
	}
	return (STATUS_MASK)status;
}

void UsbTr::SendProgress(UINT stage, UINT progressVal)
{
	StatusData sdata;
	sdata.marker = 0x51a1da1a;
	sdata.stage = stage;
	sdata.progress = progressVal;
	WriteData(&sdata, sizeof(StatusData));
}
