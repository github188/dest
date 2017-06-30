

/*****************************************************************************
   ģ����      :
   �ļ���      : bossutils.h
   ����ļ�    :
   �ļ�ʵ�ֹ���: ��װ81-mobossĿ¼��һЩ����С����
   ����        :
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2012/04/23  1.0                     ����
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
/*							IP ת��                                     */
/************************************************************************/
// �õ��ַ�����IP(����������)
std::string GetIpString(u32 dwIp);
// �õ���ֵ��IP(����������)		
u32 GetIpByString(const s8* achIp);	 

//�Ƿ��Ǿ�����IP(������)
BOOL32 IsLANIp(u32 dwIp);
//�������볤��(������)
u8 GetIpMaskLength( u32 dwIpMask );

BOOL32 IsValidIpV4(u32 dwIP);
BOOL32 IsValidIpV4(const s8* lptrIPStr);
BOOL32 IsValidIpV4(const std::string &csIP);

BOOL32 IsValidIpMask(u32 dwIpMask);
BOOL32 IsValidIpMask(const s8* lptrIpMask);
BOOL32 IsValidIpMask(const std::string lptrIpMask);

/************************************************************************/
/*							ʱ��                                        */
/************************************************************************/

time_t GetSysTime( void );
std::string GetSysTimeString( void );
std::string TimeToString( time_t dwTime);

// �õ�ϵͳ��Tick
u32 GetSysTick();

/************************************************************************/
/*								����                                    */
/************************************************************************/

//�����Ƿ��Ѿ�������
BOOL32 IsProcessRunned( const s8 * pszApplicationName );
//��ȡ��ǰ����ID
u32 GetCurrentProcessID( void );


//����ʵ��system����ֹfd���ӽ��̼̳�
int system_closefd(const char * cmdstring, u32 dwTimeOut);



/************************************************************************/
/*								�ļ�                                    */
/************************************************************************/

//����Ŀ¼
BOOL32 CreatDir(const s8* lpszDirPath);
//��������·���µ��ļ���ͬʱ�Զ�������Ҫ��Ŀ¼
BOOL32 CreatFile( const s8 * lpszFileFullPath );

// ��ȡģ�������ļ���ȫ·��, ��������'\' ���� "C:\"
std::string GetModuleFullPath(void);

// �õ�ģ���ȫ·���������ļ���
BOOL32 GetModFilePath(s8 *lpModName, s32 &nNameLen);


BOOL32 IsFileExist(const s8* szFilePath);
u32 GetFileSize(const s8* szFilePath);



/************************************************************************/
/*						  u64ת���ַ���                                 */
/************************************************************************/

u64 strtou64(/*IN*/ const s8* szU64Str);
BOOL32 u64tostr(/*IN*/ u64 lwValue, /*IN OUT*/ s8* strBuf, /*IN*/ u16 wMaxStrBufSize);

/************************************************************************/
/*						 ȫ��Ψһ��ʶ�ţ�GUID��UUID(32λ)������                                 */
/************************************************************************/

//std::string ConstructUID(void);

} // end namespace


#endif //_MO_BOSS_UTILS_H_
//End of file





