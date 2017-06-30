
#include "dbctrl.h"
#include "debugprint.h"
#include "dbopr.h"
#include "structnack.h"
#include "old_26_eventulculs.h"
#include "old_252_eventulculs.h"
#include "old_252_ulcstruct.h"
#include "errorcode.h"
#include "srvlistmgr.h"
#include "structulc.h"
#include "bossutils.h"
#include "eventself.h"
#include "config.h"
#include "pxycfg.h"
#include "centerctrl.h"

BOOL32 PostMsgToSSn(u16 wEvent, u8 *const pbyMsg, u16 wLen, u16 wAppID, u16 wInstId)
{
    BOOL32 bRet = TRUE;

    CCtrlLog(LOG_LVL_KEYSTATUS, "[PostMsgToSSn] message %u(%s)\n", wEvent, ::OspEventDesc(wEvent));

    if (OSP_OK != ::OspPost(MAKEIID(wAppID,wInstId), wEvent, pbyMsg, wLen))
    {
        CCtrlLog(LOG_LVL_ERROR, "[PostMsgToSSn] message %u(%s) Fail!", wEvent, ::OspEventDesc(wEvent));
        return FALSE;
    }

    return bRet;
}

BOOL32 PostNackToSsn(u16 wEvent, u16 wErrId, u16 wAppID, u16 wInstId)
{
    CApsBaseNack tNack;
    tNack.SetErrorId(wErrId);
    return PostMsgToSSn(wEvent, (u8*)&tNack, sizeof(tNack), wAppID, wInstId);
}

BOOL32 PostNackExtendToSsn(u16 wEvent, u16 wErrId, void* pHttpConnection, u16 wAppID, u16 wInstId, BOOL32 bIsHttpsClient )
{
    CApsBaseNack tNack;
    tNack.SetErrorId(wErrId);

    CUlsBaseNackExtend tExtend;
    tExtend.SetDetailBody(tNack);
    tExtend.SetHttpConnection(pHttpConnection);

	tExtend.SetIsHttpsConnection(bIsHttpsClient);

    return PostMsgToSSn(wEvent, (u8*)&tExtend, sizeof(tExtend), wAppID, wInstId);
}

void DBCallBack(u16 wEventID, u8 *pMsgBuf, u16 wMsgLen, IDbOpr *pDBOpr)
{
	if (NULL == pMsgBuf || NULL == pDBOpr)
    {
        CCtrlLog(LOG_LVL_ERROR, "[DBCallBack] NULL == pMsgBuf || NULL == pDBOpr\n");
        return;
    }
	
    CDBOpr *pApsDBOpr = dynamic_cast<CDBOpr *>(pDBOpr);

	switch (wEventID)
	{
    case QUERY_SERVERDEV_LIST_REQ:
        {
            if (sizeof(CGuidExtend) != wMsgLen)
            {
				CCtrlLog(LOG_LVL_ERROR, "[DBCallBack]QUERY_SERVERDEV_LIST_REQ wMsgLen(%u) != CGuidExtend size:%u\n",
					wMsgLen, sizeof(CGuidExtend));
                return;
            }

            CGuidExtend *pExtend = (CGuidExtend *)pMsgBuf;
            ProcQuerySrvDevListReq(pExtend, pApsDBOpr);
            break;
        }
    case MT_APS_LOGIN_REQ:
        {
            if (sizeof(CLoginExtend) != wMsgLen)
            {
				CCtrlLog(LOG_LVL_ERROR, "[DBCallBack]MT_APS_LOGIN_REQ wMsgLen(%u) != CLoginExtend size:%u\n",
					wMsgLen, sizeof(CLoginExtend));
                return;
            }

            CLoginExtend *pExtend = (CLoginExtend *)pMsgBuf;
            ProcUlcLoginReq(pExtend, pApsDBOpr);
            break;            
        }
	case OLD_26_ULC_ULS_LOGIN_REQ:
		{
			if (sizeof(CLoginExtend) != wMsgLen)
			{
				CCtrlLog(LOG_LVL_ERROR, "[DBCallBack]OLD_26_ULC_ULS_LOGIN_REQ wMsgLen(%u) != CLoginExtend size:%u\n",
					wMsgLen, sizeof(CLoginExtend));
				return;
			}
			
			CLoginExtend *pExtend = (CLoginExtend *)pMsgBuf;
			Proc26UlcLoginReq(pExtend, pApsDBOpr);
			break;
		}	
    case OLD_2_5_2_ULC_ULS_LOGIN_REQ:
        {
            if (sizeof(CLoginExtend) != wMsgLen)
            {
				CCtrlLog(LOG_LVL_ERROR, "[DBCallBack]OLD_2_5_2_ULC_ULS_LOGIN_REQ wMsgLen(%u) != CLoginExtend size:%u\n",
					wMsgLen, sizeof(CLoginExtend));
                return;
            }

            CLoginExtend *pExtend = (CLoginExtend *)pMsgBuf;
            Proc252UlcLoginReq(pExtend, pApsDBOpr);
            break;
        }
	//�������Դ�ӡ
	case SEARCH_PLATFORM_BACKUP_CONFIG_REQ:
		{
			PrintPlatformBackUpConfig(pApsDBOpr);
			break;
		}
	default:
		return;
	}

	return;
}

