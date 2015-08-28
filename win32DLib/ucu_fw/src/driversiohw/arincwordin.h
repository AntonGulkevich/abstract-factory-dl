#ifndef ARINCWORD_IN_H
#define ARINCWORD_IN_H
#include "../interfaces/iinputvalue.h"
#include "arinctypes.h"


class ArincIn;

class ArincWordIn : public IInputValue
{
public:
	ArincWordIn(ArincIn* channel, BYTE label);
	virtual ~ArincWordIn() {}
	// ѕолучить значение метки
	BYTE GetLabel()	{ return _label; }

	void UpdateValue();
	// ≈сли бит четности используетс€ как данные
	bool IsDataParity();

private:
	// метка слова
	BYTE _label;
	ArincIn* _channel;
	UINT _periodMin;
	UINT _periodMax;
};



#endif /* ARINCWORD_IN_H */
