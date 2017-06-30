/*****************************************************************************
ģ����      : MAU
�ļ���      : cmcmussn.h
����ʱ��    : 2014�� 08�� 26��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
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
	//�ػ�ʵ����Ϣ�������
	void DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp );
	//�ػ�ʵ�������ʼ����Ϣ
	void DaemonProcPowerOn(const CMessage *pcMsg, CApp *pcApp);

	/**************************************** CM->MCU������ز��� **********************************/
	//��������
	void DaemonProcAllMtMuteReq( s8 * pchJsonBuf );
	//���龲��
	void DaemonProcAllMtSilenceReq( s8 * pchJsonBuf );
	//ȡ����������
	void DaemonProcCancelAllMtMuteReq( s8 * pchJsonBuf );
	//ȡ�����龲��
	void DaemonProcCancelAllMtSilenceReq( s8 * pchJsonBuf );

	/**************************************** MCU->CM�ظ����� **********************************/
	void DaemonProcMcuRspOpr( CMessage * const pcMsg );

	void DaemonProcMqMsgCB( CMessage * const pcMsg );
	/**************************************** ��ͨʵ��**********************************/
	void InstanceEntry(  CMessage * const pcMsg );

	s32 SendMsgToCM( const s8 * const pbyMsgBody = NULL, u16  wLen = 0, const s8 * pbyRpcId = NULL, const s8 * pbyRpc = NULL );

public:
	s8 m_achCmuQName[MAXLEN_ALIAS+1];
};


typedef zTemplate<CCMCmuSsnInstance, 0, CCMCmuSsnData, 0> CCMCmuSsnApp;

#endif  // _CMCMUSSN_INSTANCE_H_

