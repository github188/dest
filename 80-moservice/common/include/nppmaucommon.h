#pragma once
#ifndef _NPPMAUCOMMON_H_
#define _NPPMAUCOMMON_H_
#include "osp.h"
#include "vccommon.h"
#include "jsontranslate.h"
#include "v26struct.h"
#include "cssconvertpb.h"

#ifdef	WIN32
#pragma	pack( push )
#pragma	pack( 1 )
#define	window( x ) x
#endif

//流畅（<=192Kbps）、标清(>192Kbps)、高清（720p/1080p）
#define     FLUENT_BITRATE         (u16)192
#define MQ_NPP_NGINPP_Q				"npp.nginpp.q"
#define MQ_NPP_NGINPP_K				"npp.nginpp.k"
#define MQ_NPP_NGINPP_EX			"npp.nginpp.ex"

#define MQ_NGI_NPPNGI_Q				"ngi.nppngi.q:"
#define MQ_NGI_NPPNGI_K				"ngi.nppngi.k:"
#define MQ_NGI_NPPNGI_EX			"ngi.nppngi.ex"

#define JSON_NPPREQHEADINFO         "nppreqheadinfo"     //消息头
#define JSON_ALLCONFLIST			"allconflist"		 //是否获取所有会议列表字段
#define JSON_ISLASTMSG				"islastmsg"		     //发往nppclient是否为lastmsg字段
#define JSON_SIMPLECONFINFO         "simpleconfinfo"     //TSimpleConfInfo 会议信息字段

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

#define REDIS_NEEDPWD   "needpwd"
#define REDIS_CONFREMAINTIME "remaintime"
#define REDIS_CONFMODE   "confmode"


struct TNppReqHeadInfo
{
protected:
	u32 m_dwReqEventID;                       //源请求消息号
	TKdvTime m_tReqTime;                      //请求时间
	u32 m_dwMtReqID;                         //终端请求唯一标识     
	u16 m_wErrID;                             //错误码
	u32 m_dwTimeOut;                         //超时时间
	u32 m_dwMtCallBitrate;                     //终端呼叫码率(终端参加会议时使用)
	u8  m_byMtType;                          //终端类型
	u8  m_achReqMtAliasType;                  //请求终端别名类型
	s8  m_achReqMtAlias[MAXLEN_ALIAS + 1];	      //请求终端别名，如果终端类型是电话，则为电话号码
	s8  m_achReqMtE164[MAXLEN_E164 + 1];	      //请求终端e164
	s8  m_achConfE164[MAXLEN_CONFE164 + 1];    //会议E164
	s8  m_achConfName[MAXLEN_CONFNAME + 1];  //会议名   
	s8  m_achPassword[MAXLEN_PWD + 1];        //MD5加过密的

	s8  m_achMQName[MAXLEN_ALIAS + 1];		  // MQ 队列名
	s8  m_achMQCorrelationId[MAXLEN_ALIAS + 1];  // MQ rpc模式消息Id

	s8  m_achProductId[MAXLEN_ALIAS + 1];
	s8  m_achVersionId[MAXLEN_ALIAS + 1];
	s8  m_achProctocolType[MAXLEN_ALIAS * 2];      //标识sip、323终端

	s8  m_achTimeId[64];                                   //timeid
public:
	TNppReqHeadInfo()
	{
		SetNull();
	}
	~TNppReqHeadInfo()
	{
	}

	void SetNull(void)
	{
		m_dwReqEventID = 0;
		m_dwMtCallBitrate = 0;
		m_dwMtReqID = 0;
		m_wErrID = 0;
		m_byMtType = 0;
		m_achReqMtAliasType = 0;
		m_dwTimeOut = 0;
		memset(m_achReqMtE164, 0, sizeof(m_achReqMtE164));
		memset(m_achReqMtAlias, 0, sizeof(m_achReqMtAlias));
		memset(m_achConfE164, 0, sizeof(m_achConfE164));
		memset(m_achPassword, 0, sizeof(m_achPassword));
		memset(m_achMQName, 0, sizeof(m_achMQName));
		memset(m_achConfName, 0, sizeof(m_achConfName));
		memset(m_achMQCorrelationId, 0, sizeof(m_achMQCorrelationId));
		memset(m_achProductId, 0, sizeof(m_achProductId));
		memset(m_achVersionId, 0, sizeof(m_achVersionId));
		memset(m_achProctocolType, 0, sizeof(m_achProctocolType));
		memset(m_achTimeId, 0, sizeof(m_achTimeId));
	}

