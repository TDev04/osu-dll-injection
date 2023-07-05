// Header-File for the Win32 Application.
#pragma once

// Windows Header-Files:
#include <Windows.h>

// Definitions:

// Type-Definitions:

// Reference additional Header-Files:
#include <TlHelp32.h>
#include <iostream>

// Reference additional Functions:
BOOL InjectLibrary(DWORD dProcessId, LPCSTR lpcDll);
BOOL FindProcessIdByName(LPCWSTR lpProcess);