// ---------------------------------------------------------------------------
//  ʸ�������󥯥饹
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------


#ifndef __StringVector_h__
#define __StringVector_h__


#define STRVCT_ALLOC_SIZE	32		// �����ĥ���γ������ñ��



// ʸ�������󥯥饹
class CStringVector
{
public:
	CStringVector();		// ���󥹥ȥ饯��
	~CStringVector();		// �ǥ��ȥ饯��

	int     Add(LPCTSTR lpszString);				// ʸ������ɲ�
	LPCTSTR Get(int iIndex);						// ʸ����μ���
	int     GetCount(void)	{ return m_iCount; }	// ���ǿ��μ���

protected:
	LPTSTR* m_pszVector;	// ʸ����ǡ���������
	int     m_iSize;		// ����μºݤΥ�����
	int     m_iCount;		// ���ǿ�
};


#endif	// __StringVector_h__



// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
