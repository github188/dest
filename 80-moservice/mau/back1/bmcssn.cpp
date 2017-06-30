/*****************************************************************************
ģ����      : MAU
�ļ���      : bmcssn.cpp
����ʱ��    : 2014�� 08�� 26��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
******************************************************************************/
#include "inc.h"
#include "bmcssn.h"
#include "evmau.h"
#include "mauvc.h"

extern CMauVCApp g_cMauVCApp;

#define REG_DEALFUN(event, func) deal_fun_map[event] = func;

s32 CBmcSsnData::s_nBmcRabbitMq = 0;

/*=============================================================================
�� �� ���� Init_MQPC
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
BOOL32 CBmcSsnData::Init_MQPC()
{
	// ����������
	int nRet = SafeCreateC( MQ_BMC_NTF_Q, MQ_BMC_NTF_EX, NULL, Content_MQCB, NULL, NULL, ConsumerSuccCB, ConsumerFailCB, 0 );

	return nRet;
}

/*=============================================================================
�� �� ���� Content_MQCB
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
void CBmcSsnData::Content_MQCB( amqp_connection_state_t conn, void *pbuf, size_t len, 
	response_type *prsp_type , char* pchTag)
{
	if ( NULL == pbuf )
	{
		*prsp_type = RT_NACK;
		return;
	}

	*prsp_type = RT_ACK;
	//�յ���ֱ��post��ȥ����
	OspPost( MAKEIID( AID_MAU_BMCSSN_APPID, CInstance::DAEMON ), BMC_MESSAGE_CB, pbuf, len );            
    return;
}

/*=============================================================================
�� �� ���� CBmcSsnInstance
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
CBmcSsnInstance::CBmcSsnInstance()
{

}


/*=============================================================================
�� �� ���� ~CBmcSsnInstance
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
CBmcSsnInstance::~CBmcSsnInstance()
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
void CBmcSsnInstance::DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp )
{
	if ( NULL ==  pcMsg )
	{
		return;
	}
	
	switch ( pcMsg->event )
	{
	case OSP_POWERON:
		DaemonProcPowerOn(pcMsg, pcApp);
		break;
	case BMC_MESSAGE_CB:
		DaemonProcBmcMsgCB(pcMsg);
		break;
	default:
		break;
	}
	
	
	return;
}

void CBmcSsnInstance::DaemonProcBmcMsgCB( CMessage * const pcMsg )
{
	s8* achJsonBuf = (s8*)pcMsg->content;
// 	u16 wMsgType = 0;

	//����jsonת����Ӧ������   lbg 2015/6
	s8 achJsonBufEventId[MAXLEN_MSGTYPE+1] = {0};
	s8 achJsonBufEventData[MAXLEN_MAU_MSG +1] = {0};
	GetEventType(achJsonBufEventId, achJsonBuf);
	GetEventData(achJsonBufEventData, achJsonBuf);
	DealFunMap::iterator itor = deal_fun_map.find(achJsonBufEventId);
	if (itor != deal_fun_map.end())
	{
		(*itor->second)(achJsonBufEventData);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcSsnData::Content_MQCB]eventid is %s. eventcontent is:\n", achJsonBufEventId);
		LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, achJsonBufEventData, strlen(achJsonBufEventData));
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "\n[CBmcSsnData::Content_MQCB]eventcontent end!\n");
	}       
	else if (strcmp(achJsonBufEventId, UPDATE_SYSTEM_MODE) == 0)
	{
		DealUpdateSystemMode(achJsonBuf);
	}
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
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
void CBmcSsnInstance::DaemonProcPowerOn(CMessage * const pcMsg, CApp *pcApp)
{
	//״̬��ʼ��
	NextState( STATE_IDLE );
	dynamic_cast<CBmcSsnApp*>(pcApp)->Init_MQPC();
	RegDealFun();
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DaemonProcPowerOn]RegDealFun.\n" );
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
2014/08/26  5.0         ��Ҳ                    ����
=============================================================================*/
void CBmcSsnInstance::InstanceEntry( CMessage * const pcMsg )
{
	return;
}

void CBmcSsnInstance::RegDealFun()
{
	REG_DEALFUN(CREATE_ACCOUNT, DealCreateAccount);
	REG_DEALFUN(EDIT_ACCOUNT, DealEditAccount);
	REG_DEALFUN(FORBID_ACCOUNT, DealForbidAccount);
	REG_DEALFUN(ENABLE_ACCOUNT, DealEnableAccount);
	REG_DEALFUN(CREATE_SERVER_DOMAIN, DealCreateServerDomain);
	REG_DEALFUN(EDIT_SERVER_DOMAIN, DealEditServerDomain);
	REG_DEALFUN(FORBID_SERVER_DOMAIN, DealForbidServerDomain);
	REG_DEALFUN(ENABLE_SERVER_DOMAIN, DealEnableServerDomain);
	REG_DEALFUN(DELETE_SERVER_DOMAIN, DealDeleteServerDomain);
	REG_DEALFUN(CREATE_USER_DOMAIN, DealCreateUserDomain);
	REG_DEALFUN(EDIT_USER_DOMAIN, DealEditUserDomain);
	REG_DEALFUN(FORBID_USER_DOMAIN, DealForbidUserDomain);
	REG_DEALFUN(ENABLE_USER_DOMAIN, DealEnableUserDomain);
	REG_DEALFUN(DELETE_USER_DOMAIN, DealDeleteUserDomain);
	REG_DEALFUN(CREATE_PLATFORM_DOMAIN, DealCreatePlatformDomain);
	REG_DEALFUN(EDIT_PLATFORM_DOMAIN, DealEditPlatformDomain);
	REG_DEALFUN(DELETE_PLATFORM_DOMAIN, DealDeletePlatformDomain);
	REG_DEALFUN(CREATE_SERVER, DealCreateServer);
	REG_DEALFUN(EDIT_SERVER, DealEditServer);
	REG_DEALFUN(ENABLE_SERVER, DealEnableServer);
	REG_DEALFUN(FORBID_SERVER, DealForbidServer);
	REG_DEALFUN(DELETE_SERVER, DealDeleteServer);
	REG_DEALFUN(BATCH_CREATE_ACCOUNT, DealBatchCreateAccount);
}

