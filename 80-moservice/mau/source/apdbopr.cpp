/*****************************************************************************
模块名      : MAU
文件名      : apinfodbopr.h
创建时间    : 2013年 12月 05日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/12/05  KDVP2.6     王也        创建
******************************************************************************/

#ifdef WIN32
#pragma warning( disable:4786 )
#endif

#include "inc.h"
#include "mauvc.h"
#include "apdbopr.h"
#include "evmau.h"
#include "maustruct.h"
#include "mauutility.h"

extern CMauVCApp g_cMauVCApp;

/*=============================================================================
函 数 名： CAPInfoDBOpr
功    能： AP数据库操作类构造函数
算法实现： 
全局变量： 
参    数： 
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
CAPInfoDBOpr::CAPInfoDBOpr() : CAsynDBOpr()
{}

/*=============================================================================
函 数 名： ~CAPInfoDBOpr
功    能： AP数据库操作类析构函数
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
CAPInfoDBOpr::~CAPInfoDBOpr()
{

}

const s8* CAPInfoDBOpr::GetModuleName()
{
	return "CAPInfoDBOpr";
}

u16 CAPInfoDBOpr::GetModuleId()
{
	return MID_MAU_APDB;
}

/*=============================================================================
函 数 名： GetServDomainInfo
功    能： 
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
u8 CAPInfoDBOpr::GetServDomainInfo()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETSERVDOMAININFOOPR );
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return 0;
}


/*=============================================================================
函 数 名： GetPlatFormDomainInfo
功    能： 
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
u8 CAPInfoDBOpr::GetPlatFormDomainInfo()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETPLATFORMDOMAININFOOPR );
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return 0;
}

/*=============================================================================
函 数 名： GetUserDomainInfo
功    能： 
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
u8 CAPInfoDBOpr::GetUserDomainInfo()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETUSERDOMAININFOOPR);
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return 0;
}

/*=============================================================================
函 数 名： GetMtInfo
功    能： 
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
u8 CAPInfoDBOpr::GetMtInfo()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETMTINFOOPR);
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return 0;
}

/*=============================================================================
函 数 名： GetDomainHasXmpu
功    能： 
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/03/28  KDVP5.0     阮楠楠                  创建
=============================================================================*/
u8 CAPInfoDBOpr::GetDomainHasXmpu()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETDOMAINHASXMPU );
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return 0;
}

