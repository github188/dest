/*****************************************************************************
模块名      : MAU
文件名      : vmeetingdbopr.cpp
创建时间    : 2014年 8月 26日
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
#include "vmeetingdbopr.h"
#include "evmau.h"
#include "maustruct.h"

/*=============================================================================
函 数 名： EncryptModeAdapt
功    能： 数据库中会议加密模式适配
算法实现：
全局变量：
参    数： u8 bySrcMode
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/03/10  KDVP2.5.2     曾捷        创建
=============================================================================*/
u8 CVMeetingDBOpr::EncryptModeAdapt( u8 bySrcMode )
{
	//
	//数据库中的值为 0，则转为0（不加密）
	//值为1或2，则转为2（AES加密）
	//
	
	u8 byDstMode = 0;
	
	if ( 0 == bySrcMode )
	{
		byDstMode = 0;
	}
	else if ( 1 == bySrcMode ||
		      2 == bySrcMode  )
	{
		byDstMode = 2;
	}
	
	return byDstMode;
}

/*=============================================================================
函 数 名： CVMeetingDBOpr
功    能： VMeeting数据库操作类构造函数
算法实现： 
全局变量： 
参    数： 
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
CVMeetingDBOpr::CVMeetingDBOpr() : CAsynDBOpr()
{}

/*=============================================================================
函 数 名： ~CVMeetingDBOpr
功    能： VMeeting数据库操作类析构函数
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
CVMeetingDBOpr::~CVMeetingDBOpr()
{

}

/*=============================================================================
函 数 名： PrintConnectStatus
功    能： 打印连接状态
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/12/10  KDVP2.5.2     曾捷        创建
=============================================================================*/
void CVMeetingDBOpr::PrintConnectStatus()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID(PRINT_CONNECT_STATUS);
	PostOneMsgToDBOprTask(tDBMsg, FALSE);
}

const s8* CVMeetingDBOpr::GetModuleName()
{
	return "CVMeetingDBOpr";
}

u16 CVMeetingDBOpr::GetModuleId()
{
	return MID_MAU_VMDB;
}

/*=============================================================================
函 数 名： CreateMeeting
功    能： 非模板创会
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
void CVMeetingDBOpr::CreateMeeting( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID(CREATE_MEETING_OPR);
	PostOneMsgToDBOprTask(tDBMsg, FALSE);
	
	return;
}

/*=============================================================================
函 数 名： StateCheck
功    能： 会议状态校验
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
void CVMeetingDBOpr::StateCheck()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)STATE_CHECK );
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	
	return;
}

/*=============================================================================
函 数 名： ChangeVmState
功    能： 修改vm状态
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
void CVMeetingDBOpr::ChangeVmState( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID( (u32)CHANGE_VM_STATE );
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return;
}

void CVMeetingDBOpr::CreateMeetingFailed(const u32 nMeetingID)
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetMsgBody(&nMeetingID, sizeof(nMeetingID));
	tDBMsg.SetTransactionID(CREATE_MEETING_FAILED);
	PostOneMsgToDBOprTask(tDBMsg, TRUE);
}

/*=============================================================================
函 数 名： AheadMeeting
功    能： 提前召开
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
void CVMeetingDBOpr::AheadMeeting( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID( (u32)AHEAD_MEETING );
	PostOneMsgToDBOprTask( tDBMsg, FALSE );
	return;
}

/*=============================================================================
函 数 名： SetDBMeetingE164
功    能： 设置DB中的E164
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
void CVMeetingDBOpr::SetDBMeetingE164(const s8* pchConfE164, const u32 dwMeetingID)
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetMsgBody(&dwMeetingID, sizeof(dwMeetingID));
	tDBMsg.CatMsgBody(pchConfE164, MAXLEN_CONFE164);
	tDBMsg.SetTransactionID( (u32)SET_MEETING_E164_OPR );
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return;
}

/*=============================================================================
函 数 名： ReleaseConf
功    能： 结束会议修改状态
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
void CVMeetingDBOpr::ReleaseConf(TDBOprTaskMsgItem & tDBMsg)
{
	tDBMsg.SetTransactionID(RELEASE_CONF_OPR);
	PostOneMsgToDBOprTask(tDBMsg, TRUE);
	return;
}

void CVMeetingDBOpr::ReleaseConf(const s8* pchConfE164, const s8* pchDomainMOID, const u32 nMeetingID)
{
	TReqHeadInfo tReqInfo;
	tReqInfo.SetConfE164(pchConfE164);
	tReqInfo.SetDomainMOID(pchDomainMOID);
	tReqInfo.SetMeetingID(nMeetingID);

	//zengjie[2013/01/10] 结束会议，修改meeting数据库中的v_meeting状态
	TDBOprTaskMsgItem tDBTaskMsg;
	tDBTaskMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
	tDBTaskMsg.SetTransactionID(RELEASE_CONF_OPR);
	PostOneMsgToDBOprTask(tDBTaskMsg, TRUE);
	return;
}

/*=============================================================================
函 数 名： DelayConf
功    能： 延长会议
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/09  KDVP5.0     阮楠楠        创建
=============================================================================*/
void CVMeetingDBOpr::DelayConf( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID( (u32)DELAY_CONFTIME_ORP );
	PostOneMsgToDBOprTask( tDBMsg, FALSE );
	return;
}

