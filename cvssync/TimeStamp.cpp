// ---------------------------------------------------------------------------
//  タイムスタンプ操作ツール
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------



#include <windows.h>
#include <tchar.h>
#include "TimeStamp.h"



// ファイルのタイムスタンプ取得
BOOL GetFileTimeStamp(LPCTSTR lpszFileName, TTimeStamp* pTimeStamp)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;

	// ファイルの検索
	hFind = FindFirstFile(lpszFileName, &wfd);
	if ( hFind == INVALID_HANDLE_VALUE  )
	{
		return FALSE;
	}

	// タイムスタンプ情報の取得
	pTimeStamp->ftCreation   = wfd.ftCreationTime;
	pTimeStamp->ftLastAccess = wfd.ftLastAccessTime;
	pTimeStamp->ftLastWrite  = wfd.ftLastWriteTime;

	// ファイル検索ハンドルクローズ
	FindClose(hFind);

	return TRUE;
}


// ファイルのタイムスタンプ設定
BOOL SetFileTimeStamp(LPCTSTR lpszFileName, const TTimeStamp* pTimeStamp)
{
	OSVERSIONINFO oi;
	HANDLE hFile;
	DWORD  dwFlag = 0;
	BOOL   blReturn;

	// OS のバージョン取得
	GetVersionEx(&oi);
	if ( oi.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		dwFlag = FILE_FLAG_BACKUP_SEMANTICS;
	}

	// ファイルオープン
	hFile = CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, dwFlag, 0);
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}

	// タイムスタンプ設定
	blReturn = SetFileTime(hFile, &pTimeStamp->ftCreation, &pTimeStamp->ftLastAccess, &pTimeStamp->ftLastWrite);

	// ファイルクローズ
	CloseHandle(hFile);

	return blReturn;
}


// タイムスタンプの比較
int CompareTimeStamp(const TTimeStamp* pTimeStamp1, const TTimeStamp* pTimeStamp2)
{
	// 上位ワード比較
	if ( pTimeStamp1->ftLastWrite.dwHighDateTime > pTimeStamp2->ftLastWrite.dwHighDateTime )
	{
		return 1;
	}
	if ( pTimeStamp1->ftLastWrite.dwHighDateTime < pTimeStamp2->ftLastWrite.dwHighDateTime )
	{
		return -1;
	}

	// 下位ワード比較
	if ( pTimeStamp1->ftLastWrite.dwLowDateTime > pTimeStamp2->ftLastWrite.dwLowDateTime )
	{
		return 1;
	}
	if ( pTimeStamp1->ftLastWrite.dwLowDateTime < pTimeStamp2->ftLastWrite.dwLowDateTime )
	{
		return -1;
	}

	return 0;	// 同じ
}



// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
