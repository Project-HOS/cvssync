// ---------------------------------------------------------------------------
//  文字列配列クラス
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------


#include <windows.h>
#include <tchar.h>
#include "StringVector.h"



// コンストラクタ
CStringVector::CStringVector()
{
	m_pszVector = new LPTSTR[STRVCT_ALLOC_SIZE];
	m_iSize     = STRVCT_ALLOC_SIZE;
	m_iCount    = 0;
}

// デストラクタ
CStringVector::~CStringVector()
{
	int i;

	for ( i = 0; i < m_iCount; i++ )
	{
		delete[] m_pszVector[i];
	}

	delete[] m_pszVector;
}

// 文字列の追加
int CStringVector::Add(LPCTSTR lpszString)
{
	// メモリが不足なら拡張
	if ( m_iCount + 1 > m_iSize )
	{
		LPTSTR* m_pszTmp;
		m_pszTmp = new LPTSTR[m_iSize + STRVCT_ALLOC_SIZE];
		::CopyMemory(m_pszTmp, m_pszVector, sizeof(LPTSTR) * m_iCount);
		delete[] m_pszVector;
		m_pszVector = m_pszTmp;
		m_iSize += STRVCT_ALLOC_SIZE;
	}

	// 文字列格納用メモリ確保
	m_pszVector[m_iCount] = new TCHAR[lstrlen(lpszString) + 1];

	// 文字列格納
	lstrcpy(m_pszVector[m_iCount], lpszString);

	return m_iCount++;
}


// 文字列の取得
LPCTSTR CStringVector::Get(int iIndex)
{
	if ( iIndex >= m_iCount )
	{
		return NULL;
	}

	return m_pszVector[iIndex];
}


// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
