/*****************************************************************************
模块名      : MAU
文件名      : mauutility.cpp
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
#pragma warning( disable:4786 )
#endif

#include "inc.h"
#include "mauutility.h"
#include "mauconst.h"
#include "zlib.h"
#include "evmau.h"
#include "base64.h"

#include <sstream>

extern SEMHANDLE g_hTestQConsumerSem;

//u8 GetVideoMode(const u16 wBitrate, const u8 byResolution)
//{
//	if (byResolution == VIDEO_FORMAT_HD720
//		|| byResolution == VIDEO_FORMAT_HD1080)
//	{
//		return HD_MODE;
//	}
//
//	if (wBitrate <= FLUENT_BITRATE)
//	{
//		return FLUENT_MODE;
//	}
//	else
//	{
//		return SD_MODE;
//	}
//}

//转换密码标识
BOOL32 GetNeedPswFlag(const s8 *pszConfPsw)
{
	//密码
	if ( 0 == strlen(pszConfPsw) )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void LongLogPrint(u8 byLogLvl, u16 wModuleId, const s8* achJsonBuf, 
	u32 dwPrintBuffLen, BOOL32 bAutoLineFeed/*=TRUE*/)
{
	while (dwPrintBuffLen > 0)
	{
		u32 dwCurLen = getmin(dwPrintBuffLen, 100);
		s8 achTmpBuffer[100 + 1] = { 0 };
		strncpy(achTmpBuffer, achJsonBuf, dwCurLen);
		achTmpBuffer[dwCurLen] = '\0';
		if (bAutoLineFeed)
		{
			LogPrint(byLogLvl, wModuleId, "%s\n", achTmpBuffer);
		}
		else
		{
			LogPrint(byLogLvl, wModuleId, "%s", achTmpBuffer);
		}
		achJsonBuf += dwCurLen;
		if (dwPrintBuffLen >= 100)
		{
			dwPrintBuffLen -= 100;
		}
		else
		{
			dwPrintBuffLen = 0;
		}
	}
}

API void ptempdbmsg( void )
{
	logenablemod( MID_MAU_TEMPDB );
}
API void nptempdbmsg( void )
{
	logdisablemod( MID_MAU_TEMPDB );
}

API void pvmdbmsg()
{
	logenablemod( MID_MAU_VMDB );
}
API void npvmdbmsg()
{
	logdisablemod( MID_MAU_VMDB );
}

API void pschedbmsg()
{
	logenablemod( MID_MAU_SCHEDB );
}
API void npschedbmsg()
{
	logdisablemod( MID_MAU_SCHEDB );
}

API void papdbmsg()
{
	logenablemod( MID_MAU_APDB );
}
API void npapdbmsg()
{
	logdisablemod( MID_MAU_APDB );
}

API void pmauvcmsg()
{
	logenablemod( MID_MAU_VC );
}
API void npmauvcmsg()
{
	logdisablemod( MID_MAU_VC );
}

API void ppublicmsg()
{
	logenablemod( MID_MAU_PUBLIC );
}
API void nppublicmsg()
{
	logdisablemod( MID_MAU_PUBLIC );
}

API void pmccmsg()
{
	logenablemod( MID_MAU_MCC );
}
API void npmccmsg()
{
	logdisablemod( MID_MAU_MCC );
}

API void pagwmsg()
{
	logenablemod( MID_MAU_AGW );
}
API void npagwmsg()
{
	logdisablemod( MID_MAU_AGW );
}

API void pcmumsg()
{
	logenablemod( MID_MAU_CMU );
}
API void npcmumsg()
{
	logdisablemod( MID_MAU_CMU );
}

API void pcmcmumsg()
{
	logenablemod( MID_MAU_CMCMU );
}
API void npcmcmumsg()
{
	logdisablemod( MID_MAU_CMCMU );
}

API void pcmmaumsg()
{
	logenablemod( MID_MAU_CMMAU );
}
API void npcmmaumsg()
{
	logdisablemod( MID_MAU_CMMAU );
}

API void phdumsg()
{
	logenablemod( MID_MAU_HDU );
}
API void nphdumsg()
{
	logdisablemod( MID_MAU_HDU );
}

API void pbmcmsg()
{
	logenablemod( MID_MAU_BMC );
}
API void npbmcmsg()
{
	logdisablemod( MID_MAU_BMC );
}

API void pnumsg()
{
	logenablemod( MID_MAU_NU );
}
API void npnumsg()
{
	logdisablemod( MID_MAU_NU );
}
API void pnppmsg()
{
	logenablemod(MID_MAU_NPP);
}
API void npnppmsg()
{
	logdisablemod(MID_MAU_NPP);
}
API void presmgrmsg()
{
	logenablemod( MID_MAU_RESMGR );
}
API void npresmgrmsg()
{
	logdisablemod( MID_MAU_RESMGR );
}

API void pupumsg()
{
	logenablemod( MID_MAU_UPU );
}
API void npupumsg()
{
	logdisablemod( MID_MAU_UPU );
}

API void pconfdbmsg()
{
	logenablemod( MID_MAU_CONFDB );
}
API void npconfdbmsg()
{
	logdisablemod( MID_MAU_CONFDB );
}

API void pconfbackmsg()
{
	logenablemod(MID_MAU_CONFBACK);
}

API void npconfbackmsg()
{
	logdisablemod(MID_MAU_CONFBACK);
}

API void predismsg(void)
{
	logenablemod(MID_MAU_REDIS);
}
API void npredismsg(void)
{
	logdisablemod(MID_MAU_REDIS);
}


//TKdvTime FormatStr2KdvTime(const char * szTime)
//{
//	struct tm tm1;
//
//	sscanf(szTime, "%4d-%2d-%2d %2d:%2d:%2d",
//		&tm1.tm_year,
//		&tm1.tm_mon,
//		&tm1.tm_mday,
//		&tm1.tm_hour,
//		&tm1.tm_min,
//		&tm1.tm_sec);
//
//	TKdvTime tKdvTime;
//	tKdvTime.SetYear(tm1.tm_year);
//	tKdvTime.SetMonth(tm1.tm_mon);
//	tKdvTime.SetDay(tm1.tm_mday);
//	tKdvTime.SetHour(tm1.tm_hour);
//	tKdvTime.SetMinute(tm1.tm_min);
//	tKdvTime.SetSecond(tm1.tm_sec);
//
//	return tKdvTime;
//}

BOOL32 IsNewMcu(const s8* abyMcuType)
{
	return strcmp(abyMcuType, MCU_TYPE_KDCJD10000A) == 0 ? TRUE : FALSE;
}

const s8* iptostr(const void *pAddr, s8 *pchIpAddr, u32 dwIpAddrLen)
{
#ifdef _WIN32
	const u8 *pbyAddr = (const u8*)pAddr;
	s8 achIpAddr[INET_ADDRSTRLEN] = { 0 };
	snprintf(achIpAddr, sizeof(achIpAddr), "%d.%d.%d.%d", pbyAddr[0], pbyAddr[1], pbyAddr[2], pbyAddr[3]);
	if (strlen(achIpAddr) >= dwIpAddrLen)
	{
		return NULL;
	}
	strcpy(pchIpAddr, achIpAddr);
	return pchIpAddr;
#else
	return inet_ntop(AF_INET, pAddr, pchIpAddr, dwIpAddrLen);
#endif
}


#define REDIS_MOID "moid"
#define REDIS_E164 "e164"
#define REDIS_CONFNAME "confname"
#define REDIS_MEETINGID "meetingid"
#define REDIS_MTNUM "mtnum"
#define REDIS_PASSWORD "password"
#define REDIS_PUBLISHCONF "publicconf"
#define REDIS_OPENMODE "openmode"
#define REDIS_MIXMODE "mixmode"
#define REDIS_CONFSTARTTIME "starttime"
#define REDIS_CONFENDTIME "endtime"
#define REDIS_ENCRYPTMODE "encryptmode"
#define REDIS_BITRATE "mediabitrate"
#define REDIS_RESOLUTION "mediares"
#define REDIS_CONFTYPE "conftype"
#define REDIS_NODISTURB "nodisturb"
#define REDIS_CONFDURATION "duration"
#define REDIS_DUALMODE "dualmode"
#define REDIS_CREATORNAME "creatorname"
#define REDIS_MAXJOINMTNUM "maxjoinedmt"
#define REDIS_MAXLICENSEDCONFNUM "maxlicensedconfnum"
#define REDIS_LICENSEDCONFNUM "licensedconfnum"
#define REDIS_UNLICENSEDCONFNUM "unlicensedconfnum"
#define REDIS_MCUIP "mcuip"
#define REDIS_CONFSTATE "confstate"
#define REDIS_IP "ip"
#define REDIS_PORT "port"
#define REDIS_MODE "mode"
#define REDIS_ALIVETIME "alivetime"
#define REDIS_MEDIATYPE "mediatype"

BOOL32 GetConfListFromJson(const s8* pchJsonBuf, ConfList& tConfList)
{
	if (pchJsonBuf == NULL)
	{
		OspPrintf(TRUE, FALSE, "[GetConfListFromjson is null\n");
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr jRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL ==  jRoot )
	{
		OspPrintf( TRUE, FALSE, "[GetConfListFromJson] json_loads err!\n" );
		return FALSE;
	}
	return GetConfListFromJsonObj(jRoot, tConfList);
}

BOOL32 GetConfListFromJsonObj(const json_t_ptr pjRoot, ConfList& tConfList)
{
	if (pjRoot == NULL)
	{
		OspPrintf( TRUE, FALSE, "[GetConfListFromJson]jRoot is null\n" );
		return FALSE;
	}

	const u32 nConfNum = json_array_size(pjRoot);
	OspPrintf( TRUE, FALSE, "[GetConfListFromJson]confNum: %u\n", nConfNum );
	for (u32 nConfIndex = 0; nConfIndex < nConfNum; nConfIndex++)
	{
		json_t_ptr pjConfInfo = json_array_get_s(pjRoot, nConfIndex);
		if (pjConfInfo != NULL)
		{
			TSimpleConfInfo tSimpleConfInfo;
			GetConfInfoFromJsonObj(pjConfInfo, tSimpleConfInfo);
			tConfList.push_back(tSimpleConfInfo);
		}
	}
	return TRUE;
}

