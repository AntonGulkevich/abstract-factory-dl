/*
 * workmanager.cpp
 *
 *  Created on: 24 апр. 2015 г.
 *      Author: Youbrin_A
 */

#include "workmanager.h"


#include "application/cconfiguration.h"

#include "drivermanager.h"
#include "driversiomanager.h"
#include "utilities/commod.h"
#include "utilities/diagnostic.h"
#include "utilities/console.h"
#include "utilities/failmanager.h"
#include "driversiohw/calibrate.h"
#include "utilities/calibrationmanager.h"
#include "utilities/menu.h"

WorkManager::WorkManager(DriversIOManager* driversIO, DriverManager* drivers) : _driversIO(driversIO), _drivers(drivers)
{
	RegFlag(Flags::Irq20ms, PRIORITY::Normal);
	RegFlag(Flags::Timer80ms, PRIORITY::BackGround);
	RegFlag(Flags::Timer500ms, PRIORITY::BackGround);
	RegFlag(Flags::Timer1Sec, PRIORITY::BackGround);
	_commod = new Commod(drivers->GetI2CCommod());
	_diagnostic = new Diagnostic(this);
	_console = new Console();
	Console::SetUsbTr(drivers->GetUSB());
	_config = new CConfiguration(this);
	_failManager = new FailManager(drivers->GetDisplay(), _config);
	_calibrationManager = new CalibrationManager(_drivers, _driversIO);
	_menu = new Menu(_drivers->GetKeys(), _drivers->GetDisplay());
}

WorkManager::~WorkManager()
{
	// TODO Auto-generated destructor stub
}

void WorkManager::Init()
{
	_calibrationManager->LoadCalibration();
	_calibrationManager->ApplyCalibration();

	_commod->Init();
	if (!_drivers->GetKeys()->IsBreakCombinationPressed())
		if (_config->LoadConfig() != CConfiguration::LOAD_RESULT::OK)
		{
			_drivers->GetDisplay()->SetTextByText("A210");
			for(;;);
		}
	_drivers->GetDisplay()->RunningZero();
}

DISPATH_RESULT WorkManager::ProcessFlag(Flags id)
{
	switch(id)
	{
	case Flags::Irq20ms:
		if (!_calibrationManager->IsCalibrationInProgress())
		{
			_drivers->GetPerfomance()->LogicStart();
			_config->ProcessLogic();
			_drivers->GetPerfomance()->LogicStop();
			_driversIO->UpdateOutputData();
			SetFlag(Flags::SendDiagnostic);
		} else
		{
			_calibrationManager->ProcessCalibration();
		}
		Gpio::SVSet();
		return drDispatched;
	case Flags::Timer80ms:
		_drivers->GetKeys()->UpdateState();
		_menu->ProcessInput();
		_menu->Draw();
		break;
	case Flags::Timer500ms:
	{
/*		auto val = _drivers->GetTermometr()->GetCurrentTemperature();
		_drivers->GetDisplay()->SetTextByValue((int)(val * 100), false);
		_drivers->GetDisplay()->SetPoint(false, true, false, false);*/

	}
		return drDispatched;
	case Flags::Timer1Sec:
		if (!_menu->IsActive())
			_failManager->ShowAlarms();
		_drivers->GetPerfomance()->Reset();
		return drDispatched;
	default:
		return drNotDispatched;
	}
}