void CBmcSsnInstance::DealCreateServerDomain( void *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateServerDomain]pBuf is Null.\n" );
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	memcpy( achJsonBuf, pBuf, MAXLEN_JSONBUF );
	s8 achMoid[OPERATION_MAXLEN_MOID+1] = {0};
// 	u32 dwMoidLen = MAXLEN_MSGTYPE;

	CKdvDomainInfo tDomainInfo;
	s8	achDomainName[OPERATION_MAXLEN_DOMAINNAME+1]  = {0};
	s8	achDomainGUID[OPERATION_MAXLEN_GUID+1]  = {0};
	s8  achDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};

	BOOL32 bRet = FALSE;
	//moid
	bRet = GetMOIDFromJson(achJsonBuf, achDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateServerDomain]Get moid is failure.\n" );
	}

	//name
	bRet = GetNameFromJson(achJsonBuf, achDomainName, OPERATION_MAXLEN_DOMAINNAME);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateServerDomain]Get name is failure.\n" );
	}

	//guid
	bRet = GetGuidFromJson(achJsonBuf, achDomainGUID, OPERATION_MAXLEN_GUID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateServerDomain]Get guid is failure.\n" );
	}

	tDomainInfo.SetWholeName( achDomainName );
	tDomainInfo.SetDomainGUID( achDomainGUID );
	tDomainInfo.SetUsedFlag( TRUE ) ;
	tDomainInfo.SetDomainLevel( CKdvDomainInfo::em_KdvTopLevelDomain );
	tDomainInfo.SetDomainMOID( achDomainMOID );

	g_cMauVCApp.AddDomainInfo( tDomainInfo );

	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetServDomainInfo] Domain<%s>[%s], MOID[%s]\n", 
		tDomainInfo.GetWholeName(), tDomainInfo.GetDomainGUID(), tDomainInfo.GetDomainMOID() );
}

void CBmcSsnInstance::DealEditServerDomain( void *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateServerDomain]pBuf is Null.\n" );
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	memcpy( achJsonBuf, pBuf, MAXLEN_JSONBUF );
	s8 achMoid[OPERATION_MAXLEN_MOID+1] = {0};
// 	u32 dwMoidLen = MAXLEN_MSGTYPE;
	CKdvDomainInfo tNewDomainInfo;
	s8	achDomainName[OPERATION_MAXLEN_DOMAINNAME+1]  = {0};
	s8	achDomainGUID[OPERATION_MAXLEN_GUID+1]  = {0};
	s8  achDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};

	BOOL32 bRet = FALSE;
	//moid
	bRet = GetMOIDFromJson(achJsonBuf, achDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateServerDomain]Get moid is failure.\n" );
	}

	//name
	bRet = GetNameFromJson(achJsonBuf, achDomainName, OPERATION_MAXLEN_DOMAINNAME);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateServerDomain]Get name is failure.\n" );
	}

	//guid
	bRet = GetGuidFromJson(achJsonBuf, achDomainGUID, OPERATION_MAXLEN_GUID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateServerDomain]Get guid is failure.\n" );
	}

	//Ϊ���޸�/����/����ʹ��ͬһ���������˴�ȥjson��ȡ�Ƿ����õ��ֶ�
	BOOL32 bEnabled = FALSE;
	bRet = GetEnabledFromJson(achJsonBuf, bEnabled);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateServerDomain]Get enabled is failure.\n" );
	}

	tNewDomainInfo.SetWholeName( achDomainName );
	tNewDomainInfo.SetDomainGUID( achDomainGUID );
	tNewDomainInfo.SetUsedFlag( bEnabled ) ;
	tNewDomainInfo.SetDomainLevel( CKdvDomainInfo::em_KdvTopLevelDomain );
	tNewDomainInfo.SetDomainMOID( achDomainMOID );

	g_cMauVCApp.ModDomainInfo( tNewDomainInfo );
	
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[CAPInfoDBOpr::ProcGetServDomainInfo] Domain<%s>[%s], MOID[%s]\n", 
		tNewDomainInfo.GetWholeName(), tNewDomainInfo.GetDomainGUID(), tNewDomainInfo.GetDomainMOID() );
}

void CBmcSsnInstance::DealForbidServerDomain( void *pBuf )
{
	//����ͬedit
	DealEditServerDomain(pBuf);
}

void CBmcSsnInstance::DealEnableServerDomain( void *pBuf )
{
	//����ͬedit
	DealEditServerDomain(pBuf);
}

void CBmcSsnInstance::GetEventType( s8 *pstrEvent, s8 *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMCMU, "[ConvertMsgTypeToEventId]Param is NULL, return!\n" );
		return;
	}

	u32 dwMsgTypeLen = MAXLEN_MSGTYPE;
	
	if ( !GetMsgEventFromJson( pBuf, pstrEvent, dwMsgTypeLen ) )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMCMU, "[ConvertMsgTypeToEventId]GetMsgTypeFromJson err, return!\n" );
		return;
	}
}

void CBmcSsnInstance::GetEventData( s8 *pBufData, s8 *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMCMU, "[ConvertMsgTypeToEventId]Param is NULL, return!\n" );
		return;
	}

	if ( !GetMsgDataFromJson( pBuf, pBufData ) )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_CMCMU, "[ConvertMsgTypeToEventId]GetMsgTypeFromJson err, return!\n" );
		return;
	}
}

void CBmcSsnInstance::DealCreateUserDomain( void *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateUserDomain]pBuf is Null.\n" );
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	memcpy( achJsonBuf, pBuf, MAXLEN_JSONBUF );
	s8 achMoid[OPERATION_MAXLEN_MOID+1] = {0};
