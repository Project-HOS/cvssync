// ---------------------------------------------------------------------------
//  CVS�� ʸ���������Ѵ�(EUC <-> SJIS)�դ��ե����Ʊ���ġ��� Ver 0.02
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
#include "ReadConfig.h"



// ������ؿ�
static void ReadCfgFile(LPCTSTR lpszExeFile);							// �����ɤ߹���
static BOOL FileProc(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag);	// �ե���������롼����
static BOOL File1To2(LPCTSTR lpszFile1, LPCTSTR lpszFile2);				// Tree1(EUC)����Tree2(SJIS)�إ��ԡ�����
static BOOL File2To1(LPCTSTR lpszFile1, LPCTSTR lpszFile2);				// Tree2(SJIS)����Tree1(EUC)�إ��ԡ�����
static BOOL CheckTextFile(LPCTSTR lpszFileName);						// �ƥ����ȥե�����Ƚ��
static BOOL CheckBinaryFile(LPCTSTR lpszFileName);						// �Х��ʥ�ե�����Ƚ��


// �������ѿ�
static CAssociationVector s_avConfig;



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

	// ����ե������ɤ߹���
	ReadCfgFile(argv[0]);

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



// ����ե������ɤ߹���
void ReadCfgFile(LPCTSTR lpszExeFile)
{
	TCHAR szCfgFile[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szName[_MAX_FNAME];

	// �¹ԥե�����γ�ĥ�Ҥ��Ѵ�
	_tsplitpath(lpszExeFile, szDrive, szDir, szName, NULL);
	_tmakepath(szCfgFile, szDrive, szDir, szName, _T(".cfg"));

	// ����ե������ɤ߽Ф�
	ReadConfig(szCfgFile, &s_avConfig);

	// �ǥե���ȥ��ץ���������
	s_avConfig.Add(_T("[NkfOption1]"), _T("-E -s"));	// EUC�����ꤷ��SJIS���Ѵ�
	s_avConfig.Add(_T("[NkfOption1]"), _T("-s -E"));	// SJIS�����ꤷ��EUC���Ѵ�
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
	CStringVector* psv;
	TCHAR szCommand[MAX_PATH * 2];
	TTimeStamp ts;

	psv = s_avConfig.Get(_T("[NkfOption1]"));
	
	if ( CheckTextFile(lpszFile1) )
	{
		// �ƥ����Ȥ�nkf���Ѵ�
		wsprintf(szCommand, _T("nkf32 %s -O \"%s\" \"%s\""), psv->Get(0), lpszFile1, lpszFile2);
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


// File2(SJIS) �� EUC�Ѵ�����File1�˥��ԡ�
BOOL File2To1(LPCTSTR lpszFile1, LPCTSTR lpszFile2)
{
	CStringVector* psv;
	TCHAR szCommand[MAX_PATH * 2];
	TTimeStamp ts;

	psv = s_avConfig.Get(_T("[NkfOption2]"));

	if ( CheckTextFile(lpszFile2) )
	{
		// �ƥ����Ȥ�nkf���Ѵ�
		wsprintf(szCommand, _T("nkf32 %s -O \"%s\" \"%s\""), psv->Get(0), lpszFile2, lpszFile1);
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


// �ƥ����ȥե�����Ƚ��
BOOL CheckTextFile(LPCTSTR lpszFileName)
{
	CStringVector* psv;
	TCHAR szName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	int i;

	// �ѥ�̾��ʬ��
	_tsplitpath(lpszFileName, NULL, NULL, szName, szExt);
	
	// ��ĥ�Ҥʤ��� makefile �ʤ�ƥ����Ȥȡʾ���ˡˤߤʤ�
	if ( lstrcmpi(szName, _T("makefile")) == 0
				&& lstrcmpi(szExt, _T("")) == 0 )
	{
		return TRUE;
	}
	
	// ����ե�����ξ�����ɤ߽Ф�
	psv = s_avConfig.Get(_T("[TextFile]"));

	// �����ĥ�ҤȰ��פ�����ƥ����ȤȤߤʤ�
	for ( i = 0; i < psv->GetCount(); i++ )
	{
		if ( lstrcmpi(szExt, psv->Get(i)) == 0 )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}


// �Х��ʥ�ե�����Ƚ��
BOOL CheckBinaryFile(LPCTSTR lpszFileName)
{
	CStringVector* psv;
	TCHAR szExt[_MAX_EXT];
	int i;
	
	// �ѥ�̾��ʬ��
	_tsplitpath(lpszFileName, NULL, NULL, NULL, szExt);
	
	// ����ե�����ξ�����ɤ߽Ф�
	psv = s_avConfig.Get(_T("[BinaryFile]"));

	// �����ĥ�ҤȰ��פ�����Х��ʥ�Ȥߤʤ�
	for ( i = 0; i < psv->GetCount(); i++ )
	{
		if ( lstrcmpi(szExt, psv->Get(i)) == 0 )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}



// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
