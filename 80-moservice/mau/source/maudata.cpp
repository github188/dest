/*****************************************************************************
模块名      : MAU
文件名      : maudata.cpp
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
#pragma warning(disable:4786)
#pragma warning(disable:4800)
#endif

#include "inc.h"
#include "maudata.h"
#include "evmau.h"
#include "md5.h"
#include "redisadapter.h"
#include "rabbitmqwrapper.h"
#include "cmmaussn.h"
#include "resmgrssn.h"
#include "bmclicense.h"
#include <set>
#include <string>
#include <vector>
#include <bitset>

using namespace std;
using namespace rabbitmqwrap;

extern SEMHANDLE g_hTestQConsumerSem;

extern CResMgrSsnApp g_cResMgrSsnApp;

template<typename T> void Print_Ref(T it)
{
	it.second->Print();
}

template<typename T> void Print(T it)
{
	it.second.Print();
}

/*=============================================================================
函 数 名： CMauData
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
CMauData::CMauData()
{
	m_dwTempE164Backup = OPERATION_MAXTEMPSERIALNO + 1;
	// 该版本在获取不到License时，默认为64
	m_wTempConfNum = 0;
	m_byInitCount = 0;

	memset(m_achTelnetName, 0, sizeof(m_achTelnetName));
	memset(m_achTelnetPwd, 0, sizeof(m_achTelnetPwd));
	memset(m_achMqIP, 0, sizeof(m_achMqIP));
	memset(m_achZkIP, 0, sizeof(m_achZkIP));
	memset(m_achHduPoolIP, 0, sizeof(m_achHduPoolIP));
	memset(m_achRedisIP, 0, sizeof(m_achRedisIP));
	memset(m_achRedisPwd, 0, sizeof(m_achRedisPwd));
	memset(m_achPlatformMoid, 0, sizeof(m_achPlatformMoid));
	memset(m_achPlatformGuid, 0, sizeof(m_achPlatformGuid));
	m_wZkPort = DEF_ZKPORT;
	m_wHduPoolPort = HDURESPOOL_LISTEN_PORT;
	m_wRedisPort = DEF_REDISPORT;
	m_dwRecoverTimeSpan = DEF_RECOVERTIMESPAN;
	m_dwMcuHeartBeatTimeSpan = DEF_MCUHEARTBEATTIMESPAN;
	m_dwGetConfListTimeSpan = DEF_GETCONFLISTTIMESPAN;
	m_dwListenPort = MAU_LISTEN_PORT;
	m_dwMqPort = DEF_MQPORT;
	m_bPlatformRent = TRUE;
	m_dwMqConsumerHeartBeat = DEF_MQCONSUMER_HEARTBEAT;
}



/*=============================================================================
函 数 名： ~CMauData
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
CMauData::~CMauData()
{
	return;
}

/*=============================================================================
函 数 名： ReadConfig
功    能： 读取配置文件
算法实现： 
全局变量： 
参    数： void
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::ReadConfig()
{
	s8 achProfileName[255 + 1] = { 0 };

#if defined(_PPC_) || defined(_ARM_)
	sprintf(achProfileName, "%s/%s", DIR_CONFIG, CSSCFGFILENAME);
#else
	sprintf(achProfileName, "%s/%s", DIR_CSSCONFIG, CSSCFGFILENAME);
#endif

	s8 achReturn[255+1] = {0};
	s32 nValue = 0;
	BOOL32 bRet = TRUE;
	BOOL32 bResult = TRUE;

	//数据库连接线程个数 默认1个线程
	bRet = GetRegKeyInt( achProfileName, SECTION_CSS, KEY_DBCONNTREAHDNUM, 1, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBConnThreadNum.\n" );
		nValue = 1;
	}
	m_dwDBTaskNum = nValue;

	//数据库消息队列大小 默认300条
	bRet = GetRegKeyInt( achProfileName, SECTION_CSS, KEY_DBMSGQUEUENUM, 300, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBMsgQueueNum.\n" );
		nValue = 300;
	}
	m_dwMsgQueueNum = nValue;

	// ------------------------模板
	m_tTemplateDBInfo.SetNull();

	//数据库引擎 默认使用MySQL
	bRet = GetRegKeyInt( achProfileName, SECTION_TEMPLATEDB, KEY_DBENGINETYPE, enumDBEngineTypeMySql, &nValue );
	if( !bRet || 0 == nValue )
	{
        LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBEngineType.\n" );
	}
	m_tTemplateDBInfo.SetDBEngineType( (u8)nValue );

	//数据库IP 默认为本地
	bRet = GetRegKeyString( achProfileName, SECTION_TEMPLATEDB, KEY_DBIPAddr, MAU_DEF_DBIP, achReturn,sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBIpAddr.\n" );
	}
	m_tTemplateDBInfo.SetDBIp( achReturn, strlen(achReturn) );

	//数据库端口 默认3306
	bRet = GetRegKeyInt( achProfileName, SECTION_TEMPLATEDB, KEY_DBPORT, MAU_DEF_DBPORT, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBPort.\n" );
	}
	m_tTemplateDBInfo.SetDBPort( nValue );

	//数据库名
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_TEMPLATEDB, KEY_DBNAME, MAU_DEF_DBNAME, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBName.\n" );
		strncpy( achReturn, MAU_DEF_DBNAME, sizeof(achReturn)-1 );
	}
	m_tTemplateDBInfo.SetDBName( achReturn, strlen(achReturn) );

	//数据库用户名
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_TEMPLATEDB, KEY_DBUSERNAME, MAU_DEF_DBUSERNAME, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBUserName.\n" );
		strncpy( achReturn, MAU_DEF_DBUSERNAME, sizeof(achReturn)-1 );
	}
	m_tTemplateDBInfo.SetDBUserName( achReturn, strlen(achReturn) );

	//数据库用户名密码
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_TEMPLATEDB, KEY_DBUSERPWD, MAU_DEF_DBUSERPWD, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBUserPwd.\n" );
		strncpy( achReturn, MAU_DEF_DBUSERPWD, sizeof(achReturn)-1 );
	}
	m_tTemplateDBInfo.SetDBUserPwd( achReturn, strlen(achReturn) );

	// -------------------------会管侧VMeeting
	m_tVMeetingDBInfo.SetNull();

	//数据库引擎 默认使用MySQL
	bRet = GetRegKeyInt( achProfileName, SECTION_MEETINGDB, KEY_DBENGINETYPE, enumDBEngineTypeMySql, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBEngineType.\n" );
		nValue = enumDBEngineTypeMySql;
	}
	m_tVMeetingDBInfo.SetDBEngineType( (u8)nValue );
		
	//数据库IP 默认为本地
	bRet = GetRegKeyString( achProfileName, SECTION_MEETINGDB, KEY_DBIPAddr, MAU_DEF_DBIP, achReturn,sizeof(achReturn) );
	if( !bRet && 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBIpAddr.\n" );
		strncpy( achReturn, MAU_DEF_DBIP, sizeof(achReturn)-1 );
	}
	m_tVMeetingDBInfo.SetDBIp( achReturn, strlen(achReturn) );
		
	//数据库端口 默认3306
	bRet = GetRegKeyInt( achProfileName, SECTION_MEETINGDB, KEY_DBPORT, MAU_DEF_DBPORT, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBPort.\n" );
	}
	m_tVMeetingDBInfo.SetDBPort( nValue );
		
	//数据库名
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_MEETINGDB, KEY_DBNAME, MAU_DEF_BOOKDBNAME, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBName.\n" );
		strncpy( achReturn, MAU_DEF_BOOKDBNAME, sizeof(achReturn)-1 );
	}
	m_tVMeetingDBInfo.SetDBName( achReturn, strlen(achReturn) );
		
	//数据库用户名
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_MEETINGDB, KEY_DBUSERNAME, MAU_DEF_DBUSERNAME, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBUserName.\n" );
		strncpy( achReturn, MAU_DEF_DBUSERNAME, sizeof(achReturn)-1 );
	}
	m_tVMeetingDBInfo.SetDBUserName( achReturn, strlen(achReturn) );
		
	//数据库用户名密码
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_MEETINGDB, KEY_DBUSERPWD, MAU_DEF_DBUSERPWD, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBUserPwd.\n" );
		strncpy( achReturn, MAU_DEF_DBUSERPWD, sizeof(achReturn)-1 );
	}
	m_tVMeetingDBInfo.SetDBUserPwd( achReturn, strlen(achReturn) );

	// ----------------------------会议信息数据库相关信息 支持呼叫中心
	m_tConfInfoDBInfo.SetNull();
	//数据库引擎 默认使用MySQL
	bRet = GetRegKeyInt( achProfileName, SECTION_CONFINFODB, KEY_DBENGINETYPE, enumDBEngineTypeMySql, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBEngineType.\n" );
	}
	m_tConfInfoDBInfo.SetDBEngineType( (u8)nValue );

	//数据库IP 默认为本地
	bRet = GetRegKeyString( achProfileName, SECTION_CONFINFODB, KEY_DBIPAddr, MAU_DEF_DBIP, achReturn,sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBIpAddr.\n" );
		strncpy( achReturn, MAU_DEF_DBIP, sizeof(achReturn)-1 );
	}
	m_tConfInfoDBInfo.SetDBIp( achReturn, strlen(achReturn) );
		
	//数据库端口 默认3306
	bRet = GetRegKeyInt( achProfileName, SECTION_CONFINFODB, KEY_DBPORT, MAU_DEF_DBPORT, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBPort.\n" );
		nValue = MAU_DEF_DBPORT;
	}
	m_tConfInfoDBInfo.SetDBPort( nValue );
		
	//数据库名
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_CONFINFODB, KEY_DBNAME, MAU_DEF_CONFDBNAME, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
    	LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBName.\n" );
		strncpy( achReturn, MAU_DEF_CONFDBNAME, sizeof(achReturn)-1 );
	}
	m_tConfInfoDBInfo.SetDBName( achReturn, strlen(achReturn) );
		
	//数据库用户名
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_CONFINFODB, KEY_DBUSERNAME, MAU_DEF_DBUSERNAME, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBUserName.\n" );
		strncpy( achReturn, MAU_DEF_DBUSERNAME, sizeof(achReturn)-1 );
	}
	m_tConfInfoDBInfo.SetDBUserName( achReturn, strlen(achReturn) );
		
	//数据库用户名密码
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_CONFINFODB, KEY_DBUSERPWD, MAU_DEF_DBUSERPWD, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBUserPwd.\n" );
		strncpy( achReturn, MAU_DEF_DBUSERPWD, sizeof(achReturn)-1 );
	}
	m_tConfInfoDBInfo.SetDBUserPwd( achReturn, strlen(achReturn) );

	// AP数据库信息
	m_tApDBInfo.SetNull();
	
	//数据库引擎 默认使用MySQL
	bRet = GetRegKeyInt( achProfileName, SECTION_APINFODB, KEY_DBENGINETYPE, enumDBEngineTypeMySql, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBEngineType.\n" );
	}
	m_tApDBInfo.SetDBEngineType( (u8)nValue );
	
	//数据库IP 默认为本地
	bRet = GetRegKeyString( achProfileName, SECTION_APINFODB, KEY_DBIPAddr, MAU_DEF_DBIP, achReturn,sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBIpAddr.\n" );
		strncpy( achReturn, MAU_DEF_DBIP, sizeof(achReturn)-1 );
	}
	m_tApDBInfo.SetDBIp( achReturn, strlen(achReturn) );
	
	//数据库端口 默认3306
	bRet = GetRegKeyInt( achProfileName, SECTION_APINFODB, KEY_DBPORT, MAU_DEF_DBPORT, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBPort.\n" );
		nValue = MAU_DEF_DBPORT;
	}
	m_tApDBInfo.SetDBPort( nValue );
	
	//数据库名
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_APINFODB, KEY_DBNAME, AP_DEF_DBNAME, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBName.\n" );
		strncpy( achReturn, MAU_DEF_DBNAME, sizeof(achReturn)-1 );
	}
	m_tApDBInfo.SetDBName( achReturn, strlen(achReturn) );
	
	//数据库用户名
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_APINFODB, KEY_DBUSERNAME, AP_DEF_DBUSERNAME, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBUserName.\n" );
		strncpy( achReturn, MAU_DEF_DBUSERNAME, sizeof(achReturn)-1 );
	}
	m_tApDBInfo.SetDBUserName( achReturn, strlen(achReturn) );
	
	//数据库用户名密码
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_APINFODB, KEY_DBUSERPWD, AP_DEF_DBUSERPWD, achReturn, sizeof(achReturn) );
	if( !bRet || 0 == strncmp(achReturn , "" , sizeof(achReturn)) )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBUserPwd.\n" );
		strncpy( achReturn, MAU_DEF_DBUSERPWD, sizeof(achReturn)-1 );
	}
	m_tApDBInfo.SetDBUserPwd( achReturn, strlen(achReturn) );

	bRet = GetRegKeyString( achProfileName, SECTION_LOCALINFO, KEY_TELNETNAME, MCU_TEL_USRNAME, m_achTelnetName, sizeof( m_achTelnetName ) );
	if ( !bRet )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read telnet name.\n" );
	}
	m_achTelnetName[sizeof(m_achTelnetName)-1] = '\0';
	
	bRet = GetRegKeyString( achProfileName, SECTION_LOCALINFO, KEY_TELNETPASS, MCU_TEL_PWD, m_achTelnetPwd, sizeof( m_achTelnetPwd ) );
	if ( !bRet )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read telnet pass.\n" );
	}
	m_achTelnetPwd[sizeof(m_achTelnetPwd)-1] = '\0';

	// -----------------详细的预约会议DB
	m_tScheduledConfDBInfo.SetNull();

	//数据库引擎 默认使用MySQL
	bRet = GetRegKeyInt(achProfileName, SECTION_SCHEDULEDCONFINFODB, KEY_DBENGINETYPE, enumDBEngineTypeMySql, &nValue);
	if (!bRet || 0 == nValue)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBEngineType.\n");
		nValue = enumDBEngineTypeMySql;
	}
	m_tScheduledConfDBInfo.SetDBEngineType((u8)nValue);

	//数据库IP 默认为本地
	bRet = GetRegKeyString(achProfileName, SECTION_SCHEDULEDCONFINFODB, KEY_DBIPAddr, MAU_DEF_DBIP, achReturn, sizeof(achReturn));
	if (!bRet || 0 == strncmp(achReturn, "", sizeof(achReturn)))
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBIpAddr.\n");
		strncpy(achReturn, MAU_DEF_DBIP, sizeof(achReturn) - 1);
	}
	m_tScheduledConfDBInfo.SetDBIp(achReturn, strlen(achReturn));

	//数据库端口 默认3306
	bRet = GetRegKeyInt(achProfileName, SECTION_SCHEDULEDCONFINFODB, KEY_DBPORT, MAU_DEF_DBPORT, &nValue);
	if (!bRet || 0 == nValue)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBPort.\n");
		nValue = MAU_DEF_DBPORT;
	}
	m_tScheduledConfDBInfo.SetDBPort(nValue);

	//数据库名
	memset(achReturn, 0, sizeof(achReturn));
	bRet = GetRegKeyString(achProfileName, SECTION_SCHEDULEDCONFINFODB, KEY_DBNAME, MAU_DEF_DBNAME, achReturn, sizeof(achReturn));
	if (!bRet || 0 == strncmp(achReturn, "", sizeof(achReturn)))
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBName.\n");
		strncpy(achReturn, MAU_DEF_DBNAME, sizeof(achReturn) - 1);
	}
	m_tScheduledConfDBInfo.SetDBName(achReturn, strlen(achReturn));

	//数据库用户名
	memset(achReturn, 0, sizeof(achReturn));
	bRet = GetRegKeyString(achProfileName, SECTION_SCHEDULEDCONFINFODB, KEY_DBUSERNAME, MAU_DEF_DBUSERNAME, achReturn, sizeof(achReturn));
	if (!bRet || 0 == strncmp(achReturn, "", sizeof(achReturn)))
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBUserName.\n");
		strncpy(achReturn, MAU_DEF_DBUSERNAME, sizeof(achReturn) - 1);
	}
	m_tScheduledConfDBInfo.SetDBUserName(achReturn, strlen(achReturn));

	//数据库用户名密码
	memset(achReturn, 0, sizeof(achReturn));
	bRet = GetRegKeyString(achProfileName, SECTION_SCHEDULEDCONFINFODB, KEY_DBUSERPWD, MAU_DEF_DBUSERPWD, achReturn, sizeof(achReturn));
	if (!bRet || 0 == strncmp(achReturn, "", sizeof(achReturn)))
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read DBUserPwd.\n");
		strncpy(achReturn, MAU_DEF_DBUSERPWD, sizeof(achReturn) - 1);
	}
	m_tScheduledConfDBInfo.SetDBUserPwd(achReturn, strlen(achReturn));
//-------------------------------------------------------------------------

	// MqInfo
	bRet = GetRegKeyString( achProfileName, SECTION_MQINFO, KEY_MQIPADDR, "0.0.0.0", m_achMqIP, sizeof( m_achMqIP ) );
	if ( !bRet )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read MqIpAddr.\n" );
	}
	m_achMqIP[sizeof(m_achMqIP)-1] = '\0';

	bRet = GetRegKeyInt( achProfileName, SECTION_MQINFO, KEY_MQPORT, DEF_MQPORT, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read MQPort.\n" );
	}
	m_dwMqPort = (u16)nValue;

	//核心域MQINFO
	bRet = GetRegKeyString( achProfileName, SECTION_MQINFO, KEY_TOPLEVELMQIPADDR, "0.0.0.0", m_achTopLevelMqIP, sizeof( m_achTopLevelMqIP) );
	if ( !bRet )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read MqIpAddr.\n" );
	}
	m_achTopLevelMqIP[sizeof(m_achTopLevelMqIP)-1] = '\0';

	// ZkInfo
	bRet = GetRegKeyString( achProfileName, SECTION_ZKINFO, KEY_ZKIPADDR, "0.0.0.0", m_achZkIP, sizeof( m_achZkIP ) );
	if ( !bRet )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read ZkIpAddr.\n" );
	}
	m_achZkIP[sizeof(m_achZkIP)-1] = '\0';

	bRet = GetRegKeyInt( achProfileName, SECTION_ZKINFO, KEY_ZKPORT, DEF_ZKPORT, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read ZkPort.\n" );
	}
	m_wZkPort = (u16)nValue;

	// UpuInfo
	bRet = GetRegKeyString( achProfileName, SECTION_UPUINFO, KEY_UPUIPADDR, "0.0.0.0", m_achUpuIP, sizeof( m_achUpuIP ) );
	if ( !bRet )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read MqIpAddr.\n" );
	}
	m_achUpuIP[sizeof(m_achUpuIP)-1] = '\0';

	
	// 连接电视墙资源池信息
	memset( achReturn, 0, sizeof(achReturn) );
	bRet = GetRegKeyString( achProfileName, SECTION_HDUPOOLINFO, KEY_HDUPOOL_IPADDRESS, "0.0.0.0", m_achHduPoolIP, sizeof( m_achHduPoolIP ) );
	if( !bRet )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read HdupoolIpAddr.\n" );
	}

	m_achHduPoolIP[sizeof(m_achHduPoolIP)-1] = '\0';

	bRet = GetRegKeyInt( achProfileName, SECTION_HDUPOOLINFO, KEY_HDUPOOL_PORT, HDURESPOOL_LISTEN_PORT, &nValue );
	if( !bRet || 0 == nValue )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read HduPoolPort.\n" );
	}
	m_wHduPoolPort = (u16)nValue;

	bRet = GetRegKeyString(achProfileName, SECTION_REDIS, KEY_IPADDR, "0.0.0.0", m_achRedisIP, sizeof(m_achRedisIP));
	if (!bRet)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read Redis Ip.\n");
	}
	bRet = GetRegKeyString(achProfileName, SECTION_REDIS, KEY_REDISPWD, "KedaRedis16", m_achRedisPwd, sizeof(m_achRedisPwd));
	if (!bRet)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read Redis Pwd.\n");
	}

	bRet = GetRegKeyInt(achProfileName, SECTION_REDIS, KEY_PORT, DEF_REDISPORT, &nValue);
	if (!bRet)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read Redis Port.\n");
	}
	m_wRedisPort = (u16)nValue;

	bRet = GetRegKeyInt(achProfileName, SECTION_CSS, KEY_RECOVERTIMESPAN, DEF_RECOVERTIMESPAN, &nValue);
	if (!bRet)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read recover time.\n");
	}
	m_dwRecoverTimeSpan = (u32)nValue;

	bRet = GetRegKeyInt(achProfileName, SECTION_CSS, KEY_MCUHEARTBEATTIMESPAN, DEF_MCUHEARTBEATTIMESPAN, &nValue);
	if (!bRet)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read mcu heartbeat time.\n");
	}
	m_dwMcuHeartBeatTimeSpan = (u32)nValue;

	bRet = GetRegKeyInt(achProfileName, SECTION_CSS, KEY_GETCONFLISTIMESPAN, DEF_GETCONFLISTTIMESPAN, &nValue);
	if (!bRet)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read get conflist time.\n");
	}
	m_dwGetConfListTimeSpan = (u32)nValue;

	bRet = GetRegKeyInt(achProfileName, SECTION_CSS, "listenport", MAU_LISTEN_PORT, &nValue);
	if (!bRet)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read css osp listen port .\n");
	}
	m_dwListenPort = (u32)nValue;

	bRet = GetRegKeyInt(achProfileName, SECTION_CSS, "ConsumerBHTime", DEF_MQCONSUMER_HEARTBEAT, &nValue);
	if (!bRet)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ReadConfig] Error While read css osp listen port .\n");
	}
	m_dwMqConsumerHeartBeat = (u32)nValue;

#ifdef WIN32
	bRet = GetRegKeyString(achProfileName, "windows", "moid", "", m_achPlatformMoid, sizeof(m_achPlatformMoid));
	bRet = GetRegKeyString(achProfileName, "windows", "guid", "00000000000000000000000000000000",
		m_achPlatformGuid, sizeof(m_achPlatformGuid));
#endif


	return bResult;
}

BOOL32 CMauData::Init()
{
	ReadConfig();
	m_cRedisData.Init(m_achRedisIP, m_wRedisPort, m_achRedisPwd);
	m_cRedisData.LoadAllScript((const s8**)s_pchScriptFile, LUA_SCRIPT_NUM);

	// 初始化MQ
	s32 nRet = RabbitmqClntInit(m_achMqIP, m_dwMqPort, "/", "dev", "dev");
	if (RT_OK == nRet)
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[DaemonProcLoadDBResNtf]RabbitmqClntInit ok!\n");
	}
	else
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[DaemonProcLoadDBResNtf]RabbitmqClntInit failed!\n");
	}
	return TRUE;
}

void CMauData::Quit()
{
	m_cTemplateDBOpr.Quit();
	m_cConfInfoDBOpr.Quit();
	m_cVMeetingDBOpr.Quit();
	m_cApInfoDBOpr.Quit();

	RabbitmqClntQuit();
}

BOOL32 CMauData::LoadRedisLua(vector<string>* pvecFailedScript/*=NULL*/)
{
	return m_cRedisData.LoadAllScript((const s8**)s_pchScriptFile, LUA_SCRIPT_NUM, pvecFailedScript);
}

