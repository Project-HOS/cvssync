// ---------------------------------------------------------------------------
//  CVS�� ʸ���������Ѵ�(EUC <-> SJIS)�դ��ե����Ʊ���ġ���
//    Ķ����������С������
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include "TreeSearch.h"
#include "TimeStamp.h"



// ������ؿ�
BOOL FileProc(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag);		// �ե���������롼����
BOOL File1To2(LPCTSTR lpszFile1, LPCTSTR lpszFile2);				// Tree1(EUC)����Tree2(SJIS)�إ��ԡ�����
BOOL File2To1(LPCTSTR lpszFile1, LPCTSTR lpszFile2);				// Tree2(SJIS)����Tree1(EUC)�إ��ԡ�����
BOOL CheckTextFile(LPCTSTR lpszFileName);							// �ƥ����ȥե�����Ƚ��
BOOL CheckBinaryFile(LPCTSTR lpszFileName);							// �Х��ʥ�ե�����Ƚ��



// �ᥤ��ؿ�
int main(int argc, char *argv[])
{
	CTreeSearch tc(FileProc);
	TCHAR szPath1[MAX_PATH];
	TCHAR szPath2[MAX_PATH];

	// ���������å�
	if ( argc != 3 )
	{
		printf("usage: CvsSync.exe euc-path sjis-path\n");
		return 1;
	}

	// �ѥ��θ��� '\' ��Ĥ���
	lstrcpy(szPath1, argv[1]);
	lstrcpy(szPath2, argv[2]);
	if ( szPath1[lstrlen(szPath1) - 1] != _T('\\') )
	{
		lstrcat(szPath1, _T("\\"));
	}
	if ( szPath2[lstrlen(szPath2) - 1] != _T('\\') )
	{
		lstrcat(szPath2, _T("\\"));
	}
	
	// �ĥ꡼�����¹�
	tc.Search(szPath1, szPath2);

	return 0;
}


// �ե���������롼����
BOOL FileProc(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag)
{
	TTimeStamp ts1, ts2;
	int iComp;

	switch ( iFlag )
	{
	case FILEPROC_FILE1:	// �ե����룱(EUC)�Τߤ�¸��
		File1To2(lpszFile1, lpszFile2);
		break;

	case FILEPROC_FILE2:	// �ե����룲(SJIS)�Τߤ�¸��
		File2To1(lpszFile1, lpszFile2);
		break;

	case FILEPROC_BOTH:		// ξ���Υե����뤬¸��
		GetFileTimeStamp(lpszFile1, &ts1);
		GetFileTimeStamp(lpszFile2, &ts2);
		iComp = CompareTimeStamp(&ts1, &ts2);
		if ( iComp > 0 )
		{
			File1To2(lpszFile1, lpszFile2);		// File1����������� File1 -> File2
		}
		else if ( iComp < 0 )
		{
			File2To1(lpszFile1, lpszFile2);		// File2����������� File2 -> File1
		}
		break;
	}

	return TRUE;
}


// File1(EUC) �� SJIS�Ѵ�����File2�˥��ԡ�
BOOL File1To2(LPCTSTR lpszFile1, LPCTSTR lpszFile2)
{
	TCHAR szCommand[MAX_PATH * 2];
	TTimeStamp ts;

	if ( CheckTextFile(lpszFile1) )
	{
		// �ƥ����Ȥ�nkf���Ѵ�
		wsprintf(szCommand, "nkf32 -s -O \"%s\" \"%s\"", lpszFile1, lpszFile2);
		if ( _tsystem(szCommand) != 0 )
		{
			return FALSE;	// ���顼ȯ��
		}
	}
	else if ( CheckBinaryFile(lpszFile1) )
	{
		// �Х��ʥ�ե�����Ϥ��Τޤޥ��ԡ�
		if ( !::CopyFile(lpszFile1, lpszFile2, FALSE) )
		{
			return FALSE;	// ���顼ȯ��
		}
	}
	else
	{
		// ̤��Ͽ�ե������̵��
		return TRUE;
	}

	printf("\"%s\" => \"%s\"\n", lpszFile1, lpszFile2);

	// �����ॹ����פΥ��ԡ�
	GetFileTimeStamp(lpszFile1, &ts);
	SetFileTimeStamp(lpszFile2, &ts);

	// �ե�����°���Υ��ԡ�
	::SetFileAttributes(lpszFile2, GetFileAttributes(lpszFile1));

	return TRUE;
}