// 	u32 dwMoidLen = MAXLEN_MSGTYPE;

	CKdvDomainInfo tDomainInfo;
	s8 achDomainName[OPERATION_MAXLEN_DOMAINNAME+1]   = {0};
	s8 achDomainGUID[OPERATION_MAXLEN_GUID+1]   = {0};
	s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1]  = {0};
	s8 achSDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};

	BOOL32 bRet = FALSE;
	//moid
	bRet = GetMOIDFromJson(achJsonBuf, achDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateUserDomain]Get moid is failure.\n" );
	}

	//name
	bRet = GetNameFromJson(achJsonBuf, achDomainName, OPERATION_MAXLEN_DOMAINNAME);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateUserDomain]Get name is failure.\n" );
	}

	//guid
	bRet = GetGuidFromJson(achJsonBuf, achDomainGUID, OPERATION_MAXLEN_GUID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateUserDomain]Get guid is failure.\n" );
	}

	//platformDomainMoid
	bRet = GetPlatformMoidFromJson(achJsonBuf, achSDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateUserDomain]Get platformmoid is failure.\n" );
	}

	CPlatformConfNumLimit cUserDomainConfNumLimit;
	if (!GetConfNumLimitFromJson(achJsonBuf, cUserDomainConfNumLimit, FALSE))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s]GetConfNumLimitFromJson fail\n", __FUNCTION__);
		return;
	}
	if (!g_cMauVCApp.SetMaxDomainConfNum(achDomainMOID,
		cUserDomainConfNumLimit.GetSmallConfNumLimit(), cUserDomainConfNumLimit.GetLargeConfNumLimit()))
	{
		g_cMauVCApp.m_cApInfoDBOpr.GetUserDomainInfo();	
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s]SetMaxDomainConfNum fail\n", __FUNCTION__);
	}

	BOOL32 bEnableH265 = FALSE;
	if (!GetEnableH265FromJson(achJsonBuf, bEnableH265))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[%s]GetEnableH265FromJson fail\n", __FUNCTION__);
	}

	BOOL32 bEnableHDMeeting = FALSE;
	if (!GetEnableHDMeetingFromJson(achJsonBuf, bEnableHDMeeting))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[%s]GetEnableHDMeetingFromJson fail\n", __FUNCTION__);
	}

	tDomainInfo.SetWholeName( achDomainName );
	tDomainInfo.SetDomainGUID( achDomainGUID );
	tDomainInfo.SetSDomainMOID( achSDomainMOID );
	tDomainInfo.SetUsedFlag( TRUE ) ;
	tDomainInfo.SetDomainLevel( CKdvDomainInfo::em_KdvUserDomain );
	tDomainInfo.SetDomainMOID( achDomainMOID );
	tDomainInfo.SetMaxSmallConfNum(cUserDomainConfNumLimit.GetSmallConfNumLimit());
	tDomainInfo.SetMaxLargeConfNum(cUserDomainConfNumLimit.GetLargeConfNumLimit());
	tDomainInfo.SetEnableH265(bEnableH265);
	tDomainInfo.SetEnableHDMeeting(bEnableHDMeeting);

	CKdvDomainInfo cUserDomain; 
	g_cMauVCApp.GetDomainInfo(achSDomainMOID, cUserDomain);
	
	//�����û����XMPU������
	//����ƽ̨��Domain�޸��û���Xmpu�ı�ʶ
	tDomainInfo.SetHasXmpuServer(cUserDomain.HasXmpuServer());
	g_cMauVCApp.AddDomainInfo( tDomainInfo );

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[CAPInfoDBOpr::DealCreateUserDomain] Domain<%s>[%s], MOID[%s], SDomainMOID[%s] MaxConfNum[%d]\n", 
		tDomainInfo.GetWholeName(), tDomainInfo.GetDomainGUID(), tDomainInfo.GetDomainMOID(), tDomainInfo.GetSDomainMOID(), tDomainInfo.GetMaxConfNum() );
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CAPInfoDBOpr::DealCreateUserDomain] EnableHDMeeting[%d], EnableH265[%d], IsHasXmpu[%d]\n",
		tDomainInfo.IsEnbaleHDMeeting(), tDomainInfo.IsEnableH265(), tDomainInfo.HasXmpuServer());

}

void CBmcSsnInstance::DealEditUserDomain( void *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditUserDomain]pBuf is Null.\n" );
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	memcpy( achJsonBuf, pBuf, MAXLEN_JSONBUF );
// 	u32 dwMoidLen = MAXLEN_MSGTYPE;

	CKdvDomainInfo tNewDomainInfo;
	CKdvDomainInfo tOldDomainInfo;
	s8 achDomainName[OPERATION_MAXLEN_DOMAINNAME+1]   = {0};
	s8 achDomainGUID[OPERATION_MAXLEN_GUID+1]   = {0};
	s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1]  = {0};
	s8 achSDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};

	BOOL32 bRet = FALSE;
	//moid
	bRet = GetMOIDFromJson(achJsonBuf, achDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditUserDomain]Get moid is failure.\n" );
	}
	
	//name
	bRet = GetNameFromJson(achJsonBuf, achDomainName, OPERATION_MAXLEN_DOMAINNAME);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditUserDomain]Get name is failure.\n" );
	}

	//guid
	bRet = GetGuidFromJson(achJsonBuf, achDomainGUID, OPERATION_MAXLEN_GUID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditUserDomain]Get guid is failure.\n" );
	}

	//platformDomainMoid
	bRet = GetPlatformMoidFromJson(achJsonBuf, achSDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditUserDomain]Get platformmoid is failure.\n" );
	}

	CPlatformConfNumLimit cUserDomainConfNumLimit;
	if (!GetConfNumLimitFromJson(achJsonBuf, cUserDomainConfNumLimit, FALSE))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[%s]GetConfNumLimitFromJson fail\n", __FUNCTION__);
		return;
	}
	if (!g_cMauVCApp.SetMaxDomainConfNum(achDomainMOID,
		cUserDomainConfNumLimit.GetSmallConfNumLimit(), cUserDomainConfNumLimit.GetLargeConfNumLimit()))
	{
		//ͨ�����¼������ݿ��û�����Ϣ���������û���license
		g_cMauVCApp.m_cApInfoDBOpr.GetUserDomainInfo();	
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s]SetMaxDomainConfNum fail\n", __FUNCTION__);
	}
	
	//Ϊ���޸�/����/����ʹ��ͬһ���������˴�ȥjson��ȡ�Ƿ����õ��ֶ�
	BOOL32 bEnabled = FALSE;
	bRet = GetUseFlagFromJson(achJsonBuf, bEnabled);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditUserDomain]Get enabled is failure.\n" );
	}

	BOOL32 bEnableHDMeeting = FALSE; 
	bRet = GetEnableHDMeetingFromJson(achJsonBuf, bEnableHDMeeting);
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditUserDomain]Get enableHDMeeting is failure.\n");
	}

	BOOL32 bEnableH265 = FALSE;
	bRet = GetEnableH265FromJson(achJsonBuf, bEnableH265);
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditUserDomain]Get enableH265 is failure.\n");
	}

	//	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditUserDomain]Get json is %s\n", achJsonBuf );
	OspPrintf(TRUE, FALSE, "\n Get buf is %s \n", achJsonBuf);