/*=============================================================================
函 数 名： GetDBTaskNum
功    能： 获取数据库线程个数
算法实现： 
全局变量： 
参    数： void
返 回 值： u8 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
u32 CMauData::GetDBTaskNum( void ) const
{
	return m_dwDBTaskNum;
}

/*=============================================================================
函 数 名： GetQueueSize
功    能： 获取数据库消息队列大小
算法实现： 
全局变量： 
参    数： void
返 回 值： u8 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
u32 CMauData::GetQueueSize( void ) const
{
	return m_dwMsgQueueNum;
}

/*=============================================================================
函 数 名： GetDBConnectInfo
功    能： 获取数据库连接信息
算法实现： 
全局变量： 
参    数： void
返 回 值： TDBConnectInfo
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
TDBConnectInfo CMauData::GetTemplateDBConnectInfo( void ) const
{
	return m_tTemplateDBInfo;
}

/*=============================================================================
函 数 名： GetVMeetingDBConnectInfo
功    能： 获取会议信息数据库连接信息
算法实现： 
全局变量： 
参    数： void
返 回 值： TDBConnectInfo
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
TDBConnectInfo CMauData::GetVMeetingDBConnectInfo( void ) const
{
	return m_tVMeetingDBInfo;
}

/*=============================================================================
函 数 名： GetApDBConnectInfo
功    能： 获取数据库连接信息
算法实现： 
全局变量： 
参    数： void
返 回 值： TDBConnectInfo
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
TDBConnectInfo CMauData::GetApDBConnectInfo( void ) const
{
	return m_tApDBInfo;
}

/*=============================================================================
函 数 名： GetConfInfoDBConnectInfo
功    能： 获取会议信息数据库连接信息
算法实现： 
全局变量： 
参    数： void
返 回 值： TDBConnectInfo
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
TDBConnectInfo CMauData::GetConfInfoDBConnectInfo( void ) const
{
	return m_tConfInfoDBInfo;
}

/*=============================================================================
函 数 名： IsMtExist
功    能： 判断终端是否存在
算法实现： 
全局变量： 
参    数： const CKdvMtInfo & tMtInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::IsMtExist( const s8* pchE164 )
{
	AutoLock cAutoLock(&m_cMtListLock);
	BOOL32 bRet = (m_mapMtInfo.find(pchE164) != m_mapMtInfo.end());
	return bRet ;
}

/*=============================================================================
函 数 名： AddMtInfo
功    能： 添加终端信息
算法实现： 
全局变量： 
参    数： const CKdvMtInfo & tMtInfo
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::AddMtInfo( const CMauMtInfo & tMtInfo )
{
	AutoLock cAutoLock(&m_cMtListLock);
	m_mapMtInfo[tMtInfo.GetE164()] = tMtInfo;
}

/*=============================================================================
函 数 名： ModMtInfo
功    能： 
算法实现： 
全局变量： 
参    数： const CKdvMtInfo & tMtInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::ModMtInfo(const CMauMtInfo & tMtInfo)
{
	AutoLock cAutoLock(&m_cMtListLock);

	BOOL32 bRet = FALSE;
	CMtInfoMap::iterator it = m_mapMtInfo.find(tMtInfo.GetE164());
	if ( m_mapMtInfo.end() != it)
	{
		bRet = TRUE;
		it->second = tMtInfo;
	}

	return bRet;
}

/*=============================================================================
函 数 名： DelMtInfo
功    能： 删除终端信息
算法实现： 
全局变量： 
参    数： const CKdvMtInfo & tMtInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::DelMtInfo(const CMauMtInfo & tMtInfo)
{
	AutoLock cAutoLock(&m_cMtListLock);

	BOOL32 bRet = FALSE;
	CMtInfoMap::iterator it = m_mapMtInfo.find(tMtInfo.GetE164());
	if (it != m_mapMtInfo.end())
	{
		bRet = TRUE;
		m_mapMtInfo.erase(it);
	}

	return bRet;
}


/*=============================================================================
函 数 名： GetMtInfo
功    能： 获取终端信息
算法实现： 
全局变量： 
参    数： const CKdvMtInfo & tKeyInfo  待查找的终端
           CKdvMtInfo & tOutInfo        返回查找到的终端信息
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::GetMtInfo(const s8* pchE164, CMauMtInfo & tOutInfo)
{
	AutoLock cAutoLock(&m_cMtListLock);

	BOOL32 bRet = FALSE;
	CMtInfoMap::iterator it = m_mapMtInfo.find(pchE164);
	if (m_mapMtInfo.end() != it)
	{
		bRet = TRUE;
		tOutInfo = it->second;
	}
	return bRet;
}


/*=============================================================================
函 数 名： PrintMtList
功    能： 打印终端列表信息
算法实现： 
全局变量： 
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::PrintMtList( void )
{
	AutoLock cAutoLock(&m_cMtListLock);

	for_each(m_mapMtInfo.begin(), m_mapMtInfo.end(),
		std::ptr_fun(Print<CMauData::CMtInfoMap::reference>));
	OspPrintf( TRUE, FALSE, "Total mt info num<%d>\n", m_mapMtInfo.size() );
}

u32 CMauData::GetMtListSize( void )
{
	u32 dwMtListSize = 0;
	
	AutoLock cAutoLock(&m_cMtListLock);
	dwMtListSize = m_mapMtInfo.size();
	
	return dwMtListSize;
}

BOOL32 CMauData::GetAllMtList(CKdvMtInfo * pcMtList, u32 &dwMtListSize)
{
	if ( NULL == pcMtList || 0 == dwMtListSize )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetAllMtList]Param is valid, return FALSE!\n" );
		return FALSE;
	}
	
	AutoLock cAutoLock(&m_cMtListLock);

	if ( m_mapMtInfo.size() > dwMtListSize )
	{
		return FALSE;
	}
	
	u32 dwCount = 0;
	CMtInfoMap::iterator itLoop = m_mapMtInfo.begin();
	while( m_mapMtInfo.end() != itLoop )
	{
		if ( dwCount >= dwMtListSize )
		{
			break;
		}

		itLoop->second.GetKdvMtInfo(pcMtList[dwCount]);
		dwCount++;

		++itLoop;
	}
	
	return TRUE;
}


/*=============================================================================
函 数 名： IsDomainExist
功    能： 判断域是否存在
算法实现： 
全局变量： 
参    数： const CKdvDomainInfo & tDomainInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::IsDomainExist( const s8* pchMoid )
{
	AutoLock cAutoLock(&m_cDomainListLock);
	BOOL32 bRet = (m_mapDomainInfo.find(pchMoid) != m_mapDomainInfo.end());
	return bRet ;
}

/*=============================================================================
函 数 名： AddDomainInfo
功    能： 添加域信息
算法实现： 
全局变量： 
参    数： const CKdvDomainInfo & tDomainInfo
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::AddDomainInfo( const CKdvDomainInfo & tDomainInfo )
{
//	LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[AddDomainInfo]DomainGUI<%s>!\n", tDomainInfo.GetDomainMOID() );
	AutoLock cAutoLock(&m_cDomainListLock);

	//rnn[2014/07/17]容错处理，当域guid为空时，不允许添加域信息到map表中
// 	if ( 0 == strncmp( tDomainInfo.GetDomainGUID(), "", OPERATION_MAXLEN_GUID ) )
// 	{
// 		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[AddDomainInfo]Domainguid is null, not add to map.\n" );
// 		OspSemGive( g_hDomainListSem );
// 		return;
// 	}
	//end

//	m_mapDomainInfo.Insert( tDomainInfo, tDomainInfo );

	//modif by lbg
	m_mapDomainInfo[tDomainInfo.GetDomainMOID()] = tDomainInfo;
}

/*=============================================================================
函 数 名： ModDomainInfo
功    能： 修改域信息
算法实现： 
全局变量： 
参    数： const CKdvDomainInfo & tOldInfo
           const CKdvDomainInfo & tNewInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::ModDomainInfo( const CKdvDomainInfo & tNewInfo )
{
//	LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[ModDomainInfo]DomainGUI<%s>!\n", tNewInfo.GetDomainMOID() );
	AutoLock cAutoLock(&m_cDomainListLock);

	//rnn[2014/07/17]容错处理，当域guid为空时，不允许修改map表中域信息
// 	if ( 0 == strncmp( tNewInfo.GetDomainGUID(), "", OPERATION_MAXLEN_GUID ) )
// 	{
// 		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[ModDomainInfo]Domainguid is null, not mdf to map.\n" );
// 		OspSemGive( g_hDomainListSem );
// 		return FALSE;
// 	}
	//end

// 	BOOL32 bRet = m_mapDomainInfo.Delete( tOldInfo );
// 	m_mapDomainInfo.Insert( tNewInfo, tNewInfo );
	CKdvDomainInfoMap::iterator it = m_mapDomainInfo.find(tNewInfo.GetDomainMOID());
	if( m_mapDomainInfo.end() != it )
	{
		it->second = tNewInfo;
	}

	return TRUE;
}

/*=============================================================================
函 数 名： DelDomainInfo
功    能： 删除域信息
算法实现： 
全局变量： 
参    数： const CKdvDomainInfo & tDomainInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::DelDomainInfo( const s8* pMoid )
{
	AutoLock cAutoLock(&m_cDomainListLock);

	CKdvDomainInfoMap::iterator it = m_mapDomainInfo.find(pMoid);
	if( m_mapDomainInfo.end() != it )
	{
		m_mapDomainInfo.erase(it);
	}

	return TRUE;
}

/*=============================================================================
函 数 名： GetDomainInfo
功    能： 获取域信息
算法实现： 
全局变量： 
参    数： const CKdvDomainInfo & tKeyInfo
           CKdvDomainInfo & tOutInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::GetDomainInfo( const s8* pMoid, CKdvDomainInfo & tOutInfo )
{
	AutoLock cAutoLock(&m_cDomainListLock);

	BOOL32 bRet = FALSE;

	CKdvDomainInfoMap::iterator it = m_mapDomainInfo.find(pMoid);
	if (m_mapDomainInfo.end() != it)
	{
		bRet = TRUE;
		tOutInfo = it->second;
	}

	return bRet;
}

void CMauData::AddModHduMoidUserDomainMoid(const s8 *pchHduMoid, const s8 *pchUserDomainMoid)
{
	AutoLock cAutoLock(&m_cHduUserDomainMoidLock);

	CHduUserDomainMoidMap::iterator itHdu = m_mapHduUserDomainMoid.find(pchHduMoid);
	if (itHdu != m_mapHduUserDomainMoid.end())
	{
		itHdu->second = pchUserDomainMoid;
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[CMauData::AddModHduMoidUserDomainMoid]Mod HduUserDomainMoid :%s\n", pchUserDomainMoid);
	}
	else
	{
		//增加，启用HDU2
		m_mapHduUserDomainMoid.insert(CHduUserDomainMoidMap::value_type(pchHduMoid, pchUserDomainMoid));
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[CMauData::AddModHduMoidUserDomainMoid]Hdu don't exist, add\n");
	}
}

void CMauData::DelHduUserDomainMoid(const s8 *pchHduMoid)
{
	AutoLock cAutoLock(&m_cHduUserDomainMoidLock);

	CHduUserDomainMoidMap::iterator itHdu = m_mapHduUserDomainMoid.find(pchHduMoid);
	if (itHdu != m_mapHduUserDomainMoid.end())
	{
		m_mapHduUserDomainMoid.erase(itHdu);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[CMauData::DelHduUserDomainMoid]Delete HduMoid from m_mapHduUserDomainMoid Map, Key:%s\n", pchHduMoid);
	}
	else
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[CMauData::DelHduUserDomainMoid]Delete HduMoid don't exist\n");
	}
}

BOOL32 CMauData::GetUserDomainMoidByHduMoid(const string &strHduMoid, string &strUserDomainMoid)
{
	CHduUserDomainMoidMap::iterator it = m_mapHduUserDomainMoid.begin();
	while (it != m_mapHduUserDomainMoid.end())
	{
		if (it->first != strHduMoid)
		{
			++it;	
		}
		else
		{
			strUserDomainMoid = it->second;
			return TRUE;
		}
	}
	return FALSE;
}
/*=============================================================================
函 数 名： ModDomainHasXmpuInfoBySDomainMoid
功    能： 由上级域Moid获取用户域信息
算法实现： 
全局变量： 
参    数： const CKdvDomainInfo & tKeyInfo
           CKdvDomainInfo & tOutInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/03/28  5.0         阮楠楠                  创建
=============================================================================*/
BOOL32 CMauData::ModDomainHasXmpuInfoBySDomainMoid(const s8* psDomainMoid, BOOL32 bHas)
{
	AutoLock cAutoLock(&m_cDomainListLock);

	CKdvDomainInfoMap::iterator it = m_mapDomainInfo.begin();
	for (; it != m_mapDomainInfo.end(); it++)
	{
		if (0 == strncmp( it->second.GetSDomainMOID(), psDomainMoid, OPERATION_MAXLEN_MOID )
			&& CKdvDomainInfo::em_KdvUserDomain == it->second.GetDomainLevel())
		{
			it->second.SetHasXmpuServer(bHas);
		}
	}
	return TRUE;
}


/*=============================================================================
函 数 名： PrintDomainList
功    能： 打印域列表信息
算法实现： 
全局变量： 
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::PrintDomainList( void )
{
	AutoLock cAutoLock(&m_cDomainListLock);

	//存储服务域的DomainMOID
	typedef std::vector<std::string> VecTopDomain;
	//存储DomainMOID的对应关系
	typedef std::multimap<std::string, std::string> MulmapDomain;
	typedef MulmapDomain::iterator mulmapDomainIt;
	VecTopDomain vecTopDomain;
	MulmapDomain mulmapDomain;
	CKdvDomainInfoMap::iterator it = m_mapDomainInfo.begin();
	for (; it != m_mapDomainInfo.end(); it++)
	{
		if (CKdvDomainInfo::em_KdvTopLevelDomain == it->second.GetDomainLevel())
		{
			vecTopDomain.push_back(it->first);
		}
		else
		{
			//存储上级MOID 下级MOID
			mulmapDomain.insert(make_pair(it->second.GetSDomainMOID(), it->second.GetDomainMOID()));
		}	
	}
	//打印出数据
	if (0 != vecTopDomain.size())
	{
		u32 nIndex1 = 0;
		for(; nIndex1 < vecTopDomain.size(); ++nIndex1)
		{
			
			std::string& strTopDomain = vecTopDomain[nIndex1];
			CKdvDomainInfoMap::iterator itTop = m_mapDomainInfo.find(strTopDomain);
			if (itTop != m_mapDomainInfo.end())
			{
				//打印服务域
				itTop->second.Print();

				//打印平台域
				if (mulmapDomain.count(strTopDomain) > 0)
				{
					mulmapDomainIt itPlatDomainLow = mulmapDomain.lower_bound(strTopDomain);
					mulmapDomainIt itPlatDomainUp  = mulmapDomain.upper_bound(strTopDomain);
					for (; itPlatDomainLow != itPlatDomainUp; ++itPlatDomainLow)
					{
						std::string& strPlatDomain = itPlatDomainLow->second;
						CKdvDomainInfoMap::iterator itPlatDomain = m_mapDomainInfo.find(strPlatDomain);
						if (itPlatDomain != m_mapDomainInfo.end())
						{
							itPlatDomain->second.Print();

							//打印该平台域下所有的用户域
							if (mulmapDomain.count(strPlatDomain) > 0)
							{
								mulmapDomainIt itUserDomainLow = mulmapDomain.lower_bound(strPlatDomain);
								mulmapDomainIt itUserDomainUp  = mulmapDomain.upper_bound(strPlatDomain);
								for (; itUserDomainLow != itUserDomainUp; ++itUserDomainLow)
								{
									std::string& strUserDomain = itUserDomainLow->second;
									CKdvDomainInfoMap::iterator itUserDomain = m_mapDomainInfo.find(strUserDomain);
									if (itUserDomain != m_mapDomainInfo.end())
									{
										itUserDomain->second.Print();
									}
								}
							}
						}
					}	
				}
			}
		}
	}
	OspPrintf( TRUE, FALSE, "Total Domain info num<%d>\n", m_mapDomainInfo.size() );
}

u32 CMauData::GetDomainSize(void)
{
	u32 dwDomainSize = 0;

	AutoLock cAutoLock(&m_cDomainListLock);
	dwDomainSize = m_mapDomainInfo.size();

	return dwDomainSize;
}

BOOL32 CMauData::GetAllDomainList( CKdvDomainInfo * pcDomainList, u32 &dwDomainSize )
{
	if ( NULL == pcDomainList || 0 == dwDomainSize )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetAllDomainList]Param is valid, return FALSE!\n" );
		return FALSE;
	}

	AutoLock cAutoLock(&m_cDomainListLock);

	if ( m_mapDomainInfo.size() > dwDomainSize )
	{
		return FALSE;
	}

	u32 dwCount = 0;
	CKdvDomainInfoMap::iterator it = m_mapDomainInfo.begin();
	for (; it != m_mapDomainInfo.end(); it++)
	{
		if ( dwCount >= dwDomainSize )
		{
			break;
		}
		*(pcDomainList+dwCount) = it->second;
		dwCount++;
	}

	return TRUE;
}


/*=============================================================================
函 数 名： ClearDomainInfo
功    能： 打印域列表信息
算法实现： 
全局变量： 
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/14  5.0         rnn                    创建
=============================================================================*/
void CMauData::ClearDomainInfo( void )
{
	AutoLock cAutoLock(&m_cDomainListLock);
	m_mapDomainInfo.clear();
}

void CMauData::AddTempData( const CTempCacheData & tTempData )
{
	AutoLock cAutoLock(&m_cTempCacheDataLock);

	CTempCacheData* pData = new CTempCacheData;

	*pData = tTempData;

	m_mapTempCacheInfo[tTempData.GetConfE164()] = pData;
	m_mapTempMoidCacheInfo.insert( CTempCacheMoidDataMap::value_type(tTempData.GetDomainMOID(), pData) );

	m_wTempConfNum = m_mapTempCacheInfo.size();
	
	return ;
}

void CMauData::ClearTempData()
{
	AutoLock cAutoLock(&m_cTempCacheDataLock);

	CTempCacheDataMap::iterator iter = m_mapTempCacheInfo.begin();
	CTempCacheDataMap::iterator iterEnd = m_mapTempCacheInfo.end();

	while (iter != iterEnd)
	{
		delete iter->second;
		iter++;
	}
	
	m_mapTempCacheInfo.clear();
	m_mapTempMoidCacheInfo.clear();
	m_wTempConfNum = 0;
}

BOOL32 CMauData::DelTempData( const s8* pchConfE164 )
{
	AutoLock cAutoLock(&m_cTempCacheDataLock);
	
	BOOL32 bRet = FALSE;
	CTempCacheDataMap::iterator it = m_mapTempCacheInfo.find(pchConfE164);
	if( m_mapTempCacheInfo.end() != it )
	{
		bRet = TRUE;
		CTempCacheMoidDataMap::iterator itMoid = m_mapTempMoidCacheInfo.lower_bound(it->second->GetDomainMOID());
		CTempCacheMoidDataMap::iterator itMoidEnd = m_mapTempMoidCacheInfo.upper_bound(it->second->GetDomainMOID());

		while (itMoidEnd != itMoid)
		{
			if (strcmp(itMoid->second->GetConfE164(), pchConfE164) == 0)
			{
				delete itMoid->second;
				m_mapTempMoidCacheInfo.erase(itMoid);
				break;
			}

			itMoid++;
		}

		m_mapTempCacheInfo.erase(it);
	}

	m_wTempConfNum = m_mapTempCacheInfo.size();
	
	return bRet;
}

BOOL32 CMauData::ModTempData( const CTempCacheData & tTempData )
{
	AutoLock cAutoLock(&m_cTempCacheDataLock);
	
	BOOL32 bRet = FALSE;
	CTempCacheDataMap::iterator it = m_mapTempCacheInfo.find(tTempData.GetConfE164());
	if( m_mapTempCacheInfo.end() != it )
	{
		bRet = TRUE;

		CTempCacheMoidDataMap::iterator itMoid = m_mapTempMoidCacheInfo.lower_bound(it->second->GetDomainMOID());
		CTempCacheMoidDataMap::iterator itMoidEnd = m_mapTempMoidCacheInfo.upper_bound(it->second->GetDomainMOID());

		while (itMoidEnd != itMoid)
		{
			if (strcmp(itMoid->second->GetConfE164(), tTempData.GetConfE164()) == 0)
			{
				*itMoid->second = tTempData;
				break;
			}

			itMoid++;
		}
	}
	
	return bRet;
}

u16 CMauData::GetTemplateNumByDomain(const s8* pDomainMOID, BOOL32 bIsCheckPublic /*= FALSE*/)
{
	if( NULL == pDomainMOID )
	{
		return 0;
	}
	
	AutoLock cAutoLock(&m_cTempCacheDataLock);
	
	//模板个数
	u16 wRetNum = 0;

	CTempCacheMoidDataMap::iterator iterFind = m_mapTempMoidCacheInfo.lower_bound(pDomainMOID);
	CTempCacheMoidDataMap::iterator iterEnd = m_mapTempMoidCacheInfo.upper_bound(pDomainMOID);

	while (iterFind != iterEnd)
	{
		BOOL32 bMatch = (!bIsCheckPublic || 1 == iterFind->second->GetPublicConf());
		if (bMatch)
		{
			wRetNum++;
		}	
		iterFind++;
	}
	
	//CTempCacheDataMap::iterator itLoop = m_mapTempCacheInfo.begin();
	//while( m_mapTempCacheInfo.end() != itLoop )
	//{
	//	if( 0 == strncmp(itLoop->second.GetDomainMOID(), pDomainMOID, OPERATION_MAXLEN_MOID) )
	//	{
	//		BOOL32 bMatch = (!bIsCheckPublic || 1 == itLoop->second.GetPublicConf());
	//		if (bMatch)
	//		{
	//			wRetNum++;
	//		}			
	//	}		
	//	++itLoop;
	//}
	
	return wRetNum;
}

BOOL32 CMauData::GetTemplateInfoByE164(const s8* pTempE164, CTempCacheData & tTempDataInfo)
{
	BOOL32 bRet = FALSE;
	if( NULL == pTempE164 )
	{
		return bRet;
	}
	
	AutoLock cAutoLock(&m_cTempCacheDataLock);

	CTempCacheDataMap::iterator it = m_mapTempCacheInfo.find(pTempE164);
	if (m_mapTempCacheInfo.end() != it)
	{
		bRet = TRUE;
		tTempDataInfo = *it->second;
	}
	
	return bRet;
}

BOOL32 CMauData::GetTemplateListByDomain(const s8* pDomainMOID,
	std::vector<CTempCacheData>& vecTempConf, BOOL32 bIsCheckPublic)
{
	if( NULL == pDomainMOID )
	{
		return FALSE;
	}
	
	AutoLock cAutoLock(&m_cTempCacheDataLock);

	CTempCacheMoidDataMap::iterator iterFind = m_mapTempMoidCacheInfo.lower_bound(pDomainMOID);
	CTempCacheMoidDataMap::iterator iterEnd = m_mapTempMoidCacheInfo.upper_bound(pDomainMOID);
	while (iterFind != iterEnd)
	{
		BOOL32 bMatch = (!bIsCheckPublic || 1 == iterFind->second->GetPublicConf());
		if (bMatch)
		{
			vecTempConf.push_back(*iterFind->second);
		}
		iterFind++;
	}

	return TRUE;
}

void CMauData::SetPlatformIsRent(const BOOL32 bPlatformRent)
{
	AutoLock cAutoLock(&m_cPlatformIsRentLock);
	m_bPlatformRent = bPlatformRent;
}

BOOL32 CMauData::GetPlatformIsRent()
{
	AutoLock cAutoLock(&m_cPlatformIsRentLock);
	return m_bPlatformRent;
}

CMqStatus& CMauData::GetMqStatus()
{
	return m_cMqStatus;
}

u32 CMauData::GetMqConsumerHeartBeatTime() const
{
	return m_dwMqConsumerHeartBeat;
}
void CMauData::SetMqConsumerHeartBeatTime(const u32 dwTime)
{
	m_dwMqConsumerHeartBeat = dwTime;
}

void CMauData::SetIsEduVer(const BOOL32 bEduVer)
{
	m_bEduVer = bEduVer;
}

BOOL32 CMauData::IsEduVer()
{
	return m_bEduVer;
}
BOOL32 CMauData::GetTemplateListByDomain(const s8* pDomainMOID, CTempCacheData * pTempList, u16 &wCount, BOOL32 bIsCheckPublic /*= FALSE*/)
{
	if( NULL == pDomainMOID )
	{
		return FALSE;
	}
	
	AutoLock cAutoLock(&m_cTempCacheDataLock);

	u16 wConfIdx = 0;
	CTempCacheMoidDataMap::iterator iterFind = m_mapTempMoidCacheInfo.lower_bound(pDomainMOID);
	CTempCacheMoidDataMap::iterator iterEnd = m_mapTempMoidCacheInfo.upper_bound(pDomainMOID);

	while (iterFind != iterEnd)
	{
		BOOL32 bMatch = (!bIsCheckPublic || 1 == iterFind->second->GetPublicConf());
		if (bMatch)
		{
			pTempList[wConfIdx] = *iterFind->second;
			wConfIdx++;
		}
		iterFind++;
	}

	
	//CTempCacheDataMap::iterator itLoop = m_mapTempCacheInfo.begin();
	//while( m_mapTempCacheInfo.end() != itLoop && wConfIdx < wCount)
	//{
	//	if( 0 == strncmp( itLoop->second.GetDomainMOID(), pDomainMOID, OPERATION_MAXLEN_MOID ) )
	//	{			
	//		BOOL32 bMatch = (!bIsCheckPublic || 1 == itLoop->second.GetPublicConf());
	//		if (bMatch)
	//		{
	//			pTempList[wConfIdx] = itLoop->second;
	//		}
	//		wConfIdx++;
	//	}
	//	++itLoop;
	//}

	return TRUE;
}

u16 CMauData::GetTemplateCount( void )
{
	AutoLock cAutoLock(&m_cTempCacheDataLock);

	u16 wTempCount = m_wTempConfNum;
	return wTempCount;
}

void CMauData::PrintTempCacheData( void )
{
	AutoLock cAutoLock(&m_cTempCacheDataLock);
	
	for_each(m_mapTempCacheInfo.begin(), m_mapTempCacheInfo.end(),
		std::ptr_fun(Print_Ref<CMauData::CTempCacheDataMap::const_reference>));
	OspPrintf( TRUE, FALSE, "Total TempInfo in memory num<%d>\n", m_mapTempCacheInfo.size() );
}