BOOL32 GetConfInfoFromJsonObj(const json_t_ptr pjRoot, TSimpleConfInfo& tConfInfo)
{
	if (pjRoot == NULL)
	{
		OspPrintf( TRUE, FALSE, "[GetConfInfoFromJson]jRoot is null\n" );
		return FALSE;
	}

	const s8* pchConfE164 = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, REDIS_E164, &pchConfE164));
	const s8* pchConfName = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, REDIS_CONFNAME, &pchConfName));
	u32 dwConfMtNum = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_MTNUM, dwConfMtNum));
	const s8* pchConfEndTime = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, REDIS_CONFENDTIME, &pchConfEndTime));
	const s8* pchPassword = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, REDIS_PASSWORD, &pchPassword));
	u32 dwConfBitrate = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_BITRATE, dwConfBitrate));
	u32 dwResolution = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_RESOLUTION, dwResolution));
	u32 dwPublicConf = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_PUBLISHCONF, dwPublicConf));
	u32 dwOpenMode = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_OPENMODE, dwOpenMode));
	const s8* pchStartTime = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, REDIS_CONFSTARTTIME, &pchStartTime));
	u32 dwMediaType = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_MEDIATYPE, dwMediaType));

	tConfInfo.SetConfE164(pchConfE164, MAXLEN_E164);
	tConfInfo.SetConfName(pchConfName);
	tConfInfo.SetMtNum((u8)dwConfMtNum);
	TKdvTime tKdvTime = FormatStr2KdvTime(pchConfEndTime);
	time_t tEndTime;
	tKdvTime.GetTime(tEndTime);

	TKdvTime tKdvStartTime = FormatStr2KdvTime(pchStartTime);
	tConfInfo.SetStartTime(tKdvStartTime);
	if (tKdvTime == tKdvStartTime)
	{
		const u16 wRemainTime = 0;
		tConfInfo.SetRemainTime(wRemainTime);
	}
	else
	{
		time_t tCurTime = time(NULL);
		u16 wRemainTime = (u16)((tEndTime - tCurTime) / 60);
		if (wRemainTime < (u16)1)
		{
			wRemainTime = 1;
		}
		tConfInfo.SetRemainTime(wRemainTime);
	}

	const BOOL32 bNeedPsw = (0 != strlen(pchPassword));
	tConfInfo.SetNeedPsw(bNeedPsw);
	//会议模式：0 - 高清，1 - 标清, 2 - 流畅 ,3 - 自定义
	const u8 byConfMode = GetVideoMode((u8)dwMediaType, (u16)dwConfBitrate, (u8)dwResolution);
	tConfInfo.SetConfMode(byConfMode);
	tConfInfo.SetPublicConf((u8)dwPublicConf);
	u8 byCMMode = (dwOpenMode == 0 ? CONF_OPENMODE_CLOSED : CONF_OPENMODE_OPEN);
	tConfInfo.SetOpenMode(byCMMode);
	return TRUE;
}

BOOL32 GetConfInfoFromJson(const s8* pchJsonBuf, TConfDetailInfo& tConfDetailInfo)
{
	if (pchJsonBuf == NULL)
	{
		OspPrintf(TRUE, FALSE, "[GetConfInfoFromjson is null\n");
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL ==  pjRoot )
	{
		OspPrintf( TRUE, FALSE, "[GetConfInfoFromJson] json_loads err!\n" );
		return FALSE;
	}

	return GetConfInfoFromJson(pjRoot, tConfDetailInfo);
}

BOOL32 GetConfInfoFromJson(const json_t_ptr pjRoot, TConfDetailInfo& tConfInfo)
{
	if (pjRoot == NULL)
	{
		OspPrintf( TRUE, FALSE, "[GetConfInfoFromJson]jRoot is null\n" );
		return FALSE;
	}

	const s8* pchConfBelongDomainMoid = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, REDIS_MOID, &pchConfBelongDomainMoid));
	const s8* pchConfName = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, REDIS_CONFNAME, &pchConfName));
	const s8* pchConfE164 = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, REDIS_E164, &pchConfE164));
	u32 dwEncryptMode = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_ENCRYPTMODE, dwEncryptMode));
	u32 dwConfBitrate = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_BITRATE, dwConfBitrate));
	u32 dwConfResolution = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_RESOLUTION, dwConfResolution));
	u32 dwOpenMode = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_OPENMODE, dwOpenMode));
	const s8* pchStartTime = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, REDIS_CONFSTARTTIME, &pchStartTime));
	u32 dwDuration = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_CONFDURATION, dwDuration));
	const s8* pchCreatorName = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, REDIS_CREATORNAME, &pchCreatorName));
	u32 dwDualMode = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_DUALMODE, dwDualMode));
	u32 dwPublicConf = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_PUBLISHCONF, dwPublicConf));
	const s8* pchPassword = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, REDIS_PASSWORD, &pchPassword));
	u32 dwPortMode = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_CONFTYPE, dwPortMode));
	u32 dwNoDisturb = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_NODISTURB, dwNoDisturb));
	u32 dwMaxJoinMtNum = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_MAXJOINMTNUM, dwMaxJoinMtNum));
	u32 dwMixMode = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_MIXMODE, dwMixMode));
	u32 dwMediaType = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, REDIS_MEDIATYPE, dwMediaType));

	tConfInfo.m_tNewConfBaseInfo.SetDomainMOID(pchConfBelongDomainMoid);
	tConfInfo.m_tNewConfBaseInfo.SetConfName(pchConfName);
	tConfInfo.m_tNewConfBaseInfo.SetConfE164(pchConfE164);
	tConfInfo.SetEncryptMode((u8)dwEncryptMode);
	tConfInfo.SetBitRate((u16)dwConfBitrate);
	tConfInfo.SetResolution((u8)dwConfResolution);
	const u8 byConfMode = GetVideoMode((u8)dwMediaType, (u16)dwConfBitrate, (u8)dwConfResolution);
	tConfInfo.SetConfMode(byConfMode);
	u8 byCMMode = (dwOpenMode == 0 ? CONF_OPENMODE_CLOSED : CONF_OPENMODE_OPEN);
	tConfInfo.SetOpenMode(byCMMode);
	TKdvTime tKdvTime = FormatStr2KdvTime(pchStartTime);
	tConfInfo.SetStartTime(tKdvTime);
	tConfInfo.SetDuration((u16)dwDuration);
	tConfInfo.SetConfCreater(pchCreatorName);
	tConfInfo.SetDualMode((u8)dwDualMode);
	tConfInfo.SetPublicConf((u8)dwPublicConf);
	const BOOL32 bNeedPsw = (0 != strlen(pchPassword));
	tConfInfo.SetNeedPsw(bNeedPsw);
	tConfInfo.SetPortModeConf(dwPortMode == 1 ? TRUE : FALSE);
	tConfInfo.SetCloseMode(dwNoDisturb == 0 ? FALSE : TRUE);
	tConfInfo.SetMaxJoinMtNum((u8)dwMaxJoinMtNum);
	//发给老终端的是mixmode, 5.0终端依然发discuss
	tConfInfo.SetMixMode((u8)dwMixMode);
	tConfInfo.SetDiscuss(dwMixMode == 0 ? 0 : 1);
	if (pchConfE164 != NULL && strlen(pchConfE164) != 0)
	{
		s8 achConfShortNO[MAXLEN_SHORTNO + 1] = { 0 };
		strncpy(achConfShortNO, pchConfE164 + 1, MAXLEN_SHORTNO);
		tConfInfo.SetConfShortNO(achConfShortNO);
	}
	return TRUE;
}

void ConvertStruct(const std::vector<CRedisBookConf>& vecBookConf,
	std::vector<TSimpleConfInfo>& vecSimpleConf)
{
	vector<CRedisBookConf>::const_iterator iterConf = vecBookConf.begin();
	for (; iterConf != vecBookConf.end(); ++iterConf)
	{
		TSimpleConfInfo tSimpleConf;
		ConvertStruct(*iterConf, tSimpleConf);
		vecSimpleConf.push_back(tSimpleConf);
	}
}

void ConvertStruct(const std::vector<CTempCacheData>& vecBookConf,
	std::vector<TSimpleConfInfo>& vecSimpleConf)
{
	vector<CTempCacheData>::const_iterator iterConf = vecBookConf.begin();
	for (; iterConf != vecBookConf.end(); ++iterConf)
	{
		TSimpleConfInfo tSimpleConf;
		iterConf->ToSimpleConfInfo(tSimpleConf);
		vecSimpleConf.push_back(tSimpleConf);
	}
}

void ConvertStruct(const CRedisBookConf& cBookConf, TSimpleConfInfo& tSimpleConf)
{
	tSimpleConf.SetConfName(cBookConf.GetConfName());
	tSimpleConf.SetConfE164(cBookConf.GetConfE164(), MAXLEN_CONFE164);
	tSimpleConf.SetMtNum(cBookConf.GetMtNum());
	tSimpleConf.SetRemainTime(0);
	tSimpleConf.SetOpenMode(cBookConf.GetOpenMode());
	tSimpleConf.SetConfMode(GetVideoMode(cBookConf.GetMediaType(), cBookConf.GetBitRate(), cBookConf.GetResolution()));
	tSimpleConf.SetStartTime(FormatStr2KdvTime(cBookConf.GetBookStartTime()));
	tSimpleConf.SetSatDCastMode(cBookConf.GetSatMode() == 1 ? TRUE : FALSE);
	tSimpleConf.SetPublicConf(cBookConf.GetIsPublicConf() == TRUE ? 1 : 0);
	tSimpleConf.SetNeedPsw(!cBookConf.GetPassword().empty());
}

