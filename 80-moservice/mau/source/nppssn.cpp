#include "inc.h"
#include "nppssn.h"
#include "mauvc.h"
#include "eventnumau.h"
#include "nppmaucommon.h"
#include "jsontranslate.h"
#include "kdv323common.h"
#include <bitset>

using namespace rabbitmqwrap;
extern CMauVCApp g_cMauVCApp;

s32 CNppSsnData::s_nNppSsnRabbitMq = 0;
static s8  g_achRpc[MAXLEN_ALIAS + 1] = { 0 };
static s8  g_achRpcId[MAXLEN_ALIAS + 1] = { 0 };

CNppSsnInstance::CNppSsnInstance() {}

CNppSsnInstance::~CNppSsnInstance() {}

#define SetMaxLicenseConfNumToJsonObj(pjRoot, dwMaxLicenseConfNum) SetUIntAsStringToJson(pjRoot, REDIS_MAXLICENSEDCONFNUM, dwMaxLicenseConfNum)
#define SetLicenseConfNumToJsonObj(pjRoot, dwLicenseConfNum) SetUIntAsStringToJson(pjRoot, REDIS_LICENSEDCONFNUM, dwLicenseConfNum)
#define SetIsLastMsgToJsonObj(pjRoot, bLastMsg) SetUIntAsStringToJson(pjRoot, JSON_ISLASTMSG, bLastMsg)
#define GetIsGetAllConfFromJsonObj(pjRoot, bGetAll) GetStringAsBoolFromJson(pjRoot, JSON_ALLCONFLIST, bGetAll)

void CNppSsnInstance::DaemonProcPowerOn(const CMessage *pcMsg, CApp *pcApp)
{
	CNppSsnApp* pcCMMauApp = (CNppSsnApp*)pcApp;
	//状态初始化
	NextState(STATE_IDLE);
	pcCMMauApp->Init_MQPC();
}

void CNppSsnInstance::DaemonInstanceEntry(CMessage * const pcMsg, CApp *pcApp)
{
	if (NULL == pcMsg || NULL == pcApp)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[CNppSsnInstance::DaemonInstanceEntry]Param is NULL!\n");
		return;
	}
	switch (pcMsg->event)
	{
	case OSP_POWERON:
		DaemonProcPowerOn(pcMsg, pcApp);
		break;
	case NGI_NPP_MESSAGE_CB:
		DaemonProcNgiToNppMsg(pcMsg);
		break;
	case MAU_NU_CREATECONF_ACK:
	case MAU_NU_CREATECONF_NACK:
	case MAU_NU_JOINCONF_ACK:
	case MAU_NU_JOINCONF_NACK:
		DaemonProcMauToMtRsp(pcMsg);
		break;
	case MAU_NU_CREATECONFBYTEMP_ACK:
	case MAU_NU_CREATECONFBYTEMP_NACK:
		DaemonProcCreateConfByTemRsp(pcMsg);
		break;
	default:
		break;
	}

	return;
}

void CNppSsnInstance::InstanceEntry(CMessage * const pcMsg)
{
	return;
}

BOOL32 CNppSsnData::Init_MQPC()
{
	//创建生产者
	s_nNppSsnRabbitMq = SafeCreateP(Producer_ConSuc_MQCB, Producer_ConFail_MQCB);
	if (0 == s_nNppSsnRabbitMq)
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[CNppSsnData::Init_MQPC] s_nNppSsnRabbitMq = 0, return FALSE!\n");
	}

	//创建消费者
	SafeCreateC(MQ_NPP_NGINPP_Q, MQ_NPP_NGINPP_EX, MQ_NPP_NGINPP_K,
		ContentBody_MQCB, ContenHeaderProps_MQCB, NULL, Consumer_ConSuc_MQCB, Consumer_ConFail_MQCB,
		0, 0, 0, 1, g_cMauVCApp.GetMqConsumerHeartBeatTime());

	return TRUE;
}

void CNppSsnData::ContentBody_MQCB(amqp_connection_state_t conn, void *pbuf, size_t len, response_type *prsp_type, char* pchTag)
{
	if (NULL == pbuf)
	{
		*prsp_type = RT_NACK;
		LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[ContentBody_MQCB]Buf is NULL, return!\n");
		return;
	}

	*prsp_type = RT_ACK;

	OspPost(MAKEIID(AID_MAU_NPPSSN_APPID, CInstance::DAEMON), NGI_NPP_MESSAGE_CB, pbuf, len);
}
void CNppSsnData::ContenHeaderProps_MQCB(amqp_connection_state_t conn, void *correlation_id, size_t idLen, void *replyTo, size_t replyToLen, char* pchTag)
{
	if (NULL == correlation_id || NULL == replyTo)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[CNppSsnData::ContenHeaderProps_MQCB]correlation_id or rpcto is NULL, return!\n");
		return;
	}

	s32 nRpcIdLen = (MAXLEN_ALIAS > idLen) ? idLen : MAXLEN_ALIAS;
	memset(g_achRpcId, 0, sizeof(g_achRpcId));
	strncpy(g_achRpcId, (s8*)correlation_id, nRpcIdLen);

	s32 nRpcLen = (MAXLEN_ALIAS > replyToLen) ? replyToLen : MAXLEN_ALIAS;
	memset(g_achRpc, 0, sizeof(g_achRpc));
	strncpy(g_achRpc, (s8*)replyTo, nRpcLen);

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[CNppSsnData::ContenHeaderProps_MQCB]Replay rpc id is %s, content is %s!\n", g_achRpcId, g_achRpc);

	return;
}

void CNppSsnData::Producer_ConSuc_MQCB(amqp_connection_state_t conn, char *desc)
{
	g_cMauVCApp.GetMqStatus().SetNppNgiPIsCreated(TRUE);

	s8 achSucDec[MAXLEN_ALIAS] = { 0 };
	if (NULL != desc)
	{
		strncpy(achSucDec, desc, sizeof(achSucDec) - 1);
	}

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[CNppSsnData::Producer_ConSuc_MQCB]%s.\n", achSucDec);

	return;
}

