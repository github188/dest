#pragma once

#include <vector>
#include <string>
#include <list>
#include "vccommon.h"
#include "rediswrap.h"

#define LUA_SCRIPT_FILEPATH	"./script/"

#define CREATECONF			"createconf.lua"
#define RELEASECONF			"releaseconf.lua"
#define GETCONFNUM			"getconfnum.lua"
#define GETDETAILCONFINFO	"getdetailconfinfo.lua"
#define GETCONFLISTBYDOMAIN "getconflistbydomain.lua"
#define GETCONFEXINFO		"getconfexinfo.lua"
#define ISCONFEXISTS		"isconfexists.lua"
#define CHANGECONFSTATE		"changeconfstate.lua"
#define GETCONFLISTBYCMUIP	"getconflistbycmu.lua"
#define GETALLCONFEXINFO	"getallconfexinfo.lua"
#define CONSTRUCTE164		"constructe164.lua"
#define SETMAXDOMAINCONFNUM "setmaxdomainconfnum.lua"
#define GETCMUCONFNUM       "getcmuconfnum.lua"
#define SETMONITORALIVETIME "setmonitoralivetime.lua"
#define GETCONFMONITORSINFO "getconfmonitorsinfo.lua"
#define ADDBOOKCONF         "addbookconf.lua"
#define DELBOOKCONF         "delbookconf.lua"
#define GETBOOKCONFLIST     "getbookconflist.lua"
#define GETBOOKCONF         "getbookconf.lua"
#define GETBOOKCONFE164BYMEETINGID "getbookconfe164bymeetingid.lua"
#define GETALLBOOKCONF      "getallbookconf.lua"
#define GETCREATECONFJSON   "getcreateconfjson.lua"
#define CHECKREDISDATA      "checkredisdata.lua"
#define UPDATECONF          "updateconf.lua"
#define GETSIMPLECONFLIST   "getsimpleconflist.lua"
#define GETCHARGE           "getcharge.lua"
#define GETEXPIREDMONITOR	"getexpiredmonitor.lua"
#define ISCONFDATAEXIST     "isconfdataexist.lua"
#define STARTINITREDISCONF  "startinitredisconf.lua"

#define LUA_SCRIPT(filename) LUA_SCRIPT_FILEPATH filename,
#define SCRIPT_PATH(filename) LUA_SCRIPT_FILEPATH filename
#define BEGIN_LUA_SCRIPT static const s8* const s_pchScriptFile[] = {
#define END_LUA_SCRIPT };
#define LUA_SCRIPT_NUM (_countof(s_pchScriptFile))  

BEGIN_LUA_SCRIPT
LUA_SCRIPT(CREATECONF)
LUA_SCRIPT(RELEASECONF)
LUA_SCRIPT(GETCONFNUM)
LUA_SCRIPT(GETDETAILCONFINFO)
LUA_SCRIPT(GETCONFLISTBYDOMAIN)
LUA_SCRIPT(GETCONFEXINFO)
LUA_SCRIPT(ISCONFEXISTS)
LUA_SCRIPT(CHANGECONFSTATE)
LUA_SCRIPT(GETCONFLISTBYCMUIP)
LUA_SCRIPT(GETALLCONFEXINFO)
LUA_SCRIPT(CONSTRUCTE164)
LUA_SCRIPT(SETMAXDOMAINCONFNUM)
LUA_SCRIPT(GETCMUCONFNUM)
LUA_SCRIPT(SETMONITORALIVETIME)
LUA_SCRIPT(GETCONFMONITORSINFO)
LUA_SCRIPT(ADDBOOKCONF)
LUA_SCRIPT(DELBOOKCONF)
LUA_SCRIPT(GETBOOKCONFLIST)
LUA_SCRIPT(GETBOOKCONF)
LUA_SCRIPT(GETBOOKCONFE164BYMEETINGID)
LUA_SCRIPT(GETALLBOOKCONF)
LUA_SCRIPT(GETCREATECONFJSON)
LUA_SCRIPT(CHECKREDISDATA)
LUA_SCRIPT(UPDATECONF)
LUA_SCRIPT(GETSIMPLECONFLIST)
LUA_SCRIPT(GETCHARGE)
LUA_SCRIPT(GETEXPIREDMONITOR)
LUA_SCRIPT(ISCONFDATAEXIST)
LUA_SCRIPT(STARTINITREDISCONF)
END_LUA_SCRIPT

typedef std::vector<TSimpleConfInfo> ConfList;
typedef struct tagMonitorAliveInfo TMonitorAliveInfo;
typedef struct tagDBConf TDBConf;

