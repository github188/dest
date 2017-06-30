
/*=======================================================================
模块名      :ULS
文件名      :srvlistmgr.h
相关文件    :
文件实现功能:存储从3A获得的服务器消息，给终端分配服务器

作者        :
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/07/08  1.0                       创建
=======================================================================*/
#ifndef _SRV_LIST_MGR_H_
#define _SRV_LIST_MGR_H_

#pragma warning( disable : 4786 )

#include <set>
#include <string>
#include <vector>

#include "osp.h"
#include "rwlock_api.h"
#include "structulc.h"
#include "structlogin.h"
#include "structself.h"
#include "serverdevice.h"
#include "structpxy.h"




class CSrvListMgr
{
public:
	CSrvListMgr();
	~CSrvListMgr();

/************************************************************************/
//							 插入数据                    
/************************************************************************/
public:
	//NDS(NTS)的列表信息
    void InsertToNdsList(const std::set<CBriefSrvInfo> &setNtsInfo);
    void AddNdsInfo(const CBriefSrvInfo &tNtsInfo);
    void MdfNdsInfo(const CBriefSrvInfo &tNtsInfo);
	void DelNdsInfo(const CBriefSrvInfo &tNtsInfo);

	//NMS的列表信息
    void InsertToNmsList(const std::set<CBriefSrvInfo> &setNmsInfo);
    void AddNmsInfo(const CBriefSrvInfo &tNmsInfo);
    void MdfNmsInfo(const CBriefSrvInfo &tNmsInfo);
    void DelNmsInfo(const CBriefSrvInfo &tNmsInfo);
    
    //APS(XAP/ULS)的列表信息
    void InsertToApsList(const std::set<CBriefSrvInfo> &setApsInfo);
    void AddApsInfo(const CBriefSrvInfo &tApsInfo);
    void MdfApsInfo(const CBriefSrvInfo &tApsInfo);
    void DelApsInfo(const CBriefSrvInfo &tApsInfo);

	//PAS(GK/CSU/NU)的列表信息
	void InsertToPasList(const std::set<CBriefSrvInfo> &setPasInfo);
    void AddPasInfo(const CBriefSrvInfo &tPasInfo);
    void MdfPasInfo(const CBriefSrvInfo &tPasInfo);
	void DelPasInfo(const CBriefSrvInfo &tPasInfo);

    //BGS(GK/CSU/NU)的列表信息
    void InsertToBgsList(const std::set<CBriefSrvInfo> &setBgsInfo);
    void AddBgsInfo(const CBriefSrvInfo &tBgsInfo);
    void MdfBgsInfo(const CBriefSrvInfo &tBgsInfo);
    void DelBgsInfo(const CBriefSrvInfo &tBgsInfo);

    //SUS的列表信息
    void InsertToSusList(const std::set<CBriefSrvInfo> &setSusInfo);
    void AddSusInfo(const CBriefSrvInfo &tAddSusInfo);
    void MdfSusInfo(const CBriefSrvInfo &tModifySusInfo);
    void DelSusInfo(const CBriefSrvInfo &tSusInfo);

    //Cmc(meeting)的列表信息
    void InsertToCmcList(const std::set<CBriefSrvInfo> &setCmcInfo);
    void AddCmcInfo(const CBriefSrvInfo &tCmcInfo);
    void MdfCmcInfo(const CBriefSrvInfo &tCmcInfo);
    void DelCmcInfo(const CBriefSrvInfo &tCmcInfo);

    //SNS(weibo+PlatApi)的列表信息
    void InsertToSnsList(const std::set<CBriefSrvInfo> &setSnsInfo);
    void AddSnsInfo(const CBriefSrvInfo &tSnsInfo);
    void MdfSnsInfo(const CBriefSrvInfo &tSnsInfo);
    void DelSnsInfo(const CBriefSrvInfo &tSnsInfo);

    //Xns的列表信息
    void InsertToXnsList(const std::set<CBriefSrvInfo> &setXnsInfo);
    void AddXnsInfo(const CBriefSrvInfo &tXnsInfo);
    void MdfXnsInfo(const CBriefSrvInfo &tXnsInfo);
    void DelXnsInfo(const CBriefSrvInfo &tXnsInfo);

