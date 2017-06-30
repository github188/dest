
#include "apsutility.h"
#include "dbopr.h"
#include "errorcode.h"
#include "kdvlog.h"
#include "debugprint.h"
#include "bossutils.h"

using namespace BossUtils;
using namespace std;
using namespace Json;

CBackUpConfig::CBackUpConfig()
{
	m_emBackUpMode = EM_BACKUP_CLOSE;
}

void CBackUpConfig::SetBackUpMode(const CBackUpConfig::EMBackUpMode emBackUpMode)
{
	m_emBackUpMode = emBackUpMode;
}
CBackUpConfig::EMBackUpMode CBackUpConfig::GetBackUpMode() const
{
	return m_emBackUpMode;
}

void CBackUpConfig::SetBackUpPlatMOID(const std::string& strPlatformMOID)
{
	m_strBackUpPlatMOID = strPlatformMOID;
}
const std::string& CBackUpConfig::GetBackUpPlatMOID() const
{
	return m_strBackUpPlatMOID;
}

BOOL32 IsBackUpOpen(const CBackUpConfig::EMBackUpMode emBackUpMode)
{
	return (emBackUpMode == CBackUpConfig::EM_BACKUP_AUTO || emBackUpMode == CBackUpConfig::EM_BACKUP_HAND_OPEN);
}

BOOL32 GetBackUpMode(const s8* pchDisasterBackUp, const s8* pchUseBackUpPlatform, CBackUpConfig::EMBackUpMode& emBackUpMode)
{
	const CBackUpConfig::EMBackUpMode backUpModeArray[3][2] =
	{ {CBackUpConfig::EM_BACKUP_CLOSE, CBackUpConfig::EM_BACKUP_CLOSE},
	  {CBackUpConfig::EM_BACKUP_HAND_CLOSE, CBackUpConfig::EM_BACKUP_HAND_OPEN},
	  {CBackUpConfig::EM_BACKUP_AUTO, CBackUpConfig::EM_BACKUP_AUTO} };

	const u32 dwDisasterBackUp = strtoul(pchDisasterBackUp, 0, 10);
	const u32 dwUseBackUpPlat = strtoul(pchUseBackUpPlatform, 0, 10);
	if (0 <= dwDisasterBackUp && dwDisasterBackUp <= 2
		&& 0 <= dwUseBackUpPlat && dwUseBackUpPlat <= 1)
	{
		emBackUpMode = backUpModeArray[dwDisasterBackUp][dwUseBackUpPlat];
		return TRUE;
	}
	else
	{
        CCtrlLog(LOG_LVL_ERROR, "[%s]DisasterBackUp(%s), useBackUpPlatform(%s) beyond valid value \n",
			__FUNCTION__, pchDisasterBackUp, pchUseBackUpPlatform);
		return FALSE;
	}
}

BOOL32 SetSrvAddrInfoToJson(const CSrvAddrInfo& cSrvAddrInfo, const u32 dwPriority, const u32 dwTimeOut, Value& jsonValue)
{
	CJsonConvert jsonTool;
	std::string strIP = BossUtils::GetIpString(cSrvAddrInfo.GetIpAddr());    
	jsonTool.SetValue("m_szIpAddr", strIP.c_str());
	jsonTool.SetValue("m_szNetDomainName", cSrvAddrInfo.GetNetDomainName());
	jsonTool.SetValue("m_wPort", cSrvAddrInfo.GetPort());
	jsonTool.SetValue("m_wTlsPort", cSrvAddrInfo.GetTlsPort());
	jsonTool.SetValue("priority", dwPriority);
	jsonTool.SetValue("timeout", dwTimeOut);//单位毫秒
	jsonValue = jsonTool.GetJson();
	return TRUE;
}

BOOL32 AppendApsListToJsonArray(const CApsSrvIpList& listAps, const u32 dwTimeOut, Value& arrayJson)
{
	for (u32 dwApsIndex = 0; dwApsIndex != listAps.Size(); ++dwApsIndex)
	{
		Value srvAddrInfoJsonValue;
		SetSrvAddrInfoToJson(listAps.GetValue(dwApsIndex), arrayJson.size(), dwTimeOut, srvAddrInfoJsonValue);
		arrayJson.append(srvAddrInfoJsonValue);
	}
	return TRUE;
}

