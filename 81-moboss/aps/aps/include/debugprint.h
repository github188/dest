/*=======================================================================
ģ����      :
�ļ���      :debugprint.h
����ļ�    :
�ļ�ʵ�ֹ���:���Դ�ӡ����

����        :
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/07/05  1.0                       ����
=======================================================================*/
#ifndef _DEBUG_INFO_H_
#define _DEBUG_INFO_H_

#include "osp.h"
#include "kdvtype.h"
#include "kdvlog.h"
#include "loguserdef.h"

#include <string>

#ifdef WIN32
#ifndef vsnprintf
#define vsnprintf   _vsnprintf
#endif
#ifndef snprintf
#define snprintf    _snprintf
#endif
#endif

// #define LOG_LEVEL_CRITICAL          (u8)0
// #define LOG_LEVEL_ERROR             (u8)1
// #define LOG_LEVEL_WARNING           (u8)2
// #define LOG_LEVEL_INFO              (u8)3
// #define LOG_LEVEL_DETAIL            (u8)4

class CPrintFunInOut
{
public:
	CPrintFunInOut(const s8* pchFunName, const u16 wModule = MID_ENTRANCE_APS, const u8 byLevel = LOG_LVL_KEYSTATUS);
	CPrintFunInOut(const s8* pchFunName, const s8* pchDescribe, ...);
	CPrintFunInOut(const s8* pchFunName, const u16 wModule, const s8* pchDescribe, ...);
	CPrintFunInOut(const s8* pchFunName, const u16 wModule, const u8 byLevel, const s8* pchDescribe, ...);
	~CPrintFunInOut();
private:
	void LogFunIn();
	std::string GetModuleSeperator();
private:
	std::string m_describe;
	std::string m_funName;
	u16 m_wModule;
	u8 m_byLevel;
};


void APIRegInLinux(void);
void AddEventStr();

API void help(void);
API void apsver(void);
API void quit();

//����
API void cctrlloglevel(s32 nDbgLvl);
void CCtrlLog( s32 nPrtLevel, const s8* fmt, ... )
#ifdef __GNUC__  //ֻ��GCCʱ��enable
__attribute__((format(printf, 2, 3))) //2��ʾ��2�������Ǹ�ʽ��,3��ʾ�ӵ�3��������ʼ�ǲ���.
#endif
;

//���ݿ�
API void dbpooldump();

API void dbloglevel(s32 nDbgLvl);
void DBOprLog(s32 nPrtLevel, const s8 *fmt, ...)
#ifdef __GNUC__  //ֻ��GCCʱ��enable
__attribute__((format(printf, 2, 3))) //2��ʾ��2�������Ǹ�ʽ��,3��ʾ�ӵ�3��������ʼ�ǲ���.
#endif
;

//ConSASsn
API void consaloglevel(s32 nDbgLvl);
void ConSaSsnLog(s32 nPrtLevel, const s8 *fmt, ...)
#ifdef __GNUC__  //ֻ��GCCʱ��enable
__attribute__((format(printf, 2, 3))) //2��ʾ��2�������Ǹ�ʽ��,3��ʾ�ӵ�3��������ʼ�ǲ���.
#endif
;

//ClientSsn
API void clientnode(void);
API void clientloglevel(s32 nDbgLvl);
void ClientSsnLog(s32 nPrtLevel, const s8 *fmt, ...)
#ifdef __GNUC__  //ֻ��GCCʱ��enable
__attribute__((format(printf, 2, 3))) //2��ʾ��2�������Ǹ�ʽ��,3��ʾ�ӵ�3��������ʼ�ǲ���.
#endif
;

//HttpSsn
API void httploglevel(s32 nDbgLvl);
void HttpSsnLog(s32 nPrtLevel, const s8 *fmt, ...)
#ifdef __GNUC__  //ֻ��GCCʱ��enable
__attribute__((format(printf, 2, 3))) //2��ʾ��2�������Ǹ�ʽ��,3��ʾ�ӵ�3��������ʼ�ǲ���.
#endif
;

//SrvListMgr
API void srvlistmgrloglevel(s32 nDbglvl);
void SrvListMgrLog(s32 nPrtLevel, const s8 *fmt, ...)
#ifdef __GNUC__  //ֻ��GCCʱ��enable
__attribute__((format(printf, 2, 3))) //2��ʾ��2�������Ǹ�ʽ��,3��ʾ�ӵ�3��������ʼ�ǲ���.
#endif
;

API void showsrv();
API void showcfg();
API void showmtinfo();
API void apsloglvl(s32 nLogLvl );
API void loadlogin();
API void login();
API void settryapsspan(const u32 dwSpan);
API void showbackup();

#endif //end of _DEBUG_INFO_H_
