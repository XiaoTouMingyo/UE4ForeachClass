#include "Process.h"
#include <TlHelp32.h>

DWORD GetProcessPid() {

	HWND hWnd = FindWindowA("UnrealWindow", NULL); //根据类名获取窗口
	DWORD Pid = 0;
	GetWindowThreadProcessId(hWnd, &Pid); //通过窗口获取进程ID

	if (Pid == 0)
	{
		return NULL;
	}

	return Pid;

}

HANDLE GetProcessHandle() {

	DWORD Pid = GetProcessPid();

	if (Pid == 0)
	{
		return NULL;
	}

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid); //打开进程

	if (handle == 0)
	{
		return NULL;
	}

	return handle;

}

HMODULE GetProcessMoudleBase() { //获取进程主模块

	HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessPid()); //  获取进程的所有的模块。

	if (handle == 0)
	{
		return NULL;
	}


	MODULEENTRY32 moduleEntry = {0};
	moduleEntry.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(handle, &moduleEntry)) { //获取进程第一个模块
		CloseHandle(handle);
		return NULL;
	}

	CloseHandle(handle);
	return moduleEntry.hModule;

}

DWORD64 ReadProcessDword(PVOID address) {

	DWORD64 buffer = 0;
	SIZE_T ret_size;
	ReadProcessMemory(GetProcessHandle(), address, &buffer, sizeof(DWORD64), 0);
	return buffer;
}


FName ReadProcessFname(PVOID address) {


	FName buffer = {0};
	SIZE_T ret_size;
	ReadProcessMemory(GetProcessHandle(), address, &buffer, sizeof(FName), 0);
	return buffer;
}


WORD ReadProcessWord(PVOID address) {
	WORD buffer = 0;
	SIZE_T ret_size;
	ReadProcessMemory(GetProcessHandle(), address, &buffer, sizeof(WORD), 0);
	return buffer;

}

bool ReadMemory(PVOID address, PVOID buffer, size_t size)
{
	SIZE_T ret_size;
	return ReadProcessMemory(GetProcessHandle(), address, buffer, size, &ret_size);
}
