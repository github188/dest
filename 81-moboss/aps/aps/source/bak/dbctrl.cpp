
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
				CCtrlLog(LOG_LVL_ERROR, "[DBCallBack]1111 wMsgLen(%u) != CGuidExtend size:%u\n",
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
				CCtrlLog(LOG_LVL_ERROR, "[DBCallBack]2222 wMsgLen(%u) != CLoginExtend size:%u\n",
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
				CCtrlLog(LOG_LVL_ERROR, "[DBCallBack]3333 wMsgLen(%u) != CLoginExtend size:%u\n",
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
				CCtrlLog(LOG_LVL_ERROR, "[DBCallBack]4444 wMsgLen(%u) != CLoginExtend size:%u\n",
					wMsgLen, sizeof(CLoginExtend));
                return;
            }

            CLoginExtend *pExtend = (CLoginExtend *)pMsgBuf;
            Proc252UlcLoginReq(pExtend, pApsDBOpr);
            break;
        }
	default:
		return;
	}

	return;
}

u16 ProcQuerySrvDevListReq(const CGuidExtend *ptExtend, CDBOpr *pcOprDB)
{
    if (NULL == ptExtend || NULL == pcOprDB)
    {
        CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] NULL == ptExtend || NULL == pcOprDB\n");
        return COMMON_ERR_DBENGINE;
    }

    u16 wAppID = ptExtend->GetAppID();
    u16 wInstID = ptExtend->GetInstID();
    TGUID tMyGUID = ptExtend->GetDetailBody();

    //查找自己的设备信息
    CServerDevice tMySrvDevInfo;
    u16 wErrId = pcOprDB->GetSrvDevInfoByGUID(tMyGUID.GetNOString(), tMySrvDevInfo);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

        CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] query myself device info fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }

    //查找所属平台域信息(目的是为了获取服务域MOID)
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

	set<string> setPlatformDomainMOID;
	wErrId = pcOprDB->GetAllPlatformDomainMOID(tPlatformDomain.GetServiceDomainMOID(), setPlatformDomainMOID);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

        CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetAllPlatformDomain fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }
	g_cSrvListMgr.SetAllPlatformDomainMOID(setPlatformDomainMOID);

	//查找所有平台域的信息

    if (g_cCfgFile.IsPxyMode())
    {       // 代理模式-----从配置文件读取服务器地址信息
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
    }
    else    // 正常模式-----从数据库读取服务器地址信息
    {
        //此服务域下的 NDS(NTS) 设备
        std::set<CBriefSrvInfo> setBriefSrvInfo;
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_NDS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo NTS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToNdsList(setBriefSrvInfo);

        //此服务域下的 NMS 设备
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_NMS_C, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo NMS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToNmsList(setBriefSrvInfo);

		//zjj20160908 由于历史原因,原来nms读的是nms_6000类型的nms(devicetype是312),
		//但从sp4版本开始已经不会入网nms_6000类型了
		//为了兼容老版本,所有nms_6000的类型也读一下
		setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_NMS_6000, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo NMS_6000 fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToNmsList(setBriefSrvInfo);
		

        //此服务域下的 NU 设备
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_PAS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo NU fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToPasList(setBriefSrvInfo);

        //此服务域下的 BGS 设备
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_BGS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo BGS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToBgsList(setBriefSrvInfo);

        //此服务域下的 SUS 设备
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_SUS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo SUS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToSusList(setBriefSrvInfo);

        //此服务域下的 SNS 设备
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_SNS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo SNS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToSnsList(setBriefSrvInfo);

        //此服务域下的 XNU 设备
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_XNS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo XNS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToXnsList(setBriefSrvInfo);


        //此服务域下的 CMC 设备
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_CMC, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo CMC fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToCmcList(setBriefSrvInfo);

        //此服务域下的 NTP 设备
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_NTP, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo NTP fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToNtpList(setBriefSrvInfo);

        //此服务域下的 VRS 设备 (VRS又分型号VRS2000与VRS4000)
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_VRS2000, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo VRS2000 fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToVrsList(setBriefSrvInfo);

        //
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_VRS4000, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo VRS4000 fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToVrsList(setBriefSrvInfo);

        //此服务域下的 DCS 设备
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_DCS2000, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo DCS2000 fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToDcsList(setBriefSrvInfo);

		//此服务域下的RestApi设备
		setBriefSrvInfo.clear();
		wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_RESTAPI, setBriefSrvInfo);
		if (SUCCESS_DBENGINE != wErrId)
		{		
			PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

			CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo RESTAPI fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
			return wErrId;
		}
		g_cSrvListMgr.InsertToRestList(setBriefSrvInfo);
    }

    //此服务域下的 APS 设备
    std::set<CBriefSrvInfo> setBriefSrvInfo;
    setBriefSrvInfo.clear();
    wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_APS, setBriefSrvInfo);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

        CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo PAS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }
    g_cSrvListMgr.InsertToApsList(setBriefSrvInfo);

	//检索到设置，此时也需要上报
    PostMsgToSSn(QUERY_SERVERDEV_LIST_ACK, NULL, 0, wAppID, wInstID);
    return SUCCESS_DBENGINE;
}

