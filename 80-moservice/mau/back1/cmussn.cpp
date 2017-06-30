/*****************************************************************************
模块名      : MAU
文件名      : cmussn.cpp
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
#include "cmussn.h"
#include "evmau.h"
#include "mauvc.h"
using namespace rabbitmqwrap;

s32 CCmuSsnData::s_nCmuRabbitMq = 0;

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
BOOL32 CCmuSsnData::Init_MQPC()
{
	// 创建生产者
	s_nCmuRabbitMq = SafeCreateP( ProducerSuccCB, ProducerFailCB );
	if ( 0 == s_nCmuRabbitMq )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMU, "[CCmuSsnData::Init_MQPC] s_nCmuRabbitMq = 0, return FALSE!\n" );
	}

	// 创建消费者
	SafeCreateC( MQ_MAU_CONF_Q, MQ_MAU_CONF_EX, MQ_MAU_CONF_K, Content_MQCB, NULL, NULL, ConsumerSuccCB, ConsumerFailCB, 0 );

	return TRUE;
}

/*=============================================================================
函 数 名： Content_MQCB
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
void CCmuSsnData::Content_MQCB( amqp_connection_state_t conn, void *pbuf, size_t len, response_type *prsp_type, char* pchTag )
{
	if ( NULL == pbuf )
	{
		*prsp_type = RT_NACK;
		return;
	}

	*prsp_type = RT_ACK;

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, CInstance::DAEMON), CMU_MAU_MESSAGE_CB, pbuf, len);
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
=============================================================================*/
void CCMUSsnInstance::ConvertMsgTypeToEventId(s8 * pchJsonBuf, u16 &wMsgType)
{
	if (NULL == pchJsonBuf)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_CMU, "[ConvertMsgTypeToEventId]Param is NULL, return!\n");
		return;
	}

	s8 achMsgType[MAXLEN_MSGTYPE + 1] = { 0 };
	u32 dwMsgTypeLen = MAXLEN_MSGTYPE;

	if (!GetMsgTypeFromJson(pchJsonBuf, achMsgType, dwMsgTypeLen))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_CMU, "[ConvertMsgTypeToEventId]GetMsgTypeFromJson err, return!\n");
		return;
	}

	// 	if ( 0 == strncmp( EV_MCU_MAU_LIVE_NTF, achMsgType, strlen( EV_MCU_MAU_LIVE_NTF ) ) )
	//	{
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[ConvertMsgTypeToEventId]Rcv msg.%s from Cmu!\n", achMsgType);
	//	}

		// 注册相应
	if (0 == strncmp(EV_MCU_MAU_CONNECT_ACK, achMsgType, strlen(EV_MCU_MAU_CONNECT_ACK)))
	{
		wMsgType = MCU_MAU_CONNECT_ACK;
	}
	else if (0 == strncmp(EV_MCU_MAU_CONNECT_NACK, achMsgType, strlen(EV_MCU_MAU_CONNECT_NACK)))
	{
		wMsgType = MCU_MAU_CONNECT_NACK;
	}
	//终端相关
	else if (0 == strncmp(EV_MCU_MAU_ADDMT_ACK, achMsgType, strlen(EV_MCU_MAU_ADDMT_ACK)))
	{
		wMsgType = MCU_MAU_ADDMT_ACK;
	}
	else if (0 == strncmp(EV_MCU_MAU_ADDMT_NACK, achMsgType, strlen(EV_MCU_MAU_ADDMT_NACK)))
	{
		wMsgType = MCU_MAU_ADDMT_NACK;
	}
	//会议相关
	else if (0 == strncmp(EV_MCU_MAU_CREATECONF_ACK, achMsgType, strlen(EV_MCU_MAU_CREATECONF_ACK)))
	{
		wMsgType = MCU_MAU_CREATECONF_ACK;
	}
	else if (0 == strncmp(EV_MCU_MAU_CREATECONF_NACK, achMsgType, strlen(EV_MCU_MAU_CREATECONF_NACK)))
	{
		wMsgType = MCU_MAU_CREATECONF_NACK;
	}
	else if (0 == strncmp(EV_MCU_MAU_CONFRELEASE_NTF, achMsgType, strlen(EV_MCU_MAU_CONFRELEASE_NTF)))
	{
		wMsgType = MCU_MAU_RELEASECONF_NTF;
	}
	else if (0 == strncmp(EV_MCU_MAU_CONFLIST_NTF, achMsgType, strlen(EV_MCU_MAU_CONFLIST_NTF)))
	{
		wMsgType = MCU_MAU_CONFLIST_NTF;
	}
	else if (0 == strncmp(EV_MCU_MAU_CONFINFO_ACK, achMsgType, strlen(EV_MCU_MAU_CONFINFO_ACK)))
	{
		wMsgType = MCU_MAU_CONFINFO_ACK;
	}
	else if (0 == strncmp(EV_MCU_MAU_CONFINFO_NACK, achMsgType, strlen(EV_MCU_MAU_CONFINFO_NACK)))
	{
		wMsgType = MCU_MAU_CONFINFO_NACK;
	}
	else if (0 == strncmp(EV_CMU_MAU_DELAYCONF_REQ, achMsgType, strlen(EV_CMU_MAU_DELAYCONF_REQ)))
	{
		wMsgType = MCU_MAU_DELAYCONF_REQ;
	}
	else if (0 == strncmp(EV_CMU_MAU_SUBCONFDURATION_REQ, achMsgType, strlen(EV_CMU_MAU_SUBCONFDURATION_REQ)))
	{
		wMsgType = MCU_MAU_SUBCONFDURATION_REQ;
	}
	else if (0 == strncmp(EV_CMU_MAU_CONFSYN_NTF, achMsgType, strlen(EV_CMU_MAU_CONFSYN_NTF)))
	{
		wMsgType = MCU_MAU_CONFSYN_NTF;
	}
	else if (0 == strncmp(EV_MCU_MAU_CONFSTATECHANGE_NTF, achMsgType, strlen(EV_MCU_MAU_CONFSTATECHANGE_NTF)))
	{
		wMsgType = MCU_MAU_CONFSTATECHANGE_NTF;
	}
	else if (0 == strncmp(EV_MAU_CM_GETMCULIST_ACK, achMsgType, strlen(EV_MAU_CM_GETMCULIST_ACK)))
	{
		wMsgType = MAU_CM_GETMCULIST_ACK;
	}
	else if (0 == strncmp(EV_MAU_CM_GETMCULIST_NACK, achMsgType, strlen(EV_MAU_CM_GETMCULIST_NACK)))
	{
		wMsgType = MAU_CM_GETMCULIST_NACK;
	}
	else if (0 == strncmp(EV_MAU_CM_MCULIST_NTF, achMsgType, strlen(EV_MAU_CM_MCULIST_NTF)))
	{
		wMsgType = MAU_CM_MCULIST_NTF;
	}
	else if (0 == strncmp(EV_CMU_MAU_MODIFYCONFNAME_REQ, achMsgType, strlen(EV_CMU_MAU_MODIFYCONFNAME_REQ)))
	{
		wMsgType = MCU_MAU_MODIFYCONFNAME_REQ;
	}
	// 保活
	else if ( 0 == strncmp( EV_MCU_MAU_LIVE_NTF, achMsgType, strlen( EV_MCU_MAU_LIVE_NTF ) ) )
	{
		wMsgType = MCU_MAU_LIVE_NTF;
	}
	else if (0 == strncmp(EV_MCU_MAU_RESTORECONF_ACK, achMsgType, strlen(EV_MCU_MAU_RESTORECONF_ACK)))
	{
		wMsgType = MCU_MAU_RESTORECONF_ACK;
	}
	else if (0 == strncmp(EV_MCU_MAU_RESTORECONF_NACK, achMsgType, strlen(EV_MCU_MAU_RESTORECONF_NACK)))
	{
		wMsgType = MCU_MAU_RESTORECONF_NACK;
	}
	// 透传CMU-->CM信令
	else
	{
		wMsgType = CMU_CM_CONFOPR_NTF;
	}

	return;
}


