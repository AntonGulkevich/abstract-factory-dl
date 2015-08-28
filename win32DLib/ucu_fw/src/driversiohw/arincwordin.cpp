#include "arincwordin.h"

#include "arincin.h"

ArincWordIn::ArincWordIn(ArincIn* channel, BYTE label) : _label(label), _channel(channel)
{
	_periodMax = 0;
	_periodMin = 0;
	_state.dword = 0;
}

void ArincWordIn::UpdateValue()
{
	UINT periodMax=  (_periodMax * 3) < 100 ? 100 : (_periodMax * 3);
	_state.iostArincWNoWord = _channel->GetTimeout(_label, false) > periodMax ? 1 : 0;
	if (_state.iostArincWNoWord == 1)
		_value = 0;
	else
		_value = (float)(_channel->GetValue(_label, false) & 0xFFFFFF00); // Отрезаем адрес т.к. float 24 бита
	_state.iostArincChNoSignal = _channel->GetState().iostArincChNoSignal;

}

bool ArincWordIn::IsDataParity()
{
	return _channel->GetParity() == Arinc::ParityIn::NoControl;
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