	void SetReqEventID(u32 dwReqEventID) { m_dwReqEventID = htonl(dwReqEventID); }
	u32  GetReqEventID(void) const { return ntohl(m_dwReqEventID); }

	void SetMtReqID(u32 dwMtReqID) { m_dwMtReqID = htonl(dwMtReqID); }
	u32  GetMtReqID(void) const { return ntohl(m_dwMtReqID); }

	void SetMtCallBitrate(u32 dwMtCallBitrate) { m_dwMtCallBitrate = htonl(dwMtCallBitrate); }
	u32  GetMtCallBitrate(void) const { return ntohl(m_dwMtCallBitrate); }

	void SetErrID(u16 wErrID) { m_wErrID = htons(wErrID); }
	u16  GetErrID(void) const { return ntohs(m_wErrID); }

	void SetTimeOut(u32 dwTimeOut) { m_dwTimeOut = htonl(dwTimeOut); }
	u32  GetTimeOut(void) const { return ntohl(m_dwTimeOut); }

	void SetReqTime(TKdvTime tReqTime) { m_tReqTime = tReqTime; }
	TKdvTime GetReqTime(void) const { return(m_tReqTime); }

	void SetMtType(u8 byMtType) { m_byMtType = byMtType; }
	u8 GetMtType(void) const { return m_byMtType; }

	void SetReqMtAliasType(u8 byReqMtAliasType) { m_achReqMtAliasType = byReqMtAliasType; }
	u8 GetReqMtAliasType(void) const { return m_achReqMtAliasType; }

	void SetSrcMtE164(const s8* pMtE164)
	{
		memset(m_achReqMtE164, 0, sizeof(m_achReqMtE164));
		if (NULL != pMtE164)
		{
			strncpy(m_achReqMtE164, pMtE164, sizeof(m_achReqMtE164) - 1);
			m_achReqMtE164[sizeof(m_achReqMtE164) - 1] = '\0';
		}
		return;
	}
	LPCSTR GetSrcMtE164(void) const { return m_achReqMtE164; }

	void SetMtAlias(const s8* pMtAlias)
	{
		memset(m_achReqMtAlias, 0, sizeof(m_achReqMtAlias));
		if (NULL != pMtAlias)
		{
			strncpy(m_achReqMtAlias, pMtAlias, sizeof(m_achReqMtAlias) - 1);
			m_achReqMtAlias[sizeof(m_achReqMtAlias) - 1] = '\0';
		}
		return;
	}
	LPCSTR GetMtAlias(void) const { return m_achReqMtAlias; }

	void SetProductId(const s8* pchProductId)
	{
		memset(m_achProductId, 0, sizeof(m_achProductId));
		if (NULL != pchProductId)
		{
			strncpy(m_achProductId, pchProductId, sizeof(m_achProductId) - 1);
			m_achProductId[sizeof(m_achProductId) - 1] = '\0';
		}
		return;
	}
	LPCSTR GetProductId(void) const { return m_achProductId; }

	void SetVersionId(const s8* pchVersionId)
	{
		memset(m_achVersionId, 0, sizeof(m_achVersionId));
		if (NULL != pchVersionId)
		{
			strncpy(m_achVersionId, pchVersionId, sizeof(m_achVersionId) - 1);
			m_achVersionId[sizeof(m_achVersionId) - 1] = '\0';
		}
		return;
	}
	LPCSTR GetVersionId(void) const { return m_achVersionId; }

	void SetMQName(const s8* pchMQName)
	{
		memset(m_achMQName, 0, sizeof(m_achMQName));
		if (NULL != pchMQName)
		{
			strncpy(m_achMQName, pchMQName, sizeof(m_achMQName) - 1);
			m_achMQName[sizeof(m_achMQName) - 1] = '\0';
		}
		return;
	}
	LPCSTR GetMQName(void) const { return m_achMQName; }

