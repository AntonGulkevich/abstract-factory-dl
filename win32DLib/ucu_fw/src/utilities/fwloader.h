/*
 * fwloader.h
 *
 *  Created on: 23 марта 2015 г.
 *      Author: Youbrin_A
 */

#ifndef FWLOADER_H_
#define FWLOADER_H_
#include "../types.h"

class Qspi;
class Display;
class UsbTr;

class FWLoader
{

private:

	Qspi* _qspi;
	Display* _display;
	UsbTr* _usb;

public:
	void LoadFW();

	FWLoader(Qspi* qspi, Display* display, UsbTr* usb);
	virtual ~FWLoader();
};

#endif /* FWLOADER_H_ */
