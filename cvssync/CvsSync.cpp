// ---------------------------------------------------------------------------
//  CVS用 文字コード変換(EUC <-> SJIS)付きフォルダ同期ツール
//    超いいかげんバージョン
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



// ローカル関数
BOOL FileProc(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag);		// ファイル処理ルーチン
BOOL File1To2(LPCTSTR lpszFile1, LPCTSTR lpszFile2);				// Tree1(EUC)からTree2(SJIS)へコピー処理
BOOL File2To1(LPCTSTR lpszFile1, LPCTSTR lpszFile2);				// Tree2(SJIS)からTree1(EUC)へコピー処理
BOOL CheckTextFile(LPCTSTR lpszFileName);							// テキストファイル判定
BOOL CheckBinaryFile(LPCTSTR lpszFileName);							// バイナリファイル判定



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
	TCHAR szCommand[MAX_PATH * 2];
	TTimeStamp ts;

	if ( CheckTextFile(lpszFile1) )
	{
		// テキストはnkfで変換
		wsprintf(szCommand, "nkf32 -s -O \"%s\" \"%s\"", lpszFile1, lpszFile2);
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


// File2(SJIS) を SJIS変換してFile1にコピー
BOOL File2To1(LPCTSTR lpszFile1, LPCTSTR lpszFile2)
{
	TCHAR szCommand[MAX_PATH * 2];
	TTimeStamp ts;

	if ( CheckTextFile(lpszFile2) )
	{
		// テキストはnkfで変換
		wsprintf(szCommand, "nkf32 -S -e -d -O \"%s\" \"%s\"", lpszFile2, lpszFile1);
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


// テキストファイルの拡張子（手抜きの決め打ち）
LPCTSTR lpszTextExt[] =
{
	_T(".c"),
	_T(".cpp"),
	_T(".h"),
	_T(".s"),
	_T(".asm"),
	_T(".src"),
	_T(".inc"),
	_T(".cfg"),
	_T(".x"),
	_T(".sub"),
	_T(".txt"),
	_T(".mak"),
	_T(".html"),
	_T(".htm"),
	_T(".cgi"),
	_T(".pl"),
	_T(".java"),
	NULL
};

// バイナリファイルの拡張子（超手抜きパート２）
LPCTSTR lpszBinExt[] =
{
	_T(".dsw"),
	_T(".dsp"),
	_T(".doc"),
	_T(".xls"),
	_T(".ppt"),
	_T(".jpg"),
	_T(".gif"),
	_T(".png"),
	_T(".pdf"),
	NULL
};

// ここで思いつかない拡張子は多分コピーしたくない一時ファイルに違いない
// というゴーマンな作りとなっております m(_ _)m
// というか、拡張子ぐらい設定ファイル読めるようにしろってね (^^;;



// テキストファイル判定
BOOL CheckTextFile(LPCTSTR lpszFileName)
{
	TCHAR szName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	int i;
	
	_tsplitpath(lpszFileName, NULL, NULL, szName, szExt);
	
	// 拡張子なしの makefile ならテキストと（勝手に）みなす
	if ( lstrcmpi(szName, _T("makefile")) == 0
				&& lstrcmpi(szExt, _T("")) == 0 )
	{
		return TRUE;
	}

	// 登録拡張子と同じならそれもみんなテキストじゃー！
	for ( i = 0; lpszTextExt[i] != NULL; i++ )
	{
		if ( lstrcmpi(szExt, lpszTextExt[i]) == 0 )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}


// バイナリファイル判定
BOOL CheckBinaryFile(LPCTSTR lpszFileName)
{
	TCHAR szExt[_MAX_EXT];
	int i;
	
	_tsplitpath(lpszFileName, NULL, NULL, NULL, szExt);
	
	// 登録拡張子と同じならバイナリ
	for ( i = 0; lpszBinExt[i] != NULL; i++ )
	{
		if ( lstrcmpi(szExt, lpszBinExt[i]) == 0 )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}


// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
