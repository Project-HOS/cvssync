// ---------------------------------------------------------------------------
//  設定ファイル読み込み
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "ReadConfig.h"


#define READCFG_MAX_LINE	256



//  設定ファイル読み込み
void ReadConfig(LPCTSTR szFileName, CAssociationVector* pAscVect)
{
	TCHAR szIndex[READCFG_MAX_LINE] = _T("");
	TCHAR szBuf[READCFG_MAX_LINE];
	FILE* fp;

	// ファイルオープン
	if ( (fp = _tfopen(szFileName, "r")) == NULL )
	{
		return;
	}

	// 読み込み
	while ( _fgetts(szBuf, READCFG_MAX_LINE, fp) != NULL )
	{
		// １行が長すぎたら終わる
		int iLen = lstrlen(szBuf);
		if ( iLen >= 0 && szBuf[iLen - 1] != _T('\n') )
		{
			break;	
		}
		
		// 後続の空白文字削除
		while ( iLen > 0 && _istspace(szBuf[iLen - 1]) )
		{
			iLen--;
		}
		szBuf[iLen] = _T('\0');

		if ( iLen > 3 && szBuf[0] == _T('[') && szBuf[iLen - 1] == _T(']') )
		{
			// インデックス設定
			lstrcpy(szIndex, szBuf);
		}
		else if ( szIndex[0] != _T('\0') )
		{
			// 空行でなければ連想配列に追加
			if ( szBuf[0] != _T('\0') )
			{
				pAscVect->Add(szIndex, szBuf);
			}
		}
	}

	// ファイルクローズ
	fclose(fp);
}


// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