//	OspPrintf(TRUE, FALSE, "\n MOID is %s, GUID is %s\n", achDomainMOID, achDomainGUID);
	tNewDomainInfo.SetWholeName( achDomainName );
	tNewDomainInfo.SetDomainGUID( achDomainGUID );
	tNewDomainInfo.SetSDomainMOID( achSDomainMOID );
	tNewDomainInfo.SetUsedFlag( bEnabled ) ;
	tNewDomainInfo.SetDomainLevel( CKdvDomainInfo::em_KdvUserDomain );
	tNewDomainInfo.SetDomainMOID( achDomainMOID );
	tNewDomainInfo.SetMaxSmallConfNum(cUserDomainConfNumLimit.GetSmallConfNumLimit());
	tNewDomainInfo.SetMaxLargeConfNum(cUserDomainConfNumLimit.GetLargeConfNumLimit());
	
	tNewDomainInfo.SetEnableH265(bEnableH265);
	tNewDomainInfo.SetEnableHDMeeting(bEnableHDMeeting);
	
	tOldDomainInfo.SetDomainMOID(achDomainMOID);
	g_cMauVCApp.GetDomainInfo(achDomainMOID, tOldDomainInfo);
	tNewDomainInfo.SetHasXmpuServer(tOldDomainInfo.HasXmpuServer());
	g_cMauVCApp.ModDomainInfo( tNewDomainInfo );

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CAPInfoDBOpr::DealEditUserDomain] Domain<%s>[%s], MOID[%s], SDomainMOID[%s] MaxConfNum[%d]\n",
		tNewDomainInfo.GetWholeName(), tNewDomainInfo.GetDomainGUID(), tNewDomainInfo.GetDomainMOID(), tNewDomainInfo.GetSDomainMOID(), tNewDomainInfo.GetMaxConfNum() );
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CAPInfoDBOpr::DealEditUserDomain] EnableHDMeeting[%d], EnableH265[%d], IsHasXmpu[%d]\n",
		tNewDomainInfo.IsEnbaleHDMeeting(), tNewDomainInfo.IsEnableH265(), tNewDomainInfo.HasXmpuServer());

}

void CBmcSsnInstance::DealForbidUserDomain( void *pBuf )
{
	DealEditUserDomain(pBuf);
}

void CBmcSsnInstance::DealEnableUserDomain( void *pBuf )
{
	DealEditUserDomain(pBuf);
}

void CBmcSsnInstance::DealCreatePlatformDomain( void *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreatePlatformDomain]pBuf is Null.\n" );
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	memcpy( achJsonBuf, pBuf, MAXLEN_JSONBUF );
	s8 achMoid[OPERATION_MAXLEN_MOID+1] = {0};
// 	u32 dwMoidLen = MAXLEN_MSGTYPE;

	CKdvDomainInfo tDomainInfo;
	s8 achDomainName[OPERATION_MAXLEN_DOMAINNAME+1]   = {0};
	s8 achDomainGUID[OPERATION_MAXLEN_GUID+1]   = {0};
	s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1]  = {0};
	s8 achSDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};
	s8 achPlatformPrefix[OPERATION_MAXLEN_PLATFORMPREFIX + 1] = {0};

	BOOL32 bRet = FALSE;
	//moid
	bRet = GetMOIDFromJson(achJsonBuf, achDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreatePlatformDomain]Get moid is failure.\n" );
	}

	//name
	bRet = GetNameFromJson(achJsonBuf, achDomainName, OPERATION_MAXLEN_DOMAINNAME);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreatePlatformDomain]Get name is failure.\n" );
	}

	//guid
	bRet = GetGuidFromJson(achJsonBuf, achDomainGUID, OPERATION_MAXLEN_GUID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreatePlatformDomain]Get guid is failure.\n" );
	}

	//serviceDomainMoid
	bRet = GetServerMoidFromJson(achJsonBuf, achSDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreatePlatformDomain]Get serviceDomainMoid is failure.\n" );
	}

	//numberSegment
	bRet = GetNumSegmentFromJson(achJsonBuf, achPlatformPrefix, OPERATION_MAXLEN_PLATFORMPREFIX);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreatePlatformDomain]Get numberSegment is failure.\n" );
	}
	CKdvDomainInfo tOldDomainInfo;
	bRet = g_cMauVCApp.GetDomainInfo(achDomainMOID, tOldDomainInfo);

	tDomainInfo.SetWholeName( achDomainName );
	tDomainInfo.SetDomainGUID( achDomainGUID );
	tDomainInfo.SetSDomainMOID( achSDomainMOID );
	tDomainInfo.SetUsedFlag( TRUE ) ;
	tDomainInfo.SetDomainLevel( CKdvDomainInfo::em_KdvPlatformDomain );
	tDomainInfo.SetDomainMOID( achDomainMOID );
	tDomainInfo.SetPlatformPrefix( achPlatformPrefix );

	if (bRet)
	{
		tDomainInfo.SetHasXmpuServer(tOldDomainInfo.HasXmpuServer());
		g_cMauVCApp.ModDomainInfo(tDomainInfo);
	}
	else
	{
		g_cMauVCApp.AddDomainInfo(tDomainInfo);
	}
	

	
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[CAPInfoDBOpr::DealCreatePlatformDomain] Domain<%s>[%s], MOID[%s], SDomainMOID[%s] PreFix[%s], IsHasXmpu[%d]\n", 
		tDomainInfo.GetWholeName(), tDomainInfo.GetDomainGUID(), tDomainInfo.GetDomainMOID(), tDomainInfo.GetSDomainMOID(), tDomainInfo.GetPlatformPrefix(), tDomainInfo.HasXmpuServer() );
}

void CBmcSsnInstance::DealEditPlatformDomain( void *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditPlatformDomain]pBuf is Null.\n" );
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	memcpy( achJsonBuf, pBuf, MAXLEN_JSONBUF );
	s8 achMoid[OPERATION_MAXLEN_MOID+1] = {0};