	void SetMQCorrelationId(const s8* pchMQCorrelationId)
	{
		memset(m_achMQCorrelationId, 0, sizeof(m_achMQCorrelationId));
		if (NULL != pchMQCorrelationId)
		{
			strncpy(m_achMQCorrelationId, pchMQCorrelationId, sizeof(m_achMQCorrelationId) - 1);
			m_achMQCorrelationId[sizeof(m_achMQCorrelationId) - 1] = '\0';
		}
		return;
	}
	LPCSTR GetMQCorrelationId(void) const { return m_achMQCorrelationId; }

	void SetConfE164(const s8* pConfE164)
	{
		memset(m_achConfE164, 0, sizeof(m_achConfE164));
		if (NULL != pConfE164)
		{
			strncpy(m_achConfE164, pConfE164, sizeof(m_achConfE164) - 1);
			m_achConfE164[sizeof(m_achConfE164) - 1] = '\0';
		}
		return;
	}
	LPCSTR GetConfE164(void) const { return m_achConfE164; }

	void SetConfPassword(const s8* pPassword)
	{
		memset(m_achPassword, 0, sizeof(m_achPassword));
		if (NULL != pPassword)
		{
			strncpy(m_achPassword, pPassword, sizeof(m_achPassword) - 1);
			m_achPassword[sizeof(m_achPassword) - 1] = '\0';
		}
		return;
	}
	LPCSTR GetConfPassword(void) const { return m_achPassword; }

	void SetConfName(const s8* pConfName)
	{
		memset(m_achConfName, 0, sizeof(m_achConfName));
		if (NULL != pConfName)
		{
			strncpy(m_achConfName, pConfName, sizeof(m_achConfName) - 1);
			m_achConfName[sizeof(m_achConfName) - 1] = '\0';
		}
		return;
	}
	LPCSTR GetConfName(void) const { return m_achConfName; }

	void SetProctocolType(const s8* pProctocolType)
	{
		memset(m_achProctocolType, 0, sizeof(m_achProctocolType));
		if (NULL != pProctocolType)
		{
			strncpy(m_achProctocolType, pProctocolType, sizeof(m_achProctocolType) - 1);
			m_achProctocolType[sizeof(m_achProctocolType) - 1] = '\0';
		}
		return;
	}
	LPCSTR GetProctocolType(void) const { return m_achProctocolType; }

	void SetTimeId(const s8* pTimeId)
	{
		memset(m_achTimeId, 0, sizeof(m_achTimeId));
		if (NULL != pTimeId)
		{
			strncpy(m_achTimeId, pTimeId, sizeof(m_achTimeId) - 1);
			m_achTimeId[sizeof(m_achTimeId) - 1] = '\0';
		}
		return;
	}
	LPCSTR GetTimeId(void) const { return m_achTimeId; }
	void Print(void)
	{
		OspPrintf(TRUE, FALSE, "MtReqID<%d>\n", GetMtReqID());
		OspPrintf(TRUE, FALSE, "ReqMtE164<%s>\n", GetSrcMtE164());
		OspPrintf(TRUE, FALSE, "MtCallBitrate<%d>\n", GetMtCallBitrate());
		OspPrintf(TRUE, FALSE, "ReqEvent<%d>\n", GetReqEventID());
		OspPrintf(TRUE, FALSE, "Require ConfE164<%s>\n", GetConfE164());
		OspPrintf(TRUE, FALSE, "Require ConfName<%s>\n", GetConfName());
		OspPrintf(TRUE, FALSE, "Require ConfPassword<%s>\n", GetConfPassword());
		OspPrintf(TRUE, FALSE, "Require Time<");
		GetReqTime().Print();
		OspPrintf(TRUE, FALSE, ">\n");
		OspPrintf(TRUE, FALSE, "Require Timeout<%d>\n", GetTimeOut());
		OspPrintf(TRUE, FALSE, "Require ProctocolType<%s>\n", GetProctocolType());
	}
}
#ifndef WIN32
__attribute__((packed))
#endif
;

