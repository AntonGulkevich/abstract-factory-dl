/*
 * ichannelrate.cpp
 *
 *  Created on: 14 июня 2015 г.
 *      Author: Youbrin_A
 */

#include "channelrate.h"

#include <string.h>


#include "ichannel.h"


ChannelRate::ChannelRate(IChannel* channel) : _channel(channel)
{
	_rateBuff = NULL;
	_rateBuffLength = 0;
	_valuesCount = 0;
}

void ChannelRate::CreateRateRegisters()
{
	_channel->registers_t[(UINT)REGISTER_ID::rRATE] = {REGISTER_ID::rRATE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	_channel->registers_t[(UINT)REGISTER_ID::rRATEBASE] = {REGISTER_ID::rRATEBASE, rwConstant, rtDec, 1, 50, 1, false};
	_channel->registers_t[(UINT)REGISTER_ID::rRATEREADY] = {REGISTER_ID::rRATEREADY, rwVariable, rtDec, 0, 1, 0, false};
}

void ChannelRate::CreateRateBuffer()
{
	if (_channel->GetRegister(REGISTER_ID::rRATEBASE)->IsFilled())
	{
		_rateBuffLength = _channel->GetRegister(REGISTER_ID::rRATEBASE)->GetValueInt();
		_rateBuff = new float[_rateBuffLength];
	}
	ResetRate();

}


void ChannelRate::ResetRate()
{
	if (_rateBuff != NULL)
		memset(_rateBuff, 0, _rateBuffLength * sizeof(float));
	_valuesCount = 0;
}

void ChannelRate::UpdateValue()
{
	if (_rateBuff != NULL)
	{
		float value = _channel->GetRegister(REGISTER_ID::rVALUE)->GetValueFloat();
		if (_valuesCount < _rateBuffLength)
			_rateBuff[_valuesCount++] = value;
		else
		{
			for(UINT i = 0; i < _valuesCount-1; i++)
				_rateBuff[i] = _rateBuff[i+1];
			_rateBuff[_valuesCount-1] = value;
		}
		_channel->GetRegister(REGISTER_ID::rRATE)->SetValue((_rateBuff[_valuesCount-1] - _rateBuff[0]) / (_valuesCount*_secondBase));
		//_channel->GetRegister(REGISTER_ID::rRATE)->SetValue(LineOls());
		_channel->GetRegister(REGISTER_ID::rRATEREADY)->SetValue(IsRateReady() ? 1 : 0);
	}
}

        // Возвращает коэффициент наклона линейной регрессии по МНК (без учета шага по оси аргумента)
        float ChannelRate::LineOls()
        {
        	float a = 0;
        	float b = _valuesCount-1;
            float xmean = (a + b) / 2 * _secondBase;
            float ymean = 0;
			for(UINT i = 0; i < _valuesCount; i++)
				ymean += _rateBuff[i];
			ymean /= _valuesCount;


            float cov = 0;
            for (int i = 0; i < _valuesCount; i++)
                cov += ((a + i)*_secondBase - xmean) * (_rateBuff[i] - ymean);

            cov = cov / _valuesCount;
            return 12 * cov / (((b - a) * _secondBase) * ((b - a) * _secondBase));
        }

