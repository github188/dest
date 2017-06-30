

#include "srvlistmgr.h"
#include "debugprint.h"
#include "connectsa_api.h"
#include "bossutils.h"
#include "structulc.h"
#include "structself.h"
#include "bossconst.h"
#include "config.h"
#include "sacommonconst.h"
#include "apsutility.h"

CSrvListMgr g_cSrvListMgr;

/*lint -save -e1788*/
CSrvListMgr::CSrvListMgr()
{
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_NDS] = &m_vcNdsInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_NMS_C] = &m_vcNmsInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_NMS_6000] = &m_vcNmsInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_PAS] = &m_vcPasInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_SUS] = &m_vcSusInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_SNS] = &m_vcSnsInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_XNS] = &m_vcXnsInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_CMC] = &m_vcCmcInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_NTP] = &m_vcNtpInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_VRS2000] = &m_vcVrsInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_VRS4000] = &m_vcVrsInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_DCS2000] = &m_vcDcsInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_RESTAPI] = &m_vcRestInfoEx;
	m_mapTypeServerList[DEVICE_GLOBAL_TYPE_APS] = &m_vcApsInfoEx;

	//5.1sp2�����������������⣬��TCP��ҵ���ǹ�HA�����Լ�����������岻��
	m_setNeedCheckStateServerList.insert(DEVICE_GLOBAL_TYPE_PAS);
	m_setNeedCheckStateServerList.insert(DEVICE_GLOBAL_TYPE_NDS);
	m_setNeedCheckStateServerList.insert(DEVICE_GLOBAL_TYPE_NTP);


    m_setPxyAddr.clear();

    memset(m_szMySelfMOID, 0, sizeof(m_szMySelfMOID));
    memset(m_szPlatformDomainMOID, 0, sizeof(m_szPlatformDomainMOID));
    memset(m_szServiceDomainMOID, 0, sizeof(m_szServiceDomainMOID));

    m_tApsSrvIpList.Init();

//TODO �������������ļ������Բ�����
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

const s8* CSrvListMgr::GetPlatformDomainMOID() const
{
	return m_szPlatformDomainMOID;
}

