

#include "old_ulcssn.h"
#include "debugprint.h"
#include "bossconst.h"
#include "kdvsys.h"
#include "eventself.h"
#include "old_26_eventulculs.h"
#include "old_26_structulc.h"
#include "old_252_eventulculs.h"
#include "old_252_ulcstruct.h"
#include "bossutils.h"
#include "structnack.h"
#include "structextend.h"
#include "errorcode.h"
#include "zlib.h"

#include "config.h"



using namespace BossUtils;


COldUlcSsnApp g_cOldUlcSsnApp;

CUlcSsnConfig::CUlcSsnConfig()
{
    m_wLastInstID = 0;
}

CUlcSsnConfig::~CUlcSsnConfig()
{
}

BOOL32 CUlcSsnConfig::GetIdleInstID(OUT u16 &wIdleInst)
{
	wIdleInst = 0;
	
    //1. 首先直接由上次记录开始查找
    u16 wInstID = 0;
    CApp *pApp = &g_cOldUlcSsnApp;
    for (wInstID = m_wLastInstID + 1; wInstID <= MAXINSTNUM_ULCSSN; ++wInstID)
    {
        CUlcSsnInst *pcInst = (CUlcSsnInst *)pApp->GetInstance(wInstID);
        if (NULL != pcInst && CUlcSsnInst::STATE_IDLE == pcInst->CurState())
        {
            wIdleInst = wInstID;
            m_wLastInstID = wInstID;
            return TRUE;
        }
    }
	
    //2. 查找其他实例
    for (wInstID = 1; wInstID <= m_wLastInstID; ++wInstID)
    {
        CUlcSsnInst *pcInst = (CUlcSsnInst *)pApp->GetInstance(wInstID);
        if (NULL != pcInst && CUlcSsnInst::STATE_IDLE == pcInst->CurState())
        {
            wIdleInst = wInstID;
            m_wLastInstID = wInstID;
            return TRUE;
        }
    }
	
    return FALSE;
}



/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

CUlcSsnInst::CUlcSsnInst()
{
	/*lint -save -e1566*/
	ClearInst();
	/*lint -restore*/
}

CUlcSsnInst::~CUlcSsnInst()
{

}

void CUlcSsnInst::ClearInst(void)
{
	m_dwUlcNode = INVALID_NODE;
    m_dwUlcIID   = 0;
    m_dwUlcIP    = 0;
    m_tLoginTime = 0 ;//登陆时间

	return;

}

BOOL32 CUlcSsnInst::PostMsgToUlc(u16 wEvent, u8 *const pbyMsg, u16 wLen)
{
    if (OSP_OK != post(m_dwUlcIID, wEvent, pbyMsg, wLen, m_dwUlcNode))
    {
        ClientSsnLog(LOG_LVL_ERROR, "[PostMsgToUlc] Failed - Inst.%u CurState(%u) message %u(%s) to ulc(IP.%s Node.%u ).\n",
			GetInsID(), CurState(), wEvent, ::OspEventDesc(wEvent), GetIpString(m_dwUlcIP).c_str(), m_dwUlcNode);
        return FALSE;
    }

    ClientSsnLog(LOG_LVL_KEYSTATUS, "[PostMsgToUlc] Inst.%u CurState(%u) post message %u(%s) to ulc(IP.%s Node.%u).\n",
		GetInsID(), CurState(), wEvent, ::OspEventDesc(wEvent), GetIpString(m_dwUlcIP).c_str(), m_dwUlcNode);
    return TRUE;
}

BOOL32 CUlcSsnInst::PostNackToUlc(u32 dwErrID)
{
    CApsBaseNack tNack;
    tNack.SetErrorId(dwErrID);
    Json::Value jsonVale = tNack.StructToJson();
    std::string strJson = jsonVale.toStyledString();

    BOOL32 bRet = PostMsgToUlc(OLD_26_ULS_ULC_LOGIN_NACK, (u8*)strJson.c_str(), strJson.length()+1);
    return bRet;
}


