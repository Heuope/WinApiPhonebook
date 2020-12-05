#include <windows.h>
#include <memoryapi.h>
#include <map>
#include <vector>
#include <string>
#include <stdlib.h>

TCHAR keyName[] = TEXT("WinApiCriticalSection");
TCHAR fileName[] = TEXT("C:\\Users\\konst_9hggwum\\source\\repos\\Telephony\\Telephony\\Debug\\s");
std::map <int, Page*> directory;
std::vector<std::string> directoryKeys;
std::vector<Page> pages;

struct Page
{
    int PhoneNumber;
    char FirstdName[20];
    char SecondName[20];
    char LastName[20];
    char Address[40];
    short Dom;
    short Korpus;
    short Hata;
};

void CreateFileMappingPages()
{
	HANDLE hFile;
	hFile = CreateFile(fileName, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	HANDLE hMapObject;
	int size = GetFileSize(hFile, NULL);

	hMapObject = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, keyName);

	if (hMapObject == NULL)
	{
		return;
	}
}

std::vector<Page> FindPages(int number)
{
	std::string numberString = std::to_string(number);

	std::vector<Page> findedPages;

	for (int i = 0; i < directoryKeys.size(); i++)
	{
		if (directoryKeys.at(i).find(numberString) != std::string::npos)
		{
			findedPages.push_back(*directory[std::stoi(directoryKeys.at(i))]);
		}
	}

	return findedPages;
}

void DumpEntries(std::vector<Page> pages)
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

extern "C" _declspec(dllexport) void ReadData(std::map<int, Page> *directory)
{
	CreateFileMappingPages();

	Page* readPages;
	DWORD number_of_entries = 0;

	ReadEntries(&readPages, number_of_entries);

	for (int i = 0; i < number_of_entries; i++)
	{
		(*directory)[readPages[i].PhoneNumber] = readPages[i];
	}

	if (number_of_entries != 0) delete readPages;
}