void CSrvListMgr::InsertToServerList(const u32 dwServerType, const std::set<CBriefSrvInfo> &setServerInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
	if (m_mapTypeServerList.find(dwServerType) != m_mapTypeServerList.end())
	{
		AddToSrvList(setServerInfo, *(m_mapTypeServerList[dwServerType]));
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]Server<%s> list num is %u,insert to server list!\n", __FUNCTION__,
			GetServerNameByType(dwServerType), setServerInfo.size());
	}
	else
	{
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]can't find server<%u>, insert to server list fail\n", __FUNCTION__,
			dwServerType);
	}
}
void CSrvListMgr::AddServerInfo(const u32 dwServerType, const CBriefSrvInfo &tServerInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
	if (m_mapTypeServerList.find(dwServerType) != m_mapTypeServerList.end())
	{
		AddSrvInfo(tServerInfo, *(m_mapTypeServerList[dwServerType]));
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]Add Server<%s> Success\n", __FUNCTION__,
			GetServerNameByType(dwServerType));
	}
	else
	{
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]can't find server<%u>, add server fail\n", __FUNCTION__,
			dwServerType);
	}
}
void CSrvListMgr::MdfServerInfo(const u32 dwServerType, const CBriefSrvInfo &tServerInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
	if (m_mapTypeServerList.find(dwServerType) != m_mapTypeServerList.end())
	{
		MdfSrvInfo(tServerInfo, *(m_mapTypeServerList[dwServerType]));
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]Mod Server<%s> Success\n", __FUNCTION__,
			GetServerNameByType(dwServerType));
	}
	else
	{
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]can't find server<%u>, modify server fail\n", __FUNCTION__,
			dwServerType);
	}
}
void CSrvListMgr::DelServerInfo(const u32 dwServerType, const CBriefSrvInfo &tServerInfo)
{
    CAutoWriteLock tWriteLock(m_tRWLock);
	if (m_mapTypeServerList.find(dwServerType) != m_mapTypeServerList.end())
	{
		DelSrvInfo(tServerInfo, *(m_mapTypeServerList[dwServerType]));
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]Del Server<%s> Success\n", __FUNCTION__,
			GetServerNameByType(dwServerType));
	}
	else
	{
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]can't find server<%u>, del server fail\n", __FUNCTION__,
			dwServerType);
	}
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
	map<u32, std::vector<CBriefSrvStateEx>*>::const_iterator iterServer = m_mapTypeServerList.begin();
	for (; iterServer != m_mapTypeServerList.end(); ++iterServer)
	{
		//nms��vrs�����������ͣ�ֻȡ����һ�����ͼ��ɻ�ȡ��ƽ̨���nms,vrs�������б�
		if (m_setNeedCheckStateServerList.find(iterServer->first) != m_setNeedCheckStateServerList.end())
		{
			SrvListMgrLog(LOG_LVL_KEYSTATUS, "Check <%s> state\n", GetServerNameByType(iterServer->first));
			CheckState(*(iterServer->second), SA_LISTEN_PORT);
		}
	}
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
				OspPrintf(TRUE, FALSE, "[%s]node isn't 0, but invalid, disconnect\n", __FUNCTION__);
				OspDisconnectTcpNode(dwNode);
			}

			OspPrintf(TRUE, FALSE, "[%s]reconnect:%s\n", __FUNCTION__, BossUtils::GetIpString(tAddr.GetIP()).c_str());
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
	OspPrintf(TRUE, FALSE, "\n3.APS info(needn't check state)******************\n");
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
    OspPrintf(TRUE, FALSE, "\n10.VRS info(needn't check state)******************\n");
    PrintSrvInfo(m_vcVrsInfoEx);

    //DCS
    OspPrintf(TRUE, FALSE, "\n11.DCS info(needn't check state)******************\n");
    PrintSrvInfo(m_vcDcsInfoEx);

	OspPrintf(TRUE, FALSE, "\n12.RestApi info(needn't check state)******************\n");
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
		OspPrintf(TRUE, TRUE, " SrvMOID(%s)\nNetDomainName(%s)\n PlatformDomainMOID(%s)\n", 
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
				BossUtils::GetIpString(tAddr.GetIP()).c_str(), IsServerSateOkByIspIndex(tSrvStateEx, dwIndex),
				tSrvStateEx.GetIpOspNode(dwIndex));
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

BOOL32 CSrvListMgr::GetLocalApsIspName(IN u32 dwLocalIP/*������*/, OUT std::string& strLocalApsIspName)
{
    CAutoReadLock tReadLock(m_tRWLock);

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
										   IN const s8* szClientPasMOID, OUT CPlatformSrvAddrInfo& tSrvAddrInfo)
{
    std::string strLocalApsIspName;
    g_cSrvListMgr.GetLocalApsIspName(tLoginInfo.GetApsLocalIp(), strLocalApsIspName);
    return GetPlatformSrvAddr(tLoginInfo, strClientISPName, strLocalApsIspName, szClientPasMOID, tSrvAddrInfo);
}
const std::map<u32, std::vector<CBriefSrvStateEx>*> CSrvListMgr::GetTypeServerListMap() const
{
	return m_mapTypeServerList;
}

/*
 *	��ʹ�Ҳ���ĳһ����ַ��Ҳ��Ҫֱ�ӷ���False���Ӹ���ӡ��ʾ����
 */