BOOL32 CUlcSsnInst::PostMsgToCtrl(u16 wEvent, u8 *const pbyMsg, u16 wLen)
{
    if (OSP_OK != post(MAKEIID(AID_APS_CENTERCTRL, CInstance::DAEMON), wEvent, pbyMsg, wLen))
    {
        ClientSsnLog(LOG_LVL_ERROR, "[PostMsgToCtrl] Failed - Inst.%u CurState(%u) message %u(%s).\n",
			GetInsID(), CurState(), wEvent, ::OspEventDesc(wEvent));
        return FALSE;
    }
	
    ClientSsnLog(LOG_LVL_DETAIL, "[PostMsgToCtrl] Inst.%u CurState(%u) post message %u(%s) to CCtrl.\n",
		GetInsID(), CurState(), wEvent, ::OspEventDesc(wEvent));

    return TRUE;
}

void CUlcSsnInst::DaemonInstanceDump(u32 param /*=0*/)
{
    u16 wEventId = (u16)param;
	
    switch (wEventId)
    {
    case APS_DUMP_ULC_STATE_DUMP:
        DumpAllInstState();
        break;

    default:
        ClientSsnLog(LOG_LVL_WARNING, "[DaemonInstanceDump] received wrong message %u(%s) !\n",
			wEventId, ::OspEventDesc(wEventId));
        break;
    }
	
    return;
}

void CUlcSsnInst::DumpAllInstState(void)
{
	if (CInstance::DAEMON != GetInsID())
    {
        return;
    }
	
    CApp *pcApp = (CApp *)&g_cOldUlcSsnApp;
	
    OspPrintf(TRUE, FALSE, "Node State For UlcSsn(Num.%u)..........\n", MAXINSTNUM_ULCSSN);
    OspPrintf(TRUE, FALSE, "InsID  InsState  ULCIP          IId     Node     Time\n");
    OspPrintf(TRUE, FALSE, "------ --------- ---------------  -------- -------- -----------------\n");
	
    for (u16 wInstId = 1; wInstId < (MAXINSTNUM_ULCSSN + 1); wInstId++)
    {
        CUlcSsnInst *pcSsnInst = (CUlcSsnInst *)pcApp->GetInstance(wInstId);
        if (NULL != pcSsnInst && STATE_IDLE != pcSsnInst->CurState())
        {
//             TKDMTime tKDMTime = pcSsnInst->GetLoginTime();
// 			
//             OspPrintf(TRUE, FALSE, "%.6u %.9s %.15d %.8u %.8u %.4d-%.2d-%.2d %.2d:%.2d:%.2d\n",
// 				wInstId, pcSsnInst->GetInsStateStr(), pcSsnInst->GetUlcIPString().c_str(),
// 				pcSsnInst->GetUlcIID(), pcSsnInst->GetUlcNode(),
// 				tKDMTime.GetYear(), tKDMTime.GetMonth(), tKDMTime.GetDay(),
// 				tKDMTime.GetHour(), tKDMTime.GetMinute(), tKDMTime.GetSecond());
			time_t tLoginTime = pcSsnInst->GetLoginTime();
			OspPrintf(TRUE, FALSE, "%.6u %.9s %.15d %.8u %.8u time: %s\n",
				wInstId, pcSsnInst->GetInsStateStr(), pcSsnInst->GetUlcIPString().c_str(),
				pcSsnInst->GetUlcIID(), pcSsnInst->GetUlcNode(),
				TimeToString(tLoginTime).c_str());
        }
    }
	
    return;
}

const s8 *CUlcSsnInst::GetInsStateStr()
{
    switch (CurState())
    {
    case STATE_IDLE:
        return "Idle.....";
    case STATE_NORMAL:
        return "Normal...";
    default:
        return "Unknown..";
    }
    return "Unknown..";
}


std::string CUlcSsnInst::GetUlcIPString() const
{
    return GetIpString(m_dwUlcIP);
}

