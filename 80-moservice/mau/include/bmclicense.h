/*****************************************************************************
模块名      : MAU
文件名      : bmclicense.h
创建时间    : 2015年 08月 26日
实现功能    : 
作者        : 吕冠娇
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/08/25  5.0        吕冠娇         创建
******************************************************************************/
#ifndef	_BMCLICENSE_INSTANCE_H_
#define	_BMCLICENSE_INSTANCE_H_

#include "osp.h"
#include "mauconst.h"
#include "mauutility.h"
#include "rabbitmqwrapper.h"
using namespace rabbitmqwrap;

#define BMC_LICENSE_MESSAGE_CB		0x300                             //BMC license消息回调
#define BMC_LICENSE_CONSUMER_SUC    0x301                             //BMC license消费者创建成功
#define BMC_LICENSE_ASK_CB	    	0x302                             //其他程序请求bmclicense
#define BMC_LICENSE_UPDATE_CB       0x303                             //核心域license更新

class CBmcLicenseData
{
public:
	CBmcLicenseData(){}
	virtual ~CBmcLicenseData(){}
	
public:
	BOOL32 Init_MQPC();


	static void Content_TopLevelMQCB( amqp_connection_state_t conn, void *buf, size_t len, response_type *rsp_type, char* pchTag );
	static void TopLevelProducer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc );
	static void TopLevelProducer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv);
	static void TopLevelConsumer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc, char* pchTag );
	static void TopLevelConsumer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv , char* pchTag);

	//核心域license更新推送
	static void Content_TopLevelLicenseUpdateMQCB( amqp_connection_state_t conn, void *buf, size_t len, response_type *rsp_type , char* pchTag);
	static void TopLevelLicenseUpdateConsumer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc , char* pchTag);
	static void TopLevelLicenseUpdateConsumer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv , char* pchTag);

	static void Content_PlatformMQCB( amqp_connection_state_t conn, void *buf, size_t len, response_type *rsp_type , char* pchTag);
	static void PlatformProducer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc );
	static void PlatformProducer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv );
	static void PlatformConsumer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc , char* pchTag);
	static void PlatformConsumer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv , char* pchTag);

	void SendLicenseReqMsgToBMC();
	void PrintBmcLicenseInfo();
public:
	static s32 s_nBmcLicenseRabbitMq;
	static std::string s_strPlatformRoutingKey;
	static std::string s_strPlatformQName;
	static std::string s_strPlatformBmcLicenseKey;

	static s32 s_nTopLevelDomainRabbitMq;
	static std::string s_strTopLevelRoutingKey;
	static std::string s_strTopLevelQName;
	std::string m_strLicenseInfo;
};

class CBmcLicenseInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_NORMAL,
	};
	
public:
	CBmcLicenseInstance();
	~CBmcLicenseInstance();
	
	//守护实例消息处理入口
	void DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp );
	//守护实例处理初始化消息
	void DaemonProcPowerOn( CMessage * const pcMsg );
	void DaemonProcBmcLicenseMsgCB( CMessage * const pcMsg );
	void DaemonProcBmcLicenseAskCB(CMessage * const pcMsg);
	void DaemonProcBmcLicenseUpdateCB(CMessage * const pcMsg);

	/**************************************** 普通实例**********************************/
	void InstanceEntry(  CMessage * const pcMsg );

private:
	void RequestBmcLicense();
	void HandleLicenseStatus();

private:
	BOOL32 m_bIsRcvLisence;
};


typedef zTemplate<CBmcLicenseInstance, 1, CBmcLicenseData> CBmcLicenseApp;
extern CBmcLicenseApp g_cBmcLicenseApp;


#endif  // _BMCLICENSE_INSTANCE_H_