/*=============================================================================
函 数 名： SubConfDuration
功    能： 缩短会议运行时间
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/09  KDVP5.0     阮楠楠        创建
=============================================================================*/
void CVMeetingDBOpr::SubConfDuration( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID( (u32)SUB_CONF_DURATION_ORP );
	PostOneMsgToDBOprTask( tDBMsg, FALSE );
	return;
}

void CVMeetingDBOpr::PrintProcedureStatus()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID((u32)PRINT_PROCEDURE_STATUS);
	PostOneMsgToDBOprTask( tDBMsg, FALSE );
	return;
}

void CVMeetingDBOpr::UpdateVMeetingState(TDBOprTaskMsgItem & tDBMsg)
{
	tDBMsg.SetTransactionID((u32)UPDATE_VMEETING_STATE);
	PostOneMsgToDBOprTask(tDBMsg, TRUE);
}

/*=============================================================================
函 数 名： ChangeConfName
功    能： 修改会议名称
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/22  KDVP5.0     阮楠楠        创建
=============================================================================*/
void CVMeetingDBOpr::ChangeConfName( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID( (u32)CHANGE_CONF_NAME );
	PostOneMsgToDBOprTask( tDBMsg, FALSE );
	return;
}

/*=============================================================================
函 数 名： ProcStateCheck
功    能： 
算法实现：
全局变量：
参    数： TDBOprTaskInfo * ptTaskInfo, CDBEngine * ptDBEngine
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
BOOL32 CVMeetingDBOpr::ProcStateCheck(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine )
	{
		return bRet;
	}
	
	u16 wRet = ERR_DB_SUCCESS;
	const s8* szSql = "select vm.e164, vm.meeting_id, m.state from meeting m inner join v_meeting vm on m.id=vm.meeting_id where vm.state = 2";

	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcStateCheck]szSql = %s\n", szSql );
		
	wRet = ptDBEngine->ExecSql(szSql, TRUE);
	
	//通知主控数据库数据加载完成
	TMeetingDBFinishOpr tOpr(STATE_CHECK, wRet);

	if( ERR_DB_SUCCESS != wRet )
	{
		CMauMsg cServMsg;
		cServMsg.SetMsgBody((u8*)&tOpr, sizeof(TMeetingDBFinishOpr));
		LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcStateCheck]  ProcSteteCheck failed!\n" );
		int nRet = OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg));
		LogPrint(LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcCreateMeetingOpr] post EV_MAU_FINISH_MEETING_OPR_NTF to MPCDVC %s, nRet = %d\n", (OSP_OK == nRet) ? "successful" : "fail", nRet);

		return bRet;
	}

	CMauMsg cServMsg;
	TDBConfInfo tDBConfInfo;
	s8 achE164[MAXLEN_E164 + 1] = { 0 };
	bRet = TRUE;
	while (!ptDBEngine->IsEof())
	{
		ptDBEngine->GetFieldValue(VM_E164, tDBConfInfo.m_abyE164, sizeof(tDBConfInfo.m_abyE164));
		ptDBEngine->GetFieldValue(VM_MEETING_ID, tDBConfInfo.m_dwMeetingId);
		ptDBEngine->GetFieldValue(M_STATE, tDBConfInfo.m_byState);

		LogPrint(LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcStateCheck]nMeetingID=%d, nState=%d.\n", tDBConfInfo.m_dwMeetingId, tDBConfInfo.m_byState);

		if (SERV_MSG_LEN - SERV_MSGHEAD_LEN - cServMsg.GetServMsgLen() <  sizeof(tDBConfInfo))
		{
			OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_MAU_STATECHECK_NTF, cServMsg.GetMsgBody(), cServMsg.GetMsgBodyLen());
			cServMsg.SetMsgBody();
		}

		cServMsg.CatMsgBody(&tDBConfInfo, sizeof(tDBConfInfo));
		//移动到下一数据
		ptDBEngine->MoveNext();
	}
	ptDBEngine->GoOnProcRes();

	if (cServMsg.GetMsgBodyLen() != 0)
	{
		OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_MAU_STATECHECK_NTF, cServMsg.GetMsgBody(), cServMsg.GetMsgBodyLen());
	}

	cServMsg.SetMsgBody((u8*)&tOpr, sizeof(TMeetingDBFinishOpr));
	int nRet = OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg));
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcCreateMeetingOpr] post EV_MAU_FINISH_MEETING_OPR_NTF to MPCDVC %s, nRet = %d\n", (OSP_OK == nRet) ? "successful" : "fail", nRet);

	return bRet;
}

/*=============================================================================
. 终端创会
. 终端通过公共模板创会
. 终端通过个人模板创会
=============================================================================*/
BOOL32 CVMeetingDBOpr::ProcCreateMeetingOpr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine )
	{
		return bRet;
	}

	const u8 * pbyMsg = ptTaskInfo->GetMsgBody();
	TReqHeadInfo tHeadInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof( TReqHeadInfo );
	CKdvDomainInfo tDomainInfo = *(CKdvDomainInfo*)pbyMsg;
	pbyMsg += sizeof( CKdvDomainInfo );
	u32 dwJsonBuffLen = *(u32*)pbyMsg;
	pbyMsg += sizeof(dwJsonBuffLen);
	const s8* pchJson = (const s8*)pbyMsg;

	TConfInfo tConfInfo;
	GetCreateConfParamFromJson(pchJson, tConfInfo);


	s8 achConfName[MAXLEN_CONFNAME * 2 + 1] = { 0 };
	u64 nReturn = ptDBEngine->EscapeString(achConfName, tConfInfo.GetConfName());
	if(nReturn == 0)
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_CONFDB, "[CVMeetingDBOpr::ProcCreateMeetingOpr] EscapeStr failed!\n" );
	}
	s8  szSql[256] = {0};
 	sprintf( szSql, "call %s(%d,\"%s\",\"%s\",\"%s\",\"%s\",%d,%d,%d,%d,%d, %d )",
		"CreateMeeting",
		tDomainInfo.GetMaxSmallConfNum() + tDomainInfo.GetMaxLargeConfNum(),
		achConfName,
		tHeadInfo.GetSrcMtE164(),
		tHeadInfo.GetConfE164(),
		tDomainInfo.GetDomainMOID(),
		tConfInfo.GetCapSupport().GetMainVideoResolution(),
		tConfInfo.GetBitRate(),
		tConfInfo.GetDuration(),
		tConfInfo.GetConfAttrb().IsSatDCastMode(),
		tConfInfo.GetConfAttrb().GetPublicConf(),
		GetDBConfScale(tConfInfo.GetMaxJoinedMtNum())
		);
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcCreateMeetingOpr]szSql=%s\n", szSql);
	const u16 wRet = ptDBEngine->ExecSql( szSql, TRUE );
	s32 nMeetingID   = 0;
	if( ERR_DB_SUCCESS != wRet
		|| (ptDBEngine->GetFieldValue( "retMeetingID", nMeetingID ) != ERR_DB_SUCCESS)
		|| nMeetingID == 0)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[%s] ExecSql failed<Ret:%u> or "
			 " GetFieldValue err or mettingID<%u> == 0\n", __FUNCTION__, wRet, nMeetingID);

		const u16 wErrCode = (wRet == ERR_DB_SUCCESS ? ERR_DB_INVALID_PARAM : wRet);
		TMeetingDBFinishOpr tOpr(CREATE_MEETING_OPR, wErrCode);
		CMauMsg cServMsg;
		cServMsg.SetMsgBody(&tOpr, sizeof(TMeetingDBFinishOpr));
		cServMsg.CatMsgBody(&tHeadInfo, sizeof(TReqHeadInfo));
		cServMsg.CatMsgBody(&tDomainInfo, sizeof(CKdvDomainInfo));
		cServMsg.CatMsgBody(&nMeetingID, sizeof(nMeetingID));
		cServMsg.CatMsgBody(&dwJsonBuffLen, sizeof(dwJsonBuffLen));
		cServMsg.CatMsgBody(pchJson, dwJsonBuffLen);
		OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg));
		return FALSE;
	}

	//获取DB中返回的会议召开时间
	u16 wMeetingTime = 0;
	ptDBEngine->GetFieldValue( "retCanuseTime", wMeetingTime );
	ptDBEngine->GoOnProcRes();
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[%s]wMeetingTime = %u, nMeetingID = %d.\n", 
		__FUNCTION__, wMeetingTime, nMeetingID);

	//通知主控数据库数据加载完成
	TMeetingDBFinishOpr tOpr(CREATE_MEETING_OPR, wRet);
	CMauMsg cServMsg;
	cServMsg.SetMsgBody(&tOpr, sizeof(TMeetingDBFinishOpr));
	cServMsg.CatMsgBody(&tHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody(&tDomainInfo, sizeof(CKdvDomainInfo));
	cServMsg.CatMsgBody(&nMeetingID, sizeof(nMeetingID));
	cServMsg.CatMsgBody(&wMeetingTime, sizeof(wMeetingTime));
	cServMsg.CatMsgBody(&dwJsonBuffLen, sizeof(dwJsonBuffLen));
	cServMsg.CatMsgBody(pchJson, dwJsonBuffLen);
	int nRet = OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg));
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcCreateMeetingOpr] post EV_MAU_FINISH_MEETING_OPR_NTF to MPCDVC %s, nRet = %d\n", (OSP_OK == nRet) ? "successful" : "fail", nRet);
	return bRet;
}