BOOL32 CMauData::IsConfNumReachUserDomainLimit(const CKdvDomainInfo& cDomainInfo,
	const u32 dwMaxJoinedMt, BOOL32& bReach)
{
	u32 dwSmallConfNum = 0;
	u32 dwLargeConfNum = 0;
	if (!m_cRedisData.GetConfNumByDomain(cDomainInfo.GetDomainMOID(),
		enumLicensedSmallConfNum, dwSmallConfNum))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Get licensed8Size conf num failed\n",
			__FUNCTION__);
		return FALSE;
	}
	if (!m_cRedisData.GetConfNumByDomain(cDomainInfo.GetDomainMOID(),
		enumLicensedLargeConfNum, dwLargeConfNum))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Get licensed192Size conf num failed\n",
			__FUNCTION__);
		return FALSE;
	}
	const u32 dwMaxSmallConfNum = cDomainInfo.GetMaxSmallConfNum();
	const u32 dwMaxLargeConfNum = cDomainInfo.GetMaxLargeConfNum();

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]userdomain, maxSmallConfNum: %u, "
		"currentSmallConfnum: %u, maxLargeConfNum: %u, currentLargeConfNum: %u\n",
		__FUNCTION__, dwMaxSmallConfNum, dwSmallConfNum, dwMaxLargeConfNum, dwLargeConfNum);

	return CheckConfNumIsReachLimit(dwMaxJoinedMt,
		dwSmallConfNum, dwMaxSmallConfNum,
		dwLargeConfNum, dwMaxLargeConfNum, bReach);
}

BOOL32 CMauData::IsConfNumReachPlatformDomainLimit(const CKdvDomainInfo& tDomainInfo,
	const u32 dwMaxJoinedMt, BOOL32& bReach)
{
	const u32 dwPlatformMaxSmallConfNum = m_cPlatformConfNumLimit.GetSmallConfNumLimit();
	const u32 dwPlatformMaxLargeConfNum = m_cPlatformConfNumLimit.GetLargeConfNumLimit();
	u32 dwPlatformSmallConfNum = 0;
	u32 dwPlatformLargeConfNum = 0;
	for (CKdvDomainInfoMap::iterator domainIter = m_mapDomainInfo.begin();
	domainIter != m_mapDomainInfo.end(); ++domainIter)
	{
		const s8* pchDomainMOID = domainIter->second.GetDomainMOID();
		u32 dwLicensedSmallConfNum = 0;
		u32 dwLicensedLargeConfNum = 0;
		if (!GetConfNumByDomain(pchDomainMOID, enumLicensedSmallConfNum,
			dwLicensedSmallConfNum))
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Get licensed8Size conf num failed\n",
				__FUNCTION__);
			return FALSE;
		}
		if (!GetConfNumByDomain(pchDomainMOID, enumLicensedLargeConfNum,
			dwLicensedLargeConfNum))
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Get licensed192Size conf num failed\n",
				__FUNCTION__);
			return FALSE;
		}
		dwPlatformSmallConfNum += dwLicensedSmallConfNum;
		dwPlatformLargeConfNum += dwLicensedLargeConfNum;
	}

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]platformDomain, maxSmallConfNum: %u, "
		"currentSmallConfnum: %u, maxLargeConfNum: %u, currentLargeConfNum: %u\n",
		__FUNCTION__, dwPlatformMaxSmallConfNum, dwPlatformSmallConfNum,
		dwPlatformMaxLargeConfNum, dwPlatformLargeConfNum);

	return CheckConfNumIsReachLimit(dwMaxJoinedMt, 
		dwPlatformSmallConfNum, dwPlatformMaxSmallConfNum,
		dwPlatformLargeConfNum, dwPlatformMaxLargeConfNum, bReach);
}

BOOL32 CMauData::IsConfNumReachLimit(const s8* pchJsonBuf, 
	const CKdvDomainInfo& cDoaminInfo, BOOL32& bReachLimit)
{
	json_t_ptr pjRoot;
	if (!LoadJson(pchJsonBuf, strlen(pchJsonBuf), pjRoot, TRUE))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Load json failed\n", __FUNCTION__);
		return FALSE;
	}
	return IsConfNumReachLimit(pjRoot, cDoaminInfo, bReachLimit);
}

BOOL32 CMauData::IsConfNumReachLimit(const json_t_ptr& pjRoot, const CKdvDomainInfo& cDomainInfo,
	BOOL32& bReachLimit)
{
	const s8* pchMcuIp = NULL;
	u32 dwMaxJoinedMt = 0;
	GetStringFromJson(pjRoot, JSON_CMUIP, &pchMcuIp);
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_MAXJOINEDMT, dwMaxJoinedMt));
	return IsConfNumReachLimit(cDomainInfo, dwMaxJoinedMt, IsOldMcu(pchMcuIp), bReachLimit);
}

BOOL32 CMauData::IsConfNumReachLimit(const CKdvDomainInfo& cDomainInfo,
	const u32 dwMaxJoinedMt, const BOOL32 bOldMcu, BOOL32& bReach)
{
	if (bOldMcu)
	{
		bReach = FALSE;
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]old mcu, uncheck conf num limit\n", __FUNCTION__);
		return TRUE;
	}

	ReturnIfFailed(IsConfNumReachUserDomainLimit(cDomainInfo, dwMaxJoinedMt, bReach));
	if (bReach)
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Reach UserDomainLimit, confScale: %u\n",
			__FUNCTION__, dwMaxJoinedMt);
		return TRUE;
	}
	ReturnIfFailed(IsConfNumReachPlatformDomainLimit(cDomainInfo, dwMaxJoinedMt, bReach));
	return TRUE;
}

BOOL32 CMauData::GetConfListByDomainAndMt(const s8* pchDomainMOID, const s8* pchMtE164,
	ConfList& lstConf, BOOL32 bIsCheckPublic /*= FALSE*/)
{
	if (!m_cRedisData.GetConfListByDomain(pchDomainMOID, pchMtE164, bIsCheckPublic, lstConf))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]get conf list err, moid: %s, mtE164: %s\n",
			__FUNCTION__, pchDomainMOID, pchMtE164);
		return FALSE;
	}
	return TRUE;
}

u32 CMauData::GetConfDetailInfo(const CMauMtInfo& cMtInfo, const s8* pchE164, TConfDetailInfo& tDetailInfo)
{
	CKdvDomainInfo cDomainInfo;
	if( !IsDomainExist(cMtInfo.GetDomainMOID()) 
		|| !GetDomainInfo(cMtInfo.GetDomainMOID(),cDomainInfo) 
		|| !cDomainInfo.IsUsedFlag() )
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetDomainInfo fail\n", __FUNCTION__, cMtInfo.GetDomainMOID());
		return ERR_DOMAIN_INVALID;
	}

	BOOL32 bGetConfDataSucc = m_cRedisData.GetConfInfoByE164(cMtInfo.GetDomainMOID(), pchE164, tDetailInfo);
	if ( bGetConfDataSucc 
		&& strlen(tDetailInfo.m_tNewConfBaseInfo.GetConfE164()) != 0)
	{
		//填写redis上没有的数据
		tDetailInfo.SetConfStyle(CONF_STYLE_OPENING);
		const s8* pchDomainMoid = tDetailInfo.m_tNewConfBaseInfo.GetDomainMOID();
		CKdvDomainInfo cKdvDomainInfo;
		if (GetDomainInfo(pchDomainMoid, cKdvDomainInfo))
		{
			const s8* pchDomainName = cKdvDomainInfo.GetWholeName();
			const s8* pchDomainGuid = cKdvDomainInfo.GetDomainGUID();
			tDetailInfo.m_tNewConfBaseInfo.SetDomainName(pchDomainName);
			tDetailInfo.m_tNewConfBaseInfo.SetDomainGUID(pchDomainGuid);
			return ERR_DB_SUCCESS;
		}
		else
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetDomainInfo fail\n",
				__FUNCTION__, cMtInfo.GetDomainMOID());
			return ERR_DOMAIN_INVALID;
		}
	}
	else if(!bGetConfDataSucc)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetConfInfoByE164 fail\n", __FUNCTION__);
		return ERR_DB_OPR;
	}

	//预约会议
	CRedisBookConf cBookConf;
	bGetConfDataSucc = GetBookConf(pchE164, cBookConf);
	if (bGetConfDataSucc
		&& strlen(cBookConf.GetConfE164()) != 0
		&& 0 == strcmp(cDomainInfo.GetDomainMOID(), cBookConf.GetDomainMOID()))
	{
		//受限账号请求不到非开放预约会议信息
		if (!cMtInfo.GetLimitedFlag() || cBookConf.GetIsPublicConf())
		{
			ConvertStruct(cBookConf, tDetailInfo);
			tDetailInfo.m_tNewConfBaseInfo.SetDomainGUID(cDomainInfo.GetDomainGUID());
			tDetailInfo.m_tNewConfBaseInfo.SetDomainName(cDomainInfo.GetWholeName());
			return ERR_DB_SUCCESS;
		}
		return ERR_DB_OPR;
	}
	else if (!bGetConfDataSucc)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetBookConf fail\n", __FUNCTION__);
		return ERR_DB_OPR;
	}

	//模板
	CTempCacheData cTempCacheData;
	if (GetTemplateInfoByE164(pchE164, cTempCacheData)
		&& 0 == strcmp(cDomainInfo.GetDomainMOID(), cTempCacheData.GetDomainMOID()))
	{
		if (!cMtInfo.GetLimitedFlag() || 0 != cTempCacheData.GetPublicConf())
		{
			cTempCacheData.ToConfDetailInfo(tDetailInfo);
			return ERR_DB_SUCCESS;
		}
		return ERR_DB_OPR;
	}
	return ERR_CONF_NOEXIST;
}

BOOL32 CMauData::GetConfExInfo(const s8* pchE164, TRedisConfInfo& tConfInfo)
{
	return m_cRedisData.GetConfExInfo(pchE164, tConfInfo);
}

BOOL32 CMauData::GetAllConfExInfo(std::list<TRedisConfInfo>& lstConfInfo)
{
	return m_cRedisData.GetAllConfExInfo(lstConfInfo);
}

BOOL32 CMauData::ChangeConfState(const s8* pchDomainMoid, const s8* abyConfE164, u8 byState)
{
	if ((pchDomainMoid == NULL || strlen(pchDomainMoid) == 0)
		|| (abyConfE164 == NULL || strlen(abyConfE164) == 0))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]domain<%s> or confe164<%s> invalid\n",
			__FUNCTION__, pchDomainMoid == NULL ? "NULL" : pchDomainMoid,
			abyConfE164 == NULL ? "NULL" : abyConfE164);
		return FALSE;
	}

	return m_cRedisData.ChangeConfState(pchDomainMoid, abyConfE164, byState);
}

BOOL32 CMauData::DelConf(const s8* pchDomainMoid, const s8* pchConfE164, const s8* pchMcuIp)
{
	if (pchDomainMoid == NULL || strlen(pchDomainMoid) == 0)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]domain<%s> invalid\n",
			__FUNCTION__, pchDomainMoid == NULL ? "NULL" : pchDomainMoid);
		return FALSE;
	}
	if (pchConfE164 == NULL || strlen(pchConfE164) == 0)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]e164<%s> invalid\n", 
			__FUNCTION__, pchConfE164 == NULL ? "NULL" : pchConfE164);
		return FALSE;
	}
	if (pchMcuIp == NULL || strlen(pchMcuIp) == 0)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]mcuip<%s> invalid\n",
			__FUNCTION__, pchMcuIp == NULL ? "NULL" : pchMcuIp);
		return FALSE;
	}
	return m_cRedisData.DelConf(pchDomainMoid, pchConfE164, pchMcuIp);
}

BOOL32 CMauData::GetConfExListByCmuIp(const s8* pchCmuIp, std::list<TRedisConfInfo>& lstConfEx)
{
	return m_cRedisData.GetConfListByCmuIp(pchCmuIp, lstConfEx);
}

BOOL32 CMauData::GetConfExListByCmuIp(const u32 dwCmuIp, std::list<TRedisConfInfo>& lstConfEx)
{
	s8 achIpAddr[MAXLEN_IPADDR + 1] = { 0 };
	const u32 dwNetOrderCmuIp = htonl(dwCmuIp);
	iptostr(&dwNetOrderCmuIp, achIpAddr, sizeof(achIpAddr));
	return m_cRedisData.GetConfListByCmuIp(achIpAddr, lstConfEx);
}

BOOL32 CMauData::GetConfExListByDomain(const s8* pDomainMOID, std::list<TRedisConfInfo>& lstConfInfo)
{
	return m_cRedisData.GetConfListByDomain(pDomainMOID, lstConfInfo);
}

BOOL32 CMauData::GetConfNumByDomain(const s8* pchDomainMOID, const EMConfNumType emConfNumType, u32& dwConfNum)
{
	if( NULL == pchDomainMOID || strlen(pchDomainMOID) == 0)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]domain invalid\n", __FUNCTION__);
		return FALSE;
	}
	return m_cRedisData.GetConfNumByDomain(pchDomainMOID, emConfNumType, dwConfNum);
}

BOOL32 CMauData::SetMaxDomainConfNum(const s8* pchDomainMOID, 
	const u32 dwMaxSmallConfNum, const u32 dwMaxLargeConfNum)
{
	if( NULL == pchDomainMOID )
	{
		return FALSE;
	}
	return m_cRedisData.SetMaxDomainConfNum(pchDomainMOID, dwMaxSmallConfNum, dwMaxLargeConfNum);
}

BOOL32 CMauData::GetCmuConfNum(const s8* pchCmuIp, u32& dwConfNum)
{
	if (pchCmuIp == NULL)
	{
		return FALSE;
	}
	return m_cRedisData.GetCmuConfNum(pchCmuIp, dwConfNum);
}

BOOL32 CMauData::GetCmuConfNum(const u32 dwCmuIp, u32& dwConfNum)
{
	const s8* pchCmuIp = strofip(dwCmuIp);
	return GetCmuConfNum(pchCmuIp, dwConfNum);
}

BOOL32 CMauData::UpdateMonitorAliveTime(const s8* pchE164, const s8* pchIp, const s8* pchPort)
{
	return m_cRedisData.UpdateMonitorAliveTime(pchE164, pchIp, pchPort);
}

BOOL32 CMauData::GetConfMonitorsInfo(const s8* pchE164, vector<TMonitorAliveInfo>& monitorVec)
{
	return m_cRedisData.GetConfMonitorsInfo(pchE164, monitorVec);
}

BOOL32 CMauData::GetConfStopMonitorsJson(const string& strConfE164, string& strStopMonitorsJson)
{
	vector<TMonitorAliveInfo> vecMonitors;
	if (!GetConfMonitorsInfo(strConfE164.c_str(), vecMonitors))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetConfMonitorsInfo<%s> Fail\n", __FUNCTION__, strConfE164.c_str());
		return FALSE;
	}
	if (vecMonitors.empty())
	{
		strStopMonitorsJson.clear();
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]has not monitor\n", __FUNCTION__);
		return TRUE;
	}
	else
	{
		return ConstructStopConfMonitorsJson(strConfE164, vecMonitors, strStopMonitorsJson);
	}
}

BOOL32 CMauData::ConstructStopConfMonitorsJson(const string& strConfE164,
	const std::vector<TMonitorAliveInfo>& vecMonitors, string& strJson)
{
	TRedisConfInfo tRedisConf;
	if (!GetConfExInfo(strConfE164.c_str(), tRedisConf)
		|| !IsRedisHasConfExData(tRedisConf))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetConfExInfo<%s> Fail\n", __FUNCTION__, strConfE164.c_str());
		return FALSE;
	}

	s8 pchJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;
	json_t_ptr pjRoot = NULL;
	ReturnIfFailed(LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE));
	ReturnIfFailed(SetStringToJson(pjRoot, JSON_CONFE164,  strConfE164.c_str()));
	ReturnIfFailed(SetStringToJson(pjRoot, JSON_MSGTYPE,  "CMU_CM_STOPMONITORING_NTF"));
	ReturnIfFailed(SetConfE164ToJsonObj(pjRoot, tRedisConf.GetConfE164()));
	ReturnIfFailed(SetConfNameToJsonObj(pjRoot, tRedisConf.GetConfName()));
	ReturnIfFailed(SetDomainMoidToJsonObj(pjRoot, tRedisConf.GetDomainMOID()));
	ReturnIfFailed(SetMeetingIDToJsonObj(pjRoot, tRedisConf.GetMeetingID()));
	ReturnIfFailed(SetStopMonitorsInfoToJsonObj(pjRoot, vecMonitors));
	ReturnIfFailed(DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen));
	strJson = pchJsonBuf;
	return TRUE;
}

BOOL32 CMauData::GetExpiredMonitors(vector<TMonitorAliveInfo>& vecMonitors)
{
	return m_cRedisData.GetExpiredMonitors(vecMonitors);
}

BOOL32 CMauData::IsConfExist(const s8* pConfE164, BOOL32& bExists)
{
	return m_cRedisData.IsConfExist(pConfE164, bExists);
}

BOOL32 CMauData::IsConfDataExist(const s8* pchConfE164, BOOL32& bExist)
{
	return m_cRedisData.IsConfDataExist(pchConfE164, bExist);
}

BOOL32 CMauData::SetConfData(const TRedisConfInfo& tConfData)
{
	if (strlen(tConfData.GetDomainMOID()) == 0
		|| strlen(tConfData.GetConfE164()) == 0)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]DomainMOID<%s> or confE164<%s> is empty\n",
			__FUNCTION__, tConfData.GetDomainMOID(), tConfData.GetConfE164());
		return FALSE;
	}
	return m_cRedisData.SetConfData(tConfData);
}

BOOL32 CMauData::UpdateConfData(const TRedisConfInfo& tConfData)
{
	return m_cRedisData.UpdateConfData(tConfData);
}

/*=============================================================================
函 数 名： PrintDomainList
功    能： 打印存在内存中的会议信息
算法实现： 
全局变量： 
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::PrintConfCacheData( void )
{
	std::list<TRedisConfInfo> lstConfInfo;
	m_cRedisData.GetAllConfExInfo(lstConfInfo);
	for_each(lstConfInfo.begin(), lstConfInfo.end(), std::mem_fun_ref(&TRedisConfInfo::Print));
}

//--
BOOL32 CMauData::AddBookConf(const CRedisBookConf& conf)
{
	return m_cRedisData.AddBookConf(conf);
}
BOOL32 CMauData::ModBookConf(const CRedisBookConf& conf)
{
	return m_cRedisData.ModBookConf(conf);
}

BOOL32 CMauData::DelBookConf(const s8* pchE164)
{
	return m_cRedisData.DelBookConf(pchE164);
}

BOOL32 CMauData::GetBookConfE164ByMeetingID(const u32 nMeetingID, string& strBookConfE164)
{
	return m_cRedisData.GetBookConfE164ByMeetingID(nMeetingID, strBookConfE164);
}

BOOL32 CMauData::GetBookConf(const s8* pchE164, CRedisBookConf& cBookConf)
{
	return m_cRedisData.GetBookConf(pchE164, cBookConf);
}

BOOL32 CMauData::GetAllBookConf(vector<CRedisBookConf>& bookConfVec)
{
	return m_cRedisData.GetAllBookConf(bookConfVec);
}

BOOL32 CMauData::GetTodayBookConfList(const s8* pchDomain, std::vector<CRedisBookConf>& bookConfVec)
{
	if (pchDomain == NULL || !m_cRedisData.GetBookConfList(pchDomain, bookConfVec))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetBookConfList failed, domain:%s\n", 
			__FUNCTION__, pchDomain == NULL ? "NULL" : pchDomain);
		return FALSE;
	}
	for (vector<CRedisBookConf>::iterator confIter = bookConfVec.begin(); 
	confIter != bookConfVec.end();)
	{
		time_t tCurrentTime = 0;
		time(&tCurrentTime);
		TKdvTime tKdvCurrentTime(&tCurrentTime);
		TKdvTime tBookStartTime(FormatStr2KdvTime(confIter->GetBookStartTime()));
		BOOL32 bMatch = (tKdvCurrentTime.GetDay() == tBookStartTime.GetDay()
			&& tKdvCurrentTime.GetMonth() == tBookStartTime.GetMonth()
			&& tKdvCurrentTime.GetYear() == tBookStartTime.GetYear());
		confIter = (bMatch ? ++confIter : bookConfVec.erase(confIter));
	}
	return TRUE;
}

BOOL32 CMauData::GetTodayBookConfListByMt(const s8* pchDomain, vector<CRedisBookConf>& bookConfVec,
	const s8* pchMtE164, const BOOL32 bLimitMt)
{
	if (pchDomain == NULL || !m_cRedisData.GetBookConfList(pchDomain, bookConfVec))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetBookConfList failed, domain:%s\n",
			__FUNCTION__, pchDomain == NULL ? "NULL" : pchDomain);
		return FALSE;
	}

	for (vector<CRedisBookConf>::iterator confIter = bookConfVec.begin(); 
	confIter != bookConfVec.end();)
	{
		time_t tCurrentTime = 0;
		time(&tCurrentTime);
		TKdvTime tKdvCurrentTime(&tCurrentTime);
		TKdvTime tBookStartTime(FormatStr2KdvTime(confIter->GetBookStartTime()));
		BOOL32 bMatch = (tKdvCurrentTime.GetDay() == tBookStartTime.GetDay()
			&& tKdvCurrentTime.GetMonth() == tBookStartTime.GetMonth()
			&& tKdvCurrentTime.GetYear() == tBookStartTime.GetYear()
			&& (!bLimitMt || confIter->GetIsPublicConf())
			&& (pchMtE164 != NULL && IsMtInConf(pchMtE164, confIter->GetConfDetailJson().c_str())));
		confIter = (bMatch ? ++confIter : bookConfVec.erase(confIter));
	}
	return TRUE;
}

u32 CMauData::GetBookConfList(const CMauMsg& cServMsg, std::vector<CRedisBookConf>& bookConfVec, 
	CKdvDomainInfo* pcDomainInfo/*=NULL*/, BOOL32* pbHasMcu/*=NULL*/)
{
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)cServMsg.GetMsgBody();
	CKdvDomainInfo cDomainInfo;
	if (!IsDomainExist(tReqInfo.GetDomainMOID())
		|| !GetDomainInfo(tReqInfo.GetDomainMOID(), cDomainInfo)
		|| !cDomainInfo.IsUsedFlag())
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[%s] Domain<Name:%s GUID:%s> is not exist or is unused!\n ",
			__FUNCTION__, cDomainInfo.GetWholeName(), cDomainInfo.GetDomainGUID() );
		return (u32)ERR_DOMAIN_INVALID;
	}

	s8* pchBookConfOprJson = (s8*)(cServMsg.GetMsgBody() + sizeof(TReqHeadInfo));
	u32 dwJsonBufLen = strlen(pchBookConfOprJson);
	const u8 byHasXmpuSer = (cDomainInfo.HasXmpuServer() ? 1 : 0);
	SetHasXmpuSerToJson(pchBookConfOprJson, dwJsonBufLen, byHasXmpuSer);

	if (!GetBookConfListFromJson((s8*)pchBookConfOprJson, bookConfVec)
		|| bookConfVec.empty())
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_VC, "[%s]GetBookConfListFromJson err, or conf count is 0!\n ",
			__FUNCTION__ );
		return (u32)ERR_INFO_INVALID;
	}

	if (pcDomainInfo != NULL)
	{
		*pcDomainInfo = cDomainInfo;
	}

	if (pbHasMcu != NULL)
	{
		s8 abyMcuIp[MAXLEN_IPADDR + 1] = { 0 };
		GetCmuIpFromJson(pchBookConfOprJson, abyMcuIp, sizeof(abyMcuIp) - 1);
		*pbHasMcu = (strlen(abyMcuIp) != 0);
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s] hasMcu=%u\n ", __FUNCTION__, *pbHasMcu);
	}
	return 0;
}

void CMauData::CompletedBookConf(const CKdvDomainInfo& cDomainInfo, vector<CRedisBookConf>& bookConfVec)
{
	for (vector<CRedisBookConf>::iterator confIter = bookConfVec.begin();
	confIter != bookConfVec.end(); ++confIter)
	{
		s8 achConfE164[MAXLEN_CONFE164+1] = {0};
		if (MAXLEN_CONFE164 > strlen(confIter->GetConfE164())
			&& ConstructConfE164(cDomainInfo.GetSDomainMOID(), achConfE164))
		{
			confIter->SetConfE164(achConfE164);
			LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]Construct confe164<%s>!\n", __FUNCTION__, achConfE164 );
		}

		s8 achJsonBuf[10240 * 3 + 1] = { 0 };
		strncpy(achJsonBuf, confIter->GetConfDetailJson().c_str(), sizeof(achJsonBuf));
		u32 dwJsonBufLen = strlen(achJsonBuf);
		SetBookConfE164ToJson(achJsonBuf, dwJsonBufLen, confIter->GetMeetingID(), achConfE164);
		confIter->SetConfDetailJson(achJsonBuf);
	}
}

/*=============================================================================
函 数 名： PrintAllPersonalTempE164
功    能： 打印所有个人模板E164号
算法实现：
全局变量：
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2016/04/07 5.0         王力                   创建
=============================================================================*/
void CMauData::PrintAllPersonalTempE164(void)
{
	::OspPrintf(TRUE, FALSE, "---Print All Personal Template E164---\n");
	CPersonalTempInfoMap::iterator it = m_mapPersonalTempinfo.begin();
	for (; it != m_mapPersonalTempinfo.end(); ++it)
	{
		::OspPrintf(TRUE, FALSE, "|E164:		%s\n", it->first.c_str());
	}
}