void CUlcSsnInst::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{
	if (NULL == pcMsg || NULL == pcApp)
	{
		ClientSsnLog(LOG_LVL_ERROR, "[DaemonInstanceEntry] The received msg's pointer is NULL!\n");
		return;
	}
	ClientSsnLog(LOG_LVL_KEYSTATUS, "[DaemonInstanceEntry]Receive event  (%u)%s \n", pcMsg->event, ::OspEventDesc(pcMsg->event));

	switch (pcMsg->event)
	{
	case OSP_POWERON:   //上电处理
		DaemonProcPowerOn(pcMsg);
		break;

	case OLD_26_ULC_ULS_LOGIN_REQ:
		DaemonProcUlcLoginReq(pcMsg, pcApp);
		break;
    case OLD_2_5_2_ULC_ULS_LOGIN_REQ:
        Old252_DaemonProcUlcLoginReq(pcMsg, pcApp);
        break;
	default:
		ClientSsnLog(LOG_LVL_ERROR, "[DaemonInstanceEntry] received wrong message %u(%s)!\n",
                     pcMsg->event, ::OspEventDesc(pcMsg->event));
		break;
	}

	return;
}

void CUlcSsnInst::InstanceEntry(CMessage *const pcMsg)
{
	if (NULL == pcMsg)
	{
		ClientSsnLog(LOG_LVL_ERROR, "[InstanceEntry] The received msg's pointer is NULL!\n");
		return;
	}
	ClientSsnLog(LOG_LVL_KEYSTATUS, "[InstanceEntry]Receive event (%u)%s \n", pcMsg->event, ::OspEventDesc(pcMsg->event));

	switch (pcMsg->event)
	{
    case OSP_DISCONNECT:
        ProcDisconnect(pcMsg);
        break;

/////////////////////////////////////////////////////
	case OLD_26_ULS_ULC_LOGIN_ACK:
		ProcUlcLoginAck(pcMsg);
		break;

	case OLD_26_ULS_ULC_LOGIN_NACK:
        ProcUlcLoginNack(pcMsg);
		break;

///////////////////////////////////////////////////////
    case OLD_2_5_2_ULS_ULC_LOGIN_ACK:
        Proc252UlcLoginAck(pcMsg);
        break;

    case OLD_2_5_2_ULS_ULC_LOGIN_NACK:
        Proc252UlcLoginNack(pcMsg);
        break;

	default:
		break;
	}

	return;
}

void CUlcSsnInst::DaemonProcPowerOn(CMessage *const pcMsg)
{
	if (NULL == pcMsg)
    {
        ClientSsnLog(LOG_LVL_ERROR, "[DaemonProcPowerOn] The received msg's pointer is NULL!\n");
        return;
    }
	
    // 创建连接用的监听端口
    while (INVALID_SOCKET == OspCreateTcpNode(0, g_cCfgFile.GetOspListenPort()))
    {
        ClientSsnLog(LOG_LVL_WARNING, "OspCreateTcpNode(ListenPort.%u) Err, application program will try late...\n", g_cCfgFile.GetOspListenPort());	
        OspTaskDelay(3 * 1000);       
    }
    ClientSsnLog(LOG_LVL_KEYSTATUS, "OspCreateTcpNode(ListenPort.%u) succeed!\n", g_cCfgFile.GetOspListenPort());
	
    return;
}

void CUlcSsnInst::ProcDisconnect(CMessage *const pcMsg)
{
	if (NULL == pcMsg)
    {
        ClientSsnLog(LOG_LVL_ERROR, "[ProcDisconnect] Inst.%u received msg's pointer is NULL!\n", GetInsID());
        return;
    }
    ClientSsnLog(LOG_LVL_WARNING, "[ProcDisconnect] Inst.%u Node.%u Ip.%s disconnect!\n",
		GetInsID(), m_dwUlcNode, GetIpString(m_dwUlcIP).c_str());
	
    ClearInst();
    NextState(STATE_IDLE);
	
    return;
}

