/*****************************************************************************
模块名      : MAU
文件名      : mauvc.cpp
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
#include "mauvc.h"
#include "evmau.h"
#include "eventnumau.h"
#include "evhduacs.h"
#include "resmgrssn.h"
#include "cmussn.h"
#include "hdussn.h"
#include "connectsa_api.h"
#include "mauutility.h"
#include "jsontranslate.h"
#include "rabbitmqwrapper.h"
#include <functional>
#include <sstream>
#include <bitset>

using namespace rabbitmqwrap;
using namespace std;

extern CResMgrSsnApp g_cResMgrSsnApp;
extern SEMHANDLE g_hPersonalTempSem;
CMauVCInstance::vecInitDealFun CMauVCInstance::s_InitDealfun_vec;

#define  TIMER_SPAN_MONITOR_ALIVE_CHECK    10000                    //监控保活定时器间隔数
#define  ALIVE_TIME                        30                       //30秒之内时间间隔算保活

#define CHECK_STATE(state, action)		\
if (state != CurState()){\
	LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s] Receive Msg<%d> in wrong State<%d>!\n",\
		__FUNCTION__, pcMsg->event, CurState());\
	action;}\

#define ReplyNack(errorid, reason)											\
LogPrint(LOG_LVL_WARNING, MID_MAU_VC, "[%s] %s\n", __FUNCTION__, reason);	\
tReqInfo.SetErrID(errorid);											\
cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));							\
SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cServMsg.GetServMsg(), cServMsg.GetServMsgLen())

//static void ProcRedisNotif(const s8* channel, const s8* message)
//{
//	json_t_ptr pjRoot;
//	if (!LoadJson(message, strlen(message), pjRoot, FALSE))
//	{
//		LogPrint(LOG_LVL_WARNING, MID_MAU_REDIS, "[%s] Redis Message Load Failed!\n", __FUNCTION__);
//	}
//	const s8* pchkey = NULL;
//	json_t* pjValue;
//	json_object_foreach(pjRoot, pchkey, pjValue)
//	{
//		vector<const s8*> vecKeySpilts;
//		s8* achTempKey = new s8[strlen(pchkey) + 1];
//		strcpy(achTempKey, pchkey);
//		s8* p = strtok(achTempKey, "/");
//		while (p)
//		{
//			vecKeySpilts.push_back(p);
//			p = strtok(NULL, "/");
//		}
//		// 如果是会议更新消息
//		if (vecKeySpilts.size() == 2 && strcmp(vecKeySpilts[0], "conf") == 0)
//		{
//			const s8* pchValue = json_string_value(pjValue);
//			if (strcmp(pchValue, "update") == 0)
//			{
//				json_t_ptr pjRoot = json_object();
//				SetConfE164ToJsonObj(pjRoot, vecKeySpilts[1]);
//				s8* pchJson = json_dumps(pjRoot, 0);
//				OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), REDIS_MAU_CONFINFO_UPDATE_NTF, pchJson, strlen(pchJson));
//				json_free(pchJson);
//			}
//		}
//		delete[] achTempKey;
//	}
//}

void ConnectSACB( BOOL32 bResult, u64 BmcSn, const s8* szEventStr, u8 * pMsgBuf,u32 dwMsgLen )
{
	LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[ConnectSACB] Result is : %d\n BmcSn is %d\n, szEventStr is %s\n", bResult, BmcSn, szEventStr);
	return;
}

/*=============================================================================
函 数 名： CMauVCInstance
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
CMauVCInstance::CMauVCInstance()
{
	m_dwInitState = 0;
	m_bHasMonitorCheckTimer = FALSE;
	m_strReqId = "";
}


/*=============================================================================
函 数 名： ~CMauVCInstance
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
CMauVCInstance::~CMauVCInstance()
{
	if (m_bHasMonitorCheckTimer)
	{
		KillTimer(MAU_MONITOR_ALIVE_CHECK_TIMER);
		m_bHasMonitorCheckTimer = FALSE;
	}
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
void CMauVCInstance::DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp )
{
	CMauVCApp* pcMauVcApp = dynamic_cast<CMauVCApp*>(pcApp);
	if (!pcMauVcApp) return;

	switch ( pcMsg->event )
	{
	case OSP_POWERON:
		DaemonProcPowerOn(pcMsg, pcMauVcApp);
		break;
		//打印命令
	case MAU_PRINTINFO_CMD:
		DaemonProcPrintCmd(pcMsg, pcMauVcApp);
		break;
		/**************************************** APDB回复操作 ********************************/
	case MAU_FINISH_AP_OPR_NTF:
		DaemonProcApFinishOprNtf(pcMsg, pcMauVcApp);
		break;
		/**************************************** scheduledconfinfodb回复操作 ********************************/
	case MAU_FINISHOPR_SCHEDULEDCONF_NTF:
		DaemonProcMoveScheduledConfToRedisNtf(pcMsg, pcMauVcApp);
		break;
		/**************************************** TemplateDBDB回复操作 ********************************/
		//完成加载数据库的模板通知消息
	case MAU_FINISH_LOADDB_NTF:
		DaemonProcLoadDBResNtf(pcMsg, pcMauVcApp);
		break;
		//完成模板添加通知
	case MAU_FINISH_ADDTEMP_NTF:
		DaemonProcFinishAddTemp(pcMsg, pcMauVcApp);
		break;
		//完成模板删除通知
	case MAU_FINISH_DELTEMP_NTF:
		DaemonProcFinishDelTemp(pcMsg, pcMauVcApp);
		break;
		//完成模板修改通知
	case MAU_FINISH_MODTEMP_NTF:
		DaemonProcFinishModTemp(pcMsg, pcMauVcApp);
		break;
		//完成查找指定模板通知
	case MAU_FINISH_SEARCHTEMP_NTF:
		DaemonProcFinishSearchTemp(pcMsg, pcMauVcApp);
		break;
		/**************************************** VMeetingDB回复操作 *****************************/
		//完成meeting操作完成加载通知
	case MAU_FINISH_MEETING_OPR_NTF:
		DaemonProcMeetingFinishOprNtf(pcMsg, pcMauVcApp);
		break;

		/**************************************** 会管操作 ********************************/
		// 模板相关
	case CM_MAU_CREATETEMPLATE_REQ:
	case CM_MAU_MDFTEMPLATE_REQ:
	case CM_MAU_DELTEMPLATE_REQ:
		DaemonProcTemplateOpr( pcMsg );
		break;
		//个人模板相关
	case MAU_GETPERSONALTEMPFROMCM_TIMER:
		DaemonProcGetPersonalTempTimer(pcMsg);
		break;
	case CM_MAU_ALLPERSONALCONFTEMPLATE_ACK:
		DaemonprocAllPersonalTmpAckMsg(pcMsg);
		break;
	case CM_MAU_PERSONALCONFTEMPLATE_NTF:
	case CM_MAU_DELPERSONALCONFTEMPLATE_NTF:
		DaemonProcPersonalTemplate(pcMsg);
		break;
		// 预约会议相关
	case CM_MAU_ADDBOOKCONF_REQ:
		DaemonProcBatchAddBookConf(pcMsg);
		break;
	case CM_MAU_MDFBOOKCONF_REQ:
		DaemonProcBatchModBookConf(pcMsg);
		break;
	case CM_MAU_DELBOOKCONF_REQ:
		DaemonProcBatchDelBookConf(pcMsg);
		break;
		// 结束会议
	case CM_MAU_RELEASECONF_REQ:
		DaemonProcCMReleaseConfReq( pcMsg );
		break;
		// 预约会议已召开通知
	case CM_MAU_BOOKCONFONGOING_NTF:
		DaemonProcScheduledConfAlreadyOngoingNtf( pcMsg );
		break;
	case CM_MAU_DELAYCONF_REQ:
		DaemonProcConfDelay( pcMsg );
		break;
	case CM_MAU_MONITORKEEPALIVE_NTF:
		DaemonProcMonitorKeepAliveNtf( pcMsg );
		break;
		/**************************************** 会管操作电视墙消息********************************/
	case CM_MAU_GETHDUINFO_CMD:
	case CM_MAU_SETHDUVOLUME_REQ:
	case CM_MAU_GETHDUINFOBYMOID_REQ:
		DeamonProcCmHduMsg( pcMsg );
		break;
		/**************************************** 终端创会、入会处理 ***********************/
		//终端召集会议/终端通过公共模板创会，获取到公共模板信息之后/个人模板创会，用于级联
	case NU_MAU_CREATECONF_REQ:
	case NU_MAU_CREATECONFBYTEMP_REQ:
	case MAU_MAU_MTCREATECONF_REQ:
		DaemonProcMtCreateConf(pcMsg);
		break;
	case MAU_MAU_MTCREATECONFBYBOOKCONF_REQ:
		DaemonProcMtJoinBookConf(pcMsg);
		break;
		/****************************************公共处理**********************************/
		// 创建会议
	case CM_MAU_CREATECONF_REQ:
	case MAU_MAU_CREATECONF_REQ:
		DaemonProcCreateConf( pcMsg );
		break;
		// 会管通过公共模板创建会议|终端加入公共模板
	case CM_MAU_CREATECONFBYTEMP_REQ:
	case MAU_MAU_MTCREATECONFBYPUBLICTEMP_REQ:
		DaemonProcCreateConfByPublicTemp(pcMsg);
		break;
		/**************************************** CMU操作 **********************************/
	case MAU_CMUCONNECT_NTF:
		DaemonProcMpcConnectNtf( pcMsg );
		break;
	case MCU_MAU_CONFLIST_NTF:
		DaemonProcCmuConfListNtf( pcMsg );
		break;
	case MAU_CMUDISCONNECT_NTF:
		DaemonProcMpcDisConnectNtf( pcMsg );
		break;
	case MCU_MAU_RELEASECONF_NTF:
		DaemonProcReleaseConfNtf(pcMsg);
		break;
	case MCU_MAU_RESTORECONF_ACK:
	case MCU_MAU_CREATECONF_ACK: 
		DaemonProcCreateConfAck(pcMsg);
		break;
	case MCU_MAU_CREATECONF_NACK:
		DaemonProcCreateConfNack(pcMsg);
		break;
	case MCU_MAU_RESTORECONF_NACK:
		DaemonProcRecoverConfNack(pcMsg);
		break;
	case MCU_MAU_ADDMT_ACK:
	case MCU_MAU_ADDMT_NACK:
	case MCU_MAU_DELAYCONF_REQ:
	case MCU_MAU_SUBCONFDURATION_REQ:
	case MCU_MAU_CONFSYN_NTF:
	case MCU_MAU_CONFSTATECHANGE_NTF:
	case MCU_MAU_MODIFYCONFNAME_REQ:
		DaemonProcCmuMsg( pcMsg );
		break;

		/**************************************** hdupool交互消息********************************/
	case HDUPOOL_ACS_HDUSTATUS_NOTIF:
	case HDUPOOL_ACS_CHANGEHDUVOLUME_NOTIF:
	case HDUPOOL_ACS_HDUONLINE_NOTIF:
	case HDUPOOL_ACS_HDUOFFLINE_NOTIF:
	case HDUPOOL_ACS_HDUCHANNEL_STATUS_NOTIF:
	case HDUPOOL_MAU_DISCONNECT_NTF:
		DeamonProcHduPoolMsg( pcMsg );
		break;

		/**************************************** 其他处理 ********************************/
		//GK/MCC断链,做相应的清理工作
	case MAU_NUDISCONNECT_NTF:
	case MAU_MCCDISCONNECT_NTF:
		DaemonProcDisconnectNtf( pcMsg );
		break;

	//检测会议是否结束
	case MAU_CHECKCONF_TIMER:
		DaemonProcCheckConfTimer(pcMsg);
		break;

	case MAU_RECORVERCONF_TIMER:
		DaemonProcRecoverConfTimer(pcMsg);
		break;
	case MAU_MAU_ADDTEMPLATE_NTF:
		DaemonProcTempConfNtf(pcMsg);
		break;
	case MAU_MAU_STATECHECK_NTF:
		DaemonProcOngoingConfNtf(pcMsg);
		break;
	case MAU_MAU_ENABLE_CMU_CMD:
	case MAU_MAU_DISABLE_CMU_CMD:
		DaemonEnableCmu(pcMsg);
		break;
	case BMC_MAU_CONF_LICENSE_NUM:
		DaemonPlatformConfLimit(pcMsg);
		break;
	case CM_CMU_MESSAGE_CB:
		DaemonProcCmtoCmuMsg(pcMsg);
		break;
	case CMU_CM_CONFOPR_NTF:
		DaemonProcCmutoCmMsg(pcMsg);
		break;
	case MAU_MONITOR_ALIVE_CHECK_TIMER:
		CheckAllMonitorAlive();			
		break;
	case MAU_CHECK_REDISDATA_TIMER:
		DaemonProcCheckRedisData(pcMsg);
		break;
	case MAU_INIT_REDISCONF_TIMER:
		DaemonProcInitRedisConfTimer(pcMsg, pcMauVcApp);
		break;
	case MAU_INIT_UPUADDBOOKCONF_TIMER:
		DaemonProcInitUpuAddBookConfTimer(pcMsg);
		break;
	case MAU_LOADUSERDOMAININFO_TIMER:
		DaemonProcLoadUserDomainInfoTimer(pcMsg);
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
void CMauVCInstance::DaemonProcPowerOn( const CMessage *pcMsg, CMauVCApp* pcApp)
{
	//状态初始化
	NextState( STATE_IDLE );

	//判断是否使用数据库存储模板信息
	if( !DBInit(pcApp->GetTemplateDBConnectInfo().GetDBEngineType() ) )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcPowerOn] DataBase Initial Failed!!!\n" );
		return ;
	}

	//模板存储操作类初始化
	pcApp->m_cTemplateDBOpr.Initial(pcApp->m_tTemplateDBInfo);

	//会议信息存储操作类初始化
	pcApp->m_cConfInfoDBOpr.Initial(pcApp->m_tConfInfoDBInfo);

	//会管数据库操作类初始化
	pcApp->m_cVMeetingDBOpr.Initial(pcApp->m_tVMeetingDBInfo);

	// wy[2013/12/05]预约会议详细信息数据库操作类初始化
	pcApp->m_cScheduledDBOpr.Initial(pcApp->m_tScheduledConfDBInfo);

	// ap数据库操作类初始化
	pcApp->m_cApInfoDBOpr.Initial(pcApp->m_tApDBInfo, 1);
	//注册初始化函数
	RegInitDealFun();
	//等待初始化完成
	OspDelay( 1000 );
	StartServer(pcApp);
	////加载模板信息到内存中
	//pcApp->m_cTemplateDBOpr.LoadDBData();

	//初始化数据库中信息,删除会议信息
	pcApp->m_cConfInfoDBOpr.ClearDBData();

// 	//会议状态校验
// 	g_cMauVCApp.m_cVMeetingDBOpr.StateCheck( tDBMsg );
	
	//mq初始化之后，css向CM发送请求个人模板
	SetTimer(MAU_GETPERSONALTEMPFROMCM_TIMER , 1000);

	SetTimer(MAU_CHECK_REDISDATA_TIMER, CHECK_REDISDATA_SPAN);
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
void CMauVCInstance::InstanceEntry(CMessage * const pcMsg) {}

