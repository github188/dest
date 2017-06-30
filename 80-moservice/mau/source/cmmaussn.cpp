/*****************************************************************************
模块名      : MAU
文件名      : cmmaussn.cpp
创建时间    : 2014年 08月 26日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifdef WIN32
#pragma warning( disable:4786 )
#endif

#include "inc.h"
#include "cmmaussn.h"
#include "kdvlog.h"
#include "mauutility.h"
#include "evmau.h"
#include "mauvc.h"
#include "rabbitmqwrapper.h"
using namespace rabbitmqwrap;

extern CMauVCApp g_cMauVCApp;

s32 CCMMauSsnData::s_nCMMauRabbitMq = 0;
static s8  g_achRpc[MAXLEN_ALIAS+1] = { 0 };
static s8  g_achRpcId[MAXLEN_ALIAS+1] = { 0 };

/*=============================================================================
函 数 名： Init_MQPC
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CCMMauSsnData::Init_MQPC()
{
	// 创建生产者
	s_nCMMauRabbitMq = SafeCreateP( Producer_ConSuc_MQCB, Producer_ConFail_MQCB );
	if ( 0 == s_nCMMauRabbitMq )
	{
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[CCMMauSsnData::Init_MQPC] s_nCMMauRabbitMq = 0, return FALSE!\n" );
	}

	// 创建消费者
	SafeCreateC( MQ_MAU_CMMAU_Q, MQ_MAU_CMMAU_EX, MQ_MAU_CMMAU_K, ContentBody_MQCB, ContenHeaderProps_MQCB, NULL,
		Consumer_ConSuc_MQCB, Consumer_ConFail_MQCB, 0, 0, 0, 1, g_cMauVCApp.GetMqConsumerHeartBeatTime());

	return TRUE;
}

/*=============================================================================
函 数 名： Producer_ConSuc_MQCB
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/03/09  5.0         rnn                   创建
=============================================================================*/
void CCMMauSsnData::Producer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc )
{	
	g_cMauVCApp.GetMqStatus().SetCmMauPIsCreated(TRUE);

	s8 achSucDec[MAXLEN_ALIAS] = {0};
	if ( NULL != desc )
	{
		strncpy( achSucDec, desc, sizeof(achSucDec) - 1 );
	}
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[CCMMauSsnData::Producer_ConSuc_MQCB]%s.\n", achSucDec );
}

/*=============================================================================
函 数 名： Producer_ConFail_MQCB
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/03/09  5.0         rnn                   创建
=============================================================================*/
void CCMMauSsnData::Producer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv )
{
	g_cMauVCApp.GetMqStatus().SetCmMauPIsCreated(FALSE);

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
	LogPrint( LOG_LVL_ERROR, MID_MAU_CMMAU, "[CCMMauSsnData::Producer_ConFail_MQCB]expect.%s.\n", achExpect );
	LogPrint( LOG_LVL_ERROR, MID_MAU_CMMAU, "[CCMMauSsnData::Producer_ConFail_MQCB]precv.%s.\n", achRecv );
}

/*=============================================================================
函 数 名： Consumer_ConSuc_MQCB
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/03/09  5.0         rnn                   创建
=============================================================================*/
void CCMMauSsnData::Consumer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc, char* pchTag )
{	
	g_cMauVCApp.GetMqStatus().SetMau_CmMauIsCreated(TRUE);

	s8 achSucDec[MAXLEN_ALIAS] = {0};
	if ( NULL != desc )
	{
		strncpy( achSucDec, desc, sizeof(achSucDec) - 1 );
	}
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[CCMMauSsnData::Consumer_ConSuc_MQCB]%s.\n", achSucDec );	
    return;
}

