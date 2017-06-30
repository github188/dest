#include "kdvlog.h"
#include "loguserdef.h"
#include "config.h"
#include "bossutils.h"
#include "kdvsys.h"
#include "constuls.h"
#include "srvlistmgr.h"
#include "apsutility.h"

using namespace BossUtils;

#ifdef WIN32
#ifndef vsnprintf
#define vsnprintf   _vsnprintf
#endif
#ifndef snprintf
#define snprintf    _snprintf
#endif
#endif


CCfg g_cCfgFile;

CCfg::CCfg()
{
/*lint -save -e1566*/
	Init();
/*lint -restore*/
}

void CCfg::Init()
{
	memset(m_szCfgFileName, 0, sizeof(m_szCfgFileName));
#ifdef WIN32
    snprintf(m_szCfgFileName, sizeof(m_szCfgFileName), "%s%s\\%s", BossUtils::GetModuleFullPath().c_str(), APS_CONFIG_PATH, APS_CONFIG_NAME);
    m_szCfgFileName[ sizeof(m_szCfgFileName) - 1 ] = '\0';
#else
    snprintf(m_szCfgFileName, sizeof(m_szCfgFileName), "%s%s/%s", BossUtils::GetModuleFullPath().c_str(), APS_CONFIG_PATH, APS_CONFIG_NAME);
    m_szCfgFileName[ sizeof(m_szCfgFileName) - 1 ] = '\0';
#endif
    m_wOspListenPort = 0;
    m_wHttpListenPort = 0;
	m_wHttpsListenPort = 0;
	
    memset(m_szTelnetName, 0, sizeof(m_szTelnetName));
    memset(m_szTelnetPwd, 0, sizeof(m_szTelnetPwd));
    
    m_dwDBConNum = 0;
    m_tApDBConInfo.SetNull();
    m_tIpDbConInfo.SetNull();
    m_tMqConInfo.Init();

	m_dwMtTryApsSpan = DEFAULT_MTTRYAPSSPAN;

    memset(m_szLoaclIspName, 0, sizeof(m_szLoaclIspName));
    memset(m_szForeignIspName, 0, sizeof(m_szForeignIspName));

    m_bPxyMode = FALSE;
    //
    m_wStdH323Port = 0;
    m_wNonH323Port = 0;
    m_wSipPort = 0;
	m_bUseSipTlsPort = FALSE;
	m_wSipTlsPort = 0;
}

void CCfg::PrintCfg() const
{
	OspPrintf(TRUE, FALSE, "ServerDefPort:\n");
	for (map<u32, u16>::const_iterator serverIter = m_mapServerTypePort.begin();
	serverIter != m_mapServerTypePort.end(); ++serverIter)
	{
		OspPrintf(TRUE, FALSE, "Server<%s>: %u\n", GetServerNameByType(serverIter->first), serverIter->second);
	}
	OspPrintf(TRUE, FALSE, "Server<STDH323>: %u\n", GetStdH323Port());
	OspPrintf(TRUE, FALSE, "Server<NONH323>: %u\n", GetNonH323Port());
	OspPrintf(TRUE, FALSE, "Server<SIP>: %u\n", GetSipPort());
	OspPrintf(TRUE, FALSE, "ListenPort   : Osp(%u) Http(%u)\n", GetOspListenPort(), GetHttpListenPort());
	OspPrintf(TRUE, FALSE, "RabbitMqinfo : ip(%s)  port(%u) AuthName(%s) Pwd(%s)\n", GetMqConInfo().GetIP(),
		GetMqConInfo().GetPort(), GetMqConInfo().GetUsername(),
		GetMqConInfo().GetUserPwd());
	OspPrintf(TRUE, FALSE, "       \tVHost(%s) ExName(%s) RouteKey(%s) RcvQueue(%s)\n", GetMqConInfo().GetVHost(),
		GetMqConInfo().GetExchangeName(), GetMqConInfo().GetBindingkey(),
		GetMqConInfo().GetQueueName());
	OspPrintf(TRUE, FALSE, "LocalIspName : (%s) ForeignIspName:(%s) \n", GetLoaclIspName(), GetForeignIspName());
	OspPrintf(TRUE, FALSE, "%s info      : ip(%s) port(%u) AuthName(%s) Pwd(%s) EngineType(%u)\n", m_tApDBConInfo.GetDBName(),
		m_tApDBConInfo.GetDBIP(), m_tApDBConInfo.GetDBPort(), m_tApDBConInfo.GetDBUserName(),
		m_tApDBConInfo.GetDBUserPwd(), m_tApDBConInfo.GetDBEngineType());
	OspPrintf(TRUE, FALSE, "%s info : ip(%s) port(%u) AuthName(%s) Pwd(%s) EngineType(%u)\n", m_tIpDbConInfo.GetDBName(),
		m_tIpDbConInfo.GetDBIP(), m_tIpDbConInfo.GetDBPort(), m_tIpDbConInfo.GetDBUserName(),
		m_tIpDbConInfo.GetDBUserPwd(), m_tIpDbConInfo.GetDBEngineType());
	OspPrintf(TRUE, FALSE, "login backup aps timeout: %u ms\n", m_dwMtTryApsSpan);
	OspPrintf(TRUE, FALSE, "GUID: %s\n", m_strApsGUID.c_str());
	return;
}

std::string CCfg::GetApsGUID() const
{
	return m_strApsGUID;
}

