/*****************************************************************************
模块名      : css
文件名      : confinfodbopr.cpp
创建时间    : 2015年 04月 09日
实现功能    : 
作者        : 阮楠楠
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/04/09  KDVP5.0     阮楠楠        创建
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
函 数 名： CConfInfoDBOpr
功    能： 会议信息数据库操作类构造函数
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
=============================================================================*/
CConfInfoDBOpr::CConfInfoDBOpr() : CAsynDBOpr()
{}

/*=============================================================================
函 数 名： ~CConfInfoDBOpr
功    能： 会议信息数据库操作类析构函数
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
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
函 数 名： ClearDBData
功    能： 清理数据库中的会议信息
算法实现： 
全局变量： 
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
=============================================================================*/
u8 CConfInfoDBOpr::ClearDBData( void )
{
	TDBOprTaskMsgItem tDBMsg;
	tDBMsg.SetTransactionID( (u32)CLEARDBDATAOPR );
	PostOneMsgToDBOprTask( tDBMsg, TRUE );
	
	return 0;
}

/*=============================================================================
函 数 名： AddConfInfo
功    能： 往数据库中添加会议信息
算法实现： 
全局变量： 
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
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
函 数 名： DelConfInfo
功    能： 删除数据库中的会议信息
算法实现： 
全局变量： 
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
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
函 数 名： ModConfInfo
功    能： 修改数据库中的模板信息
算法实现： 
全局变量： 
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
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
函 数 名： PrintConnectStatus
功    能： 打印连接状态
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBMsg
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
=============================================================================*/
void CConfInfoDBOpr::PrintConnectStatus()
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
2015/04/09  KDVP5.0     阮楠楠					创建
=============================================================================*/
BOOL32 CConfInfoDBOpr::DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine)
{
	//参数校验
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
	//增加会议信息
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
2015/04/09  KDVP5.0     阮楠楠					创建
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
函 数 名： ProcAddConfInfo
功    能： 向数据库中添加一条会议信息
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo		数据库任务信息
		   CDBEngine * ptDBEngine			数据库操作引擎
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
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
	//形成SQL
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
	
	//执行SQL
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
函 数 名： ProcDelConfInfo
功    能： 删除数据库中的会议信息
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo		数据库任务信息
		   CDBEngine * ptDBEngine			数据库操作引擎
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
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
	
	//请求头信息
	const TReqHeadInfo* ptReqInfo = (const TReqHeadInfo*)ptTaskInfo->GetMsgBody();
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_CONFDB, "[ProcDelConfInfo] Start delete conf<%s> from db!\n", ptReqInfo->GetConfE164() );
	
	u16 wRet = ERR_DB_SUCCESS;
	s8  szSql[256] = {0};
	
	//形成SQL
	sprintf( szSql, "delete from %s where %s = \"%s\"",
		TCONFINFO,TCONFE164, ptReqInfo->GetConfE164());

	//执行SQL
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
函 数 名： ProcModConfInfo
功    能： 修改数据库中会议信息
算法实现： 
全局变量： 
参    数： TDBOprTaskInfo * ptTaskInfo		数据库任务信息
		   CDBEngine * ptDBEngine			数据库操作引擎
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
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
	
	//请求头信息
	const TDBConf* ptConfInfo = (const TDBConf*)ptTaskInfo->GetMsgBody();
	LogPrint( LOG_LVL_DETAIL, MID_MAU_CONFDB, "[ProcModConfName] Start mod conf<%s> from db!\n", ptConfInfo->GetConfE164() );

	s8 achConfName[MAXLEN_CONFNAME * 2 + 1];
	u64 nReturn = ptDBEngine->EscapeString(achConfName, ptConfInfo->GetConfName());
	if (nReturn == 0)
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_CONFDB, "[ProcModConfName] EscapeStr failed!\n" );
	}

	//形成SQL
	s8  szSql[256] = {0};
	sprintf( szSql, "update %s set %s = '%s' where %s = '%s\'",
		TCONFINFO,
		TCONFNAME, achConfName,
		TCONFE164, ptConfInfo->GetConfE164());

	OspPrintf( TRUE, FALSE, "%s\n", szSql );
	
	//执行SQL
	u16 wRet = ptDBEngine->ExecSql(szSql);
	if( 0 != wRet )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CONFDB, "[ProcModConfName] Modify conf<%s> failed errno<%d>\n", ptConfInfo->GetConfE164(), wRet );
	}
	
	return TRUE;
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
2015/04/09  KDVP5.0     阮楠楠					创建
=============================================================================*/
const s8 * CConfInfoDBOpr::GetDBOprNameByTransId( u32 dwTransId )
{
    const s8 * pchDBOprName = NULL;

	switch ( dwTransId )
	{
	case CLEARDBDATAOPR:
		pchDBOprName = "ClearDBATAOpr";
		break;
		//增加会议信息
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