static u8 GetVideoMode(const u8 byMediaType, const u16 wBitrate, const u8 byResolution)
{
	if (byResolution == VIDEO_FORMAT_HD720)
	{
		return emHD_MODE;
	}
	else if (byResolution == VIDEO_FORMAT_HD1080)
	{
		return emFHD_MODE;
	}
	else if (byResolution == VIDEO_FORMAT_4K)
	{
		return emUHD_MODE;
	}

	if (byMediaType == MEDIA_TYPE_H265 || wBitrate > FLUENT_BITRATE)
	{
		return emSD_MODE;
	}
	else
	{
		return emFLUENT_MODE;
	}
}

static TKdvTime FormatStr2KdvTime(const char * szTime)
{
	struct tm tm1;

	sscanf(szTime, "%4d-%2d-%2d %2d:%2d:%2d",
		&tm1.tm_year,
		&tm1.tm_mon,
		&tm1.tm_mday,
		&tm1.tm_hour,
		&tm1.tm_min,
		&tm1.tm_sec);

	TKdvTime tKdvTime;
	tKdvTime.SetYear(tm1.tm_year);
	tKdvTime.SetMonth(tm1.tm_mon);
	tKdvTime.SetDay(tm1.tm_mday);
	tKdvTime.SetHour(tm1.tm_hour);
	tKdvTime.SetMinute(tm1.tm_min);
	tKdvTime.SetSecond(tm1.tm_sec);

	return tKdvTime;
}
static const u8 TranslatePollMode(const u8 byMode)
{
	//#define		CONF_POLLMODE_VIDEO              1  //仅图像轮询广播 到本地
	//#define		CONF_POLLMODE_SPEAKER            2  //轮流发言       到本地
	//#define		CONF_POLLMODE_VIDEO_SPY          3  //仅图像轮询广播 到回传
	//#define		CONF_POLLMODE_SPEAKER_SPY        4  //轮流发言       到回传
	//#define		CONF_POLLMODE_VIDEO_BOTH         5  //仅图像轮询广播 到本地和回传
	//#define		CONF_POLLMODE_SPEAKER_BOTH       6  //轮流发言       到本地和回传
	//#define     CONF_POLLMODE_VIDEO_CHAIRMAN     7  //只带视频的“主席轮询选看”轮询模式
	//#define     CONF_POLLMODE_BOTH_CHAIRMAN      8  //带音频的“主席轮询选看”轮询模式
	//轮询模式  0-不轮询广播  1-仅图像本地轮询广播  
	//3-本地轮流发言  6-级联轮询视频  7-级联轮询发言
	//8-本地+级联轮询视频   9-本地+级联轮询发言   
	//10-主席仅视频轮询  11-主席音视频轮询
	u8 byPollMode = byMode;
	if (byPollMode == CONF_POLLMODE_VIDEO)
	{
		byPollMode = 1;
	}
	else if (byPollMode == CONF_POLLMODE_SPEAKER)
	{
		byPollMode = 3;
	}
	else if (byPollMode == CONF_POLLMODE_VIDEO_SPY)
	{
		byPollMode = 6;
	}
	else if (byPollMode == CONF_POLLMODE_SPEAKER_SPY)
	{
		byPollMode = 7;
	}
	else if (byPollMode == CONF_POLLMODE_VIDEO_BOTH)
	{
		byPollMode = 8;
	}
	else if (byPollMode == CONF_POLLMODE_SPEAKER_BOTH)
	{
		byPollMode = 9;
	}
	else if (byPollMode == CONF_POLLMODE_VIDEO_CHAIRMAN)
	{
		byPollMode = 10;
	}
	else if (byPollMode == CONF_POLLMODE_BOTH_CHAIRMAN)
	{
		byPollMode = 11;
	}
	else
	{
		byPollMode = 1;
	}
	return byPollMode;
}
static json_t_ptr CreatPollModuleSetJson(const TMiniPollInfo &tPollModule)
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

static BOOL32 SetPollModuleToJson(const json_t_ptr& pjRoot, const TMiniPollInfo & tPollModule)
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
static BOOL32 SetPollModuleToJson(s8 * pchJsonBuf, u32& dwJsonBufLen, const TMiniPollInfo & tPollModule)
{
	json_t_ptr pjRoot = NULL;
	ReturnIfFailed(LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE));
	ReturnIfFailed(SetPollModuleToJson(pjRoot, tPollModule));
	return DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen);
}


