/*****************************************************************************
模块名      : MAU
文件名      : scheduledconfinfodbopr.h
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
#include "scheduledbopr.h"
#include "evmau.h"
#include "maustruct.h"
#include "upussn.h"

 extern CMauVCApp g_cMauVCApp;

/*=============================================================================
函 数 名： CScheduledConfInfoDBOpr
功    能： 预约会议详细信息数据库操作类构造函数
算法实现： 
全局变量： 
参    数： 
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
CScheduledConfInfoDBOpr::CScheduledConfInfoDBOpr() : CAsynDBOpr()
{}

/*=============================================================================
函 数 名： ~CScheduledConfInfoDBOpr
功    能： 预约会议详细信息数据库操作类析构函数
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
CScheduledConfInfoDBOpr::~CScheduledConfInfoDBOpr()
{

}

const s8* CScheduledConfInfoDBOpr::GetModuleName()
{
	return "CScheduledConfInfoDBOpr";
}

u16 CScheduledConfInfoDBOpr::GetModuleId()
{
	return MID_MAU_SCHEDB;
}

u8  CScheduledConfInfoDBOpr::GetAllScheduledConfInfoThenClear()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETALLSCHEDULEDCONFINFOOPR);
	PostOneMsgToDBOprTask( tDBMsg, TRUE );

	return 0;
}

/*=============================================================================
函 数 名： AddScheduledConfInfo
功    能： 
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/25  KDVP2.6     阮楠楠                    创建
=============================================================================*/
void CScheduledConfInfoDBOpr::PrintConnectStatus()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)PRINT_CONNECT_STATUS );
	PostOneMsgToDBOprTask( tDBMsg, FALSE );
	
	return;
}

