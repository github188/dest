
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
	//NDS(NTS)���б���Ϣ
    void InsertToNdsList(const std::set<CBriefSrvInfo> &setNtsInfo);
    void AddNdsInfo(const CBriefSrvInfo &tNtsInfo);
    void MdfNdsInfo(const CBriefSrvInfo &tNtsInfo);
	void DelNdsInfo(const CBriefSrvInfo &tNtsInfo);

	//NMS���б���Ϣ
    void InsertToNmsList(const std::set<CBriefSrvInfo> &setNmsInfo);
    void AddNmsInfo(const CBriefSrvInfo &tNmsInfo);
    void MdfNmsInfo(const CBriefSrvInfo &tNmsInfo);
    void DelNmsInfo(const CBriefSrvInfo &tNmsInfo);
    
    //APS(XAP/ULS)���б���Ϣ
    void InsertToApsList(const std::set<CBriefSrvInfo> &setApsInfo);
    void AddApsInfo(const CBriefSrvInfo &tApsInfo);
    void MdfApsInfo(const CBriefSrvInfo &tApsInfo);
    void DelApsInfo(const CBriefSrvInfo &tApsInfo);

	//PAS(GK/CSU/NU)���б���Ϣ
	void InsertToPasList(const std::set<CBriefSrvInfo> &setPasInfo);
    void AddPasInfo(const CBriefSrvInfo &tPasInfo);
    void MdfPasInfo(const CBriefSrvInfo &tPasInfo);
	void DelPasInfo(const CBriefSrvInfo &tPasInfo);

    //BGS(GK/CSU/NU)���б���Ϣ
    void InsertToBgsList(const std::set<CBriefSrvInfo> &setBgsInfo);
    void AddBgsInfo(const CBriefSrvInfo &tBgsInfo);
    void MdfBgsInfo(const CBriefSrvInfo &tBgsInfo);
    void DelBgsInfo(const CBriefSrvInfo &tBgsInfo);

    //SUS���б���Ϣ
    void InsertToSusList(const std::set<CBriefSrvInfo> &setSusInfo);
    void AddSusInfo(const CBriefSrvInfo &tAddSusInfo);
    void MdfSusInfo(const CBriefSrvInfo &tModifySusInfo);
    void DelSusInfo(const CBriefSrvInfo &tSusInfo);

    //Cmc(meeting)���б���Ϣ
    void InsertToCmcList(const std::set<CBriefSrvInfo> &setCmcInfo);
    void AddCmcInfo(const CBriefSrvInfo &tCmcInfo);
    void MdfCmcInfo(const CBriefSrvInfo &tCmcInfo);
    void DelCmcInfo(const CBriefSrvInfo &tCmcInfo);

    //SNS(weibo+PlatApi)���б���Ϣ
    void InsertToSnsList(const std::set<CBriefSrvInfo> &setSnsInfo);
    void AddSnsInfo(const CBriefSrvInfo &tSnsInfo);
    void MdfSnsInfo(const CBriefSrvInfo &tSnsInfo);
    void DelSnsInfo(const CBriefSrvInfo &tSnsInfo);

    //Xns���б���Ϣ
    void InsertToXnsList(const std::set<CBriefSrvInfo> &setXnsInfo);
    void AddXnsInfo(const CBriefSrvInfo &tXnsInfo);
    void MdfXnsInfo(const CBriefSrvInfo &tXnsInfo);
    void DelXnsInfo(const CBriefSrvInfo &tXnsInfo);

    //Ntp���б���Ϣ
    void InsertToNtpList(const std::set<CBriefSrvInfo> &setNtpInfo);
    void AddNtpInfo(const CBriefSrvInfo &tNtpInfo);
    void MdfNtpInfo(const CBriefSrvInfo &tNtpInfo);
    void DelNtpInfo(const CBriefSrvInfo &tNtpInfo);

    //Vrs���б���Ϣ
    void InsertToVrsList(const std::set<CBriefSrvInfo> &setVrsInfo);
    void AddVrsInfo(const CBriefSrvInfo &tVrsInfo);
    void MdfVrsInfo(const CBriefSrvInfo &tVrsInfo);
    void DelVrsInfo(const CBriefSrvInfo &tVrsInfo);

    //Dcs���б���Ϣ
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
//							 ��ȡ����                    
/************************************************************************/
public:
    //��ȡƽ̨��������Ϣ(��½���������ն�E164)     
    BOOL32 GetPlatformSrvAddrInfo(IN const CLoginInfo& tLoginInfo, IN const std::string strClientISPName, IN const s8* szClientPlatfromMOID, IN const s8* szClientPasMOID, OUT CPlatformSrvAddrInfo& tSrvAddrInfo);

