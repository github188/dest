/*****************************************************************************
模块名      : mpcd
文件名      : dbopr.cpp
创建时间    : 2012年 02月 01日
实现功能    : 
作者        : 陈文灿
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/02/01  KDVP2.5     陈文灿        创建
******************************************************************************/
#ifdef WIN32
#pragma warning( disable:4786 )
#endif

#include "inc.h"
#include "osp.h"
#include "dbopr.h"

/*=============================================================================
函 数 名： TDBConnectInfo
功    能： 数据库连接信息构造函数
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
//lint -sem(TDBConnectInfo::SetNull,initializer)
TDBConnectInfo::TDBConnectInfo()
{
	SetNull();
}


/*=============================================================================
函 数 名： ~TDBConnectInfo
功    能： 数据库连接信息析构函数
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
TDBConnectInfo::~TDBConnectInfo() {}


/*=============================================================================
函 数 名： SetNull
功    能： 清空数据库结构
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBConnectInfo::SetNull( void )
{
	memset( m_achDBName, 0, sizeof(m_achDBName) );
	memset( m_achDBIP, 0, sizeof(m_achDBIP) );
	memset( m_achDBUserName, 0, sizeof(m_achDBUserName) );
	memset( m_achDBUserPwd, 0, sizeof(m_achDBUserPwd) );
	m_wDBPort = 0;
	m_byDBEngineType = 0;
	m_byReserved = 0;
}

/*=============================================================================
函 数 名： SetDBName
功    能： 设置数据库名称
算法实现： 
全局变量： 
参    数： LPCSTR pchDBName		数据库名称
		   u16 wNameLen				数据库名称长度
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBConnectInfo::SetDBName( LPCSTR pchDBName, u16 wNameLen )
{
	if( NULL == pchDBName || 0 == wNameLen )
	{
		return ;
	}

	u16 wDBNameLen = wNameLen > MAXLEN_DB_NAME ? MAXLEN_DB_NAME : wNameLen;
	memcpy( m_achDBName, pchDBName, wDBNameLen );

	return ;
}

/*=============================================================================
函 数 名： GetDBName
功    能： 获取数据库名称
算法实现： 
全局变量： 
参    数： void
返 回 值： LPCSTR
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
LPCSTR TDBConnectInfo::GetDBName( void ) const
{
	return m_achDBName;
}

/*=============================================================================
函 数 名： GetDBNameLen
功    能： 获取数据库名称长度
算法实现： 
全局变量： 
参    数： void
返 回 值： u16
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
u16 TDBConnectInfo::GetDBNameLen( void ) const
{
	return strlen(m_achDBName);
}

/*=============================================================================
函 数 名： SetDBIp
功    能： 设置数据库IP地址
算法实现： 
全局变量： 
参    数： LPCSTR pchDBIp		数据库IP地址
		   u16 wIpLen				数据库IP地址长度
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBConnectInfo::SetDBIp( LPCSTR pchDBIp, u16 wIpLen )
{
	if( NULL == pchDBIp || 0 == wIpLen )
	{
		return ;
	}

	u16 wDBIpLen = wIpLen > MAXLEN_DB_IPADDR ? MAXLEN_DB_IPADDR : wIpLen;
	memcpy( m_achDBIP, pchDBIp, wDBIpLen );

	return ;
}

/*=============================================================================
函 数 名： GetDBIp
功    能： 获取数据库IP地址
算法实现： 
全局变量： 
参    数： void
返 回 值： LPCSTR
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
LPCSTR TDBConnectInfo::GetDBIp( void ) const
{
	return m_achDBIP;
}

/*=============================================================================
函 数 名： GetDBIpLen
功    能： 获取数据库IP地址长度
算法实现： 
全局变量： 
参    数： void
返 回 值： u16
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
u16 TDBConnectInfo::GetDBIpLen( void ) const
{
	return strlen(m_achDBIP);
}


/*=============================================================================
函 数 名： SetDBUserName
功    能： 设置数据库用户名
算法实现： 
全局变量： 
参    数： LPCSTR pchDBUserName		数据库用户名
		   u16 wUserNameLen			数据库用户长度
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBConnectInfo::SetDBUserName( LPCSTR pchDBUserName, u16 wUserNameLen )
{
	if( NULL == pchDBUserName || 0 == wUserNameLen )
	{
		return ;
	}

	u16 wDBUserNameLen = 0;
	wDBUserNameLen = wUserNameLen > MAXLEN_DB_USERNAME ? MAXLEN_DB_USERNAME : wUserNameLen;
	memcpy( m_achDBUserName, pchDBUserName, wDBUserNameLen );

	return ;
}

/*=============================================================================
函 数 名： GetDBUserName
功    能： 获取数据库用户名
算法实现： 
全局变量： 
参    数： void
返 回 值： LPCSTR
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
LPCSTR TDBConnectInfo::GetDBUserName( void ) const
{
	return m_achDBUserName;
}

/*=============================================================================
函 数 名： GetDBIpLen
功    能： 获取数据库用户名长度
算法实现： 
全局变量： 
参    数： void
返 回 值： u16
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
u16 TDBConnectInfo::GetDBUserNameLen( void ) const
{
	return strlen(m_achDBUserName);
}


/*=============================================================================
函 数 名： SetDBUserPwd
功    能： 设置数据库用户密码
算法实现： 
全局变量： 
参    数： LPCSTR pchDBUserPwd		数据库用户密码
		   u16 wUserNameLen				数据库用户密码长度
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBConnectInfo::SetDBUserPwd( LPCSTR pchDBUserPwd, u16 wUserNameLen )
{
	if( NULL == pchDBUserPwd || 0 == wUserNameLen )
	{
		return ;
	}

	u16 wDBUserPwdLen = wUserNameLen > MAXLEN_DB_USERPWD ? MAXLEN_DB_USERPWD : wUserNameLen;
	memcpy( m_achDBUserPwd, pchDBUserPwd, wDBUserPwdLen );

	return ;
}

/*=============================================================================
函 数 名： GetDBUserName
功    能： 获取数据库用户密码
算法实现： 
全局变量： 
参    数： void
返 回 值： LPCSTR
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
LPCSTR TDBConnectInfo::GetDBUserPwd( void ) const
{
	return m_achDBUserPwd;
}

/*=============================================================================
函 数 名： GetDBUserPwdLen
功    能： 获取数据库用户密码长度
算法实现： 
全局变量： 
参    数： void
返 回 值： u16
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
u16 TDBConnectInfo::GetDBUserPwdLen( void ) const
{
	return strlen(m_achDBUserPwd);
}


/*=============================================================================
函 数 名： SetDBPort
功    能： 设置数据库端口
算法实现： 
全局变量： 
参    数： u16			数据库端口
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBConnectInfo::SetDBPort( u16 wDBPort )
{
	m_wDBPort = wDBPort;
}

/*=============================================================================
函 数 名： GetDBPort
功    能： 获取数据库端口
算法实现： 
全局变量： 
参    数： void
返 回 值： u16
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
u16 TDBConnectInfo::GetDBPort( void ) const
{
	return m_wDBPort;
}

/*=============================================================================
函 数 名： SetDBEngineType
功    能： 设置数据库类型
算法实现： 
全局变量： 
参    数： u16			数据库端口
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBConnectInfo::SetDBEngineType( u8 byEngineType )
{
	m_byDBEngineType = byEngineType;
}

/*=============================================================================
函 数 名： GetDBEngineType
功    能： 获取数据库类型
算法实现： 
全局变量： 
参    数： void
返 回 值： u16
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/29  KDVP2.5		陈文灿                  创建
=============================================================================*/
u8 TDBConnectInfo::GetDBEngineType( void ) const
{
	return m_byDBEngineType;
}