/*=============================================================================
函 数 名： PrintConnectStatus
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
void CAPInfoDBOpr::PrintConnectStatus()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID(PRINT_CONNECT_STATUS);
	PostOneMsgToDBOprTask(tDBMsg, FALSE);
}


u8 CAPInfoDBOpr::GetHduMoidAndUserDomainMoid()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETHDUMOIDANDUSERDOMAINMOID);
	PostOneMsgToDBOprTask(tDBMsg, TRUE);
	return 0;
}

u8 CAPInfoDBOpr::GetPlatformIsRent()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETPLATFORMISRENT);
	PostOneMsgToDBOprTask(tDBMsg, TRUE);
	return 0;
}

/*=============================================================================
函 数 名： GetServDomainInfo
功    能： 获取服务域信息
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo		数据库任务信息
		   CDBEngine * ptDBEngine			数据库操作引擎
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
BOOL32 CAPInfoDBOpr::ProcGetServDomainInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetServDomainInfo] Param err, retur!\n" );
		return FALSE;
	}

	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	//形成SQL
	sprintf( szSql, "select * from %s where %s = %d", SERVDOMAIN, USEDENABLE, 1 );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[ProcGetServDomainInfo]%s.\n", szSql );
	
	// 获取ap域信息失败处理
	wRet = ptDBEngine->ExecSql(szSql, TRUE);
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetServDomainInfo] Get ServDomainInfo failed! Error id = %u\n", wRet );
		//通知主控加载数据库失败
		TMeetingDBFinishOpr tOpr(GETSERVDOMAININFOOPR, wRet);
		CMauMsg cNtfMsg;
		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
		return FALSE;
	}

	CKdvDomainInfo tDomainInfo;
	s8	achDomainName[OPERATION_MAXLEN_DOMAINNAME+1]  = {0};
	s8	achDomainGUID[OPERATION_MAXLEN_GUID+1]  = {0};
	s8  achDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};

	while ( !ptDBEngine->IsEof() )
	{
		tDomainInfo.SetNull();
		memset( achDomainName, 0, sizeof( achDomainName ) );
		memset( achDomainGUID, 0, sizeof( achDomainGUID ) );
		memset( achDomainMOID, 0, sizeof( achDomainMOID ) );

		// 域名称
		ptDBEngine->GetFieldValue( SERVDOMAINNAME, achDomainName, sizeof( achDomainName ) );
		// 域GUID
		ptDBEngine->GetFieldValue( DOMAIN_GUID, achDomainGUID, sizeof( achDomainGUID ) );
		// 域MOID
	    ptDBEngine->GetFieldValue( SERVDOMAINMOID, achDomainMOID, sizeof( achDomainMOID ) );

		tDomainInfo.SetWholeName( achDomainName );
		tDomainInfo.SetDomainGUID( achDomainGUID );
		tDomainInfo.SetUsedFlag( TRUE ) ;
		tDomainInfo.SetDomainLevel( CKdvDomainInfo::em_KdvTopLevelDomain );
		tDomainInfo.SetDomainMOID( achDomainMOID );

		g_cMauVCApp.AddDomainInfo( tDomainInfo );
		
		LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetServDomainInfo] Domain<%s>[%s], MOID[%s]\n", 
			tDomainInfo.GetWholeName(), tDomainInfo.GetDomainGUID(), tDomainInfo.GetDomainMOID() );

		// 移动到下一数据
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();

	//通知主控数据库数据加载完成
	TMeetingDBFinishOpr tOpr(GETSERVDOMAININFOOPR, wRet);
	
	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody(&tOpr, sizeof(TMeetingDBFinishOpr));
	
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[ProcGetServDomainInfo] post EV_MAU_FINISH_AP_OPR_NTF to MPCDVC %s, nRet = %d\n", 
		(OSP_OK == nRet)?"successful":"fail", nRet );

	return TRUE;
}

/*=============================================================================
函 数 名： ProcGetPlatFormDomainInfo
功    能： 获取平台域信息
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo		数据库任务信息
		   CDBEngine * ptDBEngine			数据库操作引擎
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
BOOL32 CAPInfoDBOpr::ProcGetPlatFormDomainInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetPlatFormDomainInfo] Param err, retur!\n" );
		return FALSE;
	}
	
	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	//形成SQL
	sprintf( szSql, "select * from %s", PLATFORMDOMAIN );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "%s.\n", szSql );
	
	// 获取模板信息失败处理
	wRet = ptDBEngine->ExecSql( szSql, TRUE );
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetPlatFormDomainInfo] Get PlatFormDomainInfo failed, Error id = %u\n", wRet );
		//通知主控加载数据库失败
		TMeetingDBFinishOpr tOpr(GETPLATFORMDOMAININFOOPR, wRet);
		CMauMsg cNtfMsg;
		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
		
		return FALSE;
	}
	
// 	if ( ptDBEngine->GetCount() == 0 )
//     {
//         OspPrintf( TRUE, FALSE, "[CAPInfoDBOpr::ProcGetPlatFormDomainInfo] Get PlatFormDomainInfo result set is 0\n" );
// 		//通知主控加载数据库失败
// 		TMeetingDBFinishOpr tOpr(GETPLATFORMDOMAININFOOPR, wRet);
// 		CMauMsg cNtfMsg;
// 		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
// 		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), EV_MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
// 
// 		return FALSE;
//     }
	
	CKdvDomainInfo tDomainInfo;
	s8 achDomainName[OPERATION_MAXLEN_DOMAINNAME+1]   = {0};
	s8 achDomainGUID[OPERATION_MAXLEN_GUID+1]   = {0};
	s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1]  = {0};
	s8 achSDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};
	s8 achPlatformPrefix[OPERATION_MAXLEN_PLATFORMPREFIX + 1] = {0};
	
	while ( !ptDBEngine->IsEof() )
	{
		tDomainInfo.SetNull();
		memset( achDomainName, 0, sizeof( achDomainName ) );
		memset( achDomainGUID, 0, sizeof( achDomainGUID ) );
		memset( achDomainMOID, 0, sizeof( achDomainMOID ) );
		memset( achSDomainMOID, 0, sizeof( achSDomainMOID ) );
		memset( achPlatformPrefix, 0, sizeof( achPlatformPrefix ) );
		
		// 域名称
		ptDBEngine->GetFieldValue( PLATFORMDOMAINNAME, achDomainName, sizeof( achDomainName ) );
		// 域GUID
		ptDBEngine->GetFieldValue( DOMAIN_GUID, achDomainGUID, sizeof( achDomainGUID ) );
		// 域MOID
		ptDBEngine->GetFieldValue( PLATFORMDOMAINMOID, achDomainMOID, sizeof( achDomainMOID ) );
		// 上级域MOID
		ptDBEngine->GetFieldValue( SERVDOMAINMOID, achSDomainMOID, sizeof( achSDomainMOID ) );
		// 平台域前缀
		ptDBEngine->GetFieldValue( PLATFOMRDOMAINPREFIX, achPlatformPrefix, sizeof( achPlatformPrefix ) );

		
		tDomainInfo.SetWholeName( achDomainName );
		tDomainInfo.SetDomainGUID( achDomainGUID );
		tDomainInfo.SetSDomainMOID( achSDomainMOID );
		tDomainInfo.SetUsedFlag( TRUE ) ;
		tDomainInfo.SetDomainLevel( CKdvDomainInfo::em_KdvPlatformDomain );
		tDomainInfo.SetDomainMOID( achDomainMOID );
		tDomainInfo.SetPlatformPrefix( achPlatformPrefix );

		g_cMauVCApp.AddDomainInfo( tDomainInfo );
		
		LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetPlatFormDomainInfo] Domain<%s>[%s], MOID[%s], SDomainMOID[%s] PreFix[%s]\n", 
			tDomainInfo.GetWholeName(), tDomainInfo.GetDomainGUID(), tDomainInfo.GetDomainMOID(), tDomainInfo.GetSDomainMOID(), tDomainInfo.GetPlatformPrefix() );
		
		// 移动到下一数据
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();

	//通知主控数据库数据加载完成
	TMeetingDBFinishOpr tOpr(GETPLATFORMDOMAININFOOPR, wRet);

	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(TMeetingDBFinishOpr));

	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );

	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[ProcGetPlatFormDomainInfo] post EV_MAU_FINISH_AP_OPR_NTF to MPCDVC %s, nRet = %d\n", 
		(OSP_OK == nRet)?"successful":"fail", nRet );

	return TRUE;
}

/*=============================================================================
函 数 名： ProcGetUserDomainInfo
功    能： 获取用户域信息
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo		数据库任务信息
		   CDBEngine * ptDBEngine			数据库操作引擎
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
BOOL32 CAPInfoDBOpr::ProcGetUserDomainInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetUserDomainInfo] Param err, return!\n" );
		return FALSE;
	}
	
	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[512] = {0};
	//形成SQL
	//sprintf( szSql, "select * from %s where %s = %d", USERDOMAIN, USERDOMAINUSEDFLAG, 1 );
	sprintf(szSql, "SELECT *, count(CASE privilege_key WHEN 'enableHDMeeting' THEN 'enableHDMeeting' END) AS enableHDMeeting,count(CASE privilege_key WHEN 'enableH265' THEN 'enableH265' END) AS enableH265 \
		FROM user_domain LEFT JOIN user_domain_privilege_data ON user_domain.user_domain_moid = user_domain_privilege_data.user_domain_moid where used_flag = 1 \
		GROUP BY user_domain.user_domain_moid ");
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_APDB, "%s.\n", szSql );
	
	// 获取模板信息失败处理
	wRet = ptDBEngine->ExecSql( szSql, TRUE );
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetUserDomainInfo] Get UserDomainInfo failed, Error id = %u\n", wRet );
		//通知主控加载数据库失败
		TMeetingDBFinishOpr tOpr(GETUSERDOMAININFOOPR, wRet);
		CMauMsg cNtfMsg;
		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );

		return FALSE;
	}
	
// 	if ( ptDBEngine->GetCount() == 0 )
//     {
//         LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetUserDomainInfo] Get UserDomainInfo result set is 0\n" );
// 		//通知主控加载数据库失败
// 		TMeetingDBFinishOpr tOpr(GETUSERDOMAININFOOPR, wRet);
// 		CMauMsg cNtfMsg;
// 		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
// 		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), EV_MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
// 
// 		return FALSE;
//     }
	
	CKdvDomainInfo tDomainInfo;
	s8 achDomainName[OPERATION_MAXLEN_DOMAINNAME+1]   = {0};
	s8 achDomainGUID[OPERATION_MAXLEN_GUID+1]   = {0};
	s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1]  = {0};
	s8 achSDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};
	
	while ( !ptDBEngine->IsEof() )
	{
		tDomainInfo.SetNull();
		memset( achDomainName, 0, sizeof( achDomainName ) );
		memset( achDomainGUID, 0, sizeof( achDomainGUID ) );
		memset( achDomainMOID, 0, sizeof( achDomainMOID ) );
		memset( achSDomainMOID, 0, sizeof( achSDomainMOID ) );

		// 域名称
		ptDBEngine->GetFieldValue( USERDOMAINNAME, achDomainName, sizeof( achDomainName ) );
		// 域GUID
		ptDBEngine->GetFieldValue( DOMAIN_GUID, achDomainGUID, sizeof( achDomainGUID ) );
		// 域MOID
		ptDBEngine->GetFieldValue( USERDOMAINMOID, achDomainMOID, sizeof( achDomainMOID ) );
		// 上级域MOID
		ptDBEngine->GetFieldValue( PLATFORMDOMAINMOID, achSDomainMOID, sizeof( achSDomainMOID ) );
		
		u32 dw192_1080P_ConfNumLimit = 0;
		u32 dw192_720P_ConfNumLimit = 0;
		u32 dw64_1080P_ConfNumLimit = 0;
		u32 dw64_720P_ConfNumLimit = 0;
		u32 dw32_1080P_ConfNumLimit = 0;
		u32 dw32_720P_ConfNumLimit = 0;
		u32 dw8_1080P_ConfNumLimit = 0;
		u32 dw8_720P_ConfNumLimit = 0;
		ptDBEngine->GetFieldValue(APDB_192_1080P_CONFNUM_LIMIT, dw192_1080P_ConfNumLimit);
		//ptDBEngine->GetFieldValue(APDB_192_720P_CONFNUM_LIMIT, dw192_720P_ConfNumLimit);
		//ptDBEngine->GetFieldValue(APDB_64_1080P_CONFNUM_LIMIT, dw64_1080P_ConfNumLimit);
		//ptDBEngine->GetFieldValue(APDB_64_720P_CONFNUM_LIMIT, dw64_720P_ConfNumLimit);
		//ptDBEngine->GetFieldValue(APDB_32_1080P_CONFNUM_LIMIT, dw32_1080P_ConfNumLimit);
		//ptDBEngine->GetFieldValue(APDB_32_720P_CONFNUM_LIMIT, dw32_720P_ConfNumLimit);
		ptDBEngine->GetFieldValue(APDB_8_1080P_CONFNUM_LIMIT, dw8_1080P_ConfNumLimit);
		//ptDBEngine->GetFieldValue(APDB_8_720P_CONFNUM_LIMIT, dw8_720P_ConfNumLimit);
		u32 dwUserDomainMaxLargeConfNum = dw192_1080P_ConfNumLimit
			+ dw192_720P_ConfNumLimit
			+ dw64_1080P_ConfNumLimit
			+ dw64_720P_ConfNumLimit
			+ dw32_1080P_ConfNumLimit
			+ dw32_720P_ConfNumLimit;
		u32 dwUserDomainMaxSmallConfNum = dw8_1080P_ConfNumLimit + dw8_720P_ConfNumLimit;

		u8 byEnableH265 = 0;
		ptDBEngine->GetFieldValue("enableH265", byEnableH265);
		u8 byEnableHDMeeting = 0;
		ptDBEngine->GetFieldValue("enableHDMeeting", byEnableHDMeeting);
		 
		tDomainInfo.SetWholeName( achDomainName );
		tDomainInfo.SetDomainGUID( achDomainGUID );
		tDomainInfo.SetSDomainMOID( achSDomainMOID );
		tDomainInfo.SetUsedFlag( TRUE ) ;
		tDomainInfo.SetDomainLevel( CKdvDomainInfo::em_KdvUserDomain );
		tDomainInfo.SetDomainMOID( achDomainMOID );
		tDomainInfo.SetMaxSmallConfNum(dwUserDomainMaxSmallConfNum);
		tDomainInfo.SetMaxLargeConfNum(dwUserDomainMaxLargeConfNum);
		tDomainInfo.SetEnableH265(byEnableH265);
		tDomainInfo.SetEnableHDMeeting(byEnableHDMeeting);

		g_cMauVCApp.AddDomainInfo( tDomainInfo );
		if (!g_cMauVCApp.SetMaxDomainConfNum(achDomainMOID,
			dwUserDomainMaxSmallConfNum, dwUserDomainMaxLargeConfNum))
		{
			LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[%s]SetMaxDomainConfNum Fail\n", __FUNCTION__);
			TMeetingDBFinishOpr tOpr(GETUSERDOMAININFOOPR, ERR_DB_SET_CBDATA);
			CMauMsg cNtfMsg;
			cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
			OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
			return FALSE;
		}
		
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetUserDomainInfo] Domain<%s>[%s], MOID[%s], SDomainMOID[%s] MaxConfNum[%d]\n", 
			tDomainInfo.GetWholeName(), tDomainInfo.GetDomainGUID(), tDomainInfo.GetDomainMOID(), tDomainInfo.GetSDomainMOID(), tDomainInfo.GetMaxConfNum() );
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetUserDomainInfo] EnableHDMeeting[%d], EnableH265[%d]\n",
			tDomainInfo.IsEnbaleHDMeeting(), tDomainInfo.IsEnableH265());
		// 移动到下一数据
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();
	
	//通知主控数据库数据加载完成
	TMeetingDBFinishOpr tOpr(GETUSERDOMAININFOOPR, wRet);
	
	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(TMeetingDBFinishOpr));
	
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[ProcGetPlatFormDomainInfo] post EV_MAU_FINISH_AP_OPR_NTF to MPCDVC %s, nRet = %d\n", 
		(OSP_OK == nRet)?"successful":"fail", nRet );

	
	return TRUE;
}

/*=============================================================================
函 数 名： ProcGetDomainHasXmpu
功    能： 获取域信息是否部署了xmpu服务器
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo		数据库任务信息
		   CDBEngine * ptDBEngine			数据库操作引擎
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/03/28  KDVP5.0		阮楠楠                  创建
=============================================================================*/
BOOL32 CAPInfoDBOpr::ProcGetDomainHasXmpu(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetDomainHasXmpu] Param err, retur!\n" );
		return FALSE;
	}
	
	const s8* szSQL = \
		" SELECT IFNULL(temp.mps_num, 0) AS mps_num, "PLATFORMDOMAIN"."PLATFORMDOMAINMOID	\
		" FROM "PLATFORMDOMAIN" LEFT JOIN("													\
		" SELECT count(*) AS mps_num, " PLATFORMDOMAINMOID								\
		" FROM server_info WHERE server_type = 261 AND enable = 1"						\
		" GROUP BY " PLATFORMDOMAINMOID ") AS temp"										\
		" ON " PLATFORMDOMAIN "." PLATFORMDOMAINMOID "= temp.platform_domain_moid";

	u16 wRet = ERR_DB_SUCCESS;	

	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, szSQL);
	
	// 获取服务器信息失败处理
	wRet = ptDBEngine->ExecSql(szSQL, TRUE);
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetDomainHasXmpu] Get ServerInfo failed, Error id = %u\n", wRet );
		//通知主控加载数据库失败
		TMeetingDBFinishOpr tOpr(GETUSERDOMAININFOOPR, wRet);
		CMauMsg cNtfMsg;
		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );

		return FALSE;
	}
	
	CKdvDomainInfo tPlatDomainInfo;
	s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1]  = {0};