/*=============================================================================
函 数 名： Consumer_ConFail_MQCB
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/03/09  5.0         rnn                   创建
=============================================================================*/
void CCMMauSsnData::Consumer_ConFail_MQCB(amqp_connection_state_t conn, const char *expect, const char *precv, char* pchTag)
{	
	g_cMauVCApp.GetMqStatus().SetMau_CmMauIsCreated(FALSE);

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
	
	LogPrint( LOG_LVL_ERROR, MID_MAU_CMMAU, "[CCMMauSsnData::Consumer_ConFail_MQCB]expect.%s.\n", achExpect );
	LogPrint( LOG_LVL_ERROR, MID_MAU_CMMAU, "[CCMMauSsnData::Consumer_ConFail_MQCB]precv.%s.\n", achRecv );
}

/*=============================================================================
函 数 名： ContenHeaderProps_MQCB
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CCMMauSsnData::ContenHeaderProps_MQCB( amqp_connection_state_t conn, void *correlation_id, size_t idLen, void *replyTo, size_t replyToLen, char* pchTag )
{
	if ( NULL == correlation_id || NULL == replyTo )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMMAU, "[CCMCmuSsnData::ContenHeaderProps_MQCB]correlation_id or rpcto is NULL, return!\n" );
		return;
	}
	
	s32 nRpcIdLen = ( MAXLEN_ALIAS > idLen ) ? idLen : MAXLEN_ALIAS;
	memset( g_achRpcId, 0, sizeof( g_achRpcId ) );		
	strncpy( g_achRpcId, (s8*)correlation_id, nRpcIdLen );
	
	s32 nRpcLen = ( MAXLEN_ALIAS > replyToLen ) ? replyToLen : MAXLEN_ALIAS;
	memset( g_achRpc, 0, sizeof( g_achRpc ) );
	strncpy( g_achRpc, (s8*)replyTo, nRpcLen );
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[CCMCmuSsnData::ContenHeaderProps_MQCB]Replay rpc id is %s, content is %s!\n", g_achRpcId, g_achRpc );
	
	return;
}

/*=============================================================================
函 数 名： ContentBody_MQCB
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CCMMauSsnData::ContentBody_MQCB( amqp_connection_state_t conn, void *pbuf, size_t len, response_type *prsp_type, char* pchTag )
{
	if ( NULL == pbuf )	
	{
		*prsp_type = RT_NACK;
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMMAU, "[ContentBody_MQCB]Buf is NULL, return!\n" );
		return;
	}

	*prsp_type = RT_ACK;
	
	OspPost(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON), CM_MAU_MESSAGE_CB, pbuf, len);
}

/*=============================================================================
函 数 名： ConvertMsgTypeToEventId
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/24  5.0         rnn                   创建
2016/04/07  5.0			王力				  增加个人模板相关event
=============================================================================*/
void CCMMauSsnInstance::ConvertMsgTypeToEventId( s8 * pchJsonBuf, u16 &wMsgType )
{
	if ( NULL == pchJsonBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMMAU, "[ConvertMsgTypeToEventId]Param is NULL, return!\n" );
		return;
	}
	
	s8 achMsgType[MAXLEN_MSGTYPE+1] = {0};
	u32 dwMsgTypeLen = MAXLEN_MSGTYPE;
	
	if ( !GetMsgTypeFromJson( pchJsonBuf, achMsgType, dwMsgTypeLen ) )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMMAU, "[ConvertMsgTypeToEventId]GetMsgTypeFromJson err, return!\n" );
		return;
	}

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[ConvertMsgTypeToEventId]Rcv msg.%s from CM!\n", achMsgType );
	
