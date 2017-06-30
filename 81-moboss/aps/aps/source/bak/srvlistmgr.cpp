

#include "srvlistmgr.h"
#include "debugprint.h"
#include "connectsa_api.h"
#include "bossutils.h"
#include "structulc.h"
#include "structself.h"
#include "bossconst.h"
#include "config.h"
#include "sacommonconst.h"

CSrvListMgr g_cSrvListMgr;

/*lint -save -e1788*/
CSrvListMgr::CSrvListMgr()
{
    m_vcNdsInfoEx.clear();
    m_vcNmsInfoEx.clear();
    m_vcSusInfoEx.clear();
    m_vcPasInfoEx.clear();
    m_vcApsInfoEx.clear();
    m_vcCmcInfoEx.clear();
    m_vcSnsInfoEx.clear();
    m_vcXnsInfoEx.clear();
    m_vcNtpInfoEx.clear();
    m_vcVrsInfoEx.clear();
    m_vcDcsInfoEx.clear();
    m_vcBgsInfoEx.clear();
	m_vcRestInfoEx.clear();

    m_setPxyAddr.clear();

    memset(m_szMySelfMOID, 0, sizeof(m_szMySelfMOID));
    memset(m_szPlatformDomainMOID, 0, sizeof(m_szPlatformDomainMOID));
    memset(m_szServiceDomainMOID, 0, sizeof(m_szServiceDomainMOID));

    m_tApsSrvIpList.Init();
    m_bApsSrvIpInited = FALSE;  //m_tUlsSrvIpList��Ҫ���¸�ֵ

#ifdef _TEST_   //�Բ����
	//���Դ���   APS
	CBriefSrvInfo tApssInfo;
	tApssInfo.SetPlatformDomainMOID("0b3038f3-095d-4f58-9dde-3db43f2bbeea");
	tApssInfo.SetNetDomainName("xap-test.truesens.com");
	tApssInfo.SetMOID("102ee079-d536-44a6-adcc-9ae3be268c1c");
	tApssInfo.SetSrvIpAddr("�й�����:180.168.136.202;����:127.0.0.1;�й���ͨ:172.16.80.117;�й���ͨ:172.16.80.249");
	
	CBriefSrvStateEx tApsInfoEx;
    tApsInfoEx.SetDetailBody(tApssInfo);
    tApsInfoEx.InitState();
	m_vcApsInfoEx.push_back(tApsInfoEx); 

	//���Դ���  nts
	CBriefSrvInfo tNtsInfo;
	tNtsInfo.SetPlatformDomainMOID("0b3038f3-095d-4f58-9dde-3db43f2bbeea");
	tNtsInfo.SetNetDomainName("nts-test.truesens.com");
	tNtsInfo.SetMOID("10107000000001000000000000000013");
	tNtsInfo.SetSrvIpAddr("�й�����:180.168.136.202;����:172.16.177.158");
	
	CBriefSrvStateEx tNtsInfoEx;
    tNtsInfoEx.SetDetailBody(tNtsInfo);
    tNtsInfoEx.InitState();
	m_vcNdsInfoEx.push_back(tNtsInfoEx); 

    //���Դ���  nms
    CBriefSrvInfo tNmsInfo;
    tNmsInfo.SetPlatformDomainMOID("20200000000000000000000000000001");
    tNmsInfo.SetNetDomainName("nms-test.truesens.com");
    tNmsInfo.SetMOID("10107000000001000000000000000014");
    tNmsInfo.SetSrvIpAddr("�й�����:172.16.177.158;����:172.16.177.159");
    
    CBriefSrvStateEx tNmsInfoEx;
    tNmsInfoEx.SetDetailBody(tNmsInfo);
    tNmsInfoEx.InitState();
    m_vcNmsInfoEx.push_back(tNmsInfoEx); 


    //���Դ��� sus
    CBriefSrvInfo tSusInfo;
    tSusInfo.SetPlatformDomainMOID("20200000000000000000000000000001");
    tSusInfo.SetNetDomainName("sus-test.truesens.com");
    tSusInfo.SetMOID("10109000000008000000000000000025");
    tSusInfo.SetSrvIpAddr("�й�����:189.17.16.78;�й��ƶ�:193.233.78.34;����:172.16.177.168");
    tSusInfo.SetOemMarkList("kedacom");
    
    CBriefSrvStateEx tSusSrvInfoEx;
    tSusSrvInfoEx.SetDetailBody(tSusInfo);
    tSusSrvInfoEx.InitState();
	m_vcSusInfoEx.push_back(tSusSrvInfoEx);

    //���Դ��� PAS
    CBriefSrvInfo tPasInfo;
    tPasInfo.SetPlatformDomainMOID("20200000000000000000000000000008");
    tPasInfo.SetNetDomainName("sus-test.truesens.com");
    tPasInfo.SetMOID("10109000000008000000000000000026");
    tPasInfo.SetSrvIpAddr("�й�����:189.17.16.78;����:172.16.179.4");
    
    CBriefSrvStateEx tPasSrvInfoEx;
    tPasSrvInfoEx.SetDetailBody(tPasInfo);
    tPasSrvInfoEx.InitState();
	m_vcPasInfoEx.push_back(tPasSrvInfoEx);

    //���Դ��� xns
    CBriefSrvInfo tXnsInfo;
    tXnsInfo.SetPlatformDomainMOID("20200000000000000000000000000008");
    tXnsInfo.SetNetDomainName("sus-test.truesens.com");
    tXnsInfo.SetMOID("10109000000008000000000000000026");
    tXnsInfo.SetSrvIpAddr("�й�����:189.17.16.78;����:172.16.179.4");

    CBriefSrvStateEx tXnsSrvInfoEx;
    tXnsSrvInfoEx.SetDetailBody(tPasInfo);
    tXnsSrvInfoEx.InitState();
    m_vcXnsInfoEx.push_back(tXnsSrvInfoEx);
#endif
}

CSrvListMgr::~CSrvListMgr()
{

}

void CSrvListMgr::SetMySelfMOID( const s8 * szMOID )
{
    memset( m_szMySelfMOID, 0, sizeof(m_szMySelfMOID) );
    if (NULL != szMOID)
    {
        //�˴�������strncpy,GUID�п��ܰ���������0 ���ܵ��±��ض�
        memcpy( m_szMySelfMOID, szMOID, sizeof(m_szMySelfMOID)-1 );
        m_szMySelfMOID[sizeof(m_szMySelfMOID)-1] = '\0';
    }
    return;
}

void CSrvListMgr::SetPlatformDomainMOID( const s8 * szMOID )
{
    memset( m_szPlatformDomainMOID, 0, sizeof(m_szPlatformDomainMOID) );
    if (NULL != szMOID)
    {
        //�˴�������strncpy,GUID�п��ܰ���������0 ���ܵ��±��ض�
        memcpy( m_szPlatformDomainMOID, szMOID, sizeof(m_szPlatformDomainMOID)-1 );
        m_szPlatformDomainMOID[sizeof(m_szPlatformDomainMOID)-1] = '\0';
    }
    return;
}

void CSrvListMgr::SetServiceDomainMOID( const s8 * szMOID )
{
    memset( m_szServiceDomainMOID, 0, sizeof(m_szServiceDomainMOID) );
    if (NULL != szMOID)
    {
        //�˴�������strncpy,GUID�п��ܰ���������0 ���ܵ��±��ض�
        memcpy( m_szServiceDomainMOID, szMOID, sizeof(m_szServiceDomainMOID)-1 );
        m_szServiceDomainMOID[sizeof(m_szServiceDomainMOID)-1] = '\0';
    }
    return;
}

const s8* CSrvListMgr::GetServiceDomainMOID( ) const
{
    return m_szServiceDomainMOID;
}

void CSrvListMgr::SetAllPlatformDomainMOID(const std::set<std::string>& setDomainMOID)
{
	m_setPlatformDomainMOID = setDomainMOID;
}
std::set<std::string>& CSrvListMgr::GetAllPlatformDomainMOID() 
{
	return m_setPlatformDomainMOID;
}

/************************************************************************/
//							 NTS                    
/************************************************************************/

void CSrvListMgr::InsertToNdsList(const std::set<CBriefSrvInfo> &setNtsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToNdsList] NTS list num is %u !\n", setNtsInfo.size());
    AddToSrvList(setNtsInfo, m_vcNdsInfoEx);
    return;
}

void CSrvListMgr::AddNdsInfo(const CBriefSrvInfo &tNtsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    AddSrvInfo(tNtsInfo, m_vcNdsInfoEx);
    return;
}

void CSrvListMgr::MdfNdsInfo(const CBriefSrvInfo &tNtsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    MdfSrvInfo(tNtsInfo, m_vcNdsInfoEx);
    return;
}

void CSrvListMgr::DelNdsInfo(const CBriefSrvInfo &tNtsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    DelSrvInfo(tNtsInfo, m_vcNdsInfoEx);
    return;
}