/*=============================================================================
函 数 名： CCMUSsnInstance
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
CCMUSsnInstance::CCMUSsnInstance()
{}


/*=============================================================================
函 数 名： ~CCMUSsnInstance
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
CCMUSsnInstance::~CCMUSsnInstance()
{}

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
void CCMUSsnInstance::DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp )
{
	if ( NULL ==  pcMsg )
	{
		return;
	}

	switch ( pcMsg->event )
	{
	case OSP_POWERON:
		DaemonProcPowerOn(pcMsg, pcApp);
		break;
	case RESMGR_CMUSSN_CMUINFO_NTF:
		DaemonProcCmuInfoNtf(pcMsg, pcApp);
		break;
	case CMU_MAU_MESSAGE_CB:
		DaemonCmuMessageCb(pcMsg, pcApp);
		break;
	default:
		break;
	}
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
void CCMUSsnInstance::DaemonProcPowerOn(CMessage * const pcMsg, CApp *pcApp)
{
	//状态初始化
	NextState( STATE_IDLE );

	u16 wInstId = 1;
	for ( wInstId = 1; wInstId < MAX_CMU_NUM; wInstId++ )
	{
		OspPost( MAKEIID( AID_MAU_CMUSSN_APPID, wInstId ), pcMsg->event );
	}

	CCMUSsnApp* pcCmuApp = dynamic_cast<CCMUSsnApp*>(pcApp);
	if (NULL != pcCmuApp)
	{
		pcCmuApp->Init_MQPC();
	}
	else
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_CMU, "[%s]pcCmuApp is NULL\n", __FUNCTION__);
	}
}

/*=============================================================================
函 数 名： DaemonProcCmuInfoNtf
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CCMUSsnInstance::DaemonProcCmuInfoNtf( CMessage * const pcMsg, CApp* pcApp )
{
	if ( NULL == pcMsg )
	{
		return;
	}

	CCMUSsnApp* pcCmuApp = dynamic_cast<CCMUSsnApp*>(pcApp);
	if (NULL == pcCmuApp)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_CMU, "[%s]pcCmuApp is NULL\n", __FUNCTION__);
		return;
	}

	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	u8 * pbyMsg = cServMsg.GetMsgBody();

	s32 nCmuNum = *(s32*)pbyMsg;
	pbyMsg += sizeof( s32 );

	s32 nCmuIdx = 0; 
	for ( nCmuIdx = 0; nCmuIdx < nCmuNum; nCmuIdx++ )
	{
		TCmuInfo* ptCmuInfo = (TCmuInfo*)pbyMsg;
		pbyMsg += sizeof(TCmuInfo);

		CCMUSsnInstance* pcFindInst = dynamic_cast<CCMUSsnInstance*>(pcApp->FindInstByAlias(ptCmuInfo->achMcuIP, strlen(ptCmuInfo->achMcuIP)));
		if (pcFindInst)
		{
			LogPrint(LOG_LVL_DETAIL, MID_MAU_CMU, "[%s]CMU<%s> is known!\n", __FUNCTION__, ptCmuInfo->achMcuIP);
			// 如果找到了，但PID不同，说明CMU重启过了
			if (strcmp(ptCmuInfo->achMcuPid, pcFindInst->m_tCmuInfo.achMcuPid) != 0)
			{
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[%s]CMU<%s> Pid Changed! Old:%s New:%s\n",
					__FUNCTION__, ptCmuInfo->achMcuIP, pcFindInst->m_tCmuInfo.achMcuPid, ptCmuInfo->achMcuPid);
				pcFindInst->ProcDisconnect(ptCmuInfo);
			}
			continue;
		}
		
		u16 wIdleInstID = 0;
	    u16 wExistInstID = 0;
		
		u16 wLoop = 0;
		// 分配空闲实例
		for( wLoop = 1; wLoop <= MAX_CMU_NUM; wLoop++ )
		{
			pcCmuApp->m_dwCurInsId = (pcCmuApp->m_dwCurInsId + 1) % ( MAX_CMU_NUM + 1 );
			if ( 0 == pcCmuApp->m_dwCurInsId)
			{
				pcCmuApp->m_dwCurInsId = 1;
			}

			CCMUSsnInstance * pcInst = (CCMUSsnInstance*)pcApp->GetInstance(pcCmuApp->m_dwCurInsId);
			if( NULL == pcInst )
			{
				continue;
			}

			if( STATE_IDLE == pcInst->CurState() )
			{
				wIdleInstID = pcCmuApp->m_dwCurInsId;
				break;
			}
		}

		if ( 0 == wIdleInstID )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_CMU, "[CCMUSsnInstance::DaemonProcCmuInfoNtf]There is no idle inst for CMU<QName.%s>, continue!\n", 
				ptCmuInfo->achRoutingKey);
			continue;
		}

		LogPrint( LOG_LVL_WARNING, MID_MAU_CMU, "[CCMUSsnInstance::DaemonProcCmuInfoNtf]Assign inst<Id.%d> for CMU<QName.%s> McuIp<%s> MucType<%s> McuName<%s> GKPreFix<%s>!\n", 
				wIdleInstID, ptCmuInfo->achRoutingKey, ptCmuInfo->achMcuIP, ptCmuInfo->achMcuType, ptCmuInfo->achMcuName, ptCmuInfo->achGKPreFix);

		CCMUSsnInstance* pcInst = (CCMUSsnInstance*)pcApp->GetInstance(pcCmuApp->m_dwCurInsId);
		if (pcInst)
		{
			pcInst->NextState(STATE_WAITING);
			pcInst->SetAlias(ptCmuInfo->achMcuIP, strlen(ptCmuInfo->achMcuIP));
			pcInst->SetCmuInfo(*ptCmuInfo);

			OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, wIdleInstID), CMUSSN_CMUSSN_CMUINFO_NTF, NULL, 0);
		}
	}
	return;
}

/*=============================================================================
函 数 名： ProcPowerOn
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
void CCMUSsnInstance::ProcPowerOn( CMessage * const pcMsg )
{
	ClearInst();

	return;
}

/*=============================================================================
函 数 名： ProcCmuLiveNtf
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
void CCMUSsnInstance::ProcCmuLiveNtf( CMessage * const pcMsg )
{
	if (CurState() == STATE_NORMAL)
	{
		u32 dwCmuPid = 0;
		GetCmuPidFromJson((const s8*)pcMsg->content, dwCmuPid);
		if (strtoul(m_tCmuInfo.achMcuPid, 0, 10) == dwCmuPid)
		{
			KillTimer(CMUSSN_HEADBEAR_TIMER);
			m_wHBFailCount = 0;
			SetTimer(CMUSSN_HEADBEAR_TIMER, g_cMauVCApp.GetMcuHeartBeatTimeSpan() * 1000);
		}
	}
}

/*=============================================================================
函 数 名： ProcHeadBeatTimer
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
void CCMUSsnInstance::ProcHeartBeatTimer( CMessage * const pcMsg )
{
	KillTimer(CMUSSN_HEADBEAR_TIMER);

	if(CurState() == STATE_NORMAL)
	{
		m_wHBFailCount = m_wHBFailCount % 3 + 1;
		if (3 > m_wHBFailCount)
		{
			LogPrint(LOG_LVL_WARNING, MID_MAU_CMU, "[ProcHeadBeatTimer]Cmu<%s> HB fail %d times!\n",
				m_tCmuInfo.achRoutingKey, m_wHBFailCount);
		}
		else
		{
			ProcDisconnect();
		}
	}
	else if (CurState() == STATE_WAITING)
	{
		ConnectCmu();
	}

	SetTimer(CMUSSN_HEADBEAR_TIMER, g_cMauVCApp.GetMcuHeartBeatTimeSpan() * 1000);
}

void CCMUSsnInstance::ProcGetConfListTimer(CMessage * const pcMsg)
{
	KillTimer(CMUSSN_GETCONFLIST_TIMER);
	if (CurState() == STATE_NORMAL)
	{
		ConnectCmu(TRUE);
	}
	SetTimer(CMUSSN_GETCONFLIST_TIMER, g_cMauVCApp.GetConfListTimeSpan() * 1000);
}

void CCMUSsnInstance::ProcDisconnect(const TCmuInfo* ptCmuInfo/*=NULL*/)
{
	if (CurState() == STATE_NORMAL || ptCmuInfo != NULL)
	{
		//通知MPCD主控
		CMauData::SendMsgToMauVC(MAU_CMUDISCONNECT_NTF, &m_tCmuInfo, sizeof(m_tCmuInfo));
		// 切换为等待连接状态
		NextState(STATE_WAITING);

		if (ptCmuInfo != NULL)
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[%s]mcu<%s:(%s->%s)> reboot,"
				"set instance waitting state, connect mcu and send disconnect to vc\n",
				__FUNCTION__, m_tCmuInfo.achMcuIP, m_tCmuInfo.achMcuPid, ptCmuInfo->achMcuPid);
			SetCmuInfo(*ptCmuInfo);
			SetTimer(CMUSSN_HEADBEAR_TIMER, g_cMauVCApp.GetMcuHeartBeatTimeSpan() * 1000);
		}

		m_wHBFailCount = 0;
		ConnectCmu();
	}
}

