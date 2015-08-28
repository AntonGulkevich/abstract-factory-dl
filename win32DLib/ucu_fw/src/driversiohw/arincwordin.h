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
	// �������� �������� �����
	BYTE GetLabel()	{ return _label; }

	void UpdateValue();
	// ���� ��� �������� ������������ ��� ������
	bool IsDataParity();

private:
	// ����� �����
	BYTE _label;
	ArincIn* _channel;
	UINT _periodMin;
	UINT _periodMax;
};



#endif /* ARINCWORD_IN_H */