u16 ProcQuerySrvDevListReq(const CGuidExtend *ptExtend, CDBOpr *pcOprDB)
{
	CPrintFunInOut cPrintFun(__FUNCTION__);

    if (NULL == ptExtend || NULL == pcOprDB)
    {
        CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] NULL == ptExtend || NULL == pcOprDB\n");
        return COMMON_ERR_DBENGINE;
    }

    u16 wAppID = ptExtend->GetAppID();
    u16 wInstID = ptExtend->GetInstID();
    TGUID tMyGUID = ptExtend->GetDetailBody();

    //�����Լ����豸��Ϣ
    CServerDevice tMySrvDevInfo;
    u16 wErrId = pcOprDB->GetSrvDevInfoByGUID(tMyGUID.GetNOString(), tMySrvDevInfo);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

        CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] query myself device info fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }

    //��������ƽ̨����Ϣ(Ŀ����Ϊ�˻�ȡ������MOID)
    CPlatformDomain tPlatformDomain;
    wErrId = pcOprDB->GetPlatformDomain(tMySrvDevInfo.GetPlatformDomainMOID(), tPlatformDomain);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

        CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetPlatformDomain fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }

    g_cSrvListMgr.SetMySelfMOID(tMySrvDevInfo.GetMOID());
    g_cSrvListMgr.SetPlatformDomainMOID(tPlatformDomain.GetMOID());
    g_cSrvListMgr.SetServiceDomainMOID(tPlatformDomain.GetServiceDomainMOID());

    if (g_cCfgFile.IsPxyMode())
    {       // ����ģʽ-----�������ļ���ȡ��������ַ��Ϣ
        CCtrlLog(LOG_LVL_WARNING, "[ProcQuerySrvDevListReq] curr is PxyMode!!\n");

        //
        BOOL32 bExist = g_cPxyCfg.IsConfigFileExist();
        if (!bExist)
        {
            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] the PxyCfg:%s don't exit! return FALSE\n", g_cPxyCfg.GetCfgFileName());
            return FALSE;
        }

        BOOL32 bRet = g_cPxyCfg.ReadConfigInfo();
        if (!bRet)
        {
            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] read the PxyCfg:%s fail! return FALSE\n", g_cPxyCfg.GetCfgFileName());
            return FALSE;
        }

        std::set<CPxyPlatformAddr> setPxyAddr = g_cPxyCfg.GetPxyPlatformSrvAddr();
        g_cSrvListMgr.InsertPxyAddr(setPxyAddr);

		std::set<CBriefSrvInfo> setBriefSrvInfo;
		setBriefSrvInfo.clear();
		wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_APS, setBriefSrvInfo);
		if (SUCCESS_DBENGINE != wErrId)
		{
			PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

			CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo PAS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
			return wErrId;
		}
		g_cSrvListMgr.InsertToServerList(DEVICE_GLOBAL_TYPE_APS, setBriefSrvInfo);

    }
    else    // ����ģʽ-----�����ݿ��ȡ��������ַ��Ϣ
    {
		//���¹��:ֻ���汾ƽ̨����豸
		const map<u32, vector<CBriefSrvStateEx>*> mapTypeServerList = g_cSrvListMgr.GetTypeServerListMap();
		map<u32, vector<CBriefSrvStateEx>*>::const_iterator typeServerIter = mapTypeServerList.begin();
		for (; typeServerIter != mapTypeServerList.end(); ++typeServerIter)
		{
			std::set<CBriefSrvInfo> setBriefSrvInfo;
			wErrId = pcOprDB->GetPlatformBriefSrvInfo(tPlatformDomain.GetMOID(), typeServerIter->first, setBriefSrvInfo);
			if (SUCCESS_DBENGINE == wErrId)
			{		
				CCtrlLog(LOG_LVL_ERROR, "[%s] GetSrvDevInfo %u success\n", __FUNCTION__, typeServerIter->first);
				g_cSrvListMgr.InsertToServerList(typeServerIter->first, setBriefSrvInfo);
			}
			else
			{
				CCtrlLog(LOG_LVL_ERROR, "[%s] GetSrvDevInfo %u fail,ErrId = %d(%s) !\n", __FUNCTION__, 
					typeServerIter->first, wErrId, GetApsErrorStr(wErrId));
				PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);
				return wErrId;
			}
		}
    }

	//���������ã���ʱҲ��Ҫ�ϱ�
    PostMsgToSSn(QUERY_SERVERDEV_LIST_ACK, NULL, 0, wAppID, wInstID);
    return SUCCESS_DBENGINE;
}

