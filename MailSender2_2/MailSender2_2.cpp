#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <fstream>

#ifdef _UNICODE 
#define tcout		wcout 
#define tstring		wstring	
#else 
#define tcout		cout 
#define tstring		string
#endif

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMailSlot = NULL;
	TCHAR message[50];
	DWORD bytesWritten; //number of bytes write

	ifstream fin;
	fin.open("InheritableHandle.txt");
	if (fin.is_open())
	{
		fin >> hMailSlot;
	}
	fin.close();

	//FILE* pFile = _tfopen(_T("InheritableHandle.txt"), _T("rt"));
	//_ftscanf(pFile, _T("%d"), &hMailSlot);
	//fclose(pFile);

	cout << "SECOND Exe Sender mailSlot : " << hMailSlot << endl;


	while (1)
	{
		wprintf(_T(": "));
		_getws_s(message);

		if (!WriteFile(hMailSlot, message, _tcslen(message) * sizeof(TCHAR), &bytesWritten, NULL))
		{
			_fputts(_T("SECOND EXE : Unable to write!"), stdout);
			CloseHandle(hMailSlot);
			return 1;
		}


		if (!_tcscmp(message, _T("exit")))
		{
			_fputts(_T("SECOND EXE : Good Bye!"), stdout);
			break;
		}
	}
	CloseHandle(hMailSlot);
	return 0;
}