static BOOL32 SetVipModuleToJson(const json_t_ptr& pjRoot, const TMiniVipInfo& tVipModule)
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

static BOOL32 SetVipModuleToJson(s8 * pchJsonBuf, u32& dwJsonBufLen, const TMiniVipInfo& tVipModule)
{
	json_t_ptr pjRoot = NULL;
	ReturnIfFailed(LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE));
	ReturnIfFailed(SetVipModuleToJson(pjRoot, tVipModule));
	return DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen);
}

static BOOL32 SetNppReqHeadToJsonObj(const json_t_ptr& pjRoot, TNppReqHeadInfo &tReqHeadInfo)
{
	json_t_ptr pjReqInfo = json_object();
	if (NULL == pjReqInfo)
	{
		OspPrintf(TRUE, FALSE, "[SetReqHeadToJson] json_object pjReqInfo is NULL, return!\n");
		return FALSE;
	}

	SetUIntAsStringToJson(pjReqInfo, JSON_REQHEAD_EVENTID, tReqHeadInfo.GetReqEventID());
	SetUIntAsStringToJson(pjReqInfo, JSON_REQHEAD_MTREQID, tReqHeadInfo.GetMtReqID());
	SetStringToJson(pjReqInfo, JSON_CONFE164, tReqHeadInfo.GetConfE164());
	SetStringToJson(pjReqInfo, JSON_MTNO, tReqHeadInfo.GetSrcMtE164());
	SetStringToJson(pjReqInfo, JSON_MTALIAS, tReqHeadInfo.GetMtAlias());
	SetUIntAsStringToJson(pjReqInfo, JSON_MTTYPE, tReqHeadInfo.GetMtType());
	SetUIntAsStringToJson(pjReqInfo, JSON_BITRATE, tReqHeadInfo.GetMtCallBitrate());
	SetUIntAsStringToJson(pjReqInfo, JSON_ERRCODE, tReqHeadInfo.GetErrID());
	SetStringToJson(pjReqInfo, JSON_CONFNAME, tReqHeadInfo.GetConfName());
	SetStringToJson(pjReqInfo, JSON_REQHEAD_RPC, tReqHeadInfo.GetMQName());
	SetStringToJson(pjReqInfo, JSON_REQHEAD_RPCID, tReqHeadInfo.GetMQCorrelationId());
	SetStringToJson(pjReqInfo, JSON_REQHEAD_PRODUCTID, tReqHeadInfo.GetProductId());
	SetStringToJson(pjReqInfo, JSON_REQHEAD_VERID, tReqHeadInfo.GetVersionId());
	SetStringToJson(pjReqInfo, JSON_PROTOCOL, tReqHeadInfo.GetProctocolType());
	SetStringToJson(pjReqInfo, JSON_TIMEID, tReqHeadInfo.GetTimeId());

	json_object_set_s(pjRoot, JSON_NPPREQHEADINFO, pjReqInfo);
	return TRUE;
}