void CNppSsnData::Producer_ConFail_MQCB(amqp_connection_state_t conn, const char *expect, const char *precv)
{
	g_cMauVCApp.GetMqStatus().SetNppNgiPIsCreated(FALSE);

	s8 achExpect[MAXLEN_ALIAS] = { 0 };
	s8 achRecv[MAXLEN_ALIAS] = { 0 };
	if (NULL != expect)
	{
		strncpy(achExpect, expect, sizeof(achExpect) - 1);
	}
	if (NULL != precv)
	{
		strncpy(achRecv, precv, sizeof(achRecv) - 1);
	}
	LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[CNppSsnData::Producer_ConFail_MQCB]expect.%s.\n", achExpect);
	LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[CNppSsnData::Producer_ConFail_MQCB]precv.%s.\n", achRecv);
	return;
}

void CNppSsnData::Consumer_ConSuc_MQCB(amqp_connection_state_t conn, char *desc, char* pchTag)
{
	g_cMauVCApp.GetMqStatus().SetNpp_NgiNppQIsCreated(TRUE);

	s8 achSucDec[MAXLEN_ALIAS] = { 0 };
	if (NULL != desc)
	{
		strncpy(achSucDec, desc, sizeof(achSucDec) - 1);
	}

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[CNppSsnData::Consumer_ConSuc_MQCB]%s.\n", achSucDec);
	return;
}

void CNppSsnData::Consumer_ConFail_MQCB(amqp_connection_state_t conn, const char *expect, const char *precv, char* pchTag)
{
	g_cMauVCApp.GetMqStatus().SetNpp_NgiNppQIsCreated(FALSE);

	s8 achExpect[MAXLEN_ALIAS] = { 0 };
	s8 achRecv[MAXLEN_ALIAS] = { 0 };
	if (NULL != expect)
	{
		strncpy(achExpect, expect, sizeof(achExpect) - 1);
	}
	if (NULL != precv)
	{
		strncpy(achRecv, precv, sizeof(achRecv) - 1);
	}

	LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[CNppSsnData::Consumer_ConFail_MQCB]expect.%s.\n", achExpect);
	LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[CNppSsnData::Consumer_ConFail_MQCB]precv.%s.\n", achRecv);
	return;
}

void CNppSsnInstance::DaemonProcNgiToNppMsg(CMessage * const pcMsg)
{
	const s8* pchJsonBuf = (const s8*)pcMsg->content;
	u32 dwJsonLen = pcMsg->length;

	json_t_ptr pjMsg = NULL;
	LoadJson(pchJsonBuf, dwJsonLen, pjMsg, FALSE);
	if (pjMsg == NULL) return;
	
	const s8* pchMsgType = "";
	GetMsgTypeFromJsonObj(pjMsg, &pchMsgType);

	u16 wMsgType = ConvertMsgTypeToEventId(pchMsgType);
	if (wMsgType == 0)
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_NPP, "[DaemonProcNgiToNppMsg]Unknown Message %s", pchMsgType);
		return;
	}

	TNppReqHeadInfo tNppReqHead;
	if (FALSE == GetNppReqHeadFromJson(pjMsg, tNppReqHead))
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_NPP, "[DaemonProcNgiToNppMsg]Can't Read NppReqHead!");
		return;
	}

	switch (wMsgType)
	{
	case NU_MAU_CREATECONF_REQ:
		DaemonProcMtCreateConfReq(wMsgType, pjMsg, tNppReqHead);
		break;
	case NU_MAU_JOINCONF_REQ:
		DaemonProcMtJoinConfReq(wMsgType, pjMsg, tNppReqHead);
		break;
	case NU_MAU_GETCONFDETAIL_REQ:
		DaemonProcMtGetConfDetailReq(wMsgType, pjMsg, tNppReqHead);
		break;
	case NU_MAU_GETCONFNUM_REQ:
		DaemonProcMtGetConfNumReq(wMsgType, pjMsg, tNppReqHead);
		break;
	case NU_MAU_GETCONFLIST_REQ:
	case NU_MAU_GETBOOKLIST_REQ:
	case NU_MAU_GETTEMPLATELIST_REQ:
		DaemonProcMtGetConfListReq(wMsgType, pjMsg, tNppReqHead);
		break;
	case NU_MAU_CREATECONFBYTEMP_REQ:
		DaemonProcCreateConfByTemReq(wMsgType, pjMsg, tNppReqHead);
		break;
	default:
		break;
	}
}

u16 CNppSsnInstance::ConvertMsgTypeToEventId(const s8 * pchMsgType)
{
	u16 wMsgType = 0;

	if (NULL == pchMsgType)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[CNppSsnInstance::ConvertMsgTypeToEventId]Param is NULL, return!\n");
		return wMsgType;
	}

#define MSG2EVENT(event) if (0 == strcmp( #event, pchMsgType)) { wMsgType = event;}

	//终端召集会议请求
	MSG2EVENT(NU_MAU_CREATECONF_REQ);
	//终端加入会议
	MSG2EVENT(NU_MAU_JOINCONF_REQ);
	//级联呼叫个人模板
	MSG2EVENT(NU_MAU_CREATECONFBYTEMP_REQ);
	//终端获取会议详情
	MSG2EVENT(NU_MAU_GETCONFDETAIL_REQ);
	//终端获取预约会议列表
	MSG2EVENT(NU_MAU_GETBOOKLIST_REQ);
	//终端获取模板列表
	MSG2EVENT(NU_MAU_GETTEMPLATELIST_REQ);
	//终端获取会议列表
	MSG2EVENT(NU_MAU_GETCONFLIST_REQ);
	//终端获取会议数
	MSG2EVENT(NU_MAU_GETCONFNUM_REQ);

	return wMsgType;
}

BOOL32 CNppSsnInstance::CheckAndGetMtDomain(const s8* pchMtE164,
	CMauMtInfo& cMtInfo, CKdvDomainInfo& cDomainInfo)
{
	bitset<5> errFlagBitset;
	if ((errFlagBitset[0] = !g_cMauVCApp.IsMtExist(pchMtE164))
		|| (errFlagBitset[1] = !g_cMauVCApp.GetMtInfo(pchMtE164, cMtInfo))
		|| (errFlagBitset[2] = !g_cMauVCApp.IsDomainExist(cMtInfo.GetDomainMOID()))
		|| (errFlagBitset[3] = !g_cMauVCApp.GetDomainInfo(cMtInfo.GetDomainMOID(), cDomainInfo))
		|| (errFlagBitset[4] = !cDomainInfo.IsUsedFlag()))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]Err<%s>, Because:\n"
			"0:mt<%s> not exist\n"
			"1.get mtinfo fail\n"
			"2.domain<%s> not exist\n"
			"3.get domain info fail\n"
			"4.domain is unused\n",
			__FUNCTION__, errFlagBitset.to_string().c_str(),
			pchMtE164, cDomainInfo.GetDomainGUID());
		return FALSE;
	}
	return TRUE;
}