BOOL32 CSrvListMgr::GetPlatformSrvAddr( const CLoginInfo &tLoginInfo, const std::string strClientISPName,
									   std::string strLocalApsIspName, const s8* szClientPasMOID,
										CPlatformSrvAddrInfo &tSrvAddrInfo )
{
    CAutoReadLock tReadLock(m_tRWLock);

	map<u32, vector<CBriefSrvStateEx>*>::const_iterator iterTypeServerList = m_mapTypeServerList.begin();
	for (; iterTypeServerList != m_mapTypeServerList.end(); ++iterTypeServerList)
	{
		const u32 dwServerType = iterTypeServerList->first;
		if (
			//nms��vrs�����������ͣ�ֻȡ����һ�����ͼ��ɻ�ȡ��ƽ̨���nms,vrs�������б�
			dwServerType != DEVICE_GLOBAL_TYPE_NMS_6000
			&& dwServerType != DEVICE_GLOBAL_TYPE_VRS4000)
		{
			CSrvAddrInfo tServerAddr;
			const BOOL32 bCheck = (m_setNeedCheckStateServerList.find(dwServerType) != m_setNeedCheckStateServerList.end());
			if (!AssignServerInfo(dwServerType,
				tLoginInfo.GetClientIp(), strClientISPName,
				tLoginInfo.GetApsLocalIp(), strLocalApsIspName, 
				*(m_mapTypeServerList[dwServerType]), tServerAddr,
				bCheck, szClientPasMOID, tLoginInfo.GetOemMark(), tLoginInfo.GetDeviceType()))
			{
				SrvListMgrLog(LOG_LVL_ERROR, "[%s]Find necessary Server<%u> Addr failed\n", __FUNCTION__, dwServerType);
				return FALSE;
			}
			tServerAddr.SetPort(g_cCfgFile.GetServerPort(dwServerType));
			tSrvAddrInfo.SetServerAddrInfo(dwServerType, tServerAddr);
		}
	}
    return TRUE;
}

/*
 *	�����������ַͨ��ԭ��
	1. ƥ�䱾ApsIp��Ӧ����Ӫ�̣����������һ��������ѡȡ
    1. ��1ȡ����������£�ƥ���ն˵���Ӫ�̣����������һ��������ѡȡ
	3. ��2Ҳ�߲�ͨ����ӱ�ƽ̨��ѡһ��

  ����ĳ�����ִ�и��ؾ��⣬Ŀǰ�������������Ҫ������Ҫʹ��Upu��ѯ�����Ǵ��ڼ��޳����»��в�׼��
 */
BOOL32 CSrvListMgr::AssignServerInfo(const u32 dwServerType,
	IN u32 dwClinetIp/*������*/, IN const std::string& strClientIspName,
	IN u32 dwLocalApsIp/*������*/, IN const std::string& strLocalApsIspName, 
	IN const std::vector<CBriefSrvStateEx>& vcSrvInfoEx, OUT CSrvAddrInfo& tServerAddr,
	IN BOOL32 bChckState/*= TRUE*/, const s8* pchClientPasMOID/*=""*/,
	const s8* pchClientOemMark/*=""*/, const s8* pchDevideType/*=""*/ ) const
{
	CPrintFunInOut cPrintFun(__FUNCTION__, "Server: %s", GetServerNameByType(dwServerType));

	if (dwServerType == DEVICE_GLOBAL_TYPE_SUS)
	{
		AssignSusSrvInfo(pchClientOemMark, dwClinetIp, strClientIspName, 
			dwLocalApsIp, strLocalApsIspName, tServerAddr);
	}
	else if (dwServerType == DEVICE_GLOBAL_TYPE_PAS)
	{
		if(!AssignPasSrvInfo(pchClientPasMOID, dwClinetIp, strClientIspName,
			dwLocalApsIp, strLocalApsIspName, tServerAddr))
		{
			SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]Assign Pas for Mt(%s) failed\n", __FUNCTION__, pchDevideType);
			return FALSE;
		}
	}
	else
	{
		 AssignSrvInfo(dwClinetIp, strClientIspName, dwLocalApsIp, strLocalApsIspName, 
			vcSrvInfoEx, tServerAddr, bChckState);
		if (DEVICE_GLOBAL_TYPE_XNS == dwServerType && tServerAddr.GetIpAddr() == 0)
		{
			SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]Get Xns Addr failed, return False\n", __FUNCTION__);
			return FALSE;
		}
	}
	SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s]Assign Server<%s> %s\n",
		__FUNCTION__, GetServerNameByType(dwServerType), tServerAddr.IsIpValid() ? "success" : "fail");
	return TRUE;
}

