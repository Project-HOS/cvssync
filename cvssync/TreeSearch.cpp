// ---------------------------------------------------------------------------
//  ツリー検索クラス                                                          
//                                                                            
//                                          Copyright (C) 2002 by Project HOS 
// ---------------------------------------------------------------------------


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "TreeSearch.h"



// コンストラクタ
CTreeSearch::CTreeSearch(BOOL (*pfncProc)(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag))
{
	// ファイル処理ルーチンを登録
	m_pfncProc = pfncProc;
}


// ツリーサーチ
BOOL CTreeSearch::Search(LPCTSTR lpszPath1, LPCTSTR lpszPath2)
{
	TCHAR  szSearchPath1[MAX_PATH];
	TCHAR  szSearchPath2[MAX_PATH];
	TFileData* pfdFileList1;
	TFileData* pfdFileList2;
	TFileData* pFileData;
	TFileData* pfdTmp;

	// サーチパス作成
	wsprintf(szSearchPath1, _T("%s*.*"), lpszPath1);
	wsprintf(szSearchPath2, _T("%s*.*"), lpszPath2);

	// ファイルリストの読み込み
	pfdFileList1 = CreateFileList(szSearchPath1);
	pfdFileList2 = CreateFileList(szSearchPath2);

	// リスト１からサーチ
	pFileData = pfdFileList1;
	while ( pFileData != NULL )
	{
		if ( pFileData->wfdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			// ディレクトリなら
			if ( lstrcmpi(pFileData->wfdFile.cFileName, _T(".")) != 0
					&& lstrcmpi(pFileData->wfdFile.cFileName, _T("..")) != 0
					&& lstrcmpi(pFileData->wfdFile.cFileName, _T("CVS")) != 0 )		// CVSフォルダは無視
			{
				TCHAR szSubDirectry1[MAX_PATH];
				TCHAR szSubDirectry2[MAX_PATH];
				
				// ディレクトリのフルパスを作成
				wsprintf(szSubDirectry1, "%s%s\\", lpszPath1, pFileData->wfdFile.cFileName);
				wsprintf(szSubDirectry2, "%s%s\\", lpszPath2, pFileData->wfdFile.cFileName);
	
				// 相手に対応するファイルがあるか検索
				pfdTmp = SearchFile(pfdFileList2, pFileData->wfdFile.cFileName);
				if ( pfdTmp == NULL )
				{
					// 無ければ作成
					if ( !CreateDirectory(szSubDirectry2, NULL) )
					{
						// 作成失敗
						printf("fale : create directry \"%s\"\n", szSubDirectry2);
					}
					else
					{
						// 再起的に検索
						printf("create directry \"%s\"\n", szSubDirectry2);						
						Search(szSubDirectry1, szSubDirectry2);
					}
				}
				else
				{
					// あれば確認
					if ( !(pfdTmp->wfdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
					{
						// 同名のファイルがある
						printf("fale : create directry \"%s\"\n", szSubDirectry2);
					}
					else
					{
						// 再起的に検索
						Search(szSubDirectry1, szSubDirectry2);
					}
					pfdTmp->blEnd = TRUE;	// 処理済フラグセット
				}
			}
		}
		else
		{
			// ファイルなら
			TCHAR szFileName1[MAX_PATH];
			TCHAR szFileName2[MAX_PATH];

			// ファイルのフルパスを作成
			wsprintf(szFileName1, "%s%s", lpszPath1, pFileData->wfdFile.cFileName);
			wsprintf(szFileName2, "%s%s", lpszPath2, pFileData->wfdFile.cFileName);
			
			// 相手側に同名ファイルがあるかどうか検索
			pfdTmp = SearchFile(pfdFileList2, pFileData->wfdFile.cFileName);
			if ( pfdTmp == NULL )
			{
				// 無ければFile1のみでファイル処理ルーチン呼び出し
				m_pfncProc(szFileName1, szFileName2, FILEPROC_FILE1);
			}
			else
			{
				// あれば双方ファイルありでファイル処理ルーチン呼び出し
				m_pfncProc(szFileName1, szFileName2, FILEPROC_BOTH);
				pfdTmp->blEnd = TRUE;	// 処理済フラグセット
			}
		}
		pFileData->blEnd = TRUE;		// 処理済フラグセット
		pFileData = pFileData->pNext;	// 次のファイルを検索
	}

	// リスト２の残りをサーチ
	pFileData = pfdFileList2;
	while ( pFileData != NULL )
	{
		if ( !pFileData->blEnd )	// 未処理のものを処理する
		{
			if ( pFileData->wfdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// ディレクトリなら
				if ( lstrcmpi(pFileData->wfdFile.cFileName, _T(".")) != 0
					&& lstrcmpi(pFileData->wfdFile.cFileName, _T("..")) != 0
					&& lstrcmpi(pFileData->wfdFile.cFileName, _T("CVS")) != 0 )		// CVSディレクトリを無視
				{
					TCHAR szSubDirectry1[MAX_PATH];
					TCHAR szSubDirectry2[MAX_PATH];
					
					// ディレクトリのフルパスを作成
					wsprintf(szSubDirectry1, "%s%s\\", lpszPath1, pFileData->wfdFile.cFileName);
					wsprintf(szSubDirectry2, "%s%s\\", lpszPath2, pFileData->wfdFile.cFileName);

					// ディレクトリを作成
					if ( !CreateDirectory(szSubDirectry1, NULL) )
					{
						// 作成失敗
						printf("fale : create directry \"%s\"\n", szSubDirectry1);
					}
					else
					{
						// 再起的に検索
						printf("create directry \"%s\"\n", szSubDirectry1);						
						Search(szSubDirectry1, szSubDirectry2);
					}
				}
			}
			else
			{
				// ファイルなら
				TCHAR szFileName1[MAX_PATH];
				TCHAR szFileName2[MAX_PATH];

				// ファイルのフルパス名を作成
				wsprintf(szFileName1, "%s%s", lpszPath1, pFileData->wfdFile.cFileName);
				wsprintf(szFileName2, "%s%s", lpszPath2, pFileData->wfdFile.cFileName);

				// File2のみでファイル処理ルーチンを呼び出し
				m_pfncProc(szFileName1, szFileName2, FILEPROC_FILE2);
			}
		}

		pFileData->blEnd = TRUE;		// 処理済フラグセット
		pFileData = pFileData->pNext;	// 次のファイルを検索
	}

	// ファイルリスト削除
	DeleteFileList(pfdFileList1);
	DeleteFileList(pfdFileList2);

	return TRUE;
}


// ファイルリスト作成
TFileData* CTreeSearch::CreateFileList(LPCTSTR lpszPath)
{
	TFileData* pfdFileList;
	WIN32_FIND_DATA wfdFile;	// ファイル情報
	HANDLE hFind;

	// ファイル検索ハンドルを開く
	hFind = FindFirstFile(lpszPath, &wfdFile);
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		pfdFileList = NULL;
	}
	else
	{
		TFileData* pFileData;

		// 先頭ファイルをリストに追加
		pFileData = new TFileData;
		pFileData->wfdFile = wfdFile;
		pFileData->blEnd   = FALSE;
		pFileData->pNext   = NULL;
		pfdFileList        = pFileData;

		// 移行のファイルを見つかる限り追加
		while ( FindNextFile(hFind, &wfdFile) )
		{
			pFileData = new TFileData;
			pFileData->wfdFile = wfdFile;
			pFileData->blEnd   = FALSE;
			pFileData->pNext   = pfdFileList;
			pfdFileList        = pFileData;
		}

		// ファイル検索ハンドルを閉じる
		FindClose(hFind);
	}

	return pfdFileList;
}


// ファイルリスト削除
void CTreeSearch::DeleteFileList(TFileData* pFileList)
{
	TFileData* pFileData;
	TFileData* pfdNext;

	// リストをすべて削除
	pFileData = pFileList;
	while ( pFileData != NULL )
	{
		pfdNext = pFileData->pNext;
		delete pFileData;
		pFileData = pfdNext;
	}
}


// ファイルリスト検索
TFileData* CTreeSearch::SearchFile(TFileData* pFileList, LPCTSTR lpszFileName)
{
	TFileData* pFileData;

	// 一致するファイル名を探す
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
