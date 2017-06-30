/*****************************************************************************
ģ����      : MAU
�ļ���      : apinfodbopr.h
����ʱ��    : 2013�� 12�� 05��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/12/05  KDVP2.6     ��Ҳ        ����
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
�� �� ���� CAPInfoDBOpr
��    �ܣ� AP���ݿ�����๹�캯��
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
=============================================================================*/
CAPInfoDBOpr::CAPInfoDBOpr() : CAsynDBOpr()
{}

/*=============================================================================
�� �� ���� ~CAPInfoDBOpr
��    �ܣ� AP���ݿ��������������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
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
�� �� ���� GetServDomainInfo
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
=============================================================================*/
u8 CAPInfoDBOpr::GetServDomainInfo()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETSERVDOMAININFOOPR );
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return 0;
}


/*=============================================================================
�� �� ���� GetPlatFormDomainInfo
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
=============================================================================*/
u8 CAPInfoDBOpr::GetPlatFormDomainInfo()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETPLATFORMDOMAININFOOPR );
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return 0;
}

/*=============================================================================
�� �� ���� GetUserDomainInfo
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
=============================================================================*/
u8 CAPInfoDBOpr::GetUserDomainInfo()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETUSERDOMAININFOOPR);
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return 0;
}

/*=============================================================================
�� �� ���� GetMtInfo
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
=============================================================================*/
u8 CAPInfoDBOpr::GetMtInfo()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETMTINFOOPR);
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return 0;
}

/*=============================================================================
�� �� ���� GetDomainHasXmpu
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/03/28  KDVP5.0     ����                  ����
=============================================================================*/
u8 CAPInfoDBOpr::GetDomainHasXmpu()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)GETDOMAINHASXMPU );
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return 0;
}

