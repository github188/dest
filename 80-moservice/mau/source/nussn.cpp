/*****************************************************************************
模块名      : MAU
文件名      : nussn.cpp
创建时间    : 2014年 08月 26日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifdef WIN32
#pragma warning( disable:4786 )
#endif

#include "inc.h"
#include "nussn.h"
#include "eventnumau.h"
#include "mauvc.h"
#include "evmau.h"
#include "cssconvertpb.h"
#include "v26struct.h"
#include "kdv323common.h"
#include "pasconst.h"
#include "zlib.h"
#include <typeinfo>
#include <bitset>
using namespace std;

extern CMauVCApp g_cMauVCApp;

CNUSsnInstance::DealFunMap CNUSsnInstance::s_deal_sipmtfun_map;
CNUSsnInstance::MtVerMap CNUSsnInstance::s_mapMtVer;

/*=============================================================================
函 数 名： CNUSsnInstance
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
//lint -sem(CNUSsnInstance::ClearInst,initializer)
CNUSsnInstance::CNUSsnInstance()
{
	ClearInst();
}


/*=============================================================================
函 数 名： ~CNUSsnInstance
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
CNUSsnInstance::~CNUSsnInstance(){}

/*=============================================================================
函 数 名： DaemonInstanceEntry
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CNUSsnInstance::DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp )
{
	//参数校验
	if( NULL == pcMsg || NULL == pcApp )
	{
		return ;
	}

	switch ( pcMsg->event )
	{
	case OSP_POWERON:
		DaemonProcPowerOn(pcMsg);
		break;
	case NU_MAU_REGIST_REQ:
		DaemonProcNuRegReq(pcMsg, pcApp);
		break;
	case MAU_PRINTINFO_CMD:
		DaemonProcPrintCmd(pcMsg, pcApp);
		break;
	default:
		break;
	}
}

/*=============================================================================
函 数 名： DaemonProcPowerOn
功    能： 守护实例处理初始化消息
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CNUSsnInstance::DaemonProcPowerOn( const CMessage * pcMsg )
{
	//状态初始化
	NextState( STATE_IDLE );

#ifdef WIN32
	OspRegistModule("conflistconvert.dll");
#endif
	InitConfListConvert();
	InitMtVerMap();
	RegDealFun();
	//初始化消息 发送给所有实例
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[DaemonProcPowerOn] DaemonInst PowerOn\n");
	//OspPost( MAKEIID(AID_MAU_NUSSN_APPID,CInstance::EACH), pcMsg->event, pcMsg->content, pcMsg->length );
}

/*=============================================================================
函 数 名： DaemonProcNuRegReq
功    能： 守护实例处理NU注册请求
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CNUSsnInstance::DaemonProcNuRegReq( CMessage * const pcMsg, CApp *pcApp )
{
	u32 dwNuNode = pcMsg->srcnode;
	u32 dwNuIId = pcMsg->srcid;
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[DaemonProcNuRegReq] The Nu node is :%d\n", dwNuNode);

	u8 byLoop = 0;
	u8 byIdleInstID = 0;
	u8 byExistInstID = 0;
	
	//查找GK是否已经注册
	for( byLoop = 1; byLoop <= MAX_NU_NUM; byLoop++ )
	{
		CNUSsnInstance * pcInst = (CNUSsnInstance*)pcApp->GetInstance( byLoop );
		if( NULL == pcInst )
		{
			continue;
		}
		
		//记录同IP并且已经注册的实例号
		if( STATE_NORMAL == pcInst->CurState() && dwNuNode == pcInst->m_dwNuNode )
		{
			byExistInstID = byLoop;
			break;
		}
		
		//记录找到的第一个空闲实例
		if( STATE_IDLE == pcInst->CurState() && 0 == byIdleInstID )
		{
			byIdleInstID = byLoop;
			pcInst->ProcNuRegReq(dwNuNode, dwNuIId, pcMsg->event);
		}
	}
	
	//GK已经注册 
	if( byExistInstID > 0 )
	{
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_NU, "[DaemonProcNuRegReq] The Nu node is %d, has registed at instance %d.\n", 
			dwNuNode, byExistInstID );
		OspPost( dwNuIId, pcMsg->event+2, NULL, 0, dwNuNode );
		
		//等待消息发送后 断链
		OspDelay( 500 );
		OspDisconnectTcpNode( dwNuNode );
		return ;
	}
	//没有空闲实例可用
	if( 0 == byIdleInstID )
	{
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_NU, "[DaemonProcNuRegReq] No Idle instance for new regist from nu.\n" );
		OspPost( dwNuIId, pcMsg->event+2, NULL, 0, dwNuNode );
		
		//等待消息发送后 断链
		OspDelay( 500 );
		OspDisconnectTcpNode( dwNuNode );
		return ;
	}
	return;
}

/*=============================================================================
函 数 名： DaemonProcPrintCmd
功    能： 打印
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/10/16  KDVP5.0     rnn                  创建
=============================================================================*/
void CNUSsnInstance::DaemonProcPrintCmd(CMessage * const pcMsg, CApp *pcApp)
{
	if( NULL == pcMsg->content || 0 == pcMsg->length )
	{
		return ;
	}	
	
	u16 wType = *(u16*)pcMsg->content;
	switch( wType )
	{
	case DUMPNUINFO:
		{
			OspPrintf( TRUE, FALSE, "\nInstID  IpAddr \n");
			OspPrintf( TRUE, FALSE, "\n------  ---------------  --------\n");
			CNUSsnInstance * pNUSsnInst = NULL;
			for( u8 byIdx = 1 ; byIdx <= MAXNUM_MCU_MC; byIdx++ )
			{
				pNUSsnInst = (CNUSsnInstance*)pcApp->GetInstance( byIdx );
				if( NULL == pNUSsnInst || STATE_IDLE == pNUSsnInst->CurState() )
				{
					continue;
				}
				OspPrintf( TRUE, FALSE, "%06d  %s \n", byIdx, strofip(ntohl(pNUSsnInst->m_dwNuIp)) );
			}
		}
		break;
	default:
		break;
	}
}

