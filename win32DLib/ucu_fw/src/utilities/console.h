/*
 * console.h
 *
 *  Created on: 01 θώνÿ 2015 γ.
 *      Author: Youbrin_A
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

class UsbTr;

class Console
{
private:
	static UsbTr* _usb;
	static void SendData(const char* str, bool isNewLine);
public:
	Console() {}
	static void SetUsbTr(UsbTr* usb);

	void Write(const char* str);
	void WriteLine(const char* str);
	void Format(const char* str, ...);
	void FormatLine(const char* str, ...);
	static void Trace(const char* str, ...);
	static void TraceLine(const char* str, ...);
};

#endif /* CONSOLE_H_ */