// 	u32 dwMoidLen = MAXLEN_MSGTYPE;


	s8 achDomainName[OPERATION_MAXLEN_DOMAINNAME+1]   = {0};
	s8 achDomainGUID[OPERATION_MAXLEN_GUID+1]   = {0};
	s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1]  = {0};
	s8 achSDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};
	s8 achPlatformPrefix[OPERATION_MAXLEN_PLATFORMPREFIX + 1] = {0};

	BOOL32 bRet = FALSE;
	//moid
	bRet = GetMOIDFromJson(achJsonBuf, achDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditPlatformDomain]Get moid is failure.\n" );
	}

	//name
	bRet = GetNameFromJson(achJsonBuf, achDomainName, OPERATION_MAXLEN_DOMAINNAME);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditPlatformDomain]Get name is failure.\n" );
	}

	//guid
	bRet = GetGuidFromJson(achJsonBuf, achDomainGUID, OPERATION_MAXLEN_GUID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditPlatformDomain]Get guid is failure.\n" );
	}

	//serviceDomainMoid
	bRet = GetServerMoidFromJson(achJsonBuf, achSDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditPlatformDomain]Get serviceDomainMoid is failure.\n" );
	}

	//numberSegment
	bRet = GetNumSegmentFromJson(achJsonBuf, achPlatformPrefix, OPERATION_MAXLEN_PLATFORMPREFIX);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditPlatformDomain]Get numberSegment is failure.\n" );
	}


	CKdvDomainInfo tOldDomainInfo;
	g_cMauVCApp.GetDomainInfo(achDomainMOID, tOldDomainInfo);

	CKdvDomainInfo tNewDomainInfo;
	tNewDomainInfo.SetWholeName( achDomainName );
	tNewDomainInfo.SetDomainGUID( achDomainGUID );
	tNewDomainInfo.SetSDomainMOID( achSDomainMOID );
	tNewDomainInfo.SetUsedFlag( TRUE ) ;
	tNewDomainInfo.SetDomainLevel( CKdvDomainInfo::em_KdvPlatformDomain );
	tNewDomainInfo.SetDomainMOID( achDomainMOID );
	tNewDomainInfo.SetPlatformPrefix( achPlatformPrefix );
	tNewDomainInfo.SetHasXmpuServer(tOldDomainInfo.HasXmpuServer());

	g_cMauVCApp.ModDomainInfo( tNewDomainInfo );

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_APDB, "[CAPInfoDBOpr::DealEditPlatformDomain] Domain<%s>[%s], MOID[%s], SDomainMOID[%s] PreFix[%s], IsHasXmpu[%d]\n", 
		tNewDomainInfo.GetWholeName(), tNewDomainInfo.GetDomainGUID(), tNewDomainInfo.GetDomainMOID(), tNewDomainInfo.GetSDomainMOID(), tNewDomainInfo.GetPlatformPrefix(), tNewDomainInfo.HasXmpuServer() );

}

//delete ���� ��ȡmoid����֮ǰ�Ȼ�ȡmoid�ĸ�����Ȼ���ٹ���moid�����飬��ȡ����Ȼ��ɾ��
void CBmcSsnInstance::DealDeleteServerDomain( void *pBuf )
{
	DeleteDomainInfo(pBuf);
}

void CBmcSsnInstance::DealDeleteUserDomain( void *pBuf )
{
	DeleteDomainInfo(pBuf);
}

void CBmcSsnInstance::DealDeletePlatformDomain( void *pBuf )
{
	DeleteDomainInfo(pBuf);
}

void CBmcSsnInstance::DeleteDomainInfo( void *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DeleteDomainInfo]pBuf is Null.\n" );
		return;
	}
	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	memcpy( achJsonBuf, pBuf, MAXLEN_JSONBUF );

	u32 dwNumMoid = 0;
	BOOL32 bRet = FALSE;
	//moidnum
	bRet = GetMoIdNumFromJson(achJsonBuf, dwNumMoid);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DeleteDomainInfo]Get moidnum is failure.\n" );
	}
	if (dwNumMoid == 0)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DeleteDomainInfo]Get moidnum is 0.\n" );
		return;
	}
	s8 **pachMoid;
	pachMoid = new s8*[dwNumMoid]; 
	u32 dwIndex = 0;
	for (dwIndex = 0; dwIndex < dwNumMoid; dwIndex ++)
	{
		pachMoid[dwIndex] = new s8[OPERATION_MAXLEN_APMOID+1];
	}
	bRet = GetMoIdListFromJson(achJsonBuf, pachMoid);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DeleteDomainInfo]Get moidlist is failure.\n" );
	}
	//ɾ����Ӧ��moid
	for (dwIndex = 0; dwIndex < dwNumMoid; dwIndex++)
	{
		s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1]  = {0};
		memcpy(achDomainMOID, pachMoid[dwIndex], OPERATION_MAXLEN_APMOID);
		CKdvDomainInfo cDomainInfo;
		cDomainInfo.SetDomainMOID(achDomainMOID);
		g_cMauVCApp.GetDomainInfo(achDomainMOID, cDomainInfo);
		g_cMauVCApp.DelDomainInfo(achDomainMOID);
	}
	//delete���������Դ
	for (dwIndex = 0; dwIndex < dwNumMoid; dwIndex ++)
	{
		delete [] pachMoid[dwIndex];
	}
	delete [] pachMoid;
}

void CBmcSsnInstance::DealCreateAccount( void *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateAccount]pBuf is Null.\n" );
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	memcpy( achJsonBuf, pBuf, MAXLEN_JSONBUF );
	s8 achMoid[OPERATION_MAXLEN_MOID+1] = {0};