#define MSG2EVENT(event) if (0 == strcmp( #event, achMsgType)) { wMsgType = event;	return;}

	//模板相关
	MSG2EVENT(CM_MAU_CREATETEMPLATE_REQ);
	MSG2EVENT(CM_MAU_MDFTEMPLATE_REQ);
	MSG2EVENT(CM_MAU_DELTEMPLATE_REQ);
	//个人模板相关
	MSG2EVENT(CM_MAU_PERSONALCONFTEMPLATE_NTF);
	MSG2EVENT(CM_MAU_DELPERSONALCONFTEMPLATE_NTF);
	MSG2EVENT(CM_MAU_ALLPERSONALCONFTEMPLATE_ACK);

	//预约会议相关
	MSG2EVENT(CM_MAU_ADDBOOKCONF_REQ);
	MSG2EVENT(CM_MAU_MDFBOOKCONF_REQ);
	MSG2EVENT(CM_MAU_ADDBOOKCONF_REQ);
	MSG2EVENT(CM_MAU_DELBOOKCONF_REQ);
	MSG2EVENT(CM_MAU_BOOKCONFONGOING_NTF);
	//会议相关
	MSG2EVENT(CM_MAU_CREATECONF_REQ);
	MSG2EVENT(CM_MAU_CREATECONFBYTEMP_REQ);
	MSG2EVENT(CM_MAU_RELEASECONF_REQ);
	MSG2EVENT(CM_MAU_GETCONFLIST_CMD);
	MSG2EVENT(CM_MAU_GETCONFLIST_REQ);

	MSG2EVENT(CM_MAU_DELAYCONF_REQ);
	//电视墙相关
	MSG2EVENT(CM_MAU_GETHDUINFO_CMD);
	MSG2EVENT(CM_MAU_SETHDUVOLUME_REQ);
	MSG2EVENT(CM_MAU_GETHDUINFOBYMOID_REQ);
	//获取利旧MCU列表
	MSG2EVENT(CM_MAU_GETMCULIST_REQ);
	//监控心跳
	MSG2EVENT(CM_MAU_MONITORKEEPALIVE_NTF);
}

void CCMMauSsnInstance::CompletedReqInfo(const u16 wMsgType, 
	const s8* pchJsonBuf, TReqHeadInfo& tReqInfo)
{
	if (wMsgType == CM_MAU_CREATECONFBYTEMP_REQ)
	{
		s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
		GetConfE164FromJson(pchJsonBuf, achConfE164, MAXLEN_CONFE164);
		tReqInfo.SetConfE164(achConfE164);

		//rnn[2014/12/02]此时用tReqInfo中的m_dwMtReqID字段标识meetingid
		u32 nMeetingId = 0;
		CMauData::GetMeetingIdFromJson((s8*)pchJsonBuf, nMeetingId);
		tReqInfo.SetMtReqID(nMeetingId);

		s8 achCreatorAlias[MAXLEN_ALIAS + 1] = { 0 };
		CMauData::GetConfCreatorNameFromJson((s8*)pchJsonBuf, achCreatorAlias, MAXLEN_ALIAS);
		tReqInfo.SetMtAlias(achCreatorAlias);

		s8 achCreatorMoid[OPERATION_MAXLEN_MOID + 1] = { 0 };
		CMauData::GetConfCreatorMoidFromJson((s8*)pchJsonBuf, achCreatorMoid, OPERATION_MAXLEN_MOID);
		tReqInfo.SetMtMOID(achCreatorMoid);
	}
}

