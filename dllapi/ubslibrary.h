#ifdef EMULATION

#define UBS_LIBRARY_EXPORTS
#ifdef  UBS_LIBRARY_EXPORTS
#	define UBS_API __declspec(dllexport)
#else
#	define UBS_API __declspec(dllimport)
#endif

#define UBS_MEM_SIZE 0x80000000
#define MAPFILENAME TEXT("dllmemfilemap")

#ifdef __cplusplus
#include "Factory.h"
#include "stdafx.h"

// Handle type. In C++ language the iterface type is used.
typedef DllWrapperFactory * UBS_HANDLE;
// handle to file mapping
static HANDLE hMapObjectGL = nullptr;  

#else   // C

// Handle type. In C language there are no classes, so empty struct is used
// in order to ensure type safety.
typedef struct tagUBS_HANDLE {} *UBS_HANDLE;

#endif // __cplusplus

#ifdef __cplusplus
#	define EXTERN_C extern "C"
#else
#   define EXTERN_C
#endif // __cplusplus

// Factory function that creates instances of the UBS_ object.
EXTERN_C UBS_API UBS_HANDLE returnFactory();
// Initialize file map object
BOOL createFileMap();
// Close map object
BOOL detProcess();
#endif