#ifdef _APS_PXY_
void CCfg::PrintHaPxyInfo() const
{
	std::set<CSrvAddrInfo>::iterator iter = m_atHaPxyInfo.begin();
	printf("Print HaProxy Config:\n");
	OspPrintf(TRUE, FALSE,"Print HaProxy Config:\n");
	while ( iter != m_atHaPxyInfo.end() )
	{
		(*iter).Print();
		iter++;
	}
	printf("Print Local Isp Config:\n");
	OspPrintf(TRUE, FALSE,"Print Local Isp Config:\n");
	iter = m_atLocalIspInfo.begin();
	while ( iter != m_atLocalIspInfo.end() )
	{
		(*iter).Print();
		iter++;
	}
	return;
}

BOOL32 CCfg::GetPlatformSrvAddrInfo(const u8 byIspType, const s8 *pszMtMoId, CPlatformSrvAddrInfo &cPlatformSvrAddrInfo)
{
	//选IP地址
	CSrvAddrInfo cAddr;
	BOOL32 bFound = FALSE;
	std::set<CSrvAddrInfo>::iterator iter = m_atHaPxyInfo.begin();
	while ( iter != m_atHaPxyInfo.end() )
	{
		if ( byIspType == iter->GetIspType() )
		{
			if ( pszMtMoId )
			{
				if ( strlen(pszMtMoId) == strlen(iter->GetPlatformMoId())
					&& memcmp(pszMtMoId, iter->GetPlatformMoId(), strlen(pszMtMoId)) == 0 )
				{
					cAddr = *iter;
				}
				else
				{
					iter++;
					continue;
				}
			}
			else
			{
				cAddr = *iter;
			}
			bFound = TRUE;
			LogPrint(LOG_LVL_KEYSTATUS, MID_ENTRANCE_APS, "[%s]find Ip(0x%x) for Isp:%s IspType:%u\n", __FUNCTION__,
				cAddr.GetIpAddr(), cAddr.GetNetDomainName(), byIspType);
			break;
		}
		iter++;
	}

	//没选到，随机选一个
	if ( !bFound )
	{
		srand(OspTickGet());
		int nIndex = rand()%m_atHaPxyInfo.size();
		iter = m_atHaPxyInfo.begin();
		while (nIndex-- > 0)
		{
			iter++;
		}
		cAddr = *iter;
		LogPrint(LOG_LVL_KEYSTATUS, MID_ENTRANCE_APS, "[%s]assign random Ip(0x%x) and Isp:%s for IspType:%u\n",
			__FUNCTION__, cAddr.GetIpAddr(), cAddr.GetNetDomainName(), byIspType);
	}

	//拼地址
	// 1. Nds
	cAddr.SetPort(g_cCfgFile.GetNdsPort());
	cPlatformSvrAddrInfo.SetNdsAddrInfo( cAddr );

	// 2.Nms
	cAddr.SetPort( g_cCfgFile.GetNmsPort() );
	cPlatformSvrAddrInfo.SetNmsAddrInfo( cAddr );

	// 3.Sns : Kedacom Micro Blog
	cAddr.SetPort( g_cCfgFile.GetSnsPort() );
	cPlatformSvrAddrInfo.SetWeiboAddrInfo( cAddr );

	// 4.Cmc: Kedacom Meeting Conference
	cAddr.SetPort( g_cCfgFile.GetCmcPort() );
	cPlatformSvrAddrInfo.SetMeetingAddrInfo( cAddr );
	cPlatformSvrAddrInfo.SetPlatApiAddrInfo( cAddr );

	// 5.Xns: XMPP
	cAddr.SetPort( g_cCfgFile.GetXnsPort() );
	cPlatformSvrAddrInfo.SetXmppAddrInfo( cAddr );

	// 7.Vrs
	cAddr.SetPort( g_cCfgFile.GetVrsPort() );
	cPlatformSvrAddrInfo.SetVrsAddrInfo( cAddr );

	// 8.Dcs
	cAddr.SetPort( g_cCfgFile.GetDcsPort() );
	cPlatformSvrAddrInfo.SetDcsAddrInfo( cAddr );

	// 10. Sus
	cAddr.SetPort( g_cCfgFile.GetSusPort() );
	cPlatformSvrAddrInfo.SetSusAddrInfo( cAddr );

	// 11. Aps list不需要，代理后续不过平台域的Aps
	//接入的地址看本地配置
	iter = m_atLocalIspInfo.begin();
	bFound = FALSE;
	while ( iter != m_atLocalIspInfo.end() )
	{
		if ( byIspType == iter->GetIspType() )
		{
			cAddr = *iter;
			bFound = TRUE;
			LogPrint(LOG_LVL_KEYSTATUS, MID_ENTRANCE_APS, "[%s][H323 Sip Proxy] find Ip(0x%x) for Isp:%s IspType:%u\n", __FUNCTION__,
				cAddr.GetIpAddr(), cAddr.GetNetDomainName(), byIspType);
			break;
		}
		iter++;
	}
	
	//没选到，随机选一个
	if ( !bFound )
	{
		srand(OspTickGet());
		int nIndex = rand()%m_atLocalIspInfo.size();
		iter = m_atLocalIspInfo.begin();
		while (nIndex-- > 0)
		{
			iter++;
		}
		cAddr = *iter;
		LogPrint(LOG_LVL_KEYSTATUS, MID_ENTRANCE_APS, "[%s][H323 Sip Proxy]assign random Ip(0x%x) and Isp:%s for IspType:%u\n",
			__FUNCTION__, cAddr.GetIpAddr(), cAddr.GetNetDomainName(), byIspType);
	}
	
	// 6.Ntp也是使用Udp
	cAddr.SetPort( g_cCfgFile.GetNtpPort() );
	cPlatformSvrAddrInfo.SetNtpAddrInfo( cAddr );

	// 9.1 H323Pxy
	cAddr.SetPort( g_cCfgFile.GetStdH323Port() );
	cPlatformSvrAddrInfo.SetStdH323AddrInfo( cAddr );
	
	// 9.2 H323 NStdPxy
	cAddr.SetPort( g_cCfgFile.GetNonH323Port() );
	cPlatformSvrAddrInfo.SetNonH323AddrInfo( cAddr );
	
	// 9.3 SipPxy
	cAddr.SetPort( g_cCfgFile.GetSipPort() );
	cPlatformSvrAddrInfo.SetSipAddrInfo( cAddr );
	
	return TRUE;
}
#endif