/*=============================================================================
函 数 名： CCMMauSsnInstance
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
CCMMauSsnInstance::CCMMauSsnInstance()
{
}


/*=============================================================================
函 数 名： ~CCMMauSsnInstance
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
CCMMauSsnInstance::~CCMMauSsnInstance()
{
}

/*=============================================================================
函 数 名： DaemonInstanceEntry
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CCMMauSsnInstance::DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp )
{
	if ( NULL == pcMsg || NULL == pcApp )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMMAU, "[CCMMauSsnInstance::DaemonInstanceEntry]Param is NULL!\n" );
		return;
	}

	switch ( pcMsg->event )
	{
	case OSP_POWERON:
		DaemonProcPowerOn(pcMsg, pcApp);
		break;

	case CM_MAU_MESSAGE_CB:
		DaemonProcCmtoMauMsg(pcMsg);
		break;
	case MAU_CM_ALLPERSONALCONFTEMPLATE_REQ:
		{
			PostMsg(g_cCMMauSsnApp.s_nCMMauRabbitMq, (const char *)(pcMsg->content), pcMsg->length, MQ_CM_CSSCM_K, MQ_CM_CSSCM_EX, 0, 0, NULL, NULL, PERSONALTMPACK_TIME_SPAN);
		}
		break;
	case MAU_CM_CREATETEMPLATE_ACK:
	case MAU_CM_CREATETEMPLATE_NACK:		
	case MAU_CM_MDFTEMPLATE_ACK:
	case MAU_CM_MDFTEMPLATE_NACK:
	case MAU_CM_DELTEMPLATE_ACK:
	case MAU_CM_DELTEMPLATE_NACK:
		DaemonProcTemplateRsp( pcMsg );
		break;
		
	case MAU_CM_ADDBOOKCONF_ACK:
	case MAU_CM_ADDBOOKCONF_NACK:		
	case MAU_CM_MDFBOOKCONF_ACK:
	case MAU_CM_MDFBOOKCONF_NACK:
	case MAU_CM_DELBOOKCONF_ACK:
	case MAU_CM_DELBOOKCONF_NACK:
		DaemonProcBookConfRsp( pcMsg );
		break;
		
	case MCU_MAU_CREATECONF_ACK:
	case MCU_MAU_CREATECONF_NACK:
	case MAU_CM_CREATECONF_ACK:
	case MAU_CM_CREATECONF_NACK:
	case MAU_CM_CREATECONFBYTEMP_ACK:
	case MAU_CM_CREATECONFBYTEMP_NACK:
	case MAU_CM_RELEASECONF_ACK:
	case MAU_CM_RELEASECONF_NACK:
	case MAU_CM_DELAYCONF_ACK:
	case MAU_CM_DELAYCONF_NACK:
	case MAU_CM_SETHDUVOLUME_ACK:
	case MAU_CM_SETHDUVOLUME_NACK:
		DaemonProcConfRsp( pcMsg );
		break;
	case MAU_CM_GETMCULIST_ACK:
	case MAU_CM_GETMCULIST_NACK:
		DaemonProcMcuListRsp( pcMsg );
		break;
	case MAU_CM_CONFLIST_NTF:
	case MAU_CM_RELEASECONF_NTF:
		DaemonProcConfNtf( pcMsg );
		break;
	case MAU_CM_HDULISTBYMOID_NTF:
		DaemonProcHduInfoByMoidNtf( pcMsg );
		break;

	case MAU_CM_ALLHDULIST_NTF:
		DaemonProcAllHduInfoNtf( pcMsg );
		break;
	case MAU_CM_GETHDUINFOBYMOID_ACK:
	case MAU_CM_GETHDUINFOBYMOID_NACK:
		DaemonProcHduInfoByMoidRsp(pcMsg);
		break;

	case MAU_CM_GETCONFLIST_ACK:
	case MAU_CM_GETCONFLIST_NACK:
		DaemonProcConfInfoByMoidRsp(pcMsg);
		break;
	default:
		break;
	}
	 
	return;
}

/*=============================================================================
函 数 名： DaemonProcPowerOn
功    能： 守护实例处理初始化消息
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CCMMauSsnInstance::DaemonProcPowerOn(const CMessage *pcMsg, CApp *pcApp)
{
	CCMMauSsnApp* pcCMMauApp = (CCMMauSsnApp*)pcApp;
	//状态初始化
	NextState(STATE_IDLE);
	pcCMMauApp->Init_MQPC();
}

/*=============================================================================
函 数 名： InstanceEntry
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CCMMauSsnInstance::InstanceEntry(  CMessage * const pcMsg )
{
	return;
}

/*=============================================================================
函 数 名： SendMsgToCM
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
s32 CCMMauSsnInstance::SendMsgToCM( const s8 * const pbyMsgBody /* = NULL */, u16 wLen /* = 0 */, const s8 * pbyRpcId /*= NULL*/, const s8 * pbyRpc /* = NULL  */)
{
	s32 nRet = 0;
	//RPC_MODE
	if ( NULL != pbyRpc && NULL != pbyRpcId )
	{
		nRet = PostMsg(CCMMauSsnData::s_nCMMauRabbitMq, pbyMsgBody, wLen, pbyRpc, NULL, 0, RPC_MODE, pbyRpcId, pbyRpc );
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[CCMMauSsnInstance::SendMsgToCM]Rpc<%s> RpcId<%s> Ret.%d\n", pbyRpc, pbyRpcId, nRet );
	}
	else
	{
		nRet = PostMsg(CCMMauSsnData::s_nCMMauRabbitMq, pbyMsgBody, wLen, MQ_MAU_CMMCU_REPLY_RK, MQ_MAU_CMMCU_REPLY_EX);
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[CCMCmuSsnInstance::SendMsgToCM]Ret.%d\n", nRet );
	}
	return nRet;
}

