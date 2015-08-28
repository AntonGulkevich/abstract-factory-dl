/*
 * ichannelfilter.cpp
 *
 *  Created on: 14 июня 2015 г.
 *      Author: Youbrin_A
 */

#include "channelfilter.h"
#include <string.h>
#include "ichannel.h"


const ChannelFilter::BESSCOEFF bessCoeff[2][6] =
{
    {
        {
            0.34805426E-03f,  0.69610851E-03f,  0.34805426E-03f,  0.19283978E+01f,  -.92978998E+00f,
            0.44165561E-03f,  0.88331123E-03f,  0.44165561E-03f,  0.19467433E+01f,  -.94850992E+00f},
        {
			0.76930915E-03f,  0.15386183E-02f,  0.76930915E-03,  0.18934670E+01,  -.89654425E+00,
			0.98059311E-03f,  0.19611862E-02f,  0.98059311E-03,  0.19198644E+01,  -.92378680E+00 },
        {
			0.16145729E-02f,  0.32291457E-02f,  0.16145729E-02,  0.18455065E+01,  -.85196475E+00,
			0.20703501E-02f,  0.41407003E-02f,  0.20703501E-02,  0.18820104E+01,  -.89029183E+00 },
        {
			0.29199719E-02f,  0.58399439E-02f,  0.29199719E-02,  0.17920004E+01,  -.80368032E+00,
			0.37683563E-02f,  0.75367127E-02f,  0.37683563E-02,  0.18384984E+01,  -.85357178E+00 },
		{
			0.62425649E-02f,  0.12485130E-01f,  0.62425649E-02,  0.16952652E+01,  -.72023547E+00,
			0.81446440E-02f,  0.16289288E-01f,  0.81446440E-02,  0.17564667E+01,  -.78904523E+00  },
		{
			0.10558461E-01f,  0.21116923E-01f,  0.10558461E-01,  0.16029516E+01,  -.64518545E+00,
			0.13907796E-01f,  0.27815593E-01f,  0.13907796E-01,  0.16742550E+01,  -.72988621E+00, }
    },
    {
        {
            0.20632128E-02, 0.41264257E-02, 0.20632128E-02, 0.18252809E+01, -.83353377E+00,
            0.26521473E-02, 0.53042946E-02, 0.26521473E-02, 0.18657205E+01,  -.87632913E+00 },
        {
			0.44466022E-02,   0.88932045E-02,  0.44466022E-02,  0.17430613E+01,  -.76084774E+00,
			0.57709406E-02,  0.11541881E-01,  0.57709406E-02,  0.17975336E+01,  -.82061734E+00 },
        {
			0.90193725E-02,  0.18038745E-01,  0.90193725E-02,  0.16332497E+01,  -.66932714E+00,
			0.11844391E-01,  0.23688782E-01,  0.11844391E-01,  0.17016520E+01,  -.74902953E+00},
        {
			0.15715007E-01,  0.31430015E-01,  0.15715007E-01,  0.15147734E+01,  -.57763343E+00,
			0.20873280E-01,  0.41746560E-01,  0.20873280E-01,  0.15922768E+01,  -.67576993E+00 },
        {
			0.31471692E-01,  0.62943384E-01,  0.31471692E-01,  0.13107334E+01,  -.43662022E+00,
			0.42486360E-01,  0.84972719E-01,  0.42486360E-01,  0.13904944E+01,	 -.56043982E+00},
        {
			0.50129449E-01,  0.10025890E+00,  0.50129449E-01, 0.11273248E+01, -.32784264E+00,
			0.68415457E-01,  0.13683091E+00,  0.68415457E-01,  0.11961624E+01,  -.46982419E+00 }
    }
};

ChannelFilter::ChannelFilter(IChannel* channel) : _channel(channel)
{
	filterBuff = NULL;
	filterBuffLength = 0;
	valuesCount = 0;
	filterType = FILTR_TYPE::ftNone;
}

void ChannelFilter::CreateFilterRegisters()
{
	_channel->registers_t[(UINT)REGISTER_ID::rFILTRTYPE] = {REGISTER_ID::rFILTRTYPE, rwConstant, rtDec, 0, 2, 0, false};
	_channel->registers_t[(UINT)REGISTER_ID::rFILTRBASE] = {REGISTER_ID::rFILTRBASE, rwConstant, rtDec, 1, 250.0f, 1, false};
	_channel->registers_t[(UINT)REGISTER_ID::rNOTFILTREDVALUE] = {REGISTER_ID::rNOTFILTREDVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false};
	_channel->registers_t[(UINT)REGISTER_ID::rFILTRREADY] = {REGISTER_ID::rFILTRREADY, rwVariable, rtDec, 0, 1, 0, false};
}