void CNppSsnInstance::DaemonProcMtCreateConfReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NPP);

	TReqHeadInfo tReqInfo;
	CMauMtInfo cMtInfo;
	CKdvDomainInfo cDomainInfo;
	if (!CheckAndGetMtDomain(tNppReqInfo.GetSrcMtE164(), cMtInfo, cDomainInfo))
	{
		const u32 dwErrCode = (strlen(cMtInfo.GetDomainMOID()) == 0 ? ERR_MT_NOEXIST : ERR_DOMAIN_INVALID);
		tNppReqInfo.SetErrID(dwErrCode);
		SetNppReqHeadToJsonObj(pjMsg, tNppReqInfo);
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_CREATECONF_NACK);
		json_str pchRspMsg(pjMsg);
		SendMsgToNu(pchRspMsg, strlen(pchRspMsg), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
		LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[%s]get mt or domain or add req fail\n", __FUNCTION__);
		return;
	}
	TConfInfo tConfInfo;
	GetConfInfoFromJson(pjMsg, tConfInfo);

	SetDomainMoidToJsonObj(pjMsg, cDomainInfo.GetDomainMOID());
	SetDomainNameToJsonObj(pjMsg, cDomainInfo.GetWholeName());
	UpdateCreateConfParam(cMtInfo, cDomainInfo, pjMsg);
	CompletedReqInfo(tReqInfo, tNppReqInfo, cMtInfo, wMsgType);
	SetReqHeadToJsonObj(pjMsg, tReqInfo);

	json_str pchRspMsg(pjMsg);

	CMauMsg cServMsg;
	cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
	cServMsg.CatMsgBody(pchRspMsg, strlen(pchRspMsg));

	CMauData::SendMsgToMauVC(wMsgType, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[%s]mt<E164.%s VerId.%s"
		" ProductId.%s, protocotype: %s>"
		" confname<%s> bitrate<%d> Frm_old<main.%d sec.%d> "
		" Frm<main.%d sec.%d> require create conf, send require<ID:%d> "
		" to mauvc.\n", __FUNCTION__,
		tReqInfo.GetSrcMtE164(), tReqInfo.GetVersionId(),
		tReqInfo.GetProductId(), tReqInfo.GetProctocolType(),
		tConfInfo.GetConfName(),
		tConfInfo.GetBitRate(),
		tConfInfo.GetCapSupport().GetMainVidFrmRate(),
		tConfInfo.GetCapSupport().GetSecVidFrmRate(),
		tConfInfo.GetCapSupport().GetMainVidFrmRate(),
		tConfInfo.GetCapSupport().GetSecVidFrmRate(),
		tReqInfo.GetUniqueID());
}

void CNppSsnInstance::CompletedReqInfo(TReqHeadInfo& tReqInfo, const TNppReqHeadInfo& tNppReqInfo, const CMauMtInfo& cMtInfo, const u32 dwMsgEvent)
{
	tReqInfo.SetMtReqID(tNppReqInfo.GetMtReqID());
	tReqInfo.SetSrcMtE164(tNppReqInfo.GetSrcMtE164());
	tReqInfo.SetMtAlias(tNppReqInfo.GetMtAlias());
	tReqInfo.SetVersionId(tNppReqInfo.GetVersionId());
	tReqInfo.SetProductId(tNppReqInfo.GetProductId());
	tReqInfo.SetProctocolType(tNppReqInfo.GetProctocolType());
	tReqInfo.SetTimeId(tNppReqInfo.GetTimeId());
	tReqInfo.SetMtMOID(cMtInfo.GetMOID());
	tReqInfo.SetDomainMOID(cMtInfo.GetDomainMOID());
	tReqInfo.SetReqSrcID(MAKEIID(GetAppID(), GetInsID()));
	tReqInfo.SetReqEventID(dwMsgEvent);
	tReqInfo.SetConfCreateBy((u8)CONF_CREATE_MT);
	tReqInfo.SetMQCorrelationId(tNppReqInfo.GetMQCorrelationId());
	tReqInfo.SetMQName(tNppReqInfo.GetMQName());
}

void CNppSsnInstance::UpdateCreateConfParam(const CMauMtInfo& cMtInfo, const CKdvDomainInfo& cDomainInfo, json_t_ptr& pjConfInfo)
{
	//获取终端召集会议所带的会议信息
	//受限账号不允许召集非开放会议, 所以平台侧保护，默认受限账号召集开放会议
	if (cMtInfo.GetLimitedFlag())
	{
		SetUIntAsStringToJson(pjConfInfo, JSON_PUBLISHCONF, 1);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]mt is limit and "
			"conf will be created is not a public conf, default active:"
			" change conf to public conf\n", __FUNCTION__);
	}

	u32 dwMaxJoinedMtNum = 0;
	GetConfMaxJoinedMtNumFromJsonObj(pjConfInfo, dwMaxJoinedMtNum);

	//设置会议最大与会方数默认值
	if (dwMaxJoinedMtNum == 0)
	{
		SetConfMaxJoinedMtNumToJsonObj(pjConfInfo, 8);
	}
}


s32 CNppSsnInstance::SendMsgToNu(const s8 * const pbyMsgBody, u32 dwLen, const s8 * pbyRpcId, const s8 * pbyRpc)
{
	s32 nRet = 0;
	//RPC_MODE
	if (NULL != pbyRpc && NULL != pbyRpcId)
	{
		nRet = PostMsg(CNppSsnData::s_nNppSsnRabbitMq, pbyMsgBody, dwLen, pbyRpc, NULL, 0, RPC_MODE, pbyRpcId, pbyRpc);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[CNppSsnInstance::SendMsgToNu]Rpc<%s> RpcId<%s> Ret.%d\n", pbyRpc, pbyRpcId, nRet);
	}
	else
	{
		nRet = PostMsg(CNppSsnData::s_nNppSsnRabbitMq, pbyMsgBody, dwLen, pbyRpc);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[CNppSsnInstance::SendMsgToNu]Ret.%d\n", nRet);
	}
	return TRUE;
}

