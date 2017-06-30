#ifndef	_NPPSSN_INSTANCE_H_
#define	_NPPSSN_INSTANCE_H_

#include "osp.h"
#include "mauutility.h"
#include "pasconst.h"
#include "passtruct.h"
#include "rabbitmqwrapper.h"
#include "evmau.h"

//消息号，字符串
#define EV_MAU_NU_CREATECONF_ACK					"MAU_NU_CREATECONF_ACK"
#define EV_MAU_NU_CREATECONF_NACK					"MAU_NU_CREATECONF_NACK"
#define EV_MAU_NU_JOINCONF_ACK						"MAU_NU_JOINCONF_ACK"
#define EV_MAU_NU_JOINCONF_NACK						"MAU_NU_JOINCONF_NACK"

#define EV_MAU_NU_GETCONFLIST_ACK					"MAU_NU_GETCONFLIST_ACK"
#define EV_MAU_NU_GETCONFLIST_NACK					"MAU_NU_GETCONFLIST_NACK"
#define EV_MAU_NU_CONFLIST_NTF						"MAU_NU_CONFLIST_NTF"

#define EV_MAU_NU_GETTEMPLATELIST_ACK				"MAU_NU_GETTEMPLATELIST_ACK"
#define EV_MAU_NU_GETTEMPLATELIST_NACK				"MAU_NU_GETTEMPLATELIST_NACK"
#define EV_MAU_NU_TEMPLATELIST_NTF					"MAU_NU_TEMPLATELIST_NTF"

#define EV_MAU_NU_GETBOOKLIST_ACK					"MAU_NU_GETBOOKLIST_ACK"
#define EV_MAU_NU_GETBOOKLIST_NACK					"MAU_NU_GETBOOKLIST_NACK"
#define EV_MAU_NU_BOOKLIST_NTF						"MAU_NU_BOOKLIST_NTF"

#define EV_MAU_NU_GETCONFDETAIL_ACK					"MAU_NU_GETCONFDETAIL_ACK"
#define EV_MAU_NU_GETCONFDETAIL_NACK				"MAU_NU_GETCONFDETAIL_NACK"

#define EV_NU_MAU_GETCONFNUM_ACK					"NU_MAU_GETCONFNUM_ACK"
#define EV_NU_MAU_GETCONFNUM_NACK					"NU_MAU_GETCONFNUM_NACK"

#define EV_MAU_NU_CREATECONFBYTEMP_ACK				"MAU_NU_CREATECONFBYTEMP_ACK"
#define EV_MAU_NU_CREATECONFBYTEMP_NACK				"MAU_NU_CREATECONFBYTEMP_NACK"

class CNppSsnData
{
public:
	CNppSsnData()
	{
	}

	virtual ~CNppSsnData()
	{
	}
public:
	BOOL32 Init_MQPC();
	static void ContentBody_MQCB(amqp_connection_state_t conn, void *pbuf, size_t len, response_type *prsp_type, char* pchTag);
	static void ContenHeaderProps_MQCB(amqp_connection_state_t conn, void *correlation_id, size_t idLen, void *replyTo, size_t replyToLen, char* pchTag);
	static void Producer_ConSuc_MQCB(amqp_connection_state_t conn, char *desc);
	static void Producer_ConFail_MQCB(amqp_connection_state_t conn, const char *expect, const char *precv);
	static void Consumer_ConSuc_MQCB(amqp_connection_state_t conn, char *desc, char* pchTag);
	static void Consumer_ConFail_MQCB(amqp_connection_state_t conn, const char *expect, const char *precv, char* pchTag);

public:
	static s32 s_nNppSsnRabbitMq;
};


class CNppSsnInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_NORMAL,
	};
public:
	CNppSsnInstance();
	~CNppSsnInstance();

public:
	void InstanceEntry(CMessage * const pcMsg);
	//守护实例消息处理入口
	void DaemonInstanceEntry(CMessage * const pcMsg, CApp *pcApp);
	//守护实例处理初始化消息
	void DaemonProcPowerOn(const CMessage *pcMsg, CApp *pcApp);

	void DaemonProcMtCreateConfReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo);
	void DaemonProcMtJoinConfReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo);
	void DaemonProcMtGetConfDetailReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo);
	void DaemonProcMtGetConfNumReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo);
	void DaemonProcCreateConfByTemReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo);
	void DaemonProcMtGetConfListReq(u16 wMsgType, json_t_ptr& pjMsg, TNppReqHeadInfo& tNppReqInfo);
	void DaemonProcMauToMtRsp(CMessage * const pcMsg);
	void DaemonProcCreateConfByTemRsp(CMessage * const pcMsg);

	void ConvertMauHeadToNppHead(const TReqHeadInfo &tReqInfo, TNppReqHeadInfo &tNppReqHeadInfo);
	void DaemonProcNgiToNppMsg(CMessage * const pcMsg);
	s32 SendMsgToNu(const s8 * const pbyMsgBody /* = NULL */, u32 dwLen /* = 0 */, const s8 * pbyRpcId /*= NULL*/, const s8 * pbyRpc /* = NULL  */);
	static u16 ConvertMsgTypeToEventId(const s8 * pchMsgType);
	BOOL32 CheckAndGetMtDomain(const s8* pchMtE164, CMauMtInfo& cMtInfo, CKdvDomainInfo& cDomainInfo);
	void CompletedReqInfo(TReqHeadInfo& tReqInfo, const TNppReqHeadInfo& tNppReqInfo, const CMauMtInfo& cMtInfo, const u32 dwMsgEvent);
	void UpdateCreateConfParam(const CMauMtInfo& cMtInfo, const CKdvDomainInfo& cDomainInfo, json_t_ptr& pjConfInfo);
	BOOL32 GetConfList(const u16 wMsgEvent, const s8* pchDomainMOID,
		vector<TSimpleConfInfo>& vecConfList, const s8* pchMtE164,
		const BOOL32 bMtLimit, const BOOL32 bGetAll);

	void SetConfDetailInfoToJson(json_t_ptr& pjRoot, const TConfDetailInfo& tConfDetailInfo);
	BOOL32 SetSimpleConfsToJsonObj(json_t_ptr& pjRoot, const TSimpleConfInfo *ptData, u16 wDataNum);
	BOOL32 SetSimpleConfInfoToJsonObj(json_t_ptr& pjConfArray, const TSimpleConfInfo& tConfData);
};
typedef zTemplate<CNppSsnInstance, MAX_NU_NUM, CNppSsnData, 0 > CNppSsnApp;
extern CNppSsnApp g_cNppSsnApp;
#endif