// ---------------------------------------------------------------------------
//  ʸ�������󥯥饹
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------


#include <windows.h>
#include <tchar.h>
#include "StringVector.h"



// ���󥹥ȥ饯��
CStringVector::CStringVector()
{
	m_pszVector = new LPTSTR[STRVCT_ALLOC_SIZE];
	m_iSize     = STRVCT_ALLOC_SIZE;
	m_iCount    = 0;
}

// �ǥ��ȥ饯��
CStringVector::~CStringVector()
{
	int i;

	for ( i = 0; i < m_iCount; i++ )
	{
		delete[] m_pszVector[i];
	}

	delete[] m_pszVector;
}

// ʸ������ɲ�
int CStringVector::Add(LPCTSTR lpszString)
{
	// ���꤬��­�ʤ��ĥ
	if ( m_iCount + 1 > m_iSize )
	{
		LPTSTR* m_pszTmp;
		m_pszTmp = new LPTSTR[m_iSize + STRVCT_ALLOC_SIZE];
		::CopyMemory(m_pszTmp, m_pszVector, sizeof(LPTSTR) * m_iCount);
		delete[] m_pszVector;
		m_pszVector = m_pszTmp;
		m_iSize += STRVCT_ALLOC_SIZE;
	}

	// ʸ�����Ǽ�ѥ������
	m_pszVector[m_iCount] = new TCHAR[lstrlen(lpszString) + 1];

	// ʸ�����Ǽ
	lstrcpy(m_pszVector[m_iCount], lpszString);

	return m_iCount++;
}


// ʸ����μ���
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