u16 PrintPlatformBackUpConfig(CDBOpr *pcOprDB)
{
	CPrintFunInOut cPrintFun(__FUNCTION__);

	CBackUpConfig cBackUpConfig;
	u16 wErrCode = pcOprDB->GetPlatformBackUpConfig(g_cSrvListMgr.GetPlatformDomainMOID(), cBackUpConfig);
	CCtrlLog(LOG_LVL_ERROR, "Plat(%s), BackUpMode:<%s>, backUpPlat:%s\n", g_cSrvListMgr.GetPlatformDomainMOID(),
		GetBackUpModeNameByType(cBackUpConfig.GetBackUpMode()), cBackUpConfig.GetBackUpPlatMOID().c_str());
	return 0;
}

u16 ProcUlcLoginReq(const CLoginExtend *pLoginExtend, CDBOpr *pcOprDB)
{
	CPrintFunInOut cPrintFun(__FUNCTION__);

    if (NULL == pLoginExtend || NULL == pcOprDB)
    {
        CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] NULL == pLoginExtend || NULL == pcOprDB\n");
        return COMMON_ERR_DBENGINE;
    }

    u16 wAppID = pLoginExtend->GetAppID();
    u16 wInstID = pLoginExtend->GetInstID();
    CLoginInfo tLoginInfo = pLoginExtend->GetDetailBody();

    //��֤
    CDBMtInfo tDBMtInfo;
    CPlatformAttr tPlatformAttr;
    u16 wErrId = pcOprDB->Authentication(tLoginInfo, tDBMtInfo, tPlatformAttr);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackExtendToSsn(APS_MT_LOGIN_NACK, wErrId, pLoginExtend->GetHttpConnection(), wAppID, wInstID, tLoginInfo.IsHttpsClient());

        CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] Authentication fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }

    //�ն�IP������Ӫ��
#ifdef _APS_PXY_
    CPlatformSrvAddrInfo tPlatformSrvInfo;
	u8 byIspType = g_cCfgFile.GetIspTypeByLoginApsIp(tLoginInfo.GetApsLocalIp());
    CCtrlLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq] clientIp:%s IspType:%d!\n", BossUtils::GetIpString(tLoginInfo.GetClientIp()).c_str(), byIspType);
	// pxy����Mt����Ӫ�̷��ض�Ӧ�ĵ�ַ
	g_cCfgFile.GetPlatformSrvAddrInfo(byIspType, tDBMtInfo.GetPlatformDomainMoid(), tPlatformSrvInfo);