/*=============================================================================
函 数 名： ProcChangeVmState
功    能： 
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
BOOL32 CVMeetingDBOpr::ProcChangeVmState(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine )
	{
		return bRet;
	}
	
	const u8 * pbyMsg = ptTaskInfo->GetMsgBody();
	
	s32 nMeetingID = *(s32*)pbyMsg;
	pbyMsg += sizeof( s32 );
	
	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	
	sprintf( szSql, "call %s(%d,%d)", "UpdateVmState", nMeetingID,  2);
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcChangeVmState]szSql = %s\n", szSql);

	wRet = ptDBEngine->ExecSql(szSql);
	
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcChangeVmState] ExecSql UpdateVmState fail.\n");	
		return bRet;
	}
	else
	{
		bRet = TRUE;
	}
	
	return bRet;
}

BOOL32 CVMeetingDBOpr::ProcCreateMeetingFailed(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if (NULL == ptDBEngine)
	{
		return bRet;
	}

	const u8 * pbyMsg = ptTaskInfo->GetMsgBody();

	s32 nMeetingID = *(const s32*)pbyMsg;
	pbyMsg += sizeof(s32);

	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = { 0 };

	sprintf(szSql, "call %s(%d)", "CreateFailChangeState", nMeetingID);

	LogPrint(LOG_LVL_DETAIL, MID_MAU_VMDB, "[%s]szSql = %s\n", __FUNCTION__, szSql);

	wRet = ptDBEngine->ExecSql(szSql);

	if (ERR_DB_SUCCESS != wRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VMDB, "[%s] ExecSql UpdateVmState fail.\n", __FUNCTION__);
		return bRet;
	}
	else
	{
		bRet = TRUE;
	}

	return bRet;
}

/*=============================================================================
函 数 名： ProcAheadMeeting
功    能： 处理提前召开
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
BOOL32 CVMeetingDBOpr::ProcAheadMeeting(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine )
	{
		return bRet;
	}
	
	const u8* pbyMsgBody = ptTaskInfo->GetMsgBody();
	
	const TReqHeadInfo& tReqInfo = *(const TReqHeadInfo*)pbyMsgBody;
	pbyMsgBody += sizeof(TReqHeadInfo);
	
	s32 nMeetingID = *(const s32*)(pbyMsgBody);
	pbyMsgBody += sizeof(s32);
//	nMeetingID     = ntohl( nMeetingID );

	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	
	sprintf( szSql, "call AheadMeeting(%d)", nMeetingID);

	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[ProcAheadMeeting]szSql=%s\n", szSql);
	
	wRet = ptDBEngine->ExecSql(szSql);
	
	if( ERR_DB_SUCCESS == (u32)wRet )
	{
		bRet = TRUE;
		LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CBookDBOpr::ProcAheadMeeting] AheadMeeting in db success.\n" );
	}
	else
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[CBookDBOpr::ProcAheadMeeting] ExecSql AheadMeeting fail.\n");
		
		//通知主控加载数据库失败
		TMeetingDBFinishOpr tOpr(AHEAD_MEETING, wRet);
		CMauMsg cNtfMsg;
		cNtfMsg.SetMsgBody(&tOpr, sizeof(tOpr));
		cNtfMsg.CatMsgBody(&tReqInfo, sizeof(tReqInfo));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
		return bRet;
	}
	
	
	//通知主控数据库数据加载完成
	TMeetingDBFinishOpr tOpr(AHEAD_MEETING, wRet);
	
	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody(&tOpr, sizeof(TMeetingDBFinishOpr));
	cNtfMsg.CatMsgBody(ptTaskInfo->GetMsgBody(), ptTaskInfo->GetMsgBodyLen());
	
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );

	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[ProcAheadMeeting] post EV_MAU_FINISH_MEETING_OPR_NTF to MPCDVC %s, nRet = %d\n", (OSP_OK == nRet)?"successful":"fail", nRet );
	
	return bRet;
}

/*=============================================================================
函 数 名： ProcSetMeetingE164Opr
功    能： 处理补入DB中会议的E164操作
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
BOOL32 CVMeetingDBOpr::ProcSetMeetingE164Opr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine )
	{
		return bRet;
	}

	const u8 * pbyMsg = ptTaskInfo->GetMsgBody();
	
	s32 nMeetingID = *(s32*)pbyMsg;
	pbyMsg += sizeof( s32 );
	
	s8 achConfE164[MAXLEN_CONFE164 + 1];
	memset(achConfE164, 0, sizeof(achConfE164));
	memcpy(achConfE164, pbyMsg, MAXLEN_CONFE164);
	
	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};

	sprintf( szSql, "call %s(%d,\"%s\")", "SetE164IntoMeeting", nMeetingID,  achConfE164);

	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcSetMeetingE164Opr]szSql = %s\n", szSql);
	wRet = ptDBEngine->ExecSql(szSql);
	
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcSetMeetingE164Opr] ExecSql SetE164IntoMeeting fail.\n");
		TMeetingDBFinishOpr tOpr(SET_MEETING_E164_OPR, wRet);
		
		CMauMsg cServMsg;
		cServMsg.SetMsgBody(&tOpr, sizeof(TMeetingDBFinishOpr));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg) );
		
		return bRet;
	}
	else
	{
		bRet = TRUE;
	}
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcSetMeetingE164Opr] SetMeetingE164 in db success. MeetingID = %d,E164 = %s\n", nMeetingID, achConfE164 );

	//通知主控数据库数据加载完成
	TMeetingDBFinishOpr tOpr(SET_MEETING_E164_OPR, ERR_DB_SUCCESS);
	
	CMauMsg cServMsg;
	cServMsg.SetMsgBody(&tOpr, sizeof(TMeetingDBFinishOpr));
	
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg) );

	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcSetMeetingE164Opr] post EV_MPCD_FINISH_MEETING_OPR_NTF to MPCDVC %s, nRet = %d\n", (OSP_OK == nRet)?"successful":"fail", nRet );

	return bRet;	
}

/*=============================================================================
函 数 名： ProcReleaseConfOpr
功    能： 结束会议，修改会议状态
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
BOOL32 CVMeetingDBOpr::ProcReleaseConfOpr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine )
	{
		return bRet;
	}
	
	u8 * pbyMsg = (u8*)ptTaskInfo->GetMsgBody();
	
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof( TReqHeadInfo );
	
	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	s32 nRetMeetingID = 0;
	
	sprintf( szSql, "call ReleaseConf(%d)", tReqInfo.GetMeetingID());

	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcReleaseConfOpr] szSql = %s\n", szSql);
	
	wRet = ptDBEngine->ExecSql( szSql, TRUE );
	
	CMauMsg cServMsg;
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcReleaseConfOpr]  ExecSql release meeting fail.\n");
		
		//通知主控加载数据库失败
		TMeetingDBFinishOpr tOpr(RELEASE_CONF_OPR, wRet);
		cServMsg.SetMsgBody(&tOpr, sizeof(tOpr));
		cServMsg.CatMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg) );
		return bRet;
	}
	else
	{
		bRet = TRUE;
	}
	
	u8 byState = 3;
	//获取结束会议的meetingId
	ptDBEngine->GetFieldValue("mID", nRetMeetingID);
	ptDBEngine->GetFieldValue("mState", byState);
	ptDBEngine->GoOnProcRes();

	if ( 0 == nRetMeetingID || nRetMeetingID != tReqInfo.GetMeetingID())
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcReleaseConfOpr]release meeting fail. ConfE164 = %s.nMeetingId = 0.\n", tReqInfo.GetConfE164());
		
		//通知主控加载数据库失败
		TMeetingDBFinishOpr tOpr(RELEASE_CONF_OPR, ERR_DB_EXECSQL_FAILED);
		cServMsg.SetMsgBody(&tOpr, sizeof(tOpr));
		cServMsg.CatMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg) );
		return bRet;
	}
	else
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcReleaseConfOpr]Release Conf in db success.ConfE164 = %s.nRetMeetingID=%d, byState=%d.\n", 
			tReqInfo.GetConfE164(), nRetMeetingID, byState);
		
		//通知主控数据库数据加载完成
		TMeetingDBFinishOpr tOpr(RELEASE_CONF_OPR, wRet);
		
		cServMsg.SetMsgBody(&tOpr, sizeof(TMeetingDBFinishOpr));
		cServMsg.CatMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
		cServMsg.CatMsgBody(&byState, sizeof(byState));
		int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg) );
		
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcReleaseConfOpr]post EV_MPCD_FINISH_MEETING_OPR_NTF to MPCDVC %s, nRet = %d\n",
			(OSP_OK == nRet)?"successful":"fail", nRet );
	}

	return bRet;
}

/*=============================================================================
函 数 名： ProcDelayConfOpr
功    能： 延长会议
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo, CDBEngine * ptDBEngine
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/09  KDVP5.0		阮楠楠                  创建
=============================================================================*/
BOOL32 CVMeetingDBOpr::ProcDelayConfOpr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine )
	{
		return bRet;
	}

	const u8 * pbyMsg = (const u8*)ptTaskInfo->GetMsgBody();

	const TReqHeadInfo& tReqInfo = *(const TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof( TReqHeadInfo );

	u16 wDelayTime = *(const u16*)pbyMsg;
	pbyMsg += sizeof( u16 );
	wDelayTime = ntohs( wDelayTime );

	const CKdvDomainInfo& tDomainInfo = *(const CKdvDomainInfo*)pbyMsg;
	pbyMsg += sizeof( CKdvDomainInfo );

	const TRedisConfInfo& tConfData = *(const TRedisConfInfo*)pbyMsg;

	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcDelayConfOpr] meetingID=%d, e164=%s, wMtId=%u, wMccId=%u, wDelayTime=%u\n",
		tConfData.GetMeetingID(), tReqInfo.GetConfE164(), tReqInfo.GetMtReqID(), tReqInfo.GetReqSrcID(), wDelayTime );

	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};

	//获取e164对应meetingID
	s32 nMeetingID = tConfData.GetMeetingID();

	////rnn[2014/03/20]修改获取延长会议时间存储过程
	//u32 dwCanDelayTime = 0;
	//sprintf( szSql, "call CanMeetingDelay(%d, %d, %d)", nMeetingID, tDomainInfo.GetMaxConfNum(), wDelayTime );
	//wRet = ptDBEngine->ExecSql(szSql, TRUE);
	CMauMsg cServMsg;
	//if( ERR_DB_SUCCESS != wRet )
	//{
	//	LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcDelayConfOpr] ExecSql get Can Meeting Delay fail, Delay fail.\n");
	//	
	//	//通知主控操作数据库失败
	//	TMeetingDBFinishOpr tOpr(DELAY_CONFTIME_ORP, wRet);
	//	cServMsg.SetMsgBody(&tOpr, sizeof(tOpr));
	//	cServMsg.CatMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
	//	wDelayTime = htons(wDelayTime);
	//	cServMsg.CatMsgBody(&wDelayTime, sizeof(wDelayTime));
	//	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg) );
	//	return bRet;
	//}
	//else
	//{
	//	ptDBEngine->GetFieldValue( "canUseTime", dwCanDelayTime );
	//	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcDelayConfOpr] Can Delay Time = %u.\n", dwCanDelayTime );
	//}
	//ptDBEngine->GoOnProcRes();

	//u32 dwMaxDelayTime = dwCanDelayTime;
	//LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcDelayConfOpr] dwMaxDelayTime = %u.\n", dwMaxDelayTime );

	////到这，总体可延长时间计算完毕，延长数据库中的时间
	//if ( dwMaxDelayTime >= wDelayTime )
	//{
	//	dwMaxDelayTime = (u32)wDelayTime;
	//}
	//else
	//{
	//	LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcDelayConfOpr]dwMaxDelayTime < wDelayTime. Delay fail.\n");

	//	wRet = ERR_DB_INVALID_PARAM;

	//	//通知主控操作数据库失败
	//	TMeetingDBFinishOpr tOpr(DELAY_CONFTIME_ORP, wRet);
	//	cServMsg.SetMsgBody(&tOpr, sizeof(tOpr));
	//	cServMsg.CatMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
	//	wDelayTime = htons(wDelayTime);
	//	cServMsg.CatMsgBody(&wDelayTime, sizeof(wDelayTime));
	//	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg) );
	//	return bRet;
	//}

	//延长meeting表中的时间
	sprintf( szSql, "call SetMeetingForDelay(%d,%d)", nMeetingID, wDelayTime);
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcDelayConfOpr]sql = %s, wRet =%u.\n", szSql, wDelayTime);

	wRet = ptDBEngine->ExecSql(szSql);
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcDelayConfOpr]ExecSql SetMeetingForDelay fail. Delay fail.wRet = %u.\n", wRet);

		//通知主控操作数据库失败
		TMeetingDBFinishOpr tOpr(DELAY_CONFTIME_ORP, wRet);
		cServMsg.SetMsgBody(&tOpr, sizeof(tOpr));
		cServMsg.CatMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
		wDelayTime = htons(wDelayTime);
		cServMsg.CatMsgBody(&wDelayTime, sizeof(wDelayTime));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg) );
		return bRet;
	}
	else
	{
		bRet = TRUE;
		LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcDelayConfOpr]update endTime in meeting table success.\n" );
	}

	//通知主控数据库完成
	TMeetingDBFinishOpr tOpr(DELAY_CONFTIME_ORP, ERR_DB_SUCCESS);

	cServMsg.SetMsgBody(&tOpr, sizeof(TMeetingDBFinishOpr));
	cServMsg.CatMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
	wDelayTime = htons(wDelayTime);
	cServMsg.CatMsgBody(&wDelayTime, sizeof(wDelayTime));

	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg) );
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcDelayConfOpr]post EV_MPCD_FINISH_MEETING_OPR_NTF to MPCDVC %s, nRet = %d\n", 
		(OSP_OK == nRet)?"successful":"fail", nRet );

	return bRet;
}