/************************************************************************/
//							 NMS                    
/************************************************************************/
void CSrvListMgr::InsertToNmsList(const std::set<CBriefSrvInfo> &setNmsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsettToNmsList] NMS list num is %u !\n", setNmsInfo.size());
    AddToSrvList(setNmsInfo, m_vcNmsInfoEx);
    return;
}

void CSrvListMgr::AddNmsInfo(const CBriefSrvInfo &tNmsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    AddSrvInfo(tNmsInfo, m_vcNmsInfoEx);
    return;
}

void CSrvListMgr::MdfNmsInfo(const CBriefSrvInfo &tNmsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    MdfSrvInfo(tNmsInfo, m_vcNmsInfoEx);
    return;
}

void CSrvListMgr::DelNmsInfo(const CBriefSrvInfo &tNmsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    DelSrvInfo(tNmsInfo, m_vcNmsInfoEx);
    return;
}

/************************************************************************/
//							 PAS                    
/************************************************************************/

void CSrvListMgr::InsertToApsList(const std::set<CBriefSrvInfo> &setApsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToApsList] PAS list num is %u !\n", setApsInfo.size());

    m_bApsSrvIpInited = FALSE;  //m_tPasSrvIpList��Ҫ���¸�ֵ
    AddToSrvList(setApsInfo,  m_vcApsInfoEx);
    return;
}

void CSrvListMgr::AddApsInfo(const CBriefSrvInfo &tApsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    m_bApsSrvIpInited = FALSE;  //m_tPasSrvIpList��Ҫ���¸�ֵ
    AddSrvInfo(tApsInfo, m_vcApsInfoEx);
    return;
}

void CSrvListMgr::MdfApsInfo(const CBriefSrvInfo &tApsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    m_bApsSrvIpInited = FALSE;  //m_tPasSrvIpList��Ҫ���¸�ֵ
    MdfSrvInfo(tApsInfo, m_vcApsInfoEx);
    return;
}

void CSrvListMgr::DelApsInfo(const CBriefSrvInfo &tApsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
    
    m_bApsSrvIpInited = FALSE;  //m_tPasSrvIpList��Ҫ���¸�ֵ
    DelSrvInfo(tApsInfo, m_vcApsInfoEx);
    return;
}


/************************************************************************/
//							 SUS                    
/************************************************************************/

void CSrvListMgr::InsertToSusList(const std::set<CBriefSrvInfo> &setSusInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToSusList] Sus list num is %d:\n", setSusInfo.size());
    AddToSrvList(setSusInfo, m_vcSusInfoEx);
    return;
}

void CSrvListMgr::AddSusInfo(const CBriefSrvInfo &tAddSusInfo)
{
	CAutoWriteLock tWriteLock(m_tRWLock);
	
    AddSrvInfo(tAddSusInfo, m_vcSusInfoEx);
	return;
}

void CSrvListMgr::MdfSusInfo(const CBriefSrvInfo &tModifySusInfo)
{
	CAutoWriteLock tWriteLock(m_tRWLock);

    MdfSrvInfo(tModifySusInfo, m_vcSusInfoEx);
	return;
}

void CSrvListMgr::DelSusInfo(const CBriefSrvInfo &tDeleSusInfo)
{
	CAutoWriteLock tWriteLock(m_tRWLock);

    DelSrvInfo(tDeleSusInfo, m_vcSusInfoEx);
	return;
}

/************************************************************************/
//							 PAS                    
/************************************************************************/
void CSrvListMgr::InsertToPasList(const std::set<CBriefSrvInfo> &setPasInfo)
{
	CAutoWriteLock tWriteLock(m_tRWLock);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToPasList] PAS list num is %d:\n", setPasInfo.size());
    AddToSrvList(setPasInfo, m_vcPasInfoEx);
	return;
}

void CSrvListMgr::AddPasInfo(const CBriefSrvInfo &tAddPasInfo)
{
	CAutoWriteLock tWriteLock(m_tRWLock);

    AddSrvInfo(tAddPasInfo, m_vcPasInfoEx);
	return;
}

void CSrvListMgr::MdfPasInfo(const CBriefSrvInfo &tModyPasInfo)
{
	CAutoWriteLock tWriteLock(m_tRWLock);

    MdfSrvInfo(tModyPasInfo, m_vcPasInfoEx);
    return;
}

void CSrvListMgr::DelPasInfo(const CBriefSrvInfo &tDelePasInfo)
{
	CAutoWriteLock tWriteLock(m_tRWLock);

    DelSrvInfo(tDelePasInfo, m_vcPasInfoEx);
	return;
}


/************************************************************************/
//							 BGS                    
/************************************************************************/
void CSrvListMgr::InsertToBgsList(const std::set<CBriefSrvInfo> &setBgsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToBgsList] Bgs list num is %d:\n", setBgsInfo.size());
    AddToSrvList(setBgsInfo, m_vcBgsInfoEx);
    return;
}

void CSrvListMgr::AddBgsInfo(const CBriefSrvInfo &tAddBgsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    AddSrvInfo(tAddBgsInfo, m_vcBgsInfoEx);
    return;
}

void CSrvListMgr::MdfBgsInfo(const CBriefSrvInfo &tModyBgsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    MdfSrvInfo(tModyBgsInfo, m_vcBgsInfoEx);
    return;
}

void CSrvListMgr::DelBgsInfo(const CBriefSrvInfo &tDeleBgsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    DelSrvInfo(tDeleBgsInfo, m_vcBgsInfoEx);
    return;
}
/************************************************************************/
//							 CMC(meeting)                    
/************************************************************************/

void CSrvListMgr::InsertToCmcList(const std::set<CBriefSrvInfo> &setCmciInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToCmcList] Cmc list num is %d:\n", setCmciInfo.size());
    AddToSrvList(setCmciInfo, m_vcCmcInfoEx);
    return;
}

void CSrvListMgr::AddCmcInfo(const CBriefSrvInfo &tCmcInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    AddSrvInfo(tCmcInfo, m_vcCmcInfoEx);
    return;
}

void CSrvListMgr::MdfCmcInfo(const CBriefSrvInfo &tCmcInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    MdfSrvInfo(tCmcInfo, m_vcCmcInfoEx);
    return;
}

void CSrvListMgr::DelCmcInfo(const CBriefSrvInfo &tCmcInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    DelSrvInfo(tCmcInfo, m_vcCmcInfoEx);
    return;
}

/************************************************************************/
//							 SNS(weibo+PlatApi)                    
/************************************************************************/
void CSrvListMgr::InsertToSnsList(const std::set<CBriefSrvInfo> &setSnsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToSnsList] Sns list num is %d:\n", setSnsInfo.size());
    AddToSrvList(setSnsInfo, m_vcSnsInfoEx);
    return;
}

void CSrvListMgr::AddSnsInfo(const CBriefSrvInfo &tSnsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    AddSrvInfo(tSnsInfo, m_vcSnsInfoEx);
    return;
}

void CSrvListMgr::MdfSnsInfo(const CBriefSrvInfo &tSnsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    MdfSrvInfo(tSnsInfo, m_vcSnsInfoEx);
    return;
}

void CSrvListMgr::DelSnsInfo(const CBriefSrvInfo &tSnsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    DelSrvInfo(tSnsInfo, m_vcSnsInfoEx);
    return;
}

/************************************************************************/
//							 Xns                    
/************************************************************************/
void CSrvListMgr::InsertToXnsList(const std::set<CBriefSrvInfo> &setXnsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToXnsList] Xns list num is %d:\n", setXnsInfo.size());
    AddToSrvList(setXnsInfo, m_vcXnsInfoEx);
    return;
}

void CSrvListMgr::AddXnsInfo(const CBriefSrvInfo &tXnsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    AddSrvInfo(tXnsInfo, m_vcXnsInfoEx);
    return;
}

void CSrvListMgr::MdfXnsInfo(const CBriefSrvInfo &tXnsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    MdfSrvInfo(tXnsInfo, m_vcXnsInfoEx);
    return;
}

void CSrvListMgr::DelXnsInfo(const CBriefSrvInfo &tXnsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    DelSrvInfo(tXnsInfo, m_vcXnsInfoEx);
    return;
}


/************************************************************************/
//							 Ntp                    
/************************************************************************/
void CSrvListMgr::InsertToNtpList(const std::set<CBriefSrvInfo> &setNtpInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToNtpList] Ntp list num is %d:\n", setNtpInfo.size());
    AddToSrvList(setNtpInfo, m_vcNtpInfoEx);
    return;
}

void CSrvListMgr::AddNtpInfo(const CBriefSrvInfo &tNtpInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    AddSrvInfo(tNtpInfo, m_vcNtpInfoEx);
    return;
}

void CSrvListMgr::MdfNtpInfo(const CBriefSrvInfo &tNtpInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    MdfSrvInfo(tNtpInfo, m_vcNtpInfoEx);
    return;
}