void ConvertStruct(const CRedisBookConf& cBookConf, TConfDetailInfo& tDetailConf)
{
	tDetailConf.m_tNewConfBaseInfo.SetConfE164(cBookConf.GetConfE164());
	tDetailConf.m_tNewConfBaseInfo.SetConfName(cBookConf.GetConfName());
	tDetailConf.m_tNewConfBaseInfo.SetDomainMOID(cBookConf.GetDomainMOID());
	tDetailConf.SetConfStyle(CONF_STYLE_BOOK);
	tDetailConf.SetBitRate(cBookConf.GetBitRate());
	tDetailConf.SetOpenMode(cBookConf.GetOpenMode());
	tDetailConf.SetStartTime(FormatStr2KdvTime(cBookConf.GetBookStartTime()));
	time_t tStartTime = 0;
	time_t tEndTime = 0;
	FormatStr2KdvTime(cBookConf.GetBookStartTime()).GetTime(tStartTime);
	FormatStr2KdvTime(cBookConf.GetBookEndTime().c_str()).GetTime(tEndTime);

	//EndTime为空，永久会议
	if (cBookConf.GetBookEndTime().empty())
	{
		tDetailConf.SetDuration(0);
	}
	else
	{
		tDetailConf.SetDuration((u16)((tEndTime - tStartTime) / 60));
	}
	tDetailConf.SetConfMode(GetVideoMode(cBookConf.GetMediaType(), cBookConf.GetBitRate(), cBookConf.GetResolution()));
	tDetailConf.SetPublicConf(cBookConf.GetIsPublicConf() ? 1 : 0);	//rnn
	tDetailConf.SetNeedPsw(!cBookConf.GetPassword().empty());
	tDetailConf.SetResolution(cBookConf.GetResolution());
	tDetailConf.SetSatDCastMode(cBookConf.GetSatMode() == 0 ? FALSE : TRUE);
	tDetailConf.SetDualMode(cBookConf.GetDualMode());
	tDetailConf.SetConfCreater(cBookConf.GetCreatorName());
	tDetailConf.SetCloseMode(cBookConf.GetIsPublicConf());
	tDetailConf.SetMaxJoinMtNum(cBookConf.GetMaxJoinedMt());
	//设置会议短号将会议e164转换为6位短号，终端上报详细信息时候，该字段不用，
	//nussn中会把会议E164号转化为PB中的短号发送给终端作为短号。
	s8 achConfShortNO[MAXLEN_SHORTNO + 1] = { 0 };
	strncpy(achConfShortNO, cBookConf.GetConfE164() + 1, MAXLEN_SHORTNO);
	tDetailConf.SetConfShortNO(achConfShortNO);
	tDetailConf.SetMixMode(cBookConf.GetMixMode());
	tDetailConf.SetDiscuss(cBookConf.GetMixMode() == 0 ? 0 : 1);
}

BOOL32 GetRedisConfFromJsonObj(const json_t_ptr& pjRoot, TRedisConfInfo &tConfInfo)
{
	const s8* pchCmuIp = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, "cmu", &pchCmuIp));
	u32 dwCmuPid = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, "cmupid", dwCmuPid));
	const s8* pchMoid = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, "moid", &pchMoid));
	const s8* pchE164 = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, "e164", &pchE164));
	const s8* pchRealStarTime = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, "realstarttime", &pchRealStarTime));
	const s8* pchStarTime = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, "starttime", &pchStarTime));
	const s8* pchBookTime = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, "booktime", &pchBookTime));
	u32 dwNeedLicensed = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, "needlicense", dwNeedLicensed));
	u32 dwState = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, "state", dwState));
	u32 dwMeetingId = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, "meetingid", dwMeetingId));
	const s8* pchConfName = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, "name", &pchConfName));
	u32 dwDuration = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, "duration", dwDuration));
	u32 dwCreatorType = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, "creatortype", dwCreatorType));
	u32 dwMaxJoinedMt = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, "maxjoinedmt", dwMaxJoinedMt));

	tConfInfo.SetCmuIp(pchCmuIp);
	tConfInfo.SetCmuPid(dwCmuPid);
	tConfInfo.SetConfE164(pchE164);
	tConfInfo.SetDomainMOID(pchMoid);
	tConfInfo.SetRealStartTime(pchRealStarTime);
	tConfInfo.SetStartTime(pchStarTime);
	tConfInfo.SetBookStartTime(pchBookTime);
	tConfInfo.SetNeedLicense(dwNeedLicensed);
	tConfInfo.SetState((u8)dwState);
	tConfInfo.SetMeetingID(dwMeetingId);
	tConfInfo.SetConfName(pchConfName);
	tConfInfo.SetDuration((u16)dwDuration);
	tConfInfo.SetCreatorType((u8)dwCreatorType);
	tConfInfo.SetMaxJoinedMt(dwMaxJoinedMt);
	GetCreatorInfoFromJson(pjRoot, tConfInfo, TRUE);
	return TRUE;
}

BOOL32 GetRedisConfFromJson(const s8* pchJson, TRedisConfInfo& tConfInfo)
{
	json_t_ptr pjRoot;
	if (NULL == pchJson)
	{
		OspPrintf(TRUE, FALSE, "[%s] Param is NULL, retur!\n", __FUNCTION__);
		return FALSE;
	}
	ReturnIfFailed(LoadJson(pchJson, strlen(pchJson), pjRoot, FALSE));
	return GetRedisConfFromJsonObj(pjRoot, tConfInfo);
}

BOOL32 GetRedisConfListFromJsonObj(const json_t_ptr& pjRoot, std::list<TRedisConfInfo>& lstConfList)
{
	s32 dwNum = json_array_size(pjRoot);
	for (int i = 0; i < dwNum; ++i)
	{
		json_t_ptr pjObj = json_array_get_s(pjRoot, i);
		TRedisConfInfo tConfInfo;
		GetRedisConfFromJsonObj(pjObj, tConfInfo);
		lstConfList.push_back(tConfInfo);
	}
	return TRUE;
}

BOOL32 GetRedisConfListFromJson(const s8* pchJson, std::list<TRedisConfInfo>& lstConfList)
{
	json_t_ptr pjRoot;
	ReturnIfFailed(LoadJson(pchJson, strlen(pchJson), pjRoot, FALSE));
	return GetRedisConfListFromJsonObj(pjRoot, lstConfList);
}

BOOL32 GetRedisBookConfFromJsonObj(const json_t_ptr& pjRoot, CRedisBookConf &cBookConf)
{
	const s8* pchConfName = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, JSON_CONFNAME, &pchConfName));
	u32 dwOpenMode = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_OPENMODE, dwOpenMode));
	u32 dwPublicConf = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_PUBLISHCONF, dwPublicConf));
	const s8* pchConfE164 = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, "e164", &pchConfE164));
	const s8* pchBookStartTime = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, JSON_CONFSTARTTIME, &pchBookStartTime));
	const s8* pchPassword = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, "password", &pchPassword));
	const s8* pchBookConfDetailJson = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, "detailjson", &pchBookConfDetailJson));
	u32 dwMtNum = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, "mtnum", dwMtNum));
	const s8* pchBookEndTime = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, JSON_CONFENDTIME, &pchBookEndTime));
	const s8* pchDomainMOID = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, JSON_MOID, &pchDomainMOID));
	u32 dwBitRate = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_BITRATE, dwBitRate));
	u32 dwMeetingID = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, "meetingid", dwMeetingID));
	u32 dwResolution = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_RES, dwResolution));
	u32 dwSatMode = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_SAT, dwSatMode));
	u32 dwDualMode = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_DUALMODE, dwDualMode));
	u32 dwNoDisturb = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_NODISTURB, dwNoDisturb));
	u32 dwMaxJoinedMtNum = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_MAXJOINEDMT, dwMaxJoinedMtNum));
	u32 dwMixMode = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_MIXMODE, dwMixMode));
	u32 dwMediaType = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_MEDIATYPE, dwMediaType));

	cBookConf.SetBookEndTime(pchBookEndTime);
	cBookConf.SetMtNum(dwMtNum);
	cBookConf.SetConfName(pchConfName);
	cBookConf.SetOpenMode((u8)dwOpenMode);
	cBookConf.SetIsPublicConf(dwPublicConf == 0 ? FALSE : TRUE);
	cBookConf.SetConfE164(pchConfE164);
	cBookConf.SetBookStartTime(pchBookStartTime);
	cBookConf.SetPassword(pchPassword);
	cBookConf.SetConfDetailJson(pchBookConfDetailJson);
	cBookConf.SetDomainMOID(pchDomainMOID);
	cBookConf.SetBitRate(dwBitRate);
	cBookConf.SetMeetingID(dwMeetingID);
	cBookConf.SetResolution((u8)dwResolution);
	cBookConf.SetMediaType(dwMediaType);
	cBookConf.SetSatMode((u8)dwSatMode);
	cBookConf.SetDualMode((u8)dwDualMode);
	cBookConf.SetIsConfNodisturb(dwNoDisturb == 0 ? FALSE : TRUE);
	cBookConf.SetMaxJoinedMt((u8)dwMaxJoinedMtNum);
	cBookConf.SetMixMode((u8)dwMixMode);
	GetCreatorInfoFromJson(pjRoot, cBookConf, TRUE);
	return TRUE;
}

BOOL32 GetRedisBookConfListFromJsonObj(const json_t_ptr& pjRoot, vector<CRedisBookConf>& bookConfVec)
{
	s32 dwNum = json_array_size(pjRoot);
	for (int i = 0; i < dwNum; ++i)
	{
		json_t_ptr pjObj = json_array_get_s(pjRoot, i);
		CRedisBookConf tConfInfo;
		GetRedisBookConfFromJsonObj(pjObj, tConfInfo);
		bookConfVec.push_back(tConfInfo);
	}
	return TRUE;
}

BOOL32 GetRedisBookConfFromJson(const s8* pchJson, CRedisBookConf& bookConf)
{
	if (pchJson == NULL || strlen(pchJson) == 0)
	{
		return FALSE;
	}
	json_t_ptr pjRoot;
	ReturnIfFailed(LoadJson(pchJson, strlen(pchJson), pjRoot, FALSE));
	return GetRedisBookConfFromJsonObj(pjRoot, bookConf);
}