/*=============================================================================
函 数 名： DaemonProcLoadDBResNtf
功    能： 加载数据库数据结果通知消息处理
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauVCInstance::DaemonProcLoadDBResNtf(const CMessage * pcMsg, CMauVCApp* pcApp)
{
	u16 wRet = *(u16*)pcMsg->content;
	//加载数据库数据失败
	if( 0 != wRet )
	{
		// 尝试重新加载
		pcApp->m_cTemplateDBOpr.LoadDBData();
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcLoadDBResNtf] Load db failed!!\n" );
		return ;
	}
	
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcLoadDBResNtf] Load db data successful!\n");

	++g_cMauVCApp.m_byInitCount;
	StartServer(pcApp);
}

s32 CMauVCInstance::StartServer(CMauVCApp* pcApp)
{
	if (STATE_NORMAL == CurState())
	{
		return FALSE;
	}
	//数据库操作
	if (g_cMauVCApp.m_byInitCount < s_InitDealfun_vec.size())
	{
		InitDealFun dealFun = s_InitDealfun_vec[g_cMauVCApp.m_byInitCount];
		if (dealFun != NULL)
		{
			(this->*dealFun)();
		}
		return FALSE;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[StartServer]Init sucess, InitCount: %d\n", g_cMauVCApp.m_byInitCount - 1);
	//状态切换
	NextState(STATE_NORMAL);

	SetTimer(MAU_CHECKCONF_TIMER, CHECKCONF_TIME_SPAN);
	SetTimer(MAU_RECORVERCONF_TIMER, g_cMauVCApp.GetMcuHeartBeatTimeSpan() * 3 * 1000);
	SetTimer(MAU_MONITOR_ALIVE_CHECK_TIMER, TIMER_SPAN_MONITOR_ALIVE_CHECK);

	////rnn[2014/10/14]获取ap数据库相关信息
	//pcApp->m_cApInfoDBOpr.GetPlatFormDomainInfo();
	//pcApp->m_cApInfoDBOpr.GetServDomainInfo();
	//pcApp->m_cApInfoDBOpr.GetUserDomainInfo();
	//pcApp->m_cApInfoDBOpr.GetDomainHasXmpu();
	//pcApp->m_cApInfoDBOpr.GetMtInfo();
	////end
	////wangli[2016/7/11]获取ap数据库HDU对应moid,用户域moid
	//pcApp->m_cApInfoDBOpr.GetHduMoidAndUserDomainMoid();

	//发送初始化消息
	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, CInstance::DAEMON), OSP_POWERON);
	OspPost(MAKEIID(AID_MAU_BMCSSN_APPID, CInstance::DAEMON), OSP_POWERON);
	OspPost(MAKEIID(AID_MAU_NUSSN_APPID, CInstance::DAEMON), OSP_POWERON);
	OspPost(MAKEIID(AID_MAU_MCCSSN_APPID, CInstance::DAEMON), OSP_POWERON);
	OspPost(MAKEIID(AID_MAU_RESMGRSSN_APPID, CInstance::DAEMON), OSP_POWERON);
	OspPost(MAKEIID(AID_MAU_UPUSSN_APPID, CInstance::DAEMON), OSP_POWERON);
	OspPost(MAKEIID(AID_MAU_MAUMAUSSN_APPID, CInstance::DAEMON), OSP_POWERON);
	OspPost(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON), OSP_POWERON);
	OspPost(MAKEIID(AID_MAU_CMCMUSSN_APPID, CInstance::DAEMON), OSP_POWERON);
	OspPost(MAKEIID(AID_MAU_CONNMSADPSSN_APPID, CInstance::DAEMON), OSP_POWERON);
	OspPost(MAKEIID(AID_MAU_HDUSSN_APPID, CInstance::DAEMON), OSP_POWERON);
	OspPost(MAKEIID(AID_MAU_BMCLICENSE_APPID, CInstance::DAEMON), OSP_POWERON);
	//OspPost(MAKEIID(AID_MAU_NPPSSN_APPID, CInstance::DAEMON), OSP_POWERON);

	THduPoolInfo tHduInfo;
	tHduInfo.m_dwIp = INET_ADDR(pcApp->m_achHduPoolIP);
	tHduInfo.m_wPort = HDURESPOOL_LISTEN_PORT;
	OspPost(MAKEIID(AID_MAU_HDUSSN_APPID, CInstance::DAEMON), MAU_HDUPOOL_CONNECT_CMD, &tHduInfo, sizeof(tHduInfo));

	return TRUE;
}


void CMauVCInstance::SetInitState(EmInitState emState)
{
	m_dwInitState |= (1 << emState);
}

BOOL32 CMauVCInstance::ReportCharge(const string& chargeJson)
{
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[%s]PostMsgToBMC Conf Charge Content:\n", __FUNCTION__);
	LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, chargeJson.c_str(), chargeJson.length());
	PostMsgToBMC(MODULE_EVENTSTR_MCU_CHARGE, (u8*)chargeJson.c_str(), chargeJson.length());
	return TRUE;
}

BOOL32 CMauVCInstance::NoticeCMStopConfMonitors(const s8* pchConfE164)
{
	string strStopMonitorsJson;
	if (!g_cMauVCApp.GetConfStopMonitorsJson(pchConfE164, strStopMonitorsJson))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetConfMonitorsInfo<%s>Fail\n", __FUNCTION__, pchConfE164);
		return FALSE;
	}
	else if (!strStopMonitorsJson.empty())
	{
		CMauMsg cMauMsg;
		cMauMsg.SetMsgBody(strStopMonitorsJson.c_str(), strStopMonitorsJson.length());
		g_cMauVCApp.SendMsgToCmCmuSsn(CMU_CM_CONFOPR_NTF, cMauMsg.GetServMsg(), cMauMsg.GetServMsgLen());
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]stopMonitorJson:\n", __FUNCTION__);
		LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, strStopMonitorsJson.c_str(), strStopMonitorsJson.length(), FALSE);
	}
	return TRUE;
}

BOOL32 CMauVCInstance::IsAllInited()
{
	return (m_dwInitState + 1 == 1 << emInitStateAll) ? TRUE : FALSE;
}

void CMauVCInstance::DaemonProcLoadUserDomainInfoTimer(const CMessage *pcMsg)
{
	g_cMauVCApp.m_cApInfoDBOpr.GetUserDomainInfo();	
}

void CMauVCInstance::DaemonProcInitUpuAddBookConfTimer(const CMessage *pcMsg)
{
	KillTimer(MAU_INIT_UPUADDBOOKCONF_TIMER);

	if (UpuAddAllBookConf())
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]upu add allbookconf success.\n", __FUNCTION__);
	}
	else
	{
		SetTimer(MAU_INIT_UPUADDBOOKCONF_TIMER, INIT_UPUADDBOOKCONF_SPAN);
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]upu add allbookconf fail, settimer try again\n", __FUNCTION__);
	}
}

BOOL32 CMauVCInstance::UpuAddAllBookConf()
{
	vector<CRedisBookConf> vecRedisBookConf;
	if (!g_cMauVCApp.GetAllBookConf(vecRedisBookConf))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]get allBookconf fail\n", __FUNCTION__);
		return FALSE;
	}

	vector<CRedisBookConf>::iterator iterConf = vecRedisBookConf.begin();
	for (; iterConf != vecRedisBookConf.end(); ++iterConf)
	{
		CKdvDomainInfo tDomainInfo;
		if (g_cMauVCApp.GetDomainInfo(iterConf->GetDomainMOID(), tDomainInfo))
		{
			UpuAddConf(iterConf->GetConfE164(),
				TUpuConfInfo::emScheConf,
				iterConf->GetDomainMOID(),
				tDomainInfo.GetSDomainMOID());
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Upu addBookconf<%s:%s:%s>\n",
				__FUNCTION__, iterConf->GetConfE164(), iterConf->GetDomainMOID(), tDomainInfo.GetSDomainMOID());
		}
		else
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetDomain<%s> fail\n",
				__FUNCTION__, iterConf->GetDomainMOID());
		}
	}
	return TRUE;
}

/*=============================================================================
函 数 名： DaemonProcLoadDBResNtf
功    能： 加载数据库数据结果通知消息处理
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauVCInstance::DaemonProcFinishAddTemp(const CMessage * pcMsg, CMauVCApp* pcApp)
{
	//消息体获取
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	
	//数据库操作失败
	if(ERR_DB_SUCCESS != tReqInfo.GetErrID() )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcFinishAddTemp] db opr add template failed!\n" );
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcFinishAddTemp] reply nack msg to require<ID:%d,AppID:%d InstID:%d>\n",tReqInfo.GetUniqueID(), GETAPP(tReqInfo.GetReqSrcID()), GETINS(tReqInfo.GetReqSrcID()) );
		
		//删除内存中对应的信息
		if (!pcApp->DelTempData(tReqInfo.GetConfE164()))
		{
			//内存信息删除失败 日志记录
			LogPrint(LOG_LVL_WARNING, MID_MAU_VC, "[DaemonProcFinishAddTemp] Del Template %s store in memory failed!\n", tReqInfo.GetConfE164());
		}
		
		//NACK回复
		SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID()+2, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
		return ;
	}
	
	SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID() + 1, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
	
	return;
}

/*=============================================================================
函 数 名： DaemonProcFinishDelTemp
功    能： 完成删除指定模板通知消息处理
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauVCInstance::DaemonProcFinishDelTemp(const CMessage * pcMsg, CMauVCApp* pcApp)
{
	//消息体获取
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	const u8* pbyBody = cServMsg.GetMsgBody();
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyBody;
	pbyBody += sizeof(TReqHeadInfo);
	
	//数据库执行删除模板出错
	if (ERR_DB_SUCCESS != tReqInfo.GetErrID())
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcFinishDelTemp] db opr delete template<%s> failed!\n", tReqInfo.GetConfE164() );
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcFinishDelTemp] reply nack msg to require<ID:%d,AppID:%d InstID:%d>\n",
			tReqInfo.GetUniqueID(), GETAPP(tReqInfo.GetReqSrcID()), GETINS(tReqInfo.GetReqSrcID()) );
	
		SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID()+2, cServMsg.GetServMsg(),cServMsg.GetServMsgLen() );
		return ;
	}
	
	u16 wCount = *(const u16*)(pbyBody);
	pbyBody += sizeof(u16);
	
	const s8* pchE164 = (const s8*)pbyBody;
	for (u16 wIdx = 0; wIdx < wCount; ++wIdx)
	{
		pcApp->DelTempData(pchE164);
		pchE164 += MAXLEN_CONFE164 + 1;
	}

	SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 1, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
}


/*=============================================================================
函 数 名： DaemonProcFinishModTemp
功    能： 完成修改指定模板通知消息处理
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauVCInstance::DaemonProcFinishModTemp(const CMessage * pcMsg, CMauVCApp* pcApp)
{
	//消息体获取
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	const u8* pMsg = cServMsg.GetMsgBody();
	const TReqHeadInfo& tReqInfo = *(const TReqHeadInfo*)pMsg;
	pMsg += sizeof(TReqHeadInfo);
	const TConfInfo& tTmpConf = *(const TConfInfo*)pMsg;
	pMsg += sizeof(TConfInfo);
	const TConfInfoEx& tTmpConfEx = *(const TConfInfoEx*)pMsg;
	// 获得终端信息
//     s8 *pszAliaseBuf = NULL;
//     u16 wAliasBufLen  = 0;
//     wAliasBufLen = ntohs( *(u16*)(cServMsg.GetMsgBody() + sizeof(TConfInfo) + sizeof(TReqHeadInfo)) );
//     pszAliaseBuf = (char*)(cServMsg.GetMsgBody() + sizeof(TConfInfo) + sizeof(u16) + sizeof(TReqHeadInfo) );
// 	
// 	//获得画面合成模板
// 	TVmpModule tVmpModule = *(TVmpModule *)(cServMsg.GetMsgBody() + wAliasBufLen + sizeof(TConfInfo) + sizeof(u16) + sizeof(TReqHeadInfo) );
// 	//获得轮询模板
// 	TPollModule tPollModule = *(TPollModule *)(cServMsg.GetMsgBody() + sizeof(TVmpModule) + wAliasBufLen + sizeof(TConfInfo)
//											+ sizeof(u16) + sizeof(TReqHeadInfo) );

//	TTemplateInformation tTempInfo = *(TTemplateInformation*)(cServMsg.GetMsgBody()+sizeof(TReqHeadInfo));

	//数据库执行修改模板出错
	if (ERR_DB_SUCCESS != tReqInfo.GetErrID())
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcFinishModTemp] db opr update template<%s> failed!\n", tTmpConf.GetConfE164() );
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcFinishModTemp] reply nack msg to require<ID:%d,AppID:%d InstID:%d>\n",
			tReqInfo.GetUniqueID(), GETAPP(tReqInfo.GetReqSrcID()), GETINS(tReqInfo.GetReqSrcID()) );
		//NACK 回复
		SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID()+2, cServMsg.GetServMsg(),cServMsg.GetServMsgLen() );
		return ;
	}

	TVideoStreamCap atVideoStreamCap[MAX_CONF_CAP_EX_NUM];
	u8 byCapNum = MAX_CONF_CAP_EX_NUM;
	tTmpConfEx.GetMainStreamCapEx(atVideoStreamCap, byCapNum);

	//同步修改内存中的模板信息
	CTempCacheData tTempData;
	if (pcApp->GetTemplateInfoByE164(tTmpConf.GetConfE164(), tTempData))
	{
		tTempData.SetConfName( tTmpConf.GetConfName() );
		tTempData.SetBitRate(atVideoStreamCap[0].GetMaxBitRate());
		tTempData.SetResolution(atVideoStreamCap[0].GetResolution());
		tTempData.SetMediaType(atVideoStreamCap[0].GetMediaType());
		tTempData.SetSatDCastMode( tTmpConf.GetConfAttrb().IsSatDCastMode() );
		tTempData.SetPublicConf( tTmpConf.GetConfAttrb().GetPublicConf() );
		tTempData.SetOpenMode(tTmpConf.GetConfAttrb().GetOpenMode());
		tTempData.SetMixMode(tTmpConf.GetStatus().GetMixerMode());
		tTempData.SetDuration( tTmpConf.GetDuration() );
		tTempData.SetEncryptMode( tTmpConf.GetConfAttrb().GetEncryptMode() );
		tTempData.SetDualMode( tTmpConf.GetConfAttrb().GetDualMode() );
		tTempData.SetHasXmpuSer( tTmpConf.GetConfAttrb().IsHasXmpuSer() ? 1 : 0 );
		tTempData.SetConfNoDisturb( tTmpConf.GetConfAttrb().IsConfNoDisturb() ? 1 : 0 );
		tTempData.SetPortModeConf(tTmpConf.IsPortMediaConf());
		tTempData.SetMaxJoinMtNum(tTmpConf.GetMaxJoinedMtNum());
		pcApp->ModTempData(tTempData);
	}
	else
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_VC, "[DaemonProcFinishModTemp] can't find template<%s> in memory,reply nack msg!\n",tTmpConf.GetConfE164() );
		SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID()+2, cServMsg.GetServMsg(),cServMsg.GetServMsgLen() );
	}

	SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID()+1, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
}

void CMauVCInstance::DaemonProcFinishSearchTemp(const CMessage * pcMsg, CMauVCApp* pcApp)
{
	CPrintFunInOut cPrintFun(__FUNCTION__, MID_MAU_VC);

	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	const u8* pBuff = cServMsg.GetMsgBody();
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)(pBuff);
	if (ERR_DB_SUCCESS != tReqInfo.GetErrID())
	{
		ReplyNack(tReqInfo.GetErrID(), "db operate failed");
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]reply nack msg to"
			"require<ID:%d,AppID:%d InstID:%d>\n", __FUNCTION__,
			tReqInfo.GetUniqueID(), GETAPP(tReqInfo.GetReqSrcID()),
			GETINS(tReqInfo.GetReqSrcID()));
		return;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Public Template Conf E164: %s,"
		" mtE164: %s, domainMoid: %s\n", __FUNCTION__,
		tReqInfo.GetConfE164(), tReqInfo.GetSrcMtE164(), tReqInfo.GetDomainMOID());

	pBuff += sizeof(TReqHeadInfo);
	u16 wDuration = *(u16*)pBuff;
	pBuff += sizeof(wDuration);
	pBuff += sizeof(u16);
	const s8* pbyJsonBuff = (const s8*)pBuff;

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads(pbyJsonBuff, JSON_DISABLE_EOF_CHECK, &jReason);
	if (!pjRoot)
	{
		ReplyNack(ERR_INFO_INVALID, "json_loads fail");
		return;
	}
	SetCreatorNameToJsonObj(pjRoot, tReqInfo.GetMtAlias());
	SetCreatorMoidToJsonObj(pjRoot, tReqInfo.GetMtMOID());
	SetConfE164ToJsonObj(pjRoot, "");
	//增加永久会议，0不再做判断
	//if (0 == wDuration)
	//{
	//	wDuration = DEFAULT_CONF_DURATION;
	//	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcFinishSearchTemp]tTmpConf.GetDuration() is 0 hour, Set Duration to 4 hour.\n");
	//}
	//if (wDuration > (72 * 60))
	//{
	//	wDuration = 72 * 60;
	//	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcFinishSearchTemp] tTmpConf.GetDuration() > 72 hour, Set Duration to 72 hour.\n");
	//}

	//根据源请求消息做相应处理
	switch( tReqInfo.GetReqEventID() )
	{
		//源消息为终端通过模板创建会议
		case NU_MAU_JOINCONF_REQ:
		{
			//终端通过公共模板创会，会议时长为默认4小时
			wDuration = DEFAULT_CONF_DURATION;
			SetConfDurationToJsonObj(pjRoot, wDuration);
			s8 abyPassword[MAXLEN_PWD + 1] = { 0 };
			GetConfPasswordFromJson(pjRoot, abyPassword, sizeof(abyPassword) - 1);
			if (!CheckPublicTempPassword(tReqInfo.GetConfPassword(), abyPassword))
			{
				ReplyNack(ERR_MCU_INVALIDPWD, "mt join publicTemp, but pwd err");
				return;
			}
			
			//判断模板中是否有设置主席/发言人
 			TMtAlias tMtAlias;
 			tMtAlias.SetE164Alias( tReqInfo.GetSrcMtE164() );
			SetChairAliasToJsonObj(pjRoot, tMtAlias);
			SetSpeakerAliasToJsonObj(pjRoot, tMtAlias);
			// 将呼叫终端加入终端列表
	 		u16 wBitRate = tReqInfo.GetMtCallBitrate();
			SetMtAliasListToJson(pjRoot, &tMtAlias, &wBitRate, 1);

			json_str pchJsonTemp(pjRoot); 
			cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
			cServMsg.CatMsgBody(pchJsonTemp, strlen(pchJsonTemp));
			OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON),
				MAU_MAU_MTCREATECONF_REQ, &cServMsg, cServMsg.GetServMsgLen());
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]get public temp success, "
				"go to nu_mau_createconf_req stage\n", __FUNCTION__);
		}
		break;
	case CM_MAU_CREATECONFBYTEMP_REQ:
		{
			//rnn[2014/12/02]会管通过模板创会改成会管添加meeting和vmeeting的记录
			//tReqInfo.GetMtReqID()用来表示meetingid，前面已设置
			SetConfDurationToJsonObj(pjRoot, wDuration);
			
			s32 nMeetingId = tReqInfo.GetMtReqID();
			SetMeetingIDToJsonObj(pjRoot, nMeetingId);
			json_str abyBuff(pjRoot);
			cServMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
			cServMsg.CatMsgBody(abyBuff, strlen(abyBuff));
			OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), 
					MAU_MAU_CREATECONF_REQ, &cServMsg,cServMsg.GetServMsgLen());
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]get public temp success, "
				"go to last createConf stage\n", __FUNCTION__);
		}
		break;
	default:
		LogPrint( LOG_LVL_WARNING, MID_MAU_VC, "[%s] receive wrong require<%d>"
			" when finish get template from db!\n",
			__FUNCTION__, tReqInfo.GetReqEventID());
		break;
	}
}

/*=============================================================================
函 数 名： DaemonProcTemplateOpr
功    能： 会管对模板操作
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauVCInstance::DaemonProcTemplateOpr( CMessage * const pcMsg )
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	
	//请求头信息
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	//用户信息
	s8 * pchJsonBuf = (s8*)( cServMsg.GetMsgBody()+sizeof(TReqHeadInfo) );

	u32 dwPrintBuffLen = strlen(pchJsonBuf);
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[CMauVCInstance::DaemonProcTemplateOpr] cm json len is %d, text is:.\n", dwPrintBuffLen );
	LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, pchJsonBuf, dwPrintBuffLen);
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[CMauVCInstance::DaemonProcTemplateOpr]end\n" );

	//用户有效性检验
	CKdvDomainInfo cDomainInfo;
	if( !g_cMauVCApp.IsDomainExist( tReqInfo.GetDomainMOID() ) || !g_cMauVCApp.GetDomainInfo( tReqInfo.GetDomainMOID(),cDomainInfo ) || !cDomainInfo.IsUsedFlag() )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[%s]Domain MOID:%s is not exist or is disabled!\n ",
			__FUNCTION__, tReqInfo.GetDomainMOID());
		
		//域信息不存在、获取不到域信息，域已经停用
		tReqInfo.SetErrID(ERR_DOMAIN_INVALID);
		cServMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
		SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
		return ;
	}

	switch ( pcMsg->event )
	{
	case CM_MAU_CREATETEMPLATE_REQ:
		{
			s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };

			//生成E164号
			if( !g_cMauVCApp.ConstructTemplateE164( cDomainInfo.GetSDomainMOID(), achConfE164) )
			{
				LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcTemplateOpr] construct e164 failed when DomainGUID<%s> add template\n ", 
					cDomainInfo.GetDomainGUID() );
				//生成E164出错
				tReqInfo.SetErrID(ERR_CONSTRUCT_CONFE164);
				cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
				SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID() + 2, cServMsg.GetServMsg(),cServMsg.GetServMsgLen() );
				return ;
			}

			u32 dwJsonLen = MAXLEN_JSONBUF;
			SetConfE164ToJson( pchJsonBuf, dwJsonLen, achConfE164 );
			//SetHasXmpuSerToJson( pchJsonBuf, dwJsonLen, cDomainInfo.HasXmpuServer() ? 1 : 0 );
			
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcTemplateOpr] add template, E164<%s>, has xmpu.%d.\n", achConfE164, cDomainInfo.HasXmpuServer() ? 1 : 0 );

			dwPrintBuffLen = strlen(pchJsonBuf);
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[CREATETEMPLATE_REQ] cm json len is %d, text is:.\n", dwPrintBuffLen );
			LongLogPrint(LOG_LVL_DETAIL, MID_MAU_VC, pchJsonBuf, dwPrintBuffLen);
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[CREATETEMPLATE_REQ]end\n" );

			TConfInfo tConfInfo;
			TConfInfoEx tConfInfoEx;

			GetCreateConfParamFromJson(pchJsonBuf, tConfInfo);
			GetConfInfoExFromJson(pchJsonBuf, tConfInfoEx);

			TVideoStreamCap atVideoStreamCap[MAX_CONF_CAP_EX_NUM];
			u8 byCapNum = MAX_CONF_CAP_EX_NUM;
			tConfInfoEx.GetMainStreamCapEx(atVideoStreamCap, byCapNum);


			// 模板重名处理
			BOOL32 bConfNameModify = FALSE;
			s8 achConfName[MAXLEN_CONFNAME + 1] = { 0 };
			if (!g_cMauVCApp.ConfNameRepeatProc(cDomainInfo, tConfInfo.GetConfName(),
				tConfInfo.GetConfE164(), achConfName, MAXLEN_CONFNAME, bConfNameModify))
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]ConfNameRepeatProc Fail\n ", __FUNCTION__);
				tReqInfo.SetErrID(ERR_DB_OPR);
				cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
				SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID() + 2, cServMsg.GetServMsg(),cServMsg.GetServMsgLen() );
			}
			else if (bConfNameModify)
			{	
				tConfInfo.SetConfName(achConfName);
				SetConfNameToJson( pchJsonBuf, achConfName, dwJsonLen );
			}

			CTempCacheData cTempCacheData;
			cTempCacheData.SetConfE164(tConfInfo.GetConfE164());
			cTempCacheData.SetConfName(tConfInfo.GetConfName());
			cTempCacheData.SetDomainName(cDomainInfo.GetWholeName());
			cTempCacheData.SetDomainGUID(cDomainInfo.GetDomainGUID());
			cTempCacheData.SetDomainMOID(cDomainInfo.GetDomainMOID());
			cTempCacheData.SetResolution(atVideoStreamCap[0].GetResolution());
			cTempCacheData.SetBitRate(atVideoStreamCap[0].GetMaxBitRate());
			cTempCacheData.SetMediaType(atVideoStreamCap[0].GetMediaType());
			cTempCacheData.SetMixMode(tConfInfo.GetStatus().GetMixerMode());
			cTempCacheData.SetDuration(tConfInfo.GetDuration());
			cTempCacheData.SetEncryptMode(tConfInfo.GetConfAttrb().GetEncryptMode());
			cTempCacheData.SetDualMode(tConfInfo.GetConfAttrb().GetDualMode());
			cTempCacheData.SetHasXmpuSer(tConfInfo.GetConfAttrb().IsHasXmpuSer() ? 1 : 0);
			cTempCacheData.SetOpenMode(tConfInfo.GetConfAttrb().GetOpenMode());
			cTempCacheData.SetConfNoDisturb(tConfInfo.GetConfAttrb().IsConfNoDisturb() ? 1 : 0);
			cTempCacheData.SetPortModeConf(tConfInfo.IsPortMediaConf());
			cTempCacheData.SetPublicConf(tConfInfo.GetConfAttrb().GetPublicConf());
			cTempCacheData.SetMaxJoinMtNum(tConfInfo.GetMaxJoinedMtNum());
			g_cMauVCApp.AddTempData( cTempCacheData );

			tReqInfo.SetConfName(tConfInfo.GetConfName());
			tReqInfo.SetDomainMOID( cDomainInfo.GetDomainMOID() );

			CMauMsg cAddTempMsg;
			cAddTempMsg.SetMsgBody( (u8*)&tReqInfo, sizeof( tReqInfo ) );
			cAddTempMsg.CatMsgBody( (u8*)&cDomainInfo, sizeof( cDomainInfo ) );
			cAddTempMsg.CatMsgBody( (u8*)pchJsonBuf, dwJsonLen );

			//调用模板操作类 添加模板信息
			TDBOprTaskMsgItem tDBTaskMsg;
			tDBTaskMsg.SetMsgBody( cAddTempMsg.GetMsgBody(), cAddTempMsg.GetMsgBodyLen() );
	        g_cMauVCApp.m_cTemplateDBOpr.AddTemplate( tDBTaskMsg );
		}
		break;

	case CM_MAU_MDFTEMPLATE_REQ:
		{
			json_t_ptr pjRoot = NULL;
			LoadJson(pchJsonBuf, strlen(pchJsonBuf), pjRoot, FALSE);
			const s8* pchConfName = "";
			GetConfNameFromJsonObj(pjRoot, &pchConfName);

			const s8* pchConfE164 = "";
			GetConfE164FromJsonObj(pjRoot, &pchConfE164);

			tReqInfo.SetConfE164(pchConfE164);
			tReqInfo.SetConfName(pchConfName);
			tReqInfo.SetDomainMOID( cDomainInfo.GetDomainMOID() );
			
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcTemplateOpr] send modify template<%s> command to db\n", tReqInfo.GetConfE164() );

			u32 dwJsonLen = cServMsg.GetMsgBodyLen() - sizeof( TReqHeadInfo );

			//准备相关数据
			CMauMsg cMdfTempMsg;
			cMdfTempMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
			cMdfTempMsg.CatMsgBody(pchJsonBuf, dwJsonLen);

			
			//调用模板操作类 添加模板信息
			TDBOprTaskMsgItem tDBTaskMsg;
			tDBTaskMsg.SetMsgBody( cMdfTempMsg.GetMsgBody(), cMdfTempMsg.GetMsgBodyLen() );
	        g_cMauVCApp.m_cTemplateDBOpr.ModTemplate( tDBTaskMsg );
		}
		break;

	case CM_MAU_DELTEMPLATE_REQ:
		{	
			u32 dwJsonLen = cServMsg.GetMsgBodyLen() - sizeof( TReqHeadInfo );

			CMauMsg cDelTempMsg;
			cDelTempMsg.SetMsgBody( (u8*)&tReqInfo, sizeof(tReqInfo) );
			cDelTempMsg.CatMsgBody( (u8*)pchJsonBuf, dwJsonLen );

			//调用模板操作类
			TDBOprTaskMsgItem tDBTaskMsg;
			tDBTaskMsg.SetMsgBody( cDelTempMsg.GetMsgBody(), cDelTempMsg.GetMsgBodyLen() );
			g_cMauVCApp.m_cTemplateDBOpr.DelTemplate( tDBTaskMsg );
		}
		break;

	default:
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_VC, "[DaemonProcTemplateOpr]Rcv msg is invalid, return!\n" );
		}
		break;
	}

	return;
}
/*=============================================================================
函 数 名： DaemonprocAllPersonalTmpAckMsg
功    能： 请求个人模板ACK消息
算法实现：
全局变量：
参    数： CMessage * const pcMsg
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2016/04/07  5.0         王力                    创建
=============================================================================*/
void CMauVCInstance::DaemonprocAllPersonalTmpAckMsg(CMessage *const pcMsg)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	//请求头信息
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	//用户信息
	s8 * pchJsonBuf = (s8*)(cServMsg.GetMsgBody() + sizeof(TReqHeadInfo));
	s8 achReqid[MAXLEN_CURTIME + 1] = { 0 };
	GetReqidFromJson(pchJsonBuf, achReqid, MAXLEN_CURTIME);
	if (m_strReqId == achReqid)
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonprocAllPersonalTmpAckMsg] ALL Personaltemp ack msg, KillTimer\n");
		KillTimer(MAU_GETPERSONALTEMPFROMCM_TIMER);
	}
}

/*=============================================================================
函 数 名： DaemonProcPersonalTemplate
功    能： 会管对个人模板操作
算法实现：
全局变量：
参    数： CMessage * const pcMsg
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2016/04/07  5.0         王力                    创建
=============================================================================*/
void CMauVCInstance::DaemonProcPersonalTemplate(CMessage *const pcMsg)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	//请求头信息
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	//用户信息
	s8 * pchJsonBuf = (s8*)(cServMsg.GetMsgBody() + sizeof(TReqHeadInfo));
	u32 dwJsonBufLen = cServMsg.GetMsgBodyLen() - sizeof(TReqHeadInfo);


	switch ( pcMsg->event )
	{
	case CM_MAU_PERSONALCONFTEMPLATE_NTF:
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[CMauVCInstance::DaemonProcPersonalTemplate] MOD OR ADD PersonalTem!\n ");
			LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, pchJsonBuf, dwJsonBufLen);
			
			json_t_ptr pjRoot;
			LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, FALSE);
			if (!pjRoot)
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[CMauVCInstance::DaemonProcPersonalTemplate] LoadJosn Failed!\n ");
				return;
			}
			
			CKdvDomainInfo cDomainInfo;
			if (!g_cMauVCApp.GetDomainFromJson(pchJsonBuf, cDomainInfo))
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[CMauVCInstance::DaemonProcPersonalTemplate] get domainInfo Failed!\n ");
				return;
			}
			// 获取ConfE164
			s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
			GetConfE164FromJson(pjRoot, achConfE164, MAXLEN_CONFE164);
			if (achConfE164[0] == '\0')
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[CMauVCInstance::DaemonProcPersonalTemplate] TemConfE164 is NULL,return!\n ");
				return;
			}
			
			g_cMauVCApp.AddPersonalTempInfoAndUpu(achConfE164, pchJsonBuf, cDomainInfo);
		}
		break;
	case CM_MAU_DELPERSONALCONFTEMPLATE_NTF:
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[CMauVCInstance::DaemonProcPersonalTemplate] Del PersonalTem!\n ");
			CMauMsg cTmpMsg;
			CMauData::GetTemplateListFromJson(pchJsonBuf, cTmpMsg);
			u16 wTemplateNum = *(u16*)cTmpMsg.GetMsgBody();
			
			u16 wIdx = 0;
			for (; wIdx < wTemplateNum; wIdx++)
			{
				s8  achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
				memcpy(achConfE164, (cTmpMsg.GetMsgBody() + sizeof(u16) + wIdx*MAXLEN_CONFE164), MAXLEN_CONFE164);
				g_cMauVCApp.DelPersonalTempInfoAndUpu(achConfE164);
			}
		}
		break;
	default:
		break;
	}
}

void CMauVCInstance::DaemonProcBatchAddBookConf(CMessage * const pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_VC);

	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	vector<CRedisBookConf> bookConfVec;
	CKdvDomainInfo cDomainInfo;
	BOOL32 bJsonContainMcuIp = FALSE;
	const u32 dwErrCode = g_cMauVCApp.GetBookConfList(cServMsg, bookConfVec,
		&cDomainInfo, &bJsonContainMcuIp);
	if(dwErrCode != 0)
	{
		SendReplyMsg(cServMsg, dwErrCode);
		return ;
	}

	g_cMauVCApp.CompletedBookConf(cDomainInfo, bookConfVec);
	for (vector<CRedisBookConf>::iterator confIter = bookConfVec.begin();
	confIter != bookConfVec.end(); ++confIter)
	{
		g_cMauVCApp.AddBookConf(*confIter);
		g_cMauVCApp.m_cVMeetingDBOpr.SetDBMeetingE164(confIter->GetConfE164(),
			confIter->GetMeetingID());

		if (!bJsonContainMcuIp)
		{
			UpuAddConf(confIter->GetConfE164(),
				TUpuConfInfo::emScheConf,
				cDomainInfo.GetDomainMOID(),
				cDomainInfo.GetSDomainMOID());
		}
	}
	SendReplyMsg(cServMsg, ERR_DB_SUCCESS);
}

void CMauVCInstance::DaemonProcBatchModBookConf(CMessage * const pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_VC);

	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	vector<CRedisBookConf> bookConfVec;
	const u32 dwErrCode = g_cMauVCApp.GetBookConfList(cServMsg, bookConfVec);
	if(dwErrCode != 0)
	{
		SendReplyMsg(cServMsg, dwErrCode);
		return ;
	}
	for (vector<CRedisBookConf>::iterator confIter = bookConfVec.begin();
	confIter != bookConfVec.end(); ++confIter)
	{
		g_cMauVCApp.ModBookConf( *confIter);
	}
	SendReplyMsg(cServMsg, ERR_DB_SUCCESS);
}

void CMauVCInstance::DaemonProcBatchDelBookConf(CMessage * const pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_VC);

	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	BOOL32 bJsonContainMcuIp = FALSE;
	vector<CRedisBookConf> bookConfVec;
	CKdvDomainInfo cDomainInfo;
	const u32 dwErrCode = g_cMauVCApp.GetBookConfList(cServMsg, bookConfVec, &cDomainInfo, &bJsonContainMcuIp);
	if(dwErrCode != 0)
	{
		SendReplyMsg(cServMsg, dwErrCode);
		return ;
	}

	for (vector<CRedisBookConf>::iterator confIter = bookConfVec.begin();
	confIter != bookConfVec.end(); ++confIter)
	{
		g_cMauVCApp.DelBookConf(confIter->GetConfE164());
		TRedisConfInfo tConfInfo;
		if(g_cMauVCApp.GetConfExInfo(confIter->GetConfE164(), tConfInfo)
			&& IsRedisHasConfExData(tConfInfo)
		    && confIter->GetMeetingID() == tConfInfo.GetMeetingID())
		{
			g_cMauVCApp.m_cConfInfoDBOpr.DelConfInfo(tConfInfo.GetConfE164());
			SendReleaseConfToCmu(tConfInfo.GetConfE164(), tConfInfo.GetConfName(), tConfInfo.GetCmuIp(), FALSE);
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[%s] Conf<%s> is Going.Release Meeting.\n", 
				__FUNCTION__, tConfInfo.GetConfE164() );
		}

		if (!bJsonContainMcuIp)
		{
			UpuDelConf(confIter->GetConfE164(), TUpuConfInfo::emScheConf);
		}
	}
	SendReplyMsg(cServMsg, ERR_DB_SUCCESS);
}

