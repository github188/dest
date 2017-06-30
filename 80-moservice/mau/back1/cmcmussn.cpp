/*****************************************************************************
ģ����      : MAU
�ļ���      : cmcmussn.cpp
����ʱ��    : 2014�� 08�� 26��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
******************************************************************************/
#ifdef WIN32
#pragma warning( disable:4786 )
#endif

#include "inc.h"
#include "cmcmussn.h"
#include "kdvlog.h"
#include "mauutility.h"
#include "evmau.h"
#include "mauvc.h"

s32 CCMCmuSsnData::s_nCMCmuRabbitMq = 0;
static s8  g_achRpcReplyTo[MAXLEN_ALIAS+1] = { 0 };
static s8  g_achRpcCorrelationId[MAXLEN_ALIAS+1] = { 0 };

extern CMauVCApp g_cMauVCApp;

/*=============================================================================
�� �� ���� Init_MQPC
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/09/24  5.0         rnn                   ����
=============================================================================*/
BOOL32 CCMCmuSsnData::Init_MQPC()
{
	// ����������
	s_nCMCmuRabbitMq = SafeCreateP( ProducerSuccCB, ProducerFailCB );
	if ( 0 == s_nCMCmuRabbitMq )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMCMU, "[CCMCmuSsnData::Init_MQPC] s_nCMCmuRabbitMq = 0, return FALSE!\n" );
	}

	// ����������
	SafeCreateC( MQ_MAU_CMMCU_Q, MQ_MAU_CMMCU_EX, MQ_MAU_CMMCU_K, ContentBody_MQCB, ContenHeaderProps_MQCB,
		NULL, ConsumerSuccCB, ConsumerFailCB, 0 );
	
	return TRUE;
}

/*=============================================================================
�� �� ���� ContenHeaderProps_MQCB
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
void CCMCmuSsnData::ContenHeaderProps_MQCB( amqp_connection_state_t conn, void *correlation_id, size_t idLen, void *replyTo, size_t replyToLen, char* pchTag )
{
	if ( NULL == correlation_id || NULL == replyTo )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMCMU, "[CCMCmuSsnData::ContenHeaderProps_MQCB]correlation_id or rpcto is NULL, return!\n" );
		return;
	}
	
	s32 nRpcIdLen = ( MAXLEN_ALIAS > idLen ) ? idLen : MAXLEN_ALIAS;
	memset( g_achRpcCorrelationId, 0, sizeof( g_achRpcCorrelationId ) );		
	strncpy( g_achRpcCorrelationId, (s8*)correlation_id, nRpcIdLen );
	
	s32 nRpcLen = ( MAXLEN_ALIAS > replyToLen ) ? replyToLen : MAXLEN_ALIAS;
	memset( g_achRpcReplyTo, 0, sizeof( g_achRpcReplyTo ) );
	strncpy( g_achRpcReplyTo, (s8*)replyTo, nRpcLen );
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMCMU, "[CCMCmuSsnData::ContenHeaderProps_MQCB]Replay rpc id is %s, content is %s!\n", g_achRpcCorrelationId, g_achRpcReplyTo );
	
	return;
}

/*=============================================================================
�� �� ���� ContentBody_MQCB
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/09/24  5.0         rnn                   ����
=============================================================================*/
void CCMCmuSsnData::ContentBody_MQCB( amqp_connection_state_t conn, void *pbuf, size_t len, response_type *prsp_type, char* pchTag)
{
	if ( NULL == pbuf )
	{
		*prsp_type = RT_NACK;
		return;
	}

	*prsp_type = RT_ACK;

	u8 byEnd = 0;
	CMauMsg cSrvmsg;
	cSrvmsg.SetMsgBody(g_achRpcReplyTo, sizeof(g_achRpcReplyTo));
	cSrvmsg.CatMsgBody(g_achRpcCorrelationId, sizeof(g_achRpcCorrelationId));
	cSrvmsg.CatMsgBody(pbuf, len);
	cSrvmsg.CatMsgBody(&byEnd, sizeof(byEnd));

	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), CM_CMU_MESSAGE_CB, cSrvmsg.GetServMsg(), cSrvmsg.GetServMsgLen());

	return;
}

