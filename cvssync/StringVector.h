// ---------------------------------------------------------------------------
//  文字列配列クラス
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------


#ifndef __StringVector_h__
#define __StringVector_h__


#define STRVCT_ALLOC_SIZE	32		// メモリ拡張時の割り当て単位



// 文字列配列クラス
class CStringVector
{
public:
	CStringVector();		// コンストラクタ
	~CStringVector();		// デストラクタ

	int     Add(LPCTSTR lpszString);				// 文字列の追加
	LPCTSTR Get(int iIndex);						// 文字列の取得
	int     GetCount(void)	{ return m_iCount; }	// 要素数の取得

protected:
	LPTSTR* m_pszVector;	// 文字列データの配列
	int     m_iSize;		// 配列の実際のサイズ
	int     m_iCount;		// 要素数
};


#endif	// __StringVector_h__



// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
