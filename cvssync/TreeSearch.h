// ---------------------------------------------------------------------------
//  �ĥ꡼�������饹                                                          
//                                                                            
//                                          Copyright (C) 2002 by Project HOS 
// ---------------------------------------------------------------------------


#ifndef __TreeSearch_h__
#define __TreeSearch_h__



// �ե饰���
#define FILEPROC_FILE1		0x01	// �ե����룱�Τߤ���
#define FILEPROC_FILE2		0x02	// �ե����룲�Τߤ���
#define FILEPROC_BOTH		0x03	// ξ���Υե����뤬����



// �ե��������ꥹ���ѹ�¤��
struct TFileData
{
	WIN32_FIND_DATA wfdFile;	// �ե��������
	BOOL            blEnd;		// �����Ѥߥե饰
	TFileData*      pNext;		// ���ؤΥݥ���
};


// �ĥ꡼���������饹
class CTreeSearch
{
public:
	CTreeSearch(BOOL (*pfncProc)(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag));		// ���󥹥ȥ饯��
	BOOL Search(LPCTSTR lpszPath1, LPCTSTR lpszPath2);									// ����

protected:
	TFileData* CreateFileList(LPCTSTR lpszPath);										// �ե�����ꥹ�Ⱥ���
	void       DeleteFileList(TFileData* pFileList);									// �ե�����ꥹ���˴�
	TFileData* SearchFile(TFileData* pFileList,  LPCTSTR lpszFileName);					// �ե�����θ���

	BOOL (*m_pfncProc)(LPCTSTR lpszFile1, LPCTSTR lpszFile2, int iFlag);				// �ե���������롼����
};



#endif	// __TreeSearch_h__


// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Project HOS                                         
// ---------------------------------------------------------------------------