BOOL32 CSrvListMgr::AssignSrvInfo(IN const u32 dwClinetIp/*������*/, IN const std::string& strClientIspName,
	IN const u32 dwLocalApsIp, IN const std::string& strLocalApsIspName, 
	IN const std::vector<CBriefSrvStateEx>& vcSrvInfoEx, OUT CSrvAddrInfo& tUlcSrvAddr, IN BOOL32 bChckState) const
{
    tUlcSrvAddr.Clear();

	BOOL32 bExistSameApsIspServer = TRUE;
	vector<CSrvAddrInfo> vecServerAddr;
	GetValidServerByIsp(vcSrvInfoEx, vecServerAddr, strLocalApsIspName.c_str(), bChckState);
	if (vecServerAddr.empty())
	{
        SrvListMgrLog(LOG_LVL_WARNING, "[%s] find srv for same Isp(%s) of Aps failed!,try srv for same mt isp<%s>\n",
			__FUNCTION__, strLocalApsIspName.c_str(), strClientIspName.c_str());
		bExistSameApsIspServer = FALSE;
		GetValidServerByIsp(vcSrvInfoEx, vecServerAddr, strClientIspName.c_str(), bChckState);
	}

	if (vecServerAddr.empty())
	{
        SrvListMgrLog(LOG_LVL_WARNING, "[%s] find srv for same Isp(%s) of Mt failed!, random select one from all valid server\n",
			__FUNCTION__, strClientIspName.c_str());
		GetValidServerByIsp(vcSrvInfoEx, vecServerAddr, NULL, bChckState);
		//����ѡ��ȫ�������ȷ���������Ӫ��
		//���������˵��źͱ��أ���¼apsip��δ�������������������ȷ��ص���
		vector<CSrvAddrInfo> vecBakSrvAddrInfo(vecServerAddr);
		FiltLanServer(vecBakSrvAddrInfo);
		if (!vecBakSrvAddrInfo.empty())
		{
			vecServerAddr.swap(vecBakSrvAddrInfo);
			SrvListMgrLog(LOG_LVL_WARNING, "[%s]Filt lan server,left %u server\n",
				__FUNCTION__, vecServerAddr.size());
		}
	}
           
	const u32 dwSize = vecServerAddr.size();
    if (dwSize == 0)
    {
        SrvListMgrLog(LOG_LVL_WARNING, "[%s]find srv for platform(%s) failed!\n", __FUNCTION__, m_szPlatformDomainMOID);
        return FALSE;
    }

    //5.1 ���ѡ��(���ؾ���)
	srand(OspTickGet());
 	const u32 dwRandom = rand() % dwSize;    
	tUlcSrvAddr = vecServerAddr[dwRandom];
	SrvListMgrLog(LOG_LVL_KEYSTATUS, "[%s] find srv succeed!\n", __FUNCTION__);
    return TRUE;
}

