#ifndef ARINCWORD_OUT_H
#define ARINCWORD_OUT_H
#include "../interfaces/ioutputvalue.h"
#include "arincouttable.h"


class ArincOut;

class ArincWordOut : public IOutputValue
{
public:
	static void SetArincTablePtr(ArincOutTable* arincTable) { _arincTable = arincTable; }

	ArincWordOut(ArincOut* channel, BYTE label);
	virtual ~ArincWordOut() {}
	// Получить значение метки
	BYTE GetLabel()	{ return _label; }
	// Установка периода в мс
	void SetPeriod(UINT period) { _period = period; _period20 = ((period / 20) == 0 ? 1 : period / 20);   }

	void SetIsOnline(bool val)  { _isOnline = val; }
	void UpdateValue();

	bool SetTableMode();


private:
	static ArincOutTable* _arincTable;

	// метка слова
	BYTE _label;
	//DWORD foo;
	ArincOut* _channel;
	//DWORD foo1;
	UINT _period;
	UINT _period20;
	bool _isOnline;

	ArincOutTable::Item* _tableItem;
};



#endif /* ARINCWORD_OUT_H */
