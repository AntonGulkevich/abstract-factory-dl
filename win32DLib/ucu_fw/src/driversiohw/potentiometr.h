/*
 * potentiometr.h
 *
 *  Created on: 10 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef POTENTIOMETR_H_
#define POTENTIOMETR_H_
#include "../interfaces/iinputvalue.h"
#include "../types.h"

class Potentiometr : public IInputValue
{

public:
	enum class RefType
	{
		Const0,
		Const1,
		Variable
	};

private:
	static DWORD _staticBaseAddress;
	DWORD _baseAddress;
	DWORD _inputOffset;

	const DWORD maxLevelOffset = 0x40;
	const DWORD minLevelOffset = 0x180;
	const DWORD valueOffset = 0xC0;
	const DWORD registerOffset = 0x80;
	const DWORD failHighLevelOffset = 0x100;
	const DWORD failLowLevelOffset = 0x140;
	const static DWORD refControlOffset = 0x200;
	const static DWORD resetFilterOffset = 0x214;

public:

	Potentiometr(DWORD baseAddress, DWORD inputOffset);
	virtual ~Potentiometr();

	//0x40 Ц 0x54 max signal lvl (w/r) 32 bit - ћаксимальный уровень сигнала
	void SetMaximumSignalLevel(DWORD data) { UCU_IOWR_32DIRECT(_baseAddress, _inputOffset + maxLevelOffset, data); }
	//0x180 Ц 0x194 min signal lvl (w/r) 32bit - ћинимальный уровень сигнала
	void SetMinimumSignalLevel(DWORD data) { UCU_IOWR_32DIRECT(_baseAddress, _inputOffset + minLevelOffset, data); }
	//0x80 Ц 0x94 data sigma (r) «начение —ƒјѕ÷ (24b)
	void UpdateValue();
	//0xC0 Ц 0xD4 data float (r) 32 bit - –ассчитанное значение, % (float)
	DWORD GetRegister() { return UCU_IORD_32DIRECT(_baseAddress, _inputOffset + registerOffset);  }

	static void SetRef(RefType type) { UCU_IOWR_32DIRECT(_staticBaseAddress, refControlOffset, (UINT)type); }
	static void ResetHWFilter(UINT val) {  UCU_IOWR_32DIRECT(_staticBaseAddress, resetFilterOffset, val); }
	static void SetBaseAddress(DWORD baseAddress) { _staticBaseAddress = baseAddress; }

};

#endif /* POTENTIOMETR_H_ */
