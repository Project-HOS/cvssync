// ---------------------------------------------------------------------------
//  CVS用 文字コード変換(EUC <-> SJIS)付きフォルダ同期ツール Ver 0.02
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include "TreeSearch.h"
#include "TimeStamp.h"
#include "ReadConfig.h"



// ローカル関数
static void ReadCfgFile(LPCTSTR lpszExeFile);							// 設定読み込み
static BOOL FileProc(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag);	// ファイル処理ルーチン
static BOOL File1To2(LPCTSTR lpszFile1, LPCTSTR lpszFile2);				// Tree1(EUC)からTree2(SJIS)へコピー処理
static BOOL File2To1(LPCTSTR lpszFile1, LPCTSTR lpszFile2);				// Tree2(SJIS)からTree1(EUC)へコピー処理
static BOOL CheckTextFile(LPCTSTR lpszFileName);						// テキストファイル判定
static BOOL CheckBinaryFile(LPCTSTR lpszFileName);						// バイナリファイル判定


// ローカル変数
static CAssociationVector s_avConfig;



// メイン関数
int main(int argc, char *argv[])
{
	CTreeSearch tc(FileProc);
	TCHAR szPath1[MAX_PATH];
	TCHAR szPath2[MAX_PATH];

	// 引数チェック
	if ( argc != 3 )
	{
		printf("usage: CvsSync.exe euc-path sjis-path\n");
		return 1;
	}

	// 設定ファイル読み込み
	ReadCfgFile(argv[0]);

	// パスの後ろに '\' をつける
	lstrcpy(szPath1, argv[1]);
	lstrcpy(szPath2, argv[2]);
	if ( szPath1[lstrlen(szPath1) - 1] != _T('\\') )
	{
		lstrcat(szPath1, _T("\\"));
	}
	if ( szPath2[lstrlen(szPath2) - 1] != _T('\\') )
	{
		lstrcat(szPath2, _T("\\"));
	}
	
	// ツリー検索実行
	tc.Search(szPath1, szPath2);

	return 0;
}