BOOL32 GetRedisBookConfListFromJson(const s8* pchJson, vector<CRedisBookConf>& bookConfVec)
{
	if (pchJson == NULL || strlen(pchJson) == 0)
	{
		return FALSE;
	}
	json_t_ptr pjRoot;
	ReturnIfFailed(LoadJson(pchJson, strlen(pchJson), pjRoot, FALSE));
	return GetRedisBookConfListFromJsonObj(pjRoot, bookConfVec);
}

BOOL32 GetEnableHDMeetingFromJson(const s8 * pchJsonBuf, BOOL32& bEnableHDMeeting)
{
	if (pchJsonBuf == NULL || strlen(pchJsonBuf)  == 0)
	{
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads(pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason);
	if (NULL == pjRoot)
	{
		OspPrintf(TRUE, FALSE, "[GetEnableHDMeetingFromJson] json_loads err, retur FALSE!\n");
		return FALSE;
	}

	json_t_ptr pjEnabled = json_object_get_s(pjRoot, "enableHDMeeting");
	if (NULL == pjEnabled)
	{
		OspPrintf(TRUE, FALSE, "[GetEnableHDMeetingFromJson] json_object_get enableHDMeeting is NULL, return FALSE!\n");
		return FALSE;
	}
	bEnableHDMeeting = json_is_true(pjEnabled);
	return TRUE;
}

BOOL32 GetEnableH265FromJson(const s8 * pchJsonBuf, BOOL32& bEnableH265)
{
	if (pchJsonBuf == NULL || strlen(pchJsonBuf) == 0)
	{
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads(pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason);
	if (NULL == pjRoot)
	{
		OspPrintf(TRUE, FALSE, "[GetEnableH265FromJson] json_loads err, retur FALSE!\n");
		return FALSE;
	}

	json_t_ptr pjEnabled = json_object_get_s(pjRoot, "enableH265");
	if (NULL == pjEnabled)
	{
		OspPrintf(TRUE, FALSE, "[GetEnableH265FromJson] json_object_get enableH265 is NULL, return FALSE!\n");
		return FALSE;
	}
	bEnableH265 = json_is_true(pjEnabled);
	return TRUE;
}
void GetMonitorInfoFromJsonObj(const json_t_ptr& pjRoot, TMonitorAliveInfo& tMonitorInfo)
{
	const s8* pchIp = NULL;
	if (GetStringFromJson(pjRoot, REDIS_IP, &pchIp))
	{
		strncpy(tMonitorInfo.m_achIpAddr, pchIp, sizeof(tMonitorInfo.m_achIpAddr) - 1);
	}

	const s8* pchPort = NULL;
	if (GetStringFromJson(pjRoot, REDIS_PORT, &pchPort))
	{
		strncpy(tMonitorInfo.m_achPort, pchPort, sizeof(tMonitorInfo.m_achPort) - 1);
	}

	u32 dwMediaMode = 0;
	if (GetStringAsUIntFromJson(pjRoot, REDIS_MODE, dwMediaMode))
	{
		tMonitorInfo.m_byMediaMode = (u8)dwMediaMode;
	}

	u32 dwSecondsNum = 0;
	if (GetStringAsUIntFromJson(pjRoot, REDIS_ALIVETIME, dwSecondsNum))
	{
		tMonitorInfo.m_tAliveTime = (time_t)dwSecondsNum;
	}

	const s8* pchE164 = NULL;
	if (GetStringFromJson(pjRoot, REDIS_E164, &pchE164))
	{
		strncpy(tMonitorInfo.m_achConfE164, pchE164, sizeof(tMonitorInfo.m_achConfE164) - 1);
	}

	const s8* pchMcuIp = NULL;
	if (GetStringFromJson(pjRoot, REDIS_MCUIP, &pchMcuIp))
	{
		strncpy(tMonitorInfo.m_achMcuIpAddr, pchMcuIp, sizeof(tMonitorInfo.m_achMcuIpAddr) - 1);
	}
}

/*=============================================================================
函 数 名： GetConfE164FromJson
功    能：
算法实现：
全局变量：
参    数： void
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2016/04/07  5.0         王力                创建
=============================================================================*/
BOOL32 GetReqidFromJson(const json_t_ptr& pjRoot, s8 * pchReqid, u32 dwReqidLen)
{
	json_t_ptr pjReqid = json_object_get_s(pjRoot, JSON_REQID);
	if (NULL == pjReqid)
	{
		OspPrintf(TRUE, FALSE, "[GetReqidFromJson] json_object_get Reqid is NULL, retur FALSE!\n");
		return FALSE;
	}

	strncpy(pchReqid, json_string_value(pjReqid), dwReqidLen);
	return TRUE;
}

BOOL32 GetReqidFromJson(const s8 * pchJsonBuf, s8 * pchReqid, u32 dwReqidLen)
{

	if (NULL == pchJsonBuf || NULL == pchReqid || 0 == dwReqidLen)
	{
		OspPrintf(TRUE, FALSE, "[GetReqidFromJson] Param is NULL, return FLASE!\n");
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads(pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason);
	if (NULL == pjRoot)
	{
		OspPrintf(TRUE, FALSE, "[GetReqidFromJson] json_loads err, retur FALSE!\n");
		return FALSE;
	}

	return GetReqidFromJson(pjRoot, pchReqid, dwReqidLen);
}

BOOL32 GetCreatorInfoFromJson(const json_t_ptr& pjRoot, 
	TRedisConfInfo& tConfInfo, const BOOL32 bRedisData/*=FALSE*/)
{
	const s8* pchCreatorAlias = NULL;
	if (GetStringFromJson(pjRoot, JSON_CREATORNAME, &pchCreatorAlias))
	{
		tConfInfo.SetCreatorName(pchCreatorAlias);
	}

	const s8* pchCreatorAccount = NULL;
	const s8* pchJsonAccountName = (bRedisData ? JSON_CREATORACCOUNT : JSON_CREATORMOID);
	if (GetStringFromJson(pjRoot, pchJsonAccountName, &pchCreatorAccount))
	{
		tConfInfo.SetCreatorAccount(pchCreatorAccount);
	}
	u32 dwCreatorAccountType = 0;
	if (GetStringAsUIntFromJson(pjRoot, JSON_CREATORACCOUNTTYPE, dwCreatorAccountType))
	{
		tConfInfo.SetCreatorAccountType((u8)dwCreatorAccountType);
	}
	const s8* pchCreatorTelephone = NULL;
	if (GetStringFromJson(pjRoot, JSON_CREATORTELEPHONE, &pchCreatorTelephone))
	{
		tConfInfo.SetCreatorTelephone(pchCreatorTelephone);
	}
	const s8* pchCreatorMobile = NULL;
	if (GetStringFromJson(pjRoot, JSON_CREATORMOBILE, &pchCreatorMobile))
	{
		tConfInfo.SetCreatorMobile(pchCreatorMobile);
	}
	return TRUE;
}

void GetConfMonitorsInfoFromJsonObj(const json_t_ptr& pjRoot, vector<TMonitorAliveInfo>& monitorVec)
{
	s32 dwNum = json_array_size(pjRoot);
	for (int i = 0; i < dwNum; ++i)
	{
		json_t_ptr pjObj = json_array_get_s(pjRoot, i);
		TMonitorAliveInfo tMonitorInfo;
		GetMonitorInfoFromJsonObj(pjObj, tMonitorInfo);
		monitorVec.push_back(tMonitorInfo);
	}
}

BOOL32 SetStopMonitorsInfoToJsonObj(json_t_ptr& pjRoot, const std::vector<TMonitorAliveInfo>& monitorVec)
{
	if ( NULL == pjRoot )
	{
		OspPrintf(TRUE, FALSE, "[%s] pjRoot is NULL, return\n", __FUNCTION__);
		return FALSE;
	}

	json_t_ptr pjDstAddr = json_array();
	if (NULL == pjDstAddr)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object pjDstAddr is NULL!\n", __FUNCTION__);
		return FALSE;
	}

	vector<TMonitorAliveInfo>::const_iterator iterMonitor = monitorVec.begin();
	for (; iterMonitor != monitorVec.end(); ++iterMonitor)
	{
		json_t_ptr pjTransPortAddr = json_object();
		if (NULL == pjTransPortAddr)
		{
			OspPrintf(TRUE, FALSE, "[%s] json_object pjTransPortAddr is NULL!\n", __FUNCTION__);
			return FALSE;
		}
		json_t_ptr pjAddr = json_object();
		if (NULL == pjAddr)
		{
			OspPrintf(TRUE, FALSE, "[%s] json_object pjAddr is NULL!\n", __FUNCTION__);
			return FALSE;
		}
		ReturnIfFailed(SetStringToJson(pjAddr, JSON_IP, iterMonitor->m_achIpAddr));
		ReturnIfFailed(SetStringToJson(pjAddr, JSON_PORT, iterMonitor->m_achPort));
		json_object_set_s(pjTransPortAddr, JSON_TRANSPORTADDR, pjAddr);
		json_array_append_s(pjDstAddr, pjTransPortAddr);
	}
	json_object_set_s(pjRoot, JSON_DSTADDR, pjDstAddr);
	return TRUE;
}

BOOL32 GetConfMonitorsInfoFromJson(const s8* pchJson, std::vector<TMonitorAliveInfo>& monitorVec)
{
	json_t_ptr pjRoot;
	ReturnIfFailed(LoadJson(pchJson, strlen(pchJson), pjRoot, FALSE));
	GetConfMonitorsInfoFromJsonObj(pjRoot, monitorVec);
	return TRUE;
}

BOOL32 IsMtInConf(const s8* pchMtE164, const s8* pchConfDetailJson)
{
	json_t_ptr pjRoot;
	ReturnIfFailed(LoadJson(pchConfDetailJson, strlen(pchConfDetailJson), pjRoot, FALSE));

	json_t_ptr pjMtArry = json_object_get_s( pjRoot, JSON_MT);
	if ( NULL == pjMtArry)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[%s]json not has mt\n", __FUNCTION__ );
		return FALSE;
	}
	
	const u32 dwMtNum = json_array_size(pjMtArry);
	for (u32 dwMtIndex = 0; dwMtIndex != dwMtNum; ++dwMtIndex)
	{
		json_t_ptr pjMtInfo = json_array_get_s(pjMtArry, dwMtIndex);
		if (pjMtInfo != NULL)
		{
			const s8* pchMtAlias = NULL;
			GetStringFromJson(pjMtInfo, JSON_MTNO, &pchMtAlias);
			if (strncmp(pchMtAlias, pchMtE164, MAXLEN_E164) == 0)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL32 GetDBConfInfoFromJson(const s8* pchJson, TDBConf& tDBConf)
{
	json_t_ptr pjRoot;
	ReturnIfFailed(LoadJson(pchJson, strlen(pchJson), pjRoot, FALSE));
	return GetDBConfInfoFromJson(pjRoot, tDBConf);
}

BOOL32 GetDBConfInfoFromJson(const json_t_ptr& pjRoot, TDBConf& tDBConf)
{
	const s8* pchConfName = "";
	GetStringFromJson(pjRoot, JSON_CONFNAME, &pchConfName);
	const s8* pchConfE164 = "";
	GetStringFromJson(pjRoot, JSON_CONFE164, &pchConfE164);
	const s8* pchPassword = "";
	GetStringFromJson(pjRoot, JSON_PSW, &pchPassword);
	tDBConf.SetConfE164(pchConfE164);
	tDBConf.SetConfName(pchConfName);
	tDBConf.SetConfPwd(pchPassword);
	return  TRUE;
}

BOOL32 SetChargeInfoToJson(const TRedisConfInfo& tRedisConf, std::string& chargeJson)
{
	json_t_ptr pjRoot;
	ReturnIfFailed(LoadJson(chargeJson.c_str(), chargeJson.length(), pjRoot, FALSE));

	time_t tBookStartTime = 0;
	FormatStr2KdvTime(tRedisConf.GetBookStartTime()).GetTime(tBookStartTime);
	time_t tRealStartTime = 0;
	FormatStr2KdvTime(tRedisConf.GetRealStartTime()).GetTime(tRealStartTime);
	time_t tRealEndTime(time(NULL));

	SetUIntAsStringToJson(pjRoot, "startTime", (u32)tBookStartTime);
	SetUIntAsStringToJson(pjRoot, "endTime", (u32)tBookStartTime);
	SetUIntAsStringToJson(pjRoot, "realStartTime", (u32)tRealStartTime);
	SetUIntAsStringToJson(pjRoot, "realEndTime", (u32)tRealEndTime);

	json_str achJsonBuf(pjRoot);
	chargeJson = achJsonBuf;
	return TRUE;
}

s32 B64DecodeAndUnCompressXmlMsg(s8* pchData, u8* pchXmlMsg, u32& dwXmlMsgLen)
{
	u8 abyCompressXmlMsg[10240 * 5 + 1] = { 0 };

	unsigned long dwCompressXmlMsgLen = sizeof(abyCompressXmlMsg);
	CBase64Codec::Decode((const u8*)pchData, strlen(pchData), abyCompressXmlMsg, dwCompressXmlMsgLen);

	const u32 dwErrCode = uncompress((Bytef*)pchXmlMsg, (uLongf*)&dwXmlMsgLen,
		(const Bytef*)abyCompressXmlMsg, dwCompressXmlMsgLen);
	if (dwErrCode != 0)
	{
		OspPrintf(TRUE, FALSE, "[%s] uncompress xml msg err, errcode:%d\n",
			__FUNCTION__, dwErrCode);
		return dwErrCode;
	}
	return 0;
}

BOOL32 IsOldMcu(const s8* achIp)
{
	if (!achIp || achIp[0] == 0)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL32 IsOldMcuByJson(const s8* pchJsonBuf)
{
	s8 abyMcuIp[MAXLEN_IPADDR + 1] = { 0 };
	GetCmuIpFromJson((s8*)pchJsonBuf, abyMcuIp, sizeof(abyMcuIp) - 1);
	return IsOldMcu(abyMcuIp);
}

BOOL32 GetMaxJoinedMtFromJson(const s8* pchJsonBuf, u32& dwMaxJoinedMt)
{
	if (NULL == pchJsonBuf)
	{
		OspPrintf(TRUE, FALSE, "[%s] Param is NULL, return FLASE!\n", __FUNCTION__);
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	return GetStringAsUIntFromJson(pjRoot, JSON_MAXJOINEDMT, dwMaxJoinedMt);
}

BOOL32 GetConfNumLimitFromJsonObj(const json_t_ptr& pjRoot, CPlatformConfNumLimit& cConfNumLimit, const BOOL32 bBmcLicense /* = TRUE */)
{
	if (NULL == pjRoot)
	{
		OspPrintf( TRUE, FALSE, "[GetMeetingLimitFromJson] json_loads err, return FALSE!\n" );
		return FALSE;
	}

	u32 dw192_1080P_ConfNumLimit = 0;
	u32 dw192_720P_ConfNumLimit = 0;
	u32 dw64_1080P_ConfNumLimit = 0;
	u32 dw64_720P_ConfNumLimit = 0;
	u32 dw32_1080P_ConfNumLimit = 0;
	u32 dw32_720P_ConfNumLimit = 0;
	u32 dw8_1080P_ConfNumLimit = 0;
	u32 dw8_720P_ConfNumLimit = 0;
	const s8* pchJson_192_1080P_ConfNumLimit = (bBmcLicense ? "KEY_LARGE_1080P_HD_CONFERENCE_LIMIT" : BMC_192_1080P_CONFNUM_LIMIT);
	const s8* pchJson_192_720P_ConfNumLimit = (bBmcLicense ? BMCLICENSE_192_720P_CONFNUM_LIMIT : BMC_192_720P_CONFNUM_LIMIT);
	const s8* pchJson_64_1080P_ConfNumLimit = (bBmcLicense ? BMCLICENSE_64_1080P_CONFNUM_LIMIT : BMC_64_1080P_CONFNUM_LIMIT);
	const s8* pchJson_64_720P_ConfNumLimit = (bBmcLicense ? BMCLICENSE_64_720P_CONFNUM_LIMIT : BMC_64_720P_CONFNUM_LIMIT);
	const s8* pchJson_32_1080P_ConfNumLimit = (bBmcLicense ? BMCLICENSE_32_1080P_CONFNUM_LIMIT : BMC_32_1080P_CONFNUM_LIMIT);
	const s8* pchJson_32_720P_ConfNumLimit = (bBmcLicense ? BMCLICENSE_32_720P_CONFNUM_LIMIT : BMC_32_720P_CONFNUM_LIMIT);
	const s8* pchJson_8_1080P_ConfNumLimit = (bBmcLicense ? "KEY_SMALL_1080P_HD_CONFERENCE_LIMIT" : BMC_8_1080P_CONFNUM_LIMIT);
	const s8* pchJson_8_720P_ConfNumLimit = (bBmcLicense ? BMCLICENSE_8_720P_CONFNUM_LIMIT : BMC_8_720P_CONFNUM_LIMIT);
	ReturnIfFailed(GetIntFromJson(pjRoot, pchJson_192_1080P_ConfNumLimit, dw192_1080P_ConfNumLimit));
	//ReturnIfFailed(GetIntFromJson(pjRoot, pchJson_192_720P_ConfNumLimit, dw192_720P_ConfNumLimit));
	//ReturnIfFailed(GetIntFromJson(pjRoot, pchJson_64_1080P_ConfNumLimit, dw64_1080P_ConfNumLimit));
	//ReturnIfFailed(GetIntFromJson(pjRoot, pchJson_64_720P_ConfNumLimit, dw64_720P_ConfNumLimit));
	//ReturnIfFailed(GetIntFromJson(pjRoot, pchJson_32_1080P_ConfNumLimit, dw32_1080P_ConfNumLimit));
	//ReturnIfFailed(GetIntFromJson(pjRoot, pchJson_32_720P_ConfNumLimit, dw32_720P_ConfNumLimit));
	ReturnIfFailed(GetIntFromJson(pjRoot, pchJson_8_1080P_ConfNumLimit, dw8_1080P_ConfNumLimit));
	//ReturnIfFailed(GetIntFromJson(pjRoot, pchJson_8_720P_ConfNumLimit, dw8_720P_ConfNumLimit));

	u32 dwLargeConfNumLimit = dw192_1080P_ConfNumLimit
		+ dw192_720P_ConfNumLimit
		+ dw64_1080P_ConfNumLimit
		+ dw64_720P_ConfNumLimit
		+ dw32_1080P_ConfNumLimit
		+ dw32_720P_ConfNumLimit;
	u32 dwSmallConfNumLimit = dw8_1080P_ConfNumLimit + dw8_720P_ConfNumLimit;
	cConfNumLimit.SetSmallConfNumLimit(dwSmallConfNumLimit);
	cConfNumLimit.SetLargeConfNumLimit(dwLargeConfNumLimit);
	return TRUE;
}

BOOL32 GetConfNumLimitFromJson(const s8 * pchJsonBuf,
	CPlatformConfNumLimit& cConfNumLimit, const BOOL32 bBmcLicense /*=TRUE*/)
{
	if (NULL == pchJsonBuf)
	{
		OspPrintf(TRUE, FALSE, "[%s] pchJsonBuf is NULL, retur FALSE!\n", __FUNCTION__);
		return FALSE;
	}

	json_t_ptr pjRoot;
	ReturnIfFailed(LoadJson(pchJsonBuf, strlen(pchJsonBuf), pjRoot, FALSE));
	return GetConfNumLimitFromJsonObj(pjRoot, cConfNumLimit, bBmcLicense);
}
BOOL32 GetCreateConfParamFromJson(const s8* pchJsonBuf, TConfInfo& tConfInfo)
{
	if (NULL == pchJsonBuf)
	{
		OspPrintf(TRUE, FALSE, "[%s] pchJsonBuf is NULL, retur FALSE!\n", __FUNCTION__);
		return FALSE;
	}

	json_t_ptr pjRoot;
	ReturnIfFailed(LoadJson(pchJsonBuf, strlen(pchJsonBuf), pjRoot, FALSE));
	return GetCreateConfParamFromJsonObj(pjRoot, tConfInfo);
}
BOOL32 GetCreateConfParamFromJsonObj(const json_t_ptr& pjRoot, TConfInfo& tConfInfo)
{
	if (!pjRoot)
	{
		return FALSE;
	}
	TSimCapSet tMain;
	TSimCapSet tSecond;
	TMtAlias tMtAlias;
	TMediaEncrypt tMediaEncrypt;
	TCapSupport tCapSupport;
	TCapSupportEx tCapSupportEx;
	TConfAttrb tConfAttrb;

	tMain.Clear();
	tSecond.Clear();
	tMtAlias.SetNull();
	tConfInfo.Reset();
	tMediaEncrypt.Reset();
	tCapSupport.Clear();
	tCapSupportEx.Clear();

	// ConfE164
	json_t_ptr pjConfE164 = json_object_get_s(pjRoot, JSON_CONFE164);
	if (NULL == pjConfE164)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get ConfE164 is NULL!\n", __FUNCTION__);
	}
	else
	{
		tConfInfo.SetConfE164(json_string_value(pjConfE164));
	}

	// ConfName
	json_t_ptr pjConfName = json_object_get_s(pjRoot, JSON_CONFNAME);
	if (NULL == pjConfName)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get ConfName is NULL!\n", __FUNCTION__);
	}
	else
	{
		tConfInfo.SetConfName(json_string_value(pjConfName));
	}

	// 创会者别名
	json_t_ptr pjCreatorName = json_object_get_s(pjRoot, JSON_CREATORNAME);
	if (NULL == pjCreatorName)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get CreatorName is NULL!\n", __FUNCTION__);
	}
	else
	{
		tConfInfo.SetCreaterAlias(json_string_value(pjCreatorName));
	}

	//是否端口会议
	json_t_ptr pjIsPortConf = json_object_get_s(pjRoot, JSON_ISPORTCONF);
	if (NULL == pjIsPortConf)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get IsPortConf is NULL!\n", __FUNCTION__);
	}
	else
	{
		BOOL32 bIsPortConf = (1 == atoi(json_string_value(pjIsPortConf))) ? TRUE : FALSE;
		tConfAttrb.SetPortConfFlag(bIsPortConf);
	}

	//入会密码
	json_t_ptr pjConfPwd = json_object_get_s(pjRoot, JSON_PSW);
	if (NULL == pjConfPwd)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get ConfPwd is NULL!\n", __FUNCTION__);
	}
	else
	{
		if (0 != strlen(json_string_value(pjConfPwd)))
		{
			tConfAttrb.SetNeedConfPwd(TRUE);
			tConfInfo.SetConfPwd(json_string_value(pjConfPwd));
		}
		else
		{
			tConfAttrb.SetNeedConfPwd(FALSE);
		}
	}

	json_t_ptr pjBandWidth = json_object_get_s(pjRoot, JSON_BANDWIDTH);
	if (NULL == pjBandWidth)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get BandWidth is NULL!\n", __FUNCTION__);
	}
	else
	{
		tConfInfo.SetBitRate((u16)atoi(json_string_value(pjBandWidth)));
	}

	json_t_ptr pjConfRes = json_object_get_s(pjRoot, JSON_RES);
	if (NULL == pjConfRes)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get ConfRes is NULL!\n", __FUNCTION__);
	}
	else
	{
		tConfInfo.SetMainVideoFormat((u8)atoi(json_string_value(pjConfRes)));
	}

	//会议时长
	json_t_ptr pjDuration = json_object_get_s(pjRoot, JSON_DURATION);
	if (NULL == pjDuration)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get Duration is NULL!\n", __FUNCTION__);
	}
	else
	{
		tConfInfo.SetDuration((u16)atoi(json_string_value(pjDuration)));
	}

	//是否自动加密
	json_t_ptr pjEncryptManual = json_object_get_s(pjRoot, JSON_ENCRYPT_MANUAL);
	if (NULL == pjEncryptManual)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get pjEncryptManual is NULL!\n", __FUNCTION__);
	}
	else
	{
		//tMediaEncrypt.SetEncryptMode( (u8)atoi( json_string_value( pjEncryptManual ) ) );
		//tConfInfo.SetMediaKey( tMediaEncrypt );
		tConfAttrb.SetEncKeyManual(((u8)atoi(json_string_value(pjEncryptManual))) > 0);
	}

	//码流加密模式
	json_t_ptr pjEncryptMode = json_object_get_s(pjRoot, JSON_ENCRYPTMODE);
	if (NULL == pjEncryptMode)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get EncryptMode is NULL!\n", __FUNCTION__);
	}
	else
	{
		tMediaEncrypt.SetEncryptMode((u8)atoi(json_string_value(pjEncryptMode)));
		tConfInfo.SetMediaKey(tMediaEncrypt);		
		tConfAttrb.SetEncryptMode((u8)atoi(json_string_value(pjEncryptMode)));
	}
	

	//码流加密密钥
	json_t_ptr pjEncryptKey = json_object_get_s(pjRoot, JSON_ENCRYPTKEY);
	if (NULL == pjEncryptKey)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get pjEncryptKey is NULL!\n", __FUNCTION__);
	}
	else
	{
		if (CONF_ENCRYPTMODE_NONE != tConfAttrb.GetEncryptMode() &&
			tConfAttrb.IsEncKeyManual())
		{
			u8  m_abyEncKey[MAXLEN_KEY];
			memset(m_abyEncKey, 0, sizeof(m_abyEncKey));
			u8 byLen = strlen(json_string_value(pjEncryptKey));
			if (byLen > MAXLEN_KEY)
			{
				byLen = MAXLEN_KEY;
			}
			memcpy(m_abyEncKey, (u8*)json_string_value(pjEncryptKey), byLen);
			tMediaEncrypt.SetEncryptKey((u8*)&m_abyEncKey[0], MAXLEN_KEY);

			tConfInfo.SetMediaKey(tMediaEncrypt);
		}
	}

	//主席
	json_t_ptr pjChairMan = json_object_get_s(pjRoot, JSON_CHAIRMANALIAS);
	if (NULL == pjChairMan)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get ChairMan is NULL!\n", __FUNCTION__);
	}
	else
	{
		tMtAlias.SetNull();
		tMtAlias.SetE164Alias(json_string_value(pjChairMan));
		tConfInfo.SetChairAlias(tMtAlias);
		tConfInfo.m_tStatus.m_tChairman.SetNull();
	}

	//发言人
	json_t_ptr pjSpeaker = json_object_get_s(pjRoot, JSON_SPEAKERALIAS);
	if (NULL == pjSpeaker)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get Speaker is NULL!\n", __FUNCTION__);
	}
	else
	{
		tMtAlias.SetNull();
		tMtAlias.SetE164Alias(json_string_value(pjSpeaker));
		tConfInfo.SetSpeakerAlias(tMtAlias);
		tConfInfo.m_tStatus.m_tSpeaker.SetNull();
	}

	//混音模式
	//此处与jsontranslate.h中的getconfinfo不同, 其不能设置tConfInfo.m_tStatus.mixmode
	json_t_ptr pjMixParam = json_object_get_s(pjRoot, JSON_MIXPARAM);
	if (NULL == pjMixParam)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get MixParam is NULL!\n", __FUNCTION__);
	}
	else
	{
		json_t_ptr pjMixMode = json_object_get_s(pjMixParam, JSON_MIXMODE);
		if (pjMixMode != NULL)
		{
			BOOL32 bIsDisccussConf = (u8)atoi(json_string_value(pjMixMode)) == 0 ? FALSE : TRUE;
			tConfAttrb.SetDiscussConf(bIsDisccussConf);
			tConfInfo.m_tStatus.SetMixerMode((u8)atoi(json_string_value(pjMixMode)));
		}
		else
		{
			OspPrintf(TRUE, FALSE, "[%s] json_object_get MixMode is NULL!\n", __FUNCTION__);
		}
	}

	// 双流模式 DualMode
	json_t_ptr pjDualMode = json_object_get_s(pjRoot, JSON_DUALMODE);
	if (NULL == pjDualMode)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get DualMode is NULL!\n", __FUNCTION__);
	}
	else
	{
		tConfAttrb.SetDualMode((u8)atoi(json_string_value(pjDualMode)));
	}

		//是否公共会议
	json_t_ptr pjPublicConf = json_object_get_s(pjRoot, JSON_PUBLISHCONF);
	if (NULL == pjPublicConf)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get PublicConf is NULL!\n", __FUNCTION__);
	}
	else
	{
		tConfAttrb.SetPublicConf((u8)atoi(json_string_value(pjPublicConf)));
	}

	//会议开放属性
	json_t_ptr pjOpenMode = json_object_get_s(pjRoot, JSON_OPENMODE);
	if (NULL == pjOpenMode)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get OpenMode is NULL!\n", __FUNCTION__);
	}
	else
	{
		u8 byCMMode = 0;
		switch ((u8)atoi(json_string_value(pjOpenMode)))
		{
		case 0:
			byCMMode = CONF_OPENMODE_CLOSED;
			break;
		case 1:
			byCMMode = CONF_OPENMODE_OPEN;
			break;
		default:
			byCMMode = CONF_OPENMODE_OPEN;
			break;
		}
		tConfAttrb.SetOpenMode(byCMMode);
	}

	//会议是否封闭会议 0-非封闭，1-封闭(终端免打扰)
	json_t_ptr pjClosed = json_object_get_s(pjRoot, JSON_NODISTURB);
	if (NULL == pjClosed)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get Closed is NULL!\n", __FUNCTION__);
	}
	else
	{
		tConfAttrb.SetConfNoDisturb((u8)atoi(json_string_value(pjClosed)) == 1 ? TRUE : FALSE);
	}

	//是否有XMPU服务器
	json_t_ptr pjXmpuser = json_object_get_s(pjRoot, JSON_HASXMPUSER);
	if (NULL == pjXmpuser)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get Xmpuser is NULL!\n", __FUNCTION__);
	}
	else
	{
		BOOL32 bIsHasXmpuSer = (u8)atoi(json_string_value(pjXmpuser)) == 0 ? FALSE : TRUE;
		tConfAttrb.SetHasXmpuSer(bIsHasXmpuSer);
	}

	//是否有画面合成模板
	json_t_ptr pjVmpModule = json_object_get_s(pjRoot, JSON_VMPMODULE);
	if (NULL == pjVmpModule)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get VmpModule is NULL!\n", __FUNCTION__);
	}
	else
	{
		BOOL32 bIsHasVmpModule = (u8)atoi(json_string_value(pjVmpModule)) == 0 ? FALSE : TRUE;
		tConfAttrb.SetHasVmpModule(bIsHasVmpModule);
	}

	//域Moid
	json_t_ptr pjDomainMoid = json_object_get_s(pjRoot, JSON_MOID);
	if (NULL == pjDomainMoid)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get DomainMoid is NULL!\n", __FUNCTION__);
	}
	else
	{
		tConfInfo.SetDomainMoid(json_string_value(pjDomainMoid));
	}

	//域name
	json_t_ptr pjDomainName = json_object_get_s(pjRoot, JSON_DOMAINNAME);
	if (NULL == pjDomainName)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get DomainName is NULL!\n", __FUNCTION__);
	}
	else
	{
		tConfInfo.SetDomainName(json_string_value(pjDomainName));
	}
	//会议开始时间
	json_t_ptr pjStartTime = json_object_get_s(pjRoot, JSON_CONFSTARTTIME);
	if (pjStartTime == NULL)
	{
		OspPrintf(TRUE, FALSE, "[%s] json_object_get StartTime is NULL!\n", __FUNCTION__);
	}
	else
	{
		const char* pStartTime = json_string_value(pjStartTime);
		if (pjStartTime != NULL)
		{
			struct tm tmInfo;
			time_t ti;
			sscanf(pStartTime, "%d-%d-%d %d:%d:%d",
				&(tmInfo.tm_year), &(tmInfo.tm_mon), &(tmInfo.tm_mday),
				&(tmInfo.tm_hour), &(tmInfo.tm_min), &(tmInfo.tm_sec));
			if (tmInfo.tm_year > 1900 && tmInfo.tm_mon > 0)
			{
				tmInfo.tm_year -= 1900;
				tmInfo.tm_mon--;
				tmInfo.tm_isdst = -1;
				ti = mktime(&tmInfo);
				tConfInfo.SetStartTime(ti);
			}
			else
			{
				OspPrintf(TRUE, FALSE, "[%s] json_object_get StartTime is error, tm_year: %d tm_mon: %d!\n",
					__FUNCTION__, tmInfo.tm_year, tmInfo.tm_mon);
			}
		}
	}

	// 最大入会终端数量
	// 没有则默认设置为192
	u32 dwMaxJoinedMt = MAXNUM_CONF_MT;
	GetStringAsUIntFromJson(pjRoot, JSON_MAXJOINEDMT, dwMaxJoinedMt);
	// 设置最大入会终端数量
	tConfAttrb.SetMaxJoinedMtNum(dwMaxJoinedMt);
	
	// CapSupport CapSupportEx
	tCapSupport.SetDStreamType(VIDEO_DSTREAM_H264_H239);
	tCapSupport.SetDStreamMediaType(MEDIA_TYPE_H264);
	tCapSupport.SetDStreamResolution(VIDEO_FORMAT_CIF);			//双流分辨率
	tCapSupport.SetDStreamFrmRate(VIDEO_FPS_2997_1);			//帧率
	//tMain.SetAudioMediaType(MEDIA_TYPE_G7221C);				//音频类型
	tMain.SetVideoFrameRate(VIDEO_FPS_2997_1);					//帧率
	tCapSupport.SetMainSimCapSet(tMain);
	tCapSupport.SetSecondSimCapSet(tSecond);
	//tCapSupportEx.SetMainAudioSubType(MEDIA_TYPE_G7221C_SUBTYPE_24K);		//音频子模式  todo
	tConfInfo.SetCapSupport(tCapSupport);
	//	tConfInfo.SetCapSupportEx( tCapSupportEx );   todo

	tConfInfo.SetConfAttrb(tConfAttrb);
	
	return TRUE;
}