/*=============================================================================
函 数 名： ProcNuDisconnect
功    能： 处理Nu断链
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CNUSsnInstance::ProcNuDisconnect( CMessage * const pcMsg )
{
	//IDLE状态 直接返回
	if ( STATE_IDLE == CurState() )
    {
        return;
    }
	
	//判断是否是本实例对应的GK断链
    u32 dwDiscNode = *(u32*)pcMsg->content;
    if ( dwDiscNode != m_dwNuNode )
    {
        return;
    }
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_NU, "[ProcNuDisconnect] Inst %d receive disconnect. GK %s!\n", GetInsID(), strofip(ntohl(m_dwNuIp)) );
	
	//有效的NODE  断链
	if( INVALID_NODE != m_dwNuNode )
	{
		OspDisconnectTcpNode( m_dwNuNode );
	}
	
	//通知主控 GK断链
	u32 dwNuIID = MAKEIID( GetAppID(), GetInsID() );
	
	// 断链后的GK信息
	TNuData tNuData;
	tNuData.SetIsOnline(FALSE);
	tNuData.SetIpAddr(0);
	g_cMauVCApp.SetNuData(GetInsID(), tNuData);
	
	CMauData::SendMsgToMauVC(MAU_NUDISCONNECT_NTF, (u8*)&dwNuIID, sizeof(dwNuIID));
	
	//状态机切换
	NextState( STATE_IDLE );
	
	//清空实例的相关信息
	ClearInst();
	
	return ;
}

/*=============================================================================
函 数 名： ProcNuRegReq
功    能： 处理Nu注册请求
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/ 
void CNUSsnInstance::ProcNuRegReq(const u32 dwNodeId, const u32 dwSrcId, const u16 eventId)
{
	//本实例状态非IDLE
	if( STATE_IDLE != CurState() )
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[ProcNuRegReq] instance is wrong state.%d\n", CurState());
		return ;
	}
	
	//先进行状态机切换
	NEXTSTATE( STATE_NORMAL );
	//记录GK的相关信息
	m_dwNuNode = dwNodeId;
	m_dwNuIId = dwSrcId;
	m_dwNuIp = OspNodeIpGet(m_dwNuNode);

	
	//进行断链检测
	OspNodeDiscCBRegQ( m_dwNuNode, GetAppID(), GetInsID() );
	OspSetHBParam( m_dwNuNode, DEF_OSPDISC_HBTIME, DEF_OSPDISC_HBNUM );
	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_NU, "[ProcNuRegReq] Nu<%s> regist successful in instance %d.\n", 
		strofip(ntohl(m_dwNuIp)), GetInsID() );
	
	//记录连接上的GK信息
	TNuData tNuData;
    g_cMauVCApp.GetNuData(GetInsID(), tNuData);
	tNuData.SetIsOnline(TRUE);
	tNuData.SetIpAddr(ntohl(m_dwNuIp));
	g_cMauVCApp.SetNuData(GetInsID(), tNuData);
	
	//回复ACK
	SendMsgToNu(eventId + 1 );
	return ;
}

json_t_ptr CNUSsnInstance::SetMtCreateConfInfoToJson(const CKdvDomainInfo& tDomainInfo,
	const TReqHeadInfo& tHeadInfo,
	const TCreateConfReq& tCreateConfReq)
{
	//会议信息转换
	TConfInfo tConfInfo;
	TMiniConfInfo tMiniConfInfo = tCreateConfReq.GetConfInfo();
	CMauData::StructTranslate(tMiniConfInfo, tConfInfo);
	tConfInfo.SetDomainMoid(tDomainInfo.GetDomainMOID());
	tConfInfo.SetDomainName(tDomainInfo.GetWholeName());
	//设置创会者别名
	TMtAlias tMtAlias;
	tMtAlias.SetE164Alias(tHeadInfo.GetSrcMtE164());
	tConfInfo.SetCreaterAlias(tMtAlias.m_achAlias);


	TConfInfoEx tConfInfoEx;
	GetConfInfoExFromConfInfo(tConfInfo, tConfInfoEx);
	tConfInfoEx.SetPreApplyMS(g_cMauVCApp.GetPlatformIsRent() ? TRUE : FALSE);

	TMtAlias atMtList[MAXNUM_CONF_MT];
	u32 nMtNum = 0;
	GetMtListFromMiniConfInfo(tMiniConfInfo, tMtAlias, atMtList, nMtNum);

	TVmpModuleInfo tVmpModuleInfo;
	TransMiniVmpParamToVmpModule(tMiniConfInfo.GetVmpParam(), tVmpModuleInfo);

	TMixModule tMixModule;
	TransMiniMixParamToMixModule(tMiniConfInfo.GetMiniMixParam(), tMixModule);

	json_t_ptr pjRoot = json_object();
	if (pjRoot)
	{
		SetCreatorMoidToJsonObj(pjRoot, tHeadInfo.GetMtMOID());
		SetCreateConfParamToJson(pjRoot, tConfInfo);
		SetConfInfoExToJson(pjRoot, tConfInfoEx);
		SetReqHeadToJsonObj(pjRoot, tHeadInfo);
		u16 awBitRate[MAXNUM_CONF_MT] = { 0 };
		SetMtAliasListToJson(pjRoot, atMtList, awBitRate, nMtNum);
		SetVmpModuleInfoToJsonObj(pjRoot, tVmpModuleInfo);
		TAutorecAttrb tAutorecAttrb(tMiniConfInfo.GetAutorecAttrb());
		SetRecordInfoToJsonObj(pjRoot, tAutorecAttrb);
		CMauData::SetPollModuleToJson(pjRoot, tMiniConfInfo.GetMiniPollParam());
		CMauData::SetVipModuleToJson(pjRoot, tMiniConfInfo.GetMiniVipInfo());
		SetMixModuleToJsonObj(pjRoot, tMixModule);
	}
	
	return pjRoot;
}

void CNUSsnInstance::SetAutoMixAndVmp(TMiniConfInfo& tMiniConfInfo)
{
	TMiniVmpParam tVmpParam = tMiniConfInfo.GetVmpParam();
	//按照规格，不管老终端带什么vmp参数，租赁开vmp，自建不开vmp
	if (g_cMauVCApp.GetPlatformIsRent()
		&& tVmpParam.GetVMPMode() == 0)
	{
		tVmpParam.SetVMPStyle(VMP_STYLE_DYNAMIC);
		tVmpParam.SetVMPMode(CONF_VMPMODE_AUTO);
		tVmpParam.SetVMPBrdst(TRUE);
		tVmpParam.SetIsAddMmbAlias(TRUE);
		tMiniConfInfo.SetVmpParam(tVmpParam);
	}

	//老终端不带混音参数
	//自建默认不开混音，租赁默认开混音
	TMiniMixParam tMixParam = tMiniConfInfo.GetMiniMixParam();
	if (g_cMauVCApp.GetPlatformIsRent() 
		&& tMixParam.GetMixMode() == mcuNoMix)
	{
		tMixParam.SetMixMode(mcuWholeMix);
		tMiniConfInfo.SetMiniMixParam(tMixParam);
	}
}

void CNUSsnInstance::UpdateRealResolution(TCreateConfReq &tCreateConfInfo)
{
	u8 byResolution = tCreateConfInfo.GetConfInfo().GetCapSupport().GetMainSimCapSet().GetVideoResolution();
	u8 byRealResolution = byResolution;
	// wy[2015/07/16] 兼容2.6终端召集会议
	if (VIDEO_FORMAT_AUTO == byResolution)
	{
		byRealResolution = VIDEO_FORMAT_4CIF;
	}

	u16 wBitRate = tCreateConfInfo.GetConfInfo().GetCapSupport().GetMainSimCapSet().GetVideoMaxBitRate();
	//384k以下不允许开720p或1080p，默认改为w4cif
	if (wBitRate < MIN_BITRATE_720P)
	{
		if (VIDEO_FORMAT_HD720 == byResolution || VIDEO_FORMAT_HD1080 == byResolution)
		{
			byRealResolution = VIDEO_FORMAT_W4CIF;
		}
	}

	//码率在384K到1M之间，默认修改为720P
	if (MIN_BITRATE_1080P > wBitRate && MIN_BITRATE_720P < wBitRate)
	{
		if (VIDEO_FORMAT_HD1080 == byResolution)
		{
			byRealResolution = VIDEO_FORMAT_HD720;
		}
	}
	//256以下码率强制修改为CIF会议,256以上会议强制修改为4CIF会议
	if (wBitRate < MIN_BITRATE_4CIF)
	{
		byRealResolution = VIDEO_FORMAT_WCIF;
	}
	else if (VIDEO_FORMAT_CIF == byResolution)
	{
		byRealResolution = VIDEO_FORMAT_W4CIF;
	}

	tCreateConfInfo.GetConfInfo().GetCapSupport().GetMainSimCapSet().SetVideoResolution(byRealResolution);
}

void CNUSsnInstance::CompletedReqInfo(TReqHeadInfo& tReqInfo,
	const TPasConfMsg& tPasConfMsg,
	const CMauMtInfo& cMtInfo, 
	const s8* pchTimeId, 
	const u32 dwMsgEvent)
{
	tReqInfo.SetMtReqID(tPasConfMsg.GetSqn());
	tReqInfo.SetSrcMtE164(tPasConfMsg.GetE164());
	tReqInfo.SetMtAlias(tPasConfMsg.GetE164());
	tReqInfo.SetVersionId( tPasConfMsg.GetVerisonId() );
	tReqInfo.SetProductId( tPasConfMsg.GetProductId() );
	tReqInfo.SetProctocolType(tPasConfMsg.GetProtocolType());
	tReqInfo.SetMtMOID(cMtInfo.GetMOID());
	tReqInfo.SetDomainMOID(cMtInfo.GetDomainMOID());
	tReqInfo.SetTimeId(pchTimeId);
	tReqInfo.SetReqSrcID( MAKEIID(GetAppID(),GetInsID()) );
	tReqInfo.SetReqEventID(dwMsgEvent);
	tReqInfo.SetConfCreateBy( (u8)CONF_CREATE_MT );
}

void CNUSsnInstance::UpdateCreateConfParam(const CMauMtInfo& cMtInfo,
	const CKdvDomainInfo& cDomainInfo, TCreateConfReq &tCreateConfInfo)
{
	//wASK sipmt是没有的，是否应该作区分
	UpdateRealResolution(tCreateConfInfo);

	//获取终端召集会议所带的会议信息
	// 受限账号不允许召集非开放会议,所以平台侧保护，默认受限账号召集开放会议
	TMiniConfInfo& tMiniConfInfo = tCreateConfInfo.GetConfInfo();
	if (cMtInfo.GetLimitedFlag() 
		&& (0 == tCreateConfInfo.GetConfInfo().GetPublicConf()))
	{
		tMiniConfInfo.SetPublicConf(1);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]mt is limit and "
			"conf will be created is not a public conf, default active:"
			" change conf to public conf\n", __FUNCTION__);
	}

	//增加永久会议后，永久会议Duration为0，不再做判断
	// 终端召集会议时间为0，则改为2小时
	//if (0 == tMiniConfInfo.GetDuration())
	//{
	//	tMiniConfInfo.SetDuration(120);
	//	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_VC, "[%s]conf duration is %d."
	//		" Change to 120 min.!\n", __FUNCTION__,
	//		tMiniConfInfo.GetDuration());
	//}

	//老终端不下会议规模和终端列表，则只能创小方会议
	if (tMiniConfInfo.GetMaxJoinMtNum() == 0)
	{
		const u8 byMaxJoinedMtNum = 8;
		tMiniConfInfo.SetMaxJoinMtNum(byMaxJoinedMtNum);
	}
}

void CNUSsnInstance::ProcMtGetConfListReq(CMessage * const pcMsg)
{
	TPasConfMsg tPasConfMsg(pcMsg->content, pcMsg->length);
	ProcMtGetConfListReq(tPasConfMsg, pcMsg->event);
}
void CNUSsnInstance::ProcMtGetConfListReq(TPasConfMsg& tPasConfMsg, const u16 wMsgEvent)
{
	CPrintFunInOut cPrintFun(__FUNCTION__, MID_MAU_NU, "msgEvent: %s", OspEventDesc(wMsgEvent));

	CBaseMt* ptMt = CMtFactory::GetMt(tPasConfMsg.GetProtocolType(),
		tPasConfMsg.GetProductId(), tPasConfMsg.GetVerisonId());
	if (ptMt == NULL)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]Unsupport mt type\n", __FUNCTION__);
		return;
	}
	s8  abyTimeId[64] = { 0 };
	ptMt->GetTimeId(tPasConfMsg, abyTimeId);
	
	const s8* pchE164 = tPasConfMsg.GetE164();
	CMauMtInfo cMtInfo;
	CKdvDomainInfo cDomainInfo;
	vector<TSimpleConfInfo> vecConfList;
	u32 dwErrCode = 0;
	if (!CheckAndGetMtDomain(pchE164, cMtInfo, cDomainInfo))
	{
		dwErrCode = (strlen(cMtInfo.GetDomainMOID()) == 0 ? ERR_MT_NOEXIST : ERR_DOMAIN_INVALID);
	}
	else if (!ptMt->GetConfList(wMsgEvent, cMtInfo.GetDomainMOID(),
		vecConfList, cMtInfo.GetE164(), cMtInfo.GetLimitedFlag()))
	{
		dwErrCode = ERR_DB_OPR;
	}
	if(dwErrCode != 0)
	{
		tPasConfMsg.SetErrID(ptMt->TransErrorId(dwErrCode));
		const u16 wNackRspMsgEvent = (strcmp(tPasConfMsg.GetProtocolType(),
			"sip") == 0 ? MAU_NU_TRANSPARENT_CHANNEL_NACK : wMsgEvent + 2);
		ptMt->ConstructRspMsg(wMsgEvent, NULL, 0, TRUE, tPasConfMsg, abyTimeId);
		SendMsgToNu(wNackRspMsgEvent, (u8*)&tPasConfMsg, sizeof(tPasConfMsg));
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]DomainOrMt Invalid or redis exception, errcode: %u\n",
			__FUNCTION__, tPasConfMsg.GetErrID());
		return;
	}

	const u16 wRspMsgEvent = (strcmp(tPasConfMsg.GetProtocolType(),
		"sip") == 0 ? MAU_NU_TRANSPARENT_CHANNEL_ACK : wMsgEvent + 3);
	if (vecConfList.empty())
	{
		ptMt->ConstructRspMsg(wMsgEvent, NULL, 0, TRUE, tPasConfMsg, abyTimeId);
		SendMsgToNu(wRspMsgEvent, (u8*)&tPasConfMsg, sizeof(tPasConfMsg));
		return;
	}
	TSimpleConfInfo paConfList[MAX_CONFNUM_ONEPACK];
	for (vector<TSimpleConfInfo>::iterator confIter = vecConfList.begin();
	confIter != vecConfList.end(); ++confIter)
	{
		const u32 dwHandleConfNum = distance(vecConfList.begin(), confIter) + 1;
		const u8 byConfListIndex = (dwHandleConfNum - 1) % MAX_CONFNUM_ONEPACK;
		paConfList[byConfListIndex] = *confIter;
		const BOOL32 bReachMaxConfNumOnePack = ((dwHandleConfNum % MAX_CONFNUM_ONEPACK) == 0);
		if (dwHandleConfNum == distance(vecConfList.begin(), vecConfList.end()))
		{
			u8 byConfNum = dwHandleConfNum % MAX_CONFNUM_ONEPACK; 
			if (byConfNum == 0)
			{
				byConfNum = MAX_CONFNUM_ONEPACK;
			}
			ptMt->ConstructRspMsg(wMsgEvent, paConfList, byConfNum, TRUE, tPasConfMsg, abyTimeId);
			SendMsgToNu(wRspMsgEvent, (u8*)&tPasConfMsg, (u16)sizeof(tPasConfMsg));
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]send last %u conf to nu\n",
				__FUNCTION__, byConfNum);
		}
		else if (bReachMaxConfNumOnePack)
		{
			ptMt->ConstructRspMsg(wMsgEvent, paConfList, MAX_CONFNUM_ONEPACK,
				FALSE, tPasConfMsg, abyTimeId);
			SendMsgToNu(wRspMsgEvent, (u8*)&tPasConfMsg, (u16)sizeof(tPasConfMsg));
			memset(paConfList, 0, sizeof(TSimpleConfInfo) * MAX_CONFNUM_ONEPACK);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]send %u conf to nu,"
				" not last package\n", __FUNCTION__, MAX_CONFNUM_ONEPACK);
		}
	}
}

/*=============================================================================
函 数 名： ProcMtGetConfDetailReq
功    能： 获取会议详细信息
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CNUSsnInstance::ProcMtGetConfDetailReq( CMessage * const pcMsg )
{
	if( STATE_NORMAL != CurState() )
	{
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_NU, "[ProcMtGetConfDetailReq] Receive Msg<%d> in wrong state<%d>\n", pcMsg->event, CurState() );
		return ;
	}
	
	TPasConfMsg tPasConfMsg(pcMsg->content, pcMsg->length);
	ProcMtGetConfDetailReq(tPasConfMsg, pcMsg->event);
}

void CNUSsnInstance::ProcMtGetConfDetailReq(TPasConfMsg& tPasConfMsg, const u16 wMsgEvent)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NU);
	
	CBaseMt* ptMt = CMtFactory::GetMt(tPasConfMsg.GetProtocolType(),
		tPasConfMsg.GetProductId(), tPasConfMsg.GetVerisonId());
	if (ptMt == NULL)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]Unsupport mt type\n", __FUNCTION__);
		return;
	}

	s8  abyTimeId[64] = { 0 };
	ptMt->GetTimeId(tPasConfMsg, abyTimeId);

	CMauMtInfo cMtInfo;
	CKdvDomainInfo cDomainInfo;
	s8 achConfE164[MAXLEN_CONFE164 + 1] = {0};
	const s8* pchE164 = tPasConfMsg.GetE164();
	TConfDetailInfo tConfDetailInfo;
	u32 dwErrCode = ERR_DB_SUCCESS;
	u32 dwRspMsgType = (strcmp(tPasConfMsg.GetProtocolType(), "sip") == 0 ? MAU_NU_TRANSPARENT_CHANNEL_ACK : wMsgEvent + 1);
	if (!CheckAndGetMtDomain(pchE164, cMtInfo, cDomainInfo)
		|| !ptMt->GetConfE164(tPasConfMsg, achConfE164))
	{
		dwErrCode = (strlen(cMtInfo.GetDomainMOID()) == 0 ? ERR_MT_NOEXIST : ERR_DOMAIN_INVALID);
	}
	else if ((dwErrCode = g_cMauVCApp.GetConfDetailInfo(cMtInfo, achConfE164, tConfDetailInfo)) != ERR_DB_SUCCESS)
	{
		tConfDetailInfo.m_tNewConfBaseInfo.SetConfE164(achConfE164);
	}
	ptMt->ConstructRspMsg(tConfDetailInfo, dwErrCode, tPasConfMsg, abyTimeId);
	SendMsgToNu((dwErrCode == ERR_DB_SUCCESS ? dwRspMsgType : dwRspMsgType + 1), (u8*)&tPasConfMsg, sizeof(tPasConfMsg));
	LongLogPrint(LOG_LVL_DETAIL, MID_MAU_NU, (const s8*)tPasConfMsg.GetContentBuf(), tPasConfMsg.GetContentBufLen(), FALSE);
}

/*=============================================================================
函 数 名： ProcMtGetConfNumReq
功    能： 获取该终端对应的会议个数和空闲会议个数
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CNUSsnInstance::ProcMtGetConfNumReq( CMessage * const pcMsg)
{
	TPasConfMsg tPasConfMsg(pcMsg->content, pcMsg->length);
	ProcMtGetConfNumReq(tPasConfMsg, pcMsg->event);
}

void CNUSsnInstance::ProcMtGetConfNumReq(TPasConfMsg& tPasConfMsg, const u16 wMsgEvent)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NU);
	if( STATE_NORMAL != CurState() )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_NU, "[ProcMtConfNumReq] Receive Msg<%d> in wrong state<%d>\n",
			wMsgEvent, CurState() );
		return ;
	}

	CBaseMt* ptMt = CMtFactory::GetMt(tPasConfMsg.GetProtocolType(),
		tPasConfMsg.GetProductId(), tPasConfMsg.GetVerisonId());
	if (ptMt == NULL)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]Unsupport mt type\n", __FUNCTION__);
		return;
	}

	CMauMtInfo cMtInfo;
	CKdvDomainInfo cDomainInfo;
	const s8* pchMtE164 = tPasConfMsg.GetE164();
	u32 dwLicensedConfNum = 0;
	u32 dwMaxLicensedConfNum = 0;
	u32 dwRspMsgEvent = (strcmp(tPasConfMsg.GetProtocolType(), "sip") == 0 ? MAU_NU_TRANSPARENT_CHANNEL_ACK : wMsgEvent + 1);
	u32 dwErrCode = ERR_DB_SUCCESS;
	if (!CheckAndGetMtDomain(pchMtE164, cMtInfo, cDomainInfo))
	{
		dwErrCode = (strlen(cMtInfo.GetDomainMOID()) == 0 ? ERR_MT_NOEXIST : ERR_DOMAIN_INVALID);
	}
	else if (!g_cMauVCApp.GetConfNumByDomain(cMtInfo.GetDomainMOID(), enumMaxLicensedConfNum, dwMaxLicensedConfNum)
		|| !g_cMauVCApp.GetConfNumByDomain(cMtInfo.GetDomainMOID(), enumLicensedConfNum, dwLicensedConfNum))
	{
		dwErrCode = ERR_DB_OPR;
	}
	ptMt->ConstructRspMsg(dwErrCode, (u16)dwMaxLicensedConfNum, (u16)dwMaxLicensedConfNum - dwLicensedConfNum, tPasConfMsg);
	SendMsgToNu(dwErrCode == ERR_DB_SUCCESS ? dwRspMsgEvent : dwRspMsgEvent + 1, (u8*)&tPasConfMsg, (u16)sizeof(tPasConfMsg));
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]errCode: %u,MaxLicensedConfNum: %u, licensedConfNum: %u\n",
		__FUNCTION__, dwErrCode, dwMaxLicensedConfNum, dwLicensedConfNum);
}

/*=============================================================================
函 数 名： ProcCreatConfByTempReq
功    能： NU通过个人模板创会请求
算法实现：
全局变量：
参    数： CMessage * const pcMsg
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2016/04/08  5.0         王力                    创建
=============================================================================*/
void CNUSsnInstance::ProcCreatConfByTempReq(CMessage * const pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NU);
	if (STATE_NORMAL != CurState())
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[ProcCreatConfByTempReq] Receive Msg<%d> in wrong state<%d>\n", pcMsg->event, CurState());
		return;
	}
	
	u8 * pbyMsg = pcMsg->content;
	if (NULL == pbyMsg)
	{
		return;
	}
	TPasConfMsg tPasConfMsg(pbyMsg, pcMsg->length);
	
	s8 *pchConfE164 = (s8*)(tPasConfMsg.GetContentBuf() + sizeof(u8) + sizeof(u16));
	
	string strJsonBuf;
	//查询个人模板中是否存在该会议
	BOOL32 bFindPT = g_cMauVCApp.FindPersonalTempInfo(pchConfE164, strJsonBuf);
	
	json_t_ptr pjRoot = NULL;
	LoadJson(strJsonBuf.c_str(), strJsonBuf.length(), pjRoot, FALSE);

	if (!bFindPT || !pjRoot)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[ProcCreatConfByTempReq] personaltemplate not exist.\n");
		tPasConfMsg.SetErrID(KDVP_NSM_MCU_CONFNOTEXIST);
		SendMsgToNu(pcMsg->event + 2, (u8*)&tPasConfMsg, (u16)sizeof(tPasConfMsg));
		return;
	}

	const s8* pchMoid = NULL;
	GetDomainMoidFromJsonObj(pjRoot, &pchMoid);

	//记录请求的相关信息
	TReqHeadInfo tReqInfo;
	tReqInfo.SetMtReqID(tPasConfMsg.GetSqn());
	tReqInfo.SetSrcMtE164(tPasConfMsg.GetE164());
	tReqInfo.SetConfE164(pchConfE164);
	tReqInfo.SetMtAlias(tPasConfMsg.GetE164());
	tReqInfo.SetReqSrcID( MAKEIID(GetAppID(),GetInsID()) );
	tReqInfo.SetReqEventID( pcMsg->event );
	tReqInfo.SetConfCreateBy( CONF_CREATE_MT );
	tReqInfo.SetVersionId( tPasConfMsg.GetVerisonId() );
	tReqInfo.SetProductId( tPasConfMsg.GetProductId() );
	tReqInfo.SetProctocolType(tPasConfMsg.GetProtocolType());
	tReqInfo.SetDomainMOID(pchMoid);

	//级联呼叫个人模板，租赁4小时，自建默认为永久会议
	const u32 dwPersonTemplateDuration = (g_cMauVCApp.GetPlatformIsRent() ? 240 : 0);
	SetConfDurationToJsonObj(pjRoot, dwPersonTemplateDuration);
	SetCreatorNameToJsonObj(pjRoot, tPasConfMsg.GetE164());
	
	CKdvDomainInfo tDomainInfo;
	if (!g_cMauVCApp.GetDomainInfo(pchMoid, tDomainInfo)
		|| !tDomainInfo.IsUsedFlag())
	{
		SendMsgToNu( pcMsg->event + 2, pcMsg->content, pcMsg->length );
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]domain invalid"
			" or unused\n", __FUNCTION__);
		return;
	}
	
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[ProcCreatConfByTempReq] mt<InTPasConfMsg.%d  mtE164.%s  VerId.%s  ProductId.%s> require join conf<%s>.\n",
		tPasConfMsg.GetSqn(), tPasConfMsg.GetE164(), tPasConfMsg.GetVerisonId(), tPasConfMsg.GetProductId(), pchConfE164);
	
	json_str pchData(pjRoot);
	//转发给MAUVC  由主控进行处理
	CMauMsg cServMsg;
	cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
	cServMsg.CatMsgBody(pchData, strlen(pchData));
	CMauData::SendMsgToMauVC(pcMsg->event, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
}
/*=============================================================================
函 数 名： ProcCreatConfByTempRsp
功    能： NU通过个人模板创会消息回复
算法实现：
全局变量：
参    数： CMessage * const pcMsg
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2016/04/08  5.0         王力                    创建
=============================================================================*/
void CNUSsnInstance::ProcCreatConfByTempRsp(CMessage * const pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NU);
	if (STATE_NORMAL != CurState())
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[CNUSsnInstance::ProcCreateConfRsp] Receive Msg<%d> in wrong state<%d>\n", pcMsg->event, CurState());
		return;
	}

	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();

	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof(TReqHeadInfo);

	TPasConfMsg tPasConfMsg;
	tPasConfMsg.SetProductId(tReqInfo.GetProductId());
	tPasConfMsg.SetVerisonId(tReqInfo.GetVersionId());
	tPasConfMsg.SetSqn(tReqInfo.GetMtReqID());
	tPasConfMsg.SetE164(tReqInfo.GetSrcMtE164());
	tPasConfMsg.SetProtocolType(tReqInfo.GetProctocolType());

	u8 byType = emConfMsgContentType_E164;
	u16 wConfE164 = strlen(tReqInfo.GetConfE164());
	tPasConfMsg.SetContentBuf(&byType, sizeof(u8));
	tPasConfMsg.CatContentBuf((u8 *)&wConfE164, sizeof(u16));
	tPasConfMsg.CatContentBuf((u8 *)tReqInfo.GetConfE164(), strlen(tReqInfo.GetConfE164()));

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[ProcCreatConfByTempRsp]MtReqId<InReqHead.%d InTPasConfMsg.%d>!\n", tReqInfo.GetMtReqID(), tPasConfMsg.GetSqn());

	switch (pcMsg->event)
	{
	case MAU_NU_CREATECONFBYTEMP_ACK:
		break;
	case MAU_NU_CREATECONFBYTEMP_NACK:
		{
			tPasConfMsg.SetErrID(tReqInfo.GetErrID());
		}
		break;
	default:
		break;
	}

	LongLogPrint(LOG_LVL_DETAIL, MID_MAU_NU, (s8*)tPasConfMsg.GetContentBuf(), tPasConfMsg.GetContentBufLen());
	SendMsgToNu(pcMsg->event, (u8*)&tPasConfMsg, (u16)sizeof(tPasConfMsg));
	LogPrint(LOG_LVL_DETAIL, MID_MAU_NU, "[CNUSsnInstance::ProcCreatConfByTempRsp] Send msg.%u<%s> to NU\n", pcMsg->event, OspEventDesc(pcMsg->event));
}

