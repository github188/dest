
#include "kdvlog.h"
#include "loguserdef.h"
#include "dbopr.h"
#include "structulc.h"
#include "config.h"

CDBOpr::CDBOpr()
{
	
}

CDBOpr::~CDBOpr()
{
	
}

void CDBOpr::GetAllDBEngine( std::vector<CBaseDB*>& vcDBItem )
{
    vcDBItem.clear();
    vcDBItem.push_back(&m_tApDB);
    vcDBItem.push_back(&m_tIpDB);

    return;
}

void CDBOpr::ClearResults( )
{
    m_tApDB.ClearQryResults();
    m_tIpDB.ClearQryResults();

    return;
}




u16 CDBOpr::GetSrvDevInfoByGUID(IN const s8* szDevGUID, OUT CServerDevice&  tSrvDevInfo)
{
    return m_tApDB.GetSrvDevInfoByGUID(szDevGUID, tSrvDevInfo);
}

u16 CDBOpr::GetBriefSrvInfo(IN const s8* ServiceDomainMOID, IN u16 dwDeviceType, OUT std::set<CBriefSrvInfo>& setBriefSrvInfo)
{
    return m_tApDB.GetBriefSrvInfo(ServiceDomainMOID, dwDeviceType, setBriefSrvInfo);
}

u16 CDBOpr::GetPlatformDomain(IN const s8* szPlatformDomainMOID, OUT CPlatformDomain&  tPlatformDomain)
{
    return m_tApDB.GetPlatformDomain(szPlatformDomainMOID, tPlatformDomain);
}


u16 CDBOpr::Authentication(IN const CLoginInfo &tLoginInfo, OUT CDBMtInfo& tMtInfo, OUT CPlatformAttr& tPlatformAttr)
{
    return m_tApDB.Authentication(tLoginInfo, tMtInfo, tPlatformAttr);
}

u16 CDBOpr::GetIpISPName(IN u32 dwClientIp/*Ö÷»úÐò*/, OUT std::string& strISPName)
{
    return m_tIpDB.GetIpISPName(dwClientIp, strISPName);
}

u16 CDBOpr::GetPlatformBackUpConfig(const s8* pchPlatformDomainMOID, CBackUpConfig& cBackUpConfig)
{
	return m_tApDB.GetPlatformBackUpConfig(pchPlatformDomainMOID, cBackUpConfig);
}

u16 CDBOpr::GetPlatformBriefSrvInfo(const s8* pchPlatformDomainMOID, const u16 dwDeviceType, std::set<CBriefSrvInfo>& setBriefSrvInfo)
{
	return m_tApDB.GetPlatformBriefSrvInfo(pchPlatformDomainMOID, dwDeviceType, setBriefSrvInfo);
}

