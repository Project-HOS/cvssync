// ---------------------------------------------------------------------------
//  連想文字列配列クラス                                                      
//                                                                            
//                                          Copyright (C) 2002 by Project HOS 
// ---------------------------------------------------------------------------



#include <windows.h>
#include <tchar.h>
#include "AssociationVector.h"



// コンストラクタ
CAssociationVector::CAssociationVector()
{
	m_pList = NULL;
}


// デストラクタ
CAssociationVector::~CAssociationVector()
{
	AscVectData* pNext;

	while ( m_pList != NULL )
	{
		pNext = m_pList->pNext;
		delete m_pList->lpszIndex;
		delete m_pList;
		m_pList = pNext;
	}
}


// 文字列の追加
void CAssociationVector::Add(LPCTSTR lpszIndex, LPCTSTR lpszString)
{
	CStringVector* pStrVct;

	pStrVct = Get(lpszIndex);
	if ( pStrVct == NULL )
	{
		AscVectData* pTmp;

		pTmp = new AscVectData;
		pTmp->pNext = m_pList;
		m_pList = pTmp;

		pTmp->lpszIndex = new TCHAR[lstrlen(lpszIndex) + 1];
		lstrcpy(pTmp->lpszIndex, lpszIndex);

		pStrVct = &pTmp->StrVect;
	}

	// 文字列の追加
	pStrVct->Add(lpszString);
}


// 文字列配列の取得
CStringVector* CAssociationVector::Get(LPCTSTR lpszIndex)
{
	AscVectData* pTmp;	

	pTmp = m_pList;
	while ( pTmp != NULL )
	{
		if ( lstrcmp(pTmp->lpszIndex, lpszIndex) == 0 )
		{
			return &pTmp->StrVect;
		}
		pTmp = pTmp->pNext;
	}

	return NULL;
}


// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Project HOS                                         
// ---------------------------------------------------------------------------