u16 ProcUlcLoginReq(const CLoginExtend *pLoginExtend, CDBOpr *pcOprDB)
{
    if (NULL == pLoginExtend || NULL == pcOprDB)
    {
        CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] NULL == pLoginExtend || NULL == pcOprDB\n");
        return COMMON_ERR_DBENGINE;
    }

    u16 wAppID = pLoginExtend->GetAppID();
    u16 wInstID = pLoginExtend->GetInstID();
    CLoginInfo tLoginInfo = pLoginExtend->GetDetailBody();

    //验证
    CDBMtInfo tDBMtInfo;
    CPlatformAttr tPlatformAttr;
    u16 wErrId = pcOprDB->Authentication(tLoginInfo, tDBMtInfo, tPlatformAttr);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackExtendToSsn(APS_MT_LOGIN_NACK, wErrId, pLoginExtend->GetHttpConnection(), wAppID, wInstID, tLoginInfo.IsHttpsClient());

        CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] Authentication fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }

#ifndef _APS_PXY_
    //判断终端是否漫游登录
    if ( 0 != strcmp(g_cSrvListMgr.GetServiceDomainMOID(), tDBMtInfo.GetServiceDomainMoid())
        && NULL == strstr(tDBMtInfo.GetPrivilegeList(), USERDOMAIN_PRIVILEGE_ROAM) )
    {
        wErrId = APS_ERR_NO_ROAM_PRIVILEGE;
        CCtrlLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq] %s is roam login, but don't have roam privilege!\n", tLoginInfo.GetUserName());
        PostNackExtendToSsn(APS_MT_LOGIN_NACK, wErrId, pLoginExtend->GetHttpConnection(), wAppID, wInstID, tLoginInfo.IsHttpsClient());

        CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] Authentication fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }
#endif

    //终端IP所属运营商
#ifdef _APS_PXY_
	u8 byIspType = g_cCfgFile.GetIspTypeByLoginApsIp(tLoginInfo.GetApsLocalIp());