void CUlcSsnInst::DaemonProcUlcLoginReq(CMessage *const pcMsg,  CApp* pcApp)
{
	if (NULL == pcMsg || NULL == pcApp )
	{
		ClientSsnLog(LOG_LVL_ERROR, "[DaemonProcUlcLoginReq] The receive msg's pointer is NULL!\n");
		return;
	}


	//查找空闲实例
    u16 wIdleInstId = 0;
    BOOL32 bFind = g_cOldUlcSsnApp.GetIdleInstID(wIdleInstId);
    if (FALSE == bFind)
    {
        ClientSsnLog(LOG_LVL_ERROR, "[DaemonProcUlcLoginReq]SrvNode.%u can't find idle InstID!\n", pcMsg->srcnode);

        OspDisconnectTcpNode(pcMsg->srcnode);		
        return;
    }

	//直接调用普通实例的注册函数
    CUlcSsnInst *pcInst = (CUlcSsnInst *)pcApp ->GetInstance(wIdleInstId);
    if (NULL == pcInst)
    {
        OspDisconnectTcpNode(pcMsg->srcnode);		
        return;
    }

    pcInst->ProcUlcLoginReq(pcMsg);	
    return;
}

/*lint -save -e438*/
void CUlcSsnInst::ProcUlcLoginReq(CMessage *const pcMsg)
{
	if (NULL == pcMsg)
    {
        ClientSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] The receive message's point is null !\n");
        return;
    }

    if (NULL == pcMsg->content)
    {
        ClientSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] NULL == pcMsg->content!\n");
        OspDisconnectTcpNode(pcMsg->srcnode);   
		return;
    }

	//校验当前状态
    u32 dwCurstate = CurState();
    if (STATE_IDLE != dwCurstate)
    {
        ClientSsnLog(LOG_LVL_WARNING, "[ProcUlcLoginReq] The curstate(%u) is not idle!\n", CurState());

        OspDisconnectTcpNode(pcMsg->srcnode);		
	    return;
    }

	m_dwUlcNode = pcMsg->srcnode;
	m_dwUlcIID = pcMsg->srcid;;
	m_tLoginTime = GetSysTime();

	//解压
	unsigned long wDestLen = pcMsg->length * 20;
	u8* pDestBuf = new u8[wDestLen];
	memset(pDestBuf, 0, wDestLen);
	
	s32 wErr = uncompress(pDestBuf, &wDestLen, pcMsg->content, pcMsg->length);
	if (Z_OK != wErr)
	{
		ClientSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] uncompress is failed! %d \n", wErr);

		delete[] pDestBuf;
		pDestBuf = NULL;
		return;
	}
	
	//json转化   
	CLoginInfo tLoginInfo;
	BOOL32 bRet = tLoginInfo.JsonToStruct((const s8*)pDestBuf);
	if (!bRet)
	{
		ClientSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] tLoginInfo.JsonToStruct() fail!\n");
        ClientSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] Json string:%s\n", (const s8*)pDestBuf);
        PostNackToUlc(APS_ERR_LOGIN_PARAM_ERROR);
        
        delete[] pDestBuf;
        pDestBuf = NULL;
		return;
	}    

	//断链检测
    OspNodeDiscCBRegQ(m_dwUlcNode, GetAppID(), GetInsID());
    OspSetHBParam(m_dwUlcNode, MOBOSS_HB_TIME, MOBOSS_HB_NUM);

	//消息发送到主控
    CLoginExtend tExtend;
    tExtend.SetDetailBody(tLoginInfo);
	tExtend.SetSessionID(GetAppID(), GetInsID());
	PostMsgToCtrl(pcMsg->event, (u8 *)&tExtend, sizeof(tExtend));

	//MT通过Haproxy连接uls，所以增加终端ip加以区别
	m_dwUlcIP = tLoginInfo.GetClientIp();
	ClientSsnLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginReq]Mt ip(%s) info:\n %s \n", BossUtils::GetIpString(m_dwUlcIP).c_str(), (const s8*)pDestBuf);

	delete[] pDestBuf;
    pDestBuf = NULL;
	return;
}

