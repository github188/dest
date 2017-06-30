/*****************************************************************************
ģ����      : MAU
�ļ���      : upussn.cpp
����ʱ��    : 2014�� 08�� 26��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
******************************************************************************/
#ifdef WIN32
#pragma warning( disable:4786 )
#endif

#include "inc.h"
#include <stdio.h>
#include "upussn.h"
#include "kdvlog.h"
#include "mauutility.h"
#include "mauvc.h"
#include "evmau.h"
#include "connectsa_api.h"

extern CMauVCApp g_cMauVCApp;

void handle_upu_data(int nEvent, int nResType, int nLen, void * pRes, void * pArg )
{
    printf("[handle_upu_data]Event: %d Result = %d nLen = %d\n", nEvent,nResType, nLen);

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[handle_upu_data]Event: %d Result = %d nLen = %d\n", nEvent, nResType, nLen );

	if ( UPU_EVENT_OP_RESULT == nEvent && UPU_OPERAT_OK == nResType && 0 != nLen && NULL != pRes )
	{
		u8 byIdx = 0;
		for ( byIdx = 0; byIdx < nLen; byIdx++ )
		{
			confinfo * pTmpConfInfo = (confinfo*)pRes;
			confinfo tUpuConfInfo = *(confinfo*)(pTmpConfInfo+byIdx);
			LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[handle_upu_data]Idx.%d ConfE164<%s> UserGUID<%s> PlatGUID<%s> DevGUID<%s>!\n", 
				byIdx, tUpuConfInfo.m_abyConfE164, tUpuConfInfo.m_abyUserDomain, tUpuConfInfo.m_abyPlatformID, tUpuConfInfo.m_abyDevID );
		}
	}

	if ( UPU_EVENT_CONNECTED == nEvent )
	{
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[handle_upu_data]Connected\n" );
		OspPost( MAKEIID(AID_MAU_UPUSSN_APPID, CInstance::DAEMON), MAU_UPU_CONNNECT);
	}
	else if ( UPU_EVENT_DISCONNECT == nEvent )
	{
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[handle_upu_data]DISCONNECT\n" );
		OspPost( MAKEIID(AID_MAU_UPUSSN_APPID, CInstance::DAEMON), MAU_UPU_DISCONNNECT );		
	}
}

