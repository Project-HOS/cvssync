// ---------------------------------------------------------------------------
//  ����ե������ɤ߹���
//
//                                      Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "ReadConfig.h"


#define READCFG_MAX_LINE	256



//  ����ե������ɤ߹���
void ReadConfig(LPCTSTR szFileName, CAssociationVector* pAscVect)
{
	TCHAR szIndex[READCFG_MAX_LINE] = _T("");
	TCHAR szBuf[READCFG_MAX_LINE];
	FILE* fp;

	// �ե����륪���ץ�
	if ( (fp = _tfopen(szFileName, "r")) == NULL )
	{
		return;
	}

	// �ɤ߹���
	while ( _fgetts(szBuf, READCFG_MAX_LINE, fp) != NULL )
	{
		// ���Ԥ�Ĺ�������齪���
		int iLen = lstrlen(szBuf);
		if ( iLen >= 0 && szBuf[iLen - 1] != _T('\n') )
		{
			break;	
		}
		
		// ��³�ζ���ʸ�����
		while ( iLen > 0 && _istspace(szBuf[iLen - 1]) )
		{
			iLen--;
		}
		szBuf[iLen] = _T('\0');

		if ( iLen > 3 && szBuf[0] == _T('[') && szBuf[iLen - 1] == _T(']') )
		{
			// ����ǥå�������
			lstrcpy(szIndex, szBuf);
		}
		else if ( szIndex[0] != _T('\0') )
		{
			// ���ԤǤʤ����Ϣ��������ɲ�
			if ( szBuf[0] != _T('\0') )
			{
				pAscVect->Add(szIndex, szBuf);
			}
		}
	}

	// �ե����륯����
	fclose(fp);
}


// ---------------------------------------------------------------------------
//  Copyright (C) 2002 by Ryuji Fuchikami
// ---------------------------------------------------------------------------