BOOL32 CheckConfNumIsReachLimit(const u32 dwMaxJoinedMt, 
	const u32 dwSmallConfNum, const u32 dwMaxSmallConfNum,
	const u32 dwLargeConfNum, const u32 dwMaxLargeConfNum, 
	BOOL32& bReach)
{
	bReach = FALSE;
	if ((dwSmallConfNum + dwLargeConfNum) >= (dwMaxSmallConfNum + dwMaxLargeConfNum))
	{
		bReach = TRUE;
	}
	else if (!IsSmallScaleConf(dwMaxJoinedMt) && dwLargeConfNum >= dwMaxLargeConfNum)
	{
		bReach = TRUE;
	}
	return TRUE;
}

//const u8 TranslatePollMode(const u8 byMode)
//{
//	//#define		CONF_POLLMODE_VIDEO              1  //仅图像轮询广播 到本地
//	//#define		CONF_POLLMODE_SPEAKER            2  //轮流发言       到本地
//	//#define		CONF_POLLMODE_VIDEO_SPY          3  //仅图像轮询广播 到回传
//	//#define		CONF_POLLMODE_SPEAKER_SPY        4  //轮流发言       到回传
//	//#define		CONF_POLLMODE_VIDEO_BOTH         5  //仅图像轮询广播 到本地和回传
//	//#define		CONF_POLLMODE_SPEAKER_BOTH       6  //轮流发言       到本地和回传
//	//#define     CONF_POLLMODE_VIDEO_CHAIRMAN     7  //只带视频的“主席轮询选看”轮询模式
//	//#define     CONF_POLLMODE_BOTH_CHAIRMAN      8  //带音频的“主席轮询选看”轮询模式
//	//轮询模式  0-不轮询广播  1-仅图像本地轮询广播  
//	//3-本地轮流发言  6-级联轮询视频  7-级联轮询发言
//	//8-本地+级联轮询视频   9-本地+级联轮询发言   
//	//10-主席仅视频轮询  11-主席音视频轮询
//	u8 byPollMode = byMode;
//	if (byPollMode == CONF_POLLMODE_VIDEO)
//	{
//		byPollMode = 1;
//	}
//	else if (byPollMode == CONF_POLLMODE_SPEAKER)
//	{
//		byPollMode = 3;
//	}
//	else if (byPollMode == CONF_POLLMODE_VIDEO_SPY)
//	{
//		byPollMode = 6;
//	}
//	else if (byPollMode == CONF_POLLMODE_SPEAKER_SPY)
//	{
//		byPollMode = 7;
//	}
//	else if (byPollMode == CONF_POLLMODE_VIDEO_BOTH)
//	{
//		byPollMode = 8;
//	}
//	else if (byPollMode == CONF_POLLMODE_SPEAKER_BOTH)
//	{
//		byPollMode = 9;
//	}
//	else if (byPollMode == CONF_POLLMODE_VIDEO_CHAIRMAN)
//	{
//		byPollMode = 10;
//	}
//	else if (byPollMode == CONF_POLLMODE_BOTH_CHAIRMAN)
//	{
//		byPollMode = 11;
//	}
//	else
//	{
//		byPollMode = 1;
//	}
//	return byPollMode;
//}