/*=============================================================================
函 数 名： DaemonProcTemplateRsp
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/24  5.0         rnn                   创建
=============================================================================*/
void CCMMauSsnInstance::DaemonProcTemplateRsp( CMessage * const pcMsg )
{
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	u16 wErrCode = tReqInfo.GetErrID();

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	u32 dwJsonBufLen = 0;
	
	SetMsgTypeToJson( achJsonBuf, dwJsonBufLen, OspEventDesc(pcMsg->event));
	SetErrCodeToJson( achJsonBuf, dwJsonBufLen, wErrCode );
	CMauData::SetDomainMoIdToJson( achJsonBuf, tReqInfo.GetDomainMOID(), dwJsonBufLen );

	if ( MAU_CM_CREATETEMPLATE_ACK == pcMsg->event
		|| MAU_CM_CREATETEMPLATE_NACK == pcMsg->event
		|| MAU_CM_MDFTEMPLATE_ACK == pcMsg->event
		|| MAU_CM_MDFTEMPLATE_NACK == pcMsg->event )
	{
		SetConfE164ToJson( achJsonBuf, dwJsonBufLen, tReqInfo.GetConfE164() );
	    SetConfNameToJson( achJsonBuf, tReqInfo.GetConfName(), dwJsonBufLen );
	}
	else if ( MAU_CM_DELTEMPLATE_ACK == pcMsg->event
		|| MAU_CM_DELTEMPLATE_NACK == pcMsg->event )
	{
		CMauMsg cTmpMsg;
		cTmpMsg.SetMsgBody( ( cServMsg.GetMsgBody()+sizeof(TReqHeadInfo) ), ( cServMsg.GetMsgBodyLen()-sizeof(TReqHeadInfo) ) );
		CMauData::SetTemplateListToJson( achJsonBuf, dwJsonBufLen, cTmpMsg );
	}	

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[DaemonProcTemplateRsp]Rpc id is %s, content is %s!\n", 
		tReqInfo.GetMQCorrelationId(), tReqInfo.GetMQName() );

	u32 dwPrintBuffLen = strlen(achJsonBuf);
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[DaemonProcTemplateRsp]json len is %d, text is:\n", dwPrintBuffLen );
	LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, achJsonBuf, dwPrintBuffLen);
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[DaemonProcTemplateRsp]end\n" );

    SendMsgToCM( achJsonBuf, dwJsonBufLen, tReqInfo.GetMQCorrelationId(), tReqInfo.GetMQName() );
	
	return;
}

/*=============================================================================
函 数 名： DaemonProcBookConfRsp
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/24  5.0         rnn                   创建
=============================================================================*/
void CCMMauSsnInstance::DaemonProcBookConfRsp( CMessage * const pcMsg )
{
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	
	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	u32 dwJsonBufLen = 0;
	
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	u16 wErrCode = tReqInfo.GetErrID();

//	g_cMauVCApp.GetMeetingIDByE164( tReqInfo.GetConfE164(), nMeetingID );

	SetMsgTypeToJson( achJsonBuf, dwJsonBufLen, OspEventDesc(pcMsg->event));
	SetErrCodeToJson( achJsonBuf, dwJsonBufLen, wErrCode );
//	SetMeetingIDToJson( achJsonBuf, dwJsonBufLen, nMeetingID );
//	SetConfE164ToJson( achJsonBuf, dwJsonBufLen, tReqInfo.GetConfE164() );
	CMauData::SetDomainMoIdToJson( achJsonBuf, tReqInfo.GetDomainMOID(), dwJsonBufLen );

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[DaemonProcBookConfRsp]Rpc id is %s, content is %s!\n", 
		tReqInfo.GetMQCorrelationId(), tReqInfo.GetMQName() );

	SendMsgToCM( achJsonBuf, dwJsonBufLen, tReqInfo.GetMQCorrelationId(), tReqInfo.GetMQName());
	
	return;
}

