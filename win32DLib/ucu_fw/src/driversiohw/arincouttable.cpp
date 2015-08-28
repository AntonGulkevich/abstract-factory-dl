/*
 * arincouttable.cpp
 *
 *  Created on: 01 авг. 2015 г.
 *      Author: Youbrin_A
 */

#include "arincouttable.h"

ArincOutTable::ArincOutTable(DWORD baseAddress) : _baseAddress(baseAddress)
{
	_totalWordCount = 0;
	UCU_IOWR_32DIRECT(_baseAddress, 0, 0); // Выключение всех слов
}

ArincOutTable::Item* ArincOutTable::AddWord(UINT channelNum, UINT period)
{
	if (_totalWordCount < MAX_WORDCOUNT)
	{
		_items[_totalWordCount] = new Item(_baseAddress, _totalWordCount, period, channelNum);
		_totalWordCount++;
		return _items[_totalWordCount-1];
	} else
		return UCUNULL;
}


