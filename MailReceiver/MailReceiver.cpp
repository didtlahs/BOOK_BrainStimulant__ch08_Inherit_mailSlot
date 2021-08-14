// MailReceiver.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>

#ifdef _UNICODE 
#define tcout    wcout 
#else 
#define tcout    cout 
#endif

#define SLOT_NAME _T("\\\\.\\mailslot\\temp")
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMailSlot;
	DWORD sz;

	hMailSlot = CreateMailslot(SLOT_NAME , 0, MAILSLOT_WAIT_FOREVER, nullptr);


	cout << "==================== Receive ==========" << endl;
	if (hMailSlot == INVALID_HANDLE_VALUE) 
	{
		cout << "Create MailSlot Fila!!" << endl;
		return -1;
	}

	cout << "Mail SLot Print = " << hMailSlot << endl;
	TCHAR msg[52] = { 0, };

	while (1)
	{
		if (!ReadFile(hMailSlot,
			msg,
			sizeof(TCHAR) * 50,
			&sz,
			NULL))
		{
			cout << "Sender Erorr : Read Fail" << endl;
			return -1;
		}
		_putts(msg);

		if (!_tcsncmp(msg, _T("exit"), 4))
		{
			cout << "Receive Exit Bye!!" << endl;
			break;
		}

		msg[sz / sizeof(TCHAR)] = 0; //NULL 문자 삽입
	}
	return 0;
}