/*=============================================================================
函 数 名： DaemonProcConfRsp
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/24  5.0         rnn                   创建
=============================================================================*/
void CCMMauSsnInstance::DaemonProcConfRsp( CMessage * const pcMsg )
{
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	u16 wErrCode = tReqInfo.GetErrID();

	json_t_ptr pjRoot = json_object();

	if (pjRoot)
	{
		SetMsgTypeToJsonObj(pjRoot, OspEventDesc(pcMsg->event));
		SetErrCodeToJsonObj(pjRoot, wErrCode);
		CMauData::SetReqRspToJsonObj(pjRoot, tReqInfo);

		json_str pszJsonTemp(pjRoot);
		u32 dwLen = strlen(pszJsonTemp);

		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[DaemonProcConfRsp]Rpc id is %s, content is %s!\n",
			tReqInfo.GetMQCorrelationId(), tReqInfo.GetMQName());

		SendMsgToCM(pszJsonTemp, dwLen, tReqInfo.GetMQCorrelationId(), tReqInfo.GetMQName());
	}
}


void  CCMMauSsnInstance::DaemonProcMcuListRsp( CMessage * const pcMsg )
{
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};	
	u32 dwJsonBufLen = cServMsg.GetMsgBodyLen() - sizeof(TReqHeadInfo);
	memcpy(achJsonBuf, cServMsg.GetMsgBody() + sizeof(TReqHeadInfo), dwJsonBufLen);

	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[DaemonProcMcuListRsp]Rpc id is %s, content is %s!\n", 
		tReqInfo.GetMQCorrelationId(), tReqInfo.GetMQName() );

	SendMsgToCM( achJsonBuf, dwJsonBufLen, tReqInfo.GetMQCorrelationId(), tReqInfo.GetMQName() );

	return;
}


void CCMMauSsnInstance::DaemonProcCmtoMauMsg( CMessage * const pcMsg )
{
	s8* achJsonBuf = (s8*)pcMsg->content;
	u32 dwPrintBuffLen = pcMsg->length;

	if (CheckQComsumerStatus(achJsonBuf, dwPrintBuffLen))
	{
		return;
	}

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[CCMMauSsnData::ContentBody_MQCB]cm json len is %d, text is:.\n", dwPrintBuffLen);
	LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, achJsonBuf, dwPrintBuffLen);
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[CCMMauSsnData::ContentBody_MQCB]end\n" );

	achJsonBuf = (s8*)pcMsg->content;
	u16 wMsgType = 0;
	ConvertMsgTypeToEventId( achJsonBuf, wMsgType );

	s8 achConfName[MAXLEN_CONFNAME+1] = {0};
	s8 achDomainMOID[OPERATION_MAXLEN_MOID+1] = {0};
	GetConfNameFromJson( achJsonBuf, achConfName, MAXLEN_CONFNAME );
	GetMOIDFromJson( achJsonBuf, achDomainMOID, OPERATION_MAXLEN_MOID );
	u32 dwMeetingId = 0;
	CMauData::GetMeetingIdFromJson(achJsonBuf, dwMeetingId);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetMQName( g_achRpc );   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId( g_achRpcId );	//存储rpc模式id
	tReqHeadInfo.SetReqEventID( wMsgType );
	tReqHeadInfo.SetConfName( achConfName );
	tReqHeadInfo.SetDomainMOID( achDomainMOID );
	tReqHeadInfo.SetMeetingID( dwMeetingId );
	tReqHeadInfo.SetReqSrcID( MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON) );
	CompletedReqInfo(wMsgType, achJsonBuf, tReqHeadInfo);

	CMauMsg cServMsg;
	cServMsg.SetEventId( wMsgType );
	cServMsg.SetMsgBody(&tReqHeadInfo, sizeof( tReqHeadInfo ));
	cServMsg.CatMsgBody(pcMsg->content, pcMsg->length );
	// 追加一个结束符
	u8 byEnd = 0;
	cServMsg.CatMsgBody(&byEnd, sizeof(byEnd));

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[ContentBody_MQCB]Replay rpc id is %s, content is %s!\n", 
		tReqHeadInfo.GetMQCorrelationId(), tReqHeadInfo.GetMQName() );
	
	CMauData::SendMsgToMauVC( wMsgType, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );

	memset( g_achRpc, 0, sizeof( g_achRpc ) );
	memset( g_achRpcId, 0, sizeof( g_achRpcId ) );

	return;
}

