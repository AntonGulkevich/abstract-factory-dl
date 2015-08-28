#ifndef MATH_H
#define MATH_H
#include "../types.h"
#include <cmath>
#include <iterator>
//#include <complex>

#define m_abs(X) (((X)<0) ? -(X) : (X))
#define PI       3.14159265358979323846f
#define ROUND(X) ((X)>0 ? (int)((X) + .5f) : (int)((X) - .5f))
#define SQR(X) ((X) * (X))
#define REVERSEOCT(X) (octTable[X])

extern const BYTE octTable[378];
unsigned int ftou__(float constant);

template<class T> DWORD m_min (T* data, DWORD size)
{
	if (size > 1)
	{
		DWORD min = 0;
		for(DWORD i = 1; i < size; i++)
			if (data[min] > data[i])
				min = i;
		return min;
	} else
		return 0;

}

int NOK(int a, int b, int& gcd); // Наименьшее общее кратное  /  gcd - НОД


// Класс подсчитывающий контрольную сумму по алгоритму CRC32
class CRC32
{
protected:
	UINT table[256];
	UINT _crc32;
public:
	CRC32();
    virtual ~CRC32() {}
	void ProcessCRC(void* pData, DWORD nLen);
	UINT GetCRC32() { return _crc32; }
    void Reset() { _crc32 = 0; }
};

#define m_sin(X) sinf((X)*(PI/180.0f))
#define m_cos(X) cosf((X)*(PI/180.0f))
#define m_sincos(X, S, C) sincosf((X)*(PI/180.0f), S, C)
#define m_atan(X) ((180.0f/PI) * atanf(X))
#define m_atan2(X, Y) ((180.0f/PI) * atan2f(X, Y))

UINT bitReverse (unsigned int x, int log2n);

template<class Iter_T> 
void fft (Iter_T a, Iter_T b, UINT log2n)
{   
	using namespace std;
	typedef typename iterator_traits<Iter_T>::value_type complex;  
	const complex J(0, 1);  
	UINT n = 1 << log2n;  
	for (UINT i = 0; i < n; ++i) 
	{    
		b[bitReverse(i, log2n)] = a[i];  
	}  
	for (UINT s = 1; s <= log2n; ++s) 
	{    
		UINT m  = 1 << s;    
		UINT m2 = m >> 1;    
		complex w(1, 0);    
		complex wm = exp (-J * (PI / m2));    
		for (UINT j = 0; j < m2; ++j) 
		{      
			for (UINT k = j; k < n; k += m) 
			{        
				complex t = w * b[k + m2];        
				complex u = b[k];        
				b[k] = u + t;        
				b[k+m2] = u - t;      
			}      
			w *= wm;    
		}  
	}
}

#endif
