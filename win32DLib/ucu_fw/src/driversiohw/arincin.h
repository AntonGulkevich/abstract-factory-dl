/*
 * arincin.h
 *
 *  Created on: 21 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef ARINCIN_H_
#define ARINCIN_H_
#include "../interfaces/iinputvalue.h"
#include "../types.h"
#include "arinctypes.h"

class ArincIn : public IInputValue
{
private:


	union ControlRegister
	{
		struct
		{
			Arinc::Rate rate : 2;
			Arinc::ParityIn parity : 2;
			UINT : 28;
		};
		DWORD dword;
	};

	const UINT FIFO_COUNT_OFFSET = 0x604;
	const UINT FIFO_VALUE_OFFSET = 0x500;
	const UINT FIFO_TIME_OFFSET = 0x504;
	const UINT FIFO_STATUS_OFFSET = 0x600;
	const UINT FIFO_CONTROL_OFFSET = 0x608;

	DWORD _baseAddress;
	ControlRegister _controlRegister;

	DWORD _values[256];
	DWORD _valuesId[1024];
	DWORD _timeOuts[256];
	DWORD _timeOutsId[1024];

	Arinc::WORD429 _wordBuffer[256];
	UINT _wordCount;
	UINT _timeOut;

	UINT GetFifoCount() { return UCU_IORD_32DIRECT(_baseAddress, FIFO_COUNT_OFFSET); }
	UINT GetFifoStatus() { return UCU_IORD_32DIRECT(_baseAddress, FIFO_STATUS_OFFSET); }

	UINT GetTopWordTime() { return UCU_IORD_32DIRECT(_baseAddress, FIFO_TIME_OFFSET); }
	UINT GetTopWordValue() { return UCU_IORD_32DIRECT(_baseAddress, FIFO_VALUE_OFFSET); }
	ControlRegister GetControlRegister() { _controlRegister.dword = UCU_IORD_32DIRECT(_baseAddress, FIFO_CONTROL_OFFSET); return _controlRegister; }

public:

	ArincIn(DWORD baseAddress);
	virtual ~ArincIn() {}

	void SetRate(Arinc::Rate rate) { _controlRegister.rate = rate; UCU_IOWR_32DIRECT(_baseAddress, FIFO_CONTROL_OFFSET, _controlRegister.dword); }
	Arinc::Rate GetRate() { return _controlRegister.rate; }
	void SetParity(Arinc::ParityIn parity) { _controlRegister.parity = parity; UCU_IOWR_32DIRECT(_baseAddress, FIFO_CONTROL_OFFSET, _controlRegister.dword); }
	Arinc::ParityIn GetParity() { return (Arinc::ParityIn)_controlRegister.parity; }

	void UpdateValue();
	DWORD GetValue(DWORD address, bool is10Bit) { return (is10Bit ? _valuesId[address & 0x3FF] : _values[address & 0xFF]); }
	DWORD GetTimeout(DWORD address, bool is10Bit) { return (is10Bit ? _timeOutsId[address & 0x3FF] : _timeOuts[address & 0xFF]); }

	// Диагностика
	Arinc::WORD429* GetWordBuffer() { return _wordBuffer; }
	UINT GetWordBufferCount() { return _wordCount; }


};

#endif /* ARINCIN_H_ */
