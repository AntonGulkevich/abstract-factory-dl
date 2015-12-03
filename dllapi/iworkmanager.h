#ifndef IWORKMANAGER
#define IWORKMANAGER

#include "../types.h"

struct CalibrationManager;
struct FailManager;
struct CConfiguration;
struct Console;
struct Diagnostic;
struct Commod;
enum class Flags;
struct DriverManager;
struct DriversIOManager;

struct IWorkManager
{
	virtual ~IWorkManager(){};
	virtual void Init() const = 0;
	virtual DISPATH_RESULT ProcessFlag(Flags id) = 0;
	virtual DriverManager* GetDrivers() const = 0;
	virtual DriversIOManager* GetDriversIO() const = 0;
	virtual Commod* GetCommod() const = 0;
	virtual Diagnostic* GetDiagnostic() const = 0;
	virtual Console* GetConsole() const = 0;
	virtual CConfiguration* GetConfiguration() const = 0;
	virtual FailManager* GetFailManager() const = 0;
	virtual CalibrationManager* GetCalibrationManager() const = 0;
	virtual bool ValidateConfig() const = 0;
	virtual void initCommod() const = 0;
};

#endif
