
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
#include <map>

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
    void InsertToServerList(const u32 dwServerType, const std::set<CBriefSrvInfo> &setNtsInfo);
    void AddServerInfo(const u32 dwServerType, const CBriefSrvInfo &tNtsInfo);
    void MdfServerInfo(const u32 dwServerType, const CBriefSrvInfo &tNtsInfo);
	void DelServerInfo(const u32 dwServerType, const CBriefSrvInfo &tNtsInfo);

    void SetMySelfMOID( const s8 * szMOID );
    void SetPlatformDomainMOID( const s8 * szMOID );
    void SetServiceDomainMOID( const s8 * szMOID );
    const s8* GetServiceDomainMOID( ) const;
	const s8* GetPlatformDomainMOID() const;

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
    BOOL32 GetPlatformSrvAddrInfo(IN const CLoginInfo& tLoginInfo, IN const std::string strClientISPName,
		IN const s8* szClientPasMOID, OUT CPlatformSrvAddrInfo& tSrvAddrInfo);
	const std::map<u32, std::vector<CBriefSrvStateEx>*> GetTypeServerListMap() const;

protected: //NormalMode
    BOOL32 GetPlatformSrvAddr(IN const CLoginInfo &tLoginInfo,
		IN const std::string strClientISPName, IN std::string strLocalApsIspName,
		IN const s8* szClientPasMOID , OUT CPlatformSrvAddrInfo &tSrvAddrInfo);

	//分配所有服务器通用函数
    BOOL32 AssignServerInfo(const u32 dwServerType, IN u32 dwClinetIp/*主机序*/, IN const std::string& strClientIspName,
		IN u32 dwLocalApsIp, IN const std::string& strLocalApsIspName, 
		IN const std::vector<CBriefSrvStateEx>& vcSrvInfoEx, OUT CSrvAddrInfo& tUlcSrvAddr,
		IN BOOL32 bChckState = TRUE, const s8* pchClientPasMOID = "",
		const s8* pchClientOemMark = "", const s8* pchDeviceType = "") const;

    //分配普通服务器
    BOOL32 AssignSrvInfo(IN const u32 dwClinetIp/*主机序*/, IN const std::string& strClientIspName,
		IN const u32 dwLocalApsIp, IN const std::string& strLocalApsIspName,
		IN const std::vector<CBriefSrvStateEx>& vcSrvInfoEx, OUT CSrvAddrInfo& tUlcSrvAddr, IN BOOL32 bChckState = TRUE) const;
    //分配PAS服务器
    BOOL32 AssignPasSrvInfo(IN const s8* szClientPasMOID, 
		IN const u32 dwClinetIp/*主机序*/, IN const std::string& strClientISPName,
		IN const u32 dwLocalApsIp, IN const std::string& strLocalApsISPName,
		OUT CSrvAddrInfo& tUlcSrvAddr) const;
    //分配SUS服务器
    BOOL32 AssignSusSrvInfo(IN const s8* szClientOemMark,
		IN const u32 dwClinetIp/*主机序*/, IN const std::string& strClientISPName,
		IN const u32 dwLocalApsIp, IN const std::string& strLocalApsISPName,
		OUT CSrvAddrInfo& tUlcSrvAddr) const;

public:
    BOOL32 GetLocalApsIspName(IN u32 dwLocalIP/*主机序*/, OUT std::string& strLocalApsIspName);
	BOOL32 IsIpEqualIspName(IN u32 dwLocalIP/*主机序*/, IN  std::string& strIspName );

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
    
private:    //正常模式下的数据
	std::vector<CBriefSrvStateEx> m_vcNdsInfoEx;      //NDS(NTS)网络监测服务
	std::vector<CBriefSrvStateEx> m_vcNmsInfoEx;      //NMS网管
    std::vector<CBriefSrvStateEx> m_vcPasInfoEx;      //Pas接入
    std::vector<CBriefSrvStateEx> m_vcSusInfoEx;      //Sus

    std::vector<CBriefSrvStateEx> m_vcCmcInfoEx;      //CMC(meeting+PlatApi)
    std::vector<CBriefSrvStateEx> m_vcSnsInfoEx;      //SNS(weibo)
    std::vector<CBriefSrvStateEx> m_vcXnsInfoEx;      //Xns(XMPP)

    std::vector<CBriefSrvStateEx> m_vcNtpInfoEx;      //时间同步
    std::vector<CBriefSrvStateEx> m_vcVrsInfoEx;      //视频录播
    std::vector<CBriefSrvStateEx> m_vcDcsInfoEx;      //数据会议
	std::vector<CBriefSrvStateEx> m_vcRestInfoEx;     //RestApi

	std::vector<CBriefSrvStateEx> m_vcApsInfoEx;      //APS(XAP)(不需要状态)


private:    //pxy模式下的数据
    std::set<CPxyPlatformAddr> m_setPxyAddr;

private:
    CApsSrvIpList m_tApsSrvIpList;
	std::map<u32, std::vector<CBriefSrvStateEx>*> m_mapTypeServerList;
	std::set<u32> m_setNeedCheckStateServerList;
};

extern CSrvListMgr g_cSrvListMgr;

#endif  //end of _SRV_LIST_MGR_H_ 