enum EMConfNumType
{
	enumLicensedConfNum = 0,
	enumMaxLicensedConfNum,
	enumUnLicensedConfNum,
	enumLicensedSmallConfNum,
	enumLicensedLargeConfNum
};

class TRedisConfInfo
{
public:
	TRedisConfInfo() {
		memset(this, 0, sizeof(*this));
	}
	void SetConfE164(const s8* pchE164)
	{
		strncpy(m_achConfE164, pchE164, sizeof(m_achConfE164) - 1);
	}
	const s8* GetConfE164() const
	{
		return m_achConfE164;
	}

	void SetConfName(const s8* pchConfName)
	{
		strncpy(m_achConfName, pchConfName, sizeof(m_achConfName) - 1);
	}
	const s8* GetConfName() const
	{
		return m_achConfName;
	}
	void SetDomainMOID(const s8* achMoid)
	{
		strncpy(m_achDomainMOID, achMoid, sizeof(m_achDomainMOID) - 1);
	}
	const s8* GetDomainMOID() const
	{
		return m_achDomainMOID;
	}
	void SetCmuIp(const s8* achIp)
	{
		strncpy(m_achCmuIp, achIp, sizeof(m_achCmuIp) - 1);
	}
	void SetCmuIp(u32 dwIp)
	{
		in_addr in;
		in.s_addr = htonl(dwIp);
		strncpy(m_achCmuIp, inet_ntoa(in), sizeof(m_achCmuIp) - 1);
	}
	const s8* GetCmuIp() const
	{
		return m_achCmuIp;
	}
	void SetCmuPid(const s8* achPid)
	{
		m_dwCmuPid = strtoul(achPid, 0, 10);
	}
	void SetCmuPid(u32 dwPid)
	{
		m_dwCmuPid = dwPid;
	}
	const u32 GetCmuPid() const
	{
		return m_dwCmuPid;
	}
	void SetRealStartTime(const s8* achTime)
	{
		strncpy(m_achRealStartTime, achTime, sizeof(m_achRealStartTime) - 1);
	}
	void SetRealStartTime(const time_t time)
	{
		TKdvTime tTime(&time);
		tTime.GetString(m_achRealStartTime, sizeof(m_achRealStartTime));
	}
	const s8* GetRealStartTime() const
	{
		return m_achRealStartTime;
	}
	void SetStartTime(time_t time)
	{
		TKdvTime tTime(&time);
		tTime.GetString(m_achStartTime, sizeof(m_achStartTime));
	}
	void SetStartTime(const s8* achStartTime)
	{
		strncpy(m_achStartTime, achStartTime, sizeof(m_achStartTime) - 1);
	}
	const s8* GetStartTime() const
	{
		return m_achStartTime;
	}
	void SetBookStartTime(const s8* achStartTime)
	{
		strncpy(m_achBookStartTime, achStartTime, sizeof(m_achBookStartTime) - 1);
	}
	void SetBookStartTime(time_t time)
	{
		TKdvTime tTime(&time);
		tTime.GetString(m_achBookStartTime, sizeof(m_achBookStartTime));
	}
	const s8* GetBookStartTime() const
	{
		return m_achBookStartTime;
	}
	void SetNeedLicense(BOOL32 bNeed)
	{
		m_byNeedLicense = bNeed;
	}
	BOOL32 NeedLicense() const
	{
		return m_byNeedLicense;
	}
	void SetState(u8 byState)
	{
		m_byState = byState;
	}
	const u8 GetState() const
	{
		return m_byState;
	}
	void SetMeetingID(u32 dwMeetingId)
	{
		m_dwMeetingId = dwMeetingId;
	}
	u32 GetMeetingID() const
	{
		return m_dwMeetingId;
	}
	void SetDuration(u16 wDuration)
	{
		m_wDuration = wDuration;
	}
	u16 GetDuration() const
	{
		return m_wDuration;
	}
	u8 GetCreatorType() const
	{
		return m_byCreatorType;
	}
	void SetCreatorType(const u8 byCreatorType)
	{
		m_byCreatorType = byCreatorType;
	}
	void SetMaxJoinedMt(const u32 dwMaxJoinedMt)
	{
		m_dwMaxJoinedMt = dwMaxJoinedMt;
	}
	u32 GetMaxJoinedMt() const
	{
		return m_dwMaxJoinedMt;
	}
	void SetCreatorName(const s8* pchName)
	{
		strncpy(m_achCreatorName, pchName, sizeof(m_achCreatorName) - 1);
	}
	const s8* GetCreatorName() const
	{
		return m_achCreatorName;
	}
	void SetCreatorAccount(const s8* pchAccount)
	{
		strncpy(m_achCreatorAccount, pchAccount, sizeof(m_achCreatorAccount) - 1);
	}
	const s8* GetCreatorAccount() const
	{
		return m_achCreatorAccount;
	}
	void SetCreatorAccountType(const u8 byAccountType)
	{
		m_byCreatorAccountType = byAccountType;
	}
	u8 GetCreatorAccountType() const
	{
		return m_byCreatorAccountType;
	}
	void SetCreatorTelephone(const s8* pchTelephone)
	{
		strncpy(m_achCreatorTelephone, pchTelephone, sizeof(m_achCreatorTelephone) - 1);
	}
	const s8* GetCreatorTelephone() const
	{
		return m_achCreatorTelephone;
	}
	void SetCreatorMobile(const s8* pchMobile)
	{
		strncpy(m_achCreatorMobile, pchMobile, sizeof(m_achCreatorMobile) - 1);
	}
	const s8* GetCreatorMobile() const
	{
		return m_achCreatorMobile;
	}
	void Print() const
	{
		::OspPrintf(TRUE, FALSE, "\nName:        %s\n", m_achConfName);
		::OspPrintf(TRUE, FALSE, "E164:        %s\n", m_achConfE164);
		::OspPrintf(TRUE, FALSE, "State:       %d\n", m_byState);
		::OspPrintf(TRUE, FALSE, "DomainMoid:  %s\n", m_achDomainMOID);
		::OspPrintf(TRUE, FALSE, "Cmu:         %s:%d\n", m_achCmuIp, m_dwCmuPid);
		::OspPrintf(TRUE, FALSE, "RealStartTime:   %s\n", m_achRealStartTime);
		::OspPrintf(TRUE, FALSE, "StartTime:   %s\n", m_achStartTime);
		::OspPrintf(TRUE, FALSE, "BookTime:    %s\n", m_achBookStartTime);
		::OspPrintf(TRUE, FALSE, "Duration:    %d\n", m_wDuration);
		::OspPrintf(TRUE, FALSE, "NeedLicense: %d\n", m_byNeedLicense);
		::OspPrintf(TRUE, FALSE, "MeetingId:   %d\n", m_dwMeetingId);
		::OspPrintf(TRUE, FALSE, "CreatorType:   %d\n", m_byCreatorType);
		::OspPrintf(TRUE, FALSE, "MaxjoinedMt:   %d\n", m_dwMaxJoinedMt);
		::OspPrintf(TRUE, FALSE, "CreatorName:          %s\n", m_achCreatorName);
		::OspPrintf(TRUE, FALSE, "CreatorAccount:       %s\n", m_achCreatorAccount);
		::OspPrintf(TRUE, FALSE, "CreatorAccountType:   %d\n", m_byCreatorAccountType);
		::OspPrintf(TRUE, FALSE, "CreatorTelephone:     %s\n", m_achCreatorTelephone);
		::OspPrintf(TRUE, FALSE, "CreatorMobile:        %s\n", m_achCreatorMobile);
		::OspPrintf(TRUE, FALSE, "\n");
	}