/*=============================================================================
函 数 名： PrintPersonalTempByE164
功    能： 通过E164号获取个人模板
算法实现：
全局变量：
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2016/04/07 5.0         王力                   创建
=============================================================================*/
void CMauData::PrintPersonalTempByE164(const s8 * pchE164)
{
	::OspPrintf(TRUE, FALSE, "---Print Personal TemplateInfo By E164---\n");
	
	CPersonalTempInfoMap::iterator it = m_mapPersonalTempinfo.find(pchE164);
	if (it != m_mapPersonalTempinfo.end())
	{
		::OspPrintf(TRUE, FALSE, "PersonalTemplatInfo:\n	%s\n", it->second.c_str());
	}
}

/*=============================================================================
函 数 名： FindPersonalTempInfo
功    能： 
算法实现：
全局变量：
参    数： void
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2016/04/09  5.0         王力                    创建
=============================================================================*/
BOOL32 CMauData::FindPersonalTempInfo(const s8 *pchConfE164, string &strJsonBuf)
{
	AutoLock cAutoLock(&m_cPersonalTempLock);

	CPersonalTempInfoMap::const_iterator it = m_mapPersonalTempinfo.find(pchConfE164);
	if (it == m_mapPersonalTempinfo.end())
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[FindPersonalTempInfo] personaltemplate not exist.\n");
		return FALSE;
	}
	else
	{
		strJsonBuf = it->second;
		return  TRUE;
	}
}

BOOL32 CMauData::AddPersonalTempInfoAndUpu(const s8 *pchConfE164,
	const s8* pchJsonBuf,
	const CKdvDomainInfo& cDomainInfo)
{
	AutoLock cAutoLock(&m_cPersonalTempLock);

	m_mapPersonalTempinfo[pchConfE164] = pchJsonBuf;
	UpuAddConf(pchConfE164, TUpuConfInfo::emTempConf, 
		cDomainInfo.GetDomainMOID(), cDomainInfo.GetSDomainMOID());
	return TRUE;
}

BOOL32 CMauData::DelPersonalTempInfoAndUpu(const s8 *pchConfE164)
{
	AutoLock cAutoLock(&m_cPersonalTempLock);

	UpuDelConf(pchConfE164, TUpuConfInfo::emTempConf);
	m_mapPersonalTempinfo.erase(pchConfE164);
	return TRUE;
}

/*=============================================================================
函 数 名： PrintBookConfData
功    能： 打印会议预约信息
算法实现： 
全局变量： 
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::PrintBookConfData(const u8 byPrintParam /*=0*/, const BOOL32 bPrintDetailJson/*=FALSE*/)
{
	if (byPrintParam == 0)
	{
		::OspPrintf(TRUE, FALSE, "---Print All Book Conf---\n");
	}
	else if(byPrintParam == 1)
	{
		::OspPrintf(TRUE, FALSE, "---Print Today Book Conf---\n");
	}
	else
	{
		::OspPrintf(TRUE, FALSE, "---INVALID PRINT PARAM---\n");
		return;
	}
	::OspPrintf(TRUE, FALSE, "---------------------------\n");
	vector<CRedisBookConf> bookConfVec;
	m_cRedisData.GetAllBookConf(bookConfVec);
	for (vector<CRedisBookConf>::const_iterator confIter = bookConfVec.begin();
	confIter != bookConfVec.end(); ++confIter)
	{
		time_t tCurrentTime = time(NULL);
		TKdvTime tKdvBookStartTime = FormatStr2KdvTime(confIter->GetBookStartTime());
		time_t tBookStartTime = 0;
		tKdvBookStartTime.GetTime(tBookStartTime);
		TKdvTime tKdvCurrentTime(&tCurrentTime);
		if ((byPrintParam == 1
			&& tKdvBookStartTime.GetDay() == tKdvCurrentTime.GetDay()
			&& tKdvBookStartTime.GetMonth() == tKdvBookStartTime.GetMonth()
			&& tKdvBookStartTime.GetYear() == tKdvBookStartTime.GetYear())
			|| byPrintParam == 0)
		{
			confIter->Print(bPrintDetailJson);
			::OspPrintf(TRUE, FALSE, "---------------------------\n");
		}
	}
}

//--------------------------------------------------------------
//会议恢复相关
BOOL32 CMauData::GetCreateConfJson(const string& confE164, const BOOL32 bHasXmpu, string& createConfJson)
{
	return m_cRedisData.GetCreateConfJson(confE164, bHasXmpu, createConfJson);
}

BOOL32 CMauData::IsConfExistAndBelongMcu(const std::string& confE164, const string& mcuIp)
{
	TRedisConfInfo tRedisConf;
	if (!GetConfExInfo(confE164.c_str(), tRedisConf) 
		|| !IsRedisHasConfExData(tRedisConf))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]conf<E164:%s> inexistence in redis\n",
			__FUNCTION__, confE164.c_str());
		return FALSE;
	}
	if (strncmp(tRedisConf.GetCmuIp(), mcuIp.c_str(), MAXLEN_IPADDR) != 0)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]ipDiff, cmuIp: %s, redis cmuIp: %s\n",
			__FUNCTION__, mcuIp.c_str(), tRedisConf.GetCmuIp());
		return FALSE;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]conf<E164:%s> belong mcu: %s\n",
		__FUNCTION__, confE164.c_str(), mcuIp.c_str());
	return TRUE;
}

void CMauData::PrintNeedRecorverConf()
{
	list<TRedisConfInfo> tRedisConfList;
	if (GetAllConfExInfo(tRedisConfList))
	{
		for (list<TRedisConfInfo>::iterator confIter = tRedisConfList.begin();
		confIter != tRedisConfList.end(); ++confIter)
		{
			if (confIter->GetState() == enumConnectWaitingState)
			{
				confIter->Print();
				::OspPrintf(TRUE, FALSE, "-------------------------\n");
			}
		}
	}
}

BOOL32 CMauData::CheckRedisData(string& checkResStr)
{
	return m_cRedisData.CheckRedisData(checkResStr);
}

BOOL32 CMauData::StartInitRedisConf()
{
	return m_cRedisData.StartInitRedisConf();
}

void CMauData::SetRecoverTimeSpan(const u32 dwSeconds)
{
	m_dwRecoverTimeSpan = dwSeconds;
}

u32 CMauData::GetRecoverTimeSpan() const
{
	return m_dwRecoverTimeSpan;
}

void CMauData::SetMcuHeartBeatTimeSpan(const u32 dwSeconds)
{
	m_dwMcuHeartBeatTimeSpan = dwSeconds;
}

u32 CMauData::GetMcuHeartBeatTimeSpan() const
{
	return m_dwMcuHeartBeatTimeSpan;
}

void CMauData::SetConfListTimeSpan(const u32 dwSeconds)
{
	m_dwGetConfListTimeSpan = dwSeconds;
}

u32 CMauData::GetConfListTimeSpan() const
{
	return m_dwGetConfListTimeSpan;;
}
//--------------------------------------------------------------

/*=============================================================================
函 数 名： AssignMpcID
功    能： 分配MPC
算法实现： 
全局变量： 
参    数： u32 dwMpcIp 主机序
返 回 值： u16
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::AssignMpcID(const s8* lpIp, TCmuData& cMauData)
{
	AutoLock cMcuListLock(&m_cMcuListLock);

	if (lpIp != NULL && lpIp[0] != 0)
	{
		MapCmuData::iterator it = m_mapCmuData.find(inet_addr(lpIp));
		if (it != m_mapCmuData.end())
		{
			cMauData = it->second;
			return TRUE;
		}
	}
	else
	{
		u32 dwMinConfNum = 0;
		MapCmuData::iterator itTmp = m_mapCmuData.end();
		MapCmuData::iterator it = m_mapCmuData.begin();
		for (; it != m_mapCmuData.end(); ++it)
		{
			u32 dwCmuConfNum = 0;
			bitset<5> mcuStatusBitset;
			if((mcuStatusBitset[0] = (it->second.m_byConnectState == enumMpcConnectState))
				&& (mcuStatusBitset[1] = (!it->second.m_bDisable))
				&& (mcuStatusBitset[2] = (GetCmuConfNum(it->second.GetCmuIp(), dwCmuConfNum)))
				&& (mcuStatusBitset[3] = (dwCmuConfNum < it->second.GetMaxConfNum()))
				&& (mcuStatusBitset[4] = (IsNewMcu(it->second.GetCmuType()))))
			{
				if (itTmp == m_mapCmuData.end())
				{
					itTmp = it;
					dwMinConfNum = dwCmuConfNum;
				}
				else if (dwMinConfNum > dwCmuConfNum)
				{
					itTmp = it;
					dwMinConfNum = dwCmuConfNum;
				}
			}
			else
			{
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]mcu<Ip:%s> can't use to create conf, cause: %s\n"
					"0:cmu not connect\n"
					"1:mcu disable\n"
					"2:getcmuconfnum fail\n"
					"3:mcuConfNum>=McuMaxConfNum\n"
					"4:is not new mcu\n",
					__FUNCTION__, cMauData.GetCmuIp(), mcuStatusBitset.to_string().c_str());
			}
		}
		if (itTmp != m_mapCmuData.end())
		{
			cMauData = itTmp->second;
			return TRUE;
		}
	}
	return FALSE;
}

/*=============================================================================
函 数 名： AddMpc
功    能： 添加MPC
算法实现： 
全局变量： 
参    数： u16 wInstID
TMpcData & tMpcData
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::AddMpc(TCmuData & tCmuData )
{
	tCmuData.m_byConnectState = enumMpcConnectState;
	m_mapCmuData[inet_addr(tCmuData.GetCmuIp())] = tCmuData;
	return TRUE;
}

/*=============================================================================
函 数 名： IsMpcConnect
功    能： 判断MPC是否连接
算法实现： 
全局变量： 
参    数： u16 wInstID
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::IsMpcConnect( const s8* achCmuIp )
{
	AutoLock cMcuListLock(&m_cMcuListLock);

	MapCmuData::iterator it = m_mapCmuData.find(inet_addr(achCmuIp));
	if (it != m_mapCmuData.end())
	{
		return (it->second.m_byConnectState == enumMpcConnectState) ? TRUE : FALSE;
	}
	
    return FALSE;
}

/*=============================================================================
函 数 名： RemoveMpc
功    能： 移除MPC信息
算法实现： 
全局变量： 
参    数： u16 wInstID
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::RemoveMpc(const s8* achCmuIp)
{
	return RemoveMpc(inet_addr(achCmuIp));
}

BOOL32 CMauData::RemoveMpc(u32 dwCmuIp)
{
	AutoLock cMcuListLock(&m_cMcuListLock);

	m_mapCmuData.erase(dwCmuIp);
	return TRUE;
}

/*=============================================================================
函 数 名： GetMpcData
功    能： 获得MPC信息
算法实现： 
全局变量： 
参    数： u16 wMpcId
TMpcData &tMpcData
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::GetMpcData(const s8* achIp, TCmuData &tMpcData)
{
	return GetMpcData(inet_addr(achIp), tMpcData);
}

BOOL32 CMauData::GetMpcData(u32 dwIp, TCmuData &tMpcData)
{
	AutoLock cMcuListLock(&m_cMcuListLock);

	MapCmuData::iterator it = m_mapCmuData.find(dwIp);
	if (it != m_mapCmuData.end())
	{
		tMpcData = it->second;
		return TRUE;
	}
	return FALSE;
}

void CMauData::GetAllMpcData(MapCmuData& mapCmuData)
{
	AutoLock cMcuListLock(&m_cMcuListLock);
	mapCmuData = m_mapCmuData;
}

/*=============================================================================
函 数 名： SetMpcData
功    能： 设置MPC信息
算法实现： 
全局变量： 
参    数： u16 wMpcId
TMpcData &tMpcData
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::SetMpcData(TCmuData &tCmuData)
{
	AutoLock cMcuListLock(&m_cMcuListLock);
	return AddMpc(tCmuData);
}

BOOL32 CMauData::EnableMpc(const s8* achCmuIp, BOOL32 bEnable)
{
	return EnableMpc(inet_addr(achCmuIp), bEnable);
}

BOOL32 CMauData::EnableMpc(u32 dwIp, BOOL32 bEnable)
{
	AutoLock cMcuListLock(&m_cMcuListLock);

	MapCmuData::iterator it = m_mapCmuData.find(dwIp);
	if (it != m_mapCmuData.end())
	{
		it->second.Enable(bEnable);
		return TRUE;
	}
	return FALSE;
}

/*=============================================================================
函 数 名： PrintCmuData
功    能： 打印MPC信息
算法实现： 
全局变量： 
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::PrintCmuData( void )
{
	AutoLock cMcuListLock(&m_cMcuListLock);
	MapCmuData::iterator it = m_mapCmuData.begin();
	for (; it != m_mapCmuData.end(); ++it)
	{
		if (it->second.m_byConnectState == enumMpcDisconnectState)
		{
			continue;
		}
		
		OspPrintf( TRUE, FALSE, "\n" );
		it->second.Print();
	}
}

/*=============================================================================
函 数 名： GetNuData
功    能： 获得Nu信息
算法实现： 
全局变量： 
参    数： u16 wGkId
TGkData &tGkData
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::GetNuData( u16 wNuId, TNuData &tNuData )
{
    if ( 0 == wNuId || wNuId > MAX_NU_NUM )
    {
        return FALSE;
    }
	
    tNuData = m_atNuData[wNuId-1];
    return TRUE;
}

/*=============================================================================
函 数 名： SetNuData
功    能： 设置Nu信息
算法实现： 
全局变量： 
参    数： u16 wGkId
TGkData &tGkData
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::SetNuData( u16 wNuId, TNuData &tNuData )
{
    if ( 0 == wNuId || wNuId > MAX_NU_NUM )
    {
        return FALSE;
    }
    
    m_atNuData[wNuId-1] = tNuData;
    return TRUE;
}

/*=============================================================================
函 数 名： IsConfNameExist
功    能： 会议名存在判断
算法实现： 
全局变量： 
参    数： CKdvDomainInfo tDomainInfo, const s8 *pszConfName
返 回 值： BOOL32 - 如果有重名会议，返回true；反之返回false 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::IsConfNameExist(const s8* pszDomainMOID, const s8 *pszConfName, const s8 *pszConfE164, BOOL32& bExist)
{
	bExist = FALSE;
	if ( NULL == pszConfName || NULL == pszConfE164 )
	{
		return FALSE;
	}
	
	// 记录每次的会议数值
	list<TRedisConfInfo> confList;
	if (!GetConfExListByDomain(pszDomainMOID, confList))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[IsConfNameExist]GetConfExListByDomain Fail\n");
		return FALSE;
	}
	LogPrint( LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[IsConfNameExist]ConfNum=%d.\n", confList.size());

	for (list<TRedisConfInfo>::iterator confIter = confList.begin(); confIter != confList.end(); ++confIter)
	{ 
		if (0 == strncmp( confIter->GetConfName(), pszConfName, MAXLEN_CONFNAME) 
			&& 0 != strncmp( confIter->GetConfE164(), pszConfE164, MAXLEN_CONFE164))
		{	
			LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[IsConfNameExist]pszConfName=%s,e164<%s> is exist.\n", pszConfName, pszConfE164);
			bExist = TRUE;
			return TRUE;
		}
	}
	
	//预约会议
	vector<CRedisBookConf> bookConfVec;
	if (!GetTodayBookConfList(pszDomainMOID, bookConfVec))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[IsConfNameExist]GetTodayBookConfList Fail\n");
		return FALSE;
	}
	for (vector<CRedisBookConf>::const_iterator confIter = bookConfVec.begin();
	confIter != bookConfVec.end(); ++confIter)
	{
		if (0 == strncmp(confIter->GetConfName(), pszConfName, MAXLEN_CONFNAME)
			&& 0 != strncmp(confIter->GetConfE164(), pszConfE164, MAXLEN_CONFE164))
		{
			LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[%s]book :pszConfName=%s,e164<%s> is exist.\n",
				__FUNCTION__, pszConfName, pszConfE164);
			bExist = TRUE;
			return TRUE;
		}
	}
	return TRUE;
}

/*=============================================================================
函 数 名： IsTempNameExist
功    能： 模板名存在判断
算法实现： 
全局变量： 
参    数： CKdvDomainInfo tDomainInfo, const s8 *pszTempName, const s8 *pszTempKey
返 回 值： BOOL32 - 如果有重名模板，返回true；反之返回false 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/22  5.0         rnn                    创建
=============================================================================*/
BOOL32 CMauData::IsTempNameExist(const s8* pszDomainMOID, const s8 *pszTempName, const s8 *pszTempKey, BOOL32& bExist)
{
	bExist = FALSE;
	if ( NULL == pszTempName || NULL == pszTempKey )
	{
		return FALSE;
	}
	
	//获取模板个数
	u16 wTempNum = GetTemplateNumByDomain(pszDomainMOID);
	
	// 记录模板数值
	LogPrint( LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[IsTempNameExist]wTempNum=%d.\n", wTempNum);
	
	if ( 0 != wTempNum )
	{
		CTempCacheData *pacTempData = new CTempCacheData[wTempNum + 1];
		
		//获取终端所在域的会议信息
		GetTemplateListByDomain(pszDomainMOID, pacTempData, wTempNum );
		
		u16 wTempIdx = 0;
		for ( wTempIdx = 0; wTempIdx < wTempNum; wTempIdx++)
		{
			if ( 0 == strncmp( pacTempData[wTempIdx].GetConfName(), pszTempName, MAXLEN_CONFNAME) &&
				0 != strncmp( pacTempData[wTempIdx].GetConfE164(), pszTempKey, MAXLEN_CONFE164))
			{	
				LogPrint( LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[IsTempNameExist]pszTempName=%s, key<%s> is exist.\n", pszTempName, pszTempKey);
				delete [] pacTempData; 
				bExist = TRUE;
				return TRUE;
			}
		}
		delete [] pacTempData; 
	}
	
	//预约会议
	vector<CRedisBookConf> bookConfVec;
	if (!GetTodayBookConfList(pszDomainMOID, bookConfVec))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[IsConfNameExist]GetTodayBookConfList Fail\n");
		return FALSE;
	}
	for (vector<CRedisBookConf>::const_iterator confIter = bookConfVec.begin();
	confIter != bookConfVec.end(); ++confIter)
	{
		if (0 == strncmp(confIter->GetConfName(), pszTempName, MAXLEN_CONFNAME)
			&& 0 != strncmp(confIter->GetConfE164(), pszTempKey, MAXLEN_CONFE164))
		{
			LogPrint( LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[%s]book :pszConfName=%s,e164<%s> is exist.\n",
				__FUNCTION__, pszTempName, pszTempKey);
			bExist = TRUE;
			return TRUE;
		}
	}
	
	return TRUE;
}

/*=============================================================================
函 数 名： GetPlatformPrefix
功    能： 修改域信息
算法实现： 
全局变量： 
参    数： const CKdvDomainInfo & tOldInfo
           const CKdvDomainInfo & tNewInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::GetPlatformPrefix( const s8* pchMoid, s8* pbyPlatformPrefix )
{
	if ( NULL == pbyPlatformPrefix )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetPlatformPrefix]PlatformPrefix is NULL, return.\n" );	
		return FALSE;
	}

	LogPrint( LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[GetPlatformPrefix]DomainMOID<%s>!\n", pchMoid );

	AutoLock cAutoLock(&m_cDomainListLock);

	CKdvDomainInfoMap::iterator it = m_mapDomainInfo.find(pchMoid);
	if( m_mapDomainInfo.end() != it )
	{
		memcpy( pbyPlatformPrefix, it->second.GetPlatformPrefix(), OPERATION_MAXLEN_PLATFORMPREFIX );
		return TRUE;
	}

	return FALSE;
}

/*=============================================================================
函 数 名： GetPlatformPrefix
功    能： 修改域信息
算法实现： 
全局变量： 
参    数： s8* psPlatformPrefix
		   CKdvDomainInfo & cPlatDomainInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::GetPlatDomainInfoByPrefix( s8* psPlatformPrefix, CKdvDomainInfo & cPlatDomainInfo )
{
	if ( NULL == psPlatformPrefix )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetPlatDomainInfoByPrefix]PlatformPrefix is NULL, return.\n" );	
		return FALSE;
	}
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[GetPlatDomainInfoByPrefix]PlatformPrefix<%s>!\n", psPlatformPrefix );
	
	AutoLock cAutoLock(&m_cDomainListLock);

	CKdvDomainInfoMap::iterator it = m_mapDomainInfo.begin();
	for (; it != m_mapDomainInfo.end(); it++)
	{
		if( CKdvDomainInfo::em_KdvPlatformDomain == it->second.GetDomainLevel()
			&& 0 == strncmp( it->second.GetPlatformPrefix(), psPlatformPrefix, OPERATION_MAXLEN_PLATFORMPREFIX ) )
		{
			cPlatDomainInfo = it->second;
			return TRUE;
		}
	}
	
	return FALSE;
}

/*=============================================================================
函 数 名： ConstructConfE164
功    能： 构造完整的会议E164
算法实现： 
全局变量： 
参    数： const CKdvDomainInfo & tInfo
           s8 * pConfE164
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::ConstructConfE164( const s8* pPlatformDomain, s8* pConfE164 )
{
	if(NULL == pPlatformDomain || NULL == pConfE164 )
	{
		return FALSE;
	}
	s8 achE164Prex[MAXLEN_CONFE164+1] = {0};
    //	 XXX	  NNNN 
    //  ------   ------
    //  平台前缀   流水号
	GetPlatformPrefix( pPlatformDomain, achE164Prex );
	achE164Prex[OPERATION_MAXLEN_PLATFORMPREFIX] = '\0';

	std::string strE164;
	BOOL32 bResult = m_cRedisData.ConstructE164(achE164Prex, "0", "8999", strE164);
	if (bResult && !strE164.empty())
	{
		strcpy(pConfE164, strE164.c_str());
		return TRUE;
	}
	return FALSE;
}


/*=============================================================================
函 数 名： ConstructTemplateE164
功    能： 构造模板E164
算法实现： 
全局变量： 
参    数： const CKdvDomainInfo & tInfo
           s8 * pConfE164
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::ConstructTemplateE164( const s8* pPlatformDomain, s8* pConfE164 )
{
	if( NULL == pPlatformDomain || NULL == pConfE164 )
	{
		return FALSE;
	}

	//	 XXX	  NNNN 
	//  ------   ------
	//  平台前缀   流水号
	s8 achE164Prex[MAXLEN_CONFE164+1] = {0};
	GetPlatformPrefix(pPlatformDomain, achE164Prex );
	achE164Prex[MAXLEN_CONFE164] = '\0';

	//对平台前缀要进行重新设置，以便与会议E164号进行区分
	if (strcmp(achE164Prex, "000") == 0)
	{
		memcpy(achE164Prex, "001", 3);
	}
	else
	{
		memcpy(achE164Prex, "000", 3);
	}

	std::set<u32> setSerialNo;

	CTempCacheDataMap::iterator itTmpLoop = m_mapTempCacheInfo.begin();
	while( m_mapTempCacheInfo.end() != itTmpLoop )
	{
		s8 *pachConfE164 = (s8 *)itTmpLoop->second->GetConfE164();
		if( 0 == strncmp( achE164Prex, pachConfE164, OPERATION_MAXLEN_PLATFORMPREFIX ))
		{
			u32 dwSN = itTmpLoop->second->GetTempSerialCode();
			setSerialNo.insert(dwSN);
		}
		++itTmpLoop;
	}

	if (setSerialNo.size() == OPERATION_MAXTEMPSERIALNO)
	{/*E164号完全被占用，不分配E164号*/
		return FALSE;
	}
	u32 dwAsignSN = m_dwTempE164Backup;
	if (1 == dwAsignSN)
	{/*如果上一个使用的模板E164号是1，那么下一个将要判断的合适的模板E164是最大值*/
		dwAsignSN = OPERATION_MAXTEMPSERIALNO;
	}
	else
	{/*下一个将要判断的合适的模板E164是是上一个使用的减1*/
		--dwAsignSN;
	}



	if( setSerialNo.empty() )
	{/*如果当前没有模板时，直接使用这次轮流到的模板E164*/
		m_dwTempE164Backup = dwAsignSN;
	}
	else
	{
		std::set<u32>::iterator iterFind = setSerialNo.find(dwAsignSN);
		if( iterFind == setSerialNo.end() )
		{/*该模板E164号没有被使用，直接使用*/
			m_dwTempE164Backup = dwAsignSN;
		}
		else
		{
			if (dwAsignSN == *setSerialNo.begin())
			{/*该模板E164号是正好是当前会议模板列表的最小的E164号*/
				--dwAsignSN;
			}
			else
			{
				std::set<u32>::iterator iter = iterFind;
				for (; iter != setSerialNo.end(); --iter )
				{
					if (*iter == dwAsignSN)
					{/*如果当前模板E164被占用，取下一个将要轮流的E164*/
						--dwAsignSN;
					}
					if (dwAsignSN > *iter)
					{/*当前轮流的模板E164比当前要比较的会议E164号大，说明符合条件*/
						break;
					}
					if (iter == setSerialNo.begin())
					{/*会议模板中的E164全部被占用，取最小的E164号减1*/
						if (dwAsignSN == *setSerialNo.begin())
						{
							--dwAsignSN;
						}
						break;
					}

				}	
			}


			if (0 == dwAsignSN)
			{/*从最开始的最大值9999开始循环*/
				dwAsignSN =  OPERATION_MAXTEMPSERIALNO;
				if (*setSerialNo.rbegin() == OPERATION_MAXTEMPSERIALNO)
				{/*最大值9999被占用，向下寻找最近的没有被占用的E164号*/
					std::set<u32>::iterator iter2 = --setSerialNo.end();
					std::set<u32>::iterator iterNext2;
					for (; iter2 != iterFind; --iter2 )
					{
						iterNext2 = iter2;
						--iterNext2;
						if (*iter2 - *iterNext2 > 1)
						{
							dwAsignSN = *iter2 - 1;
							break;
						}    
					}
				}
			}
			m_dwTempE164Backup = dwAsignSN;
		}
	}

	sprintf( pConfE164, "%s%04d", achE164Prex, dwAsignSN );
	pConfE164[MAXLEN_CONFE164] = '\0';

	return TRUE;
}

