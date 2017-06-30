
#include "inc.h"
#include "redisadapter.h"
#include "mauutility.h"
#include "mauvc.h"

#include <sstream>

using namespace std;

BOOL32 CRedisConfData::GetConfListByDomain(const s8* pchDomainInfo, const s8* pchMtE164,
	const BOOL32 bCheckPublic, ConfList& lstConf)
{
	AUTOLOCK;
	s8 achCheckPublic[2] = { 0 };
	snprintf(achCheckPublic, 2, "%d", bCheckPublic);

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchDomainInfo);
	vecArgs.push_back(pchMtE164);
	vecArgs.push_back(achCheckPublic);
	CRedisResult cResult;
	if (!EvalScript(SCRIPT_PATH(GETSIMPLECONFLIST), vecKeys, vecArgs, &cResult))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_REDIS, "[%s]EvalScript Failed\n", __FUNCTION__);
		return FALSE;
	}
	const s8* pchConfListJsonBuf = cResult.GetString();
	if (pchConfListJsonBuf == NULL || strlen(pchConfListJsonBuf) == 0)
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_REDIS, "[%s]redis no data, but right\n", __FUNCTION__);
		return TRUE;
	}
	if (!GetConfListFromJson(pchConfListJsonBuf, lstConf))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_REDIS, "[%s]GetConfListFromJson Failed\n", __FUNCTION__);
		return FALSE;
	}
	return TRUE;
}

BOOL32 CRedisConfData::GetConfListByDomain(const s8* pchDomainMOID, std::list<TRedisConfInfo>& lstConfInfo)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchDomainMOID);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETCONFLISTBYDOMAIN), vecKeys, vecArgs, &cResult))
	{
		const s8* pchJsonBuf = cResult.GetString();
		return GetRedisConfListFromJson(pchJsonBuf, lstConfInfo);
	}
	return FALSE;
}

BOOL32 CRedisConfData::GetConfNumByDomain(const s8* pchDomain, 
	const EMConfNumType emConfNumType, u32& dwConfNum)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchDomain);
	vecArgs.push_back(ConfNumTypeToString((u8)emConfNumType));
	CRedisResult cResult;
	if (!EvalScript(SCRIPT_PATH(GETCONFNUM), vecKeys, vecArgs, &cResult))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_REDIS, "[%s]EvalScript Failed\n", __FUNCTION__);
		return FALSE;
	}
	dwConfNum = cResult.GetInteger();
	return TRUE;
}

BOOL32 CRedisConfData::GetCmuConfNum(const s8* pchCmuIp, u32& dwConfNum)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchCmuIp);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETCMUCONFNUM), vecKeys, vecArgs, &cResult))
	{
		dwConfNum = cResult.GetInteger();
		return TRUE;
	}
	return FALSE;
}

BOOL32 CRedisConfData::SetMaxDomainConfNum(const s8* pchDomainMOID,
	const u32 dwSmallConfNum, const u32 dwLargeConfNum)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	s8 achMaxSmallConfNum[100 + 1] = { 0 };
	s8 achMaxLargeConfNum[100 + 1] = { 0 };
	snprintf(achMaxSmallConfNum, 100, "%u", dwSmallConfNum);
	snprintf(achMaxLargeConfNum, 100, "%u", dwLargeConfNum);

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchDomainMOID);
	vecArgs.push_back(achMaxSmallConfNum);
	vecArgs.push_back(achMaxLargeConfNum);
	return EvalScript(SCRIPT_PATH(SETMAXDOMAINCONFNUM), vecKeys, vecArgs);
}

