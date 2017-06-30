/*****************************************************************************
ģ����      : MAU
�ļ���      : resmgrssn.cpp
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
#include "resmgrssn.h"
#include "evmau.h"

extern CMauVCApp g_cMauVCApp;

/*=============================================================================
�� �� ���� CResSsnData
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
CResSsnData::CResSsnData()
{
	IZkLogHelp::SetPrintFunc(LogPrint);
	m_pzkClient = IZkApplyClient::Create(RESOURCETYPE_MCU, &ResourceCBFunc, this);
}

/*=============================================================================
�� �� ���� ~CResSsnData
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
//lint -sem(CResSsnData::Quit,cleanup)
CResSsnData::~CResSsnData()
{
	Quit();
}

/*=============================================================================
�� �� ���� ConnectResource
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
BOOL32 CResSsnData::ConnectResource()
{
	s8 achHost[128] = { 0 };
	sprintf(achHost, "%s:%d", g_cMauVCApp.m_achZkIP, g_cMauVCApp.m_wZkPort);

	if (NULL == m_pzkClient)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_RESMGR, "[CResSsnData::ConnectResource] pzkClient is NULL, return FALSE!\n");
		return FALSE;
	}

	if (!m_pzkClient->Connect(achHost))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_RESMGR, "[CResSsnData::ConnectResource] Connect ZK Server err, return FALSE!\n");
		return FALSE;
	}

	return TRUE;
}

/*=============================================================================
�� �� ���� ResourceCBFunc
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
void CResSsnData::ResourceCBFunc(CallbackParam* pParam)
{
	if (NULL == pParam)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_RESMGR, "[CResSsnData::ResourceCBFunc]pParam is NULL, return!\n");
		return;
	}

	switch ((ZkCallbackType)pParam->type)
	{
		case SourceChangeCb:
		{
			s32 nSourceNum = pParam->source_change_param.len;
			//����ֻ֧��256��MCU,Ԥ���������ϱ�CMU��ɵĳ������
			if (MAX_CMU_NUM < nSourceNum)
			{
				nSourceNum = MAX_CMU_NUM;
			}

			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_RESMGR, "[CResSsnData::ResourceCBFunc]Mcu num is %d!\n", nSourceNum);

			CMauMsg cServMsg;
			cServMsg.SetMsgBody(&nSourceNum, sizeof(s32));

			s32 nSourceIdx = 0;
			for (nSourceIdx = 0; nSourceIdx < nSourceNum; nSourceIdx++)
			{
				NodeValue * pNodeValue = pParam->source_change_param.values[nSourceIdx];
				if (NULL == pNodeValue)
				{
					continue;
				}

				TCmuInfo tCmuInfo;
		
				// �ݶ� MCU ֻ�ϱ�zk RoutingKey���˴�ֱ��ȡIdxΪ0��value��������ͨ��pNodeValue->GetCount()������ȡֵ
				u8 byIndex = 0;
				//memcpy( achRoutingKey, pNodeValue->GetValue( byIndex ), sizeof( achRoutingKey ) );

				if (NULL != pNodeValue->GetValue(ZK_KEY_MQK))
				{
					strncpy(tCmuInfo.achRoutingKey, pNodeValue->GetValue(ZK_KEY_MQK), sizeof(tCmuInfo.achRoutingKey) - 1);
				}
				if (NULL != pNodeValue->GetValue(ZK_KEY_MCUIP))
				{
					strncpy(tCmuInfo.achMcuIP, pNodeValue->GetValue(ZK_KEY_MCUIP), sizeof(tCmuInfo.achMcuIP) - 1);
				}
				if (NULL != pNodeValue->GetValue(ZK_KEY_MCUTYPE))
				{
					strncpy(tCmuInfo.achMcuType, pNodeValue->GetValue(ZK_KEY_MCUTYPE), sizeof(tCmuInfo.achMcuType) - 1);
				}
				if (NULL != pNodeValue->GetValue(ZK_KEY_MCUPID))
				{
					strncpy(tCmuInfo.achMcuPid, pNodeValue->GetValue(ZK_KEY_MCUPID), sizeof(tCmuInfo.achMcuPid) - 1);
				}
				if (NULL != pNodeValue->GetValue(ZK_KEY_MAXCONF))
				{
					u32 dwConfNum = 0;
					sscanf(pNodeValue->GetValue(ZK_KEY_MAXCONF), "%u", &dwConfNum);
					tCmuInfo.byMaxConfNum = (u8)dwConfNum;
				}

				LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_RESMGR, "[CResSsnData::ResourceCBFunc]Mcu Idx.%d : KName<%s> McuIp<%s> MucType<%s>\n",
					nSourceIdx, tCmuInfo.achRoutingKey, tCmuInfo.achMcuIP, tCmuInfo.achMcuType);

				cServMsg.CatMsgBody(&tCmuInfo, sizeof(tCmuInfo));
			}

			if (0 != nSourceNum)
			{
				::OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, CInstance::DAEMON), RESMGR_CMUSSN_CMUINFO_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
			}
		}
		break;

	case ConnectCb:
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_RESMGR, "[CResSsnData::ResourceCBFunc]Connect to zk serv, return!\n");
		break;

	case DisconnectCb:
		LogPrint(LOG_LVL_WARNING, MID_MAU_RESMGR, "[CResSsnData::ResourceCBFunc]Disconnect to zk serv, return!\n");
		// ��������
		::OspPost(MAKEIID(AID_MAU_RESMGRSSN_APPID, CInstance::DAEMON), OSP_POWERON);
		break;

	default:
		break;
	}

	return;
}

BOOL32 CResSsnData::IsConnectedZK()
{
	BOOL32 bConnect = FALSE;
	if (m_pzkClient)
	{
		bConnect = m_pzkClient->GetSystemState() == zkConnected ? TRUE : FALSE;
	}
	return bConnect;
}

const s8* CResSsnData::GetZkState()
{
	if (m_pzkClient)
	{
		switch (m_pzkClient->GetSystemState())
		{
		case zkConnected:
			return "Connected";
		case zkDisconnect:
			return "Disconnected";
		case zkConnecting:
			return "Connecting";
		case zkReConnecting:
			return "ReConnecting";
		}
	}
	return "Invalid";
}

void CResSsnData::Quit()
{
	if (m_pzkClient)
	{
		IZkApplyClient::Destory(m_pzkClient);
		m_pzkClient = NULL;
	}
}

/*=============================================================================
�� �� ���� CResMgrSsnInstance
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
CResMgrSsnInstance::CResMgrSsnInstance()
{
}

/*=============================================================================
�� �� ���� ~CResMgrSsnInstance
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
CResMgrSsnInstance::~CResMgrSsnInstance()
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
void CResMgrSsnInstance::DaemonInstanceEntry(CMessage * const pcMsg, CApp *pcApp)
{
	switch (pcMsg->event)
	{
	case OSP_POWERON:
		DaemonProcPowerOn(pcMsg, pcApp);
		break;

	default:
		break;
	}

	return;
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
void CResMgrSsnInstance::DaemonProcPowerOn(const CMessage * pcMsg, CApp *pcApp)
{
	//״̬��ʼ��
	NextState(STATE_IDLE);

	((CResMgrSsnApp*)pcApp)->ConnectResource();
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
void CResMgrSsnInstance::InstanceEntry(CMessage * const pcMsg)
{
}
