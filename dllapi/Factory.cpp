// Factory.cpp: implementation of the Factory class.
#ifdef EMULATION
#include "Factory.h"


CConfiguration* Factory::CreateConfiguration(WorkManager* workManager)
{
	return (new CConfiguration(workManager) );
}

WorkManager* Factory::CreateWorkManager(DriversIOManager* driversIO, DriverManager* drivers)
{
	return (new WorkManager(driversIO, drivers));
}

CRegister* Factory::CreateCRegister()
{
	return (new CRegister());
}
#endif