void CNppSsnInstance::DaemonProcMtJoinConfReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NPP);

	TReqHeadInfo tReqInfo;
	CMauMtInfo cMtInfo;
	CKdvDomainInfo cDomainInfo;

	if (!CheckAndGetMtDomain(tNppReqInfo.GetSrcMtE164(), cMtInfo, cDomainInfo))
	{
		const u32 dwErrCode = (strlen(cMtInfo.GetDomainMOID()) == 0 ? ERR_MT_NOEXIST : ERR_DOMAIN_INVALID);
		tNppReqInfo.SetErrID(dwErrCode);
		
		SetNppReqHeadToJsonObj(pjMsg, tNppReqInfo);
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_JOINCONF_NACK);
		json_str pchMsgJson(pjMsg);
		SendMsgToNu(pchMsgJson, strlen(pchMsgJson), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
		LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[%s]get mt or domain or "
			"add req fail\n", __FUNCTION__);
		return;
	}

	CompletedReqInfo(tReqInfo, tNppReqInfo, cMtInfo, wMsgType);
	tReqInfo.SetSrcMtE164(tNppReqInfo.GetSrcMtE164());
	tReqInfo.SetConfE164(tNppReqInfo.GetConfE164());
	tReqInfo.SetMtType(tNppReqInfo.GetMtType());
	tReqInfo.SetConfPassword(tNppReqInfo.GetConfPassword());
	tReqInfo.SetMtCallBitrate(tNppReqInfo.GetMtCallBitrate());
	if (MT_TYPE_NONE == tReqInfo.GetMtType())
	{
		tReqInfo.SetMtType(tNppReqInfo.GetMtType());
	}
	else if (MT_TYPE_PHONE == tReqInfo.GetMtType())
	{
		tReqInfo.SetMtAlias(tNppReqInfo.GetMtAlias());
		tReqInfo.SetReqMtAliasType(tNppReqInfo.GetReqMtAliasType());
		LogPrint(LOG_LVL_DETAIL, MID_MAU_NPP, "[%s] phone<%s>\n",
			__FUNCTION__, tReqInfo.GetMtAlias());
	}
	else
	{
		tReqInfo.SetMtAlias(tNppReqInfo.GetMtAlias());
		tReqInfo.SetReqMtAliasType(tNppReqInfo.GetReqMtAliasType());
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[%s]MtReqId<InReqHead.%d InTPasConfMsg.%d>!\n",
		__FUNCTION__, tReqInfo.GetMtReqID(), tNppReqInfo.GetMtReqID());

	CMauMsg cServMsg;
	cServMsg.SetMsgBody((u8*)&tReqInfo, sizeof(tReqInfo));
	CRedisBookConf cBookConf;
	TRedisConfInfo tRedisConf;
	BOOL32 bSucc = g_cMauVCApp.GetConfExInfo(tReqInfo.GetConfE164(), tRedisConf);
	if (bSucc && IsRedisHasConfExData(tRedisConf))
	{
		CMauData::NoticeCmuSsnAddMt(tRedisConf.GetCmuIp(), tReqInfo);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[%s]conf:%s exist, "
			"notice mcu:%s add mt:%s\n", __FUNCTION__,
			tRedisConf.GetConfE164(), tRedisConf.GetCmuIp(), tReqInfo.GetSrcMtE164());
	}
	else if (bSucc
		&& (bSucc = g_cMauVCApp.GetBookConf(tReqInfo.GetConfE164(), cBookConf))
		&& strlen(cBookConf.GetConfE164()) != 0)
	{
		CMauData::SendMsgToMauVC(MAU_MAU_MTCREATECONFBYBOOKCONF_REQ,
			cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[%s]mt:%s join bookconf:%s\n",
			__FUNCTION__, tReqInfo.GetSrcMtE164(), cBookConf.GetConfE164());
	}
	else if(bSucc)
	{
		CMauData::SendMsgToMauVC(MAU_MAU_MTCREATECONFBYPUBLICTEMP_REQ,
			cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[%s]mt:%s join public Template<%s>\n",
			__FUNCTION__, tReqInfo.GetSrcMtE164(), tReqInfo.GetConfE164());
	}
	else
	{
		tNppReqInfo.SetErrID(ERR_DB_OPR);
		SetNppReqHeadToJsonObj(pjMsg, tNppReqInfo);
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_JOINCONF_NACK);
		json_str pchMsgJson(pjMsg);
		SendMsgToNu(pchMsgJson, strlen(pchMsgJson), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]Redis Exception\n", __FUNCTION__);
	}
	LogPrint(LOG_LVL_DETAIL, MID_MAU_NPP, "[%s] mt<type.%u %s VerId.%s ProductId.%s>"
		" callbitrate<%d> require join conf<%s>,  send require<ID:%d> to mpcdvc.\n",
		__FUNCTION__, tReqInfo.GetMtType(), tReqInfo.GetSrcMtE164(),
		tNppReqInfo.GetVersionId(), tNppReqInfo.GetProductId(),
		tReqInfo.GetMtCallBitrate(), tReqInfo.GetConfE164(), tReqInfo.GetUniqueID());
}

