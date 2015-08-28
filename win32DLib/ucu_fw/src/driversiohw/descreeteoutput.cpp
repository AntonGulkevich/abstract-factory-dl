/*
 * DescreeteInput.cpp
 *
 *  Created on: 20 ���. 2015 �.
 *      Author: Youbrin_A
 */

#include "descreeteoutput.h"

#include "../utilities/m_math.h"

UINT DescreeteOutput::allValues = 0;




void DescreeteOutput::UpdateValue()
{
	allValues = (allValues & _mask) | ( (ftou__(_value) & 1) << _outputNum);
	UCU_IOWR_32DIRECT(_baseAddress, VALUE_OFFSET, allValues);


	// ��� ������, �� ��� �������� ����������� ������� ������
	// ������� 27-����� ��������������� ������ ����� ��� � ������
	if (_outputNum <= 4)
	{
		UINT state = (~UCU_IORD_32DIRECT(_baseAddress, STATE04_OFFSET)) & 0x7;
		UINT pairNum = _outputNum >> 1;

		if ((((allValues & 0x1F) >> (pairNum << 1)) & 0x3) == 0)
			_state.iostCommandFail = (state >> pairNum) & 1;
		else
			_state.iostCommandFail = 0;
	}
	else if (_outputNum == 5 || _outputNum == 6 || _outputNum == 7)
	{
		UINT state = UCU_IORD_32DIRECT(_baseAddress, STATE57_OFFSET) & 0x3F;
		_state.iostCommandFail = (state >> ((_outputNum-5) << 1) & 0x3) == 2 ? 1 : 0;
	}
}