    //Ntp的列表信息
    void InsertToNtpList(const std::set<CBriefSrvInfo> &setNtpInfo);
    void AddNtpInfo(const CBriefSrvInfo &tNtpInfo);
    void MdfNtpInfo(const CBriefSrvInfo &tNtpInfo);
    void DelNtpInfo(const CBriefSrvInfo &tNtpInfo);

    //Vrs的列表信息
    void InsertToVrsList(const std::set<CBriefSrvInfo> &setVrsInfo);
    void AddVrsInfo(const CBriefSrvInfo &tVrsInfo);
    void MdfVrsInfo(const CBriefSrvInfo &tVrsInfo);
    void DelVrsInfo(const CBriefSrvInfo &tVrsInfo);

    //Dcs的列表信息
    void InsertToDcsList(const std::set<CBriefSrvInfo> &setDcsInfo);
    void AddDcsInfo(const CBriefSrvInfo &tDcsInfo);
    void MdfDcsInfo(const CBriefSrvInfo &tDcsInfo);
    void DelDcsInfo(const CBriefSrvInfo &tDcsInfo);

	void InsertToRestList(const std::set<CBriefSrvInfo> &setRestInfo);
	void AddRestInfo(const CBriefSrvInfo &tRestInfo);
	void MdfRestInfo(const CBriefSrvInfo &tRestInfo);
	void DelRestInfo(const CBriefSrvInfo &tRestInfo);

    void SetMySelfMOID( const s8 * szMOID );
    void SetPlatformDomainMOID( const s8 * szMOID );
    void SetServiceDomainMOID( const s8 * szMOID );
    const s8* GetServiceDomainMOID( ) const;

	void SetAllPlatformDomainMOID(const std::set<std::string>&);
	std::set<std::string>& GetAllPlatformDomainMOID();

public:
    void InsertPxyAddr(const std::set<CPxyPlatformAddr>& setAddr) { m_setPxyAddr = setAddr; }

protected:	
    void AddToSrvList(IN const std::set<CBriefSrvInfo> &setSrvInfo, IN OUT std::vector<CBriefSrvStateEx> &vcSrvEx);
    void AddSrvInfo(IN const CBriefSrvInfo& tSrvInfo, IN OUT std::vector<CBriefSrvStateEx> &vcSrvEx);
    void MdfSrvInfo(IN const CBriefSrvInfo& tModifySrvInfo, IN OUT std::vector<CBriefSrvStateEx>& vcSrvEx);
    void DelSrvInfo(IN const CBriefSrvInfo& tDeletSrvInfo, IN OUT std::vector<CBriefSrvStateEx>& vcSrvEx);

    void CloseOspNodes(IN CStateBase& tSrvStateEx);


/************************************************************************/
//							 获取数据                    
/************************************************************************/
public:
    //获取平台服务器信息(登陆名必须是终端E164)     
    BOOL32 GetPlatformSrvAddrInfo(IN const CLoginInfo& tLoginInfo, IN const std::string strClientISPName, IN const s8* szClientPlatfromMOID, IN const s8* szClientPasMOID, OUT CPlatformSrvAddrInfo& tSrvAddrInfo);

protected: //NormalMode
    BOOL32 GetPlatformSrvAddr(IN const CLoginInfo &tLoginInfo, IN const std::string strClientISPName, IN std::string strLocalApsIspName, IN const s8* szClientPlatfromMOID, IN const s8* szClientPasMOID , OUT CPlatformSrvAddrInfo &tSrvAddrInfo);

