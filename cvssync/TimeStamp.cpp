// ---------------------------------------------------------------------------
//  �����ॹ��������ġ���
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------



#include <windows.h>
#include <tchar.h>
#include "TimeStamp.h"



// �ե�����Υ����ॹ����׼���
BOOL GetFileTimeStamp(LPCTSTR lpszFileName, TTimeStamp* pTimeStamp)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;

	// �ե�����θ���
	hFind = FindFirstFile(lpszFileName, &wfd);
	if ( hFind == INVALID_HANDLE_VALUE  )
	{
		return FALSE;
	}

	// �����ॹ����׾���μ���
	pTimeStamp->ftCreation   = wfd.ftCreationTime;
	pTimeStamp->ftLastAccess = wfd.ftLastAccessTime;
	pTimeStamp->ftLastWrite  = wfd.ftLastWriteTime;

	// �ե����븡���ϥ�ɥ륯����
	FindClose(hFind);

	return TRUE;
}


// �ե�����Υ����ॹ���������
BOOL SetFileTimeStamp(LPCTSTR lpszFileName, const TTimeStamp* pTimeStamp)
{
	OSVERSIONINFO oi;
	HANDLE hFile;
	DWORD  dwFlag = 0;
	BOOL   blReturn;

	// OS �ΥС���������
	GetVersionEx(&oi);
	if ( oi.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		dwFlag = FILE_FLAG_BACKUP_SEMANTICS;
	}

	// �ե����륪���ץ�
	hFile = CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, dwFlag, 0);
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}

	// �����ॹ���������
	blReturn = SetFileTime(hFile, &pTimeStamp->ftCreation, &pTimeStamp->ftLastAccess, &pTimeStamp->ftLastWrite);

	// �ե����륯����
	CloseHandle(hFile);

	return blReturn;
}


// �����ॹ����פ����
int CompareTimeStamp(const TTimeStamp* pTimeStamp1, const TTimeStamp* pTimeStamp2)
{
	// ��̥�����
	if ( pTimeStamp1->ftLastWrite.dwHighDateTime > pTimeStamp2->ftLastWrite.dwHighDateTime )
	{
		return 1;
	}
	if ( pTimeStamp1->ftLastWrite.dwHighDateTime < pTimeStamp2->ftLastWrite.dwHighDateTime )
	{
		return -1;
	}

	// ���̥�����
	if ( pTimeStamp1->ftLastWrite.dwLowDateTime > pTimeStamp2->ftLastWrite.dwLowDateTime )
	{
		return 1;
	}
	if ( pTimeStamp1->ftLastWrite.dwLowDateTime < pTimeStamp2->ftLastWrite.dwLowDateTime )
	{
		return -1;
	}

	return 0;	// Ʊ��
}



// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