/*=============================================================================
函 数 名： ProcConnectCmu
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
void CCMUSsnInstance::ConnectCmu(const BOOL32 bIgnoreState/*=FALSE*/)
{
	if (CurState() == STATE_WAITING || bIgnoreState)
	{
		json_t_ptr pjRoot = json_object();
		if (pjRoot)
		{
			SetMsgTypeToJsonObj(pjRoot, EV_MAU_MCU_GETCONFLIST_CMD);
			SetStringToJson(pjRoot, JSON_QNAME, MQ_MAU_CONF_Q);

			json_str abyJson(pjRoot);
			u32 dwJsonLen = strlen(abyJson);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[CCMUSsnInstance::ProcConnectCmu]Key<%s> Ip<%s>!\n",
				m_tCmuInfo.achRoutingKey, m_tCmuInfo.achMcuIP);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[CCMUSsnInstance::ProcConnectCmu]mq handle is %d.\n", CCmuSsnData::s_nCmuRabbitMq);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[CCMUSsnInstance::ProcConnectCmu]json len is %d, text is:.\n", dwJsonLen);
			LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, abyJson, dwJsonLen);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[CCMUSsnInstance::ProcConnectCmu]end\n");

			SendMsgToCmu(abyJson, dwJsonLen, CONNECT_TIMEOUT);
		}
		else
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_CMU, "[CCMUSsnInstance::ProcConnectCmu] Construct Connect Json Failed!\n");
		}
	}
}