// File2(SJIS) �� SJIS�Ѵ�����File1�˥��ԡ�
BOOL File2To1(LPCTSTR lpszFile1, LPCTSTR lpszFile2)
{
	TCHAR szCommand[MAX_PATH * 2];
	TTimeStamp ts;

	if ( CheckTextFile(lpszFile2) )
	{
		// �ƥ����Ȥ�nkf���Ѵ�
		wsprintf(szCommand, "nkf32 -S -e -d -O \"%s\" \"%s\"", lpszFile2, lpszFile1);
		if ( _tsystem(szCommand) != 0 )
		{
			return FALSE;	// ���顼ȯ��
		}
	}
	else if ( CheckBinaryFile(lpszFile2) )
	{
		// �Х��ʥ�ե�����Ϥ��Τޤޥ��ԡ�
		if ( !::CopyFile(lpszFile2, lpszFile1, FALSE) )
		{
			return FALSE;	// ���顼ȯ��
		}
	}
	else
	{
		// ̤��Ͽ�ե������̵��
		return TRUE;
	}

	printf("\"%s\" <= \"%s\"\n", lpszFile1, lpszFile2);

	// �����ॹ����פΥ��ԡ�
	::GetFileTimeStamp(lpszFile2, &ts);
	::SetFileTimeStamp(lpszFile1, &ts);

	// �ե�����°���Υ��ԡ�
	::SetFileAttributes(lpszFile1, GetFileAttributes(lpszFile2));

	return TRUE;
}


// �ƥ����ȥե�����γ�ĥ�ҡʼ�ȴ���η���Ǥ���
LPCTSTR lpszTextExt[] =
{
	_T(".c"),
	_T(".cpp"),
	_T(".h"),
	_T(".s"),
	_T(".asm"),
	_T(".src"),
	_T(".inc"),
	_T(".cfg"),
	_T(".x"),
	_T(".sub"),
	_T(".txt"),
	_T(".mak"),
	_T(".html"),
	_T(".htm"),
	_T(".cgi"),
	_T(".pl"),
	_T(".java"),
	NULL
};

// �Х��ʥ�ե�����γ�ĥ�ҡ�Ķ��ȴ���ѡ��ȣ���
LPCTSTR lpszBinExt[] =
{
	_T(".dsw"),
	_T(".dsp"),
	_T(".doc"),
	_T(".xls"),
	_T(".ppt"),
	_T(".jpg"),
	_T(".gif"),
	_T(".png"),
	_T(".pdf"),
	NULL
};

// �����ǻפ��Ĥ��ʤ���ĥ�Ҥ�¿ʬ���ԡ��������ʤ�����ե�����˰㤤�ʤ�
// �Ȥ��������ޥ�ʺ��ȤʤäƤ���ޤ� m(_ _)m
// �Ȥ���������ĥ�Ҥ��餤����ե������ɤ��褦�ˤ���äƤ� (^^;;



// �ƥ����ȥե�����Ƚ��
BOOL CheckTextFile(LPCTSTR lpszFileName)
{
	TCHAR szName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	int i;
	
	_tsplitpath(lpszFileName, NULL, NULL, szName, szExt);
	
	// ��ĥ�Ҥʤ��� makefile �ʤ�ƥ����Ȥȡʾ���ˡˤߤʤ�
	if ( lstrcmpi(szName, _T("makefile")) == 0
				&& lstrcmpi(szExt, _T("")) == 0 )
	{
		return TRUE;
	}

	// ��Ͽ��ĥ�Ҥ�Ʊ���ʤ餽���ߤ�ʥƥ����Ȥ��㡼��
	for ( i = 0; lpszTextExt[i] != NULL; i++ )
	{
		if ( lstrcmpi(szExt, lpszTextExt[i]) == 0 )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}


// �Х��ʥ�ե�����Ƚ��
BOOL CheckBinaryFile(LPCTSTR lpszFileName)
{
	TCHAR szExt[_MAX_EXT];
	int i;
	
	_tsplitpath(lpszFileName, NULL, NULL, NULL, szExt);
	
	// ��Ͽ��ĥ�Ҥ�Ʊ���ʤ�Х��ʥ�
	for ( i = 0; lpszBinExt[i] != NULL; i++ )
	{
		if ( lstrcmpi(szExt, lpszBinExt[i]) == 0 )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}


// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