void CNppSsnInstance::DaemonProcMtGetConfDetailReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NPP);

	CMauMtInfo cMtInfo;
	CKdvDomainInfo cDomainInfo;
	const s8 *pchConfE164 = tNppReqInfo.GetConfE164();
	const s8* pchE164 = tNppReqInfo.GetSrcMtE164();
	if (!g_cMauVCApp.IsMtExist(pchE164)
		|| !g_cMauVCApp.GetMtInfo(pchE164, cMtInfo)
		|| !g_cMauVCApp.IsDomainExist(cMtInfo.GetDomainMOID())
		|| !g_cMauVCApp.GetDomainInfo(cMtInfo.GetDomainMOID(), cDomainInfo)
		|| !cDomainInfo.IsUsedFlag())
	{
		const u32 dwErrCode = (strlen(cMtInfo.GetDomainMOID()) == 0 ? ERR_MT_NOEXIST : ERR_DOMAIN_INVALID);
		tNppReqInfo.SetErrID(dwErrCode);
		SetNppReqHeadToJsonObj(pjMsg, tNppReqInfo);
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_GETCONFDETAIL_NACK);
		json_str pchRsp(pjMsg);
		SendMsgToNu(pchRsp, strlen(pchRsp), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());

		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s] mt<%s> not exist or mt<%s> "
			" in domain<%s> not exist or is unused or GetConfE164<%s> err, nack it.\n",
			__FUNCTION__, tNppReqInfo.GetSrcMtE164(), cMtInfo.GetE164(),
			cDomainInfo.GetDomainMOID(), pchConfE164);
		return;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[ProcMtGetConfDetailReq] conf e164<%s>, src mt is <%s VerId.%s ProductId.%s>.\n",
		pchConfE164, tNppReqInfo.GetSrcMtE164(), tNppReqInfo.GetVersionId(), tNppReqInfo.GetProductId());

	TConfDetailInfo tConfDetailInfo;
	const u32 dwErrCode = g_cMauVCApp.GetConfDetailInfo(cMtInfo, pchConfE164, tConfDetailInfo);
	if (dwErrCode != ERR_DB_SUCCESS)
	{
		tConfDetailInfo.m_tNewConfBaseInfo.SetConfE164(pchConfE164);
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_GETCONFDETAIL_NACK);
	}
	else
	{
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_GETCONFDETAIL_ACK);
	}
	tNppReqInfo.SetErrID(dwErrCode);
	SetNppReqHeadToJsonObj(pjMsg, tNppReqInfo);
	SetConfDetailInfoToJson(pjMsg, tConfDetailInfo);
	json_str pchRsp(pjMsg);
	SendMsgToNu(pchRsp, strlen(pchRsp), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
}

void CNppSsnInstance::DaemonProcMtGetConfNumReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NPP);

	CMauMtInfo cMtInfo;
	CKdvDomainInfo cDomainInfo;
	const s8* pchMtE164 = tNppReqInfo.GetSrcMtE164();
	if (!g_cMauVCApp.IsMtExist(pchMtE164)
		|| !g_cMauVCApp.GetMtInfo(pchMtE164, cMtInfo)
		|| !g_cMauVCApp.IsDomainExist(cMtInfo.GetDomainMOID())
		|| !g_cMauVCApp.GetDomainInfo(cMtInfo.GetDomainMOID(), cDomainInfo)
		|| !cDomainInfo.IsUsedFlag())
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[DaemonProcMtGetConfNumReq] mt<%s> not exist or mt<%s> in domain<%s> not exist or is unused, nack it.\n",
			tNppReqInfo.GetSrcMtE164(), cMtInfo.GetE164(), cDomainInfo.GetDomainMOID());

		const u32 dwErrCode = (strlen(cMtInfo.GetDomainMOID()) == 0 ? ERR_MT_NOEXIST : ERR_DOMAIN_INVALID);
		tNppReqInfo.SetErrID(dwErrCode);
		SetNppReqHeadToJsonObj(pjMsg, tNppReqInfo);
		SetMsgTypeToJsonObj(pjMsg, EV_NU_MAU_GETCONFNUM_NACK);
		json_str pchMsgJson(pjMsg);
		SendMsgToNu(pchMsgJson, strlen(pchMsgJson), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
		return;
	}

	u32 dwLicensedConfNum = 0;
	u32 dwMaxLicensedConfNum = 0;
	if (g_cMauVCApp.GetConfNumByDomain(cMtInfo.GetDomainMOID(), enumMaxLicensedConfNum, dwMaxLicensedConfNum)
		&& g_cMauVCApp.GetConfNumByDomain(cMtInfo.GetDomainMOID(), enumLicensedConfNum, dwLicensedConfNum))
	{
		tNppReqInfo.SetErrID(ERR_DB_SUCCESS);
		SetNppReqHeadToJsonObj(pjMsg, tNppReqInfo);
		SetMsgTypeToJsonObj(pjMsg, EV_NU_MAU_GETCONFNUM_ACK);
		SetMaxLicenseConfNumToJsonObj(pjMsg, dwMaxLicensedConfNum);
		SetLicenseConfNumToJsonObj(pjMsg, dwLicensedConfNum);
		json_str pchMsgJson(pjMsg);
		SendMsgToNu(pchMsgJson, strlen(pchMsgJson), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[%s]MaxLicensedConfNum: %u, licensedConfNum: %u\n",
			__FUNCTION__, dwMaxLicensedConfNum, dwLicensedConfNum);
	}
	else
	{
		tNppReqInfo.SetErrID(ERR_DB_OPR);
		SetNppReqHeadToJsonObj(pjMsg, tNppReqInfo);
		SetMsgTypeToJsonObj(pjMsg, EV_NU_MAU_GETCONFNUM_NACK);
		json_str pchMsgJson(pjMsg);
		SendMsgToNu(pchMsgJson, strlen(pchMsgJson), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
		LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[%s]GetConfNumErr, domainMoid: %s\n",
			__FUNCTION__, cMtInfo.GetDomainMOID());
	}
}