// 	u8 byEnabled = 0;

	while ( !ptDBEngine->IsEof() )
	{
		tPlatDomainInfo.SetNull();
		memset( achDomainMOID, 0, sizeof( achDomainMOID ) );

		// 平台域MOID
		ptDBEngine->GetFieldValue(PLATFORMDOMAINMOID, achDomainMOID, sizeof(achDomainMOID));
		s32 dwMpsNum = 0;
		ptDBEngine->GetFieldValue("mps_num", dwMpsNum);
		
		//存在mps服务器，则没有xmpu。MPS服务器对应ap数据库中类型值为261
		// 根据moid与平台域信息进行比较，相同的域则设置对应的平台域和用户域hasxmpu字段
		if ( g_cMauVCApp.GetDomainInfo( achDomainMOID, tPlatDomainInfo ) )
		{	
			tPlatDomainInfo.SetHasXmpuServer(!(dwMpsNum > 0));
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[ProcGetDomainHasXmpu] Is Has XMPU: %d\n", tPlatDomainInfo.HasXmpuServer());
			g_cMauVCApp.ModDomainInfo(tPlatDomainInfo);
		}
		//根据平台域Domain修改用户域Xmpu的标识
		g_cMauVCApp.ModDomainHasXmpuInfoBySDomainMoid( achDomainMOID, !(dwMpsNum > 0));
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[ProcGetDomainHasXmpu] Get MpsNum is %d\n", dwMpsNum);
		// 移动到下一数据
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();
	
	//通知主控数据库数据加载完成
	TMeetingDBFinishOpr tOpr(GETDOMAINHASXMPU, wRet);
	
	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(TMeetingDBFinishOpr));
	
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
	
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[ProcGetDomainHasXmpu] post EV_MAU_FINISH_AP_OPR_NTF to MPCDVC %s, nRet = %d\n",
		(OSP_OK == nRet)?"successful":"fail", nRet );

	
	return TRUE;
}