BOOL32 CCfg::ReadConfigInfo()
{
	BOOL32 bRet = FALSE;
    //原有数据清零
    Init();
	
    // 本地信息
    bRet = LoadCfgLocalInfo();	
    if (!bRet)
    {
        OspPrintf(TRUE, FALSE, "[ReadConfigInfo] Error while reading Local info!\n");
        return bRet;
    }
	
    // AP数据库配置部分
    bRet = LoadApDBCfgInfo();	
    if (!bRet)
    {
        OspPrintf(TRUE, FALSE, "[ReadConfigInfo] Error while reading ApDB info!\n");
        return bRet;
    }

    // IP数据库配置部分
    bRet = LoadIpDBCfgInfo();
    if (!bRet)
    {
        OspPrintf(TRUE, FALSE, "[ReadConfigInfo] Error while reading IpDB info!\n");
        return bRet;
    }

    // MQ
    bRet = LoadMqCfgInfo();
    if (!bRet)
    {
        OspPrintf(TRUE, FALSE, "[ReadConfigInfo] Error while reading Mq info!\n");
        return bRet;
    }

    //服务器端口
    bRet = LoadSrvPortInfo();
    if (!bRet)
    {
        OspPrintf(TRUE, FALSE, "[ReadConfigInfo] Error while reading SrvPort info!\n");
        return bRet;
    }
    
	// HaProxy的服务信息
	bRet = LoadHaPxyInfo();
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_ENTRANCE_APS, "[%s]LoadHaPxyInfo failed\n", __FUNCTION__);
		return FALSE;
	}

	LoadGUID();
	LoadTestLoginInfo();
    return TRUE;
}

BOOL32 CCfg::LoadCfgLocalInfo()
{
	BOOL32 bRet = FALSE;
	s32  nKeyVal = 0 ;
	
	//OSP 监听端口
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_LOCALINFO, KEY_OSP_LISTEN_PORT, APS_OSP_LISTEN_PORT, &nKeyVal);
    m_wOspListenPort = (u16)nKeyVal;	
    if (FALSE == bRet || 1024 > m_wOspListenPort)
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgLocalInfo] Error while reading %s !\n", KEY_OSP_LISTEN_PORT);
    }

    //HTTP 监听端口
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_LOCALINFO, KEY_HTTP_LISTEN_PORT, APS_HTTP_LISTEN_PORT, &nKeyVal);
    m_wHttpListenPort = (u16)nKeyVal;	
    if (FALSE == bRet || 1024 > m_wHttpListenPort)
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgLocalInfo] Error while reading %s !\n", KEY_HTTP_LISTEN_PORT);
    }

	//HTTPS 监听端口
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_LOCALINFO, KEY_HTTPS_LISTEN_PORT, APS_HTTPS_LISTEN_PORT, &nKeyVal);
    m_wHttpsListenPort = (u16)nKeyVal;	
    if (FALSE == bRet || 1024 > m_wHttpsListenPort)
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgLocalInfo] Error while reading %s !\n", KEY_HTTPS_LISTEN_PORT);
    }
   
    //Telnet用户名
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_LOCALINFO, KEY_TELNET_NAME, DEFAULT_TELNET_NAME,
		m_szTelnetName, sizeof(m_szTelnetName));	
    if (FALSE == bRet)
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgLocalInfo] Error while reading %s!\n", KEY_TELNET_NAME);
    }	
    m_szTelnetName[sizeof(m_szTelnetName) - 1] = '\0';

    //Telnet端口
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_LOCALINFO, KEY_TELNET_PWD, DEFAULT_TELNET_PWD,
		m_szTelnetPwd, sizeof(m_szTelnetPwd));	
    if (FALSE == bRet)
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgLocalInfo] Error while reading %s!\n", KEY_TELNET_PWD);
    }	
    m_szTelnetPwd[sizeof(m_szTelnetPwd) - 1] = '\0';

    //本地运营商名
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_LOCALINFO, KEY_LOACLISP_NAME, DEFAULT_LOACLISP_NAME,
        m_szLoaclIspName, sizeof(m_szLoaclIspName));	
    if (FALSE == bRet)
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgLocalInfo] Error while reading %s!\n", KEY_LOACLISP_NAME);
    }	
    m_szLoaclIspName[sizeof(m_szLoaclIspName) - 1] = '\0';

    //海外运营商名
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_LOCALINFO, KEY_FOREIGNISP_PWD, DEFAULT_FOREIGNISP_PWD,
        m_szForeignIspName, sizeof(m_szForeignIspName));	
    if (FALSE == bRet)
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgLocalInfo] Error while reading %s!\n", KEY_FOREIGNISP_PWD);
    }	
    m_szForeignIspName[sizeof(m_szForeignIspName) - 1] = '\0';

    //是否代理模式
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_LOCALINFO, KEY_ISPXYMODE, 0, &nKeyVal);
    m_bPxyMode = (BOOL32)nKeyVal;	
    if (FALSE == bRet)
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgLocalInfo] Error while reading %s !\n", KEY_ISPXYMODE);
    }
    
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_LOCALINFO, "mttryapsspan", DEFAULT_MTTRYAPSSPAN, &nKeyVal);
    if (FALSE == bRet)
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgLocalInfo] Error while reading mttryapsspan !\n");
    }
	m_dwMtTryApsSpan = nKeyVal;
    return TRUE;
}

