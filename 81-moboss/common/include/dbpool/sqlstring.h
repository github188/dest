
/*=======================================================================
模块名      : DBPool
文件名      : sqlstring.h
相关文件    :
文件实现功能:sql语句处理类

作者        :
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2010/12/30  1.0                     创建
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
	
	//整体操作
	BOOL32 Format(const s8* pszFmt, ...)
#ifdef __GNUC__  //只在GCC时才enable
		__attribute__((format(printf, 2, 3))) //2表示第2个参数是格式串,3表示从第3个参数开始是参数.
#endif
;
	
	//分多次操作
	BOOL32 AppendString(const s8* pszFmt, ...)
#ifdef __GNUC__  //只在GCC时才enable
		__attribute__((format(printf, 2, 3))) //2表示第2个参数是格式串,3表示从第3个参数开始是参数.
#endif
;
	
	const s8* GetBuf(void) const;
	
	u32 GetBufLen(void) const;
	
protected:
	s8  m_achSqlString[MAXLEN_SQL_STRING+1];
	u32 m_dwSqlLen;
};


// 防止SQL注入转换
class CSqlInjectionString
{
public:
	CSqlInjectionString(const s8* pszSource);
	CSqlInjectionString(const TSqlString& tSqlstr);
	CSqlInjectionString(const CSqlInjectionString& tOther);		// 拷贝构造
	void operator = (const CSqlInjectionString& tOther);	// 赋值构造

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


