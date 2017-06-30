
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

	set<string> setPlatformDomainMOID;
	wErrId = pcOprDB->GetAllPlatformDomainMOID(tPlatformDomain.GetServiceDomainMOID(), setPlatformDomainMOID);
    if (SUCCESS_DBENGINE != wErrId)
    {		
        PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

        CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetAllPlatformDomain fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
        return wErrId;
    }
	g_cSrvListMgr.SetAllPlatformDomainMOID(setPlatformDomainMOID);

	//��������ƽ̨�����Ϣ

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
    }
    else    // ����ģʽ-----�����ݿ��ȡ��������ַ��Ϣ
    {
        //�˷������µ� NDS(NTS) �豸
        std::set<CBriefSrvInfo> setBriefSrvInfo;
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_NDS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo NTS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToNdsList(setBriefSrvInfo);

        //�˷������µ� NMS �豸
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_NMS_C, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo NMS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToNmsList(setBriefSrvInfo);

		//zjj20160908 ������ʷԭ��,ԭ��nms������nms_6000���͵�nms(devicetype��312),
		//����sp4�汾��ʼ�Ѿ���������nms_6000������
		//Ϊ�˼����ϰ汾,����nms_6000������Ҳ��һ��
		setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_NMS_6000, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo NMS_6000 fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToNmsList(setBriefSrvInfo);
		

        //�˷������µ� NU �豸
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_PAS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo NU fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToPasList(setBriefSrvInfo);

        //�˷������µ� BGS �豸
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_BGS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo BGS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToBgsList(setBriefSrvInfo);

        //�˷������µ� SUS �豸
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_SUS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo SUS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToSusList(setBriefSrvInfo);

        //�˷������µ� SNS �豸
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_SNS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo SNS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToSnsList(setBriefSrvInfo);

        //�˷������µ� XNU �豸
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_XNS, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo XNS fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToXnsList(setBriefSrvInfo);


        //�˷������µ� CMC �豸
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_CMC, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo CMC fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToCmcList(setBriefSrvInfo);

        //�˷������µ� NTP �豸
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_NTP, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo NTP fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToNtpList(setBriefSrvInfo);

        //�˷������µ� VRS �豸 (VRS�ַ��ͺ�VRS2000��VRS4000)
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

        //�˷������µ� DCS �豸
        setBriefSrvInfo.clear();
        wErrId = pcOprDB->GetBriefSrvInfo(tPlatformDomain.GetServiceDomainMOID(), DEVICE_GLOBAL_TYPE_DCS2000, setBriefSrvInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {		
            PostNackToSsn(QUERY_SERVERDEV_LIST_NACk, wErrId, wAppID, wInstID);

            CCtrlLog(LOG_LVL_ERROR, "[ProcQuerySrvDevListReq] GetSrvDevInfo DCS2000 fail,ErrId = %d(%s) !\n", wErrId, GetApsErrorStr(wErrId));
            return wErrId;
        }
        g_cSrvListMgr.InsertToDcsList(setBriefSrvInfo);

		//�˷������µ�RestApi�豸
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

    //�˷������µ� APS �豸
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

	//���������ã���ʱҲ��Ҫ�ϱ�
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

#ifndef _APS_PXY_
    //�ж��ն��Ƿ����ε�¼
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

    //�ն�IP������Ӫ��
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
	//zjj20160822 ����dmzģʽ������ĳ����Ӫ����
	//������Ҫ�ж��ն�ע�����Ӫ���Ƿ���Ҫdmzģʽ���������Ҫ
	//��������ͨ��ģʽ
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
				CCtrlLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq] loaapsispname.%s ip.%08x is not local��ip\n",
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

    //��������ַ
	// ��ʹĳ����ַ�Ҳ�����ApsҲ����Ҫ����Login_Nack [12/14/2015 pengguofeng]
    CPlatformSrvAddrInfo tPlatformSrvInfo;
#ifdef _APS_PXY_
	// pxy����Mt����Ӫ�̷��ض�Ӧ�ĵ�ַ
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
    BOOL32 bRet = g_cSrvListMgr.GetPlatformSrvAddrInfo(tLoginInfo, "", tDBMtInfo.GetPlatformDomainMoid(),  tDBMtInfo.GetPASMoid(), tConfSrvInfo);
	if ( bRet == FALSE )
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
    BOOL32 bRet = g_cSrvListMgr.GetPlatformSrvAddrInfo(tLoginInfo, "", tDBMtInfo.GetPlatformDomainMoid(),  tDBMtInfo.GetPASMoid(), tConfSrvInfo);
	if ( bRet == FALSE )
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

