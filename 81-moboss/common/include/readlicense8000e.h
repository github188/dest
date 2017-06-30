/*=======================================================================
模块名      :
文件名      : readlicense.h
相关文件    :
文件实现功能 : 读取license文件的内容
作者        : 邵云东
版本        : KCMV1.0  Copyright(C) 2003-2008 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2007/01/09  KCM1.0      邵云东      创建
2009/04/01  kdv1.0      zhanghb	    Add  writelicense Interface for 8000E
=======================================================================*/
#ifndef _READLICENSE_H_
#define _READLICENSE_H_

#include <string.h>

///////////////////////////////////////////////////////////////////
//	Linux 操作系统头文件
///////////////////////////////////////////////////////////////////
#if defined(_LINUX_)
#ifdef PWLIB_SUPPORT
#include <ptlib.h>
#endif

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include <malloc.h>

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/times.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <termios.h>
#include <signal.h>
#include "syslog.h"

#endif //_LINUX_


#ifdef _MSC_VER

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#ifdef WINVER
#undef WINVER
#endif

/* vc is vs2005 and later */
#if (_MSC_VER >= 1400)
#define _WIN32_WINNT 0x0501 //support only winxp and above
#define WINVER 0x0501 //support only winxp and above
#else
#define _WIN32_WINNT 0x400
#define WINVER 0x0400
#endif

#include <malloc.h>
#ifdef _AFXDLL
#include <afx.h>
#include <afxwin.h>
#include <winsock2.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#endif

#include <assert.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib, "User32.lib")

#endif	// _MSC_VER

#include "kdvtype.h"

#define  KDC_CALL	extern "C"

typedef struct tagKey_Value 
{
	tagKey_Value()
	{
		memset(achKey, 0, sizeof(achKey));
		memset(achValue, 0, sizeof(achValue));
	}
	s8 achKey[1024];
	s8 achValue[1024];
}TKeyValue;

/*========================================================================
函 数 名：ReadLicenseFile
功    能：读取License文件
参    数：s8 *pchPath					[in]License文件名(全路径名－请以'\0'结尾)
		  
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
========================================================================*/
KDC_CALL u16 ReadLicenseFile(const s8 *pchPath);

/*========================================================================
函 数 名：WriteLicenseFile
功    能：写License文件
参    数：BOOL32 bToUsb = FALSE             [in]    是否写入USB设备
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
2007/06/12  1.0     王昊    允许写入USB设备
========================================================================*/
KDC_CALL u16 WriteLicenseFile(BOOL32 bToUsb = FALSE);

/*========================================================================
函 数 名：GetKeyValueNum
功    能：获取License文件中有多少Key－value值对
参    数：
返 回 值：Key－value值对数
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/23	KCM1.0	邵云东	创建
========================================================================*/
KDC_CALL u16 GetKeyValueNum();
/*========================================================================
函 数 名：EnumKeyList
功    能：枚举License文件中有多少Key值
参    数：TKeyValue *pchKeyValueList	[out]Key-Value值列表
		  s32 &nListLen					[in/out]Key-Value值列表空间大小/Key-Value值数
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
========================================================================*/
KDC_CALL u16 EnumKeyList(TKeyValue *pchKeyValueList, s32 &nListLen);
/*========================================================================
函 数 名：GetValue
功    能：根据Key值获取相应的值
参    数：s8 *pchKey					[in]Key值
		  s8 *pchValue					[out]值
		  s32 &nValueLen				[in/out]值有效长度
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
========================================================================*/
KDC_CALL u16 GetValue(const s8 *pchKey, s8 *pchValue, s32 &nValueLen);
/*========================================================================
函 数 名：GetErrorCodeDescription
功    能：获取调用产生的错误码的描述
参    数：u16 wErrorCode				[in]错误码
		  s8 *pchDescription			[out]错误码描述
		  s32 &nLen						[in/out]错误码描述有效长度
		  s8 *pchIniFilePath			[in]错误码描述文件全路径(包括文件名)，请以'\0'结尾
返 回 值：BOOL32 TRUE : 成功   FALSE : 失败
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
========================================================================*/
KDC_CALL BOOL32 GetErrorCodeDescription(u16 wErrorCode, s8 *pchDescription, s32 &nLen, s8 *pchIniFilePath);

/*=============================================================================
函 数 名: SetValue
功    能: 向内存设置数据
参    数: LPCSTR lpszKey                    [in]    段名
          LPCSTR lpszValue                  [in]    键值
注    意: 仅刷新读取出的数据内存
返 回 值: 错误码
-------------------------------------------------------------------------------
修改纪录:
日      期  版本    修改人  修改内容
2007/06/12  1.0     王昊    创建
=============================================================================*/
KDC_CALL u16 SetValue(LPCSTR lpszKey, LPCSTR lpszValue);
/*=============================================================================
函 数 名: GenEncodeData
功    能: 加密
参    数: s8* ptrInBuf,                  [in]    待加解密数据
          u16 wInBufLen                  [in]    数据长度(按字节计算)
          BOOL32 bDirect                 [in]    加密: TRUE; 解密: FALSE
          s8* ptrOutBuf                  [out]   加解密后的数据
注    意: 无
返 回 值: 错误码
-------------------------------------------------------------------------------
修改纪录:
日      期  版本    修改人  修改内容
2009/04/02  1.0     zhanghb    创建
=============================================================================*/
KDC_CALL void GenEncodeData(s8* ptrInBuf, u16 wInBufLen, BOOL32 bDirect,s8* ptrOutBuf);
/*=============================================================================
函 数 名: GetKeyValue
功    能: 得到key和value
参    数: 
注    意: 无
返 回 值: 错误码
-------------------------------------------------------------------------------
修改纪录:
日      期  版本    修改人  修改内容
2009/04/02  1.0     zhanghb    创建
=============================================================================*/
u16 GetKeyValue(s8 *pchBuf, s8 *pchKey, s32 &nKeyLen, s8 *pchValue, s32 &nValueLen);
/*=============================================================================
函 数 名: CreateLicenseFile
功    能: 生成license文件
参    数: 
注    意: 
返 回 值: 错误码
-------------------------------------------------------------------------------
修改纪录:
日      期  版本    修改人  修改内容
2009/04/02  1.0     zhanghb    创建
=============================================================================*/

#ifdef WIN32
KDC_CALL u16 CreateLicenseFile( BSTR pbyAuthCode,BSTR pbyLicenseKey,BSTR pbyLicenseContent);
#endif


#endif // _READLICENSE_H_