// 	u32 dwMoidLen = MAXLEN_MSGTYPE;

	CMauMtInfo tMtInfo;
	s8 achE164[MAXLEN_E164+1]   = {0};
	s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};
	u16 wMtType = 0;
	BOOL32 bLimited = FALSE;

	BOOL32 bRet = FALSE;
	bRet = GetMOIDFromJson(achJsonBuf, achMoid, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateAccount]Get moid is failure.\n");
	}
	//moid
	bRet = GetUserDomainIdFromJson(achJsonBuf, achDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateAccount]Get user_moid is failure.\n" );
	}

	//e164
	bRet = GetE164FromJson(achJsonBuf, achE164, MAXLEN_E164);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateAccount]Get E164 is failure.\n" );
	}

	//device_type
	bRet = GetDeviceTypeFromJson(achJsonBuf, wMtType);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateAccount]Get wMtType is failure.\n" );
	}

	//limited
	bRet = GetLimitedFromJson(achJsonBuf, bLimited);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateAccount]Get Limited is failure.\n" );
	}

	tMtInfo.SetDomainMOID( achDomainMOID );
	tMtInfo.SetMOID(achMoid);
	tMtInfo.SetE164( achE164 );
	tMtInfo.SetMtType( wMtType );
	tMtInfo.SetLimitedFlag( bLimited );

	g_cMauVCApp.AddMtInfo( tMtInfo );

	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[CBmcSsnData::DealCreateAccount] E164NO<%s> DomainMOID<%s> MtType<%d> MtLimited<%d>\n", 
		tMtInfo.GetE164(), tMtInfo.GetDomainMOID(), tMtInfo.GetMtType(), tMtInfo.GetLimitedFlag() );

}

void CBmcSsnInstance::DealEditAccount( void *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditAccount]pBuf is Null.\n" );
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	memcpy( achJsonBuf, pBuf, MAXLEN_JSONBUF );
	s8 achMoid[OPERATION_MAXLEN_MOID+1] = {0};
// 	u32 dwMoidLen = MAXLEN_MSGTYPE;

	CMauMtInfo tMtInfo;
	s8 achE164[MAXLEN_E164+1]   = {0};
	s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};
	u16 wMtType = 0;
	BOOL32 bLimited = FALSE;

	BOOL32 bRet = FALSE;
	bRet = GetMOIDFromJson(achJsonBuf, achMoid, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateAccount]Get moid is failure.\n");
	}
	//moid
	bRet = GetUserDomainIdFromJson(achJsonBuf, achDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditAccount]Get moid is failure.\n" );
	}

	//e164
	bRet = GetE164FromJson(achJsonBuf, achE164, MAXLEN_E164);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditAccount]Get E164 is failure.\n" );
	}

	//device_type
	bRet = GetDeviceTypeFromJson(achJsonBuf, wMtType);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditAccount] Get wMtType is failure.\n" );
	}

	//limited
	bRet = GetLimitedFromJson(achJsonBuf, bLimited);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealEditAccount]Get Limited is failure.\n" );
	}

	tMtInfo.SetMOID(achMoid);
	tMtInfo.SetDomainMOID( achDomainMOID );                          
	tMtInfo.SetE164( achE164 );
	tMtInfo.SetMtType( wMtType );
	tMtInfo.SetLimitedFlag( bLimited );

	g_cMauVCApp.ModMtInfo( tMtInfo );
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[CBmcSsnData::DealEditAccount] E164NO<%s> DomainMOID<%s> MtType<%d> MtLimited<%d>\n", 
		tMtInfo.GetE164(), tMtInfo.GetDomainMOID(), tMtInfo.GetMtType(), tMtInfo.GetLimitedFlag() );
}

void CBmcSsnInstance::DealForbidAccount( void *pBuf )
{
	//��Ϊȱ��enable�ֶ�  ����forbid��ɾ��
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealForbidAccount]pBuf is Null.\n" );
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	memcpy( achJsonBuf, pBuf, MAXLEN_JSONBUF );
	s8 achMoid[OPERATION_MAXLEN_MOID+1] = {0};
// 	u32 dwMoidLen = MAXLEN_MSGTYPE;

	CMauMtInfo tMtInfo;
	s8 achE164[MAXLEN_E164+1]   = {0};
	s8 achDomainMOID[OPERATION_MAXLEN_APMOID+1] = {0};
	u16 wMtType = 0;
	BOOL32 bLimited = FALSE;

	BOOL32 bRet = FALSE;
	bRet = GetMOIDFromJson(achJsonBuf, achMoid, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealCreateAccount]Get moid is failure.\n");
	}
	//userdomainid
	bRet = GetUserDomainIdFromJson(achJsonBuf, achDomainMOID, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealForbidAccount]Get moid is failure.\n" );
	}

	//e164
	bRet = GetE164FromJson(achJsonBuf, achE164, MAXLEN_E164);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealForbidAccount]Get E164 is failure.\n" );
	}

	//device_type
	bRet = GetDeviceTypeFromJson(achJsonBuf, wMtType);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealForbidAccount] Get wMtType is failure.\n" );
	}

	//limited
	bRet = GetLimitedFromJson(achJsonBuf, bLimited);
	if (!bRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealForbidAccount]Get Limited is failure.\n" );
	}
	tMtInfo.SetMOID(achMoid);
	tMtInfo.SetDomainMOID( achDomainMOID );                          
	tMtInfo.SetE164( achE164 );
	tMtInfo.SetMtType( wMtType );
	tMtInfo.SetLimitedFlag( bLimited );

	g_cMauVCApp.DelMtInfo( tMtInfo );
	LogPrint( LOG_LVL_DETAIL, MID_MAU_APDB, "[CBmcSsnData::DealForbidAccount] E164NO<%s> DomainMOID<%s> MtType<%d> MtLimited<%d>\n", 
		tMtInfo.GetE164(), tMtInfo.GetDomainMOID(), tMtInfo.GetMtType(), tMtInfo.GetLimitedFlag() );
}

void CBmcSsnInstance::DealEnableAccount( void *pBuf )
{
	//��Ϊȱ��enable�ֶ�  ����enable��create�߼�
	DealCreateAccount(pBuf);
}

void CBmcSsnInstance::DealCreateServer( void *pBuf )
{
	DealServerInfoNtf(pBuf);
}

void CBmcSsnInstance::DealEditServer( void *pBuf )
{
	DealServerInfoNtf(pBuf);
}

void CBmcSsnInstance::DealEnableServer( void *pBuf )
{
	DealServerInfoNtf(pBuf);
}

void CBmcSsnInstance::DealForbidServer( void *pBuf )
{
	DealServerInfoNtf(pBuf);
}

