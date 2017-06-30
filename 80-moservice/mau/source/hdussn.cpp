/*****************************************************************************
ģ����      : MAU
�ļ���      : hdussn.cpp
����ʱ��    : 2015�� 01�� 26��
ʵ�ֹ���    : 
����        : ����
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2015/01/26  5.0         ����          ����
******************************************************************************/
#include "inc.h"
#include "evmau.h"
#include "maustruct.h"
#include "hdussn.h"
#include "mauvc.h"

using namespace MediaAPI;

/*=============================================================================
�� �� ���� CHduPoolSsnInstance
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
=============================================================================*/
CHduPoolSsnInstance::CHduPoolSsnInstance()
{
	m_dwHduPoolNode = INVALID_NODE;
	m_dwHduPoolIId = 0;
	m_dwIp = 0;
	m_wPort = HDURESPOOL_LISTEN_PORT;
}

/*=============================================================================
�� �� ���� ~CMpcdSsnInstance
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
=============================================================================*/
CHduPoolSsnInstance::~CHduPoolSsnInstance()
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
2015/01/26  5.0         ����          ����
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
�� �� ���� DaemonProcPowerOn
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
=============================================================================*/
void CHduPoolSsnInstance::DaemonProcPowerOn( CMessage * const pcMsg )
{
	NextState( STATE_IDLE );
}

/*=============================================================================
�� �� ���� DaemonProcDisconnect
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
=============================================================================*/
void CHduPoolSsnInstance::DaemonProcDisconnect( CMessage * const pcMsg )
{
	u32 dwNode = *(u32*)pcMsg->content;
	if( INVALID_NODE == m_dwHduPoolNode || dwNode != m_dwHduPoolNode )
	{
		return ;
	}

	//�����������屨������hdu��Ϣ��
	CMauData::SendMsgToMauVC(HDUPOOL_MAU_DISCONNECT_NTF, NULL, 0);

	NextState( STATE_IDLE );

	m_dwHduPoolNode = INVALID_NODE;
	m_dwHduPoolIId = 0;

	//��������
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
�� �� ���� DaemonProcConnectHduPool
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
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
�� �� ���� DaemonProcRegHduPool
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
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
�� �� ���� DaemonProcHduPoolRegRsp
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
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
			
			// ע��ɹ�������hdu��Ϣ
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
�� �� ���� ConnectHduPool
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u32 & dwHduPoolNode
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
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
�� �� ���� RegistHduPool
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
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
�� �� ���� DaemonProcHduInfoNtf
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
=============================================================================*/
void CHduPoolSsnInstance::DaemonProcHduInfoNtf( CMessage * const pcMsg )
{
    if ( STATE_NORMAL != CurState() )
    {
        LogPrint( LOG_LVL_ERROR, MID_MAU_HDU, "[DaemonProcHduInfoNtf]MPCD receive msg.%d<%s> at wrong state(%d)!\n", pcMsg->event, OspEventDesc(pcMsg->event), CurState() );
        return;
    }
					
    LogPrint( LOG_LVL_DETAIL, MID_MAU_HDU, "[DaemonProcHduInfoNtf]receive Hduinfo successful!\n" );
			
	// ת��mauvc�߳�
	CHduMsg cHduMsg( pcMsg->content, pcMsg->length );
//	cHduMsg.SetSrcSsnId( (u8)GetInsID() );
			
	CMauData::SendMsgToMauVC(pcMsg->event, cHduMsg.GetServMsg(), cHduMsg.GetServMsgLen());

    return;
}

/*=============================================================================
�� �� ���� DaemonProcHduInfoReqRsp
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
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
�� �� ���� DaemonProcSetHduChnVolume
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� CMessage * const pcMsg
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/01/26  5.0         ����          ����
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
	//ת��Ϊhdu��֪�ṹ
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

	//��������ͨ�����������hdupool
	CHduMsg cHduMsg;
	cHduMsg.SetMsgBody( (u8*)&tHduVolInfo, sizeof(THduVolInfo) );	
	OspPost( MAKEIID( APPID_HDUPOOL_ACSSSN, CInstance::DAEMON ), ACS_HDUPOOL_CHANGEHDUVOLUME_CMD, cHduMsg.GetServMsg(), cHduMsg.GetServMsgLen(), m_dwHduPoolNode );
	
    return;
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
2015/01/26  5.0         ����          ����
=============================================================================*/
void CHduPoolSsnInstance::InstanceEntry( CMessage * const pcMsg )
{

}
