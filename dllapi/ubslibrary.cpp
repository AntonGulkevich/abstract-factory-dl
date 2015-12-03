#ifdef EMULATION
#include "UbsLibrary.h"
// Factory function that creates instances if the UBS_ object.
EXTERN_C UBS_API  UBS_HANDLE returnFactory()
{
	DllWrapperFactory * pObj = static_cast<DllWrapperFactory*>(&FactorySingleton::Instance());
	return pObj;
}


BOOL createFileMap()
{	
	if (hMapObjectGL ==nullptr)
		hMapObjectGL = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, UBS_MEM_SIZE, MAPFILENAME);
	if (hMapObjectGL == nullptr)
		return FALSE;
	return TRUE;
}

BOOL detProcess()
{	
	return CloseHandle(hMapObjectGL);
}
#endif