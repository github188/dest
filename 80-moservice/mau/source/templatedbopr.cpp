/*****************************************************************************
模块名      : MAU
文件名      : teemplatedbopr.cpp
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
#include "templatedbopr.h"
#include "mauvc.h"
#include "evmau.h"
#include "mauconst.h"
#include "mauutility.h"

/*=============================================================================
函 数 名： CTemplateDBOpr
功    能： 模板存储数据库操作类构造函数
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
CTemplateDBOpr::CTemplateDBOpr() : CAsynDBOpr()
{}

/*=============================================================================
函 数 名： ~CTemplateDBOpr
功    能： 模板存储数据库操作类析构函数
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
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
函 数 名： LoadDBData
功    能： 加载数据库数据
算法实现： 
全局变量： 
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CTemplateDBOpr::LoadDBData( void )
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)LOADTEMPLATEOPR );

	PostOneMsgToDBOprTask( tDBMsg, TRUE );

	return ;
}

/*=============================================================================
函 数 名： AddTemplate
功    能： 往数据库中添加模板信息
算法实现： 
全局变量： 
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
u8 CTemplateDBOpr::AddTemplate( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID( (u32)ADDTEMPLATEOPR );
	PostOneMsgToDBOprTask( tDBMsg, FALSE );

	return 0;
}


/*=============================================================================
函 数 名： DelTemplate
功    能： 删除数据库中的模板信息
算法实现： 
全局变量： 
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
u8 CTemplateDBOpr::DelTemplate( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID( (u32)DELTEMPLATEOPR );	
	PostOneMsgToDBOprTask( tDBMsg, FALSE );

	return 0;
}

/*=============================================================================
函 数 名： GetTemplateInfo
功    能： 从数据库中获取模板信息
算法实现： 
全局变量： 
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
u8 CTemplateDBOpr::GetTemplateInfo(TReqHeadInfo& tHeadInfo, const u16 wConfDuration)
{
	//因为会馆创会将带上duration参数，为了保持数据库操作的上下文一致性
	//终端模板创会也加上duration参数
	TDBOprTaskMsgItem tDBTaskMsg;
	tDBTaskMsg.SetMsgBody(&tHeadInfo, sizeof(tHeadInfo));
	tDBTaskMsg.CatMsgBody(&wConfDuration, sizeof(wConfDuration));
	tDBTaskMsg.SetTransactionID( (u32)FINDTEMPLATEOPR );	
	PostOneMsgToDBOprTask( tDBTaskMsg, FALSE );
	return 0;
}

/*=============================================================================
函 数 名： ModTemplate
功    能： 修改数据库中的模板信息
算法实现： 
全局变量： 
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
u8 CTemplateDBOpr::ModTemplate( TDBOprTaskMsgItem & tDBMsg )
{
	tDBMsg.SetTransactionID( (u32)MODTEMPLATEOPR );
	PostOneMsgToDBOprTask( tDBMsg, FALSE );

	return 0;
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
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CTemplateDBOpr::PrintConnectStatus()
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID(PRINT_CONNECT_STATUS);
	PostOneMsgToDBOprTask(tDBMsg, FALSE);
}

/*=============================================================================
函 数 名： DBOprReqEntry
功    能： MPCD数据库请求响应处理函数
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo	数据库任务信息
		   CDBEngine * ptDBEngine       数据库操作引擎
		   BOOL32 bAccept               连接线程是否与数据库断链
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CTemplateDBOpr::DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine)
{
	//参数校验
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
	//增加模板
	case ADDTEMPLATEOPR:
		bRet = ProcAddTemplate( ptTaskInfo, ptDBEngine );
		break;
	//获取模板详细信息
	case FINDTEMPLATEOPR:
		bRet = ProcGetTemplate( ptTaskInfo, ptDBEngine );
		break;
	//修改模板
	case MODTEMPLATEOPR:
		bRet = ProcModTemplate( ptTaskInfo, ptDBEngine );
		break;
	//删除模板
	case DELTEMPLATEOPR:
		bRet = ProcDelTemplate( ptTaskInfo, ptDBEngine );
		break;
	//打印连接状态
	case PRINT_CONNECT_STATUS:
		bRet = PrintStatus(ptDBEngine);
		break;
	default:
		break;

	}

	return bRet;
}


/*=============================================================================
函 数 名： ProcLoadDBData
功    能： 将数据库中的模板信息加载到内存中
算法实现： 
全局变量： 
参    数： CDBEngine * ptDBEngine		数据库操作引擎
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
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
		//通知主控加载数据库失败
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
	//不考虑加锁 加载数据库数据为第一步
	while( !ptDBEngine->IsEof() )
	{
		memset( achTempKey, 0, sizeof(achTempKey) );
		memset( achTempName, 0, sizeof(achTempName) );
		memset( achDomainName, 0, sizeof(achDomainName) );
		memset( achDomainGUID, 0, sizeof(achDomainGUID) );
		memset( achTempData, 0, sizeof(achTempData));
		memset( achDomainMOID, 0, sizeof(achDomainMOID) );

		//模板E164
		ptDBEngine->GetFieldValue( TTEMPLATEKEY, achTempKey, sizeof(achTempKey) );
		//模板名称
		ptDBEngine->GetFieldValue( TTEMPLATENAME, achTempName, sizeof(achTempName) );
		//模板所属域域名
		ptDBEngine->GetFieldValue( TDOMAINNAME, achDomainName, sizeof(achDomainName) );
		//模板所属域GUID
		ptDBEngine->GetFieldValue( TDOMAINGUID, achDomainGUID, sizeof(achDomainGUID) );
		//模板详细信息
		ptDBEngine->GetFieldValue( TTEMPLATEDATA, achTempData, sizeof(achTempData) );
		//模板所属域Moid
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

		//移动到下一数据
		ptDBEngine->MoveNext();
	}

	ptDBEngine->GoOnProcRes();

	if (nIdx > 0)
	{
		OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_MAU_ADDTEMPLATE_NTF, (u8*)atTemplateCacheData, sizeof(CTempCacheData) * nIdx);
	}

	LogPrint( LOG_LVL_DETAIL, MID_MAU_TEMPDB, "[CTemplateDBOpr::ProcLoadDBData] Load db data successful! wRet = %u\n", wRet );
	//通知主控数据库数据加载完成
	int nRet = OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_LOADDB_NTF, &wRet, sizeof(wRet) );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_TEMPDB, "[CTemplateDBOpr::ProcLoadDBData] post EV_MAU_FINISH_LOADDB_NTF to MAUVC %s, nRet = %d\n", (OSP_OK == nRet)?"successful":"fail", nRet );

	return bRet;
}


/*=============================================================================
函 数 名： ProcAddTemplate
功    能： 向数据库中添加一条模板记录
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo		数据库任务信息
		   CDBEngine * ptDBEngine			数据库操作引擎
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CTemplateDBOpr::ProcAddTemplate(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine)
{
	if( NULL == ptDBEngine || NULL == ptTaskInfo )
	{
		return FALSE;
	}
	
	const u8 * pMsg = ptTaskInfo->GetMsgBody();

	//请求头信息
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

	//时间
	s8 achCreateTime[20+1] = {0};

	//创建时间
	TKdvTime tCurTime;
	time_t tiCurTime = ::time( NULL );
	tCurTime.SetTime( &tiCurTime );
	tCurTime.GetString( achCreateTime, sizeof(achCreateTime) );

	LogPrint(LOG_LVL_DETAIL, MID_MAU_TEMPDB, "[ProcAddTemplate] Start add template<%s> to db!\n", pchConfE164);

	u16 wRet = ERR_DB_SUCCESS;
	// 修改长度，否则终端多了放不下
	s8  szSql[0x8000] = {0};


	//模板中加轮询信息
	//形成SQL
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

	//执行SQL
	wRet = ptDBEngine->ExecSql(szSql);

	//发送添加通知
	CMauMsg cServMsg;
	tReqInfo.SetErrID(wRet);
	tReqInfo.SetConfE164(pchConfE164);
	cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_ADDTEMP_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
	
	return ( ERR_DB_SUCCESS == wRet ) ? TRUE : FALSE;
}


/*=============================================================================
函 数 名： ProcDelTemplate
功    能： 从数据库中删除模板信息
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo		数据库任务信息
		   CDBEngine * ptDBEngine			数据库操作引擎
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
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
	
	//请求头信息
	TReqHeadInfo tReqInfo;
	memcpy( (void*)&tReqInfo, pbyMsg, sizeof(tReqInfo) );
	pbyMsg += sizeof(TReqHeadInfo);

	// 支持批量删除模板
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

		//形成SQL
		sprintf( szSql, "delete from %s where %s = \"%s\"", TTEMPLATEINFO, TTEMPLATEKEY, achConfE164 );
		//执行SQL
		wSqlRet = ptDBEngine->ExecSql(szSql);
		if ( ERR_DB_SUCCESS != wSqlRet )
		{
			continue;
		}

		cTmpMsg.CatMsgBody( (u8*)achConfE164, sizeof( achConfE164 ) );
	}

	u16 wCount = cTmpMsg.GetMsgBodyLen()/sizeof( achConfE164 );
	
	//发送成功添加通知
	
	tReqInfo.SetErrID(wSqlRet);
	CMauMsg cDelMsg;
	cDelMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
	cDelMsg.CatMsgBody(&wCount, sizeof(u16));
	cDelMsg.CatMsgBody(cTmpMsg.GetMsgBody(), cTmpMsg.GetMsgBodyLen());
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_DELTEMP_NTF, cDelMsg.GetServMsg(), cDelMsg.GetServMsgLen() );
	return ( ERR_DB_SUCCESS == wSqlRet ) ? TRUE : FALSE;
}


/*=============================================================================
函 数 名： ProcGetTemplate
功    能： 从数据库获取模板的详细信息
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo
           CDBEngine * ptDBEngine
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
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
	//请求头信息
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)ptTaskInfo->GetMsgBody();

	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	//形成SQL
	sprintf( szSql, "select %s from %s where %s = \"%s\"", TTEMPLATEDATA, TTEMPLATEINFO, TTEMPLATEKEY, tReqInfo.GetConfE164() );
	
	// 获取模板信息失败处理
	wRet = ptDBEngine->ExecSql(szSql, TRUE);
	if( ERR_DB_SUCCESS != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_TEMPDB, "[ProcGetTemplate] Get Temp %s failed! Error id = %u\n", tReqInfo.GetConfE164(), wRet );
		//发送查询
		tReqInfo.SetErrID(wRet);
		CMauMsg cServMsg;
		cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_SEARCHTEMP_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
		return FALSE;
	}

	if (ptDBEngine->IsEof())
	{
        LogPrint( LOG_LVL_ERROR, MID_MAU_TEMPDB, "[ProcGetTemplate] Get Temp %s result set is 0\n", tReqInfo.GetConfE164() );
        //发送查询
		tReqInfo.SetErrID(ERR_DB_NO_RESULESET);
        CMauMsg cServMsg;
		cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
        OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_SEARCHTEMP_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
		return FALSE;
    }
	
	s8	achTempData[MAXLEN_JSONBUF+1] = {0};
	
	//模板详细信息
	ptDBEngine->GetFieldValue(TTEMPLATEDATA, achTempData, sizeof(achTempData));
	ptDBEngine->GoOnProcRes();

	LogPrint( LOG_LVL_DETAIL, MID_MAU_TEMPDB, "[ProcGetTemplate] Get Temp %s successful!\n", tReqInfo.GetConfE164() );

	//发送查询
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
函 数 名： ProcModTemplate
功    能： 修改数据库中的模板信息
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo
		   CDBEngine * ptDBEngine
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
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

	//时间
	s8 achLastModifyTime[20+1] = {0};
	TKdvTime tCurTime;
	time_t tiCurTime = ::time( NULL );
	tCurTime.SetTime( &tiCurTime );
	tCurTime.GetString( achLastModifyTime, sizeof(achLastModifyTime) );

	u16 wRet = ERR_DB_SUCCESS;
	//修改长度，否则终端多了放不下
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

	//发送添加通知
	CMauMsg cServMsg;
	tReqInfo.SetErrID( wRet );
	cServMsg.SetMsgBody( ( u8* )&tReqInfo, sizeof(tReqInfo) );
	cServMsg.CatMsgBody((u8*)&tConfInfo, sizeof(TConfInfo));
	cServMsg.CatMsgBody((u8*)&tConfInfoEx, sizeof(TConfInfoEx));

	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_FINISH_MODTEMP_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
	
	return ( ERR_DB_SUCCESS == wRet ) ? TRUE : FALSE;
}
