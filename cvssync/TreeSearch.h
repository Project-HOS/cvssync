// ---------------------------------------------------------------------------
//  ツリー検索クラス                                                          
//                                                                            
//                                          Copyright (C) 2002 by Project HOS 
// ---------------------------------------------------------------------------


#ifndef __TreeSearch_h__
#define __TreeSearch_h__



// フラグ定義
#define FILEPROC_FILE1		0x01	// ファイル１のみある
#define FILEPROC_FILE2		0x02	// ファイル２のみある
#define FILEPROC_BOTH		0x03	// 両方のファイルがある



// ファイル情報リスト用構造体
struct TFileData
{
	WIN32_FIND_DATA wfdFile;	// ファイル情報
	BOOL            blEnd;		// 処理済みフラグ
	TFileData*      pNext;		// 次へのポインタ
};


// ツリーサーチクラス
class CTreeSearch
{
public:
	CTreeSearch(BOOL (*pfncProc)(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag));		// コンストラクタ
	BOOL Search(LPCTSTR lpszPath1, LPCTSTR lpszPath2);									// 検索

protected:
	TFileData* CreateFileList(LPCTSTR lpszPath);										// ファイルリスト作成
	void       DeleteFileList(TFileData* pFileList);									// ファイルリスト破棄
	TFileData* SearchFile(TFileData* pFileList,  LPCTSTR lpszFileName);					// ファイルの検索

	BOOL (*m_pfncProc)(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag);				// ファイル処理ルーチン
};



#endif	// __TreeSearch_h__


// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Project HOS                                         
// ---------------------------------------------------------------------------