static BOOL32 GetNppReqHeadFromJson(json_t_ptr pjRoot, TNppReqHeadInfo &tReqHeadInfo)
{
	if (NULL == pjRoot)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_loads err, retur FALSE!\n");
		return FALSE;
	}
	json_t_ptr pjReqHeadInfo = json_object_get_s(pjRoot, JSON_NPPREQHEADINFO);
	if (NULL == pjReqHeadInfo)
	{
		return FALSE;
	}

	json_t_ptr pjConfE164 = json_object_get_s(pjReqHeadInfo, JSON_CONFE164);
	if (NULL == pjConfE164)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get ConfE164 is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetConfE164(json_string_value(pjConfE164));
	}

	json_t_ptr pjMtType = json_object_get_s(pjReqHeadInfo, JSON_MTTYPE);
	if (NULL == pjMtType)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjMtType is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetMtType((u8)atoi(json_string_value(pjMtType)));
	}

	json_t_ptr pjMtNO = json_object_get_s(pjReqHeadInfo, JSON_MTNO);
	if (NULL == pjMtNO)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get MtNO is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetSrcMtE164(json_string_value(pjMtNO));
	}

	json_t_ptr pjMtAlias = json_object_get_s(pjReqHeadInfo, JSON_MTALIAS);
	if (NULL == pjMtAlias)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjMtAlias is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetMtAlias(json_string_value(pjMtAlias));
	}

	json_t_ptr pjEventId = json_object_get_s(pjReqHeadInfo, JSON_REQHEAD_EVENTID);
	if (NULL == pjEventId)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjEventId is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetReqEventID((u32)strtoul(json_string_value(pjEventId), 0, 10));
	}

	json_t_ptr pjErrId = json_object_get_s(pjReqHeadInfo, JSON_ERRCODE);
	if (NULL == pjErrId)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjErrId is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetErrID((u32)strtoul(json_string_value(pjErrId), 0, 10));
	}

	json_t_ptr pjMtReqId = json_object_get_s(pjReqHeadInfo, JSON_REQHEAD_MTREQID);
	if (NULL == pjMtReqId)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjMtReqId is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetMtReqID((u32)strtoul(json_string_value(pjMtReqId), 0, 10));
	}

	json_t_ptr pjMtCallBitrate = json_object_get_s(pjReqHeadInfo, JSON_BITRATE);
	if (NULL == pjMtCallBitrate)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjMtCallBitrate is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetMtCallBitrate((u32)strtoul(json_string_value(pjMtCallBitrate), 0, 10));
	}

	json_t_ptr pjConfName = json_object_get_s(pjReqHeadInfo, JSON_CONFNAME);
	if (NULL == pjConfName)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjConfName is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetConfName(json_string_value(pjConfName));
	}

	json_t_ptr pjRpc = json_object_get_s(pjReqHeadInfo, JSON_REQHEAD_RPC);
	if (NULL == pjRpc)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjRpc is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetMQName(json_string_value(pjRpc));
	}

	json_t_ptr pjRpcId = json_object_get_s(pjReqHeadInfo, JSON_REQHEAD_RPCID);
	if (NULL == pjRpcId)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjRpcId is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetMQCorrelationId(json_string_value(pjRpcId));
	}

	json_t_ptr pjProductId = json_object_get_s(pjReqHeadInfo, JSON_REQHEAD_PRODUCTID);
	if (NULL == pjProductId)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjProductId is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetProductId(json_string_value(pjProductId));
	}

	json_t_ptr pjVersionId = json_object_get_s(pjReqHeadInfo, JSON_REQHEAD_VERID);
	if (NULL == pjVersionId)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjVersionId is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetVersionId(json_string_value(pjVersionId));
	}

	json_t_ptr pjProtocolType = json_object_get_s(pjReqHeadInfo, JSON_PROTOCOL);
	if (NULL == pjProtocolType)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjProctocolType is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetProctocolType(json_string_value(pjProtocolType));
	}

	json_t_ptr pjTimeId = json_object_get_s(pjReqHeadInfo, JSON_TIMEID);
	if (NULL == pjTimeId)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] json_object_get pjTimeId is NULL, retur FALSE!\n");
	}
	else
	{
		tReqHeadInfo.SetTimeId(json_string_value(pjTimeId));
	}

	return TRUE;
}