BOOL32 CAPInfoDBOpr::ProcGetPlatformIsRent(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine)
{
	if (NULL == ptDBEngine || NULL == ptTaskInfo)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_APDB, "[%s] Param err, return!\n", __FUNCTION__);
		return FALSE;
	}

	u32 dwRet = ptDBEngine->ExecSql("select cfg_value from t_sys_cfg where cfg_key = \"system.mode\"", TRUE);
	if (ERR_DB_SUCCESS == dwRet && !ptDBEngine->IsEof())
	{
		u8 byServerRent = 0;
		ptDBEngine->GetFieldValue("cfg_value", byServerRent);
		g_cMauVCApp.SetPlatformIsRent(byServerRent == 0 ? TRUE : FALSE);
		LogPrint(LOG_LVL_ERROR, MID_MAU_APDB, "[%s]Platform rent value: %u\n", __FUNCTION__, byServerRent);
	}
	else if (ptDBEngine->IsEof())
	{
		dwRet = ERR_DB_SET_CBDATA;
		LogPrint(LOG_LVL_ERROR, MID_MAU_APDB, "[%s]no data\n", __FUNCTION__);
	}
	CMauMsg cNtfMsg;
	TMeetingDBFinishOpr tOpr(GETPLATFORMISRENT, dwRet);
	cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg));
	return TRUE;
}

