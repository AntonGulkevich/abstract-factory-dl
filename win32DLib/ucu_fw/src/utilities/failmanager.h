/*
 * failmanager.h
 *
 *  Created on: 04 июля 2015 г.
 *      Author: Youbrin_A
 */

#ifndef FAILMANAGER_H_
#define FAILMANAGER_H_
#include "../driversio/ichannel.h"
#include "../types.h"

class Display;
class CConfiguration;

class FailManager
{


private:

	enum STATUS_FIRST
	{
		sfEmpty = 0,
		sfNone = '_',
		sfStart = 'A',
		sfGroup = 'Г',
		sfSync = 'C',
		sfAnalog = '0',
		sfPotentiometr = '1',
		sfRS = '3',
		sfAngleOut = '4',
		sfNPTOut = '5',
		sfCommand = 'P',
		sfArincIn1 = 8,
		sfArincIn2 = 9,
		sfArincIn3 = 10,
		sfArincIn4 = 11,
		sfArincOut1 = 12,
		sfArincOut2 = 13,
		sfArincOut3 = 14,
		sfArincOut4 = 15,
		sfArincIn5 = 16,
		sfArincIn6 = 17,
	};

	enum STATUS_SECOND
	{
		ssEmpty = 0,
		ssNone = '_',
		ssCommod = '2',
		ssRAM = '0',
		ssFlash = '3',
		ssBootFlash = '1',
		ssADC = '1',
		ssRVFail = '2',
		ssSDADC = '2',
		ssNoIrqAnalog = '3',
		ssNoIrqFreq = '4',
		ssArincIn = '5',
		ssArincOut = '6',

	};

	enum STATUS_FOURTH
	{
		sfrEmpty = 0xFF,
        sfrNone = '_',
		sfrNoSignal = 0,
		sfrHighLevel = 1,
		sfrLowLevel = 2,
		sfrBreak = 3,
		sfrBadAccuracy = 4,
		sfrCheckFail = 5,
		sfrDACFail = 4,
		sfrDACOverLoad = 0,
        sfrSDADCFail = 4,
		sfrADCFail = 0,
		sfrArincNoWord = 6,
		sfrArincOutOfLimit = 7,
        sfrFactoryFail = 4,
	};

	union STATUS
	{
		struct
		{
			STATUS_FOURTH fourth;
			UINT third;
			STATUS_SECOND second;
			STATUS_FIRST first;
			bool in;
		};
		struct
		{
			STATUS_FOURTH fourth;
			UINT sub;
			WORD value;
			bool in;
		} group;
	};

public:


	struct FailData
	{
		IChannel::ChannelData chData;
		STATUS status;
	};

private:

	const static UINT maxFailsCount = 256;

	Display* _display;
	CConfiguration* _config;
	UINT _currentFail;
	FailData _fails[maxFailsCount];
	UINT _totalFail;

	void PrepareFails();

public:




	FailManager(Display* display, CConfiguration* config);
	virtual ~FailManager() {}

	void ShowAlarms();
	const FailData* GetFails() { return _fails; }
	UINT GetFailsCount() { return _totalFail; }
	UINT GetCurrentFail() { return _currentFail; }
};

#endif /* FAILMANAGER_H_ */