static BOOL32 GetNppReqHeadFromJson(const s8 * pchJsonBuf, TNppReqHeadInfo &tReqHeadInfo)
{
	if (NULL == pchJsonBuf)
	{
		OspPrintf(TRUE, FALSE, "[GetReqHeadFromJson] Param is NULL, return FLASE!\n");
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads(pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason);

	return GetNppReqHeadFromJson(pjRoot, tReqHeadInfo);
}

static void SetCreateConfParamToJson(json_t_ptr& pjRoot, const TConfInfo& tConfInfo/*, s32 nMeetingId*/)
{
	if (!pjRoot) return;

	s8 achTmpBuf[256] = { 0 };
	u8 byTmp = 0;

	// ConfE164
	SetConfE164ToJsonObj(pjRoot, tConfInfo.GetConfE164());

	// ConfName
	SetConfNameToJsonObj(pjRoot, tConfInfo.GetConfName());

	// CreatorName
	SetCreatorNameToJsonObj(pjRoot, tConfInfo.GetCreaterAlias());

	// 域名
	SetDomainNameToJsonObj(pjRoot, tConfInfo.GetDomainName());

	// 域Moid
	SetDomainMoidToJsonObj(pjRoot, tConfInfo.GetDomainMoid());

	//confid
	s8 achConfId[MAXLEN_CONFID * 2 + 1] = { 0 };
	tConfInfo.GetConfId().GetConfIdString(achConfId, sizeof(achConfId));
	SetStringToJson(pjRoot, JSON_CONFID, achConfId);

	// 是否端口会议
	SetBoolAsStringToJson(pjRoot, JSON_ISPORTCONF, tConfInfo.GetConfAttrb().IsPortMediaConf());

	// 会议密码
	SetStringToJson(pjRoot, JSON_PSW, tConfInfo.GetConfPwd());

	// 码率
	SetUIntAsStringToJson(pjRoot, JSON_BANDWIDTH, tConfInfo.GetBitRate());

	// 分辨率
	SetUIntAsStringToJson(pjRoot, JSON_RES, tConfInfo.GetMainVideoFormat());

	// 会议时长
	SetConfDurationToJsonObj(pjRoot, tConfInfo.GetDuration());

	// 码流加密模式
	SetUIntAsStringToJson(pjRoot, JSON_ENCRYPTMODE, tConfInfo.GetConfAttrb().GetEncryptMode());

	memset(achTmpBuf, 0, sizeof(achTmpBuf));
	s32 nTmpLen = 0;
	tConfInfo.GetMediaKey().GetEncryptKey((u8*)achTmpBuf, &nTmpLen);
	// 自动加密
	SetBoolAsStringToJson(pjRoot, JSON_ENCRYPT_MANUAL, nTmpLen != 0);

	// 码流加密密钥
	SetStringToJson(pjRoot, JSON_ENCRYPTKEY, achTmpBuf);

	// 主席
	SetChairAliasToJsonObj(pjRoot, tConfInfo.GetChairAlias());

	// 发言人
	SetSpeakerAliasToJsonObj(pjRoot, tConfInfo.GetSpeakerAlias());

	// 初始哑音
	SetBoolAsStringToJson(pjRoot, JSON_ALLMUTE, tConfInfo.GetConfAttrb().IsAllInitDumb());

	// 初始静音
	SetBoolAsStringToJson(pjRoot, JSON_ALLSILENCE, tConfInfo.GetConfAttrb().IsAllInitSilence());

	// 双流开启模式
	SetUIntAsStringToJson(pjRoot, JSON_DUALMODE, tConfInfo.GetConfAttrb().GetDualMode());

	//发言人的源
	SetUIntAsStringToJson(pjRoot, JSON_SPEAKERSRC, tConfInfo.GetConfAttrb().GetSpeakerSrc());

	// 呼叫模式,默认定时呼叫
	SetUIntAsStringToJson(pjRoot, JSON_CALLMODE, 2);

	// 呼叫间隔
	SetUIntAsStringToJson(pjRoot, JSON_CALLINTERVAL, tConfInfo.m_tStatus.GetCallInterval());

	// 呼叫次数
	SetUIntAsStringToJson(pjRoot, JSON_CALLTIMES, tConfInfo.m_tStatus.GetCallTimes());

	// 是否卫星会议
	SetBoolAsStringToJson(pjRoot, JSON_SAT, tConfInfo.GetConfAttrb().IsSatDCastMode());

	// 是否公开会议
	SetUIntAsStringToJson(pjRoot, JSON_PUBLISHCONF, tConfInfo.GetConfAttrb().GetPublicConf());
	// 会议开放属性
	switch (tConfInfo.GetConfAttrb().GetOpenMode())
	{
	case CONF_OPENMODE_CLOSED:
		byTmp = 0;
		break;
	case CONF_OPENMODE_OPEN:
		byTmp = 1;
		break;
	default:
		byTmp = 1;
		break;
	}
	SetUIntAsStringToJson(pjRoot, JSON_OPENMODE, byTmp);
	// 终端免打扰
	SetBoolAsStringToJson(pjRoot, JSON_NODISTURB, tConfInfo.GetConfAttrb().IsConfNoDisturb());
	// 视频质量
	SetBoolAsStringToJson(pjRoot, JSON_VIDEOPRIOR, tConfInfo.GetConfAttrb().IsQualityPri());
	// 是否有画面合成模板
	SetBoolAsStringToJson(pjRoot, JSON_VMPMODULE, tConfInfo.GetConfAttrb().IsHasVmpModule());
	// 是否有轮询模板
	SetBoolAsStringToJson(pjRoot, JSON_POLLMODULE, tConfInfo.GetConfAttrb().IsHasPollModule());
	// 是否有混音模板
	SetBoolAsStringToJson(pjRoot, JSON_MIXMODULE, tConfInfo.GetConfAttrb().IsHasMixModule());
	// 是否有电视墙模板
	SetBoolAsStringToJson(pjRoot, JSON_HDUMODULE, tConfInfo.GetConfAttrb().IsHasTvWallModule());

	// 是否部署了xmpu服务器
	SetHasXmpuSerToJsonObj(pjRoot, tConfInfo.GetConfAttrb().IsHasXmpuSer());

	memset(achTmpBuf, 0, sizeof(achTmpBuf));
	tConfInfo.GetKdvStartTime().GetString(achTmpBuf, 20);
	// 会议开始时间
	SetStringToJson(pjRoot, JSON_CONFSTARTTIME, achTmpBuf);

	memset(achTmpBuf, 0, sizeof(achTmpBuf));
	time_t dwEndtime = (u32)(tConfInfo.GetStartTime() + tConfInfo.GetDuration() * 60);
	TKdvTime tEndTime;
	tEndTime.SetTime(&dwEndtime);
	tEndTime.GetString(achTmpBuf, sizeof(achTmpBuf));
	// 会议结束时间
	SetStringToJson(pjRoot, JSON_CONFENDTIME, achTmpBuf);

	// 最大终端数限制
	SetConfMaxJoinedMtNumToJsonObj(pjRoot, tConfInfo.GetMaxJoinedMtNum());

	// 归一重整
	SetBoolAsStringToJson(pjRoot, JSON_ONEREFORMING, tConfInfo.GetConfAttrb().IsAdjustUniformPack());

	// 级联信息
	TCascadeSetToCM tCascadeSet;
	tCascadeSet.m_byCascadeMode = tConfInfo.GetConfAttrb().IsSupportCascade() ? 1 : 0;
	tCascadeSet.m_byIsUpload = tConfInfo.GetConfAttrb().IsConfDisPlaySpySrc() ? 1 : 0;
	tCascadeSet.m_byIsSpy = tConfInfo.GetConfAttrb().IsSupportMultiSpy() ? 1 : 0;
	tCascadeSet.m_dwSpyBW = tConfInfo.GetConfAttrb().GetSndSpyBandWidth();
	json_t_ptr pjCascadeInfo = CreatCascadeSetJson(tCascadeSet);
	if (NULL != pjCascadeInfo)
	{
		json_object_set_s(pjRoot, JSON_CASCADECONF, pjCascadeInfo);
	}

	//vmp信息
// 	TVMPParam tVmpParam = tConfInfo.m_tStatus.GetVmpParam();
// 	json_t_ptrpjVmpParam = CreatVmpParamSetJson(tVmpParam);
// 	if (pjVmpParam != NULL)
// 	{
// 		json_object_set_new( pjRoot, JSON_VMPPARAM, pjVmpParam );
// 	}
	//mix信息
// 	TMixParam tMixParam = tConfInfo.m_tStatus.GetMixerParam();
// 	json_t_ptrpjMixParam = CreatMixParamSetJson(tMixParam);
// 	if (pjMixParam != NULL)
// 	{
// 		json_object_set_new( pjRoot, JSON_MIXPARAM, pjMixParam );
// 	}
}

static void SetCreateConfParamToJson(s8 * pchJsonBuf, u32 &dwJsonBufLen, const TConfInfo& tConfInfo/*, s32 nMeetingId*/)
{
	json_t_ptr pjRoot = NULL;
	if (!LoadJson(pchJsonBuf, dwJsonBufLen, pjRoot, TRUE)) return;

	SetCreateConfParamToJson(pjRoot, tConfInfo);

	DumpJson(pjRoot, pchJsonBuf, MAXLEN_JSONBUF, dwJsonBufLen);
}

#ifdef WIN32
#pragma	pack( pop )
#endif
#endif/*_NPPMAUCOMMON_H_*/