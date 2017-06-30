/*****************************************************************************
ģ����      : css
�ļ���      : confinfodbopr.cpp
����ʱ��    : 2015�� 04�� 09��
ʵ�ֹ���    : 
����        : ����
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2015/04/09  KDVP5.0     ����        ����
******************************************************************************/

#ifdef WIN32
#pragma warning( disable:4786 )
#endif

#include "inc.h"
#include "mauvc.h"
#include "confinfodbopr.h"
#include "evmau.h"
#include "connectsa_api.h"

/*=============================================================================
�� �� ���� CConfInfoDBOpr
��    �ܣ� ������Ϣ���ݿ�����๹�캯��
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
CConfInfoDBOpr::CConfInfoDBOpr() : CAsynDBOpr()
{}

/*=============================================================================
�� �� ���� ~CConfInfoDBOpr
��    �ܣ� ������Ϣ���ݿ��������������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
CConfInfoDBOpr::~CConfInfoDBOpr()
{

}

const s8* CConfInfoDBOpr::GetModuleName()
{
	return "ConfInfoDBOprTaskProc";
}

u16 CConfInfoDBOpr::GetModuleId()
{
	return MID_MAU_CONFDB;
}

/*=============================================================================
�� �� ���� ClearDBData
��    �ܣ� �������ݿ��еĻ�����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskMsgItem & tDBMsg
�� �� ֵ�� u8 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
u8 CConfInfoDBOpr::ClearDBData( void )
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)CLEARDBDATAOPR );
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	
	return 0;
}

/*=============================================================================
�� �� ���� AddConfInfo
��    �ܣ� �����ݿ�����ӻ�����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskMsgItem & tDBMsg
�� �� ֵ�� u8 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
u8 CConfInfoDBOpr::AddConfInfo(const s8* pchConfE164, const s8* pchConfName, const s8* pchPwd)
{
	TDBConf tConfInfo;
	tConfInfo.SetConfE164(pchConfE164);
	tConfInfo.SetConfName(pchConfName);
	tConfInfo.SetConfPwd(pchPwd);

	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID(ADDCONFINFOOPR);
	tDBMsg.SetMsgBody(&tConfInfo, sizeof(tConfInfo));
	PostOneMsgToDBOprTask(tDBMsg, TRUE);
	return 0;
}

/*=============================================================================
�� �� ���� DelConfInfo
��    �ܣ� ɾ�����ݿ��еĻ�����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskMsgItem & tDBMsg
�� �� ֵ�� u8
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
u8 CConfInfoDBOpr::DelConfInfo(const s8* pchConfE164)
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)DELCONFINFOOPR );	
	TReqHeadInfo tReqInfo;
	tReqInfo.SetConfE164(pchConfE164);
	tDBMsg.SetMsgBody((u8*)&tReqInfo, sizeof(tReqInfo));
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	return 0;
}

/*=============================================================================
�� �� ���� ModConfInfo
��    �ܣ� �޸����ݿ��е�ģ����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskMsgItem & tDBMsg
�� �� ֵ�� u8
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
u8 CConfInfoDBOpr::ModConfName(const s8* pchConfE164, const s8* pchConfName  )
{
	TDBConf tConfInfo;
	tConfInfo.SetConfE164(pchConfE164);
	tConfInfo.SetConfName(pchConfName);

	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)MODCONFNAMEOPR);
	tDBMsg.SetMsgBody(&tConfInfo, sizeof(tConfInfo));
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
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
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
void CConfInfoDBOpr::PrintConnectStatus()
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
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
BOOL32 CConfInfoDBOpr::DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine)
{
	//����У��
	if( NULL == ptTaskInfo || NULL == ptDBEngine )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CONFDB, "[DBOprRegEntry] ERROR: param is NULL!\n" );
		return FALSE;
	}

	BOOL32 bRet = FALSE;

	switch( ptTaskInfo->GetTransactionID() )
	{
	case CLEARDBDATAOPR:
		bRet = ProcClearConfInfo( ptTaskInfo, ptDBEngine );
		break;
	//���ӻ�����Ϣ
	case ADDCONFINFOOPR:
		bRet = ProcAddConfInfo( ptTaskInfo, ptDBEngine );
		break;
	case MODCONFNAMEOPR:
		bRet = ProcModConfName( ptTaskInfo, ptDBEngine );
		break;
	case DELCONFINFOOPR:
		bRet = ProcDelConfInfo( ptTaskInfo, ptDBEngine );
		break;
	case PRINT_CONNECT_STATUS:
		bRet = PrintStatus(ptDBEngine);
		break;
	default:
		break;
	}

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
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
BOOL32 CConfInfoDBOpr::ProcClearConfInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine )
	{
		return bRet;
	}
	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	sprintf( szSql, "delete from %s where %s = %s", TCONFINFO, TCONFGUID, GetSrvGUID().GetNOString() );
	
	wRet = ptDBEngine->ExecSql( szSql );
	if( 0 != wRet )
	{
		bRet = TRUE;
		LogPrint( LOG_LVL_ERROR, MID_MAU_CONFDB, "[ProcClearConfInfo] clear confinfo failed(MPCDGUID:%s)\n", GetSrvGUID().GetNOString() );
	}

	return bRet;
}

/*=============================================================================
�� �� ���� ProcAddConfInfo
��    �ܣ� �����ݿ������һ��������Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo		���ݿ�������Ϣ
		   CDBEngine * ptDBEngine			���ݿ��������
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
BOOL32 CConfInfoDBOpr::ProcAddConfInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if (NULL == ptDBEngine || NULL == ptTaskInfo)
	{
		return bRet;
	}

	const TDBConf* ptConf = (const TDBConf*)ptTaskInfo->GetMsgBody();

	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[1024] = { 0 };

	s8 achConfName[MAXLEN_CONFNAME * 2 + 1] = { 0 };
	u64 nReturn = ptDBEngine->EscapeString(achConfName, ptConf->GetConfName());
	if (nReturn == 0)
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_CONFDB, "[ProcAddConfInfo] EscapeStr failed!\n" );
	}
	//�γ�SQL
	sprintf( szSql, "replace into %s (%s,%s,%s,%s,%s,%s) values('%s','%s',%d,'%s','%s','%s')",
		TCONFINFO,TCONFE164,TCONFNAME,TNEEDPASSWORD,TCONFPASSWORD,TCONFGUID,TCONFID,
		ptConf->GetConfE164(),
		achConfName,
		ptConf->HasPwd(),
		ptConf->GetConfPwd(),
		GetSrvGUID().GetNOString(),
		ptConf->GetConfE164()
	);

	LongLogPrint(LOG_LVL_DETAIL, MID_MAU_CONFDB, szSql, strlen(szSql));
	
	//ִ��SQL
	wRet = ptDBEngine->ExecSql(szSql);
	if( 0 != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CONFDB, "[ProcAddConfInfo] Add conf<%s> failed errno<%d>\n", ptConf->GetConfE164(), wRet );
	}
	else
	{
		bRet = TRUE;
		LogPrint( LOG_LVL_DETAIL, MID_MAU_CONFDB, "[ProcAddConfInfo] Add conf<%s> succuss\n", ptConf->GetConfE164() );
	}

	return bRet;
}

/*=============================================================================
�� �� ���� ProcDelConfInfo
��    �ܣ� ɾ�����ݿ��еĻ�����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo		���ݿ�������Ϣ
		   CDBEngine * ptDBEngine			���ݿ��������
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
BOOL32 CConfInfoDBOpr::ProcDelConfInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	BOOL32 bRet = FALSE;
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		return bRet;
	}
	
	if( ptTaskInfo->GetMsgBodyLen() < sizeof(TReqHeadInfo) )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CONFDB, "[ProcDelConfInfo] ERROR: Wrong MsgBody Len<%d>!\n", ptTaskInfo->GetMsgBodyLen() );
		return bRet;
	}
	
	//����ͷ��Ϣ
	const TReqHeadInfo* ptReqInfo = (const TReqHeadInfo*)ptTaskInfo->GetMsgBody();
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CONFDB, "[ProcDelConfInfo] Start delete conf<%s> from db!\n", ptReqInfo->GetConfE164() );
	
	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	
	//�γ�SQL
	sprintf( szSql, "delete from %s where %s = \"%s\"",
		TCONFINFO,TCONFE164, ptReqInfo->GetConfE164());

	//ִ��SQL
	wRet = ptDBEngine->ExecSql(szSql);
	if( 0 != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CONFDB, "[ProcDelConfInfo] Delete conf<%s> failed errno<%d>\n", ptReqInfo->GetConfE164(), wRet );
	}
	else
	{
		bRet = TRUE;
	}
	
	return bRet;
}

/*=============================================================================
�� �� ���� ProcModConfInfo
��    �ܣ� �޸����ݿ��л�����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� TDBOprTaskInfo * ptTaskInfo		���ݿ�������Ϣ
		   CDBEngine * ptDBEngine			���ݿ��������
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
BOOL32 CConfInfoDBOpr::ProcModConfName(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		return FALSE;
	}
	
	if( ptTaskInfo->GetMsgBodyLen() < sizeof(TDBConf) )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CONFDB, "[ProcModConfName] ERROR: Wrong MsgBody Len<%d>!\n", ptTaskInfo->GetMsgBodyLen() );
		return FALSE;
	}
	
	//����ͷ��Ϣ
	const TDBConf* ptConfInfo = (const TDBConf*)ptTaskInfo->GetMsgBody();
	LogPrint( LOG_LVL_DETAIL, MID_MAU_CONFDB, "[ProcModConfName] Start mod conf<%s> from db!\n", ptConfInfo->GetConfE164() );

	s8 achConfName[MAXLEN_CONFNAME * 2 + 1];
	u64 nReturn = ptDBEngine->EscapeString(achConfName, ptConfInfo->GetConfName());
	if (nReturn == 0)
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_CONFDB, "[ProcModConfName] EscapeStr failed!\n" );
	}

	//�γ�SQL
	s8  szSql[256] = {0};
	sprintf( szSql, "update %s set %s = '%s' where %s = '%s\'",
		TCONFINFO,
		TCONFNAME, achConfName,
		TCONFE164, ptConfInfo->GetConfE164());

	OspPrintf( TRUE, FALSE, "%s\n", szSql );
	
	//ִ��SQL
	u16 wRet = ptDBEngine->ExecSql(szSql);
	if( 0 != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CONFDB, "[ProcModConfName] Modify conf<%s> failed errno<%d>\n", ptConfInfo->GetConfE164(), wRet );
	}
	
	return TRUE;
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
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
const s8 * CConfInfoDBOpr::GetDBOprNameByTransId( u32 dwTransId )
{
    const s8 * pchDBOprName = NULL;

	switch ( dwTransId )
	{
	case CLEARDBDATAOPR:
		pchDBOprName = "ClearDBATAOpr";
		break;
		//���ӻ�����Ϣ
	case ADDCONFINFOOPR:
		pchDBOprName = "AddConfInfoOpr";
		break;
	case MODCONFNAMEOPR:
		pchDBOprName = "ModConfNameOpr";
		break;
	case DELCONFINFOOPR:
		pchDBOprName = "DelConfInfoOpr";
		break;
	default:
		pchDBOprName = NULL;
		break;
	}

	return pchDBOprName;
}