/*=============================================================================
函 数 名： DaemonProcCMReleaseConfReq
功    能： 处理会管结会请求
算法实现： 
全局变量： 
参    数： 
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauVCInstance::DaemonProcCMReleaseConfReq( const CMessage * pcMsg )
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);

	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	s8 * pchJsonBuf = (s8*)(cServMsg.GetMsgBody() + sizeof(TReqHeadInfo));

	s8 achConfE164[MAXLEN_CONFE164 + 1] = {0};
	GetConfE164FromJson(pchJsonBuf, achConfE164, MAXLEN_CONFE164);
	tReqInfo.SetConfE164(achConfE164);

	BOOL32 bEndAll = FALSE;
	GetEndAllSubConfFromJson(pchJsonBuf, bEndAll);

	CMauMsg cRspMsg;
	//用户有效性检验
	CKdvDomainInfo cDomainInfo;
	if (!g_cMauVCApp.GetDomainFromJson(pchJsonBuf, cDomainInfo) || !cDomainInfo.IsUsedFlag())
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[CMauVCInstance::DaemonProcCMReleaseConfReq] Domain<Name:%s GUID:%s> is not exist or is unused!\n ",
			cDomainInfo.GetWholeName(), cDomainInfo.GetDomainGUID() );
		
		//域信息不存在、获取不到域信息，域已经停用
		tReqInfo.SetErrID(ERR_DOMAIN_INVALID);
		tReqInfo.SetDomainMOID(cDomainInfo.GetDomainMOID());
		cRspMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
		SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID()+2, cRspMsg.GetServMsg(), cRspMsg.GetServMsgLen() );
		return ;
	}
	
	tReqInfo.SetDomainMOID( cDomainInfo.GetDomainMOID() );

	TRedisConfInfo tConfInfo;
	BOOL32 bResult = g_cMauVCApp.GetConfExInfo(achConfE164, tConfInfo);
	if(bResult && IsRedisHasConfExData(tConfInfo))
	{
		SendReleaseConfToCmu(tConfInfo.GetConfE164(), tConfInfo.GetConfName(), tConfInfo.GetCmuIp(), bEndAll);
		if (!ReleaseConf(cDomainInfo.GetDomainMOID(), achConfE164, tConfInfo.GetCmuIp()))
		{
			LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[%s]ReleaseConf Fail\n", __FUNCTION__);
		}

		LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DaemonProcCMReleaseConfReq]Send MAU_MCU_RELEASECONF_CMD to CEU.%s!\n", tConfInfo.GetCmuIp() );

		//返回结会成功
		cRspMsg.SetMsgBody( (u8*)&tReqInfo, sizeof(TReqHeadInfo) );
		SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID() + 1, cRspMsg.GetServMsg(), cRspMsg.GetServMsgLen() );
	}
	else
	{
		//返回结会失败
		tReqInfo.SetErrID(ERR_INFO_INVALID);
		cRspMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
		SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cRspMsg.GetServMsg(), cRspMsg.GetServMsgLen());

		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]GetConfDataByE164<%s> fail.check state in v_meeting.\n",
			__FUNCTION__, achConfE164);

		//如果会议不存于内存中，修改meeting中状态不对的情况
		TDBOprTaskMsgItem tDBTaskMsg;
		tDBTaskMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
		g_cMauVCApp.m_cVMeetingDBOpr.ReleaseConf(tDBTaskMsg);
	}
}

/*=============================================================================
函 数 名： DaemonProcScheduledConfAlreadyOngoingNtf
功    能： 预约会议已召开通知
算法实现： 
全局变量： 
参    数： 
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauVCInstance::DaemonProcScheduledConfAlreadyOngoingNtf( CMessage * const pcMsg )
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);

	//请求头信息
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	//用户信息
	s8 * pchJsonBuf = (s8*)( cServMsg.GetMsgBody()+sizeof(TReqHeadInfo) );

	//用户有效性检验
	CKdvDomainInfo cDomainInfo;
	if (!g_cMauVCApp.GetDomainFromJson(pchJsonBuf, cDomainInfo) || !cDomainInfo.IsUsedFlag())
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[CMauVCInstance::DaemonProcScheduledConfAlreadyOngoingNtf] Domain<Name:%s GUID:%s> is not exist or is unused!\n ",
			cDomainInfo.GetWholeName(), cDomainInfo.GetDomainGUID());

		//域信息不存在、获取不到域信息，域已经停用
		tReqInfo.SetDomainMOID(cDomainInfo.GetDomainMOID());
		cServMsg.SetMsgBody((u8*)&tReqInfo, sizeof(TReqHeadInfo));
		SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
		return;
	}
	
	tReqInfo.SetDomainMOID(cDomainInfo.GetDomainMOID());

	u32 nMeetingID = 0;
	CMauData::GetMeetingIdFromJson(pchJsonBuf, nMeetingID);

	s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
	GetConfE164FromJson(pchJsonBuf, achConfE164, sizeof(achConfE164) - 1);
	
	TRedisConfInfo tConfInfo;
	BOOL32 bResult = g_cMauVCApp.GetConfExInfo(achConfE164, tConfInfo);
	if(!bResult 
		|| !IsRedisHasConfExData(tConfInfo)
		|| nMeetingID != tConfInfo.GetMeetingID())
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[CMauVCInstance::DaemonProcScheduledConfAlreadyOngoingNtf] GetConfDataByMeetingID err, return!\n " );
		
		cServMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
		SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
		return;
	}
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[CMauVCInstance::DaemonProcScheduledConfAlreadyOngoingNtf]E164<%s> already ongoing\n", tConfInfo.GetConfE164() );

	g_cMauVCApp.DelBookConf(achConfE164);
}

void CMauVCInstance::DaemonProcCreateConfByPublicTemp(const CMessage *  pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_VC, OspEventDesc(pcMsg->event));
	CHECK_STATE(STATE_NORMAL, return);

	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	s8 * pchJsonBuf = (s8*)(cServMsg.GetMsgBody() + sizeof(TReqHeadInfo));

	CKdvDomainInfo tDomainInfo;	
	if (!g_cMauVCApp.GetDomainInfo(tReqInfo.GetDomainMOID(), tDomainInfo)
		|| !tDomainInfo.IsUsedFlag())
	{
		ReplyNack(ERR_DOMAIN_INVALID, "Domain unexist or not use");
		return;
	}

	TRedisConfInfo tRedisConf;
	BOOL32 bResult = g_cMauVCApp.GetConfExInfo(tReqInfo.GetConfE164(), tRedisConf);
	if (!bResult)
	{
		ReplyNack(ERR_DB_OPR, "GetConfExInfo Fail");
		return;
	}
	else if(!IsRedisHasConfExData(tRedisConf)
			|| tRedisConf.GetState() == enumConnectWaitingState)
	{
		if (!g_cMauVCApp.IsTemplateExist(tReqInfo.GetConfE164()))
		{
			ReplyNack(ERR_CONF_NOEXIST, "Public Template inexistence");
			return;
		}

		u16 wConfDuration = 0;
		CMauData::GetConfDurationFromJson(pchJsonBuf, wConfDuration);
		g_cMauVCApp.m_cTemplateDBOpr.GetTemplateInfo(tReqInfo, wConfDuration);
		LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[%s] mt<%s> require join template<%s>,"
			" get template info from db.\n",
			__FUNCTION__, tReqInfo.GetSrcMtE164(), tReqInfo.GetConfE164());
	}
	else 
	{
		if (!IsMt(tReqInfo.GetReqEventID()))
		{
			ReplyNack(ERR_CONF_ONGOING, "conf on going");
			return;
		}
		else
		{
			CMauData::NoticeCmuSsnAddMt(tRedisConf.GetCmuIp(), tReqInfo);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]mt<%s> require join conf<%s>,"
				"send add mt command to mcu<%s>\n", __FUNCTION__,
				tReqInfo.GetSrcMtE164(), tReqInfo.GetConfE164(), tRedisConf.GetCmuIp());
		}
	}
}

void CMauVCInstance::DaemonProcMtJoinBookConf(const CMessage *  pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_VC, OspEventDesc(pcMsg->event));
	CHECK_STATE(STATE_NORMAL, return);

	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	CKdvDomainInfo tDomainInfo;	
	if (!g_cMauVCApp.GetDomainInfo(tReqInfo.GetDomainMOID(), tDomainInfo)
		|| !tDomainInfo.IsUsedFlag())
	{
		ReplyNack(ERR_DOMAIN_INVALID, "Domain Unexist or not use");
		return;
	}
	
	TRedisConfInfo tRedisConf;
	BOOL32 bResult = g_cMauVCApp.GetConfExInfo(tReqInfo.GetConfE164(), tRedisConf);
	if (!bResult)
	{
		ReplyNack(ERR_DB_OPR, "redis exception");
		return;
	}
	else if (!IsRedisHasConfExData(tRedisConf)
		|| tRedisConf.GetState() == enumConnectWaitingState)
	{
		CRedisBookConf cBookConf;
		bResult = g_cMauVCApp.GetBookConf(tReqInfo.GetConfE164(), cBookConf);
		if (bResult
			&& strlen(cBookConf.GetConfE164()) != 0)
		{
			BOOL32 bReachLimit = FALSE;
			if (!g_cMauVCApp.IsConfNumReachLimit(tDomainInfo, cBookConf.GetMaxJoinedMt(), FALSE, bReachLimit)
				|| bReachLimit)
			{
				ReplyNack(bReachLimit ? ERR_CONFNUM_EXCEED : ERR_DB_OPR,
					bReachLimit ? "reach conf num limit." : "IsConfNumReachLimit failed");
				return;
			}
			if (CheckPassword(tReqInfo.GetConfPassword(), cBookConf.GetPassword().c_str()))
			{
				u32 dwErrCode = AheadCreateBookConf(tReqInfo, cBookConf);
				if (dwErrCode != ERR_DB_SUCCESS)
				{
					ReplyNack(dwErrCode, "AheadCreateBookConf Fail");
					return;
				}
				LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[%s] mt<%s> require join book conf<%s>.\n",
					__FUNCTION__, tReqInfo.GetSrcMtE164(), tReqInfo.GetConfE164());
			}
			else
			{
				ReplyNack(ERR_MCU_INVALIDPWD, "password error");
				return;
			}
		}
		else if (!bResult)
		{
			ReplyNack(ERR_DB_OPR, "redis exception");
			return;
		}
		else
		{
			ReplyNack(ERR_CONF_NOEXIST, "conf inexistence");
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s] mt<%s> require join conf<%s>\n",
				__FUNCTION__, tReqInfo.GetSrcMtE164(), tReqInfo.GetConfE164());
			return;
		}
	}
	else
	{
		CMauData::NoticeCmuSsnAddMt(tRedisConf.GetCmuIp(), tReqInfo);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s] mt<%s> require join conf<%s>,"
			"send add mt command to mpc<%s>\n", __FUNCTION__,
			tReqInfo.GetSrcMtE164(), tReqInfo.GetConfE164(), tRedisConf.GetCmuIp());
	}
}


void CMauVCInstance::DaemonProcConfDelay( const CMessage * pcMsg )
{
	CHECK_STATE(STATE_NORMAL, return);

	CMauMsg cServMsg( pcMsg->content, pcMsg->length );

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	u32 dwJsonBufLen = pcMsg->length - sizeof(TReqHeadInfo);

	//请求头信息
	TReqHeadInfo tReqInfo;
	tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();

	u8 * pbyMsg = cServMsg.GetMsgBody() + sizeof(TReqHeadInfo);

	memcpy(achJsonBuf, pbyMsg, dwJsonBufLen);

	u16 wDelayTime = 0;
	GetDelayFromJson(achJsonBuf, wDelayTime);

	s8 achE164[MAXLEN_E164 + 1] = { 0 };
	BOOL32 bRet = GetConfE164FromJson(achJsonBuf, achE164, MAXLEN_E164);
	if (!bRet || strlen(achE164) == 0)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[%s]Get E164<%s>failure or empty.\n",
			__FUNCTION__, achE164);
		SetErrCodeToJson(achJsonBuf, dwJsonBufLen, 0); //todo 延长会议失败错误码

		CMauMsg cRplMsg;
		cRplMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
		cRplMsg.CatMsgBody(achJsonBuf, dwJsonBufLen);
		SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cRplMsg.GetServMsg(), cRplMsg.GetServMsgLen());
		return;
	}

	tReqInfo.SetConfE164(achE164);

	CMauMsg cRplMsg;
	cRplMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));

	TRedisConfInfo tConfInfo;
	BOOL32 bResult = g_cMauVCApp.GetConfExInfo(achE164, tConfInfo);
	if (!bResult || !IsRedisHasConfExData(tConfInfo))
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[%s] getconfexinfo<%s> Fail.Delay Conf Nack.\n",
			__FUNCTION__, achE164);

		//返回失败消息
		SetErrCodeToJson(achJsonBuf, dwJsonBufLen, 0); //todo 延长会议失败错误码
		cRplMsg.CatMsgBody(achJsonBuf, dwJsonBufLen);

		SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cRplMsg.GetServMsg(), cRplMsg.GetServMsgLen());
		return;
	}

	u16 wDuartion = tConfInfo.GetDuration();

	//不支持延长永久会议
	if (0 == wDuartion)
	{
		LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DaemonProcConfDelay] conf duration = %u , ConfDelay Fail.\n", wDuartion);

		//返回失败消息
		SetErrCodeToJson(achJsonBuf, dwJsonBufLen, 0); //todo 延长会议失败错误码
		cRplMsg.CatMsgBody(achJsonBuf, dwJsonBufLen);

		SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cRplMsg.GetServMsg(), cRplMsg.GetServMsgLen());
		return;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[CMauVCInstance::DaemonProcConfDelay] now conf duration = %u.\n", wDuartion);


	//校验延长时间是否合法
	if ( (0xffff - wDuartion < wDelayTime) )
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DaemonProcConfDelay] Conf Time > 0xffff. Delay fail.\n");

		//返回失败消息
		SetErrCodeToJson(achJsonBuf, dwJsonBufLen, 0); //todo 延长会议失败错误码
		cRplMsg.CatMsgBody(achJsonBuf, dwJsonBufLen);

		SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cRplMsg.GetServMsg(), cRplMsg.GetServMsgLen());
		return;
	}
	wDuartion += wDelayTime;
	tConfInfo.SetDuration(wDuartion);
	if (!g_cMauVCApp.SetConfData(tConfInfo))
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]SetConfData Fail\n", __FUNCTION__);
		SetErrCodeToJson(achJsonBuf, dwJsonBufLen, 0); //todo 延长会议失败错误码
		cRplMsg.CatMsgBody(achJsonBuf, dwJsonBufLen);
		SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cRplMsg.GetServMsg(), cRplMsg.GetServMsgLen());
		return;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]delay %u, duration change to %u\n",
		__FUNCTION__, wDelayTime, wDuartion);

	SetConfDurationToJson(achJsonBuf, dwJsonBufLen, tConfInfo.GetDuration());
	cRplMsg.CatMsgBody(achJsonBuf, dwJsonBufLen);
	SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 1, cRplMsg.GetServMsg(), cRplMsg.GetServMsgLen());

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_MAU_CMU_CONFDURATION_NTF);
	CMauData::SendMsgToCmuSsn(tConfInfo.GetCmuIp(), MAU_CMU_CONFDURATION_NTF, achJsonBuf, dwJsonBufLen);
}

void CMauVCInstance::DaemonProcMonitorKeepAliveNtf( const CMessage * pcMsg )
{
	//非工作状态, 不进行任何处理
	CHECK_STATE(STATE_NORMAL, return);
	
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	
	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	u32 dwJsonBufLen = pcMsg->length - sizeof(TReqHeadInfo);
	
	//请求头信息
	TReqHeadInfo tReqInfo;
	tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	
	u8 * pbyMsg = cServMsg.GetMsgBody() + sizeof(TReqHeadInfo);
	
	memcpy(achJsonBuf, pbyMsg, dwJsonBufLen);
	
	s8 achE164[MAXLEN_E164+1];
	memset(achE164, 0, sizeof(achE164));
	BOOL32 bRet = GetConfE164FromJson(achJsonBuf, achE164, MAXLEN_E164);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[CMauVCInstance::DaemonProcMonitorKeepAliveNtf]Get E164 is failure.\n" );
	}
	
	tReqInfo.SetConfE164(achE164);
	
	//保活通知地址列表
	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( achJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL == pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[CMauVCInstance::DaemonProcMonitorKeepAliveNtf] json_loads err, return!\n" );
		return;
	}
	
	json_t_ptr pjDstAddrList = json_object_get_s(pjRoot, JSON_DSTADDRLIST);
	if ( NULL == pjDstAddrList )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[CMauVCInstance::DaemonProcMonitorKeepAliveNtf] json_object_get JSON_DSTADDRLIST err, return!\n" );
		return;
	}

	u16 wArraySize = json_array_size(pjDstAddrList);
	if ( wArraySize > 0 )
	{
		u16 wAddrIdx = 0;
		for ( wAddrIdx = 0; wAddrIdx < wArraySize; wAddrIdx++ )
		{
			json_t_ptr pjDstAddr = json_array_get_s( pjDstAddrList, wAddrIdx );
			if ( NULL == pjDstAddr )
			{
				continue;
			}
			
			json_t_ptr pjTransportAddr = json_object_get_s( pjDstAddr, JSON_TRANSPORTADDR );
			if ( NULL == pjTransportAddr )
			{
				continue;
			}
			s8 achIP[50] = {0};
			s8 achPort[50] = {0};
			GetTransportAddrFromJson(pjTransportAddr, achIP, achPort);
			UpdateMtAliveTime(achE164, achIP, achPort);
		}
	}
	
	return;
}

void CMauVCInstance::DaemonProcMtCreateConf(const CMessage *  pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_VC, "event: %s",
		OspEventDesc(pcMsg->event));
	CHECK_STATE(STATE_NORMAL, return);

	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	u8 * pbyMsg = cServMsg.GetMsgBody();
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof( TReqHeadInfo );
	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = cServMsg.GetMsgBodyLen() - sizeof(TReqHeadInfo);
	memcpy(achJsonBuf, pbyMsg, dwJsonBufLen);

	LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]mt<%s>\n",
		__FUNCTION__, tReqInfo.GetSrcMtE164());

	CKdvDomainInfo tDomainInfo;	
	if (!g_cMauVCApp.GetDomainInfo(tReqInfo.GetDomainMOID(), tDomainInfo))
	{
		ReplyNack(ERR_DOMAIN_INVALID, "Domain Not Exists or unused");
		return;
	}

	s8 achConfE164[MAXLEN_CONFE164+1] = {0};
	if (!GetConfE164FromJson(achJsonBuf, achConfE164, MAXLEN_CONFE164)
		|| strlen(achConfE164) == 0)
	{
		if (!g_cMauVCApp.ConstructConfE164(tDomainInfo.GetSDomainMOID(),
			achConfE164))
		{
			ReplyNack(ERR_CONSTRUCT_CONFE164, "Construct e164 failed");
			return;
		}
		SetConfE164ToJson(achJsonBuf, dwJsonBufLen, achConfE164);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s] Construct confe164<%s>!\n",
			__FUNCTION__, achConfE164);
	}
	else
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]has predistribute "
			" or personal template confe164<%s>!\n", __FUNCTION__, achConfE164);
	}
	tReqInfo.SetConfE164( achConfE164 );

	s8 achConfName[MAXLEN_CONFNAME + 1] = { 0 };
	s8 achConfNewName[MAXLEN_CONFNAME + 1] = { 0 };
	GetConfNameFromJson(achJsonBuf, achConfName, sizeof(achConfName));
	BOOL32 bConfNameModify = FALSE;
	if (!g_cMauVCApp.ConfNameRepeatProc(tDomainInfo, achConfName,
		tReqInfo.GetConfE164(), achConfNewName, MAXLEN_CONFNAME, bConfNameModify))
	{
		ReplyNack(ERR_DB_OPR, "ConfNameRepeatProc Fail");
		return;
	}
	else if(bConfNameModify)
	{	
		//如果存在重名会议则修改会议名
		SetConfNameToJson(achJsonBuf, achConfNewName, dwJsonBufLen);
		tReqInfo.SetConfName(achConfNewName);
	}

	BOOL32 bReachLimit = FALSE;
	if (!g_cMauVCApp.IsConfNumReachLimit(achJsonBuf, tDomainInfo, bReachLimit)
		|| bReachLimit)
	{
		ReplyNack(bReachLimit ? ERR_CONFNUM_EXCEED : ERR_DB_OPR,
			bReachLimit ? "reach conf limit." : "IsConfNumReachLimit fail");
		return;
	}
	
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
	tDBMsg.CatMsgBody(&tDomainInfo, sizeof(CKdvDomainInfo));
	tDBMsg.CatMsgBody(&dwJsonBufLen, sizeof(dwJsonBufLen));
	tDBMsg.CatMsgBody(achJsonBuf, dwJsonBufLen);
	g_cMauVCApp.m_cVMeetingDBOpr.CreateMeeting( tDBMsg );
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]into vmdb process\n", __FUNCTION__);
}

void CMauVCInstance::DaemonProcCreateMeetingFinishNtf(const CMessage * pcMsg)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	TMeetingDBFinishOpr tOpr = *(TMeetingDBFinishOpr *)pbyMsg;
	pbyMsg = pbyMsg + sizeof( TMeetingDBFinishOpr );
	TReqHeadInfo tHeadInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg = pbyMsg + sizeof(TReqHeadInfo);
	if (0 != tOpr.GetOprRet())
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[CMauVCInstance::DaemonProcCreateMeetingFinishNtf] Create Meeting By Virtual failed!!\n");
		cServMsg.SetMsgBody((u8*)&tHeadInfo, sizeof(tHeadInfo));
		SendReplyMsg(tHeadInfo, tHeadInfo.GetReqEventID() + 2, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
		return;
	}

	CKdvDomainInfo* ptDomainInfo = (CKdvDomainInfo*)pbyMsg;
	pbyMsg = pbyMsg + sizeof(CKdvDomainInfo);
	s32 nMeetingID = *(s32 *)pbyMsg;
	pbyMsg = pbyMsg + sizeof(s32);
	u16 wDuration = *(u16*)pbyMsg;
	pbyMsg += sizeof(wDuration);
	u32 dwJsonBufLen = *(u32*)pbyMsg;
	pbyMsg += sizeof(dwJsonBufLen);
	const s8* pchJson = (const s8*)pbyMsg;
	
	json_t_ptr pjRoot = NULL;
	LoadJson(pchJson, dwJsonBufLen, pjRoot, TRUE);
	SetMeetingIDToJsonObj(pjRoot, nMeetingID);
	SetIntAsStringToJson(pjRoot, JSON_DURATION, wDuration);

	json_str achJsonBuf(pjRoot);
	cServMsg.SetMsgBody(&tHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody(achJsonBuf, strlen(achJsonBuf));
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), 
		MAU_MAU_CREATECONF_REQ, &cServMsg, sizeof(cServMsg) );
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Finish vmdb create conf, "
		"duration: %d, meetingID: %d\n", __FUNCTION__, wDuration, nMeetingID);
}

std::pair<u32, std::string> CMauVCInstance::CheckAndGetCreateConfInfo(const CMauMsg& cServMsg,
	const BOOL32 bMt,
	TReqHeadInfo& tReqInfo,
	string& strJsonBuf,
	TRedisConfInfo& tRedisConfInfo,
	u32& nMeetingID,
	BOOL32& bPersonOrPublicTempConf
	)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_VC);

	u8 * pbyMsg = cServMsg.GetMsgBody();
	tReqInfo = *(TReqHeadInfo*)pbyMsg;

	const s8* pchBufJson = (s8*)pbyMsg + sizeof(TReqHeadInfo);
	json_t_ptr pjRoot = NULL;
	if (!LoadJson(pchBufJson, strlen(pchBufJson), pjRoot, FALSE) 
		|| !pjRoot)
	{
		return make_pair(ERR_INFO_INVALID, "json format error"); 
	}

	CKdvDomainInfo cDomainInfo;
	if (!g_cMauVCApp.GetDomainInfo(tReqInfo.GetDomainMOID(), cDomainInfo) 
		|| !cDomainInfo.IsUsedFlag())
	{
		return make_pair(ERR_DOMAIN_INVALID, "Domain Not Exists or unused");
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Doamin<%s> valid\n",
		__FUNCTION__, cDomainInfo.GetDomainMOID());

	TCmuData tCmuData;
	const s8* pchMcuIp = NULL;
	GetStringFromJson(pjRoot, JSON_IP, &pchMcuIp);
	if (!g_cMauVCApp.AssignMpcID(pchMcuIp, tCmuData))
	{
		return make_pair(ERR_NOIDLEFITRES, "AssignMpcID failed");
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Assign mcu<Ip:%s, Type: %s> success\n",
		__FUNCTION__, tCmuData.GetCmuIp(), tCmuData.GetCmuType());

	TConfInfoEx tConfInfoEx;
	if (!GetConfInfoExFromJson(pchBufJson, tConfInfoEx))
	{
		return make_pair(ERR_INFO_INVALID, "get conf info Error");
	}
	TVideoStreamCap atVideoStreamCap[MAX_CONF_CAP_EX_NUM];
	u8 byCapNum = MAX_CONF_CAP_EX_NUM;
	tConfInfoEx.GetMainStreamCapEx(atVideoStreamCap, byCapNum);
	if (!cDomainInfo.IsEnableH265())
	{
		if(atVideoStreamCap[0].GetResolution() == VIDEO_FORMAT_4K || atVideoStreamCap[0].GetMediaType() == MEDIA_TYPE_H265)
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]domainmoid:%s is not enableH265\n", __FUNCTION__, cDomainInfo.GetDomainMOID());
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]tConfInfoEx's MainStreamCap %s\n",
				__FUNCTION__, atVideoStreamCap[0].GetResolution() == VIDEO_FORMAT_4K ? "res is 4k" : "mediatype is h265");
			return make_pair(ERR_INFO_INVALID, "Is not enableH265");
		}
	}
	if (!cDomainInfo.IsEnbaleHDMeeting())
	{
		if (atVideoStreamCap[0].GetResolution() == VIDEO_FORMAT_HD1080 || atVideoStreamCap[0].GetResolution() == VIDEO_FORMAT_4K)
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]domainmoid:%s is not enableHDMeeting\n", __FUNCTION__, cDomainInfo.GetDomainMOID());
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]tConfInfoEx's MainStreamCap Resolution is %s! \n",
				__FUNCTION__, atVideoStreamCap[0].GetResolution() == VIDEO_FORMAT_HD1080 ? "1080p" : "4k");
			return make_pair(ERR_INFO_INVALID, "Is not enableHDMeeting");
		}
	}

	TConfInfo tConfInfo;
	if (!GetCreateConfParamFromJsonObj(pjRoot, tConfInfo)
		|| !GetStringAsUIntFromJson(pjRoot, JSON_MEETINTID, nMeetingID))
	{
		return make_pair(ERR_INFO_INVALID, "get conf info Error");
	}
	tReqInfo.SetMeetingID(nMeetingID);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]get conf info success\n", __FUNCTION__);

	if (g_cMauVCApp.m_bEduVer)
	{
		//最大视频分辨率支持到1080P * 30（支持 720P *60）
		if (!IsResLE(atVideoStreamCap[0].GetResolution(), VIDEO_FORMAT_HD1080))
		{
			atVideoStreamCap[0].SetResolution(VIDEO_FORMAT_HD1080);
			atVideoStreamCap[0].SetFrameRate(VIDEO_FPS_USRDEFINED_30);
			tConfInfoEx.SetMainStreamCapEx(atVideoStreamCap, 1);
			SetConfInfoExToJson(pjRoot, tConfInfoEx);
		}
		else if (atVideoStreamCap[0].GetResolution() == VIDEO_FORMAT_HD1080 && atVideoStreamCap[0].GetUserDefFrameRate() > VIDEO_FPS_USRDEFINED_30)
		{
			atVideoStreamCap[0].SetFrameRate(VIDEO_FPS_USRDEFINED_30);
			tConfInfoEx.SetMainStreamCapEx(atVideoStreamCap, 1);
			SetConfInfoExToJson(pjRoot, tConfInfoEx);
		}

		//与会方仅支持6方
		tConfInfo.SetMaxJoinedMtNum( 6 );
		SetConfMaxJoinedMtNumToJsonObj(pjRoot, tConfInfo.GetMaxJoinedMtNum());
		//仅支持端口会议
		SetBoolAsStringToJson(pjRoot, JSON_ISPORTCONF, 1);
		
		// 仅支持非合并级联
		TCascadeSetToCM tCascadeSet;
		tCascadeSet.m_byCascadeMode = 2;
		tCascadeSet.m_byIsUpload = 0;
		tCascadeSet.m_byIsSpy = 0;
		tCascadeSet.m_dwSpyBW = 0;
		json_t_ptr pjCascadeInfo = CreatCascadeSetJson(tCascadeSet);
		if (NULL != pjCascadeInfo)
		{
			json_object_set_s(pjRoot, JSON_CASCADECONF, pjCascadeInfo);
		}

		//不支持自主合成
		SetBoolAsStringToJson(pjRoot, JSON_ISSUPPORTSELFVMP, 0);

		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]IS EDU Version\n", __FUNCTION__);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Set MainStreamCap Resolution:%d, FrameRate:%d, MaxJoinedMtNum:%d, CascadeMode:%d, IsSupportVmp:0! \n", __FUNCTION__,
			atVideoStreamCap[0].GetResolution(), atVideoStreamCap[0].GetFrameRate(), tConfInfo.GetMaxJoinedMtNum(), tCascadeSet.m_byCascadeMode);
	}

	BOOL32 bSucc = TRUE;
	string strBookConfE164;
	if (!bMt 
		&& (strlen(tConfInfo.GetConfE164()) == 0)
		&& (bSucc = g_cMauVCApp.GetBookConfE164ByMeetingID(nMeetingID, strBookConfE164))
		&& !strBookConfE164.empty())
	{
		tConfInfo.SetConfE164(strBookConfE164.c_str());
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]BookConf, but json has not e164\n", __FUNCTION__);
	}
	else if (!bSucc)
	{
		return make_pair(ERR_DB_OPR, "GetBookConfByMeetingID Fail");
	}

	BOOL32 bReachLimit = FALSE;
	if (!g_cMauVCApp.IsConfNumReachLimit(cDomainInfo, tConfInfo.GetMaxJoinedMtNum(),
		IsOldMcu(pchMcuIp), bReachLimit)
		|| bReachLimit)
	{
		return make_pair((bReachLimit ? ERR_CONFNUM_EXCEED : ERR_DB_OPR),
			(bReachLimit ? "reach conf num limit" : "IsConfNumReachLimit failed"));
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]confscale: %u, conf num not"
		" exceed limit\n", __FUNCTION__, tConfInfo.GetMaxJoinedMtNum());

	bPersonOrPublicTempConf = MAXLEN_CONFE164 > strlen(tConfInfo.GetConfE164());
	if (!bMt && bPersonOrPublicTempConf)
	{
		s8 achConfE164[MAXLEN_CONFE164+1] = {0};
		if (!g_cMauVCApp.ConstructConfE164(cDomainInfo.GetSDomainMOID(), achConfE164))
		{
			return make_pair(ERR_CONSTRUCT_CONFE164, "Construct e164 failed");
		}
		tConfInfo.SetConfE164(achConfE164);
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s] Construct confe164<%s>\n",
			__FUNCTION__, achConfE164 );
	}
	tReqInfo.SetConfE164(tConfInfo.GetConfE164());
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]conf E164: %s\n",
		__FUNCTION__, tConfInfo.GetConfE164());

	//所有终端重命名都在数据库操作之前处理
	//重命名处理必须放在会议e164号操作之前处理
	s8 achConfName[MAXLEN_CONFNAME + 1] = {0};
	BOOL32 bConfNameModify = FALSE;
	if (!bMt && !g_cMauVCApp.ConfNameRepeatProc(cDomainInfo, tConfInfo.GetConfName(),
		tConfInfo.GetConfE164(), achConfName, MAXLEN_CONFNAME, bConfNameModify))
	{
		return make_pair(ERR_DB_OPR, "ConfNameRepeatProc Fail");
	}
	else if(!bMt && bConfNameModify)
	{	
		tConfInfo.SetConfName(achConfName);
		SetStringToJson(pjRoot, JSON_CONFNAME, achConfName);
	}
	tReqInfo.SetConfName(tConfInfo.GetConfName());
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]conf name: %s\n",
		__FUNCTION__, tConfInfo.GetConfName());

	const time_t tRealStartTime = time(NULL);
	tRedisConfInfo.SetConfE164(tConfInfo.GetConfE164());
	tRedisConfInfo.SetConfName(tConfInfo.GetConfName());
	tRedisConfInfo.SetDomainMOID(cDomainInfo.GetDomainMOID());
	tRedisConfInfo.SetState(enumPrepareState);
	tRedisConfInfo.SetMeetingID(nMeetingID);
	tRedisConfInfo.SetDuration(tConfInfo.GetDuration());
	tRedisConfInfo.SetStartTime(tRealStartTime);
	tRedisConfInfo.SetRealStartTime(tRealStartTime);
	tRedisConfInfo.SetCreatorType(tReqInfo.GetConfCreateBy());
	tRedisConfInfo.SetMaxJoinedMt(tConfInfo.GetMaxJoinedMtNum());
	tRedisConfInfo.SetCmuIp(tCmuData.GetCmuIp());
	tRedisConfInfo.SetCmuPid(tCmuData.GetCmuPid());
	BOOL32 bNeedLicense = IsNewMcu(tCmuData.GetCmuType());
	tRedisConfInfo.SetNeedLicense(bNeedLicense);
	GetCreatorInfoFromJson(pjRoot, tRedisConfInfo);
	// 读取不到则设置为相同的
	const s8* pchStartTime = NULL;
	GetStringFromJson(pjRoot, JSON_BOOKSTARTTIME, &pchStartTime);
	if (pchStartTime == NULL || pchStartTime[0] == 0)
	{
		tRedisConfInfo.SetBookStartTime(tRealStartTime);
	}
	else
	{
		tRedisConfInfo.SetBookStartTime(pchStartTime);
	}

	SetMsgTypeToJsonObj(pjRoot, EV_MAU_CMU_CREATECONF_REQ);
	SetReqHeadToJsonObj(pjRoot, tReqInfo);
	SetHasXmpuSerToJsonObj(pjRoot, cDomainInfo.HasXmpuServer() ? 1 : 0);
	SetConfE164ToJsonObj(pjRoot, tRedisConfInfo.GetConfE164());
	SetConfDurationToJsonObj(pjRoot, tRedisConfInfo.GetDuration());
	SetConfStartTimeToJsonObj(pjRoot, tRedisConfInfo.GetRealStartTime());
	json_str abyBuff(pjRoot);
	strJsonBuf = abyBuff;
	return make_pair(0, "");
}

void CMauVCInstance::DaemonProcCreateConf(const CMessage * pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_VC, OspEventDesc(pcMsg->event));

	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	const BOOL32 bMt = IsMt(tReqInfo.GetReqEventID());
	string strCreateConfJson;
	TRedisConfInfo tRedisConf;
	u32 nMeetingID = 0;
	BOOL32 bPersonOrPublicTempConf = FALSE;
	const pair<u32, string> pairRes = CheckAndGetCreateConfInfo(cServMsg, bMt,
		tReqInfo, strCreateConfJson, tRedisConf, nMeetingID,
		bPersonOrPublicTempConf);
	if (pairRes.first != 0)
	{
		ReplyNack(pairRes.first, pairRes.second.c_str());
		if (bMt)
		{
			g_cMauVCApp.m_cVMeetingDBOpr.CreateMeetingFailed(nMeetingID);
		}
		return;
	}

	if (!bMt && bPersonOrPublicTempConf)
	{
		g_cMauVCApp.m_cVMeetingDBOpr.SetDBMeetingE164(tRedisConf.GetConfE164(), nMeetingID);
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]setDBMeetingE164: %s,"
			" meetingID: %u\n", __FUNCTION__, tRedisConf.GetConfE164(), nMeetingID);
	}
	else if(!bMt)
	{
		g_cMauVCApp.DelBookConf(tRedisConf.GetConfE164());
		UpuDelConf(tRedisConf.GetConfE164(), TUpuConfInfo::emScheConf);
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]del redis and upu bookConf: %s\n",
			__FUNCTION__, tRedisConf.GetConfE164());
	}

	TRedisConfInfo tTempConf;
	if (!g_cMauVCApp.GetConfExInfo(tRedisConf.GetConfE164(), tTempConf))
	{
		if (bMt)
		{
			g_cMauVCApp.m_cVMeetingDBOpr.CreateMeetingFailed(nMeetingID);
		}
		ReplyNack(ERR_DB_OPR, "Redis Error");
		return;
	}
	else if(IsRedisHasConfExData(tTempConf)
			&& tTempConf.GetState() != enumConnectWaitingState)
	{
		if (bMt)
		{
			CMauData::NoticeCmuSsnAddMt(tTempConf.GetCmuIp(), tReqInfo);
			LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]conf exist and not wait state,"
				"mt req, notcie cmu add mt\n", __FUNCTION__, tRedisConf.GetConfE164());
		}
		else
		{
			ReplyNack(ERR_CONF_ONGOING, "conf has existense");
		}
		return;
	}
	if(!g_cMauVCApp.SetConfData(tRedisConf))
	{
		if (bMt)
		{
			g_cMauVCApp.m_cVMeetingDBOpr.CreateMeetingFailed(nMeetingID);
		}
		ReplyNack(ERR_DB_OPR, "Redis Error");
		return;
	}
	CMauData::SendMsgToCmuSsn(tRedisConf.GetCmuIp(), MAU_MCU_CREATECONF_REQ,
		strCreateConfJson.c_str(), strCreateConfJson.length());
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Create conf<%s>"
		" meetingID<%d> to cmu: %s:%u\n", __FUNCTION__,
		tRedisConf.GetConfE164(), tRedisConf.GetMeetingID(), tRedisConf.GetCmuIp(), tRedisConf.GetCmuPid());
}

void CMauVCInstance::DaemonProcAheadMeetingFinishNtf(const CMessage * pcMsg)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	TMeetingDBFinishOpr tOpr = *(TMeetingDBFinishOpr *)pbyMsg;
	pbyMsg = pbyMsg + sizeof( TMeetingDBFinishOpr );
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof( TReqHeadInfo );

	//加载数据库数据失败
	if( 0 != tOpr.GetOprRet() )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcAheadMeetingFinishNtf] AHead Meeting failed!!\n" );
		cServMsg.SetMsgBody( (u8*)&tReqInfo, sizeof(tReqInfo) );
		SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID()+2, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
		return ;
	}

	s32 nMeetingId = *(s32*)pbyMsg;
	pbyMsg += sizeof( s32 );
	u32 dwJsonBufLen = *(u32*)pbyMsg;
	pbyMsg += sizeof(dwJsonBufLen);
	const s8* pchJsonBuff = (const s8*)pbyMsg;

	json_t_ptr pjRoot = NULL;
	LoadJson(pchJsonBuff, dwJsonBufLen, pjRoot, TRUE);
	SetMeetingIDToJsonObj(pjRoot, nMeetingId);
	json_str achJsonBuf(pjRoot);

	cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
	cServMsg.CatMsgBody(achJsonBuf, strlen(achJsonBuf));
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), 
			MAU_MAU_CREATECONF_REQ, &cServMsg, sizeof(cServMsg) );
}

/*=============================================================================
函 数 名： DaemonProcDelayConfFinishNtf
功    能： 延长会议
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo, CDBEngine * ptDBEngine
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/25  KDVP5.0		阮楠楠                  创建
=============================================================================*/
void CMauVCInstance::DaemonProcDelayConfFinishNtf( const CMessage * pcMsg )
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	
	TMeetingDBFinishOpr tOpr = *(TMeetingDBFinishOpr *)pbyMsg;
	pbyMsg += sizeof( TMeetingDBFinishOpr );
	
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof( TReqHeadInfo );
	
	u16 wDelayTime = *(u16*)pbyMsg; //此处为网络序
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcDelayConfFinishNtf] e164=%s, wMtId=%u, wMccId=%u, wDelayTime=%u.\n", 
		tReqInfo.GetConfE164(), tReqInfo.GetMtReqID(), tReqInfo.GetReqSrcID(), ntohs(wDelayTime) );

	//MCU发来的请求，需要回复给MCU
	TRedisConfInfo tConfInfo;
	BOOL32 bResult = g_cMauVCApp.GetConfExInfo(tReqInfo.GetConfE164(), tConfInfo);
	if (bResult && IsRedisHasConfExData(tConfInfo))
	{
		s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
		u32 dwJsonBufLen = 0;

		SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqInfo);
		SetDelayToJson(achJsonBuf, dwJsonBufLen, ntohs(wDelayTime));
		//加载数据库数据失败
		if (0 != tOpr.GetOprRet())
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcDelayConfFinishNtf] Delay Conf %s failed", tReqInfo.GetConfE164());

			SetErrCodeToJson(achJsonBuf, dwJsonBufLen, 0); //todo 延长会议失败错误码
			SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_MAU_CMU_DELAYCONF_NACK);
			//返回失败消息
			CMauData::SendMsgToCmuSsn(tConfInfo.GetCmuIp(), MAU_MCU_DELAYCONF_NACK, achJsonBuf, dwJsonBufLen);

			return;
		}

		//延长内存中的时间
		u16 wNewDuration = tConfInfo.GetDuration() + ntohs(wDelayTime);
		tConfInfo.SetDuration(wNewDuration);
		if (!g_cMauVCApp.SetConfData(tConfInfo))
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]SetConfData Fail\n", __FUNCTION__);

			SetErrCodeToJson(achJsonBuf, dwJsonBufLen, 0); //todo 延长会议失败错误码
			SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_MAU_CMU_DELAYCONF_NACK);
			CMauData::SendMsgToCmuSsn(tConfInfo.GetCmuIp(), MAU_MCU_DELAYCONF_NACK, achJsonBuf, dwJsonBufLen);
			return;
		}

		LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcDelayConfFinishNtf] Delay conf %s success. NewDuration = %u.\n", tReqInfo.GetConfE164(), wNewDuration);
		SetConfDurationToJson(achJsonBuf, dwJsonBufLen, tConfInfo.GetDuration());

		SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_MAU_CMU_DELAYCONF_ACK);
		CMauData::SendMsgToCmuSsn(tConfInfo.GetCmuIp(), MAU_MCU_DELAYCONF_ACK, achJsonBuf, dwJsonBufLen);			
	}	
}

