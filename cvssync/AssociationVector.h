// ---------------------------------------------------------------------------
//  Ϣ��ʸ�������󥯥饹                                                      
//                                                                            
//                                          Copyright (C) 2002 by Project HOS 
// ---------------------------------------------------------------------------


#ifndef __AssociationVector_h__
#define __AssociationVector_h__


#include "StringVector.h"


// ʸ��������ǡ���
struct AscVectData
{
	LPTSTR        lpszIndex;		// ����ǥå���ʸ����
	CStringVector StrVect;			// ʸ��������
	AscVectData*  pNext;			// ��������
};


// ʸ�������󥯥饹
class CAssociationVector
{
public:
	CAssociationVector();		// ���󥹥ȥ饯��
	~CAssociationVector();		// �ǥ��ȥ饯��

	void           Add(LPCTSTR lpszIndex, LPCTSTR lpszString);		// ʸ������ɲ�
	CStringVector* Get(LPCTSTR lpszIndex);							// ʸ��������μ���

protected:
	AscVectData* m_pList;		// ʸ��������ǡ����Υꥹ��
};


#endif	// __AssociationVector_h__


// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Project HOS                                         
// ---------------------------------------------------------------------------
