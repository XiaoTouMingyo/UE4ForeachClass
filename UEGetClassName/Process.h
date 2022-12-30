#include <Windows.h>

struct FName
{
	int ComparisonIndex;
	int Number;
};

class UObject
{
public:
	void* vtf; //0x0000
	char pad_0008[4]; //0x0008
	DWORD Index; //0x000C
	DWORD64 Class; //0x0010
	FName Name; //0x0018
	DWORD64 Outer; //0x0020
};


HANDLE GetProcessHandle();

HMODULE GetProcessMoudleBase();

DWORD64 ReadProcessDword(PVOID address);
FName ReadProcessFname(PVOID address);
WORD ReadProcessWord(PVOID address);


bool ReadMemory(PVOID address, PVOID buffer, size_t size);