/*=============================================================================
函 数 名： CResultSet
功    能： 数据库结果集构造函数
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/28  KDVP2.5		陈文灿                  创建
=============================================================================*/
CResultSet::CResultSet()
{

}


/*=============================================================================
函 数 名： ~CResultSet
功    能： 数据库结果集析构函数
算法实现： 
全局变量： 
参    数： 
返 回 值：  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/28  KDVP2.5		陈文灿                  创建
=============================================================================*/
CResultSet::~CResultSet()
{

}


/*=============================================================================
函 数 名： MoveNext
功    能： 移动到下一条记录
算法实现： 
全局变量： 
参    数： 
返 回 值： TRUE		到最后一条记录
		   FALSE	未到最后一条记录
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/28  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 CResultSet::MoveNext( void )
{
	return m_cDBRs.MoveNext();
}


/*=============================================================================
函 数 名： MoveNNext
功    能： 移动到指定位置的记录
算法实现： 
全局变量： 
参    数： u32 dwNStep         移动的步数
           u32 &dwRealStep     实际移动的步数
返 回 值： TRUE		到最后一条记录
		   FALSE	未到最后一条记录
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/28  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 CResultSet::MoveNNext( u32 dwNStep, u32 &dwRealStep )
{
	dwRealStep = 0;
	if( dwNStep > 0 )
	{
		while( (dwNStep > 0) && (!m_cDBRs.IsEof()) )
		{
			m_cDBRs.MoveNext();
			dwNStep--;
			dwRealStep++;
		}
	}

	return ( !m_cDBRs.IsEof() );
}


/*=============================================================================
函 数 名： GetFieldCount
功    能： 获取结果集的列数
算法实现： 
全局变量： 
参    数： void
返 回 值： u16      列数
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/28  KDVP2.5		陈文灿                  创建
=============================================================================*/
u16 CResultSet::GetFieldCount( void )
{
	return m_cDBRs.GetFieldCount();
}

