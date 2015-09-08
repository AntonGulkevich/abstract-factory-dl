#ifdef EMULATION

#if !defined(AFX_DLLWRAPPERFACTORY_H__6878082E_F1BB_4FA5_A493_45E9EF20959D__INCLUDED_)
#define AFX_DLLWRAPPERFACTORY_H__6878082E_F1BB_4FA5_A493_45E9EF20959D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../driversio/ichannel.h"
#include "../compilier/CParser.h"
#include "../workmanager.h"
#include "../driversio/cregister.h"
#include "../application/cconfiguration.h"

class DriverManager;
class DriversIOManager;
class CPattern;
class Commod;
class Console;
enum eProducts
{
	CONFIGURATION, 
	PARCER,
	CHANNEL,
	WORKMANAGER,
	REGISTER
};

class DllWrapperFactory
{
public:
	DllWrapperFactory(){};
	virtual ~DllWrapperFactory(){};
	virtual CConfiguration * __stdcall CreateConfiguration(WorkManager* workManager) = 0;
	virtual WorkManager * __stdcall CreateWorkManager(DriversIOManager* driversIO, DriverManager* drivers) = 0;
	virtual CRegister * __stdcall CreateCRegister() = 0;
};

#endif // !defined(AFX_DLLWRAPPERFACTORY_H__6878082E_F1BB_4FA5_A493_45E9EF20959D__INCLUDED_)

#endif