//����PAS������
BOOL32 CSrvListMgr::AssignPasSrvInfo(IN const s8* szClientPasMOID,
	IN const u32 dwClinetIp/*������*/, IN const std::string& strClientISPName,
	IN const u32 dwLocalApsIp, IN const std::string& strLocalApsISPName,
	OUT CSrvAddrInfo& tUlcSrvAddr) const
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
	for (; iter != m_vcPasInfoEx.end(); iter++)
	{
		tPasSrvInfo = iter->GetDetailBody();
		for (u8 byIndex = 0; byIndex < OPERATION_MAXNUM_ISP; ++byIndex)
		{
			if( tPasSrvInfo.GetSrvAddr(byIndex, tSrvAddr)
				&& tSrvAddr.IsValid())
			{
				SrvListMgrLog( LOG_LVL_KEYSTATUS, "[AssignPasSrvInfo] ip:%08x_%s link.%d node.%d oper.%s moid:%s\n",
							tSrvAddr.GetIP(),
							BossUtils::GetIpString(tSrvAddr.GetIP()).c_str(),
							IsServerSateOkByIspIndex(*iter, byIndex),
							iter->GetIpOspNode(byIndex),
							tSrvAddr.GetOperatorName(),
							tPasSrvInfo.GetMOID() );
			}
		}
	}

	std::vector<CBriefSrvStateEx> vcPreferPasInfoEx ;
	for (iter = m_vcPasInfoEx.begin(); iter != m_vcPasInfoEx.end(); iter++)
	{
		tPasSrvInfo = iter->GetDetailBody();
		for (u8 byIndex = 0; byIndex < OPERATION_MAXNUM_ISP; ++byIndex)
		{
			if (!IsServerSateOkByIspIndex(*iter, byIndex)
				|| tPasSrvInfo.GetSrvAddr(byIndex, tSrvAddr) == FALSE )
			{
				continue;
			}
			//ƥ����ѡPas MOID
			if ( 0 == strcmp(iter->GetDetailBody().GetMOID(), szClientPasMOID) )
			{
				vcPreferPasInfoEx.push_back(*iter);
			}
		}
	}
	
	if(0 != vcPreferPasInfoEx.size() && 
	   AssignSrvInfo(dwClinetIp, strClientISPName, dwLocalApsIp, strLocalApsISPName, vcPreferPasInfoEx, tUlcSrvAddr))
	{
		SrvListMgrLog(LOG_LVL_KEYSTATUS, "[AssignPasSrvInfo] Assign the selective preference Pas server succeed \n");
		return TRUE;
	}

	//2.��ѡ�Ҳ�������������ַ��ѡ��
	SrvListMgrLog(LOG_LVL_KEYSTATUS, "[AssignPasSrvInfo] Assign the selective preference Pas server failed \n");
	if( !AssignSrvInfo(dwClinetIp, strClientISPName, dwLocalApsIp, strLocalApsISPName, m_vcPasInfoEx, tUlcSrvAddr) )
    {
		return FALSE;
	}
    return TRUE;
}

//����SUS������
BOOL32 CSrvListMgr::AssignSusSrvInfo(IN const s8* szClientOemMark,
	IN const u32 dwClinetIp/*������*/, IN const std::string& strClientISPName,
	IN const u32 dwLocalApsIp, const std::string& strLocalApsISPName,
	OUT CSrvAddrInfo& tUlcSrvAddr) const
{
	if (NULL == szClientOemMark)
    {
        return FALSE;
    }
    tUlcSrvAddr.Clear();
    
    std::map<u32, CSrvAddrInfo> mapFilterIp; //u32��ʾSn����0��ʼ
	CNetAddrInfo tSrvAddr;
	//�������ƥ����Oem��Sus������
	std::vector<CBriefSrvStateEx> vcSameOemSusInfoEx;
    std::vector<CBriefSrvStateEx>::const_iterator iter = m_vcSusInfoEx.begin();
	//1.У��OemMark�Ƿ���Ч
    for (; iter != m_vcSusInfoEx.end(); ++iter)
    {
        CBriefSrvStateEx tSusSrvInfoEx = *iter;
        CBriefSrvInfo tSusSrvInfo = tSusSrvInfoEx.GetDetailBody();
       
        //0.OEM
        if (strlen(szClientOemMark) > 0)
        {
            COemMark tMtOem(szClientOemMark);
            COemMarkList tOemMarkList = tSusSrvInfo.GetOemMarkList();
            if ( tOemMarkList.Exist(tMtOem) )  
            {
				vcSameOemSusInfoEx.push_back(*iter);
            }
        }
        else
        {
            COemMarkList tOemMarkList = tSusSrvInfo.GetOemMarkList();
            if (tOemMarkList.Size() > 0)
            {
				//���ն�OemΪ�յ�����£�COemMark�Ĺ��캯����Ĭ�Ϲ���һ��"kedacom"��Oem
                COemMark tMtOem("");
                if ( tOemMarkList.Exist(tMtOem) )  
                {
					vcSameOemSusInfoEx.push_back(*iter);
                }
            }
        }
    }
	//��ƥ����Oem��Sus�������з���Sus������,5.1sp2����SUS������������״̬���
	if ( !AssignSrvInfo(dwClinetIp, strClientISPName, dwLocalApsIp, strLocalApsISPName, vcSameOemSusInfoEx, tUlcSrvAddr, FALSE) )
	{
		return FALSE;
	}
    return TRUE;
}

/*lint -restore*/
