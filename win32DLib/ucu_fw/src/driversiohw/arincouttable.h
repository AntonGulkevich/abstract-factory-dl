/*
 * arincouttable.h
 *
 *  Created on: 01 рту. 2015 у.
 *      Author: Youbrin_A
 */

#ifndef ARINCOUTTABLE_H_
#define ARINCOUTTABLE_H_
#include "../types.h"

class ArincOutTable
{
public:
	class Item
	{
	private:
		const UINT ENABLE_OFFSET = 0x000;
		const UINT DATA_OFFSET = 0x100;
		const UINT PERIOD_OFFSET = 0x200;
		const UINT CHANNELNUM_OFFSET = 0x300;

		DWORD _baseAddress;
		DWORD _selfNum;
	public:
		Item(DWORD baseAddress, DWORD selfNum, DWORD period, DWORD channelNum) : _baseAddress(baseAddress), _selfNum(selfNum)
		{
			UCU_IOWR_32DIRECT(_baseAddress, PERIOD_OFFSET + _selfNum*4, period * 100);
			UCU_IOWR_32DIRECT(_baseAddress, CHANNELNUM_OFFSET + _selfNum*4, channelNum);
		}

		void Update(DWORD data) { UCU_IOWR_32DIRECT(_baseAddress, DATA_OFFSET + _selfNum*4, data); }
		void EnableOut(bool isEnable) { UCU_IOWR_32DIRECT(_baseAddress, ENABLE_OFFSET, isEnable ?
				UCU_IORD_32DIRECT(_baseAddress, ENABLE_OFFSET) | (1 << _selfNum) :
				UCU_IORD_32DIRECT(_baseAddress, ENABLE_OFFSET) & ~(1 << _selfNum)); }
	};

private:


	static const UINT MAX_WORDCOUNT = 32;

	Item* _items[MAX_WORDCOUNT];

	DWORD _baseAddress;
	DWORD _totalWordCount;

public:
	ArincOutTable(DWORD baseAddress);
	virtual ~ArincOutTable() {}

	Item* AddWord(UINT channelNum, UINT period);

};

#endif /* ARINCOUTTABLE_H_ */