BOOL32 CRedisConfData::GetConfInfoByE164(const s8* pchDomain, const s8* pchE164, TConfDetailInfo& tConfDetail)
{
	CPrintFunInOut cPrintAssist(__FUNCTION__, MID_MAU_REDIS, "DomainMoid: %s, ConfE164: %s",
		pchDomain == NULL ? "NULL" : pchDomain, pchE164 == NULL ? "NULL" : pchE164);
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchDomain);
	vecArgs.push_back(pchE164);
	CRedisResult cResult;
	if (!EvalScript(SCRIPT_PATH(GETDETAILCONFINFO), vecKeys, vecArgs, &cResult))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_REDIS, "[%s]EvalScript Failed\n", __FUNCTION__);
		return FALSE;
	}
	const s8* pchConfDetailJsonBuf = cResult.GetString();
	if (pchConfDetailJsonBuf == NULL || strlen(pchConfDetailJsonBuf) == 0)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_REDIS, "[%s]redis no data\n", __FUNCTION__);
		return TRUE;
	}
	if (!GetConfInfoFromJson(pchConfDetailJsonBuf, tConfDetail))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_REDIS, "[%s]GetConfInfoFromJson Failed\n", __FUNCTION__);
	}
	return TRUE;
}

const s8* GetStateStr(u8 byState)
{
	switch (byState)
	{
	case enumPrepareState:
		return "1";
	case enumRunningState:
		return "2";
	case enumConnectWaitingState:
		return "3";
	case enumInitedState:
		return "4";
	default:
		return "0";
	}
}

BOOL32 CRedisConfData::SetConfData(const TRedisConfInfo& tConfInfo)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	s8 achCmuPid[12] = { 0 };
	sprintf(achCmuPid, "%u", tConfInfo.GetCmuPid());
	s8 achDuration[12] = { 0 };
	sprintf(achDuration, "%u", tConfInfo.GetDuration());
	s8 achCreatorType[12] = { 0 };
	sprintf(achCreatorType, "%u", tConfInfo.GetCreatorType());
	s8 achMeetingId[10] = { 0 };
	sprintf(achMeetingId, "%u", tConfInfo.GetMeetingID());
	s8 achMaxJoinedMt[100] = { 0 };
	sprintf(achMaxJoinedMt, "%u", tConfInfo.GetMaxJoinedMt());
	s8 achCreatorAccountType[12] = { 0 };
	sprintf(achCreatorAccountType, "%u", tConfInfo.GetCreatorAccountType());

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(tConfInfo.GetConfE164());
	vecArgs.push_back(tConfInfo.GetDomainMOID());
	vecArgs.push_back(tConfInfo.GetCmuIp());
	vecArgs.push_back(achCmuPid);
	vecArgs.push_back(tConfInfo.GetStartTime());
	vecArgs.push_back(tConfInfo.GetBookStartTime());
	vecArgs.push_back(achDuration);
	vecArgs.push_back(tConfInfo.NeedLicense() ? "1" : "0");
	vecArgs.push_back(GetStateStr(tConfInfo.GetState()));
	vecArgs.push_back(tConfInfo.GetConfName());
	vecArgs.push_back(achMeetingId);
	vecArgs.push_back(achCreatorType);
	vecArgs.push_back(achMaxJoinedMt);
	vecArgs.push_back(tConfInfo.GetCreatorName());
	vecArgs.push_back(tConfInfo.GetCreatorAccount());
	vecArgs.push_back(achCreatorAccountType);
	vecArgs.push_back(tConfInfo.GetCreatorTelephone());
	vecArgs.push_back(tConfInfo.GetCreatorMobile());
	vecArgs.push_back(tConfInfo.GetRealStartTime());
	return EvalScript(SCRIPT_PATH(CREATECONF), vecKeys, vecArgs);
}

BOOL32 CRedisConfData::UpdateConfData(const TRedisConfInfo& tConfInfo)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	s8 achCmuPid[12] = { 0 };
	sprintf(achCmuPid, "%u", tConfInfo.GetCmuPid());

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(tConfInfo.GetConfE164());
	vecArgs.push_back(tConfInfo.GetCmuIp());
	vecArgs.push_back(achCmuPid);
	vecArgs.push_back(tConfInfo.GetStartTime());
	vecArgs.push_back(GetStateStr(tConfInfo.GetState()));
	vecArgs.push_back(tConfInfo.GetConfName());
	return EvalScript(SCRIPT_PATH(UPDATECONF), vecKeys, vecArgs);
}