void CNUSsnInstance::ProcMtJoinConfReq(CMessage * const pcMsg)
{
	if (pcMsg->content == NULL)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]msgEvent: %s content is NULL\n",
			__FUNCTION__, OspEventDesc(pcMsg->event));
		return;
	}
	TPasConfMsg tPasConfMsg(pcMsg->content, pcMsg->length);
	ProcMtJoinConf(tPasConfMsg, pcMsg->event);
}

void CNUSsnInstance::ProcMtJoinConf(TPasConfMsg& tPasConfMsg, const u16 wMsgEvent)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NU);
	if (STATE_NORMAL != CurState())
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s] Receive Msg<%d> in wrong state<%d>\n",
			__FUNCTION__, wMsgEvent, CurState());
		return;
	}
	CBaseMt* pcMt = CMtFactory::GetMt(tPasConfMsg.GetProtocolType(),
		tPasConfMsg.GetProductId(), tPasConfMsg.GetVerisonId());
	if (pcMt == NULL)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]unsupoort mt type\n", __FUNCTION__);
		return;
	}
	s8  abyTimeId[64] = { 0 };
	TJoinConfReq tJoinConfReq;
	pcMt->GetJoinedConfInfo(tPasConfMsg, tJoinConfReq, abyTimeId);

	TReqHeadInfo tReqInfo;
	CMauMtInfo cMtInfo;
	CKdvDomainInfo cDoaminInfo;
	if (!CheckAndGetMtDomain(tPasConfMsg.GetE164(), cMtInfo, cDoaminInfo))
	{
		const u32 dwErrCode = (strlen(cMtInfo.GetDomainMOID()) == 0 ? ERR_MT_NOEXIST : ERR_DOMAIN_INVALID);
		tPasConfMsg.SetErrID(pcMt->TransErrorId(dwErrCode));
		pcMt->ConstructRspMsg(wMsgEvent,  tJoinConfReq.GetConfE164(),
			abyTimeId, tPasConfMsg.GetErrID(), tPasConfMsg);
		const u16 wRspMsgEvent = (strcmp(tPasConfMsg.GetProtocolType(),
			"sip") == 0 ? MAU_NU_TRANSPARENT_CHANNEL_NACK : wMsgEvent + 2);
		SendMsgToNu(wRspMsgEvent, (u8*)&tPasConfMsg, sizeof(tPasConfMsg));
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]get mt or domain or "
			"add req fail, errCode:%u\n", __FUNCTION__, dwErrCode);
		return;
	}

	CompletedReqInfo(tReqInfo, tPasConfMsg, cMtInfo, abyTimeId, wMsgEvent);
	tReqInfo.SetSrcMtE164( tJoinConfReq.GetMtE164() );
	tReqInfo.SetConfE164( tJoinConfReq.GetConfE164() );
	tReqInfo.SetMtType( tJoinConfReq.GetMtType() );
	tReqInfo.SetConfPassword(tJoinConfReq.GetPassword());
	tReqInfo.SetMtCallBitrate(tJoinConfReq.GetCallBitRate());
	if ( MT_TYPE_NONE == tReqInfo.GetMtType() )
	{
		tReqInfo.SetMtType( (u8)MT_TYPE_MT );
	}
	else if ( MT_TYPE_PHONE == tReqInfo.GetMtType() )
	{
		tReqInfo.SetMtAlias( tJoinConfReq.GetPhoneNumber() );
		tReqInfo.SetReqMtAliasType( mtAliasTypePartNumber );
		LogPrint( LOG_LVL_DETAIL, MID_MAU_NU, "[%s] phone<%s>\n",
			__FUNCTION__, tReqInfo.GetMtAlias() );
	}
	else
	{ 
		tReqInfo.SetMtAlias(tJoinConfReq.GetMtE164());
		tReqInfo.SetReqMtAliasType( mtAliasTypeE164 );
	}
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]MtReqId<InReqHead.%d InTPasConfMsg.%d>!\n",
		__FUNCTION__, tReqInfo.GetMtReqID(), tPasConfMsg.GetSqn() );

	CMauMsg cServMsg;
	cServMsg.SetMsgBody( (u8*)&tReqInfo, sizeof(tReqInfo) );

	TRedisConfInfo tRedisConf;
	CRedisBookConf cBookConf;

	const u16 ERROR_GETCONFINFO_FAILED = 0;
	const u16 ERROR_GETBOOKINFO_FAILED = 1;
	try
	{
		do 
		{
			if (!g_cMauVCApp.GetConfExInfo(tReqInfo.GetConfE164(), tRedisConf))
			{
				throw ERROR_GETCONFINFO_FAILED;
			}
			if (IsRedisHasConfExData(tRedisConf))
			{
				CMauData::NoticeCmuSsnAddMt(tRedisConf.GetCmuIp(), tReqInfo);
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]conf:%s exist, "
					"notice mcu:%s add mt:%s\n", __FUNCTION__,
					tRedisConf.GetConfE164(), tRedisConf.GetCmuIp(), tReqInfo.GetSrcMtE164());
				break;
			}

			if (!g_cMauVCApp.GetBookConf(tReqInfo.GetConfE164(), cBookConf))
			{
				throw ERROR_GETBOOKINFO_FAILED;
			}

			if (strlen(cBookConf.GetConfE164()) != 0)
			{
				CMauData::SendMsgToMauVC(MAU_MAU_MTCREATECONFBYBOOKCONF_REQ,
					cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]mt:%s join bookconf:%s\n",
					__FUNCTION__, tReqInfo.GetSrcMtE164(), cBookConf.GetConfE164());
				break;
			}

			CMauData::SendMsgToMauVC(MAU_MAU_MTCREATECONFBYPUBLICTEMP_REQ,
				cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]mt:%s join public Template<%s>\n",
				__FUNCTION__, tReqInfo.GetSrcMtE164(), tReqInfo.GetConfE164());
		} while (0);
	}
	catch (u16 wError)
	{
		tPasConfMsg.SetErrID(pcMt->TransErrorId(ERR_DB_OPR));
		pcMt->ConstructRspMsg(wMsgEvent, tJoinConfReq.GetConfE164(),
			abyTimeId, tPasConfMsg.GetErrID(), tPasConfMsg);
		const u16 wRspMsgEvent = (strcmp(tPasConfMsg.GetProtocolType(),
			"sip") == 0 ? MAU_NU_TRANSPARENT_CHANNEL_NACK : wMsgEvent + 2);
		SendMsgToNu(wRspMsgEvent, (u8*)&tPasConfMsg, sizeof(tPasConfMsg));
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]%s Fail, Error Code %d\n",
			__FUNCTION__, wError == ERROR_GETCONFINFO_FAILED ? "GetConfInfo" : "GetBookInfo",
			tPasConfMsg.GetErrID());
	}
}