/*=============================================================================
函 数 名： IsEof
功    能： 是否到最后一条记录或者判断记录是否存在
算法实现： 
全局变量： 
参    数： void
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/28  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 CResultSet::IsEof( void )
{
	return m_cDBRs.IsEof();
}

/*=============================================================================
函 数 名： CDBEngine
功    能： 数据库操作引擎构造函数
算法实现： 
全局变量： 
参    数： 
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/28  KDVP2.5		陈文灿                  创建
=============================================================================*/
CDBEngine::CDBEngine( void )
{
	m_pcDBOprer = NULL;
	m_pcDBResultSet = NULL;
	m_bConnectDB = FALSE;
	m_pServerCbFunc = NULL;
	m_pClientCbFunc = NULL;
	m_dwUserData = 0;
	m_byDBEngineType = 0;
}


/*=============================================================================
函 数 名： ~CDBEngine
功    能： 数据库操作引擎析构函数
算法实现： 
全局变量： 
参    数： 
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/28  KDVP2.5		陈文灿                  创建
=============================================================================*/
CDBEngine::~CDBEngine( void )
{
	DisconnectDB();
	if( NULL != m_pcDBOprer )
	{
		delete m_pcDBOprer;
		m_pcDBOprer = NULL;
	}

	if( NULL != m_pcDBResultSet )
	{
		delete m_pcDBResultSet;
		m_pcDBResultSet = NULL;
	}
}