BOOL32 CRedisConfData::DelConf(const s8* pchDomainMoid, const s8* pchE164, const s8* pchMcuIp)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchE164);
	vecArgs.push_back(pchDomainMoid);
	vecArgs.push_back(pchMcuIp);
	return EvalScript(SCRIPT_PATH(RELEASECONF), vecKeys, vecArgs);
}

BOOL32 CRedisConfData::GetConfExInfo(const s8* pchE164, TRedisConfInfo& tConfInfo)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchE164);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETCONFEXINFO), vecKeys, vecArgs, &cResult))
	{
		const s8* pchJson = cResult.GetString();
		if (pchJson != NULL && strlen(pchJson) != 0)
		{
			GetRedisConfFromJson(pchJson, tConfInfo);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL32 CRedisConfData::GetAllConfExInfo(std::list<TRedisConfInfo>& lstConfInfo)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;
	std::vector<const s8*> vecKeys, vecArgs;
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETALLCONFEXINFO), vecKeys, vecArgs, &cResult))
	{
		const s8* pchJsonBuf = cResult.GetString();
		return GetRedisConfListFromJson(pchJsonBuf, lstConfInfo);
	}
	return FALSE;
}

BOOL32 CRedisConfData::IsConfExist(const s8* abyE164, BOOL32& bExists)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(abyE164);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(ISCONFEXISTS), vecKeys, vecArgs, &cResult))
	{
		s32 nExists = cResult.GetInteger();
		bExists = (nExists == 0 ? FALSE : TRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL32 CRedisConfData::IsConfDataExist(const s8* pchConfE164, BOOL32& bExist)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchConfE164);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(ISCONFDATAEXIST), vecKeys, vecArgs, &cResult))
	{
		s32 dwExists = cResult.GetInteger();
		bExist = (dwExists == 0 ? FALSE : TRUE);
		return TRUE;
	}
	return FALSE;
}
BOOL32 CRedisConfData::ChangeConfState(const s8* pchMoid, const s8* pchE164, u8 byState)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	s8 achState[10] = { 0 };
	sprintf(achState, "%d", byState);
	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchMoid);
	vecArgs.push_back(pchE164);
	vecArgs.push_back(achState);
	return EvalScript(SCRIPT_PATH(CHANGECONFSTATE), vecKeys, vecArgs);
}

BOOL32 CRedisConfData::GetConfListByCmuIp(const s8* pchCmuIp, std::list<TRedisConfInfo>& lstConfInfo)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchCmuIp);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETCONFLISTBYCMUIP), vecKeys, vecArgs, &cResult))
	{
		const s8* pchJsonBuf = cResult.GetString();
		return GetRedisConfListFromJson(pchJsonBuf, lstConfInfo);
	}
	return FALSE;
}

BOOL32 CRedisConfData::ConstructE164(const s8* pchPrefix, const s8* pchBegin, const s8* pchEnd, std::string& strE164)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchPrefix);
	vecArgs.push_back(pchBegin);
	vecArgs.push_back(pchEnd);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(CONSTRUCTE164), vecKeys, vecArgs, &cResult))
	{
		strE164 = cResult.GetString();
		return (strE164.empty() ? FALSE : TRUE);
	}
	return FALSE;
}

BOOL32 CRedisConfData::UpdateMonitorAliveTime(const s8* pchE164, const s8* pchIp, const s8* pchPort)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	s8 achCurTime[100 + 1] = { 0 };
	time_t tCurTime = time(NULL);
	snprintf(achCurTime, 100, "%u", tCurTime);

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchE164);
	vecArgs.push_back(pchIp);
	vecArgs.push_back(pchPort);
	vecArgs.push_back(achCurTime);
	return EvalScript(SCRIPT_PATH(SETMONITORALIVETIME), vecKeys, vecArgs);
}

BOOL32 CRedisConfData::GetConfMonitorsInfo(const s8* pchE164, std::vector<TMonitorAliveInfo>& vecMonitors)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchE164);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETCONFMONITORSINFO), vecKeys, vecArgs, &cResult))
	{
		const s8* pchJsonBuf = cResult.GetString();
		return GetConfMonitorsInfoFromJson(pchJsonBuf, vecMonitors);
	}
	return FALSE;
}