/*=============================================================================
函 数 名： DaemonProcSubConfDurationFinishNtf
功    能： 
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo, CDBEngine * ptDBEngine
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/25  KDVP5.0		阮楠楠                  创建
=============================================================================*/
void CMauVCInstance::DaemonProcSubConfDurationFinishNtf( const CMessage * pcMsg )
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	
	TMeetingDBFinishOpr tOpr = *(TMeetingDBFinishOpr *)pbyMsg;
	pbyMsg += sizeof( TMeetingDBFinishOpr );
	
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof( TReqHeadInfo );
	
	u16 wDuration = *(u16*)pbyMsg;
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcSubConfDurationFinishNtf] e164=%s, wDuration=%u.\n", 
		tReqInfo.GetConfE164(), wDuration );
	
	//MCU发来的请求，需要回复给MCU
	if (tReqInfo.GetReqEventID() == MCU_MAU_SUBCONFDURATION_REQ)
	{
		TRedisConfInfo tConfInfo;
		BOOL32 bResult = g_cMauVCApp.GetConfExInfo(tReqInfo.GetConfE164(), tConfInfo);
		if (bResult && IsRedisHasConfExData(tConfInfo))
		{
			s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
			u32 dwJsonBufLen = 0;

			SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqInfo);
			SetDelayToJson(achJsonBuf, dwJsonBufLen, wDuration);
			//加载数据库数据失败
			if (0 != tOpr.GetOprRet())
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcDelayConfFinishNtf] Delay Conf %s failed", tReqInfo.GetConfE164());

				SetErrCodeToJson(achJsonBuf, dwJsonBufLen, 0); //todo 延长会议失败错误码
				SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_MAU_CMU_SUBCONFDURATION_NACK);
				//返回失败消息
				CMauData::SendMsgToCmuSsn(tConfInfo.GetCmuIp(), MAU_MCU_SUBCONFDURATION_NACK, achJsonBuf, dwJsonBufLen);

				return;
			}

			TKdvTime tStartTime = FormatStr2KdvTime(tConfInfo.GetRealStartTime());
			time_t dwStart;
			tStartTime.GetTime(dwStart);
			time_t dwNow = time(NULL);
			//会议持续时间 = 已经开会的时间 + 会议希望再召开的时间
			u16 wNewDuration = (u16)((dwNow - dwStart) / 60 + wDuration);
			tConfInfo.SetDuration(wNewDuration);
			if (!g_cMauVCApp.SetConfData(tConfInfo))
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]SetConfData Fail", __FUNCTION__);

				SetErrCodeToJson(achJsonBuf, dwJsonBufLen, 0); //todo 延长会议失败错误码
				SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_MAU_CMU_SUBCONFDURATION_NACK);
				CMauData::SendMsgToCmuSsn(tConfInfo.GetCmuIp(), MAU_MCU_SUBCONFDURATION_NACK, achJsonBuf, dwJsonBufLen);
				return;
			}

			LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcDelayConfFinishNtf] Delay conf %s success. NewDuration = %u.\n", tReqInfo.GetConfE164(), wNewDuration);

			if (tReqInfo.GetReqEventID() == CM_MAU_DELAYCONF_REQ)
			{
				SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_MAU_CMU_SUBCONFDURATION_ACK);
				CMauData::SendMsgToCmuSsn(tConfInfo.GetCmuIp(), MAU_MCU_SUBCONFDURATION_ACK, achJsonBuf, dwJsonBufLen);
			}
		}
		else
		{
			LogPrint(LOG_LVL_WARNING, MID_MAU_VC, "[DaemonProcDelayConfFinishNtf] Get Conf.%s Data failed.\n", tReqInfo.GetConfE164());
		}
	}
}

/*=============================================================================
函 数 名： DaemonProcReleaseConfFinishNtf
功    能： 结会修改会议状态完成
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo, CDBEngine * ptDBEngine
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/25  KDVP5.0		阮楠楠                  创建
=============================================================================*/
void CMauVCInstance::DaemonProcReleaseConfFinishNtf( const CMessage * pcMsg )
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	
	TMeetingDBFinishOpr tOpr = *(TMeetingDBFinishOpr *)pbyMsg;
	pbyMsg += sizeof( TMeetingDBFinishOpr );
	
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof( TReqHeadInfo );
	
	//操作数据库数据失败
	if(ERR_DB_SUCCESS != tOpr.GetOprRet() )
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcReleaseConfFinishNtf] release fail. e164=%s.nMeetingId=%d.\n", 
			tReqInfo.GetConfE164(), tReqInfo.GetMeetingID());	
	}
	else
	{
		u8 byConfState = *(u8*)pbyMsg;
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcReleaseConfFinishNtf] release success. e164=%s.nMeetingId=%d. byState=%d\n",
			tReqInfo.GetConfE164(), tReqInfo.GetMeetingID(), byConfState);
	
		//如果该会议被结束，则删除预约会议里的信息
		CRedisBookConf cBookConf;
		const u8 RELEASE_STATUS = 3;
		if (byConfState == RELEASE_STATUS 
			&& g_cMauVCApp.GetBookConf(tReqInfo.GetConfE164(), cBookConf)
			&& strlen(cBookConf.GetConfE164()) != 0)
		{
			g_cMauVCApp.DelBookConf(tReqInfo.GetConfE164());
			UpuDelConf(tReqInfo.GetConfE164(), TUpuConfInfo::emScheConf);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]upu remove book conf %s\n", __FUNCTION__, tReqInfo.GetConfE164());
		}

		UpuDelConf(tReqInfo.GetConfE164(), TUpuConfInfo::emOnGoingConf);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]upu remove ongoing conf %s\n", __FUNCTION__, tReqInfo.GetConfE164());

		//发送结会通知给会管
		json_t_ptr pjRoot = json_object();
		if (pjRoot)
		{
			CMauData::SetReqRspToJsonObj(pjRoot, tReqInfo);
			SetMsgTypeToJsonObj(pjRoot, EV_MAU_CM_RELEASECONF_NTF);
			json_str pchJson(pjRoot);
			CMauData::SendMsgToCmSsn(MAU_CM_RELEASECONF_NTF, pchJson, strlen(pchJson));
		}
	}
}

/*=============================================================================
函 数 名： DeamonProcSetMeetingE164FinishNtf
功    能： 设置数据库中的E164
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo, CDBEngine * ptDBEngine
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/25  KDVP5.0		阮楠楠                  创建
=============================================================================*/
void CMauVCInstance::DeamonProcSetMeetingE164FinishNtf( const CMessage * pcMsg )
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	
	TMeetingDBFinishOpr tOpr = *(TMeetingDBFinishOpr *)pbyMsg;
	pbyMsg += sizeof( TMeetingDBFinishOpr );
	
	//操作数据库数据失败
	if( 0 != tOpr.GetOprRet() )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_VC, "[DeamonProcSetMeetingE164FinishNtf] set meeting E164 fail.\n" );	
	}
	else
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DeamonProcSetMeetingE164FinishNtf] set meeting E164 success.\n" );	
	}
	
	//更新预约会议信息
	//TDBOprTaskMsgItem tDBMsg;
	//g_cMauVCApp.m_cVMeetingDBOpr.GetBookMeetingList( tDBMsg );

	//TDBOprTaskMsgItem tDBTaskMsg;
	//g_cMauVCApp.m_cScheduledDBOpr.GetAllScheduledConfInfo( tDBTaskMsg );

	//rnn todo加载预约会议的终端列表信息
//	g_cMauVCApp.GetTodayBookConfMtInfo();
	
	return;
}


void CMauVCInstance::DeamonProcMeetingStateCheck(const CMessage * pcMsg, CMauVCApp* pcApp)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();

	TMeetingDBFinishOpr tOpr = *(TMeetingDBFinishOpr *)pbyMsg;

	if (ERR_DB_SUCCESS != tOpr.GetOprRet())
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_VC, "[DeamonProcMeetingStateCheck] check meeting status fail.\n");
		
		//加载预约会议信息
		//会议状态校验
		g_cMauVCApp.m_cVMeetingDBOpr.StateCheck();
	}
	else
	{
//TODO  删除所有数据库中没有的会议
//		g_cMauVCApp.DelConfDataByState(enumInitedState);

		LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DeamonProcMeetingStateCheck] check meeting status success.\n");
		
//		SetInitState(emInitStateCheckConf);
		StartServer(pcApp);
	}

}

u32 CMauVCInstance::AheadCreateBookConf(TReqHeadInfo& tReqInfo, CRedisBookConf& cBookConf)
{
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[%s] AHead Meeting by scheduled conf info!!\n", __FUNCTION__ );

	const s8* pszJsonBuff = cBookConf.GetConfDetailJson().c_str();
	const u32 dwJsonBufLen = cBookConf.GetConfDetailJson().length();

	json_t_ptr pjRoot;
	LoadJson(pszJsonBuff, dwJsonBufLen, pjRoot, TRUE);

	//判断时间是否早于会议原定时间
	time_t tNowTime = time(NULL);
	time_t tOldBeginTime;
	FormatStr2KdvTime(cBookConf.GetBookStartTime()).GetTime(tOldBeginTime);

	u16 wConfDuration = 0;
	// 如果延迟开始了，仍开启到原结束时间
	if (tNowTime > tOldBeginTime)
	{
		//当前时间已经超过会议原定时间，仍然允许创会
		LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcFinishGetScheduledConfNtf] tNowTime - tOldBeginTime > 0,Ahead meeting to oldEndTime.\n");
		
		if (!cBookConf.GetBookEndTime().empty())
		{
			wConfDuration = (u16)(cBookConf.GetDuration() - (tNowTime - tOldBeginTime) / 60);
		}
		LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcFinishGetScheduledConfNtf] ConfDurtion is %d\n", wConfDuration);

	}
	else
	{
		//如果提前了超过一小时召开，则设置持续时间为一小时，否则持续到原结束时间
		u16 wAheadTime = (u16)(tOldBeginTime - tNowTime);
		if (wAheadTime > 60 * 60)
		{
			//临时会议
			wConfDuration = 60;
		}
		else
		{
			if (!cBookConf.GetBookEndTime().empty())
			{
				wConfDuration = (u16)(cBookConf.GetDuration() + (tOldBeginTime - tNowTime) / 60);
			}
		}
		LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcFinishGetScheduledConfNtf] tNowTime - tOldBeginTime > 0,Ahead meeting.dwConfDuration = %d.\n",
			wConfDuration);
	}

	SetUIntAsStringToJson(pjRoot, JSON_DURATION, wConfDuration);

	s8 achTmpBuf[20];
	time_t tCurTime = time(NULL);
	TKdvTime tStartTime(&tCurTime);
	tStartTime.GetString(achTmpBuf, sizeof(achTmpBuf));
	SetStringToJson(pjRoot, JSON_CONFSTARTTIME, achTmpBuf);

	//获取终端所在域的详细信息
	CKdvDomainInfo tDomainInfo;
	g_cMauVCApp.GetDomainInfo( tReqInfo.GetDomainMOID(), tDomainInfo );
	
	const s8* pchConfName = "";
	GetConfNameFromJsonObj(pjRoot, &pchConfName);

	//会议重名处理
	s8 achConfName[MAXLEN_CONFNAME + 1] = { 0 };
	BOOL32 bConfNameModify = FALSE;
	if (!g_cMauVCApp.ConfNameRepeatProc(tDomainInfo, pchConfName,
		tReqInfo.GetConfE164(), achConfName, MAXLEN_CONFNAME, bConfNameModify))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]ConfNameRepeatProc Fail\n", __FUNCTION__);
		return ERR_DB_OPR;
	}
	else if(bConfNameModify)
	{	
		SetStringToJson(pjRoot, JSON_CONFNAME, achConfName);
	}

	// 获得终端列表及呼叫码率列表
	u16 wMtNum = 0;
	u16 awMtDialRate[MAXNUM_CONF_MT] = { 0 };
	TMtAlias atMtAlias[MAXNUM_CONF_MT];
	GetMtAliasListFromJson(pjRoot, atMtAlias, awMtDialRate, wMtNum);

	TMixModule tMixModule;
	GetMixModuleFromJson(pjRoot, tMixModule);

	TVmpModuleInfo tVmpModule;
	GetVmpModuleFromJson(pjRoot, tVmpModule);

	TMiniPollInfo tPollModule;
	CMauData::GetPollModuleFromJson(pjRoot, tPollModule);

	u8 abyVipList[MAXNUM_VIP] = { 0 };
	u16 wVipNum = 0;
	GetVipMtListFromJson(pjRoot, abyVipList, wVipNum);

	u16 wMaxJoinedMt = 0;
	CMauData::GetConfMaxJoinedMtNum(pjRoot, wMaxJoinedMt);

	// TODO: 设置会议码率
	//将呼叫终端插入会议列表
	CMauData::InsertReqMt(tReqInfo, 0, atMtAlias, awMtDialRate, wMtNum, wMaxJoinedMt, tVmpModule, tPollModule, tMixModule, abyVipList);

	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcAheadMeetingFinishNtf] AHead Meeting success!!\n");

	//	TODO 区分电话和终端
	//  	if( MT_TYPE_PHONE ==  tReqInfo.GetMtType() )
	//  	{
	//  		cServMsg.SetSrcMtId( CONF_CREATE_PHONE );
	//  	}
	//  	else
	//  	{
	//  		cServMsg.SetSrcMtId( CONF_CREATE_MT );
	//  	}

	//判断模板中是否有设置主席/发言人
	TMtAlias tMtAlias;
	tMtAlias.SetE164Alias( tReqInfo.GetSrcMtE164() );

	const s8* pchChairMan = "";
	GetChairAliasFromJsonObj(pjRoot, &pchChairMan);
	//主席为空
	if(strlen(pchChairMan) == 0)
	{
		SetChairAliasToJsonObj(pjRoot, tMtAlias);
	}
	
	const s8* pchSpeaker = "";
	GetSpeakerAliasFromJsonObj(pjRoot, &pchSpeaker);
	//发言人为空
	if (strlen(pchSpeaker) == 0)
	{
		SetSpeakerAliasToJsonObj(pjRoot, tMtAlias);
	}

	SetMsgTypeToJsonObj(pjRoot, EV_MAU_CMU_CREATECONF_REQ);
	SetMixModuleToJsonObj(pjRoot, tMixModule);
	SetVmpModuleInfoToJsonObj(pjRoot, tVmpModule);
	CMauData::SetPollModuleToJson(pjRoot, tPollModule);
	SetReqHeadToJsonObj(pjRoot, tReqInfo);
	SetMtAliasListToJson(pjRoot, atMtAlias, awMtDialRate, wMtNum);
	SetVipMtListToJsonObj(pjRoot, abyVipList, wVipNum);
	SetConfE164ToJsonObj(pjRoot, cBookConf.GetConfE164());

	json_str pszJsonTemp(pjRoot);
	u32 dwLen = strlen(pszJsonTemp);
// TODO: 终端入会密码设置
// 	if ( 0 != strlen( tConfInfo.GetConfPwd() ) )
// 	{
// 		TConfAttrb tConfAttrib = tConfInfo.GetConfAttrb();
// 		tConfAttrib.SetNeedConfPwd(TRUE);
// 		tConfInfo.SetConfAttrb(tConfAttrib);
// 		TConfInfo tConfInfo;
// 		TPassword tPassword;
// 		tPassword.SetPassword((s8*)tConfInfo.GetConfPwd(), TRUE);
// 		tConfInfo.SetConfPwd( tPassword.GetPassword() );	
// 	}
// 	else
// 	{
// 		LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcFinishGetScheduledConfNtf]join conf psw is empty.\n");
// 	}
	
	TDBOprTaskMsgItem tDBTaskMsg;
	tDBTaskMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
	s32 nMeetingId = cBookConf.GetMeetingID();
	tDBTaskMsg.CatMsgBody(&nMeetingId, sizeof(s32));
	tDBTaskMsg.CatMsgBody(&dwLen, sizeof(dwLen));
	tDBTaskMsg.CatMsgBody(pszJsonTemp, dwLen);

	g_cMauVCApp.m_cVMeetingDBOpr.AheadMeeting( tDBTaskMsg );
	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： DaemonProcMeetingFinishOprNtf
功    能： 处理完成会管数据库操作通知
算法实现： 
全局变量： 
参    数： 
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauVCInstance::DaemonProcMeetingFinishOprNtf( const CMessage * pcMsg, CMauVCApp* pcApp)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	TMeetingDBFinishOpr tOpr = *(TMeetingDBFinishOpr*)cServMsg.GetMsgBody();
	
	switch (tOpr.GetOprType())
	{
	case CVMeetingDBOpr::CREATE_MEETING_OPR:                    //非模板创会
		DaemonProcCreateMeetingFinishNtf( pcMsg );
		break;
    case CVMeetingDBOpr::AHEAD_MEETING:							//提前召开
		DaemonProcAheadMeetingFinishNtf( pcMsg );
		break;
	case CVMeetingDBOpr::DELAY_CONFTIME_ORP:					//完成延长会议在数据库中创会操作
		DaemonProcDelayConfFinishNtf( pcMsg );
		break;
	case CVMeetingDBOpr::SUB_CONF_DURATION_ORP:                 
		DaemonProcSubConfDurationFinishNtf( pcMsg );		    //完成缩短会议运行时间操作
		break;
	case CVMeetingDBOpr::RELEASE_CONF_OPR:						//完成结会修改会议状态操作
		DaemonProcReleaseConfFinishNtf( pcMsg );
		break;		
	case CVMeetingDBOpr::SET_MEETING_E164_OPR:					//完成设置DB中的E164操作
		DeamonProcSetMeetingE164FinishNtf( pcMsg );
		break;
	case CVMeetingDBOpr::STATE_CHECK:
		DeamonProcMeetingStateCheck(pcMsg, pcApp);
		break;
	default:
		LogPrint( LOG_LVL_WARNING, MID_MAU_VC, "[DaemonProcMeetingFinishOprNtf] Unknow Opr Type.\n");
		break;
	}
	
	return;
}

