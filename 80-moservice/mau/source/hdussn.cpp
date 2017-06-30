/*****************************************************************************
模块名      : MAU
文件名      : hdussn.cpp
创建时间    : 2015年 01月 26日
实现功能    : 
作者        : 阮楠楠
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/01/26  5.0         阮楠楠          创建
******************************************************************************/
#include "inc.h"
#include "evmau.h"
#include "maustruct.h"
#include "hdussn.h"
#include "mauvc.h"

using namespace MediaAPI;

/*=============================================================================
函 数 名： CHduPoolSsnInstance
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
CHduPoolSsnInstance::CHduPoolSsnInstance()
{
	m_dwHduPoolNode = INVALID_NODE;
	m_dwHduPoolIId = 0;
	m_dwIp = 0;
	m_wPort = HDURESPOOL_LISTEN_PORT;
}

/*=============================================================================
函 数 名： ~CMpcdSsnInstance
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
CHduPoolSsnInstance::~CHduPoolSsnInstance()
{

}

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
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
void CHduPoolSsnInstance::DaemonInstanceEntry( CMessage * const pcMsg , CApp * pcApp )
{
	if( NULL == pcMsg || NULL == pcApp )
	{
		return ;
	}

	switch( pcMsg->event )
	{
	case OSP_POWERON:
		DaemonProcPowerOn( pcMsg );
		break;
	case OSP_DISCONNECT:
		DaemonProcDisconnect( pcMsg );
		break;
	case MAU_HDUPOOL_CONNECT_CMD:
		DaemonProcConnectHduPool(pcMsg);
		break;
	case MAU_HDUPOOL_CONNECT_TIMER:
		DaemonProcConnectTimer( pcMsg );
		break;
	case MAU_HDUPOOL_REGISTER_TIMER:
		DaemonProcRegHduPool( pcMsg );
		break;
	case HDUPOOL_ACS_CONN_ACK:
	case HDUPOOL_ACS_CONN_NACK:
		DaemonProcHduPoolRegRsp( pcMsg );
		break;

	case HDUPOOL_ACS_HDUSTATUS_NOTIF:
	case HDUPOOL_ACS_CHANGEHDUVOLUME_NOTIF:
	case HDUPOOL_ACS_HDUONLINE_NOTIF:
	case HDUPOOL_ACS_HDUOFFLINE_NOTIF:
	case HDUPOOL_ACS_HDUCHANNEL_STATUS_NOTIF:
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_HDU, "(hdupool->acsssn-->*hdussn->mau){%s}\n",
															OspEventDesc(pcMsg->event));
		DaemonProcHduInfoNtf( pcMsg );
		break;

	case HDUPOOL_ACS_GETSTATUS_ACK:
	case HDUPOOL_ACS_GETSTATUS_NACK:
		DaemonProcHduInfoReqRsp( pcMsg );
		break;

 	case CM_MAU_SETHDUVOLUME_REQ:
		DaemonProcSetHduChnVolume( pcMsg );
 		break;

	default:
		break;
	}

	return ;
}

/*=============================================================================
函 数 名： DaemonProcPowerOn
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
void CHduPoolSsnInstance::DaemonProcPowerOn( CMessage * const pcMsg )
{
	NextState( STATE_IDLE );
}

/*=============================================================================
函 数 名： DaemonProcDisconnect
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
void CHduPoolSsnInstance::DaemonProcDisconnect( CMessage * const pcMsg )
{
	u32 dwNode = *(u32*)pcMsg->content;
	if( INVALID_NODE == m_dwHduPoolNode || dwNode != m_dwHduPoolNode )
	{
		return ;
	}

	//断链后首先清报过来的hdu信息表
	CMauData::SendMsgToMauVC(HDUPOOL_MAU_DISCONNECT_NTF, NULL, 0);

	NextState( STATE_IDLE );

	m_dwHduPoolNode = INVALID_NODE;
	m_dwHduPoolIId = 0;

	//重新连接
	DaemonProcConnectTimer( pcMsg );
    
	return ;
}

void CHduPoolSsnInstance::DaemonProcConnectHduPool(CMessage * const pcMsg)
{
	switch (CurState())
	{
		case STATE_IDLE:
		{
			const THduPoolInfo* ptHduInfo = (const THduPoolInfo*)pcMsg->content;
			m_dwIp = ptHduInfo->m_dwIp;
			m_wPort = ptHduInfo->m_wPort;

			KillTimer(MAU_HDUPOOL_CONNECT_TIMER);

			if (0 == m_dwIp)
			{
				LogPrint(LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcConnectHduPool] HduPool IpAddr is not correct , do not connect it.\n");
				return;
			}

			BOOL32 bRet = FALSE;
			bRet = ConnectHduPool();
			if (bRet)
			{
				SetTimer(MAU_HDUPOOL_REGISTER_TIMER, REGISTHDUPOOL_TIMEOUT);
			}
			else
			{
				SetTimer(MAU_HDUPOOL_CONNECT_TIMER, CONNECTHDUPOOL_TIMEOUT);
			}
		}
		break;
	default:
		LogPrint(LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcConnectHduPool]: EV_MPCD_HDUPOOL_CONNECT_TIMER received in wrong state %u!\n", CurState());
		break;
	}
}

/*=============================================================================
函 数 名： DaemonProcConnectHduPool
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
void CHduPoolSsnInstance::DaemonProcConnectTimer( CMessage * const pcMsg )
{
	switch( CurState() )
	{
	case STATE_IDLE:
		{
			KillTimer( MAU_HDUPOOL_CONNECT_TIMER );

			if( 0 == m_dwIp)
			{
				LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcConnectHduPool] HduPool IpAddr is not correct , do not connect it.\n" );
				return;
			}

			BOOL32 bRet = FALSE;
			bRet = ConnectHduPool();
			if( bRet )
			{ 
				SetTimer(MAU_HDUPOOL_REGISTER_TIMER, REGISTHDUPOOL_TIMEOUT); 
			}
			else
			{
				SetTimer(MAU_HDUPOOL_CONNECT_TIMER, CONNECTHDUPOOL_TIMEOUT);
			}
		}
		break;
	default:
		LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcConnectHduPool]: EV_MPCD_HDUPOOL_CONNECT_TIMER received in wrong state %u!\n", CurState());
		break;
	}
}

/*=============================================================================
函 数 名： DaemonProcRegHduPool
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
void CHduPoolSsnInstance::DaemonProcRegHduPool( CMessage * const pcMsg )
{
	KillTimer( MAU_HDUPOOL_REGISTER_TIMER );

    switch( CurState() )
    {
	case STATE_IDLE:
		{
			RegistHduPool();
			SetTimer(MAU_HDUPOOL_REGISTER_TIMER, REGISTHDUPOOL_TIMEOUT);
		}
        break;
    default:
        LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcRegHduPool]EV_MPCD_HDUPOOL_REGISTER_TIMER received in wrong state %u!\n", CurState());
        break;
    }
    
    return;
}

/*=============================================================================
函 数 名： DaemonProcHduPoolRegRsp
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
void CHduPoolSsnInstance::DaemonProcHduPoolRegRsp( CMessage * const pcMsg )
{
	
    if ( STATE_IDLE != CurState() )
    {
        LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcHduPoolRegRsp]MPCD receive msg.%d<%s> at wrong state(%d)!\n", pcMsg->event, OspEventDesc(pcMsg->event), CurState() );
        return;
    }
	
    switch( pcMsg->event )
    {
    case HDUPOOL_ACS_CONN_ACK:
        {
            NEXTSTATE( STATE_NORMAL );
            
            KillTimer( MAU_HDUPOOL_REGISTER_TIMER );
			
            m_dwHduPoolNode = pcMsg->srcnode;
            m_dwHduPoolIId = pcMsg->srcid;

			in_addr in = { m_dwIp };
            LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcHduPoolRegRsp]Reg HduPool<%s> successful!\n", inet_ntoa(in));
			
			// 注册成功后请求hdu信息
			OspPost( MAKEIID( APPID_HDUPOOL_ACSSSN, CInstance::DAEMON ), ACS_HDUPOOL_GETSTATUS_REQ, NULL, 0, m_dwHduPoolNode );
            LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_HDU, "(*hdussn-->acsssn->hduppol){%s}\n", OspEventDesc(ACS_HDUPOOL_GETSTATUS_REQ));
		}
        break;
    case HDUPOOL_ACS_CONN_NACK:
		{
			in_addr in = { m_dwIp };
			LogPrint(LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcHduPoolRegRsp]Reg HduPool<%s> failed!\n", inet_ntoa(in));
		}
		break;
    default:
        LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcHduPoolRegRsp]receive wrong msg.%d<%s>!\n", pcMsg->event, OspEventDesc(pcMsg->event) );
        break;
    }
	
    return;
}

/*=============================================================================
函 数 名： ConnectHduPool
功    能： 
算法实现： 
全局变量： 
参    数： u32 & dwHduPoolNode
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
BOOL32 CHduPoolSsnInstance::ConnectHduPool()
{
    BOOL32 bRet = TRUE;
    
    if( !OspIsValidTcpNode(m_dwHduPoolNode))
    {
		m_dwHduPoolNode = OspConnectTcpNode(m_dwIp, m_wPort, 15, 4);
		in_addr in = { m_dwIp };
		LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[CHduPoolSsnInstance::ConnectHduPool]HduPool ip<%s> port<%d>!\n", inet_ntoa(in), m_wPort);
        
        if (OspIsValidTcpNode(m_dwHduPoolNode))
        {
            ::OspNodeDiscCBRegQ(m_dwHduPoolNode, GetAppID(), GetInsID());
        }
        else
        {
            bRet = FALSE;
        }
    }
    return bRet;
}


/*=============================================================================
函 数 名： RegistHduPool
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
void CHduPoolSsnInstance::RegistHduPool()
{
	CHduMsg cHduMsg;

    int nRet = post( MAKEIID( APPID_HDUPOOL_ACSSSN, CInstance::DAEMON ), ACS_HDUPOOL_CONN_REQ,
        cHduMsg.GetServMsg(), cHduMsg.GetServMsgLen(), m_dwHduPoolNode );
    
    if ( OSP_ERROR == nRet ) 
    {
        LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[RegistHduPool]Send Message ACS_HDUPOOL_CONN_REQ failed.\n" );
    }
    else
    {
        LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_HDU, "[RegistHduPool]Send Message MPCD_HDUPOOL_REGISTER_REQ to HduPool node.%u \n", m_dwHduPoolNode);
    }
    
    return;
}

/*=============================================================================
函 数 名： DaemonProcHduInfoNtf
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
void CHduPoolSsnInstance::DaemonProcHduInfoNtf( CMessage * const pcMsg )
{
    if ( STATE_NORMAL != CurState() )
    {
        LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcHduInfoNtf]MPCD receive msg.%d<%s> at wrong state(%d)!\n", pcMsg->event, OspEventDesc(pcMsg->event), CurState() );
        return;
    }
					
    LogPrint( LOG_LVL_DETAIL, MID_MAU_HDU, "[DaemonProcHduInfoNtf]receive Hduinfo successful!\n" );
			
	// 转给mauvc线程
	CHduMsg cHduMsg( pcMsg->content, pcMsg->length );
//	cHduMsg.SetSrcSsnId( (u8)GetInsID() );
			
	CMauData::SendMsgToMauVC(pcMsg->event, cHduMsg.GetServMsg(), cHduMsg.GetServMsgLen());

    return;
}

/*=============================================================================
函 数 名： DaemonProcHduInfoReqRsp
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
void CHduPoolSsnInstance::DaemonProcHduInfoReqRsp( CMessage * const pcMsg )
{
	
    if ( STATE_NORMAL != CurState() )
    {
        LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcHduInfoReqRsp]MPCD receive msg.%d<%s> at wrong state(%d)!\n", pcMsg->event, OspEventDesc(pcMsg->event), CurState() );
        return;
    }
	
    switch( pcMsg->event )
    {
    case HDUPOOL_ACS_GETSTATUS_ACK:
        {				
            LogPrint( LOG_LVL_DETAIL, MID_MAU_HDU, "[DaemonProcHduInfoReqRsp]get Hduinfo successful!\n" );
		}
        break;
    case HDUPOOL_ACS_GETSTATUS_NACK:
        LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcHduInfoReqRsp]get Hduinfo failed!\n" );
		break;
		
    default:
        LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcHduInfoReqRsp]receive wrong msg.%d<%s>!\n", pcMsg->event, OspEventDesc(pcMsg->event) );
        break;
    }
	
    return;
}

/*=============================================================================
函 数 名： DaemonProcSetHduChnVolume
功    能： 
算法实现： 
全局变量： 
参    数： CMessage * const pcMsg
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
void CHduPoolSsnInstance::DaemonProcSetHduChnVolume( CMessage * const pcMsg )
{
	
    if ( STATE_NORMAL != CurState() )
    {
        LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcSetHduChnVolume]MPCD receive msg.%d<%s> at wrong state(%d)!\n", pcMsg->event, OspEventDesc(pcMsg->event), CurState() );
        return;
    }

	CMauMsg cServMsg( pcMsg->content, pcMsg->length );
	u8 * pbyMsg = cServMsg.GetMsgBody();
	pbyMsg += sizeof(TReqHeadInfo);
	u8 byHduId = *(u8*)pbyMsg;
	pbyMsg += sizeof( byHduId );
	u8 byHduChnId = *(u8*)pbyMsg;
	pbyMsg += sizeof( byHduChnId );
	u8 bySilence = *(u8*)pbyMsg;
	pbyMsg += sizeof( bySilence );
	u8 byHduVolum = *(u8*)pbyMsg;
// 	pbyMsg += sizeof( byHduVolum );

	BOOL32 bIsMute = FALSE;
	//转换为hdu感知结构
	THduVolInfo tHduVolInfo;
	tHduVolInfo.SetEqpId( byHduId );
	tHduVolInfo.SetChnlIdx( byHduChnId );
	bIsMute = ( bySilence == 1 ) ? TRUE : FALSE;
	tHduVolInfo.SetIsMute( bIsMute );
	if ( bIsMute )
	{
		byHduVolum = 0;
	}
	tHduVolInfo.SetVolume( byHduVolum );

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_HDU, "[DaemonProcSetHduChnVolume]send hdu.%d chn.%d volume.%d to hdupool.\n",
		tHduVolInfo.GetEqpId(), tHduVolInfo.GetChnlIdx(), tHduVolInfo.GetVolume() );

	//发送设置通道音量命令给hdupool
	CHduMsg cHduMsg;
	cHduMsg.SetMsgBody( (u8*)&tHduVolInfo, sizeof(THduVolInfo) );	
	OspPost( MAKEIID( APPID_HDUPOOL_ACSSSN, CInstance::DAEMON ), ACS_HDUPOOL_CHANGEHDUVOLUME_CMD, cHduMsg.GetServMsg(), cHduMsg.GetServMsgLen(), m_dwHduPoolNode );
	
    return;
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
2015/01/26  5.0         阮楠楠          创建
=============================================================================*/
void CHduPoolSsnInstance::InstanceEntry( CMessage * const pcMsg )
{

}
