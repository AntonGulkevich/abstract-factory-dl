#ifdef EMULATION
#include "SharedMemory.h"
#include <iostream>

BOOL writeData(const HANDLE hMapObject, LPBYTE data, DWORD size, DWORD offset)
{
	DWORD multiOffset = (offset / PAGE_SIZE_SYS)*PAGE_SIZE_SYS;
	DWORD deltaOffset = offset - multiOffset;
	LPBYTE lpvMem = static_cast<LPBYTE>(MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, multiOffset, size + deltaOffset));
	if (lpvMem == nullptr)	return FALSE;
	lpvMem += deltaOffset;
	while (size--)	*lpvMem++ = *data++;
	UnmapViewOfFile(lpvMem);
	return TRUE;
}

BOOL readData(const HANDLE hMapObject, LPBYTE data, DWORD size, DWORD offset)
{
	DWORD multiOffset = (offset / PAGE_SIZE_SYS)*PAGE_SIZE_SYS;
	DWORD deltaOffset = offset - multiOffset;
	LPBYTE lpvMem = static_cast<LPBYTE>(MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, multiOffset, size + deltaOffset));
	if (lpvMem == nullptr)	return FALSE;
	lpvMem += deltaOffset;
	memcpy(data, lpvMem, size);
	UnmapViewOfFile(lpvMem);
	return TRUE;
}

FLOAT read_float(DWORD base, DWORD offset)
{
	HANDLE hMapObject = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, MAPFILENAME);
	if (hMapObject == nullptr) return NULL;
	int size = sizeof(DWORD);
	LPBYTE buff = new BYTE[size];

	readData(hMapObject, buff, size, base + offset);

	FLOAT res;
	memcpy(&res, buff, size);
	delete[] buff;
	CloseHandle(hMapObject);
	return res;
}
DWORD read_dword(DWORD base, DWORD offset)
{
	HANDLE hMapObject = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, MAPFILENAME);
	if (hMapObject == nullptr) return NULL; 
	int size = sizeof(DWORD);
	LPBYTE buff = new BYTE[size];

	readData(hMapObject, buff, size, base + offset);

	DWORD res;
	memcpy(&res, buff, size);
	delete[] buff;
	CloseHandle(hMapObject);
	return res;
}

VOID write_float(DWORD base, DWORD offset, FLOAT data)
{
	HANDLE hMapObject = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, MAPFILENAME);
	if (hMapObject == nullptr) return;
	int size = sizeof(FLOAT);
	LPBYTE buff = new BYTE[size];
	memcpy(buff, &data, size);
	writeData(hMapObject, buff, size, base + offset);
	delete[] buff;
	CloseHandle(hMapObject);
}

VOID write_dword(DWORD base, DWORD offset, DWORD data)
{
	HANDLE hMapObject = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, MAPFILENAME);
	if (hMapObject == nullptr) return;
	int size = sizeof(DWORD);
	LPBYTE buff = new BYTE[size];
	memcpy(buff, &data, size);
	writeData(hMapObject, buff, size, base + offset);
	delete[] buff;
	CloseHandle(hMapObject);
}
#endif