BOOL32 CScheduledConfInfoDBOpr::ProcGetAllScheduledConfInfoThenClear( TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine )
{
	if( NULL == ptDBEngine )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_SCHEDB, "[CScheduledConfInfoDBOpr::ProcAllGetScheduledConfInfo] Param err, return!\n" );
		return FALSE;
	}

	s8  szSql[256] = {0};
	sprintf( szSql, "select * from %s", SCHEDULEDINFO);
	u16 wRet = ptDBEngine->ExecSql( szSql, TRUE );
	if (ERR_DB_SUCCESS != wRet || ptDBEngine->IsEof())
	{
		CMauMsg cNtfMsg;
		TMeetingDBFinishOpr tOpr(GETALLSCHEDULEDCONFINFOOPR, wRet);
		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
		OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_FINISHOPR_SCHEDULEDCONF_NTF, &cNtfMsg, sizeof(cNtfMsg));
		LogPrint( LOG_LVL_ERROR, MID_MAU_SCHEDB, "[ProcAllGetScheduledConfInfo] Get All scheduled conf  Error id = %u\n", wRet );
		return FALSE;
	}

	s32 nMeetingID = 0;
	s8  achConfName[ MAXLEN_CONFNAME+1 ] = { 0 };
	s8  achConfE164[ MAXLEN_CONFE164+1 ]     = { 0 };
	s8	achDomainGUID[ OPERATION_MAXLEN_GUID+1 ] = { 0 };
	s8  achDomainMOID[ OPERATION_MAXLEN_MOID+1 ] = { 0 };	//域的MOID
	s8	achDomainName[OPERATION_MAXLEN_DOMAINNAME+1] = {0};
	s8	achScheduledConfData[MAXLEN_JSONBUF+1] = {0};
	s8  achStartTime[ 20+1 ] = { 0 };
	s8  achEndTime[ 20+1 ] = { 0 };
	TKdvTime  tStartTime;						    //开始时间

	while( !ptDBEngine->IsEof() )
	{
		nMeetingID     = 0;

		memset( achConfName,   0, sizeof(achConfName) );
		memset( achConfE164,   0, sizeof(achConfE164) );
		memset( achDomainGUID, 0, sizeof(achDomainGUID) );
		memset( achDomainMOID, 0, sizeof(achDomainMOID) );
		memset( achStartTime,  0, sizeof(achStartTime) );
		memset( achEndTime,    0, sizeof(achEndTime) );

		//预约会议E164
		ptDBEngine->GetFieldValue( TCONFE164, achConfE164, sizeof(achConfE164) );
		//预约会议MeetingId
		ptDBEngine->GetFieldValue( TMEETINGID, nMeetingID );
		//预约会议名称
		ptDBEngine->GetFieldValue( TCONFNAME, achConfName, sizeof(achConfName) );
		//预约会议所属域域名
		ptDBEngine->GetFieldValue( TDOMAINNAME, achDomainName, sizeof(achDomainName) );
		//预约会议所属域GUID
		ptDBEngine->GetFieldValue( TDOMAINGUID, achDomainGUID, sizeof(achDomainGUID) );
		//预约会议所属域MOID
		ptDBEngine->GetFieldValue( TDOMAINMOID, achDomainMOID, sizeof(achDomainMOID) );
		//开始时间
		ptDBEngine->GetFieldValue( TSTARTTIME, achStartTime, sizeof(achStartTime) );
		//结束时间
		ptDBEngine->GetFieldValue( TENDTIME, achEndTime, sizeof(achEndTime) );
		//预约会议详细信息
		ptDBEngine->GetFieldValue( TCONFDATA, achScheduledConfData, sizeof(achScheduledConfData) );

		LogPrint( LOG_LVL_DETAIL, MID_MAU_SCHEDB, "[ProcAllGetScheduledConfInfo] Get scheduled conf %s successful!\n", achConfE164 );

		TConfInfo tConfInfo;
		if (!GetCreateConfParamFromJson(achScheduledConfData, tConfInfo))
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_SCHEDB, "[ProcAllGetScheduledConfInfo] Get Schedule<E164%s> info from json err!\n", achConfE164 );
		}

		TConfInfoEx tConfInfoEx;
		if ( !GetConfInfoExFromJson( achScheduledConfData, tConfInfoEx ) )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_SCHEDB, "[ProcAllGetScheduledConfInfo] Get TConfInfoEx<E164%s> info from json err!\n", achConfE164 );
		}
		// 终端列表
		TMtAlias atMtAlias[MAXNUM_CONF_MT];
		u16 awBitRate[MAXNUM_CONF_MT] = { 0 };
		u16 wMtNum = 0;

		GetMtAliasListFromJson(achScheduledConfData, atMtAlias, awBitRate, wMtNum);
		tStartTime = FormatStr2KdvTime(achStartTime);

		TVideoStreamCap atVideoStreamCap[MAX_CONF_CAP_EX_NUM];
		u8 byCapNum = MAX_CONF_CAP_EX_NUM;
		tConfInfoEx.GetMainStreamCapEx(atVideoStreamCap, byCapNum);

		CRedisBookConf cBookConf;
		cBookConf.SetConfE164(achConfE164);
		cBookConf.SetMeetingID(nMeetingID);
		cBookConf.SetBookStartTime(achStartTime);
		cBookConf.SetBookEndTime(achEndTime);
		cBookConf.SetConfName(achConfName);
		cBookConf.SetDomainMOID(achDomainMOID);
		cBookConf.SetPassword(tConfInfo.GetConfPwd());
		cBookConf.SetIsPublicConf(tConfInfo.GetConfAttrb().GetPublicConf() == 0 ? FALSE : TRUE);
		cBookConf.SetOpenMode(tConfInfo.GetConfAttrb().GetOpenMode());
		cBookConf.SetDualMode(tConfInfo.GetConfAttrb().GetDualMode());
		cBookConf.SetIsConfNodisturb(tConfInfo.GetConfAttrb().IsConfNoDisturb());
		cBookConf.SetMaxJoinedMt(tConfInfo.GetMaxJoinedMtNum());
		cBookConf.SetMixMode(tConfInfo.GetStatus().GetMixerMode());
		cBookConf.SetMtNum(wMtNum);
		const u16 wBitRate = atVideoStreamCap[0].GetMaxBitRate();
		cBookConf.SetBitRate(wBitRate);
		const u8 byResolution = atVideoStreamCap[0].GetResolution();
		cBookConf.SetResolution(byResolution);
		const u8 byMediaType = atVideoStreamCap[0].GetMediaType();
		cBookConf.SetMediaType(byMediaType);
		cBookConf.SetConfDetailJson(achScheduledConfData);
		json_t_ptr pjRoot = NULL;
		if (LoadJson(achScheduledConfData, strlen(achScheduledConfData), pjRoot, FALSE))
		{
			GetCreatorInfoFromJson(pjRoot, cBookConf);
		}
		if (!g_cMauVCApp.AddBookConf(cBookConf))
		{
			wRet = ERR_DB_SET_CBDATA;
			LogPrint(LOG_LVL_ERROR, MID_MAU_SCHEDB, "[%s]AddBookConf Fail\n", __FUNCTION__);
			break;
		}

		ptDBEngine->MoveNext();
	}
	ptDBEngine->GoOnProcRes();

	//只为兼容sp2, 若有数据启动之后删除 
	if (wRet == ERR_DB_SUCCESS)
	{
		sprintf(szSql, "truncate table %s", SCHEDULEDINFO);
		wRet = ptDBEngine->ExecSql(szSql);
	}

	CMauMsg cNtfMsg;
	TMeetingDBFinishOpr tOpr(GETALLSCHEDULEDCONFINFOOPR, wRet);
	cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_FINISHOPR_SCHEDULEDCONF_NTF, &cNtfMsg, sizeof(cNtfMsg));
	LogPrint(LOG_LVL_ERROR, MID_MAU_SCHEDB, "[%s]truncate table<%s>, res: %u\n", __FUNCTION__, SCHEDULEDINFO, wRet);
	return TRUE;
}

