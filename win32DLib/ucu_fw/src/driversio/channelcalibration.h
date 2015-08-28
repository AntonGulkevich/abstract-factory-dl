/*
 * ichannelcalibration.h
 *
 *  Created on: 14 июня 2015 г.
 *      Author: Youbrin_A
 */

#ifndef ICHANNELCALIBRATION_H_
#define ICHANNELCALIBRATION_H_
#include "../types.h"

class IChannel;

class ChannelCalibration
{
private:
	IChannel* _channel;
	float** koeffTable; // Коэффициенты для LINEAR
	BYTE koeffCount; // Длина таблицы (количество пар)

	// Параметры для ручной тарировки ????

public:
	ChannelCalibration(IChannel* channel);
	virtual ~ChannelCalibration() {}
	float GetValueByKoeff(float deg);

	void CreateClibrationRegisters();
	// Построение таблицы для Linear из регистра PASSPORT или тарировки
	void CreateKoeffTable();
};

#endif /* ICHANNELCALIBRATION_H_ */