void CNppSsnInstance::DaemonProcCreateConfByTemReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NPP);

	try {
		const s8* pchConfE164 = tNppReqInfo.GetConfE164();

		string strConfJson;
		//查询个人模板中是否存在该会议
		BOOL32 bFindPT = g_cMauVCApp.FindPersonalTempInfo(pchConfE164, strConfJson);

		if (!bFindPT)
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[DaemonProcCreateConfByTemReq] Personal Template not exist.\n", __FUNCTION__);
			throw KDVP_NSM_MCU_CONFNOTEXIST;
		}
		json_t_ptr pjConfInfo = NULL;
		LoadJson(strConfJson.c_str(), strConfJson.length(), pjConfInfo, FALSE);

		const s8* pchDomainMoid = "";
		GetDomainMoidFromJsonObj(pjConfInfo, &pchDomainMoid);

		CKdvDomainInfo tDomainInfo;
		if (!g_cMauVCApp.GetDomainInfo(pchDomainMoid, tDomainInfo)
			|| !tDomainInfo.IsUsedFlag())
		{
			LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[%s]domain invalid or unused\n", __FUNCTION__);
			throw KDVP_NSM_TIMEOUT_ERR;
		}

		//记录请求的相关信息
		TReqHeadInfo tReqInfo;
		tReqInfo.SetMtReqID(tNppReqInfo.GetMtReqID());
		tReqInfo.SetSrcMtE164(tNppReqInfo.GetSrcMtE164());
		tReqInfo.SetConfE164(pchConfE164);
		tReqInfo.SetMtAlias(tNppReqInfo.GetMtAlias());
		tReqInfo.SetReqSrcID(MAKEIID(GetAppID(), GetInsID()));
		tReqInfo.SetReqEventID(wMsgType);
		tReqInfo.SetConfCreateBy((u8)CONF_CREATE_MT);
		tReqInfo.SetVersionId(tNppReqInfo.GetVersionId());
		tReqInfo.SetProductId(tNppReqInfo.GetProductId());
		tReqInfo.SetProctocolType(tNppReqInfo.GetProctocolType());
		tReqInfo.SetDomainMOID(pchDomainMoid);
		tReqInfo.SetMQCorrelationId(tNppReqInfo.GetMQCorrelationId());
		tReqInfo.SetMQName(tNppReqInfo.GetMQName());

		//级联呼叫个人模板，租赁4小时，自建默认为永久会议
		const u32 dwPersonTemplateDuration = (g_cMauVCApp.GetPlatformIsRent() ? 240 : 0);
		SetConfDurationToJsonObj(pjConfInfo, dwPersonTemplateDuration);

		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[ProcCreatConfByTempReq] mt<InTPasConfMsg.%d  mtE164.%s  VerId.%s  ProductId.%s> require join conf<%s>.\n",
			tNppReqInfo.GetMtReqID(), tNppReqInfo.GetSrcMtE164(), tNppReqInfo.GetVersionId(), tNppReqInfo.GetProductId(), pchConfE164);

		json_str pchConfJson(pjConfInfo);

		//转发给MAUVC  由主控进行处理
		CMauMsg cServMsg;
		cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
		cServMsg.CatMsgBody(pchConfJson, strlen(pchConfJson));
		CMauData::SendMsgToMauVC(wMsgType, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
	}
	catch (u16 wErrorCode)
	{
		SetErrCodeToJsonObj(pjMsg, wErrorCode);
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_CREATECONFBYTEMP_NACK);
		json_str pchResult(pjMsg);
		SendMsgToNu(pchResult, strlen(pchResult), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
	}
}

void CNppSsnInstance::DaemonProcMtGetConfListReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NPP);

	const s8* pchE164 = tNppReqInfo.GetSrcMtE164();
	CMauMtInfo cMtInfo;
	CKdvDomainInfo cDomainInfo;
	vector<TSimpleConfInfo> vecConfList;
	u32 dwErrCode = 0;

	BOOL32 bGetAll = FALSE;
	if (!GetIsGetAllConfFromJsonObj(pjMsg, bGetAll))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[%s]Get IsAllConfList From JSON Failed\n", __FUNCTION__);
		return;
	}

	if (!CheckAndGetMtDomain(pchE164, cMtInfo, cDomainInfo))
	{
		dwErrCode = (strlen(cMtInfo.GetDomainMOID()) == 0 ? ERR_MT_NOEXIST : ERR_DOMAIN_INVALID);
	}
	else if (!GetConfList(wMsgType, cMtInfo.GetDomainMOID(),
		vecConfList, cMtInfo.GetE164(), cMtInfo.GetLimitedFlag(), bGetAll))
	{
		dwErrCode = ERR_DB_OPR;
	}
	if (dwErrCode != 0)
	{
		if (wMsgType == NU_MAU_GETCONFLIST_REQ)
		{
			SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_GETCONFLIST_NACK);
		}
		else if (wMsgType == NU_MAU_GETBOOKLIST_REQ)
		{
			SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_GETBOOKLIST_NACK);
		}
		else if (wMsgType == NU_MAU_GETTEMPLATELIST_REQ)
		{
			SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_GETTEMPLATELIST_NACK);
		}
		tNppReqInfo.SetErrID(dwErrCode);
		SetNppReqHeadToJsonObj(pjMsg, tNppReqInfo);
		json_str pchMsgJson(pjMsg);
		SendMsgToNu(pchMsgJson, strlen(pchMsgJson), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
		LogPrint(LOG_LVL_ERROR, MID_MAU_NPP, "[%s]domain invalid or unused\n", __FUNCTION__);
		return;
	}

	if (wMsgType == NU_MAU_GETCONFLIST_REQ)
	{
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_CONFLIST_NTF);
	}
	else if (wMsgType == NU_MAU_GETBOOKLIST_REQ)
	{
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_BOOKLIST_NTF);
	}
	else if (wMsgType == NU_MAU_GETTEMPLATELIST_REQ)
	{
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_TEMPLATELIST_NTF);
	}
	if (vecConfList.empty())
	{
		SetIsLastMsgToJsonObj(pjMsg, TRUE);
		json_str pchRsp(pjMsg);
		SendMsgToNu(pchRsp, strlen(pchRsp) , tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
		return;
	}

	TSimpleConfInfo paConfList[MAX_CONFNUM_ONEPACK];
	u16 wConfNum = 0;
	vector<TSimpleConfInfo>::iterator confIter = vecConfList.begin();
	while(confIter != vecConfList.end())
	{
		paConfList[wConfNum++] = *confIter++;
		BOOL32 bIsLastPack = (confIter == vecConfList.end());
		BOOL32 bReachMaxConfNum = (wConfNum == MAX_CONFNUM_ONEPACK);
		if (bIsLastPack || bReachMaxConfNum)
		{
			SetIsLastMsgToJsonObj(pjMsg, bIsLastPack);
			SetSimpleConfsToJsonObj(pjMsg, paConfList, wConfNum);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[%s]send %s %u conf to nu\n",
				__FUNCTION__, bIsLastPack ? "last" : "", wConfNum);

			json_str pchRsp(pjMsg);
			SendMsgToNu(pchRsp, strlen(pchRsp), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
			wConfNum = 0;
		}
	}
}

