/*
 * workmanager.h
 *
 *  Created on: 24 апр. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef WORKMANAGER_H_
#define WORKMANAGER_H_
#include "driversiomanager.h"
#include "interfaces/idispatchflags.h"
#include "interfaces/iusingflags.h"
#include "types.h"

class DriverManager;
class Commod;
class Diagnostic;
class Console;
class CConfiguration;
class FailManager;
class CalibrationManager;
class Menu;


class WorkManager : public IDispatchFlags, public IUsingFlags
{
private:
	DriversIOManager* _driversIO;
	DriverManager* _drivers;

	Commod* _commod;
	Diagnostic* _diagnostic;
	Console* _console;
	CConfiguration* _config;
	FailManager* _failManager;
	CalibrationManager* _calibrationManager;
	Menu* _menu;


public:
	WorkManager(DriversIOManager* driversIO, DriverManager* drivers);
	virtual ~WorkManager();

	void Init();
	DISPATH_RESULT ProcessFlag(Flags id);


	DriverManager* GetDrivers() { return _drivers; }
	DriversIOManager* GetDriversIO() { return _driversIO; }
	Commod* GetCommod() { return _commod; }
	Diagnostic* GetDiagnostic() { return _diagnostic; }
	Console* GetConsole() { return _console; }
	CConfiguration* GetConfiguration() { return _config; }
	FailManager* GetFailManager() { return _failManager; }
	CalibrationManager* GetCalibrationManager() { return _calibrationManager; }


};

#endif /* WORKMANAGER_H_ */
