#ifdef EMULATION
#ifdef _MANAGED
#pragma managed(push, off)
#endif
#include "UbsLibrary.h"

#undef EM_DEBUG
#ifdef  EM_DEBUG
#define D_COUT(debStr) (std::cout << (std::string) debStr << std::endl)
#else
#define D_COUT(debStr) UNREFERENCED_PARAMETER ((std::string) debStr)
#endif
//Defines the entry point for the DLL application. 
//Every time when process call to dll dllmain exec with ul_reason_for_call
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved	)
{
	BOOL fInit;
	BOOL fIgnore;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		D_COUT("DLL_PROCESS_ATTACH");
		createFileMap();
		break;
	case DLL_THREAD_ATTACH:
		D_COUT("DLL_THREAD_ATTACH");
		break;
	case DLL_THREAD_DETACH:
		D_COUT("DLL_THREAD_DETACH");
		break;
	case DLL_PROCESS_DETACH:
		D_COUT("DLL_PROCESS_DETACH");
		detProcess();
		break;
	default:
		break;
	}
	return TRUE;
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(lpReserved);
};
#ifdef _MANAGED
#pragma managed(pop)
#endif

#endif //EMULATION
