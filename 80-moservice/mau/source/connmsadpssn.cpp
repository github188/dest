/*****************************************************************************
模块名      : connectadpssn通信模块
文件名      : connectadpssn.cpp
相关文件    : 
文件实现功能: connectadpssn通信模块实例定义
作者        : 阮楠楠
版本        : V5.0  Copyright(C) 2009-2010 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/01/15  5.0         阮楠楠        创建
******************************************************************************/

#include "inc.h"
#include "connmsadpssn.h"
#include "srvmoduledef.h"
#include "srvmoduleinfo_mcu.h"
#include "srvmodulenamedef.h"

extern CResMgrSsnApp g_cResMgrSsnApp;

CONNAdpCALLBACK tCallBack;// = { SetCssPfmCallBackFunc };

//获取模块性能参数的回调函数 网关模块定时调用
void SetCssPfmCallBackFunc( CAdpServMsg & cAdpServMsg )
{
 	TCssModuleInfo tMpcdInfo;
 	strcpy(tMpcdInfo.m_achModuleVersion, KDV_MCU_PREFIX);
 
 	//连接到CSS上MPC（CMU）的数量
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
 	
 	//连接到CSS上NU的数量
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
 
 	//会议模板数
 	tMpcdInfo.m_dwConfTempNum = g_cMauVCApp.GetTemplateCount();
 
 	/*
 		css与其他逻辑服务器连接无直链，通过MQ进行收发消息，
 		css存活就可以认为与这些服务器直连；
 		m_tMpcdConnectedMpcInfo和m_tMpcdConnectedNuInfo不用上报
 	*/
 	tMpcdInfo.m_byIsConnectBmc = 1;
 
 	//24小时内同时预约的最大会议数(待定)
 	tMpcdInfo.m_dwOrderConfNum = g_cMauVCApp.GetBookConfNum();
 
 	//当前连接服务器 MCU，ZK，UPU；
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
函 数 名： DaemonInstanceEntry
功    能： 
算法实现： 
全局变量： 
参    数：  CMessage * const pcMsg
           CApp * pcApp
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/15  5.0         阮楠楠        创建
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
函 数 名： DaemonProcPowerOn
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/15  5.0         阮楠楠        创建
=============================================================================*/
void CConNmsAdpInst::DaemonProcPowerOn( CMessage * const pcMsg )
{
	tCallBack.fpGetPfm       = SetCssPfmCallBackFunc;
    tCallBack.fpConnNmsAdpOk = ConnNMSSuccessCallBackFunc;

	StartConnectAdp( em_CSS, &tCallBack );
    return;
}

/*=============================================================================
函 数 名： DaemonProcOspDisconnect
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/15  5.0         阮楠楠        创建
=============================================================================*/
void CConNmsAdpInst::DaemonProcOspDisconnect( CMessage * const pcMsg )
{
	QuitConnectAdp(em_CSS);
	return;
}

/*=============================================================================
函 数 名： DaemonProcTransMsgToAdp
功    能： 
算法实现： 
全局变量： 
参    数： CMessage* const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/15  5.0         阮楠楠        创建
=============================================================================*/
void CConNmsAdpInst::DaemonProcTransMsgToAdp( CMessage* const pcMsg )
{
	CAdpServMsg cAdpSerMsg;
	cAdpSerMsg.SetMsgBody();

	SendToAdp( &cAdpSerMsg );
	return;
}

/*=============================================================================
函 数 名： InstanceEntry
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/15  5.0         阮楠楠        创建
=============================================================================*/
void CConNmsAdpInst::InstanceEntry( CMessage * const pcMsg )
{
	return;
}
