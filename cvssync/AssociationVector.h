// ---------------------------------------------------------------------------
//  連想文字列配列クラス                                                      
//                                                                            
//                                          Copyright (C) 2002 by Project HOS 
// ---------------------------------------------------------------------------


#ifndef __AssociationVector_h__
#define __AssociationVector_h__


#include "StringVector.h"


// 文字列配列データ
struct AscVectData
{
	LPTSTR        lpszIndex;		// インデックス文字列
	CStringVector StrVect;			// 文字列配列
	AscVectData*  pNext;			// 次の要素
};


// 文字列配列クラス
class CAssociationVector
{
public:
	CAssociationVector();		// コンストラクタ
	~CAssociationVector();		// デストラクタ

	void           Add(LPCTSTR lpszIndex, LPCTSTR lpszString);		// 文字列の追加
	CStringVector* Get(LPCTSTR lpszIndex);							// 文字列配列の取得

protected:
	AscVectData* m_pList;		// 文字列配列データのリスト
};


#endif	// __AssociationVector_h__


// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Project HOS                                         
// ---------------------------------------------------------------------------
