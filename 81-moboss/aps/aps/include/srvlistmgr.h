
/*=======================================================================
ģ����      :ULS
�ļ���      :srvlistmgr.h
����ļ�    :
�ļ�ʵ�ֹ���:�洢��3A��õķ�������Ϣ�����ն˷��������

����        :
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/07/08  1.0                       ����
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
//							 ��������                    
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
//							 ��ȡ����                    
/************************************************************************/
public:
    //��ȡƽ̨��������Ϣ(��½���������ն�E164)     
    BOOL32 GetPlatformSrvAddrInfo(IN const CLoginInfo& tLoginInfo, IN const std::string strClientISPName,
		IN const s8* szClientPasMOID, OUT CPlatformSrvAddrInfo& tSrvAddrInfo);
	const std::map<u32, std::vector<CBriefSrvStateEx>*> GetTypeServerListMap() const;

protected: //NormalMode
    BOOL32 GetPlatformSrvAddr(IN const CLoginInfo &tLoginInfo,
		IN const std::string strClientISPName, IN std::string strLocalApsIspName,
		IN const s8* szClientPasMOID , OUT CPlatformSrvAddrInfo &tSrvAddrInfo);

	//�������з�����ͨ�ú���
    BOOL32 AssignServerInfo(const u32 dwServerType, IN u32 dwClinetIp/*������*/, IN const std::string& strClientIspName,
		IN u32 dwLocalApsIp, IN const std::string& strLocalApsIspName, 
		IN const std::vector<CBriefSrvStateEx>& vcSrvInfoEx, OUT CSrvAddrInfo& tUlcSrvAddr,
		IN BOOL32 bChckState = TRUE, const s8* pchClientPasMOID = "",
		const s8* pchClientOemMark = "", const s8* pchDeviceType = "") const;

    //������ͨ������
    BOOL32 AssignSrvInfo(IN const u32 dwClinetIp/*������*/, IN const std::string& strClientIspName,
		IN const u32 dwLocalApsIp, IN const std::string& strLocalApsIspName,
		IN const std::vector<CBriefSrvStateEx>& vcSrvInfoEx, OUT CSrvAddrInfo& tUlcSrvAddr, IN BOOL32 bChckState = TRUE) const;
    //����PAS������
    BOOL32 AssignPasSrvInfo(IN const s8* szClientPasMOID, 
		IN const u32 dwClinetIp/*������*/, IN const std::string& strClientISPName,
		IN const u32 dwLocalApsIp, IN const std::string& strLocalApsISPName,
		OUT CSrvAddrInfo& tUlcSrvAddr) const;
    //����SUS������
    BOOL32 AssignSusSrvInfo(IN const s8* szClientOemMark,
		IN const u32 dwClinetIp/*������*/, IN const std::string& strClientISPName,
		IN const u32 dwLocalApsIp, IN const std::string& strLocalApsISPName,
		OUT CSrvAddrInfo& tUlcSrvAddr) const;

public:
    BOOL32 GetLocalApsIspName(IN u32 dwLocalIP/*������*/, OUT std::string& strLocalApsIspName);
	BOOL32 IsIpEqualIspName(IN u32 dwLocalIP/*������*/, IN  std::string& strIspName );

/************************************************************************/
//							 IP״̬���                    
/************************************************************************/
public:
    void CheckStateTimer();
    
protected:
    void CheckState(std::vector<CBriefSrvStateEx>& vcSrvInfoEx, u16 wPort);
	
/************************************************************************/
//							 ��ӡ����������Ϣ                    
/************************************************************************/
public:
	void PrintAllSrvsInfo();

protected:
	void PrintSrvInfo(std::vector<CBriefSrvStateEx>& vcSrvInfoEx);

private:
    RWLock m_tRWLock;
    s8 m_szMySelfMOID[OPERATION_MAXLEN_MOID+1];        //�Լ���MOID
    s8 m_szPlatformDomainMOID[OPERATION_MAXLEN_MOID+1];//����ƽ̨��MOID
    s8 m_szServiceDomainMOID[OPERATION_MAXLEN_MOID+1]; //����������MOID
    
private:    //����ģʽ�µ�����
	std::vector<CBriefSrvStateEx> m_vcNdsInfoEx;      //NDS(NTS)���������
	std::vector<CBriefSrvStateEx> m_vcNmsInfoEx;      //NMS����
    std::vector<CBriefSrvStateEx> m_vcPasInfoEx;      //Pas����
    std::vector<CBriefSrvStateEx> m_vcSusInfoEx;      //Sus

    std::vector<CBriefSrvStateEx> m_vcCmcInfoEx;      //CMC(meeting+PlatApi)
    std::vector<CBriefSrvStateEx> m_vcSnsInfoEx;      //SNS(weibo)
    std::vector<CBriefSrvStateEx> m_vcXnsInfoEx;      //Xns(XMPP)

    std::vector<CBriefSrvStateEx> m_vcNtpInfoEx;      //ʱ��ͬ��
    std::vector<CBriefSrvStateEx> m_vcVrsInfoEx;      //��Ƶ¼��
    std::vector<CBriefSrvStateEx> m_vcDcsInfoEx;      //���ݻ���
	std::vector<CBriefSrvStateEx> m_vcRestInfoEx;     //RestApi

	std::vector<CBriefSrvStateEx> m_vcApsInfoEx;      //APS(XAP)(����Ҫ״̬)


private:    //pxyģʽ�µ�����
    std::set<CPxyPlatformAddr> m_setPxyAddr;

private:
    CApsSrvIpList m_tApsSrvIpList;
	std::map<u32, std::vector<CBriefSrvStateEx>*> m_mapTypeServerList;
	std::set<u32> m_setNeedCheckStateServerList;
};

extern CSrvListMgr g_cSrvListMgr;

#endif  //end of _SRV_LIST_MGR_H_ 
