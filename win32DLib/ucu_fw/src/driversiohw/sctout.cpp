/*
 * sktout.cpp
 *
 *  Created on: 13 апр. 2015 г.
 *      Author: Youbrin_A
 */

#include "sctout.h"

#include "../utilities/m_math.h"
#include "analogin.h"

SCTOut::SCTOut(DWORD baseAddress, DWORD outputOffset, AnalogIn* control) :
_baseAddress(baseAddress), _outputOffset(outputOffset), _control(control)
{
	_state.dword = 0;
	SetMinRefLevel((DWORD)(SQR(8.0f * (float)((1 << 13)-1) / 100.0f)));
	SetMaxRefLevel((DWORD)(SQR(13.5f * (float)((1 << 13)-1) / 100.0f)));
	//SetMaxDeviation(1/3.0f);
	SetMaxDeviation(1/2.0f); // TODO Загрублено отклонение УБРАТЬ!!!
	SetType(SCTType::SCT);
}

SCTOut::~SCTOut()
{
	// TODO Auto-generated destructor stub
}

void SCTOut::SetType(SCTType type)
{
	UCU_IOWR_32DIRECT(_baseAddress, _outputOffset + typeOffset, (UINT)type);
	switch (type) {
		case SCTType::SCT:
			_control->SetAngleType(AnalogIn::ANGLE_TYPE::ControlSCT);
			break;
		case SCTType::ZSelsin:
			_control->SetAngleType(AnalogIn::ANGLE_TYPE::ControlSelsin);
			break;
		default:
			break;
	}
	// Глубина тупизны зашкаливает, надо как то это получше сделать
	_control->SetDelay(560 + _outputOffset * 5); // 560 + номер канала * 20
	_control->SetSyncNumber(_outputOffset / 4 + 5);
}

void SCTOut::SetCalibration(Calibrations& calib)
{
	//0x140 – 0x154 sin gain (w/r) 32bit - Масштабный коэффициент калибровки sin (float)
	UCU_IOWR_32DIRECTF(_baseAddress, _outputOffset + 0x140, calib.scaleSin);
	//0x180 – 0x194 sin offset (w/r) 16 bit - Сдвиговый коэффициент калибровки sin
	UCU_IOWR_32DIRECT(_baseAddress, _outputOffset + 0x180, calib.offsetSin);
	//0x1C0 – 0x1D4 cos gain (w/r) 32 bit - Масштабный коэффициент калибровки cos (float)
	UCU_IOWR_32DIRECTF(_baseAddress, _outputOffset + 0x1C0, calib.scaleCos);
	//0x200 – 0x214 cos offset (w/r) 16 bit- Сдвиговый коэффициент калибровки cos
	UCU_IOWR_32DIRECT(_baseAddress, _outputOffset + 0x200, calib.offsetCos);

}

void SCTOut::UpdateValue()
{
	float add = (int)(_value / 360) * 360.0f; // Приведение к -180..180
	_value -= add;
	_value = _value > 180 ? _value - 360 : _value;
	_value = _value < -180 ? _value + 360 : _value;
	UCU_IOWR_32DIRECTF(_baseAddress, _outputOffset + valueOffset, _value);

	_state.iostAnalogDACFail = UCU_IORD_32DIRECT(_baseAddress, _outputOffset + failOffset);
	_state.iostAnalogDACOverload = UCU_IORD_32DIRECT(_baseAddress, _outputOffset + overloadOffset);
	_state.iostAnalogRefFail = UCU_IORD_32DIRECT(_baseAddress, _outputOffset + failRefOffset);
	_state.iostAnalogRefOverload = UCU_IORD_32DIRECT(_baseAddress, _outputOffset + refOverloadOffset);
	_state.iostAnalogSyncFail = _control->GetState().iostAnalogSyncFail;

}

void SCTOut::PrepareCalibrationSin0()
{
	SetType(SCTType::SCT);
	_control->SetDelay(620);
	_control->SetGainCoefficient(AnalogIn::GAIN_COEFFICIENTS::K_x1);
	SetMaxDeviation(360); // Отключение контроля
	SetMinRefLevel(0);
	SetMaxRefLevel(0xFFFFFFF);
	SetPhaseShift(0);
	UCU_IOWR_32DIRECTF(_baseAddress, _outputOffset + valueOffset, 0);
}

void SCTOut::PrepareCalibrationCos0()
{
	SetType(SCTType::SCT);
	_control->SetDelay(620);
	_control->SetGainCoefficient(AnalogIn::GAIN_COEFFICIENTS::K_x1);
	SetMaxDeviation(360); // Отключение контроля
	SetMinRefLevel(0);
	SetMaxRefLevel(0xFFFFFFF);
	SetPhaseShift(0);
	UCU_IOWR_32DIRECTF(_baseAddress, _outputOffset + valueOffset, 90);
}

void SCTOut::PrepareCalibrationScale()
{
	SetType(SCTType::SCT);
	_control->SetDelay(620);
	_control->SetGainCoefficient(AnalogIn::GAIN_COEFFICIENTS::K_x02);
	SetMaxDeviation(360); // Отключение контроля
	SetMinRefLevel(0);
	SetMaxRefLevel(0xFFFFFFF);
	SetPhaseShift(0);
	UCU_IOWR_32DIRECTF(_baseAddress, _outputOffset + valueOffset, 45);
}

void SCTOut::ToNormalWork()
{
	SetMinRefLevel((DWORD)(SQR(8.0f * (float)((1 << 13)-1) / 100.0f)));
	SetMaxRefLevel((DWORD)(SQR(13.5f * (float)((1 << 13)-1) / 100.0f)));
	SetMaxDeviation(1/3.0f);
	SetType((SCTType)UCU_IORD_32DIRECT(_baseAddress, _outputOffset + typeOffset));

}