/*=============================================================================
函 数 名： IsTemplateExist
功    能： 判断模板是否存在
算法实现： 
全局变量： 
参    数： const CKdvDomainInfo & tInfo
           s8 * pConfE164
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::IsTemplateExist( const s8 *pszTemplateE164 )
{
	BOOL32 bRet = FALSE;
	if ( NULL == pszTemplateE164 )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[IsTemplateExist] Param is NULL, return FLASE!\n" );
		return bRet;
	}

	AutoLock cAutoLock(&m_cTempCacheDataLock);
	bRet = (m_mapTempCacheInfo.find(pszTemplateE164) != m_mapTempCacheInfo.end());

	return bRet;
}

/*=============================================================================
函 数 名： ConfNameRepeatProc
功    能： 会议重名处理
算法实现： 
全局变量： 
参    数： CKdvDomainInfo tDomainInfo, const s8 *pszConfNameSrc, s8 *pszConfNameBuffer, s32 nBufferLen
返 回 值： BOOL32 - 如果修改了会议名，返回true；反之返回false 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::ConfNameRepeatProc(CKdvDomainInfo tDomainInfo, const s8 *pszConfNameSrc,
	const s8 *pszConfE164, s8 *pszConfNameBuffer, u32 nBufferLen, BOOL32& bModify)
{
	bModify = FALSE;
	BOOL32 bExist = FALSE;
	if(NULL == pszConfNameSrc || NULL == pszConfE164 || NULL == pszConfNameBuffer)
	{
		return FALSE;
	}
	
	//存储原会议名
	s8 achConfName[MAXLEN_CONFNAME + 1] = {0};

	//判断是检测会议还是模板重名
	s8 achE164Prex[MAXLEN_CONFE164+1] = {0};
	if (!GetPlatformPrefix(tDomainInfo.GetSDomainMOID(), achE164Prex))
	{
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[ConfNameRepeatProc]GetPlatformPreFix<%s> Fail\n", 
			tDomainInfo.GetDomainMOID());
		return FALSE;
	}
	achE164Prex[MAXLEN_CONFE164] = '\0';
	//如果e164前6位与域gk前缀相同，则是会议，否则是模板
	if ( 0 == strncmp( achE164Prex, pszConfE164, OPERATION_MAXLEN_PLATFORMPREFIX ) )
	{
		if (!IsConfNameExist(tDomainInfo.GetDomainMOID(), pszConfNameSrc, pszConfE164, bExist))
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[ConfNameRepeatProc]IsConfNameExist Fail\n");
			return FALSE;
		}
	}
	else
	{
		if (!IsTempNameExist(tDomainInfo.GetDomainMOID(), pszConfNameSrc, pszConfE164, bExist))
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[ConfNameRepeatProc]IsTempNameExist Fail\n");
			return FALSE;
		}
	}

	//会议名存在
	if ( TRUE == bExist)
	{
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[ConfNameRepeatProc]pszConfNameSrc=%s is exist.\n", pszConfNameSrc);
		
		//此层循环用于循环数字后缀
		for(s32 nIdxNum = 1; nIdxNum <= 999; nIdxNum++)
		{
			memset(achConfName, 0, sizeof(achConfName));
			sprintf(achConfName, "%s%d", pszConfNameSrc, nIdxNum);
			if ( 0 == strncmp( achE164Prex, pszConfE164, OPERATION_MAXLEN_PLATFORMPREFIX ) )
			{
				if (!IsConfNameExist(tDomainInfo.GetDomainMOID(), achConfName, pszConfE164, bExist))
				{
					LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[ConfNameRepeatProc]IsConfNameExist Fail\n");
					return FALSE;
				}
			}
			else
			{
				if (!IsTempNameExist(tDomainInfo.GetDomainMOID(), achConfName, pszConfE164, bExist))
				{
					LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[ConfNameRepeatProc]IsTempNameExist Fail\n");
					return FALSE;
				}
			}		
			if (FALSE == bExist)
			{
				LogPrint( LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[ConfNameRepeatProc] conf name as<%s> not exist.\n", achConfName );
				break;
			}
			else
			{
				LogPrint( LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[ConfNameRepeatProc] conf name<%s> is exist.keep ++.\n", achConfName );
			}
		}
		
		//处理完成的会议名拷贝返回
		if ( strlen(achConfName) > nBufferLen )
		{
			//长度不足
			LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[ConfNameRepeatProc]Conf Name Out Buffer Len is not enough.\n");
			return FALSE;
		}
		else
		{
			memcpy(pszConfNameBuffer, achConfName, strlen(achConfName));
			bModify = TRUE;
			LogPrint( LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[ConfNameRepeatProc] Replace conf name as : %s.\n", pszConfNameBuffer );
			return TRUE;
		}
	}
	else
	{
		strncpy(pszConfNameBuffer, pszConfNameSrc, nBufferLen);
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[ConfNameRepeatProc]pszConfNameSrc=%s not exist.can use.\n", pszConfNameSrc);
		//无重名不处理
		return TRUE;
	}	
}

/*=============================================================================
函 数 名： SendMsgToMauVC
功    能： 向MAUVC发送消息
算法实现： 
全局变量： 
参    数： 
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::SendMsgToMauVC(u16 wEvent, const void *pbyMsg, u16 wLen, u32 dwSrcIID)
{
	return ::OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), wEvent, pbyMsg, wLen, 0, dwSrcIID);
}

BOOL32 CMauData::SendMsgToCmuSsn(const s8* pchCmuIp, u16 wEvent, const void*  pbyMsg, u16 wLen)
{
	return ::OspPost(pchCmuIp, strlen(pchCmuIp), AID_MAU_CMUSSN_APPID, wEvent, pbyMsg, wLen);
}

BOOL32 CMauData::NoticeCmuSsnAddMt(const s8* pchCmuIp, const TReqHeadInfo& tReqInfo)
{
	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	u32 dwJsonBufLen = 0;
	SetMsgTypeToJson( achJsonBuf, dwJsonBufLen, EV_MAU_MCU_ADDMT_REQ );
	SetConfE164ToJson( achJsonBuf, dwJsonBufLen, tReqInfo.GetConfE164() );
	SetReqHeadToJson( achJsonBuf, dwJsonBufLen, tReqInfo );
	SetAddMtTypeToJson(achJsonBuf, dwJsonBufLen, ADD_MT_TYPE_MT);
	CMauData::SendMsgToCmuSsn(pchCmuIp, MAU_MCU_ADDMT_REQ, achJsonBuf, dwJsonBufLen);
	return TRUE;
}

/*=============================================================================
函 数 名： SendMsgToNuSsn
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::SendMsgToNuSsn( u16 wNuID, u16 wEvent, const void *pbyMsg, u16 wLen )
{
	if( 0 >= wNuID || MAX_NU_NUM <= wNuID )
	{
		return FALSE;
	}
	else
	{
		::OspPost( MAKEIID(AID_MAU_NUSSN_APPID,wNuID), wEvent, pbyMsg, wLen );
		return TRUE;
	}
}

/*=============================================================================
函 数 名： SendMsgToCmSsn
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::SendMsgToCmSsn( u16 wEvent, const void *pbyMsg /* = NULL */, u16 wLen /* = 0  */)
{
	::OspPost( MAKEIID( AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON ), wEvent, pbyMsg, wLen );

	return TRUE;
}

/*=============================================================================
函 数 名： SendMsgToCmCmuSsn
功    能： 向CMCMU发送消息
算法实现： 
全局变量： 
参    数： 
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/26  5.0         rnn                    创建
=============================================================================*/
BOOL32 CMauData::SendMsgToCmCmuSsn( u16 wEvent, const void *pbyMsg, u16 wLen )
{
	OspPost( MAKEIID(AID_MAU_CMCMUSSN_APPID, CInstance::DAEMON), wEvent, pbyMsg, wLen );
	
	return TRUE;
}

/*=============================================================================
函 数 名： SendMsgToHduSsn
功    能： 向hdussn发送消息
算法实现： 
全局变量： 
参    数： 
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/26  5.0         rnn                    创建
=============================================================================*/
BOOL32 CMauData::SendMsgToHduSsn( u16 wEvent, const void* pbyMsg, u16 wLen )
{
	OspPost( MAKEIID(AID_MAU_HDUSSN_APPID, CInstance::DAEMON), wEvent, pbyMsg, wLen );
	
	return TRUE;
}

/*=============================================================================
函 数 名： StructTranslate
功    能： 终端召集会议的结构体转为会议结构体
算法实现： 
全局变量： 
参    数： TMiniConfInfo & tMiniConfInfo
TConfInfo & tCOnfInfo
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::StructTranslate( TMiniConfInfo & tMiniConfInfo, TConfInfo & tConfInfo )
{
	BOOL32 bNeedPwd = GetNeedPswFlag(tMiniConfInfo.GetConfPwd());

	tConfInfo.SetConfId( tMiniConfInfo.GetConfId() );
	tConfInfo.SetDuration( tMiniConfInfo.GetDuration() );
	tConfInfo.SetBitRate( tMiniConfInfo.GetBitRate() );
	tConfInfo.SetConfName( tMiniConfInfo.GetConfName() );
	tConfInfo.SetMediaKey( tMiniConfInfo.GetMediaKey() );
	tConfInfo.SetCapSupport( tMiniConfInfo.GetCapSupport() );
	//设置主席终端
	tConfInfo.SetChairAlias(tMiniConfInfo.GetChairmanAlias());

	//设置发言人终端
	tConfInfo.SetSpeakerAlias(tMiniConfInfo.GetSpeakerAlias());

	tConfInfo.SetConfE164(tMiniConfInfo.GetConfE164());
	tConfInfo.SetTalkHoldTime((u8)tMiniConfInfo.GetMiniMixParam().GetVacinterval());
	TConfStatus tConfStatus(tConfInfo.GetStatus());
	tConfStatus.SetMixerMode(tMiniConfInfo.GetMiniMixParam().GetMixMode());
	tConfStatus.SetCallTimes(tMiniConfInfo.GetCallTimes());
	tConfStatus.SetCallInterval(tMiniConfInfo.GetCallInterval());
	tConfInfo.SetStatus(tConfStatus);
	
	TConfAttrb tConfAttrb;
	tConfAttrb.SetOpenMode( tMiniConfInfo.GetOpenMode() );
	tConfAttrb.SetNeedConfPwd(bNeedPwd);
	tConfAttrb.SetEncryptMode( tMiniConfInfo.GetEncryptMode() );
	tConfAttrb.SetReleaseMode( tMiniConfInfo.IsReleaseNoMt() );
	tConfAttrb.SetSupportCascade( tMiniConfInfo.IsSupportCascade() );
	tConfAttrb.SetDiscussConf( tMiniConfInfo.IsDiscussConf() );
	tConfAttrb.SetAllInitDumb( tMiniConfInfo.IsAllInitDumb() );
	tConfAttrb.SetDualMode( tMiniConfInfo.GetDualMode());
	tConfAttrb.SetSatDCastMode( tConfInfo.GetConfAttrb().IsSatDCastMode() );
	tConfAttrb.SetPublicConf( tMiniConfInfo.GetPublicConf() );
	tConfAttrb.SetConfNoDisturb( tMiniConfInfo.IsCloseModeConf() );
	tConfAttrb.SetHasMixModule(TRUE);
	tConfAttrb.SetDiscussConf(tMiniConfInfo.IsDiscussConf());
	tConfAttrb.SetHasVmpModule(TRUE);
	tConfAttrb.SetHasPollModule(TRUE);
	tConfAttrb.SetPortConfFlag(tMiniConfInfo.IsPortModeConf());
	tConfAttrb.SetUniformMode(tMiniConfInfo.IsOnereforming() ? CONF_UNIFORMMODE_VALID : CONF_UNIFORMMODE_NONE);
	tConfAttrb.SetMaxJoinedMtNum(tMiniConfInfo.GetMaxJoinMtNum());
	tConfAttrb.SetQualityPri(tMiniConfInfo.GetVideoMode());
	tConfInfo.SetConfAttrb( tConfAttrb );

	// 终端发来的密码需要用MD5加密
	if (TRUE == bNeedPwd)
	{
		CMD5Encrypt md5cls;
		s8 achOldPwd[MAXLEN_PWD + 1] = { 0 };
		memcpy(achOldPwd, tMiniConfInfo.GetConfPwd(), MAXLEN_PWD);
		s8 achMD5Pwd[MAXLEN_PWD + 1] = { 0 };
		md5cls.GetEncrypted(achOldPwd, achMD5Pwd);
		tConfInfo.SetConfPwd(achMD5Pwd);
	}
}


/*=============================================================================
函 数 名： GetPollModuleFromJson
功    能： Json转换为TPollModule
算法实现：
全局变量：
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::GetPollModuleFromJson(const json_t_ptr& pjRoot, TMiniPollInfo &tPollModule)
{
	if (!pjRoot)
	{
		OspPrintf(TRUE, FALSE, "[GetPollModuleFromJson] json_loads err!\n");
		return FALSE;
	}
	u32 dwPollnum = 0;
	u16 wKeeptime = 0;

	json_t_ptr pjPollModule = json_object_get_s(pjRoot, JSON_POLLMODULE);
	if (NULL == pjPollModule)
	{
		OspPrintf(TRUE, FALSE, "[GetPollModuleFromJson] json_object_get PollModule is NUll, return FALSE!\n");
		return FALSE;
	}

	BOOL32 bIsHasPollModule = (u8)atoi(json_string_value(pjPollModule)) == 0 ? FALSE : TRUE;
	if (bIsHasPollModule)
	{
		json_t_ptr pjPollParam = json_object_get_s(pjRoot, JSON_POLLPARAM);
		if (NULL == pjPollParam)
		{
			OspPrintf(TRUE, FALSE, "[GetPollModuleFromJson] json_object_get PollParam is NUll, return FALSE!\n");
			return FALSE;
		}

		json_t_ptr pjPollMode = json_object_get_s(pjPollParam, JSON_POLLMODE);   //轮询模式
		if (NULL == pjPollMode)
		{
			OspPrintf(TRUE, FALSE, "[GetPollModuleFromJson] json_object_get PollMode is NUll!\n");
		}
		else
		{
			tPollModule.SetMediaMode((u8)atoi(json_string_value(pjPollMode)));
		}

		json_t_ptr pjPollNum = json_object_get_s(pjPollParam, JSON_POLLNUM);   //轮询次数
		if (NULL == pjPollNum)
		{
			OspPrintf(TRUE, FALSE, "[GetPollModuleFromJson] json_object_get PollNum is NUll!\n");
		}
		else
		{
			dwPollnum = (u32)strtoul(json_string_value(pjPollNum), 0, 10);
			tPollModule.SetPollNum(dwPollnum);
		}

		json_t_ptr pjKeepTime = json_object_get_s(pjPollParam, JSON_POLLKEEPTIME);   //轮询间隔
		if (NULL == pjKeepTime)
		{
			OspPrintf(TRUE, FALSE, "[GetPollModuleFromJson] json_object_get KeepTime is NUll, return FALSE!\n");
		}
		else
		{
			wKeeptime = (u16)atoi(json_string_value(pjKeepTime));
			tPollModule.SetKeepTime(wKeeptime);
		}

		json_t_ptr pjPollMemValueArray = json_object_get_s(pjPollParam, JSON_POLLMT);
		if (NULL == pjPollMemValueArray)
		{
			OspPrintf(TRUE, FALSE, "[GetPollModuleFromJson] json_object_get PollMemValueArray is NUll, return FALSE!\n");
			return FALSE;
		}

		u32 dwPollMemNum = (u32)json_array_size(pjPollMemValueArray);
		u32 dwPollLoop = 0;
		u32 dwValidIdx = 0;
		for (dwPollLoop = 0; dwPollLoop < dwPollMemNum; dwPollLoop++)
		{
			json_t_ptr pjPollMemValue = json_array_get_s(pjPollMemValueArray, dwPollLoop);
			if (NULL == pjPollMemValue)
			{
				OspPrintf(TRUE, FALSE, "[GetPollModuleFromJson] json_array_get<Idx.%d> PollMemValue is NUll, continue!\n", dwPollLoop);
				continue;
			}

			json_t_ptr pjPollIndex = json_object_get_s(pjPollMemValue, JSON_POLLIDX);
			if (NULL == pjPollIndex)
			{
				OspPrintf(TRUE, FALSE, "[GetPollModuleFromJson] json_object_get<Idx.%d> PollIndex is NUll!\n", dwPollLoop);
				continue;
			}
			if (MAXNUM_CONF_MT < (u32)strtoul(json_string_value(pjPollIndex), 0, 10))
			{
				continue;
			}

			tPollModule.SetPollMemIdx((u8)dwValidIdx, (u8)atoi(json_string_value(pjPollIndex)));
			dwValidIdx++;
		}
	}
	return TRUE;
}

void CMauData::SetReqRspToJsonObj(const json_t_ptr& pjRoot, const TReqHeadInfo &tReqHeadInfo)
{
	SetConfE164ToJsonObj(pjRoot, tReqHeadInfo.GetConfE164());
	SetConfNameToJsonObj(pjRoot, tReqHeadInfo.GetConfName());
	SetDomainMoidToJsonObj(pjRoot, tReqHeadInfo.GetDomainMOID());
	SetMeetingIDToJsonObj(pjRoot, tReqHeadInfo.GetMeetingID());
}

/*=============================================================================
函 数 名： SetBookConfE164ToJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/21  5.0         rnn                   创建
=============================================================================*/
void CMauData::SetBookConfE164ToJson( s8 * pchJsonBuf, u32 &dwJsonBufLen, const s32 nMeetingId, const s8 * pchE164 )
{
	json_t_ptr pjRoot = NULL;
	if (!LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE)) return;
	
	json_t_ptr pjConfArray = json_object_get_s( pjRoot, JSON_CONFLIST );
	if ( NULL == pjConfArray )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[SetBookConfE164ToJson] json_object_get pjConfArray is NULL!\n" );
		return;
	}
	
	u16 wBookConfNum = 0;
	u16 wMtIdx = 0;
	wBookConfNum = json_array_size( pjConfArray );	
	for ( wMtIdx = 0; wMtIdx < wBookConfNum; wMtIdx++ )
	{
		json_t_ptr pjConfInfo = json_array_get_s( pjConfArray, wMtIdx );
		if ( NULL == pjConfInfo )
		{
			continue;
		}
		
		json_t_ptr pjMeetingId = json_object_get_s( pjConfInfo, JSON_MEETINTID );
		if ( NULL == pjMeetingId )
		{
			continue;
		}
		
		s32 nBookId = 0;
		nBookId = atoi( json_string_value( pjMeetingId ) );
		if ( nMeetingId != nBookId )
		{
			continue;
		}
		else
		{
			json_t_ptr pjConfE164 = json_string( pchE164 );
			if ( NULL == pjConfE164 )
			{
				continue;
			}
			json_object_set_s( pjConfInfo, JSON_CONFE164, pjConfE164 );
			break;
		}
	}

	DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen);
}

/*=============================================================================
函 数 名： SetDelBookConfToJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/30  5.0         rnn                   创建
=============================================================================*/
void CMauData:: SetDelBookConfToJson( s8 * pchJsonBuf, u32 &dwJsonBufLen, CBookInfo * pcBookInfo, u16 wBookNum )
{
	if ( NULL == pchJsonBuf || NULL == pcBookInfo || 0 == wBookNum )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[SetDelBookConfToJson] Param is NULL, return!\n" );
		return;
	}
	
	json_t_ptr pjRoot = NULL;
	if (!LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE)) return;
	
	json_t_ptr pjConfInfoArray = json_array();
	if ( NULL == pjConfInfoArray )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[SetDelBookConfToJson] json_array pjConfInfoArray is NULL, return!\n" );
		return;
	}
	
    s8 achTmpBuf[256] = {0};
	
	u16 wConfIdx = 0;
	for ( wConfIdx = 0; wConfIdx < wBookNum; wConfIdx++ )
	{
		json_t_ptr pjConfInfo = json_object();
		if ( NULL == pjConfInfo )
		{
			continue;
		}
		
		json_t_ptr pjConfE164 = json_string( pcBookInfo[wConfIdx].GetConfE164() );
		if ( NULL == pjConfE164 )
		{
			continue;
		}
		json_object_set_s( pjConfInfo, JSON_CONFE164, pjConfE164 );
		
		memset( achTmpBuf, 0, sizeof( achTmpBuf ) );
		sprintf( achTmpBuf, "%d", pcBookInfo[wConfIdx].GetMeetingID() );	
		json_t_ptr pjMeetingId = json_string( achTmpBuf );
		if ( NULL == pjMeetingId )
		{
			continue;
		}
		json_object_set_s( pjConfInfo, JSON_MEETINTID, pjMeetingId );

		json_t_ptr pjStartTime = json_string( pcBookInfo[wConfIdx].GetStartTime() );
		if ( NULL == pjStartTime )
		{
			continue;
		}
		json_object_set_s( pjConfInfo, JSON_CONFSTARTTIME, pjStartTime );

		json_t_ptr pjEndTime = json_string( pcBookInfo[wConfIdx].GetStartTime() );
		if ( NULL == pjEndTime )
		{
			continue;
		}
		json_object_set_s( pjConfInfo, JSON_CONFENDTIME, pjEndTime );
		
		json_array_append_s( pjConfInfoArray, pjConfInfo );
	}	
	
	json_object_set_s( pjRoot, JSON_CONFLIST, pjConfInfoArray );
	
	DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen);
}

void CMauData::SetHduInfoToJsonObj(const json_t_ptr& pjRoot, CHduInfo * pcHduInfo, u16 wHduNum)
{
	u16 wHduIdx = 0;
	json_t_ptr pjHduInfoArray = json_array();
	if (NULL == pjHduInfoArray)
	{
		LogPrint(LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[SetHduInfoToJson] json_array pjHduInfoArray is NULL, return!\n");
	}
	else
	{
		for (wHduIdx = 0; wHduIdx < wHduNum; wHduIdx++)
		{
			json_t_ptr pjHduInfo = json_object();
			if (NULL == pjHduInfo)
			{
				continue;
			}

			//hduid
			SetUIntAsStringToJson(pjHduInfo, JSON_HDUID, pcHduInfo[wHduIdx].GetEqpId());
			//isonline
			SetBoolAsStringToJson(pjHduInfo, JSON_HDUISONLINE, pcHduInfo[wHduIdx].IsOnline());
			//chnnum
			SetUIntAsStringToJson(pjHduInfo, JSON_HDUCHNNUM, pcHduInfo[wHduIdx].GetChnNum());
			//hdualias
			SetStringToJson(pjHduInfo, JSON_HDUALIAS, pcHduInfo[wHduIdx].GetAlias());
			//chninfo
			json_t_ptr pjHduChnArray = json_array();
			if (NULL == pjHduChnArray)
			{
				LogPrint(LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[SetHduInfoToJson] json_array HduChnArray is NULL, return!\n");
			}
			else
			{
				u8 byHduChnIdx = 0;
				for (byHduChnIdx = 0; byHduChnIdx < MAXNUM_HDUEQP_CHANNEL; byHduChnIdx++)
				{
					json_t_ptr pjHduChnInfo = json_object();
					if (NULL == pjHduChnInfo)
					{
						continue;
					}

					//chnnidx
					SetUIntAsStringToJson(pjHduChnInfo, JSON_HDUCHLIDX, byHduChnIdx);
					//issilence
					SetBoolAsStringToJson(pjHduChnInfo, JSON_HDUCHNISSILENCE, pcHduInfo[wHduIdx].GetIsMute(byHduChnIdx));
					//volume
					SetUIntAsStringToJson(pjHduChnInfo, JSON_VOLUME, pcHduInfo[wHduIdx].GetVolume(byHduChnIdx));
					//chnstatus
					SetUIntAsStringToJson(pjHduChnInfo, JSON_HDUCHNSTATUS, pcHduInfo[wHduIdx].GetChnStatus(byHduChnIdx));
					json_array_append_s(pjHduChnArray, pjHduChnInfo);
				}

				json_object_set_s(pjHduInfo, JSON_HDUCHNINFO, pjHduChnArray);
			}

			json_array_append_s(pjHduInfoArray, pjHduInfo);
		}

		json_object_set_s(pjRoot, JSON_HDULIST, pjHduInfoArray);
	}
}

/*=============================================================================
函 数 名： SetAllHduInfoToJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/30  5.0         rnn                   创建
=============================================================================*/
void CMauData::SetAllHduInfoToJson( s8 * pchJsonBuf, u32 &dwJsonBufLen, u8 * pbyMsg, u16 wMoIdNum )
{
	if ( NULL == pchJsonBuf || NULL == pbyMsg || 0 == wMoIdNum )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[SetAllHduInfoToJson] Param is NULL, return!\n" );
		return;
	}
	
	json_t_ptr pjRoot = NULL;
	if (!LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE)) return;
		
	//域moid
	s8 achDomainMoid[OPERATION_MAXLEN_MOID + 1] = {0};
	u16 wMoIdx = 0;
	u16 wHduNum = 0;
	
	json_t_ptr pjAllHduInfoArray = json_array();
	if ( NULL == pjAllHduInfoArray )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[SetAllHduInfoToJson] json_array pjAllHduInfoArray is NULL, return!\n" );
	}
	else
	{	
		for ( wMoIdx = 0; wMoIdx < wMoIdNum; wMoIdx++ )
		{
			memcpy( achDomainMoid, pbyMsg, OPERATION_MAXLEN_MOID);
			pbyMsg += OPERATION_MAXLEN_MOID;

			wHduNum = 0;
			wHduNum = *(u16*)pbyMsg;
			pbyMsg += sizeof(u16);
			CHduInfo *pcHduInfo = (CHduInfo*)pbyMsg;
			pbyMsg += ( wHduNum * sizeof(CHduInfo) );

			json_t_ptr pjAllHduInfo = SetHduListByMoidToJsonObj(achDomainMoid, pcHduInfo, wHduNum);
			if (pjAllHduInfo)
			{
				json_array_append_s(pjAllHduInfoArray, pjAllHduInfo);
			}
		}
		
		json_object_set_s( pjRoot, JSON_ALLHDUINFO, pjAllHduInfoArray );
	}
	
	DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen);
}