/*=============================================================================
函 数 名： ProcCmuInfoNtf
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
void CCMUSsnInstance::ProcCmuInfoNtf( CMessage * const pcMsg )
{
	ConnectCmu();
	SetTimer(CMUSSN_HEADBEAR_TIMER, g_cMauVCApp.GetMcuHeartBeatTimeSpan() * 1000);
	SetTimer(CMUSSN_GETCONFLIST_TIMER, g_cMauVCApp.GetConfListTimeSpan() * 1000);
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
void CCMUSsnInstance::InstanceEntry(  CMessage * const pcMsg )
{
	//参数检查
	if( NULL == pcMsg )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMU, "[InstanceEntry]Param pcMsg is NULL!\n" );
		return;
	}

	switch( pcMsg->event )
	{
		//上电初始化消息
	case OSP_POWERON:
		ProcPowerOn( pcMsg );
		break;

	case CMUSSN_CMUSSN_CMUINFO_NTF:
		ProcCmuInfoNtf( pcMsg );
		break;

	case CMUSSN_HEADBEAR_TIMER:
		ProcHeartBeatTimer( pcMsg );
		break;

	case CMUSSN_GETCONFLIST_TIMER:
		ProcGetConfListTimer(pcMsg);
		break;

		// MAU投递给CMU
	case MAU_MCU_ADDMT_REQ:
	case MAU_MCU_CREATECONF_REQ:
	case MAU_MCU_RELEASECONF_CMD:
	case MAU_MCU_DELAYCONF_ACK:
	case MAU_MCU_DELAYCONF_NACK:
	case MAU_CMU_CONFDURATION_NTF:
	case MAU_MCU_SUBCONFDURATION_ACK:
	case MAU_MCU_SUBCONFDURATION_NACK:
		// 透传给CM
	case CM_CMU_CONFOPR_NTF:
	case MAU_MCU_RESTORECONF_REQ:
	case MAU_MCU_UPDATEALLCONFDATA_CMD:
		ProcMsgToCmu( pcMsg );
		break;
	case MCU_MAU_LIVE_NTF:
		ProcCmuLiveNtf(pcMsg);
		break;
		// CMU回复处理
	case MCU_MAU_CREATECONF_ACK:
	case MCU_MAU_CREATECONF_NACK:
	case MCU_MAU_CONFINFO_ACK:
	case MCU_MAU_CONFINFO_NACK:
	case MCU_MAU_RELEASECONF_NTF:
	case MCU_MAU_CONFLIST_NTF:
	case MCU_MAU_CONFSYN_NTF:
	case MCU_MAU_ADDMT_ACK:      // MAU 添加终端响应
	case MCU_MAU_ADDMT_NACK:
	case MCU_MAU_RESTORECONF_ACK:
	case MCU_MAU_RESTORECONF_NACK:
		ProcCmuLiveNtf(pcMsg);
		ProcCmuRspToMauVc( pcMsg );
		break;
		// CMU请求处理
	case MCU_MAU_DELAYCONF_REQ:
	case MCU_MAU_CONFSTATECHANGE_NTF:
	case MCU_MAU_SUBCONFDURATION_REQ:
	case MCU_MAU_MODIFYCONFNAME_REQ:
		ProcCmuLiveNtf(pcMsg);
		ProcCmuReqToMauVc( pcMsg );
		break;

	case CMU_CM_CONFOPR_NTF:
		ProcCmuLiveNtf(pcMsg);
		ProcCmuRspToCM( pcMsg );
		break;
//  MCU发给CM的消息 ---end---

	case MAU_MCU_MTLIST_NTF:
		ProcMtListNtf( pcMsg );
		break;

	case MAU_MCU_SRVDOMAINLIST_NTF:
		ProcSrvDomainNtf( pcMsg );
		break;
		
	case MAU_MCU_PLATFORMDOMAINLIST_NTF:
		ProcPlatformDomainNtf( pcMsg );
		break;

	case MAU_MCU_USERDOMAIN_NTF:
		ProcUserDomainNtf( pcMsg );
		break;

	default:
		break;
	}
}

/*=============================================================================
函 数 名： ClearInst
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
void CCMUSsnInstance::ClearInst()
{
	m_wHBFailCount = 0;
	// 	m_dwMcuIp = 0;
	// 	memset( m_achCmuRoutingKey, 0, sizeof( m_achCmuRoutingKey ) );
	// 	memset( m_achCmuType, 0, sizeof( m_achCmuType ) );
	// 	memset( m_achCmuName, 0, sizeof( m_achCmuName ) );
	m_tCmuInfo.SetNull();
	NextState(STATE_IDLE);
}
/*=============================================================================
函 数 名： ProcMsgToCmu
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/25  5.0         rnn                    创建
=============================================================================*/
void CCMUSsnInstance::ProcMsgToCmu( CMessage * const pcMsg )
{
	if ( STATE_NORMAL != CurState())
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMU, "[ProcMsgToCmu]Cmussn<Id.%d> state<%d> err, return!\n", 
			GetInsID(), CurState() );
		return;
	}

	if (pcMsg->event == MAU_MCU_RESTORECONF_REQ)
	{
		string strConfE164;
		BOOL32 bHasXmpu = FALSE;
		if (!GetRecoverConfNeedInfoFromJson(string((const s8*)pcMsg->content, pcMsg->length), strConfE164, bHasXmpu))
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_CMU, "[%s]GetRecoverConfNeedInfoFromJson fail\n", __FUNCTION__);
			return;
		}

		string createConfJson;
		if (!g_cMauVCApp.GetCreateConfJson(strConfE164.c_str(), bHasXmpu, createConfJson))
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_CMU, "[%s]Get CreateConfJson fail\n", __FUNCTION__);
			return;
		}
		if (createConfJson.empty())
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_CMU, "[%s]Conf<%s>BaseData unexist and state is prepare,"
				" will delete by checkconftimer \n", __FUNCTION__, strConfE164.c_str());
			return;
		}
		SendMsgToCmu(createConfJson.c_str(), createConfJson.length());
	}
	else
	{
		//直接透传
		SendMsgToCmu((const s8*)pcMsg->content, pcMsg->length);
	}
}

