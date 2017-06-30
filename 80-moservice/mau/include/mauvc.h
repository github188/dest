/*****************************************************************************
模块名      : MAU
文件名      : mauvc.h
创建时间    : 2014年 08月 26日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifndef	_MAUVC_INSTANCE_H_
#define	_MAUVC_INSTANCE_H_

#include "maudata.h"
#include "upussn.h"
#include "ztemplatedaemon.h"

void ConnectSACB( BOOL32 bResult, u64 BmcSn, const s8* szEventStr, u8 * pMsgBuf,u32 dwMsgLen );

enum emModieMode
{
	emModeVideo = 1,
	emModeAudio = 2,
	emModeSecond = 3
};

class CMauVCInstance;
class CMauData;
typedef zTemplate<CMauVCInstance, 0, CMauData, 0> CMauVCApp;

class CMauVCInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_NORMAL,
	};

	enum EmInitState
	{
		emInitStateLoadTemplate,
//		emInitStateLoadOngoingConf,
//		emInitStateCheckConf,
		emInitStateAll
	};
public:
	CMauVCInstance();
	~CMauVCInstance();

public:
	//守护实例消息处理入口
	void DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp );
	//守护实例处理初始化消息
	void DaemonProcPowerOn(const CMessage *pcMsg, CMauVCApp* pcApp);

	void InstanceEntry(  CMessage * const pcMsg );

public:
	//打印命令
	void DaemonProcPrintCmd(const CMessage * pcMsg, CMauVCApp* pcApp);
	/**************************************** APDB回复操作 **********************************/
	//AP数据库操作完成通知
	void DaemonProcApFinishOprNtf(const CMessage * pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetDomainInfoFromApFinishNtf( const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetPlatformDomainInfoFromApFinishNtf( const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetUserDomainInfoFromApFinishNtf( const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetAllMtInfoFromApFinishNtf( const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetDomainHasXmpuFromApFinishNtf( const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetHduMoidAndDomainMoidApFinishNtf(const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetPlatformIsRentNtf(const CMessage *   pcMsg, CMauVCApp* pcApp);
	/**************************************** APDB回复操作 **********************************/
	void DaemonProcMoveScheduledConfToRedisNtf(const CMessage *   pcMsg, CMauVCApp* pcApp);
	/**************************************** TemplateDB回复操作 **********************************/
	//数据库数据加载结果消息处理
	void DaemonProcLoadDBResNtf(const CMessage * pcMsg, CMauVCApp* pcApp);
	//数据库完成模板添加通知
	void DaemonProcFinishAddTemp( const CMessage * pcMsg, CMauVCApp* pcApp);
	//数据库完成模板删除通知
	void DaemonProcFinishDelTemp( const CMessage * pcMsg, CMauVCApp* pcApp);
	//数据库完成模板修改通知
	void DaemonProcFinishModTemp( const CMessage * pcMsg, CMauVCApp* pcApp);
	//数据库完成模板查找通知
	void DaemonProcFinishSearchTemp( const CMessage * pcMsg, CMauVCApp* pcApp);
	
	/**************************************** VMeetingDB回复操作 ********************************/
	//会管数据库操作完成通知
	void DaemonProcMeetingFinishOprNtf(const CMessage *pcMsg, CMauVCApp *pcApp);
	//处理数据库中创会完成通知
	void DaemonProcCreateMeetingFinishNtf( const CMessage * pcMsg );
	void DaemonProcCreateConf( const CMessage * pcMsg );
	std::pair<u32, std::string> CheckAndGetCreateConfInfo(const CMauMsg& cServMsg,
		const BOOL32 bMt,
		TReqHeadInfo& tReqHeadInfo,
		string& strJsonBuf,
		TRedisConfInfo& tRedisConfInfo,
		u32& nMeetingID,
		BOOL32& bTempConf
		);
	//处理数据库中提前召开完成通知
	void DaemonProcAheadMeetingFinishNtf( const CMessage * pcMsg );
	//处理延长会议
	void DaemonProcDelayConfFinishNtf( const CMessage * pcMsg );
	//缩短会议运行时间
	void DaemonProcSubConfDurationFinishNtf( const CMessage * pcMsg );
	//处理结会完成通知
	void DaemonProcReleaseConfFinishNtf( const CMessage * pcMsg );
	//处理设置E164完成通知
	void DeamonProcSetMeetingE164FinishNtf( const CMessage * pcMsg );
	
	void DeamonProcMeetingStateCheck(const CMessage * pcMsg, CMauVCApp* pcApp);

	/**************************************** 会管操作 ********************************/
	//会管对模板操作
	void DaemonProcTemplateOpr( CMessage * const pcMsg );
	//会馆对个人模板操作
	void DaemonprocAllPersonalTmpAckMsg(CMessage *const pcMsg);
	void DaemonProcPersonalTemplate(CMessage *const pcMsg);
	//会管对预约操作
	void DaemonProcBatchAddBookConf(CMessage * const pcMsg);
	void DaemonProcBatchModBookConf(CMessage * const pcMsg);
	void DaemonProcBatchDelBookConf(CMessage * const pcMsg);
	//会管结会操作
	void DaemonProcCMReleaseConfReq( const CMessage * pcMsg );
	//预约会议已召开通知
	void DaemonProcScheduledConfAlreadyOngoingNtf( CMessage * const pcMsg );
	// 会议延迟处理
	void DaemonProcConfDelay( const CMessage * pcMsg );
	// 会管监控保活通知
	void DaemonProcMonitorKeepAliveNtf( const CMessage * pcMsg );

	//个人模板请求定时
	void DaemonProcGetPersonalTempTimer(const CMessage *pcMsg);

	/**************************************** 终端创会、入会处理 **************************/
	//来自终端的请求消息处理(召集会议/参加会议)
	void DaemonProcMtCreateConf(const CMessage *  pcMsg);
	void DaemonProcMtJoinBookConf(const CMessage *  pcMsg);
	void DaemonProcCreateConfByPublicTemp(const CMessage *  pcMsg);
	u32 AheadCreateBookConf(TReqHeadInfo& tReqInfo, CRedisBookConf& bookConf);

	/**************************************** CMU操作 **********************************/
	void DaemonProcMpcConnectNtf( const CMessage * pcMsg );
	void DaemonProcMpcDisConnectNtf( const CMessage * pcMsg );
	void DaemonProcCmuMsg( const CMessage *  pcMsg );
	void DaemonProcCreateConfAck( const CMessage *  pcMsg );
	void DaemonProcCreateConfNack( const CMessage * pcMsg );
	void DaemonProcRecoverConfNack(const CMessage * pcMsg);
	void DaemonProcReleaseConfNtf( const CMessage *  pcMsg );
	void AddConfInfo(const s8* pchConfE164, const s8* pchConfName, const s8* pchPwd);
	void DaemonProcCmuConfListNtf(const CMessage * pcMsg);
	void SendReleaseConfToCmu(const s8* pchConfE164, const s8* pchConfName, const s8* pchCmuIp, BOOL32 bEndAll);
	void SendUpdateAllConfDataToCmu(const s8* pchCmuIp, const s8* pchConfE164,
		const s8* pchConfName, const s8* pchDomainMOID);
		
	/**************************************** hdupool交互消息 ************************************/
	void DeamonProcHduPoolMsg( const CMessage * pcMsg );
	/**************************************** 会管操作电视墙消息********************************/
	void DeamonProcCmHduMsg( const CMessage * pcMsg );

    /**************************************** 其他操作 ************************************/
    void DaemonProcDisconnectNtf( const CMessage * pcMsg );
	void SendReplyMsg( const TReqHeadInfo & tReqInfo, u32 wEvent, const void* pbyMsg = NULL, u16 wLen = 0 );
	void SendReplyMsg(CMauMsg& cMsg, const u32 dwErrCode);
	void DaemonProcCheckConfTimer(const CMessage* pcMsg);
	BOOL32 CleanExceptionBookConfs();
	void DaemonProcTempConfNtf(const CMessage* pcMsg);
	void DaemonProcOngoingConfNtf(const CMessage* pcMsg);
	void DaemonEnableCmu(const CMessage* pcMsg);
	void DaemonPlatformConfLimit(const CMessage* pcMsg);
	BOOL32 ReleaseConf(const s8* pchMoid, const s8* pchConfE164, 
		const s8* pchCmuIp, const BOOL32 bReportCharge = TRUE);
	void SetVmpParamToJson(json_t_ptr pjRoot, TVMPParam tVmpParam);
	void DaemonProcCmtoCmuMsg(CMessage * const pcMsg);
	void DaemonProcCmutoCmMsg(CMessage * const pcMsg);
	s32 StartServer(CMauVCApp* pcApp);
	void SetInitState(EmInitState emState);
	BOOL32 ReportCharge(const std::string& strCharge);
	BOOL32 NoticeCMStopConfMonitors(const s8* pchConfE164);

	//----------------------------------------------------------------------------
	//会议恢复
	void DaemonProcInitRedisConfTimer(const CMessage *pcMsg, CMauVCApp* pcApp);
	void DaemonProcCheckRedisData(const CMessage* pcMsg);
	void DaemonProcRecoverConfTimer(const CMessage* pcMsg);
	void RecoverConfList(u32 confNum = 0, const TCmuData* ptCmu = NULL);
	void RecoverConfList(const list<TRedisConfInfo>& confList, const TCmuData* ptCmu = NULL);
	BOOL32 RecoverConf(const TRedisConfInfo& tRebuildConf, const TCmuData* ptCmu = NULL);
	BOOL32 RecoverMeeting(const TCmuData& tCmu, const TRedisConfInfo& tConfInfo);
	BOOL32 AccordRedisConfAndCmuConfList(const TMpcConfData* tMcuConfList, const u32 dwConfNum,
		const s8* pchMcuIp, const u32 dwMcuPid);
	BOOL32 AccordRedisConfAndCmuConf(const TMpcConfData& tMcuConf, 
		const s8* pchMcuIp, const u32 dwMcuPid);
	BOOL32 LogAccordConditon(const TMpcConfData& tMcuConf, 
		const s8* pchMcuIp, const u32 dwMcuPid);
	BOOL32 ReleaseMcuConf(const TMpcConfData& tMcuConf,
		const s8* pchMcuIp, const u32 dwMcuPid);
	BOOL32 UpdateRedisConf(const TMpcConfData& tMcuConf,
		const s8* pchMcuIp, const u32 dwMcuPid);
	BOOL32 NoticeMcuFlushRedisData(const TMpcConfData& tMcuConf, 
		const s8* pchMcuIp, const u32 dwMcuPid);
	BOOL32 UpdateRedisConfAndNoticeMcuFlushDataAndStopMonitor(const TMpcConfData& tMcuConf, 
		const s8* pchMcuIp, const u32 dwMcuPid);
	BOOL32 SwitchConf(const TMpcConfData& tMcuConf, const s8* pchMcuIp, const u32 dwMcuPid);

	//----------------------------------------------------------------------------
	void DaemonProcLoadUserDomainInfoTimer(const CMessage *pcMsg);
	void DaemonProcInitUpuAddBookConfTimer(const CMessage *pcMsg);
	BOOL32 UpuAddAllBookConf();
	BOOL32 IsAllInited();
	s32 m_dwInitState;

	//监控信息
	BOOL32 GetTransportAddrFromJson( json_t_ptr pjTransportAddr, LPSTR lpIPAddr, LPSTR lpPort);
	void UpdateMtAliveTime(LPCSTR lpConfE164, LPCSTR lpIPAddr, LPCSTR lpPort);

	//css心跳监控处理
	void CheckAllMonitorAlive();
	void SetStopMonitorInfoToJson(s8* achJsonBuf, u32 &dwJsonBufLen, const TMonitorAliveInfo& tMtAlive);

	//cmu监控通知处理
	BOOL32 GetMoniAliveInfoFromJson(s8* achJsonBuf, TMonitorAliveInfo& tMtAlive);

	void PrintMonitorInfo();
	void PrintConfMonitor(LPCSTR lpConfE164);

	//数据库操作
	void RegInitDealFun();
	void LoadDBData();
	void GetServDomainInfo();
	void GetPlatFormDomainInfo();
	void GetUserDomainInfo();
	void GetMtInfo();
	void GetDomainHasXmpu();
	void GetHduMoidAndUserDomainMoid();
	void GetPlatformIsRent();
	void InitRedisConf();
	void MoveScheduleDbDataToRedis();

	typedef void (CMauVCInstance::*InitDealFun)();
	typedef std::vector<InitDealFun> vecInitDealFun;
	static vecInitDealFun s_InitDealfun_vec;
private:
	BOOL32 m_bHasMonitorCheckTimer;
	string m_strReqId; //个人模板请求，随机生成的字符串，与ACK字符串比较
};

#endif  // _MAUVC_INSTANCE_H_
