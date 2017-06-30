


#ifndef _STRUCT_MQ_MSG_PARSE_H_
#define _STRUCT_MQ_MSG_PARSE_H_

#include <string.h>
#include <stdio.h>
#include "kdvtype.h"



enum MqMsgCBEvent
{
    EV_MQMSG_BEGIN                      = 99,

    //��E164�� �ص�����:CMTDevice
    EV_MQMSG_ADD_MT_DEVICE,             //100
    EV_MQMSG_ENABLE_MT_DEVICE,
    EV_MQMSG_DISENABLE_MT_DEVICE,
    EV_MQMSG_MDF_MT_DEVICE,

    //IP��ַ�� �ص�����:CIpRegion
    EV_MQMSG_ADD_IP_REGION              = 110,   
    EV_MQMSG_ENABLE_IP_REGION,
    EV_MQMSG_DISENABLE_IP_REGION,
    EV_MQMSG_MDF_IP_REGION,

    //����ն�  �ص�����:CMTDevice
    EV_MQMSG_ADD_MONITOR                = 120,   
    EV_MQMSG_ENABLE_MONITOR,
    EV_MQMSG_DISENABLE_MONITOR,
    EV_MQMSG_MDF_MONITOR,

    //�������ն� �ص�����:CMTDevice
    EV_MQMSG_ADD_THIRDPARTY              = 130,   
    EV_MQMSG_ENABLE_THIRDPARTY,
    EV_MQMSG_DISENABLE_THIRDPARTY,
    EV_MQMSG_MDF_THIRDPARTY,


    //�ص�����:CServerDevice
    EV_MQMSG_ADD_SERVER_DEVICE          = 200,   
    EV_MQMSG_DEL_SERVER_DEVICE, //�ص�����:MOID
    EV_MQMSG_ENABLE_SERVER_DEVICE,
    EV_MQMSG_DISENABLE_SERVER_DEVICE,
    EV_MQMSG_MDF_SERVER_DEVICE,


    //�ص�����:CServiceDomain
    EV_MQMSG_ADD_SERVICE_DOMAIN         = 300,   
    EV_MQMSG_DEL_SERVICE_DOMAIN, //�ص�����:MOID
    EV_MQMSG_ENABLE_SERVICE_DOMAIN,
    EV_MQMSG_DISENABLE_SERVICE_DOMAIN,
    EV_MQMSG_MDF_SERVICE_DOMAIN,

    //�ص�����:CPlatformDomain
    EV_MQMSG_ADD_PLATFORM_DOMAIN        = 400,   
    EV_MQMSG_DEL_PLATFORM_DOMAIN,   //ֻ��MOID��Ч
    EV_MQMSG_ENABLE_PLATFORM_DOMAIN,    //Ŀǰҵ�񲻴��ڴ���Ϣ
    EV_MQMSG_DISENABLE_PLATFORM_DOMAIN, //Ŀǰҵ�񲻴��ڴ���Ϣ
    EV_MQMSG_MDF_PLATFORM_DOMAIN,

	// �ص�����:CPlatformDomain
	EV_MQMSG_ADD_PLATFORM_LICENSE       = 410,//�Խ�ƽ̨��license����
	EV_MQMSG_UPDATE_PLATFORM_LICENSE    = 411,//�Խ�ƽ̨��license����

    //�ص�����:CUserDomain
    EV_MQMSG_ADD_USER_DOMAIN            = 500,   
    EV_MQMSG_DEL_USER_DOMAIN,   //�ص�����:MOID
    EV_MQMSG_ENABLE_USER_DOMAIN,
    EV_MQMSG_DISENABLE_USER_DOMAIN,
    EV_MQMSG_MDF_USER_DOMAIN,

    EV_MQMSG_END
};

typedef void (*MqMsgParseCB) ( MqMsgCBEvent emEventID, void* pMsgBody, u32 wMsgLen);


#endif  //_STRUCT_MQ_MSG_PARSE_H_