BOOL32 CRedisConfData::GetExpiredMonitors(std::vector<TMonitorAliveInfo>& vecMonitors)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	s8 achCurTime[100 + 1] = { 0 };
	time_t tCurTime = time(NULL);
	snprintf(achCurTime, 100, "%u", tCurTime);

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(achCurTime);

	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETEXPIREDMONITOR), vecKeys, vecArgs, &cResult))
	{
		const s8* pchJsonBuf = cResult.GetString();
		return GetConfMonitorsInfoFromJson(pchJsonBuf, vecMonitors);
	}
	return FALSE;
}

BOOL32 CRedisConfData::AddBookConf(const CRedisBookConf& cConfInfo)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	s8 achMeetingId[10] = { 0 };
	snprintf(achMeetingId, sizeof(achMeetingId), "%u", cConfInfo.GetMeetingID());
	s8 achOpenMode[12] = { 0 };
	snprintf(achOpenMode, sizeof(achOpenMode), "%u", cConfInfo.GetOpenMode());
	s8 achMtNum[12] = { 0 };
	snprintf(achMtNum, sizeof(achMtNum), "%u", cConfInfo.GetMtNum());
	s8 achBitRate[12] = { 0 };
	snprintf(achBitRate, sizeof(achBitRate), "%u", cConfInfo.GetBitRate());
	s8 achResolution[12] = { 0 };
	snprintf(achResolution, sizeof(achResolution), "%u", cConfInfo.GetResolution());
	s8 achSatMode[12] = { 0 };
	snprintf(achSatMode, sizeof(achSatMode), "%u", cConfInfo.GetSatMode());
	s8 achDualMode[12] = { 0 };
	snprintf(achDualMode, sizeof(achDualMode), "%u", cConfInfo.GetDualMode());
	s8 achNoDisturb[12] = { 0 };
	snprintf(achNoDisturb, sizeof(achNoDisturb), "%u", cConfInfo.GetIsConfNodisturb() ? 1 : 0);
	s8 achMaxJoinedMtNum[12] = { 0 };
	snprintf(achMaxJoinedMtNum, sizeof(achMaxJoinedMtNum), "%u", cConfInfo.GetMaxJoinedMt());
	s8 achCreatorAccountType[12] = { 0 };
	snprintf(achCreatorAccountType, sizeof(achCreatorAccountType), "%u", cConfInfo.GetCreatorAccountType());
	s8 achMixMode[12] = { 0 };
	snprintf(achMixMode, sizeof(achMixMode), "%u", cConfInfo.GetMixMode());
	s8 achMediaType[12] = { 0 };
	snprintf(achMediaType, sizeof(achMediaType), "%u", cConfInfo.GetMediaType());

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(cConfInfo.GetConfE164());
	vecArgs.push_back(cConfInfo.GetConfName());
	vecArgs.push_back(achMeetingId);
	vecArgs.push_back(cConfInfo.GetBookStartTime());
	vecArgs.push_back(cConfInfo.GetBookEndTime().c_str());
	vecArgs.push_back(cConfInfo.GetDomainMOID());
	vecArgs.push_back(cConfInfo.GetConfDetailJson().c_str());
	vecArgs.push_back(cConfInfo.GetPassword().c_str());
	vecArgs.push_back(cConfInfo.GetIsPublicConf() ? "1" : "0");
	vecArgs.push_back(achOpenMode);
	vecArgs.push_back(achMtNum);
	vecArgs.push_back(achBitRate);
	vecArgs.push_back(achResolution);
	vecArgs.push_back(achSatMode);
	vecArgs.push_back(achDualMode);
	vecArgs.push_back(achNoDisturb);
	vecArgs.push_back(achMaxJoinedMtNum);
	vecArgs.push_back(cConfInfo.GetCreatorName());
	vecArgs.push_back(cConfInfo.GetCreatorAccount());
	vecArgs.push_back(achCreatorAccountType);
	vecArgs.push_back(cConfInfo.GetCreatorTelephone());
	vecArgs.push_back(cConfInfo.GetCreatorMobile());
	vecArgs.push_back(achMixMode);
	vecArgs.push_back(achMediaType);
	return EvalScript(SCRIPT_PATH(ADDBOOKCONF), vecKeys, vecArgs);
}