/*=============================================================================
函 数 名： DaemonProcMpcDisConnectNtf
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
void CMauVCInstance::DaemonProcMpcDisConnectNtf( const CMessage * pcMsg )
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC, "mcu:%s:%s disconnect, mcu type: %s\n",
		((TCmuInfo*)pcMsg->content)->achMcuIP, ((TCmuInfo*)pcMsg->content)->achMcuPid,
		((TCmuInfo*)pcMsg->content)->achMcuType);

	const TCmuInfo* ptCmuInfo = (TCmuInfo*)pcMsg->content;
	g_cMauVCApp.RemoveMpc(ptCmuInfo->achMcuIP);

	list<TRedisConfInfo> confList;
	if (!g_cMauVCApp.GetConfExListByCmuIp(ptCmuInfo->achMcuIP, confList))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetConfExListByCmuIp Fail\n", __FUNCTION__);
		return;
	}
	for (list<TRedisConfInfo>::iterator confIter = confList.begin(); confIter != confList.end(); )
	{
		//若是没数据且处于创建状态的会议，将不会恢复成功，且由CHECKCONFTIMER删除
		if (!g_cMauVCApp.ChangeConfState(confIter->GetDomainMOID(),
			confIter->GetConfE164(), enumConnectWaitingState))
		{
			confIter = confList.erase(confIter);
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]ChangeConfState Fail\n", __FUNCTION__);
			continue;
		}
		else
		{
			confIter->SetState(enumConnectWaitingState);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]change ConfE164: %s to waiting state\n",
				__FUNCTION__, confIter->GetConfE164());
			++confIter;
		}
	}

	if(IsNewMcu(ptCmuInfo->achMcuType))
	{
		RecoverConfList(confList);
	}
	return;
}


/*=============================================================================
函 数 名： DaemonProcMpcConnectNtf
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
void CMauVCInstance::DaemonProcMpcConnectNtf( const CMessage * pcMsg )
{
	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	
	const u8* pBuff = cServMsg.GetMsgBody();
	const TCmuInfo* ptCmuInfo = (const TCmuInfo*)pBuff;

    TCmuData tCmuData;
	g_cMauVCApp.GetMpcData(ptCmuInfo->achMcuIP, tCmuData);
	tCmuData.m_tInfo = *ptCmuInfo;
	g_cMauVCApp.SetMpcData(tCmuData);
}

/*=============================================================================
函 数 名： DaemonProcCmuConfListNtf
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
void CMauVCInstance::DaemonProcCmuConfListNtf( const CMessage * pcMsg )
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_VC);
	const s8* pchJsonBuf = (const s8*)pcMsg->content;
	u16 wConfMcuNum = 0;
	TMpcConfData atConfData[MAXNUM_MCU_CONF];
	CMauData::GetConfDataFromJson(pchJsonBuf, atConfData, wConfMcuNum);
	s8 achCmuIp[MAXLEN_IPADDR + 1] = { 0 };
	GetCmuIpFromJson(pchJsonBuf, achCmuIp, MAXLEN_IPADDR);
	u32 dwCmuPid = 0;
	GetCmuPidFromJson(pchJsonBuf, dwCmuPid);

	//调整mcu上报的会与redis上的会，保证数据的一致性
	if (!AccordRedisConfAndCmuConfList(atConfData, wConfMcuNum, achCmuIp, dwCmuPid))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]AccordRedisConfAndCmuConfList Fail\n", __FUNCTION__);
		return;
	}

	//检测redis有该mcu的会，但mcu上报会议列表没有会
	//若是旧mcu则删除会议信息
	//否则恢复该会议(相关逻辑: 1.启动时36秒开始恢复 2.恢复失败不处理，仍然10秒恢复一次)
	BOOL32 bOldMcu = FALSE;
	TCmuData tCmuData;
	if (g_cMauVCApp.GetMpcData(achCmuIp, tCmuData)) 
	{
		bOldMcu = !IsNewMcu(tCmuData.GetCmuType());
	}
	else
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetMpcData<%s> fail\n", __FUNCTION__, achCmuIp);
		return;
	}
	list<TRedisConfInfo> confList;
	if (g_cMauVCApp.GetConfExListByCmuIp(achCmuIp, confList))
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]mcu<%s> has %u confs\n",
			__FUNCTION__, achCmuIp, confList.size());
	}
	else
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetConfExListByCmuIp<%s> Error\n",
			__FUNCTION__, achCmuIp);
		return;
	}
	list<TRedisConfInfo>::iterator confIter = confList.begin();
	for (; confIter != confList.end(); ++confIter)
	{
		BOOL32 bExist = TRUE;
		if (IsConfInCmuConfList(confIter->GetConfE164(), atConfData, wConfMcuNum, bExist))
		{
			if (!bExist)
			{
				const BOOL32 bConfReleased = (confIter->GetCmuPid() == dwCmuPid
					&& strcmp(confIter->GetCmuIp(), achCmuIp) == 0);
				if (confIter->GetState() == enumPrepareState)
				{
					continue;
				}
				else if (bOldMcu || bConfReleased)
				{
					ReleaseConf(confIter->GetDomainMOID(), confIter->GetConfE164(), confIter->GetCmuIp());
					LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]oldMcu, redisConf<%s> inexistence in cmuconflist, release\n",
						__FUNCTION__, confIter->GetConfE164());
				}
				else
				{
					confIter->SetState(enumConnectWaitingState);
					RecoverConf(*confIter);
					LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]not oldMcu, redisConf<%s> inexistence in cmuconflist, recover\n",
						__FUNCTION__, confIter->GetConfE164());
				}
			}
			else
			{
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]conf<%s> in mcu conflist\n",
					__FUNCTION__, confIter->GetConfE164());
			}
		}
		else
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]IsConfInCmuConfList Error\n", __FUNCTION__);
		}
	}

	//获取mcu信息
	//TCmuData tCmuData;
	//g_cMauVCApp.GetMpcData(GETINS(pcMsg->srcid), tCmuData);

 //   u16 wConfMcuIdx = 0;
	//for (wConfMcuIdx = 0; wConfMcuIdx < wConfMcuNum; wConfMcuIdx++)
	//{
	//	BOOL32 bNeedRelease = TRUE;
	//	TRedisConfInfo tConfInfo;
	//	TMpcConfData& tMpcData = atConfData[wConfMcuIdx];
	//	if (g_cMauVCApp.GetConfExInfo(tMpcData.GetConfIdAttr().GetConfE164(), tConfInfo))
	//	{
	//		time_t starttime;
	//		tMpcData.GetStartTime().GetTime(starttime);

	//		TKdvTime tRealStartTime = FormatStr2KdvTime(tConfInfo.GetRealStartTime());
	//		time_t tmRealStart;
	//		tRealStartTime.GetTime(tmRealStart);
	//		// 如果上报的会议先召开
	//		if (tmRealStart >= starttime)
	//		{
	//			// 如果不在同一个mcu上
	//			if (strcmp(tConfInfo.GetCmuIp(), achCmuIp) != 0)
	//			{
	//				// 删除后开的会议
	//				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcCmuConfListNtf]Conf %s @ Mpc<%s> is Newer, Release it!\n",
	//					tMpcData.GetConfIdAttr().GetConfE164(), tConfInfo.GetCmuIp());
	//				SendReleaseConfToCmu(tMpcData.GetConfIdAttr().GetConfE164(),
	//					tMpcData.GetConfIdAttr().GetConfName(),
	//					tConfInfo.GetCmuIp(), FALSE);
	//			}

	//			s8 pchRealStartTime[30] = { 0 };
	//			tMpcData.GetStartTime().GetString(pchRealStartTime, 30);
	//			tConfInfo.SetRealStartTime(pchRealStartTime);
	//			tConfInfo.SetCmuIp(achCmuIp);
	//			tConfInfo.SetCmuPid(dwCmuPid);
	//			tConfInfo.SetState(enumRunningState);
	//			g_cMauVCApp.SetConfData(tConfInfo);
	//
 //				AddConfInfo(tMpcData.GetConfIdAttr().GetConfE164(),
	//				tMpcData.GetConfIdAttr().GetConfName(),
	//				tMpcData.GetConfIdAttr().GetConfPwd());
	//		}
	//		// 如果上报会议后召开
	//		else
	//		{
	//			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcCmuConfListNtf]Conf %s @ Mpc<%s> is Newer, Release it!\n",
	//				tMpcData.GetConfIdAttr().GetConfE164(), achCmuIp);
	//			// 结束上报的会议
	//			SendReleaseConfToCmu(tMpcData.GetConfIdAttr().GetConfE164(),
	//				tMpcData.GetConfIdAttr().GetConfName(),
	//				achCmuIp, FALSE);
	//		}
	//	}
	//	// 如果该会议在css里不存在，则直接结掉
	//	else
	//	{
	//		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcCmuConfListNtf]Conf<%s> @ Mpc<%s> Unexist, Release it!\n",
	//			tMpcData.GetConfIdAttr().GetConfE164(), achCmuIp);
	//		SendReleaseConfToCmu(tMpcData.GetConfIdAttr().GetConfE164(),
	//			tMpcData.GetConfIdAttr().GetConfName(),
	//			achCmuIp, FALSE);
	//	}

	//	if (IsNewMcu(tCmuData.GetCmuType()))
	//	{
	//		CKdvDomainInfo tDomainInfo;
	//		g_cMauVCApp.GetDomainInfo(tConfInfo.GetDomainMOID(), tDomainInfo);
	//		TUpuConfInfo tUpuConfInfo;
	//		tUpuConfInfo.AddConfType(TUpuConfInfo::emOnGoingConf);
	//		tUpuConfInfo.SetE164(tConfInfo.GetConfE164());
	//		tUpuConfInfo.SetDomainMoid(tConfInfo.GetDomainMOID());
	//		tUpuConfInfo.SetPlatformMoid(tDomainInfo.GetSDomainMOID());
	//		tUpuConfInfo.SetMcuIp(inet_addr(tConfInfo.GetCmuIp()));
	//		UpuAddConf(tUpuConfInfo);
	//	}
	//}


//	std::list<TRedisConfInfo> lstRedisConf;
//	g_cMauVCApp.GetConfExListByCmuIp(achCmuIp, lstRedisConf);
//
//	// 过滤掉已经存在的会议
//	u32 i = 0;
//	for (; i < wConfMcuNum; ++i)
//	{
//		std::list<TRedisConfInfo>::iterator it = lstRedisConf.begin();
//		for (; it != lstRedisConf.end(); ++it)
//		{
//			if (strcmp(atConfData[i].GetConfIdAttr().GetConfE164(), it->GetConfE164()) == 0)
//			{
//				lstRedisConf.erase(it);
//				break;
//			}
//		}
//	}
//
//	std::list<TRedisConfInfo>::iterator it = lstRedisConf.begin();
//	for (; it != lstRedisConf.end(); ++it)
//	{
//// 		// 如果cmu重启过，需要恢复会议
//// 		if (it->GetCmuPid() != dwCmuPid)
//// 		{
//// 			//恢复会议
//// 		}
//// 		// 否则代表会议是主动结束的，删除会议信息
//// 		else
//// 		{
//			::OspPrintf(TRUE, FALSE, "[%s] Conf %s on %s has been released by cmu, remove it!\n",
//				__FUNCTION__, it->GetConfE164(), it->GetCmuIp());
//			g_cMauVCApp.DelConf(it->GetDomainMOID(), it->GetConfE164(), it->GetCmuIp());
////		}
//	}
//	// 恢复会议逻辑
}

void CMauVCInstance::SendReleaseConfToCmu(const s8* pchConfE164, const s8* pchConfName, const s8* pchCmuIp, BOOL32 bEndAll)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC, "mcuIp:%s, confE164: %s, confName: %s",
		pchCmuIp, pchConfE164, pchConfName);
	json_t_ptr pjRoot = json_object();
	SetMsgTypeToJsonObj(pjRoot, EV_MAU_MCU_RELEASECONF_CMD);
	SetEndAllSubConfToJsonObj(pjRoot, bEndAll);
	SetConfE164ToJsonObj(pjRoot, pchConfE164);
	SetCmuIpToJsonObj(pjRoot, pchCmuIp);

	//发送给cmu结束会议
	json_str pchJsonBuff(pjRoot);
	CMauData::SendMsgToCmuSsn(pchCmuIp, MAU_MCU_RELEASECONF_CMD, pchJsonBuff, strlen(pchJsonBuff));
}

void CMauVCInstance::SendUpdateAllConfDataToCmu(
	const s8* pchCmuIp,
	const s8* pchConfE164,
	const s8* pchConfName,
	const s8* pchDomainMOID)
{
	json_t_ptr pjRoot = json_object();
	SetMsgTypeToJsonObj(pjRoot, EV_MAU_MCU_UPDATEALLCONFDATA_CMD);
	SetConfE164ToJsonObj(pjRoot, pchConfE164);
	SetDomainMoidToJsonObj(pjRoot, pchDomainMOID);
	SetConfNameToJsonObj(pjRoot, pchConfName);

	json_str strJson(pjRoot);
	CMauData::SendMsgToCmuSsn(pchCmuIp, MAU_MCU_UPDATEALLCONFDATA_CMD, strJson, strlen(strJson));
}

/*=============================================================================
函 数 名： DaemonProcCmuMsg
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
void CMauVCInstance::DaemonProcCmuMsg( const CMessage * pcMsg )
{
	CHECK_STATE(STATE_NORMAL, return);

	LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DaemonProcCmuMsg]Recv msg.%u<%s>\n", 
		pcMsg->event, OspEventDesc(pcMsg->event) );

	const s8* pchJson = (const s8*)pcMsg->content;

	TReqHeadInfo tReqInfo;
	GetReqHeadFromJson(pchJson, tReqInfo);
	s8 achCmuIp[MAXLEN_IPADDR + 1] = { 0 };
	GetCmuIpFromJson(pchJson, achCmuIp, MAXLEN_IPADDR);

	//任何MCU操作都需要做归属判断
	if(!g_cMauVCApp.IsConfExistAndBelongMcu(tReqInfo.GetConfE164(), achCmuIp))
	{
		return;
	}

	switch ( pcMsg->event )
	{
	case MCU_MAU_ADDMT_ACK:
		{
			LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DaemonProcCmuMsg] receive add mt<%s> ack reply in conf<%s> MtReqId<%d>.\n ",
				tReqInfo.GetSrcMtE164(), tReqInfo.GetConfE164(), tReqInfo.GetMtReqID());
			CMauMsg cServMsg;
			cServMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
			SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 1, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
		}
		break;
	case MCU_MAU_ADDMT_NACK:
		{
			u16 wErrCode = 0;
			GetErrCodeFromJson(pchJson, wErrCode);
			tReqInfo.SetErrID(wErrCode);
			LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DaemonProcCmuMsg] receive add mt<%s> nack reply in conf<%s> MtReqId<%d> with error<%d>\n ",
				tReqInfo.GetSrcMtE164(), tReqInfo.GetConfE164(), tReqInfo.GetErrID(), tReqInfo.GetMtReqID());
			CMauMsg cServMsg;
			cServMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
			SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
		}
		break;

	case MCU_MAU_SUBCONFDURATION_REQ:
		{
			OspPrintf(TRUE, FALSE, "[ProcSubConfDurationReq] MPC_MPCD_SUBCONFDURATION_REQ.\n");

			u16 wSubTime = 0;
			GetDelayFromJson( pchJson, wSubTime );
			
			TDBOprTaskMsgItem tDBMsg;
			tDBMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
			tDBMsg.CatMsgBody(&wSubTime, sizeof(u16));
			
			g_cMauVCApp.m_cVMeetingDBOpr.SubConfDuration( tDBMsg );
		}
		break;

	case MCU_MAU_DELAYCONF_REQ:
		{
			u16 wDelayTime = 0;
			GetDelayFromJson( pchJson, wDelayTime );

			s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
			u32 dwJsonBufLen = 0;
			
			SetReqHeadToJson( achJsonBuf, dwJsonBufLen, tReqInfo );
			SetDelayToJson( achJsonBuf, dwJsonBufLen, wDelayTime );
			
			//获取当前会议的持续时间
			TRedisConfInfo tConfInfo;
			BOOL32 bResult = g_cMauVCApp.GetConfExInfo(tReqInfo.GetConfE164(), tConfInfo);
			if(!bResult 
				|| !IsRedisHasConfExData(tConfInfo))
			{
				LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DaemonProcCmuMsg] GetConfDataByE164 Fail.Delay Conf Nack.\n");
				
				//返回失败消息
				SetErrCodeToJson( achJsonBuf, dwJsonBufLen, 0 ); //todo 延长会议失败错误码
				SetMsgTypeToJson( achJsonBuf, dwJsonBufLen, EV_MAU_CMU_DELAYCONF_NACK );
				CMauData::SendMsgToCmuSsn(achCmuIp, MAU_MCU_DELAYCONF_NACK, achJsonBuf, dwJsonBufLen);
				return;
			}
			else
			{
				LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DaemonProcCmuMsg] now conf duration = %u.\n", tConfInfo.GetDuration());
			}

			//永久会议不支持延长
			if (!tConfInfo.GetDuration())
			{
				LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DaemonProcCmuMsg] conf duration is 0. Delay fail.\n");

				//返回失败消息
				SetErrCodeToJson(achJsonBuf, dwJsonBufLen, 0); //todo 延长会议失败错误码
				SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_MAU_CMU_DELAYCONF_NACK);
				CMauData::SendMsgToCmuSsn(achCmuIp, MAU_MCU_DELAYCONF_NACK, achJsonBuf, dwJsonBufLen);
				return;
			}

			//校验延长时间是否合法
			if ( (wDelayTime + tConfInfo.GetDuration() > 0xffff ) )
			{
				LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DaemonProcCmuMsg] Conf Time > 0xffff. Delay fail.\n");
				
				//返回失败消息
				SetErrCodeToJson( achJsonBuf, dwJsonBufLen, 0 ); //todo 延长会议失败错误码
				SetMsgTypeToJson( achJsonBuf, dwJsonBufLen, EV_MAU_CMU_DELAYCONF_NACK );
				CMauData::SendMsgToCmuSsn(achCmuIp, MAU_MCU_DELAYCONF_NACK, achJsonBuf, dwJsonBufLen);
				return;
			}
			
			//获取会议所在域信息 - 后面会用到最大会议数
			CKdvDomainInfo tDomainInfo;
			
			//获取终端所在域的详细信息
			g_cMauVCApp.GetDomainInfo(tConfInfo.GetDomainMOID(), tDomainInfo );
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DaemonProcCmuMsg] max confNum = %u.\n", tDomainInfo.GetMaxConfNum() );
			
			TDBOprTaskMsgItem tDBMsg;
			
			wDelayTime = htons( wDelayTime );
			
			tDBMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
			tDBMsg.CatMsgBody(&wDelayTime, sizeof(wDelayTime));
			tDBMsg.CatMsgBody(&tDomainInfo, sizeof(tDomainInfo));
			tDBMsg.CatMsgBody(&tConfInfo, sizeof(tConfInfo));

			g_cMauVCApp.m_cVMeetingDBOpr.DelayConf(tDBMsg);
			return;
		}
		break;
	case MCU_MAU_MODIFYCONFNAME_REQ:
		{
 			s8 achConfName[MAXLEN_CONFNAME + 1] = { 0 };
 			GetConfNameFromJson((s8*)pchJson, achConfName, MAXLEN_CONFNAME);
 
			BOOL32 bConfNameModify = FALSE;
			TRedisConfInfo tRedisConf;
			u32 dwJsonBufLen = strlen(pchJson);
			BOOL32 bCanModify = (g_cMauVCApp.GetConfExInfo(tReqInfo.GetConfE164(), tRedisConf)
				&& IsRedisHasConfExData(tRedisConf)
				&& g_cMauVCApp.IsConfNameExist(tRedisConf.GetDomainMOID(),
					achConfName, tReqInfo.GetConfE164(), bConfNameModify)
				&& !bConfNameModify);
			if(bCanModify)
			{
				tRedisConf.SetConfName(achConfName);
				if (g_cMauVCApp.SetConfData(tRedisConf))
				{
					g_cMauVCApp.m_cConfInfoDBOpr.ModConfName(tReqInfo.GetConfE164(), achConfName);
					LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Modify ConfName, ConfE164: %s, ConfName: %s\n",
															__FUNCTION__, tReqInfo.GetConfE164(), achConfName);
				}
				else
				{
					bCanModify = FALSE;
					LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]SetConfData Fail\n", __FUNCTION__);
				}
			}
			else
			{
				LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[%s]ConfE164: %s unexist or confname has exist\n", 
						__FUNCTION__, tReqInfo.GetConfE164());
			}
			SetMsgTypeToJson((s8*)pchJson, dwJsonBufLen,
				bCanModify ? EV_MAU_CMU_MODIFYCONFNAME_ACK : EV_MAU_CMU_MODIFYCONFNAME_NACK);
 			CMauData::SendMsgToCmuSsn(achCmuIp, CM_CMU_CONFOPR_NTF, pchJson, dwJsonBufLen);
		}
		break;

//  更新confinfodb
 	case MCU_MAU_CONFSTATECHANGE_NTF:
		{
			u8 byType = 0;
			if (!GetConfStateTypeFromJson((s8*)pchJson, byType))
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetConfStateTypeFromJson<%s> fail\n", 
					__FUNCTION__, tReqInfo.GetConfE164());
				return;
			}
			TRedisConfInfo tRedisConf;
			if (!g_cMauVCApp.GetConfExInfo(tReqInfo.GetConfE164(), tRedisConf)
				|| !IsRedisHasConfExData(tRedisConf))
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]ConfStateChange<type: %u> getconfexinfo<%s> fail\n",
					__FUNCTION__, byType, tReqInfo.GetConfE164());
				return;
			}

			if (emModifyConfPwdType == byType)
			{
				u8 byNeedPwd = 0;
				s8 achConfPwd[MAXLEN_PWD + 1] = { 0 };
				CMauData::GetConfPwdFromJson((s8*)pchJson, byNeedPwd, achConfPwd, MAXLEN_PWD);
				AddConfInfo(tReqInfo.GetConfE164(), tRedisConf.GetConfName(), achConfPwd);
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcCmuMsg] receive conf<%s:%s> change "
					" to newneedpwd<%d> pwd<%s> from mpc<%s>\n ",
					tReqInfo.GetConfE164(), tRedisConf.GetConfName(), byNeedPwd, achConfPwd, achCmuIp);
			}
			else if (emModifyConfNameType == byType)
			{
				s8 achConfName[MAXLEN_CONFNAME + 1] = { 0 };
				GetConfNameFromJson(pchJson, achConfName, MAXLEN_CONFNAME);
				tRedisConf.SetConfName(achConfName);
				g_cMauVCApp.SetConfData(tRedisConf);
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcCmuMsg] receive conf<%s> change "
					" confName: %s\n ",
					tReqInfo.GetConfE164(), tRedisConf.GetConfName());
			}
		}
		break;
	default:
		break;
	}

	return;
}

void CMauVCInstance::DaemonProcCreateConfAck(const CMessage *  pcMsg)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC, "msg: %s", OspEventDesc(pcMsg->event));

	CHECK_STATE(STATE_NORMAL, return);

	const s8* pchJson = (const s8*)pcMsg->content;
	s8 achCmuIp[MAXLEN_IPADDR + 1] = { 0 };
	GetCmuIpFromJson(pchJson, achCmuIp, MAXLEN_IPADDR);
	TConfInfo tConfInfo;
	GetCreateConfParamFromJson(pchJson, tConfInfo);

	// 加入会议恢复逻辑之后，mcu的任何操作都需要判断会议是否存在及若存在是否归属于该MCU
	// 会议的归属以redis:confex信息为准
	if (!g_cMauVCApp.IsConfExistAndBelongMcu(tConfInfo.GetConfE164(), achCmuIp))
	{
		SendReleaseConfToCmu(tConfInfo.GetConfE164(), tConfInfo.GetConfName(), achCmuIp, FALSE);
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[%s]conf<E164:%s, name: %s> unbelong to mcu: %s, release\n", 
			__FUNCTION__, tConfInfo.GetConfE164(), tConfInfo.GetConfName(), achCmuIp);
		return;
	}

	TRedisConfInfo tRedisInfo;
	if(g_cMauVCApp.GetConfExInfo(tConfInfo.GetConfE164(), tRedisInfo)
		&& IsRedisHasConfExData(tRedisInfo)
	    && g_cMauVCApp.ChangeConfState(tConfInfo.GetDomainMoid(), tConfInfo.GetConfE164(), enumRunningState))
	{
		u32 dwMeetingId = tRedisInfo.GetMeetingID();
		TDBOprTaskMsgItem tDBTaskMsg;
		tDBTaskMsg.SetMsgBody(&dwMeetingId, sizeof(dwMeetingId));
		g_cMauVCApp.m_cVMeetingDBOpr.ChangeVmState(tDBTaskMsg);

		// 将会议信息写入数据库
		AddConfInfo(tConfInfo.GetConfE164(), tConfInfo.GetConfName(), tConfInfo.GetConfPwd());
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]AddConfInfo<%s:%s:%s> to db\n",
			__FUNCTION__, tConfInfo.GetConfE164(), tConfInfo.GetConfName(), tConfInfo.GetConfPwd());

		s8 abyMcuUrl[MAXLEN_MCUURL + 1] = { 0 };
		GetMcuUrlFromJson(pchJson, abyMcuUrl, sizeof(abyMcuUrl) - 1);
		if (strcmp(abyMcuUrl, "") == 0)
		{
			CKdvDomainInfo tDomainInfo;
			if (g_cMauVCApp.GetDomainInfo(tConfInfo.GetDomainMoid(), tDomainInfo))
			{
				UpuAddConf(tConfInfo.GetConfE164(), TUpuConfInfo::emOnGoingConf,
					tDomainInfo.GetDomainMOID(), tDomainInfo.GetSDomainMOID(), tRedisInfo.GetCmuIp());
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]notOldMcuConf<%s:%s:%s>, add upu ongoingconf\n",
					__FUNCTION__, tConfInfo.GetConfE164(), tConfInfo.GetConfName(), tRedisInfo.GetCmuIp());
			}
			else
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]getdomain<%s> fail\n",
					__FUNCTION__, tConfInfo.GetDomainMoid());
			}
		}

		if(pcMsg->event == MCU_MAU_CREATECONF_ACK)
		{
			CMauMsg cRspAckMsg;
			TReqHeadInfo tReqInfo;
			GetReqHeadFromJson(pchJson, tReqInfo);
			tReqInfo.SetConfE164(tConfInfo.GetConfE164());
			tReqInfo.SetMeetingID(dwMeetingId);
			cRspAckMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
			cRspAckMsg.CatMsgBody(tConfInfo.GetConfE164(), (MAXLEN_CONFE164 + 1));
			SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 1, cRspAckMsg.GetServMsg(), cRspAckMsg.GetServMsgLen());
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]ConfE164.%s, reply ack<ori:%s>\n", 
				__FUNCTION__, tConfInfo.GetConfE164(), OspEventDesc(tReqInfo.GetReqEventID()));
		}
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]ack from Mcu<%s>, Change Conf<%s:%s:%s> to running state sucess\n",
			__FUNCTION__, achCmuIp, tRedisInfo.GetConfName(), tRedisInfo.GetConfE164(), tRedisInfo.GetDomainMOID());
	}
	else
	{
		SendReleaseConfToCmu(tConfInfo.GetConfE164(), tConfInfo.GetConfName(), achCmuIp, FALSE);
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Change conf<%s> state to running fail, notice mcu release conf\n",
			__FUNCTION__, tConfInfo.GetConfE164());
	}
}

void CMauVCInstance::DaemonProcCreateConfNack(const CMessage *  pcMsg)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC);

	CHECK_STATE(STATE_NORMAL, return);

	const s8* pchJson = (const s8*)pcMsg->content;
	s8 achCmuIp[MAXLEN_IPADDR + 1] = { 0 };
	GetCmuIpFromJson(pchJson, achCmuIp, MAXLEN_IPADDR);
	s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
	GetConfE164FromJson(pchJson, achConfE164, MAXLEN_CONFE164);

	//若删除redis会议失败，则不做其他处理，以redis数据为准
	TRedisConfInfo tRedisConfInfo;
	if (g_cMauVCApp.IsConfExistAndBelongMcu(achConfE164, achCmuIp)
		&& g_cMauVCApp.GetConfExInfo(achConfE164, tRedisConfInfo)
		&& IsRedisHasConfExData(tRedisConfInfo))
	{
		const u32 nMeetingID = tRedisConfInfo.GetMeetingID();
		g_cMauVCApp.m_cVMeetingDBOpr.CreateMeetingFailed(nMeetingID);
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]meetingID<%d> ChangeVmState:CMU_MAU_CREATECONF_NACK.\n",
			__FUNCTION__, nMeetingID);

		if (g_cMauVCApp.DelConf(tRedisConfInfo.GetDomainMOID(), tRedisConfInfo.GetConfE164(), achCmuIp))
		{
			LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]DelConf<e164:%s> success\n",
				__FUNCTION__, achConfE164);
		}
		else
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Del conf<e164:%s> fail\n",
				__FUNCTION__, achConfE164);
		}
	}
	
		
	TReqHeadInfo tReqInfo;
	GetReqHeadFromJson(pchJson, tReqInfo);
	u16 wErrCode = 0;
	GetErrCodeFromJson(pchJson, wErrCode);
	tReqInfo.SetErrID(wErrCode);
	CMauMsg cRspNackMsg;
	cRspNackMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
	SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cRspNackMsg.GetServMsg(), cRspNackMsg.GetServMsgLen());
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s] receive creat conf<%s> nack reply errno<%d>, "
		" nack reply to original request<Event:%s>\n ",
		__FUNCTION__, tReqInfo.GetConfE164(), wErrCode, OspEventDesc(tReqInfo.GetReqEventID()));
}

void CMauVCInstance::DaemonProcRecoverConfNack(const CMessage * pcMsg)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC);

	CHECK_STATE(STATE_NORMAL, return);

	const s8* pchJson = (const s8*)pcMsg->content;
	s8 achCmuIp[MAXLEN_IPADDR + 1] = { 0 };
	GetCmuIpFromJson(pchJson, achCmuIp, MAXLEN_IPADDR);
	s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
	GetConfE164FromJson(pchJson, achConfE164, MAXLEN_CONFE164);
	if (!g_cMauVCApp.IsConfExistAndBelongMcu(achConfE164, achCmuIp))
	{
		return;
	}

	TRedisConfInfo tRedisConf;
	if (!g_cMauVCApp.GetConfExInfo(achConfE164, tRedisConf)
		|| !IsRedisHasConfExData(tRedisConf))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetConfExInfo Fail\n", __FUNCTION__);
		return;
	}
	const u32 nMeetingID = tRedisConf.GetMeetingID();
	g_cMauVCApp.m_cVMeetingDBOpr.CreateMeetingFailed(nMeetingID);
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Conf<E164:%s,meetingID:%d> CreateMeetingFailed.\n",
		__FUNCTION__, tRedisConf.GetConfE164(), nMeetingID);
}

void CMauVCInstance::DaemonProcReleaseConfNtf(const CMessage *  pcMsg)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC);

	CHECK_STATE(STATE_NORMAL, return);

	const s8* pchJson = (const s8*)pcMsg->content;
	s8 achCmuIp[MAXLEN_IPADDR + 1] = { 0 };
	GetCmuIpFromJson(pchJson, achCmuIp, MAXLEN_IPADDR);
	s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
	GetConfE164FromJson(pchJson, achConfE164, MAXLEN_CONFE164);
	if (!g_cMauVCApp.IsConfExistAndBelongMcu(achConfE164, achCmuIp))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[%s]conf<E164:%s> unbelong to mcu: %s, ignore\n",
			__FUNCTION__, achConfE164, achCmuIp);
		return;
	}

	s8 achMOID[OPERATION_MAXLEN_MOID + 1] = { 0 };
	GetMOIDFromJson(pchJson, achMOID, OPERATION_MAXLEN_MOID);
	ReleaseConf(achMOID, achConfE164, achCmuIp);

	//只要有mcu结会消息，就触发会议恢复
	TCmuData tMcu;
	if (g_cMauVCApp.GetMpcData(achCmuIp, tMcu))
	{
		RecoverConfList(1, &tMcu);
	}
}

void CMauVCInstance::AddConfInfo(const s8* pchConfE164, const s8* pchConfName, const s8* pchPwd)
{
	g_cMauVCApp.m_cConfInfoDBOpr.AddConfInfo(pchConfE164, pchConfName, pchPwd);
}

BOOL32 CMauVCInstance::ReleaseConf(const s8*pchMoid, const s8* pchConfE164,
	const s8* pchCmuIp, const BOOL32 bReportCharge/*=TRUE*/)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_VC, "moid: %s, confE164: %s, mcuIp: %s, reportCharge: %s",
		pchMoid, pchConfE164, pchCmuIp, bReportCharge ? "YES" : "NO");

	TRedisConfInfo tConfInfo;
	if (!g_cMauVCApp.GetConfExInfo(pchConfE164, tConfInfo)
		|| !IsRedisHasConfExData(tConfInfo))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Get Conf<%s> fail\n", __FUNCTION__, pchConfE164);
		return FALSE;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]LocalIp<%s> RemoteIp<%s>\n",
		__FUNCTION__, pchCmuIp, tConfInfo.GetCmuIp());

	if (strcmp(pchCmuIp, tConfInfo.GetCmuIp()) == 0)
	{
		string strChargeJson;
		if (bReportCharge
			&& !g_cMauVCApp.ConstructChargeJson(tConfInfo.GetConfE164(), strChargeJson))
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]ConstructChargeJson Fail\n", __FUNCTION__);
			return FALSE;
		}
		if(!g_cMauVCApp.DelConf(tConfInfo.GetDomainMOID(),
				tConfInfo.GetConfE164(), tConfInfo.GetCmuIp()))
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Del conf<%s> failed\n",
				__FUNCTION__, tConfInfo.GetConfE164());
			return FALSE;
		}
		else
		{
			if (bReportCharge)
			{
				ReportCharge(strChargeJson);
			}
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Del redis conf<%s> success\n",
				__FUNCTION__, tConfInfo.GetConfE164());
		}

		g_cMauVCApp.m_cVMeetingDBOpr.ReleaseConf(tConfInfo.GetConfE164(),
			tConfInfo.GetDomainMOID(), tConfInfo.GetMeetingID());
		g_cMauVCApp.m_cConfInfoDBOpr.DelConfInfo(tConfInfo.GetConfE164());
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]release meeting conf<%s> and del confinfodb conf\n",
			__FUNCTION__, tConfInfo.GetConfE164());
	}
	else
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC,
			"[CMauVCInstance::DaemonProcCmuMsg] receive conf<%s> release notify, mpc<%s> confId not exist in mpcd.\n",
			pchConfE164, pchCmuIp);
	}
	return TRUE;
}