const BOOL32 IsMt(const u16 wMsgType)
{
	return (wMsgType != CM_MAU_CREATECONF_REQ
		&& wMsgType != CM_MAU_CREATECONFBYTEMP_REQ);
}

BOOL32 CheckPassword(const s8* pchReqPassword, const s8* pchBookConfPassword)
{
	//终端通过预约会议创会，如果有密码，校验密码
	//第二个判断用于兼容不发密码的老终端，让它直接创会
	if (0 != strlen(pchBookConfPassword) && 0 != strlen(pchReqPassword))
	{
		LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[%s]confPsw = %s, mtPsw = %s.\n",
			pchBookConfPassword, pchReqPassword);

		if (0 != strncmp(pchBookConfPassword, pchReqPassword, MAXLEN_PWD))
		{
			return FALSE;
		}
		else
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]create conf psw correct.\n", __FUNCTION__);
		}
	}
	else
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]conf psw is empty.pass.\n", __FUNCTION__);
	}
	return TRUE;
}

BOOL32 CheckPublicTempPassword(const s8* pchReqPassword, const s8* pchTempPwd)
{
	//终端通过模板创会，如果模板有密码，校验密码
	//第二个判断用于兼容不发密码的老终端，让它直接创会
	if (0 != strlen(pchTempPwd) && 0 != strlen(pchReqPassword))
	{
		// 终端发Null密码的md5为：d41d8cd98f00b204e9800998ecf8427e
		if (0 == strcmp("d41d8cd98f00b204e9800998ecf8427e", pchReqPassword))
		{
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[%s]mt send"
				" psw =d41d8cd98f00b204e9800998ecf8427e.pass.\n", __FUNCTION__);
		}
		else
		{
			LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[%s]confPsw = %s, mtPsw = %s.\n", 
				__FUNCTION__, pchTempPwd, pchReqPassword);

			if (0 != strncmp(pchTempPwd, pchReqPassword, MAXLEN_PWD))
			{
				LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[%s]psw error\n", __FUNCTION__);
				return FALSE;
			}
			else
			{
				LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[%s]create conf"
					" psw correct.\n", __FUNCTION__);
			}
		}
	}
	else
	{
		LogPrint( LOG_LVL_DETAIL, MID_MAU_VC, "[%s]conf psw is empty.pass.\n", __FUNCTION__ );
	}
	return TRUE;
}

