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
	s_nCMCmuRabbitMq = SafeCreateP( Producer_ConSuc_MQCB, Producer_ConFail_MQCB );
	if ( 0 == s_nCMCmuRabbitMq )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMCMU, "[CCMCmuSsnData::Init_MQPC] s_nCMCmuRabbitMq = 0, return FALSE!\n" );
	}

	// ����������
	SafeCreateC( MQ_MAU_CMMCU_Q, MQ_MAU_CMMCU_EX, MQ_MAU_CMMCU_K, ContentBody_MQCB, ContenHeaderProps_MQCB,
		NULL, Consumer_ConSuc_MQCB, Consumer_ConFail_MQCB, 0, 0, 0, 1, g_cMauVCApp.GetMqConsumerHeartBeatTime());
	
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
�� �� ���� Producer_ConSuc_MQCB
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/03/09  5.0         rnn                   ����
=============================================================================*/
void CCMCmuSsnData::Producer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc )
{	
	g_cMauVCApp.GetMqStatus().SetCmCmuPIsCreated(TRUE);

	s8 achSucDec[MAXLEN_ALIAS] = {0};
	if ( NULL != desc )
	{
		strncpy( achSucDec, desc, sizeof(achSucDec) - 1 );
	}
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMCMU, "[CCMCmuSsnData::Producer_ConSuc_MQCB]%s.\n", achSucDec );
	
    return;
}

/*=============================================================================
�� �� ���� Producer_ConFail_MQCB
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/03/09  5.0         rnn                   ����
=============================================================================*/
void CCMCmuSsnData::Producer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv )
{
	g_cMauVCApp.GetMqStatus().SetCmCmuPIsCreated(FALSE);

	s8 achExpect[MAXLEN_ALIAS] = {0};
	s8 achRecv[MAXLEN_ALIAS] = {0};
	if ( NULL != expect  )
	{
		strncpy( achExpect, expect, sizeof(achExpect) - 1 );
	}
	if ( NULL != precv  )
	{
		strncpy( achRecv, precv, sizeof(achRecv) - 1 );
	}
	LogPrint( LOG_LVL_ERROR, MID_MAU_CMCMU, "[CCMCmuSsnData::Producer_ConFail_MQCB]expect.%s.\n", achExpect );
	LogPrint( LOG_LVL_ERROR, MID_MAU_CMCMU, "[CCMCmuSsnData::Producer_ConFail_MQCB]precv.%s.\n", achRecv );
}

/*=============================================================================
�� �� ���� Consumer_ConSuc_MQCB
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/03/09  5.0         rnn                   ����
=============================================================================*/
void CCMCmuSsnData::Consumer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc, char* pchTag )
{	
	g_cMauVCApp.GetMqStatus().SetMau_CmMcuIsCreated(TRUE);

	s8 achSucDec[MAXLEN_ALIAS] = {0};
	if ( NULL != desc )
	{
		strncpy( achSucDec, desc, sizeof(achSucDec) - 1 );
	}
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMCMU, "[CCMCmuSsnData::Consumer_ConSuc_MQCB]%s.\n", achSucDec );	
    return;
}

/*=============================================================================
�� �� ���� Consumer_ConFail_MQCB
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/03/09  5.0         rnn                   ����
=============================================================================*/
void CCMCmuSsnData::Consumer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv, char* pchTag )
{	
	g_cMauVCApp.GetMqStatus().SetMau_CmMcuIsCreated(FALSE);

	s8 achExpect[MAXLEN_ALIAS] = {0};
	s8 achRecv[MAXLEN_ALIAS] = {0};
	if ( NULL != expect  )
	{
		strncpy( achExpect, expect, sizeof(achExpect) - 1 );
	}
	if ( NULL != precv  )
	{
		strncpy( achRecv, precv, sizeof(achRecv) - 1 );
	}
	
	LogPrint( LOG_LVL_ERROR, MID_MAU_CMCMU, "[CCMCmuSsnData::Consumer_ConFail_MQCB]expect.%s.\n", achExpect );
	LogPrint( LOG_LVL_ERROR, MID_MAU_CMCMU, "[CCMCmuSsnData::Consumer_ConFail_MQCB]precv.%s.\n", achRecv );
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