void CNUSsnInstance::ProcMauToMtRsp(CMessage * const pcMsg)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NU, OspEventDesc(pcMsg->event));
	if (STATE_NORMAL != CurState())
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[CNUSsnInstance::ProcCreateConfRsp] Receive Msg<%d> in wrong state<%d>\n", pcMsg->event, CurState());
		return;
	}

	CMauMsg cServMsg(pcMsg->content, pcMsg->length);
	u8 * pbyMsg = cServMsg.GetMsgBody();
	TReqHeadInfo tReqInfo = *(TReqHeadInfo*)pbyMsg;
	pbyMsg += sizeof(TReqHeadInfo);

	TPasConfMsg tPasConfMsg;
	tPasConfMsg.SetProductId(tReqInfo.GetProductId());
	tPasConfMsg.SetVerisonId(tReqInfo.GetVersionId());
	tPasConfMsg.SetSqn(tReqInfo.GetMtReqID());
	tPasConfMsg.SetE164(tReqInfo.GetSrcMtE164());
	tPasConfMsg.SetProtocolType(tReqInfo.GetProctocolType());
	tPasConfMsg.SetIsLastMsg(TRUE);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[ProcMauToMtRsp]MtReqId<InReqHead.%d InTPasConfMsg.%d>!\n", tReqInfo.GetMtReqID(), tPasConfMsg.GetSqn());

	CBaseMt* ptMt = CMtFactory::GetMt(tPasConfMsg.GetProtocolType(),
		tPasConfMsg.GetProductId(), tPasConfMsg.GetVerisonId());
	if (ptMt == NULL)
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]unsupport mt type\n", __FUNCTION__);
		return;
	}

	tReqInfo.SetErrID(ptMt->TransErrorId(tReqInfo.GetErrID()));
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[ProcMauToMtRsp]RspErrCode: %u\n", tReqInfo.GetErrID());
	
	const u16 wMsgEvent = pcMsg->event;
	if (wMsgEvent == MAU_NU_CREATECONF_ACK
		|| wMsgEvent == MAU_NU_CREATECONF_NACK
		|| wMsgEvent == MAU_NU_JOINCONF_ACK
		|| wMsgEvent == MAU_NU_JOINCONF_NACK)
	{
		ptMt->ConstructRspMsg(wMsgEvent, tReqInfo.GetConfE164(),
			tReqInfo.GetTimeId(), tReqInfo.GetErrID(), tPasConfMsg);
		const u16 wRspMsgEvent = (strcmp(tPasConfMsg.GetProtocolType(), "sip") == 0 ? MAU_NU_TRANSPARENT_CHANNEL_ACK : wMsgEvent);
		SendMsgToNu(wRspMsgEvent, (u8*)&tPasConfMsg, (u16)sizeof(tPasConfMsg));
		LongLogPrint(LOG_LVL_DETAIL, MID_MAU_NU, (s8*)tPasConfMsg.GetContentBuf(), tPasConfMsg.GetContentBufLen());
	}
}

