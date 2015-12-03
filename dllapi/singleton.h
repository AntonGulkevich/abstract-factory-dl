#ifdef EMULATION

#if !defined(AFX_SINGLETON_H__3D2EDB15_46F9_46E5_9BDC_792A0D30FF02__INCLUDED_)
#define AFX_SINGLETON_H__3D2EDB15_46F9_46E5_9BDC_792A0D30FF02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* good old meyers' singleton */

template<class T>
class CSingleton
{
public:
	static T& Instance()
	{
		static T theSingleton;
		return theSingleton;
	}
	/* more (non-static) functions here */
private:
	CSingleton(){};								// ctor hidden
	CSingleton(CSingleton const&);				// copy ctor hidden
	CSingleton& operator=(CSingleton const&);	// assign op. hidden
	~CSingleton(){};							// dtor hidden
};

#endif // !defined(AFX_SINGLETON_H__3D2EDB15_46F9_46E5_9BDC_792A0D30FF02__INCLUDED_)
#endif
