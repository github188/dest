/*****************************************************************************
ģ����      : MAU
�ļ���      : mauvc.h
����ʱ��    : 2014�� 08�� 26��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
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
	//�ػ�ʵ����Ϣ�������
	void DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp );
	//�ػ�ʵ�������ʼ����Ϣ
	void DaemonProcPowerOn(const CMessage *pcMsg, CMauVCApp* pcApp);

	void InstanceEntry(  CMessage * const pcMsg );

public:
	//��ӡ����
	void DaemonProcPrintCmd(const CMessage * pcMsg, CMauVCApp* pcApp);
	/**************************************** APDB�ظ����� **********************************/
	//AP���ݿ�������֪ͨ
	void DaemonProcApFinishOprNtf(const CMessage * pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetDomainInfoFromApFinishNtf( const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetPlatformDomainInfoFromApFinishNtf( const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetUserDomainInfoFromApFinishNtf( const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetAllMtInfoFromApFinishNtf( const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetDomainHasXmpuFromApFinishNtf( const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetHduMoidAndDomainMoidApFinishNtf(const CMessage *   pcMsg, CMauVCApp* pcApp);
	void DaemonProcGetPlatformIsRentNtf(const CMessage *   pcMsg, CMauVCApp* pcApp);
	/**************************************** APDB�ظ����� **********************************/
	void DaemonProcMoveScheduledConfToRedisNtf(const CMessage *   pcMsg, CMauVCApp* pcApp);
	/**************************************** TemplateDB�ظ����� **********************************/
	//���ݿ����ݼ��ؽ����Ϣ����
	void DaemonProcLoadDBResNtf(const CMessage * pcMsg, CMauVCApp* pcApp);
	//���ݿ����ģ�����֪ͨ
	void DaemonProcFinishAddTemp( const CMessage * pcMsg, CMauVCApp* pcApp);
	//���ݿ����ģ��ɾ��֪ͨ
	void DaemonProcFinishDelTemp( const CMessage * pcMsg, CMauVCApp* pcApp);
	//���ݿ����ģ���޸�֪ͨ
	void DaemonProcFinishModTemp( const CMessage * pcMsg, CMauVCApp* pcApp);
	//���ݿ����ģ�����֪ͨ
	void DaemonProcFinishSearchTemp( const CMessage * pcMsg, CMauVCApp* pcApp);
	
	/**************************************** VMeetingDB�ظ����� ********************************/
	//������ݿ�������֪ͨ
	void DaemonProcMeetingFinishOprNtf(const CMessage *pcMsg, CMauVCApp *pcApp);
	//�������ݿ��д������֪ͨ
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
	//�������ݿ�����ǰ�ٿ����֪ͨ
	void DaemonProcAheadMeetingFinishNtf( const CMessage * pcMsg );
	//�����ӳ�����
	void DaemonProcDelayConfFinishNtf( const CMessage * pcMsg );
	//���̻�������ʱ��
	void DaemonProcSubConfDurationFinishNtf( const CMessage * pcMsg );
	//���������֪ͨ
	void DaemonProcReleaseConfFinishNtf( const CMessage * pcMsg );
	//��������E164���֪ͨ
	void DeamonProcSetMeetingE164FinishNtf( const CMessage * pcMsg );
	
	void DeamonProcMeetingStateCheck(const CMessage * pcMsg, CMauVCApp* pcApp);

	/**************************************** ��ܲ��� ********************************/
	//��ܶ�ģ�����
	void DaemonProcTemplateOpr( CMessage * const pcMsg );
	//��ݶԸ���ģ�����
	void DaemonprocAllPersonalTmpAckMsg(CMessage *const pcMsg);
	void DaemonProcPersonalTemplate(CMessage *const pcMsg);
	//��ܶ�ԤԼ����
	void DaemonProcBatchAddBookConf(CMessage * const pcMsg);
	void DaemonProcBatchModBookConf(CMessage * const pcMsg);
	void DaemonProcBatchDelBookConf(CMessage * const pcMsg);
	//��ܽ�����
	void DaemonProcCMReleaseConfReq( const CMessage * pcMsg );
	//ԤԼ�������ٿ�֪ͨ
	void DaemonProcScheduledConfAlreadyOngoingNtf( CMessage * const pcMsg );
	// �����ӳٴ���
	void DaemonProcConfDelay( const CMessage * pcMsg );
	// ��ܼ�ر���֪ͨ
	void DaemonProcMonitorKeepAliveNtf( const CMessage * pcMsg );

	//����ģ������ʱ
	void DaemonProcGetPersonalTempTimer(const CMessage *pcMsg);

	/**************************************** �ն˴��ᡢ��ᴦ�� **************************/
	//�����ն˵�������Ϣ����(�ټ�����/�μӻ���)
	void DaemonProcMtCreateConf(const CMessage *  pcMsg);
	void DaemonProcMtJoinBookConf(const CMessage *  pcMsg);
	void DaemonProcCreateConfByPublicTemp(const CMessage *  pcMsg);
	u32 AheadCreateBookConf(TReqHeadInfo& tReqInfo, CRedisBookConf& bookConf);

	/**************************************** CMU���� **********************************/
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
		
	/**************************************** hdupool������Ϣ ************************************/
	void DeamonProcHduPoolMsg( const CMessage * pcMsg );
	/**************************************** ��ܲ�������ǽ��Ϣ********************************/
	void DeamonProcCmHduMsg( const CMessage * pcMsg );

    /**************************************** �������� ************************************/
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
	//����ָ�
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

	//�����Ϣ
	BOOL32 GetTransportAddrFromJson( json_t_ptr pjTransportAddr, LPSTR lpIPAddr, LPSTR lpPort);
	void UpdateMtAliveTime(LPCSTR lpConfE164, LPCSTR lpIPAddr, LPCSTR lpPort);

	//css������ش���
	void CheckAllMonitorAlive();
	void SetStopMonitorInfoToJson(s8* achJsonBuf, u32 &dwJsonBufLen, const TMonitorAliveInfo& tMtAlive);

	//cmu���֪ͨ����
	BOOL32 GetMoniAliveInfoFromJson(s8* achJsonBuf, TMonitorAliveInfo& tMtAlive);

	void PrintMonitorInfo();
	void PrintConfMonitor(LPCSTR lpConfE164);

	//���ݿ����
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
	string m_strReqId; //����ģ������������ɵ��ַ�������ACK�ַ����Ƚ�
};

#endif  // _MAUVC_INSTANCE_H_
