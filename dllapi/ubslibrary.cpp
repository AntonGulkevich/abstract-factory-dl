#ifdef EMULATION

#include "stdafx.h"
#include "UbsLibrary.h"


// Factory function that creates instances if the UBS_ object.
EXTERN_C UBS_API UBS_HANDLE returnFactory()
{
	//return (new CDebuggerComponentFactory());
	DllWrapperFactory * pObj = static_cast<DllWrapperFactory*>(&FactorySingleton::Instance());
	return pObj;
}

#endif
