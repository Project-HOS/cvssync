// ---------------------------------------------------------------------------
//  タイムスタンプ操作ツール
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------


#ifndef __TimeStamp_h__
#define __TimeStamp_h__


// タイムスタンプ情報格納構造体
struct TTimeStamp
{
	FILETIME ftCreation;		//  生成時刻
	FILETIME ftLastAccess;		//  最終アクセス時刻
	FILETIME ftLastWrite;		//  最終更新時刻
};


BOOL GetFileTimeStamp(LPCTSTR lpszFileName, TTimeStamp* pTimeStamp);					// タイムスタンプ取得
BOOL SetFileTimeStamp(LPCTSTR lpszFileName, const TTimeStamp* pTimeStamp);				// タイムスタンプ設定
int  CompareTimeStamp(const TTimeStamp* pTimeStamp1, const TTimeStamp* pTimeStamp2);	// タイムスタンプ比較


#endif	// __TimeStamp_h__



// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