    //分配普通服务器
    BOOL32 AssignSrvInfo(IN u32 dwClinetIp/*主机序*/, IN const std::string& strClientIspName, IN const std::string& strLocalApsIspName, IN const s8* szClientPlatfromMOID, IN const std::vector<CBriefSrvStateEx>& vcSrvInfoEx, OUT CSrvAddrInfo& tUlcSrvAddr, IN BOOL32 bChckState = TRUE) const;
    //分配PAS服务器
    BOOL32 AssignPasSrvInfo(IN const s8* szClientPasMOID, IN u32 dwClinetIp/*主机序*/, IN const std::string& strClientISPName, IN const std::string& strLocalApsISPName, IN const s8* szClientPlatfromMOID, OUT CSrvAddrInfo& tUlcSrvAddr) const;
    //分配SUS服务器
    BOOL32 AssignSusSrvInfo(IN const s8* szClientOemMark, IN u32 dwClinetIp/*主机序*/, IN const std::string& strClientISPName, IN const std::string& strLocalApsISPName, IN const s8* szClientPlatfromMOID, OUT CSrvAddrInfo& tUlcSrvAddr) const;
    //分配ULS服务器
    const CApsSrvIpList& AssignApsSrvList( );

public:
    BOOL32 GetLocalPasIspName(IN u32 dwLocalIP/*主机序*/, OUT std::string& strLocalApsIspName) const;
	BOOL32 IsIpEqualIspName(IN u32 dwLocalIP/*主机序*/, IN  std::string& strIspName );


public: //单元测试
#ifdef _TEST_
    std::vector<CBriefSrvStateEx> GetSetSusInfoEx() const { return m_vcSusInfoEx; }
    std::vector<CBriefSrvStateEx> GetSetPasInfoEx() const { return m_vcPasInfoEx; }
#endif

/************************************************************************/
//							 IP状态检测                    
/************************************************************************/
public:
    void CheckStateTimer();
    
protected:
    void CheckState(std::vector<CBriefSrvStateEx>& vcSrvInfoEx, u16 wPort);
	
/************************************************************************/
//							 打印服务器的信息                    
/************************************************************************/
public:
	void PrintAllSrvsInfo();

protected:
	void PrintSrvInfo(std::vector<CBriefSrvStateEx>& vcSrvInfoEx);

private:
    RWLock m_tRWLock;
    s8 m_szMySelfMOID[OPERATION_MAXLEN_MOID+1];        //自己的MOID
    s8 m_szPlatformDomainMOID[OPERATION_MAXLEN_MOID+1];//所属平台域MOID
    s8 m_szServiceDomainMOID[OPERATION_MAXLEN_MOID+1]; //所属服务域MOID
	std::set<std::string> m_setPlatformDomainMOID;  //所属服务域下的所有平台域MOID
    
private:    //正常模式下的数据
	std::vector<CBriefSrvStateEx> m_vcNdsInfoEx;      //NDS(NTS)
	std::vector<CBriefSrvStateEx> m_vcNmsInfoEx;      //NMS
    std::vector<CBriefSrvStateEx> m_vcPasInfoEx;      //Pas
    std::vector<CBriefSrvStateEx> m_vcBgsInfoEx;      //BGS
    std::vector<CBriefSrvStateEx> m_vcSusInfoEx;      //Sus

    std::vector<CBriefSrvStateEx> m_vcCmcInfoEx;      //CMC(meeting+PlatApi)
    std::vector<CBriefSrvStateEx> m_vcSnsInfoEx;      //SNS(weibo)
    std::vector<CBriefSrvStateEx> m_vcXnsInfoEx;      //Xns

    std::vector<CBriefSrvStateEx> m_vcNtpInfoEx;      //时间同步
    std::vector<CBriefSrvStateEx> m_vcVrsInfoEx;      //视频录播
    std::vector<CBriefSrvStateEx> m_vcDcsInfoEx;      //数据会议
	std::vector<CBriefSrvStateEx> m_vcRestInfoEx;     //RestApi

	std::vector<CBriefSrvStateEx> m_vcApsInfoEx;      //APS(XAP)(不需要状态)


private:    //pxy模式下的数据
    std::set<CPxyPlatformAddr> m_setPxyAddr;

private:
    CApsSrvIpList m_tApsSrvIpList;
    BOOL32 m_bApsSrvIpInited;   //m_tApsSrvIpList是否已赋值
};

extern CSrvListMgr g_cSrvListMgr;

#endif  //end of _SRV_LIST_MGR_H_ 
