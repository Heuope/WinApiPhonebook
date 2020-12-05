#include <iostream>
#include <windows.h>
#include <memoryapi.h>
#include <string>
#include <map>
#include <vector>
#include <stdlib.h>

#define BUFFERSIZE 5000
#define MEMSIZE 90024 

typedef int (__stdcall* PFN_MyFunction)();
typedef void(*MYFUNC)(char*);

TCHAR keyName[] = TEXT("WinApiCriticalSection");
TCHAR dllName[] = TEXT("TelephonyLib");
TCHAR fileName[] = TEXT("C:\\Users\\konst_9hggwum\\OneDrive\\Desktop\\WinApiPhonebook\\Telephony\\Debug\\s");

typedef struct
{
	int PhoneNumber;
	char FirstName[20];
} Page;

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

int main()
{
	Page temp1;
	Page temp2;
	char some[20] = "asd";

	temp1.PhoneNumber = 132;
	temp1.FirstName[0] = some[0];
	temp1.FirstName[1] = '\0';

	temp2.PhoneNumber = 21;
	temp2.FirstName[0] = some[0];
	temp2.FirstName[1] = '\0';

	pages.push_back(temp1);
	pages.push_back(temp2);

	DumpEntries();
	//CreateFileMappingPages();

	Page* readPages;
	DWORD number_of_entries = 0;

	ReadEntries(&readPages, number_of_entries);

	for (int i = 0; i < number_of_entries; i++)
	{
		directory[readPages[i].PhoneNumber] = &readPages[i];
		directoryKeys.push_back(std::to_string(readPages[i].PhoneNumber));
	}

	std::vector<Page> se = FindPages(2);

	if (number_of_entries != 0) delete readPages;

	//HINSTANCE hMyDll;
	//if ((hMyDll = LoadLibrary(dllName)) == NULL)
	//{ 
	//	/* не удалось загрузить DLL */ 
	//}
	//else 
	//{
	//	MYFUNC pfnMyFunction;

	//	pfnMyFunction = (MYFUNC)GetProcAddress(hMyDll, "Summa");

	//	int iCode = pfnMyFunction(1, 2);
	//	std::cout << iCode << std::endl;
	//}

	//FreeLibrary(hMyDll);
}