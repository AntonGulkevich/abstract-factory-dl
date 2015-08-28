/*
 * rsin.h
 *
 *  Created on: 19 мая 2015 г.
 *      Author: Youbrin_A
 */

#ifndef RS_H_
#define RS_H_

class RS
{
public:
	enum class BaudRate
	{
		br4800 = 4800,
		br9600 = 9600,
		br19200 = 19200,
		br38400 = 38400,
		br57600 = 57600,
		br115200 = 115200
	};

	enum ParityType
	{
		None = 0,
		Even = 1,  // Чет
		Odd = 2,    // Нечет
		Always1 = 3,
		Always0 = 4,
	};


};

#endif /* RS_H_ */