void CSrvListMgr::DelNtpInfo(const CBriefSrvInfo &tNtpInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    DelSrvInfo(tNtpInfo, m_vcNtpInfoEx);
    return;
}

/************************************************************************/
//							 Vrs                    
/************************************************************************/
void CSrvListMgr::InsertToVrsList(const std::set<CBriefSrvInfo> &setVrsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToVrsList] Vrs list num is %d:\n", setVrsInfo.size());
    AddToSrvList(setVrsInfo, m_vcVrsInfoEx);
    return;
}

void CSrvListMgr::AddVrsInfo(const CBriefSrvInfo &tVrsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    AddSrvInfo(tVrsInfo, m_vcVrsInfoEx);
    return;
}

void CSrvListMgr::MdfVrsInfo(const CBriefSrvInfo &tVrsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    MdfSrvInfo(tVrsInfo, m_vcVrsInfoEx);
    return;
}

void CSrvListMgr::DelVrsInfo(const CBriefSrvInfo &tVrsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    DelSrvInfo(tVrsInfo, m_vcVrsInfoEx);
    return;
}



/************************************************************************/
//							 RestApi                    
/************************************************************************/
void CSrvListMgr::InsertToRestList(const std::set<CBriefSrvInfo> &setRestInfo)
{
	CAutoWriteLock tWriteLock(m_tRWLock);

	SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToRestList] Rest list num is %d:\n", setRestInfo.size());
	AddToSrvList( setRestInfo, m_vcRestInfoEx);
	return;
}

void CSrvListMgr::AddRestInfo(const CBriefSrvInfo &tRestInfo)
{
	CAutoWriteLock tWriteLock(m_tRWLock);

	AddSrvInfo(tRestInfo, m_vcRestInfoEx);
	return;
}

void CSrvListMgr::MdfRestInfo(const CBriefSrvInfo &tRestInfo)
{
	CAutoWriteLock tWriteLock(m_tRWLock);

	MdfSrvInfo(tRestInfo, m_vcRestInfoEx);
	return;
}

void CSrvListMgr::DelRestInfo(const CBriefSrvInfo &tRestInfo)
{
	CAutoWriteLock tWriteLock(m_tRWLock);

	DelSrvInfo(tRestInfo, m_vcRestInfoEx);
	return;
}


/************************************************************************/
//							 Dcs                    
/************************************************************************/
void CSrvListMgr::InsertToDcsList(const std::set<CBriefSrvInfo> &setDcsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[InsertToDcsList] Dcs list num is %d:\n", setDcsInfo.size());
    AddToSrvList(setDcsInfo, m_vcDcsInfoEx);
    return;
}

void CSrvListMgr::AddDcsInfo(const CBriefSrvInfo &tDcsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    AddSrvInfo(tDcsInfo, m_vcDcsInfoEx);
    return;
}

void CSrvListMgr::MdfDcsInfo(const CBriefSrvInfo &tDcsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    MdfSrvInfo(tDcsInfo, m_vcDcsInfoEx);
    return;
}

void CSrvListMgr::DelDcsInfo(const CBriefSrvInfo &tDcsInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);

    DelSrvInfo(tDcsInfo, m_vcDcsInfoEx);
    return;
}

/************************************************************************/
//							                     
/************************************************************************/
void CSrvListMgr::AddSrvInfo(IN const CBriefSrvInfo& tSrvInfo, IN OUT std::vector<CBriefSrvStateEx>& vcSrvEx)
{
    //find
    std::vector<CBriefSrvStateEx>::iterator iter = vcSrvEx.begin();
    for ( ; iter != vcSrvEx.end(); ++iter )
    {
        if ( 0 == strcmp(tSrvInfo.GetMOID(), iter->GetDetailBody().GetMOID()) )
        {
            SrvListMgrLog(LOG_LVL_WARNING, "[AddSrvInfo] Srv:%s is existed, Add failed!\n", tSrvInfo.GetMOID());
            tSrvInfo.Print();
            return;
        }
    }

	/*
	 *	��Ӳ���ֻ����ԭ�����������ӵ�ַ�������ԭ���ĵ�ַ��Ӱ��
	 */
    CBriefSrvStateEx tSrvInfoEx;
    tSrvInfoEx.SetDetailBody(tSrvInfo);
    tSrvInfoEx.InitState();
    vcSrvEx.push_back(tSrvInfoEx);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[AddSrvInfo] Add Srv succeed !\n");
    tSrvInfo.Print();
    return;	
}

void CSrvListMgr::MdfSrvInfo(IN const CBriefSrvInfo& tModifySrvInfo, IN OUT std::vector<CBriefSrvStateEx>& vcSrvEx)
{
    //find
    std::vector<CBriefSrvStateEx>::iterator iter = vcSrvEx.begin();
    for ( ; iter != vcSrvEx.end(); ++iter )
    {
        if ( 0 == strcmp(tModifySrvInfo.GetMOID(), iter->GetDetailBody().GetMOID()) )
        {
            break;
        }
    }

    if (iter == vcSrvEx.end())
    {
        SrvListMgrLog(LOG_LVL_ERROR, "[MdfSrvInfo] Srv(%s) is not existed, modify failed!\n", tModifySrvInfo.GetMOID());
        tModifySrvInfo.Print();
        return;
    }
    
	/*
	 *	1.����ԭ�ڴ��ڲ���ĵ�ַ�����¼ӵĵ�ַ�����޸�״̬����Ϊ��ʱ��������������
	 *  2.����ԭ�ڴ�����Ҫ��ɾ���ĵ�ַ����Ҫ���������Node
	 */
	CNetAddrInfo cAddr;
	u32 dwOldNode = INVALID_NODE;
	for (u8 byLoop = 0; byLoop < OPERATION_MAXNUM_ISP; byLoop++)
	{
		iter->GetDetailBody().GetSrvAddr(byLoop, cAddr);
		if ( tModifySrvInfo.IsIpExist(cAddr.GetIP()) )
		{
			continue;
		}
		//�����ڴ���ԭ�е�ַ�Ѿ���ɾ������Ҫ�رն�Ӧ�Ľ��
		dwOldNode = iter->GetIpOspNode(byLoop);
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]Server:%s not exist, close its node.%u\n", __FUNCTION__, BossUtils::GetIpString(cAddr.GetIP()).c_str(), dwOldNode);
		OspDisconnectTcpNode(dwOldNode);
		iter->SetIpOspNode(byLoop, INVALID_NODE);

	}

    //���¼���
	// 	���ڱ����ڴ棬ֱ�ӵ�SetDetailBody���򵥣����ٵ������� [12/11/2015 pengguofeng]
    iter->SetDetailBody(tModifySrvInfo);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[MdfSrvInfo] Srv(%s) is modify succ!\n", tModifySrvInfo.GetMOID());
    tModifySrvInfo.Print();
    
    return;
}

void CSrvListMgr::DelSrvInfo(IN const CBriefSrvInfo& tDeletSrvInfo, IN OUT std::vector<CBriefSrvStateEx>& vcSrvEx)
{
    //find
    std::vector<CBriefSrvStateEx>::iterator iter = vcSrvEx.begin();
    for ( ; iter != vcSrvEx.end(); ++iter )
    {
        if ( 0 == strcmp(tDeletSrvInfo.GetMOID(), iter->GetDetailBody().GetMOID()) )
        {
            break;
        }
    }

    if (iter == vcSrvEx.end())
    {
        SrvListMgrLog(LOG_LVL_ERROR, "[DelSrvInfo] Srv(%s) is not existed, modify failed!\n", tDeletSrvInfo.GetMOID());
        tDeletSrvInfo.Print();
        return;
    }
     
	/*
	 *	ɾ���ض���ɾ�����е�ַ
	 */
    //������
    CloseOspNodes(*iter);
    vcSrvEx.erase(iter);

    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[DelSrvInfo] Srv(%s) delet succ!\n", tDeletSrvInfo.GetMOID());
    return;
}

void CSrvListMgr::AddToSrvList(IN const std::set<CBriefSrvInfo> &setSrvInfo, IN OUT std::vector<CBriefSrvStateEx>& vcSrvEx)
{
    std::set<CBriefSrvInfo>::const_iterator iter = setSrvInfo.begin();
    for ( ; iter != setSrvInfo.end(); ++iter )
    {
        AddSrvInfo(*iter, vcSrvEx);
    }
    
    return;
}

void CSrvListMgr::CloseOspNodes(IN CStateBase& tSrvStateEx)
{
    for (u32 dwIndex = 0; dwIndex < OPERATION_MAXNUM_ISP; ++dwIndex)
    {
        u32 dwOspNode = tSrvStateEx.GetIpOspNode(dwIndex);
        OspDisconnectTcpNode(dwOspNode);
    }
}


