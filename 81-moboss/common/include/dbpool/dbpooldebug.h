/*=======================================================================
模块名      :
文件名      :dbpooldebug.h
相关文件    :
文件实现功能:调试打印函数

作者        : 
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/04/23  1.0					      创建
=======================================================================*/

#ifndef _DB_POOL_DEBUG_H_
#define _DB_POOL_DEBUG_H_

#include "osp.h"
#include "kdvtype.h"


#ifdef WIN32
#ifndef vsnprintf
#define vsnprintf   _vsnprintf
#endif
#ifndef snprintf
#define snprintf    _snprintf
#endif
#endif


#define DP_LOGLEVEL_CRITICAL			(u8)0
#define DP_LOGLEVEL_ERROR				(u8)1
#define DP_LOGLEVEL_WARNING				(u8)2
#define DP_LOGLEVEL_INFO				(u8)3
#define DP_LOGLEVEL_DETAIL				(u8)4

void  DBPoolAPIRegInLinux();			// 注册调试打印函数

API void dbpoolhelp(void);
API void dbpoolver(void);

API void dbpoolloglevel(s32 nDbgLvl);

void DBPoolLog( s32 nPrtLevel, const s8* fmt, ... )
#ifdef __GNUC__  //只在GCC时才enable
__attribute__((format(printf, 2, 3))) 
#endif
;


#endif // _DB_POOL_DEBUG_H_
// END OF FILE