BOOL32 CAPInfoDBOpr::ProcGetHduMoidAndUserMoid(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine)
{
	if (NULL == ptDBEngine || NULL == ptTaskInfo)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetHduMoidAndUserMoid] Param err, return!\n");
		return FALSE;
	}

	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = { 0 };
	//形成SQL
	sprintf(szSql, "select server_moid, user_domain_moid from %s where %s in(341,342,343,344,325)AND %s = 1", SERVERINFO, SERVERTYPE, USEDENABLE);

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "%s.\n", szSql);

	// 获取模板信息失败处理
	wRet = ptDBEngine->ExecSql(szSql, TRUE);
	if (ERR_DB_SUCCESS != wRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetHduMoidAndUserMoid] Get HduMoidAndUsermoid failed, Error id = %u\n", wRet);
		//通知主控加载数据库失败
		TMeetingDBFinishOpr tOpr(GETHDUMOIDANDUSERDOMAINMOID, wRet);
		CMauMsg cNtfMsg;
		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
		OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg));

		return FALSE;
	}

	s8 achHduMoid[OPERATION_MAXLEN_APMOID + 1] = { 0 };
	s8 achHduUserDomainMoid[OPERATION_MAXLEN_APMOID + 1] = { 0 };

	while (!ptDBEngine->IsEof())
	{
		memset(achHduMoid, 0, sizeof(achHduMoid));
		memset(achHduUserDomainMoid, 0, sizeof(achHduUserDomainMoid));

		// HDUMOID
		ptDBEngine->GetFieldValue(SERVERMOID, achHduMoid, sizeof(achHduMoid));
		// 用户域MOID
		ptDBEngine->GetFieldValue(USERDOMAINMOID, achHduUserDomainMoid, sizeof(achHduUserDomainMoid));

		g_cMauVCApp.AddModHduMoidUserDomainMoid(achHduMoid, achHduUserDomainMoid);

		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetHduMoidAndUserMoid] HduMoid: [%s], Belong UserdomainMoid: [%s]", achHduMoid, achHduUserDomainMoid);

		// 移动到下一数据
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();

	//通知主控数据库数据加载完成
	TMeetingDBFinishOpr tOpr(GETHDUMOIDANDUSERDOMAINMOID, wRet);

	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(TMeetingDBFinishOpr));

	int nRet = OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg));

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[ProcGetHduMoidAndUserMoid] post EV_MAU_FINISH_AP_OPR_NTF to MPCDVC %s, nRet = %d\n",
		(OSP_OK == nRet) ? "successful" : "fail", nRet);


	return TRUE;
}
/*=============================================================================
函 数 名： ProcGetMtInfo
功    能： 
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo		数据库任务信息
		   CDBEngine * ptDBEngine			数据库操作引擎
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
=============================================================================*/
BOOL32 CAPInfoDBOpr::ProcGetMtInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetMtInfo] Param err, retur!\n" );
		return FALSE;
	}
	
	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	//形成SQL
	sprintf( szSql, "select * from %s where %s = %d", USERINFO, USEDENABLE, 1 );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "%s.\n", szSql );
	
	// 获取模板信息失败处理
	wRet = ptDBEngine->ExecSql(szSql, TRUE);
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetMtInfo]  GetMtInfo failed, Error id = %u\n", wRet );
		//通知主控加载数据库失败
		TMeetingDBFinishOpr tOpr(GETMTINFOOPR, wRet);
		CMauMsg cNtfMsg;
		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );

		return FALSE;
	}
	
