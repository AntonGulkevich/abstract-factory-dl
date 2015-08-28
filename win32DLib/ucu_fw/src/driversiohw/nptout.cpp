/*
 * sktout.cpp
 *
 *  Created on: 13 апр. 2015 г.
 *      Author: Youbrin_A
 */

#include "nptout.h"
#include "nptcontrol.h"

NPTOut::NPTOut(DWORD baseAddress, DWORD outputOffset, NptControl* control) :
_baseAddress(baseAddress), _outputOffset(outputOffset), _control(control)
{
	_control->SetType(AnalogIn::TYPE::ControlNPT);
	_control->SetGainCoefficient(AnalogIn::GAIN_COEFFICIENTS::K_x01);
	UpdateValue();

}

NPTOut::~NPTOut()
{
	// TODO Auto-generated destructor stub
}


void NPTOut::SetCalibration(Calibrations& calib)
{
	//0x10 Ц 0x18 Ц gain (w/r) 32 bit - ћасштабный коэффициент калибровки (float).
	UCU_IOWR_32DIRECTF(_baseAddress, _outputOffset + 0x10, calib.scale);
	//0x20 Ц 0x28 Ц offset (w/r) 16 bit - —двиговый коэффициент калибровки
	UCU_IOWR_32DIRECT(_baseAddress, _outputOffset + 0x20, calib.offset);

}

void NPTOut::UpdateValue()
{
	UCU_IOWR_32DIRECTF(_baseAddress, _outputOffset + valueOffset, _value);
	_state.iostAnalogDACFail = UCU_IORD_32DIRECT(_baseAddress, _outputOffset + failOffset);
	_state.iostAnalogDACOverload = UCU_IORD_32DIRECT(_baseAddress, _outputOffset + overloadOffset);
	// ќдиночный отказ отклонение от контрольного, только при отстутствии RefShift
	if (!_control->IsRefShifted())
		_state.iostAnalogSingleDACFail = UCU_IORD_32DIRECT(_baseAddress, _outputOffset + failSingleOffset);
}
