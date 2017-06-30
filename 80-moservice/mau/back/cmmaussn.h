/*****************************************************************************
模块名      : MAU
文件名      : cmmaussn.h
创建时间    : 2014年 08月 26日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifndef	_CMMAUSSN_INSTANCE_H_
#define	_CMMAUSSN_INSTANCE_H_

#include "osp.h"
#include "mauconst.h"
#include "mauutility.h"
#include "ztemplatedaemon.h"
#include "rabbitmqwrapper.h"

class CCMMauSsnData
{
public:
	CCMMauSsnData()
	{
	}
	
	virtual ~CCMMauSsnData()
	{
	}
	
public:
	BOOL32 Init_MQPC();
	
	static void ContentBody_MQCB( amqp_connection_state_t conn, void *pbuf, size_t len, response_type *prsp_type, char* pchTag);
	static void ContenHeaderProps_MQCB( amqp_connection_state_t conn, void *correlation_id, size_t idLen, void *replyTo, size_t replyToLen, char* pchTag );

public:
	static s32 s_nCMMauRabbitMq;
};

class CCMMauSsnInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_NORMAL,
	};
	
public:
	CCMMauSsnInstance();
	~CCMMauSsnInstance();
	
public:
	//守护实例消息处理入口
	void DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp );
	//守护实例处理初始化消息
	void DaemonProcPowerOn(const CMessage *pcMsg, CApp *pcApp);

	/**************************************** MAU->CM回复操作 **********************************/
	//模板相关回复
	void DaemonProcTemplateRsp( CMessage * const pcMsg );
	//预约会议相关回复
	void DaemonProcBookConfRsp( CMessage * const pcMsg );
	//会议相关回复
	void DaemonProcConfRsp( CMessage * const pcMsg );
	//会议列表回复
	void DaemonProcMcuListRsp( CMessage * const pcMsg );

	void DaemonProcMcuListNtf( CMessage * const pcMsg );

	void DaemonProcCmtoMauMsg( CMessage * const pcMsg );

	//会议相关信息通知
	void DaemonProcConfNtf( CMessage * const pcMsg );

	//hdu相关通知
	void DaemonProcHduInfoByMoidNtf( CMessage * const pcMsg );
	void DaemonProcAllHduInfoNtf( CMessage * const pcMsg );
	void DaemonProcHduInfoByMoidRsp(CMessage * const pcMsg);
	void DaemonProcConfInfoByMoidRsp(CMessage * const pcMsg);
	/**************************************** 普通实例**********************************/
	void InstanceEntry(  CMessage * const pcMsg );
	s32 SendMsgToCM( const s8 * const pbyMsgBody = NULL, u16  wLen = 0, const s8 * pbyRpcId = NULL, const s8 * pbyRpc = NULL );
	static void ConvertMsgTypeToEventId( s8 * pchJsonBuf, u16 &wMsgType );
	static void CompletedReqInfo(const u16 wMsgType, const s8* pchJsonBuf, TReqHeadInfo& tReqInfo);
};


typedef zTemplate<CCMMauSsnInstance, 0, CCMMauSsnData, 0> CCMMauSsnApp;
extern CCMMauSsnApp g_cCMMauSsnApp;


#endif  // _CMMAUSSN_INSTANCE_H_