BOOL32 SetApsListToJson(
	const CApsSrvIpList& listMainPlatAps,
	const CApsSrvIpList& listThisPlatAps,
	const CApsSrvIpList& listBackUpPlatAps,
	const u32 dwTimeOut,
	Value& jsonValue)
{
	Value arrayJson(Json::arrayValue);
	AppendApsListToJsonArray(listMainPlatAps, dwTimeOut, arrayJson);
	AppendApsListToJsonArray(listThisPlatAps, dwTimeOut, arrayJson);
	AppendApsListToJsonArray(listBackUpPlatAps, dwTimeOut, arrayJson);
	jsonValue["m_tSrvInfo"]["m_tApsBackupList"]["m_atArray"] = arrayJson;
	jsonValue["m_tSrvInfo"]["m_tApsBackupList"]["m_dwCurSize"] = listMainPlatAps.Size() 
		+ listThisPlatAps.Size() + listBackUpPlatAps.Size();
	return TRUE;
}

//如果是本平台域的终端，若本平台灾备手动开启，则拒掉请求
//如果是其他平台域的终端，查看是否自动灾备或者手动且开启灾备，且灾备列表是否是本平台域,则获取终端所在平台的aps列表
u16 GetMainOrBackUpApsList(const s8* pchMtPlatDomainMOID, const s8* pchThisPlatDomainMOID,
	set<CBriefSrvInfo>& setMainPlatAps, set<CBriefSrvInfo>& setBackUpPlatAps, CDBOpr* pcDBOpr)
{
	CBackUpConfig cBackUpConfig;
	u16 wErrCode = pcDBOpr->GetPlatformBackUpConfig(pchMtPlatDomainMOID, cBackUpConfig);
	if (wErrCode != SUCCESS_DBENGINE)
	{
		return wErrCode;
	}
	CCtrlLog(LOG_LVL_ERROR, "[%s]MtPlat(%s),ThisPlat(%s), BackUpMode:%s, backUpPlat:%s\n", __FUNCTION__,
		pchMtPlatDomainMOID, pchThisPlatDomainMOID,
		GetBackUpModeNameByType((u8)cBackUpConfig.GetBackUpMode()), cBackUpConfig.GetBackUpPlatMOID().c_str());

	const BOOL32 bMtBelongThisPlat = (strcmp(pchMtPlatDomainMOID, pchThisPlatDomainMOID) == 0);
	if (!bMtBelongThisPlat
		&& (!IsBackUpOpen(cBackUpConfig.GetBackUpMode())
			|| cBackUpConfig.GetBackUpPlatMOID() != pchThisPlatDomainMOID))

	{
		CCtrlLog(LOG_LVL_ERROR, "[%s]MainPlat,but backup close, reject\n", __FUNCTION__);
		return APS_ERR_REJECT_LOGIN;
	}
	else if (bMtBelongThisPlat && cBackUpConfig.GetBackUpMode() == CBackUpConfig::EM_BACKUP_CLOSE)
	{
		CCtrlLog(LOG_LVL_ERROR, "[%s]thisPlat,but backup close, do nothing\n", __FUNCTION__);
		return SUCCESS_DBENGINE;
	}
	else if (bMtBelongThisPlat && cBackUpConfig.GetBackUpMode() == CBackUpConfig::EM_BACKUP_HAND_OPEN)
	{
		CCtrlLog(LOG_LVL_ERROR, "[%s]this plat,but backup hand open, reject\n", __FUNCTION__);
		return APS_ERR_REJECT_LOGIN;
	}

	set<CBriefSrvInfo> setOtherPlatAps;
	wErrCode = pcDBOpr->GetPlatformBriefSrvInfo(bMtBelongThisPlat ? cBackUpConfig.GetBackUpPlatMOID().c_str() : pchMtPlatDomainMOID,
		DEVICE_GLOBAL_TYPE_APS, setOtherPlatAps);
	if (wErrCode == SUCCESS_DBENGINE)
	{
		setOtherPlatAps.swap(bMtBelongThisPlat ? setBackUpPlatAps : setMainPlatAps);
	}
	return wErrCode;
}

