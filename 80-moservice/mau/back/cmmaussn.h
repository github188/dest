/*****************************************************************************
ģ����      : MAU
�ļ���      : cmmaussn.h
����ʱ��    : 2014�� 08�� 26��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
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
	//�ػ�ʵ����Ϣ�������
	void DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp );
	//�ػ�ʵ�������ʼ����Ϣ
	void DaemonProcPowerOn(const CMessage *pcMsg, CApp *pcApp);

	/**************************************** MAU->CM�ظ����� **********************************/
	//ģ����ػظ�
	void DaemonProcTemplateRsp( CMessage * const pcMsg );
	//ԤԼ������ػظ�
	void DaemonProcBookConfRsp( CMessage * const pcMsg );
	//������ػظ�
	void DaemonProcConfRsp( CMessage * const pcMsg );
	//�����б�ظ�
	void DaemonProcMcuListRsp( CMessage * const pcMsg );

	void DaemonProcMcuListNtf( CMessage * const pcMsg );

	void DaemonProcCmtoMauMsg( CMessage * const pcMsg );

	//���������Ϣ֪ͨ
	void DaemonProcConfNtf( CMessage * const pcMsg );

	//hdu���֪ͨ
	void DaemonProcHduInfoByMoidNtf( CMessage * const pcMsg );
	void DaemonProcAllHduInfoNtf( CMessage * const pcMsg );
	void DaemonProcHduInfoByMoidRsp(CMessage * const pcMsg);
	void DaemonProcConfInfoByMoidRsp(CMessage * const pcMsg);
	/**************************************** ��ͨʵ��**********************************/
	void InstanceEntry(  CMessage * const pcMsg );
	s32 SendMsgToCM( const s8 * const pbyMsgBody = NULL, u16  wLen = 0, const s8 * pbyRpcId = NULL, const s8 * pbyRpc = NULL );
	static void ConvertMsgTypeToEventId( s8 * pchJsonBuf, u16 &wMsgType );
	static void CompletedReqInfo(const u16 wMsgType, const s8* pchJsonBuf, TReqHeadInfo& tReqInfo);
};


typedef zTemplate<CCMMauSsnInstance, 0, CCMMauSsnData, 0> CCMMauSsnApp;
extern CCMMauSsnApp g_cCMMauSsnApp;


#endif  // _CMMAUSSN_INSTANCE_H_

