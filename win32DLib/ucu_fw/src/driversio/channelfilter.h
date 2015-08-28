
/*
 * ichannelfilter.h
 *
 *  Created on: 14 ���� 2015 �.
 *      Author: Youbrin_A
 */

#ifndef ICHANNELFILTER_H_
#define ICHANNELFILTER_H_
#include "../types.h"

class IChannel;

class ChannelFilter
{
public:

	struct BESSCOEFF
	{

	    float b01;
	    float b11;
	    float b21;
	    float a11;
	    float a21;
	    float b02;
	    float b12;
	    float b22;
	    float a12;
	    float a22;
	};


private:


	IChannel* _channel;
	float* filterBuff; // ������ �������� ��� �������
	UINT filterBuffLength; // ����� �������
	FILTR_TYPE filterType; // ��� �������
	UINT filterSubType; // ������ ��� bessel ���������
	UINT valuesCount; // ���������� �������� � ������

	float Bessel(float value);
	float Window(float value);

public:
	ChannelFilter(IChannel* channel);
	virtual ~ChannelFilter() {}
	float GetFiltredValue();

	void CreateFilterRegisters();
	void CreateFilterBuffer();
	void ResetFiltr();
	void UpdateValue();
	// ������������ ������ ��� ������� ����������
	void NormalizeBuffer(float breakPoint);
	bool IsFilterReady() { return valuesCount == filterBuffLength; }
};

#endif /* ICHANNELFILTER_H_ */
