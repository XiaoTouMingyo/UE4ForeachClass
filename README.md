# UE4ForeachClass
UE4遍历类名工具


### 需要修改如下：

```
DWORD64 GNameOffset = 0x2E6E0C0;
```

以及算法中的0xc：

```
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

```