protected: //NormalMode
    BOOL32 GetPlatformSrvAddr(IN const CLoginInfo &tLoginInfo, IN const std::string strClientISPName, IN std::string strLocalApsIspName, IN const s8* szClientPlatfromMOID, IN const s8* szClientPasMOID , OUT CPlatformSrvAddrInfo &tSrvAddrInfo);

    //������ͨ������
    BOOL32 AssignSrvInfo(IN u32 dwClinetIp/*������*/, IN const std::string& strClientIspName, IN const std::string& strLocalApsIspName, IN const s8* szClientPlatfromMOID, IN const std::vector<CBriefSrvStateEx>& vcSrvInfoEx, OUT CSrvAddrInfo& tUlcSrvAddr, IN BOOL32 bChckState = TRUE) const;
    //����PAS������
    BOOL32 AssignPasSrvInfo(IN const s8* szClientPasMOID, IN u32 dwClinetIp/*������*/, IN const std::string& strClientISPName, IN const std::string& strLocalApsISPName, IN const s8* szClientPlatfromMOID, OUT CSrvAddrInfo& tUlcSrvAddr) const;
    //����SUS������
    BOOL32 AssignSusSrvInfo(IN const s8* szClientOemMark, IN u32 dwClinetIp/*������*/, IN const std::string& strClientISPName, IN const std::string& strLocalApsISPName, IN const s8* szClientPlatfromMOID, OUT CSrvAddrInfo& tUlcSrvAddr) const;
    //����ULS������
    const CApsSrvIpList& AssignApsSrvList( );

public:
    BOOL32 GetLocalPasIspName(IN u32 dwLocalIP/*������*/, OUT std::string& strLocalApsIspName) const;
	BOOL32 IsIpEqualIspName(IN u32 dwLocalIP/*������*/, IN  std::string& strIspName );


public: //��Ԫ����
#ifdef _TEST_
    std::vector<CBriefSrvStateEx> GetSetSusInfoEx() const { return m_vcSusInfoEx; }
    std::vector<CBriefSrvStateEx> GetSetPasInfoEx() const { return m_vcPasInfoEx; }
#endif

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
	std::set<std::string> m_setPlatformDomainMOID;  //�����������µ�����ƽ̨��MOID
    
private:    //����ģʽ�µ�����
	std::vector<CBriefSrvStateEx> m_vcNdsInfoEx;      //NDS(NTS)
	std::vector<CBriefSrvStateEx> m_vcNmsInfoEx;      //NMS
    std::vector<CBriefSrvStateEx> m_vcPasInfoEx;      //Pas
    std::vector<CBriefSrvStateEx> m_vcBgsInfoEx;      //BGS
    std::vector<CBriefSrvStateEx> m_vcSusInfoEx;      //Sus

    std::vector<CBriefSrvStateEx> m_vcCmcInfoEx;      //CMC(meeting+PlatApi)
    std::vector<CBriefSrvStateEx> m_vcSnsInfoEx;      //SNS(weibo)
    std::vector<CBriefSrvStateEx> m_vcXnsInfoEx;      //Xns

    std::vector<CBriefSrvStateEx> m_vcNtpInfoEx;      //ʱ��ͬ��
    std::vector<CBriefSrvStateEx> m_vcVrsInfoEx;      //��Ƶ¼��
    std::vector<CBriefSrvStateEx> m_vcDcsInfoEx;      //���ݻ���
	std::vector<CBriefSrvStateEx> m_vcRestInfoEx;     //RestApi

	std::vector<CBriefSrvStateEx> m_vcApsInfoEx;      //APS(XAP)(����Ҫ״̬)


private:    //pxyģʽ�µ�����
    std::set<CPxyPlatformAddr> m_setPxyAddr;

private:
    CApsSrvIpList m_tApsSrvIpList;
    BOOL32 m_bApsSrvIpInited;   //m_tApsSrvIpList�Ƿ��Ѹ�ֵ
};

extern CSrvListMgr g_cSrvListMgr;

#endif  //end of _SRV_LIST_MGR_H_ 