void CBmcSsnInstance::DealDeleteServer(void *pBuf)
{
	if (NULL == pBuf)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnInstance::DealDeleteServer]pBuf is Null.\n");
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	memcpy(achJsonBuf, pBuf, MAXLEN_JSONBUF);
	
	u16 wServerType = 0;
	GetServerTypeFromJson(achJsonBuf, wServerType);
	//��ɾ��HDU2����������Ϣ��������
	if (SERVER_TYPE_HDU2 != wServerType && SERVER_TYPE_HDU2_L != wServerType && SERVER_TYPE_HDU2_S != wServerType
		&& SERVER_TYPE_HDU5 != wServerType && SERVER_TYPE_HDU5_T != wServerType)
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcSsnInstance::DealDeleteServer]ServerType Don't exit.\n");
		return;
	}

	BOOL32 bRet = FALSE;
	u32 dwNumMoid = 0;
	bRet = GetMoIdNumFromJson(achJsonBuf, dwNumMoid);
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealDeleteServer]Get moidnum is failure.\n");
	}
	if (dwNumMoid == 0)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealDeleteServer]Get moidnum is 0.\n");
		return;
	}
	s8 **pachMoid;
	pachMoid = new s8*[dwNumMoid];
	u32 dwIndex = 0;
	for (dwIndex = 0; dwIndex < dwNumMoid; dwIndex++)
	{
		pachMoid[dwIndex] = new s8[OPERATION_MAXLEN_APMOID + 1];
	}
	bRet = GetMoIdListFromJson(achJsonBuf, pachMoid);
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealDeleteServer]Get moidlist is failure.\n");
	}
	//ɾ����Ӧ��moid
	for (dwIndex = 0; dwIndex < dwNumMoid; dwIndex++)
	{
		s8 achMoid[OPERATION_MAXLEN_APMOID + 1] = { 0 };
		memcpy(achMoid, pachMoid[dwIndex], OPERATION_MAXLEN_APMOID);
		g_cMauVCApp.DelHduUserDomainMoid(achMoid);
	}
	//delete���������Դ
	for (dwIndex = 0; dwIndex < dwNumMoid; dwIndex++)
	{
		delete[] pachMoid[dwIndex];
	}
	delete[] pachMoid;
}


BOOL32 CBmcSsnInstance::GetServerTypeFromJson( s8 * pchJsonBuf, u16& wServerType )
{
	json_error_t jReason;
	json_t_ptr pjRoot = json_loads( pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason );
	if ( NULL ==  pjRoot )
	{
		OspPrintf( TRUE, FALSE, "[GetDeviceTypeFromJson] json_loads err, return FALSE!\n" );
		return FALSE;
	}

	json_t_ptr pjDeviceType = json_object_get_s( pjRoot, "serverType" );
	if ( NULL == pjDeviceType )
	{
		OspPrintf( TRUE, FALSE, "[GetDeviceTypeFromJson] json_object_get pjDeviceType is NULL, return FALSE!\n" );
		return FALSE;
	}

	wServerType = (u16)json_integer_value( pjDeviceType );

	return TRUE;
}

void CBmcSsnInstance::DealServerInfoNtf( void *pBuf )
{
	if ( NULL == pBuf )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealServerInfoNtf]pBuf is Null.\n" );
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF+1] = {0};
	memcpy( achJsonBuf, pBuf, MAXLEN_JSONBUF );
	u16 wServerType = 0;
	GetServerTypeFromJson( achJsonBuf, wServerType );
	//������xmpu������
	if (SERVER_TYPE_MPS == wServerType || SERVER_TYPE_XMPU == wServerType)
	{
		//ͬ�����ݿ��������
		g_cMauVCApp.m_cApInfoDBOpr.GetDomainHasXmpu();
		LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcSsnInstance::DealServerInfoNtf]IT IS MPS OR XMPU SERVER.\n" );
	}
	else if (SERVER_TYPE_HDU2 == wServerType || SERVER_TYPE_HDU2_L == wServerType || SERVER_TYPE_HDU2_S == wServerType
		|| SERVER_TYPE_HDU5 == wServerType || SERVER_TYPE_HDU5_T == wServerType)
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcSsnInstance::DealServerInfoNtf]IT IS HDU SERVER.\n");
		DealHduServerInfoNtf(pBuf);
	}
	else
	{
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcSsnInstance::DealServerInfoNtf]ServerType Don't exit.\n");
	}
}

void CBmcSsnInstance::DealHduServerInfoNtf(void *pBuf)
{
	if (NULL == pBuf)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealHduServerInfoNtf]pBuf is Null.\n");
		return;
	}

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	memcpy(achJsonBuf, pBuf, MAXLEN_JSONBUF);
	s8 achMoid[OPERATION_MAXLEN_MOID + 1] = { 0 };
	s8 achUserDomainMoid[OPERATION_MAXLEN_MOID + 1] = { 0 };
	BOOL32 bEnabled = FALSE;
	s8 achEvent[MAXLEN_MSGTYPE + 1 ] = { 0 };
	BOOL32 bRet = FALSE;
	bRet = GetMOIDFromJson(achJsonBuf, achMoid, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealHduServerInfoNtf]Get moid is failure.\n");
		return;
	}

	bRet = GetUserDomainIdFromJson(achJsonBuf, achUserDomainMoid, OPERATION_MAXLEN_APMOID);
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealHduServerInfoNtf]Get UserDomainmoid is failure.\n");
		return;
	}
	
	if (strcmp(achMoid, "") == 0  || strcmp(achUserDomainMoid, "") == 0)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealHduServerInfoNtf]HDU moid or UserdomainMoid is NULL.\n");
		return;
	}

	bRet = GetEnabledFromJson(achJsonBuf, bEnabled);
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealHduServerInfoNtf]Get enabled is failure.\n");
	}

	if (bEnabled)
	{
		//���ӡ��޸ġ�����HDU
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcSsnData::DealHduServerInfoNtf]Get enabled is True, ADD or Mod HduMoidAndUserDomainMoid.\n");
		g_cMauVCApp.AddModHduMoidUserDomainMoid(achMoid, achUserDomainMoid);
	}
	else
	{
		//ͣ��HDU
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcSsnData::DealHduServerInfoNtf]Get enabled is false, Delete HduMoidAndUserDomainMoid.\n");
		g_cMauVCApp.DelHduUserDomainMoid(achMoid);
	}

}

