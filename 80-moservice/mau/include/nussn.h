/*****************************************************************************
模块名      : MAU
文件名      : nussn.h
创建时间    : 2014年 08月 26日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifndef	_NUSSN_INSTANCE_H_
#define	_NUSSN_INSTANCE_H_

#include "osp.h"
#include "mauutility.h"
#include "mauconst.h"
#include "pasconst.h"
#include "passtruct.h"

class CNUSsnInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_NORMAL,
	};

	enum EmMtVer
	{
		emMtUnknown = 0,
		emMtV25,
		emTTV26,
		emMtV26,
		emMtV50
	};
	enum EmSipMt
	{
		SIP_MT_CREATECONF_REQ = 103,
		SIP_MT_E164_NTF,
		SIP_MT_CREATECONF_RSP,
		SIP_MT_JOINCONF_REQ = 108,
		SIP_MT_JOINCONF_RSP,
		SIP_MT_GETCONFNUM_REQ = 110,
		SIP_MT_GETCONFNUM_RSP,
		SIP_MT_GETBOOKLIST_REQ,
		SIP_MT_GETBOOKLIST_RSP,
		SIP_MT_GETCONFDETAIL_REQ = 114,
		SIP_MT_GETCONFDETAIL_RSP,
		SIP_MT_GETCONFLIST_REQ,
		SIP_MT_GETCONFLIST_RSP,
		SIP_MT_GETTEMPLIST_REQ,
		SIP_MT_GETTEMPLIST_RSP,

	};
	
public:
	CNUSsnInstance();
	~CNUSsnInstance();
	
public:
	//守护实例消息处理入口
	void DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp );
	//守护实例处理初始化消息
	void DaemonProcPowerOn( const CMessage *   pcMsg );
	//守护实例处理NU注册请求
	void DaemonProcNuRegReq( CMessage * const pcMsg, CApp *pcApp );
	//守护实例处理打印NU信息请求
	void DaemonProcPrintCmd( CMessage * const pcMsg, CApp *pcApp);

	void InstanceEntry(  CMessage * const pcMsg );
	//处理Nu断链请求
	void ProcNuDisconnect( CMessage * const pcMsg );
	//处理Nu注册请求
	void ProcNuRegReq(const u32 dwNodeId, const u32 dwSrcId, const u16 eventId);

	//sip终端消息
	void ProcSipMtMsg(CMessage * pcMsg);
	void RegDealFun();

	//获取该终端对应的会议个数和空闲会议个数
	void ProcMtGetConfNumReq(CMessage * const pcMsg);
	void ProcMtGetConfNumReq(TPasConfMsg& tPasMsg, const u16 wMsgEvent);

	//获取会议详细信息
	void ProcMtGetConfDetailReq( CMessage * const pcMsg );
	void ProcMtGetConfDetailReq(TPasConfMsg& tPasMsg, const u16 wMsgEvent);

	//获取会议列表
	void ProcMtGetConfListReq(CMessage * const pcMsg);
	void ProcMtGetConfListReq(TPasConfMsg& tPasMsg, const u16 wMsgEvent);

	//终端加入已召开会议/通过模板、预约会议创会
	void ProcMtJoinConfReq( CMessage * const pcMsg );
	void ProcMtJoinConf(TPasConfMsg& tPasMsg, const u16 wMsgEvent);

	//处理终端召集会议请求
	void ProcMtCreateConfReq(CMessage * const pcMsg);
	void ProcMtCreateConf(TPasConfMsg& tPasMsg, const u16 wMsgEvent);

	//终端通过个人模板创会
	void ProcCreatConfByTempReq( CMessage * const pcMsg );
	//终端通过个人模板创会回复
	void ProcCreatConfByTempRsp( CMessage * const pcMsg);
	//MT请求MPCD答复消息处理
	void ProcMauToMtRsp( CMessage * const pcMsg );

protected:
	static void InitMtVerMap();
	static void RegMtVer(EmMtVer emMtVer, const s8* pchProductId, const s8* pchVersionId);
	static EmMtVer FindMtVer(const s8* pchProductId, const s8* pchVersionId);
	static json_t_ptr SetMtCreateConfInfoToJson(const CKdvDomainInfo& tDomainInfo,
		const TReqHeadInfo& tHeadInfo, const TCreateConfReq& tCreateConfReq);
	BOOL32 CheckAndGetMtDomain(const s8* pchMtE164,
		CMauMtInfo& cMtInfo, CKdvDomainInfo& cDomainInfo);
	//对自动分辨率进行处理，并做一些限制（适用于企业终端）
	void UpdateRealResolution(TCreateConfReq &tCreateConfInfo);
	void UpdateCreateConfParam(const CMauMtInfo& cMtInfo,
		const CKdvDomainInfo& cDomainInfo,
		TCreateConfReq &tCreateConfInfo);
	void CompletedReqInfo(TReqHeadInfo& tReqInfo,
		const TPasConfMsg& tPasConfMsg,
		const CMauMtInfo& cMtInfo,
		const s8* pchTimeId,
		const u32 dwMsgEvent);
	//向Nu发送消息
	void SendMsgToNu( u16 wEvent, u8 * const pbyMsg = NULL, u16 wLen = 0 );
	//清空实例的相关信息
	void ClearInst( void );
	u32 GetMsgType(const u32 dwSipMsgType);

public:
	u32	m_dwNuNode;
	u32 m_dwNuIId;
	u32 m_dwNuIp;
public:
	static void SetAutoMixAndVmp(TMiniConfInfo& tMiniConfInfo);
	static EmMtVer GetMtVer(const s8* pchProductId, const s8* pchVersionId);

	typedef void (CNUSsnInstance::*DealFun)(TPasConfMsg& tPasConfMsg, const u16 wMsgEvent);
	typedef std::map<u16, DealFun> DealFunMap;
	static DealFunMap s_deal_sipmtfun_map;

	typedef std::map<std::pair<std::string, std::string>, EmMtVer> MtVerMap;
	static MtVerMap s_mapMtVer;
};

typedef zTemplate<CNUSsnInstance, MAX_NU_NUM> CNUSsnApp;

//-----mt struct----------------
class CBaseMt
{
public:
	virtual BOOL32 GetCreateConfInfo(TPasConfMsg& tPasConfMsg,
		TCreateConfReq& tCreateConfInfo, s8* pchTimeId) = 0;
	virtual BOOL32 GetJoinedConfInfo(TPasConfMsg& tPasConfMsg,
		TJoinConfReq& tJoinConfReq, s8* pchTimeId) = 0;
	virtual BOOL32 GetTimeId(TPasConfMsg& tPasConfMsg, s8* pchTimeId);
	virtual BOOL32 ConstructRspMsg(const u16 wMsgEvent, const s8 *pchConfE164,
		const s8 *pTimeId, u16 wErrId, TPasConfMsg& tPasConfMsg) = 0;
	virtual BOOL32 ConstructRspMsg(const u16 wMsgEvent, const TSimpleConfInfo* paConfList,
		const u16 wConfNum, const BOOL32 bLast,
		TPasConfMsg& tPasConfMsg, const s8* pchTimeId = "") = 0;
	virtual BOOL32 ConstructRspMsg(TConfDetailInfo& tConfDetailInfo, const u32 dwErrCode,
		TPasConfMsg& tPasConfMsg, const s8* pchTimeID = "") = 0;
	virtual BOOL32 ConstructRspMsg(const u32 dwErrCode, const u16 wTotalConfNum, const u16 wIdleConfNum, TPasConfMsg& tPasConfMsg) = 0;
	virtual BOOL32 GetConfList(const u16 wMsgEvent, const s8* pchDoaminMOID,
		std::vector<TSimpleConfInfo>& vecConfList, const s8* pchMtE164 = "", 
		const BOOL32 bMtLimit = FALSE, const BOOL32 bGetAll = FALSE);
	virtual BOOL32 GetConfE164(TPasConfMsg& tPasConfMsg, s8* pchConfE164) = 0;
	//将cmu错误id转成终端错误id
	virtual u32 TransErrorId(u32 dwCmuErrorId);
};

class CSipMt : public CBaseMt
{
public:
	virtual BOOL32 GetCreateConfInfo(TPasConfMsg& tPasConfMsg, TCreateConfReq& tCreateConfInfo, s8* pchTimeId);
	virtual BOOL32 GetJoinedConfInfo(TPasConfMsg& tPasConfMsg, TJoinConfReq& tJoinConfReq, s8* pchTimeId);
	virtual BOOL32 GetConfE164(TPasConfMsg& tPasConfMsg, s8* pchConfE164);
	virtual BOOL32 ConstructRspMsg(const u16 wMsgEvent, const s8 *pchConfE164,
		const s8 *pTimeId, u16 wErrId, TPasConfMsg& tPasConfMsg);
	virtual BOOL32 ConstructRspMsg(const u16 wMsgEvent, const TSimpleConfInfo* paConfList, 
		const u16 wConfNum, const BOOL32 bLast,
		TPasConfMsg& tPasConfMsg, const s8* pchTimeId = "");
	virtual BOOL32 ConstructRspMsg(TConfDetailInfo& tConfDetailInfo, const u32 dwErrCode,
		TPasConfMsg& tPasConfMsg, const s8* pchTimeID = "");
	virtual BOOL32 ConstructRspMsg(const u32 dwErrCode, const u16 wTotalConfNum, const u16 wIdleConfNum, TPasConfMsg& tPasConfMsg);
 
};

class CH323Mt : public CBaseMt
{
public:
	virtual BOOL32 GetCreateConfInfo(TPasConfMsg& tPasConfMsg, TCreateConfReq& tCreateConfInfo, s8* pchTimeId);
	virtual BOOL32 GetJoinedConfInfo(TPasConfMsg& tPasConfMsg, TJoinConfReq& tJoinConfReq, s8* pchTimeId);
	virtual BOOL32 GetConfE164(TPasConfMsg& tPasConfMsg, s8* pchConfE164);
	virtual BOOL32 ConstructRspMsg(const u16 wMsgEvent, const s8 *pchConfE164,
		const s8 *pTimeId, u16 wErrId, TPasConfMsg& tPasConfMsg);
	virtual BOOL32 ConstructRspMsg(const u16 wMsgEvent, const TSimpleConfInfo* paConfList, 
		const u16 wConfNum, const BOOL32 bLast,
		TPasConfMsg& tPasConfMsg, const s8* pchTimeId = "");
	virtual BOOL32 ConstructRspMsg(TConfDetailInfo& tConfDetailInfo, const u32 dwErrCode,
		TPasConfMsg& tPasConfMsg, const s8* pchTimeID = "");
	virtual BOOL32 ConstructRspMsg(const u32 dwErrCode, const u16 wTotalConfNum, const u16 wIdleConfNum, TPasConfMsg& tPasConfMsg);
};

//老终端公共处理类
class COldMt : public CBaseMt
{
public:
	virtual CNUSsnInstance::EmMtVer GetMtVer() const = 0;
	virtual BOOL32 GetCreateConfInfo(TPasConfMsg& tPasConfMsg,
		TCreateConfReq& tCreateConfInfo, s8* pchTimeId);
	virtual BOOL32 GetJoinedConfInfo(TPasConfMsg& tPasConfMsg,
		TJoinConfReq& tJoinConfReq, s8* pchTimeId);
	virtual BOOL32 GetConfE164(TPasConfMsg& tPasConfMsg, s8* pchConfE164);
	virtual BOOL32 ConstructRspMsg(const u16 wMsgEvent, const s8 *pchConfE164,
		const s8 *pTimeId, u16 wErrId, TPasConfMsg& tPasConfMsg);
	virtual BOOL32 ConstructRspMsg(const u16 wMsgEvent, const TSimpleConfInfo* paConfList, 
		const u16 wConfNum, const BOOL32 bLast,
		TPasConfMsg& tPasConfMsg, const s8* pchTimeId = "");
	virtual BOOL32 ConstructRspMsg(TConfDetailInfo& tConfDetailInfo, const u32 dwErrCode,
		TPasConfMsg& tPasConfMsg, const s8* pchTimeID = "");
	virtual BOOL32 ConstructRspMsg(const u32 dwErrCode, const u16 wTotalConfNum, const u16 wIdleConfNum, TPasConfMsg& tPasConfMsg);
	//将cmu错误id转成终端错误id
	u32 TransErrorId(u32 dwCmuErrorId);
};

class CV25Mt : public COldMt
{
public:
	CNUSsnInstance::EmMtVer GetMtVer() const;
	BOOL32 GetConfList(const u16 wMsgEvent, const s8* pchDoaminMOID,
		std::vector<TSimpleConfInfo>& vecConfList, const s8* pchMtE164 = "", 
		const BOOL32 bMtLimit = FALSE, const BOOL32 bGetAll = FALSE);
};
class CTTV26Mt : public COldMt
{
public:
	CNUSsnInstance::EmMtVer GetMtVer() const;
};

class CV26Mt : public COldMt
{
public:
	CNUSsnInstance::EmMtVer GetMtVer() const;
};

class CMtFactory
{
public:
	static CBaseMt* GetMt(const std::string& strMtProtocolType,
		const std::string& strProductId,
		const std::string& strVersionId);
};

#endif  // _NUSSN_INSTANCE_H_