	s8 m_achConfName[MAXLEN_CONFNAME + 1];
	s8 m_achConfE164[MAXLEN_CONFE164 + 1];
	s8 m_achDomainMOID[OPERATION_MAXLEN_MOID + 1];		     //域的MOID
	s8 m_achCmuIp[20];                              //记录会议所在mcu的IP
	s8 m_achCreatorName[128 + 1];
	s8 m_achCreatorAccount[128 + 1];
	u8 m_byCreatorAccountType;
	s8 m_achCreatorTelephone[128 + 1];
	s8 m_achCreatorMobile[128 + 1];
	u32 m_dwCmuPid;
	s8 m_achRealStartTime[20]; //会议初始创会时间
	s8 m_achStartTime[20];
	u16 m_wDuration;
	s8 m_achBookStartTime[20];
	u8 m_byNeedLicense;
	u8 m_byState;
	u32 m_dwMeetingId;
	u8 m_byCreatorType;
	u32 m_dwMaxJoinedMt;
};

class CRedisBookConf : public TRedisConfInfo
{
public:
	CRedisBookConf();
	void SetConfDetailJson(const std::string& jsonData);
	void SetBookEndTime(const std::string& time);
	void SetMtNum(const u32 dwNum);
	void SetOpenMode(const u8 byOpenMode);
	void SetSatMode(const u8 bySatMode);
	void SetIsPublicConf(const BOOL32 bPublicConf);
	void SetPassword(const std::string& password);
	void SetResolution(const u8 byResolution);
	void SetBitRate(const u16 wBitRate);
	void SetMediaType(const u8 byMediaType);
	void SetDualMode(const u8 byDualMode);
	void SetIsConfNodisturb(const BOOL32 bNodisturb);
	void SetMixMode(const u8 byMixMode);
	const std::string& GetConfDetailJson() const;
	const std::string& GetBookEndTime() const;
	const std::string& GetPassword() const;
	u32 GetMtNum() const;
	u8 GetOpenMode() const;
	u8 GetSatMode() const;
	BOOL32 GetIsPublicConf() const;
	u16 GetBitRate() const;
	u8 GetResolution() const;
	u8 GetMediaType() const;
	u8 GetDualMode() const;
	BOOL32 GetIsConfNodisturb() const;
	u16 GetDuration() const;
	u8 GetMixMode() const;
	void Print(const BOOL32 bPrintDetailJson = FALSE) const;
private:
	std::string m_confDetailJson;
	std::string m_password;
	std::string m_endTime;
	BOOL32 m_bPublicConf;
	BOOL32 m_bNoDisturb;
	u32 m_dwMtNum;
	u16 m_wBitRate;
	u8 m_byResolution;
	u8 m_byMediaType;
	u8 m_bySatMode;
	u8 m_byDualMode;
	u8 m_byOpenMode;
	u8 m_byMixMode;
};

