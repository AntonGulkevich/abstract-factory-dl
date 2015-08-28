#include "cregister.h"
#include <cstring>

const char* CRegister::noname = "noname";

CRegister::CRegister() : filled(false)	
{ 
	dataArray = NULL; 
	dataStr = NULL;
	sizeArray = 0;
	_data = 0;
	_emulatedData = 0;
	filled = false;
	isEmulated = false;
}

CRegister::~CRegister()	
{ 
	if (dataArray != NULL) 
		delete[] dataArray;
	if (dataStr)
		delete[] dataStr;
} 

void CRegister::SetValueArray(float* arr, UINT size)
{ 
	if (dataArray)
		delete[] dataArray;
	if (size > 1000)
		size = 1000;
	dataArray = new float[size];
	memcpy(dataArray, arr, size * sizeof(float));
	sizeArray = size; 
	filled = true; 
}

void CRegister::SetValue(const char* lData)	
{ 
	if (dataStr)
		delete[] dataStr;
	WORD size = strlen(lData)+1;
	if (size > 1000)
		size = 1000;
	dataStr = new char[size];
	memcpy(dataStr, lData, size);
	dataStr[size-1] = 0;
	filled = true; 
}

CRegister& CRegister::Copy(const CRegister& copy)
{
	sizeArray = copy.sizeArray;
	if (copy.dataArray && sizeArray > 0 && sizeArray < 0xFF)
	{
		dataArray = new float[sizeArray];
		memcpy(dataArray, copy.dataArray, sizeArray * sizeof(float));
	}
	else
		dataArray = NULL;
	if (copy.dataStr)
	{
		WORD size = strlen(copy.dataStr)+1;
		if (size > 1000)
			size = 1000;
		dataStr = new char[size];
		memcpy(dataStr, copy.dataStr, size);
	} else
		dataStr = NULL;
	_data = copy._data;
	filled = copy.filled;
	return (*this);
}

const char* CRegister::GetRusName(REGISTER_ID rId)
{
	switch(rId)
	{
	case REGISTER_ID::rCORRECTIONDELTA:
		return "Добавка ";
	case REGISTER_ID::rCORRECTIONSIGN:
		return "Знак отработки ";
	case REGISTER_ID::rDELAY:
		return "Задержка ";
    default:
        return "Неизвестный ";
	}
}