/*=============================================================================
函 数 名： SetCallBackUserData
功    能： 标记用户连接的回调函数
算法实现： 
全局变量： 
参    数： DBCbFunc pServerCbFunc
		   DBCbFunc pClientCbFunc
		   u32 dwUserData
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
void CDBEngine::SetCallBackUserData( DBCbFunc pServerCbFunc, DBCbFunc pClientCbFunc, u32 dwUserData )
{
	m_dwUserData = dwUserData;
	m_pServerCbFunc = pServerCbFunc;
	m_pClientCbFunc = pClientCbFunc;
}


/*=============================================================================
函 数 名： ConnectDB
功    能： 创建相应连接,并连接数据库
算法实现： 
全局变量： 
参    数： const TDBConnectInfo * ptDBConnectInfo
返 回 值： u16	错误码
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u16	CDBEngine::ConnectDB( const TDBConnectInfo * ptDBConnectInfo )
{
	if( IsConnectDB() )
	{
		return ERR_DB_SUCCESS;
	}

	if( NULL == ptDBConnectInfo )
	{
		return ERR_DB_INVALID_PARAM;
	}

	if( NULL == m_pcDBOprer )
	{
		m_pcDBOprer = new CDBHandler;
		if( NULL == m_pcDBOprer )
		{
			return ERR_DB_ALLOC_MEMORY;
		}
	}

	//FIXME: m_pcDBOprer 和 m_pcDBResultSet 什么删除
	if( NULL == m_pcDBResultSet )
	{
		m_pcDBResultSet = new CResultSet;
		if( NULL == m_pcDBResultSet )
		{
			return ERR_DB_ALLOC_MEMORY;
		}
	}

	
	BOOL32 bRet = FALSE;
	bRet = m_pcDBOprer->SetupSrvErrFuc( m_pServerCbFunc, (void*)m_dwUserData );
	if( !bRet )
	{
		return ERR_DB_SET_CBDATA;
	}

	bRet = m_pcDBOprer->SetupCtErrFuc( m_pClientCbFunc, (void*)m_dwUserData );
	if( !bRet )
	{
		return ERR_DB_SET_CBDATA;
	}


	bRet = m_pcDBOprer->ConnecttoSrv( ptDBConnectInfo->GetDBUserName(),
		                              ptDBConnectInfo->GetDBUserPwd(),
									  ptDBConnectInfo->GetDBName(),
									  ptDBConnectInfo->GetDBIp(),
									  ptDBConnectInfo->GetDBPort() );
	if( !bRet )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	m_bConnectDB = TRUE;
	m_byDBEngineType = ptDBConnectInfo->GetDBEngineType();

	return ERR_DB_SUCCESS;
}


/*=============================================================================
函 数 名： DisconnectDB
功    能： 断开数据库
算法实现： 
全局变量： 
参    数： void
返 回 值： u16	错误码
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u16	CDBEngine::DisconnectDB( void )
{
	u16 wRet = ERR_DB_SUCCESS;

	if( IsConnectDB() )
	{
		BOOL32 bRet = m_pcDBOprer->CloseConnection();
		wRet = (TRUE == bRet) ? ERR_DB_SUCCESS : ERR_DB_DISCONNECT_FAILED ;
	}

	//设置为断开状态
	m_bConnectDB = FALSE;
	m_byDBEngineType = 0;

	return wRet ;
}


/*=============================================================================
函 数 名： IsConnectDB
功    能： 判断是否有连接数据库
算法实现： 
全局变量： 
参    数： void
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 CDBEngine::IsConnectDB( void )
{
	if( NULL == m_pcDBOprer || NULL == m_pcDBResultSet )
	{
		return FALSE;
	}

	return m_bConnectDB;
}

/*=============================================================================
函 数 名： GetDBEngineType
功    能： 获取数据库引擎类型
算法实现： 
全局变量： 
参    数： void
返 回 值： u8
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u8 CDBEngine::GetDBEngineType( void ) const
{
	return m_byDBEngineType;
}

/*=============================================================================
函 数 名： GetDBUserData
功    能： 获取数据库对象用户参数
算法实现： 
全局变量： 
参    数： void
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetDBUserData( void ) const
{
	return m_dwUserData;
}

/*=============================================================================
函 数 名： ExecSql
功    能： 执行Sql语句
算法实现： 
全局变量： 
参    数： LPCSTR pchSql		SQL语句
		   BOOL32 bNeedRet		是否需要返回结果集
返 回 值： u32					错误码
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32	CDBEngine::ExecSql( LPCSTR pchSql, BOOL32 bNeedRet )
{
	if( NULL == pchSql )
	{
		return ERR_DB_INVALID_PARAM;
	}

	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	u16 wRet = DB_EXEC_ERRID_FAIL;
	if ( TRUE == bNeedRet )
	{
		wRet = m_pcDBOprer->ExecSql( pchSql, &m_pcDBResultSet->m_cDBRs );
		switch ( wRet )
		{
		case DB_EXEC_ERRID_SUCC:
			{
				//挪到第一条
				m_pcDBResultSet->MoveNext();
			}
			break;
		case DB_EXEC_ERRID_FAIL:
		case DB_EXEC_ERRID_LOST_CON:
			// something bad happened!
			break;
		default:
			// should not be here!
			break;
		}
	}
	else
	{
		wRet = m_pcDBOprer->ExecSql( pchSql );
	}

	if ( DB_EXEC_ERRID_SUCC != wRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： MoveNext
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 CDBEngine::MoveNext( void )
{
	if( !IsConnectDB() )
	{
		return FALSE;
	}

	return m_pcDBResultSet->MoveNext();
}

/*=============================================================================
函 数 名： GetFiledCount
功    能： 获取结果集中字段总数
算法实现： 
全局变量： 
参    数： void
返 回 值： u16
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFiledCount( void )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_NO_RESULESET;
	}

	return m_pcDBResultSet->GetFieldCount();
}

/*=============================================================================
函 数 名： MoveNNext
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 CDBEngine::MoveNNext( u32 dwNStep, BOOL32 bGoOnProcRes )
{
	if( !IsConnectDB() )
	{
		return FALSE;
	}

	if( 0 == dwNStep )
	{
		return FALSE;
	}

	BOOL32 bRet = FALSE;
	u32	dwRealStep = 0;
	u32 dwNextNStep = 0;
	bRet = m_pcDBResultSet->MoveNNext( dwNStep, dwRealStep );
	//整个数据集可能包含两块,前一块数据集跳转完毕,则尝试跳转下一个数据集
	if( FALSE == bRet && TRUE == bGoOnProcRes )
	{
		if( TRUE == IsEof() && ERR_DB_SUCCESS != GoOnProcRes() )
		{
			return bRet ;
		}

		if( dwNStep >= dwRealStep )
		{
			dwNextNStep = dwNStep - dwRealStep;
			bRet = m_pcDBResultSet->MoveNNext( dwNextNStep, dwNextNStep );
		}
	}

	return bRet;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段名取字段值
算法实现： 
全局变量： 
参    数： LPCSTR pchName
		   s32 &nValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, s32 &nValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, nValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段名取字段值
算法实现： 
全局变量： 
参    数： LPCSTR pchName
		   u32 &dwValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, u32 &dwValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, dwValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段名取字段值
算法实现： 
全局变量： 
参    数： LPCSTR pchName
		   s16 &sValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, s16 &sValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, sValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段名取字段值
算法实现： 
全局变量： 
参    数： LPCSTR pchName
		   u16 &wValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, u16 &wValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, wValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段名取字段值
算法实现： 
全局变量： 
参    数： LPCSTR pchName
		   s8 &chValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, s8 &chValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, chValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段名取字段值
算法实现： 
全局变量： 
参    数： LPCSTR pchName
		   u8 &byValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, u8 &byValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, byValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段名取字段值
算法实现： 
全局变量： 
参    数： LPCSTR pchName
		   LPSTR pchStr
		   u16 wSize
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, LPSTR pchStr, u16 wSize )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, pchStr, wSize );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段名取字段值
算法实现： 
全局变量： 
参    数： LPCSTR pchName
		   void* pImageBuf
		   u16 wImageBufLen
		   u16 &wImageOutLen
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, void* pImageBuf, u16 wImageBufLen, u16 &wImageOutLen )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, pImageBuf, wImageBufLen, wImageOutLen );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段索引取字段值
算法实现： 
全局变量： 
参    数： u8 byIndex
		   s32 &nValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, s32 &nValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, nValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段索引取字段值
算法实现： 
全局变量： 
参    数： u8 byIndex
		   u32 &dwValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, u32 &dwValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, dwValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段索引取字段值
算法实现： 
全局变量： 
参    数： u8 byIndex
		   s16 &sValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, s16 &sValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, sValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段索引取字段值
算法实现： 
全局变量： 
参    数： u8 byIndex
		   u16 &wValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, u16 &wValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, wValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段索引取字段值
算法实现： 
全局变量： 
参    数： u8 byIndex
		   s8 &chValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, s8 &chValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, chValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段索引取字段值
算法实现： 
全局变量： 
参    数： u8 byIndex
		   u8 &byValue
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, u8 &byValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, byValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段索引取字段值
算法实现： 
全局变量： 
参    数： u8 byIndex
		   LPSTR pStr
		   u16 wSize
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, LPSTR pStr, u16 wSize )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, pStr, wSize );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： GetFieldValue
功    能： 获字段索引取字段值
算法实现： 
全局变量： 
参    数： u8 byIndex
		   void *pImageBuf
		   u16 wImageBufLen
		   u16 &wImageOutLen
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, void *pImageBuf, u16 wImageBufLen, u16 &wImageOutLen )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, pImageBuf, wImageBufLen, wImageOutLen );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
函 数 名： IsEof
功    能： 是否到记录尾或者记录是否存在
算法实现： 
全局变量： 
参    数： void
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32	CDBEngine::IsEof( void )
{
	if( !IsConnectDB() )
	{
		return TRUE;
	}

	return m_pcDBResultSet->IsEof();
}

/*=============================================================================
函 数 名： GoOnProcRes
功    能： 继续处理结果集
算法实现： 
全局变量： 
参    数： void
返 回 值： u16
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/30  KDVP2.5		陈文灿                  创建
=============================================================================*/
u16 CDBEngine::GoOnProcRes( void )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_NO_RESULESET;
	}

	u16	wRet = ERR_DB_SUCCESS;
	EOpCode eRet = m_pcDBOprer->GoOnProcRes();
	if( EOpCode_FindRs != eRet )
	{
		return ERR_DB_NO_RESULESET;
	}
	if( FALSE == m_pcDBResultSet->MoveNext() )
	{
		return ERR_DB_NO_RESULESET;
	}

	return wRet;
}