/************************************************************************/
//							 IP״̬���                    
/************************************************************************/
void CSrvListMgr::CheckStateTimer()
{
    if (g_cCfgFile.IsPxyMode())
    {
        return; //Pxy Mode�²���Ҫ���״̬
    }


    CAutoReadLock tReadLock(m_tRWLock); //��Ϊֻ��ı�״̬��Ϣ��Ϊ�����Ч�ʣ�ʹ�ö������������ʲô����

    //����1.���ַ�����û�н������˿�
    //    2.ping���п��ܱ�����
    //����ͳһʹ�ü��guard�ķ�ʽ����guard����Ϊ��̨�����ϵ�ҵ���Ǻõ�

    //NTS
    CheckState(m_vcNdsInfoEx, SA_LISTEN_PORT);

    //NMS
    CheckState(m_vcNmsInfoEx, SA_LISTEN_PORT);

    //Sus
    CheckState(m_vcSusInfoEx, SA_LISTEN_PORT);

    //Pas
    CheckState(m_vcPasInfoEx, SA_LISTEN_PORT);

    //BGS���ü��
    //CheckState(m_vcBgsInfoEx, SA_LISTEN_PORT);

    //Cmc
    CheckState(m_vcCmcInfoEx, SA_LISTEN_PORT);

    //SNS(weibo+meeting)
    CheckState(m_vcSnsInfoEx, SA_LISTEN_PORT);

    //Xns
    CheckState(m_vcXnsInfoEx, SA_LISTEN_PORT);

    //Ntp
    CheckState(m_vcNtpInfoEx, SA_LISTEN_PORT);

    //Aps���ü��
    //CheckState(m_vcApsInfoEx, SA_LISTEN_PORT);

    //Vrs���ü��
    //CheckState(m_vcVrsInfoEx, SA_LISTEN_PORT);

    //Dcs���ü��
    //CheckState(m_vcDcsInfoEx, SA_LISTEN_PORT);
    return;    
}

void CSrvListMgr::CheckState(std::vector<CBriefSrvStateEx>& vcSrvInfoEx, u16 wPort)
{
    std::vector<CBriefSrvStateEx>::iterator iter = vcSrvInfoEx.begin();

	u32 dwNode = 0;
    for ( ; iter != vcSrvInfoEx.end(); ++iter )
    {
        for (u32 dwIndex = 0; dwIndex < OPERATION_MAXNUM_ISP; ++dwIndex)
        {
            CNetAddrInfo tAddr;
            BOOL32 bRet = iter->GetDetailBody().GetSrvAddr(dwIndex, tAddr);
            if (!bRet || !tAddr.IsValid())
            {
                continue;
            }

			dwNode = iter->GetIpOspNode(dwIndex);
			if ( dwNode != INVALID_NODE
				&& OspIsValidTcpNode(dwNode) )
			{
				continue;
			}

			//���³�����Ҫ������
			//1. dwNode == 0
			//2. dwNode != 0 ����Node����Ч
			if ( dwNode != INVALID_NODE )
			{
				OspDisconnectTcpNode(dwNode);
			}

            //������������
            dwNode = OspConnectTcpNode(htonl(tAddr.GetIP()), wPort, MOBOSS_HB_TIME, MOBOSS_HB_NUM, 1*1000);			            
            //�޸ı������ڴ��е�node
            iter->SetIpOspNode(dwIndex, dwNode);
        }
    }
}

/************************************************************************/
//							 ��ӡ����������Ϣ                   
/************************************************************************/
void CSrvListMgr::PrintAllSrvsInfo()
{
#ifdef _APS_PXY_
	OspPrintf(TRUE, FALSE, "------------start------------\n");
	OspPrintf(TRUE, FALSE, "IspName:%s  <--->  IspType:%u\n", ISP_CN_TELECOM, emTypeZGDX);
	OspPrintf(TRUE, FALSE, "IspName:%s  <--->  IspType:%u\n", ISP_CN_UNICOM, emTypeZGLT);
	OspPrintf(TRUE, FALSE, "IspName:%s  <--->  IspType:%u\n", ISP_CN_MOBILE, emTypeZGYD);
	OspPrintf(TRUE, FALSE, "IspName:%s  <--->  IspType:%u\n", ISP_CN_WIRED, emTypeYXT);
	OspPrintf(TRUE, FALSE, "IspName:%s  <--->  IspType:%u\n", ISP_CN_CTT, emTypeTT);
	OspPrintf(TRUE, FALSE, "IspName:%s  <--->  IspType:%u\n", ISP_OVERSEAS, emTypeHW);
	OspPrintf(TRUE, FALSE, "IspName:%s  <--->  IspType:%u\n", ISP_CN_LOCAL, emTypeBD);
	OspPrintf(TRUE, FALSE, "IspName:%s  <--->  IspType:%u\n", ISP_OTHER, emTypeQT);
	OspPrintf(TRUE, FALSE, "------------end ------------\n");
	g_cCfgFile.PrintHaPxyInfo();
	return;
#endif

    if (g_cCfgFile.IsPxyMode())
    {
        OspPrintf(TRUE, FALSE, "current is Pxy Mode\n");
		std::set<CPxyPlatformAddr>::iterator itPxy = m_setPxyAddr.begin();
		while ( itPxy != m_setPxyAddr.end() )
		{
			(*itPxy).Print();
			itPxy++;
		}
        return;
    }

	CAutoReadLock tReadLock(m_tRWLock);
	
    OspPrintf(TRUE, FALSE, "***************************** ALL srv info ***************************\n\n");
	
	//nts
    OspPrintf(TRUE, FALSE, "\n1.NTS info******************\n");
	PrintSrvInfo(m_vcNdsInfoEx);
	
	//nms
	OspPrintf(TRUE, FALSE, "\n2.NMS info******************\n");
	PrintSrvInfo(m_vcNmsInfoEx);

	//aps
	OspPrintf(TRUE, FALSE, "\n3.APS info******************\n");
	PrintSrvInfo(m_vcApsInfoEx);
	
	//sus
	OspPrintf(TRUE, FALSE, "\n4.SUS info******************\n");
	PrintSrvInfo(m_vcSusInfoEx);
	
	//Pas
	OspPrintf(TRUE, FALSE, "\n5.PAS info******************\n");
	PrintSrvInfo(m_vcPasInfoEx);


    //Cmc
    OspPrintf(TRUE, FALSE, "\n6.CMC(meeting+PlatApi) info******************\n");
    PrintSrvInfo(m_vcCmcInfoEx);

    //SNS
    OspPrintf(TRUE, FALSE, "\n7.SNS(weibo) info******************\n");
    PrintSrvInfo(m_vcSnsInfoEx);

    //XNS
    OspPrintf(TRUE, FALSE, "\n8.XNS info******************\n");
    PrintSrvInfo(m_vcXnsInfoEx);
    
    //NTP
    OspPrintf(TRUE, FALSE, "\n9.NTP info******************\n");
    PrintSrvInfo(m_vcNtpInfoEx);

    //VRS
    OspPrintf(TRUE, FALSE, "\n10.VRS info******************\n");
    PrintSrvInfo(m_vcVrsInfoEx);

    //DCS
    OspPrintf(TRUE, FALSE, "\n11.DCS info******************\n");
    PrintSrvInfo(m_vcDcsInfoEx);

    //Bgs
    OspPrintf(TRUE, FALSE, "\n12.BGS info******************\n");
    PrintSrvInfo(m_vcBgsInfoEx);

	OspPrintf(TRUE, FALSE, "\n13.RestApi info******************\n");
	PrintSrvInfo(m_vcRestInfoEx);
	
	OspPrintf(TRUE, FALSE, "\n**************************************************************************\n\n");
}

void CSrvListMgr::PrintSrvInfo(std::vector<CBriefSrvStateEx>& vcSrvInfoEx)
{
	std::vector<CBriefSrvStateEx>::iterator iter = vcSrvInfoEx.begin();
	std::vector<CBriefSrvStateEx>::iterator iterEnd = vcSrvInfoEx.end();
	for ( ; iter != iterEnd; ++iter)
	{
		CBriefSrvStateEx tSrvStateEx = *iter;
		CBriefSrvInfo tSrvInfo = tSrvStateEx.GetDetailBody();
		OspPrintf(TRUE, TRUE, " SrvGUID(%s)\n NetDomainName(%s)\n PlatformDomainMOID(%s)\n", 
			tSrvInfo.GetMOID(), tSrvInfo.GetNetDomainName(), tSrvInfo.GetPlatformDomainMOID());

        std::string strOem;
        COemMarkList  tList = tSrvInfo.GetOemMarkList();
        for (u32 dwListIdx = 0; dwListIdx < tList.Size(); ++dwListIdx)
        {
           COemMark tMark = tList.GetValue(dwListIdx);
           strOem += tMark.GetOemMarkString();
        }
        OspPrintf(TRUE, TRUE, " OEM Mark(%s)\n", strOem.c_str());

		
		for(u32 dwIndex = 0; dwIndex < OPERATION_MAXNUM_ISP; ++dwIndex)
		{
			CNetAddrInfo tAddr;
			BOOL32 bRet = tSrvStateEx.GetDetailBody().GetSrvAddr(dwIndex, tAddr);
			if (!bRet || !tAddr.IsValid())
			{
				continue;
			}
			std::string strIsp = tAddr.GetOperatorName();
			OspPrintf(TRUE, FALSE, " OperatorName(%s) OperatorType(%d) IP(%s) state(%d) Node(%u)\n",
				tAddr.GetOperatorName(), TransIspNameToType(strIsp),
				BossUtils::GetIpString(tAddr.GetIP()).c_str(), tSrvStateEx.GetIpState(dwIndex), tSrvStateEx.GetIpOspNode(dwIndex));
		}
		OspPrintf(TRUE, TRUE, "\n\n");
	}
}