const u8 GetDBConfScale(const u32 dwMaxJoinedMt)
{
	//v_meeting, confscale, 会议规模 0：空，1：4方视频会议,2：8方视频会议,3：大型视频会议 
	if (dwMaxJoinedMt <= 8)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

BOOL32 IsSmallScaleConf(const u32 dwMaxJoinedMt)
{
	return (dwMaxJoinedMt <= 8);
}

BOOL32 CheckQComsumerStatus(const s8* pchJsonBuf, const u32 dwJsonLen)
{
	static time_t timeRec = 0;
	if (pchJsonBuf != NULL && strncmp(pchJsonBuf, "testQ", 5) == 0)
	{
		string strCurTime(pchJsonBuf, dwJsonLen);
		strCurTime = strCurTime.substr(5, strCurTime.length() - 5);
		time_t aTime = (time_t)strtoul(strCurTime.c_str(), 0, 10);
		if ((aTime - timeRec) == 1)
		{
			OspSemGive(g_hTestQConsumerSem);
		}
		timeRec = aTime;
		return TRUE;
	}
	return FALSE;
}

BOOL32 IsCreateConfTimeOut(const s8* pchCreateTime)
{
	time_t tCreateTime = 0;
	FormatStr2KdvTime(pchCreateTime).GetTime(tCreateTime);
	return IsCreateConfTimeOut(tCreateTime);
}

BOOL32 IsCreateConfTimeOut(const time_t createTime)
{
	const time_t curTime = time(NULL);
	return (curTime > createTime && (curTime - createTime) > CREATECONF_TIMEOUT_SPAN);
}

BOOL32 IsConfInCmuConfList(const s8* pchConfE164,
	const TMpcConfData* paCmuConf,
	const u32 dwConfNum,
	BOOL32& bExist)
{
	if (pchConfE164 == NULL || strlen(pchConfE164) == 0
		|| paCmuConf == NULL)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_PUBLIC, "[%s]pchConfE164<%s> == NULL or strlen == 0"
			"or paCmuConf<%s> == NULL\n", __FUNCTION__, pchConfE164 == NULL ? "NULL" : pchConfE164,
			paCmuConf == NULL ? "NULL" : "notNULL");
		return FALSE;
	}

	u32 dwConfIndex = 0;
	for (; dwConfIndex < dwConfNum; dwConfIndex++)
	{
		if (strncmp(paCmuConf[dwConfIndex].GetConfIdAttr().GetConfE164(),
			pchConfE164, MAXLEN_CONFE164) == 0)
		{
			bExist = TRUE;
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_PUBLIC, "[%s]Cmu has Conf<%s>\n",
				__FUNCTION__, paCmuConf[dwConfIndex].GetConfIdAttr().GetConfE164());
			return TRUE;
		}
	}
	bExist = FALSE;
	return TRUE;
}