/*=============================================================================
函 数 名： DeamonProcHduPoolMsg
功    能： 处理hdu信息上报
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/30  5.0		阮楠楠						创建
=============================================================================*/
void CMauVCInstance::DeamonProcHduPoolMsg( const CMessage * pcMsg )
{
	CHECK_STATE(STATE_NORMAL, return);
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[CMpcdVCInstance::DeamonProcHduPoolMsg]Recv msg.%u<%s>\n", pcMsg->event, OspEventDesc(pcMsg->event) );
	
	CHduMsg cHduMsg( pcMsg->content, pcMsg->length );
	u8 * pbyMsg = cHduMsg.GetMsgBody();
	
	switch( pcMsg->event )
	{
	case HDUPOOL_ACS_HDUSTATUS_NOTIF:
		{
			u8 byHduNum = *(u8*)pbyMsg;
			pbyMsg += sizeof( u8 );
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DeamonProcHduPoolMsg]hdumsg len is %d,size THduEqp %d, hdu num is %u.\n", 
				cHduMsg.GetMsgBodyLen(), sizeof(THduEqp), byHduNum );
			LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "(hdupool->acsssn-->hdussn->*mau){%s}\n", OspEventDesc(pcMsg->event));
			//将hdu信息存入map表
			for ( u8 byHudIdx = 0; byHudIdx < byHduNum; byHudIdx++ )
			{
				THduEqp tHdu = *(THduEqp*)pbyMsg;
				pbyMsg += sizeof( THduEqp );
				CHduInfo cHduInfo;
				cHduInfo.SetNull();
				cHduInfo.SetOnline( tHdu.IsOnline() );
				cHduInfo.SetEqpId( tHdu.GetEqpId() );
				cHduInfo.SetChnNum( tHdu.GetChnNum() );
				cHduInfo.SetBrdType( tHdu.GetBrdType() );
				cHduInfo.SetLayer( tHdu.GetLayer() );
				cHduInfo.SetSlot( tHdu.GetSlot() );
				cHduInfo.SetIp( tHdu.GetIp() );
				cHduInfo.SetVolume( 0, tHdu.GetVolume(0) );
				cHduInfo.SetVolume( 1, tHdu.GetVolume(1) );
				cHduInfo.SetIsMute( 0, tHdu.GetIsMute(0) );
				cHduInfo.SetIsMute( 1, tHdu.GetIsMute(1) );
				cHduInfo.SetChnStatus( 0, tHdu.GetChnlStatus(0) );
				cHduInfo.SetChnStatus( 1, tHdu.GetChnlStatus(1) );
				cHduInfo.SetAlias( tHdu.GetAlias() );
				cHduInfo.SetMoid( (s8*)tHdu.GetMoid(), OPERATION_MAXLEN_APMOID );

				LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "(mau)[AcsId: %u, EqpId: %u, Channel:0->ChannelStatus: %u, Channel:1->ChannelStatus: %u]\n",
															cHduMsg.GetAcsId(), tHdu.GetEqpId(), cHduInfo.GetChnStatus(0), cHduInfo.GetChnStatus(1));
				
				g_cMauVCApp.AddHduInfo( cHduInfo );
			}			
		}
		break;

	case HDUPOOL_ACS_CHANGEHDUVOLUME_NOTIF:
		{
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DeamonProcHduPoolMsg]hdumsg len is %d, size THduVolInfo.%d\n", 
				cHduMsg.GetMsgBodyLen(), sizeof(THduVolInfo) );
			THduVolInfo tHduVolInfo;
			tHduVolInfo = *(THduVolInfo*)pbyMsg;
			CHduInfo cHduInfo;
			if ( g_cMauVCApp.GetHduInfo( tHduVolInfo.GetEqpId(), cHduInfo ) )
			{
				cHduInfo.SetIsMute( tHduVolInfo.GetChnlIdx(), tHduVolInfo.GetIsMute() );
				cHduInfo.SetVolume( tHduVolInfo.GetChnlIdx(), tHduVolInfo.GetVolume() );
				g_cMauVCApp.ModHduInfo( cHduInfo );
				LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC,  "[DeamonProcHduPoolMsg]change hdu.%d chn.%d volume.%d mute.%d.\n",
					cHduInfo.GetEqpId(), tHduVolInfo.GetChnlIdx(),
					cHduInfo.GetVolume( tHduVolInfo.GetChnlIdx()), cHduInfo.GetIsMute(tHduVolInfo.GetChnlIdx()) );
			}

		}
		break;

	case HDUPOOL_MAU_DISCONNECT_NTF:
		{
			//资源池断链，删除map中所有hdu信息
			g_cMauVCApp.DeleteAllHduInfo();	
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DeamonProcHduPoolMsg]DeleteAllHduInfo.\n");
		}
		break;

		//hdu外设上线通知
	case HDUPOOL_ACS_HDUONLINE_NOTIF:
		{
			THduEqp tHdu = *(THduEqp*)pbyMsg;
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DeamonProcHduPoolMsg]hdu.%d is online.\n", tHdu.GetEqpId() );
			CHduInfo cHduInfo;
			cHduInfo.SetNull();
			cHduInfo.SetOnline( tHdu.IsOnline() );
			cHduInfo.SetEqpId( tHdu.GetEqpId() );
			cHduInfo.SetChnNum( tHdu.GetChnNum() );
			cHduInfo.SetBrdType( tHdu.GetBrdType() );
			cHduInfo.SetLayer( tHdu.GetLayer() );
			cHduInfo.SetSlot( tHdu.GetSlot() );
			cHduInfo.SetIp( tHdu.GetIp() );
			cHduInfo.SetVolume( 0, tHdu.GetVolume(0) );
			cHduInfo.SetVolume( 1, tHdu.GetVolume(1) );
			cHduInfo.SetIsMute( 0, tHdu.GetIsMute(0) );
			cHduInfo.SetIsMute( 1, tHdu.GetIsMute(1) );
			cHduInfo.SetChnStatus( 0, tHdu.GetChnlStatus(0) );
			cHduInfo.SetChnStatus( 1, tHdu.GetChnlStatus(1) );
			cHduInfo.SetAlias( tHdu.GetAlias() );
			cHduInfo.SetMoid( (s8*)tHdu.GetMoid(), OPERATION_MAXLEN_APMOID);

			g_cMauVCApp.AddHduInfo( cHduInfo );
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "(mau)[AcsId: %u, EqpId: %u, Channel:0->ChannelStatus: %u, Channel:1->ChannelStatus: %u]\n",
				cHduMsg.GetAcsId(), tHdu.GetEqpId(), cHduInfo.GetChnStatus(0), cHduInfo.GetChnStatus(1));
		}
		break;

	//外设掉线通知
	case HDUPOOL_ACS_HDUOFFLINE_NOTIF:
		{
			//由消息头获取掉线hdu id
			u8 byHduId = cHduMsg.GetEqpId();
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DeamonProcHduPoolMsg]hdu.%d is offline.\n", byHduId );
			CHduInfo cHduInfo;
			cHduInfo.SetNull();
			if (g_cMauVCApp.GetHduInfo(byHduId, cHduInfo))
			{
				cHduInfo.SetOnline(FALSE);
				for(u8 byChannelIndex = 0; byChannelIndex < cHduInfo.GetChnNum(); ++byChannelIndex)
				{
					const u8 byChnStatus = cHduInfo.GetChnStatus(byChannelIndex);
					//HDU通道被占用，下线时设为空闲
					if (HDU_CHNL_OCCUPIED == byChnStatus)
					{
						cHduInfo.SetChnStatus(byChannelIndex, HDU_CHNL_VACANET);
					}
					LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "(mau)[AcsId: %u, EqpId: %u, ChannelIndex: %u, ChannelStatus: %u]\n",
															cHduMsg.GetAcsId(), byHduId, byChannelIndex, cHduInfo.GetChnStatus(byChannelIndex));
				}

				g_cMauVCApp.ModHduInfo(cHduInfo);
			}
			else
			{
				LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "(mau)#EqpId: %d# is not exist.\n", byHduId);
			}
		}
		break;

	//HDU通道状态上报通知
	case HDUPOOL_ACS_HDUCHANNEL_STATUS_NOTIF:
		{
			const u8 byEqpId = cHduMsg.GetEqpId();
			CHduInfo cHduInfo;
			if(g_cMauVCApp.GetHduInfo(byEqpId, cHduInfo))
			{
				const u8 byChannelIndex = cHduMsg.GetChnIndex();
				const u8 byChannelStatus = *((u8*)pbyMsg);
				const u8 byOldChnStatus = cHduInfo.GetChnStatus(byChannelIndex);
				cHduInfo.SetChnStatus(byChannelIndex, byChannelStatus);
				g_cMauVCApp.ModHduInfo(cHduInfo);
				const BOOL32 bIsChannelStatusChange = (byOldChnStatus != byChannelStatus);
				LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "(mau)[AcsId: %u, EqpId: %u, ChannelIndex: %u,oldChnStatus: %u, CurChannelStatus: %u]\n",
														cHduMsg.GetAcsId(), byEqpId, byChannelIndex, byOldChnStatus, byChannelStatus);
				if (bIsChannelStatusChange)
				{
					LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DeamonProcHduPoolMsg][channelStatusChange, send msg to cm]\n");
					if ( !g_cMauVCApp.ProSendHduBelongDoaminOwnHduList2Cm(cHduInfo.GetMoid()) )
					{
						LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DeamonProcHduPoolMsg]post hdu to cpa failed.\n" );
					}
				}
				else
				{
					LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DeamonProcHduPoolMsg][channelStatus UnChange]\n");
					
				}
			}
			else
			{
				LogPrint(LOG_LVL_WARNING, MID_MAU_VC, "(mau)#EqpId: %d# is not exist.\n", byEqpId);
			}
		}
		break;

	default:
		break;
	}

	if (pcMsg->event != HDUPOOL_ACS_HDUCHANNEL_STATUS_NOTIF)
	{
		if ( !g_cMauVCApp.ProSendHduInfo2CmByMoid() )
		{
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[DeamonProcHduPoolMsg]post hdu to cpa failed.\n" );
		}
	}
	
	return ;
}

/*=============================================================================
函 数 名： DeamonProcCmHduMsg
功    能： 会管对电视墙操作
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/30  5.0		阮楠楠						创建
=============================================================================*/
void CMauVCInstance::DeamonProcCmHduMsg( const CMessage * pcMsg )
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	
	//请求头信息
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	//用户信息
	s8 * pchJsonBuf = (s8*)( cServMsg.GetMsgBody()+sizeof(TReqHeadInfo) );
	u32 dwPrintBuffLen = strlen(pchJsonBuf);
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DeamonProcCmHduMsg] cm json len is %d, text is:.\n", dwPrintBuffLen );
	LongLogPrint(LOG_LVL_DETAIL, MID_MAU_VC, pchJsonBuf, dwPrintBuffLen);
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[CMauVCInstance::DeamonProcCmHduMsg]end\n" );

	s8 achConfE164[MAXLEN_CONFE164+1] = {0};

	switch ( pcMsg->event )
	{
	case CM_MAU_GETHDUINFO_CMD:
		{
			if ( !g_cMauVCApp.ProSendAllHduInfo2Cm() )
			{
				LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[CMauVCInstance::DeamonProcCmHduMsg]Don't has any Hdu Info!\n" );
			}
		}
		break;
	case CM_MAU_SETHDUVOLUME_REQ:
		{
			GetConfE164FromJson( pchJsonBuf, achConfE164, MAXLEN_CONFE164 );
			tReqInfo.SetConfE164( achConfE164 );
			
			//用户有效性检验
			CKdvDomainInfo cDomainInfo;
			if( !g_cMauVCApp.IsDomainExist( tReqInfo.GetDomainMOID() ) || !g_cMauVCApp.GetDomainInfo( tReqInfo.GetDomainMOID(),cDomainInfo ) || !cDomainInfo.IsUsedFlag() )
			{
				LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[CMauVCInstance::DeamonProcCmHduMsg] Domain<Name:%s GUID:%s> is not exist or is unused!\n ",
					cDomainInfo.GetWholeName(), cDomainInfo.GetDomainGUID() );
				
				//域信息不存在、获取不到域信息，域已经停用
				tReqInfo.SetErrID(ERR_DOMAIN_INVALID);
				cServMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
				SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
				return ;
			}

			u8 byHduId = 0;
			u8 byHduChnIdx = 0;
			u8 byHduSchedIdx = 0;
			u8 byHduChnIsSilence = 0;
			u8 byHduChnVolume = 0;

			CMauData::GetHduInfoFromJson(pchJsonBuf, byHduId, byHduChnIdx, byHduSchedIdx, byHduChnIsSilence, byHduChnVolume);

			CMauMsg cHduMsg;
			cHduMsg.SetEventId( CM_MAU_SETHDUVOLUME_REQ );
			cHduMsg.SetMsgBody( (u8*)&tReqInfo, sizeof(tReqInfo) );
			cHduMsg.CatMsgBody( (u8*)&byHduId, sizeof(byHduId) );
			cHduMsg.CatMsgBody( (u8*)&byHduChnIdx, sizeof(byHduChnIdx) );
			cHduMsg.CatMsgBody( (u8*)&byHduChnIsSilence, sizeof(byHduChnIsSilence) );
			cHduMsg.CatMsgBody( (u8*)&byHduChnVolume, sizeof(byHduChnVolume) );

			CMauData::SendMsgToHduSsn( cHduMsg.GetEventId(), cHduMsg.GetServMsg(), cHduMsg.GetServMsgLen() );
			
			cServMsg.SetMsgBody( (u8*)&tReqInfo, sizeof(tReqInfo) );
			SendReplyMsg( tReqInfo, tReqInfo.GetReqEventID()+1, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
			
		}
		break;
	case CM_MAU_GETHDUINFOBYMOID_REQ:
		{
			cServMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));

			s8 achMoid[OPERATION_MAXLEN_MOID + 1] = { 0 };
			GetMOIDFromJson(pchJsonBuf, achMoid, OPERATION_MAXLEN_MOID);

			CKdvDomainInfo tKdvDomainInfo;
			if (g_cMauVCApp.GetDomainInfo(achMoid, tKdvDomainInfo))
			{
				cServMsg.CatMsgBody(achMoid, sizeof(achMoid));

				CMauData::CHduInfoMap::iterator itHduLoop = g_cMauVCApp.m_mapHduInfo.begin();
				for (; g_cMauVCApp.m_mapHduInfo.end() != itHduLoop; ++itHduLoop)
				{
					if (g_cMauVCApp.IsHduBelongToMe(achMoid, itHduLoop->second.GetMoid()))
					{
						cServMsg.CatMsgBody(&itHduLoop->second, sizeof(itHduLoop->second));
					}
				}
				SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 1, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
			}
			else
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[CMauVCInstance::DeamonProcCmHduMsg] Domain<Moid:%s> is not exist!\n ", achMoid);

				//域信息不存在、获取不到域信息，域已经停用
				tReqInfo.SetErrID(ERR_DOMAIN_INVALID);
				cServMsg.SetMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
				cServMsg.CatMsgBody(achMoid, sizeof(achMoid));
				SendReplyMsg(tReqInfo, tReqInfo.GetReqEventID() + 2, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
			}

			break;
		}
	default:
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_VC, "[DeamonProcCmHduMsg]Rcv msg is invalid, return!\n" );
		}
		break;
	}

	return;
}

/*=============================================================================
函 数 名： DaemonProcDisconnectNtf
功    能： 处理NU/MCC断链通知
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauVCInstance::DaemonProcDisconnectNtf( const CMessage * pcMsg )
{
	if( NULL == pcMsg->content || 0 == pcMsg->length )
	{
		return ;
	}
}


/*=============================================================================
函 数 名： SendReplyMsg
功    能： 
算法实现： 
全局变量： 
参    数： 
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauVCInstance::SendReplyMsg(CMauMsg& cServMsg, const u32 dwErrCode)
{
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	cServMsg.SetErrorCode((u16)dwErrCode);
	const u32 dwEventID = tReqInfo.GetReqEventID() + (dwErrCode == ERR_DB_SUCCESS ? 1 : 2);
	SendReplyMsg( tReqInfo, dwEventID, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
}

void CMauVCInstance::SendReplyMsg( const TReqHeadInfo & tReqInfo, u32 wEvent, const void* pbyMsg, u16 wLen )
{
	u32 dwDstIID = tReqInfo.GetReqSrcID();
	post( dwDstIID, wEvent, pbyMsg, wLen );
}

/*=============================================================================
函 数 名： DaemonProcPrintCmd
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/11  KDVP5.0     rnn                  创建
2016/04/07  KDVP5.0     王力                增加打印所有个人模板E164、通过E164号打印个人模板
=============================================================================*/
void CMauVCInstance::DaemonProcPrintCmd( const CMessage * pcMsg, CMauVCApp* pcApp )
{
	if (NULL == pcMsg->content || 0 == pcMsg->length)
	{
		return;
	}
	
	u16 wType = *(u16*)pcMsg->content;
	switch( wType )
	{
	case DUMPALLPERSONALTEMPE164:
		{
			pcApp->PrintAllPersonalTempE164();
		}
		break;
	case DUMPALLDOMAININFO:
		{
			pcApp->PrintDomainList();
		}
		break;
	case DUMPALLMTINFO:
		{
			pcApp->PrintMtList();
		}
		break;
	case DUMPALLCONFCACHEINFO:
		{
			pcApp->PrintConfCacheData();
		}
		break;
	case DUMPALLTEMPCACHEINFO:
		{
			pcApp->PrintTempCacheData();
		}
		break;
	case DUMPCMUINFO:
		{
			pcApp->PrintCmuData();
		}
		break;
	case DUMPDBCFGINFO:
			{
				OspPrintf( TRUE, FALSE, "\n" );
				
				OspPrintf( TRUE, FALSE, "--------DB general cfg--------\n");
				OspPrintf( TRUE, FALSE, "DBConnThreadNum: %u\n", pcApp->GetDBTaskNum() );
				OspPrintf( TRUE, FALSE, "DBMsgQueueNum: %u\n", pcApp->GetQueueSize() );
				
				TDBConnectInfo tDBInfo = pcApp->GetTemplateDBConnectInfo();
				OspPrintf( TRUE, FALSE, "--------TemplateInfoDB--------\n" );
				OspPrintf( TRUE, FALSE, "DBEngineType:%u\n", tDBInfo.GetDBEngineType() );
				OspPrintf( TRUE, FALSE, "DBIpAddr:%s\n", tDBInfo.GetDBIp() );
				OspPrintf( TRUE, FALSE, "DBPort:%d\n", tDBInfo.GetDBPort() );
				OspPrintf( TRUE, FALSE, "DBName:%s\n", tDBInfo.GetDBName() );
				
				tDBInfo = pcApp->GetConfInfoDBConnectInfo();
				OspPrintf( TRUE, FALSE, "--------ConfInfoDB--------\n" );
				OspPrintf( TRUE, FALSE, "DBEngineType:%u\n", tDBInfo.GetDBEngineType() );
				OspPrintf( TRUE, FALSE, "DBIpAddr:%s\n", tDBInfo.GetDBIp() );
				OspPrintf( TRUE, FALSE, "DBPort:%d\n", tDBInfo.GetDBPort() );
				OspPrintf( TRUE, FALSE, "DBName:%s\n", tDBInfo.GetDBName() );

				tDBInfo = pcApp->GetVMeetingDBConnectInfo();
				OspPrintf( TRUE, FALSE, "--------MeetingDB--------\n" );
				OspPrintf( TRUE, FALSE, "DBEngineType:%u\n", tDBInfo.GetDBEngineType() );
				OspPrintf( TRUE, FALSE, "DBIpAddr:%s\n", tDBInfo.GetDBIp() );
				OspPrintf( TRUE, FALSE, "DBPort:%d\n", tDBInfo.GetDBPort() );
				OspPrintf( TRUE, FALSE, "DBName:%s\n", tDBInfo.GetDBName() );

				tDBInfo = pcApp->GetApDBConnectInfo();
				OspPrintf( TRUE, FALSE, "--------ApDB--------\n" );
				OspPrintf( TRUE, FALSE, "DBEngineType:%u\n", tDBInfo.GetDBEngineType() );
				OspPrintf( TRUE, FALSE, "DBIpAddr:%s\n", tDBInfo.GetDBIp() );
				OspPrintf( TRUE, FALSE, "DBPort:%d\n", tDBInfo.GetDBPort() );
				OspPrintf( TRUE, FALSE, "DBName:%s\n", tDBInfo.GetDBName() );

				OspPrintf( TRUE, FALSE, "--------MqInfo--------\n" );
				OspPrintf( TRUE, FALSE, "MqIpAddr:%s\n", pcApp->m_achMqIP);
				OspPrintf( TRUE, FALSE, "MqPort:%u\n", pcApp->m_dwMqPort);
				OspPrintf( TRUE, FALSE, "TopLevelMqIpAddr:%s\n", pcApp->m_achTopLevelMqIP);

				OspPrintf( TRUE, FALSE, "--------UpuInfo--------\n" );
				OspPrintf( TRUE, FALSE, "UpuIpAddr:%s\n", pcApp->m_achUpuIP);
				OspPrintf(TRUE, FALSE, "UpuState:%s\n", g_cUpuSsnApp.m_cUpuClient.IsConnected() ? "Connected" : "Disconnected");
				
				OspPrintf( TRUE, FALSE, "--------ZkInfo--------\n" );
				OspPrintf( TRUE, FALSE, "ZkIpAddr:%s\n", pcApp->m_achZkIP );
				OspPrintf( TRUE, FALSE, "ZkPort:%d\n", pcApp->m_wZkPort );
				OspPrintf( TRUE, FALSE, "State: %s\n", g_cResMgrSsnApp.GetZkState() );

				OspPrintf(TRUE, FALSE, "---------Redis---------\n");
				OspPrintf(TRUE, FALSE, "IP: %s\n", pcApp->m_achRedisIP);
				OspPrintf(TRUE, FALSE, "Port: %d\n", pcApp->m_wRedisPort);

				OspPrintf( TRUE, FALSE, "--------CmuRoutingKey--------\n" );
				CApp *pcCmuApp = &g_cCMUSsnApp;
				
				u16 wLoop = 0;
		        for( wLoop = 1; wLoop <= MAX_CMU_NUM; wLoop++ )
				{
					CCMUSsnInstance * pcInst = (CCMUSsnInstance*)pcCmuApp->GetInstance( wLoop );
					if ( NULL == pcInst || CCMUSsnInstance::STATE_IDLE == pcInst->CurState() )
					{
						continue;
					}

					OspPrintf( TRUE, FALSE, "CmuInstId<%d> RoutingKey<%s> Ip<%s> CmuType<%s>\n", 
						wLoop, pcInst->m_tCmuInfo.achRoutingKey, pcInst->m_tCmuInfo.achMcuIP, pcInst->m_tCmuInfo.achMcuType);
				}
			}
		break;
	case DUMPDBSTATUS:
		{
			pcApp->PrintDBStatus();
		}
		break;
	case DUMPALLHDUINFO:
		{
			pcApp->PrintHduList();
		}
		break;
	case DUMPAPDBHDUINFO:
		{
			pcApp->PrintApDBHduInfo();
		}
		break;
	case DUMPALLMONITORINFO:
		{
			PrintMonitorInfo();
		}
		break;
	default:
		break;
	}
}