/*=============================================================================
函 数 名： ProcPrintConnectStatus
功    能： 打印连接状态
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
BOOL32 CScheduledConfInfoDBOpr::ProcPrintConnectStatus(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	OspPrintf( TRUE, FALSE, "scheduled DB connect status = %d.\n", CheckDBConnectState(ptDBEngine) );

	return TRUE;
}

/*=============================================================================
函 数 名： DBOprReqEntry
功    能： 
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo	数据库任务信息
		   CDBEngine * ptDBEngine       数据库操作引擎
		   BOOL32 bAccept               连接线程是否与数据库断链
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
BOOL32 CScheduledConfInfoDBOpr::DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine)
{
	//参数校验
	if( NULL == ptTaskInfo || NULL == ptDBEngine )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_SCHEDB, "[CScheduledConfInfoDBOpr::DBOprRegEntry] ERROR: param is NULL!\n" );
		return FALSE;
	}

	BOOL32 bRet = FALSE;

	switch( ptTaskInfo->GetTransactionID() )
	{
	case GETALLSCHEDULEDCONFINFOOPR:
		bRet = ProcGetAllScheduledConfInfoThenClear( ptTaskInfo, ptDBEngine );
		break;
	case PRINT_CONNECT_STATUS:
		bRet = ProcPrintConnectStatus(ptTaskInfo, ptDBEngine);
		break;
	default:
		break;
	}

	return bRet;
}

/*=============================================================================
函 数 名： GetDBOprNameByTransId
功    能： 获取DB操作类型
算法实现： 
全局变量： 
参    数： u32 byTransId  事务ID
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
const s8 * CScheduledConfInfoDBOpr::GetDBOprNameByTransId( u32 dwTransId )
{
    const s8 * pchDBOprName = NULL;

	switch ( dwTransId )
	{
	case CLEARDBDATAOPR:
		pchDBOprName = "ClearDBDataOpr";
		break;
	case GETALLSCHEDULEDCONFINFOOPR:
		pchDBOprName = "GetAllScheduledConfInfoOpr";
		break;
	case PRINT_CONNECT_STATUS:
		pchDBOprName = "PrintConnectStatus";
		break;
	default:
		pchDBOprName = NULL;
		break;
	}

	return pchDBOprName;
}