BOOL32 IsRedisHasConfExData(const TRedisConfInfo& tRedisConf)
{
	return strlen(tRedisConf.GetConfE164()) != 0;
}

BOOL32 IsExceptionBookConf(const s8* pchBookConfStartTime)
{
	//如果预约会议开始时间小于当前时间超过1小时，则认为是异常的预约会议
	const time_t tCurTime = time(NULL);
	time_t tBookConfStartTime = 0;
	FormatStr2KdvTime(pchBookConfStartTime).GetTime(tBookConfStartTime);
	return (tCurTime > tBookConfStartTime) && ((tCurTime - tBookConfStartTime) > 60 * 60);
}

BOOL32 ConstructGetRecoverConfNeedInfoToJson(const s8* pchConfE164, const BOOL32 bHasXmpu, s8* pchJsonBuf, u32& dwJsonBufLen)
{
	json_t_ptr pjRoot = NULL;
	ReturnIfFailed(LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE));
	ReturnIfFailed(SetStringToJson(pjRoot, JSON_CONFE164,  pchConfE164));
	ReturnIfFailed(SetBoolAsStringToJson(pjRoot, JSON_HASXMPUSER,  bHasXmpu));
	return DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen);
}

BOOL32 GetRecoverConfNeedInfoFromJson(const string& strJson, string& strConfE164, BOOL32& bHasXmpu)
{
	json_t_ptr pjRoot;
	ReturnIfFailed(LoadJson(strJson.c_str(), strJson.length(), pjRoot, FALSE));
	const s8* pchConfE164 = NULL;
	ReturnIfFailed(GetStringFromJson(pjRoot, JSON_CONFE164, &pchConfE164));
	strConfE164 = pchConfE164;
	u32 dwHasXmpu = 0;
	ReturnIfFailed(GetStringAsUIntFromJson(pjRoot, JSON_HASXMPUSER, dwHasXmpu));
	bHasXmpu = dwHasXmpu == 0 ? FALSE : TRUE;
	return TRUE;
}

BOOL32 GetConfInfoExFromConfInfo(const TConfInfo& tConfInfo, TConfInfoEx& tConfInfoEx)
{
	TSimCapSet tMainSim = tConfInfo.GetCapSupport().GetMainSimCapSet();
	TDStreamCap tDSSim = tConfInfo.GetCapSupport().GetDStreamCapSet();
	TVideoStreamCap tMainCap = tMainSim.GetVideoCap();
	TVideoStreamCap tDsCap = tDSSim.GetVideoStremCap();
	TAudioTypeDesc tAudioType;
	tAudioType.SetAudioMediaType(MEDIA_TYPE_G7221C);
	tAudioType.SetAudioTrackNum(1);
	u8 byCapNum = 1;
	tConfInfoEx.SetMainStreamCapEx(&tMainCap, byCapNum);
	tConfInfoEx.SetDoubleStreamCapEx(&tDsCap, byCapNum);
	tConfInfoEx.SetAudioTypeDesc(&tAudioType, byCapNum);
	return TRUE;
}

BOOL32 TransMiniMixParamToMixModule(const TMiniMixParam& tMiniMixParam, TMixModule& tMixModule)
{
	u8 byMixMode = tMiniMixParam.GetMixMode();
	tMixModule.SetModuleMixMode(byMixMode);
	if (byMixMode != mcuNoMix)
	{
		for (s32 nIdex = 0; nIdex < MAXNUM_MIXING_MEMBER; nIdex++)
		{
			if (tMiniMixParam.GetMixMemIdx(nIdex) == 0)
			{
				continue;
			}
			tMixModule.SetMixMtIdx(nIdex, tMiniMixParam.GetMixMemIdx(nIdex));
		}
	}
	tMixModule.SetVACPeriodTime(tMiniMixParam.GetVacinterval());
	return TRUE;
}

BOOL32 TransMiniVmpParamToVmpModule(const TMiniVmpParam& tMiniVmpParam, TVmpModuleInfo& tVmpModuleInfo)
{
	//首先判断是否存在vmp 不存在的话按默认值来设
	TVMPParam tVmpParam;
	u8 byVmpMode = tMiniVmpParam.GetVMPMode();
	if (byVmpMode != 0)
	{
		tVmpParam.SetVMPAuto(tMiniVmpParam.GetVMPMode() == 2 ? TRUE : FALSE);
		tVmpParam.SetVMPStyle(tMiniVmpParam.GetVMPStyle());
		tVmpParam.SetVMPMode(tMiniVmpParam.GetVMPMode());
		tVmpParam.SetIsRimEnabled(tMiniVmpParam.GetIsRimEnabled());
		tVmpParam.SetVMPBrdst(tMiniVmpParam.IsVMPBrdst());
		tVmpParam.SetVoiceHint(tMiniVmpParam.GetIsVoiceHintEnable());
		tVmpParam.SetIsAddMmbAlias(tMiniVmpParam.IsAddMmbAlias());
		tVmpParam.SetVMPBatchPoll(tMiniVmpParam.IsVMPBatchPoll());
		tVmpParam.SetVMPBatchPollKeepTime(tMiniVmpParam.GetVMPBatchPollKeepTime());
		tVmpParam.SetVMPBatchPollPollNum(tMiniVmpParam.GetVMPBatchPollPollNum());
		TVMPMember tVmpMember;
		for (s32 nIdx = 0; nIdx < MAXNUM_VMP_MEMBER; nIdx++)
		{
			const TMiniVmpMember *ptMiniVmpMember = tMiniVmpParam.GetVmpMember(nIdx);
			if (ptMiniVmpMember != NULL)
			{
				if (ptMiniVmpMember->GetMemberType() == VMP_MEMBERTYPE_NULL)
				{
					continue;
				}

				tVmpMember.Clear();
				tVmpMember.SetChnIdx(ptMiniVmpMember->GetMemberIdx());
				tVmpMember.SetMemberType(ptMiniVmpMember->GetMemberType());
				tVmpMember.SetMemStatus(ptMiniVmpMember->GetMemStatus());

				tVmpParam.SetVmpMember(nIdx, tVmpMember);
			}
		}
	}
	tVmpModuleInfo.SetVmpParam(tVmpParam);
	return TRUE;
}

BOOL32 GetMtListFromMiniConfInfo(const TMiniConfInfo& tMiniConfInfo, const TMtAlias& tCreatorMt, TMtAlias* atMtList, u32& nMtNum)
{
	s32 nIndex = 0;
	BOOL32 bCreatorMtInMtList = FALSE;
	for (; nIndex < MAXNUM_CONF_MT; nIndex++)
	{
		if (!tMiniConfInfo.GetMtListMem(nIndex)->IsNull())
		{
			atMtList[nMtNum++] = *tMiniConfInfo.GetMtListMem(nIndex);
			if (*tMiniConfInfo.GetMtListMem(nIndex) == tCreatorMt)
			{
				bCreatorMtInMtList = TRUE;
			}
			atMtList[nMtNum - 1].Print();
		}
		else
		{
			break;
		}
	}
	if (!bCreatorMtInMtList)
	{
		atMtList[nMtNum++] = tCreatorMt;
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]creator isn't in mtList, add it:\n",
			__FUNCTION__);
		atMtList[nMtNum - 1].Print();
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "THE NUM OF MTLIST IS %d\n", nMtNum);
	return TRUE;
}