void CBmcSsnInstance::DealUpdateSystemMode(void *pBuf)
{
	if (NULL == pBuf)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[%s]pBuf is Null.\n", __FUNCTION__);
		return;
	}

	json_t_ptr pjRoot = NULL;
	LoadJson((s8*)pBuf, strlen((s8*)pBuf), pjRoot, FALSE);
	u32 dwPlatformIsRentVal = 0;
	GetStringAsUIntFromJson(pjRoot, "data", dwPlatformIsRentVal);
	g_cMauVCApp.SetPlatformIsRent(dwPlatformIsRentVal == 0 ? TRUE : FALSE);
}

void CBmcSsnInstance::DealBatchCreateAccount(void *pBuf)
{
	if (NULL == pBuf)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealBatchCreateAccount]pBuf is Null.\n");
		return;
	}

	BOOL32 bRet = FALSE;
	u32 dwAccountNum = 0;
	bRet = GetAccountNumFromJson((s8*)pBuf, dwAccountNum);
	if (!bRet)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealBatchCreateAccount]Get dwAccountNum is failure.\n");
	}
	if (dwAccountNum == 0)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealBatchCreateAccount]Get dwAccountNum is 0.\n");
		return;
	}
	LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcSsnData::DealBatchCreateAccount]dwAccountNum is %d.\n",dwAccountNum);

	vector<CMauMtInfo> vec_MauMtInfo;
	GetAccountListFormJson((s8*)pBuf, vec_MauMtInfo);
	vector<CMauMtInfo>::iterator itor = vec_MauMtInfo.begin();
	for (; itor != vec_MauMtInfo.end(); itor++)
	{
		g_cMauVCApp.AddMtInfo(*itor);
		LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[GetAccountListFormJson] E164NO<%s> DomainMOID<%s> MtType<%d> MtLimited<%d>\n",
			(*itor).GetE164(), (*itor).GetDomainMOID(), (*itor).GetMtType(), (*itor).GetLimitedFlag());
	}
}

BOOL32 CBmcSsnInstance::GetAccountNumFromJson(const s8 * pchJsonBuf, u32& dwAccountNum)
{
	if (NULL == pchJsonBuf)
	{
		OspPrintf(TRUE, FALSE, "[GetAccountNumFromJson]Param is NULL, return!\n");
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads(pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason);
	if (NULL == pjRoot)
	{
		OspPrintf(TRUE, FALSE, "[GetAccountNumFromJson] json_loads err!\n");
		return FALSE;
	}
	dwAccountNum = json_array_size(pjRoot);
	return TRUE;
}

BOOL32 CBmcSsnInstance::GetAccountListFormJson(const s8 * pchJsonBuf, vector<CMauMtInfo>& vec_MauMtInfo)
{
	if (NULL == pchJsonBuf)
	{
		OspPrintf(TRUE, FALSE, "[GetAccountListFormJson]Param is NULL, return!\n");
		return FALSE;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = json_loads(pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason);
	if (NULL == pjRoot)
	{
		OspPrintf(TRUE, FALSE, "[GetAccountListFormJson] json_loads err!\n");
		return FALSE;
	}

	u32 dwAccountNum = 0;
	dwAccountNum = json_array_size(pjRoot);

	u32 dwAccountIdx = 0;
	for (dwAccountIdx = 0; dwAccountIdx < dwAccountNum; dwAccountIdx++)
	{
		json_t_ptr pjAccountInfo = json_array_get_s(pjRoot, dwAccountIdx);
		if (NULL == pjAccountInfo)
		{
			continue;
		}
		s8 achMoid[OPERATION_MAXLEN_MOID + 1] = { 0 };
		CMauMtInfo tMtInfo;
		s8 achE164[MAXLEN_E164 + 1] = { 0 };
		s8 achDomainMOID[OPERATION_MAXLEN_APMOID + 1] = { 0 };
		u16 wMtType = 0;
		BOOL32 bLimited = FALSE;
		//moid
		json_t_ptr pjMoId = json_object_get_s(pjAccountInfo, JSON_MOID);
		if (NULL == pjMoId)
		{
			OspPrintf(TRUE, FALSE, "[GetAccountListFormJson] json_object_get moid is NULL, retur FALSE!\n");
		}
		else
		{
			strncpy(achMoid, json_string_value(pjMoId), OPERATION_MAXLEN_MOID);
		}
		//userDomainMoid
		json_t_ptr pjUserMoId = json_object_get_s(pjAccountInfo, "userDomainMoid");
		if (NULL == pjUserMoId)
		{
			OspPrintf(TRUE, FALSE, "[GetAccountListFormJson] json_object_get domainmoid is NULL, retur FALSE!\n");
		}
		else
		{
			strncpy(achDomainMOID, json_string_value(pjUserMoId), OPERATION_MAXLEN_APMOID);
		}
		//e164
		json_t_ptr pjE164 = json_object_get_s(pjAccountInfo, "e164");
		if (NULL == pjE164)
		{
			OspPrintf(TRUE, FALSE, "[GetAccountListFormJson] json_object_get pjE164 is NULL, return FALSE!\n");
		}
		else
		{
			strncpy(achE164, json_string_value(pjE164), MAXLEN_E164);
		}
		//deviceType
		json_t_ptr pjDeviceType = json_object_get_s(pjAccountInfo, "deviceType");
		if (NULL == pjDeviceType)
		{
			OspPrintf(TRUE, FALSE, "[GetAccountListFormJson] json_object_get pjDeviceType is NULL, return FALSE!\n");
			return FALSE;
		}
		else
		{
			wMtType = (u16)atoi(json_string_value(pjDeviceType));
		}

		//limited
		json_t_ptr pjLimited = json_object_get_s(pjAccountInfo, "limited");
		if (NULL == pjLimited)
		{
			OspPrintf(TRUE, FALSE, "[GetAccountListFormJson] json_object_get limited is NULL, return FALSE!\n");
			return FALSE;
		}
		else
		{
			bLimited = json_is_true(pjLimited);
		}

		tMtInfo.SetDomainMOID(achDomainMOID);
		tMtInfo.SetMOID(achMoid);
		tMtInfo.SetE164(achE164);
		tMtInfo.SetMtType(wMtType);
		tMtInfo.SetLimitedFlag(bLimited);
		vec_MauMtInfo.push_back(tMtInfo);
	}

	return TRUE;
}
CBmcSsnInstance::DealFunMap CBmcSsnInstance::deal_fun_map;

