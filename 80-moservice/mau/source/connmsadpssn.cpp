/*****************************************************************************
ģ����      : connectadpssnͨ��ģ��
�ļ���      : connectadpssn.cpp
����ļ�    : 
�ļ�ʵ�ֹ���: connectadpssnͨ��ģ��ʵ������
����        : ����
�汾        : V5.0  Copyright(C) 2009-2010 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2015/01/15  5.0         ����        ����
******************************************************************************/

#include "inc.h"
#include "connmsadpssn.h"
#include "srvmoduledef.h"
#include "srvmoduleinfo_mcu.h"
#include "srvmodulenamedef.h"

extern CResMgrSsnApp g_cResMgrSsnApp;

CONNAdpCALLBACK tCallBack;// = { SetCssPfmCallBackFunc };

//��ȡģ�����ܲ����Ļص����� ����ģ�鶨ʱ����
void SetCssPfmCallBackFunc( CAdpServMsg & cAdpServMsg )
{
 	TCssModuleInfo tMpcdInfo;
 	strcpy(tMpcdInfo.m_achModuleVersion, KDV_MCU_PREFIX);
 
 	//���ӵ�CSS��MPC��CMU��������
 	CApp *pcCmuApp = &g_cCMUSsnApp;
 	u32 nMpcCount = 0;
 	for (s32 nIstIndex = 0; nIstIndex < MAX_CMU_NUM; ++nIstIndex)
 	{
 		CCMUSsnInstance* pInstance = static_cast<CCMUSsnInstance*>(pcCmuApp->GetInstance(nIstIndex));
 		if (pInstance != NULL && pInstance->CurState() == CCMUSsnInstance::STATE_NORMAL)
 		{
 			++nMpcCount;
 		}
 	}
 	tMpcdInfo.m_dwConnectedMpcNum = nMpcCount;
 	
 	//���ӵ�CSS��NU������
 	CApp* pcNuApp = &g_cNUSsnApp;
 	u32 nNUCount = 0;
 	for (s32 nIstIdx = 0; nIstIdx < MAX_NU_NUM; ++nIstIdx)
 	{
 		CNUSsnInstance* pInstance = static_cast<CNUSsnInstance*>(pcNuApp->GetInstance(nIstIdx));
 		if (pInstance != NULL && pInstance->CurState() == CCMUSsnInstance::STATE_NORMAL)
 		{
 			++nNUCount;
 		}
 	}
 	tMpcdInfo.m_dwConnectedNuNum = nNUCount;
 
 	//����ģ����
 	tMpcdInfo.m_dwConfTempNum = g_cMauVCApp.GetTemplateCount();
 
 	/*
 		css�������߼�������������ֱ����ͨ��MQ�����շ���Ϣ��
 		css���Ϳ�����Ϊ����Щ������ֱ����
 		m_tMpcdConnectedMpcInfo��m_tMpcdConnectedNuInfo�����ϱ�
 	*/
 	tMpcdInfo.m_byIsConnectBmc = 1;
 
 	//24Сʱ��ͬʱԤԼ����������(����)
 	tMpcdInfo.m_dwOrderConfNum = g_cMauVCApp.GetBookConfNum();
 
 	//��ǰ���ӷ����� MCU��ZK��UPU��
	u32 dwSrvNum = 0;
	MapCmuData mapCmuData;
	g_cMauVCApp.GetAllMpcData(mapCmuData);
	for (MapCmuData::iterator mcuIter = mapCmuData.begin();
	mcuIter != mapCmuData.end(); ++mcuIter)
	{
		const BOOL32 bMcuOnline = ((mcuIter->second).ConnectState() == enumMpcConnectState ? TRUE : FALSE);
		tMpcdInfo.m_tConnState.m_tSrvInfo[dwSrvNum].m_bIsConnect = bMcuOnline;
		const u32 dwMcuIndex = distance(mapCmuData.begin(), mcuIter);
		sprintf(tMpcdInfo.m_tConnState.m_tSrvInfo[dwSrvNum].m_achSrvType, "%s%d", "CMU", dwMcuIndex);
		strcpy(tMpcdInfo.m_tConnState.m_tSrvInfo[dwSrvNum].m_achSrvIp, mcuIter->second.GetCmuIp());
		++dwSrvNum;
	}

 	if (g_cMauVCApp.m_achZkIP[0] != 0)
 	{
 		tMpcdInfo.m_tConnState.m_tSrvInfo[dwSrvNum].m_bIsConnect = g_cResMgrSsnApp.IsConnectedZK();
		strcpy(tMpcdInfo.m_tConnState.m_tSrvInfo[dwSrvNum].m_achSrvType, SRV_NAME_ZK);
 		memcpy(tMpcdInfo.m_tConnState.m_tSrvInfo[dwSrvNum].m_achSrvIp, 
 			g_cMauVCApp.m_achZkIP, sizeof(g_cMauVCApp.m_achZkIP));
 		++dwSrvNum;
 	}
 	upuclient_t* pClient = g_cUpuSsnApp.m_cUpuClient.GetClient();
 	if (pClient != NULL && g_cMauVCApp.m_achUpuIP[0] != 0)
 	{
 		tMpcdInfo.m_tConnState.m_tSrvInfo[dwSrvNum].m_bIsConnect = g_cUpuSsnApp.m_cUpuClient.IsConnected();
		strcpy(tMpcdInfo.m_tConnState.m_tSrvInfo[dwSrvNum].m_achSrvType, SRV_NAME_UPU);
 		memcpy(tMpcdInfo.m_tConnState.m_tSrvInfo[dwSrvNum].m_achSrvIp, 
 			g_cMauVCApp.m_achUpuIP, sizeof(g_cMauVCApp.m_achUpuIP));
 		++dwSrvNum;
 	}
 	tMpcdInfo.m_tConnState.m_dwSrvNum = dwSrvNum;
 	cAdpServMsg.SetMsgBody( (u8*)&tMpcdInfo , sizeof( tMpcdInfo ) );
 
 	LogPrint(LOG_LVL_DETAIL, MID_MAU_PUBLIC, "[SetCssPfmCallBackFunc] Callback function get TMpcdModuleInfo:\n");
 	LogPrint(LOG_LVL_DETAIL, MID_MAU_PUBLIC, "	m_dwConnectedMpcNum: %d m_dwConnectedNuNum: %d"
 		"m_dwConfTempNum: %d\n	m_dwOrderConfNum: %d m_byIsConnectBmc: %d m_wModuleType: %d\n", 
 		tMpcdInfo.m_dwConnectedMpcNum, tMpcdInfo.m_dwConnectedNuNum, tMpcdInfo.m_dwConfTempNum, 
 		tMpcdInfo.m_dwOrderConfNum, tMpcdInfo.m_byIsConnectBmc, tMpcdInfo.m_wModuleType);
 	for (u32 dwIndex = 0; dwIndex < dwSrvNum; ++dwIndex)
 	{
 		LogPrint(LOG_LVL_DETAIL, MID_MAU_PUBLIC, "	m_dwSrvType: %s, m_bIsConnect: %d, m_achSrvIp: %s\n",
 			tMpcdInfo.m_tConnState.m_tSrvInfo[dwIndex].m_achSrvType, 
 			tMpcdInfo.m_tConnState.m_tSrvInfo[dwIndex].m_bIsConnect,
 			tMpcdInfo.m_tConnState.m_tSrvInfo[dwIndex].m_achSrvIp);
 	}
}