class CRedisConfData : public CRedisWrap
{
public:
	BOOL32 GetConfListByDomain(const s8* pchDomainInfo, const s8* pchMtE164,
		const BOOL32 bCheckPublic, ConfList& lstConf);
	BOOL32 GetConfListByDomain(const s8* pchDomainMOID, std::list<TRedisConfInfo>& lstConf);
	BOOL32 GetConfNumByDomain(const s8* pchDomainMOID, 
		const EMConfNumType emConfNumType, u32& dwConfNum);
	BOOL32 GetCmuConfNum(const s8* pchCmuIp, u32& dwNum);
	BOOL32 SetMaxDomainConfNum(const s8* pchDomainMOID, 
		const u32 dwMaxSmallConfNum, const u32 dwMaxLargeConfNum);
	BOOL32 GetConfInfoByE164(const s8* achDomainInfo, const s8* achE164,
		TConfDetailInfo& tConfDetail);
	BOOL32 SetConfData(const TRedisConfInfo& tConfInfo);
	BOOL32 UpdateConfData(const TRedisConfInfo& tConfInfo);
	BOOL32 DelConf(const s8* pchDomainMoid, const s8* pchE164, const s8* pchMcuIp);
	BOOL32 GetConfExInfo(const s8* pchE164, TRedisConfInfo& tConfInfo);
	BOOL32 GetAllConfExInfo(std::list<TRedisConfInfo>& lstConfInfo);
	BOOL32 IsConfExist(const s8* abyE164, BOOL32& bExists);
	BOOL32 IsConfDataExist(const s8* pchConfE164, BOOL32& bExist);
	BOOL32 ChangeConfState(const s8* achMoid, const s8* achE164, u8 byState);
	BOOL32 GetConfListByCmuIp(const s8* pchCmuIp, std::list<TRedisConfInfo>& lstConfE164);
	BOOL32 ConstructE164(const s8* pchPrefix, const s8* pchBegin, const s8* pchEnd,
		std::string& strE164);
	BOOL32 UpdateMonitorAliveTime(const s8* pchE164, const s8* pchIp, const s8* pchPort);
	BOOL32 GetConfMonitorsInfo(const s8* pchE164, std::vector<TMonitorAliveInfo>& vecMonitors);
	BOOL32 GetExpiredMonitors(std::vector<TMonitorAliveInfo>& vecMonitors);
	BOOL32 AddBookConf(const CRedisBookConf& cConfInfo);
	BOOL32 ModBookConf(const CRedisBookConf& cConfInfo);
	BOOL32 DelBookConf(const s8* pchConfE164);
	BOOL32 GetBookConfList(const s8* pchDomainMOID, std::vector<CRedisBookConf>& confVec);
	BOOL32 GetBookConf(const s8* pchConfE164, CRedisBookConf& bookConf);
	BOOL32 GetBookConfE164ByMeetingID(const u32 dwMeetingID, std::string& strBookConfE164);
	BOOL32 GetAllBookConf(std::vector<CRedisBookConf>& confVec);
	BOOL32 GetCreateConfJson(const std::string& confE164, const BOOL32 bHasXmpu, std::string& createConfJson);
	BOOL32 CheckRedisData(std::string& checkResStr);
	BOOL32 GetChargeJson(const s8* pchConfE164, std::string& chargeJson);
	BOOL32 StartInitRedisConf();
private:
	OspLock m_cLock;
};