BOOL32 CCfg::LoadApDBCfgInfo()
{
	m_tApDBConInfo.SetNull();
    BOOL32 bRet = FALSE;
    s32 nKeyVal;
	
    // 数据库IP
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_AP_DBCON, KEY_DBIP, DEFAULT_DBIP,
		m_tApDBConInfo.m_achDBIP, sizeof(m_tApDBConInfo.m_achDBIP));	
    if (!bRet || 0 == strlen(m_tApDBConInfo.m_achDBIP))
    {
        OspPrintf(TRUE, FALSE, "[LoadApDBCfgInfo] Error while reading %s!\n", KEY_DBIP);
    }	
    m_tApDBConInfo.m_achDBIP[sizeof(m_tApDBConInfo.m_achDBIP) - 1] = '\0';

    // 数据端口
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_AP_DBCON, KEY_DBPORT, DEFAULT_DBPORT, &nKeyVal);	
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadApDBCfgInfo] Error while reading %s!\n", KEY_DBPORT);
    }	
    m_tApDBConInfo.m_wDBPort = (u16)nKeyVal;

    // 数据库名称
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_AP_DBCON, KEY_DBNAME, DEFAULT_APDBNAME,
		m_tApDBConInfo.m_achDBName, sizeof(m_tApDBConInfo.m_achDBName));	
    if (!bRet || 0 == strlen(m_tApDBConInfo.m_achDBName))
    {
        OspPrintf(TRUE, FALSE, "[LoadApDBCfgInfo] Error while reading %s!\n", KEY_DBNAME);
    }	
    m_tApDBConInfo.m_achDBName[sizeof(m_tApDBConInfo.m_achDBName) - 1] = '\0';

    // 数据库用户名
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_AP_DBCON, KEY_DBUSERNAME, DEFAULT_DB_USERNAME,
		m_tApDBConInfo.m_achDBUserName, sizeof(m_tApDBConInfo.m_achDBUserName));	
    if (!bRet || 0 == strlen(m_tApDBConInfo.m_achDBUserName))
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgValueFromFile] Error while reading %s!\n", KEY_DBUSERNAME);
    }	
    m_tApDBConInfo.m_achDBUserName[sizeof(m_tApDBConInfo.m_achDBUserName) - 1] = '\0';

    // 数据库密码
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_AP_DBCON, KEY_DBUSERPWD, DEFAULT_DB_USERPWD,
		m_tApDBConInfo.m_achDBUserPwd, sizeof(m_tApDBConInfo.m_achDBUserPwd));	
    if (!bRet)
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgValueFromFile] Error while reading %s!\n", KEY_DBUSERPWD);
    }	
    m_tApDBConInfo.m_achDBUserPwd[sizeof(m_tApDBConInfo.m_achDBUserPwd) - 1] = '\0';

    // 数据库引擎类型
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_AP_DBCON, KEY_DBENGINETYPE, DEFAULT_DB_ENGINETYPE, &nKeyVal);
    if (!bRet)
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgValueFromFile] Error while reading %s!\n", KEY_DBENGINETYPE);
    }	
    m_tApDBConInfo.m_emDBEngineType = (EmDBEngineType)nKeyVal;
	
    if (m_tApDBConInfo.m_emDBEngineType > Em_DBEngineType_Mysql)
    {
        m_tApDBConInfo.m_emDBEngineType = Em_DBEngineType_Mysql;
    }
	
    return TRUE;
}

