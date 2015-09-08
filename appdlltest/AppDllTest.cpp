// AppDllTest.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include "../win32DLib/ucu_fw/src/dllapi/factory.h"
#include "../win32DLib/ucu_fw/src/driversio/cregister.h"
#include "../win32DLib/ucu_fw/src/application/cconfiguration.h"
#include "../win32DLib/ucu_fw/src/driversio/ichannel.h"
#include "../win32DLib/ucu_fw/src/compilier/CParser.h"

typedef Factory * (__stdcall *DLLGETFACTORY)(void);

int main(int argc, char* argv[])
{
	auto hInstDll = LoadLibrary(__T("win32DLib.dll"));
	if (!hInstDll) std::cout<<("Failed to load dll\n");
	auto pDllGetFactory = reinterpret_cast<DLLGETFACTORY>(GetProcAddress(hInstDll, "returnFactory"));
	auto pMyFactory = (pDllGetFactory)();
	if (pMyFactory == nullptr) return 0;
	auto testRegister = static_cast<CRegister *>(pMyFactory->CreateCRegister());
	system("PAUSE");

	return 0;
}