/*=============================================================================
函 数 名： DaemonProcApFinishOprNtf
功    能： 处理完成ap数据库操作通知
算法实现： 
全局变量： 
参    数： 
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/11  KDVP5.0     rnn                  创建
=============================================================================*/
void CMauVCInstance::DaemonProcApFinishOprNtf(const CMessage * pcMsg, CMauVCApp* pcApp)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	TApDBFinishOpr tOpr = *(TApDBFinishOpr*)cServMsg.GetMsgBody();
	
	switch (tOpr.GetOprType())
	{
	case CAPInfoDBOpr::GETSERVDOMAININFOOPR:						//获取域信息
		DaemonProcGetDomainInfoFromApFinishNtf( pcMsg, pcApp );
		break;
		
    case CAPInfoDBOpr::GETPLATFORMDOMAININFOOPR:					//获取平台域信息
		DaemonProcGetPlatformDomainInfoFromApFinishNtf( pcMsg, pcApp );
		break;
		
	case CAPInfoDBOpr::GETUSERDOMAININFOOPR:						//获取用户域信息
		DaemonProcGetUserDomainInfoFromApFinishNtf( pcMsg, pcApp );
		break;

	case CAPInfoDBOpr::GETDOMAINHASXMPU:							//获取域是否部署了xmpu信息
		DaemonProcGetDomainHasXmpuFromApFinishNtf( pcMsg, pcApp );
		break;
		
	case CAPInfoDBOpr::GETMTINFOOPR:								//获取终端信息
		DaemonProcGetAllMtInfoFromApFinishNtf( pcMsg, pcApp );
		break;	
	case CAPInfoDBOpr::GETHDUMOIDANDUSERDOMAINMOID:					//获取HduMoid,以及对应的User_domain_moid
		DaemonProcGetHduMoidAndDomainMoidApFinishNtf( pcMsg,pcApp );
		break;
	case CAPInfoDBOpr::GETPLATFORMISRENT:
		DaemonProcGetPlatformIsRentNtf(pcMsg, pcApp);
		break;
	default:
		LogPrint( LOG_LVL_WARNING, MID_MAU_VC, "[DaemonProcApFinishOprNtf] Unknow Opr Type.\n");
		break;
	}
	
	return;
}
	
/*=============================================================================
函 数 名： DaemonProcGetDomainInfoFromApFinishNtf
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/11  KDVP5.0     rnn                  创建
=============================================================================*/
void CMauVCInstance::DaemonProcGetDomainInfoFromApFinishNtf( const CMessage * pcMsg, CMauVCApp* pcApp)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	
	TApDBFinishOpr tOpr = *(TApDBFinishOpr *)pbyMsg;
	
	//加载数据库数据失败
	if( 0 != tOpr.GetOprRet() )
	{		
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcGetDomainInfoFromApFinishNtf] get domaininfo from ap failed, get again.\n" );
		
		//再次获取域信息
		g_cMauVCApp.m_cApInfoDBOpr.GetServDomainInfo();	
	}
	else
	{
		//获取xmpu信息
		//g_cMauVCApp.m_cApInfoDBOpr.GetDomainHasXmpu();
		++g_cMauVCApp.m_byInitCount;
		StartServer(pcApp);
	}
}

/*=============================================================================
函 数 名： DaemonProcGetPlatformDomainInfoFromApFinishNtf
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/11  KDVP5.0     rnn                  创建
=============================================================================*/
void CMauVCInstance::DaemonProcGetPlatformDomainInfoFromApFinishNtf( const CMessage * pcMsg, CMauVCApp* pcApp)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	
	TApDBFinishOpr tOpr = *(TApDBFinishOpr *)pbyMsg;

	//加载数据库数据失败
	if( 0 != tOpr.GetOprRet() )
	{		
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcGetPlatformDomainInfoFromApFinishNtf] get domaininfo from ap failed, get again.\n" );
		
		//再次获取平台域信息	
		g_cMauVCApp.m_cApInfoDBOpr.GetPlatFormDomainInfo();	
		return;
	}
	++g_cMauVCApp.m_byInitCount;
	StartServer(pcApp);
}

/*=============================================================================
函 数 名： DaemonProcGetUserDomainInfoFromApFinishNtf
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/11  KDVP5.0     rnn                  创建
=============================================================================*/
void CMauVCInstance::DaemonProcGetUserDomainInfoFromApFinishNtf( const CMessage * pcMsg, CMauVCApp* pcApp)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	
	TApDBFinishOpr tOpr = *(TApDBFinishOpr *)pbyMsg;
	
	//加载数据库数据失败
	if( 0 != tOpr.GetOprRet() )
	{		
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcGetUserDomainInfoFromApFinishNtf] get domaininfo from ap failed, get again.\n" );
		
		SetTimer(MAU_LOADUSERDOMAININFO_TIMER, 1000);
		return;
	}
	
	if (g_cMauVCApp.m_byInitCount < s_InitDealfun_vec.size())
	{
		if (UpuAddAllBookConf())
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]upu add all bookconf success\n", __FUNCTION__);
		}
		else
		{
			SetTimer(MAU_INIT_UPUADDBOOKCONF_TIMER, INIT_UPUADDBOOKCONF_SPAN);
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]upu addall bookconf fail, settimer add again\n", __FUNCTION__);
		}
		++g_cMauVCApp.m_byInitCount;
		StartServer(pcApp);
	}
}

/*=============================================================================
函 数 名： DaemonProcGetDomainHasXmpuFromApFinishNtf
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/03/28  KDVP5.0     rnn                  创建
=============================================================================*/
void CMauVCInstance::DaemonProcGetDomainHasXmpuFromApFinishNtf( const CMessage * pcMsg, CMauVCApp* pcApp)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	
	TApDBFinishOpr tOpr = *(TApDBFinishOpr *)pbyMsg;

	//加载数据库数据失败
	if( 0 != tOpr.GetOprRet() )
	{		
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcGetDomainHasXmpuFromApFinishNtf] get domaininfo from ap failed, get again.\n" );
		
		//再次获取xmpu信息
		g_cMauVCApp.m_cApInfoDBOpr.GetDomainHasXmpu();
		return;
	}
	++g_cMauVCApp.m_byInitCount;
	StartServer(pcApp);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcGetDomainHasXmpuFromApFinishNtf] get domainhasxmpu from ap sucess.\n");
}

/*=============================================================================
函 数 名： DaemonProcGetAllMtInfoFromApFinishNtf
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/11  KDVP5.0     rnn                  创建
=============================================================================*/
void CMauVCInstance::DaemonProcGetAllMtInfoFromApFinishNtf( const CMessage * pcMsg, CMauVCApp* pcApp)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	
	TApDBFinishOpr tOpr = *(TApDBFinishOpr *)pbyMsg;
	
	//加载数据库数据失败
	if( 0 != tOpr.GetOprRet() )
	{		
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcGetAllMtInfoFromApFinishNtf] get domaininfo from ap failed, get again.\n" );
		
		//再次获取终端信息	
		g_cMauVCApp.m_cApInfoDBOpr.GetMtInfo();	
		return;
	}
	++g_cMauVCApp.m_byInitCount;
	StartServer(pcApp);
}

void CMauVCInstance::DaemonProcGetPlatformIsRentNtf(const CMessage *   pcMsg, CMauVCApp* pcApp)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	TApDBFinishOpr tOpr = *(TApDBFinishOpr *)pbyMsg;
	if (tOpr.GetOprRet() != ERR_DB_SUCCESS)
	{
		g_cMauVCApp.m_cApInfoDBOpr.GetPlatformIsRent();
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetPlatfromIsRent Fail, try again\n", __FUNCTION__);
		return;
	}
	++g_cMauVCApp.m_byInitCount;
	LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetPlatfromIsRent success\n", __FUNCTION__);
	StartServer(pcApp);
}

void CMauVCInstance::DaemonProcMoveScheduledConfToRedisNtf(const CMessage *   pcMsg, CMauVCApp* pcApp)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	TApDBFinishOpr tOpr = *(TApDBFinishOpr *)pbyMsg;
	if (tOpr.GetOprRet() != ERR_DB_SUCCESS)
	{
		g_cMauVCApp.m_cScheduledDBOpr.GetAllScheduledConfInfoThenClear();
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Move ScheduledConf To Redis Fail, try again\n", __FUNCTION__);
		return;
	}
	++g_cMauVCApp.m_byInitCount;
	LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Move ScheduledConf To Redis success\n", __FUNCTION__);
	StartServer(pcApp);
}

void CMauVCInstance::DaemonProcGetHduMoidAndDomainMoidApFinishNtf(const CMessage *   pcMsg, CMauVCApp* pcApp)
{
	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();

	TApDBFinishOpr tOpr = *(TApDBFinishOpr *)pbyMsg;

	//加载数据库数据失败
	if (0 != tOpr.GetOprRet())
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcGetHduMoidAndDomainMoidApFinishNtf] get hdumoid,userdomainmoid from ap failed, get again.\n");

		//再次获取xmpu信息
		g_cMauVCApp.m_cApInfoDBOpr.GetHduMoidAndUserDomainMoid();
		return;
	}
	++g_cMauVCApp.m_byInitCount;
	StartServer(pcApp);
}

//检测会议不仅是检测会议是否到时，还需要对各种状态进行处理
void CMauVCInstance::DaemonProcCheckConfTimer( const CMessage* pcMsg )
{
	CPrintFunInOut cPrintFun(__FUNCTION__, MID_MAU_VC);
	KillTimer(MAU_CHECKCONF_TIMER);
	SetTimer(MAU_CHECKCONF_TIMER, CHECKCONF_TIME_SPAN);

	CleanExceptionBookConfs();

	std::list<TRedisConfInfo> lstRedisInfo;
	g_cMauVCApp.GetAllConfExInfo(lstRedisInfo);
	std::list<TRedisConfInfo>::iterator it = lstRedisInfo.begin();
	for (; it != lstRedisInfo.end(); ++it)
	{
		time_t starttime;
		FormatStr2KdvTime(it->GetRealStartTime()).GetTime(starttime);
		
		const time_t endtime = starttime +  it->GetDuration() * 60;
		const time_t curtime = time(NULL);
		const u16 wLeftTime = (u16)((endtime - curtime) / 60);
		
		//若永久会议是prepare状态, 没有基础数据则删除
		//有基础数据，并且创会或者恢复超时，则设为恢复状态
		if (it->GetState() == enumPrepareState)
		{
			//创会消息丢失或mcu已重启但zk未通知时间段内的创会导致的创会状态的会议需要清除
			BOOL32 bBaseConfDataExist = FALSE;
			const BOOL32 bSuccess = g_cMauVCApp.IsConfDataExist(it->GetConfE164(), bBaseConfDataExist);
			if ( bSuccess && !bBaseConfDataExist && IsCreateConfTimeOut(starttime))
			{
				ReleaseConf(it->GetDomainMOID(), it->GetConfE164(), it->GetCmuIp(), FALSE);
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]conf<%s> base data unexist"
					" and state is prepare state and conf real StartTime<%u> is less than"
					" curtime<%u> 30s, release conf\n", __FUNCTION__,
					it->GetConfE164(), starttime, curtime);
				continue;
			}
			else if (!bSuccess)
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]IsConfDataExist<%s> fail\n", __FUNCTION__, it->GetConfE164());
				continue;
			}
			else
			{
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Conf<%s,%s> state is prepare,"
					"in createconf or restoreconf curtime<%u>, starttime<%u>\n",
					__FUNCTION__, it->GetDomainMOID(), it->GetConfE164(), curtime, starttime);
			}
		}
		else if (it->GetState() == enumRunningState)
		{
			//若会议是正召开，但没有MCU缓存，需更改状态为待恢复
			if (!g_cMauVCApp.IsMpcConnect(it->GetCmuIp()))
			{
				const BOOL32 bChangeStateSuccess = g_cMauVCApp.ChangeConfState(it->GetDomainMOID(),
					it->GetConfE164(), enumConnectWaitingState);
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Conf<%s> belong mcu<%s>, but disconnect"
					" change conf state to wait\n", __FUNCTION__, it->GetConfE164(), it->GetCmuIp());
			}
		}

		//永久会议无需检测是否到时
		if (0 == it->GetDuration())
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcCheckConfTimer] ConfE164: %s, Duration is 0, Don't need to test\n", it->GetConfE164());
			continue;
		}

		if (it->GetState() == enumRunningState || it->GetState() == enumPrepareState)
		{
			if (0 == wLeftTime)
			{
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcCheckConfTimer]"
					"Check conf E164: %s left time: %d, release conf !\n", 
					it->GetConfE164(), wLeftTime);

				//发送给cmu结束会议
				SendReleaseConfToCmu(it->GetConfE164(), it->GetConfName(), it->GetCmuIp(), FALSE);
				continue;
			}

			if (wLeftTime == 15 || wLeftTime <= 10)
			{
				json_t_ptr pjRoot = json_object();
				SetMsgTypeToJsonObj(pjRoot, EV_CMU_CM_CONFTIMELEFT_NTF);
				SetConfE164ToJsonObj(pjRoot, it->GetConfE164());
				SetConfTimeLeftToJsonObj(pjRoot, wLeftTime);
				SetDomainMoidToJsonObj(pjRoot, it->GetDomainMOID());
				SetConfNameToJsonObj(pjRoot, it->GetConfName());
				SetMeetingIDToJsonObj(pjRoot, it->GetMeetingID());

				json_str strJsonToCM(pjRoot);
				u32 dwBuffLen = strlen(strJsonToCM);

				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcCheckConfTimer]"
					"Check conf E164: %s left time: %d \n", it->GetConfE164(), wLeftTime);
				//发送会议剩余时间到CM
				CMauMsg cTempMsg;
				cTempMsg.SetMsgBody(strJsonToCM, dwBuffLen);
				CMauData::SendMsgToCmCmuSsn(CMU_CM_CONFOPR_NTF,  cTempMsg.GetServMsg(), cTempMsg.GetServMsgLen());
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcCheckConfTimer] Send Msg CMU_CM_CONFTIMELEFT_NTF to CM\n");

				//发送会议剩余时间通知到cmu
				SetMsgTypeToJsonObj(pjRoot, EV_MAU_CMU_CONFTIMELEFT_NTF);
				json_str strJsonToCmu(pjRoot);
				dwBuffLen = strlen(strJsonToCmu);
				CMauData::SendMsgToCmuSsn(it->GetCmuIp(), CM_CMU_CONFOPR_NTF, strJsonToCmu, dwBuffLen);
			}
		}
		else if (it->GetState() == enumConnectWaitingState)
		{
			if (curtime >= endtime)
			{
				ReleaseConf(it->GetDomainMOID(), it->GetConfE164(), it->GetCmuIp());
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]conf<%s> is waiting state, and curtime<%u>"
					" >= endtime<%u>, ReleaseConf\n", __FUNCTION__, it->GetConfE164(), curtime, endtime);
			}
		}
	}
}

BOOL32 CMauVCInstance::CleanExceptionBookConfs()
{
	vector<CRedisBookConf> vecBookConfs;
	if (!g_cMauVCApp.GetAllBookConf(vecBookConfs))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetAllBookConf Fail\n", __FUNCTION__);
		return FALSE;
	}
	vector<CRedisBookConf>::iterator iterBookConf = vecBookConfs.begin();
	for (; iterBookConf != vecBookConfs.end(); ++iterBookConf)
	{
		if (IsExceptionBookConf(iterBookConf->GetBookStartTime()))
		{
			g_cMauVCApp.DelBookConf(iterBookConf->GetConfE164());
			UpuDelConf(iterBookConf->GetConfE164(), TUpuConfInfo::emScheConf);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]bookConf<%s:%s> exception, del\n",
				__FUNCTION__, iterBookConf->GetConfE164(), iterBookConf->GetBookStartTime());
		}
	}
	return TRUE;
}

/*=============================================================================
函 数 名： DaemonProcPersTmpAckTimer
功    能：
算法实现：
全局变量：
参    数： const CMessage *pcMsg
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2016/04/08  KDVP5.0     王力                  创建
=============================================================================*/
void CMauVCInstance::DaemonProcGetPersonalTempTimer(const CMessage *pcMsg)
{
	KillTimer(MAU_GETPERSONALTEMPFROMCM_TIMER);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcGetPersonalTempTimer] All Personal Conf Template Req\n");
	time_t aTime = time(NULL);
	s8 achReqId[20] = { 0 };
	sprintf(achReqId, "%u", aTime);

	m_strReqId = achReqId;

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	sprintf(achJsonBuf, "{\"type\":\"MAU_CM_ALLPERSONALCONFTEMPLATE_REQ\",\"reqid\":\"%s\"}", achReqId);
	OspPost(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON), MAU_CM_ALLPERSONALCONFTEMPLATE_REQ, achJsonBuf, strlen(achJsonBuf));
	SetTimer(MAU_GETPERSONALTEMPFROMCM_TIMER, PERSONALTMPACK_TIME_SPAN);
}

//---------------会议恢复begin------------------------------------
void CMauVCInstance::DaemonProcCheckRedisData(const CMessage* pcMsg)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__);
	KillTimer(MAU_CHECK_REDISDATA_TIMER);
	SetTimer(MAU_CHECK_REDISDATA_TIMER, CHECK_REDISDATA_SPAN);

	string checkResStr;
	if (g_cMauVCApp.CheckRedisData(checkResStr))
	{
		LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, 
			checkResStr.c_str(), checkResStr.length(), FALSE);
	}
	else
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[%s]Check redis data err\n", __FUNCTION__);
	}
}

void CMauVCInstance::DaemonProcRecoverConfTimer(const CMessage* pcMsg)
{
	CPrintFunInOut cPrintFun(__FUNCTION__, MID_MAU_VC);
	KillTimer(MAU_RECORVERCONF_TIMER);
	SetTimer(MAU_RECORVERCONF_TIMER, g_cMauVCApp.GetRecoverTimeSpan() * 1000);
	RecoverConfList();
}

BOOL32 CMauVCInstance::RecoverMeeting(const TCmuData& tCmu, const TRedisConfInfo& tConfInfo)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC);

	//检测该会议是否存在监控,有的话，需要通知会馆停监控
	//因为监控是不恢复的，而会馆不感知mcu是否断链丢失监控
	if (!NoticeCMStopConfMonitors(tConfInfo.GetConfE164()))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]NoticeCMStopConfMonitors<%s> Fail\n",
			__FUNCTION__, tConfInfo.GetConfE164());
		return FALSE;
	}

	CKdvDomainInfo cDomainInfo;
	if (!g_cMauVCApp.GetDomainInfo(tConfInfo.GetDomainMOID(), cDomainInfo))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetDoaminInfo<%s> Fail\n", __FUNCTION__,
			tConfInfo.GetDomainMOID());
		return FALSE;
	}

	TRedisConfInfo tRedisConfInfo(tConfInfo);
	tRedisConfInfo.SetState(enumPrepareState);
	tRedisConfInfo.SetCmuIp(tCmu.GetCmuIp());
	tRedisConfInfo.SetCmuPid(tCmu.GetCmuPid());
	time_t tCurTime = time(NULL);
	tRedisConfInfo.SetStartTime(tCurTime);
	if (!g_cMauVCApp.UpdateConfData(tRedisConfInfo))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]updateconfdata<%s> fail\n",
			__FUNCTION__, tRedisConfInfo.GetConfE164());
		return FALSE;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Set conf owner<%s:%u>\n",
		__FUNCTION__, tRedisConfInfo.GetCmuIp(), tRedisConfInfo.GetCmuPid());

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;
	if (!ConstructGetRecoverConfNeedInfoToJson(tRedisConfInfo.GetConfE164(), cDomainInfo.HasXmpuServer(),
		achJsonBuf, dwJsonBufLen))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]ConstructGetRecoverConfNeedInfoJson fail\n");
		return FALSE;
	}
	CMauData::SendMsgToCmuSsn(tCmu.GetCmuIp(), MAU_MCU_RESTORECONF_REQ, achJsonBuf, dwJsonBufLen);
	return TRUE;
}

BOOL32 CMauVCInstance::RecoverConf(const TRedisConfInfo& tRebuildConf,
	const TCmuData* ptCmu/*=NULL*/)
{
	//利旧会议在mcu断链之后，保持waiting状态，除非重连之后更新为运行状态
	//否则将直到会议结束才清除会议状态(利旧会议不恢复）
	if (tRebuildConf.GetState() == enumRunningState
		|| (tRebuildConf.GetState() == enumPrepareState && !IsCreateConfTimeOut(tRebuildConf.GetStartTime()))
		|| strlen(tRebuildConf.GetConfE164()) == 0 
		|| !tRebuildConf.NeedLicense())
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]conf<%s,%d,%s>can't recover\n",
			__FUNCTION__, tRebuildConf.GetConfE164(), tRebuildConf.GetState(), tRebuildConf.GetStartTime());
		return FALSE;
	}

	TCmuData cBestFitMcu;
	if (ptCmu != NULL)
	{
		cBestFitMcu = *ptCmu;
	}
	else if (!g_cMauVCApp.AssignMpcID("", cBestFitMcu))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]assign mcu fail\n", __FUNCTION__);
		return FALSE;
	}
	if (!RecoverMeeting(cBestFitMcu, tRebuildConf))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]RecoverMeeting fail\n", __FUNCTION__);
		return FALSE;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]use mcuIp: %s to recorver conf<E164:%s>\n", 
		__FUNCTION__, cBestFitMcu.GetCmuIp(), tRebuildConf.GetConfE164());
	return TRUE;
}

//confnum默认为0：检测并恢复所有会议，若ptCmu不为NULL,则通过该指定的mcu进行会议恢复
void CMauVCInstance::RecoverConfList(u32 dwConfNum/*=0*/, const TCmuData* ptCmu/*=NULL*/)
{
	list<TRedisConfInfo> reBuildConfList;
	g_cMauVCApp.GetAllConfExInfo(reBuildConfList);
	if (dwConfNum == 0)
	{
		dwConfNum = reBuildConfList.size();
	}

	u32 dwRecorverConfNum = 0;
	for (list<TRedisConfInfo>::const_iterator confIter = reBuildConfList.begin();
	confIter != reBuildConfList.end(); ++confIter)
	{
		if (dwRecorverConfNum < dwConfNum)
		{
			if ( RecoverConf(*confIter, ptCmu) )
			{
				++dwRecorverConfNum;
			}
			else
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]RecoverConf:%s fail or don't need to recover\n", __FUNCTION__, confIter->GetConfE164());
			}
		}
	}
}

void CMauVCInstance::RecoverConfList(const list<TRedisConfInfo>& confList,
	const TCmuData* ptCmu/*=NULL*/)
{
	for (list<TRedisConfInfo>::const_iterator confIter = confList.begin();
	confIter != confList.end(); ++confIter)
	{
		RecoverConf(*confIter, ptCmu);
	}
}

BOOL32 CMauVCInstance::AccordRedisConfAndCmuConfList(const TMpcConfData* tMcuConfList,
	const u32 dwConfNum, const s8* pchMcuIp, const u32 dwMcuPid)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC, 
		"confNum: %d, mcuIp: %s, mcuPid: %u", dwConfNum, pchMcuIp, dwMcuPid);
	for (u32 dwConfIndex = 0; dwConfIndex != dwConfNum; ++dwConfIndex)
	{
		const TMpcConfData& tMcuConf = tMcuConfList[dwConfIndex];
		if (!AccordRedisConfAndCmuConf(tMcuConf, pchMcuIp, dwMcuPid))
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]AccordRedisConfAndCmuConf Fail\n", __FUNCTION__);
			return FALSE;
		}
	}
	return TRUE;
}

static s8 TranslateConfState(const u8 byState)
{
	s8 chRecoverState = -1;
	if (enumRunningState == byState)
	{
		chRecoverState = 0;
	}
	else if (enumPrepareState == byState)
	{
		chRecoverState = 1;
	}
	else if (enumConnectWaitingState == byState)
	{
		chRecoverState = 2;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]ConfState: %s, TranslatedState: %d\n",
		__FUNCTION__, ConfStateToString(byState), chRecoverState);
	return chRecoverState;
}

static u8 GetConfCondition(const s8* pchMcuConfTime, const s8* pchRedisConfTime,
	const s8* pchMcuIp, const s8* pchRedisMcuIp)
{
	bitset<2> confConditionBitset;
	time_t tCmuConfTime = 0;
	FormatStr2KdvTime(pchMcuConfTime).GetTime(tCmuConfTime);
	time_t tRedisConfTime = 0;
	FormatStr2KdvTime(pchRedisConfTime).GetTime(tRedisConfTime);
	confConditionBitset[0] = (strncmp(pchMcuIp, pchRedisMcuIp, MAXLEN_IPADDR) != 0);
	confConditionBitset[1] = (tCmuConfTime > tRedisConfTime);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]mcuConfTime:[%s],redisConfTime:[%s]. "
		"mcuIp: %s, redisIp: %s, ConfCondition: %u\n",
		__FUNCTION__, pchMcuConfTime, pchRedisConfTime, pchMcuIp, pchRedisMcuIp, 
		confConditionBitset.to_ulong());
	return (u8)confConditionBitset.to_ulong();
}

BOOL32 CMauVCInstance::LogAccordConditon(const TMpcConfData& tMcuConf, 
	const s8* pchMcuIp, const u32 dwMcuPid)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC);
	s8 achConfStartTime[100 + 1] = { 0 };
	tMcuConf.GetStartTime().GetString(achConfStartTime, sizeof(achConfStartTime));
	TRedisConfInfo tConfInfo;
	if (g_cMauVCApp.GetConfExInfo(tMcuConf.GetConfIdAttr().GetConfE164(), tConfInfo)
		&& IsRedisHasConfExData(tConfInfo))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]ConfE164: %s, mcuIp: %s, redisMcuIp: %s, "
			"mcuConfStartTime: %s, redisConfStartTime: %s\n", __FUNCTION__,
			tMcuConf.GetConfIdAttr().GetConfE164(), pchMcuIp, tConfInfo.GetCmuIp(),
			achConfStartTime, tConfInfo.GetStartTime());
	}
	else
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]ConfE164: %s, but redis hasn't this conf,"
			" mcuIp: %s, mcuConfStartTime: %s, \n", __FUNCTION__,
			tMcuConf.GetConfIdAttr().GetConfE164(), pchMcuIp, achConfStartTime);
	}
	return TRUE;
}

BOOL32 CMauVCInstance::ReleaseMcuConf(const TMpcConfData& tMcuConf,
	const s8* pchMcuIp, const u32 dwMcuPid)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC);
	SendReleaseConfToCmu(tMcuConf.GetConfIdAttr().GetConfE164(),
		tMcuConf.GetConfIdAttr().GetConfName(), pchMcuIp, FALSE);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]SendReleaseConf<%s,%s> to cmu<%s>\n",
		__FUNCTION__, tMcuConf.GetConfIdAttr().GetConfE164(),
		tMcuConf.GetConfIdAttr().GetConfName(), pchMcuIp);
	return TRUE;
}

BOOL32 CMauVCInstance::UpdateRedisConf(const TMpcConfData& tMcuConf,
	const s8* pchMcuIp, const u32 dwMcuPid)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC);
	TRedisConfInfo tRedisConfInfo;
	if (g_cMauVCApp.GetConfExInfo(tMcuConf.GetConfIdAttr().GetConfE164(), tRedisConfInfo)
		&& IsRedisHasConfExData(tRedisConfInfo))
	{
		tRedisConfInfo.SetState(enumRunningState);
		tRedisConfInfo.SetCmuIp(pchMcuIp);
		tRedisConfInfo.SetCmuPid(dwMcuPid);
		tRedisConfInfo.SetConfName(tMcuConf.GetConfIdAttr().GetConfName());
		time_t tConfStartTime = 0;
		tMcuConf.GetStartTime().GetTime(tConfStartTime);
		tRedisConfInfo.SetStartTime(tConfStartTime);
		if (g_cMauVCApp.UpdateConfData(tRedisConfInfo))
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]UpdateConf<%s:%s:%u> success\n",
				__FUNCTION__ ,tRedisConfInfo.GetConfE164(), tRedisConfInfo.GetCmuIp(), tRedisConfInfo.GetCmuPid());
			return TRUE;
		}
		else
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]UpdateConf<e164:%s> err\n",
				__FUNCTION__ ,tRedisConfInfo.GetConfE164());
			return FALSE;
		}

	}
	else
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetConfExInfoErr, E164: %s\n",
			tMcuConf.GetConfIdAttr().GetConfE164());
		return FALSE;
	}
}

BOOL32 CMauVCInstance::NoticeMcuFlushRedisData(const TMpcConfData& tMcuConf,
	const s8* pchMcuIp, const u32 dwMcuPid)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC);
	SendUpdateAllConfDataToCmu(pchMcuIp, tMcuConf.GetConfIdAttr().GetConfE164(),
		tMcuConf.GetConfIdAttr().GetConfName(), tMcuConf.GetConfIdAttr().GetDomainMOID());
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Notice mcu<%s:%u> flush conf<%s:%s:%s> data to redis\n",
		__FUNCTION__, pchMcuIp, dwMcuPid, tMcuConf.GetConfIdAttr().GetConfE164(),
		tMcuConf.GetConfIdAttr().GetConfName(), tMcuConf.GetConfIdAttr().GetDomainMOID());
	return TRUE;
}