void ConnNMSSuccessCallBackFunc()
{
	return;
}

/*=============================================================================
�� �� ���� DaemonInstanceEntry
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ����  CMessage * const pcMsg
           CApp * pcApp
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/15  5.0         ����        ����
=============================================================================*/
void CConNmsAdpInst::DaemonInstanceEntry( CMessage * const pcMsg, CApp * pcApp )
{
    switch( pcMsg->event )
    {
    case OSP_POWERON:
        DaemonProcPowerOn( pcMsg );
        break;

	case OSP_DISCONNECT:
		DaemonProcOspDisconnect( pcMsg );
		break;

	default:
		break;
    }

	return;
}

/*=============================================================================
�� �� ���� DaemonProcPowerOn
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/15  5.0         ����        ����
=============================================================================*/
void CConNmsAdpInst::DaemonProcPowerOn( CMessage * const pcMsg )
{
	tCallBack.fpGetPfm       = SetCssPfmCallBackFunc;
    tCallBack.fpConnNmsAdpOk = ConnNMSSuccessCallBackFunc;

	StartConnectAdp( em_CSS, &tCallBack );
    return;
}

/*=============================================================================
�� �� ���� DaemonProcOspDisconnect
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/15  5.0         ����        ����
=============================================================================*/
void CConNmsAdpInst::DaemonProcOspDisconnect( CMessage * const pcMsg )
{
	QuitConnectAdp(em_CSS);
	return;
}

/*=============================================================================
�� �� ���� DaemonProcTransMsgToAdp
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage* const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/15  5.0         ����        ����
=============================================================================*/
void CConNmsAdpInst::DaemonProcTransMsgToAdp( CMessage* const pcMsg )
{
	CAdpServMsg cAdpSerMsg;
	cAdpSerMsg.SetMsgBody();

	SendToAdp( &cAdpSerMsg );
	return;
}

/*=============================================================================
�� �� ���� InstanceEntry
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/15  5.0         ����        ����
=============================================================================*/
void CConNmsAdpInst::InstanceEntry( CMessage * const pcMsg )
{
	return;
}