/*=============================================================================
函 数 名： InstanceEntry
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CNUSsnInstance::InstanceEntry(  CMessage * const pcMsg )
{
	//参数校验
	if( NULL == pcMsg )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_NU, "[InstanceEntry] param CMessage is NULL!\n" );
		return ;
	}

	switch ( pcMsg->event )
	{
	case OSP_DISCONNECT:
		ProcNuDisconnect( pcMsg );
		break;
	case NU_MAU_CREATECONF_REQ:             // 终端召集会议请求
		ProcMtCreateConfReq( pcMsg );
		break;
	case NU_MAU_JOINCONF_REQ:               // 终端加入已召开会议/通过模板、预约会议创会
		ProcMtJoinConfReq( pcMsg );
		break;
	case NU_MAU_GETCONFLIST_REQ:            // 终端获取即时会议列表
	case NU_MAU_GETTEMPLATELIST_REQ:        // 终端获取公共模板列表
	case NU_MAU_GETBOOKLIST_REQ:            // 终端获取预约会议列表
		ProcMtGetConfListReq(pcMsg);
		break;
	case NU_MAU_GETCONFDETAIL_REQ:          // 终端获取会议详细信息
		ProcMtGetConfDetailReq( pcMsg );
		break;
	case NU_MAU_GETCONFNUM_REQ:             // 终端获取会议数目
		ProcMtGetConfNumReq(pcMsg);
		break;
	case NU_MAU_TRANSPARENT_CHANNEL_REQ:
		ProcSipMtMsg(pcMsg);
		break;
	case MAU_NU_CREATECONF_ACK:
	case MAU_NU_CREATECONF_NACK:
	case MAU_NU_JOINCONF_ACK:
	case MAU_NU_JOINCONF_NACK:
		ProcMauToMtRsp( pcMsg );
		break;
	case NU_MAU_CREATECONFBYTEMP_REQ:  //终端通过个人模板创会请求
		ProcCreatConfByTempReq(pcMsg);
		break;
	case MAU_NU_CREATECONFBYTEMP_ACK:
	case MAU_NU_CREATECONFBYTEMP_NACK:
		ProcCreatConfByTempRsp(pcMsg);
		break;
	default:
		break;
	}
}


/*=============================================================================
函 数 名： SendMsgToNu
功    能： 向GK发送消息
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CNUSsnInstance::SendMsgToNu( u16 wEvent, u8* const pbyMsg, u16 wLen )
{
	if( OspIsValidTcpNode( m_dwNuNode ) )
	{
		//sip消息需要压缩
		if (MAU_NU_TRANSPARENT_CHANNEL_ACK == wEvent
			|| MAU_NU_TRANSPARENT_CHANNEL_NACK == wEvent)
		{
			TPasConfMsg tPasConfMsg(pbyMsg, wLen);
			u8 achCompressXmlMsg[LEN_10240 * 2 + 1] = { 0 };
			u32 dwCompressXmlMsgLen = sizeof(achCompressXmlMsg);
			const u32 dwErrCode = compress(achCompressXmlMsg, (uLongf*)&dwCompressXmlMsgLen,
				tPasConfMsg.GetContentBuf(), tPasConfMsg.GetContentBufLen());
			if (dwErrCode != 0)
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[ProcSipMtMsg] compress sip msg err, errCode: %d\n", dwErrCode);
				return ;
			}
			tPasConfMsg.SetContentBuf(achCompressXmlMsg, dwCompressXmlMsgLen);
			post( m_dwNuIId, wEvent, (u8*)&tPasConfMsg, sizeof(tPasConfMsg), m_dwNuNode );
		}
		else
		{
			post( m_dwNuIId, wEvent, pbyMsg, wLen, m_dwNuNode );
		}
		return ;
	}
	
	return ;
}


/*=============================================================================
函 数 名： ClearInst
功    能： 清空实例的相关数据
算法实现： 
全局变量： 
参    数： 
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0         王也                    创建
=============================================================================*/
void CNUSsnInstance::ClearInst( void )
{
	m_dwNuIId  = 0;
	m_dwNuIp   = 0;
	m_dwNuNode =INVALID_NODE;
}

CNUSsnInstance::EmMtVer CNUSsnInstance::GetMtVer(const s8* pchProductId, const s8* pchVersionId)
{
	EmMtVer emVer = emMtUnknown;

	// 5.0及后续终端VerId为"MT-50"
	if (0 == strncmp("MT-50", pchVersionId, 5))
	{
		emVer = emMtV50;
	}
	else
	{
		emVer = FindMtVer(pchProductId, pchVersionId);
	}

	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[CNUSsnInstance::GetMtVer]ProductId: %s, VersionId: %s, RealVer:%d\n", pchProductId, pchVersionId, emVer);

	return emVer;
}

void CNUSsnInstance::RegDealFun()
{
	s_deal_sipmtfun_map[SIP_MT_CREATECONF_REQ] = &CNUSsnInstance::ProcMtCreateConf;
	s_deal_sipmtfun_map[SIP_MT_JOINCONF_REQ] = &CNUSsnInstance::ProcMtJoinConf;
	s_deal_sipmtfun_map[SIP_MT_GETCONFNUM_REQ] = &CNUSsnInstance::ProcMtGetConfNumReq;
	s_deal_sipmtfun_map[SIP_MT_GETCONFDETAIL_REQ] = &CNUSsnInstance::ProcMtGetConfDetailReq;

	s_deal_sipmtfun_map[SIP_MT_GETCONFLIST_REQ] = &CNUSsnInstance::ProcMtGetConfListReq;
	s_deal_sipmtfun_map[SIP_MT_GETBOOKLIST_REQ] = &CNUSsnInstance::ProcMtGetConfListReq;
	s_deal_sipmtfun_map[SIP_MT_GETTEMPLIST_REQ] = &CNUSsnInstance::ProcMtGetConfListReq;
}

void CNUSsnInstance::ProcSipMtMsg( CMessage * const pcMsg )
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NU);
	if( STATE_NORMAL != CurState() )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_NU, "[ProcSipMtMsg] Receive Msg<%d> in wrong state<%d>\n", pcMsg->event, CurState() );
		return ;
	}

	u8 * pbyMsg = pcMsg->content;
	if ( NULL ==  pbyMsg )
	{
		return;
	}
	//首先判断是否是sip终端
	TPasConfMsg tPasConfMsg(pbyMsg, pcMsg->length);
	if (strcmp(tPasConfMsg.GetProtocolType(), "sip") != 0)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_NU, "[ProcSipMtMsg] is not sip mt,return!" );
		return ;
	}

	u16 wEventId = 0;
	s8 achB64XmlMsg[LEN_10240 * 5 + 1] = { 0 };
	CTMessageWrap::ConvertXmlMsg((u8*)tPasConfMsg.GetContentBuf(), wEventId, achB64XmlMsg, sizeof(achB64XmlMsg));
	//检查消息类型，防止接入误投sip消息
	if (s_deal_sipmtfun_map.find(wEventId) == s_deal_sipmtfun_map.end())
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_NU, "[ProcSipMtMsg]event err, eventid: %u\n", wEventId );
		return ;
	}
	u8 achUncompressXmlMsg[LEN_10240 * 20 + 1] = { 0 };
	u32 dwUncompressXmlMsgLen = sizeof(achUncompressXmlMsg);
	const s32 dwErrCode = B64DecodeAndUnCompressXmlMsg(achB64XmlMsg, achUncompressXmlMsg, dwUncompressXmlMsgLen);
	if (dwErrCode != 0)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_NU, "[ProcSipMtMsg]decode and uncompressxml msg err,"
			"errCode: %d", dwErrCode);
		return ;
	}
	tPasConfMsg.SetContentBuf((const u8*)achUncompressXmlMsg, dwUncompressXmlMsgLen);
	LogPrint(LOG_LVL_REPEAT, MID_MAU_NU, "[%s]sip req xml content: \n", __FUNCTION__);
	LongLogPrint(LOG_LVL_REPEAT, MID_MAU_NU, (const s8*)tPasConfMsg.GetContentBuf(), tPasConfMsg.GetContentBufLen());

	DealFunMap::iterator itor = s_deal_sipmtfun_map.find(wEventId);
	if (itor != s_deal_sipmtfun_map.end())
	{
		DealFun dealFun = s_deal_sipmtfun_map[wEventId];
		if (dealFun != NULL)
		{		
			(this->*dealFun)(tPasConfMsg, GetMsgType(wEventId));
		}
	}  
}

void CNUSsnInstance::ProcMtCreateConfReq(CMessage * const pcMsg)
{
	if (pcMsg->content == NULL)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_VC, "[%s]msgEvent: %s content is NULL\n",
			__FUNCTION__, OspEventDesc(pcMsg->event));
		return;
	}
	TPasConfMsg tPasConfMsg((u8*)pcMsg->content, pcMsg->length);
	ProcMtCreateConf(tPasConfMsg, pcMsg->event);
}

