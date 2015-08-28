#ifndef CDISPLAY_H_
#define CDISPLAY_H_
#include "../types.h"

// Драйвер индикатора
class Display
{
  public:

#ifdef _UCU_EMULATION
    Display(UINT baseAddress) {}
#else
    Display(UINT baseAddress_) : baseAddress(baseAddress_) { }
#endif

    void Init();
	void Show();
    void Hide();
    void RunningZero();
    void RunningB();
    void SetValue(BYTE* text);
	void SetPoint(bool p1, bool p2, bool p3, bool p4);
	void SetTextByValue(int lValue, bool sign);
	void SetTextByText(const char* text, bool clearPoint = false);
	BYTE GetByteByDigit(BYTE digit);
	BYTE GetByteByLetter(char ch);
private:
    BYTE value[4];
	bool points[4];
	static BYTE digitTable[16];
	BYTE curZero;
	const UINT baseAddress;
	/*
#ifndef _UCU_EMULATION
    volatile BYTE* const registers;
#endif
*/
};

#endif /* CDISPLAY_H_ */
