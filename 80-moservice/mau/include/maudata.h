/*****************************************************************************
模块名      : MAU
文件名      : maudata.h
创建时间    : 2014年 08月 26日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifndef	_MPCDDATA_H_
#define	_MPCDDATA_H_

#include "osp.h"
#include "mcuconst.h"
#include "mauutility.h"
#include "kdvsys.h"
#include "templatedbopr.h"
#include "maustruct.h"
#include "apdbopr.h"
#include "vmeetingdbopr.h"
#include "confinfodbopr.h"
#include "redisadapter.h"
#include "scheduledbopr.h"

class CMauData
{
public:
	CMauData();
	virtual ~CMauData();

public:
	BOOL32 ReadConfig( void );
	BOOL32 Init();
	void Quit();
	BOOL32 LoadRedisLua(std::vector<string>* pvecFailedScript = NULL);
	u32 GetDBTaskNum(void) const;
	u32 GetQueueSize( void ) const ;
	TDBConnectInfo GetTemplateDBConnectInfo( void ) const;
	TDBConnectInfo GetConfInfoDBConnectInfo( void ) const;
	TDBConnectInfo GetVMeetingDBConnectInfo( void ) const;
	TDBConnectInfo GetApDBConnectInfo( void ) const;
	void   PrintDBStatus( void );
	void   CheckEnv();

	//个人模板相关
	void PrintAllPersonalTempE164(void);
	void PrintPersonalTempByE164( const s8* pchE164 );
	//终端列表操作
	BOOL32 IsMtExist( const s8* pchE164 );
	void AddMtInfo( const CMauMtInfo & tMtInfo );
	BOOL32 ModMtInfo(const CMauMtInfo & tMtInfo);
	BOOL32 DelMtInfo(const CMauMtInfo & tMtInfo);
	BOOL32 GetMtInfo(const s8* pchE164, CMauMtInfo & tOutInfo);
	void   PrintMtList( void );
	u32    GetMtListSize( void );
	BOOL32 GetAllMtList(CKdvMtInfo * pcMtList, u32 &dwMtListSize);

	//域列表操作
	BOOL32 IsDomainExist( const s8* pMoid );
	void AddDomainInfo( const CKdvDomainInfo & tDomainInfo );
	BOOL32 ModDomainInfo( const CKdvDomainInfo & tNewInfo );
	BOOL32 DelDomainInfo( const s8* pMoid );
	BOOL32 GetDomainInfo( const s8* pMoid, CKdvDomainInfo & tOutInfo );
	BOOL32 ModDomainHasXmpuInfoBySDomainMoid(const s8* psDomainMoid, BOOL32 bHas);
	void   ClearDomainInfo( void );
	void   PrintDomainList( void );
	u32    GetDomainSize( void );
	BOOL32 GetAllDomainList( CKdvDomainInfo * pcDomainList, u32 &dwDomainSize );
	BOOL32 GetPlatformPrefix( const s8* pMoid, s8* pbyPlatformPrefix ); 
	BOOL32 GetPlatDomainInfoByPrefix( s8* psPlatformPrefix, CKdvDomainInfo & cPlatDomainInfo );

	u16 GetTemplateCount( void );

	BOOL32 ProSendHduBelongDoaminOwnHduList2Cm(const std::string& strHduMoid);
	BOOL32 GetHduGuidBelongDoaminInfo(const std::string&, CKdvDomainInfo&);

	void AddModHduMoidUserDomainMoid(const s8 *pchHduMoid, const s8 *pchUserDomainMoid);
	void DelHduUserDomainMoid(const s8 *pchHduMoid);
	BOOL32 GetUserDomainMoidByHduMoid(const string &strHduMoid, string &strUserDomainMoid);
	// Json相关处理
	static void SetMcuListToJson(s8 * pchJsonBuf, u32 &dwJsonBufLen, vector<string>& vctMcuName, 
		vector<string>& vctMcuType, vector<string>& vctMcuIp, vector<string>& vctGkPrefix, u8 byMcuNum);
	static void SetTemplateListToJson(s8 * pchJsonBuf, u32 &dwJsonBufLen, CMauMsg cServmsg);
	static void SetDomainMoIdToJson(s8 * pchJsonBuf, const s8 * pchMoId, u32 &dwJsonBufLen);
	static void SetReqRspToJsonObj(const json_t_ptr& pjRoot, const TReqHeadInfo &tReqHeadInfo);

	static void SetBookConfE164ToJson(s8 * pchJsonBuf, u32 &dwJsonBufLen, const s32 nMeetingId, const s8 * pchE164);
	static void SetDelBookConfToJson(s8 * pchJsonBuf, u32 &dwJsonBufLen, CBookInfo * pcBookInfo, u16 wBookNum);
	static void SetHduInfoToJsonObj(const json_t_ptr& pjRoot, CHduInfo * pcHduInfo, u16 wHduNum);

	static void SetAllHduInfoToJson(s8 * pchJsonBuf, u32 &dwJsonBufLen, u8 * pbyMsg, u16 wMoIdNum);

	static json_t_ptr SetHduListByMoidToJsonObj(const s8 * achDomainMoid, const CHduInfo * pcHduInfo, u16 wHduNum);
	static json_t_ptr HduInfoToJsonObj(const CHduInfo& cHduInfo);

	static void SetConfMaxJoinedMtNum(s8 * pchJsonBuf, u32 &dwJsonBufLen, u16 wMaxMtNum);

	static void GetCmuIdFromJson( s8 * pchJsonBuf, u16 &wCmuId );
	static void GetMeetingIdFromJson(s8 * pchJsonBuf, u32 &nMeetingId);
	static BOOL32 GetBookConfListNumFromJson(s8 * pchJsonBuf, u16 &wBookNum);
	static void GetConfDataFromJson(const s8 * pchJsonBuf, TMpcConfData * ptConfData, u16 &wConfNum);
	static BOOL32 GetBookConfListFromJson(s8 * pchJsonBuf, std::vector<CRedisBookConf>& confVec);
	static void GetConfSynDataFromJsom(s8 * pchJsonBuf, TConfSynData & tConfSynData);
	static void GetCmFlagFromJson(s8 * pchJsonBuf, u16 &wCmFlag);
	static void GetConfDurationFromJson(s8 * pchJsonBuf, u16 &wConfDuration);
	static void GetConfPwdFromJson(s8 * pchJsonBuf, u8 & byNeedPwd, s8 * pchPwd, u32 dwPwdLen);
	static void GetHduInfoFromJson(s8 * pchJsonBuf, u8 &byHduId, u8 &byHduChnIdx, u8 &byHduSchedIdx, u8 &HduSilence, u8 &byHduVolume);
	static void GetConfCreatorNameFromJson(s8 * pchJsonBuf, s8 * pchCreatorName, u32 dwLen);
	static void GetConfCreatorMoidFromJson(s8 * pchJsonBuf, s8 * pchCreatorMoid, u32 dwLen);
	static BOOL32 GetConfMaxJoinedMtNum(json_t_ptr pjRoot, u16& wMaxMtNum);

	static BOOL32 GetTemplateListFromJson(s8 * pchJsonBuf, CMauMsg &cServMsg);
	static json_t_ptr CreatPollModuleSetJson(const TMiniPollInfo &tPollModule);
	static BOOL32 SetPollModuleToJson(const json_t_ptr& pjRoot, const TMiniPollInfo & tPollModule);
	static BOOL32 SetVipModuleToJson(const json_t_ptr& pjRoot, const TMiniVipInfo& tVipModule);
	static BOOL32 GetPollModuleFromJson(const json_t_ptr& pjRoot, TMiniPollInfo &tPollModule);

	BOOL32 GetDomainFromJson(s8 * pchJsonBuf, CKdvDomainInfo & tOutInfo);

	// NU信息
	BOOL32 GetNuData( u16 wNuId, TNuData &tNuData );
    BOOL32 SetNuData( u16 wNuId, TNuData &tNuData );	

	static BOOL32 PackTMtAliasArray( TMtAlias *ptMtAlias, u16 *pawMtDialRate, u16 wMtNum, char *pszBuf, u16 &wBufLen );
	static BOOL32 UnPackTMtAliasArray(char *pszBuf, u16 wBufLen, const TConfInfo *ptConfInfo, TMtAlias *ptMtAlias, u16* pawMtDialRate, u8 &byMtNum);
	static void InsertReqMt(const TReqHeadInfo& tReqInfo, u16 dwConfBitRate, TMtAlias* patMtAlias, u16* awMtCallBitrate, u16& wMtNum, u16 wMaxMtNum, TVmpModule& tVmpModule, TMiniPollInfo& tPollModule, TMixModule& tMixModule, u8* abyVipMt);
	static BOOL32 SendMsgToMauVC(u16 wEvent, const void *pbyMsg = NULL, u16 wLen = 0, u32 dwSrcIID = MAKEIID(INVALID_APP, INVALID_INS));
	static BOOL32 SendMsgToCmuSsn(const s8* pchCmuIp, u16 wEvent, const void* pbyMsg, u16 wLen);
	static BOOL32 NoticeCmuSsnAddMt(const s8* pchCmuIp, const TReqHeadInfo& tReqInfo);
	static BOOL32 SendMsgToNuSsn(u16 wNuID, u16 wEvent, const void *pbyMsg = NULL, u16 wLen = 0);
	static BOOL32 SendMsgToCmSsn(u16 wEvent, const void *pbyMsg = NULL, u16 wLen = 0);
	static BOOL32 SendMsgToCmCmuSsn( u16 wEvent, const void*pbyMsg = NULL, u16 wLen = 0 );
	static BOOL32 SendMsgToHduSsn(u16 wEvent, const void *pbyMsg = NULL, u16 wLen = 0);


	static void StructTranslate( TMiniConfInfo & tMiniConfInfo, TConfInfo & tConfInfo );
	static BOOL32 TransConfE1642ShortNO(const s8* pConfE164, s8* pShortNO);
//	static void SetConfChargeToJson(const json_t_ptr& pjRoot, const CConfCacheData &cConfCacheData);

	static BOOL32 GetBookStartTimeFromJson(const s8 * pchJsonBuf, s8* pBookStartTime, u32 dwTimeLen);
	static BOOL32 GetBookStartTimeFromJson(const json_t_ptr& pjRoot, s8* pBookStartTime, u32 dwTimeLen);

	//MCU相关
	BOOL32 GetMpcData(const s8* achIp, TCmuData &tMpcData);
	BOOL32 GetMpcData(u32 dwIp, TCmuData &tMpcData);
	void GetAllMpcData(MapCmuData& mapCmuData);
protected:
	BOOL32 ConstructConfE164(const s8* pPlatformDomain, s8* pConfE164);
	BOOL32 ConstructTemplateE164(const s8* pPlatformDomain, s8* pConfE164);
	
public:
	//会议数量相关
	BOOL32 IsConfNumReachLimit(const s8* pchJsonBuf,
		const CKdvDomainInfo& cDoaminInfo, BOOL32& bReachLimit);
	BOOL32 IsConfNumReachLimit(const json_t_ptr& pjRoot, const CKdvDomainInfo& cDoaminInfo,
		BOOL32& bReachLimit);
	BOOL32 IsConfNumReachLimit(const CKdvDomainInfo& tDomainInfo, 
		const u32 dwMaxJoinedMt, const BOOL32 bOldMcu, BOOL32& bReach);
	BOOL32 IsConfNumReachUserDomainLimit(const CKdvDomainInfo& tDomainInfo,
		const u32 dwMaxJoinedMt, BOOL32& bReach);
	BOOL32 IsConfNumReachPlatformDomainLimit(const CKdvDomainInfo& tDomainInfo,
		const u32 dwMaxJoinedMt, BOOL32& bReach);
	BOOL32 GetConfNumByDomain(const s8* pDomainMOID, 
		const EMConfNumType emConfNumType, u32& dwConfNum);
	void SetPlatConfLimitNum(const CPlatformConfNumLimit& cPlatformConfNumLimit);
	const CPlatformConfNumLimit& GetPlatConfLimitNum() const;

	void PrintConfCacheData(void);
	BOOL32 IsConfExist(const s8* pConfE164, BOOL32& bExists);
	BOOL32 IsConfDataExist(const s8* pchConfE164, BOOL32& bExist);
	BOOL32 GetConfListByDomainAndMt(const s8* pDomainMOID, const s8* pchMtE164, 
		ConfList& lstConf, const BOOL32 bIsCheckPublic = FALSE);
	u32 GetConfDetailInfo(const CMauMtInfo& cMtInfo, const s8* pchE164, TConfDetailInfo& tDetailInfo);
	BOOL32 GetConfExInfo(const s8* pchE164, TRedisConfInfo& tConfInfo);
	BOOL32 GetAllConfExInfo(std::list<TRedisConfInfo>& lstConfInfo);
	BOOL32 SetConfData(const TRedisConfInfo& tConfData);
	BOOL32 UpdateConfData(const TRedisConfInfo& tConfData);
	BOOL32 ChangeConfState(const s8* pchDomainMoid, const s8* abyConfE164, u8 byState);
	BOOL32 DelConf(const s8* pchDomainMoid, const s8* pchConfE164, const s8* pchMcuIp);
	BOOL32 GetConfExListByCmuIp(const s8* pchCmuIp, std::list<TRedisConfInfo>& lstConfEx);
	BOOL32 GetConfExListByCmuIp(const u32 dwCmuIp, std::list<TRedisConfInfo>& lstConfEx);
	BOOL32 GetConfExListByDomain(const s8* pDomainMOID, std::list<TRedisConfInfo>& lstConfInfo);
	BOOL32 SetMaxDomainConfNum(const s8* pchDomainMOID, 
		const u32 dwMaxSmallConfNum, const u32 dwMaxLargeConfNum);
	BOOL32 GetCmuConfNum(const s8* pchCmuIp, u32& dwConfNum);
	BOOL32 GetCmuConfNum(const u32 dwCmuIp, u32& dwConfNum);
	BOOL32 UpdateMonitorAliveTime(const s8* pchE164, const s8* pchIp, const s8* pchPort);
	BOOL32 GetConfMonitorsInfo(const s8* pchE164, std::vector<TMonitorAliveInfo>& monitorVec);
	BOOL32 GetConfStopMonitorsJson(const string& strConfE164, string& strStopMonitorsjson);
	BOOL32 ConstructStopConfMonitorsJson(const string& strConfE164, const std::vector<TMonitorAliveInfo>& vecMonitors, string& strJson);
	BOOL32 GetExpiredMonitors(vector<TMonitorAliveInfo>& vecMonitors);
	//redis预约会议操作
	BOOL32 AddBookConf(const CRedisBookConf& conf);
	BOOL32 ModBookConf(const CRedisBookConf& conf);
	BOOL32 DelBookConf(const s8* pchE164);
	BOOL32 GetBookConfE164ByMeetingID(const u32 nMeetingID, std::string& strBookConfE164);
	BOOL32 GetBookConf(const s8* pchE164, CRedisBookConf& cBookConf);
	BOOL32 GetAllBookConf(std::vector<CRedisBookConf>& bookConfVec);
	u16 GetBookConfNum();
	BOOL32 GetTodayBookConfListByMt(const s8* pchDomain, std::vector<CRedisBookConf>& confVec,
								const s8* pchMtE164, const BOOL32 bLimitMt);
	BOOL32 GetTodayBookConfList(const s8* pchDomain, std::vector<CRedisBookConf>& confVec);
	u32 GetBookConfList(const CMauMsg& cMsg, std::vector<CRedisBookConf>& confList, 
							CKdvDomainInfo* pcDomainInfo = NULL, BOOL32* bHasMcu = NULL);
	void CompletedBookConf(const CKdvDomainInfo& cDomainInfo, std::vector<CRedisBookConf>& confList);
	void PrintBookConfData(const u8 byPrintParam = 0, const BOOL32 bPrintDetailJson = FALSE);
	BOOL32 UpuAddAllBookConf();

	//会议恢复相关
	BOOL32 GetCreateConfJson(const std::string& confE164, const BOOL32 bHasXmpu, string& createConfJson);
	BOOL32 IsConfExistAndBelongMcu(const std::string& confE164, const string& mcuIp);
	void PrintNeedRecorverConf();
	BOOL32 CheckRedisData(std::string& checkResStr);
	BOOL32 SetRecoverTime(const s8* pchConfE164);
	BOOL32 GetRecoverTime(const s8* pchConfE164, time_t* ptime);
	BOOL32 StartInitRedisConf();
	void SetRecoverTimeSpan(const u32 dwSeconds);
	u32 GetRecoverTimeSpan() const;
	void SetMcuHeartBeatTimeSpan(const u32 dwSeconds);
	u32 GetMcuHeartBeatTimeSpan() const;
	void SetConfListTimeSpan(const u32 dwSeconds);
	u32 GetConfListTimeSpan() const;
	
	//话单相关
	BOOL32 ConstructChargeJson(const s8* pchConfE164, std::string& chargeJson);

	BOOL32 AddPersonalTempInfoAndUpu(const s8 *pchConfE164, const s8* pchJsonBuf, const CKdvDomainInfo& cDomainInfo);
	BOOL32 DelPersonalTempInfoAndUpu(const s8 *pchConfE164);
	BOOL32 FindPersonalTempInfo(const s8 *pchConfE164, string &strJsonBuf);

	BOOL32 GetTemplateListByDomain(const s8* pDomainMOID, 
		std::vector<CTempCacheData>& vecTempList, BOOL32 bIsCheckPublic = FALSE);

	void SetPlatformIsRent(const BOOL32 bPlatformRent);
	BOOL32 GetPlatformIsRent();

	void SetIsEduVer(const BOOL32 bEduVer);
	BOOL32 IsEduVer();
	//测试css环境相关
	CMqStatus& GetMqStatus();
	u32 GetMqConsumerHeartBeatTime() const;
	void SetMqConsumerHeartBeatTime(const u32 dwTime);
protected:
	//模板缓存信息操作
	void AddTempData(const CTempCacheData & tTempData);
	void ClearTempData(void);
	BOOL32 DelTempData(const s8* pchConfE164);
	BOOL32 ModTempData(const CTempCacheData & tTempData);
	BOOL32 GetTemplateInfoByE164(const s8* pTempE164, CTempCacheData & tTempDataInfo);
	BOOL32 GetTemplateListByDomain(const s8* pDomainMOID, CTempCacheData * pTempList, u16 &wCount, BOOL32 bIsCheckPublic = FALSE);
	u16 GetTemplateNumByDomain(const s8* pDomainMOID, BOOL32 bIsCheckPublic = FALSE);
	void PrintTempCacheData(void);
	//CMU相关操作
	BOOL32 AssignMpcID(const s8* lpIp, TCmuData & tCmuData);
//	u16 AssignMpcID();
	BOOL32 AddMpc(TCmuData & tCmuData);
	BOOL32 IsMpcConnect(const s8* achCmuIp);
	BOOL32 RemoveMpc(const s8* achCmuIp);
	BOOL32 RemoveMpc(u32 dwCmuIp);
	BOOL32 SetMpcData(TCmuData &tCmuData);
	BOOL32 EnableMpc(const s8* achCmuIp, BOOL32 bEnable);
	BOOL32 EnableMpc(u32 dwIp, BOOL32 bEnable);
	void PrintCmuData(void);

	BOOL32 ConfNameRepeatProc(CKdvDomainInfo tDomainInfo, const s8 *pszConfNameSrc,
		const s8 *pszConfE164, s8 *pszConfNameBuffer, u32 nBufferLen, BOOL32& bModify);
	BOOL32 IsConfNameExist(const s8* pszDomainMOID, const s8 *pszConfName, const s8 *pszConfE164, BOOL32& bExist);
	BOOL32 IsTempNameExist(const s8* pszDomainMOID, const s8 *pszTempName, const s8 *pszTempKey, BOOL32& bExist);
	BOOL32 IsTemplateExist(const s8 *pszTemplateE164);

	//hdu信息操作
	BOOL32 IsHduExist(CHduInfo & cHduInfo);
	void AddHduInfo(CHduInfo & cHduInfo);
	BOOL32 ModHduInfo(CHduInfo & cHduInfo);
	BOOL32 DelHduInfo(CHduInfo & cHduInfo);
	BOOL32 GetHduInfo(u8 byHduId, CHduInfo & cOutInfo);
	void PrintHduList(void);
	void PrintApDBHduInfo(void);
	BOOL32 ProSendHduInfo2CmByMoid(void);
	BOOL32 ProSendAllHduInfo2Cm(void);
	BOOL32 DeleteAllHduInfo(void);
	BOOL32 IsHduBelongToMe(const string &strDomainMoid, const string &strHduMoid);

public:
	s8  m_achTelnetName[MAXLEN_TELNET_NAME+1];
    s8  m_achTelnetPwd[MAXLEN_TELNET_PWD+1];
	s8	m_achUpuIP[20+1];			//upu服务器的IP
	s8	m_achMqIP[20+1];			//mq服务器的IP
	u32 m_dwMqPort;					//mq服务器端口
	s8	m_achTopLevelMqIP[20+1];	//核心域mq服务器的IP
	s8	m_achZkIP[20+1];			//zk资源池的IP
	u16 m_wZkPort;					//zk资源池的端口
	s8  m_achHduPoolIP[20+1];		//hdupool的IP
	u16 m_wHduPoolPort;				//hdupool的端口
	s8	m_achRedisIP[20 + 1];		//Redis IP地址
	s8  m_achRedisPwd[1024 + 1];     //配置文件redis密码
	u16 m_wRedisPort;				//Redis 端口
	u16 m_wTempConfNum;			//会议模板数量
	u16 m_wBookConfNum;			//预约会议数量
	s8  m_achPlatformMoid[40 + 1];
	s8  m_achPlatformGuid[40 + 1];
	u32 m_dwRecoverTimeSpan;        //定时恢复时间(单位:s)
	u32 m_dwMcuHeartBeatTimeSpan;   //mcu心跳检测时间(单位:s)
	u32 m_dwGetConfListTimeSpan;   //向mcu定时请求会议列表(单位:s)
	u32 m_dwListenPort;				//css osp监听端口
	u32 m_dwMqConsumerHeartBeat;    //MQ消费者心跳检测时间(单位:s)

	CTemplateDBOpr m_cTemplateDBOpr;
 	CConfInfoDBOpr m_cConfInfoDBOpr;
    CVMeetingDBOpr m_cVMeetingDBOpr;
	CAPInfoDBOpr m_cApInfoDBOpr;
	CScheduledConfInfoDBOpr m_cScheduledDBOpr;

	typedef std::map<std::string, CTempCacheData*>      CTempCacheDataMap;
	typedef std::multimap<std::string, CTempCacheData*> CTempCacheMoidDataMap;
	typedef std::map<std::string, CKdvDomainInfo> CKdvDomainInfoMap;
	typedef std::map<std::string, CMauMtInfo> CMtInfoMap;
	typedef std::map<std::string, std::string> CHduUserDomainMoidMap;
	typedef std::map<u8, CHduInfo> CHduInfoMap;
	typedef std::map<std::string, std::string> CPersonalTempInfoMap; //key : E164号 value : json字符串保存个人模板信息
private:
	u8	m_byDataStoreType;				    //数据存储类型
	u32 m_dwDBTaskNum;					    //数据库连接线程数
	u32 m_dwMsgQueueNum;				    //数据库消息队列数
	TDBConnectInfo m_tTemplateDBInfo;	    //模板DB数据库信息
	TDBConnectInfo m_tConfInfoDBInfo;	    //会议信息DB数据库信息
	TDBConnectInfo m_tVMeetingDBInfo;		//会管侧虚拟会议DB数据库信息
	TDBConnectInfo m_tApDBInfo;             //AP数据库信息——读取用户域、平台域、用户域、终端信息
	TDBConnectInfo m_tScheduledConfDBInfo;  //本模块预约会议DB数据库信息

	CTempCacheDataMap		m_mapTempCacheInfo;			// 模板信息缓存 Key是E164
	CTempCacheMoidDataMap	m_mapTempMoidCacheInfo;		// 模板信息缓存 Key是Moid

	CKdvDomainInfoMap			m_mapDomainInfo;

	CHduUserDomainMoidMap m_mapHduUserDomainMoid;    // 保存HUD对应用户域Moid, Key是HduMoid

	CMtInfoMap m_mapMtInfo;

	CHduInfoMap m_mapHduInfo;
	
	CPersonalTempInfoMap m_mapPersonalTempinfo;

	TNuData m_atNuData[MAX_NU_NUM];
	MapCmuData m_mapCmuData;
	std::map<std::string, time_t> m_mapConfRecoverTime;
	u32 m_dwTempE164Backup;                           //上次刚使用过的模板E164号
	CPlatformConfNumLimit m_cPlatformConfNumLimit;     //平台域会议限制数目

	CRedisConfData m_cRedisData;

	OspLock m_cMcuListLock;
	OspLock m_cPlatformIsRentLock;
	OspLock m_cHduUserDomainMoidLock;
	OspLock m_cPersonalTempLock;
	OspLock m_cDomainListLock;
	OspLock m_cMtListLock;
	OspLock m_cTempCacheDataLock;

	CMqStatus m_cMqStatus;

	u8 m_byInitCount;    //初始化操作计数

	BOOL32 m_bPlatformRent; //平台是租赁还是自建
	BOOL32 m_bEduVer;       //是否为EDU版本
	friend class CMauVCInstance;
};

#endif  // _MPCDDATA_H_