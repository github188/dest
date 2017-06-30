
#ifndef __APSUTILITY_H_
#define __APSUTILITY_H_

#include "kdvtype.h"
#include "structulc.h"
#include "structself.h"
#include "../jsoncpp/json.h"
#include "config.h"
#include <string>
#include <set>

class CBackUpConfig
{
public:
	enum EMBackUpMode{EM_BACKUP_CLOSE, EM_BACKUP_AUTO, EM_BACKUP_HAND_OPEN, EM_BACKUP_HAND_CLOSE};
public:
	CBackUpConfig();
	void SetBackUpMode(const EMBackUpMode emBackUpMode);
	EMBackUpMode GetBackUpMode() const;
	void SetBackUpPlatMOID(const std::string& strPlatformMOID);
	const std::string& GetBackUpPlatMOID() const;
private:
	EMBackUpMode m_emBackUpMode;
	std::string m_strBackUpPlatMOID;
};

BOOL32 IsBackUpOpen(const CBackUpConfig::EMBackUpMode emBackUpMode);
BOOL32 GetBackUpMode(const s8* pchDisasterBackUp, const s8* pchUseBackUpPlatform, CBackUpConfig::EMBackUpMode& emBackUpMode);
BOOL32 SetApsListToJson(
	const CApsSrvIpList& listMainPlatAps,
	const CApsSrvIpList& listThisPlatAps,
	const CApsSrvIpList& listBackUpPlatAps,
	const u32 dwTimeOut,
	Json::Value& jsonValue);
BOOL32 GetValidServerByIsp(const std::vector<CBriefSrvStateEx>& vcSrvInfoEx, std::vector<CSrvAddrInfo>& vecServerAddr,
	const s8* pchIspName, const BOOL32 bCheck = TRUE);
BOOL32 FiltLanServer(std::vector<CSrvAddrInfo>& vecServerAddr);
BOOL32 IsServerSateOkByIspIndex(const CBriefSrvStateEx& cServer, const u32 dwIspIndex);
BOOL32 SortApsServerList(const std::set<CBriefSrvInfo>& setAps, CApsSrvIpList& listAps,
	const std::string& strApsLocalIsp, const std::string& strClientIsp);
const s8* GetServerNameByType(const u32 dwServerType);
const s8* GetBackUpModeNameByType(const u32 dwType);

class CDBOpr;
u16 GetMainOrBackUpApsList(const s8* pchMtPlatDoaminMOID, const s8* pchThisPlatDomainMOID,
	std::set<CBriefSrvInfo>& cMainApsList, std::set<CBriefSrvInfo>& cBackUpApsList, CDBOpr* pcDBOpr);

#endif