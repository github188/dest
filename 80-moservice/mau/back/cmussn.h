/*****************************************************************************
模块名      : MAU
文件名      : cmussn.h
创建时间    : 2014年 08月 26日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifndef	_CMUSSN_INSTANCE_H_
#define	_CMUSSN_INSTANCE_H_

#include "osp.h"
#include "mauconst.h"
#include "mauutility.h"
#include "mauvc.h"
#include "rabbitmqwrapper.h"

extern CMauVCApp g_cMauVCApp;

#define CONNECT_TIMEOUT         (u16)10*1000

class CCmuSsnData
{
public:
	CCmuSsnData()
	{
		m_dwCurInsId = 0;
	}

	virtual ~CCmuSsnData()
	{
	}

public:
	BOOL32 Init_MQPC();

	static void Content_MQCB( amqp_connection_state_t conn, void *buf, size_t len, response_type *rsp_type, char* pchTag );

public:
	static s32 s_nCmuRabbitMq;
	u32 m_dwCurInsId;
};

class CCMUSsnInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_WAITING,
		STATE_NORMAL,
	};
	
public:
	CCMUSsnInstance();
	~CCMUSsnInstance();
	
public:
	//守护实例消息处理入口
	//守护实例处理初始化消息
	void DaemonInstanceEntry(CMessage * const pcMsg, CApp *pcApp);
	void DaemonProcPowerOn(CMessage * const pcMsg, CApp *pcApp);

	void DaemonProcCmuInfoNtf(CMessage * const pcMsg, CApp *pcApp);
	void DaemonCmuMessageCb(CMessage * const pcMsg, CApp *pcApp);
	void InstanceEntry(CMessage * const pcMsg);
	//处理初始化消息
	void ProcPowerOn( CMessage * const pcMsg );
	void ProcCmuInfoNtf( CMessage * const pcMsg );
	void ProcHeartBeatTimer( CMessage * const pcMsg );
	void ProcGetConfListTimer( CMessage * const pcMsg );
	void ProcDisconnect(const TCmuInfo* ptCmuInfo = NULL);
	void ProcCmuLiveNtf(CMessage * const pcMsg);

	//处理CM发送的转给mcu的请求
	void ProcMsgToCmu( CMessage * const pcMsg );

	/**************************************** MCU回复操作 **********************************/
	void ProcCmuRspToMauVc( CMessage * const pcMsg );
	void ProcCmuRspToCM( CMessage * const pcMsg );

	/**************************************** MCU请求操作 **********************************/
	void ProcCmuReqToMauVc( CMessage * const pcMsg );

	/**************************************** 域/终端信息同步操作 **********************************/
	void ProcMtListNtf( CMessage * const pcMsg );
	void ProcSrvDomainNtf( CMessage * const pcMsg );
	void ProcPlatformDomainNtf( CMessage * const pcMsg );
	void ProcUserDomainNtf( CMessage * const pcMsg );

	void ProcMtListNtf( CKdvMtInfo * pcMtInfo = NULL, BOOL32 bDel = FALSE );
	void ProcDomainNtf( CKdvDomainInfo * pcDomainInfo = NULL, BOOL32 bDel = FALSE );

	void ClearInst();
	void ConnectCmu(const BOOL32 bIgnoreState = FALSE);
	s32 SendMsgToCmu(const s8 * const pbyMsgBody = NULL, u32 dwLen = 0, u32 dwTTL = 0);
	void SendMsgToCmCmuSsn(u16 wEvent, u8* const pbyMsg = NULL, u16 wLen = 0);
	void SendMsgToMauVc(u16 wEvent, u8* const pbyMsg = NULL, u16 wLen = 0);
	void SetCmuInfo(const TCmuInfo& tCmuInfo);
	void ConvertMsgTypeToEventId( s8 * pchJsonBuf, u16 &wMsgType );
public:
	u16 m_wHBFailCount;
	TCmuInfo m_tCmuInfo;
};

typedef zTemplate<CCMUSsnInstance, MAX_CMU_NUM, CCmuSsnData, MAXLEN_IPADDR> CCMUSsnApp;
extern CCMUSsnApp g_cCMUSsnApp;

#endif  // _CMUSSN_INSTANCE_H_