#else
    std::string strISPName;
    wErrId = pcOprDB->GetIpISPName(tLoginInfo.GetClientIp(), strISPName);
    if (SUCCESS_DBENGINE != wErrId && ERR_DBENGINE_INTER_NO_RESULTSET != wErrId)
    {		
        PostNackExtendToSsn(APS_MT_LOGIN_NACK, wErrId, pLoginExtend->GetHttpConnection(), wAppID, wInstID, tLoginInfo.IsHttpsClient());

        CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] GetIpISPName fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }
	u8 byIspType = TransIspNameToType(strISPName);

	BOOL32 bIsUseDMZ = FALSE;
	//zjj20160822 由于dmz模式作用于某个运营商下
	//所以需要判断终端注册的运营商是否需要dmz模式，如果不需要
	//则还是走普通的模式
	if( g_cCfgFile.IsDmzMode() )
	{
		std::string strLocalApsIspName;
		if( g_cCfgFile.IsLocalIpUseDmz( tLoginInfo.GetApsLocalIp(),strLocalApsIspName ) )
		{
			strISPName = strLocalApsIspName;
			CCtrlLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq] operator useDmz strLocalApsIspName.%s\n",strLocalApsIspName.c_str() );
			bIsUseDMZ = TRUE;
		}
		/*if( emTypeBD == byIspType )
		{
			if( !g_cSrvListMgr.IsIpEqualIspName(tLoginInfo.GetApsLocalIp(),strISPName ) )
			{
				CCtrlLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq] loaapsispname.%s ip.%08x is not local的ip\n",
						strISPName.c_str(),tLoginInfo.GetClientIp() );
				byIspType = emTypeQT;
			}
		}
		if( byIspType != emTypeQT )
		{
			if( g_cCfgFile.IsOperatorUseDmz(strISPName) )
			{
				//byIspType = emTypeBD;
				bIsUseDMZ = TRUE;
			}
		}
		else
		{	
			std::string strLocalApsIspName;
			g_cSrvListMgr.GetLocalPasIspName(tLoginInfo.GetApsLocalIp(), strLocalApsIspName);
			CCtrlLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq] loaapsispname.%s\n",strLocalApsIspName.c_str() );
			if( g_cCfgFile.IsOperatorUseDmz(strLocalApsIspName) )
			{
				//byIspType = emTypeBD;
				bIsUseDMZ = TRUE;
				strISPName = strLocalApsIspName;
				CCtrlLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq] operator useDmz\n" );
			}	
			else
			{
				if( g_cCfgFile.IsLocalIpUseDmz( tLoginInfo.GetApsLocalIp(),strLocalApsIspName ) )
				{
					strISPName = strLocalApsIspName;
					CCtrlLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq] operator useDmz strLocalApsIspName.%s\n",strLocalApsIspName.c_str() );
					bIsUseDMZ = TRUE;
				}
				else
				{
					CCtrlLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq] operator not useDmz\n" );
				}	
			}
			
			
		}*/
		
	}
#endif

#ifdef _APS_PXY_
    CCtrlLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq] clientIp:%s IspType:%d!\n", BossUtils::GetIpString(tLoginInfo.GetClientIp()).c_str(), byIspType);
#else
    CCtrlLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq] clientIp:%s IspName:%s! byIspType:%d\n", BossUtils::GetIpString(tLoginInfo.GetClientIp()).c_str(), strISPName.c_str(), byIspType);
#endif

    //服务器地址
	// 即使某个地址找不到，Aps也不需要返回Login_Nack [12/14/2015 pengguofeng]
    CPlatformSrvAddrInfo tPlatformSrvInfo;
#ifdef _APS_PXY_
	// pxy根据Mt的运营商返回对应的地址
	g_cCfgFile.GetPlatformSrvAddrInfo(byIspType, tDBMtInfo.GetPlatformDomainMoid(), tPlatformSrvInfo);
#else

	//if ( g_cCfgFile.IsDmzMode() && byIspType != emTypeBD )
	if( bIsUseDMZ )
	{
		g_cCfgFile.GetPlatformDmzAddrInfo( strISPName, tDBMtInfo.GetPlatformDomainMoid(), tPlatformSrvInfo);
	} 
	else
	{
		BOOL32 bRet = g_cSrvListMgr.GetPlatformSrvAddrInfo(tLoginInfo, strISPName, tDBMtInfo.GetPlatformDomainMoid(),  tDBMtInfo.GetPASMoid(), tPlatformSrvInfo);
		if ( bRet == FALSE )
		{
			//replay Nack
			wErrId = APS_ERR_ASSIGN_SRV_ERROR;
			PostNackExtendToSsn(APS_MT_LOGIN_NACK, wErrId, pLoginExtend->GetHttpConnection(), wAppID, wInstID, tLoginInfo.IsHttpsClient());

			CCtrlLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] GetPlatformSrvAddrInfo fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
			return wErrId;
		}
	}