/*=============================================================================
�� �� ���� CUpuSsnInstance
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
CUpuSsnInstance::CUpuSsnInstance()
{
}


/*=============================================================================
�� �� ���� ~CUpuSsnInstance
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
CUpuSsnInstance::~CUpuSsnInstance()
{
}

/*=============================================================================
�� �� ���� DaemonInstanceEntry
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
void CUpuSsnInstance::DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp )
{
	switch ( pcMsg->event )
	{
	case OSP_POWERON:
		DaemonProcPowerOn(pcMsg, pcApp);
		break;
	case MAU_UPU_CONNNECT:
		DaemonConnect(pcMsg, pcApp);
		break;
	case MAU_UPU_DISCONNNECT:
		DaemonDisConnect(pcMsg, pcApp);
		break;
	case MAU_UPU_ADD_CONF:
		DaemonAddConf(pcMsg, pcApp);
		break;
	case MAU_UPU_DEL_CONF:
		DaemonDelConf(pcMsg, pcApp);
		break;
	case MAU_UPU_SENDALLINFO:
		DaemonSendAllInfo(pcMsg, pcApp);
		break;
	case MAU_PRINTINFO_CMD:
		Print(pcApp, 0);
		break;
	default:
		break;
	}
}

/*=============================================================================
�� �� ���� DaemonProcPowerOn
��    �ܣ� �ػ�ʵ�������ʼ����Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
void CUpuSsnInstance::DaemonProcPowerOn( const CMessage * pcMsg, CApp* pcApp)
{
	CUpuSsnApp* pcUpuApp = (CUpuSsnApp*)pcApp;
	//״̬��ʼ��
	NextState(STATE_NORMAL);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[CUpuSsnInstance::DaemonProcPowerOn] Rcv Power On Msg!\n");
	if (!pcUpuApp->m_cUpuClient.Init(handle_upu_data))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_UPU, "[CUpuSsnInstance::DaemonProcPowerOn] Init Upu Client failed!\n" );
		return;
	}
	
	pcUpuApp->m_cUpuClient.Start();
}

void CUpuSsnInstance::DaemonConnect(const CMessage * pcMsg, CApp* pcApp)
{
	CUpuSsnApp* pcUpuApp = (CUpuSsnApp*)pcApp;

	pcUpuApp->m_cUpuClient.SetConnectState(TRUE);

	const s8* pchPlatformGUID = g_cMauVCApp.m_achPlatformGuid;
	// ����clientid
	pcUpuApp->m_cUpuClient.SetClientId(pchPlatformGUID);
	
	// �����Ϻ���ջ��飬�����ϱ�
	pcUpuApp->m_cUpuClient.RemoveConfByDevid(pchPlatformGUID);

	OspPost(MAKEIID(AID_MAU_UPUSSN_APPID, CInstance::DAEMON), MAU_UPU_SENDALLINFO);
}

void CUpuSsnInstance::DaemonDisConnect(const CMessage * pcMsg, CApp* pcApp)
{
	CUpuSsnApp* pcUpuApp = (CUpuSsnApp*)pcApp;
	pcUpuApp->m_cUpuClient.SetConnectState(FALSE);
}

void CUpuSsnInstance::DaemonAddConf( CMessage * const pcMsg, CApp* pcApp)
{
	CUpuSsnApp* pcUpuApp = (CUpuSsnApp*)pcApp;
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[CUpuSsnInstance::DaemonAddConf]rev msg MAU_UPU_ADD_CONF!\n");

	const TUpuConfInfo* ptUpuConfInfo = (const TUpuConfInfo*)pcMsg->content;

	if (ptUpuConfInfo != 0)
	{
		TUpuConfInfoSet::iterator iter = pcUpuApp->m_vctConfInfo.find(ptUpuConfInfo->m_achConfE164);

		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[CUpuSsnInstance::DaemonAddConf]Add Conf. E164:%s!\n", ptUpuConfInfo->m_achConfE164);
		// �Ѵ������������
		if (iter != pcUpuApp->m_vctConfInfo.end())
		{
			iter->second.AddConfType(ptUpuConfInfo->GetConfType());
			if (ptUpuConfInfo->IsOngoingConf())
			{
				iter->second.SetMcuIp(ptUpuConfInfo->GetMcuIp());
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[%s]ongoing conf, setMcuIp:%s\n", 
					__FUNCTION__, strofip(ptUpuConfInfo->GetMcuIp(), TRUE));
			}
			pcUpuApp->m_cUpuClient.AddConf(iter->second);
		}
		else
		{
			pcUpuApp->m_vctConfInfo[ptUpuConfInfo->m_achConfE164] = *ptUpuConfInfo;
			pcUpuApp->m_cUpuClient.AddConf(*ptUpuConfInfo);
		}
	}
}

void CUpuSsnInstance::DaemonDelConf( CMessage * const pcMsg, CApp* pcApp)
{
	CUpuSsnApp* pcUpuApp = (CUpuSsnApp*)pcApp;
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[CUpuSsnInstance::DaemonDelConf]rev msg MAU_UPU_DEL_CONF!\n");

	const TUpuConfInfo* ptUpuConfInfo = (const TUpuConfInfo*)pcMsg->content;

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[CUpuSsnInstance::DaemonDelConf]Del Conf. E164:%s, Type:%d!\n", ptUpuConfInfo->GetE164(), ptUpuConfInfo->GetConfType());

	TUpuConfInfoSet::iterator iter = pcUpuApp->m_vctConfInfo.find(ptUpuConfInfo->m_achConfE164);

	if (iter != pcUpuApp->m_vctConfInfo.end())
	{
		iter->second.RemoveConfType(ptUpuConfInfo->GetConfType());
		if (iter->second.GetConfType() == TUpuConfInfo::emNULL)
		{
			pcUpuApp->m_cUpuClient.RemoveConfByE164(iter->second.GetE164());
			pcUpuApp->m_vctConfInfo.erase(iter);
		}
		else
		{
			// �Ǽ�ʱ���飬�����McuIp
			if (!iter->second.IsOngoingConf())
			{
				iter->second.SetMcuIp(0);
			}
			pcUpuApp->m_cUpuClient.AddConf(iter->second);
		}		
	}
}

void CUpuSsnInstance::DaemonSendAllInfo( CMessage * const pcMsg, CApp* pcApp)
{
	CUpuSsnApp* pcUpuApp = (CUpuSsnApp*)pcApp;
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[CUpuSsnInstance::DaemonSendAllInfo]rev msg MAU_UPU_SENDALLINFO!\n");
	TUpuConfInfoSet::iterator iter = pcUpuApp->m_vctConfInfo.begin();
	TUpuConfInfoSet::iterator iterEnd = pcUpuApp->m_vctConfInfo.end();
	while(iter != iterEnd)
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[CUpuSsnInstance::DaemonSendAllInfo]Add Conf. E164:%s!\n", iter->second.m_achConfE164);
		pcUpuApp->m_cUpuClient.AddConf(iter->second);
		iter++;
	}
}

/*=============================================================================
�� �� ���� InstanceEntry
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
void CUpuSsnInstance::InstanceEntry(  CMessage * const pcMsg )
{
	return;
}

void CUpuSsnInstance::Print(CApp* pcApp, u32 /* = 0 */)
{
	CUpuSsnApp* pcUpuApp = (CUpuSsnApp*)pcApp;
 	::OspPrintf(TRUE, FALSE, "\n| %-10s| %-10s| %-10s| %-10s|\n", "E164", "Ongoing", "Schedule", "Template");
	::OspPrintf(TRUE, FALSE, "|-------------------------------------------|\n");
 	TUpuConfInfoSet::iterator it = pcUpuApp->m_vctConfInfo.begin();
 	for (; it != pcUpuApp->m_vctConfInfo.end(); ++it)
 	{
 		::OspPrintf(TRUE, FALSE, "| %-10s| %-10d| %-10d| %-10d|\n", it->second.GetE164(), it->second.IsOngoingConf(), it->second.IsScheduleConf(), it->second.IsTempConf());
 	}
}

