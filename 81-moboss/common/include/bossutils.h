

/*****************************************************************************
   模块名      :
   文件名      : bossutils.h
   相关文件    :
   文件实现功能: 封装81-moboss目录下一些公用小函数
   作者        :
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2012/04/23  1.0                     创建
******************************************************************************/

#ifndef _MO_BOSS_UTILS_H_
#define _MO_BOSS_UTILS_H_


#pragma warning (disable:4786 )

#include <string>
#include <stdarg.h>
#include "kdvtype.h"


#ifdef WIN32
#ifndef vsnprintf
#define vsnprintf   _vsnprintf
#endif
#ifndef snprintf
#define snprintf    _snprintf
#endif
#endif

#ifdef WIN32
#ifndef FormatU64
#define FormatU64   "%I64u"
#endif
#endif

#ifdef _LINUX_
#ifndef FormatU64
#define FormatU64   "%llu"
#endif
#endif

//#define GUID_LEN   36 

namespace BossUtils{

/************************************************************************/
/*							IP 转换                                     */
/************************************************************************/
// 得到字符串型IP(传入主机序)
std::string GetIpString(u32 dwIp);
// 得到数值型IP(返回主机序)		
u32 GetIpByString(const s8* achIp);	 

//是否是局域网IP(主机序)
BOOL32 IsLANIp(u32 dwIp);
//子网掩码长度(主机序)
u8 GetIpMaskLength( u32 dwIpMask );

BOOL32 IsValidIpV4(u32 dwIP);
BOOL32 IsValidIpV4(const s8* lptrIPStr);
BOOL32 IsValidIpV4(const std::string &csIP);

BOOL32 IsValidIpMask(u32 dwIpMask);
BOOL32 IsValidIpMask(const s8* lptrIpMask);
BOOL32 IsValidIpMask(const std::string lptrIpMask);

/************************************************************************/
/*							时间                                        */
/************************************************************************/

time_t GetSysTime( void );
std::string GetSysTimeString( void );
std::string TimeToString( time_t dwTime);

// 得到系统的Tick
u32 GetSysTick();

/************************************************************************/
/*								进程                                    */
/************************************************************************/

//进程是否已经被启动
BOOL32 IsProcessRunned( const s8 * pszApplicationName );
//获取当前进程ID
u32 GetCurrentProcessID( void );


//重新实现system，防止fd被子进程继承
int system_closefd(const char * cmdstring, u32 dwTimeOut);



/************************************************************************/
/*								文件                                    */
/************************************************************************/

//创建目录
BOOL32 CreatDir(const s8* lpszDirPath);
//创建任意路径下的文件，同时自动创建需要的目录
BOOL32 CreatFile( const s8 * lpszFileFullPath );

// 获取模块所在文件夹全路径, 包括最后的'\' 例如 "C:\"
std::string GetModuleFullPath(void);

// 得到模块的全路径，包括文件名
BOOL32 GetModFilePath(s8 *lpModName, s32 &nNameLen);


BOOL32 IsFileExist(const s8* szFilePath);
u32 GetFileSize(const s8* szFilePath);



/************************************************************************/
/*						  u64转换字符串                                 */
/************************************************************************/

u64 strtou64(/*IN*/ const s8* szU64Str);
BOOL32 u64tostr(/*IN*/ u64 lwValue, /*IN OUT*/ s8* strBuf, /*IN*/ u16 wMaxStrBufSize);

/************************************************************************/
/*						 全球唯一标识号（GUID或UUID(32位)）生成                                 */
/************************************************************************/

//std::string ConstructUID(void);

} // end namespace


#endif //_MO_BOSS_UTILS_H_
//End of file





