#include  "Factory.h"

extern "C" EXPORT DllWrapperFactory * returnFactory()
{
	DllWrapperFactory * pObj =
		static_cast<dllwrapperfactory*>(&FactorySingleton::Instance());
	return pObj;
}