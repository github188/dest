

#include "centerctrl.h"
#include "config.h"
#include "debugprint.h"
#include "dbctrl.h"
#include "eventself.h"
#include "srvlistmgr.h"
#include "connectsa_api.h"
#include "mqmsgparse.h"

CCenterCtrlApp g_cCenterCtrlApp;

#define  m_tDBPool          g_cCenterCtrlApp.GetDBPool()


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


#define OperateSrvListMgr( dwType, tBriefInfo, Operate )        \
{                                                               \
    switch(dwType)                                              \
    {                                                           \
    case DEVICE_GLOBAL_TYPE_NDS:                                \
        {                                                       \
            g_cSrvListMgr.Operate##NdsInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
    case DEVICE_GLOBAL_TYPE_NMS_C:                              \
    case DEVICE_GLOBAL_TYPE_NMS_6000:                              \
        {                                                       \
            g_cSrvListMgr.Operate##NmsInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
    case DEVICE_GLOBAL_TYPE_PAS:                                \
        {                                                       \
            g_cSrvListMgr.Operate##PasInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
    case DEVICE_GLOBAL_TYPE_SUS:                                \
        {                                                       \
            g_cSrvListMgr.Operate##SusInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
    case DEVICE_GLOBAL_TYPE_APS:                                \
        {                                                       \
            g_cSrvListMgr.Operate##ApsInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
    case DEVICE_GLOBAL_TYPE_CMC:                                \
        {                                                       \
            g_cSrvListMgr.Operate##CmcInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
    case DEVICE_GLOBAL_TYPE_SNS:                                \
        {                                                       \
            g_cSrvListMgr.Operate##SnsInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
    case DEVICE_GLOBAL_TYPE_XNS:                                \
        {                                                       \
            g_cSrvListMgr.Operate##XnsInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
    case DEVICE_GLOBAL_TYPE_NTP:                                \
        {                                                       \
            g_cSrvListMgr.Operate##NtpInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
    case DEVICE_GLOBAL_TYPE_VRS2000:                            \
    case DEVICE_GLOBAL_TYPE_VRS4000:                            \
        {                                                       \
            g_cSrvListMgr.Operate##VrsInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
    case DEVICE_GLOBAL_TYPE_DCS2000:                            \
        {                                                       \
            g_cSrvListMgr.Operate##DcsInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
    case DEVICE_GLOBAL_TYPE_BGS:                                \
        {                                                       \
            g_cSrvListMgr.Operate##BgsInfo(tBriefInfo);         \
            break;                                              \
        }                                                       \
	case DEVICE_GLOBAL_TYPE_RESTAPI:							\
		{														\
			g_cSrvListMgr.Operate##RestInfo(tBriefInfo);		\
			break;												\
		}														\
     default:                                                   \
        break;                                                  \
    }                                                           \
}  



CCenterCtrlData::CCenterCtrlData()
{
	memset(m_atMtInfo, 0, sizeof(m_atMtInfo));
	OspSemBCreate(&m_hInitLock);
	OspSemTake(m_hInitLock);
	m_bInited = FALSE;
}

CCenterCtrlData::~CCenterCtrlData()
{
	OspSemDelete( m_hInitLock );
}

void CCenterCtrlData::TakeInitLock()
{
	if( !m_bInited )
	{
		OspSemTake(m_hInitLock);
	}	
}

void CCenterCtrlData::ParseMqMsg(void *pbuf, u32 len)
{	
	m_cJsonMsgParse.MqConsumerMsgCB( pbuf,len );
}

BOOL32 CCenterCtrlData::StarMqMsgParse()
{
    //注册关心的消息
    m_cJsonMsgParse.RegisterEvent(EV_MQMSG_ADD_SERVER_DEVICE);
    m_cJsonMsgParse.RegisterEvent(EV_MQMSG_DEL_SERVER_DEVICE);
    m_cJsonMsgParse.RegisterEvent(EV_MQMSG_ENABLE_SERVER_DEVICE);
    m_cJsonMsgParse.RegisterEvent(EV_MQMSG_DISENABLE_SERVER_DEVICE);
    m_cJsonMsgParse.RegisterEvent(EV_MQMSG_MDF_SERVER_DEVICE);

	m_cJsonMsgParse.SetMqMsgReceiver(this);

    //启动

#ifdef _TEST_
TGUID tMyGUID;
tMyGUID.SetNOString("10107000000002000000000000000001");
#else
    //获取自己的GUID
    TGUID tMyGUID = GetSrvGUID();
    if (!tMyGUID.IsValid())
    {
        printf("[StarMqMsgParse] 从ConnectSaLib获取设备GUID失败, 稍后再尝试获取....\n");
        CCtrlLog(LOG_LVL_WARNING, "[StarMqMsgParse] 从ConnectSaLib获取设备GUID失败, 稍后再尝试获取....\n");
        return FALSE;
    }
#endif  //_TEST_

    CMqConfig tMqConf = g_cCfgFile.GetMqConInfo();
	if ( !CCfg::IsValidIp(tMqConf.GetIP()))
	{
        printf("[%s:%d]Mq Ip:%s is Not Valid\n",__FUNCTION__, __LINE__, tMqConf.GetIP());
        CCtrlLog(LOG_LVL_WARNING, "[%s:%d]Mq Ip:%s is Not Valid\n",__FUNCTION__, __LINE__, tMqConf.GetIP());
		return FALSE;
	}
    /*std::string strQueueFullName = tMqConf.GetQueueName();
    strQueueFullName += ":";
    strQueueFullName += GetSrvGUID().GetNOString();
    tMqConf.SetQueueName(strQueueFullName.c_str());

    BOOL32 bRet = m_tMqMsgParse.StarParse(tMqConf, this);*/

	m_bInited = TRUE;
	OspSemGive(m_hInitLock);
    return TRUE;
}

void CCenterCtrlData::MqMsgParseCB( MqMsgCBEvent emEventID, void* pMsgBody, u32 wMsgLen)
{
    if (NULL == pMsgBody || sizeof(CServerDevice) != wMsgLen)
    {
        CCtrlLog(LOG_LVL_WARNING, "[MqMsgParseCB] NULL == pMsgBody || sizeof(CServerDevice) != wMsgLen \n");
        return;
    }    
	if (emEventID == EV_MQMSG_ADD_PLATFORM_DOMAIN
		|| emEventID == EV_MQMSG_ENABLE_PLATFORM_DOMAIN)
	{
		CPlatformDomain* pcPlatformDoamin = (CPlatformDomain*)pMsgBody;
		g_cSrvListMgr.GetAllPlatformDomainMOID().insert(pcPlatformDoamin->GetMOID());
		return;
	}
	else if(emEventID == EV_MQMSG_DISENABLE_PLATFORM_DOMAIN)
	{ 
		CPlatformDomain* pcPlatformDoamin = (CPlatformDomain*)pMsgBody;
		g_cSrvListMgr.GetAllPlatformDomainMOID().erase(pcPlatformDoamin->GetMOID());
		return;
	}

    CServerDevice* pSrvDevInfo = (CServerDevice*)pMsgBody;
    CBriefSrvInfo tBriefInfo = Convert(*pSrvDevInfo);

	if ((emEventID == EV_MQMSG_ADD_SERVER_DEVICE || emEventID == EV_MQMSG_ENABLE_SERVER_DEVICE)
		&& (pSrvDevInfo->GetServerType() == DEVICE_GLOBAL_TYPE_APS)
		&& (strcmp(pSrvDevInfo->GetServiceDomainMOID(), g_cSrvListMgr.GetServiceDomainMOID()) != 0))
	{
        CCtrlLog(LOG_LVL_KEYSTATUS, "[MqMsgParseCB]add or enable aps, but this aps unbelong servicedomainMOID\n");
		return;
	}

    switch(emEventID)
    {
    case EV_MQMSG_ADD_SERVER_DEVICE:
        {
            if ( pSrvDevInfo->IsUsedFlag() )    //入网时就是启用状态
            {
                OperateSrvListMgr(pSrvDevInfo->GetServerType(), tBriefInfo, Add);  
            }
            break;
        }
    case EV_MQMSG_ENABLE_SERVER_DEVICE:
        {
            OperateSrvListMgr(pSrvDevInfo->GetServerType(), tBriefInfo, Add);
            break;
        }
    case EV_MQMSG_DEL_SERVER_DEVICE:
    case EV_MQMSG_DISENABLE_SERVER_DEVICE:
        {
            OperateSrvListMgr(pSrvDevInfo->GetServerType(), tBriefInfo, Del);
            break;
        }
    case EV_MQMSG_MDF_SERVER_DEVICE:
        {
            OperateSrvListMgr(pSrvDevInfo->GetServerType(), tBriefInfo, Mdf);
            break;
        }

    default:
        break;
    }   
}
                                                            
BOOL32 CCenterCtrlData::AddMtInfo(TMtRegNetInfo& tMtInfo)
{
	BOOL32 bFound = FALSE;
	u32 dwIndex = 0;
	u32 dwIdleLoop = MAXINSTNUM_ULCSSN;
	while ( dwIndex < MAXINSTNUM_ULCSSN )
	{
		if ( tMtInfo.m_byNetType == m_atMtInfo[dwIndex].m_byNetType 
			&& memcmp( tMtInfo.m_achMtMoid, m_atMtInfo[dwIndex].m_achMtMoid, sizeof(tMtInfo.m_achMtMoid)-1) == 0 )
		{
			bFound = TRUE;
			break;
		}
		if ( m_atMtInfo[dwIndex].m_byNetType == 0
			&& dwIdleLoop == MAXINSTNUM_ULCSSN )
		{
			dwIdleLoop = dwIndex;
		}
		dwIndex++;
	}
	if ( bFound )
	{
		CCtrlLog(LOG_LVL_KEYSTATUS, "[%s]Mt[%s] already Registered\n", __FUNCTION__, tMtInfo.m_achMtMoid);
		return TRUE;
	}
	if ( dwIdleLoop < MAXINSTNUM_ULCSSN )
	{
		m_atMtInfo[dwIdleLoop].m_byNetType = tMtInfo.m_byNetType;
		memcpy(m_atMtInfo[dwIdleLoop].m_achMtMoid, tMtInfo.m_achMtMoid, sizeof(m_atMtInfo[dwIdleLoop].m_achMtMoid)-1 );
	}
	else
	{
		CCtrlLog(LOG_LVL_KEYSTATUS, "[%s]Mt[%s] find IdleIndex failed\n", __FUNCTION__, tMtInfo.m_achMtMoid);
	}
	return TRUE;
}

/*
 *  批量上报终端MoId给网管服务器,仅在第一次连网管时有用
*/
void CCenterCtrlData::ReportAllMtInfo2Nms()
{
	u32 dwNum = 0;
	TMtInfoOnAps tMtInfoList;
	for (u32 dwIndex = 0; dwIndex < MAXINSTNUM_ULCSSN;dwIndex++ )
	{
		if ( m_atMtInfo[dwIndex].m_byNetType == 0)
		{
			continue;
		}
		tMtInfoList.m_tMtInfo[dwNum].m_byNetType = m_atMtInfo[dwIndex].m_byNetType;
		memcpy(tMtInfoList.m_tMtInfo[dwNum].m_achMtMoid, m_atMtInfo[dwIndex].m_achMtMoid, sizeof(tMtInfoList.m_tMtInfo[dwNum].m_achMtMoid)-1);

		if ( dwNum == NMSADP_MAX_RTP_MT_COUNT_ON_APS -1)
		{
			tMtInfoList.m_dwMtCount = dwNum+1;
			CCtrlLog(LOG_LVL_KEYSTATUS, "[%s]Get MtNum:%u report to NMS\n", __FUNCTION__, tMtInfoList.m_dwMtCount);
			ApsRptAllMtInfo(&tMtInfoList);
			dwNum = 0;
			memset(&tMtInfoList, 0, sizeof(TMtInfoOnAps));
			continue;
		}
		dwNum++;
	}
	if ( dwNum == 0 )
	{
		CCtrlLog(LOG_LVL_ERROR, "[%s]all Mt has reported To NMS!\n", __FUNCTION__);
		return;
	}
	tMtInfoList.m_dwMtCount = dwNum;
	ApsRptAllMtInfo(&tMtInfoList);
	CCtrlLog(LOG_LVL_KEYSTATUS, "[%s]last MtNum:%u report to NMS\n", __FUNCTION__, dwNum);
	return;
}

void CCenterCtrlData::Print() const
{
	//目前只打印Mt信息
	for (u32 dwIndex = 0; dwIndex < MAXINSTNUM_ULCSSN; dwIndex++)
	{
		if ( m_atMtInfo[dwIndex].m_byNetType == 0 )
		{
			continue;
		}
		OspPrintf(TRUE, FALSE, "RegMtInfo: MtIspType:%u  MtMoId:[%s]\n", m_atMtInfo[dwIndex].m_byNetType,
			m_atMtInfo[dwIndex].m_achMtMoid);
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

CCenterCtrlInst::CCenterCtrlInst()
{

}

CCenterCtrlInst::~CCenterCtrlInst()
{

}

void CCenterCtrlInst::DaemonInstanceDump(u32 param /* = 0 */)
{
    u16 wEventId = param;

    switch(wEventId) 
    {
    case APS_DBPOOL_STATE_DUMP:
		{
			m_tDBPool.DumpState();
			std::vector<CDBState> vecDbState = m_tDBPool.GetDBState();
			for (int i = 0; i < vecDbState.size(); i++)
			{
				OspPrintf(TRUE, FALSE, "index: %u DbName:[%s] DbIp:[%s] Connected:%u\n",
					i+1, vecDbState[i].GetDBName(), vecDbState[i].GetDBIP(), vecDbState[i].GetConnectState());
			}
		}
        break;

    default:
        CCtrlLog(LOG_LVL_WARNING, "[DaemonInstanceDump] received wrong message %u(%s)!\n", wEventId, ::OspEventDesc(wEventId));
        break;
    }

    return;
}

void CCenterCtrlInst::ProcPostToDBMsg(CMessage *const pcMsg)
{
    if (NULL == pcMsg)
    {
        CCtrlLog(LOG_LVL_ERROR, "[ProcPostToDBMsg]NULL == pcMsg\n");
        return;
    }
    CCtrlLog(LOG_LVL_DETAIL, "[ProcPostToDBMsg] Receive msg.%u<%s>\n",
        pcMsg->event , OspEventDesc(pcMsg->event));

    BOOL32 bRet = m_tDBPool.InsertMsgToDBPool(pcMsg->event, pcMsg->content, pcMsg->length);
    if (FALSE == bRet)
    {
        CCtrlLog(LOG_LVL_ERROR, "[ProcPostToDBMsg]InsertMsgToDBPool fail, message %u(%s) \n",
            pcMsg->event, ::OspEventDesc(pcMsg->event));
    }
}

void CCenterCtrlInst::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{
	if (NULL == pcMsg || NULL == pcApp)
	{
		CCtrlLog(LOG_LVL_ERROR, "[DaemonInstanceEntry] NULL == pcMsg || NULL == pcApp!\n");
		return;
	}

	//来至终端服务器消息
	if ( AID_OLD_26_APS_ULCSSN == GETAPP(pcMsg->srcid)
        || AID_APS_HTTP_SSN == GETAPP(pcMsg->srcid) )
	{
		ProcPostToDBMsg(pcMsg);
		return;
	}

	//其他消息
	switch (pcMsg->event )
	{
	case OSP_POWERON:
		DaemonProcPowerOn(pcMsg);
		break;
    
    case QUERY_SERVERDEV_LIST_TIMER:
        QueryServerDevListTimeOut(pcMsg);
        break;

    case QUERY_SERVERDEV_LIST_ACK:
        QueryServerDevListAck(pcMsg);
        break;

    case QUERY_SERVERDEV_LIST_NACk:
        QueryServerDevListNack(pcMsg);
        break;

    //服务器的健康检查, 放到CConSaSsnApp中去;
    //原因:很多线路都不通时，此函数会造成很长时间的堵塞,放在此处，会导整个程序性能的严重下降
    //case CHECK_HEALTH_TIMER:
    //break;
	default:
		CCtrlLog(LOG_LVL_WARNING, "[DaemonInstanceEntry] Wrong message %u(%s) received!\n",
			pcMsg->event, ::OspEventDesc(pcMsg->event));
        break;
	}
}

void CCenterCtrlInst::InstanceEntry(CMessage *const pcMsg)
{
    return;
}

void CCenterCtrlInst::DaemonProcPowerOn(CMessage *const pcMsg)
{
    //*****************启动顺序**************
    //start dbpool
    //请求服务器 列表
    //mqMsgPrase
    //ulcSsn/http/oldUlcSsn

	std::vector<TDBConnectInfo> tInfo = g_cCfgFile.GetDBConInfo();
	if ( tInfo.size() == 0 )
	{
        CCtrlLog(LOG_LVL_ERROR, "[CCenterCtrlInst::DaemonProcPowerOn] GetDBConInfo failed(size is 0)!\n");
		return;
	}

	CCtrlLog(LOG_LVL_KEYSTATUS, "[%s:%d]tinfo.size: %u tInfo[0].GetIp:[%s:%d][%s][%s][%s]\n", __FUNCTION__, __LINE__, 
		tInfo.size(), tInfo.begin()->GetDBIP(), tInfo.begin()->GetDBPort(),
		tInfo.begin()->GetDBName(), tInfo.begin()->GetDBUserName(), tInfo.begin()->GetDBUserPwd());

	BOOL32 bRet = m_tDBPool.StartDBPool(tInfo, DBCallBack);
    if (FALSE == bRet)
    {
        CCtrlLog(LOG_LVL_ERROR, "[CCenterCtrlInst::DaemonProcPowerOn] StartDBPool() fail !\n");
        return;
    }
	// apsPxy不需要Bmc推送服务器地址，都是从配置文件中获取的 [12/5/2015 pengguofeng]
#ifndef _APS_PXY_
    //请求服务器 列表
    SetTimer(QUERY_SERVERDEV_LIST_TIMER, 100);
#else
    static BOOL32 s_bInit = FALSE;
    if (FALSE == s_bInit)        //防止多次给app发送OSP_POWERON消息
    {
        s_bInit = TRUE;
        
        //启动http
        ::OspPost(MAKEIID(AID_APS_HTTP_SSN, CInstance::DAEMON), OSP_POWERON);
		
        //启动2.6版本的osp
        ::OspPost(MAKEIID(AID_OLD_26_APS_ULCSSN, CInstance::DAEMON), OSP_POWERON);
    }
#endif
	
    return;
}


void CCenterCtrlInst::QueryServerDevListTimeOut(CMessage *const pcMsg)
{
    KillTimer(QUERY_SERVERDEV_LIST_TIMER);

#ifdef _TEST_
    TGUID tMyGUID;
    tMyGUID.SetNOString("10107000000002000000000000000001");
#else
    //获取自己的GUID
    //TGUID tMyGUID = GetSrvGUID();
	TGUID tMyGUID;
	tMyGUID.SetNOString("10107000000002000000000000000009");
    if (!tMyGUID.IsValid())
    {
        printf("[QueryServerDevListTimeOut] GetSrvGUID failed\n");
        CCtrlLog(LOG_LVL_WARNING, "[QueryServerDevListTimeOut] GetSrvGUID failed\n");

        SetTimer(QUERY_SERVERDEV_LIST_TIMER, 5*1000);
        return;
    }
    printf("[QueryServerDevListTimeOut] GetSrvGUID:%s succeed\n", tMyGUID.GetNOString());
    CCtrlLog(LOG_LVL_WARNING, "[QueryServerDevListTimeOut] GetSrvGUID:%s succeed\n", tMyGUID.GetNOString());
#endif  //_TEST_

    CGuidExtend tExtend;
    tExtend.SetSessionID(GetAppID(), GetInsID());
    tExtend.SetDetailBody(tMyGUID);
    BOOL32 bRet = m_tDBPool.InsertMsgToDBPool(QUERY_SERVERDEV_LIST_REQ, (u8 *)&tExtend, sizeof(tExtend));
    if (!bRet)
    {
        CCtrlLog(LOG_LVL_WARNING, "[QueryServerDevListTimeOut] InsertMsgToDBPool fail\n");

        SetTimer(QUERY_SERVERDEV_LIST_TIMER, 5*1000);
        return;
    }

    SetTimer(QUERY_SERVERDEV_LIST_TIMER, 15*1000);
}

void CCenterCtrlInst::QueryServerDevListNack(CMessage *const pcMsg)
{
    KillTimer(QUERY_SERVERDEV_LIST_TIMER);

    CCtrlLog(LOG_LVL_WARNING, "[QueryServerDevListNack] Get Server Dev List Failed\n");

    SetTimer(QUERY_SERVERDEV_LIST_TIMER, 5*1000);
}

void CCenterCtrlInst::QueryServerDevListAck(CMessage *const pcMsg)
{
    CCtrlLog(LOG_LVL_KEYSTATUS, "[QueryServerDevListAck] Get Server Dev List Succeed!\n");
    KillTimer(QUERY_SERVERDEV_LIST_TIMER);

    //开始接收MQ消息
    while(TRUE)   //启动MQ
    {
        BOOL32 bRet = g_cCenterCtrlApp.StarMqMsgParse();
        if (!bRet)
        {
            CCtrlLog(LOG_LVL_WARNING, "[QueryServerDevListAck] StarMqMsgParse failed,retry after 3 secs\n");
            OspDelay(1000*3);
        }
        break;
    }
    CCtrlLog(LOG_LVL_KEYSTATUS, "[QueryServerDevListAck] StarMqMsgParse succ\n");
#ifndef _APS_PXY_
    static BOOL32 s_bInit = FALSE;
    if (FALSE == s_bInit)        //防止多次给app发送OSP_POWERON消息
    {
        s_bInit = TRUE;
        
        //启动http
        ::OspPost(MAKEIID(AID_APS_HTTP_SSN, CInstance::DAEMON), OSP_POWERON);

        //启动2.6版本的osp
        ::OspPost(MAKEIID(AID_OLD_26_APS_ULCSSN, CInstance::DAEMON), OSP_POWERON);
    }
#endif
}
