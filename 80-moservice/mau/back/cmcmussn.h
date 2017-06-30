/*****************************************************************************
模块名      : MAU
文件名      : cmcmussn.h
创建时间    : 2014年 08月 26日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifndef	_CMCMUSSN_INSTANCE_H_
#define	_CMCMUSSN_INSTANCE_H_

#include "osp.h"
#include "mauconst.h"
#include "mauutility.h"
#include "ztemplatedaemon.h"
#include "rabbitmqwrapper.h"
using namespace rabbitmqwrap;

class CCMCmuSsnData
{
public:
	CCMCmuSsnData()
	{
	}
	
	virtual ~CCMCmuSsnData()
	{
	}
	
public:
	BOOL32 Init_MQPC();

	static void ContentBody_MQCB( amqp_connection_state_t conn, void *pbuf, size_t len, response_type *prsp_type, char* pchTag );
	static void ContenHeaderProps_MQCB( amqp_connection_state_t conn, void *correlation_id, size_t idLen, void *replyTo, size_t replyToLen, char* pchTag );
	
public:
	static s32 s_nCMCmuRabbitMq;
};

class CCMCmuSsnInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_NORMAL,
	};
	
public:
	CCMCmuSsnInstance();
	~CCMCmuSsnInstance();
	
public:
	//守护实例消息处理入口
	void DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp );
	//守护实例处理初始化消息
	void DaemonProcPowerOn(const CMessage *pcMsg, CApp *pcApp);

	/**************************************** CM->MCU会议相关操作 **********************************/
	//会议哑音
	void DaemonProcAllMtMuteReq( s8 * pchJsonBuf );
	//会议静音
	void DaemonProcAllMtSilenceReq( s8 * pchJsonBuf );
	//取消会议哑音
	void DaemonProcCancelAllMtMuteReq( s8 * pchJsonBuf );
	//取消会议静音
	void DaemonProcCancelAllMtSilenceReq( s8 * pchJsonBuf );

	/**************************************** MCU->CM回复操作 **********************************/
	void DaemonProcMcuRspOpr( CMessage * const pcMsg );

	void DaemonProcMqMsgCB( CMessage * const pcMsg );
	/**************************************** 普通实例**********************************/
	void InstanceEntry(  CMessage * const pcMsg );

	s32 SendMsgToCM( const s8 * const pbyMsgBody = NULL, u16  wLen = 0, const s8 * pbyRpcId = NULL, const s8 * pbyRpc = NULL );

public:
	s8 m_achCmuQName[MAXLEN_ALIAS+1];
};


typedef zTemplate<CCMCmuSsnInstance, 0, CCMCmuSsnData, 0> CCMCmuSsnApp;

#endif  // _CMCMUSSN_INSTANCE_H_