/*=============================================================================
�� �� ���� PrintConnectStatus
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/25  KDVP2.6     ����                    ����
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
�� �� ���� GetServDomainInfo
��    �ܣ� ��ȡ��������Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo		���ݿ�������Ϣ
		   CDBEngine * ptDBEngine			���ݿ��������
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
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
	//�γ�SQL
	sprintf( szSql, "select * from %s where %s = %d", SERVDOMAIN, USEDENABLE, 1 );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[ProcGetServDomainInfo]%s.\n", szSql );
	
	// ��ȡap����Ϣʧ�ܴ���
	wRet = ptDBEngine->ExecSql(szSql, TRUE);
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetServDomainInfo] Get ServDomainInfo failed! Error id = %u\n", wRet );
		//֪ͨ���ؼ������ݿ�ʧ��
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

		// ������
		ptDBEngine->GetFieldValue( SERVDOMAINNAME, achDomainName, sizeof( achDomainName ) );
		// ��GUID
		ptDBEngine->GetFieldValue( DOMAIN_GUID, achDomainGUID, sizeof( achDomainGUID ) );
		// ��MOID
	    ptDBEngine->GetFieldValue( SERVDOMAINMOID, achDomainMOID, sizeof( achDomainMOID ) );

		tDomainInfo.SetWholeName( achDomainName );
		tDomainInfo.SetDomainGUID( achDomainGUID );
		tDomainInfo.SetUsedFlag( TRUE ) ;
		tDomainInfo.SetDomainLevel( CKdvDomainInfo::em_KdvTopLevelDomain );
		tDomainInfo.SetDomainMOID( achDomainMOID );

		g_cMauVCApp.AddDomainInfo( tDomainInfo );
		
		LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetServDomainInfo] Domain<%s>[%s], MOID[%s]\n", 
			tDomainInfo.GetWholeName(), tDomainInfo.GetDomainGUID(), tDomainInfo.GetDomainMOID() );

		// �ƶ�����һ����
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();

	//֪ͨ�������ݿ����ݼ������
	TMeetingDBFinishOpr tOpr(GETSERVDOMAININFOOPR, wRet);
	
	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody(&tOpr, sizeof(TMeetingDBFinishOpr));
	
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[ProcGetServDomainInfo] post EV_MAU_FINISH_AP_OPR_NTF to MPCDVC %s, nRet = %d\n", 
		(OSP_OK == nRet)?"successful":"fail", nRet );

	return TRUE;
}

/*=============================================================================
�� �� ���� ProcGetPlatFormDomainInfo
��    �ܣ� ��ȡƽ̨����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo		���ݿ�������Ϣ
		   CDBEngine * ptDBEngine			���ݿ��������
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
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
	//�γ�SQL
	sprintf( szSql, "select * from %s", PLATFORMDOMAIN );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "%s.\n", szSql );
	
	// ��ȡģ����Ϣʧ�ܴ���
	wRet = ptDBEngine->ExecSql( szSql, TRUE );
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetPlatFormDomainInfo] Get PlatFormDomainInfo failed, Error id = %u\n", wRet );
		//֪ͨ���ؼ������ݿ�ʧ��
		TMeetingDBFinishOpr tOpr(GETPLATFORMDOMAININFOOPR, wRet);
		CMauMsg cNtfMsg;
		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
		
		return FALSE;
	}
	
// 	if ( ptDBEngine->GetCount() == 0 )
//     {
//         OspPrintf( TRUE, FALSE, "[CAPInfoDBOpr::ProcGetPlatFormDomainInfo] Get PlatFormDomainInfo result set is 0\n" );
// 		//֪ͨ���ؼ������ݿ�ʧ��
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
		
		// ������
		ptDBEngine->GetFieldValue( PLATFORMDOMAINNAME, achDomainName, sizeof( achDomainName ) );
		// ��GUID
		ptDBEngine->GetFieldValue( DOMAIN_GUID, achDomainGUID, sizeof( achDomainGUID ) );
		// ��MOID
		ptDBEngine->GetFieldValue( PLATFORMDOMAINMOID, achDomainMOID, sizeof( achDomainMOID ) );
		// �ϼ���MOID
		ptDBEngine->GetFieldValue( SERVDOMAINMOID, achSDomainMOID, sizeof( achSDomainMOID ) );
		// ƽ̨��ǰ׺
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
		
		// �ƶ�����һ����
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();

	//֪ͨ�������ݿ����ݼ������
	TMeetingDBFinishOpr tOpr(GETPLATFORMDOMAININFOOPR, wRet);

	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(TMeetingDBFinishOpr));

	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );

	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[ProcGetPlatFormDomainInfo] post EV_MAU_FINISH_AP_OPR_NTF to MPCDVC %s, nRet = %d\n", 
		(OSP_OK == nRet)?"successful":"fail", nRet );

	return TRUE;
}

/*=============================================================================
�� �� ���� ProcGetUserDomainInfo
��    �ܣ� ��ȡ�û�����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo		���ݿ�������Ϣ
		   CDBEngine * ptDBEngine			���ݿ��������
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
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
	//�γ�SQL
	//sprintf( szSql, "select * from %s where %s = %d", USERDOMAIN, USERDOMAINUSEDFLAG, 1 );
	sprintf(szSql, "SELECT *, count(CASE privilege_key WHEN 'enableHDMeeting' THEN 'enableHDMeeting' END) AS enableHDMeeting,count(CASE privilege_key WHEN 'enableH265' THEN 'enableH265' END) AS enableH265 \
		FROM user_domain LEFT JOIN user_domain_privilege_data ON user_domain.user_domain_moid = user_domain_privilege_data.user_domain_moid where used_flag = 1 \
		GROUP BY user_domain.user_domain_moid ");
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_APDB, "%s.\n", szSql );
	
	// ��ȡģ����Ϣʧ�ܴ���
	wRet = ptDBEngine->ExecSql( szSql, TRUE );
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetUserDomainInfo] Get UserDomainInfo failed, Error id = %u\n", wRet );
		//֪ͨ���ؼ������ݿ�ʧ��
		TMeetingDBFinishOpr tOpr(GETUSERDOMAININFOOPR, wRet);
		CMauMsg cNtfMsg;
		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );

		return FALSE;
	}
	
// 	if ( ptDBEngine->GetCount() == 0 )
//     {
//         LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetUserDomainInfo] Get UserDomainInfo result set is 0\n" );
// 		//֪ͨ���ؼ������ݿ�ʧ��
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

		// ������
		ptDBEngine->GetFieldValue( USERDOMAINNAME, achDomainName, sizeof( achDomainName ) );
		// ��GUID
		ptDBEngine->GetFieldValue( DOMAIN_GUID, achDomainGUID, sizeof( achDomainGUID ) );
		// ��MOID
		ptDBEngine->GetFieldValue( USERDOMAINMOID, achDomainMOID, sizeof( achDomainMOID ) );
		// �ϼ���MOID
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
		// �ƶ�����һ����
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();
	
	//֪ͨ�������ݿ����ݼ������
	TMeetingDBFinishOpr tOpr(GETUSERDOMAININFOOPR, wRet);
	
	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(TMeetingDBFinishOpr));
	
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[ProcGetPlatFormDomainInfo] post EV_MAU_FINISH_AP_OPR_NTF to MPCDVC %s, nRet = %d\n", 
		(OSP_OK == nRet)?"successful":"fail", nRet );

	
	return TRUE;
}

/*=============================================================================
�� �� ���� ProcGetDomainHasXmpu
��    �ܣ� ��ȡ����Ϣ�Ƿ�����xmpu������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo		���ݿ�������Ϣ
		   CDBEngine * ptDBEngine			���ݿ��������
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/03/28  KDVP5.0		����                  ����
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
	
	// ��ȡ��������Ϣʧ�ܴ���
	wRet = ptDBEngine->ExecSql(szSQL, TRUE);
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetDomainHasXmpu] Get ServerInfo failed, Error id = %u\n", wRet );
		//֪ͨ���ؼ������ݿ�ʧ��
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

		// ƽ̨��MOID
		ptDBEngine->GetFieldValue(PLATFORMDOMAINMOID, achDomainMOID, sizeof(achDomainMOID));
		s32 dwMpsNum = 0;
		ptDBEngine->GetFieldValue("mps_num", dwMpsNum);
		
		//����mps����������û��xmpu��MPS��������Ӧap���ݿ�������ֵΪ261
		// ����moid��ƽ̨����Ϣ���бȽϣ���ͬ���������ö�Ӧ��ƽ̨����û���hasxmpu�ֶ�
		if ( g_cMauVCApp.GetDomainInfo( achDomainMOID, tPlatDomainInfo ) )
		{	
			tPlatDomainInfo.SetHasXmpuServer(!(dwMpsNum > 0));
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[ProcGetDomainHasXmpu] Is Has XMPU: %d\n", tPlatDomainInfo.HasXmpuServer());
			g_cMauVCApp.ModDomainInfo(tPlatDomainInfo);
		}
		//����ƽ̨��Domain�޸��û���Xmpu�ı�ʶ
		g_cMauVCApp.ModDomainHasXmpuInfoBySDomainMoid( achDomainMOID, !(dwMpsNum > 0));
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[ProcGetDomainHasXmpu] Get MpsNum is %d\n", dwMpsNum);
		// �ƶ�����һ����
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();
	
	//֪ͨ�������ݿ����ݼ������
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
	//�γ�SQL
	sprintf(szSql, "select server_moid, user_domain_moid from %s where %s in(341,342,343,344,325)AND %s = 1", SERVERINFO, SERVERTYPE, USEDENABLE);

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "%s.\n", szSql);

	// ��ȡģ����Ϣʧ�ܴ���
	wRet = ptDBEngine->ExecSql(szSql, TRUE);
	if (ERR_DB_SUCCESS != wRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetHduMoidAndUserMoid] Get HduMoidAndUsermoid failed, Error id = %u\n", wRet);
		//֪ͨ���ؼ������ݿ�ʧ��
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
		// �û���MOID
		ptDBEngine->GetFieldValue(USERDOMAINMOID, achHduUserDomainMoid, sizeof(achHduUserDomainMoid));

		g_cMauVCApp.AddModHduMoidUserDomainMoid(achHduMoid, achHduUserDomainMoid);

		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetHduMoidAndUserMoid] HduMoid: [%s], Belong UserdomainMoid: [%s]", achHduMoid, achHduUserDomainMoid);

		// �ƶ�����һ����
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();

	//֪ͨ�������ݿ����ݼ������
	TMeetingDBFinishOpr tOpr(GETHDUMOIDANDUSERDOMAINMOID, wRet);

	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(TMeetingDBFinishOpr));

	int nRet = OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg));

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[ProcGetHduMoidAndUserMoid] post EV_MAU_FINISH_AP_OPR_NTF to MPCDVC %s, nRet = %d\n",
		(OSP_OK == nRet) ? "successful" : "fail", nRet);


	return TRUE;
}
/*=============================================================================
�� �� ���� ProcGetMtInfo
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo		���ݿ�������Ϣ
		   CDBEngine * ptDBEngine			���ݿ��������
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
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
	//�γ�SQL
	sprintf( szSql, "select * from %s where %s = %d", USERINFO, USEDENABLE, 1 );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "%s.\n", szSql );
	
	// ��ȡģ����Ϣʧ�ܴ���
	wRet = ptDBEngine->ExecSql(szSql, TRUE);
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetMtInfo]  GetMtInfo failed, Error id = %u\n", wRet );
		//֪ͨ���ؼ������ݿ�ʧ��
		TMeetingDBFinishOpr tOpr(GETMTINFOOPR, wRet);
		CMauMsg cNtfMsg;
		cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(tOpr));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );

		return FALSE;
	}
	
// 	if ( ptDBEngine->GetCount() == 0 )
//     {
//         LogPrint( LOG_LVL_ERROR, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetMtInfo] GetMtInfo result set is 0\n" );
// 		//֪ͨ���ؼ������ݿ�ʧ��
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

		// ��GUID
		//ptDBEngine->GetFieldValue( DOMAIN_GUID, achDomainGUID, sizeof( achDomainGUID ) );
	
		// �ն�MOID
		ptDBEngine->GetFieldValue( USERMOID, achMOID, sizeof(achMOID) );
		// ��MOID
		ptDBEngine->GetFieldValue( USERDOMAINMOID, achDomainMOID, sizeof( achDomainMOID ) );
		// E164
		ptDBEngine->GetFieldValue( MTE164NO, achE164, sizeof( achE164 ) );
		// �ն�����
		ptDBEngine->GetFieldValue( MTTYPE, wMtType );
		// �����ն�
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
		
		// �ƶ�����һ����
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();
	
	//֪ͨ�������ݿ����ݼ������
	TMeetingDBFinishOpr tOpr(GETMTINFOOPR, wRet);
	
	CMauMsg cNtfMsg;
	cNtfMsg.SetMsgBody((u8*)&tOpr, sizeof(TMeetingDBFinishOpr));
	
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_AP_OPR_NTF, &cNtfMsg, sizeof(cNtfMsg) );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[ProcGetPlatFormDomainInfo] post EV_MAU_FINISH_AP_OPR_NTF to MPCDVC %s, nRet = %d\n", 
		(OSP_OK == nRet)?"successful":"fail", nRet );

	
	return TRUE;
}

/*=============================================================================
�� �� ���� DBOprReqEntry
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo	���ݿ�������Ϣ
		   CDBEngine * ptDBEngine       ���ݿ��������
		   BOOL32 bAccept               �����߳��Ƿ������ݿ����
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
=============================================================================*/
BOOL32 CAPInfoDBOpr::DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine)
{
	//����У��
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
�� �� ���� GetDBOprNameByTransId
��    �ܣ� ��ȡDB��������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u32 byTransId  ����ID
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
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