/*=============================================================================
函 数 名： ProcSubConfDurationOpr
功    能： 
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo, CDBEngine * ptDBEngine
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/09  KDVP5.0		阮楠楠                  创建
=============================================================================*/
BOOL32 CVMeetingDBOpr::ProcSubConfDurationOpr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine )
	{
		return bRet;
	}
	
	const u8 * pbyMsg = ptTaskInfo->GetMsgBody();
	
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof( TReqHeadInfo );
	
	u16 wDuration = *(u16*)pbyMsg;
	
	//缩短数据库中会议时间
	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	
	sprintf( szSql, "call SubConfDuration(%d,\"%s\")", wDuration, tReqInfo.GetConfE164() );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcSubConfDurationOpr] SQL: = %s\n", szSql);
	wRet = ptDBEngine->ExecSql(szSql);
	
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CBookDBOpr::ProcSubConfDurationOpr] ExecSql sub meeting duration fail.wRet = %d\n", wRet);
		
		//通知主控加载数据库失败
		CMauMsg cServMsg;
		TMeetingDBFinishOpr tOpr(SUB_CONF_DURATION_ORP, wRet);
		cServMsg.SetMsgBody(&tOpr, sizeof(tOpr));
		cServMsg.CatMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
		cServMsg.CatMsgBody(&wDuration, sizeof(wDuration));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg) );
		return bRet;
	}
	else
	{	
		bRet = TRUE;
	}
	
	
	
	//通知主控数据库数据加载完成
	TMeetingDBFinishOpr tOpr(SUB_CONF_DURATION_ORP, ERR_DB_SUCCESS);
	CMauMsg cServMsg;
	cServMsg.SetMsgBody(&tOpr, sizeof(TMeetingDBFinishOpr));
	cServMsg.CatMsgBody(&tReqInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody(&wDuration, sizeof(wDuration));
	
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MEETING_OPR_NTF, &cServMsg, sizeof(cServMsg) );
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VMDB, "[CVMeetingDBOpr::ProcSubConfDurationOpr] post EV_MPCD_FINISH_MEETING_OPR_NTF to MPCDVC %s, nRet = %d\n", (OSP_OK == nRet)?"successful":"fail", nRet );
	
	return bRet;
}

