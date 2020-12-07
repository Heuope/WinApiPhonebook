#include <iostream>
#include <windows.h>
#include <memoryapi.h>
#include <string>
#include <map>
#include <vector>
#include <stdlib.h>

TCHAR keyName[] = TEXT("WinApiTelephony");
TCHAR fileName[] = TEXT("C:\\Users\\konst\\Desktop\\WinApiPhonebook\\Telephony\\x64\\Debug\\s");

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

Page* readPages;
std::map <int, Page*> directory;
std::vector<std::string> directoryKeys;
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

void ReadEntries(Page** entries, DWORD& number_of_entries)
{
	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, keyName);
	if (hMapFile == NULL)
	{
		return;
	}

	DWORD* num_entries = (DWORD*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (num_entries == NULL)
	{
		CloseHandle(hMapFile);
	}
	number_of_entries = *num_entries;

	if (number_of_entries == 0)
	{
		*entries = NULL;
	}

	Page* tmpEntries = (Page*)(num_entries + 1);

	*entries = new Page[*num_entries];

	for (UINT i = 0; i < *num_entries; i++)
	{
		(*entries)[i] = tmpEntries[i];
	}

	UnmapViewOfFile(num_entries);
	CloseHandle(hMapFile);
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

void ClearData()
{
	delete readPages;
}

void ReadData()
{
	CreateFileMappingPages();

	DWORD number_of_entries = 0;

	ReadEntries(&readPages, number_of_entries);

	for (int i = 0; i < number_of_entries; i++)
	{
		directory[readPages[i].PhoneNumber] = &readPages[i];
		directoryKeys.push_back(std::to_string(readPages[i].PhoneNumber));
	}
}

std::vector<std::string> TransformPagesInString(std::vector<Page> * pages)
{
	std::vector<std::string> result;

	for (int i = 0; i < pages->size(); i++)
	{
		std::string temp;
		temp += std::to_string(pages->at(i).PhoneNumber);
		temp += " | ";
		temp += pages->at(i).FirstName;
		temp += " | ";
		temp += pages->at(i).SecondName;
		temp += " | ";
		temp += pages->at(i).LastName;
		temp += " | ";
		temp += pages->at(i).Address;
		temp += " | ";
		temp += std::to_string(pages->at(i).Dom);
		temp += " | ";
		temp += std::to_string(pages->at(i).Korpus);
		temp += " | ";
		temp += std::to_string(pages->at(i).Hata);

		result.push_back(temp);
	}
	return result;
}

std::vector<Page> FindPages(int number)
{
	if (directoryKeys.size() == 0)
		ReadData();

	std::string numberString = std::to_string(number);

	std::vector<Page> findedPages;

	if (number == -1)
	{
		for (int i = 0; i < directoryKeys.size(); i++)
		{
			findedPages.push_back(*directory[std::stoi(directoryKeys.at(i))]);
		}
		return findedPages;
	}

	for (int i = 0; i < directoryKeys.size(); i++)
	{
		if (directoryKeys.at(i).find(numberString) != std::string::npos)
		{
			findedPages.push_back(*directory[std::stoi(directoryKeys.at(i))]);
		}
	}

	return findedPages;
}

extern "C" _declspec(dllexport) std::vector<std::string> FindDataByPhoneNuber(int number)
{
	std::vector<Page> tempPages = FindPages(number);
	std::vector<std::string> result = TransformPagesInString(&tempPages);
	
	return result;
}