#else
	set<CBriefSrvInfo> setMainAps;
	set<CBriefSrvInfo> setBackUpAps;
	if((wErrId = GetMainOrBackUpApsList(tDBMtInfo.GetPlatformDomainMoid(), g_cSrvListMgr.GetPlatformDomainMOID(),
			setMainAps, setBackUpAps, pcOprDB)) != SUCCESS_DBENGINE)
    {		
        PostNackExtendToSsn(APS_MT_LOGIN_NACK, APS_ERR_REJECT_LOGIN, pLoginExtend->GetHttpConnection(), wAppID, wInstID, tLoginInfo.IsHttpsClient());
        CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] GetBackUpApsList fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }

    std::string strClientISPName;
    wErrId = pcOprDB->GetIpISPName(tLoginInfo.GetClientIp(), strClientISPName);
    if (SUCCESS_DBENGINE != wErrId && ERR_DBENGINE_INTER_NO_RESULTSET != wErrId)
    {		
        PostNackExtendToSsn(APS_MT_LOGIN_NACK, wErrId, pLoginExtend->GetHttpConnection(), wAppID, wInstID, tLoginInfo.IsHttpsClient());

        CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] GetIpISPName fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }
	const u8 byIspType = TransIspNameToType(strClientISPName);

    std::string strLocalApsIspName;
    g_cSrvListMgr.GetLocalApsIspName(tLoginInfo.GetApsLocalIp(), strLocalApsIspName);

    CPlatformSrvAddrInfo tPlatformSrvInfo;
	CApsSrvIpList listMainPlatAps;
	CApsSrvIpList listBackUpPlatAps;
	SortApsServerList(setMainAps, listMainPlatAps, strLocalApsIspName, strClientISPName);
	SortApsServerList(setBackUpAps, listBackUpPlatAps, strLocalApsIspName, strClientISPName);
	tPlatformSrvInfo.SetMainPlatApsList(listMainPlatAps);
	tPlatformSrvInfo.SetBackUpApsList(listBackUpPlatAps);
	if(!g_cSrvListMgr.GetPlatformSrvAddrInfo(tLoginInfo, strClientISPName, tDBMtInfo.GetPASMoid(), tPlatformSrvInfo))
	{
		//replay Nack
		wErrId = APS_ERR_ASSIGN_SRV_ERROR;
		PostNackExtendToSsn(APS_MT_LOGIN_NACK, wErrId, pLoginExtend->GetHttpConnection(), wAppID, wInstID, tLoginInfo.IsHttpsClient());

		CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] GetPlatformSrvAddrInfo fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
		return wErrId;
	}
    CCtrlLog(LOG_LVL_KEYSTATUS, "[%s] clientIp:%s IspName:%s! byIspType:%d\n", __FUNCTION__,
		BossUtils::GetIpString(tLoginInfo.GetClientIp()).c_str(), strClientISPName.c_str(), byIspType);
#endif

    //�ն���Ϣ
    CMtInfo tMtInfo;
    tMtInfo.SetMoid(tDBMtInfo.GetMoid());
    tMtInfo.SetAccount(tDBMtInfo.GetAccount());
    tMtInfo.SetE164NO(tDBMtInfo.GetE164NO());
    tMtInfo.SetEmail(tDBMtInfo.GetEmail());
    tMtInfo.SetPrivilegeList(tDBMtInfo.GetPrivilegeList());
    tMtInfo.SetXmppJID(tDBMtInfo.GetXmppJID());
    tMtInfo.SetXmppPwd(tLoginInfo.GetPassWord());   //Ŀǰ�Ĳ�����ֱ��ʹ�õ�¼����

    CUlcFullInfo tFullInfo;
    tFullInfo.SetMtInfo(tMtInfo);
	CSrvAddrInfo cNmsAddr = tPlatformSrvInfo.GetNmsAddrInfo();
//	cNmsAddr.SetPort(NMS_50MT_LISTENPORT);
    tPlatformSrvInfo.SetNmsAddrInfo(cNmsAddr);
	tFullInfo.SetSrvInfo(tPlatformSrvInfo);
    tFullInfo.SetPlatformAttr(tPlatformAttr);

    CUlcFullInfoExtend tExtend;
    tExtend.SetDetailBody(tFullInfo);
    tExtend.SetHttpConnection(pLoginExtend->GetHttpConnection());   //��סhttp ���Ӿ��
    tExtend.SetIsHttpsConnection(tLoginInfo.IsHttpsClient());

    PostMsgToSSn(APS_MT_LOGIN_ACK, (u8*)&tExtend, sizeof(tExtend), wAppID, wInstID);
	// ��֤�ɹ���Ҫ�ϱ����� [11/19/2015 pengguofeng]
	TMtRegNetInfo  tMtNmcInfo;
	u32 dwLen = strlen( tDBMtInfo.GetMoid() );
	if ( dwLen > sizeof(tMtNmcInfo.m_achMtMoid) - 1 )
	{
		dwLen = sizeof(tMtNmcInfo.m_achMtMoid) - 1;
	}
	if ( dwLen > 0 )
	{
		memcpy(tMtNmcInfo.m_achMtMoid, tDBMtInfo.GetMoid(), dwLen);
	}
	tMtNmcInfo.m_byNetType = (EOperatorsType)byIspType;
	ApsAddOneMtInfo(&tMtNmcInfo);
	CCtrlLog(LOG_LVL_KEYSTATUS, "[%s]ApsAddOneMtInfo OK MtMoid:%s  IspType:%u\n", __FUNCTION__, tMtNmcInfo.m_achMtMoid, tMtNmcInfo.m_byNetType);
	if ( g_cCenterCtrlApp.AddMtInfo(tMtNmcInfo) )
	{
		CCtrlLog(LOG_LVL_KEYSTATUS, "[%s]AddMtInfo succeed\n", __FUNCTION__);
	}
	else
	{
		CCtrlLog(LOG_LVL_ERROR, "[%s]AddMtInfo failed\n", __FUNCTION__);
	}
    return SUCCESS_DBENGINE;
}