/*=============================================================================
函 数 名： DaemonProcAllHduInfoNtf
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/24  5.0         rnn                   创建
=============================================================================*/
void CCMMauSsnInstance::DaemonProcAllHduInfoNtf( CMessage * const pcMsg )
{
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );

	u8 *pbyMsg = cServMsg.GetMsgBody();

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	u32 dwJsonBufLen = 0;
	
	u16 wMoidNum = *(u16*)pbyMsg;
	pbyMsg += sizeof(u16);

	LogPrint( LOG_LVL_DETAIL, MID_MAU_CMMAU, "[DaemonProcAllHduInfoNtf]Hdu moid Num is %d.\n", wMoidNum );
	SetMsgTypeToJson( achJsonBuf, dwJsonBufLen, EV_MAU_CM_ALLHDUINFO_NTF );
	CMauData::SetAllHduInfoToJson( achJsonBuf, dwJsonBufLen, pbyMsg, wMoidNum );

	SendMsgToCM( achJsonBuf, dwJsonBufLen );

	LogPrint( LOG_LVL_DETAIL, MID_MAU_CMMAU, "[DaemonProcAllHduInfoNtf]json len is %d, text is:\n", dwJsonBufLen);
	LongLogPrint(LOG_LVL_DETAIL, MID_MAU_CMMAU, achJsonBuf, dwJsonBufLen);
	LogPrint( LOG_LVL_DETAIL, MID_MAU_CMMAU, "[DaemonProcAllHduInfoNtf]end\n" );
	
	return;
}

void CCMMauSsnInstance::DaemonProcConfInfoByMoidRsp(CMessage * const pcMsg)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 *pbyMsg = cServMsg.GetMsgBody();

	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	u16 wErrCode = tReqInfo.GetErrID();

	pbyMsg += sizeof(tReqInfo);
	s8* pchJson = (s8*)pbyMsg;
	u32 dwJsonLen = cServMsg.GetMsgBodyLen() - sizeof(tReqInfo);

	json_t_ptr pjRoot = NULL;
	LoadJson(pchJson, dwJsonLen, pjRoot, TRUE);

	if (pjRoot)
	{
		SetMsgTypeToJsonObj(pjRoot, OspEventDesc(pcMsg->event));
		SetErrCodeToJsonObj(pjRoot, wErrCode);
		SetDomainMoidToJsonObj(pjRoot, tReqInfo.GetDomainMOID());

		json_str pchTempJson(pjRoot);
		u32 dwTempJsonLen = strlen(pchTempJson);
		LogPrint(LOG_LVL_DETAIL, MID_MAU_CMMAU, "[DaemonProcHduInfoByMoidNtf]json len is %d, text is:\n", dwTempJsonLen);
		LongLogPrint(LOG_LVL_DETAIL, MID_MAU_CMMAU, pchTempJson, dwTempJsonLen);
		SendMsgToCM(pchTempJson, dwTempJsonLen, tReqInfo.GetMQCorrelationId(), tReqInfo.GetMQName());
	}
}