BOOL32 CRedisConfData::ModBookConf(const CRedisBookConf& cConfInfo)
{
	return AddBookConf(cConfInfo);
}

BOOL32 CRedisConfData::DelBookConf(const s8* pchConfE164)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchConfE164);
	return EvalScript(SCRIPT_PATH(DELBOOKCONF), vecKeys, vecArgs);
}

BOOL32 CRedisConfData::GetAllBookConf(vector<CRedisBookConf>& bookConfVec)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETALLBOOKCONF), vecKeys, vecArgs, &cResult))
	{
		const s8* pchJsonBuf = cResult.GetString();
		return GetRedisBookConfListFromJson(pchJsonBuf, bookConfVec);
	}
	return FALSE;
}

BOOL32 CRedisConfData::GetBookConf(const s8* pchConfE164, CRedisBookConf& bookConf)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchConfE164);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETBOOKCONF), vecKeys, vecArgs, &cResult))
	{
		const s8* pchJsonBuf = cResult.GetString();
		if (pchJsonBuf != NULL && strlen(pchJsonBuf) != 0)
		{
			GetRedisBookConfFromJson(pchJsonBuf, bookConf);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL32 CRedisConfData::GetBookConfE164ByMeetingID(const u32 dwMeetingID, string& strBookConfE164)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	s8 achMeetingID[10] = { 0 };
	sprintf(achMeetingID, "%u", dwMeetingID);

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(achMeetingID);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETBOOKCONFE164BYMEETINGID), vecKeys, vecArgs, &cResult))
	{
		const s8* pchJsonBuf = cResult.GetString();
		if (pchJsonBuf != NULL && strlen(pchJsonBuf) != 0)
		{
			strBookConfE164 = pchJsonBuf;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL32 CRedisConfData::GetBookConfList(const s8* pchDomainMOID, std::vector<CRedisBookConf>& bookConfVec)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(pchDomainMOID);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETBOOKCONFLIST), vecKeys, vecArgs, &cResult))
	{
		const s8* pchJsonBuf = cResult.GetString();
		return GetRedisBookConfListFromJson(pchJsonBuf, bookConfVec);
	}
	return FALSE;
}

//--redisBookConf

CRedisBookConf::CRedisBookConf():
	m_bPublicConf(FALSE),
	m_bNoDisturb(FALSE),
	m_dwMtNum(0),
	m_wBitRate(0),
	m_byResolution(0),
	m_bySatMode(0),
	m_byDualMode(0),
	m_byOpenMode(0),
	m_byMixMode(0){}

