#ifdef EXPORT_THIS
#define EXPORT _declspec(dllexport)
#else
#define EXPORT _declspec(dllimport)
#endif

enum eProducts
{
	PRODUCT1
};

class Base;
class Derived;

class DllWrapperFactory
{
public:
	DllWrapperFactory();
	virtual ~DllWrapperFactory();
	virtual Base * __stdcall    CreateProduct(eProducts iValue) = 0;
protected:
	virtual Derived * __stdcall CreateDerived() = 0;
};

class Factory : public DllWrapperFactory
{
public:
	Factory();
	virtual ~Factory();
	Base * __stdcall    CreateProduct(eProducts iValue);
protected:
	Derived * __stdcall CreateDerived();
};
