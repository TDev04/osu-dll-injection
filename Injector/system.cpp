#include "system.h"

// Get process id by its name

BOOL FindProcessIdByName(LPCWSTR lpProcess)
{
    PROCESSENTRY32 ProcessEntry;
    ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(hSnapshot, &ProcessEntry)) {
        do {
            if (!lstrcmp(ProcessEntry.szExeFile, lpProcess)) {
                CloseHandle(hSnapshot);
                return ProcessEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &ProcessEntry));
    }
}

// Perform the injection

BOOL InjectLibrary(DWORD ProcessId, LPCSTR DllPath)
{
	char pdll[MAX_PATH];
	GetFullPathNameA(DllPath, MAX_PATH, pdll, NULL);

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	if (hProc == NULL) 
	{
		MessageBox(NULL, L"Process couldn't be attached. Perhaps you require elevation? Or isn't running?", L"Injector", MB_OK | MB_ICONEXCLAMATION);
		return EXIT_FAILURE;
	}

	LPVOID lpRemoteMem = VirtualAllocEx(hProc, NULL, strlen(pdll), MEM_COMMIT, PAGE_READWRITE);
	if (lpRemoteMem == NULL) 
	{
		MessageBox(NULL, L"Allocation error.", L"Injector", MB_OK | MB_ICONEXCLAMATION);
		return EXIT_FAILURE;
	}

	if (WriteProcessMemory(hProc, lpRemoteMem, pdll, strlen(pdll), 0) == NULL) 
	{
		MessageBox(NULL, L"Error writting to the process memory.", L"Injector", MB_OK | MB_ICONEXCLAMATION);
		return EXIT_FAILURE;
	}

	HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), lpRemoteMem, NULL, NULL);
	if (hRemoteThread == NULL) 
	{
		MessageBox(NULL, L"Couldn't create a thread to the target. This app in some cases must be executed as administrator to work.", L"Injector", MB_OK | MB_ICONEXCLAMATION);
		return EXIT_FAILURE;
	}

	WaitForSingleObject(hRemoteThread, INFINITE);

	VirtualFreeEx(hProc, lpRemoteMem, strlen(pdll), MEM_RELEASE);
	CloseHandle(hProc);
	CloseHandle(hRemoteThread);
}