void CNUSsnInstance::ProcMtCreateConf(TPasConfMsg& tPasConfMsg, const u16 wMsgEvent)
{
	CPrintFunInOut cPrintFunAssist(__FUNCTION__, MID_MAU_NU);
	if( STATE_NORMAL != CurState() )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_NU, "[%s]Receive Msg<%d> in wrong state<%d>\n",
			__FUNCTION__, wMsgEvent, CurState() );
		return ;
	}
	
	s8  abyTimeId[64] = { 0 };
	TCreateConfReq tCreateConfInfo;
	CBaseMt* pcMt = CMtFactory::GetMt(tPasConfMsg.GetProtocolType(),
		tPasConfMsg.GetProductId(), tPasConfMsg.GetVerisonId());
	if (pcMt == NULL)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]unsupoort mt type\n",
			__FUNCTION__);
		return;
	}
	pcMt->GetCreateConfInfo(tPasConfMsg, tCreateConfInfo, abyTimeId);

	TReqHeadInfo tReqInfo;
	CMauMtInfo cMtInfo;
	CKdvDomainInfo cDomainInfo;
	if (!CheckAndGetMtDomain(tPasConfMsg.GetE164(), cMtInfo, cDomainInfo))
	{
		const u32 dwErrCode = (strlen(cMtInfo.GetDomainMOID()) == 0 ? ERR_MT_NOEXIST : ERR_DOMAIN_INVALID);
		tPasConfMsg.SetErrID(pcMt->TransErrorId(dwErrCode));
		pcMt->ConstructRspMsg(wMsgEvent, "", abyTimeId, tPasConfMsg.GetErrID(), tPasConfMsg);
		const u16 wRspMsgEvent = (strcmp(tPasConfMsg.GetProtocolType(),
			"sip") == 0 ? MAU_NU_TRANSPARENT_CHANNEL_NACK : wMsgEvent + 2);
		SendMsgToNu(wRspMsgEvent, (u8*)&tPasConfMsg, sizeof(tPasConfMsg));
		LogPrint(LOG_LVL_ERROR, MID_MAU_NU, "[%s]get mt or domain or "
			"add req fail,errCode:%u\n", __FUNCTION__, dwErrCode);
		return;
	}

	CompletedReqInfo(tReqInfo, tPasConfMsg, cMtInfo, abyTimeId, wMsgEvent);
	UpdateCreateConfParam(cMtInfo, cDomainInfo, tCreateConfInfo);

	json_t_ptr pjRoot = SetMtCreateConfInfoToJson(cDomainInfo, tReqInfo, tCreateConfInfo);
	if (pjRoot)
	{
		json_str pchJson(pjRoot);

		CMauMsg cServMsg;
		cServMsg.SetMsgBody(&tReqInfo, sizeof(tReqInfo));
		cServMsg.CatMsgBody(pchJson, strlen(pchJson));
		CMauData::SendMsgToMauVC(wMsgEvent, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]mt<E164.%s VerId.%s"
			" ProductId.%s, protocotype: %s>"
			" confname<%s> bitrate<%d> Frm_old<main.%d sec.%d> "
			" Frm<main.%d sec.%d> require create conf, send require<ID:%d> "
			" to mauvc.\n", __FUNCTION__,
			tPasConfMsg.GetE164(), tPasConfMsg.GetVerisonId(),
			tPasConfMsg.GetProductId(), tPasConfMsg.GetProtocolType(),
			tCreateConfInfo.GetConfInfo().GetConfName(),
			tCreateConfInfo.GetConfInfo().GetBitRate(),
			tCreateConfInfo.GetConfInfo().GetCapSupport().GetMainVidFrmRate(),
			tCreateConfInfo.GetConfInfo().GetCapSupport().GetSecVidFrmRate(),
			tCreateConfInfo.GetConfInfo().GetCapSupport().GetMainVidFrmRate(),
			tCreateConfInfo.GetConfInfo().GetCapSupport().GetSecVidFrmRate(),
			tReqInfo.GetUniqueID());
	}
}

void CNUSsnInstance::InitMtVerMap()
{
	RegMtVer(emMtV25, "ts6610.C", "9");
	RegMtVer(emMtV25, "ts5610.C", "9");
	
	RegMtVer(emTTV26, "TTiPhone", "10");
	RegMtVer(emTTV26, "TTAndroidPhone", "10");
	RegMtVer(emTTV26, "ts3210", "10");

	RegMtVer(emMtV26, "HD100", "10");
	RegMtVer(emMtV26, "HD100S", "10");
	RegMtVer(emMtV26, "HD1000", "10");
	RegMtVer(emMtV26, "WD1000", "10");
	RegMtVer(emMtV26, "TrueLink", "10");
}

void CNUSsnInstance::RegMtVer(EmMtVer emMtVer, const s8* pchProductId, const s8* pchVersionId)
{
	s_mapMtVer[make_pair(pchProductId, pchVersionId)] = emMtVer;
}

CNUSsnInstance::EmMtVer CNUSsnInstance::FindMtVer(const s8* pchProductId, const s8* pchVersionId)
{
	MtVerMap::iterator it = s_mapMtVer.find(make_pair(pchProductId, pchVersionId));
	return (it == s_mapMtVer.end()) ? emMtUnknown : it->second;
}

BOOL32 CNUSsnInstance::CheckAndGetMtDomain(const s8* pchMtE164, 
	CMauMtInfo& cMtInfo, CKdvDomainInfo& cDomainInfo)
{
	bitset<5> errFlagBitset;
	if ((errFlagBitset[0] = !g_cMauVCApp.IsMtExist(pchMtE164))
		|| (errFlagBitset[1] = !g_cMauVCApp.GetMtInfo(pchMtE164, cMtInfo))
		|| (errFlagBitset[2] = !g_cMauVCApp.IsDomainExist(cMtInfo.GetDomainMOID()))
		|| (errFlagBitset[3] = !g_cMauVCApp.GetDomainInfo(cMtInfo.GetDomainMOID(), cDomainInfo))
		|| (errFlagBitset[4] = !cDomainInfo.IsUsedFlag()))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_NU, "[%s]Err<%s>, Because:\n"
			"0:mt<%s> not exist\n"
			"1.get mtinfo fail\n"
			"2.domain<%s> not exist\n"
			"3.get domain info fail\n"
			"4.domain is unused\n",
			__FUNCTION__, errFlagBitset.to_string().c_str(),
			pchMtE164, cDomainInfo.GetDomainGUID());
		return FALSE;
	}
	return TRUE;
}

u32 CNUSsnInstance::GetMsgType(const u32 dwSipMsgType)
{
	if (dwSipMsgType == SIP_MT_CREATECONF_REQ)
	{
		return NU_MAU_CREATECONF_REQ;

	}
	else if (dwSipMsgType == SIP_MT_JOINCONF_REQ)
	{
		return NU_MAU_JOINCONF_REQ;
	}
	else if (dwSipMsgType == SIP_MT_GETTEMPLIST_REQ)
	{
		return NU_MAU_GETTEMPLATELIST_REQ;
	}
	else if (dwSipMsgType == SIP_MT_GETCONFLIST_REQ)
	{
		return NU_MAU_GETCONFLIST_REQ;
	}
	else if (dwSipMsgType == SIP_MT_GETBOOKLIST_REQ)
	{
		return NU_MAU_GETBOOKLIST_REQ;
	}
	else if (dwSipMsgType == SIP_MT_GETCONFNUM_REQ)
	{
		return NU_MAU_GETCONFNUM_REQ;
	}
	else if (dwSipMsgType == SIP_MT_GETCONFDETAIL_REQ)
	{
		return NU_MAU_GETCONFDETAIL_REQ;
	}
	return 0;
}

//----mt------------------------
static CSipMt s_cSipMt;
static CH323Mt s_cH323Mt;
static CV25Mt s_cV25Mt;
static CTTV26Mt s_cTTV26Mt;
static CV26Mt s_cV26Mt;
CBaseMt* CMtFactory::GetMt(const std::string& strMtProtocolType,
	const std::string& strProductId,
	const std::string& strVersionId)
{
	if (strMtProtocolType == "sip")
	{
		return &s_cSipMt;
	}
	else
	{
		const CNUSsnInstance::EmMtVer emMtVer = CNUSsnInstance::GetMtVer(strProductId.c_str(), strVersionId.c_str());
		if (emMtVer == CNUSsnInstance::emMtV50)
		{
			return &s_cH323Mt;
		}
		else if (emMtVer == CNUSsnInstance::emMtV25)
		{
			return &s_cV25Mt;
		}
		else if (emMtVer == CNUSsnInstance::emTTV26)
		{
			return &s_cTTV26Mt;
		}
		else if (emMtVer == CNUSsnInstance::emMtV26)
		{
			return &s_cV26Mt;
		}
		else
		{
			return NULL;
		}
	}
}

