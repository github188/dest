


#ifndef _STRUCT_MQ_MSG_PARSE_H_
#define _STRUCT_MQ_MSG_PARSE_H_

#include <string.h>
#include <stdio.h>
#include "kdvtype.h"



enum MqMsgCBEvent
{
    EV_MQMSG_BEGIN                      = 99,

    //纯E164号 回调内容:CMTDevice
    EV_MQMSG_ADD_MT_DEVICE,             //100
    EV_MQMSG_ENABLE_MT_DEVICE,
    EV_MQMSG_DISENABLE_MT_DEVICE,
    EV_MQMSG_MDF_MT_DEVICE,

    //IP地址段 回调内容:CIpRegion
    EV_MQMSG_ADD_IP_REGION              = 110,   
    EV_MQMSG_ENABLE_IP_REGION,
    EV_MQMSG_DISENABLE_IP_REGION,
    EV_MQMSG_MDF_IP_REGION,

    //监控终端  回调内容:CMTDevice
    EV_MQMSG_ADD_MONITOR                = 120,   
    EV_MQMSG_ENABLE_MONITOR,
    EV_MQMSG_DISENABLE_MONITOR,
    EV_MQMSG_MDF_MONITOR,

    //第三方终端 回调内容:CMTDevice
    EV_MQMSG_ADD_THIRDPARTY              = 130,   
    EV_MQMSG_ENABLE_THIRDPARTY,
    EV_MQMSG_DISENABLE_THIRDPARTY,
    EV_MQMSG_MDF_THIRDPARTY,


    //回调内容:CServerDevice
    EV_MQMSG_ADD_SERVER_DEVICE          = 200,   
    EV_MQMSG_DEL_SERVER_DEVICE, //回调内容:MOID
    EV_MQMSG_ENABLE_SERVER_DEVICE,
    EV_MQMSG_DISENABLE_SERVER_DEVICE,
    EV_MQMSG_MDF_SERVER_DEVICE,


    //回调内容:CServiceDomain
    EV_MQMSG_ADD_SERVICE_DOMAIN         = 300,   
    EV_MQMSG_DEL_SERVICE_DOMAIN, //回调内容:MOID
    EV_MQMSG_ENABLE_SERVICE_DOMAIN,
    EV_MQMSG_DISENABLE_SERVICE_DOMAIN,
    EV_MQMSG_MDF_SERVICE_DOMAIN,

    //回调内容:CPlatformDomain
    EV_MQMSG_ADD_PLATFORM_DOMAIN        = 400,   
    EV_MQMSG_DEL_PLATFORM_DOMAIN,   //只有MOID有效
    EV_MQMSG_ENABLE_PLATFORM_DOMAIN,    //目前业务不存在此消息
    EV_MQMSG_DISENABLE_PLATFORM_DOMAIN, //目前业务不存在此消息
    EV_MQMSG_MDF_PLATFORM_DOMAIN,

	// 回调内容:CPlatformDomain
	EV_MQMSG_ADD_PLATFORM_LICENSE       = 410,//自建平台的license增加
	EV_MQMSG_UPDATE_PLATFORM_LICENSE    = 411,//自建平台的license更新

    //回调内容:CUserDomain
    EV_MQMSG_ADD_USER_DOMAIN            = 500,   
    EV_MQMSG_DEL_USER_DOMAIN,   //回调内容:MOID
    EV_MQMSG_ENABLE_USER_DOMAIN,
    EV_MQMSG_DISENABLE_USER_DOMAIN,
    EV_MQMSG_MDF_USER_DOMAIN,

    EV_MQMSG_END
};

typedef void (*MqMsgParseCB) ( MqMsgCBEvent emEventID, void* pMsgBody, u32 wMsgLen);


#endif  //_STRUCT_MQ_MSG_PARSE_H_


