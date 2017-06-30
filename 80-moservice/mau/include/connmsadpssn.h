/*****************************************************************************
模块名      : connmsadpssn通信模块
文件名      : connmsadpssn.h
相关文件    : 
文件实现功能: connmsadpssn通信模块实例定义
作者        : 阮楠楠
版本        : V5.0  Copyright(C) 2009-2010 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/01/15  5.0         阮楠楠        创建
******************************************************************************/
#ifndef _MAU_CONNMSADPSSN_H_
#define _MAU_CONNMSADPSSN_H_

#include "evmau.h"
#include "connectadp_api.h"
#include "mauvc.h"
#include "cmussn.h"
#include "nussn.h"
#include "upussn.h"
#include "mcuver.h"
#include "resmgrssn.h"
#include "ztemplatedaemon.h"

extern CCMUSsnApp g_cCMUSsnApp;
extern CNUSsnApp g_cNUSsnApp;
extern CMauVCApp g_cMauVCApp;
extern CUpuSsnApp g_cUpuSsnApp;

//定义用于通知配置信息是否完全的回调函数
void SetCssPfmCallBackFunc( CAdpServMsg & cAdpServMsg );
//网管连接成功回调通知
void ConnNMSSuccessCallBackFunc();

class CConNmsAdpInst : public CInstance  
{
public:
    //守护实例入口
    void DaemonInstanceEntry( CMessage * const pcMsg, CApp * pcApp );
    //普通实例入口
    void InstanceEntry( CMessage * const pcMsg );
	
    void DaemonProcPowerOn( CMessage * const pcMsg );
	void DaemonProcOspDisconnect( CMessage * const pcMsg );
	void DaemonProcTransMsgToAdp( CMessage* const pcMsg );
	
};

typedef zTemplate< CConNmsAdpInst, 0, CAppNoData, 0 > CConNmsAdpApp;

#endif
