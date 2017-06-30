
#include "consassn.h"
#include "debugprint.h"
#include "bossutils.h"
#include "eventself.h"
#include "bossversion.h"
#include "srvlistmgr.h"
#include "centerctrl.h"
#include "srvmoduledef.h"
#include "srvmoduleinfo_nu.h"
#include "connectadp_api.h"
#include "config.h"

CConSaSsnApp g_cConSaSsnApp;

/*
 *	此回调 网管组件会每30秒调一次
 */

void NmsGetPfmCb(CAdpServMsg& cServMsg)
{
	ConSaSsnLog(LOG_LVL_KEYSTATUS, "%s:%d in tick.%u\n", __FUNCTION__, __LINE__, OspTickGet());

    TApsModuleInfo tModuleInfo;

    //tModuleInfo.SetEvType();
    strncpy(tModuleInfo.m_achModuleVersion, MO_BOSS_VER_PREFIX, sizeof(tModuleInfo.m_achModuleVersion)-1);
    tModuleInfo.m_achModuleVersion[sizeof(tModuleInfo.m_achModuleVersion)-1] = '\0';

    //数据库连接状态
    std::vector<CDBState> vcDBState = g_cCenterCtrlApp.GetDBPoolConnectState();
	ConSaSsnLog(LOG_LVL_KEYSTATUS, "%s:%d DB size.%u\n", __FUNCTION__, __LINE__, vcDBState.size());

    TSrvInfo tDBInfo;
	u8 bySrvInfoNum = 0;
	for (u8 byLoop = 0; byLoop < (u8)vcDBState.size(); byLoop++)
	{
		sprintf(tDBInfo.m_achSrvType, "Mysql%d", byLoop+1); 
        strncpy(tDBInfo.m_achSrvIp, vcDBState[byLoop].GetDBIP(), sizeof(tDBInfo.m_achSrvIp)-1 );
        tDBInfo.m_bIsConnect = vcDBState[byLoop].GetConnectState();
		ConSaSsnLog(LOG_LVL_KEYSTATUS, "%s:%d DB Ip:%s ConnectState.%u\n", __FUNCTION__, __LINE__, vcDBState[byLoop].GetDBIP(), vcDBState[byLoop].GetConnectState());
		tModuleInfo.m_tConnState.m_tSrvInfo[bySrvInfoNum++] = tDBInfo;
	}
	// 规格明确不需要报BMC认证信息 [11/14/2015 pengguofeng]
/*
    //BMC连接状态
    TSrvInfo tBmcInfo;
    tBmcInfo.m_dwSrvType = emServTypeBMC;
	// 由于不和BMC直接关联，故不报IP
    //tBmcInfo.m_achSrvIp;
    tBmcInfo.m_bIsConnect = GetConnectBmcState();
    tModuleInfo.m_tConnState.m_tSrvInfo[bySrvInfoNum++] = tBmcInfo;
*/
    //Rmq连接状态
    TSrvInfo tMqInfo;
	sprintf(tMqInfo.m_achSrvType, "MqServer"); 
    strncpy(tMqInfo.m_achSrvIp, g_cCfgFile.GetMqConInfo().GetIP(), sizeof(tMqInfo.m_achSrvIp)-1 );
    tMqInfo.m_bIsConnect = g_cCenterCtrlApp.GetConnectRmqState();
	ConSaSsnLog(LOG_LVL_KEYSTATUS, "%s:%d Mq Ip:%s ConnectState.%u\n", __FUNCTION__, __LINE__, g_cCfgFile.GetMqConInfo().GetIP(), g_cCenterCtrlApp.GetConnectRmqState());
    tModuleInfo.m_tConnState.m_tSrvInfo[bySrvInfoNum++] = tMqInfo;
   
    //连接的服务器数量
    tModuleInfo.m_tConnState.m_dwSrvNum = bySrvInfoNum;
	ConSaSsnLog(LOG_LVL_KEYSTATUS, "%s:%d Mq bySrvInfoNum.%u\n", __FUNCTION__, __LINE__, bySrvInfoNum);

    //
    cServMsg.SetMsgBody( (u8*)&tModuleInfo , sizeof( tModuleInfo ) );
    return ;
}

// 第一次连网管时全报 [11/19/2015 pengguofeng]
void OnNmsConnect()
{
	g_cCenterCtrlApp.ReportAllMtInfo2Nms();
}

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


CConSaSsnInst::CConSaSsnInst()
{
	ClearInst();
}

CConSaSsnInst::~CConSaSsnInst()
{

}

void CConSaSsnInst::ClearInst()
{

}

void CConSaSsnInst::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{
	if (NULL == pcMsg || NULL == pcApp)
    {
        ConSaSsnLog(LOG_LVL_ERROR, "[DaemonInstanceEntry] NULL == pcMsg || NULL == pcApp!\n");
        return;
    }

	switch (pcMsg->event)
	{
	case OSP_POWERON:
		DaemonProcPowerOn(pcMsg);
		break;

        //检查服务器状态
        //理论上应该放到CCenterCtrlApp中,但是当很多线路都不通时，
        //此函数会造成很长时间的堵塞, 放在CCenterCtrlApp中,会导整个程序性能的严重下降
    case CHECK_HEALTH_TIMER:
        CheckSrvStateTimeOut(pcMsg);  
        break;

	default:
		break;
	}
    
	return;

}

void CConSaSsnInst::InstanceEntry(CMessage *const pMsg)
{
	return;
}


void CConSaSsnInst::DaemonProcPowerOn(CMessage *const pcMsg)
{
	if (NULL == pcMsg)
    {
        ConSaSsnLog(LOG_LVL_ERROR, "[DaemonProcPowerOn] The received msg's pointer is NULL!\n");
        return;
    }

    //服务器健康状态监测
    SetTimer(CHECK_HEALTH_TIMER, 10*1000);

    //连接网管
    CONNAdpCALLBACK tNmstCallBack;
    tNmstCallBack.fpGetPfm = NmsGetPfmCb;
    tNmstCallBack.fpConnNmsAdpOk = OnNmsConnect;
    if( StartConnectAdp(em_APS, &tNmstCallBack) )
	{
		ConSaSsnLog(LOG_LVL_KEYSTATUS, "%s:%d StartConnectAdp succeed\n", __FUNCTION__, __LINE__);
	}
	else
	{
		ConSaSsnLog(LOG_LVL_KEYSTATUS, "%s:%d StartConnectAdp failed\n", __FUNCTION__, __LINE__);
	}
}

/*
 *	定期连服务器
 */
void CConSaSsnInst::CheckSrvStateTimeOut(CMessage *const pcMsg)
{
	CPrintFunInOut cPrintFun(__FUNCTION__);

    KillTimer(CHECK_HEALTH_TIMER);

    g_cSrvListMgr.CheckStateTimer();

    SetTimer(CHECK_HEALTH_TIMER, 30*1000);
}