// 設定ファイル読み込み
void ReadCfgFile(LPCTSTR lpszExeFile)
{
	TCHAR szCfgFile[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szName[_MAX_FNAME];

	// 実行ファイルの拡張子を変換
	_tsplitpath(lpszExeFile, szDrive, szDir, szName, NULL);
	_tmakepath(szCfgFile, szDrive, szDir, szName, _T(".cfg"));

	// 設定ファイル読み出し
	ReadConfig(szCfgFile, &s_avConfig);

	// デフォルトオプションを設定
	s_avConfig.Add(_T("[NkfOption1]"), _T("-E -s"));	// EUCを想定してSJISに変換
	s_avConfig.Add(_T("[NkfOption1]"), _T("-s -E"));	// SJISを想定してEUCに変換
}



// ファイル処理ルーチン
BOOL FileProc(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag)
{
	TTimeStamp ts1, ts2;
	int iComp;

	switch ( iFlag )
	{
	case FILEPROC_FILE1:	// ファイル１(EUC)のみが存在
		File1To2(lpszFile1, lpszFile2);
		break;

	case FILEPROC_FILE2:	// ファイル２(SJIS)のみが存在
		File2To1(lpszFile1, lpszFile2);
		break;

	case FILEPROC_BOTH:		// 両方のファイルが存在
		GetFileTimeStamp(lpszFile1, &ts1);
		GetFileTimeStamp(lpszFile2, &ts2);
		iComp = CompareTimeStamp(&ts1, &ts2);
		if ( iComp > 0 )
		{
			File1To2(lpszFile1, lpszFile2);		// File1が新しければ File1 -> File2
		}
		else if ( iComp < 0 )
		{
			File2To1(lpszFile1, lpszFile2);		// File2が新しければ File2 -> File1
		}
		break;
	}

	return TRUE;
}


// File1(EUC) を SJIS変換してFile2にコピー
BOOL File1To2(LPCTSTR lpszFile1, LPCTSTR lpszFile2)
{
	CStringVector* psv;
	TCHAR szCommand[MAX_PATH * 2];
	TTimeStamp ts;

	psv = s_avConfig.Get(_T("[NkfOption1]"));
	
	if ( CheckTextFile(lpszFile1) )
	{
		// テキストはnkfで変換
		wsprintf(szCommand, _T("nkf32 %s -O \"%s\" \"%s\""), psv->Get(0), lpszFile1, lpszFile2);
		if ( _tsystem(szCommand) != 0 )
		{
			return FALSE;	// エラー発生
		}
	}
	else if ( CheckBinaryFile(lpszFile1) )
	{
		// バイナリファイルはそのままコピー
		if ( !::CopyFile(lpszFile1, lpszFile2, FALSE) )
		{
			return FALSE;	// エラー発生
		}
	}
	else
	{
		// 未登録ファイルは無視
		return TRUE;
	}

	printf("\"%s\" => \"%s\"\n", lpszFile1, lpszFile2);

	// タイムスタンプのコピー
	GetFileTimeStamp(lpszFile1, &ts);
	SetFileTimeStamp(lpszFile2, &ts);

	// ファイル属性のコピー
	::SetFileAttributes(lpszFile2, GetFileAttributes(lpszFile1));

	return TRUE;
}


// File2(SJIS) を EUC変換してFile1にコピー
BOOL File2To1(LPCTSTR lpszFile1, LPCTSTR lpszFile2)
{
	CStringVector* psv;
	TCHAR szCommand[MAX_PATH * 2];
	TTimeStamp ts;

	psv = s_avConfig.Get(_T("[NkfOption2]"));

	if ( CheckTextFile(lpszFile2) )
	{
		// テキストはnkfで変換
		wsprintf(szCommand, _T("nkf32 %s -O \"%s\" \"%s\""), psv->Get(0), lpszFile2, lpszFile1);
		if ( _tsystem(szCommand) != 0 )
		{
			return FALSE;	// エラー発生
		}
	}
	else if ( CheckBinaryFile(lpszFile2) )
	{
		// バイナリファイルはそのままコピー
		if ( !::CopyFile(lpszFile2, lpszFile1, FALSE) )
		{
			return FALSE;	// エラー発生
		}
	}
	else
	{
		// 未登録ファイルは無視
		return TRUE;
	}

	printf("\"%s\" <= \"%s\"\n", lpszFile1, lpszFile2);

	// タイムスタンプのコピー
	::GetFileTimeStamp(lpszFile2, &ts);
	::SetFileTimeStamp(lpszFile1, &ts);

	// ファイル属性のコピー
	::SetFileAttributes(lpszFile1, GetFileAttributes(lpszFile2));

	return TRUE;
}


// テキストファイル判定
BOOL CheckTextFile(LPCTSTR lpszFileName)
{
	CStringVector* psv;
	TCHAR szName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	int i;

	// パス名を分解
	_tsplitpath(lpszFileName, NULL, NULL, szName, szExt);
	
	// 拡張子なしの makefile ならテキストと（勝手に）みなす
	if ( lstrcmpi(szName, _T("makefile")) == 0
				&& lstrcmpi(szExt, _T("")) == 0 )
	{
		return TRUE;
	}
	
	// 設定ファイルの情報を読み出し
	psv = s_avConfig.Get(_T("[TextFile]"));

	// 指定拡張子と一致したらテキストとみなす
	for ( i = 0; i < psv->GetCount(); i++ )
	{
		if ( lstrcmpi(szExt, psv->Get(i)) == 0 )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}


// バイナリファイル判定
BOOL CheckBinaryFile(LPCTSTR lpszFileName)
{
	CStringVector* psv;
	TCHAR szExt[_MAX_EXT];
	int i;
	
	// パス名を分解
	_tsplitpath(lpszFileName, NULL, NULL, NULL, szExt);
	
	// 設定ファイルの情報を読み出し
	psv = s_avConfig.Get(_T("[BinaryFile]"));

	// 指定拡張子と一致したらバイナリとみなす
	for ( i = 0; i < psv->GetCount(); i++ )
	{
		if ( lstrcmpi(szExt, psv->Get(i)) == 0 )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}



// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