//ISPName = NULL,则返回所有有效地址
BOOL32 GetValidServerByIsp(const std::vector<CBriefSrvStateEx>& vcSrvInfoEx, std::vector<CSrvAddrInfo>& vecServerAddr,
	const s8* pchIspName, const BOOL32 bCheck /*= TRUE*/)
{
	std::vector<CBriefSrvStateEx>::const_iterator iterServer = vcSrvInfoEx.begin();
	for (; iterServer != vcSrvInfoEx.end(); ++iterServer)
	{
		for (u32 dwIspIndex = 0; dwIspIndex != OPERATION_MAXNUM_ISP; ++dwIspIndex)
		{
			CNetAddrInfo cServerNetAddrInfo;
			iterServer->GetDetailBody().GetSrvAddr(dwIspIndex, cServerNetAddrInfo);
			const BOOL32 bIspNameEqual = (pchIspName == NULL || (strcmp(pchIspName, cServerNetAddrInfo.GetOperatorName()) == 0));
			const BOOL32 bStateOk = (!bCheck || IsServerSateOkByIspIndex(*iterServer, dwIspIndex));
			if (cServerNetAddrInfo.IsValid()
				&& bIspNameEqual 
				&& bStateOk)
			{
				CSrvAddrInfo cSameIspServer;
				cSameIspServer.SetIpAddr(cServerNetAddrInfo.GetIP());
				cSameIspServer.SetNetDomainName(iterServer->GetDetailBody().GetNetDomainName());
				vecServerAddr.push_back(cSameIspServer);
				SrvListMgrLog(LOG_LVL_WARNING, "[%s] find srv<%s> for same Isp(%s) success\n",
					__FUNCTION__, BossUtils::GetIpString(cServerNetAddrInfo.GetIP()).c_str(),
					pchIspName == NULL ? "NULL<needn't match isp>" : pchIspName);
			}
			else
			{
				SrvListMgrLog(LOG_LVL_REPEAT, "[%s] server ip<%u> is 0, server isp<%s> != %s, or state not ok\n",
					__FUNCTION__, cServerNetAddrInfo.GetIP(), cServerNetAddrInfo.GetOperatorName(), pchIspName);
			}
		}
	}
	return TRUE;
}

BOOL32 FiltLanServer(vector<CSrvAddrInfo>& vecServerAddr)
{
	vector<CSrvAddrInfo>::iterator iterServer = vecServerAddr.begin();
	for (; iterServer != vecServerAddr.end(); )
	{
		iterServer = (BossUtils::IsLANIp(iterServer->GetIpAddr()) ? vecServerAddr.erase(iterServer) : ++iterServer);
	}
	return TRUE;
}

//5.1sp2外网地址不检测状态
BOOL32 IsServerSateOkByIspIndex(const CBriefSrvStateEx& cServer, const u32 dwIspIndex)
{
	CNetAddrInfo cNetAddrInfo;
	/*IP为零不列入外网*/
	if (cServer.GetDetailBody().GetSrvAddr(dwIspIndex, cNetAddrInfo)
		&& !BossUtils::IsLANIp(cNetAddrInfo.GetIP()) && 0 != cNetAddrInfo.GetIP())
	{
		SrvListMgrLog(LOG_LVL_REPEAT, "[%s]Server ip<%s> is wlan, state is ok(needn't check)\n",
			__FUNCTION__, BossUtils::GetIpString(cNetAddrInfo.GetIP()).c_str());
		return TRUE;
	}
	else
	{
		return cServer.GetIpState(dwIspIndex);
	}
}

