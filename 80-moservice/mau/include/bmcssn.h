/*****************************************************************************
模块名      : MAU
文件名      : bmcssn.h
创建时间    : 2014年 08月 26日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifndef	_BMCSSN_INSTANCE_H_
#define	_BMCSSN_INSTANCE_H_

#include "osp.h"
#include "mauconst.h"
#include "mauutility.h"
#include "ztemplatedaemon.h"
#include "rabbitmqwrapper.h"
using namespace rabbitmqwrap;

#define CREATE_ACCOUNT              "createAccount"        //创建账号信息
#define EDIT_ACCOUNT                "editAccount"          //修改账号信息
#define FORBID_ACCOUNT              "forbidAccount"        //禁用账号信息
#define ENABLE_ACCOUNT              "enableAccount"        //启用账号信息
#define CREATE_SERVER_DOMAIN		"createServiceDomain"  //创建服务域
#define EDIT_SERVER_DOMAIN		    "editServiceDomain"    //修改服务域
#define FORBID_SERVER_DOMAIN		"forbidServiceDomain"  //禁用服务域
#define ENABLE_SERVER_DOMAIN		"enableServiceDomain"  //启用服务域
#define DELETE_SERVER_DOMAIN        "deleteServiceDomain"  //删除服务域
#define CREATE_USER_DOMAIN			"createUserDomain"	  //创建用户域
#define EDIT_USER_DOMAIN		    "editUserDomain"       //修改用户域
#define FORBID_USER_DOMAIN			"forbidUserDomain"     //禁用用户域
#define ENABLE_USER_DOMAIN			"enableUserDomain"     //启用用户域
#define DELETE_USER_DOMAIN          "deleteUserDomain"	  //删除服务域
#define CREATE_PLATFORM_DOMAIN		"createPlatformDomain" //创建平台域
#define EDIT_PLATFORM_DOMAIN		"editPlatformDomain"   //修改平台域
#define DELETE_PLATFORM_DOMAIN      "deletePlatformDomain" //删除服务域
#define CREATE_SERVER               "createServer"         //创建服务器
#define EDIT_SERVER                 "editServer"           //修改服务器
#define ENABLE_SERVER               "enableServer"         //启动服务器
#define FORBID_SERVER               "forbidServer"         //禁用服务器
#define DELETE_SERVER				"deleteServer"		   //删除服务器
#define BATCH_CREATE_ACCOUNT	    "batchCreateAccount"   //批量创建账号
#define UPDATE_SYSTEM_MODE          "updateSystemMode"     //更新环境是租赁还是自建

#define BMC_MESSAGE_CB  0x200     //BMC消息回调
#define SERVER_TYPE_MPS  261
#define SERVER_TYPE_XMPU 293
#define SERVER_TYPE_HDU2   341
#define SERVER_TYPE_HDU2_L 342
#define SERVER_TYPE_HDU2_S 343
#define SERVER_TYPE_HDU5   344
#define SERVER_TYPE_HDU5_T 325

class CBmcSsnData
{
public:
	CBmcSsnData()
	{
	}
	
	virtual ~CBmcSsnData()
	{
	}
	
public:
	BOOL32 Init_MQPC();

	static void Content_MQCB( amqp_connection_state_t conn, void *buf, size_t len, response_type *rsp_type , char* pchTag);

	//static void ConvertMsgTypeToEventId( s8 * pchJsonBuf, u16 &wMsgType );

	static void Consumer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc, char* pchTag );
	static void Consumer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv , char* pchTag);

public:
	static s32 s_nBmcRabbitMq;
};

class CBmcSsnInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_NORMAL,
	};
	
public:
	CBmcSsnInstance();
	~CBmcSsnInstance();
	
public:
	static void GetEventType(s8 *pstrEvent, s8 *pBuf);
	static void GetEventData(s8 *pBufData, s8 *pBuf);
	static void RegDealFun();
	static void DealCreateAccount(void *pBuf);
	static void DealEditAccount(void *pBuf);
	static void DealForbidAccount(void *pBuf);
	static void DealEnableAccount(void *pBuf);
	static void DealCreateServerDomain(void *pBuf);
	static void DealEditServerDomain(void *pBuf);
	static void DealForbidServerDomain(void *pBuf);
	static void DealEnableServerDomain(void *pBuf);
	static void DealDeleteServerDomain(void *pBuf);
	static void DealCreateUserDomain(void *pBuf);
	static void DealEditUserDomain(void *pBuf);
	static void DealForbidUserDomain(void *pBuf);
	static void DealEnableUserDomain(void *pBuf);
	static void DealDeleteUserDomain(void *pBuf);
	static void DealCreatePlatformDomain(void *pBuf);
	static void DealEditPlatformDomain(void *pBuf);
	static void DealDeletePlatformDomain(void *pBuf);
	static void DeleteDomainInfo(void *pBuf);
	static void DealCreateServer(void *pBuf);
	static void DealEditServer(void *pBuf);
	static void DealEnableServer(void *pBuf);
	static void DealForbidServer(void *pBuf);
	static void DealDeleteServer(void *pBuf);
	static void DealServerInfoNtf(void *pBuf);
	static void DealHduServerInfoNtf(void *pBuf);
	static void DealBatchCreateAccount(void *pBuf);
	static void DealUpdateSystemMode(void *pBuf);
	static BOOL32 GetServerTypeFromJson( s8 * pchJsonBuf, u16& wServerType );
	static BOOL32 GetServerDomainIdFromJson( s8 * pchJsonBuf, s8 * pchDomainMOID, u32 dwDomainMOIDLen );
	static BOOL32 GetAccountNumFromJson(const s8 * pchJsonBuf, u32& dwAccountNum);
	static BOOL32 GetAccountListFormJson(const s8 * pchJsonBuf, vector<CMauMtInfo>& vec_MauMtInfo);
	//守护实例消息处理入口
	void DaemonInstanceEntry(CMessage * const pcMsg, CApp *pcApp);
	//守护实例处理初始化消息
	void DaemonProcPowerOn(CMessage * const pcMsg, CApp *pcApp);
	void DaemonProcBmcMsgCB(CMessage * const pcMsg);
	/**************************************** 普通实例**********************************/
	void InstanceEntry(  CMessage * const pcMsg );

public:
	s8 m_achCmuQName[MAXLEN_ALIAS+1];
	typedef void (*DealFun)(void*);
	typedef std::map<std::string, DealFun> DealFunMap;
	static DealFunMap deal_fun_map;
};

typedef zTemplate<CBmcSsnInstance, 0, CBmcSsnData, 0> CBmcSsnApp;

#endif  // _BMCSSN_INSTANCE_H_
