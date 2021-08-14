// MailSender.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

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

#define SLOT_NAME _T("\\\\.\\mailslot\\temp")

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMailSlot;
	TCHAR message[50];
	DWORD bytesWritten; //number of bytes write


	// 상속 부분.
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = nullptr;
	sa.bInheritHandle = true;


	hMailSlot = CreateFile(
		SLOT_NAME, //메일슬롯의 이름(주소)
		GENERIC_WRITE, //사용되는 용도
		FILE_SHARE_READ,
		&sa,
		OPEN_EXISTING, //생성 방식
		FILE_ATTRIBUTE_NORMAL,
		NULL);


	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to create mailslot!\n"), stdout);
		return 1;
	}

	cout << "SENDER hMailSlot = " << hMailSlot << endl;

	
	ofstream fout;
	fout.open("InheritableHandle.txt");
	
	if (fout.is_open())
	{
		fout << hMailSlot;
	}
	fout.close();
	
	//FILE* pFile = _tfopen(_T("InheritableHandle.txt"), _T("wt"));

	//_ftprintf(pFile, _T("%d"), hMailSlot);
	//fclose(pFile);


	TCHAR command[] = _T("MailSender2_2.exe");

	STARTUPINFO si = {0,};
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	CreateProcess(NULL, command, NULL, NULL
		, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);



	while (1)
	{
		wprintf(_T(": "));
		_getws_s(message);

		if (!WriteFile(hMailSlot, message, _tcslen(message) * sizeof(TCHAR), &bytesWritten, NULL))
		{
			_fputts(_T("Unable to write!"), stdout);
			CloseHandle(hMailSlot);
			return 1;
		}

		if (!_tcscmp(message, _T("exit")))
		{
			_fputts(_T("Good Bye!"), stdout);
			break;
		}
	}
	CloseHandle(hMailSlot);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	
	return 0;
}


