// ---------------------------------------------------------------------------
//  �ĥ꡼�������饹                                                          
//                                                                            
//                                          Copyright (C) 2002 by Project HOS 
// ---------------------------------------------------------------------------


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "TreeSearch.h"



// ���󥹥ȥ饯��
CTreeSearch::CTreeSearch(BOOL (*pfncProc)(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag))
{
	// �ե���������롼�������Ͽ
	m_pfncProc = pfncProc;
}


// �ĥ꡼������
BOOL CTreeSearch::Search(LPCTSTR lpszPath1, LPCTSTR lpszPath2)
{
	TCHAR  szSearchPath1[MAX_PATH];
	TCHAR  szSearchPath2[MAX_PATH];
	TFileData* pfdFileList1;
	TFileData* pfdFileList2;
	TFileData* pFileData;
	TFileData* pfdTmp;

	// �������ѥ�����
	wsprintf(szSearchPath1, _T("%s*.*"), lpszPath1);
	wsprintf(szSearchPath2, _T("%s*.*"), lpszPath2);

	// �ե�����ꥹ�Ȥ��ɤ߹���
	pfdFileList1 = CreateFileList(szSearchPath1);
	pfdFileList2 = CreateFileList(szSearchPath2);

	// �ꥹ�ȣ����饵����
	pFileData = pfdFileList1;
	while ( pFileData != NULL )
	{
		if ( pFileData->wfdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			// �ǥ��쥯�ȥ�ʤ�
			if ( lstrcmpi(pFileData->wfdFile.cFileName, _T(".")) != 0
					&& lstrcmpi(pFileData->wfdFile.cFileName, _T("..")) != 0
					&& lstrcmpi(pFileData->wfdFile.cFileName, _T("CVS")) != 0 )		// CVS�ե������̵��
			{
				TCHAR szSubDirectry1[MAX_PATH];
				TCHAR szSubDirectry2[MAX_PATH];
				
				// �ǥ��쥯�ȥ�Υե�ѥ������
				wsprintf(szSubDirectry1, "%s%s\\", lpszPath1, pFileData->wfdFile.cFileName);
				wsprintf(szSubDirectry2, "%s%s\\", lpszPath2, pFileData->wfdFile.cFileName);
	
				// �����б�����ե����뤬���뤫����
				pfdTmp = SearchFile(pfdFileList2, pFileData->wfdFile.cFileName);
				if ( pfdTmp == NULL )
				{
					// ̵����к���
					if ( !CreateDirectory(szSubDirectry2, NULL) )
					{
						// ��������
						printf("fale : create directry \"%s\"\n", szSubDirectry2);
					}
					else
					{
						// �Ƶ�Ū�˸���
						printf("create directry \"%s\"\n", szSubDirectry2);						
						Search(szSubDirectry1, szSubDirectry2);
					}
				}
				else
				{
					// ����г�ǧ
					if ( !(pfdTmp->wfdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
					{
						// Ʊ̾�Υե����뤬����
						printf("fale : create directry \"%s\"\n", szSubDirectry2);
					}
					else
					{
						// �Ƶ�Ū�˸���
						Search(szSubDirectry1, szSubDirectry2);
					}
					pfdTmp->blEnd = TRUE;	// �����ѥե饰���å�
				}
			}
		}
		else
		{
			// �ե�����ʤ�
			TCHAR szFileName1[MAX_PATH];
			TCHAR szFileName2[MAX_PATH];

			// �ե�����Υե�ѥ������
			wsprintf(szFileName1, "%s%s", lpszPath1, pFileData->wfdFile.cFileName);
			wsprintf(szFileName2, "%s%s", lpszPath2, pFileData->wfdFile.cFileName);
			
			// ���¦��Ʊ̾�ե����뤬���뤫�ɤ�������
			pfdTmp = SearchFile(pfdFileList2, pFileData->wfdFile.cFileName);
			if ( pfdTmp == NULL )
			{
				// ̵�����File1�Τߤǥե���������롼����ƤӽФ�
				m_pfncProc(szFileName1, szFileName2, FILEPROC_FILE1);
			}
			else
			{
				// ����������ե����뤢��ǥե���������롼����ƤӽФ�
				m_pfncProc(szFileName1, szFileName2, FILEPROC_BOTH);
				pfdTmp->blEnd = TRUE;	// �����ѥե饰���å�
			}
		}
		pFileData->blEnd = TRUE;		// �����ѥե饰���å�
		pFileData = pFileData->pNext;	// ���Υե�����򸡺�
	}

	// �ꥹ�ȣ��λĤ�򥵡���
	pFileData = pfdFileList2;
	while ( pFileData != NULL )
	{
		if ( !pFileData->blEnd )	// ̤�����Τ�Τ��������
		{
			if ( pFileData->wfdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// �ǥ��쥯�ȥ�ʤ�
				if ( lstrcmpi(pFileData->wfdFile.cFileName, _T(".")) != 0
					&& lstrcmpi(pFileData->wfdFile.cFileName, _T("..")) != 0
					&& lstrcmpi(pFileData->wfdFile.cFileName, _T("CVS")) != 0 )		// CVS�ǥ��쥯�ȥ��̵��
				{
					TCHAR szSubDirectry1[MAX_PATH];
					TCHAR szSubDirectry2[MAX_PATH];
					
					// �ǥ��쥯�ȥ�Υե�ѥ������
					wsprintf(szSubDirectry1, "%s%s\\", lpszPath1, pFileData->wfdFile.cFileName);
					wsprintf(szSubDirectry2, "%s%s\\", lpszPath2, pFileData->wfdFile.cFileName);

					// �ǥ��쥯�ȥ�����
					if ( !CreateDirectory(szSubDirectry1, NULL) )
					{
						// ��������
						printf("fale : create directry \"%s\"\n", szSubDirectry1);
					}
					else
					{
						// �Ƶ�Ū�˸���
						printf("create directry \"%s\"\n", szSubDirectry1);						
						Search(szSubDirectry1, szSubDirectry2);
					}
				}
			}
			else
			{
				// �ե�����ʤ�
				TCHAR szFileName1[MAX_PATH];
				TCHAR szFileName2[MAX_PATH];

				// �ե�����Υե�ѥ�̾�����
				wsprintf(szFileName1, "%s%s", lpszPath1, pFileData->wfdFile.cFileName);
				wsprintf(szFileName2, "%s%s", lpszPath2, pFileData->wfdFile.cFileName);

				// File2�Τߤǥե���������롼�����ƤӽФ�
				m_pfncProc(szFileName1, szFileName2, FILEPROC_FILE2);
			}
		}

		pFileData->blEnd = TRUE;		// �����ѥե饰���å�
		pFileData = pFileData->pNext;	// ���Υե�����򸡺�
	}

	// �ե�����ꥹ�Ⱥ��
	DeleteFileList(pfdFileList1);
	DeleteFileList(pfdFileList2);

	return TRUE;
}


// �ե�����ꥹ�Ⱥ���
TFileData* CTreeSearch::CreateFileList(LPCTSTR lpszPath)
{
	TFileData* pfdFileList;
	WIN32_FIND_DATA wfdFile;	// �ե��������
	HANDLE hFind;

	// �ե����븡���ϥ�ɥ�򳫤�
	hFind = FindFirstFile(lpszPath, &wfdFile);
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		pfdFileList = NULL;
	}
	else
	{
		TFileData* pFileData;

		// ��Ƭ�ե������ꥹ�Ȥ��ɲ�
		pFileData = new TFileData;
		pFileData->wfdFile = wfdFile;
		pFileData->blEnd   = FALSE;
		pFileData->pNext   = NULL;
		pfdFileList        = pFileData;

		// �ܹԤΥե�����򸫤Ĥ���¤��ɲ�
		while ( FindNextFile(hFind, &wfdFile) )
		{
			pFileData = new TFileData;
			pFileData->wfdFile = wfdFile;
			pFileData->blEnd   = FALSE;
			pFileData->pNext   = pfdFileList;
			pfdFileList        = pFileData;
		}

		// �ե����븡���ϥ�ɥ���Ĥ���
		FindClose(hFind);
	}

	return pfdFileList;
}


// �ե�����ꥹ�Ⱥ��
void CTreeSearch::DeleteFileList(TFileData* pFileList)
{
	TFileData* pFileData;
	TFileData* pfdNext;

	// �ꥹ�Ȥ򤹤٤ƺ��
	pFileData = pFileList;
	while ( pFileData != NULL )
	{
		pfdNext = pFileData->pNext;
		delete pFileData;
		pFileData = pfdNext;
	}
}


// �ե�����ꥹ�ȸ���
TFileData* CTreeSearch::SearchFile(TFileData* pFileList, LPCTSTR lpszFileName)
{
	TFileData* pFileData;

	// ���פ���ե�����̾��õ��
	pFileData = pFileList;
	while ( pFileData != NULL )
	{
		if ( !pFileData->blEnd
				&& lstrcmpi(pFileData->wfdFile.cFileName, lpszFileName) == 0 )
		{
			break;
		}
		pFileData = pFileData->pNext;
	}
	
	return pFileData;
}


// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Project HOS                                         
// ---------------------------------------------------------------------------