void CRedisBookConf::SetConfDetailJson(const std::string& jsonData)
{
	m_confDetailJson = jsonData;
}
const std::string& CRedisBookConf::GetConfDetailJson() const
{
	return m_confDetailJson;
}
void CRedisBookConf::SetBookEndTime(const std::string& strTime)
{
	m_endTime = strTime;
}
const std::string& CRedisBookConf::GetBookEndTime() const
{
	return m_endTime;
}
void CRedisBookConf::SetMtNum(const u32 dwNum)
{
	m_dwMtNum = dwNum;
}
u32 CRedisBookConf::GetMtNum() const
{
	return m_dwMtNum;
}
void CRedisBookConf::SetOpenMode(const u8 byOpenMode)
{
	m_byOpenMode = byOpenMode;
}
u8 CRedisBookConf::GetOpenMode() const
{
	return m_byOpenMode;
}
void CRedisBookConf::SetSatMode(const u8 bySatMode)
{
	m_bySatMode = bySatMode;
}
u8 CRedisBookConf::GetSatMode() const
{
	return m_bySatMode;
}
void CRedisBookConf::SetIsPublicConf(const BOOL32 bPublicConf)
{
	m_bPublicConf = bPublicConf;
}
BOOL32 CRedisBookConf::GetIsPublicConf() const
{
	return m_bPublicConf;
}
void CRedisBookConf::SetPassword(const std::string& password)
{
	m_password = password;
}
const string& CRedisBookConf::GetPassword() const
{
	return m_password;
}
void CRedisBookConf::SetBitRate(const u16 wBitRate)
{
	m_wBitRate = wBitRate;
}
u16 CRedisBookConf::GetBitRate() const
{
	return m_wBitRate;
}
void CRedisBookConf::SetMediaType(const u8 byMediaType)
{
	m_byMediaType = byMediaType;
}
u8 CRedisBookConf::GetMediaType() const
{
	return m_byMediaType;
}
void CRedisBookConf::SetResolution(const u8 byResolution)
{
	m_byResolution = byResolution;
}
u8 CRedisBookConf::GetResolution() const
{
	return m_byResolution;
}
void CRedisBookConf::SetDualMode(const u8 byDualMode)
{
	m_byDualMode = byDualMode;
}
u8 CRedisBookConf::GetDualMode() const
{
	return m_byDualMode;
}

void CRedisBookConf::SetMixMode(const u8 byMixMode)
{
	m_byMixMode = byMixMode;
}
u8 CRedisBookConf::GetMixMode() const
{
	return m_byMixMode;
}

void CRedisBookConf::SetIsConfNodisturb(const BOOL32 bNodisturb)
{
	m_bNoDisturb = bNodisturb;
}
BOOL32 CRedisBookConf::GetIsConfNodisturb() const
{
	return m_bNoDisturb;
}

u16 CRedisBookConf::GetDuration() const
{
	time_t tStartTime = 0;
	time_t tEndTime = 0;
	FormatStr2KdvTime(m_achBookStartTime).GetTime(tStartTime);
	FormatStr2KdvTime(m_endTime.c_str()).GetTime(tEndTime);
	return (u16)(tEndTime - tStartTime) / 60;
}
void CRedisBookConf::Print(const BOOL32 bPrintDetailJson/*= FALSE*/) const
{
	::OspPrintf(TRUE, FALSE, "|Name:           %s\n", m_achConfName);
	::OspPrintf(TRUE, FALSE, "|E164:           %s\n", m_achConfE164);
	::OspPrintf(TRUE, FALSE, "|DomainMoid:     %s\n", m_achDomainMOID);
	::OspPrintf(TRUE, FALSE, "|StartTime:      %s\n", m_achBookStartTime);
	::OspPrintf(TRUE, FALSE, "|BookEndTime:    %s\n", m_endTime.c_str());
	::OspPrintf(TRUE, FALSE, "|creatorName:          %s\n", m_achCreatorName);
	::OspPrintf(TRUE, FALSE, "|creatorAccount:       %s\n", m_achCreatorAccount);
	::OspPrintf(TRUE, FALSE, "|creatorAccountType:   %d\n", m_byCreatorAccountType);
	::OspPrintf(TRUE, FALSE, "|creatorTelephone:     %s\n", m_achCreatorTelephone);
	::OspPrintf(TRUE, FALSE, "|creatorMobile:        %s\n", m_achCreatorMobile);
	::OspPrintf(TRUE, FALSE, "|password:       %s\n", m_password.c_str());
	::OspPrintf(TRUE, FALSE, "|MeetingId:      %d\n", m_dwMeetingId);
	::OspPrintf(TRUE, FALSE, "|mtNum:          %d\n", m_dwMtNum);
	::OspPrintf(TRUE, FALSE, "|openmode:       %d\n", m_byOpenMode);
	::OspPrintf(TRUE, FALSE, "|publicConf:     %u\n", m_bPublicConf);
	::OspPrintf(TRUE, FALSE, "|bitRate:        %u\n", m_wBitRate);
	::OspPrintf(TRUE, FALSE, "|Resolution:     %u\n", m_byResolution);
	::OspPrintf(TRUE, FALSE, "|satMode:        %d\n", m_bySatMode);
	::OspPrintf(TRUE, FALSE, "|dualMode:       %d\n", m_byDualMode);
	::OspPrintf(TRUE, FALSE, "|mixMode:       %d\n",  m_byMixMode);
	::OspPrintf(TRUE, FALSE, "|bNodisturb:     %d\n", m_bNoDisturb);
	::OspPrintf(TRUE, FALSE, "|maxJoinedMtNum: %d\n", m_dwMaxJoinedMt);
	::OspPrintf(TRUE, FALSE, "|mediaType:      %d\n", m_byMediaType);
	if (bPrintDetailJson)
	{
		::OspPrintf(TRUE, FALSE, "|Detail:\n %s\n", m_confDetailJson.c_str());
	}
}
//--redisbookconf--