void CNppSsnInstance::DaemonProcMauToMtRsp(CMessage * const pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NPP);

	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof(TReqHeadInfo);

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[DaemonProcMauToMtRsp]MtReqId<InReqHead.%d InTPasConfMsg.%d>!\n", tReqInfo.GetMtReqID(), tReqInfo.GetMtReqID());
	
	json_t_ptr pjMsg = json_object();
	u16 wMsgType = pcMsg->event;
	
	if (wMsgType == MAU_NU_CREATECONF_ACK)
	{
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_CREATECONF_ACK);
	}
	else if (wMsgType == MAU_NU_CREATECONF_NACK)
	{
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_CREATECONF_NACK);
	}
	else if (wMsgType == MAU_NU_JOINCONF_ACK)
	{
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_JOINCONF_ACK);
	}
	else if (wMsgType == MAU_NU_JOINCONF_NACK)
	{
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_JOINCONF_NACK);
	}
	TNppReqHeadInfo tNppReqInfo;
	ConvertMauHeadToNppHead(tReqInfo, tNppReqInfo);
	SetNppReqHeadToJsonObj(pjMsg, tNppReqInfo);

	json_str pchMsgJson(pjMsg);
	SendMsgToNu(pchMsgJson, strlen(pchMsgJson), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
}

void CNppSsnInstance::DaemonProcCreateConfByTemRsp(CMessage * const pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NPP);

	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();

	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof(TReqHeadInfo);

	json_t_ptr pjMsg = json_object();

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NPP, "[DaemonProcCreateConfByTemRsp]MtReqId<InReqHead.%d InTPasConfMsg.%d>!\n", tReqInfo.GetMtReqID(), tReqInfo.GetMtReqID());

	TNppReqHeadInfo tNppReqInfo;
	ConvertMauHeadToNppHead(tReqInfo, tNppReqInfo);
	u16 wMsgType = pcMsg->event;
	if (wMsgType == MAU_NU_CREATECONFBYTEMP_ACK)
	{
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_CREATECONFBYTEMP_ACK);
	}
	else if (wMsgType == MAU_NU_CREATECONFBYTEMP_NACK)
	{
		SetMsgTypeToJsonObj(pjMsg, EV_MAU_NU_CREATECONFBYTEMP_NACK);
	}

	SetNppReqHeadToJsonObj(pjMsg, tNppReqInfo);

	LogPrint(LOG_LVL_DETAIL, MID_MAU_NPP, "[CNUSsnInstance::DaemonProcCreateConfByTemRsp] Send msg.%u<%s> to NU\n", pcMsg->event, OspEventDesc(pcMsg->event));

	json_str pchMsgJson(pjMsg);
	SendMsgToNu(pchMsgJson, strlen(pchMsgJson), tNppReqInfo.GetMQCorrelationId(), tNppReqInfo.GetMQName());
}

