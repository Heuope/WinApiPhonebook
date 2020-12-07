#include <iostream>
#include <windows.h>
#include <memoryapi.h>
#include <string>
#include <map>
#include <vector>
#include <stdlib.h>

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
TCHAR fileName[] = TEXT("..\\telephonyBD");

std::vector<Page> pages;

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

void CopyData(char* dest, const char* source)
{
	for (int i = 0; i < std::strlen(source); i++)
	{
		dest[i] = source[i];
		dest[i + 1] = '\0';
	}
}

void CopyPage(Page* dest, Page* source)
{
	dest->PhoneNumber = source->PhoneNumber;
	dest->Dom = source->Dom;
	dest->Hata = source->Hata;
	dest->Korpus = source->Korpus;
	CopyData(dest->Address, source->Address);
	CopyData(dest->FirstName, source->FirstName);
	CopyData(dest->SecondName, source->SecondName);
	CopyData(dest->LastName, source->LastName);
}

std::string CreateRandomName()
{
	std::string result;
	std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < rand() % 10 + 2; i++)
	{
		result += alphabet[rand() % 25];
	}

	return result;
}

std::vector<Page> CreateRandomPages()
{
	std::vector<Page> result;
	
	for (int i = 0; i < rand() % 50 + 25; i++)
	{
		Page temp;

		temp.PhoneNumber = i;
		temp.Dom = rand() % 100 + 1;
		temp.Hata = rand() % 150 + 1;
		temp.Korpus = rand() % 250 + 1;		
		CopyData(temp.Address, CreateRandomName().c_str());
		CopyData(temp.FirstName, CreateRandomName().c_str());
		CopyData(temp.SecondName, CreateRandomName().c_str());
		CopyData(temp.LastName, CreateRandomName().c_str());

		result.push_back(temp);
	}

	return result;
}

void CreateDump()
{
	std::vector<Page> temp = CreateRandomPages();

	for (int i = 0; i < temp.size(); i++)
	{
		pages.push_back(temp.at(i));
	}

	DumpEntries();
}

int main()
{	
	CreateDump();
}