BOOL32 CRedisConfData::GetCreateConfJson(const std::string& confE164, const BOOL32 bHasXmpu, string& createConfJson)
{
	TRedisConfInfo tRedisConf;
	if (!GetConfExInfo(confE164.c_str(), tRedisConf)
		|| !IsRedisHasConfExData(tRedisConf))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_REDIS, "[%s]GetConfExInfo<%s> fail\n",
			__FUNCTION__, confE164.c_str());
		return FALSE;
	}

	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	time_t curTime = time(NULL);
	s8 achCurTime[100 + 1] = { 0 };
	TKdvTime(&curTime).GetString(achCurTime, 100);
	time_t tRealStartTime = 0;
	FormatStr2KdvTime(tRedisConf.GetRealStartTime()).GetTime(tRealStartTime);
	const u16 wGoingMinute = (u16)((curTime - tRealStartTime) / 60);
	const u16 wLeftMinute = tRedisConf.GetDuration() == 0 ? 0 : (tRedisConf.GetDuration() - wGoingMinute);
	s8 achConfDuration[100 + 1] = { 0 };
	snprintf(achConfDuration, 100, "%u", wLeftMinute);

	s8 achHasXmpu[20 + 1] = { 0 };
	snprintf(achHasXmpu, 20, "%u", bHasXmpu);

	std::vector<const s8*> vecKeys, vecArgs;
	vecArgs.push_back(confE164.c_str());
	vecArgs.push_back(achCurTime);
	vecArgs.push_back(achConfDuration);
	vecArgs.push_back(achHasXmpu);
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(GETCREATECONFJSON), vecKeys, vecArgs, &cResult))
	{
		const s8* pchCreateConfJson = cResult.GetString();
		if (pchCreateConfJson != NULL)
		{
			createConfJson = pchCreateConfJson;
			return TRUE;
		}
		else
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_REDIS, "[%s]res json is invalid\n", __FUNCTION__);
		}
	}
	return FALSE;
}

BOOL32 CRedisConfData::CheckRedisData(string& checkResStr)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	CRedisResult cResult;
	if (EvalScript(SCRIPT_PATH(CHECKREDISDATA), vecKeys, vecArgs, &cResult))
	{
		const s8* pchCheckRes = cResult.GetString();
		if (pchCheckRes != NULL && strlen(pchCheckRes) != 0)
		{
			checkResStr = pchCheckRes;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL32 CRedisConfData::GetChargeJson(const s8* pchConfE164, std::string& chargeJson)
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	CRedisResult cResult;
	vecArgs.push_back(pchConfE164);
	if (EvalScript(SCRIPT_PATH(GETCHARGE), vecKeys, vecArgs, &cResult))
	{
		const s8* pchChargeJson = cResult.GetString();
		if (pchChargeJson != NULL && strlen(pchChargeJson) != 0)
		{
			chargeJson = pchChargeJson;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL32 CRedisConfData::StartInitRedisConf()
{
	AUTOLOCK;
	if (!m_pcRedisImplement) return FALSE;

	std::vector<const s8*> vecKeys, vecArgs;
	return EvalScript(SCRIPT_PATH(STARTINITREDISCONF), vecKeys, vecArgs);
}
