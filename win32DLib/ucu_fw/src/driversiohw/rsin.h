/*
 * rsin.h
 *
 *  Created on: 19 мая 2015 г.
 *      Author: Youbrin_A
 */

#ifndef RSIN_H_
#define RSIN_H_
#include "../interfaces/iinputvalue.h"
#include "../types.h"
#include "rs.h"

class RSIn : public IInputValue
{
public:

	enum class WorkMode
	{
		Normal,
		Packet,
		Time,
	};

	union Status
	{
		struct
		{
			UINT overflow : 1;
			UINT time : 1;
			UINT packet : 1;
			UINT parityError : 1;
			UINT : 28;

		};
		DWORD dword;
	};

	union MarkerMode
	{
		struct
		{
			UINT isStartMarker16bit : 1;
			UINT isStopMarker16bit : 1;
			UINT isUseAltStopMarker : 1;
			UINT : 29;

		};
		DWORD dword;
	};


private:


	const UINT BAUDRATE_OFFSET = 0;
	const UINT NUMBERBIT_OFFSET = 4;
	const UINT PARITY_OFFSET = 8;
	const UINT STOPBITS_OFFSET = 0xC;
	const UINT COUNT_OFFSET = 0x30;
	const UINT COUNTPT_OFFSET = 0x60;
	const UINT DATA_OFFSET = 0x28;
	const UINT STATUS_OFFSET = 0x2C;
	const UINT WORKMODE_OFFSET = 0x10;

	const UINT MARKERMODE_OFFSET = 0x14;
	const UINT STARTMARKER_OFFSET = 0x1C;
	const UINT STOPMARKER_OFFSET = 0x20;
	const UINT ALTERNATIVEMARKER_OFFSET = 0x24;
	const UINT TIME_OFFSET = 0x18;
	const UINT RESET_OFFSET = 0x5C;
	const UINT CLEAR_BUFFER_OFFSET = 0x64;

	const UINT IRQENABLE_OFFSET = 0x58;

	DWORD _baseAddress;
	BYTE _inputBuffer[1024];
	UINT _receivedCount;
	WorkMode _workMode;
	MarkerMode _markerMode;

	UINT GetCount() { return UCU_IORD_32DIRECT(_baseAddress, COUNT_OFFSET); }
	UINT GetCountPT() { return UCU_IORD_32DIRECT(_baseAddress, COUNTPT_OFFSET); }
	Status GetStatus() { Status st; st.dword = UCU_IORD_32DIRECT(_baseAddress, STATUS_OFFSET); return st; }
	void ResetStatus() { UCU_IOWR_32DIRECT(_baseAddress, RESET_OFFSET, 0xF); }
	void ClearBuffer() { UCU_IOWR_32DIRECT(_baseAddress, CLEAR_BUFFER_OFFSET, 0x1); }

public:
	RSIn(DWORD baseAddress);
	virtual ~RSIn() {}


	void SetBaudRate(RS::BaudRate rate) { UCU_IOWR_32DIRECT(_baseAddress, BAUDRATE_OFFSET, 50000000 / (UINT)rate); }
	RS::BaudRate GetBaudRate() { return (RS::BaudRate)(50000000 / UCU_IORD_32DIRECT(_baseAddress, BAUDRATE_OFFSET)); }
	void SetNumberBits(UINT val) { UCU_IOWR_32DIRECT(_baseAddress, NUMBERBIT_OFFSET, val); }
	UINT GetNumberBits() { return UCU_IORD_32DIRECT(_baseAddress, NUMBERBIT_OFFSET); }
	void SetParityType(RS::ParityType parity) { UCU_IOWR_32DIRECT(_baseAddress, PARITY_OFFSET, (UINT)parity); }
	RS::ParityType GetParityType() { return (RS::ParityType)UCU_IORD_32DIRECT(_baseAddress, PARITY_OFFSET); }
	// Стоповый биты 1 или 2
	void SetStopBits(UINT val) { UCU_IOWR_32DIRECT(_baseAddress, STOPBITS_OFFSET, val-1); }
	UINT GetStopBits() { return UCU_IORD_32DIRECT(_baseAddress, STOPBITS_OFFSET) + 1; }

	void SetWorkMode(WorkMode mode) { _workMode = mode; UCU_IOWR_32DIRECT(_baseAddress, WORKMODE_OFFSET, (UINT)mode); }
	void SetStartMarker(DWORD marker, bool is16bit) { UCU_IOWR_32DIRECT(_baseAddress, STARTMARKER_OFFSET, marker);
		_markerMode.isStartMarker16bit = (is16bit ? 1 : 0); UCU_IOWR_32DIRECT(_baseAddress, MARKERMODE_OFFSET, _markerMode.dword); }
	void SetStopMarker(DWORD marker, bool is16bit) { UCU_IOWR_32DIRECT(_baseAddress, STOPMARKER_OFFSET, marker);
		_markerMode.isStopMarker16bit = (is16bit ? 1 : 0); UCU_IOWR_32DIRECT(_baseAddress, MARKERMODE_OFFSET, _markerMode.dword); }
	void SetAltStopMarker(DWORD marker, bool isUse) { UCU_IOWR_32DIRECT(_baseAddress, ALTERNATIVEMARKER_OFFSET, marker);
		_markerMode.isUseAltStopMarker = (isUse ? 1 : 0); UCU_IOWR_32DIRECT(_baseAddress, MARKERMODE_OFFSET, _markerMode.dword); }
	void SetTime(DWORD time) { UCU_IOWR_32DIRECT(_baseAddress, TIME_OFFSET, time); }

	void UpdateValue();

	// Диагностика
	UINT GetBufferSize() { return _receivedCount; }
	BYTE* GetBuffer() { return _inputBuffer; }
};

#endif /* RSIN_H_ */