json_t_ptr CMauData::SetHduListByMoidToJsonObj(const s8 * achDomainMoid, const CHduInfo * pcHduInfo, u16 wHduNum)
{
	json_t_ptr pjAllHduInfo = json_object();

	if (pjAllHduInfo)
	{
		//moid
		SetStringToJson(pjAllHduInfo, JSON_MOID, achDomainMoid);

		json_t_ptr pjHduListArray = json_array();
		if (NULL == pjHduListArray)
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[SetAllHduInfoToJson] json_array pjHduListArray is NULL, return!\n");
		}
		else
		{
			u16 wHduIdx = 0;
			for (wHduIdx = 0; wHduIdx < wHduNum; wHduIdx++)
			{
				json_t_ptr pjHduInfo = HduInfoToJsonObj(pcHduInfo[wHduIdx]);
				if (pjHduInfo)
				{
					json_array_append_s(pjHduListArray, pjHduInfo);
				}
			}

			json_object_set_s(pjAllHduInfo, JSON_HDULIST, pjHduListArray);
		}
	}
	return pjAllHduInfo;
}

json_t_ptr CMauData::HduInfoToJsonObj(const CHduInfo& cHduInfo)
{
	json_t_ptr pjHduInfo = json_object();
	if (pjHduInfo)
	{
		//hduid
		SetUIntAsStringToJson(pjHduInfo, JSON_HDUID, cHduInfo.GetEqpId());
		//isonline
		SetBoolAsStringToJson(pjHduInfo, JSON_HDUISONLINE, cHduInfo.IsOnline());
		//chnnum
		SetUIntAsStringToJson(pjHduInfo, JSON_HDUCHNNUM, cHduInfo.GetChnNum());
		//hdualias
		SetStringToJson(pjHduInfo, JSON_HDUALIAS, cHduInfo.GetAlias());
		//chninfo
		json_t_ptr pjHduChnArray = json_array();
		if (NULL == pjHduChnArray)
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[HduInfoToJsonObj] json_array HduChnArray is NULL, return!\n");
		}
		else
		{
			u8 byHduChnIdx = 0;
			for (byHduChnIdx = 0; byHduChnIdx < MAXNUM_HDUEQP_CHANNEL; byHduChnIdx++)
			{
				json_t_ptr pjHduChnInfo = json_object();
				if (NULL == pjHduChnInfo)
				{
					continue;
				}

				//chnnidx
				SetUIntAsStringToJson(pjHduChnInfo, JSON_HDUCHLIDX, byHduChnIdx);
				//issilence
				SetBoolAsStringToJson(pjHduChnInfo, JSON_HDUCHNISSILENCE, cHduInfo.GetIsMute(byHduChnIdx));
				//volume
				SetUIntAsStringToJson(pjHduChnInfo, JSON_VOLUME, cHduInfo.GetVolume(byHduChnIdx));
				//chnstatus
				SetUIntAsStringToJson(pjHduChnInfo, JSON_HDUCHNSTATUS, cHduInfo.GetChnStatus(byHduChnIdx));

				json_array_append_s(pjHduChnArray, pjHduChnInfo);
			}

			json_object_set_s(pjHduInfo, JSON_HDUCHNINFO, pjHduChnArray);
		}
	}
	return pjHduInfo;
}

void CMauData::SetConfMaxJoinedMtNum(s8 * pchJsonBuf, u32 &dwJsonBufLen, u16 wMaxMtNum)
{
	json_t_ptr pjRoot = NULL;
	if (!LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE)) return;

	// 最大终端数限制
	SetConfMaxJoinedMtNumToJsonObj(pjRoot, wMaxMtNum);

	DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen);
}

BOOL32 CMauData::GetConfMaxJoinedMtNum(json_t_ptr pjRoot, u16& wMaxMtNum)
{
	if (!pjRoot)
	{
		OspPrintf(TRUE, FALSE, "[%s]pjRoot is NULL\n", __FUNCTION__);
		return FALSE;
	}

	json_t_ptr pjTemp = json_object_get_s(pjRoot, JSON_MAXJOINEDMT);
	if (NULL == pjTemp)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[%s] json_object_get %s is NULL, retur FALSE!\n", __FUNCTION__, JSON_MAXJOINEDMT);
		return FALSE;
	}
	wMaxMtNum = atoi(json_string_value(pjTemp));
	return TRUE;
}

/*=============================================================================
函 数 名： GetCmuIdFromJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::GetCmuIdFromJson( s8 * pchJsonBuf, u16 &wCmuId )
{
	if ( NULL == pchJsonBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetCmuIdFromJson] Param is NULL, return FLASE!\n" );
		return;
	}
	
	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL ==  pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetCmuIdFromJson] json_loads err, retur FALSE!\n" );
		return;
	}
	
	json_t_ptr pjCmuId = json_object_get_s( pjRoot, JSON_CMUID );
	if ( NULL == pjCmuId )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetCmuIdFromJson] json_object_get CmuId is NULL, retur FALSE!\n" );
		return;
	}
	
	wCmuId = atoi( json_string_value( pjCmuId ) );
	return;
}

/*=============================================================================
函 数 名： GetCmFlagFromJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::GetCmFlagFromJson( s8 * pchJsonBuf, u16 &wCmFlag )
{
	if ( NULL == pchJsonBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetCmFlagFromJson] Param is NULL, return FLASE!\n" );
		return;
	}
	
	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL ==  pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetCmFlagFromJson] json_loads err, return FALSE!\n" );
		return;
	}
	
	json_t_ptr pjCmFlag = json_object_get_s( pjRoot, JSON_CMFLAG );
	if ( NULL == pjCmFlag )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetCmFlagFromJson] json_object_get CmFlag is NULL, return FALSE!\n" );
		return;
	}
	
	wCmFlag = atoi( json_string_value( pjCmFlag ) );
	
	return;
}

/*=============================================================================
函 数 名： GetHduInfoFromJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/31  5.0         rnn                    创建
=============================================================================*/
void CMauData::GetHduInfoFromJson( s8 * pchJsonBuf, u8 &byHduId, u8 &byHduChnIdx, u8 &byHduSchedIdx, u8 &byHduSilence, u8 &byHduVolume )
{
	if ( NULL == pchJsonBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[GetHduInfoFromJson]Param is NULL, return!\n" );
		return;
	}
	
	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL == pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[GetHduInfoFromJson] json_loads err!\n" );
		return;
	}
	
	// hduid
	json_t_ptr pjHduId = json_object_get_s( pjRoot, JSON_HDUID );
	if ( NULL == pjHduId )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[GetHduInfoFromJson] json_object_get HduId is NULL!\n" );
	}
	else
	{		
		byHduId = (u8)atoi( json_string_value( pjHduId ) );
	}
	
	// chnnlidx
	json_t_ptr pjChnnlidx = json_object_get_s( pjRoot, JSON_HDUCHLIDX );
	if ( NULL == pjChnnlidx )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[GetHduInfoFromJson] json_object_get Chnnlidx is NULL!\n" );
	}
	else
	{
		byHduChnIdx =  (u8)atoi( json_string_value( pjChnnlidx ) );
	}	
	
	// hduschedidx
	json_t_ptr pjHduschedidx = json_object_get_s( pjRoot, JSON_HDUSCHEDIDX );
	if ( NULL == pjHduschedidx )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[GetHduInfoFromJson] json_object_get Hduschedidx is NULL!\n" );
	}
	else
	{
		byHduSchedIdx = (u8)atoi( json_string_value( pjHduschedidx ) );
	}

	// chnnlidx
	json_t_ptr pjIssilence = json_object_get_s( pjRoot, JSON_HDUCHNISSILENCE );
	if ( NULL == pjIssilence )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[GetHduInfoFromJson] json_object_get Issilence is NULL!\n" );
	}
	else
	{
		byHduSilence =  (u8)atoi( json_string_value( pjIssilence ) );
	}	
	
	// volume
	json_t_ptr pjVolume = json_object_get_s( pjRoot, JSON_VOLUME );
	if ( NULL == pjVolume )
	{
		LogPrint( LOG_LVL_ERROR, MID_MCU_MAUSSN, "[GetHduInfoFromJson] json_object_get Volume is NULL!\n" );
	}
	else
	{
		byHduVolume = (u8)atoi( json_string_value( pjVolume ) );
	}
	
	return;
}

void CMauData::GetConfCreatorNameFromJson(s8 * pchJsonBuf, s8 * pchCreatorName, u32 dwLen)
{
	if (NULL == pchJsonBuf || NULL == pchCreatorName)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfPwdFromJson] Param is NULL, return!\n");
		return;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads(pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason);
	if (NULL == pjRoot)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfPwdFromJson] json_loads err, return!\n");
		return;
	}

	json_t_ptr pjCreatorName = json_object_get_s(pjRoot, JSON_CREATORNAME);
	if (NULL == pjCreatorName)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfPwdFromJson] json_object_get ConfPwd is NULL!\n");
	}
	else
	{
		strncpy(pchCreatorName, json_string_value(pjCreatorName), dwLen);
	}
}

void CMauData::GetConfCreatorMoidFromJson(s8 * pchJsonBuf, s8 * pchCreatorMoid, u32 dwLen)
{
	if (NULL == pchJsonBuf || NULL == pchCreatorMoid)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfPwdFromJson] Param is NULL, return!\n");
		return;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads(pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason);
	if (NULL == pjRoot)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfPwdFromJson] json_loads err, return!\n");
		return;
	}

	json_t_ptr pjCreatorMoid = json_object_get_s(pjRoot, JSON_CREATORMOID);
	if (NULL == pjCreatorMoid)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfPwdFromJson] json_object_get ConfPwd is NULL!\n");
	}
	else
	{
		strncpy(pchCreatorMoid, json_string_value(pjCreatorMoid), dwLen);
	}

	return;
}

