/*
 * calibrationmanager.h
 *
 *  Created on: 14 авг. 2015 г.
 *      Author: Youbrin_A
 */

#ifndef CALIBRATIONMANAGER_H_
#define CALIBRATIONMANAGER_H_
#include "../driversiomanager.h"

class DriverManager;

class CalibrationManager
{
private:
	DriverManager* _drivers;
	DriversIOManager* _driversIO;


	// процедура калибровки
	DriversIOManager::Calibration _calibration;
	UINT _calibrateState;
	short _calib[DriversIOManager::SCTOutCount*2][50];

	// Проверка корректности калибровок
	bool CheckCalibration(DriversIOManager::Calibration& calib);
	void CalcAvg(int gainIndex);
	void CalcAvgSCT(int number, int regNum);
	void CalcScaleSCT(int number);
	void SaveCalibration();
	void ClearCalibration();

public:
	CalibrationManager(DriverManager* drivers, DriversIOManager* _driversIO);
	virtual ~CalibrationManager() {}

	void LoadCalibration();
	void ApplyCalibration();

	//Калибровка
	DriversIOManager::Calibration& GetCalibration();
	DWORD GetBlockNumber() { return _calibration.blockNumber; }
	// Проверяет корректность калибровок и запоминает их, возвращает false если калибровки не корректны
	bool SetCalibration(DriversIOManager::Calibration& calib);
	void StartCalibrationIn();
	bool IsCalibrationInProgress() { return _calibrateState > 0; }
	void ProcessCalibration();


};

#endif /* CALIBRATIONMANAGER_H_ */
