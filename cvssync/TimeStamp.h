// ---------------------------------------------------------------------------
//  �����ॹ��������ġ���
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------


#ifndef __TimeStamp_h__
#define __TimeStamp_h__


// �����ॹ����׾����Ǽ��¤��
struct TTimeStamp
{
	FILETIME ftCreation;		//  ��������
	FILETIME ftLastAccess;		//  �ǽ�������������
	FILETIME ftLastWrite;		//  �ǽ���������
};


BOOL GetFileTimeStamp(LPCTSTR lpszFileName, TTimeStamp* pTimeStamp);					// �����ॹ����׼���
BOOL SetFileTimeStamp(LPCTSTR lpszFileName, const TTimeStamp* pTimeStamp);				// �����ॹ���������
int  CompareTimeStamp(const TTimeStamp* pTimeStamp1, const TTimeStamp* pTimeStamp2);	// �����ॹ��������


#endif	// __TimeStamp_h__



// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