//base mt
BOOL32 CBaseMt::GetConfList(const u16 wMsgEvent, const s8* pchDomainMOID,
	vector<TSimpleConfInfo>& vecConfList, const s8* pchMtE164,
	const BOOL32 bMtLimit, const BOOL32 bGetAll)
{
	if (wMsgEvent == NU_MAU_GETCONFLIST_REQ || bGetAll)
	{
		vector<TSimpleConfInfo> vecSimpleConfList;
		if (!g_cMauVCApp.GetConfListByDomainAndMt(pchDomainMOID,
			pchMtE164, vecSimpleConfList, bMtLimit))
		{
			return FALSE;
		}
		vecConfList.insert(vecConfList.end(), vecSimpleConfList.begin(),
			vecSimpleConfList.end());
	}
	if (wMsgEvent == NU_MAU_GETBOOKLIST_REQ || bGetAll)
	{
		vector<CRedisBookConf> vecBookConf;
		if (g_cMauVCApp.GetTodayBookConfListByMt(pchDomainMOID, vecBookConf,
			pchMtE164, bMtLimit))
		{
			vector<TSimpleConfInfo> vecSimpleConfList;
			ConvertStruct(vecBookConf, vecSimpleConfList);
			vecConfList.insert(vecConfList.end(), vecSimpleConfList.begin(),
				vecSimpleConfList.end());
		}
		else
		{
			return FALSE;
		}
	}
	if (wMsgEvent == NU_MAU_GETTEMPLATELIST_REQ || bGetAll)
	{
		vector<CTempCacheData> vecTempConfList;
		if(g_cMauVCApp.GetTemplateListByDomain(pchDomainMOID, 
			vecTempConfList, bMtLimit))
		{
			vector<TSimpleConfInfo> vecSimpleConfList;
			ConvertStruct(vecTempConfList, vecSimpleConfList);
			vecConfList.insert(vecConfList.end(), vecSimpleConfList.begin(),
				vecSimpleConfList.end());
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL32 CBaseMt::GetTimeId(TPasConfMsg& tPasConfMsg, s8* pchTimeId)
{
	return CConfListReq::XmlMsgToGkMsg(tPasConfMsg.GetContentBuf(), tPasConfMsg.GetContentBufLen(), pchTimeId);
}

u32 CBaseMt::TransErrorId(u32 dwCmuErrorId)
{
	switch (dwCmuErrorId)
	{
	case ERR_MCU_CONFE164_REPEAT:
	{
		return KDVP_NSM_MCU_CONFE164REPEAT;
	}
	case ERR_MCU_CONFNODISTURB:
	{
		return KDVP_NSM_MCU_PEER_NODISTURB;
	}
	case ERR_MCU_INVALIDPWD:
	{
		return KDVP_NSM_MCU_PASSERR;
	}
	case ERR_CONFNUM_EXCEED:
	{
		return KDVP_NSM_MCU_MAXCONFNUM;
	}
	case ERR_MCU_CONFNUM_EXCEED:
	{
		return KDVP_NSM_MCU_MAXCONFNUM;
	}
	case ERR_MCU_SETENCRYPT:
	{
		return KDVP_NSM_MCU_ENCRYPEERR;
	}
	case ERR_CONF_NOEXIST:
	{
		return KDVP_NSM_MCU_CONFNOTEXIST;
	}
	case ERR_MCU_OVERMAXCONNMT:
	{
		return KDVP_NSM_MCU_MAXMTNUM;
	}
	case  ERR_MCU_CONFOVERFLOW:
	{
		return KDVP_NSM_MCU_NOIDLEVPU;
	}
	case ERR_DB_OPR:
	case ERR_DOMAIN_INVALID:
	case ERR_MT_NOEXIST:
	case ERR_INFO_INVALID:
	case ERR_DB_NO_RESULESET:
	case ERR_NOIDLEFITRES:
	{
		return KDVP_NSM_TIMEOUT_ERR;
	}
	default:
	{
		return dwCmuErrorId;
	}
	}
}

//sipmt
BOOL32 CSipMt::GetCreateConfInfo(TPasConfMsg& tPasConfMsg, TCreateConfReq& tCreateConfInfo,
	s8* pchTimeId)
{
	return CCreateConfReq::XmlMsgToGkMsg(tPasConfMsg.GetContentBuf(),
		tPasConfMsg.GetContentBufLen(), tCreateConfInfo, pchTimeId);
}
BOOL32 CSipMt::GetJoinedConfInfo(TPasConfMsg& tPasConfMsg, TJoinConfReq& tJoinConfReq,
	s8* pchTimeId)
{
	return CJoinConfReq::XmlMsgToGkMsg(tPasConfMsg.GetContentBuf(), tPasConfMsg.GetContentBufLen(),
		tJoinConfReq, pchTimeId);
}
BOOL32 CSipMt::GetConfE164(TPasConfMsg& tPasConfMsg, s8* pchConfE164)
{
	u16 wConfE164Len = MAXLEN_CONFE164 + 1;
	s8  abyTimeId[64] = {0};
	return CConfDetailReq::XmlMsgToGkMsg(tPasConfMsg.GetContentBuf(), tPasConfMsg.GetContentBufLen(),
		pchConfE164, wConfE164Len, abyTimeId);
}
BOOL32 CSipMt::ConstructRspMsg(const u16 wMsgEvent, const s8 *pchConfE164,
	const s8 *pTimeId, u16 wErrId, TPasConfMsg& tPasConfMsg)
{
	tPasConfMsg.SetIsLastMsg(TRUE);
	u8  pbyXmlMsg[LEN_10240] = { 0 };
	u32 dwXmlMsgLen = LEN_10240;
	BOOL32 bSuccess = FALSE;
	if (wMsgEvent == MAU_NU_CREATECONF_ACK
		|| wMsgEvent == MAU_NU_CREATECONF_NACK
		|| wMsgEvent == NU_MAU_CREATECONF_REQ)
	{
		bSuccess = CCreateConfRsp::GkMsgToXmlMsg(CNUSsnInstance::SIP_MT_CREATECONF_RSP,
			pchConfE164, pTimeId, wErrId, pbyXmlMsg, dwXmlMsgLen);
	}
	else if (wMsgEvent == MAU_NU_JOINCONF_ACK
		|| wMsgEvent == MAU_NU_JOINCONF_NACK
		|| wMsgEvent == NU_MAU_JOINCONF_REQ)
	{
		bSuccess = CJoinConfRsp::GkMsgToXmlMsg(CNUSsnInstance::SIP_MT_JOINCONF_RSP,
			pchConfE164, pTimeId, wErrId, pbyXmlMsg, dwXmlMsgLen);
	}
	else if (wMsgEvent == NU_MAU_GETCONFNUM_REQ)
	{
		bSuccess = CConfNumRsp::GkMsgToXmlMsg(CNUSsnInstance::SIP_MT_GETCONFNUM_RSP,
			0, 0, pbyXmlMsg, dwXmlMsgLen );
	}
	else if (wMsgEvent == NU_MAU_GETCONFDETAIL_REQ)
	{
		TConfDetailInfo tConfDetailInfo;
		bSuccess = CConfDetailRsp::GkMsgToXmlMsg(CNUSsnInstance::SIP_MT_GETCONFDETAIL_RSP,
			tConfDetailInfo, pTimeId, 0, pbyXmlMsg, dwXmlMsgLen);
	}
	tPasConfMsg.SetContentBuf(pbyXmlMsg, dwXmlMsgLen);
	return bSuccess;
}
BOOL32 CSipMt::ConstructRspMsg(const u16 wMsgEvent, const TSimpleConfInfo* paConfList,
	const u16 wConfNum, const BOOL32 bLast,
	TPasConfMsg& tPasConfMsg, const s8* pchTimeId)
{
	tPasConfMsg.SetIsLastMsg(bLast);
	tPasConfMsg.SetStructNum(wConfNum);
	u8  abyMsg[LEN_10240] = { 0 };
	u32 dwMsgLen = LEN_10240;
	BOOL32 bSuccess = FALSE;
	if (wMsgEvent == NU_MAU_GETCONFLIST_REQ)
	{
		bSuccess = CGetConfListRsp::GkMsgToXmlMsg(CNUSsnInstance::SIP_MT_GETCONFLIST_RSP, paConfList,
			wConfNum, bLast, pchTimeId, abyMsg, dwMsgLen);
	}
	else if (wMsgEvent == NU_MAU_GETBOOKLIST_REQ)
	{
		bSuccess = CGetBookListRsp::GkMsgToXmlMsg(CNUSsnInstance::SIP_MT_GETBOOKLIST_RSP, paConfList,
			wConfNum, bLast, pchTimeId, abyMsg, dwMsgLen);
	}
	else if (wMsgEvent == NU_MAU_GETTEMPLATELIST_REQ)
	{
		bSuccess = CTemplateConfListRsp::GkMsgToXmlMsg(CNUSsnInstance::SIP_MT_GETTEMPLIST_RSP, paConfList,
			wConfNum, bLast, pchTimeId, abyMsg, dwMsgLen);
	}
	tPasConfMsg.SetContentBuf(abyMsg, dwMsgLen);
	return bSuccess;
}
BOOL32 CSipMt::ConstructRspMsg(TConfDetailInfo& tConfDetailInfo, const u32 dwErrCode,
	TPasConfMsg& tPasConfMsg, const s8* pchTimeID/* = ""*/)
{
	tPasConfMsg.SetIsLastMsg(TRUE);
	tPasConfMsg.SetErrID(TransErrorId(dwErrCode));
	u8  abyPbMsg[LEN_10240] = { 0 };
	u32 dwPbMsgLen = LEN_10240;
	CConfDetailRsp::GkMsgToXmlMsg(CNUSsnInstance::SIP_MT_GETCONFDETAIL_RSP, tConfDetailInfo, pchTimeID,
		tPasConfMsg.GetErrID(), abyPbMsg, dwPbMsgLen);
	tPasConfMsg.SetContentBuf(abyPbMsg, dwPbMsgLen);
	return TRUE;
}
BOOL32 CSipMt::ConstructRspMsg(const u32 dwErrCode, const u16 wTotalConfNum, const u16 wIdleConfNum, TPasConfMsg& tPasConfMsg)
{
	tPasConfMsg.SetIsLastMsg(TRUE);
	tPasConfMsg.SetErrID(TransErrorId(dwErrCode));
	u8  abyPbMsg[LEN_10240] = { 0 };
	u32 dwPbMsgLen = LEN_10240;
	CConfNumRsp::GkMsgToXmlMsg(CNUSsnInstance::SIP_MT_GETCONFNUM_RSP, wTotalConfNum, wIdleConfNum, abyPbMsg, dwPbMsgLen);
	tPasConfMsg.SetContentBuf(abyPbMsg, dwPbMsgLen);
	return TRUE;
}

//h323mt
BOOL32 CH323Mt::GetCreateConfInfo(TPasConfMsg& tPasConfMsg, TCreateConfReq& tCreateConfInfo,
	s8* pchTimeId)
{
	return CCreateConfReq::PbMsgToGkMsg(tPasConfMsg.GetContentBuf(),
		tPasConfMsg.GetContentBufLen(), tCreateConfInfo);
}
BOOL32 CH323Mt::GetJoinedConfInfo(TPasConfMsg& tPasConfMsg, TJoinConfReq& tJoinConfReq,
	s8* pchTimeId)
{
	return CJoinConfReq::PbMsgToGkMsg(tPasConfMsg.GetContentBuf(),
				tPasConfMsg.GetContentBufLen(), tJoinConfReq);
}
BOOL32 CH323Mt::GetConfE164(TPasConfMsg& tPasConfMsg, s8* pchConfE164)
{
	u16 wConfE164Len = MAXLEN_CONFE164 + 1;
	return CConfDetailReq::PbMsgToGkMsg(tPasConfMsg.GetContentBuf(), tPasConfMsg.GetContentBufLen(), pchConfE164, wConfE164Len);
}
BOOL32 CH323Mt::ConstructRspMsg(const u16 wMsgEvent, const s8 *pchConfE164,
	const s8 *pTimeId, u16 wErrId, TPasConfMsg& tPasConfMsg)
{
	u8  pbyXmlMsg[LEN_10240] = { 0 };
	u32 dwXmlMsgLen = LEN_10240;
	BOOL32 bSuccess = TRUE;
	tPasConfMsg.SetIsLastMsg(TRUE);
	if (wMsgEvent == MAU_NU_CREATECONF_ACK)
	{
		bSuccess = CCreateConfRsp::GkMsgToPbMsg(0, pchConfE164, pbyXmlMsg, dwXmlMsgLen);
		tPasConfMsg.SetContentBuf(pbyXmlMsg, dwXmlMsgLen);
	}
	else if (wMsgEvent == MAU_NU_JOINCONF_ACK)
	{
		bSuccess = CJoinConfRsp::GkMsgToPbMsg(0, pchConfE164, pbyXmlMsg, dwXmlMsgLen);
		tPasConfMsg.SetContentBuf(pbyXmlMsg, dwXmlMsgLen);
	}
	else if(wMsgEvent == MAU_NU_CREATECONF_NACK
		|| wMsgEvent == MAU_NU_JOINCONF_NACK)
	{
		tPasConfMsg.SetErrID(wErrId);
	}
	return bSuccess;
}
BOOL32 CH323Mt::ConstructRspMsg(const u16 wMsgEvent,
	const TSimpleConfInfo* paConfList,
	const u16 wConfNum, const BOOL32 bLast,
	TPasConfMsg& tPasConfMsg, const s8* pchTimeId)
{
	tPasConfMsg.SetIsLastMsg(bLast);
	tPasConfMsg.SetStructNum(wConfNum);
	//rnn todo下发预约会议信息加上终端列表
	u8  abyPbMsg[LEN_10240] = { 0 };
	u32 dwPbMsgLen = LEN_10240;
	const BOOL32 bSuccess = CGetBookListRsp::GkMsgToPbMsg(0,
		paConfList, wConfNum, abyPbMsg, dwPbMsgLen);
	tPasConfMsg.SetContentBuf(abyPbMsg, dwPbMsgLen);
	return bSuccess;
}
BOOL32 CH323Mt::ConstructRspMsg(TConfDetailInfo& tConfDetailInfo, const u32 dwErrCode,
	TPasConfMsg& tPasConfMsg, const s8* pchTimeID/* = ""*/)
{
	tPasConfMsg.SetIsLastMsg(TRUE);
	tPasConfMsg.SetErrID(TransErrorId(dwErrCode));
	u8  abyPbMsg[LEN_10240] = { 0 };
	u32 dwPbMsgLen = LEN_10240;
	CConfDetailRsp::GkMsgToPbMsg(0, tConfDetailInfo, abyPbMsg, dwPbMsgLen);
	tPasConfMsg.SetContentBuf(abyPbMsg, dwPbMsgLen);
	return TRUE;
}
BOOL32 CH323Mt::ConstructRspMsg(const u32 dwErrCode, const u16 wTotalConfNum, const u16 wIdleConfNum, TPasConfMsg& tPasConfMsg)
{
	tPasConfMsg.SetIsLastMsg(TRUE);
	tPasConfMsg.SetErrID(TransErrorId(dwErrCode));
	u8  abyPbMsg[LEN_10240] = { 0 };
	u32 dwPbMsgLen = LEN_10240;
	CConfNumRsp::GkMsgToPbMsg(0, wTotalConfNum, wIdleConfNum, abyPbMsg, dwPbMsgLen);
	tPasConfMsg.SetContentBuf(abyPbMsg, dwPbMsgLen);
	return TRUE;
}

//oldmt
BOOL32 COldMt::GetCreateConfInfo(TPasConfMsg& tPasConfMsg,
	TCreateConfReq& tCreateConfInfo, s8* pchTimeId)
{
	TMiniConfInfo tMiniConfInfo;
	if (typeid(*this) == typeid(CV26Mt))
	{
		v26::TCreateConfReq tCreateConfInfo_V26;
		// 兼容老终端转换TCreateConfReq_Old为TCreateConfReq
		tCreateConfInfo_V26 = *(v26::TCreateConfReq*)tPasConfMsg.GetContentBuf();
		ConvertStruct(tCreateConfInfo_V26, tCreateConfInfo);
		tMiniConfInfo = tCreateConfInfo.GetConfInfo();
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]Open Mode: %d, \n", __FUNCTION__, tMiniConfInfo.GetOpenMode());
	}
	else
	{
		v26::TMiniConfInfo tMiniConfInfo_V26;
		tMiniConfInfo_V26 = *(v26::TMiniConfInfo*)tPasConfMsg.GetContentBuf();
		ConvertStruct(tMiniConfInfo_V26, tMiniConfInfo);
		//2.5,ttv26终端不带是否公开会议，默认公开
		tMiniConfInfo.SetPublicConf(1);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]Open Mode: %d, \n", __FUNCTION__,tMiniConfInfo_V26.GetOpenMode());
	}
	//老终端默认支持合并级联
	tMiniConfInfo.SetSupportCascade(TRUE);

	//老终端不带会议视频模式，默认质量优先
	tMiniConfInfo.SetVideoMode(emQUALITY_PRI);

	// 老终端创会不带混音参数，默认租赁开混音，自建不开混音
	// 老终端创会不管其下发VMP参数，默认租赁开VMP，自建不开VMP
	CNUSsnInstance::SetAutoMixAndVmp(tMiniConfInfo);

	// 2.5、2.6终端双流发起模式默认任意终端发起
	tMiniConfInfo.SetDualMode(CONF_DUALMODE_EACHMTATWILL);

	//非公开会议，并且无免打扰字段，设置为免打扰会议
	if (0 == tMiniConfInfo.GetOpenMode() && !tMiniConfInfo.IsCloseModeConf())
	{
		tMiniConfInfo.SetCloseMode(TRUE);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s] Set Conf Is Close Mode\n", __FUNCTION__);
	}

	//老终端召集会议时间为0，则改为4小时
	if (0 == tMiniConfInfo.GetDuration())
	{
		tMiniConfInfo.SetDuration(240);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_NU, "[%s]conf duration is %d."
			" Change to 240 min.!\n", __FUNCTION__,
			tMiniConfInfo.GetDuration());
	}

	//老终端创会不带呼叫次数和呼叫间隔，最新规格定时呼叫，不限次数，呼叫间隔20s
	tMiniConfInfo.SetCallTimes(0);
	tMiniConfInfo.SetCallInterval(20);
	tCreateConfInfo.SetConfInfo(tMiniConfInfo);
	return TRUE;
}
BOOL32 COldMt::GetJoinedConfInfo(TPasConfMsg& tPasConfMsg, TJoinConfReq& tJoinConfReq,
	s8* pchTimeId)
{
	tJoinConfReq = *(TJoinConfReq*)tPasConfMsg.GetContentBuf();
	return TRUE;
}
BOOL32 COldMt::GetConfE164(TPasConfMsg& tPasConfMsg, s8* pchConfE164)
{
	v26::TNuConfE164 tNuConfE164 = *(v26::TNuConfE164*)tPasConfMsg.GetContentBuf();
	memcpy(pchConfE164, tNuConfE164.GetAliasName(), MAXLEN_CONFE164);
	return TRUE;
}
BOOL32 COldMt::ConstructRspMsg(const u16 wMsgEvent, const s8 *pchConfE164,
	const s8 *pTimeId, u16 wErrId, TPasConfMsg& tPasConfMsg)
{
	tPasConfMsg.SetIsLastMsg(TRUE);
	if (wMsgEvent == MAU_NU_CREATECONF_NACK
		|| wMsgEvent == MAU_NU_JOINCONF_NACK)
	{
		tPasConfMsg.SetErrID(wErrId);
	}
	else if (wMsgEvent == MAU_NU_CREATECONF_ACK
		|| wMsgEvent == MAU_NU_JOINCONF_ACK)
	{
		tPasConfMsg.SetContentBuf((const u8*)pchConfE164, strlen(pchConfE164));
	}
	return TRUE;
}
BOOL32 COldMt::ConstructRspMsg(const u16 wMsgEvent, const TSimpleConfInfo* paConfList,
	const u16 wConfNum, const BOOL32 bLast,
	TPasConfMsg& tPasConfMsg, const s8* pchTimeId)
{
	tPasConfMsg.SetStructNum(wConfNum);
	tPasConfMsg.SetIsLastMsg(bLast);
	tPasConfMsg.SetContentBuf(NULL, 0);
	for (u16 wTransLop = 0; wTransLop < wConfNum; wTransLop++)
	{
		if (typeid(*this) == typeid(CV25Mt))
		{
			v26::TConfNameInfo tConfInfo;
			ConvertStruct(paConfList[wTransLop], tConfInfo);
			tPasConfMsg.CatContentBuf((u8*)&tConfInfo,
				sizeof(v26::TConfNameInfo));
		}
		else
		{
			v26::TSimpleConfInfo tConfInfo;
			ConvertStruct(paConfList[wTransLop], tConfInfo);
			tPasConfMsg.CatContentBuf((u8*)&tConfInfo,
				sizeof(v26::TSimpleConfInfo));
		}
	}
	return TRUE;
}
BOOL32 COldMt::ConstructRspMsg(TConfDetailInfo& tConfDetailInfo, const u32 dwErrCode,
	TPasConfMsg& tPasConfMsg, const s8* pchTimeID /*= ""*/)
{
	tPasConfMsg.SetIsLastMsg(TRUE);
	tPasConfMsg.SetErrID(TransErrorId(dwErrCode));
	v26::TConfDetailInfo tConfDetialInfov26;
	ConvertStruct(tConfDetailInfo, tConfDetialInfov26);
	tPasConfMsg.SetContentBuf((u8*)&tConfDetialInfov26, sizeof(tConfDetialInfov26));
	return TRUE;
}
BOOL32 COldMt::ConstructRspMsg(const u32 dwErrCode, const u16 wTotalConfNum, const u16 wIdleConfNum, TPasConfMsg& tPasConfMsg)
{
	tPasConfMsg.SetIsLastMsg(TRUE);
	tPasConfMsg.SetErrID(TransErrorId(dwErrCode));
	TDomainConfNumInfo tConfNumInfo;
	tConfNumInfo.SetTotalConfNum(wTotalConfNum);
	tConfNumInfo.SetIdleConfNum(wIdleConfNum);
	tPasConfMsg.SetContentBuf((u8*)&tConfNumInfo, sizeof(tConfNumInfo));
	return TRUE;
}
u32 COldMt::TransErrorId(u32 dwCmuErrorId)
{
	switch (dwCmuErrorId)
	{
	case ERR_MCU_CONFE164_REPEAT:
	{
		return v26::kdvp_nsm_mcu_confe164repeat;
	}
	case ERR_MCU_CONFNODISTURB:
	{
		return v26::kdvp_nsm_mcu_peer_nodisturb;
	}
	case ERR_MCU_INVALIDPWD:
	{
		return v26::kdvp_nsm_mcu_passerr;
	}
	case ERR_CONFNUM_EXCEED:
	{
		return v26::kdvp_nsm_mcu_maxconfnum;
	}
	case ERR_MCU_CONFNUM_EXCEED:
	{
		return v26::kdvp_nsm_mcu_maxconfnum;
	}
	case ERR_MCU_SETENCRYPT:
	{
		return v26::kdvp_nsm_mcu_encrypeerr;
	}
	case ERR_CONF_NOEXIST:
	{
		return v26::kdvp_nsm_mcu_confnotexist;
	}
	case ERR_MCU_OVERMAXCONNMT:
	{
		return v26::kdvp_nsm_mcu_maxmtnum;
	}
	case  ERR_MCU_CONFOVERFLOW:
	{
		return v26::kdvp_nsm_mcu_noidlevpu;
	}
	case ERR_DB_OPR:
	case ERR_DOMAIN_INVALID:
	case ERR_MT_NOEXIST:
	case ERR_INFO_INVALID:
	case ERR_DB_NO_RESULESET:
	case ERR_NOIDLEFITRES:
	{
		return v26::kdvp_nsm_timeout_err;
	}
	default:
	{
		return dwCmuErrorId;
	}
	}
}
//v25mt
CNUSsnInstance::EmMtVer CV25Mt::GetMtVer() const
{
	return CNUSsnInstance::emMtV25;
}
BOOL32 CV25Mt::GetConfList(const u16 wMsgEvent, const s8* pchDoaminMOID,
	std::vector<TSimpleConfInfo>& vecConfList, const s8* pchMtE164,
	const BOOL32 bMtLimit, const BOOL32 bGetAll)
{
	return CBaseMt::GetConfList(wMsgEvent, pchDoaminMOID,
		vecConfList, pchMtE164, bMtLimit, TRUE);
}

//ttv26mt
CNUSsnInstance::EmMtVer CTTV26Mt::GetMtVer() const
{
	return CNUSsnInstance::emTTV26;
}

//v26mt
CNUSsnInstance::EmMtVer CV26Mt::GetMtVer() const
{
	return CNUSsnInstance::emMtV26;
}