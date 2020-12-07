#include <iostream>
#include <windows.h>
#include <memoryapi.h>
#include <string>
#include <map>
#include <vector>
#include <stdlib.h>

typedef std::vector<std::string>(*MYFUNC)(int);

typedef struct
{
	int PhoneNumber;
	char FirstName[20];
	char SecondName[20];
	char LastName[20];
	char Address[40];
	short Dom;
	short Korpus;
	short Hata;
} Page;

TCHAR keyName[] = TEXT("WinApiTelephony");
TCHAR dllName[] = TEXT("TelephonyLib");
TCHAR fileName[] = TEXT("C:\\Users\\konst\\Desktop\\WinApiPhonebook\\Telephony\\x64\\Debug\\s");

std::vector<Page> pages;

void CreateFileMappingPages()
{
	HANDLE hFile;
	hFile = CreateFile(fileName, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	HANDLE hMapObject;
	int size = GetFileSize(hFile, NULL);

	hMapObject = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, size, keyName);

	if (hMapObject == NULL)
	{
		return;
	}
}

void DumpEntries()
{
	HANDLE hFile;
	hFile = CreateFile(fileName, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	HANDLE hMapObject;
	int size = pages.size() * sizeof(Page) + sizeof(DWORD);

	hMapObject = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, size, keyName);

	if (hMapObject == NULL) 
	{
		return;
	}

	void* vMapData;

	vMapData = MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, size);
	if (vMapData == NULL) 
	{
		CloseHandle(hMapObject);
	}

	(*(DWORD*)vMapData) = pages.size();
	Page* eArray = (Page*)(((DWORD*)vMapData) + 1);
	for (int i = pages.size() - 1; i >= 0; i--) eArray[i] = pages.at(i);

	UnmapViewOfFile(vMapData);
}

std::wstring GetExePath()
{
	TCHAR buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

void CopyData(char* dest, const char* source)
{
	for (int i = 0; i < std::strlen(source); i++)
	{
		dest[i] = source[i];
		dest[i + 1] = '\0';
	}
}

void CreateDump()
{
	Page temp1;
	Page temp2;

	temp1.PhoneNumber = 132;
	temp1.Dom = 42;
	temp1.Hata = 42;
	temp1.Korpus = 42;
	CopyData(temp1.Address, "asd");
	CopyData(temp1.FirstName, "asd");
	CopyData(temp1.SecondName, "asd");
	CopyData(temp1.LastName, "asd");

	temp2.PhoneNumber = 45;
	temp2.Dom = 1;
	temp2.Hata = 2;
	temp2.Korpus = 3;
	CopyData(temp2.Address, "e");
	CopyData(temp2.FirstName, "e");
	CopyData(temp2.SecondName, "aese");
	CopyData(temp2.LastName, "eeee");

	pages.push_back(temp1);
	pages.push_back(temp2);

	DumpEntries();
}

int main()
{	
	//CreateDump();	

	HINSTANCE hMyDll;
	if ((hMyDll = LoadLibrary(dllName)) == NULL)
	{ 
		
	}
	else 
	{
		MYFUNC pfnMyFunction;

		pfnMyFunction = (MYFUNC)GetProcAddress(hMyDll, "FindDataByPhoneNuber");

		std::vector<std::string> iCode = pfnMyFunction(3);
	}

	FreeLibrary(hMyDll);
}