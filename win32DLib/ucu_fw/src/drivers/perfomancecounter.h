/*
 * perfomancecounter.h
 *
 *  Created on: 06 июля 2015 г.
 *      Author: Youbrin_A
 */

#ifndef PERFOMANCECOUNTER_H_
#define PERFOMANCECOUNTER_H_
#include "../types.h"

class PerfomanceCounter
{
private:
	static DWORD _baseAddress;
	UINT _lastBG;
	UINT _lastIrq;
	UINT _lastLogic;
	UINT _lastDiag;
	UINT _totalBG;
	UINT _totalIrq;
	UINT _totalLogic;
	UINT _totalDiag;
	UINT _lastReset;

	UINT _irqWhileBg;
	UINT _irqWhileLogic;
	UINT _irqWhileDiag;


public:
	PerfomanceCounter(DWORD baseAddress);
	virtual ~PerfomanceCounter();

	static void WaitMs(UINT value)
	{
		if (_baseAddress != 0)
		{
			UINT val = UCU_IORD_32DIRECT(_baseAddress, 0x74);
			while ((val + value * 50000) > UCU_IORD_32DIRECT(_baseAddress, 0x74));
		}
	}

	UINT GetValue() { return UCU_IORD_32DIRECT(_baseAddress, 0x74); }

	void Start() { UCU_IOWR_32DIRECT(_baseAddress, 0x70, 1); }
	void Stop() { UCU_IOWR_32DIRECT(_baseAddress, 0x70, 0); }

	void ProcessStart() { _lastBG = GetValue(); _irqWhileBg = 0; }
	void ProcessStop() { _totalBG += GetValue()-_lastBG - _irqWhileBg;  }
	void LogicStart() { _lastLogic = GetValue(); _irqWhileLogic = 0; }
	void LogicStop() { _totalLogic += GetValue()-_lastLogic - _irqWhileLogic; }
	void DiagStart() { _lastDiag = GetValue(); _irqWhileDiag = 0; }
	void DiagStop() { _totalDiag += GetValue()-_lastDiag - _irqWhileDiag; }
	void IrqStart() { _lastIrq = GetValue(); }
	void IrqStop() { UINT last = GetValue()-_lastIrq; _totalIrq += last; _irqWhileBg += last; _irqWhileLogic += last; _irqWhileDiag += last; }
	void Reset() { _lastReset = _lastBG = _lastIrq = _lastLogic = _lastDiag =  GetValue(); _totalBG = _totalIrq = _totalLogic = _totalDiag =  0; }
	float GetLoadBg() { return (float)_totalBG / (GetValue() - _lastReset) * 100.0; }
	float GetLoadLogic() { return (float)_totalLogic / (GetValue() - _lastReset) * 100.0; }
	float GetLoadDiag() { return (float)_totalDiag / (GetValue() - _lastReset) * 100.0; }
	float GetLoadIrq() { return (float)_totalIrq / (GetValue() - _lastReset)* 100.0; }


};

#endif /* PERFOMANCECOUNTER_H_ */