// 	if ( ptDBEngine->GetCount() == 0 )
//     {
//         LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetMtInfo] GetMtInfo result set is 0\n" );
// 		//通知主控加载数据库失败
// 		TMeetingDBFinishOpr tOpr(GETMTINFOOPR, wRet);
// 		CMauMsg cNtfMsg;
// 		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
// 		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), EV_MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
// 
// 		return FALSE;
//     }
	
	CMauMtInfo tMtInfo;
	s8 achDomainGUID[OPERATION_MAXLEN_GUID+1]  = {0};
	s8 achE164[MAXLEN_E164+1]   = {0};
	s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};
	s8 achMOID[OPERATION_MAXLEN_APMOID + 1] = { 0 };
	u16 wMtType = 0;
	u16 wLimited = 0;
	
	while ( !ptDBEngine->IsEof() )
	{
		tMtInfo.Init();
		memset( achDomainGUID, 0, sizeof( achDomainGUID ) );
		memset( achE164, 0, sizeof( achE164 ) );
		memset( achDomainMOID, 0, sizeof( achDomainMOID ) );
		wMtType = 0;
		wLimited = 0;

		// 域GUID
		//ptDBEngine->GetFieldValue( DOMAIN_GUID, achDomainGUID, sizeof( achDomainGUID ) );
	
		// 终端MOID
		ptDBEngine->GetFieldValue( USERMOID, achMOID, sizeof(achMOID) );
		// 域MOID
		ptDBEngine->GetFieldValue( USERDOMAINMOID, achDomainMOID, sizeof( achDomainMOID ) );
		// E164
		ptDBEngine->GetFieldValue( MTE164NO, achE164, sizeof( achE164 ) );
		// 终端类型
		ptDBEngine->GetFieldValue( MTTYPE, wMtType );
		// 受限终端
		ptDBEngine->GetFieldValue( MTLIMITED, wLimited );
		
		tMtInfo.SetMOID( achMOID );
		tMtInfo.SetDomainMOID( achDomainMOID );
		tMtInfo.SetE164( achE164 );
		tMtInfo.SetMtType( wMtType );
		BOOL32 bLimited = ( 1 == wLimited ) ? TRUE : FALSE;
		tMtInfo.SetLimitedFlag( bLimited );
		
		g_cMauVCApp.AddMtInfo( tMtInfo );
		
		LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetMtInfo] E164NO<%s> DomainMOID<%s> MtType<%d>\n", 
			tMtInfo.GetE164(), tMtInfo.GetDomainMOID(), tMtInfo.GetMtType() );
		
		// 移动到下一数据
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();
	
	//通知主控数据库数据加载完成
	TMeetingDBFinishOpr tOpr(GETMTINFOOPR, wRet);
	
	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(TMeetingDBFinishOpr));
	
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[ProcGetPlatFormDomainInfo] post EV_MAU_FINISH_AP_OPR_NTF to MPCDVC %s, nRet = %d\n", 
		(OSP_OK == nRet)?"successful":"fail", nRet );

	
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
BOOL32 CAPInfoDBOpr::DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine)
{
	//参数校验
	if( NULL == ptTaskInfo || NULL == ptDBEngine )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::DBOprRegEntry] ERROR: param is NULL!\n" );
		return FALSE;
	}

	BOOL32 bRet = FALSE;

	switch( ptTaskInfo->GetTransactionID() )
	{
	case GETSERVDOMAININFOOPR:
		bRet = ProcGetServDomainInfo(ptTaskInfo, ptDBEngine);
		break;
	case GETPLATFORMDOMAININFOOPR:
		bRet = ProcGetPlatFormDomainInfo(ptTaskInfo, ptDBEngine);
		break;
	case GETUSERDOMAININFOOPR:
		bRet = ProcGetUserDomainInfo(ptTaskInfo, ptDBEngine);
		break;
	case GETMTINFOOPR:
		bRet = ProcGetMtInfo(ptTaskInfo, ptDBEngine);
		break;
	case GETDOMAINHASXMPU:
		bRet = ProcGetDomainHasXmpu(ptTaskInfo, ptDBEngine);
		break;
	case PRINT_CONNECT_STATUS:
		bRet = PrintStatus(ptDBEngine);
		break;
	case GETHDUMOIDANDUSERDOMAINMOID:
		bRet = ProcGetHduMoidAndUserMoid(ptTaskInfo, ptDBEngine);
		break;
	case GETPLATFORMISRENT:
		bRet = ProcGetPlatformIsRent(ptTaskInfo, ptDBEngine);
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
const s8 * CAPInfoDBOpr::GetDBOprNameByTransId( u32 dwTransId )
{
    const s8 * pchDBOprName = NULL;

	switch ( dwTransId )
	{
	case CLEARDBDATAOPR:
		pchDBOprName = "ClearDBDataOpr";
		break;
	case GETSERVDOMAININFOOPR:
		pchDBOprName = "GetServDomainOpr";
		break;
	case GETPLATFORMDOMAININFOOPR:
		pchDBOprName = "GetPlatFormDomainOpr";
		break;
	case GETUSERDOMAININFOOPR:
		pchDBOprName = "GetUserDomainOpr";
		break;
	case GETMTINFOOPR:
		pchDBOprName = "GetMtInfoOpr";
		break;
	case GETDOMAINHASXMPU:
		pchDBOprName = "GetDomainHasXmpuOpr";
		break;
	case PRINT_CONNECT_STATUS:
		pchDBOprName = "PrintConnectStatus";
		break;
	case GETHDUMOIDANDUSERDOMAINMOID:
		pchDBOprName = "GetHduMoidAndUserDomainMoidOpr";
		break;
	default:
		pchDBOprName = NULL;
		break;
	}

	return pchDBOprName;
}