/************************************************************************/
//							 ��ȡ����                    
/************************************************************************/
BOOL32 CSrvListMgr::IsIpEqualIspName(IN u32 dwLocalIP/*������*/, IN  std::string& strIspName)
{
//strLocalApsIspName = "unknow";
    
    //find
    std::vector<CBriefSrvStateEx>::const_iterator iter = m_vcApsInfoEx.begin();
    for ( ; iter != m_vcApsInfoEx.end(); ++iter )
    {
        if ( 0 == strcmp(iter->GetDetailBody().GetMOID(), m_szMySelfMOID) )
        {
            break;
        }
    }
    if (iter == m_vcApsInfoEx.end())
    {
        SrvListMgrLog(LOG_LVL_WARNING, "[GetLocalPasIspName] Get local APS(%s) info is failed!\n", m_szMySelfMOID);
        return FALSE;
    }   

    CBriefSrvInfo tSelfInfo;  
    tSelfInfo = iter->GetDetailBody();

	OspPrintf(1,0,"[IsIpEqualIspName] strIspName.%s\n",strIspName.c_str() ); 
	
	CNetAddrInfo tSrvAddr;
    for (u8 byIndex = 0; byIndex < OPERATION_MAXNUM_ISP; byIndex++)
    {
        tSrvAddr.Clear();
        BOOL32 bRet = tSelfInfo.GetSrvAddr(byIndex, tSrvAddr);
		OspPrintf(1,0,"[IsIpEqualIspName] tSrvAddr.GetOperatorName().%s\n",tSrvAddr.GetOperatorName());
		if( strcasecmp( tSrvAddr.GetOperatorName(),strIspName.c_str() ) == 0 )
		{
			OspPrintf(1,0,"[IsIpEqualIspName] dwLocalIP.%08x tSrvAddr.%08x\n",dwLocalIP,tSrvAddr.GetIP() );
			
			if( dwLocalIP != tSrvAddr.GetIP() )
			{
				return FALSE;
			}
			return TRUE;
		}        
    }

	return FALSE;
}

BOOL32 CSrvListMgr::GetLocalPasIspName(IN u32 dwLocalIP/*������*/, OUT std::string& strLocalApsIspName) const
{
    //strLocalApsIspName.clear();
    strLocalApsIspName = "unknow";
    
    //find
    std::vector<CBriefSrvStateEx>::const_iterator iter = m_vcApsInfoEx.begin();
    for ( ; iter != m_vcApsInfoEx.end(); ++iter )
    {
        if ( 0 == strcmp(iter->GetDetailBody().GetMOID(), m_szMySelfMOID) )
        {
            break;
        }
    }
    if (iter == m_vcApsInfoEx.end())
    {
        SrvListMgrLog(LOG_LVL_WARNING, "[GetLocalPasIspName] Get local APS(%s) info is failed!\n", m_szMySelfMOID);
        return FALSE;
    }   

    CBriefSrvInfo tSelfInfo;  
    tSelfInfo = iter->GetDetailBody();

    
    //���ݱ���APS��Ip��ַ����
    CNetAddrInfo tSrvAddr;
    for (u8 byIndex = 0; byIndex < OPERATION_MAXNUM_ISP; byIndex++)
    {
        tSrvAddr.Clear();
        BOOL32 bRet = tSelfInfo.GetSrvAddr(byIndex, tSrvAddr);
        if (bRet && dwLocalIP == tSrvAddr.GetIP())
        {
            //��¼����APS����Ӫ����Ϣ
            strLocalApsIspName = tSrvAddr.GetOperatorName();
            
            SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetLocalPasIspName] Local APS(%s) info: Ip(%s), OperatorName(%s)\n", 
                tSelfInfo.GetMOID(), BossUtils::GetIpString(dwLocalIP).c_str(), tSrvAddr.GetOperatorName());            
            return TRUE;
        }
    }
   
    SrvListMgrLog(LOG_LVL_WARNING, "[GetLocalPasIspName] Local APS(%s) Ip(%s)�޷�ƥ����Ӫ��\n", 
        tSelfInfo.GetMOID(), BossUtils::GetIpString(dwLocalIP).c_str());     
    return FALSE;
}

BOOL32 CSrvListMgr::GetPlatformSrvAddrInfo(IN const CLoginInfo& tLoginInfo, IN const std::string strClientISPName,
										   IN const s8* szClientPlatfromMOID, IN const s8* szClientPasMOID,
										   OUT CPlatformSrvAddrInfo& tSrvAddrInfo)
{
    CAutoReadLock tReadLock(m_tRWLock);

    std::string strLocalApsIspName;
    GetLocalPasIspName(tLoginInfo.GetApsLocalIp(), strLocalApsIspName);

    BOOL32 bRet = GetPlatformSrvAddr(tLoginInfo, strClientISPName, strLocalApsIspName, szClientPlatfromMOID, szClientPasMOID, tSrvAddrInfo);

    return bRet;
}

/*
 *	��ʹ�Ҳ���ĳһ����ַ��Ҳ��Ҫֱ�ӷ���False���Ӹ���ӡ��ʾ����
 */