/*=============================================================================
函 数 名： TDBOprTaskMsgItem
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
//lint -sem(TDBOprTaskMsgItem::SetNull,initializer)
TDBOprTaskMsgItem::TDBOprTaskMsgItem( )
{
	SetNull();
}

/*=============================================================================
函 数 名： ~TDBOprTaskMsgItem
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
TDBOprTaskMsgItem::~TDBOprTaskMsgItem( )
{
	SetNull();
}

/*=============================================================================
函 数 名： SetNull
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBOprTaskMsgItem::SetNull( void )
{
	m_dwSessionID = 0;
	m_dwTransactionID = 0;
	m_dwEventID = 0;
	m_dwMsgLen = 0;
	m_bDurable = FALSE;
	memset( m_abyReqMsgBuf, 0, sizeof(m_abyReqMsgBuf) );
}

/*=============================================================================
函 数 名： SetTransactionID
功    能： 设置事务ID
算法实现： 
全局变量： 
参    数： u32		事务ID(主机序)
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBOprTaskMsgItem::SetTransactionID( u32 dwTransactionID )
{
	m_dwTransactionID = htonl( dwTransactionID );
}

/*=============================================================================
函 数 名： GetTransactionID
功    能： 获取事务ID
算法实现： 
全局变量： 
参    数： void
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 TDBOprTaskMsgItem::GetTransactionID( void ) const
{
	return ntohl( m_dwTransactionID );
}

/*=============================================================================
函 数 名： SetSessionID
功    能： 设置会话ID
算法实现： 
全局变量： 
参    数： u32
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBOprTaskMsgItem::SetSessionID( u32 dwSessionID )
{
	m_dwSessionID = htonl( dwSessionID );
}

/*=============================================================================
函 数 名： GetSessionID
功    能： 设置会话ID
算法实现： 
全局变量： 
参    数： void
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 TDBOprTaskMsgItem::GetSessionID( void ) const 
{
	return ntohl( m_dwSessionID );
}

/*=============================================================================
函 数 名： SetEventID
功    能： 设置消息ID
算法实现： 
全局变量： 
参    数： u32
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBOprTaskMsgItem::SetEventID( u32 dwEventID )
{
	m_dwEventID = htonl( dwEventID );
}

/*=============================================================================
函 数 名： GetEventID
功    能： 设置会话ID
算法实现： 
全局变量： 
参    数： void
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 TDBOprTaskMsgItem::GetEventID( void ) const 
{
	return ntohl( m_dwEventID );
}

void TDBOprTaskMsgItem::SetDurable(BOOL32 bDurable)
{
	m_bDurable = bDurable;
}

BOOL32 TDBOprTaskMsgItem::IsDurable()
{
	return m_bDurable;
}

/*=============================================================================
函 数 名： SetRecordTime
功    能： 设置请求时间
算法实现： 
全局变量： 
参    数： const TKdvTime &tRecordTime
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBOprTaskMsgItem::SetRecordTime( const TKdvTime &tRecordTime )
{
	m_tRecordTime = tRecordTime;
}

/*=============================================================================
函 数 名： GetRecordTime
功    能： 获取请求时间
算法实现： 
全局变量： 
参    数： void
返 回 值： TKdvTime
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
TKdvTime TDBOprTaskMsgItem::GetRecordTime( void ) const
{
	return m_tRecordTime;
}

/*=============================================================================
函 数 名： SetMsgBody
功    能： 设置请求时间
算法实现： 
全局变量： 
参    数： const u8* pbyMsg
		   u32 wMsgLen
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 TDBOprTaskMsgItem::SetMsgBody( const void* pbyMsg, u32 wMsgLen )
{
	if( NULL == pbyMsg || 0 >= wMsgLen || wMsgLen > SERV_MSG_LEN )
	{
		return FALSE;
	}

	memcpy( m_abyReqMsgBuf, pbyMsg, wMsgLen );
	m_dwMsgLen = wMsgLen;

	return TRUE;
}

BOOL32 TDBOprTaskMsgItem::CatMsgBody(const void* pbyMsg, u32 wMsgLen)
{
	if (NULL == pbyMsg || 0 >= wMsgLen || wMsgLen > SERV_MSG_LEN - m_dwMsgLen)
	{
		return FALSE;
	}

	memcpy(m_abyReqMsgBuf + m_dwMsgLen, pbyMsg, wMsgLen);
	m_dwMsgLen += wMsgLen;

	return TRUE;
}

/*=============================================================================
函 数 名： GetMsgBodyLen
功    能： 获取请求消息长度
算法实现： 
全局变量： 
参    数： void
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 TDBOprTaskMsgItem::GetMsgBodyLen( void ) const
{
	return m_dwMsgLen;
}

/*=============================================================================
函 数 名： GetMsgBody
功    能： 获取请求消息
算法实现： 
全局变量： 
参    数： void
返 回 值： u32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
const u8* TDBOprTaskMsgItem::GetMsgBody( void ) const
{
	return m_abyReqMsgBuf;
}

/*=============================================================================
函 数 名： TDBOprTaskInfo
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
//lint -sem(TDBOprTaskInfo::SetNull,initializer)
TDBOprTaskInfo::TDBOprTaskInfo( void )
{
	SetNull();
}

/*=============================================================================
函 数 名： ~TDBOprTaskInfo
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
TDBOprTaskInfo::~TDBOprTaskInfo(void) {}

/*=============================================================================
函 数 名： SetNull
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBOprTaskInfo::SetNull( void )
{
	m_byDBConState = 0;
	m_byCancelTransaction = 0;
	m_byTerminateTask = 0;
	m_pData = NULL;
	m_hTaskID = 0;
	m_bIsIdle = TRUE;
}

/*=============================================================================
函 数 名： SetDBConState
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBOprTaskInfo::SetDBConState( BOOL32 bDBConOK )
{
	m_byDBConState = (TRUE == bDBConOK) ? 1 : 0;
}

/*=============================================================================
函 数 名： IsDBConOK
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 TDBOprTaskInfo::IsDBConOK( void ) const
{
	return ( 1 == m_byDBConState ) ? TRUE : FALSE;
}

/*=============================================================================
函 数 名： SetTerminateTask
功    能： 
算法实现： 
全局变量： 
参    数： BOOL32 bTerminate
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBOprTaskInfo::SetTerminateTask( BOOL32 bTerminate )
{
	m_byTerminateTask = ( TRUE == bTerminate ) ? 1 : 0;
}

/*=============================================================================
函 数 名： IsTerminateTask
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 TDBOprTaskInfo::IsTerminateTask( void ) const
{
	return ( 1 == m_byTerminateTask ) ? TRUE : FALSE ;
}


void TDBOprTaskInfo::SetUserData(void* pData)
{
	m_pData = pData;
}

void* TDBOprTaskInfo::GetUserData(void)
{
	return m_pData;
}

/*=============================================================================
函 数 名： SetTaskID
功    能： 设置消息ID
算法实现：
全局变量：
参    数： u32
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBOprTaskInfo::SetTaskID(TASKHANDLE hTaskID)
{
	m_hTaskID = hTaskID;
}

/*=============================================================================
函 数 名： GetTaskID
功    能： 获取线程任务ID
算法实现：
全局变量：
参    数： void
返 回 值： TASKHANDLE
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
TASKHANDLE TDBOprTaskInfo::GetTaskID(void)
{
	return m_hTaskID;
}

void TDBOprTaskInfo::SetIsIdle(BOOL32 bIdle)
{
	m_bIsIdle = bIdle;
}

BOOL32 TDBOprTaskInfo::IsIdle() const
{
	return m_bIsIdle;
}

/*=============================================================================
函 数 名： TDBMsgQueue
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
TDBMsgQueue::TDBMsgQueue( u32 dwMaxSize )
{
	m_dwMaxSize = dwMaxSize;
	m_dwLostMsg = 0;
	m_hSemQueueLock = NULL;
}

/*=============================================================================
函 数 名： ~TDBMsgQueue
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
//lint -sem(TDBMsgQueue::Clear,cleanup)
TDBMsgQueue::~TDBMsgQueue( )
{
	Clear();
}

/*=============================================================================
函 数 名： SetTaskSem
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/06/18  KDVP2.5.2	曾捷                  创建
=============================================================================*/
BOOL32 TDBMsgQueue::SetQueueSem( SEMHANDLE hSemTaskLock )
{
	if( NULL == hSemTaskLock )
	{
		return FALSE;
	}
	
	m_hSemQueueLock = hSemTaskLock;
	return TRUE;
}