BOOL32 CCfg::LoadIpDBCfgInfo()
{
    m_tIpDbConInfo.SetNull();
    BOOL32 bRet = FALSE;
    s32 nKeyVal;

    // 数据库IP
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_IP_DBCON, KEY_DBIP, DEFAULT_DBIP,
        m_tIpDbConInfo.m_achDBIP, sizeof(m_tIpDbConInfo.m_achDBIP));
    if (!bRet || 0 == strlen(m_tIpDbConInfo.m_achDBIP))
    {
        OspPrintf(TRUE, FALSE, "[LoadIpDBCfgInfo] Error while reading %s!\n", KEY_DBIP);
    }
    m_tIpDbConInfo.m_achDBIP[sizeof(m_tIpDbConInfo.m_achDBIP) - 1] = '\0';

    // 数据端口
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_IP_DBCON, KEY_DBPORT, DEFAULT_DBPORT, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadIpDBCfgInfo] Error while reading %s!\n", KEY_DBPORT);
    }
    m_tIpDbConInfo.m_wDBPort = (u16)nKeyVal;

    // 数据库名称
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_IP_DBCON, KEY_DBNAME, DEFAULT_APDBNAME,
        m_tIpDbConInfo.m_achDBName, sizeof(m_tIpDbConInfo.m_achDBName));
    if (!bRet || 0 == strlen(m_tIpDbConInfo.m_achDBName))
    {
        OspPrintf(TRUE, FALSE, "[LoadIpDBCfgInfo] Error while reading %s!\n", KEY_DBNAME);
    }
    m_tIpDbConInfo.m_achDBName[sizeof(m_tIpDbConInfo.m_achDBName) - 1] = '\0';

    // 数据库用户名
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_IP_DBCON, KEY_DBUSERNAME, DEFAULT_DB_USERNAME,
        m_tIpDbConInfo.m_achDBUserName, sizeof(m_tIpDbConInfo.m_achDBUserName));
    if (!bRet || 0 == strlen(m_tIpDbConInfo.m_achDBUserName))
    {
        OspPrintf(TRUE, FALSE, "[LoadCfgValueFromFile] Error while reading %s!\n", KEY_DBUSERNAME);
    }
    m_tIpDbConInfo.m_achDBUserName[sizeof(m_tIpDbConInfo.m_achDBUserName) - 1] = '\0';

    // 数据库密码
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_IP_DBCON, KEY_DBUSERPWD, DEFAULT_DB_USERPWD,
        m_tIpDbConInfo.m_achDBUserPwd, sizeof(m_tIpDbConInfo.m_achDBUserPwd));
    if (!bRet)
    {
        OspPrintf(TRUE, FALSE, "[LoadIpDBCfgInfo] Error while reading %s!\n", KEY_DBUSERPWD);
    }
    m_tIpDbConInfo.m_achDBUserPwd[sizeof(m_tIpDbConInfo.m_achDBUserPwd) - 1] = '\0';

    // 数据库引擎类型
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_IP_DBCON, KEY_DBENGINETYPE, DEFAULT_DB_ENGINETYPE, &nKeyVal);
    if (!bRet)
    {
        OspPrintf(TRUE, FALSE, "[LoadIpDBCfgInfo] Error while reading %s!\n", KEY_DBENGINETYPE);
    }
    m_tIpDbConInfo.m_emDBEngineType = (EmDBEngineType)nKeyVal;
    if (m_tIpDbConInfo.m_emDBEngineType > Em_DBEngineType_Mysql)
    {
        m_tIpDbConInfo.m_emDBEngineType = Em_DBEngineType_Mysql;
    }

    return TRUE;
}

BOOL32 CCfg::LoadMqCfgInfo(void)
{
    m_tMqConInfo.Init();
    BOOL32 bRet = FALSE;
    s32 nKeyVal = 0;
    s8 szValue[1024+1] = {0};

    // MQ服务器IP
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_MQCON, KEY_MQIP, DEFAULT_MQIP, szValue, sizeof(szValue));
    if (!bRet || 0 == strlen(szValue))
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", DEFAULT_MQIP);
    }
    m_tMqConInfo.SetIP(szValue);

    // port
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_MQCON, KEY_MQPORT, DEFAULT_MQPORT, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_MQPORT);
    }
    m_tMqConInfo.SetPort(nKeyVal);

    // vhost
    memset(szValue, 0, sizeof(szValue));
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_MQCON, KEY_MQVHOST, DEFAULT_MQVHOST, szValue, sizeof(szValue));
    if (!bRet || 0 == strlen(szValue))
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_MQVHOST);
    }
    m_tMqConInfo.SetVhost(szValue);

    // 用户名
    memset(szValue, 0, sizeof(szValue));
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_MQCON, KEY_MQUSERNAME, DEFAULT_MQUSERNAME, szValue, sizeof(szValue));
    if (!bRet || 0 == strlen(szValue))
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_MQUSERNAME);
    }
    m_tMqConInfo.SetUsername(szValue);

    // 密码
    memset(szValue, 0, sizeof(szValue));
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_MQCON, KEY_MQUSERPWD, DEFAULT_MQUSERPWD, szValue, sizeof(szValue));
    if (!bRet || 0 == strlen(szValue))
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_MQUSERPWD);
    }
    m_tMqConInfo.SetUserPwd(szValue);

    // Exchange
    memset(szValue, 0, sizeof(szValue));
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_MQCON, KEY_MQEXCHANGENAME, DEFAULT_MQEXCHANGENAME, szValue, sizeof(szValue));
    if (!bRet || 0 == strlen(szValue))
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_MQEXCHANGENAME);
    }
    m_tMqConInfo.SetExchangeName(szValue);

    // Queue
    memset(szValue, 0, sizeof(szValue));
    bRet = GetRegKeyString(m_szCfgFileName, SECTION_MQCON, KEY_MQQueueName, DEFAULT_MQQueueName, szValue, sizeof(szValue));
    if (!bRet || 0 == strlen(szValue))
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_MQQueueName);
    }
    m_tMqConInfo.SetQueueName(szValue);

    return TRUE;
}

