#ifndef ICREGISTER
#define ICREGISTER
#include "../types.h"

struct ICRegister
{
	virtual ~ICRegister() {};
	virtual float GetValueFloat() const = 0;
	virtual UCU_FLOAT GetValueUCUFloat() const = 0;
	virtual int GetValueInt() const = 0;
	virtual UINT GetValueUInt()  const = 0;
	virtual float GetValueArray(UINT num) const = 0;
	virtual BYTE GetArraySize() const  = 0;
	virtual float* GetArray() const  = 0;
	virtual const char* GetValueStr() const = 0;
	virtual bool IsFilled() const = 0;
	virtual void SetValue(const UINT lData) = 0;
	virtual void SetValue(const int lData) = 0;
	virtual void SetValue(const float lData) = 0;
	virtual void SetValue(const char* lData) = 0;
	virtual void SetValueArray(float* arr, UINT size) = 0;
	virtual void SetFilled(const bool lFilled = true) = 0;
	virtual bool IsEmulated() const = 0;
};
#endif