void* CUpuClient::UpuInitProc(void * pParam)
 {
	CUpuClient* pData = (CUpuClient*)pParam;
	assert(pData != NULL);
	while (!pData->IsTerminal())
	{
		upuclient_t * ptClient = pData->GetClient();
		if (NULL != ptClient)
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[CUpuSsnInstance::UpuInitProc] Start To Connect %s:%d!\n", g_cMauVCApp.m_achUpuIP, 2100);
			upu_client_connect(ptClient, g_cMauVCApp.m_achUpuIP, 2100, 1);
			upu_client_loop(ptClient);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[CUpuSsnInstance::UpuInitProc] Upu Loop Over!\n");
			OspDelay(3000);
		}
		else
		{
			LogPrint(LOG_LVL_WARNING, MID_MAU_UPU, "[CUpuSsnInstance::UpuInitProc] Start To Connect failed!\n");
		}
	}
	LogPrint(LOG_LVL_WARNING, MID_MAU_UPU, "[CUpuSsnInstance::UpuInitProc] Thread Exit!\n");

	return 0;
}

/*=============================================================================
�� �� ���� ClientInit
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� upu_callback pfCallBack
�� �� ֵ�� BOOL32 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/12/17  5.0         �ܹ��                   ����
=============================================================================*/
BOOL32 CUpuClient::Init( upu_callback pfCallBack )
{
	if (m_pClient)
	{
		return FALSE;
	}

	m_pClient = upu_client_init();
	if (!m_pClient)
	{
		printf("upu_client_init() failed!\n");
		return FALSE;
	}

    upu_client_set_callback(m_pClient, pfCallBack, NULL);
	return TRUE;
}