/*=============================================================================
函 数 名： SendMsgToCmu
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
s32 CCMUSsnInstance::SendMsgToCmu( const s8 * const pbyMsgBody /* = NULL */, u32 dwLen /* = 0 */, u32 dwTTL)
{
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[%s]cm json len is %d, text is:.\n", __FUNCTION__, dwLen);
	LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, (const s8*)pbyMsgBody, dwLen);
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[%s]end\n", __FUNCTION__ );
	LogPrint(LOG_LVL_DETAIL, MID_MAU_CMU, "[%s]RoutingKey: %s\n", __FUNCTION__, m_tCmuInfo.achRoutingKey);

	return PostMsg(CCmuSsnData::s_nCmuRabbitMq, pbyMsgBody, dwLen, m_tCmuInfo.achRoutingKey, MQ_MCU_EX, 0, 0, NULL, NULL, dwTTL );
}

/*=============================================================================
函 数 名： SendMsgToCmCmuSsn
功    能： 向CMCMU发送消息
算法实现： 
全局变量： 
参    数： 
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/26  5.0         rnn                    创建
=============================================================================*/
void CCMUSsnInstance::SendMsgToCmCmuSsn( u16 wEvent, u8* const pbyMsg, u16 wLen )
{
	OspPost( MAKEIID(AID_MAU_CMCMUSSN_APPID, CInstance::DAEMON), wEvent, pbyMsg, wLen );
	
	return ;
}

