#ifndef CREGISTER_H
#define CREGISTER_H

#include "../utilities/m_math.h"

class Diagnostic;
//  ласс управл€ющий значением "регистра устройства".
class CRegister
{
public:
	CRegister();
	virtual ~CRegister();

	float GetValueFloat()		const { return (float)GetValueUCUFloat(); }
	UCU_FLOAT GetValueUCUFloat()		const { return isEmulated ? _emulatedData : _data; }
	int GetValueInt()			const { return (int)ftou__(GetValueUCUFloat()); }
	UINT GetValueUInt()			const { return ftou__(GetValueUCUFloat()); }
	float GetValueArray(UINT num) const { return ((num < sizeArray) ? dataArray[num] : 0); }
	BYTE GetArraySize() const { return sizeArray; }
	float* GetArray() const { return dataArray; }

	const char* GetValueStr()	const { return (dataStr) ? dataStr : noname; }
	bool IsFilled()				const { return filled; }

	void SetValue(const UINT lData)			{ _data = (UCU_FLOAT)lData;	filled = true; }
	void SetValue(const int lData)			{ _data = (UCU_FLOAT)lData;	filled = true; }
	void SetValue(const float lData)		{ _data = (UCU_FLOAT)lData;	filled = true; }
//	void SetValue(const UCU_FLOAT lData)		{ data = lData;	filled = true; }
	void SetValue(const char* lData);
	void SetValueArray(float* arr, UINT size);

	void SetFilled(const bool lFilled = true)	{ filled = lFilled; }
	CRegister& operator=(const CRegister& copy) { return Copy(copy); }

	static const char* GetRusName(REGISTER_ID rId);

	friend Diagnostic;
	// ”брать после отладки
	bool IsEmulated() { return isEmulated; }
private:
	CRegister& Copy(const CRegister& copy);
	void SetEmulated(bool isEmul) { isEmulated = isEmul; }
	bool GetEmulated() { return isEmulated; }
	void SetEmulatedValue(float lData)		{ _emulatedData = (UCU_FLOAT)lData; }
	// „исловое значение регистра
	UCU_FLOAT _data;
	// Ёмулируемое числовое значение регистра
	UCU_FLOAT _emulatedData;
	// “екстовое значение регистра
	//std::string dataStr;
	char* dataStr;
	// массив
	float* dataArray;
	BYTE sizeArray;
	// ѕризнак заполненного регистра
	bool filled;
	// Ёмул€ци€ значени€ из диагностики
	bool isEmulated;
	static const char* noname;
};

#endif