u16 Proc26UlcLoginReq(const CLoginExtend *ptExtend, CDBOpr *pcOprDB)
{
    if (NULL == ptExtend || NULL == pcOprDB)
    {
        CCtrlLog(LOG_LVL_ERROR, "[Proc26UlcLoginReq] NULL == ptExtend || NULL == pcOprDB\n");
        return COMMON_ERR_DBENGINE;
    }

    u16 wAppID = ptExtend->GetAppID();
    u16 wInstID = ptExtend->GetInstID();
    CLoginInfo tLoginInfo = ptExtend->GetDetailBody();
    //��֤
    CDBMtInfo tDBMtInfo;
    CPlatformAttr tPlatformAttr;
    u16 wErrId = pcOprDB->Authentication(tLoginInfo, tDBMtInfo, tPlatformAttr);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackToSsn(OLD_26_ULS_ULC_LOGIN_NACK, wErrId, wAppID, wInstID);

        CCtrlLog(LOG_LVL_ERROR, "[Proc26UlcLoginReq] Authentication fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }

	set<CBriefSrvInfo> setMainAps;
	set<CBriefSrvInfo> setBackUpAps;
	if((wErrId = GetMainOrBackUpApsList(tDBMtInfo.GetPlatformDomainMoid(), g_cSrvListMgr.GetPlatformDomainMOID(),
			setMainAps, setBackUpAps, pcOprDB)) != SUCCESS_DBENGINE)
    {		
        //���ն˲������¼�ֱ�ƽ̨�������ܣ�����֪�������з��ظ��ն�һ���û������ڵĴ�����
		PostNackToSsn(OLD_26_ULS_ULC_LOGIN_NACK, APS_ERR_ACCOUNT_NAME_NOTEXIST, wAppID, wInstID);
        CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] GetBackUpApsList fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }

    //�ն�IP������Ӫ��
#ifdef _APS_PXY_
	u8 byIspType = g_cCfgFile.GetIspTypeByLoginApsIp(tLoginInfo.GetApsLocalIp());
#else
	u8 byIspType = (u8)emTypeBD;
    //����nginxת��osp������ն˵�Դ��ַ�Ѿ������ˣ����ܵ�IP DB�в�ѯ��Ӫ������
    tLoginInfo.SetClientIp(0);  //ǿ��д��0
#endif

    //�ն���Ϣ
    COld26UlcMtInfo tMtInfo;
    tMtInfo.SetE164NO(tDBMtInfo.GetE164NO());
    tMtInfo.SetEmail(tDBMtInfo.GetEmail());
    tMtInfo.SetMoid(tDBMtInfo.GetMoid());
    tMtInfo.SetPrivilegeList(tDBMtInfo.GetPrivilegeList());

    //��������ַ
    CPlatformSrvAddrInfo tConfSrvInfo;
#ifdef _APS_PXY_
	// pxy����Mt����Ӫ�̷��ض�Ӧ�ĵ�ַ
	g_cCfgFile.GetPlatformSrvAddrInfo((u8)byIspType, tDBMtInfo.GetPlatformDomainMoid(), tConfSrvInfo);
#else
	if (!g_cSrvListMgr.GetPlatformSrvAddrInfo(tLoginInfo, "", tDBMtInfo.GetPASMoid(), tConfSrvInfo))
	{
		//replay Nack: 2.6���նˣ�������¼��ʱ�����Ǵ����E164�ţ�������
		wErrId = APS_ERR_ASSIGN_SRV_ERROR;
        PostNackToSsn(OLD_26_ULS_ULC_LOGIN_NACK, wErrId, wAppID, wInstID);
        CCtrlLog(LOG_LVL_ERROR, "[Proc26UlcLoginReq] GetPlatformSrvAddrInfo fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
	}
#endif

    //PlatApi url
    std::string strPlatApiUrl;
    if (tConfSrvInfo.GetPlatApiAddrInfo().IsIpValid())
    {
        strPlatApiUrl = BossUtils::GetIpString(tConfSrvInfo.GetPlatApiAddrInfo().GetIpAddr());
    }
    else
    {
        strPlatApiUrl = tConfSrvInfo.GetPlatApiAddrInfo().GetNetDomainName();
    }

    //Moooo url
    std::string strMooooUrl;
    if (tConfSrvInfo.GetWeiboAddrInfo().IsIpValid())
    {
        strMooooUrl = BossUtils::GetIpString(tConfSrvInfo.GetWeiboAddrInfo().GetIpAddr());
    }
    else
    {
        strMooooUrl = tConfSrvInfo.GetWeiboAddrInfo().GetNetDomainName();
    }

    //xmpp url
    std::string strXmppUrl;
    if (tConfSrvInfo.GetXmppAddrInfo().IsIpValid())
    {
        strXmppUrl = BossUtils::GetIpString(tConfSrvInfo.GetXmppAddrInfo().GetIpAddr());
    }
    else
    {
        strXmppUrl = tConfSrvInfo.GetXmppAddrInfo().GetNetDomainName();
    }

    //ƽ̨��Ϣ
    COld26XnuInfo tXnu;
    tXnu.SetXmppUrl(strXmppUrl.c_str());
    tXnu.SetXmppPort(g_cCfgFile.GetServerPort(DEVICE_GLOBAL_TYPE_XNS));
    tXnu.SetXmppJID(tDBMtInfo.GetXmppJID());
    tXnu.SetPwd(tLoginInfo.GetPassWord());

    COld26UlcPlatformSrvInfo tSrvInfo;
    tSrvInfo.SetXnuInfo(tXnu);
    tSrvInfo.SetPlatApiUrl(strPlatApiUrl.c_str());
    tSrvInfo.SetMooooUrl(strMooooUrl.c_str());    
    tSrvInfo.SetSusAddrInfo(tConfSrvInfo.GetSusAddrInfo());
	CSrvAddrInfo cNmsAddr = tConfSrvInfo.GetNmsAddrInfo();
//	cNmsAddr.SetPort(NMS_26MT_LISTENPORT);
    tSrvInfo.SetNmsAddrInfo(cNmsAddr);
    tSrvInfo.SetNtsAddrInfo(tConfSrvInfo.GetNdsAddrInfo());
    tSrvInfo.SetCsuAddrInfo(tConfSrvInfo.GetNonH323AddrInfo());
    tSrvInfo.SetXapList(tConfSrvInfo.GetApsList());

    COld26UlcFullInfo tFullInfo;
    tFullInfo.SetMtInfo(tMtInfo);
    tFullInfo.SetSrvInfo(tSrvInfo);
	tFullInfo.SetPlatVer( "v50" );
	
    PostMsgToSSn(OLD_26_ULS_ULC_LOGIN_ACK, (u8*)&tFullInfo, sizeof(tFullInfo), wAppID, wInstID);

	// ��֤�ɹ���Ҫ�ϱ����ܣ����ն���Tcp��ֱ�����ApsIp����Ӫ�� [11/19/2015 pengguofeng]
	TMtRegNetInfo  tMtNmcInfo;
	u32 dwLen = strlen( tDBMtInfo.GetMoid() );
	if ( dwLen > sizeof(tMtNmcInfo.m_achMtMoid) - 1 )
	{
		dwLen = sizeof(tMtNmcInfo.m_achMtMoid) - 1;
	}
	memcpy(tMtNmcInfo.m_achMtMoid, tDBMtInfo.GetMoid(), dwLen);
	tMtNmcInfo.m_byNetType = (EOperatorsType)byIspType;
	ApsAddOneMtInfo(&tMtNmcInfo);
	CCtrlLog(LOG_LVL_KEYSTATUS, "[%s]ApsAddOneMtInfo OK MtMoid:%s  IspType:%u\n", __FUNCTION__, tMtNmcInfo.m_achMtMoid, tMtNmcInfo.m_byNetType);
	if ( g_cCenterCtrlApp.AddMtInfo(tMtNmcInfo) )
	{
		CCtrlLog(LOG_LVL_KEYSTATUS, "[%s]AddMtInfo succeed\n", __FUNCTION__);
	}
	else
	{
		CCtrlLog(LOG_LVL_ERROR, "[%s]AddMtInfo failed\n", __FUNCTION__);
	}

    return SUCCESS_DBENGINE;
}


/*lint -save -e845 -e669*/
u16 Proc252UlcLoginReq(const CLoginExtend *ptExtend, CDBOpr *pcOprDB)
{
    if (NULL == ptExtend || NULL == pcOprDB)
    {
        CCtrlLog(LOG_LVL_ERROR, "[Proc252UlcLoginReq] NULL == ptExtend || NULL == pcOprDB\n");
        return COMMON_ERR_DBENGINE;
    }

    u16 wAppID = ptExtend->GetAppID();
    u16 wInstID = ptExtend->GetInstID();
    CLoginInfo tLoginInfo = ptExtend->GetDetailBody();
    //��֤
    CDBMtInfo tDBMtInfo;
    CPlatformAttr tPlatformAttr;
    u16 wErrId = pcOprDB->Authentication(tLoginInfo, tDBMtInfo, tPlatformAttr);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackToSsn(OLD_2_5_2_ULS_ULC_LOGIN_NACK, wErrId, wAppID, wInstID);
        CCtrlLog(LOG_LVL_ERROR, "[Proc252UlcLoginReq] Authentication fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }
	

	set<CBriefSrvInfo> setMainAps;
	set<CBriefSrvInfo> setBackUpAps;
	if((wErrId = GetMainOrBackUpApsList(tDBMtInfo.GetPlatformDomainMoid(), g_cSrvListMgr.GetPlatformDomainMOID(),
		setMainAps, setBackUpAps, pcOprDB)) != SUCCESS_DBENGINE)
	{		
		//���ն˲������¼�ֱ�ƽ̨�������ܣ�����֪�������з��ظ��ն�һ���û������ڵĴ�����
		PostNackToSsn(OLD_2_5_2_ULS_ULC_LOGIN_NACK, APS_ERR_ACCOUNT_NAME_NOTEXIST, wAppID, wInstID);
		CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] GetBackUpApsList fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
		return wErrId;
	}

    //�ն�IP������Ӫ��
#ifdef _APS_PXY_
	u8 byIspType = g_cCfgFile.GetIspTypeByLoginApsIp(tLoginInfo.GetApsLocalIp());
#else
	u8 byIspType = (u8)emTypeBD;
    //����nginxת��osp������ն˵�Դ��ַ�Ѿ������ˣ����ܵ�IP DB�в�ѯ��Ӫ������
    tLoginInfo.SetClientIp(0);  //ǿ��д��0
#endif

//     //2.5.2�汾���նˣ��ڵ�¼������û��Я���ն�IP��ַ��Ϣ�����Ҿ���nginxת��osp������ն˵�Դ��ַ�Ѿ������ˣ����ܵ�IP DB�в�ѯ��Ӫ������
//     //Ϊ�˽�����ݵ����⣬ֻ�ܽ���aps��ISP�������ն˵�ISP
//     tLoginInfo.SetClientIp(tLoginInfo.GetApsLocalIp());
//     std::string strISPName;
//     wErrId = pcOprDB->GetIpISPName(tLoginInfo.GetClientIp(), strISPName);
//     if (SUCCESS_DBENGINE != wErrId && ERR_DBENGINE_INTER_NO_RESULTSET != wErrId)
//     {
//         wErrId = APS_ERR_SERVER_INSIDE_ERR;
//         PostNackToSsn(OLD_2_5_2_ULS_ULC_LOGIN_NACK, wErrId, wAppID, wInstID);
// 
//         CCtrlLog(LOG_LVL_ERROR, "[Proc252UlcLoginReq] GetIpISPName fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
//         return wErrId;
//     }
//     CCtrlLog(LOG_LVL_KEYSTATUS, "[Proc252UlcLoginReq] ApsLocalIp:%s IspName:%s!\n", BossUtils::GetIpString(tLoginInfo.GetApsLocalIp()).c_str(), strISPName.c_str());

    //��������ַ
    CPlatformSrvAddrInfo tConfSrvInfo;
#ifdef _APS_PXY_
	// pxy����Mt����Ӫ�̷��ض�Ӧ�ĵ�ַ
	g_cCfgFile.GetPlatformSrvAddrInfo((u8)byIspType, tDBMtInfo.GetPlatformDomainMoid(), tConfSrvInfo);
#else
	if (!g_cSrvListMgr.GetPlatformSrvAddrInfo(tLoginInfo, "", tDBMtInfo.GetPASMoid(), tConfSrvInfo))
	{
		//replay Nack: 2.5���նˣ�������¼��ʱ�����Ǵ����E164�ţ�������
		wErrId = APS_ERR_ASSIGN_SRV_ERROR;
        PostNackToSsn(OLD_2_5_2_ULS_ULC_LOGIN_NACK, wErrId, wAppID, wInstID);
        CCtrlLog(LOG_LVL_ERROR, "[Proc252UlcLoginReq] GetPlatformSrvAddrInfo fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
	}
#endif

    //�ṹ��ת��
    TOld252SrvAddrListInfo tSrvAddrListInfo;
    tSrvAddrListInfo.SetGkAddr(tConfSrvInfo.GetNonH323AddrInfo());
    tSrvAddrListInfo.SetNmaAddr(tConfSrvInfo.GetNmsAddrInfo());
    tSrvAddrListInfo.SetNtsAddr(tConfSrvInfo.GetNdsAddrInfo());
    tSrvAddrListInfo.SetSusAddr(tConfSrvInfo.GetSusAddrInfo());
    
    //��ӡ
    COldSrvAddrInfo tGkInfo = tSrvAddrListInfo.GetGkAddr();
    COldSrvAddrInfo tNmaInfo = tSrvAddrListInfo.GetNmaAddr();
    COldSrvAddrInfo tNtsInfo = tSrvAddrListInfo.GetNtsAddr();
    COldSrvAddrInfo tSusInfo = tSrvAddrListInfo.GetSusAddr();
    CCtrlLog(LOG_LVL_DETAIL, "[Proc252UlcLoginReq]**********GK IpAddr:%s, Nma IpAddr:%s, Nts IpAddr:%s, SUS IpAddr:%s !\n", 
        BossUtils::GetIpString(tGkInfo.GetIpAddr()).c_str(), BossUtils::GetIpString(tNmaInfo.GetIpAddr()).c_str(), 
        BossUtils::GetIpString(tNtsInfo.GetIpAddr()).c_str(), BossUtils::GetIpString(tSusInfo.GetIpAddr()).c_str());


    CApsSrvIpList tUlsIpList = tConfSrvInfo.GetApsList();

    u32 dwOffset = 0;
    u8 byUlsIpBuf[MAXNUM_ULSIPLIST*12*sizeof(u32)+16] = { 0 };

    //����
    u8 bySrvType = emSrvTypeUls;
	if ( dwOffset < sizeof(byUlsIpBuf)-1 )
	{
		memcpy(byUlsIpBuf + dwOffset, &bySrvType, sizeof(bySrvType) );
		dwOffset += sizeof(bySrvType);
	}

    //����
    u8 byIpNum = tUlsIpList.Size();
	if ( dwOffset < sizeof(byUlsIpBuf)-1 )
	{
	    memcpy(byUlsIpBuf + dwOffset, &byIpNum, sizeof(byIpNum) );
		dwOffset += sizeof(byIpNum);
	}

    //ÿ��IP(������)
    for (u32 dwIndex = 0; dwIndex < tUlsIpList.Size() && (dwOffset+sizeof(u32)) < sizeof(byUlsIpBuf); ++dwIndex)
    {
        CSrvAddrInfo tUlsIp = tUlsIpList.GetValue(dwIndex);
        u32 dwIp = tUlsIp.GetIpAddr();
        dwIp = htonl(dwIp);
		if ( dwOffset < sizeof(byUlsIpBuf)-1 )
		{
			memcpy(byUlsIpBuf + dwOffset, &dwIp, sizeof(u32) );
			dwOffset += sizeof(u32);
		}
    }

    CServMsg tAck;
    tAck.SetMsgBody((u8*)&tSrvAddrListInfo, sizeof(tSrvAddrListInfo));
    tAck.CatMsgBody(byUlsIpBuf, dwOffset);
    PostMsgToSSn(OLD_2_5_2_ULS_ULC_LOGIN_ACK, (u8*)tAck.GetMsgBody(), tAck.GetMsgBodyLen(), wAppID, wInstID);	
	// ��֤�ɹ���Ҫ�ϱ����ܣ����ն���Tcp��ֱ�����ApsIp����Ӫ�� [11/19/2015 pengguofeng]
	TMtRegNetInfo  tMtNmcInfo;
	u32 dwLen = strlen( tDBMtInfo.GetMoid() );
	if ( dwLen > sizeof(tMtNmcInfo.m_achMtMoid) - 1 )
	{
		dwLen = sizeof(tMtNmcInfo.m_achMtMoid) - 1;
	}
	memcpy(tMtNmcInfo.m_achMtMoid, tDBMtInfo.GetMoid(), dwLen);
	tMtNmcInfo.m_byNetType = (EOperatorsType)byIspType;
	ApsAddOneMtInfo(&tMtNmcInfo);
	CCtrlLog(LOG_LVL_KEYSTATUS, "[%s]ApsAddOneMtInfo OK MtMoid:%s  IspType:%u\n", __FUNCTION__, tMtNmcInfo.m_achMtMoid, tMtNmcInfo.m_byNetType);
	if ( g_cCenterCtrlApp.AddMtInfo(tMtNmcInfo) )
	{
		CCtrlLog(LOG_LVL_KEYSTATUS, "[%s]AddMtInfo succeed\n", __FUNCTION__);
	}
	else
	{
		CCtrlLog(LOG_LVL_ERROR, "[%s]AddMtInfo failed\n", __FUNCTION__);
	}

    return SUCCESS_DBENGINE;
}
/*lint -restore*/
