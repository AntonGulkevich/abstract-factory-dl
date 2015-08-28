/*
 * usbtr.h
 *
 *  Created on: 25 марта 2015 г.
 *      Author: Youbrin_A
 */

#ifndef USBTR_H_
#define USBTR_H_
#include "../interfaces/iusingflags.h"
#include "../types.h"
#include "../interfaces/iusingirq.h"
#include "../drivers/gpio.h"

class UsbTr : public IUsingIrq, public IUsingFlags
{
public:

	enum PACKET_RESULT
	{
		NoWriteData = 0x22,
		WriteBufferReady = 0x11,
		Break = 0x00,
		Correct = 0x77,
		WrongCrc = 0x88,
		WrongPacketNumber = 0x99,
		ReTry = 0xAA,
	};

	enum STATUS_MASK
	{
		Data64Received = 1,
		DataXReceived = 2,
		DataSendRequest = 4,
		DiagnosticAnalogReceived = 8,
		DiagnosticDigitalReceived = 16,
		TimeoutError = 32,
		IncorrectMarker = 64,
		SomeError = TimeoutError + IncorrectMarker,
		All = 0xFFFFFFFF

	};

	// Подтверждение
	struct StatusData
	{
		UINT marker;
		UINT stage;
		UINT progress;
	};


private:
	const UINT dataOffset = 0x4;

	static DWORD _baseAddress;
	DWORD _irqId;
	DWORD _isInputDiagAnalogPacketCount;
	DWORD _isInputDiagDigitalPacketCount;

	// Команда от ПК
	WORD _command;

	DWORD GetStatus() { return UCU_IORD_32DIRECT(_baseAddress, 0xC); }

	bool IsWriteBufferBusy() { return UCU_IORD_32DIRECT(_baseAddress, 0x38) != 0; }
	void ResetReadBuffer() { UCU_IOWR_32DIRECT(_baseAddress, 0x34, 1); }

public:


	// Размер полученных данных

	UINT GetPacketSize() { return UCU_IORD_32DIRECT(_baseAddress, 0x28) + 1; }

	void SetTimeout(UINT timeout) { UCU_IOWR_32DIRECT(_baseAddress, 0x40, timeout * 50000);  }


	UsbTr(DWORD baseAddress, DWORD irqId);
	virtual ~UsbTr();

	void Init();

	virtual void OnIrq(DWORD number);

	// Ожидание информации от USB
	STATUS_MASK WaitForStatus(UINT mask);
	// Ожидание информации от USB несколько попыток при ошибке
	STATUS_MASK WaitForStatusWithTry(UINT mask, int tryCount);

	void ResetStatus(UINT mask) { UCU_IOWR_32DIRECT(_baseAddress, 0x20, mask); }
	// Разрешает прерывания по маске
	void MaskIrq(UINT mask) { UCU_IOWR_32DIRECT(_baseAddress, 0x2C, mask); }

	void Receive64kData(void* data);
	UINT ReceiveData(void* data);
	// Выдача данных в ПК по запросу. Если size = 0, отправляется ответ отказ от выдачи данных
	void WriteData(const void* data, UINT size);

	UINT GetTotalPacketCount() { return UCU_IORD_32DIRECT(_baseAddress, 0x8); }
	UINT GetCurrentPacketNumber() { return UCU_IORD_32DIRECT(_baseAddress, 0x10); }
	UINT GetCRC() { return UCU_IORD_32DIRECT(_baseAddress, 0x14); }
	UINT GetMarker() { return UCU_IORD_32DIRECT(_baseAddress, 0x0); }
	UINT GetPLDDate()  { return UCU_IORD_32DIRECT(_baseAddress, 0x60); }


	// Вызов этой функции отправляет байт в ответ на запрос от ПК
	void SendAnswer(PACKET_RESULT result) { UCU_IOWR_32DIRECT(_baseAddress, 0x18, result); }
	void SendProgress(UINT stage, UINT progressVal);

	WORD GetCommand() { return _command; }

};

#endif /* USBTR_H_ */