void CUlcSsnInst::ProcUlcLoginAck(CMessage *const pcMsg)
{
	if (NULL == pcMsg)
	{
		ClientSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginAck] The receive msg's pointer is NULL!\n");
		return;
	}

	if (NULL == pcMsg->content || sizeof(COld26UlcFullInfo) != pcMsg->length)
	{
		ClientSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginAck]NULL == pcMsg->content || sizeof(COld26UlcFullInfo) != pcMsg->length!\n");
		return;
	}

	NextState(STATE_NORMAL);

	COld26UlcFullInfo tFullInfo = *(COld26UlcFullInfo*)pcMsg->content;
	Json::Value jsonValue = tFullInfo.StructToJson();
    std::string strJson = jsonValue.toStyledString();

	//压缩
	unsigned long wDestLen = compressBound(strJson.length() + 1);
	u8* pDestBuff = new u8[wDestLen];
	memset(pDestBuff, 0, wDestLen);

	if (Z_OK != compress(pDestBuff, &wDestLen, (u8*)strJson.c_str(), strJson.length() + 1))
	{
		ClientSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginAck] Source buffer Compress failed!\n");

		delete[] pDestBuff;
		pDestBuff = NULL;
		return;
	}

	PostMsgToUlc(pcMsg->event, pDestBuff, (u16)wDestLen);
    
    ClientSsnLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginAck] ulc:ip(%s) E164(%s) login succ! Ulc Full info:\n %s \n", 
		BossUtils::GetIpString(m_dwUlcIP).c_str(), tFullInfo.GetMtInfo().GetE164NO(), strJson.c_str());

	delete[] pDestBuff;
	pDestBuff = NULL;
    return;
}

void CUlcSsnInst::ProcUlcLoginNack(CMessage *const pcMsg)
{
	if (NULL == pcMsg)
	{
		ClientSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginNack] The receive msg's pointer is NULL!\n");
		return;
	}

	CApsBaseNack tNack = *(CApsBaseNack*)pcMsg->content;
	PostNackToUlc(tNack.GetErrorId());

    ClientSsnLog(LOG_LVL_WARNING, "[ProcUlcLoginNack] ulc:%s login fail!\n", GetUlcIPString().c_str());
    return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                  2.5.2版本终端                                                */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CUlcSsnInst::Old252_DaemonProcUlcLoginReq(CMessage *const pcMsg, CApp* pcApp)
{
    if (NULL == pcMsg || NULL == pcApp )
    {
        ClientSsnLog(LOG_LVL_ERROR, "[Old252_DaemonProcUlcLoginReq] The receive msg's pointer is NULL!\n");
        return;
    }

    //查找空闲实例
    u16 wIdleInstId = 0;
    BOOL32 bFind = g_cOldUlcSsnApp.GetIdleInstID(wIdleInstId);
    if (FALSE == bFind)
    {
        PostMsgToUlc(OLD_2_5_2_ULS_ULC_LOGIN_NACK, NULL, 0);

        ClientSsnLog(LOG_LVL_ERROR, "[Old252_DaemonProcUlcLoginReq]SrvNode.%u can't find idle InstID!\n", pcMsg->srcnode);
        OspDisconnectTcpNode(pcMsg->srcnode);

        return;
    }

    //直接调用普通实例的注册函数
    CUlcSsnInst *pcInst = (CUlcSsnInst *)pcApp ->GetInstance(wIdleInstId);
    if (NULL == pcInst)
    {
        return;
    }
    pcInst->Old252_ProcUlcLoginReq(pcMsg);

    return;
}

