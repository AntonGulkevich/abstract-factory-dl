#ifdef EXPORT_THIS
#define EXPORT _declspec(dllexport)
#else
#define EXPORT _declspec(dllimport)
#endif

#include "application/cconfiguration.h"
#include "compilier/CParser.h"

//abstract factory + pure virtual functions
class BaseAPI
{
public:
	virtual 
};
