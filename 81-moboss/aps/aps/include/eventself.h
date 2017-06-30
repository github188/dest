
/*=======================================================================
模块名      :XAP
文件名      :eventself.h
相关文件    :
文件实现功能:XAP内部通信消息定义

作者        :
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/07/12  1.0                       创建
=======================================================================*/
#ifndef _EVENT_APS_SELF_H_
#define _EVENT_APS_SELF_H_

#include "osp.h"
#include "eventid.h"

#define EV_APS_SELF_BEG                          5000


/************************************************************************/
/*								Dump                    
/************************************************************************/
//dump dbpool的状态
OSPEVENT(APS_DBPOOL_STATE_DUMP,                 EV_APS_SELF_BEG + 1);

//dump ulc的状态
OSPEVENT(APS_DUMP_ULC_STATE_DUMP,               EV_APS_SELF_BEG + 2);

//dump 服务器状态
OSPEVENT(APS_DUMP_SRV_STATE_DUMP,               EV_APS_SELF_BEG + 4);


/************************************************************************/
/*								                    
/************************************************************************/
//健康检测定时器
OSPEVENT(CHECK_HEALTH_TIMER,                    EV_APS_SELF_BEG + 10);
           

/************************************************************************/
/*								请求服务器列表                   
/************************************************************************/
//请求服务器列表定时器
OSPEVENT(QUERY_SERVERDEV_LIST_TIMER,            EV_APS_SELF_BEG + 30);
OSPEVENT(QUERY_SERVERDEV_LIST_REQ,              EV_APS_SELF_BEG + 31);
OSPEVENT(QUERY_SERVERDEV_LIST_ACK,              EV_APS_SELF_BEG + 32);
OSPEVENT(QUERY_SERVERDEV_LIST_NACk,             EV_APS_SELF_BEG + 33);


/************************************************************************/
/*								登陆                    
/************************************************************************/
//ULC登陆请求消息           内容:CUlcLoginInfo
OSPEVENT( MT_APS_LOGIN_REQ,				        EV_APS_SELF_BEG + 40);

//ULC登陆请求消息成功答复   内容:CUlcFullInfo
OSPEVENT( APS_MT_LOGIN_ACK,				        EV_APS_SELF_BEG + 41 );

//ULC登陆请求消息失败答复   内容:(u32_ErrorCode)
OSPEVENT( APS_MT_LOGIN_NACK,			        EV_APS_SELF_BEG + 42 );

OSPEVENT( SEARCH_PLATFORM_BACKUP_CONFIG_REQ,	EV_APS_SELF_BEG + 43 );

#endif //end of _EVENT_APS_SELF_H_