/*=============================================================================
函 数 名： GetMeetingIdFromJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::GetMeetingIdFromJson( s8 * pchJsonBuf, u32 &nMeetingId )
{
	if ( NULL == pchJsonBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetMeetingIdFromJson] Param is NULL, return FLASE!\n" );
		return;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL ==  pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetMeetingIdFromJson] json_loads err, retur FALSE!\n" );
		return;
	}
	
	json_t_ptr pjMeetingId = json_object_get_s( pjRoot, JSON_MEETINTID );
	if ( NULL == pjMeetingId )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetMeetingIdFromJson] json_object_get meetingid is NULL, retur FALSE!\n" );
		return;
	}
	
	nMeetingId = strtoul(json_string_value(pjMeetingId), NULL, 10);
	
	return;
}


/*=============================================================================
函 数 名： GetConfDataFromJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::GetConfDataFromJson(const s8 * pchJsonBuf, TMpcConfData * ptConfData, u16 &wConfNum )
{
	if ( NULL == pchJsonBuf || NULL == ptConfData )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfDataFromJson]Param is null, return!\n" );
		return;
	}
	
	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL == pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfDataFromJson] pjRoot is NULL, return!\n" );
		return;
	}
	
	json_t_ptr pjConfDataArray = json_object_get_s( pjRoot, JSON_CONFDATAINFO );
	if ( NULL == pjConfDataArray )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfDataFromJson]json_object_get pjConfDataArray is null, return!\n" );
		return;
	}
	
	wConfNum = json_array_size( pjConfDataArray );
	u16 wConfDataIdx = 0;
	for ( wConfDataIdx = 0; wConfDataIdx < wConfNum; wConfDataIdx++ )
	{
		json_t_ptr pjConfDataInfo = json_array_get_s( pjConfDataArray, wConfDataIdx );
		if ( NULL == pjConfDataInfo )
		{
			continue;
		}
		
		TConfIdentityAttr tConfAttr;
		
		// ConfE164
		json_t_ptr pjConfE164 = json_object_get_s( pjConfDataInfo, JSON_CONFE164 );
		if ( NULL == pjConfE164 )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string ConfE164 is NULL!\n" );
		}
		else
		{
			tConfAttr.SetConfE164( json_string_value( pjConfE164 ) );
		}
		
		// ConfName
		json_t_ptr pjConfName = json_object_get_s( pjConfDataInfo, JSON_CONFNAME );
		if ( NULL == pjConfName )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string ConfName is NULL!\n" );
		}
		else
		{
			tConfAttr.SetConfName( json_string_value( pjConfName ) );
		}
		
		// ConfPwd
		json_t_ptr pjConfPwd = json_object_get_s( pjConfDataInfo, JSON_PSW );
		if ( NULL == pjConfPwd )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string ConfPwd is NULL!\n" );
		}
		else
		{
			tConfAttr.SetConfPwd( json_string_value( pjConfPwd ) );
		}

		// MOID
		json_t_ptr pjMOID = json_object_get_s( pjConfDataInfo, JSON_MOID );
		if ( NULL == pjMOID )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string MOID is NULL!\n" );
		}
		else
		{
			tConfAttr.SetDomainMOID( json_string_value( pjMOID ) );
		}

		// DomainName
		json_t_ptr pjDomainName = json_object_get_s( pjConfDataInfo, JSON_DOMAINNAME );
		if ( NULL == pjDomainName )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string DomainName is NULL!\n" );
		}
		else
		{
			tConfAttr.SetDomainName( json_string_value( pjDomainName ) );
		}

		ptConfData[wConfDataIdx].SetConfIdAttr( tConfAttr );

		// StartTime
		json_t_ptr pjStartTime = json_object_get_s( pjConfDataInfo, JSON_CONFSTARTTIME );
		if ( NULL == pjStartTime )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string StartTime is NULL!\n" );
		}
		else
		{
			time_t tTime = (time_t)strtoul(json_string_value(pjStartTime), 0, 10);
			if (tTime >= 0)
			{
				ptConfData[wConfDataIdx].SetStartTime( tTime );
			}
		}

		// ConfId
		json_t_ptr pjConfId = json_object_get_s( pjConfDataInfo, JSON_CONFID );
		if ( NULL == pjConfId )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string ConfId is NULL!\n" );
		}
		else
		{
			CConfId cConfId;
			cConfId.SetConfId( json_string_value( pjConfId ) );
			ptConfData[wConfDataIdx].SetConfId( cConfId );
		}

		// Duration
		json_t_ptr pjDuration = json_object_get_s( pjConfDataInfo, JSON_DURATION );
		if ( NULL == pjDuration )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string Duration is NULL!\n" );
		}
		else
		{
			ptConfData[wConfDataIdx].SetDuration( (u16)atoi( json_string_value( pjDuration ) ) );
		}

		// MtNum
		json_t_ptr pjMtNum = json_object_get_s( pjConfDataInfo, JSON_CONFMTNUM );
		if ( NULL == pjMtNum )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string MtNum is NULL!\n" );
		}
		else
		{
			ptConfData[wConfDataIdx].SetMtNum( (u8)atoi( json_string_value( pjMtNum ) ) );
		}

		// OpenMode
		json_t_ptr pjOpenMode = json_object_get_s( pjConfDataInfo, JSON_OPENMODE );
		if ( NULL == pjOpenMode )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string OpenMode is NULL!\n" );
		}
		else
		{
			ptConfData[wConfDataIdx].SetOpenMode( (u8)atoi( json_string_value( pjOpenMode ) ) );
		}

		// DualMode
		json_t_ptr pjDualMode = json_object_get_s(pjConfDataInfo, JSON_DUALMODE);
		if (NULL == pjDualMode)
		{
			LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string OpenMode is NULL!\n");
		}
		else
		{
			ptConfData[wConfDataIdx].SetDualMode((u8)atoi(json_string_value(pjDualMode)));
		}

		// PublicConf
		json_t_ptr pjPublicConf = json_object_get_s( pjConfDataInfo, JSON_PUBLISHCONF );
		if ( NULL == pjPublicConf )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string PublicConf is NULL!\n" );
		}
		else
		{
			ptConfData[wConfDataIdx].SetPublicConf( (u8)atoi( json_string_value( pjPublicConf ) ) );
		}

		// ConfIdx
		json_t_ptr pjConfIdx = json_object_get_s( pjConfDataInfo, JSON_CONFIDX );
		if ( NULL == pjConfIdx )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string ConfIdx is NULL!\n" );
		}
		else
		{
			ptConfData[wConfDataIdx].SetConfIdx( (u32)strtoul( json_string_value( pjConfIdx ), 0, 10 ) );
		}

		// Bitrate
		json_t_ptr pjBitRate = json_object_get_s( pjConfDataInfo, JSON_MEDIABITRATE );
		if ( NULL == pjBitRate )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string BitRate is NULL!\n" );
		}
		else
		{
			ptConfData[wConfDataIdx].SetBitRate( (u16)atoi( json_string_value( pjBitRate ) ) );
		}
		
		// Res
		json_t_ptr pjRes = json_object_get_s( pjConfDataInfo, JSON_MEDIARES );
		if ( NULL == pjRes )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string Res is NULL!\n" );
		}
		else
		{
			ptConfData[wConfDataIdx].SetResolution( (u8)atoi( json_string_value( pjRes ) ) );
		}

		// URL
		json_t_ptr pjConfUrl = json_object_get_s( pjConfDataInfo, JSON_URL );
		if ( NULL == pjConfUrl )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfDataFromJson] json_string ConfUrl is NULL!\n" );
		}
		else
		{
			ptConfData[wConfDataIdx].SetMcuUrl( json_string_value( pjConfUrl ) );
		}

	}
	
	return;
}

/*=============================================================================
函 数 名： GetBookConfListNumFromJson
功    能： 获取预约会议数目
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::GetBookConfListNumFromJson( s8 * pchJsonBuf, u16 &wBookNum )
{
	if ( NULL == pchJsonBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetBookConfListNumFromJson]Param is null, return!\n" );
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL == pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetBookConfListNumFromJson] pjRoot is NULL, return!\n" );
		return FALSE;
	}

	json_t_ptr pjBookInfoArray = json_object_get_s( pjRoot, JSON_CONFLIST );
	if ( NULL == pjBookInfoArray )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetBookConfListNumFromJson]json_object_get pjBookInfoArray is null, return!\n" );
		return FALSE;
	}
	
	wBookNum = json_array_size( pjBookInfoArray );

	return TRUE;
}

/*=============================================================================
函 数 名： GetBookConfListFromJson
功    能： 获取预约会议列表
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::GetBookConfListFromJson(s8 * pchJsonBuf, vector<CRedisBookConf>& bookConfVec)
{
	if (NULL == pchJsonBuf)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetBookConfListFromJson]Param is null, return!\n");
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads(pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason);
	if (NULL == pjRoot)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetBookConfListFromJson] pjRoot is NULL, return!\n");
		return FALSE;
	}

	TConfInfo tConfInfo;
	if (!GetCreateConfParamFromJsonObj(pjRoot, tConfInfo))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[%s]getconfinfo fail\n", __FUNCTION__);
		return FALSE;
	}
	CRedisBookConf cBookConf;
	cBookConf.SetConfName(tConfInfo.GetConfName());
	cBookConf.SetDomainMOID(tConfInfo.GetDomainMoid());
	cBookConf.SetPassword(tConfInfo.GetConfPwd());
	cBookConf.SetIsPublicConf(tConfInfo.GetConfAttrb().GetPublicConf() == 0 ? FALSE : TRUE);
	cBookConf.SetOpenMode(tConfInfo.GetConfAttrb().GetOpenMode());
	cBookConf.SetDualMode(tConfInfo.GetConfAttrb().GetDualMode());
	cBookConf.SetIsConfNodisturb(tConfInfo.GetConfAttrb().IsConfNoDisturb());
	cBookConf.SetMaxJoinedMt(tConfInfo.GetMaxJoinedMtNum());
	cBookConf.SetMixMode(tConfInfo.GetStatus().GetMixerMode());
	const u32 dwMtNum = json_array_size(json_object_get_s(pjRoot, JSON_MT));
	cBookConf.SetMtNum(dwMtNum);
	GetCreatorInfoFromJson(pjRoot, cBookConf);

	TConfInfoEx tConfInfoEx;
	if (GetConfInfoExFromJson(pchJsonBuf, tConfInfoEx))
	{
		TVideoStreamCap atVideoStreamCap[MAX_CONF_CAP_EX_NUM];
		u8 byCapNum = MAX_CONF_CAP_EX_NUM;
		tConfInfoEx.GetMainStreamCapEx(atVideoStreamCap, byCapNum);
		const u16 wBitRate = atVideoStreamCap[0].GetMaxBitRate();
		cBookConf.SetBitRate(wBitRate);
		const u8 byResolution = atVideoStreamCap[0].GetResolution();
		cBookConf.SetResolution(byResolution);
		const u8 byMediaType = atVideoStreamCap[0].GetMediaType();
		cBookConf.SetMediaType(byMediaType);
	}
	cBookConf.SetConfDetailJson(pchJsonBuf);

	json_t_ptr pjBookInfoArray = json_object_get_s( pjRoot, JSON_CONFLIST );
	if ( NULL == pjBookInfoArray )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetMtListFromJson]json_object_get pjBookInfoArray is null, return!\n" );
		return FALSE;
	}
	
	const u16 wBookNum = json_array_size( pjBookInfoArray );
	u16 wBookIdx = 0;
	for ( wBookIdx = 0; wBookIdx < wBookNum; wBookIdx++ )
	{
		json_t_ptr pjBookInfo = json_array_get_s( pjBookInfoArray, wBookIdx );
		if ( NULL == pjBookInfo )
		{
			continue;
		}
		
		json_t_ptr pjMeetingId = json_object_get_s( pjBookInfo, JSON_MEETINTID );
		json_t_ptr pjConfE164  = json_object_get_s( pjBookInfo, JSON_CONFE164 );
		if ( NULL == pjMeetingId || NULL ==  pjConfE164)
		{
			continue;
		}
		CRedisBookConf bookConf(cBookConf);
		bookConf.SetConfE164( json_string_value( pjConfE164 ) );
		bookConf.SetMeetingID( atoi( json_string_value( pjMeetingId ) ) );

		json_t_ptr pjStartTime = json_object_get_s( pjBookInfo, JSON_CONFSTARTTIME );
		json_t_ptr pjEndTime   = json_object_get_s( pjBookInfo, JSON_CONFENDTIME );
		if (NULL != pjStartTime && NULL != pjEndTime)
		{
			bookConf.SetBookStartTime( json_string_value( pjStartTime ) );
			bookConf.SetBookEndTime( json_string_value( pjEndTime ) );
		}
		bookConfVec.push_back(bookConf);
	}
	return TRUE;
}

/*=============================================================================
函 数 名： GetDomainFromJson
功    能： 获取域信息
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::GetDomainFromJson( s8 * pchJsonBuf, CKdvDomainInfo & tOutInfo )
{
	if ( NULL == pchJsonBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetDomainFromJson]Param is null, return FALSE!\n" );
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );;
	if ( NULL ==pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetDomainFromJson] pjRoot is NULL, return!\n" );
		return FALSE;
	}

	json_t_ptr pjMoId = json_object_get_s( pjRoot, JSON_MOID );
	if ( NULL == pjMoId )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetDomainFromJson] json_object_get pjMoId is NULL, retur FALSE!\n" );
		return FALSE;
	}

	CKdvDomainInfo cKdyDomain;
	cKdyDomain.SetDomainMOID( json_string_value( pjMoId ) );

	if ( !GetDomainInfo( cKdyDomain.GetDomainMOID(), tOutInfo ) )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetDomainGUIDFromJson]GetDomainInfoByMoid err, return FLASE!\n" );
		return FALSE;
	}

	return TRUE;
}

/*=============================================================================
函 数 名： GetTemplateListFromJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::GetTemplateListFromJson( s8 * pchJsonBuf, CMauMsg &cServMsg )
{
	if ( NULL == pchJsonBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetTemplateListFromJson]Param is null, return!\n" );
		return FALSE;
	}
	
	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL == pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetTemplateListFromJson] pjRoot is NULL, return!\n" );
		return FALSE;
	}
	
	json_t_ptr pjTemplateArray = json_object_get_s( pjRoot, JSON_TEMPLATELIST );
	if ( NULL == pjTemplateArray )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetMtListFromJson]json_object_get TemplateArray is null, return!\n" );
		return FALSE;
	}
	
	u16 wNum = json_array_size( pjTemplateArray );
	cServMsg.SetMsgBody( (u8*)&wNum, sizeof( u16 ) );

	u16 wIdx = 0;
	for ( wIdx = 0; wIdx < wNum; wIdx++ )
	{
		json_t_ptr pjTemplateInfo = json_array_get_s( pjTemplateArray, wIdx );
		if ( NULL == pjTemplateInfo )
		{
			continue;
		}
		
		json_t_ptr pjE164 = json_object_get_s( pjTemplateInfo, JSON_CONFE164 );
		if ( NULL == pjE164 )
		{
			continue;
		}

		cServMsg.CatMsgBody( (u8*)json_string_value( pjE164 ), MAXLEN_CONFE164 );
	}
	
	return TRUE;
}

json_t_ptr CMauData::CreatPollModuleSetJson(const TMiniPollInfo &tPollModule)
{
	json_t_ptr pjPollSet = json_object();
	if (NULL == pjPollSet)
	{
		OspPrintf(TRUE, FALSE, "[CreatPollModuleSetJson] pjPollSet is NULL, return!\n");
		return NULL;
	}

	const u8 byPollMode = TranslatePollMode(tPollModule.GetMediaMode());
	SetUIntAsStringToJson(pjPollSet, JSON_POLLMODE, byPollMode);
	SetUIntAsStringToJson(pjPollSet, JSON_POLLKEEPTIME, tPollModule.GetKeepTime());
	SetUIntAsStringToJson(pjPollSet, JSON_POLLNUM, tPollModule.GetPollNum());

	json_t_ptr pjPollMtArray = json_array();
	if (NULL == pjPollMtArray)
	{
		OspPrintf(TRUE, FALSE, "[CreatPollModuleSetJson] json_object VmpMtArray is NULL!\n");
	}
	else
	{
		u8 byMtIdx = 0;
		for (byMtIdx = 0; byMtIdx < MAXNUM_POLL_MEMBER; byMtIdx++)
		{
			json_t_ptr pjPollMtObj = json_object();
			if (NULL == pjPollMtObj)
			{
				OspPrintf(TRUE, FALSE, "[CreatPollModuleSetJson] json_object PollMtObj is NULL!\n");
				continue;
			}

			if (tPollModule.GetPollMemIdx(byMtIdx) != 0)
			{
				SetUIntAsStringToJson(pjPollMtObj, JSON_POLLIDX, tPollModule.GetPollMemIdx(byMtIdx));
			}
			else
			{
				break;
			}
			json_array_append_s(pjPollMtArray, pjPollMtObj);
		}
	}

	json_object_set_s(pjPollSet, JSON_POLLMT, pjPollMtArray);
	return pjPollSet;
}

BOOL32 CMauData::SetPollModuleToJson(const json_t_ptr& pjRoot, const TMiniPollInfo & tPollModule)
{
	if (!pjRoot)
	{
		return FALSE;
	}
	json_t_ptr pjPollParam = CreatPollModuleSetJson(tPollModule);
	if (pjPollParam != NULL)
	{
		json_object_set_s(pjRoot, JSON_POLLPARAM, pjPollParam);
		return TRUE;
	}
	return FALSE;
}

BOOL32 CMauData::SetVipModuleToJson(const json_t_ptr& pjRoot, const TMiniVipInfo& tVipModule)
{
	if (!pjRoot)
	{
		return FALSE;
	}

	json_t_ptr pjVipMtArray = json_array();
	if (NULL == pjVipMtArray)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object VipMtArray is NULL!\n", __FUNCTION__);
	}
	else
	{
		u8 byMtIdx = 0;
		for (byMtIdx = 0; byMtIdx < MAXNUM_VIP_LIST; byMtIdx++)
		{
			json_t_ptr pjVipMtObj = json_object();
			if (NULL == pjVipMtObj)
			{
				OspPrintf(TRUE, FALSE, "[%s] json_object VipMtObj is NULL!\n", __FUNCTION__);
				continue;
			}

			if (tVipModule.GetVipMemIdx(byMtIdx) != 0)
			{
				SetUIntAsStringToJson(pjVipMtObj, JSON_MTINDEX, tVipModule.GetVipMemIdx(byMtIdx));
			}
			else
			{
				break;
			}
			json_array_append_s(pjVipMtArray, pjVipMtObj);
		}
	}

	if (pjVipMtArray != NULL)
	{
		json_object_set_s(pjRoot, JSON_VIPMT, pjVipMtArray);
		return TRUE;
	}
	return FALSE;
}

/*=============================================================================
函 数 名： GetConfSynDataFromJsom
功    能： Json转换为需同步的会议信息
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::GetConfSynDataFromJsom( s8 * pchJsonBuf, TConfSynData & tConfSynData )
{
	if ( NULL == pchJsonBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] pchJsonBuf is NULL, retur FALSE!\n" );
		return;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL == pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] pjRoot is NULL, return!\n" );
		return;
	}

	// ConfE164
	json_t_ptr pjConfE164 = json_object_get_s( pjRoot, JSON_CONFE164 );
	if ( NULL == pjConfE164 )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] json_string ConfE164 is NULL!\n" );
	}
	else
	{
		tConfSynData.SetConfE164( json_string_value( pjConfE164 ) );
	}
	
	// ConfPwd
	json_t_ptr pjConfPwd = json_object_get_s( pjRoot, JSON_PSW );
	if ( NULL == pjConfPwd )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] json_string ConfPwd is NULL!\n" );
	}
	else
	{
		tConfSynData.SetConfPwd( json_string_value( pjConfPwd ) );
	}

	// Duration
	json_t_ptr pjDuration = json_object_get_s( pjRoot, JSON_DURATION );
	if ( NULL == pjDuration )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] json_string Duration is NULL!\n" );
	}
	else
	{
		tConfSynData.SetDuration( (u16)atoi( json_string_value( pjDuration ) ) );
	}

	// MtNum
	json_t_ptr pjMtNum = json_object_get_s( pjRoot, JSON_CONFMTNUM );
	if ( NULL == pjMtNum )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] json_string MtNum is NULL!\n" );
	}
	else
	{
		tConfSynData.SetMtNum( (u8)atoi( json_string_value( pjMtNum ) ) );
	}

	// OpenMode
	json_t_ptr pjOpenMode = json_object_get_s( pjRoot, JSON_OPENMODE );
	if ( NULL == pjOpenMode )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] json_string OpenMode is NULL!\n" );
	}
	else
	{
		tConfSynData.SetOpenMode( (u8)atoi( json_string_value( pjOpenMode ) ) );
	}

	// DualMode
	json_t_ptr pjDualMode = json_object_get_s(pjRoot, JSON_DUALMODE);
	if (NULL == pjDualMode)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] json_string DualMode is NULL!\n");
	}
	else
	{
		tConfSynData.SetDualMode((u8)atoi(json_string_value(pjDualMode)));
	}

	// PublicConf
	json_t_ptr pjPublicConf = json_object_get_s( pjRoot, JSON_PUBLISHCONF );
	if ( NULL == pjPublicConf )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] json_string PublicConf is NULL!\n" );
	}
	else
	{
		tConfSynData.SetPublicConf( (u8)atoi( json_string_value( pjPublicConf ) ) );
	}

	// ConfIdx
	json_t_ptr pjConfIdx = json_object_get_s( pjRoot, JSON_CONFIDX );
	if ( NULL == pjConfIdx )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] json_string ConfIdx is NULL!\n" );
	}
	else
	{
		tConfSynData.SetConfIdx( (u32)strtoul( json_string_value( pjConfIdx ), 0, 10 ) );
	}

	// Bitrate
	json_t_ptr pjBitRate = json_object_get_s( pjRoot, JSON_MEDIABITRATE );
	if ( NULL == pjBitRate )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] json_string BitRate is NULL!\n" );
	}
	else
	{
		tConfSynData.SetBitRate( (u16)atoi( json_string_value( pjBitRate ) ) );
	}
	
	// Res
	json_t_ptr pjRes = json_object_get_s( pjRoot, JSON_MEDIARES );
	if ( NULL == pjRes )
	{
		LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[GetConfSynDataFromJsom] json_string Res is NULL!\n" );
	}
	else
	{
		tConfSynData.SetResolution( (u8)atoi( json_string_value( pjRes ) ) );
	}
	
}

void CMauData::SetMcuListToJson(s8 * pchJsonBuf, u32 &dwJsonBufLen, vector<string>& vctMcuName, 
	vector<string>& vctMcuType, vector<string>& vctMcuIp, vector<string>& vctGkPrefix, u8 byMcuNum)
{
	json_t_ptr pjRoot = NULL;
	if (!LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE)) return;

	// MCULIST
	json_t_ptr pjMcuListArray = json_array();
	if ( NULL == pjMcuListArray )
	{
		OspPrintf( TRUE, FALSE, "[SetMcuListToJson]json_array pjMcuListArray is NULL return!\n" );
		return;
	}

	u8 byIdx = 0;
	for ( byIdx = 0; byIdx < byMcuNum; byIdx++ )
	{
		json_t_ptr pjMcuInfo = json_object();
		if ( NULL == pjMcuInfo )
		{
			OspPrintf( TRUE, FALSE, "[SetMcuListToJson]json_object pjMcuInfo is NULL, continue!\n" );
			return;
		}

		SetStringToJson(pjMcuInfo, JSON_MCUNAME, vctMcuName[byIdx].c_str());
		SetStringToJson(pjMcuInfo, JSON_MCUTYPE, vctMcuType[byIdx].c_str());
		SetStringToJson(pjMcuInfo, JSON_MCUIP, vctMcuIp[byIdx].c_str());
		SetStringToJson(pjMcuInfo, JSON_GKPREFIX, vctGkPrefix[byIdx].c_str());

		json_array_append_s( pjMcuListArray, pjMcuInfo );

	}

	json_object_set_s( pjRoot, JSON_MCULIST, pjMcuListArray );

	DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen);
}


/*=============================================================================
函 数 名： SetTemplateListToJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::SetTemplateListToJson( s8 * pchJsonBuf, u32 &dwJsonBufLen, CMauMsg cServmsg )
{
	json_t_ptr pjRoot = NULL;
	if (!LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE)) return;

	json_t_ptr pjTemplateArray = json_array();
	if ( NULL == pjTemplateArray )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[SetTemplateListToJson]json_array pjTemplateArray is NULL, return!\n" );
		return;
	}
	
	u16 wDataNum = *(u16*)cServmsg.GetMsgBody();
	u16 wDataIdx = 0;
	for ( wDataIdx = 0; wDataIdx < wDataNum; wDataIdx++ )
	{
		json_t_ptr pjTemplateInfo = json_object();
		if ( NULL == pjTemplateInfo )
		{
			LogPrint( LOG_LVL_WARNING, MID_MAU_PUBLIC, "[SetTemplateListToJson]json_object pjTemplateInfo is NULL, continue!\n" );
			continue;
		}

		SetStringToJson(pjTemplateInfo, JSON_CONFE164, (s8*)(cServmsg.GetMsgBody() + sizeof(u16) + wDataIdx*MAXLEN_CONFE164));

		json_array_append_s( pjTemplateArray, pjTemplateInfo );
	}

	json_object_set_s( pjRoot, JSON_TEMPLATELIST, pjTemplateArray );
	
	DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen);
}

/*=============================================================================
函 数 名： SetDomainMoIdToJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CMauData::SetDomainMoIdToJson( s8 * pchJsonBuf, const s8 * pchMoId, u32 &dwJsonBufLen )
{
	if ( NULL == pchJsonBuf || NULL == pchMoId )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[SetDomainMoIdToJson] Param is NULL, retur!\n" );
		return;
	}
	json_t_ptr pjRoot = NULL;
	if (!LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE)) return;
	if (!SetDomainMoidToJsonObj(pjRoot, pchMoId)) return;
	DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen);
}

/*=============================================================================
函 数 名： PackTMtAliasArray
功    能： 终端别名信息打包
算法实现： 
全局变量： 
参    数： TMtAlias *ptMtAlias
u16 *pawMtDialRate
u16 wMtNum
char *pszBuf
u16 &wBufLen
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::PackTMtAliasArray( TMtAlias *ptMtAlias, u16 *pawMtDialRate, u16 wMtNum, char *pszBuf, u16 &wBufLen )
{
	TMtAlias *ptMtTempAlias = ptMtAlias;
	char *pszTemp = pszBuf;
	u16 wMtCount = 0;

	if( NULL == pszBuf || NULL == ptMtAlias || 0 == wMtNum || wMtNum > MAXNUM_CONF_MT )
	{
        wBufLen = 0;
		return FALSE;
	}

    while( wMtCount < wMtNum )
	{
		if( !ptMtTempAlias->IsNull() )
		{
			*pszTemp++ = ptMtTempAlias->m_AliasType;
			if( ptMtTempAlias->m_AliasType == mtAliasTypeTransportAddress )
			{
				*pszTemp++ = sizeof(TTransportAddr);
				memcpy( pszTemp, &ptMtTempAlias->m_tTransportAddr, sizeof(TTransportAddr) );
				pszTemp = pszTemp + sizeof(TTransportAddr);
			}
			else
			{
				*pszTemp++ = strlen( ptMtTempAlias->m_achAlias );
				memcpy( pszTemp, ptMtTempAlias->m_achAlias, strlen( ptMtTempAlias->m_achAlias ) );
				pszTemp = pszTemp + strlen( ptMtTempAlias->m_achAlias );
			}

			//设置呼叫速率
			*((u16*)pszTemp) = htons(*pawMtDialRate++);
			pszTemp += sizeof(u16);
		}
		
		ptMtTempAlias++;
		wMtCount++;
	}
	
	wBufLen = pszTemp - pszBuf;
	
	return TRUE;
}

/*=============================================================================
函 数 名： UnPackTMtAliasArray
功    能： 终端别名信息解包
算法实现： 
全局变量： 
参    数： TMtAlias *ptMtAlias
u16 *pawMtDialRate
u16 wMtNum
char *pszBuf
u16 &wBufLen
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::UnPackTMtAliasArray( char *pszBuf, u16 wBufLen, const TConfInfo *ptConfInfo, 
									  TMtAlias *ptMtAlias, u16* pawMtDialRate, u8 &byMtNum )
{
	u8 byMtCount = 0;
    char *pszTemp = pszBuf;
	u8 byAliasLen;
	
	if( NULL == pszBuf || NULL == ptConfInfo || 
		NULL == ptMtAlias || NULL == pawMtDialRate )
	{
		return FALSE;
	}
	
	while( pszTemp - pszBuf < wBufLen )
	{
		ptMtAlias->m_AliasType = *pszTemp++;//得到别名类型
        if( ptMtAlias->m_AliasType != mtAliasTypeTransportAddress && 
            ptMtAlias->m_AliasType != mtAliasTypeE164 && 
            ptMtAlias->m_AliasType != mtAliasTypeH323ID && 
            ptMtAlias->m_AliasType != mtAliasTypeH320ID &&
            ptMtAlias->m_AliasType != mtAliasTypePartNumber ) //mtAliasTypeH320Alias类型不用作呼叫信息，所以不判断
		{
            byMtNum = byMtCount;
			return FALSE;
		}
		
		byAliasLen = *pszTemp++;//得到别名长度
		if( byAliasLen > MAXLEN_ALIAS )
		{
            byMtNum = byMtCount;
			return FALSE;
		}
		
		if( ptMtAlias->m_AliasType == mtAliasTypeTransportAddress )
		{
			if( byAliasLen != sizeof(TTransportAddr) )
			{
                byMtNum = byMtCount;
				return FALSE;
			}
			memcpy( &ptMtAlias->m_tTransportAddr, pszTemp, byAliasLen );
		}
		else
		{
            memset( ptMtAlias->m_achAlias, 0, sizeof( ptMtAlias->m_achAlias) );
			memcpy( ptMtAlias->m_achAlias, pszTemp, byAliasLen );
		}
		pszTemp = pszTemp + byAliasLen;
		
		*pawMtDialRate = *((u16*)pszTemp);//得到呼叫速率信息;
		*pawMtDialRate = ntohs(*pawMtDialRate);
		
		if (*pawMtDialRate == 0 || *pawMtDialRate > ptConfInfo->GetBitRate()+AUDIO_BITRATE_DEFAULT)
		{
			*pawMtDialRate = ptConfInfo->GetBitRate()+AUDIO_BITRATE_DEFAULT;			
		}
		
		LogPrint( LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[UnPackTMtAliasArray] Mt.%s dialrate is %u\n", ptMtAlias->m_achAlias, *pawMtDialRate );
		
		pawMtDialRate++;
		pszTemp += sizeof(u16);
		
		ptMtAlias++;
		byMtCount++;
	}
	
	byMtNum = byMtCount;
	return TRUE;
}


void CMauData::InsertReqMt(const TReqHeadInfo& tReqInfo, u16 dwConfBitRate, TMtAlias* patMtAlias, u16* awMtCallBitrate, u16& wMtNum, u16 wMaxMtNum,
	TVmpModule& tVmpModule, TMiniPollInfo& tPollModule, TMixModule& tMixModule, u8* abyVipMt)
{
	if (strlen(tReqInfo.GetSrcMtE164()) != 0)
	{
		u32 dwMtCallBitrate = tReqInfo.GetMtCallBitrate();
		u8 byMtNumInUse = 0;

		TMtAlias tMtE164Alias;
		if (mtAliasTypePartNumber == tReqInfo.GetReqMtAliasType())
		{
			tMtE164Alias.SetPartNumber(tReqInfo.GetSrcMtE164());
		}
		else
		{
			tMtE164Alias.SetE164Alias(tReqInfo.GetSrcMtE164());
		}

		u16 dwCallBitRate = dwConfBitRate;
		//由于模板中只记录了视频码率，所以要+音频码率
		if (dwMtCallBitrate < dwCallBitRate && dwMtCallBitrate != 0)
		{
			dwCallBitRate = dwMtCallBitrate;
		}

		byMtNumInUse = 1;

		// 		BOOL32 bInMtTable = FALSE;
		BOOL32 bRepeat = FALSE;
		for (u8 wLoop = 0; wLoop < wMtNum && byMtNumInUse < wMaxMtNum; wLoop++)
		{
			if (patMtAlias[wLoop] == tMtE164Alias)
			{
				bRepeat = TRUE;
				break;
			}
		}
		// 如果不存在，则加进去
		if (!bRepeat)
		{
			// 如果终端数已满
			if (wMtNum == wMaxMtNum)
			{
				// 将最后一个设置为呼叫终端
				patMtAlias[wMaxMtNum - 1] = tMtE164Alias;
				awMtCallBitrate[wMaxMtNum - 1] = dwCallBitRate;

				// 清除原终端在会议里的残留
				for (u8 byVmpIdx = 0; byVmpIdx < MAXNUM_VMP_MEMBER; byVmpIdx++)
				{
					if (wMtNum == tVmpModule.m_abyVmpMember[byVmpIdx])
					{
						tVmpModule.m_abyVmpMember[byVmpIdx] = 0;
					}
				}
				for (u8 byPollIdx = 0; byPollIdx < MAXNUM_CONF_MT; byPollIdx++)
				{
					if (wMtNum == tPollModule.GetPollMemIdx(byPollIdx))
					{
						tPollModule.SetPollMemIdx(byPollIdx, 0);
					}
				}
				for (u8 bMixIdx = 0; bMixIdx < MAXNUM_CONF_MT; bMixIdx++)
				{
					if (wMtNum == tMixModule.m_bySpecMixMember[bMixIdx])
					{
						tMixModule.m_bySpecMixMember[bMixIdx] = 0;
					}
				}
				for (u8 bVipIdx = 0; bVipIdx < MAXNUM_VIP; bVipIdx++)
				{
					if (wMtNum == abyVipMt[bVipIdx])
					{
						abyVipMt[bVipIdx] = 0;
					}
				}
			}
			else
			{
				patMtAlias[wMtNum] = tMtE164Alias;
				++wMtNum;
			}
		}
	}
}

/*=============================================================================
函 数 名： PrintDBStatus
功    能： 打印数据库连接状态
算法实现： 
全局变量： 
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/09/28  5.0         rnn                    创建
=============================================================================*/
void CMauData::PrintDBStatus( void )
{
	m_cVMeetingDBOpr.PrintConnectStatus();
	m_cConfInfoDBOpr.PrintConnectStatus();
	m_cTemplateDBOpr.PrintConnectStatus();
	m_cApInfoDBOpr.PrintConnectStatus();
}