void CCMMauSsnInstance::DaemonProcHduInfoByMoidRsp(CMessage * const pcMsg)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 *pbyMsg = cServMsg.GetMsgBody();

	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	u16 wErrCode = tReqInfo.GetErrID();

	pbyMsg += sizeof(tReqInfo);
	s8* achDomainMoid = (s8*)pbyMsg;
	pbyMsg += (OPERATION_MAXLEN_MOID + 1);

	u16 wHduNum = 0;
	wHduNum = (cServMsg.GetMsgBodyLen() - OPERATION_MAXLEN_MOID - 1 - sizeof(TReqHeadInfo)) / sizeof(CHduInfo);
	CHduInfo *pcHduInfo = (CHduInfo*)pbyMsg;

	json_t_ptr pjRoot = json_object();

	if (pjRoot)
	{
		SetMsgTypeToJsonObj(pjRoot, OspEventDesc(pcMsg->event));
		SetErrCodeToJsonObj(pjRoot, wErrCode);
		SetDomainMoidToJsonObj(pjRoot, achDomainMoid);
		CMauData::SetHduInfoToJsonObj(pjRoot, pcHduInfo, wHduNum);

		json_str abyJson(pjRoot);
		u32 dwJsonLen = strlen(abyJson);
		LogPrint(LOG_LVL_DETAIL, MID_MAU_CMMAU, "[DaemonProcHduInfoByMoidNtf]json len is %d, text is:\n", dwJsonLen);
		LongLogPrint(LOG_LVL_DETAIL, MID_MAU_CMMAU, abyJson, dwJsonLen);
		SendMsgToCM(abyJson, dwJsonLen, tReqInfo.GetMQCorrelationId(), tReqInfo.GetMQName());
	}
}

/*=============================================================================
函 数 名： DaemonProcHduInfoByMoidNtf
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/24  5.0         rnn                   创建
=============================================================================*/
void CCMMauSsnInstance::DaemonProcHduInfoByMoidNtf( CMessage * const pcMsg )
{
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );

	u8 *pbyMsg = cServMsg.GetMsgBody();
	
	//域guid
	s8 achDomainMoid[OPERATION_MAXLEN_MOID + 1] = {0};
	memcpy( achDomainMoid, pbyMsg, OPERATION_MAXLEN_MOID);
	pbyMsg += OPERATION_MAXLEN_MOID;
	u16 wHduNum = 0;
	wHduNum = (cServMsg.GetMsgBodyLen() - OPERATION_MAXLEN_MOID) / sizeof(CHduInfo);
	CHduInfo *pcHduInfo = (CHduInfo*)pbyMsg;
	
	json_t_ptr pjRoot = json_object();
	if (pjRoot)
	{
		SetMsgTypeToJsonObj(pjRoot, EV_MAU_CM_HDUINFO_NTF);
		SetDomainMoidToJsonObj(pjRoot, achDomainMoid);
		CMauData::SetHduInfoToJsonObj(pjRoot, pcHduInfo, wHduNum);

		json_str abyJson(pjRoot);
		u32 dwJsonLen = strlen(abyJson);
		SendMsgToCM(abyJson, dwJsonLen);
		LogPrint(LOG_LVL_DETAIL, MID_MAU_CMMAU, "[DaemonProcHduInfoByMoidNtf]json len is %d, text is:\n", dwJsonLen);
		LongLogPrint(LOG_LVL_DETAIL, MID_MAU_CMMAU, abyJson, dwJsonLen);
		LogPrint(LOG_LVL_DETAIL, MID_MAU_CMMAU, "[DaemonProcHduInfoByMoidNtf]end\n");
	}
}

/*=============================================================================
函 数 名： DaemonProcConfNtf
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/24  5.0         rnn                   创建
=============================================================================*/
void CCMMauSsnInstance::DaemonProcConfNtf( CMessage * const pcMsg )
{
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[%s]json len is %d, text is:.\n", __FUNCTION__, pcMsg->length);
	LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, (const s8*)pcMsg->content, pcMsg->length);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMMAU, "[%s]end\n", __FUNCTION__);
	SendMsgToCM( (const s8*)pcMsg->content, pcMsg->length);
}