BOOL32 CNppSsnInstance::GetConfList(const u16 wMsgEvent, const s8* pchDomainMOID,
	vector<TSimpleConfInfo>& vecConfList, const s8* pchMtE164, const BOOL32 bMtLimit, const BOOL32 bGetAll)
{
	if (wMsgEvent == NU_MAU_GETCONFLIST_REQ || bGetAll)
	{
		vector<TSimpleConfInfo> vecSimpleConfList;
		if (!g_cMauVCApp.GetConfListByDomainAndMt(pchDomainMOID,
			pchMtE164, vecSimpleConfList, bMtLimit))
		{
			return FALSE;
		}
		vecConfList.insert(vecConfList.end(), vecSimpleConfList.begin(),
			vecSimpleConfList.end());
	}
	if (wMsgEvent == NU_MAU_GETBOOKLIST_REQ || bGetAll)
	{
		vector<CRedisBookConf> vecBookConf;
		if (g_cMauVCApp.GetTodayBookConfListByMt(pchDomainMOID, vecBookConf,
			pchMtE164, bMtLimit))
		{
			vector<TSimpleConfInfo> vecSimpleConfList;
			ConvertStruct(vecBookConf, vecSimpleConfList);
			vecConfList.insert(vecConfList.end(), vecSimpleConfList.begin(),
				vecSimpleConfList.end());
		}
		else
		{
			return FALSE;
		}
	}
	if (wMsgEvent == NU_MAU_GETTEMPLATELIST_REQ || bGetAll)
	{
		vector<CTempCacheData> vecTempConfList;
		if (g_cMauVCApp.GetTemplateListByDomain(pchDomainMOID,
			vecTempConfList, bMtLimit))
		{
			vector<TSimpleConfInfo> vecSimpleConfList;
			ConvertStruct(vecTempConfList, vecSimpleConfList);
			vecConfList.insert(vecConfList.end(), vecSimpleConfList.begin(),
				vecSimpleConfList.end());
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}
void CNppSsnInstance::SetConfDetailInfoToJson(json_t_ptr& pjRoot, const TConfDetailInfo& tConfDetailInfo)
{
	if (!pjRoot)
	{
		return;
	}

	s8 achTmpBuf[256] = { 0 };

	SetStringToJson(pjRoot, REDIS_MOID, tConfDetailInfo.m_tNewConfBaseInfo.GetDomainMOID());
	//ConfE164
	SetStringToJson(pjRoot, REDIS_E164, tConfDetailInfo.m_tNewConfBaseInfo.GetConfE164());
	//ConfName
	SetStringToJson(pjRoot, REDIS_CONFNAME, tConfDetailInfo.m_tNewConfBaseInfo.GetConfName());

	SetUIntAsStringToJson(pjRoot, REDIS_ENCRYPTMODE, tConfDetailInfo.GetEncryptMode());

	SetUIntAsStringToJson(pjRoot, REDIS_CONFMODE, tConfDetailInfo.GetConfMode());

	SetUIntAsStringToJson(pjRoot, REDIS_BITRATE, tConfDetailInfo.GetBitRate());

	SetUIntAsStringToJson(pjRoot, REDIS_RESOLUTION, tConfDetailInfo.GetResolution());
	SetUIntAsStringToJson(pjRoot, REDIS_OPENMODE, tConfDetailInfo.GetOpenMode());

	tConfDetailInfo.GetStartTime().GetString(achTmpBuf, 20);
	//会议开始时间
	SetStringToJson(pjRoot, REDIS_CONFSTARTTIME, achTmpBuf);

	SetUIntAsStringToJson(pjRoot, REDIS_CONFDURATION, tConfDetailInfo.GetDuration());
	//CreatorName
	SetStringToJson(pjRoot, REDIS_CREATORNAME, tConfDetailInfo.GetConfCreater());

	SetUIntAsStringToJson(pjRoot, REDIS_DUALMODE, tConfDetailInfo.GetDualMode());

	SetUIntAsStringToJson(pjRoot, REDIS_PUBLISHCONF, tConfDetailInfo.GetPublicConf());

	SetUIntAsStringToJson(pjRoot, REDIS_NEEDPWD, tConfDetailInfo.IsNeedPsw());

	SetUIntAsStringToJson(pjRoot, REDIS_CONFTYPE, tConfDetailInfo.IsPortModeConf());

	SetUIntAsStringToJson(pjRoot, REDIS_NODISTURB, tConfDetailInfo.IsCloseMode());

	SetUIntAsStringToJson(pjRoot, REDIS_MAXJOINMTNUM, tConfDetailInfo.GetMaxJoinMtNum());

	SetUIntAsStringToJson(pjRoot, REDIS_MIXMODE, tConfDetailInfo.GetMixMode());
}

BOOL32 CNppSsnInstance::SetSimpleConfsToJsonObj(json_t_ptr& pjRoot, const TSimpleConfInfo *ptData, u16 wDataNum)
{
	if (NULL == pjRoot || NULL == ptData || 0 == wDataNum)
	{
		OspPrintf(TRUE, FALSE, "[SetSimpleConfInfoToJson] Param is NULL, return!\n");
		return FALSE;
	}

	json_t_ptr pjConfDataArray = json_array();
	if (NULL == pjConfDataArray)
	{

		OspPrintf(TRUE, FALSE, "[SetSimpleConfInfoToJson] json_array pjConfDataArray is NULL, return!\n");
		return FALSE;
	}

	u16 wConfDataIdx = 0;
	for (wConfDataIdx = 0; wConfDataIdx < wDataNum; wConfDataIdx++)
	{
		SetSimpleConfInfoToJsonObj(pjConfDataArray, ptData[wConfDataIdx]);
	}

	json_object_set_s(pjRoot, JSON_SIMPLECONFINFO, pjConfDataArray);

	return TRUE;
}

BOOL32 CNppSsnInstance::SetSimpleConfInfoToJsonObj(json_t_ptr& pjConfArray, const TSimpleConfInfo& tConfData)
{
	json_t_ptr pjConfDataInfo = json_object();
	if (NULL == pjConfDataInfo)
	{
		return FALSE;
	}

	s8 achConfId[MAXLEN_CONFID * 2 + 1] = { 0 };
	tConfData.GetConfId().GetConfIdString(achConfId, sizeof(achConfId));
	SetStringToJson(pjConfDataInfo, JSON_CONFID, achConfId);

	SetStringToJson(pjConfDataInfo, REDIS_E164, tConfData.GetConfE164());

	SetStringToJson(pjConfDataInfo, REDIS_CONFNAME, tConfData.GetConfName());

	SetUIntAsStringToJson(pjConfDataInfo, REDIS_MTNUM, tConfData.GetMtNum());

	SetUIntAsStringToJson(pjConfDataInfo, REDIS_CONFREMAINTIME, tConfData.GetRemainTime());

	//StartTime
	time_t tTime;
	tConfData.GetStartTime().GetTime(tTime);
	SetUIntAsStringToJson(pjConfDataInfo, REDIS_CONFSTARTTIME, (u32)tTime);

	SetUIntAsStringToJson(pjConfDataInfo, REDIS_NEEDPWD, tConfData.IsNeedPsw());

	SetUIntAsStringToJson(pjConfDataInfo, REDIS_CONFMODE, tConfData.GetConfMode());

	SetUIntAsStringToJson(pjConfDataInfo, REDIS_PUBLISHCONF, tConfData.GetPublicConf());

	SetUIntAsStringToJson(pjConfDataInfo, REDIS_OPENMODE, tConfData.GetOpenMode());

	json_array_append_s(pjConfArray, pjConfDataInfo);

	return TRUE;
}

void CNppSsnInstance::ConvertMauHeadToNppHead(const TReqHeadInfo &tReqInfo, TNppReqHeadInfo &tNppReqHeadInfo)
{
	tNppReqHeadInfo.SetReqEventID(tReqInfo.GetReqEventID());
	tNppReqHeadInfo.SetReqTime(tReqInfo.GetReqTime());
	tNppReqHeadInfo.SetMtReqID(tReqInfo.GetMtReqID());
	tNppReqHeadInfo.SetErrID(tReqInfo.GetErrID());
	tNppReqHeadInfo.SetTimeOut(tReqInfo.GetTimeOut());
	tNppReqHeadInfo.SetMtCallBitrate(tReqInfo.GetMtCallBitrate());
	tNppReqHeadInfo.SetMtType(tReqInfo.GetMtType());
	tNppReqHeadInfo.SetReqMtAliasType(tReqInfo.GetReqMtAliasType());
	tNppReqHeadInfo.SetMtAlias(tReqInfo.GetMtAlias());
	tNppReqHeadInfo.SetSrcMtE164(tReqInfo.GetSrcMtE164());
	tNppReqHeadInfo.SetConfE164(tReqInfo.GetConfE164());
	tNppReqHeadInfo.SetConfName(tReqInfo.GetConfName());
	tNppReqHeadInfo.SetConfPassword(tReqInfo.GetConfPassword());
	tNppReqHeadInfo.SetMQCorrelationId(tReqInfo.GetMQCorrelationId());
	tNppReqHeadInfo.SetMQName(tReqInfo.GetMQName());
	tNppReqHeadInfo.SetVersionId(tReqInfo.GetVersionId());
	tNppReqHeadInfo.SetProductId(tReqInfo.GetProductId());
	tNppReqHeadInfo.SetProctocolType(tReqInfo.GetProctocolType());
	tNppReqHeadInfo.SetTimeId(tReqInfo.GetTimeId());
}