BOOL32 AppendServerSetToApsList(const set<CSrvAddrInfo>& setServer, CApsSrvIpList& listAps)
{
	set<CSrvAddrInfo>::const_iterator iterServer = setServer.begin();
	for (; iterServer != setServer.end(); ++iterServer)
	{
		listAps.Append(*iterServer);
	}
	return TRUE;
}

BOOL32 SortApsServerList(const set<CBriefSrvInfo>& setAps, CApsSrvIpList& listAps,
	const string& strApsLocalIsp, const string& strClientIsp)
{
	set<CSrvAddrInfo> setSameLocalApsIspServer;
	set<CSrvAddrInfo> setSameClientIspServer;
	set<CSrvAddrInfo> setOtherIspServer;
	set<CBriefSrvInfo>::iterator iterAps = setAps.begin();
	for (; iterAps != setAps.end(); ++iterAps)
	{
		for (u32 dwIspIndex = 0; dwIspIndex != OPERATION_MAXNUM_ISP; ++dwIspIndex)
		{
			CNetAddrInfo cServerAddr;
			iterAps->GetSrvAddr(dwIspIndex, cServerAddr);
			if(strApsLocalIsp == cServerAddr.GetOperatorName())
			{
				setSameLocalApsIspServer.insert(CSrvAddrInfo(cServerAddr.GetIP(), APS_HTTP_LISTEN_PORT, DEFAULT_TLS_PORT, iterAps->GetNetDomainName()));
			}
			else if (strClientIsp == cServerAddr.GetOperatorName())
			{
				setSameClientIspServer.insert(CSrvAddrInfo(cServerAddr.GetIP(), APS_HTTP_LISTEN_PORT, DEFAULT_TLS_PORT, iterAps->GetNetDomainName()));
			}
			else if (cServerAddr.IsValid())
			{
				setOtherIspServer.insert(CSrvAddrInfo(cServerAddr.GetIP(), APS_HTTP_LISTEN_PORT, DEFAULT_TLS_PORT, iterAps->GetNetDomainName()));
			}
		}
	}
	AppendServerSetToApsList(setSameLocalApsIspServer, listAps);
	AppendServerSetToApsList(setSameClientIspServer, listAps);
	AppendServerSetToApsList(setOtherIspServer, listAps);
	return TRUE;
}

const s8* GetServerNameByType(const u32 dwServerType)
{
	switch (dwServerType)
	{
	case DEVICE_GLOBAL_TYPE_NDS:
		return "NDS";
	case DEVICE_GLOBAL_TYPE_NMS_C:
		return "NMS_C";
	case DEVICE_GLOBAL_TYPE_NMS_6000:
		return "NMS_6000";
	case DEVICE_GLOBAL_TYPE_PAS:
		return "PAS";
	case DEVICE_GLOBAL_TYPE_SUS:
		return "SUS";
	case DEVICE_GLOBAL_TYPE_SNS:
		return "SNS";
	case DEVICE_GLOBAL_TYPE_XNS:
		return "XNS";
	case DEVICE_GLOBAL_TYPE_CMC:
		return "CMC";
	case DEVICE_GLOBAL_TYPE_NTP:
		return "NTP";
	case DEVICE_GLOBAL_TYPE_VRS2000:
		return "VRS2000";
	case DEVICE_GLOBAL_TYPE_VRS4000:
		return "VRS4000";
	case DEVICE_GLOBAL_TYPE_DCS2000:
		return "DCS2000";
	case DEVICE_GLOBAL_TYPE_RESTAPI:
		return "RESTAPI";
	case DEVICE_GLOBAL_TYPE_APS:
		return "APS";
	default:
		break;
	}
	return "";
}

const s8* GetBackUpModeNameByType(const u32 dwType)
{
	switch (dwType)
	{
	case CBackUpConfig::EM_BACKUP_CLOSE:
		return "close";
	case CBackUpConfig::EM_BACKUP_AUTO:
		return "auto";
	case CBackUpConfig::EM_BACKUP_HAND_OPEN:
		return "hand_open";
	case CBackUpConfig::EM_BACKUP_HAND_CLOSE:
		return "hand_close";
	default:
		break;
	}
	return "";
}