BOOL32 CVMeetingDBOpr::ProcPrintProcedureStatus(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if (ptDBEngine->IsConnectDB())
	{
		OspPrintf(TRUE, FALSE, "---meeting db connect ok\n");
	}
	else
	{
		OspPrintf(TRUE, FALSE, ">>>meeting db unconnect\n");
		return FALSE;
	}

	vector<string> vecProcedure;
	vecProcedure.push_back("'AheadMeeting'");
	vecProcedure.push_back("'CreateFailChangeState'");
	vecProcedure.push_back("'CreateMeeting'");
	vecProcedure.push_back("'CreateV'");
	vecProcedure.push_back("'ReleaseConf'");
	vecProcedure.push_back("'SetMeetingForDelay'");
	vecProcedure.push_back("'UpdateVmState'");

	string strSQL = "select name from MySQL.proc where db = 'meeting' and"
		 " `type` = 'PROCEDURE' and name = ";
	vector<string>::const_iterator procedureIter = vecProcedure.begin();
	for (; procedureIter != vecProcedure.end(); ++procedureIter)
	{
		string strRealSql;
		strRealSql = strSQL + *procedureIter;
		const BOOL32 bRet = ptDBEngine->ExecSql(strRealSql.c_str(), TRUE);
		s8 achProcedureName[100 + 1] = { 0 };
		if (bRet != ERR_DB_SUCCESS)
		{
			OspPrintf(TRUE, FALSE, ">>>Check Procedure %s exist failed, sql:%s\n",
				procedureIter->c_str(), strRealSql.c_str());
			continue;
		}
		if(!ptDBEngine->IsEof()
			&& ptDBEngine->GetFieldValue("name", achProcedureName, 100) == ERR_DB_SUCCESS
			&& strncmp(procedureIter->substr(1, procedureIter->length() -2).c_str(),
				achProcedureName, procedureIter->length()) == 0)
		{
			OspPrintf(TRUE, FALSE, "---Procedure %s ok\n", procedureIter->c_str());
		}
		else
		{
			OspPrintf(TRUE, FALSE, ">>>Procedure %s unexist\n", procedureIter->c_str());
		}
	}

	return TRUE;
}

