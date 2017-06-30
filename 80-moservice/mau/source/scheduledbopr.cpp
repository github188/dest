/*****************************************************************************
ģ����      : MAU
�ļ���      : scheduledconfinfodbopr.h
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
#include "mauvc.h"
#include "scheduledbopr.h"
#include "evmau.h"
#include "maustruct.h"
#include "upussn.h"

 extern CMauVCApp g_cMauVCApp;

/*=============================================================================
�� �� ���� CScheduledConfInfoDBOpr
��    �ܣ� ԤԼ������ϸ��Ϣ���ݿ�����๹�캯��
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
=============================================================================*/
CScheduledConfInfoDBOpr::CScheduledConfInfoDBOpr() : CAsynDBOpr()
{}

/*=============================================================================
�� �� ���� ~CScheduledConfInfoDBOpr
��    �ܣ� ԤԼ������ϸ��Ϣ���ݿ��������������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
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
�� �� ���� AddScheduledConfInfo
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
	s8  achDomainMOID[ OPERATION_MAXLEN_MOID+1 ] = { 0 };	//���MOID
	s8	achDomainName[OPERATION_MAXLEN_DOMAINNAME+1] = {0};
	s8	achScheduledConfData[MAXLEN_JSONBUF+1] = {0};
	s8  achStartTime[ 20+1 ] = { 0 };
	s8  achEndTime[ 20+1 ] = { 0 };
	TKdvTime  tStartTime;						    //��ʼʱ��

	while( !ptDBEngine->IsEof() )
	{
		nMeetingID     = 0;

		memset( achConfName,   0, sizeof(achConfName) );
		memset( achConfE164,   0, sizeof(achConfE164) );
		memset( achDomainGUID, 0, sizeof(achDomainGUID) );
		memset( achDomainMOID, 0, sizeof(achDomainMOID) );
		memset( achStartTime,  0, sizeof(achStartTime) );
		memset( achEndTime,    0, sizeof(achEndTime) );

		//ԤԼ����E164
		ptDBEngine->GetFieldValue( TCONFE164, achConfE164, sizeof(achConfE164) );
		//ԤԼ����MeetingId
		ptDBEngine->GetFieldValue( TMEETINGID, nMeetingID );
		//ԤԼ��������
		ptDBEngine->GetFieldValue( TCONFNAME, achConfName, sizeof(achConfName) );
		//ԤԼ��������������
		ptDBEngine->GetFieldValue( TDOMAINNAME, achDomainName, sizeof(achDomainName) );
		//ԤԼ����������GUID
		ptDBEngine->GetFieldValue( TDOMAINGUID, achDomainGUID, sizeof(achDomainGUID) );
		//ԤԼ����������MOID
		ptDBEngine->GetFieldValue( TDOMAINMOID, achDomainMOID, sizeof(achDomainMOID) );
		//��ʼʱ��
		ptDBEngine->GetFieldValue( TSTARTTIME, achStartTime, sizeof(achStartTime) );
		//����ʱ��
		ptDBEngine->GetFieldValue( TENDTIME, achEndTime, sizeof(achEndTime) );
		//ԤԼ������ϸ��Ϣ
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
		// �ն��б�
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

	//ֻΪ����sp2, ������������֮��ɾ�� 
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
�� �� ���� ProcPrintConnectStatus
��    �ܣ� ��ӡ����״̬
�㷨ʵ�֣�
ȫ�ֱ�����
��    ���� TDBOprTaskMsgItem & tDBMsg
�� �� ֵ�� u8
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
=============================================================================*/
BOOL32 CScheduledConfInfoDBOpr::ProcPrintConnectStatus(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	OspPrintf( TRUE, FALSE, "scheduled DB connect status = %d.\n", CheckDBConnectState(ptDBEngine) );

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
BOOL32 CScheduledConfInfoDBOpr::DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine)
{
	//����У��
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