/*=============================================================================
函 数 名： GetTaskSem
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/06/18  KDVP2.5.2	曾捷                  创建
=============================================================================*/
SEMHANDLE TDBMsgQueue::GetQueueSem( void ) const
{
	return m_hSemQueueLock;
}


/*=============================================================================
函 数 名： PushBackOneMsg
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 TDBMsgQueue::PushBackOneMsg( const TDBOprTaskMsgItem& tDBMsg )
{
	BOOL32 bRet = FALSE;
	OspSemTake(m_hSemQueueLock);

	if( m_dwMaxSize > GetCurMsgNum() )
	{
		m_cDBMsgQueue.push( tDBMsg );
		bRet =  TRUE;
	}

	OspSemGive(m_hSemQueueLock);
	return bRet;
}

/*=============================================================================
函 数 名： PopFrontOneMsg
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 TDBMsgQueue::PopFrontOneMsg(TDBOprTaskMsgItem* ptDBMsg)
{
	BOOL32 bRet = TRUE;
	OspSemTake(m_hSemQueueLock);

	if( TRUE == IsEmpty() )
	{
		bRet = FALSE;
	}
	else
	{
		if (ptDBMsg)
		{
			*ptDBMsg = m_cDBMsgQueue.front();
		}
		m_cDBMsgQueue.pop();
	}
	OspSemGive(m_hSemQueueLock);
	return bRet;
}

/*=============================================================================
函 数 名： GetFirstMsg
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
TDBOprTaskMsgItem TDBMsgQueue::GetFirstMsg( void )
{
	OspSemTake(m_hSemQueueLock);
	TDBOprTaskMsgItem& tDBMsgItem = m_cDBMsgQueue.front();
	OspSemGive(m_hSemQueueLock);
	return tDBMsgItem;
}

/*=============================================================================
函 数 名： GetNextMsg
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
TDBOprTaskMsgItem TDBMsgQueue::GetNextMsg( u32 dwPos )
{
	if( 1 == dwPos )
	{
		return ( TDBOprTaskMsgItem )m_cDBMsgQueue.front();
	}
	return (TDBOprTaskMsgItem)m_cDBMsgQueue.front();
}

/*=============================================================================
函 数 名： RemoveOneMsg
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 TDBMsgQueue::RemoveOneMsg( u32 dwPos )
{
	if( 1 == dwPos )
	{
		return PopFrontOneMsg();
	}

	return FALSE;
}

/*=============================================================================
函 数 名： IsEmpty
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 TDBMsgQueue::IsEmpty( void ) const
{
	return m_cDBMsgQueue.empty();
}

/*=============================================================================
函 数 名： Clear
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
void TDBMsgQueue::Clear( void )
{
	while( FALSE == IsEmpty() )
	{
		PopFrontOneMsg();
	}
	return ;
}

/*=============================================================================
函 数 名： GetCurMsgNum
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 TDBMsgQueue::GetCurMsgNum( void ) const
{
	return m_cDBMsgQueue.size();
}

/*=============================================================================
函 数 名： GetQueueSize
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/01/04  KDVP2.5		陈文灿                  创建
=============================================================================*/
u32 TDBMsgQueue::GetQueueSize( void ) const
{
	return m_dwMaxSize;
}

/*=============================================================================
函 数 名： SetCharacterSet
功    能： 设置编码字符集
算法实现： 
全局变量： 
参    数： LPCSTR pchCharSet
返 回 值： BOOL32	设置是否成功
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/03/31  5.0			阮楠楠                  创建
=============================================================================*/
BOOL32	CDBEngine::SetCharacterSet( LPCSTR pchCharSet )
{
	if( NULL == pchCharSet )
	{
		return FALSE;
	}

	if( !IsConnectDB() )
	{
		return FALSE;
	}

	return m_pcDBOprer->SetCharacterSet( pchCharSet );
}

u64 CDBEngine::EscapeString( LPSTR lpTo,LPCSTR lpFrom,BOOL32 isHandle/*=FALSE*/,u64 nLen/*=0*/ )
{
	if (NULL == m_pcDBOprer)
	{
		return 0;
	}

	return m_pcDBOprer->EscapeStr(lpTo, lpFrom, isHandle, nLen);
}