void CMauData::CheckEnv()
{
	CPrintFunInOut cPrintFun(__FUNCTION__, MID_MAU_PUBLIC);

//. 启动加载情况
	const u8 byFinishedStep = m_byInitCount;
	OspPrintf(TRUE, FALSE, "***init redis conf\n");
	OspPrintf(TRUE, FALSE, "***move scheduleDB data to redis\n");
	OspPrintf(TRUE, FALSE, "***load publish template\n");
	OspPrintf(TRUE, FALSE, "***load platform domain\n");
	OspPrintf(TRUE, FALSE, "***load service domain\n");
	OspPrintf(TRUE, FALSE, "***load user domain\n");
	OspPrintf(TRUE, FALSE, "***load domain has xmpu\n");
	OspPrintf(TRUE, FALSE, "***load mt info\n");
	OspPrintf(TRUE, FALSE, "***load hdumoid and userdomain moid\n");
	OspPrintf(TRUE, FALSE, "***load platform is rent or self build\n");
	if (byFinishedStep >= CMauVCInstance::s_InitDealfun_vec.size())
	{
		OspPrintf(TRUE, FALSE, "---poweron starting<%u>items is ok\n", CMauVCInstance::s_InitDealfun_vec.size());
	}
	else
	{
		OspPrintf(TRUE, FALSE, ">>>poweron starting items is not finished, in %u step\n", byFinishedStep);
	}
		

//. 检查某个q是否有两个消费者, 后续可检查全部q
	OspSemTake(g_hTestQConsumerSem);
	time_t curTime = time(NULL);
	time_t curTime2 = curTime + 1;
	s8 achCurTime[100 + 1] = { 0 };
	s8 achCurTime2[100 + 1] = { 0 };
	snprintf(achCurTime, 100, "%u", curTime);
	snprintf(achCurTime2, 100, "%u", curTime2);
	string strTest("testQ");
	string strTest2("testQ");
	strTest += achCurTime;
	strTest2 += achCurTime2;
	PostMsg(g_cCMMauSsnApp.s_nCMMauRabbitMq, strTest.c_str(), strTest.length(), MQ_MAU_CMMAU_K, MQ_MAU_CMMAU_EX);
	PostMsg(g_cCMMauSsnApp.s_nCMMauRabbitMq, strTest2.c_str(), strTest2.length(), MQ_MAU_CMMAU_K, MQ_MAU_CMMAU_EX);
	if (!OspSemTakeByTime(g_hTestQConsumerSem, 500))
	{
		OspPrintf(TRUE, FALSE, ">>>mau.cmmau.q has at least 2 consumer, may has 2 css or 0 consumer\n");
	}
	else
	{
		OspPrintf(TRUE, FALSE, "---mau.cmmau.q consumer num ok\n");
	}
	OspSemGive(g_hTestQConsumerSem);

//. mq消费者状态
	m_cMqStatus.GetMau_CmMauIsCreated() ? OspPrintf(TRUE, FALSE, "---consumer mau.cmmau.q status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>consumer mau.cmmau.q create fail\n");
	m_cMqStatus.GetMau_CmMcuIsCreated() ? OspPrintf(TRUE, FALSE, "---consumer mau.cmmcu.q status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>consumer mau.cmmcu.q create fail\n");
	m_cMqStatus.GetMau_ConfIsCreated() ? OspPrintf(TRUE, FALSE, "---consumer mau.conf.q status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>consumer mau.conf.q create fail\n");
	m_cMqStatus.GetMau_NotifyIsCreated() ? OspPrintf(TRUE, FALSE, "---consumer mau.notify.q status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>consumer mau.notify.q create fail\n");
	m_cMqStatus.GetMovisionLicenseQIsCreated() ? OspPrintf(TRUE, FALSE, "---consumer css.license.q:guid status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>consumer css.license.q:guid create fail\n");
	m_cMqStatus.GetMovisionLicenseUpdateQIsCreated() ? OspPrintf(TRUE, FALSE, "---consumer css.license.ntf.q:moid status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>consumer css.license.notify.q:moid create fail\n");
	m_cMqStatus.GetPlatformLicenseQIsCreated() ? OspPrintf(TRUE, FALSE, "---consumer platform.license.q status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>consumer platform.license.q create fail\n");
	//m_cMqStatus.GetNpp_NgiNppQIsCreated() ? OspPrintf(TRUE, FALSE, "---consumer npp.nginpp.q ok\n")
	//: OspPrintf(TRUE, FALSE, ">>>consumer npp.nginpp.q create fail\n");

//. mq生产者状态
	m_cMqStatus.GetCmMauPIsCreated() ? OspPrintf(TRUE, FALSE, "---Producer CmMau status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>Producer cmMau producer create fail\n");
	m_cMqStatus.GetCmCmuPIsCreated() ? OspPrintf(TRUE, FALSE, "---Producer CmCmu status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>Producer cmCmu producer create fail\n");
	m_cMqStatus.GetCmuPIsCreated() ? OspPrintf(TRUE, FALSE, "---Producer Cmu status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>Producer cmu producer create fail\n");
	m_cMqStatus.GetMovisionLicensePIsCreated() ? OspPrintf(TRUE, FALSE, "---Producer movision license status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>Producer movision license producer create fail\n");
	m_cMqStatus.GetPlatformLicensePIsCreated() ? OspPrintf(TRUE, FALSE, "---Producer platform license status ok\n")
	: OspPrintf(TRUE, FALSE, ">>>Producer platform license producer create fail\n");
	//m_cMqStatus.GetNppNgiPIsCreated() ? OspPrintf(TRUE, FALSE, "---Producer NppNgi status ok\n")
	//: OspPrintf(TRUE, FALSE, ">>>Producer NppNgi producer create fail\n");

//. 是否连上redis
	if (!m_cRedisData.IsConnected())
	{
		OspPrintf(TRUE, FALSE, ">>>redis unconnect\n");
	}
	else
	{
		OspPrintf(TRUE, FALSE, "---redis connect ok\n");
	}

//. 是否连上zk
	if (strncmp(g_cResMgrSsnApp.GetZkState(), "Connected", 9) == 0)
	{
		OspPrintf(TRUE, FALSE, "---zk connect ok\n");
	}
	else
	{
		OspPrintf(TRUE, FALSE, ">>>zk unconnect\n");
	}

//. 是否连上upu
	if (g_cUpuSsnApp.m_cUpuClient.IsConnected())
	{
		OspPrintf(TRUE, FALSE, "---upu connect ok\n");
	}
	else
	{
		OspPrintf(TRUE, FALSE, ">>>upu unconnect\n");
	}

//. 是否连有cmu
	if (m_mapCmuData.size() == 0)
	{
		OspPrintf(TRUE, FALSE, ">>>connected cmu num is 0\n");
	}
	else
	{
		OspPrintf(TRUE, FALSE, "---cmu num ok\n");
	}

//. 是否根据sa接口获取到平台域的guid和moid
	const u32 dwPlatformMoidLen = strlen(m_achPlatformMoid);
	const u32 dwPlatformGuidLen = strlen(m_achPlatformGuid);
	if (dwPlatformGuidLen == 0 || dwPlatformMoidLen == 0)
	{
		if (dwPlatformMoidLen == 0)
		{
			OspPrintf(TRUE, FALSE, ">>>platform moid is empty\n");
		}
		if (dwPlatformGuidLen == 0)
		{
			OspPrintf(TRUE, FALSE, ">>>platform guid is empty\n");
		}
	}
	else
	{
		OspPrintf(TRUE, FALSE, "---get platform moid and guid ok\n");
	}

//. 是否获取到license
	if (g_cBmcLicenseApp.m_strLicenseInfo.length() == 0)
	{
		OspPrintf(TRUE, FALSE, ">>>license is empty\n");
	}
	else
	{
		OspPrintf(TRUE, FALSE, "---license is ok\n");
	}

//. 检测meeting数据库是否连接成功，及其存储过程是否存在
	m_cVMeetingDBOpr.PrintProcedureStatus();
	OspDelay(100);

//. lua脚本是否都load成功
	vector<string> vecFailedScript;
	LoadRedisLua(&vecFailedScript);
	if (vecFailedScript.size() == 0)
	{
		OspPrintf(TRUE, FALSE, "---lua script ok\n");
	}
	else
	{
		vector<string>::const_iterator iterScript = vecFailedScript.begin();
		for (; iterScript != vecFailedScript.end(); ++iterScript)
		{
			OspPrintf(TRUE, FALSE, ">>>lua script<%s> load fail\n", iterScript->c_str());
		}
	}
	
//. 是否连上数据库
	m_cConfInfoDBOpr.PrintConnectStatus();
	m_cTemplateDBOpr.PrintConnectStatus();
	m_cApInfoDBOpr.PrintConnectStatus();
	m_cScheduledDBOpr.PrintConnectStatus();
	OspDelay(100);
}

u16 CMauData::GetBookConfNum()
{
	//todu以后再加redis接口
	vector<CRedisBookConf> bookConfVec;
	GetAllBookConf(bookConfVec);
	return bookConfVec.size();
}


/*=============================================================================
函 数 名： TransConfE1642ShortNO
功    能： 将会议e164转换为电话呼叫短号
算法实现： 
全局变量： 
参    数： s8* pConfE164, s8* pShortNO
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
BOOL32 CMauData::TransConfE1642ShortNO( const s8* pConfE164, s8* pShortNO )
{
	if( NULL == pConfE164 || NULL == pShortNO )
	{
		return FALSE;
	}

	strncpy( pShortNO, pConfE164+1, MAXLEN_SHORTNO );
	pShortNO[MAXLEN_SHORTNO] = '\0';

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[TransConfE1642ShortNO]ConfE164.%s, ShortNO.%s.\n", pConfE164, pShortNO );

	return TRUE;
}

/*=============================================================================
函 数 名： GetConfDurationFromJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/22  5.0         阮楠楠                   创建
=============================================================================*/
void CMauData::GetConfDurationFromJson( s8 * pchJsonBuf, u16 &wConfDuration )
{
	if ( NULL == pchJsonBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfDurationFromJson] Param is NULL, return!\n" );
		return;
	}
	
	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL ==  pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfDurationFromJson] json_loads err!\n" );
		return;
	}
	
	json_t_ptr pjConfDuration = json_object_get_s( pjRoot, JSON_DURATION );
	if ( NULL == pjConfDuration )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfDurationFromJson] json_object_get ConfDuration is NULL!\n" );
	}
	else
	{
		wConfDuration = atoi( json_string_value( pjConfDuration ) );
	}

	return;
}

/*=============================================================================
函 数 名： GetConfPwdFromJson
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/22  5.0         阮楠楠                   创建
=============================================================================*/
void CMauData::GetConfPwdFromJson( s8 * pchJsonBuf, u8 & byNeedPwd, s8 * pchPwd, u32 dwPwdLen )
{
	if ( NULL == pchJsonBuf || NULL == pchPwd )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfPwdFromJson] Param is NULL, return!\n" );
		return;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL ==  pjRoot )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfPwdFromJson] json_loads err, return!\n" );
		return;
	}

	json_t_ptr pjNeedPwd = json_object_get_s( pjRoot, JSON_ISNEEDPWD );
	if ( NULL == pjNeedPwd )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfPwdFromJson] json_object_get NeedPwd is NULL!\n" );
	}
	else
	{
		byNeedPwd = (u8)atoi( json_string_value( pjNeedPwd ) );
	}
	
	if ( 1 == byNeedPwd )
	{
		json_t_ptr pjConfPwd = json_object_get_s( pjRoot, JSON_PSW );
		if ( NULL == pjConfPwd )
		{
			LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[GetConfPwdFromJson] json_object_get ConfPwd is NULL!\n" );
		}
		else
		{
			strncpy(pchPwd, json_string_value(pjConfPwd), MAXLEN_PWD);
		}
	}
	else
	{
		memset( pchPwd, 0, MAXLEN_PWD );
	}
	return;
}

/*=============================================================================
函 数 名： IsHduExist
功    能： 判断hdu是否存在
算法实现： 
全局变量： 
参    数： const CHduInfo & cHduInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/30  5.0         rnn                    创建
=============================================================================*/
BOOL32 CMauData::IsHduExist( CHduInfo & cHduInfo )
{
	BOOL32 bRet = (m_mapHduInfo.find( cHduInfo.GetEqpId() ) != m_mapHduInfo.end());
	return bRet ;
}

/*=============================================================================
函 数 名： AddHduInfo
功    能： 添加Hdu信息
算法实现： 
全局变量： 
参    数： const CHduInfo & cHduInfo
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/30  5.0         rnn                    创建
=============================================================================*/
void CMauData::AddHduInfo( CHduInfo & cHduInfo )
{
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[AddHduInfo]HduMOID<%s>!\n", cHduInfo.GetMoid() );
	m_mapHduInfo[cHduInfo.GetEqpId()] = cHduInfo;
}

/*=============================================================================
函 数 名： ModHduInfo
功    能： 修改Hdu信息
算法实现： 
全局变量： 
参    数： const CHduInfo & cOldInfo
           const CHduInfo & cNewInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/30  5.0         rnn                    创建
=============================================================================*/
BOOL32 CMauData::ModHduInfo(CHduInfo & cHduInfo)
{
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[ModHduInfo]HduMOID<%s>!\n", cHduInfo.GetMoid());
	BOOL32 bRet = FALSE;
	CHduInfoMap::iterator it = m_mapHduInfo.find(cHduInfo.GetEqpId());
	if (m_mapHduInfo.end() != it)
	{
		bRet = TRUE;
		it->second = cHduInfo;
	}

	return bRet;
}

/*=============================================================================
函 数 名： DelHduInfo
功    能： 删除Hdu信息
算法实现： 
全局变量： 
参    数： const CHduInfo & cHduInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/30  5.0         rnn                    创建
=============================================================================*/
BOOL32 CMauData::DelHduInfo( CHduInfo & cHduInfo )
{
	BOOL32 bRet = FALSE;
	CHduInfoMap::iterator it = m_mapHduInfo.find(cHduInfo.GetEqpId());
	if (it != m_mapHduInfo.end())
	{
		bRet = TRUE;
		m_mapHduInfo.erase(it);
	}
	return bRet;
}

/*=============================================================================
函 数 名： GetHduInfo
功    能： 获取Hdu信息
算法实现： 
全局变量： 
参    数： const CHduInfo & cKeyInfo
           CHduInfo & cOutInfo
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/30  5.0         rnn                    创建
=============================================================================*/
BOOL32 CMauData::GetHduInfo(u8 byHduId, CHduInfo & cOutInfo)
{
	BOOL32 bRet = FALSE;
	CHduInfoMap::iterator it = m_mapHduInfo.find(byHduId);
	if (it != m_mapHduInfo.end())
	{
		bRet = TRUE;
		cOutInfo = it->second;
	}
	return bRet;
}

/*=============================================================================
函 数 名： DeleteAllHduInfo
功    能： 删除map表中所有Hdu信息
算法实现： 
全局变量： 
参    数： void
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/30  5.0         rnn                    创建
=============================================================================*/
BOOL32 CMauData::DeleteAllHduInfo( void )
{
	m_mapHduInfo.clear();
	return TRUE;
}

/*=============================================================================
函 数 名： PrintHduList
功    能： 打印Hdu列表信息
算法实现： 
全局变量： 
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/30  5.0         rnn                    创建
=============================================================================*/
void CMauData::PrintHduList( void )
{
	for_each(m_mapHduInfo.begin(), m_mapHduInfo.end(),
		ptr_fun(Print<CMauData::CHduInfoMap::const_reference>));
	LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "Total Hdu info num<%d>\n", m_mapHduInfo.size() );

	return ;
}
void CMauData::PrintApDBHduInfo( void )
{
	::OspPrintf(TRUE, FALSE, "---Print All APDB Server_info HDU Moid AND UserDomainMoid---\n");
	s32 nIndex = 0;
	CHduUserDomainMoidMap::iterator it = m_mapHduUserDomainMoid.begin();
	for (; it != m_mapHduUserDomainMoid.end(); ++it)
	{
		++nIndex;
		::OspPrintf(TRUE, FALSE, "HDU:%d\n", nIndex);
		::OspPrintf(TRUE, FALSE, "Moid:					%s\n", it->first.c_str());
		::OspPrintf(TRUE, FALSE, "UserDomainMoid:		%s\n", it->second.c_str());
	}
	::OspPrintf(TRUE, FALSE, "HDU num is %d\n", nIndex);
}

BOOL32 CMauData::IsHduBelongToMe(const string &strDomainMoid, const string &strHduMoid)
{
	CHduUserDomainMoidMap::iterator it = m_mapHduUserDomainMoid.find(strHduMoid);
	if (it != m_mapHduUserDomainMoid.end() && it->second == strDomainMoid)
	{
		return TRUE;
	}

	return FALSE;
}

/*=============================================================================
函 数 名： ProSendHduInfo2CmByMoid
功    能： 将Hdu信息传给cpa
算法实现： 
全局变量： 
参    数： void
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/30  5.0         rnn                    创建
=============================================================================*/
BOOL32 CMauData::ProSendHduInfo2CmByMoid( void )
{
	AutoLock cAutoLock(&m_cDomainListLock);

	CMauMsg cServMsg;
	CKdvDomainInfoMap::iterator it = m_mapDomainInfo.begin();
	for (; it != m_mapDomainInfo.end(); it++)
	{
		if (it->second.GetDomainLevel() != CKdvDomainInfo::em_KdvUserDomain)
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[ProSendHduInfo2CmByMoid]DomainLevel is not KdvUserDomian\n");
			continue;
		}

		cServMsg.SetMsgBody(it->second.GetDomainMOID(), OPERATION_MAXLEN_MOID);
		u16 wHduNumByMoid = 0;
		CHduInfoMap::iterator itHduLoop = m_mapHduInfo.begin();
		while( m_mapHduInfo.end() != itHduLoop )
		{
			if (IsHduBelongToMe(it->second.GetDomainMOID(), itHduLoop->second.GetMoid()))
			{
				wHduNumByMoid++;
				cServMsg.CatMsgBody(&itHduLoop->second, sizeof(CHduInfo));
			}
			++itHduLoop;
		}
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[ProSendHduInfo2CmByMoid] KDVDomainMoid :%s, has %d Hdu", it->second.GetDomainMOID(), wHduNumByMoid);
		cServMsg.SetEventId( MAU_CM_HDULISTBYMOID_NTF );
		SendMsgToCmSsn( MAU_CM_HDULISTBYMOID_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
	}
	
	return TRUE;
}

BOOL32 CMauData::ProSendHduBelongDoaminOwnHduList2Cm(const string& strHdumoid)
{
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[ProSendHduBelongedDoaminOwnHduList2Cm]hduMoid: %s\n", strHdumoid.c_str());
	
	std::string strUserDomainMoid;
	if (!GetUserDomainMoidByHduMoid(strHdumoid, strUserDomainMoid))
	{
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[ProSendHduBelongedDoaminOwnHduList2Cm]GetHduUserDomainMoid Fail\n");
		return FALSE;
	}
	else
	{
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[ProSendHduBelongedDoaminOwnHduList2Cm]GetHduUserDomainMoid Sucess\n");
	}

	CMauMsg cServMsg;
	cServMsg.SetMsgBody( strUserDomainMoid.c_str(), OPERATION_MAXLEN_MOID);

	BOOL32 bIsDomainInfoHasAnyHdu = FALSE;
	CHduInfoMap::iterator hduIter = m_mapHduInfo.begin();
	for(; m_mapHduInfo.end() != hduIter; ++hduIter)
	{
		const string aHduMoid(hduIter->second.GetMoid());
	
		if (IsHduBelongToMe(strUserDomainMoid, aHduMoid))
		{
			bIsDomainInfoHasAnyHdu = TRUE;
			const CHduInfo cHduInfo(hduIter->second);
			cServMsg.CatMsgBody((s8*)&cHduInfo, sizeof(cHduInfo));
			LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[ProSendHduBelongedDoaminOwnHduList2Cm]HduMoid: %s belong to UserDomianMoid: %s\n",
				aHduMoid.c_str(), strUserDomainMoid.c_str());
		}
		else
		{
			LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[ProSendHduBelongedDoaminOwnHduList2Cm]HduMoid: %s don't belong to UserDomianMoid: %s\n",
				aHduMoid.c_str(), strUserDomainMoid.c_str());
		}
	}

	if (bIsDomainInfoHasAnyHdu)
	{
		cServMsg.SetEventId( MAU_CM_HDULISTBYMOID_NTF );
		SendMsgToCmSsn( MAU_CM_HDULISTBYMOID_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
	}
	return bIsDomainInfoHasAnyHdu;
}

BOOL32 CMauData::GetHduGuidBelongDoaminInfo(const std::string& hduGuid, CKdvDomainInfo& cKdvDomainInfo )
{
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[GetHduGuidBelongedDoaminInfo]hduGuid: %s\n", hduGuid.c_str());

	AutoLock cAutoLock(&m_cDomainListLock);

	CKdvDomainInfoMap::iterator domainIter = m_mapDomainInfo.begin();
	for (; domainIter != m_mapDomainInfo.end(); domainIter++)
	{
		const string domainGuid(domainIter->second.GetDomainGUID());
		if (IsHduBelongToMe(domainGuid.c_str(), hduGuid.c_str()))
		{
			LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[GetHduGuidBelongedDoaminInfo]hduGuid: %s belonged domainGuid: %s\n", 
				hduGuid.c_str(), domainGuid.c_str());
			cKdvDomainInfo = domainIter->second;
			break;
		}
		else
		{
			LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_VC, "[GetHduGuidBelongedDoaminInfo]hduGuid: %s don't belonged domainGuid: %s\n", 
				hduGuid.c_str(), domainGuid.c_str());
		}
	}
	const BOOL32 bIsHduBelongAnyDoamin = (domainIter != m_mapDomainInfo.end());
	return bIsHduBelongAnyDoamin;
}

/*=============================================================================
函 数 名： ProSendAllHduInfo2Cm
功    能： 将Hdu信息传给cm
算法实现： 
全局变量： 
参    数： void
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/30  5.0         rnn                    创建
=============================================================================*/
BOOL32 CMauData::ProSendAllHduInfo2Cm( void )
{
	AutoLock cAutoLock(&m_cDomainListLock);
	
	CMauMsg cServMsg;
	u16 wMoidNum = 0;
	u16 wHduNumByMoid = 0;

	CKdvDomainInfoMap::iterator it = m_mapDomainInfo.begin();
	for (; it != m_mapDomainInfo.end(); it++)
	{
		wHduNumByMoid = 0;
		CHduInfoMap::iterator itHduLoop = m_mapHduInfo.begin();
		while (m_mapHduInfo.end() != itHduLoop)
		{
			if (IsHduBelongToMe(it->second.GetDomainMOID(), itHduLoop->second.GetMoid()))
			{
				wHduNumByMoid++;
			}
			++itHduLoop;
		}

		if (0 != wHduNumByMoid)
		{
			wMoidNum++;
		}
	}
	if ( 0 != wMoidNum )
	{
		cServMsg.SetMsgBody( (u8*)&wMoidNum, sizeof(wMoidNum) );
		it = m_mapDomainInfo.begin();
		for (; it != m_mapDomainInfo.end(); it++)
		{

			CHduInfoMap::iterator itHduLoop = m_mapHduInfo.begin();
			std::vector<CHduInfo*> vecHduInfo;
			while( m_mapHduInfo.end() != itHduLoop )
			{
				if (IsHduBelongToMe(it->second.GetDomainMOID(), itHduLoop->second.GetMoid()))
				{
					vecHduInfo.push_back(&itHduLoop->second);
				}
				++itHduLoop;
			}

			wHduNumByMoid = vecHduInfo.size();

			if ( 0 != wHduNumByMoid )
			{
				cServMsg.CatMsgBody( (u8*)it->second.GetDomainMOID(), OPERATION_MAXLEN_MOID );
				cServMsg.CatMsgBody( (u8*)&wHduNumByMoid, sizeof(wHduNumByMoid) );
				std::vector<CHduInfo*>::iterator itHduInfo = vecHduInfo.begin();
				while (vecHduInfo.end() != itHduInfo)
				{
					cServMsg.CatMsgBody( (u8*)(*itHduInfo), sizeof( CHduInfo ) );
					++itHduInfo;
				}
			}					
		}
	}
	else
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[ProSendAllHduInfo2Cm]Now no Hdu in our company.\n" );
		return FALSE;
	}
	
	cServMsg.SetEventId( MAU_CM_ALLHDULIST_NTF );
	SendMsgToCmSsn( MAU_CM_ALLHDULIST_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen() );
	
	return TRUE;
}

// void CMauData::SetConfChargeToJson(const json_t_ptr& pjRoot, const CConfCacheData &cConfCacheData)
// {
// 	//confname
// 	SetStringToJson(pjRoot, "confName", cConfCacheData.GetConfName());
// 	//confe164
// 	SetStringToJson(pjRoot, JSON_CONFE164, cConfCacheData.GetConfE164());
// 	//createType
// 	SetUIntAsStringToJson(pjRoot, "createType", cConfCacheData.GetCreatorType());
// 	//creatorId
// 	SetStringToJson(pjRoot, "creatorId", cConfCacheData.GetCreatorMOID());
// 	//startTime
// 	SetUIntAsStringToJson(pjRoot, "startTime", (u32)cConfCacheData.GetBookStartTime());
// 	//endTime
// 	SetUIntAsStringToJson(pjRoot, "endTime", (u32)cConfCacheData.GetBookEndTime());
// 	//realStartTime
// 	SetUIntAsStringToJson(pjRoot, "realStartTime", (u32)cConfCacheData.GetConfStartTime());
// 	//realEndTime
// 	SetUIntAsStringToJson(pjRoot, "realEndTime", (u32)time(NULL));
// 	//bandWidth
// 	SetUIntAsStringToJson(pjRoot, "bandWidth", cConfCacheData.GetBitRate());
// 	
// 	//资源的暂时不做, 全部填0？
// 	//usedCp
// 	SetUIntAsStringToJson(pjRoot, "usedCp", 0);
// 	//usedAudmix
// 	SetUIntAsStringToJson(pjRoot, "usedAudmix", 0);
// 	//usedAdapt
// 	SetUIntAsStringToJson(pjRoot, "usedAdapt", 0);
// 	//usedRec
// 	SetUIntAsStringToJson(pjRoot, "usedRec", 0);
// 	//usedTvWall
// 	SetUIntAsStringToJson(pjRoot, "usedTvWall", 0);
// 	//userDomainMoid
// 	SetStringToJson(pjRoot, "userDomainMoid", cConfCacheData.GetDomainMOID());
// 	//portconf
// 	SetBoolAsStringToJson(pjRoot, "portMeeting", cConfCacheData.GetPortConf());
// }

void CMauData::SetPlatConfLimitNum(const CPlatformConfNumLimit& cPlatformConfNumLimit)
{
	m_cPlatformConfNumLimit = cPlatformConfNumLimit;
}

const CPlatformConfNumLimit& CMauData::GetPlatConfLimitNum() const
{
	return m_cPlatformConfNumLimit;
}

BOOL32 CMauData::GetBookStartTimeFromJson(const s8 * pchJsonBuf, s8* pBookStartTime, u32 dwTimeLen)
{
	if ( NULL == pchJsonBuf || NULL == pBookStartTime || 0 ==  dwTimeLen )
	{
		OspPrintf( TRUE, FALSE, "[GetBookStartTimeFromJson] Param is NULL, return FLASE!\n" );
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );

	if (NULL == pjRoot)
	{
		OspPrintf(TRUE, FALSE, "[GetBookStartTimeFromJson] json_loads err, return FALSE!\n");
		return FALSE;
	}

	return GetBookStartTimeFromJson(pjRoot, pBookStartTime, dwTimeLen);
}

BOOL32 CMauData::GetBookStartTimeFromJson(const json_t_ptr& pjRoot, s8* pBookStartTime, u32 dwTimeLen)
{
	json_t_ptr pjBookStartTime = json_object_get_s(pjRoot, JSON_BOOKSTARTTIME);
	if (NULL == pjBookStartTime)
	{
		OspPrintf(TRUE, FALSE, "[GetBookStartTimeFromJson] json_object_get pjBookStartTime is NULL, return FALSE!\n");
		return FALSE;
	}

	u32 dwLen = MAXLEN_ALIAS >= dwTimeLen ? dwTimeLen : MAXLEN_ALIAS;
	strncpy(pBookStartTime, json_string_value(pjBookStartTime), dwLen);

	return TRUE;
}

BOOL32 CMauData::ConstructChargeJson(const s8* pchConfE164, string& chargeJson)
{
	if (!m_cRedisData.GetChargeJson(pchConfE164, chargeJson))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Get conf<%s> ChargeJson failed\n",
			__FUNCTION__, pchConfE164);
		return FALSE;
	}
	TRedisConfInfo tRedisConf;
	if (!GetConfExInfo(pchConfE164, tRedisConf) 
		|| !IsRedisHasConfExData(tRedisConf))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]Get conf<%s> failed\n",
			__FUNCTION__, pchConfE164);
		return FALSE;
	}
	if (!SetChargeInfoToJson(tRedisConf, chargeJson))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]SetChargeInfoToJson err\n", __FUNCTION__);
		return FALSE;
	}
	return TRUE;
}