BOOL32 CCfg::LoadSrvPortInfo(void)
{
    BOOL32 bRet = FALSE;
    s32 nKeyVal = 0;

    // NDS
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_NDS, 0, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_NDS);
    }
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_NDS] = nKeyVal;

    // NMS
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_NMS, 0, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_NMS);
    }
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_NMS_C] = nKeyVal;
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_NMS_6000] = nKeyVal;

    // SNS
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_SNS, 0, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_SNS);
    }
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_SNS] = nKeyVal;

    // CMC
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_CMC, 0, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_CMC);
    }
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_CMC] = nKeyVal;

    // XNS
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_XNS, 0, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_XNS);
    }
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_XNS] = nKeyVal;

    // NTP
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_NTP, 0, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_NTP);
    }
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_NTP] = nKeyVal;

    // VRS
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_VRS, 0, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_VRS);
    }
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_VRS2000] = nKeyVal;
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_VRS4000] = nKeyVal;

    // DCS
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_DCS, 0, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_DCS);
    }
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_DCS2000] = nKeyVal;

    // SUS
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_SUS, 63000, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_SUS);
    }
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_SUS] = nKeyVal;

    // StdH323
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_STDH323, 1719, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_STDH323);
    }
    m_wStdH323Port = nKeyVal;

    // NonH323
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_NONH323, 1759, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_NONH323);
    }
    m_wNonH323Port = nKeyVal;

    // SIP
    bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_SIP, 5060, &nKeyVal);
    if (!bRet || 0 == nKeyVal)
    {
        OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_SIP);
    }
    m_wSipPort = nKeyVal;

	
	bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_USESRVTLSPORT_SIP, FALSE, &nKeyVal);
	if (!bRet || 0 == nKeyVal)
	{
		OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_USESRVTLSPORT_SIP);
	}
	m_bUseSipTlsPort = nKeyVal;


	// SIP TlsPort
	if(TRUE == m_bUseSipTlsPort)
	{
		bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVTLSPORT_SIP, 0, &nKeyVal);
		if (!bRet || 0 == nKeyVal)
		{
			OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVTLSPORT_SIP);
		}
		m_wSipTlsPort = nKeyVal;

	}

	//RestApi
	bRet = GetRegKeyInt(m_szCfgFileName, SECTION_SRVPORT, KEY_SRVPORT_RESTAPI, 80, &nKeyVal);
	if (!bRet || 0 == nKeyVal)
	{
		OspPrintf(TRUE, FALSE, "[LoadMqCfgInfo] Error while reading %s!\n", KEY_SRVPORT_RESTAPI);
	}
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_RESTAPI] = nKeyVal;

	//aps
	m_mapServerTypePort[DEVICE_GLOBAL_TYPE_APS] = APS_HTTP_LISTEN_PORT;
    return TRUE;
}

BOOL32 CCfg::LoadTestLoginInfo()
{
    s8 szValue[1024+1] = {0};
    GetRegKeyString(m_szCfgFileName, "test", "e164", "", szValue, sizeof(szValue));
	m_cTestLoginInfo.SetUserName(szValue);
    GetRegKeyString(m_szCfgFileName, "test", "pwd", "888888", szValue, sizeof(szValue));
	m_cTestLoginInfo.SetPassWord(szValue);
    GetRegKeyString(m_szCfgFileName, "test", "oem", "", szValue, sizeof(szValue));
	m_cTestLoginInfo.SetOemMark(szValue);
    GetRegKeyString(m_szCfgFileName, "test", "type", "TrueLink", szValue, sizeof(szValue));
	m_cTestLoginInfo.SetDeviceType(szValue);
    GetRegKeyString(m_szCfgFileName, "test", "apsip", "127.0.0.1", szValue, sizeof(szValue));
	m_cTestLoginInfo.SetApsLocalIp(BossUtils::GetIpByString(szValue));
    GetRegKeyString(m_szCfgFileName, "test", "clientip", "127.0.0.1", szValue, sizeof(szValue));
	m_cTestLoginInfo.SetClientIp(BossUtils::GetIpByString(szValue));
	return TRUE;
}

BOOL32 CCfg::SetGUID(const s8* pchGUID)
{
	if (pchGUID == NULL)
	{
		return FALSE;
	}
	m_strApsGUID = pchGUID;
	return TRUE;
}

BOOL32 CCfg::LoadGUID()
{
#ifdef WIN32
	s8 achGUID[OPERATION_MAXLEN_GUID + 1] = { 0 };
    if(!GetRegKeyString(m_szCfgFileName, "windows", "guid", "00000000000000000000000000000000", achGUID, sizeof(achGUID)))
    {
        OspPrintf(TRUE, FALSE, "[%s] Error while reading GUID!\n", __FUNCTION__);
    }	
	m_strApsGUID = achGUID;
#endif
	return TRUE;
}

