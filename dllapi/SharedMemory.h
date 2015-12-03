#ifdef EMULATION
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <windows.h>

#define UBS_MEM_SIZE 0xffffffff
#define PAGE_SIZE_SYS 0x10000
#define MAPFILENAME TEXT("dllmemfilemap")


BOOL writeData(const HANDLE hMapObject, LPBYTE data, DWORD size, DWORD offset);
BOOL readData(const HANDLE hMapObject, LPBYTE data, DWORD size, DWORD offset);
VOID write_dword(DWORD base, DWORD offset, DWORD data);
VOID write_float(DWORD base, DWORD offset, FLOAT data);
DWORD read_dword(DWORD base, DWORD offset);
FLOAT read_float(DWORD base, DWORD offset);
#endif

#endif //SHARED_MEMORY_H