void CCMUSsnInstance::SendMsgToMauVc(u16 wEvent, u8* const pbyMsg /*= NULL*/, u16 wLen /*= 0*/)
{
	CMauData::SendMsgToMauVC(wEvent, pbyMsg, wLen, MAKEIID(GetAppID(), GetInsID()));
}

/*=============================================================================
函 数 名： ProcCmuRspToMauVc
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
void CCMUSsnInstance::ProcCmuRspToMauVc( CMessage * const pcMsg )
{
	// 收到列表标示建链成功
	if (MCU_MAU_CONFLIST_NTF == pcMsg->event)
	{
		NextState(STATE_NORMAL);
		CMauMsg cServMsg;
		cServMsg.SetMsgBody(&m_tCmuInfo, sizeof(m_tCmuInfo));
		SendMsgToMauVc(MAU_CMUCONNECT_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
	}
	else
	{
		const s8* achJsonBuf = (const s8*)pcMsg->content;
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[CCMUSsnInstance::ProcCmuRspToMauVc]json len is %d, text is:\n", pcMsg->length);
		LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, achJsonBuf, pcMsg->length);
	}
	SendMsgToMauVc(pcMsg->event, pcMsg->content, pcMsg->length);
}

/*=============================================================================
函 数 名： ProcCmuRspToCM
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
void CCMUSsnInstance::ProcCmuRspToCM( CMessage * const pcMsg )
{
	SendMsgToMauVc(pcMsg->event, pcMsg->content, pcMsg->length);
}

/*=============================================================================
函 数 名： ProcCmuReqToMauVc
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/11/11  5.0         rnn                   创建
=============================================================================*/
void CCMUSsnInstance::ProcCmuReqToMauVc( CMessage * const pcMsg )
{
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[CCMUSsnInstance::ProcCmuReqToMauVc] cm json len is %d, text is:\n", pcMsg->length);
	LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, (const s8*)pcMsg->content, pcMsg->length);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[CCMUSsnInstance::ProcCmuReqToMauVc]end\n");
	SendMsgToMauVc(pcMsg->event, pcMsg->content, pcMsg->length);
}