BOOL32 CMauVCInstance::UpdateRedisConfAndNoticeMcuFlushDataAndStopMonitor(const TMpcConfData& tMcuConf,
	const s8* pchMcuIp, const u32 dwMcuPid)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC);
	//发生会议切换时，需停止会馆监控
	if (!NoticeCMStopConfMonitors(tMcuConf.GetConfIdAttr().GetConfE164()))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]NoticeCMStopConfMonitors Fail\n", __FUNCTION__);
		return FALSE;
	}

	if (!UpdateRedisConf(tMcuConf, pchMcuIp, dwMcuPid))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]UpdateRedisConf Fail\n", __FUNCTION__);
		return FALSE;
	}
	NoticeMcuFlushRedisData(tMcuConf, pchMcuIp, dwMcuPid);
	return TRUE;
}

BOOL32 CMauVCInstance::SwitchConf(const TMpcConfData& tMcuConf, const s8* pchMcuIp, const u32 dwMcuPid)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC);
	TRedisConfInfo tRedisConf;
	if (g_cMauVCApp.GetConfExInfo(tMcuConf.GetConfIdAttr().GetConfE164(), tRedisConf)
		&& IsRedisHasConfExData(tRedisConf))
	{
		if (!UpdateRedisConfAndNoticeMcuFlushDataAndStopMonitor(tMcuConf, pchMcuIp, dwMcuPid))
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]UpdateRedisConfAndNoticeMcuFlushData Fail\n", __FUNCTION__);
			return FALSE;
		}
		SendReleaseConfToCmu(tRedisConf.GetConfE164(), tRedisConf.GetConfName(), tRedisConf.GetCmuIp(), FALSE);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Send ReleaseConf<%s,%s> to cmu<%s>\n",
			__FUNCTION__, tRedisConf.GetConfE164(), tRedisConf.GetConfName(), tRedisConf.GetCmuIp());
		return TRUE;
	}
	else
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetConfExInfo<E164:%s> err\n", __FUNCTION__,
			tMcuConf.GetConfIdAttr().GetConfE164());
		return FALSE;
	}
}

typedef BOOL32 (CMauVCInstance::*TAccordRedisAndMcuConfFun)(const TMpcConfData& tMcuConf,
	const s8* pchMcuIp, const u32 dwMcuPid);
#define RECORVER_LOG (&CMauVCInstance::LogAccordConditon)
#define UPDATE_REDISCONF (&CMauVCInstance::UpdateRedisConf)
#define RELEASE_MCUCONF (&CMauVCInstance::ReleaseMcuConf)
#define UPDATE_FLUSH_STOP (&CMauVCInstance::UpdateRedisConfAndNoticeMcuFlushDataAndStopMonitor)
#define SWITCH_CONF (&CMauVCInstance::SwitchConf)
BOOL32 CMauVCInstance::AccordRedisConfAndCmuConf(const TMpcConfData& tMcuConf,
	const s8* pchMcuIp, const u32 dwMcuPid)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_VC, 
		"mcuIp: %s, pid: %u, confE164: %s, confName: %s", pchMcuIp, dwMcuPid,
		tMcuConf.GetConfIdAttr().GetConfE164(), tMcuConf.GetConfIdAttr().GetConfName());
/*  1. 仅log
	2. 结束redis上的当前会所属mcu的该会
	3. 更新redis会议为该cmu会议数据，包括设置会议归属, 更新状态为正运行
	4. 结束上报的mcu会
	5. 通知mcu刷新会议数据到redis
	6. 若会馆有监控需要停止
										   正运行                  创建中            待恢复     
	-------------------------------------------------------------------------------------------
	cmuTime <= redis && ipEqual(00)          1                  |    3           |     3(mcu短暂断链)
	--------------------------------------------------------------------------------------------
	cmuTime <= redis && !ipEqual(01)         4                  |    4           |     356
	---------------------------------------------------------------------------------------------
	cmuTime > redis && ipEqual(10)           1                  |    3           |     3
	----------------------------------------------------------------------------------------------
	cmuTime > redis && !ipEqual(11)          2356               |    2356        |     356
	----------------------------------------------------------------------------------------------*/

	TRedisConfInfo tConfInfo;
	if (!g_cMauVCApp.GetConfExInfo(tMcuConf.GetConfIdAttr().GetConfE164(), tConfInfo))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetConfExInfo Fail\n", __FUNCTION__);
		return FALSE;
	}
	else if(!IsRedisHasConfExData(tConfInfo))
	{
		//如果会不存在redis上，则结该cmu的会
		ReleaseMcuConf(tMcuConf, pchMcuIp, dwMcuPid);
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]ConfE164:%s unexist, release\n",
			__FUNCTION__, tMcuConf.GetConfIdAttr().GetConfE164());
		return TRUE;
	}
	s8 achCmuConfTime[100 + 1] = { 0 };
	tMcuConf.GetStartTime().GetString(achCmuConfTime, sizeof(achCmuConfTime));
	const s8 chRedisConfState = TranslateConfState(tConfInfo.GetState());
	const u8 byConfCondition = GetConfCondition(achCmuConfTime,
		tConfInfo.GetStartTime(), pchMcuIp, tConfInfo.GetCmuIp());
	if ((0 <= chRedisConfState && chRedisConfState <= 2) 
		&& (0 <= byConfCondition && byConfCondition <= 3))
	{
		TAccordRedisAndMcuConfFun tAccordFuncArry[3][4] =
		{
			//00               01                 10                 11
			{RECORVER_LOG,     RELEASE_MCUCONF,   RECORVER_LOG,      SWITCH_CONF},   //正运行
			{UPDATE_REDISCONF, RELEASE_MCUCONF,   UPDATE_REDISCONF,  SWITCH_CONF},   //创建中
			{UPDATE_REDISCONF, UPDATE_FLUSH_STOP, UPDATE_REDISCONF,  UPDATE_FLUSH_STOP   }         //待恢复
		};
		TAccordRedisAndMcuConfFun tAccordFun = tAccordFuncArry[chRedisConfState][byConfCondition];
		if (!(this->*tAccordFun)(tMcuConf, pchMcuIp, dwMcuPid))
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Accord Fail\n", __FUNCTION__);
			return FALSE;
		}

		TCmuData tCmuData;
		if (tAccordFun != &CMauVCInstance::ReleaseMcuConf )
		{
			// 将会议信息写入数据库
			AddConfInfo(tMcuConf.GetConfIdAttr().GetConfE164(), 
				tMcuConf.GetConfIdAttr().GetConfName(), tMcuConf.GetConfIdAttr().GetConfPwd());
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]AddConfInfo<%s:%s:%s> to db\n",
				__FUNCTION__, tMcuConf.GetConfIdAttr().GetConfE164(), tMcuConf.GetConfIdAttr().GetConfName(),
				tMcuConf.GetConfIdAttr().GetConfPwd());

			if (g_cMauVCApp.GetMpcData(pchMcuIp, tCmuData) && IsNewMcu(tCmuData.GetCmuType()))
			{
				CKdvDomainInfo tDomainInfo;
				if (g_cMauVCApp.GetDomainInfo(tConfInfo.GetDomainMOID(), tDomainInfo))
				{
					UpuAddConf(tConfInfo.GetConfE164(), TUpuConfInfo::emOnGoingConf,
						tDomainInfo.GetDomainMOID(), tDomainInfo.GetSDomainMOID(), pchMcuIp);
					LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]upu add ongoing conf<%s:%s:%s>\n",
						__FUNCTION__, tConfInfo.GetConfE164(), tDomainInfo.GetDomainMOID(), pchMcuIp);
				}
				else
				{
					LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]getdomain<%s> fail\n",
						__FUNCTION__, tConfInfo.GetDomainMOID());
				}
			}
		}
	}
	else
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Exception<confSate: %d, confCondition: %u\n",
			__FUNCTION__, chRedisConfState, byConfCondition);
	}
	return TRUE;
}

void CMauVCInstance::DaemonProcInitRedisConfTimer(const CMessage *pcMsg, CMauVCApp* pcApp)
{
	KillTimer(MAU_INIT_REDISCONF_TIMER);

	InitRedisConf();
}
//---------------会议恢复end------------------------------------

void CMauVCInstance::DaemonProcTempConfNtf(const CMessage* pcMsg)
{
	switch (pcMsg->event)
	{
		case MAU_MAU_ADDTEMPLATE_NTF:
		{
			const CTempCacheData* pTempCahceData = (const CTempCacheData*)pcMsg->content;
			u32 dwNum = pcMsg->length / sizeof(CTempCacheData);
			for (u32 i = 0; i < dwNum; ++i)
			{
				g_cMauVCApp.AddTempData(pTempCahceData[i]);
			}
		}
		break;
	default:
		break;
	}

}

void CMauVCInstance::DaemonProcOngoingConfNtf(const CMessage * pcMsg)
{
	switch (pcMsg->event)
	{
		case MAU_MAU_STATECHECK_NTF:
		{
// 			u16 wNum = pcMsg->length / sizeof(TDBConfInfo);
// 			const TDBConfInfo* atDBConfInfo = (const TDBConfInfo*)pcMsg->content;
// 			u8 byState = 3;
// 			TDBOprTaskMsgItem tDBMsgEnd;
// 			tDBMsgEnd.SetMsgBody(&byState, sizeof(byState));
// 			byState = 1;
// 			TDBOprTaskMsgItem tDBMsgBook;
// 			tDBMsgBook.SetMsgBody(&byState, sizeof(byState));
// 			for (u16 wIdx = 0; wIdx < wNum; ++wIdx)
// 			{
// 				CMauData::CConfCacheDataMap::iterator it = g_cMauVCApp.m_mapConfCacheInfo.find(atDBConfInfo[wIdx].m_abyE164);
// 				// 如果是同一个会议
// 				if (it != g_cMauVCApp.m_mapConfCacheInfo.end())
// 				{
// 					if (it->second->GetMeetingID() == atDBConfInfo[wIdx].m_dwMeetingId)
// 					{
// 						it->second->SetConfState(enumConnectWaitingState);
// 						continue;
// 					}
// 				}
// 				if (atDBConfInfo[wIdx].m_byState == 1)
// 				{
// 					tDBMsgBook.CatMsgBody(&atDBConfInfo[wIdx].m_dwMeetingId, sizeof(atDBConfInfo[wIdx].m_dwMeetingId));
// 				}
// 				else
// 				{
// 					tDBMsgEnd.CatMsgBody(&atDBConfInfo[wIdx].m_dwMeetingId, sizeof(atDBConfInfo[wIdx].m_dwMeetingId));
// 				}
// 			}
// 
// 			if (tDBMsgBook.GetMsgBodyLen() > sizeof(u8))
// 			{
// 				g_cMauVCApp.m_cVMeetingDBOpr.UpdateVMeetingState(tDBMsgBook);
// 			}
// 
// 			if (tDBMsgEnd.GetMsgBodyLen() > sizeof(u8))
// 			{
// 				g_cMauVCApp.m_cVMeetingDBOpr.UpdateVMeetingState(tDBMsgEnd);
// 			}
		}
		break;
	default:
		break;
	}
}

void CMauVCInstance::DaemonEnableCmu(const CMessage * pcMsg)
{
	BOOL32 bEnable = (pcMsg->event == MAU_MAU_ENABLE_CMU_CMD) ? TRUE : FALSE;

	u32 wMpcIp = *(const u16*)pcMsg->content;

	g_cMauVCApp.EnableMpc(wMpcIp, bEnable);
}

void CMauVCInstance::DaemonPlatformConfLimit(const CMessage* pcMsg)
{
	const CPlatformConfNumLimit cConfNumLimit(*(const CPlatformConfNumLimit*)pcMsg->content);
	g_cMauVCApp.SetPlatConfLimitNum(cConfNumLimit);
	return;
}

void CMauVCInstance::SetVmpParamToJson(json_t_ptr pjRoot, TVMPParam tVmpParam)
{
	if (NULL == pjRoot)
	{
		OspPrintf(TRUE, FALSE, "[SetVmpParamToJson]pjRoot is NULL, return!\n");
		return;
	}

	//vmp信息
	json_t_ptr pjVmpParam = CreatVmpParamSetJson(tVmpParam);
	if (pjVmpParam != NULL)
	{
		json_object_set_s(pjRoot, JSON_VMPPARAM, pjVmpParam);
	}
}

BOOL32 CMauVCInstance::GetTransportAddrFromJson(json_t_ptr pjTransportAddr, LPSTR lpIPAddr, LPSTR lpPort)
{
	
	if ( NULL == pjTransportAddr )
	{
		LogPrint( LOG_LVL_WARNING, MID_MCU_MAUSSN, "[GetTransportAddrFromJson] pjTransportAddr err, return!\n" );
		return FALSE;
	}
	
	json_t_ptr pjIp = json_object_get_s(pjTransportAddr, JSON_IP);
	if ( NULL == pjIp )
	{
		LogPrint( LOG_LVL_WARNING, MID_MCU_MAUSSN, "[GetTransportAddrFromJson] json_object_get JSON_IP err, return!\n" );
		return FALSE;
	}
	const s8* pIPAddress = json_string_value(pjIp);
	strcpy(lpIPAddr, pIPAddress);
	
	json_t_ptr pjPort = json_object_get_s(pjTransportAddr, JSON_PORT);
	if ( NULL == pjPort )
	{
		LogPrint( LOG_LVL_WARNING, MID_MCU_MAUSSN, "[GetTransportAddrFromJson] json_object_get JSON_PORT err, return!\n" );
		return FALSE;
	}
	const s8* pPort = json_string_value(pjPort);
	strcpy(lpPort, pPort);
	
	return TRUE;
}

void CMauVCInstance::UpdateMtAliveTime(LPCSTR lpConfE164, LPCSTR lpIPAddr, LPCSTR lpPort)
{
	g_cMauVCApp.UpdateMonitorAliveTime(lpConfE164, lpIPAddr, lpPort);
}

void CMauVCInstance::SetStopMonitorInfoToJson(s8* achJsonBuf, u32 &dwJsonBufLen, const TMonitorAliveInfo& tMtAlive)
{
	if ( NULL == achJsonBuf )
	{
		return;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = NULL;
	if ( 0 != dwJsonBufLen )
	{
		pjRoot = json_loads( achJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	}
	else
	{
		pjRoot = json_object();
	}

	if ( NULL == pjRoot )
	{
		OspPrintf( TRUE, FALSE, "[SetStopMoniInfoToJson] pjRoot is NULL, return\n" );
		return;
	}

	json_t_ptr pjDstAddr = json_array();
	if (NULL == pjDstAddr)
	{
		OspPrintf( TRUE, FALSE, "[SetStopMoniInfoToJson] json_object pjDstAddr is NULL!\n" );
	}
	json_t_ptr pjTransPortAddr = json_object();
	if (NULL == pjTransPortAddr)
	{
		OspPrintf(TRUE, FALSE, "[SetStopMoniInfoToJson] json_object pjTransPortAddr is NULL!\n");
	}
	json_t_ptr pjAddr = json_object();
	if (NULL == pjAddr)
	{
		OspPrintf( TRUE, FALSE, "[SetStopMoniInfoToJson] json_object pjAddr is NULL!\n" );
	}

	SetStringToJson(pjAddr, JSON_IP, tMtAlive.m_achIpAddr);
	SetStringToJson(pjAddr, JSON_PORT, tMtAlive.m_achPort);
	json_object_set_s(pjTransPortAddr, JSON_TRANSPORTADDR, pjAddr);
	json_array_append_s(pjDstAddr, pjTransPortAddr);
	json_object_set_s(pjRoot, JSON_DSTADDR, pjDstAddr);

	json_str pchData(pjRoot);
	if ( NULL == pchData )
	{
		OspPrintf( TRUE, FALSE, "[SetStopMoniInfoToJson] json_dumps is NULL!\n" );
		return;
	}

	if ( MAXLEN_JSONBUF < strlen( pchData ) )
	{
		OspPrintf( TRUE, FALSE, "[SetStopMoniInfoToJson] pchData len > MAXLEN_JSONBUF, return!\n" );
		return;
	}

	dwJsonBufLen = strlen( pchData );

	memcpy( achJsonBuf, pchData, dwJsonBufLen );
	return;
}

void CMauVCInstance::CheckAllMonitorAlive()
{
	SetTimer(MAU_MONITOR_ALIVE_CHECK_TIMER, TIMER_SPAN_MONITOR_ALIVE_CHECK);

	vector<TMonitorAliveInfo> vecMonitors;
	g_cMauVCApp.GetExpiredMonitors(vecMonitors);
	for (vector<TMonitorAliveInfo>::iterator it = vecMonitors.begin();
	it != vecMonitors.end(); ++it)
	{
		u32 dwJsonBufLen = 0;
		s8 achJsonBuf[MAXLEN_JSONBUF + 1] = {0};
		SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_STOPMONITORING_CMD);
		SetConfE164ToJson(achJsonBuf, dwJsonBufLen, it->m_achConfE164);
		SetStopMonitorInfoToJson(achJsonBuf, dwJsonBufLen, *it);
		CMauData::SendMsgToCmuSsn(it->m_achMcuIpAddr, CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);
		LogPrint(LOG_LVL_WARNING, MID_MAU_VC, "[CMauVCInstance::StopMonitor] css to cmu stop monitor"
			" json data:%s, data len:%d, mcu: %s\n", achJsonBuf, dwJsonBufLen, it->m_achMcuIpAddr);
	}
}

void CMauVCInstance::DaemonProcCmtoCmuMsg(CMessage * const pcMsg)
{
	CMauMsg cSrvmsg(pcMsg->content, pcMsg->length);

	u8* pbuf = cSrvmsg.GetMsgBody();

	const s8* achRpcReplyTo = (const s8*)pbuf;

	pbuf += MAXLEN_ALIAS + 1;

	const s8* achRpcCorrelationId = (const s8*)pbuf;

	pbuf += MAXLEN_ALIAS + 1;

	s8* achJsonBuf = (s8*)pbuf;

	u32 dwJsonBufLen = strlen(achJsonBuf);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMCMU, "[CCMCmuSsnData::ContentBody_MQCB]cmu1 cm json len is %d, text is:.\n", dwJsonBufLen);
	LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMCMU, achJsonBuf, dwJsonBufLen);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMCMU, "[CCMCmuSsnData::ContentBody_MQCB]end\n");

	s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
	GetConfE164FromJson(achJsonBuf, achConfE164, MAXLEN_CONFE164);
	TRedisConfInfo tRedisInfo;
	BOOL32 bResult = g_cMauVCApp.GetConfExInfo(achConfE164, tRedisInfo);
	if (!bResult || !IsRedisHasConfExData(tRedisInfo))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_CMCMU, "[CCMCmuSsnData::ContentBody_MQCB]Conf %s Not Exist err, return!\n", achConfE164);
		return;
	}

	s8 achMsgType[MAXLEN_MSGTYPE + 1] = { 0 };
	u32 dwMsgTypeLen = MAXLEN_MSGTYPE;
	GetMsgTypeFromJson(achJsonBuf, achMsgType, dwMsgTypeLen);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_CMCMU, "[CCMCmuSsnData::ContentBody_MQCB]Rcv msg.%s from CM!\n", achMsgType);

	u16 wMsgType = CM_CMU_CONFOPR_NTF;
	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetReqEventID(wMsgType);
	tReqHeadInfo.SetMQName(achRpcReplyTo);   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId(achRpcCorrelationId);	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	CMauData::SendMsgToCmuSsn(tRedisInfo.GetCmuIp(), wMsgType, achJsonBuf, dwJsonBufLen);
}


BOOL32 CMauVCInstance::GetMoniAliveInfoFromJson(s8* achJsonBuf, TMonitorAliveInfo& tMtAlive)
{
	OspPrintf(TRUE, FALSE, "[CMauVCInstance::GetMoniAliveInfoFromJson]get monitor alive info json data:%s, data len:%d\n", achJsonBuf, strlen(achJsonBuf));
	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( achJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL == pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[CMauVCInstance::GetAddrFromJson] json_loads err, return!\n" );
		return FALSE;
	}
	json_t_ptr pjMediaMode = json_object_get_s(pjRoot, JSON_MEDIAMODE);
	if ( NULL == pjMediaMode)
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[CMauVCInstance::GetAddrFromJson] json_object_get mediamode err, return!\n" );
		return FALSE;
	}
	tMtAlive.m_byMediaMode = (u8)atoi(json_string_value(pjMediaMode));

	json_t_ptr pjDstAddr = json_object_get_s(pjRoot, JSON_DSTADDR);
	if ( NULL == pjDstAddr )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[CMauVCInstance::GetAddrFromJson] json_object_get addr, return!\n");
		return FALSE;
	}

	json_t_ptr pjTransportAddr = json_object_get_s( pjDstAddr, JSON_TRANSPORTADDR );
	if ( NULL == pjTransportAddr )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[CMauVCInstance::GetAddrFromJson] json_object_get transportaddr, return!\n");
		return FALSE;
	}

	json_t_ptr pjIp = json_object_get_s(pjTransportAddr, JSON_IP);
	if ( NULL == pjIp )
	{
		LogPrint( LOG_LVL_WARNING, MID_MCU_MAUSSN, "[CMauVCInstance::GetAddrFromJson] json_object_get JSON_IP err, return!\n" );
		return FALSE;
	}
	const s8* pchIP = json_string_value(pjIp);
	strcpy(tMtAlive.m_achIpAddr, pchIP);
	OspPrintf(TRUE, FALSE, "[CMauVCInstance::GetMoniAliveInfoFromJson]get monitor alive info ip address:%s\n", tMtAlive.m_achIpAddr);

	json_t_ptr pjPort = json_object_get_s(pjTransportAddr, JSON_PORT);
	if ( NULL == pjPort )
	{
		LogPrint( LOG_LVL_WARNING, MID_MCU_MAUSSN, "[CMauVCInstance::GetAddrFromJson] json_object_get JSON_PORT err, return!\n" );
		return FALSE;
	}
	const s8* pchPort = json_string_value(pjPort);
	strcpy(tMtAlive.m_achPort, pchPort);
	OspPrintf(TRUE, FALSE, "[CMauVCInstance::GetMoniAliveInfoFromJson]get monitor alive info port:%s\n", tMtAlive.m_achPort);
	return TRUE;
}

void CMauVCInstance::PrintConfMonitor(LPCSTR lpConfE164)
{
	vector<TMonitorAliveInfo> monitorVec;
	g_cMauVCApp.GetConfMonitorsInfo(lpConfE164, monitorVec);
	OspPrintf(TRUE, FALSE, "conf E164:%s, monitor info %d notes\n", lpConfE164, monitorVec.size());

	for (vector<TMonitorAliveInfo>::iterator monitorIter = monitorVec.begin();
	monitorIter != monitorVec.end(); ++monitorIter)
	{
		OspPrintf(TRUE, FALSE, "conf E164:%s, monitor ip:%s, monitor port:%s, media mode:%d, mcuIp:%s",
			lpConfE164, monitorIter->m_achIpAddr, monitorIter->m_achPort, monitorIter->m_byMediaMode, 
			monitorIter->m_achMcuIpAddr);
		TKdvTime tKdvTime;
		tKdvTime.SetTime(&monitorIter->m_tAliveTime);
		s8 achString[100] = { 0 };
		sprintf(achString, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d\n",
			tKdvTime.GetYear(), tKdvTime.GetMonth(), tKdvTime.GetDay(),
			tKdvTime.GetHour(), tKdvTime.GetMinute(), tKdvTime.GetSecond());
		OspPrintf(TRUE, FALSE, achString);
	}
}

void CMauVCInstance::PrintMonitorInfo()
{
	list<TRedisConfInfo> confList;
	g_cMauVCApp.GetAllConfExInfo(confList);
	for (list<TRedisConfInfo>::const_iterator confIter = confList.begin();
	confIter != confList.end(); ++confIter)
	{
		PrintConfMonitor(confIter->GetConfE164());
	}
}

void CMauVCInstance::DaemonProcCmutoCmMsg(CMessage * const pcMsg)
{
	CMauMsg cServMsg;
	cServMsg.SetMsgBody(pcMsg->content, pcMsg->length);

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = MAXLEN_JSONBUF >= cServMsg.GetMsgBodyLen() ? cServMsg.GetMsgBodyLen() : MAXLEN_JSONBUF;
	memcpy(achJsonBuf, cServMsg.GetMsgBody(), dwJsonBufLen);

	s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
	json_t_ptr pjRoot;
	if (LoadJson(achJsonBuf, dwJsonBufLen, pjRoot, FALSE))
	{
		GetConfE164FromJson(pjRoot, achConfE164, MAXLEN_CONFE164);
		TRedisConfInfo tRedisInfo;
		BOOL32 bResult = g_cMauVCApp.GetConfExInfo(achConfE164, tRedisInfo);
		if (bResult && IsRedisHasConfExData(tRedisInfo))
		{
			SetConfE164ToJsonObj(pjRoot, tRedisInfo.GetConfE164());
			SetConfNameToJsonObj(pjRoot, tRedisInfo.GetConfName());
			SetDomainMoidToJsonObj(pjRoot, tRedisInfo.GetDomainMOID());
			SetMeetingIDToJsonObj(pjRoot, tRedisInfo.GetMeetingID());
			json_str pchData(pjRoot);
			cServMsg.SetMsgBody(pchData, strlen(pchData));
			CMauData::SendMsgToCmCmuSsn(CMU_CM_CONFOPR_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
		}
		else
		{
			LogPrint(LOG_LVL_WARNING, MID_MAU_VC, "[CCMCmuSsnInstance::DaemonProcMcuRspOpr]can't find conf %s, drop msg\n", achConfE164);
		}
	}
	return;
}


void CMauVCInstance::RegInitDealFun()
{
	s_InitDealfun_vec.push_back(&CMauVCInstance::InitRedisConf);
	s_InitDealfun_vec.push_back(&CMauVCInstance::MoveScheduleDbDataToRedis);
	s_InitDealfun_vec.push_back(&CMauVCInstance::LoadDBData);
	s_InitDealfun_vec.push_back(&CMauVCInstance::GetPlatFormDomainInfo);
	s_InitDealfun_vec.push_back(&CMauVCInstance::GetServDomainInfo);
	s_InitDealfun_vec.push_back(&CMauVCInstance::GetUserDomainInfo);
	s_InitDealfun_vec.push_back(&CMauVCInstance::GetDomainHasXmpu);
	s_InitDealfun_vec.push_back(&CMauVCInstance::GetMtInfo);
	s_InitDealfun_vec.push_back(&CMauVCInstance::GetHduMoidAndUserDomainMoid);
	s_InitDealfun_vec.push_back(&CMauVCInstance::GetPlatformIsRent);
}

void CMauVCInstance::LoadDBData()
{
	//加载模板信息到内存中
	g_cMauVCApp.m_cTemplateDBOpr.LoadDBData();
}

void CMauVCInstance::GetServDomainInfo()
{
	g_cMauVCApp.m_cApInfoDBOpr.GetServDomainInfo();
}

void CMauVCInstance::GetPlatFormDomainInfo()
{
	g_cMauVCApp.m_cApInfoDBOpr.GetPlatFormDomainInfo();
}

void CMauVCInstance::GetUserDomainInfo()
{
	g_cMauVCApp.m_cApInfoDBOpr.GetUserDomainInfo();
}

void CMauVCInstance::GetMtInfo()
{
	g_cMauVCApp.m_cApInfoDBOpr.GetMtInfo();
}

void CMauVCInstance::GetDomainHasXmpu()
{
	g_cMauVCApp.m_cApInfoDBOpr.GetDomainHasXmpu();
}

void CMauVCInstance::GetHduMoidAndUserDomainMoid()
{
	g_cMauVCApp.m_cApInfoDBOpr.GetHduMoidAndUserDomainMoid();
}

void CMauVCInstance::GetPlatformIsRent()
{
	g_cMauVCApp.m_cApInfoDBOpr.GetPlatformIsRent();
}

void CMauVCInstance::InitRedisConf()
{
	if (!g_cMauVCApp.StartInitRedisConf())
	{
		SetTimer(MAU_INIT_REDISCONF_TIMER, INIT_REDISCONF_SPAN);
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Init RedisConf fail, settimer<1s> to init\n",
			__FUNCTION__);
	}
	else
	{
		++g_cMauVCApp.m_byInitCount;
		StartServer(&g_cMauVCApp);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Init redisConf success, start server\n",
			__FUNCTION__);
	}

}

void CMauVCInstance::MoveScheduleDbDataToRedis()
{
	//迁移sp2预约会议数据库数据到redis
	g_cMauVCApp.m_cScheduledDBOpr.GetAllScheduledConfInfoThenClear();
}
