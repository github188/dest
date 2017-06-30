/*****************************************************************************
   模块名      : 4.8网管定义
   文件名      : snmpclt.h
   创建时间    : 2014年 5月 26日
   实现功能    : 消息中心客户端头文件
   作者        : 周晶晶
   版本        : 0.1
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   14/06/12    0.1         周晶晶      创建 
******************************************************************************/


#ifndef _SNMPCLT_H_
#define _SNMPCLT_H_

#include "osp.h"
#include "kdvlog.h"
#include "loguserdef.h"
#include "kdvsys.h"
#include "mcuconst.h"
#include "mcustruct.h"
#include "commonlib.h"
#include "mcuver.h"
#include "evagtsvc.h"
#include "mcuagtstruct.h"
#include "evbrdmgrsvc.h"
#include "evconfsvc.h"
#include "appdef.h"
#include "agtcomm.h"
#include "ipmi_api.h"
#include "evmcu.h"

const u16 DUMPCLTINFO = 1;


//网管服务端标识定义
#define BOARD_NONE			(u8)0//ceu单板
#define BOARD_CEU			(u8)1//ceu单板
#define BOARD_UMU			(u8)2//umu单板
#define BOARD_SMU			(u8)7//smu单板	//跟驱动、网管定义的id冲突，修改为较大的值
#define BOARD_XMPU			(u8)4//xmpu单板

#define	 FAN_MCU_OBJECT				(u8)5	//告警标识1,2,3,4的定义在agtcomm.h中
#define	 POWER_MCU_OBJECT			(u8)6
#define	 POWERTEMP_MCU_OBJECT		(u8)7
#define	 FANTEMP_MCU_OBJECT			(u8)8
#define	 DISKCOUNT_MCU_OBJECT		(u8)9

#define SNMPSVC_BRDMGR		(u8)1//单板管理服务
#define SNMPSVC_MEDIARES	(u8)2//媒体资源服务
#define SNMPSVC_MTACCESSRES	(u8)3//终端接入服务
// #define	SNMPSVC_CONF		(u8)4//传统媒体会议服务
// #define	SNMPSVC_MEDIACONF	(u8)5//传统媒体会议服务
#define	SNMPSVC_ACS			(u8)4//ACS会议服务

//typedef void (*SnmpMsgCenterConnected)(void);

typedef void (*SnmpMsgRecvCallBack)( const u32 &dwSrcNode,const u32 &dwSrcIId,
							const u16 &wEvent,const u8* pbyMsg,
							const u16 &wLen);





BOOL32 SnmpCltInit( SnmpMsgRecvCallBack CltSnmpMsgRecv,const u8 &byBoardType = 0,const u8 &bySlot = 0,const u8 &byMsState = 0xff );

BOOL32 SnmpCltQuit();

API void snmpcltver();

void snmpclthelp();



#endif