/*=============================================================================
�� �� ���� CCMCmuSsnInstance
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
CCMCmuSsnInstance::CCMCmuSsnInstance()
{
	memset( m_achCmuQName, 0, sizeof( m_achCmuQName ) );
}


/*=============================================================================
�� �� ���� ~CCMCmuSsnInstance
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
CCMCmuSsnInstance::~CCMCmuSsnInstance()
{
	memset( m_achCmuQName, 0, sizeof( m_achCmuQName ) );
}

/*=============================================================================
�� �� ���� DaemonInstanceEntry
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
void CCMCmuSsnInstance::DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp )
{
	switch ( pcMsg->event )
	{
	case OSP_POWERON:
		DaemonProcPowerOn(pcMsg, pcApp);
		break;

	case CMU_CM_CONFOPR_NTF:
		DaemonProcMcuRspOpr( pcMsg );
		break;

	default:
		break;
	}

	return;
}

/*=============================================================================
�� �� ���� DaemonProcPowerOn
��    �ܣ� �ػ�ʵ�������ʼ����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
void CCMCmuSsnInstance::DaemonProcPowerOn(const CMessage *pcMsg, CApp *pcApp)
{
	//״̬��ʼ��
	NextState( STATE_IDLE );
	dynamic_cast<CCMCmuSsnApp*>(pcApp)->Init_MQPC();
}

/*=============================================================================
�� �� ���� InstanceEntry
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
void CCMCmuSsnInstance::InstanceEntry(  CMessage * const pcMsg )
{
	
}


/*=============================================================================
�� �� ���� SendMsgToCM
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
s32 CCMCmuSsnInstance::SendMsgToCM( const s8 * const pbyMsgBody /* = NULL */, u16 wLen /* = 0 */, const s8 * pbyRpcId /*= NULL*/, const s8 * pbyRpc /* = NULL  */)
{
	s32 nRet = 0;

	//RPC_MODE
	if ( NULL != pbyRpc && NULL != pbyRpcId )
	{
		nRet = PostMsg(CCMCmuSsnApp::s_nCMCmuRabbitMq, pbyMsgBody, wLen, pbyRpc, NULL, 0, RPC_MODE, pbyRpcId, pbyRpc );
		LogPrint( LOG_LVL_DETAIL, MID_MAU_CMCMU, "[CCMCmuSsnInstance::SendMsgToCM]Rpc<%s> RpcId<%s> Ret.%d\n", pbyRpc, pbyRpcId, nRet );
	}
	else
	{
		nRet = PostMsg(CCMCmuSsnApp::s_nCMCmuRabbitMq, pbyMsgBody, wLen, MQ_MAU_CMMCU_REPLY_RK, MQ_MAU_CMMCU_REPLY_EX );
		LogPrint( LOG_LVL_DETAIL, MID_MAU_CMCMU, "[CCMCmuSsnInstance::SendMsgToCM]Ex<%s> Key<%s> Ret.%d\n", MQ_MAU_CMMCU_REPLY_EX, MQ_MAU_CMMCU_REPLY_RK, nRet );
	}
	return nRet;
}

/*=============================================================================
�� �� ���� DaemonProcMcuRspOpr
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/09/28  5.0         rnn                    ����
=============================================================================*/
void CCMCmuSsnInstance::DaemonProcMcuRspOpr( CMessage * const pcMsg )
{
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );

	const s8* achJsonBuf = (const s8*) cServMsg.GetMsgBody();	

	u32 dwJsonBufLen = cServMsg.GetMsgBodyLen();

	TReqHeadInfo tReqHead;
	GetReqHeadFromJson( achJsonBuf, tReqHead );
	
	if (0 != strlen(tReqHead.GetMQName()) && 0 != strlen(tReqHead.GetMQCorrelationId()))
	{
		SendMsgToCM(achJsonBuf, dwJsonBufLen, tReqHead.GetMQCorrelationId(), tReqHead.GetMQName());
	}
	else
	{
		SendMsgToCM(achJsonBuf, dwJsonBufLen);
	}

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMCMU, "[CCMCmuSsnInstance::DaemonProcMcuRspOpr]conf info json len is %d, text is:.\n", dwJsonBufLen);
	LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMCMU, achJsonBuf, dwJsonBufLen);
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMCMU, "[CCMCmuSsnInstance::DaemonProcMcuRspOpr]end\n" );

	return;
}

