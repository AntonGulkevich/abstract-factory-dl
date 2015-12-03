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

DriverManager* Factory::CreateDriverManager()
{
	return (new DriverManager());
}

DriversIOManager* Factory::CreateDriversIoManager()
{
	return (new DriversIOManager());
}

PerfomanceCounter* Factory::CreatePerfomanceCounter(DWORD baseAddress)
{
	return (new PerfomanceCounter(baseAddress));
}
#endif