BOOL32 CCfg::LoadHaPxyInfo()
{
#ifdef _APS_PXY_
	// ApsPxy不需要sa,所以要从配置项里读Guid
	m_atHaPxyInfo.clear();
	s8 achKey[128] = {0};
	s8 achValue[256] = {0};
	s32 nVal = 0;
	const s8 achDelim[] = ", \t";
	s8 *pchToken = NULL;
	if ( !GetRegKeyInt(m_szCfgFileName, SECTION_PXYCONFIG, "EntryNum", 0, &nVal) )
	{
		LogPrint(LOG_LVL_ERROR, MID_ENTRANCE_APS, "[%s]Get EntryNum of %s failed\n", __FUNCTION__, SECTION_PXYCONFIG);
		return FALSE;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_ENTRANCE_APS, "[%s]Get EntryNum(%u) of %s succeed\n", __FUNCTION__, nVal, SECTION_PXYCONFIG);

	CSrvAddrInfo cPxySrvInfo;
	s32 nLoop = 0;
	for (nLoop = 0; nLoop < nVal; nLoop++ )
	{
		memset(achKey, 0, sizeof(achKey));
		sprintf(achKey, "Entry%d", nLoop);
		if ( !GetRegKeyString(m_szCfgFileName, SECTION_PXYCONFIG, achKey, NULL, achValue, sizeof(achValue)-1) )
		{
			LogPrint(LOG_LVL_ERROR, MID_ENTRANCE_APS, "[%s]Get %s of %s failed\n", __FUNCTION__, achKey, SECTION_PXYCONFIG);
			continue;
		}
		//格式：EntryX = 192.168.1.10     www.Kedacom.com   10107000000002000000000000000001  中国联通/或者参照srvmodule_nu.h里的宏定义:2
		//1.ip
		pchToken = strtok(achValue, achDelim);
		if ( !pchToken )
		{
			continue;
		}
		cPxySrvInfo.SetIpAddr( ntohl(inet_addr(pchToken)) );
		//2. domain name
		pchToken = strtok(NULL, achDelim);
		if ( !pchToken )
		{
			continue;
		}
		cPxySrvInfo.SetNetDomainName(pchToken);
		//3. Plat MoId
		pchToken = strtok(NULL, achDelim);
		if ( !pchToken )
		{
			continue;
		}
		cPxySrvInfo.SetPlatformMoId(pchToken);
		//4. Isp Name
		pchToken = strtok(NULL, achDelim);
		if ( !pchToken )
		{
			continue;
		}
		//if is digital
		EOperatorsType emIspType = emTypeQT;
		if ( isdigit(pchToken[0]) )
		{
			emIspType = (EOperatorsType)atoi(pchToken);
		}
		else // else is String
		{
			std::string strIspName = pchToken;
			emIspType = TransIspNameToType(strIspName);
		}
		cPxySrvInfo.SetIspType( emIspType );
		cPxySrvInfo.Print(); //打印
		m_atHaPxyInfo.insert(cPxySrvInfo);
	}

	// Read local Isp info
	m_atLocalIspInfo.clear();
	if ( !GetRegKeyInt(m_szCfgFileName, SECTION_LOCALISPCONFIG, "EntryNum", 0, &nVal) )
	{
		LogPrint(LOG_LVL_ERROR, MID_ENTRANCE_APS, "[%s]Get EntryNum of %s failed\n", __FUNCTION__, SECTION_LOCALISPCONFIG);
		return FALSE;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_ENTRANCE_APS, "[%s]Get EntryNum(%u) of %s succeed\n", __FUNCTION__, nVal, SECTION_LOCALISPCONFIG);
	cPxySrvInfo.Clear();
	for (nLoop = 0; nLoop < nVal; nLoop++ )
	{
		memset(achKey, 0, sizeof(achKey));
		sprintf(achKey, "Entry%d", nLoop);
		if ( !GetRegKeyString(m_szCfgFileName, SECTION_LOCALISPCONFIG, achKey, NULL, achValue, sizeof(achValue)-1) )
		{
			LogPrint(LOG_LVL_ERROR, MID_ENTRANCE_APS, "[%s]Get %s of %s failed\n", __FUNCTION__, achKey, SECTION_PXYCONFIG);
			continue;
		}
		//格式：EntryX = 192.168.1.10 中国联通/或者参照srvmodule_nu.h里的宏定义:2
		//1.ip
		pchToken = strtok(achValue, achDelim);
		if ( !pchToken )
		{
			continue;
		}
		cPxySrvInfo.SetIpAddr( ntohl(inet_addr(pchToken)) );
		//4. Isp Name
		pchToken = strtok(NULL, achDelim);
		if ( !pchToken )
		{
			continue;
		}
		//if is digital
		EOperatorsType emIspType = emTypeQT;
		if ( isdigit(pchToken[0]) )
		{
			emIspType = (EOperatorsType)atoi(pchToken);
		}
		else // else is String
		{
			std::string strIspName = pchToken;
			emIspType = TransIspNameToType(strIspName);
		}
		cPxySrvInfo.SetIspType( emIspType );
		cPxySrvInfo.Print(); //打印
		m_atLocalIspInfo.insert(cPxySrvInfo);
	}
#endif
	return TRUE;
}

BOOL32 CCfg::IsValidIp(const s8* pachIp)
{
	if ( !pachIp )
	{
		return FALSE;
	}

	//是否为"0" "0.0.0.0"
	//是否为"255.255.255.255"
	u32 dwIp = inet_addr(pachIp);
	if ( dwIp == 0 || dwIp == (u32)0xFFFFFFFF)
	{
		return FALSE;
	}

	return TRUE;
}

#ifdef _APS_PXY_
u8 CCfg::GetIspTypeByLoginApsIp(const u32 & dwApsLocalIp) const
{
	std::set<CSrvAddrInfo>::const_iterator iter = m_atLocalIspInfo.begin();
	while ( iter != m_atLocalIspInfo.end() )
	{
		if ( iter->GetIpAddr() == dwApsLocalIp )
		{
			OspPrintf(TRUE, FALSE, "[%s]Get Isp Type(%u) Ip(%x) Succeed\n",
				__FUNCTION__, (int)iter->GetIspType(), dwApsLocalIp);
			return iter->GetIspType();
		}
		iter++;
	}
	OspPrintf(TRUE, FALSE, "[%s]Get Ip(%x) Isp Type failed\n", __FUNCTION__, dwApsLocalIp);
	return (u8)emTypeBD;
}
#endif

