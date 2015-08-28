#include "arincwordout.h"

#include "arincout.h"


ArincOutTable* ArincWordOut::_arincTable;

ArincWordOut::ArincWordOut(ArincOut* channel, BYTE label) : _label(label), _channel(channel)
{
	_isOnline = true;
	_tableItem = UCUNULL;
	_period20 = 1;
	_period = 20;

	/*foo = 0x12345678;
	foo1 = 0x87654321;*/
}

void ArincWordOut::UpdateValue()
{
	Arinc::WORD429 w;
	w.value = (((DWORD)_value) << 8) | _label;
	w.time = 0;

	if (_tableItem != UCUNULL)
	{
		_tableItem->Update(w.value);
		_tableItem->EnableOut(_isOnline);
	}
	if (_isOnline)
	{
		if (ArincOut::GetCurrentTact() % (_period20) == 0 || _period < 20)
		{
			_channel->SendWord(w);
		}
	}
}


bool ArincWordOut::SetTableMode()
{
	_tableItem = _arincTable->AddWord(_channel->GetSelfNum(), _period);
	return _tableItem != UCUNULL;
}
/*UINT ArincWordOut::CalcParity(DWORD data)
{
    if (parity == 2)
        return (data >> 31) & 1;
    else
    {
        UINT par = parity + 1;
        for (UINT i = 1; i < 0x7FFFFFFF; i<<=1)
            if (data & i)
                par++;
        return par & 1;
    }
         
}
*/


