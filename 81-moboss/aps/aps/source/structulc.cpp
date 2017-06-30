
#include "structulc.h"
#include "kdvlog.h"
#include "debugprint.h"
#include "config.h"

void CPlatformSrvAddrInfo::SetServerAddrInfo(const u32 dwType, CSrvAddrInfo& tAddrInfo)
{
	std::map<u32, CSrvAddrInfo*> m_mapTypeServerAddr;
	m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_NDS] = &m_tNdsAddrInfo;
	m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_NMS_C] = &m_tNmsAddrInfo;
	m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_SUS] = &m_tSusAddrInfo;
	m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_SNS] = &m_tWeiboAddrInfo;
	m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_XNS] = &m_tXmppAddrInfo;
	m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_NTP] = &m_tNtpAddrInfo;
	m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_VRS2000] = &m_tVrsAddrInfo;
	m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_DCS2000] = &m_tDcsAddrInfo;
	m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_RESTAPI] = &m_tRestAddrInfo;
	//m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_CMC] 
	//m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_PAS]
	//m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_APS]
	//m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_BGS]
	//m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_NMS_6000]
	//m_mapTypeServerAddr[DEVICE_GLOBAL_TYPE_VRS4000]
	if (dwType == DEVICE_GLOBAL_TYPE_APS)
	{
		m_tApsSrvIpList.Append(tAddrInfo);
        SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]APS Addr(%s:%u), DomainName(%s)\n", __FUNCTION__,
			BossUtils::GetIpString(tAddrInfo.GetIpAddr()).c_str(), tAddrInfo.GetPort(), tAddrInfo.GetNetDomainName());
	}
	else if (dwType == DEVICE_GLOBAL_TYPE_CMC)
	{
		SetMeetingAddrInfo(tAddrInfo);
		SetPlatApiAddrInfo(tAddrInfo);
	}
	else if (dwType == DEVICE_GLOBAL_TYPE_PAS)
	{
		//将pas服务器分成3个服务器告知终端
		tAddrInfo.SetPort(g_cCfgFile.GetStdH323Port());
		SetStdH323AddrInfo(tAddrInfo);   //标准H323地址信息

		tAddrInfo.SetPort(g_cCfgFile.GetNonH323Port());   
		SetNonH323AddrInfo(tAddrInfo);   //摩云非标H323地址信息

		tAddrInfo.SetPort(g_cCfgFile.GetSipPort());
		tAddrInfo.SetTlsPort(g_cCfgFile.GetSipTlsPort());
		SetSipAddrInfo(tAddrInfo);       //SIP协议的地址信息
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]Get Pas Addr(%s:KEY_SRVPORT_SIP=%u, KEY_SRVTLSPORT_SIP=%u), DomainName(%s) succeed\n", __FUNCTION__,
			BossUtils::GetIpString(tAddrInfo.GetIpAddr()).c_str(), tAddrInfo.GetPort(), tAddrInfo.GetTlsPort(), tAddrInfo.GetNetDomainName());
	}
	else
	{
		*(m_mapTypeServerAddr[dwType]) = tAddrInfo;
	}
}
