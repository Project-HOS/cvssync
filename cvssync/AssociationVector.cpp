// ---------------------------------------------------------------------------
//  Ϣ��ʸ�������󥯥饹                                                      
//                                                                            
//                                          Copyright (C) 2002 by Project HOS 
// ---------------------------------------------------------------------------



#include <windows.h>
#include <tchar.h>
#include "AssociationVector.h"



// ���󥹥ȥ饯��
CAssociationVector::CAssociationVector()
{
	m_pList = NULL;
}


// �ǥ��ȥ饯��
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


// ʸ������ɲ�
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

	// ʸ������ɲ�
	pStrVct->Add(lpszString);
}


// ʸ��������μ���
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