/*=============================================================================
函 数 名： ProcMtListNtf
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/11/11  5.0         rnn                   创建
=============================================================================*/
void CCMUSsnInstance::ProcMtListNtf( CKdvMtInfo * pcMtInfo /* = NULL */, BOOL32 bDel /* = FALSE  */)
{
	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	u32 dwJsonBufLen = 0;
	u32 dwMtListSize = 0;	

	if ( NULL != pcMtInfo )  // 修改指定的MtInfo
	{
		dwMtListSize = 1;
		SetMsgTypeToJson( achJsonBuf, dwJsonBufLen, EV_MAU_MCU_MTLIST_NTF );
		SetMtListToJson( achJsonBuf, dwJsonBufLen, pcMtInfo, dwMtListSize, bDel );
		SendMsgToCmu( achJsonBuf, dwJsonBufLen);

	}
	else // 批量通知MtInfo
	{
		dwMtListSize = g_cMauVCApp.GetMtListSize();
		CKdvMtInfo * pcLocalMtInfo = new CKdvMtInfo[dwMtListSize];
		if ( NULL == pcLocalMtInfo )
		{
			LogPrint( LOG_LVL_ERROR, MID_MAU_CMU, "[ProcMtListNtf]new mtinfo err, return!\n" );
			return;
		}
		
		if ( !g_cMauVCApp.GetAllMtList( pcLocalMtInfo, dwMtListSize ) )
		{
			LogPrint( LOG_LVL_ERROR, MID_MAU_CMU, "[ProcMtListNtf]GetAllMtList err, return!\n" );
			delete [] pcLocalMtInfo;
			return;
		}
		
		CKdvMtInfo atMtInfo[MAX_NTFNUM_ONEPACK];
		CKdvMtInfo * pcTmpInfo = atMtInfo;
		
		memset( atMtInfo, 0, sizeof( atMtInfo ) );
		u16 wLoop = 0;
		u16 wMtInfoNum = 0;
		for( wLoop = 0; wLoop < dwMtListSize; wLoop++ )
		{
			*pcTmpInfo = *(pcLocalMtInfo+wLoop);
			
			//达到80个，或到达末尾，则发送一条消息
			if( ( wLoop + 1 )%MAX_NTFNUM_ONEPACK == 0 || wLoop == dwMtListSize - 1 )
			{
				SetMsgTypeToJson( achJsonBuf, dwJsonBufLen, EV_MAU_MCU_MTLIST_NTF );
				SetMtListToJson( achJsonBuf, dwJsonBufLen, atMtInfo, (wMtInfoNum+1), bDel );
				SendMsgToCmu( achJsonBuf, dwJsonBufLen);
				
				wMtInfoNum = 0;
				memset( atMtInfo, 0, sizeof( atMtInfo ) );
				pcTmpInfo = atMtInfo;
				
				continue;
			}
			
			wMtInfoNum++;
			pcTmpInfo++;
			
		}
		
		delete [] pcLocalMtInfo;
	}
}

/*=============================================================================
函 数 名： ProcMtListNtf
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/11/11  5.0         rnn                   创建
=============================================================================*/
void CCMUSsnInstance::ProcMtListNtf( CMessage * const pcMsg )
{
	CMauMsg cSrvMsg( pcMsg->content, pcMsg->length );

	BOOL32 bDel = FALSE;
	CKdvMtInfo * pcMtInfo = NULL;
	if ( sizeof( CKdvMtInfo ) == ( cSrvMsg.GetMsgBodyLen() - sizeof( bDel ) )  )
	{
		pcMtInfo = (CKdvMtInfo*)cSrvMsg.GetMsgBody();
	}

	ProcMtListNtf( pcMtInfo, bDel );

	return;
}

/*=============================================================================
函 数 名： ProcDomainNtf
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/11/11  5.0         rnn                   创建
=============================================================================*/
void CCMUSsnInstance::ProcDomainNtf( CKdvDomainInfo * pcDomainInfo /* = NULL */, BOOL32 bDel /* = FALSE  */)
{
	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	u32 dwJsonBufLen = 0;
	u32 dwDomainListSize = 0;	
	
	if ( NULL != pcDomainInfo )  // 修改指定的DomainInfo
	{
		dwDomainListSize = 1;
		SetMsgTypeToJson( achJsonBuf, dwJsonBufLen, EV_MAU_MCU_DOMAINLIST_NTF );
		SetDomainListToJson( achJsonBuf, dwJsonBufLen, pcDomainInfo, dwDomainListSize, bDel );
		SendMsgToCmu( achJsonBuf, dwJsonBufLen);
		
	}
	else // 批量通知DomainInfo
	{
		dwDomainListSize = g_cMauVCApp.GetDomainSize();
		CKdvDomainInfo * pcLocalDomainInfo = new CKdvDomainInfo[dwDomainListSize];
		if ( NULL == pcLocalDomainInfo )
		{
			LogPrint( LOG_LVL_ERROR, MID_MAU_CMU, "[ProcDomainNtf]new CKdvDomainInfo err, return!\n" );
			return;
		}
		
		if ( !g_cMauVCApp.GetAllDomainList( pcLocalDomainInfo, dwDomainListSize ) )
		{
			LogPrint( LOG_LVL_ERROR, MID_MAU_CMU, "[ProcDomainNtf]GetAllDomainList err, return!\n" );
			delete [] pcLocalDomainInfo;
			return;
		}

		CKdvDomainInfo atDomainInfo[MAX_NTFNUM_ONEPACK];
		CKdvDomainInfo * pcTmpInfo = atDomainInfo;

		memset( atDomainInfo, 0, sizeof( atDomainInfo ) );
		u16 wLoop = 0;
		u16 wDomainInfoNum = 0;
		for( wLoop = 0; wLoop < dwDomainListSize; wLoop++ )
		{
			*pcTmpInfo = *(pcLocalDomainInfo+wLoop);
			
			//达到80个，或到达末尾，则发送一条消息
			if( ( wLoop + 1 )%MAX_NTFNUM_ONEPACK == 0 || wLoop == dwDomainListSize - 1 )
			{
				SetMsgTypeToJson( achJsonBuf, dwJsonBufLen, EV_MAU_MCU_DOMAINLIST_NTF );
				SetDomainListToJson( achJsonBuf, dwJsonBufLen, atDomainInfo, (wDomainInfoNum+1), bDel );
				SendMsgToCmu(achJsonBuf, dwJsonBufLen);
				
				wDomainInfoNum = 0;
				memset( atDomainInfo, 0, sizeof( atDomainInfo ) );
				pcTmpInfo = atDomainInfo;
				
				continue;
			}
			
			wDomainInfoNum++;
			pcTmpInfo++;
			
		}

		
		delete [] pcLocalDomainInfo;
	}

	return;
}