void CUpuClient::Destory()
{
	if (m_pClient)
	{
		upu_client_fini(m_pClient);
		m_pClient = NULL;
	}
}

BOOL32 CUpuClient::Start()
{
	if (m_pClient == NULL)
	{
		return FALSE;
	}
	
	m_bTerminal = FALSE;
	m_hUpuTask = OspTaskCreate(UpuInitProc, "upuinit", DBOPRTASK_PRIORITY, DBOPRTASK_STACKSIZE, (u32)this, 0);
	
	if (m_hUpuTask == NULL)
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[CUpuSsnInstance::Start] Start Upu Thread Failed!\n");
		return FALSE;
	}
	return TRUE;
}

void CUpuClient::Stop()
{
	m_bTerminal = TRUE;
	if (m_pClient)
	{
		upu_client_stop_loop(m_pClient);
	}

	if (m_hUpuTask != NULL)
	{
		WaitForThreadExit(m_hUpuTask);
		m_hUpuTask = NULL;
	}
}

/*=============================================================================
�� �� ���� GetClient
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� upuclient_t * 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/12/17  5.0         �ܹ��                   ����
=============================================================================*/
upuclient_t * CUpuClient::GetClient(void)
{
	return m_pClient;
}

BOOL32 CUpuClient::SetClientId(const s8* pchClientId)
{
	if (m_pClient)
	{
		set_client_id(m_pClient, pchClientId);
		return TRUE;
	}
	return FALSE;
}

BOOL32 CUpuClient::RemoveConfByDevid(const s8* pchDevid)
{
	if (m_pClient)
	{
		confinfo tUpuConfInfo = {0};
		conf_set_devid(&tUpuConfInfo, pchDevid);

		int nRet = del_conf_by_devid(m_pClient, &tUpuConfInfo);
		if (0 != nRet)
		{
			LogPrint(LOG_LVL_WARNING, MID_MAU_UPU, "[%s]upu client del_conf_by_devid(%s) failed! ret = %d\n", __FUNCTION__, pchDevid, nRet);
			return FALSE;
		}
		else
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[%s]del_conf_by_devid(%s) from upu\n", __FUNCTION__, pchDevid);
			return TRUE;
		}
	}
	else
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_UPU, "[%s]upuclient has not inited\n", __FUNCTION__, pchDevid);
		return FALSE;
	}
}

BOOL32 CUpuClient::RemoveConfByE164(const s8* pchE164)
{
	if (m_pClient)
	{
		confinfo tUpuConfInfo = {0};
		conf_set_confe164(&tUpuConfInfo, pchE164);

		int nRet = del_conf_by_e164(m_pClient, &tUpuConfInfo);
		if (0 != nRet)
		{
			LogPrint(LOG_LVL_WARNING, MID_MAU_UPU, "[CUpuSsnInstance::ProcDelConf]meeting E164<%s> upu client del_conf_by_e164() failed! ret = %d\n", pchE164, nRet);
			return FALSE;
		}
		else
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[CUpuSsnInstance::ProcDelConf]Del Book Conf E164: %s from upu\n", pchE164);
			return TRUE;
		}
	}
	else
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_UPU, "[CUpuSsnInstance::ProcDelConf]meetingID<%s> Get upu client for del conf failed!\n", pchE164);
		return FALSE;
	}
}