BOOL32 CSrvListMgr::GetPlatformSrvAddr( const CLoginInfo &tLoginInfo, const std::string strClientISPName,
									   std::string strLocalApsIspName, const s8* szClientPlatfromMOID,
									   const s8* szClientPasMOID, CPlatformSrvAddrInfo &tSrvAddrInfo )
{
    //NDS(NTS)
    CSrvAddrInfo tNdsSrvAddr;
    BOOL32 bRet = AssignSrvInfo(tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, m_vcNdsInfoEx, tNdsSrvAddr);
    if (!bRet)
    {
		SrvListMgrLog(LOG_LVL_ERROR, "[%s]Find Nds Addr failed\n", __FUNCTION__);
    }
    tNdsSrvAddr.SetPort(g_cCfgFile.GetNdsPort());

    tSrvAddrInfo.SetNdsAddrInfo(tNdsSrvAddr);
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Nts Addr(%s:%u), DomainName(%s) succeed\n",
		BossUtils::GetIpString(tNdsSrvAddr.GetIpAddr()).c_str(), tNdsSrvAddr.GetPort(), tNdsSrvAddr.GetNetDomainName());

    //NMS
    CSrvAddrInfo tNmsSrvAddr;
    bRet = AssignSrvInfo(tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, m_vcNmsInfoEx, tNmsSrvAddr);
    if (!bRet)
    {
		SrvListMgrLog(LOG_LVL_ERROR, "[%s]Find Nms Addr failed\n", __FUNCTION__);
    }
    tNmsSrvAddr.SetPort(g_cCfgFile.GetNmsPort());

    tSrvAddrInfo.SetNmsAddrInfo(tNmsSrvAddr);
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Nms Addr(%s:%u), DomainName(%s) succeed\n",
		BossUtils::GetIpString(tNmsSrvAddr.GetIpAddr()).c_str(), tNmsSrvAddr.GetPort(), tNmsSrvAddr.GetNetDomainName());

    //Sns
    CSrvAddrInfo tSnsSrvAddr;
    bRet = AssignSrvInfo(tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, m_vcSnsInfoEx, tSnsSrvAddr);
    if (!bRet)
    {
		SrvListMgrLog(LOG_LVL_ERROR, "[%s]Find Sns Addr failed\n", __FUNCTION__);
    }
    tSnsSrvAddr.SetPort(g_cCfgFile.GetSnsPort());

    tSrvAddrInfo.SetWeiboAddrInfo(tSnsSrvAddr);
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Sns Addr(%s:%u), DomainName(%s) succeed\n",
		BossUtils::GetIpString(tSnsSrvAddr.GetIpAddr()).c_str(), tSnsSrvAddr.GetPort(), tSnsSrvAddr.GetNetDomainName());

    //Cmc
    CSrvAddrInfo tCmcSrvAddr;
    bRet = AssignSrvInfo(tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, m_vcCmcInfoEx, tCmcSrvAddr);
    if (!bRet)
    {
		SrvListMgrLog(LOG_LVL_ERROR, "[%s]Find Cmc Addr failed\n", __FUNCTION__);
    }
    tCmcSrvAddr.SetPort(g_cCfgFile.GetCmcPort());

    tSrvAddrInfo.SetMeetingAddrInfo(tCmcSrvAddr);
    tSrvAddrInfo.SetPlatApiAddrInfo(tCmcSrvAddr);   //platAPI��mettingĿǰ�Ǻϲ���
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Cmc Addr(%s:%u), DomainName(%s) succeed\n",
		BossUtils::GetIpString(tCmcSrvAddr.GetIpAddr()).c_str(), tCmcSrvAddr.GetPort(), tCmcSrvAddr.GetNetDomainName());

    //Xns
    CSrvAddrInfo tXnsSrvAddr;
    bRet = AssignSrvInfo(tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, m_vcXnsInfoEx, tXnsSrvAddr);
    if (!bRet)
    {
		SrvListMgrLog(LOG_LVL_ERROR, "[%s]Find Xns Addr failed\n", __FUNCTION__);
    }
    tXnsSrvAddr.SetPort(g_cCfgFile.GetXnsPort());

    tSrvAddrInfo.SetXmppAddrInfo(tXnsSrvAddr);
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Xns Addr(%s:%u), DomainName(%s) succeed\n",
		BossUtils::GetIpString(tXnsSrvAddr.GetIpAddr()).c_str(), tXnsSrvAddr.GetPort(), tXnsSrvAddr.GetNetDomainName());

	if ( tXnsSrvAddr.GetIpAddr() == 0 )
	{
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Xns Addr failed, return False\n");
		return FALSE;
	}
    //Ntp
    CSrvAddrInfo tNtpSrvAddr;
    bRet = AssignSrvInfo(tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, m_vcNtpInfoEx, tNtpSrvAddr);
    if (!bRet)
    {
		SrvListMgrLog(LOG_LVL_ERROR, "[%s]Find Ntp Addr failed\n", __FUNCTION__);
    }
    tNtpSrvAddr.SetPort(g_cCfgFile.GetNtpPort());

    tSrvAddrInfo.SetNtpAddrInfo(tNtpSrvAddr);
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Ntp Addr(%s:%u), DomainName(%s) succeed\n",
		BossUtils::GetIpString(tNtpSrvAddr.GetIpAddr()).c_str(), tNtpSrvAddr.GetPort(), tNtpSrvAddr.GetNetDomainName());

    //Vrs(�����״̬)
    CSrvAddrInfo tVrsSrvAddr;
    bRet = AssignSrvInfo(tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, m_vcVrsInfoEx, tVrsSrvAddr, FALSE);
    if (!bRet)
    {
		SrvListMgrLog(LOG_LVL_ERROR, "[%s]Find Vrs Addr failed\n", __FUNCTION__);
    }
    tVrsSrvAddr.SetPort(g_cCfgFile.GetVrsPort());

    tSrvAddrInfo.SetVrsAddrInfo(tVrsSrvAddr);
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Vrs Addr(%s:%u), DomainName(%s) succeed\n",
		BossUtils::GetIpString(tVrsSrvAddr.GetIpAddr()).c_str(), tVrsSrvAddr.GetPort(), tVrsSrvAddr.GetNetDomainName());

    //Dcs(�����״̬)
    CSrvAddrInfo tDcsSrvAddr;
    bRet = AssignSrvInfo(tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, m_vcDcsInfoEx, tDcsSrvAddr, FALSE);
    if (!bRet)
    {
		SrvListMgrLog(LOG_LVL_ERROR, "[%s]Find Dcs Addr failed\n", __FUNCTION__);
    }
    tDcsSrvAddr.SetPort(g_cCfgFile.GetDcsPort());

    tSrvAddrInfo.SetDcsAddrInfo(tDcsSrvAddr);
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Dcs Addr(%s:%u), DomainName(%s) succeed\n",
		BossUtils::GetIpString(tDcsSrvAddr.GetIpAddr()).c_str(), tDcsSrvAddr.GetPort(), tDcsSrvAddr.GetNetDomainName());

	//RestApi(�����״̬)
	CSrvAddrInfo tRestSrvAddr;
	bRet = AssignSrvInfo(tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, m_vcRestInfoEx, tRestSrvAddr, FALSE);
	if (!bRet)
	{
		SrvListMgrLog(LOG_LVL_ERROR, "[%s]Find RestApi Addr failed\n", __FUNCTION__);
	}
	tRestSrvAddr.SetPort(g_cCfgFile.GetRestApiPort());

	tSrvAddrInfo.SetRestAddrInfo(tRestSrvAddr);
	SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Rest Addr(%s:%u), DomainName(%s) succeed\n",
		BossUtils::GetIpString(tRestSrvAddr.GetIpAddr()).c_str(), tRestSrvAddr.GetPort(), tRestSrvAddr.GetNetDomainName());

    //PAS(GK/CSU/NU)
    CSrvAddrInfo tPasSrvAddr;
    //�����������ĵ�����
    if (   0 == strcasecmp("SKY X500 1080P60", tLoginInfo.GetDeviceType())
        || 0 == strcasecmp("SKY X500 1080P30", tLoginInfo.GetDeviceType())
        || 0 == strcasecmp("SKY X500 720P60", tLoginInfo.GetDeviceType())
		|| 0 == strcasecmp("TrueSens X500 1080P", tLoginInfo.GetDeviceType())
		|| 0 == strcasecmp("TrueSens X500 720", tLoginInfo.GetDeviceType())
		|| 0 == strcasecmp("OEM X500 1080P60", tLoginInfo.GetDeviceType())
		|| 0 == strcasecmp("OEM X500 1080P60 MO", tLoginInfo.GetDeviceType()) )
    {
        //��H265�������ն� ��BGS��ַαװ��PAS��ַ
        bRet = AssignSrvInfo(tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, m_vcBgsInfoEx, tPasSrvAddr, FALSE);
        if (!bRet)
        {
			SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Assign BGS for Mt(%s) failed\n", tLoginInfo.GetDeviceType());
        }
		// ��û������BGS��ַ������Ȼ����PAS��ַ
		if (0 == tPasSrvAddr.GetIpAddr())
		{
			SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]BGS ip is invalid, get pas addr.\n");
			bRet = AssignPasSrvInfo(szClientPasMOID, tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, tPasSrvAddr);
			if (!bRet)
			{
				SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]111 Assign Pas for Mt(%s) failed\n", tLoginInfo.GetDeviceType());
			}
		}
    }
    else
    {
        bRet = AssignPasSrvInfo(szClientPasMOID, tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, tPasSrvAddr);
        if (!bRet)
        {
			SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]222 Assign Pas for Mt(%s) failed\n", tLoginInfo.GetDeviceType());
        }
    }
	if ( tPasSrvAddr.GetIpAddr() == 0 )
	{
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Nu Addr failed, return False\n");
		return FALSE;
	}

    //��pas�������ֳ�3����������֪�ն�
    tPasSrvAddr.SetPort(g_cCfgFile.GetStdH323Port());
    tSrvAddrInfo.SetStdH323AddrInfo(tPasSrvAddr);   //��׼H323��ַ��Ϣ

    tPasSrvAddr.SetPort(g_cCfgFile.GetNonH323Port());   
    tSrvAddrInfo.SetNonH323AddrInfo(tPasSrvAddr);   //Ħ�ƷǱ�H323��ַ��Ϣ

    tPasSrvAddr.SetPort(g_cCfgFile.GetSipPort());
    tSrvAddrInfo.SetSipAddrInfo(tPasSrvAddr);       //SIPЭ��ĵ�ַ��Ϣ
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Pas Addr(%s:%u), DomainName(%s) succeed\n",
		BossUtils::GetIpString(tPasSrvAddr.GetIpAddr()).c_str(), tPasSrvAddr.GetPort(), tPasSrvAddr.GetNetDomainName());

    //SUS
    CSrvAddrInfo tSusSrvAddr;
    bRet = AssignSusSrvInfo(tLoginInfo.GetOemMark(), tLoginInfo.GetClientIp(), strClientISPName, strLocalApsIspName, szClientPlatfromMOID, tSusSrvAddr);
    if (!bRet)
    {
		SrvListMgrLog(LOG_LVL_ERROR, "[%s]Find Sus Addr failed\n", __FUNCTION__);
    }
    tSusSrvAddr.SetPort(g_cCfgFile.GetSusPort());

    tSrvAddrInfo.SetSusAddrInfo(tSusSrvAddr);
    SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Get Sus Addr(%s:%u), DomainName(%s) succeed\n",
		BossUtils::GetIpString(tSusSrvAddr.GetIpAddr()).c_str(), tSusSrvAddr.GetPort(), tSusSrvAddr.GetNetDomainName());

    //APS list: 
    tSrvAddrInfo.SetApsList(AssignApsSrvList());
    for (u32 dwIndex = 0; dwIndex < tSrvAddrInfo.GetApsList().Size(); dwIndex++)
    {
        CSrvAddrInfo tUlsInfo = tSrvAddrInfo.GetApsList().GetValue(dwIndex);
        SrvListMgrLog(LOG_LVL_KEYSTATUS, "[GetPlatformSrvAddr]Id:%u APS Addr(%s:%u), DomainName(%s)\n", dwIndex, BossUtils::GetIpString(tUlsInfo.GetIpAddr()).c_str(), tUlsInfo.GetPort(), tUlsInfo.GetNetDomainName());
    }

    return TRUE;
}