void ChannelFilter::CreateFilterBuffer()
{
	filterType = (FILTR_TYPE)_channel->GetRegister(REGISTER_ID::rFILTRTYPE)->GetValueUInt();
	switch(filterType)
	{
	case FILTR_TYPE::ftBessel:
		filterBuffLength = 8;
        filterBuff = new float[filterBuffLength];
		filterSubType = _channel->GetRegister(REGISTER_ID::rFILTRBASE)->GetValueInt() - 1;
		break;
    case FILTR_TYPE::ftWindow:
    	filterBuffLength = _channel->GetRegister(REGISTER_ID::rFILTRBASE)->GetValueInt() + 1;
    	filterBuff = new float[filterBuffLength];
        break;
	default:
		filterBuff = NULL;
		filterBuffLength = 0;
	}
    ResetFiltr();
}


void ChannelFilter::ResetFiltr()
{
	if (filterBuff != NULL)
		memset(filterBuff, 0, filterBuffLength * sizeof(float));
	valuesCount = 0;
}

void ChannelFilter::UpdateValue()
{
	float value = _channel->GetRegister(REGISTER_ID::rVALUE)->GetValueFloat();
	_channel->GetRegister(REGISTER_ID::rNOTFILTREDVALUE)->SetValue(value);
	filterType = (FILTR_TYPE)_channel->GetRegister(REGISTER_ID::rFILTRTYPE)->GetValueUInt();
	switch(filterType)
	{
	case FILTR_TYPE::ftBessel:
		value = Bessel(value);
		_channel->GetRegister(REGISTER_ID::rVALUE)->SetValue(value);
		break;
    case FILTR_TYPE::ftWindow:
    	value = Window(value);
    	_channel->GetRegister(REGISTER_ID::rVALUE)->SetValue(value);
    	break;
	default:
		break;
	}
	_channel->GetRegister(REGISTER_ID::rFILTRREADY)->SetValue(IsFilterReady() ? 1 : 0);
}


void ChannelFilter::NormalizeBuffer(float breakPoint)
{

}

float ChannelFilter::Bessel(float value)
{
	if (valuesCount > 2)
		return 0;
	// 2 каскада фильтров второго порядка
	// buff - входной буффер на 8 значений 1-4 первый каскад 5-8 второй каскад
//	const float gain_koeff[3] =  { 1.0f / 0.981486f, 1.0f / 0.996807f};

	float output1;
	float output2;
	UINT step = 0;

	UINT ft = (UINT)filterType;

	filterBuff[0] = value + bessCoeff[step][ft].a11 * filterBuff[1] + bessCoeff[step][ft].a21 * filterBuff[2];
	output1    = bessCoeff[step][ft].b01* filterBuff[0] + bessCoeff[step][ft].b11 * filterBuff[1] + bessCoeff[step][ft].b21 * filterBuff[2];// 1 step

	filterBuff[4+0] = output1 + bessCoeff[step][ft].a12 * filterBuff[4+1] + bessCoeff[step][ft].a22 * filterBuff[4+2];
	output2    = bessCoeff[step][ft].b02 * filterBuff[4+0] + bessCoeff[step][ft].b12 * filterBuff[4+1] + bessCoeff[step][ft].b22 * filterBuff[4+2];// 2 step

	filterBuff[2] = filterBuff[1];
	filterBuff[1] = filterBuff[0];

	filterBuff[4+2] = filterBuff[4+1];
	filterBuff[4+1] = filterBuff[4+0];


	return output2;// * gain_koeff[step][filtrType];

}

float ChannelFilter::Window(float value)
{
    // ДЛя оптимизации можно всегда хранить сумму и учитывать только крайние значения, но есть же процессор!
    float sum = 0;
    if (valuesCount < filterBuffLength) // Наполнение буфера
    	filterBuff[valuesCount++] = value;
    else // Циркуляция
    {
    	filterBuff[valuesCount-1] = value;
		for(UINT i = 0; i < valuesCount-1; i++)
			filterBuff[i] = filterBuff[i+1];
    }
    for(UINT i = 0; i < valuesCount-1; i++)
    	sum += filterBuff[i];
	sum += value;
	return sum / valuesCount;
}






