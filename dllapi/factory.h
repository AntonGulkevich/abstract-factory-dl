// Factory.h: interface for the Factory class.

#if !defined(AFX_FACTORY_H__E53DAB11_464D_48E3_9290_3C3460C1F740__INCLUDED_)
#define AFX_FACTORY_H__E53DAB11_464D_48E3_9290_3C3460C1F740__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dllwrapperfactory.h"
#include "singleton.h"


class Factory : public DllWrapperFactory
{
public:
	Factory(){};
	virtual ~Factory(){};
	CConfiguration * __stdcall CreateConfiguration(WorkManager* workManager) override;
	WorkManager * __stdcall CreateWorkManager(DriversIOManager* driversIO, DriverManager* drivers) override;
	CRegister * __stdcall CreateCRegister() override;
	DriverManager * __stdcall CreateDriverManager() override;
	DriversIOManager *__stdcall CreateDriversIoManager() override;
	PerfomanceCounter *__stdcall CreatePerfomanceCounter(DWORD baseAddress) override;
};

typedef CSingleton<Factory> FactorySingleton;

#endif // !defined(AFX_FACTORY_H__E53DAB11_464D_48E3_9290_3C3460C1F740__INCLUDED_)