/*
 *	�����������ַͨ��ԭ��
    1. ƥ���ն˵���Ӫ�̣����������һ��������ѡȡ
	2. ��1ȡ����������£�ƥ�䱾ApsIp��Ӧ����Ӫ�̣����������һ��������ѡȡ
	3. ��2Ҳ�߲�ͨ����ӱ�ƽ̨��ѡһ��

  ����ĳ�����ִ�и��ؾ��⣬Ŀǰ�������������Ҫ������Ҫʹ��Upu��ѯ�����Ǵ��ڼ��޳����»��в�׼��
 */
BOOL32 CSrvListMgr::AssignSrvInfo(IN u32 dwClinetIp/*������*/, IN const std::string& strClientIspName, IN const std::string& strLocalApsIspName, IN const s8* szClientPlatfromMOID, IN const std::vector<CBriefSrvStateEx>& vcSrvInfoEx, OUT CSrvAddrInfo& tUlcSrvAddr, IN BOOL32 bChckState) const
{
    tUlcSrvAddr.Clear();

    std::map<u32, CSrvAddrInfo> mapFilterIp; //u32��ʾSn����0��ʼ

    std::vector<CBriefSrvStateEx>::const_iterator iter = vcSrvInfoEx.begin();
	CNetAddrInfo tSrvAddr;
	CBriefSrvInfo tSrvList;
	u8 byLoop = 0;
	u32 dwSn = 0;
	//1.�ҵ��ն�����ƽ̨�� ��ApsIpͬ��Ӫ�̵ĵ�ַ
	while ( iter != vcSrvInfoEx.end() )
	{
		tSrvList = iter->GetDetailBody();
		if ( 0 == strcmp(tSrvList.GetPlatformDomainMOID(), szClientPlatfromMOID) )
		{
			for (byLoop = 0; byLoop < OPERATION_MAXNUM_ISP; byLoop++ )
			{
				//��·δͨ������
				// ��Ҫ�����·״̬�������ж� [1/28/2016 pengguofeng]
				if ( bChckState && iter->GetIpState(byLoop) == FALSE )
				{
					continue;
				}
				//��ȡ��ַʧ�ܣ�����
				if ( !tSrvList.GetSrvAddr(byLoop, tSrvAddr) || !tSrvAddr.IsValid())
				{
					continue;
				}
				//��Ӫ�̲�ƥ�䣬����
				if ( 0 != strcmp(tSrvAddr.GetOperatorName(), strLocalApsIspName.c_str()) )
				{
					continue;
				}
				tUlcSrvAddr.SetIpAddr(tSrvAddr.GetIP());
				tUlcSrvAddr.SetNetDomainName(tSrvList.GetNetDomainName());
				dwSn = mapFilterIp.size();
                mapFilterIp.insert(std::make_pair(dwSn, tUlcSrvAddr));
			}
		}
		iter++;
	}

	if ( mapFilterIp.size() == 0 )
	{
        SrvListMgrLog(LOG_LVL_WARNING, "[%s] find srv for same Isp(%s) of Aps failed!\n",
			__FUNCTION__, strLocalApsIspName.c_str());
		iter = vcSrvInfoEx.begin();
		//2.�ҵ��ն�����ƽ̨�� ���ն�ͬ��Ӫ�̵ĵ�ַ
		while ( iter != vcSrvInfoEx.end() )
		{
			tSrvList = iter->GetDetailBody();
			if ( 0 == strcmp(tSrvList.GetPlatformDomainMOID(), szClientPlatfromMOID) )
			{
				for (byLoop = 0; byLoop < OPERATION_MAXNUM_ISP; byLoop++ )
				{
					//��·δͨ������
					// ��Ҫ�����·״̬�������ж� [1/28/2016 pengguofeng]
					if ( bChckState && iter->GetIpState(byLoop) == FALSE )
					{
						continue;
					}
					//��ȡ��ַʧ�ܣ�����
					if ( !tSrvList.GetSrvAddr(byLoop, tSrvAddr) || !tSrvAddr.IsValid())
					{
						continue;
					}
					//��Ӫ�̲�ƥ�䣬����
					if ( 0 != strcmp(tSrvAddr.GetOperatorName(), strClientIspName.c_str()) )
					{
						continue;
					}
					tUlcSrvAddr.SetIpAddr(tSrvAddr.GetIP());
					tUlcSrvAddr.SetNetDomainName(tSrvList.GetNetDomainName());
					dwSn = mapFilterIp.size();
					mapFilterIp.insert(std::make_pair(dwSn, tUlcSrvAddr));
				}
			}
			iter++;
		}	
	}

	if ( mapFilterIp.size() == 0 )
	{
        SrvListMgrLog(LOG_LVL_WARNING, "[%s] find srv for same Isp(%s) of Mt failed!\n",
			__FUNCTION__, strClientIspName.c_str());
		iter = vcSrvInfoEx.begin();
		//3.�ն�����ƽ̨�� ��ѡһ���ĵ�ַ
		while ( iter != vcSrvInfoEx.end() )
		{
			tSrvList = iter->GetDetailBody();
			if ( 0 == strcmp(tSrvList.GetPlatformDomainMOID(), szClientPlatfromMOID) )
			{
				for (byLoop = 0; byLoop < OPERATION_MAXNUM_ISP; byLoop++ )
				{
					//��·δͨ������
					if ( bChckState && iter->GetIpState(byLoop) == FALSE )
					{
						continue;
					}
					//��ȡ��ַʧ�ܣ�����
					if ( !tSrvList.GetSrvAddr(byLoop, tSrvAddr) || !tSrvAddr.IsValid())
					{
						continue;
					}
					tUlcSrvAddr.SetIpAddr(tSrvAddr.GetIP());
					tUlcSrvAddr.SetNetDomainName(tSrvList.GetNetDomainName());
					dwSn = mapFilterIp.size();
					mapFilterIp.insert(std::make_pair(dwSn, tUlcSrvAddr));
				}
			}
			iter++;
		}	
	}
           
    //4.�����ַ
    u32 dwSize = mapFilterIp.size();
    if (dwSize == 0)
    {
        SrvListMgrLog(LOG_LVL_WARNING, "[%s]find srv for platform(%s) failed!\n", __FUNCTION__, szClientPlatfromMOID);
        return FALSE;
    }

    //5.1 ���ѡ��(���ؾ���)
	srand(OspTickGet());
 	u32 dwRandom = rand() % dwSize;    
    tUlcSrvAddr = mapFilterIp[dwRandom];
	SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s] find srv succeed!\n", __FUNCTION__);
    return TRUE;
}