BOOL32 CUpuClient::AddConf(const TUpuConfInfo& tConfInfo)
{
	//������Ϣд��UPU
	if (m_pClient)
	{
		confinfo tUpuConfInfo = {0};
		conf_set_confe164(&tUpuConfInfo, tConfInfo.GetE164());
		conf_set_userdomain(&tUpuConfInfo, tConfInfo.m_achDomainMOID);
		//��ȡGUID
		TGUID tGuid = GetSrvGUID();
		conf_set_devid(&tUpuConfInfo, tGuid.GetNOString());

		if (tConfInfo.IsOngoingConf())
		{
			conf_set_ip(&tUpuConfInfo, strofip(tConfInfo.GetMcuIp(),TRUE));
			conf_set_conftype(&tUpuConfInfo, INSTANT);
		}
		else if(tConfInfo.IsScheduleConf())
		{
			conf_set_conftype(&tUpuConfInfo, RESERVE);
		}
		else if (tConfInfo.IsTempConf())
		{
			conf_set_conftype(&tUpuConfInfo, TEMPLET);
		}

		// ��û���
		conf_set_platformid(&tUpuConfInfo, tConfInfo.m_achPaltformMOID);

		int nRet = add_conf(m_pClient, &tUpuConfInfo);
		if (0 != nRet)
		{
			LogPrint(LOG_LVL_WARNING, MID_MAU_UPU, "[%s]Add Conf<E164.%s> failed! ret = %d\n", __FUNCTION__, tConfInfo.m_achConfE164, nRet);
			return FALSE;
		}
		else
		{
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_UPU, "[%s]Add Conf<E164.%s> to upu\n", __FUNCTION__, tConfInfo.m_achConfE164);
			return TRUE;
		}
	}
	else
	{
		LogPrint(LOG_LVL_WARNING, MID_MAU_UPU, "[%s]Add Conf<E164.%s> Get upu client failed!\n", __FUNCTION__, tConfInfo.m_achConfE164);
		return FALSE;
	}
}

void CUpuClient::SetConnectState(BOOL32 bState)
{
	m_bConnected = bState;
}

BOOL32 CUpuClient::IsConnected()
{
	return m_bConnected;
}

BOOL32 CUpuClient::IsTerminal()
{
	return m_bTerminal;
}

void CUpuSsnData::Quit()
{
	m_cUpuClient.Stop();
}

BOOL32 UpuAddConf(const s8* pchConfE164,
	const TUpuConfInfo::EmConfType emConfType,
	const s8* pchDomainMOID,
	const s8* pchPlatformDomainMOID,
	const s8* pchMcuIp /*= NULL*/)
{
	if ((pchConfE164 == NULL || strlen(pchConfE164) == 0)
		|| (pchDomainMOID == NULL || strlen(pchDomainMOID) == 0)
		|| (pchPlatformDomainMOID == NULL || strlen(pchPlatformDomainMOID) == 0))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_UPU, "[%s]param invalid\n", __FUNCTION__);
		return FALSE;
	}

	TUpuConfInfo tUpuConfInfo;
	tUpuConfInfo.AddConfType(emConfType);
	tUpuConfInfo.SetE164(pchConfE164);
	tUpuConfInfo.SetDomainMoid(pchDomainMOID);
	tUpuConfInfo.SetPlatformMoid(pchPlatformDomainMOID);
	if (pchMcuIp != NULL 
		&& strlen(pchMcuIp) != 0
		&& emConfType == TUpuConfInfo::emOnGoingConf)
	{
		tUpuConfInfo.SetMcuIp(inet_addr(pchMcuIp));
	}
	OspPost(MAKEIID(AID_MAU_UPUSSN_APPID, CInstance::DAEMON), MAU_UPU_ADD_CONF, &tUpuConfInfo, sizeof(tUpuConfInfo));
	return TRUE;
}

BOOL32 UpuDelConf(const s8* pchConfE164, const TUpuConfInfo::EmConfType emConfType)
{
	TUpuConfInfo tConfInfo;
	tConfInfo.SetE164(pchConfE164);
	tConfInfo.AddConfType(emConfType);
	OspPost(MAKEIID(AID_MAU_UPUSSN_APPID, CInstance::DAEMON),
		MAU_UPU_DEL_CONF, &tConfInfo, sizeof(tConfInfo));
	return TRUE;
}

