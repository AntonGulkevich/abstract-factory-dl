#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <SDKDDKVer.h> //Allow to detect Windows version 
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <memory.h>
#include <string>