//����PAS������
BOOL32 CSrvListMgr::AssignPasSrvInfo(IN const s8* szClientPasMOID, IN u32 dwClinetIp/*������*/, IN const std::string& strClientISPName, IN const std::string& strLocalApsISPName, IN const s8* szClientPlatfromMOID, OUT CSrvAddrInfo& tUlcSrvAddr) const
{
    if (NULL == szClientPasMOID)
    {
        return FALSE;
    }
    tUlcSrvAddr.Clear();
	CBriefSrvInfo tPasSrvInfo;
	CNetAddrInfo tSrvAddr;
	//1. ��ѡPas
    std::vector<CBriefSrvStateEx>::const_iterator iter = m_vcPasInfoEx.begin();
	std::map<u32, CSrvAddrInfo> mapFilterIp;
	u32 dwSn = 0;
	mapFilterIp.clear();
	std::string strTmpIsp = strClientISPName;
	u8 byMtIpsType = TransIspNameToType(strTmpIsp);
	u8 byPasIspType = 0;

	SrvListMgrLog(LOG_LVL_KEYSTATUS, "[AssignPasSrvInfo] 111 MtIpsType is .%d.\n",byMtIpsType );

	for (; iter != m_vcPasInfoEx.end(); iter++)
	{
		tPasSrvInfo = iter->GetDetailBody();
		for (u8 byIndex = 0; byIndex < OPERATION_MAXNUM_ISP; ++byIndex)
		{
			if( tPasSrvInfo.GetSrvAddr(byIndex, tSrvAddr) )
			{
				SrvListMgrLog( LOG_LVL_KEYSTATUS, "[AssignPasSrvInfo] ip:%08x_%s link.%d node.%d oper.%s moid:%s\n",
							tSrvAddr.GetIP(),
							BossUtils::GetIpString(tSrvAddr.GetIP()).c_str(),
							iter->GetIpState(byIndex),
							iter->GetIpOspNode(byIndex),
							tSrvAddr.GetOperatorName(),
							tPasSrvInfo.GetMOID() );
			}
		}
	}


	iter = m_vcPasInfoEx.begin();
	if( emTypeQT != byMtIpsType )
	{
    for (; iter != m_vcPasInfoEx.end(); iter++)
    {
        tPasSrvInfo = iter->GetDetailBody();
        for (u8 byIndex = 0; byIndex < OPERATION_MAXNUM_ISP; ++byIndex)
        {
            if ( iter->GetIpState(byIndex) == FALSE 
				|| tPasSrvInfo.GetSrvAddr(byIndex, tSrvAddr) == FALSE )
            {
                continue;
            }

            //��ѡPAS
			// ����ĿǰԼ������ѡ���ն�ͬ��Ӫ�̵ĵ�ַ�����û��ƥ�䣬������ѡPAS��������ַ�����һ�� [12/15/2015 pengguofeng]
            if ( 0 == strcmp(tPasSrvInfo.GetMOID(), szClientPasMOID) )
            {
				if ( 0 == dwClinetIp && 0 == strClientISPName.length() )
				{
					byMtIpsType = emTypeBD;
				}

				SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]AssignPasSrvInfo MtIpsType is %d\n", __FUNCTION__, byMtIpsType );
				if ( emTypeBD == byMtIpsType )
				{
					SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]AssignPasSrvInfo SrvAddr Operator %s,  LocalApsIspName %s\n", __FUNCTION__, tSrvAddr.GetOperatorName(), strLocalApsISPName.c_str() );
					//��Ӫ�̲�ƥ�䣬����
					if ( 0 != strcmp(tSrvAddr.GetOperatorName(), strLocalApsISPName.c_str()) )
					{
						continue;
					}
					tUlcSrvAddr.SetIpAddr(tSrvAddr.GetIP());
					tUlcSrvAddr.SetNetDomainName(tPasSrvInfo.GetNetDomainName());
					dwSn = mapFilterIp.size();
					mapFilterIp.insert(std::make_pair(dwSn, tUlcSrvAddr));
				}
				else
				{
					strTmpIsp = tSrvAddr.GetOperatorName();
					byPasIspType = TransIspNameToType( strTmpIsp );
					//��Ӫ��ƥ�䣬ֱ�ӷ���
					if ( byMtIpsType == byPasIspType )
					{
						SrvListMgrLog(LOG_LVL_ERROR, "[%s]find same IspName(%s) Pas succeed\n", __FUNCTION__, strClientISPName.c_str());
						tUlcSrvAddr.SetIpAddr( tSrvAddr.GetIP() );
						tUlcSrvAddr.SetNetDomainName( tPasSrvInfo.GetNetDomainName() );
						return TRUE;
					}
					else
					{
						if ( ( !BossUtils::IsLANIp( dwClinetIp ) && byPasIspType == emTypeBD ) || ( byPasIspType == emTypeHW && byMtIpsType !=  emTypeHW  ) )
						{ 
							continue;
						}

						//û��ƥ�䣬�Ȱѵ�ַ���棬�����������ȡһ��
						tUlcSrvAddr.SetIpAddr( tSrvAddr.GetIP() );
						tUlcSrvAddr.SetNetDomainName( tPasSrvInfo.GetNetDomainName() );
						dwSn = mapFilterIp.size();
						mapFilterIp.insert(std::make_pair(dwSn, tUlcSrvAddr));
					}

					}
				}
            }
        }
    }

	if ( 0 !=mapFilterIp.size() )
	{
		srand(OspTickGet());
		u32 dwIndex = rand() % mapFilterIp.size();
		tUlcSrvAddr = mapFilterIp[dwIndex];
		SrvListMgrLog(LOG_LVL_ERROR, "[%s]find same IspName(%s) Pas failed, return random IspAddr of this pas\n", __FUNCTION__, strClientISPName.c_str());
		return TRUE;
	}

	//2.��ѡ�Ҳ�������������ַ��ѡ��
	if( !AssignSrvInfo(dwClinetIp, strClientISPName, strLocalApsISPName, szClientPlatfromMOID, m_vcPasInfoEx, tUlcSrvAddr) )
    {
		return FALSE;
	}
    return TRUE;
}

//����SUS������
BOOL32 CSrvListMgr::AssignSusSrvInfo(IN const s8* szClientOemMark, IN u32 dwClinetIp/*������*/, IN const std::string& strClientISPName, IN const std::string& strLocalApsISPName, IN const s8* szClientPlatfromMOID, OUT CSrvAddrInfo& tUlcSrvAddr) const
{
	if (NULL == szClientOemMark)
    {
        return FALSE;
    }
    tUlcSrvAddr.Clear();
    
    std::map<u32, CSrvAddrInfo> mapFilterIp; //u32��ʾSn����0��ʼ
	CNetAddrInfo tSrvAddr;
    std::vector<CBriefSrvStateEx>::const_iterator iter = m_vcSusInfoEx.begin();
	//1.У��OemMark�Ƿ���Ч
    for (; iter != m_vcSusInfoEx.end(); ++iter)
    {
        CBriefSrvStateEx tSusSrvInfoEx = *iter;
        CBriefSrvInfo tSusSrvInfo = tSusSrvInfoEx.GetDetailBody();
        for (u8 byIndex = 0; byIndex < OPERATION_MAXNUM_ISP; ++byIndex)
        {
            if (iter->GetIpState(byIndex) == FALSE
				|| !tSusSrvInfo.GetSrvAddr(byIndex, tSrvAddr) )
            {
                continue;
            }
            
            //0.OEM
            if (strlen(szClientOemMark) > 0)
            {
                COemMark tMtOem(szClientOemMark);
                COemMarkList tOemMarkList = tSusSrvInfo.GetOemMarkList();
                if ( !tOemMarkList.Exist(tMtOem) )  //һƱ���
                {
					SrvListMgrLog(LOG_LVL_ERROR, "[%s]Mt OemMark(%s) not exist\n", __FUNCTION__, szClientOemMark);
                    return FALSE;
                }
            }
            else
            {
                COemMarkList tOemMarkList = tSusSrvInfo.GetOemMarkList();
                if (tOemMarkList.Size() > 0)
                {
                    COemMark tMtOem("");
                    if ( !tOemMarkList.Exist(tMtOem) )  //һƱ���
                    {
						SrvListMgrLog(LOG_LVL_ERROR, "[%s]Mt OemMark("") not exist\n", __FUNCTION__);
                        return FALSE;
                    }
                }
            }
		}
    }
	if ( !AssignSrvInfo(dwClinetIp, strClientISPName, strLocalApsISPName, szClientPlatfromMOID, m_vcSusInfoEx, tUlcSrvAddr) )
	{
		return FALSE;
	}
    return TRUE;
}

const CApsSrvIpList& CSrvListMgr::AssignApsSrvList()
{
    if (!m_bApsSrvIpInited)
    {
		m_tApsSrvIpList.Init();

        //��û�г�ʼֵ�����и�ֵ
        std::vector<CBriefSrvStateEx>::const_iterator iter = m_vcApsInfoEx.begin();
        std::vector<CBriefSrvStateEx>::const_iterator iterEnd = m_vcApsInfoEx.end();
        for (; iter != iterEnd; ++iter)
        {
            CBriefSrvStateEx tUlsSrvInfoEx = *iter;
            CBriefSrvInfo tUlsSrvInfo = tUlsSrvInfoEx.GetDetailBody();

			//�����жϵ�ǰ��uls���������Ƿ�����ЧIP(��ip�Ƿ�Ϊ��)��Ϊ��ʱ����������Ϣ
			if (!tUlsSrvInfo.HasValidIp())
			{
				CSrvAddrInfo tUlcTemp;
                tUlcTemp.SetIpAddr(0);
                tUlcTemp.SetPort(60080);
                tUlcTemp.SetNetDomainName(tUlsSrvInfo.GetNetDomainName());

				if ( !m_tApsSrvIpList.Exist(tUlcTemp) )
				{
					m_tApsSrvIpList.Append(tUlcTemp);
				}
			}

			//�������Чip���򷵻�ip������
            for (u8 byIndex = 0; byIndex < OPERATION_MAXNUM_ISP; ++byIndex)
            {
                CNetAddrInfo tAddr;
                BOOL32 bRet = tUlsSrvInfo.GetSrvAddr(byIndex, tAddr);
                if (!bRet || !tAddr.IsValid())
                {
                    continue;
                }
                
                CSrvAddrInfo tUlcTemp;
                tUlcTemp.SetIpAddr(tAddr.GetIP());
                tUlcTemp.SetPort(60080);
                tUlcTemp.SetNetDomainName(tUlsSrvInfo.GetNetDomainName());
                
				if ( !m_tApsSrvIpList.Exist(tUlcTemp) )
				{
					m_tApsSrvIpList.Append(tUlcTemp);
				}
                
            }
        }
        
        m_bApsSrvIpInited = TRUE;
    }
             
    return m_tApsSrvIpList;
}
/*lint -restore*/
