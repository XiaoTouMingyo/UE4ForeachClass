#include "Process.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>


DWORD64 GNames = 0;
DWORD64 GNameOffset = 0x2E6E0C0;

std::string GetName_Old(int i) //旧版本算法
{
	DWORD64 G_Names = ReadProcessDword((PVOID)GNames);
	int Id = (int)(i / (int)0x4000);
	int Idtemp = (int)(i % (int)0x4000);
	auto NamePtr = ReadProcessDword((PVOID)(G_Names + Id * 8));
	auto Name = ReadProcessDword((PVOID)(NamePtr + 8 * Idtemp));
	char name[0x100] = { 0 };
	if (ReadMemory(PVOID(Name + 0xC), name, 0x100))
	{
		return name;
	}
	return std::string();
}

std::string GetName_New(int key) //新版本算法
{
	DWORD chunkOffset = ((int)(key) >> 16); // Block
	WORD nameOffset = key;
	auto namePoolChunk = ReadProcessDword((LPVOID)(GNames + ((chunkOffset + 2) * 8)));
	auto entryOffset = namePoolChunk + (uint32_t)(2 * nameOffset);
	WORD nameLength = ReadProcessWord((PVOID)entryOffset) >> 6;
	if (nameLength > 256)nameLength = 255;
	static char cBuf[256];
	if (ReadMemory((LPVOID)(entryOffset + 2), cBuf, nameLength))
	{
		cBuf[nameLength] = 0;
		return cBuf;
	}
	return std::string();
}

/*

在UE4中每个类都会继承Uobject对象，不然不会被引擎承认是一个类

*/
DWORD64 GetClass(DWORD64 Address) {

	DWORD64 UobjectClass = ReadProcessDword((PVOID)(Address + 0x10));
	if (UobjectClass != NULL)
	{
		return UobjectClass;
	}

	return NULL;

}

DWORD64 GetOuter(DWORD64 Address)
{
	auto outer = ReadProcessDword((PVOID)(Address + 0x20));
	if (outer != NULL)
		return outer;
	return uint64_t();
};

FName GetFName(DWORD64 Address) {

	FName Name = ReadProcessFname((PVOID)(Address + 0x18));
	if (Name.ComparisonIndex)
		return Name;
	return FName{};

}

std::string GetName(DWORD64 Address) {

	int FnameComparisonIndex = GetFName(Address).ComparisonIndex; //这里获取的是基类的Gname编号
	std::string GetName = GetName_Old(FnameComparisonIndex); //旧算法获取Name

	return GetName;
}

std::string GetClassName(DWORD64 Address) {

	DWORD64 UobjectClass = GetClass(Address); //获取基类地址
	std::string name = GetName(UobjectClass);
	return name;
}

std::string GetOuterName(DWORD64 Address)
{
	auto klass = GetClass(Address);
	if (klass != NULL)
	{
		std::string temp;
		for (auto outer = GetOuter(Address); outer!=0; outer = GetOuter(outer))
		{
			temp = GetName(outer) + "." + temp;
		}

		temp += GetName(Address);  //自己的类名

		return temp;
	}

	return std::string("(null)");
}


void main() {

	GNames = (DWORD64)GetProcessMoudleBase() + GNameOffset; //获取GName地址
	
	DWORD64 Address = 0x7FF638A61060; //遍历地址

	for (size_t i = 0; i < 0x300; i++)
	{

		DWORD64 Tmp = ReadProcessDword((PVOID)(Address+i));
		std::string Klass = GetClassName(Tmp);
		std::string outerName = GetOuterName(Tmp);
		if (Klass.empty() || outerName.empty())
		{
			continue;
		}

		if (outerName.find("null") != outerName.npos || outerName.find("None") != outerName.npos)
		{
			continue;
		}
		printf("[%p] %s %s \r\n", (Address + i), outerName.c_str(), Klass.c_str());

	}


}
