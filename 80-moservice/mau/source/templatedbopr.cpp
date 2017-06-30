/*****************************************************************************
ģ����      : MAU
�ļ���      : teemplatedbopr.cpp
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
#include "templatedbopr.h"
#include "mauvc.h"
#include "evmau.h"
#include "mauconst.h"
#include "mauutility.h"

/*=============================================================================
�� �� ���� CTemplateDBOpr
��    �ܣ� ģ��洢���ݿ�����๹�캯��
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
CTemplateDBOpr::CTemplateDBOpr() : CAsynDBOpr()
{}

/*=============================================================================
�� �� ���� ~CTemplateDBOpr
��    �ܣ� ģ��洢���ݿ��������������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
CTemplateDBOpr::~CTemplateDBOpr()
{
	
}

const s8* CTemplateDBOpr::GetModuleName()
{
	return "CTemplateDBOpr";
}

u16 CTemplateDBOpr::GetModuleId()
{
	return MID_MAU_TEMPDB;
}

/*=============================================================================
�� �� ���� LoadDBData
��    �ܣ� �������ݿ�����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
void CTemplateDBOpr::LoadDBData( void )
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)LOADTEMPLATEOPR );

	PostOneMsgToDBOprTask( tDBMsg, TRUE );

	return ;
}

/*=============================================================================
�� �� ���� AddTemplate
��    �ܣ� �����ݿ������ģ����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskMsgItem & tDBMsg
�� �� ֵ�� u8 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
u8 CTemplateDBOpr::AddTemplate( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID( (u32)ADDTEMPLATEOPR );
	PostOneMsgToDBOprTask( tDBMsg, FALSE );

	return 0;
}


/*=============================================================================
�� �� ���� DelTemplate
��    �ܣ� ɾ�����ݿ��е�ģ����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskMsgItem & tDBMsg
�� �� ֵ�� u8
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
u8 CTemplateDBOpr::DelTemplate( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID( (u32)DELTEMPLATEOPR );	
	PostOneMsgToDBOprTask( tDBMsg, FALSE );

	return 0;
}

/*=============================================================================
�� �� ���� GetTemplateInfo
��    �ܣ� �����ݿ��л�ȡģ����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskMsgItem & tDBMsg
�� �� ֵ�� u8
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
u8 CTemplateDBOpr::GetTemplateInfo(TReqHeadInfo& tHeadInfo, const u16 wConfDuration)
{
	//��Ϊ��ݴ��Ὣ����duration������Ϊ�˱������ݿ������������һ����
	//�ն�ģ�崴��Ҳ����duration����
	TDBOprTaskMsgItem tDBTaskMsg;
	tDBTaskMsg.SetMsgBody(&tHeadInfo, sizeof(tHeadInfo));
	tDBTaskMsg.CatMsgBody(&wConfDuration, sizeof(wConfDuration));
	tDBTaskMsg.SetTransactionID( (u32)FINDTEMPLATEOPR );	
	PostOneMsgToDBOprTask( tDBTaskMsg, FALSE );
	return 0;
}

/*=============================================================================
�� �� ���� ModTemplate
��    �ܣ� �޸����ݿ��е�ģ����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskMsgItem & tDBMsg
�� �� ֵ�� u8
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
u8 CTemplateDBOpr::ModTemplate( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID( (u32)MODTEMPLATEOPR );
	PostOneMsgToDBOprTask( tDBMsg, FALSE );

	return 0;
}

/*=============================================================================
�� �� ���� PrintConnectStatus
��    �ܣ� ��ӡ����״̬
�㷨ʵ�֣�
ȫ�ֱ�����
��    ���� TDBOprTaskMsgItem & tDBMsg
�� �� ֵ�� u8
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
void CTemplateDBOpr::PrintConnectStatus()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID(PRINT_CONNECT_STATUS);
	PostOneMsgToDBOprTask(tDBMsg, FALSE);
}

/*=============================================================================
�� �� ���� DBOprReqEntry
��    �ܣ� MPCD���ݿ�������Ӧ������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo	���ݿ�������Ϣ
		   CDBEngine * ptDBEngine       ���ݿ��������
		   BOOL32 bAccept               �����߳��Ƿ������ݿ����
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
BOOL32 CTemplateDBOpr::DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine)
{
	//����У��
	if( NULL == ptTaskInfo || NULL == ptDBEngine )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_TEMPDB, "[CTemplateDBOpr::DBOprRegEntry] ERROR: param is NULL!\n" );
		return FALSE;
	}

	BOOL32 bRet = FALSE;

	switch( ptTaskInfo->GetTransactionID() )
	{
	case LOADTEMPLATEOPR:
		bRet = ProcLoadDBData( ptDBEngine );
		break;
	//����ģ��
	case ADDTEMPLATEOPR:
		bRet = ProcAddTemplate( ptTaskInfo, ptDBEngine );
		break;
	//��ȡģ����ϸ��Ϣ
	case FINDTEMPLATEOPR:
		bRet = ProcGetTemplate( ptTaskInfo, ptDBEngine );
		break;
	//�޸�ģ��
	case MODTEMPLATEOPR:
		bRet = ProcModTemplate( ptTaskInfo, ptDBEngine );
		break;
	//ɾ��ģ��
	case DELTEMPLATEOPR:
		bRet = ProcDelTemplate( ptTaskInfo, ptDBEngine );
		break;
	//��ӡ����״̬
	case PRINT_CONNECT_STATUS:
		bRet = PrintStatus(ptDBEngine);
		break;
	default:
		break;

	}

	return bRet;
}


/*=============================================================================
�� �� ���� ProcLoadDBData
��    �ܣ� �����ݿ��е�ģ����Ϣ���ص��ڴ���
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CDBEngine * ptDBEngine		���ݿ��������
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
BOOL32 CTemplateDBOpr::ProcLoadDBData( CDBEngine * ptDBEngine )
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine )
	{
		return bRet;
	}
	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	sprintf( szSql, "select %s,%s,%s,%s,%s,%s from %s", 
		TTEMPLATEKEY, TTEMPLATENAME, TDOMAINNAME, TDOMAINGUID, TDOMAINMOID, TTEMPLATEDATA, TTEMPLATEINFO );

	wRet = ptDBEngine->ExecSql( szSql, TRUE );
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_TEMPDB, "[CTemplateDBOpr::ProcLoadDBData] Load db data failed!\n" );
		//֪ͨ���ؼ������ݿ�ʧ��
		OspPost( MAKEIID( AID_MAU_MAUVC_APPID,CInstance::DAEMON ), MAU_FINISH_LOADDB_NTF, &wRet, sizeof(wRet) );
		return bRet;
	}
	else
	{
		bRet = TRUE;
	}

	s8  achTempName[MAXLEN_CONFNAME+1] = {0};
	s8  achTempKey[MAXLEN_CONFE164+1] = {0};
	s8	achDomainName[OPERATION_MAXLEN_DOMAINNAME+1] = {0};
	s8	achDomainGUID[OPERATION_MAXLEN_GUID+1] = {0};
	s8	achTempData[SERV_MSG_LEN+1] = {0};
	s8	achDomainMOID[OPERATION_MAXLEN_MOID+1];

	CTempCacheData atTemplateCacheData[10];
	u32 nIdx = 0;
	//�����Ǽ��� �������ݿ�����Ϊ��һ��
	while( !ptDBEngine->IsEof() )
	{
		memset( achTempKey, 0, sizeof(achTempKey) );
		memset( achTempName, 0, sizeof(achTempName) );
		memset( achDomainName, 0, sizeof(achDomainName) );
		memset( achDomainGUID, 0, sizeof(achDomainGUID) );
		memset( achTempData, 0, sizeof(achTempData));
		memset( achDomainMOID, 0, sizeof(achDomainMOID) );

		//ģ��E164
		ptDBEngine->GetFieldValue( TTEMPLATEKEY, achTempKey, sizeof(achTempKey) );
		//ģ������
		ptDBEngine->GetFieldValue( TTEMPLATENAME, achTempName, sizeof(achTempName) );
		//ģ������������
		ptDBEngine->GetFieldValue( TDOMAINNAME, achDomainName, sizeof(achDomainName) );
		//ģ��������GUID
		ptDBEngine->GetFieldValue( TDOMAINGUID, achDomainGUID, sizeof(achDomainGUID) );
		//ģ����ϸ��Ϣ
		ptDBEngine->GetFieldValue( TTEMPLATEDATA, achTempData, sizeof(achTempData) );
		//ģ��������Moid
		ptDBEngine->GetFieldValue( TDOMAINMOID, achDomainMOID, sizeof(achDomainMOID) );
		
		TConfInfo tConfInfo;
		TConfInfoEx tConfInfoEx;

		GetCreateConfParamFromJson(achTempData, tConfInfo);
		GetConfInfoExFromJson(achTempData, tConfInfoEx);

		TVideoStreamCap atVideoStreamCap[MAX_CONF_CAP_EX_NUM];
		u8 byCapNum = MAX_CONF_CAP_EX_NUM;
		tConfInfoEx.GetMainStreamCapEx(atVideoStreamCap, byCapNum);
		CTempCacheData& tTemplateCacheData = atTemplateCacheData[nIdx];
		tTemplateCacheData.SetConfE164( achTempKey );
		tTemplateCacheData.SetConfName( achTempName );
		tTemplateCacheData.SetDomainName( achDomainName );
		tTemplateCacheData.SetDomainGUID( achDomainGUID );
		tTemplateCacheData.SetBitRate(atVideoStreamCap[0].GetMaxBitRate());
		tTemplateCacheData.SetResolution(atVideoStreamCap[0].GetResolution());
		tTemplateCacheData.SetMediaType(atVideoStreamCap[0].GetMediaType());
		tTemplateCacheData.SetConfPsw(tConfInfo.GetConfPwd());
		tTemplateCacheData.SetSatDCastMode(tConfInfo.GetConfAttrb().IsSatDCastMode());
		tTemplateCacheData.SetPublicConf(tConfInfo.GetConfAttrb().GetPublicConf());
		tTemplateCacheData.SetDomainMOID( achDomainMOID );
		tTemplateCacheData.SetOpenMode(tConfInfo.GetConfAttrb().GetOpenMode());
		tTemplateCacheData.SetMixMode(tConfInfo.GetStatus().GetMixerMode());
		tTemplateCacheData.SetDuration(tConfInfo.GetDuration());
		tTemplateCacheData.SetEncryptMode(tConfInfo.GetConfAttrb().GetEncryptMode());
		tTemplateCacheData.SetDualMode(tConfInfo.GetConfAttrb().GetDualMode());
		tTemplateCacheData.SetHasXmpuSer(tConfInfo.GetConfAttrb().IsHasVmpModule() ? 1 : 0);
		tTemplateCacheData.SetConfNoDisturb(tConfInfo.GetConfAttrb().IsConfNoDisturb() ? 1 : 0);
		tTemplateCacheData.SetPortModeConf(tConfInfo.IsPortMediaConf());
		tTemplateCacheData.SetMaxJoinMtNum(tConfInfo.GetMaxJoinedMtNum());
	
		++nIdx;
		if (nIdx >= _countof(atTemplateCacheData))
		{
			OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_MAU_ADDTEMPLATE_NTF, (u8*)atTemplateCacheData, sizeof(atTemplateCacheData));
			nIdx = 0;
		}

		//�ƶ�����һ����
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();

	if (nIdx > 0)
	{
		OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_MAU_ADDTEMPLATE_NTF, (u8*)atTemplateCacheData, sizeof(CTempCacheData) * nIdx);
	}

	LogPrint( LOG_LVL_DETAIL, MID_MAU_TEMPDB, "[CTemplateDBOpr::ProcLoadDBData] Load db data successful! wRet = %u\n", wRet );
	//֪ͨ�������ݿ����ݼ������
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_LOADDB_NTF, &wRet, sizeof(wRet) );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_TEMPDB, "[CTemplateDBOpr::ProcLoadDBData] post EV_MAU_FINISH_LOADDB_NTF to MAUVC %s, nRet = %d\n", (OSP_OK == nRet)?"successful":"fail", nRet );

	return bRet;
}


/*=============================================================================
�� �� ���� ProcAddTemplate
��    �ܣ� �����ݿ������һ��ģ���¼
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo		���ݿ�������Ϣ
		   CDBEngine * ptDBEngine			���ݿ��������
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
BOOL32 CTemplateDBOpr::ProcAddTemplate(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		return FALSE;
	}
	
	const u8 * pMsg = ptTaskInfo->GetMsgBody();

	//����ͷ��Ϣ
	TReqHeadInfo tReqInfo;
	memcpy( (void*)&tReqInfo, pMsg, sizeof(tReqInfo) );
	pMsg += sizeof(TReqHeadInfo);

	CKdvDomainInfo cDomainInfo;
	memcpy( (void*)&cDomainInfo, pMsg, sizeof( cDomainInfo ) );
	pMsg += sizeof( CKdvDomainInfo );

	const s8* achJson = (const s8*)(pMsg);
	u32 dwPrintBuffLen = strlen(achJson);
	LogPrint( LOG_LVL_DETAIL, MID_MAU_TEMPDB, "[ProcAddTemplate]cm json len is %d, text is:.\n", dwPrintBuffLen );
	LongLogPrint(LOG_LVL_DETAIL, MID_MAU_TEMPDB, achJson, dwPrintBuffLen);
	LogPrint( LOG_LVL_DETAIL, MID_MAU_TEMPDB, "\n" );

	json_t_ptr pjRoot = NULL;
	LoadJson(achJson, dwPrintBuffLen, pjRoot, FALSE);

	const s8* pchConfName = "";
	GetConfNameFromJsonObj(pjRoot, &pchConfName);
	const s8* pchConfE164 = "";
	GetConfE164FromJsonObj(pjRoot, &pchConfE164);

	//ʱ��
	s8 achCreateTime[20+1] = {0};

	//����ʱ��
	TKdvTime tCurTime;
	time_t tiCurTime = ::time( NULL );
	tCurTime.SetTime( &tiCurTime );
	tCurTime.GetString( achCreateTime, sizeof(achCreateTime) );

	LogPrint(LOG_LVL_DETAIL, MID_MAU_TEMPDB, "[ProcAddTemplate] Start add template<%s> to db!\n", pchConfE164);

	u16 wRet = ERR_DB_SUCCESS;
	// �޸ĳ��ȣ������ն˶��˷Ų���
	s8  szSql[0x8000] = {0};


	//ģ���м���ѯ��Ϣ
	//�γ�SQL
	s8 achConfName[MAXLEN_CONFNAME * 2 + 1];
	u64 nReturn = ptDBEngine->EscapeString(achConfName, pchConfName);
	if (nReturn == 0)
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_CONFDB, "[ProcAddTemplate] EscapeStr failed!\n" );
	}

	s8 achJsonMsg[SERV_MSG_LEN * 2 + 1];
	nReturn = ptDBEngine->EscapeString(achJsonMsg, (s8*)pMsg);
	if (nReturn == 0)
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_CONFDB, "[ProcAddTemplate] EscapeStr failed!\n" );
	}

	sprintf( szSql, "insert into %s (%s,%s,%s,%s,%s,%s,%s,%s) values('%s','%s','%s','%s','%s','%s','%s','%s')",
		     TTEMPLATEINFO,TTEMPLATEKEY,TTEMPLATENAME,TDOMAINNAME,TDOMAINGUID,TDOMAINMOID,TCREATETIME,TLASTMODIFYTIME,TTEMPLATEDATA,
			 pchConfE164,
			 achConfName, 
			 cDomainInfo.GetWholeName(),
			 cDomainInfo.GetDomainGUID(), 
			 cDomainInfo.GetDomainMOID(),
			 achCreateTime, 
			 achCreateTime,
			 achJsonMsg
			 );

	LogPrint(LOG_LVL_DETAIL, MID_MAU_CONFDB, "SQL: %s\n", szSql);

	//ִ��SQL
	wRet = ptDBEngine->ExecSql(szSql);

	//�������֪ͨ
	CMauMsg cServMsg;
	tReqInfo.SetErrID(wRet);
	tReqInfo.SetConfE164(pchConfE164);
	cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_ADDTEMP_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
	
	return ( ERR_DB_SUCCESS == wRet ) ? TRUE : FALSE;
}


/*=============================================================================
�� �� ���� ProcDelTemplate
��    �ܣ� �����ݿ���ɾ��ģ����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo		���ݿ�������Ϣ
		   CDBEngine * ptDBEngine			���ݿ��������
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
BOOL32 CTemplateDBOpr::ProcDelTemplate(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		return FALSE;
	}
	
	if( ptTaskInfo->GetMsgBodyLen() < sizeof(TReqHeadInfo) )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_TEMPDB, "[ProcDelTemplate] ERROR: Wrong MsgBody Len<%d>!\n", ptTaskInfo->GetMsgBodyLen() );
		return FALSE;
	}

	const u8 * pbyMsg = ptTaskInfo->GetMsgBody();
	
	//����ͷ��Ϣ
	TReqHeadInfo tReqInfo;
	memcpy( (void*)&tReqInfo, pbyMsg, sizeof(tReqInfo) );
	pbyMsg += sizeof(TReqHeadInfo);

	// ֧������ɾ��ģ��
	CMauMsg cServMsg;
	CMauMsg cTmpMsg;
	cTmpMsg.SetMsgBody();
	CMauData::GetTemplateListFromJson( (s8*)pbyMsg, cServMsg );

	u16 wSqlRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	s8  achConfE164[MAXLEN_CONFE164+1] = {0};
	u16 wTemplateNum = *(u16*)cServMsg.GetMsgBody();

	u16 wIdx = 0;
	for ( wIdx = 0; wIdx < wTemplateNum; wIdx++ )
	{
		memset( szSql, 0,  sizeof( szSql ) );
		memset( achConfE164, 0, sizeof( achConfE164 ) );

		memcpy( achConfE164, ( cServMsg.GetMsgBody()+sizeof(u16)+wIdx*MAXLEN_CONFE164 ), MAXLEN_CONFE164 );

		LogPrint( LOG_LVL_DETAIL, MID_MAU_TEMPDB, "[ProcDelTemplate] Start delete template<%s> from db!\n", achConfE164 );

		//�γ�SQL
		sprintf( szSql, "delete from %s where %s = \"%s\"", TTEMPLATEINFO, TTEMPLATEKEY, achConfE164 );
		//ִ��SQL
		wSqlRet = ptDBEngine->ExecSql(szSql);
		if ( ERR_DB_SUCCESS != wSqlRet )
		{
			continue;
		}

		cTmpMsg.CatMsgBody( (u8*)achConfE164, sizeof( achConfE164 ) );
	}

	u16 wCount = cTmpMsg.GetMsgBodyLen()/sizeof( achConfE164 );
	
	//���ͳɹ����֪ͨ
	
	tReqInfo.SetErrID(wSqlRet);
	CMauMsg cDelMsg;
	cDelMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
	cDelMsg.CatMsgBody(&wCount, sizeof(u16));
	cDelMsg.CatMsgBody(cTmpMsg.GetMsgBody(), cTmpMsg.GetMsgBodyLen());
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_DELTEMP_NTF, cDelMsg.GetServMsg(), cDelMsg.GetServMsgLen() );
	return ( ERR_DB_SUCCESS == wSqlRet ) ? TRUE : FALSE;
}


/*=============================================================================
�� �� ���� ProcGetTemplate
��    �ܣ� �����ݿ��ȡģ�����ϸ��Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo
           CDBEngine * ptDBEngine
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
BOOL32 CTemplateDBOpr::ProcGetTemplate(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		return FALSE;
	}

	if (ptTaskInfo->GetMsgBodyLen() < sizeof(TReqHeadInfo))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_TEMPDB, "[ProcGetTemplate] ERROR: Wrong MsgBody Len<%d>!\n", ptTaskInfo->GetMsgBodyLen() );
		return FALSE;
	}
	//����ͷ��Ϣ
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)ptTaskInfo->GetMsgBody();

	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	//�γ�SQL
	sprintf( szSql, "select %s from %s where %s = \"%s\"", TTEMPLATEDATA, TTEMPLATEINFO, TTEMPLATEKEY, tReqInfo.GetConfE164() );
	
	// ��ȡģ����Ϣʧ�ܴ���
	wRet = ptDBEngine->ExecSql(szSql, TRUE);
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_TEMPDB, "[ProcGetTemplate] Get Temp %s failed! Error id = %u\n", tReqInfo.GetConfE164(), wRet );
		//���Ͳ�ѯ
		tReqInfo.SetErrID(wRet);
		CMauMsg cServMsg;
		cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_SEARCHTEMP_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
		return FALSE;
	}

	if (ptDBEngine->IsEof())
	{
        LogPrint( LOG_LVL_ERROR, MID_MAU_TEMPDB, "[ProcGetTemplate] Get Temp %s result set is 0\n", tReqInfo.GetConfE164() );
        //���Ͳ�ѯ
		tReqInfo.SetErrID(ERR_DB_NO_RESULESET);
        CMauMsg cServMsg;
		cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
        OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_SEARCHTEMP_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
		return FALSE;
    }
	
	s8	achTempData[MAXLEN_JSONBUF+1] = {0};
	
	//ģ����ϸ��Ϣ
	ptDBEngine->GetFieldValue(TTEMPLATEDATA, achTempData, sizeof(achTempData));
	ptDBEngine->GoOnProcRes();

	LogPrint( LOG_LVL_DETAIL, MID_MAU_TEMPDB, "[ProcGetTemplate] Get Temp %s successful!\n", tReqInfo.GetConfE164() );

	//���Ͳ�ѯ
	CMauMsg cServMsg;
	tReqInfo.SetErrID(ERR_DB_SUCCESS);
	cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
	u16 wConfDuration = *((u16*)((u8*)ptTaskInfo->GetMsgBody() + sizeof(TReqHeadInfo)));
	cServMsg.CatMsgBody((u8*)&wConfDuration, sizeof(wConfDuration));
	const u16 wLen = strlen(achTempData);
	cServMsg.CatMsgBody(&wLen, sizeof(wLen));
	cServMsg.CatMsgBody(achTempData, wLen);
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_SEARCHTEMP_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );

	return TRUE;
}

/*=============================================================================
�� �� ���� ProcModTemplate
��    �ܣ� �޸����ݿ��е�ģ����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo
		   CDBEngine * ptDBEngine
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
BOOL32 CTemplateDBOpr::ProcModTemplate(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		return FALSE;
	}
	
	const u8 * pMsg = ptTaskInfo->GetMsgBody();

	TReqHeadInfo tReqInfo;
	memcpy( (void*)&tReqInfo, pMsg, sizeof(tReqInfo) );
	pMsg += sizeof(TReqHeadInfo);

	TConfInfo tConfInfo;
	TConfInfoEx tConfInfoEx;

	GetCreateConfParamFromJson((s8*)pMsg, tConfInfo);
	GetConfInfoExFromJson((s8*)pMsg, tConfInfoEx);

	//ʱ��
	s8 achLastModifyTime[20+1] = {0};
	TKdvTime tCurTime;
	time_t tiCurTime = ::time( NULL );
	tCurTime.SetTime( &tiCurTime );
	tCurTime.GetString( achLastModifyTime, sizeof(achLastModifyTime) );

	u16 wRet = ERR_DB_SUCCESS;
	//�޸ĳ��ȣ������ն˶��˷Ų���
	s8  szSql[0x8000] = {0};

	LogPrint(LOG_LVL_DETAIL, MID_MAU_TEMPDB, "[ProcModTemplate] Start modify template<%s> in db!\n", tConfInfo.GetConfE164());
	s8 achConfName[MAXLEN_CONFNAME * 2 + 1];
	u64 nReturn = ptDBEngine->EscapeString(achConfName, tConfInfo.GetConfName());
	if (nReturn == 0)
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_CONFDB, "[ProcModTemplate] EscapeStr failed!\n" );
	}
	s8 achJsonMsg[SERV_MSG_LEN * 2 + 1];
	nReturn = ptDBEngine->EscapeString(achJsonMsg, (s8*)pMsg);
	if (nReturn == 0)
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_CONFDB, "[ProcModTemplate] EscapeStr failed!\n" );
	}
	sprintf( szSql, "update %s set %s = '%s',%s = '%s', %s ='%s' where %s = '%s\'",
		TTEMPLATEINFO,
		TTEMPLATENAME, achConfName,
		TLASTMODIFYTIME, achLastModifyTime,
		TTEMPLATEDATA, achJsonMsg,
		TTEMPLATEKEY, tConfInfo.GetConfE164());
	
	wRet = ptDBEngine->ExecSql(szSql);

	//�������֪ͨ
	CMauMsg cServMsg;
	tReqInfo.SetErrID( wRet );
	cServMsg.SetMsgBody( ( u8* )&tReqInfo, sizeof(tReqInfo) );
	cServMsg.CatMsgBody((u8*)&tConfInfo, sizeof(TConfInfo));
	cServMsg.CatMsgBody((u8*)&tConfInfoEx, sizeof(TConfInfoEx));

	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MODTEMP_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
	
	return ( ERR_DB_SUCCESS == wRet ) ? TRUE : FALSE;
}