#endif

    //终端信息
    CMtInfo tMtInfo;
    tMtInfo.SetMoid(tDBMtInfo.GetMoid());
    tMtInfo.SetAccount(tDBMtInfo.GetAccount());
    tMtInfo.SetE164NO(tDBMtInfo.GetE164NO());
    tMtInfo.SetEmail(tDBMtInfo.GetEmail());
    tMtInfo.SetPrivilegeList(tDBMtInfo.GetPrivilegeList());
    tMtInfo.SetXmppJID(tDBMtInfo.GetXmppJID());
    tMtInfo.SetXmppPwd(tLoginInfo.GetPassWord());   //目前的策略是直接使用登录密码

    CUlcFullInfo tFullInfo;
    tFullInfo.SetMtInfo(tMtInfo);
	CSrvAddrInfo cNmsAddr = tPlatformSrvInfo.GetNmsAddrInfo();
//	cNmsAddr.SetPort(NMS_50MT_LISTENPORT);
    tPlatformSrvInfo.SetNmsAddrInfo(cNmsAddr);
	tFullInfo.SetSrvInfo(tPlatformSrvInfo);
    tFullInfo.SetPlatformAttr(tPlatformAttr);

    CUlcFullInfoExtend tExtend;
    tExtend.SetDetailBody(tFullInfo);
    tExtend.SetHttpConnection(pLoginExtend->GetHttpConnection());   //记住http 连接句柄
    tExtend.SetIsHttpsConnection(tLoginInfo.IsHttpsClient());

    PostMsgToSSn(APS_MT_LOGIN_ACK, (u8*)&tExtend, sizeof(tExtend), wAppID, wInstID);
	// 认证成功需要上报网管 [11/19/2015 pengguofeng]
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

    //验证
    CDBMtInfo tDBMtInfo;
    CPlatformAttr tPlatformAttr;
    u16 wErrId = pcOprDB->Authentication(tLoginInfo, tDBMtInfo, tPlatformAttr);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackToSsn(OLD_26_ULS_ULC_LOGIN_NACK, wErrId, wAppID, wInstID);

        CCtrlLog(LOG_LVL_ERROR, "[Proc26UlcLoginReq] Authentication fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }
    //终端IP所属运营商
#ifdef _APS_PXY_
	u8 byIspType = g_cCfgFile.GetIspTypeByLoginApsIp(tLoginInfo.GetApsLocalIp());
#else
	u8 byIspType = (u8)emTypeBD;
    //经过nginx转发osp信令后，终端的源地址已经不对了，不能到IP DB中查询运营商名称
    tLoginInfo.SetClientIp(0);  //强制写成0
#endif

    //终端信息
    COld26UlcMtInfo tMtInfo;
    tMtInfo.SetE164NO(tDBMtInfo.GetE164NO());
    tMtInfo.SetEmail(tDBMtInfo.GetEmail());
    tMtInfo.SetMoid(tDBMtInfo.GetMoid());
    tMtInfo.SetPrivilegeList(tDBMtInfo.GetPrivilegeList());

    //服务器地址
    CPlatformSrvAddrInfo tConfSrvInfo;
#ifdef _APS_PXY_
	// pxy根据Mt的运营商返回对应的地址
	g_cCfgFile.GetPlatformSrvAddrInfo((u8)byIspType, tDBMtInfo.GetPlatformDomainMoid(), tConfSrvInfo);
#else
    BOOL32 bRet = g_cSrvListMgr.GetPlatformSrvAddrInfo(tLoginInfo, "", tDBMtInfo.GetPlatformDomainMoid(),  tDBMtInfo.GetPASMoid(), tConfSrvInfo);
	if ( bRet == FALSE )
	{
		//replay Nack: 2.6的终端，遇到登录超时，或是错误的E164号，会重连
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

    //平台信息
    COld26XnuInfo tXnu;
    tXnu.SetXmppUrl(strXmppUrl.c_str());
    tXnu.SetXmppPort(g_cCfgFile.GetXnsPort());
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

	// 认证成功需要上报网管：老终端走Tcp，直接填本地ApsIp的运营商 [11/19/2015 pengguofeng]
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

    //验证
    CDBMtInfo tDBMtInfo;
    CPlatformAttr tPlatformAttr;
    u16 wErrId = pcOprDB->Authentication(tLoginInfo, tDBMtInfo, tPlatformAttr);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackToSsn(OLD_2_5_2_ULS_ULC_LOGIN_NACK, wErrId, wAppID, wInstID);

        CCtrlLog(LOG_LVL_ERROR, "[Proc252UlcLoginReq] Authentication fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }

    //终端IP所属运营商
#ifdef _APS_PXY_
	u8 byIspType = g_cCfgFile.GetIspTypeByLoginApsIp(tLoginInfo.GetApsLocalIp());
#else
	u8 byIspType = (u8)emTypeBD;
    //经过nginx转发osp信令后，终端的源地址已经不对了，不能到IP DB中查询运营商名称
    tLoginInfo.SetClientIp(0);  //强制写成0
#endif

//     //2.5.2版本的终端，在登录信令中没有携带终端IP地址信息，并且经过nginx转发osp信令后，终端的源地址已经不对了，不能到IP DB中查询运营商名称
//     //为了解决兼容的问题，只能将本aps的ISP，当成终端的ISP
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

    //服务器地址
    CPlatformSrvAddrInfo tConfSrvInfo;
#ifdef _APS_PXY_
	// pxy根据Mt的运营商返回对应的地址
	g_cCfgFile.GetPlatformSrvAddrInfo((u8)byIspType, tDBMtInfo.GetPlatformDomainMoid(), tConfSrvInfo);
#else
    BOOL32 bRet = g_cSrvListMgr.GetPlatformSrvAddrInfo(tLoginInfo, "", tDBMtInfo.GetPlatformDomainMoid(),  tDBMtInfo.GetPASMoid(), tConfSrvInfo);
	if ( bRet == FALSE )
	{
		//replay Nack: 2.5的终端，遇到登录超时，或是错误的E164号，会重连
		wErrId = APS_ERR_ASSIGN_SRV_ERROR;
        PostNackToSsn(OLD_2_5_2_ULS_ULC_LOGIN_NACK, wErrId, wAppID, wInstID);
		
        CCtrlLog(LOG_LVL_ERROR, "[Proc252UlcLoginReq] GetPlatformSrvAddrInfo fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
	}
#endif

    //结构体转换
    TOld252SrvAddrListInfo tSrvAddrListInfo;
    tSrvAddrListInfo.SetGkAddr(tConfSrvInfo.GetNonH323AddrInfo());
    tSrvAddrListInfo.SetNmaAddr(tConfSrvInfo.GetNmsAddrInfo());
    tSrvAddrListInfo.SetNtsAddr(tConfSrvInfo.GetNdsAddrInfo());
    tSrvAddrListInfo.SetSusAddr(tConfSrvInfo.GetSusAddrInfo());
    
    //打印
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

    //类型
    u8 bySrvType = emSrvTypeUls;
	if ( dwOffset < sizeof(byUlsIpBuf)-1 )
	{
		memcpy(byUlsIpBuf + dwOffset, &bySrvType, sizeof(bySrvType) );
		dwOffset += sizeof(bySrvType);
	}

    //数量
    u8 byIpNum = tUlsIpList.Size();
	if ( dwOffset < sizeof(byUlsIpBuf)-1 )
	{
	    memcpy(byUlsIpBuf + dwOffset, &byIpNum, sizeof(byIpNum) );
		dwOffset += sizeof(byIpNum);
	}

    //每个IP(网络序)
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
	// 认证成功需要上报网管：老终端走Tcp，直接填本地ApsIp的运营商 [11/19/2015 pengguofeng]
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