void CUlcSsnInst::Old252_ProcUlcLoginReq(CMessage *const pcMsg)
{
    if (NULL == pcMsg || NULL == pcMsg->content)
    {
        ClientSsnLog(LOG_LVL_ERROR, "[Old252_ProcUlcLoginReq] The receive message's point is null !\n");
        return;
    }

    //校验当前状态
    u32 dwCurstate = CurState();
    if (STATE_IDLE != dwCurstate)
    {
        ClientSsnLog(LOG_LVL_WARNING, "[Old252_ProcUlcLoginReq] The curstate(%u) is not idle!\n", CurState());
        return;
    }

    m_dwUlcNode = pcMsg->srcnode;
    m_dwUlcIID = pcMsg->srcid;
    m_dwUlcIP = ntohl(OspNodeIpGet(m_dwUlcNode));
    m_tLoginTime = GetSysTime();

    //断链检测
    OspNodeDiscCBRegQ(m_dwUlcNode, GetAppID(), GetInsID());
    OspSetHBParam(m_dwUlcNode, MOBOSS_HB_TIME, MOBOSS_HB_NUM);


    CServMsg tLoginMsg;
    tLoginMsg.SetServMsg(pcMsg->content, pcMsg->length);

    //TS6610终端为了兼容5.0平台，特意补发了一个版本，新发的版本中，将登录的平台IP地址放到了TOld252MtLoginInfo结构体中
    TOld252MtLoginInfo tMtLoginInfo = *(TOld252MtLoginInfo*)tLoginMsg.GetMsgBody();
    u32 dwUlsIP_net = tMtLoginInfo.GetUlsIP();  //获取出来的是网络序     
    u32 dwUlsIP_host = ntohl(dwUlsIP_net);
    ClientSsnLog(LOG_LVL_KEYSTATUS, "[Old252_ProcUlcLoginReq]Mt LoginName(%s) Oem(%s) password:%s UlsIP(%u %s)\n", 
        tMtLoginInfo.GetLoginName(), tMtLoginInfo.GetOemRemark(), tMtLoginInfo.GetPassword(), dwUlsIP_net, BossUtils::GetIpString(dwUlsIP_host).c_str() );

    /////////
    //结构体转换   
    CLoginInfo tUlcLogInfo;
    tUlcLogInfo.SetDeviceType("TS6610"); //只可能是TS6610，直接写死算了，不然还要搞一个设备类型的u32与字符的对照表
    tUlcLogInfo.SetUserName(tMtLoginInfo.GetLoginName());
    tUlcLogInfo.SetOemMark(tMtLoginInfo.GetOemRemark());
    tUlcLogInfo.SetEncryptedAccountPwd(tMtLoginInfo.GetPassword());	
    tUlcLogInfo.SetApsLocalIp(dwUlsIP_host);  //

    //消息发送到主控
    CLoginExtend tExtend;
    tExtend.SetDetailBody(tUlcLogInfo);
    tExtend.SetSessionID(GetAppID(), GetInsID());
    PostMsgToCtrl(pcMsg->event, (u8 *)&tExtend, sizeof(tExtend));
   
    ClientSsnLog(LOG_LVL_KEYSTATUS, "[Old252_ProcUlcLoginReq]Mt ip(%s)  LocalXapIp:%s \n", BossUtils::GetIpString(m_dwUlcIP).c_str(), BossUtils::GetIpString(dwUlsIP_host).c_str());
    return;
}

void CUlcSsnInst::Proc252UlcLoginAck(CMessage *const pcMsg)
{
    if (NULL == pcMsg)
    {
        ClientSsnLog(LOG_LVL_ERROR, "[Proc252UlcLoginAck] The receive msg's pointer is NULL!\n");
        return;
    }

    //
    NextState(STATE_NORMAL);

    PostMsgToUlc(OLD_2_5_2_ULS_ULC_LOGIN_ACK, pcMsg->content, pcMsg->length);
    ClientSsnLog(LOG_LVL_KEYSTATUS, "[Proc252UlcLoginAck] ulc login succ!\n ");
    return;
}

void CUlcSsnInst::Proc252UlcLoginNack(CMessage *const pcMsg)
{
    if (NULL == pcMsg)
    {
        ClientSsnLog(LOG_LVL_ERROR, "[Proc252UlcLoginNack] The receive msg's pointer is NULL!\n");
        return;
    }
    ClientSsnLog(LOG_LVL_WARNING, "[Proc252UlcLoginNack] ulc:%s login fail!\n", GetUlcIPString().c_str());

    PostMsgToUlc(OLD_2_5_2_ULS_ULC_LOGIN_NACK, NULL, 0);
    OspDisconnectTcpNode(pcMsg->srcnode);

    return;
}

/*lint -restore*/