std::vector<TDBConnectInfo> CCfg::GetDBConInfo() const
{
    std::vector<TDBConnectInfo> tInfo;
	/*
	 *	规格确认：IP填0或255.255.255.255，则认为是无效地址，不需要上报
	 */
	if ( CCfg::IsValidIp(m_tApDBConInfo.GetDBIP()) )
	{
		OspPrintf(TRUE, FALSE, "%s:%d ApDB ip:%s IsValid\n", __FUNCTION__, __LINE__, m_tApDBConInfo.GetDBIP() );
		printf("%s:%d ApDB ip:%s IsValid\n", __FUNCTION__, __LINE__, m_tApDBConInfo.GetDBIP() );
		tInfo.push_back(m_tApDBConInfo);
	}
	else
	{
		OspPrintf(TRUE, FALSE, "%s:%d ApDB ip:%s Is NOT Valid\n", __FUNCTION__, __LINE__, m_tApDBConInfo.GetDBIP() );
		printf("%s:%d ApDB ip:%s Is NOT Valid\n", __FUNCTION__, __LINE__, m_tApDBConInfo.GetDBIP() );
	}

	if ( CCfg::IsValidIp(m_tIpDbConInfo.GetDBIP()) )
	{
		OspPrintf(TRUE, FALSE, "%s:%d IpDB ip:%s IsValid\n", __FUNCTION__, __LINE__, m_tIpDbConInfo.GetDBIP() );
		printf("%s:%d IpDB ip:%s IsValid\n", __FUNCTION__, __LINE__, m_tIpDbConInfo.GetDBIP() );
		tInfo.push_back(m_tIpDbConInfo);
	}
	else
	{
		OspPrintf(TRUE, FALSE, "%s:%d IpDB ip:%s Is NOT Valid\n", __FUNCTION__, __LINE__, m_tIpDbConInfo.GetDBIP() );
		printf("%s:%d IpDB ip:%s Is NOT Valid\n", __FUNCTION__, __LINE__, m_tIpDbConInfo.GetDBIP() );
	}

    return tInfo;
}

EmDBEngineType CCfg::GetDBEngineType(void) const
{
    return m_tApDBConInfo.m_emDBEngineType;
}

const s8 *CCfg::GetDBEngineTypeDesc(void) const
{
    switch (GetDBEngineType())
    {
    case Em_DBEngineType_Sybase:
        return "Sybase";
		
    case Em_DBEngineType_Mysql:
        return "Mysql";

    default:
        return "Unknown";
    }
}

/*
 *	运营商匹配策略:
 1. 字符模糊匹配
 */
EOperatorsType TransIspNameToType(IN std::string& strIspName)
{
	EOperatorsType emIspType= emTypeQT;

	do 
	{
		std::string strKnownIspName;
		std::string strKnownIspName_UTF8;
		
		//比较
		strKnownIspName = ISP_CN_TELECOM;
		strKnownIspName_UTF8 = ISP_CN_TELECOM_UTF8;
		if ( strKnownIspName.rfind(strIspName.c_str()) != std::string::npos 
			|| strKnownIspName_UTF8.rfind(strIspName.c_str()) != std::string::npos
			)
		{
			emIspType = emTypeZGDX;
			break;
		}
		strKnownIspName = ISP_CN_MOBILE;
		strKnownIspName_UTF8 = ISP_CN_MOBILE_UTF8;
		if ( strKnownIspName.rfind(strIspName.c_str()) != std::string::npos 
			|| strKnownIspName_UTF8.rfind(strIspName.c_str()) != std::string::npos
			)
		{
			emIspType = emTypeZGYD;
			break;
		}
		strKnownIspName = ISP_CN_UNICOM;
		strKnownIspName_UTF8 = ISP_CN_UNICOM_UTF8;
		if (  strKnownIspName.rfind(strIspName.c_str()) != std::string::npos 
			|| strKnownIspName_UTF8.rfind(strIspName.c_str()) != std::string::npos
			)
		{
			emIspType = emTypeZGLT;
			break;
		}
		strKnownIspName = ISP_CN_WIRED;
		strKnownIspName_UTF8 = ISP_CN_WIRED_UTF8;
		if ( strKnownIspName.rfind(strIspName.c_str()) != std::string::npos 
			|| strKnownIspName_UTF8.rfind(strIspName.c_str()) != std::string::npos
			)
		{
			emIspType = emTypeYXT;
			break;
		}
		strKnownIspName = ISP_CN_CTT;
		strKnownIspName_UTF8 = ISP_CN_CTT_UTF8;
		if ( strKnownIspName.rfind(strIspName.c_str()) != std::string::npos 
			|| strKnownIspName_UTF8.rfind(strIspName.c_str()) != std::string::npos
			)
		{
			emIspType = emTypeTT;
			break;
		}
		strKnownIspName = ISP_OVERSEAS;
		strKnownIspName_UTF8 = ISP_OVERSEAS_UTF8;
		if ( strKnownIspName.rfind(strIspName.c_str()) != std::string::npos 
			|| strKnownIspName_UTF8.rfind(strIspName.c_str()) != std::string::npos
			)
		{
			emIspType = emTypeHW;
			break;
		}
		strKnownIspName = ISP_CN_LOCAL;
		strKnownIspName_UTF8 = ISP_CN_LOCAL_UTF8;
		if ( strKnownIspName.rfind(strIspName.c_str()) != std::string::npos 
			|| strKnownIspName_UTF8.rfind(strIspName.c_str()) != std::string::npos
			)
		{
			emIspType = emTypeBD;
			break;
		}
		
	} while (0);
	return emIspType;
}