/*=============================================================================
函 数 名： ProcSrvDomainNtf
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/11/11  5.0         rnn                   创建
=============================================================================*/
void CCMUSsnInstance::ProcSrvDomainNtf( CMessage * const pcMsg )
{
	CMauMsg cSrvMsg( pcMsg->content, pcMsg->length );
	
	BOOL32 bDel = FALSE;
	CKdvDomainInfo * pcDomainInfo = NULL;
	if ( sizeof( CKdvDomainInfo ) == ( cSrvMsg.GetMsgBodyLen() - sizeof( bDel ) )  )
	{
		pcDomainInfo = (CKdvDomainInfo*)cSrvMsg.GetMsgBody();
	}
	
	ProcDomainNtf( pcDomainInfo, bDel );

	return;
}

/*=============================================================================
函 数 名： ProcPlatformDomainNtf
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/11/11  5.0         rnn                   创建
=============================================================================*/
void CCMUSsnInstance::ProcPlatformDomainNtf( CMessage * const pcMsg )
{
	CMauMsg cSrvMsg( pcMsg->content, pcMsg->length );
	
	BOOL32 bDel = FALSE;
	CKdvDomainInfo * pcDomainInfo = NULL;
	if ( sizeof( CKdvDomainInfo ) == ( cSrvMsg.GetMsgBodyLen() - sizeof( bDel ) )  )
	{
		pcDomainInfo = (CKdvDomainInfo*)cSrvMsg.GetMsgBody();
	}
	
	ProcDomainNtf( pcDomainInfo, bDel );

	return;
}

/*=============================================================================
函 数 名： ProcUserDomainNtf
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/11/11  5.0         rnn                   创建
=============================================================================*/
void CCMUSsnInstance::ProcUserDomainNtf( CMessage * const pcMsg )
{
	CMauMsg cSrvMsg( pcMsg->content, pcMsg->length );
	
	BOOL32 bDel = FALSE;
	CKdvDomainInfo * pcDomainInfo = NULL;
	if ( sizeof( CKdvDomainInfo ) == ( cSrvMsg.GetMsgBodyLen() - sizeof( bDel ) )  )
	{
		pcDomainInfo = (CKdvDomainInfo*)cSrvMsg.GetMsgBody();
	}
	
	ProcDomainNtf( pcDomainInfo, bDel );
	return;
}

void CCMUSsnInstance::SetCmuInfo(const TCmuInfo& tCmuInfo)
{
	m_tCmuInfo = tCmuInfo;
}

void CCMUSsnInstance::DaemonCmuMessageCb(CMessage * const pcMsg, CApp *pcApp)
{
	s8* achJsonBuf = (s8*)pcMsg->content;
	u32 dwJsonBufLen = MAXLEN_JSONBUF < pcMsg->length ? MAXLEN_JSONBUF : pcMsg->length;

	s8 abyIp[MAXLEN_IPADDR + 1] = {0};
	GetCmuIpFromJson(achJsonBuf, abyIp, MAXLEN_IPADDR);

	u16 wMsgType = 0;
	ConvertMsgTypeToEventId( achJsonBuf, wMsgType );
	if (MCU_MAU_CONFLIST_NTF == wMsgType)
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMU, "[%s]%s\n", __FUNCTION__, string(achJsonBuf, dwJsonBufLen).c_str());
	}

	OspPost(abyIp, strlen(abyIp), AID_MAU_CMUSSN_APPID, wMsgType, achJsonBuf, dwJsonBufLen);
}
