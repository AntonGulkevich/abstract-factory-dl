/*
 * arincout.h
 *
 *  Created on: 04 мая 2015 г.
 *      Author: Youbrin_A
 */

#ifndef ARINCOUT_H_
#define ARINCOUT_H_
#include "../interfaces/ioutputvalue.h"
#include "../types.h"
#include "arinctypes.h"

class ArincOut : public IOutputValue
{
public:
	enum Mode
	{
		Normal,
		Table,
	};


private:

	union ControlRegister
	{
		struct
		{
			Arinc::Rate rate : 2;
			Arinc::ParityOut parity : 4;
			UINT reverse : 1;
			UINT : 25;
		};
		DWORD dword;
	};

	const UINT DATA_OFFSET = 0x0;
	const UINT FIFO_STATUS_OFFSET = 0x8;
	const UINT CONTROL_OFFSET = 0x4;
	const UINT CLEAR_FIFO_OFFSET = 0xC;
	const UINT MODE_OFFSET = 0x18;

	const static UINT diagBufferLength = 128;

	DWORD _baseAddress;
	DWORD _baseAddressControl;
	UINT _selfNumber;  // Для контроля
	ControlRegister _controlRegister;
	UINT _channelTimeout;

	UINT GetFifoStatus() { return UCU_IORD_32DIRECT(_baseAddress, FIFO_STATUS_OFFSET); }
	bool IsSignalTimeOut() { return ((UCU_IORD_32DIRECT(_baseAddressControl, 0x10) >> _selfNumber) & 1) == 1; }

	static UINT _tactNum; // Номер 20 мс такта, для организации выходного потока

	// Диагностика
	// Минимальный период между началами слов, для разных скоростей
	const UINT wordPauses[3] = { 2880, 720, 360 };

	// Время от последнего выданного слова в мкс
	int _wordWaitmks;
	Arinc::WORD429 _wordBuffer[diagBufferLength];
	UINT _wordCount;
	Mode _mode;
	void AddWordToDiag();

public:

	static void UpdateTactNumber() { _tactNum++; }
	static UINT GetCurrentTact() { return _tactNum; }

	ArincOut(DWORD baseAddress, DWORD baseAddressControl, UINT selfNum);
	virtual ~ArincOut() {}

	void SetRate(Arinc::Rate rate) { _controlRegister.rate = rate; UCU_IOWR_32DIRECT(_baseAddress, CONTROL_OFFSET, _controlRegister.dword); }
	Arinc::Rate GetRate() { return _controlRegister.rate; }
	void SetParity(Arinc::ParityOut parity) { _controlRegister.parity = parity; UCU_IOWR_32DIRECT(_baseAddress, CONTROL_OFFSET, _controlRegister.dword); }
	Arinc::ParityOut GetParity() { return (Arinc::ParityOut)_controlRegister.parity; }
	void SetReverse(bool isReverse) { _controlRegister.reverse = isReverse ? 1 : 0; UCU_IOWR_32DIRECT(_baseAddress, CONTROL_OFFSET, _controlRegister.dword); }
	// Установка таймаута в мс
	void SetTimeout(UINT timeoutMs) { _channelTimeout = timeoutMs * 50000; UCU_IOWR_32DIRECT(_baseAddressControl, _selfNumber * 4, _channelTimeout); }
	void SetMode(Mode mode) { _mode = mode; UCU_IOWR_32DIRECT(_baseAddress, MODE_OFFSET, (UINT)mode); }
	void ClearFifo() { UCU_IOWR_32DIRECT(_baseAddress, CLEAR_FIFO_OFFSET, 1); }

	void SendWord(Arinc::WORD429 data);
	UINT GetSelfNum() { return _selfNumber; }
	Mode GetMode() { return _mode; }


	// Диагностика
	Arinc::WORD429* GetWordBuffer() { return _wordBuffer; }
	UINT GetWordBufferCount() { return _wordCount; }
	void ResetWordBuffer() { _wordCount = 0; }

	// Обновленные данных по отказам
	void UpdateValue();
};

#endif /* ARINCOUT_H_ */