BOOL32 CVMeetingDBOpr::ProcUpdateVMeetingState(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	const u8* pbyBody = ptTaskInfo->GetMsgBody();
	u8 byState = *pbyBody;
	pbyBody += sizeof(u8);

	u16 wNum = (ptTaskInfo->GetMsgBodyLen() - sizeof(u8)) / sizeof(s32);
	const s32* adwMeetingId = (const s32*)pbyBody;

	if (wNum > 0)
	{
		s8 abyTemp[20];
		sprintf(abyTemp, "%d", byState);

		string strSQL = "update v_meeting set state =";
		strSQL += abyTemp;
		strSQL += " where meeting_id in (";

		for (u16 i = 0; i < wNum; ++i)
		{
			sprintf(abyTemp, "%d,", adwMeetingId[i]);
			strSQL += abyTemp;
		}

		strSQL[strSQL.length() - 1] = ')';

		LogPrint(LOG_LVL_DETAIL, MID_MAU_VMDB, "[%s]szSql=%s\n", __FUNCTION__, strSQL.c_str());

		bRet = ptDBEngine->ExecSql(strSQL.c_str());
	}
	else
	{
		bRet = TRUE;
	}
	return bRet;
}

/*=============================================================================
函 数 名： ProcChangeConfNameOpr
功    能： 处理从DB中修改会议名
算法实现：
全局变量：
参    数： TDBOprTaskInfo * ptTaskInfo, CDBEngine * ptDBEngine
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/22  KDVP5.0		阮楠楠                  创建
=============================================================================*/
BOOL32 CVMeetingDBOpr::ProcChangeConfNameOpr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine )
	{
		return bRet;
	}
	
	const u8 * pbyMsg = ptTaskInfo->GetMsgBody();
	
 	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof( TReqHeadInfo );
	
	//会议名
	s8 achConfName[MAXLEN_CONFNAME+1] = {0};
	memcpy( achConfName, pbyMsg, MAXLEN_CONFNAME );

	u16 wConfNameLen = strlen(achConfName);

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VMDB, "[ProcChangeConfNameOpr] e164 = %s, len=%d, confName=%s.\n", tReqInfo.GetConfE164(), wConfNameLen, achConfName);

	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	
	sprintf( szSql, "call %s(\"%s\",\"%s\")",
		"ChangeMeetingName",
		tReqInfo.GetConfE164(),
		achConfName
		);

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VMDB, "szSql = %s\n", szSql);

	wRet = ptDBEngine->ExecSql(szSql, FALSE);
	
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[ProcChangeConfNameOpr] ExecSql ChangeMeetingName failed!\n" );
		return bRet;
	}
	else
	{
		bRet = TRUE;
	}

	return bRet;
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
BOOL32 CVMeetingDBOpr::DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine)
{
	//参数校验
	if( NULL == ptTaskInfo || NULL == ptDBEngine )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VMDB, "[CVMeetingDBOpr::DBOprRegEntry] ERROR: param is NULL!\n" );
		return FALSE;
	}

	BOOL32 bRet = FALSE;

	switch( ptTaskInfo->GetTransactionID() )
	{
	case STATE_CHECK:              //会议状态校验
		bRet = ProcStateCheck( ptTaskInfo, ptDBEngine );
		break;
	case CREATE_MEETING_OPR:       //非模板创会
		bRet = ProcCreateMeetingOpr( ptTaskInfo, ptDBEngine );
		break;
	case CHANGE_VM_STATE:          //修改vm状态
		bRet = ProcChangeVmState(ptTaskInfo, ptDBEngine);
		break;
	case CREATE_MEETING_FAILED:          //修改vm状态
		bRet = ProcCreateMeetingFailed(ptTaskInfo, ptDBEngine);
		break;
	case AHEAD_MEETING:            //提前召开
		bRet = ProcAheadMeeting(ptTaskInfo, ptDBEngine);
		break;
	case SET_MEETING_E164_OPR:      //设置会议数据库中E164
		bRet = ProcSetMeetingE164Opr(ptTaskInfo, ptDBEngine);
		break;
	case RELEASE_CONF_OPR:          //结束会议操作
		bRet = ProcReleaseConfOpr(ptTaskInfo, ptDBEngine);
		break;
	case DELAY_CONFTIME_ORP:		//延长会议
		bRet = ProcDelayConfOpr(ptTaskInfo, ptDBEngine);
		break;
	case SUB_CONF_DURATION_ORP:     //缩短会议运行时间操作
		bRet = ProcSubConfDurationOpr(ptTaskInfo, ptDBEngine);
		break;
	case CHANGE_CONF_NAME:			//修改会议名称
		bRet = ProcChangeConfNameOpr(ptTaskInfo, ptDBEngine);
		break;
	case UPDATE_VMEETING_STATE:
		bRet = ProcUpdateVMeetingState(ptTaskInfo, ptDBEngine);
		break;
	case PRINT_CONNECT_STATUS:
		bRet = PrintStatus(ptDBEngine);
		break;
	case PRINT_PROCEDURE_STATUS:
		bRet = ProcPrintProcedureStatus(ptTaskInfo, ptDBEngine);
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
const s8 * CVMeetingDBOpr::GetDBOprNameByTransId( u32 dwTransId )
{
    const s8 * pchDBOprName = NULL;

	switch ( dwTransId )
	{
	case CLEARDBDATAOPR:
		pchDBOprName = "ClearDBDataOpr";
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
