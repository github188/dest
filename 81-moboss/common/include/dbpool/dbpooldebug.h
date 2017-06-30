/*=======================================================================
ģ����      :
�ļ���      :dbpooldebug.h
����ļ�    :
�ļ�ʵ�ֹ���:���Դ�ӡ����

����        : 
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/04/23  1.0					      ����
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

void  DBPoolAPIRegInLinux();			// ע����Դ�ӡ����

API void dbpoolhelp(void);
API void dbpoolver(void);

API void dbpoolloglevel(s32 nDbgLvl);

void DBPoolLog( s32 nPrtLevel, const s8* fmt, ... )
#ifdef __GNUC__  //ֻ��GCCʱ��enable
__attribute__((format(printf, 2, 3))) 
#endif
;


#endif // _DB_POOL_DEBUG_H_
// END OF FILE


