
/*=======================================================================
ģ����      : DBPool
�ļ���      : sqlstring.h
����ļ�    :
�ļ�ʵ�ֹ���:sql��䴦����

����        :
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2010/12/30  1.0                     ����
=======================================================================*/

#ifndef _DB_POOL_SQL_STRING_H_
#define _DB_POOL_SQL_STRING_H_

#include "kdvtype.h"
#include "dbpoolconst.h"


class TSqlString
{
public:
	TSqlString();
	void Empty(void);
	
	//�������
	BOOL32 Format(const s8* pszFmt, ...)
#ifdef __GNUC__  //ֻ��GCCʱ��enable
		__attribute__((format(printf, 2, 3))) //2��ʾ��2�������Ǹ�ʽ��,3��ʾ�ӵ�3��������ʼ�ǲ���.
#endif
;
	
	//�ֶ�β���
	BOOL32 AppendString(const s8* pszFmt, ...)
#ifdef __GNUC__  //ֻ��GCCʱ��enable
		__attribute__((format(printf, 2, 3))) //2��ʾ��2�������Ǹ�ʽ��,3��ʾ�ӵ�3��������ʼ�ǲ���.
#endif
;
	
	const s8* GetBuf(void) const;
	
	u32 GetBufLen(void) const;
	
protected:
	s8  m_achSqlString[MAXLEN_SQL_STRING+1];
	u32 m_dwSqlLen;
};


// ��ֹSQLע��ת��
class CSqlInjectionString
{
public:
	CSqlInjectionString(const s8* pszSource);
	CSqlInjectionString(const TSqlString& tSqlstr);
	CSqlInjectionString(const CSqlInjectionString& tOther);		// ��������
	void operator = (const CSqlInjectionString& tOther);	// ��ֵ����

	~CSqlInjectionString();

public:	
	const s8* GetString() const;

private:
	void Convert(const s8 *pszSource);
	
private:
	s8* m_pszDest;	
};

#endif // _DB_POOL_SQL_STRING_H_
// END OF FILE


