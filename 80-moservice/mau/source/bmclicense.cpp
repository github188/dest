/*****************************************************************************
ģ����      : MAU
�ļ���      : bmclicense.cpp
����ʱ��    : 2015�� 08�� 25��
ʵ�ֹ���    : 
����        : ���ڽ�
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2015/08/25  5.0        ���ڽ�          ����
******************************************************************************/
#ifdef WIN32
#pragma warning( disable:4786 )
#endif

#include "inc.h"

#include "bmclicense.h"
#include "evmau.h"
#include "mauvc.h"
#include "connectsa_api.h"
#include "kdvencrypt.h"

extern CMauVCApp g_cMauVCApp;

static BOOL32 GetDomainAndKeyFromJson(s8 * pchJsonBuf, s8* pchDomain, u32 dwDomainLen, s8* pchRmqKey, u32 dwKeyLen);

CBmcLicenseApp g_cBmcLicenseApp;

//ƽ̨��
s32 CBmcLicenseData::s_nBmcLicenseRabbitMq = 0;
std::string CBmcLicenseData::s_strPlatformRoutingKey;
std::string CBmcLicenseData::s_strPlatformQName;

//������
s32 CBmcLicenseData::s_nTopLevelDomainRabbitMq = 0;
std::string CBmcLicenseData::s_strTopLevelRoutingKey;
std::string CBmcLicenseData::s_strTopLevelQName;

#define PLATFORM_LICENSE_AES_KEY					   "DKshxQgZpLMNGx7P"
#define PLATFORM_LICENSE_AES_IV				     	   "JpxFCig2bUahtnEO"
#define JSON_ROUTING_KEY           "routingKey"					  //Routing Key
#define JSON_DOMAIN_MOID           "platformDomainMoid"			  //ƽ̨ moid
#define EDU_OCCU				   "OCCU"		//EDUϵ��MCU�ͺ�ǰ׺��OCCU6000A,OCCU6000B, OCCU4000A....
#define TIMER_EVENT_LICENSE_REQ   6000									  //������Ȩ����ʱ��
#define TIMER_SPAN_LICENSE_REQ    60000                                  //������Ȩ����ʱ�������
#define TIMER_SPAN_GET_GUID_MOID  1000									 //��ȡƽ̨MOID&&GUID��ʱ��

u8 Char2Hex(s8 byChar)
{
	if (byChar >= '0' && byChar <= '9')
	{
		return byChar - '0';
	}
	if (byChar >= 'a' && byChar <= 'f')
	{
		return byChar - 'a' + 10;
	}
	if (byChar >= 'A' && byChar <= 'F')
	{
		return byChar - 'A' + 10;
	}
	return 0;
}

void HexStr2Byte(const u8* pchHex, u32 dwHexLen,
	u8* pbyByte, u32& dwByteLen)
{
	dwByteLen = 0;
	u32 dwHexIndex = 0;
	for (; dwHexIndex < dwHexLen; dwHexIndex += 2)
	{
		pbyByte[dwByteLen++] = (Char2Hex(pchHex[dwHexIndex]) << 4)
							  + Char2Hex(pchHex[dwHexIndex + 1]);
	}
}
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
2015/08/25  5.0     ���ڽ�                  ����
=============================================================================*/
BOOL32 CBmcLicenseData::Init_MQPC()
{
	//���������ߺ��������Ի�ȡ�������license
	s_nTopLevelDomainRabbitMq = SafeCreateProducer(
		g_cMauVCApp.m_achTopLevelMqIP, 5672, "/", "dev", "dev",
		TopLevelProducer_ConSuc_MQCB, TopLevelProducer_ConFail_MQCB);
	if (0 == s_nTopLevelDomainRabbitMq)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::Init_MQPC] s_nTopLevelDomainRabbitMq = 0, return FALSE!\n" );
	}

	s_strTopLevelRoutingKey = MQ_TOPLEVEL_BMC_LICENSE_K;
	s_strTopLevelRoutingKey += g_cMauVCApp.m_achPlatformGuid;
	string topLevelBmcLicenseQ(MQ_TOPLEVEL_BMC_LICENSE_Q);
	topLevelBmcLicenseQ += g_cMauVCApp.m_achPlatformGuid;
	BOOL32 nRet = SafeCreateConsumer(
					g_cMauVCApp.m_achTopLevelMqIP, 5672, "/", "dev", "dev",
					topLevelBmcLicenseQ.c_str(),
					MQ_TOPLEVEL_BMC_LICENSE_EX,
					s_strTopLevelRoutingKey.c_str(),
					Content_TopLevelMQCB,
					NULL, NULL, TopLevelConsumer_ConSuc_MQCB, TopLevelConsumer_ConFail_MQCB,
					0, 0, 0, 1, g_cMauVCApp.GetMqConsumerHeartBeatTime());
	if (FALSE == nRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s] CreateConSumer<%s> False\n",
			__FUNCTION__, topLevelBmcLicenseQ.c_str());
	}

	string topLevelBmcLicenseNotifyKey(MQ_TOPLEVEL_BMC_LICENSE_NOTIFY_K);
	topLevelBmcLicenseNotifyKey += g_cMauVCApp.m_achPlatformMoid;
	string topLevelBmcLicenseNotifyQ(MQ_TOPLEVEL_BMC_LICENSE_NTF_Q);
	topLevelBmcLicenseNotifyQ += g_cMauVCApp.m_achPlatformMoid;
	nRet = SafeCreateConsumer(
					g_cMauVCApp.m_achTopLevelMqIP, 5672, "/", "dev", "dev",
					topLevelBmcLicenseNotifyQ.c_str(),
					MQ_TOPLEVEL_BMC_LICENSE_EX,
					topLevelBmcLicenseNotifyKey.c_str(),
					Content_TopLevelLicenseUpdateMQCB,
					NULL, NULL, TopLevelLicenseUpdateConsumer_ConSuc_MQCB,
					TopLevelLicenseUpdateConsumer_ConFail_MQCB,
					0, 0, 0, 1, g_cMauVCApp.GetMqConsumerHeartBeatTime());
	if (FALSE == nRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s]CreateConSumer<%s> False\n",
			__FUNCTION__, topLevelBmcLicenseNotifyQ.c_str());
	}

	// ���������ߺ��������Է����Ӻ������ȡ��license
	s_nBmcLicenseRabbitMq = SafeCreateP( PlatformProducer_ConSuc_MQCB, PlatformProducer_ConFail_MQCB );
	if ( 0 == s_nBmcLicenseRabbitMq )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::Init_MQPC] s_nBmcLicenseRabbitMq = 0, return FALSE!\n" );
	}

	s_strPlatformRoutingKey = MQ_PLATFORM_BMC_LICENSE_K;
	s_strPlatformQName = MQ_PLATFORM_BMC_LICENSE_Q;
	nRet = SafeCreateC( s_strPlatformQName.c_str(),
						MQ_PLATFORM_BMC_LICENSE_EX,
						s_strPlatformRoutingKey.c_str(),
						Content_PlatformMQCB,
						NULL, NULL, PlatformConsumer_ConSuc_MQCB, PlatformConsumer_ConFail_MQCB,
						0, 0, 0, 1, g_cMauVCApp.GetMqConsumerHeartBeatTime());
	if (FALSE == nRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s]CreateConSumer<platform.license.q> False\n", 
			__FUNCTION__);
	}

	return TRUE;
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
2015/08/25  5.0     ���ڽ�                  ����
=============================================================================*/
void CBmcLicenseData::Content_TopLevelMQCB( amqp_connection_state_t conn, void *pbuf, size_t len, response_type *prsp_type, char* pchTag)
{
	if ( NULL == pbuf )
	{
		*prsp_type = RT_NACK;
		return;
	}
	*prsp_type = RT_ACK;
	OspPost( MAKEIID( AID_MAU_BMCLICENSE_APPID, CInstance::DAEMON ), BMC_LICENSE_MESSAGE_CB, pbuf, len );            
    return;
}

void CBmcLicenseData::Content_PlatformMQCB( amqp_connection_state_t conn,
	void *pbuf, size_t len, response_type *prsp_type, char* pchTag)
{
	if ( NULL == pbuf )
	{
		*prsp_type = RT_NACK;
		LogPrint( LOG_LVL_DETAIL, MID_MAU_BMC, "[CBmcLicenseData::Content_PlatformMQCB]pbuf is null\n");
		return;
	}
	*prsp_type = RT_ACK;

	OspPost( MAKEIID( AID_MAU_BMCLICENSE_APPID, CInstance::DAEMON ), BMC_LICENSE_ASK_CB, pbuf, len );            
}

void CBmcLicenseData::Content_TopLevelLicenseUpdateMQCB(amqp_connection_state_t conn,
	void *pbuf, size_t len, response_type *prsp_type, char* pchTag)
{
	if ( NULL == pbuf )
	{
		*prsp_type = RT_NACK;
		return;
	}
	*prsp_type = RT_ACK;
	OspPost( MAKEIID( AID_MAU_BMCLICENSE_APPID, CInstance::DAEMON ), BMC_LICENSE_UPDATE_CB, pbuf, len );            
    return;
}

void CBmcLicenseData::TopLevelLicenseUpdateConsumer_ConSuc_MQCB(amqp_connection_state_t conn,
	char *desc, char* pchTag)
{
	g_cMauVCApp.GetMqStatus().SetMovisionLicenseUpdateQIsCreated(TRUE);

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[%s]Create TopLevelLicenseUpdateConsumer success "
		"desc: %s\n", __FUNCTION__, desc);
}

void CBmcLicenseData::TopLevelLicenseUpdateConsumer_ConFail_MQCB(amqp_connection_state_t conn,
	const char *expect, const char *precv, char* pchTag)
{
	g_cMauVCApp.GetMqStatus().SetMovisionLicenseUpdateQIsCreated(FALSE);

	s8 achExpect[MAXLEN_ALIAS] = {0};
	s8 achRecv[MAXLEN_ALIAS] = {0};
	if ( NULL != expect  )
	{
		strncpy( achExpect, expect, sizeof(achExpect) - 1 );
	}
	if ( NULL != precv  )
	{
		strncpy( achRecv, precv, sizeof(achRecv) - 1 );
	}
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::TopLevelConsumer_ConFail_MQCB]expect.%s.\n", achExpect );
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::TopLevelConsumer_ConFail_MQCB]precv.%s.\n", achRecv );
}

void CBmcLicenseData::TopLevelProducer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc )
{	
	g_cMauVCApp.GetMqStatus().SetMovisionLicensePIsCreated(TRUE);

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[%s]%s.\n", __FUNCTION__, desc);
}

void CBmcLicenseData::PlatformProducer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc)
{	
	g_cMauVCApp.GetMqStatus().SetPlatformLicensePIsCreated(TRUE);

	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[%s]%s.\n", __FUNCTION__, desc);
}

/*=============================================================================
�� �� ���� Producer_ConFail_MQCB
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/08/25  5.0     ���ڽ�                  ����
=============================================================================*/
void CBmcLicenseData::PlatformProducer_ConFail_MQCB( amqp_connection_state_t conn,
	const char *expect, const char *precv)
{
	g_cMauVCApp.GetMqStatus().SetPlatformLicensePIsCreated(FALSE);

	s8 achExpect[MAXLEN_ALIAS] = {0};
	s8 achRecv[MAXLEN_ALIAS] = {0};
	if ( NULL != expect  )
	{
		strncpy( achExpect, expect, sizeof(achExpect) - 1 );
	}
	if ( NULL != precv  )
	{
		strncpy( achRecv, precv, sizeof(achRecv) - 1 );
	}
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::Producer_ConFail_MQCB]expect.%s.\n", achExpect );
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::Producer_ConFail_MQCB]precv.%s.\n", achRecv );
}

void CBmcLicenseData::TopLevelProducer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv )
{
	g_cMauVCApp.GetMqStatus().SetMovisionLicensePIsCreated(FALSE);

	s8 achExpect[MAXLEN_ALIAS] = {0};
	s8 achRecv[MAXLEN_ALIAS] = {0};
	if ( NULL != expect  )
	{
		strncpy( achExpect, expect, sizeof(achExpect) - 1 );
	}
	if ( NULL != precv  )
	{
		strncpy( achRecv, precv, sizeof(achRecv) - 1 );
	}
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::TopLevelProducer_ConFail_MQCB]expect.%s.\n", achExpect );
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::TopLevelProducer_ConFail_MQCB]precv.%s.\n", achRecv );
}

/*=============================================================================
�� �� ���� Consumer_ConSuc_MQCB
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/08/25  5.0     ���ڽ�                  ����
=============================================================================*/
void CBmcLicenseData::TopLevelConsumer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc, char* pchTag )
{	
	g_cMauVCApp.GetMqStatus().SetMovisionLicenseQIsCreated(TRUE);

	s8 achSucDec[MAXLEN_ALIAS] = {0};
	if ( NULL != desc )
	{
		strncpy( achSucDec, desc, sizeof(achSucDec) - 1 );
	}
	OspPost(MAKEIID(AID_MAU_BMCLICENSE_APPID, CInstance::DAEMON), BMC_LICENSE_CONSUMER_SUC);
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcLicenseData::TopLevelConsumer_ConSuc_MQCB]%s.\n", achSucDec );	
    return;
}

void CBmcLicenseData::PlatformConsumer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc, char* pchTag )
{	
	g_cMauVCApp.GetMqStatus().SetPlatformLicenseQIsCreated(TRUE);

	s8 achSucDec[MAXLEN_ALIAS] = {0};
	if ( NULL != desc )
	{
		strncpy( achSucDec, desc, sizeof(achSucDec) - 1 );
	}
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcLicenseData::PlatformConsumer_ConSuc_MQCB]%s.\n", achSucDec );	
    return;
}
/*=============================================================================
�� �� ���� Consumer_ConFail_MQCB
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/08/25  5.0     ���ڽ�                  ����
=============================================================================*/
void CBmcLicenseData::PlatformConsumer_ConFail_MQCB( amqp_connection_state_t conn,
	const char *expect, const char *precv , char* pchTag)
{	
	g_cMauVCApp.GetMqStatus().SetPlatformLicenseQIsCreated(FALSE);

	s8 achExpect[MAXLEN_ALIAS] = {0};
	s8 achRecv[MAXLEN_ALIAS] = {0};
	if ( NULL != expect  )
	{
		strncpy( achExpect, expect, sizeof(achExpect) - 1 );
	}
	if ( NULL != precv  )
	{
		strncpy( achRecv, precv, sizeof(achRecv) - 1 );
	}
	
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::Consumer_ConFail_MQCB]expect.%s.\n", achExpect );
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::Consumer_ConFail_MQCB]precv.%s.\n", achRecv );
}

void CBmcLicenseData::TopLevelConsumer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect,
	const char *precv, char* pchTag )
{	
	g_cMauVCApp.GetMqStatus().SetMovisionLicenseQIsCreated(FALSE);

	s8 achExpect[MAXLEN_ALIAS] = {0};
	s8 achRecv[MAXLEN_ALIAS] = {0};
	if ( NULL != expect  )
	{
		strncpy( achExpect, expect, sizeof(achExpect) - 1 );
	}
	if ( NULL != precv  )
	{
		strncpy( achRecv, precv, sizeof(achRecv) - 1 );
	}
	
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::TopLevelConsumer_ConFail_MQCB]expect.%s.\n", achExpect );
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::TopLevelConsumer_ConFail_MQCB]precv.%s.\n", achRecv );
}

void CBmcLicenseData::SendLicenseReqMsgToBMC()
{
	json_t_ptr pjMsg = json_object();
	if (pjMsg)
	{
		SetStringToJson(pjMsg, JSON_ROUTING_KEY, s_strTopLevelRoutingKey.c_str());
		SetStringToJson(pjMsg, JSON_DOMAIN_MOID, g_cMauVCApp.m_achPlatformMoid);
		json_str strMsg(pjMsg);
		if (strMsg != NULL)
		{
			//���������������License
			PostMsg(s_nTopLevelDomainRabbitMq, strMsg, strlen(strMsg), MQ_TOPLEVEL_BMC_LICENSE_PRODUCER_K,
				MQ_TOPLEVEL_BMC_LICENSE_EX, 0, 0, NULL, NULL, TIMER_SPAN_LICENSE_REQ);
			LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcLicenseData::SendLicenseReqMsgToBMC]License: %s\n",
				g_cMauVCApp.m_achPlatformMoid);
		}
	}
}

void HexStr2Byte(const u8* pchCipher, const u32 dwCipherLen,
	u8* pchBinaryCipher, u32& dwBinaryCipherLen);
void CBmcLicenseData::PrintBmcLicenseInfo()
{
	u8 achBinaryCipher[10240 + 1] = { 0 };
	u32 dwBinaryCipherLen = 0;
	HexStr2Byte((const u8*)m_strLicenseInfo.c_str(), m_strLicenseInfo.length(),
		achBinaryCipher, dwBinaryCipherLen);

	u8 achLicenseJson[10240 + 1] = { 0 };
	KdvAES(PLATFORM_LICENSE_AES_KEY, strlen(PLATFORM_LICENSE_AES_KEY),
		MODE_CBC, DIR_DECRYPT, PLATFORM_LICENSE_AES_IV,
		achBinaryCipher, dwBinaryCipherLen, achLicenseJson);
	OspPrintf(TRUE, FALSE, "plainTextLicense:\n%s\n", achLicenseJson);
	OspPrintf(TRUE, FALSE, "CipherLicense(len=%u):\n %s\n", m_strLicenseInfo.length(), m_strLicenseInfo.c_str());
}

static BOOL32 GetDomainAndKeyFromJson(s8 * pchJsonBuf, s8* pchDomain, u32 dwDomainLen, s8* pchRmqKey, u32 dwKeyLen)
{
	json_t_ptr pjRoot = NULL;
	ReturnIfFailed(LoadJson(pchJsonBuf, strlen(pchJsonBuf), pjRoot, FALSE));

	const s8* pchDomainStr = NULL;
	const s8* pchRmqKeyStr = NULL;
	if (!GetStringFromJson(pjRoot, JSON_DOMAIN_MOID, &pchDomainStr)
		|| !GetStringFromJson(pjRoot, JSON_ROUTING_KEY, &pchRmqKeyStr))
	{
		OspPrintf( TRUE, FALSE, "[GetMeetingLimitFromJson]Get Doamin or RoutineKey err\n" );
		return FALSE;
	}

	strncpy(pchDomain, pchDomainStr, dwDomainLen);
	strncpy(pchRmqKey, pchRmqKeyStr, dwKeyLen);
	return TRUE;
}

CBmcLicenseInstance::CBmcLicenseInstance(): m_bIsRcvLisence(FALSE){}
CBmcLicenseInstance::~CBmcLicenseInstance(){}

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
2015/08/25  5.0     ���ڽ�                  ����
=============================================================================*/

void CBmcLicenseInstance::DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp )
{
	if ( NULL ==  pcMsg )
	{
		return;
	}
	
	switch ( pcMsg->event )
	{
	case OSP_POWERON:
		DaemonProcPowerOn( pcMsg );
		break;
	case BMC_LICENSE_CONSUMER_SUC:
		{
			//���ͻ�����Ȩ��Ϣ����
			RequestBmcLicense();
		}
		break;
	case BMC_LICENSE_MESSAGE_CB:
		DaemonProcBmcLicenseMsgCB(pcMsg);
		break;
	case BMC_LICENSE_ASK_CB:
		DaemonProcBmcLicenseAskCB(pcMsg);
		break;
	case BMC_LICENSE_UPDATE_CB:
		DaemonProcBmcLicenseUpdateCB(pcMsg);
		break;
	case BMC_MEETING_LICENSE_TIMER:
		{
			//����涨ʱ��δ�յ�LISENCE�������������Ͷ�ʱ���
			HandleLicenseStatus();
		}
		break;
	default:
		break;
	}	
	return;
}

BOOL32 GetPlatformLicenseInfoFromCipher(const u8* pchCipher, const u32 dwCipherLen, CPlatformConfNumLimit& cLimit, s8 *pchMcuType, u32 dwMcuTypeLen)
{
	u8 achBinaryCipher[10240 + 1] = { 0 };
	u32 dwBinaryCipherLen = 0;
	//bmc����license���ܵ������Ƕ���������
	//ͨ��mq�·�֮������л����ַ�������ʽ
	HexStr2Byte(pchCipher, dwCipherLen, achBinaryCipher, dwBinaryCipherLen);

	u8 achLicenseJson[10240 + 1] = { 0 };
	s32 nRes = KdvAES(PLATFORM_LICENSE_AES_KEY, strlen(PLATFORM_LICENSE_AES_KEY),
		MODE_CBC, DIR_DECRYPT, PLATFORM_LICENSE_AES_IV,
		achBinaryCipher, dwBinaryCipherLen, achLicenseJson);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[%s]KdvAES res: %d\n", __FUNCTION__, nRes);

	const s8* achJsonBuf = (const s8*)achLicenseJson;
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcLicenseData::Content_MQCB]eventcontent is\n");
	LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, achJsonBuf, strlen(achJsonBuf));

	json_t_ptr pjRoot = NULL;
	LoadJson(achJsonBuf, strlen(achJsonBuf), pjRoot, FALSE);
	if (pjRoot == NULL)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[%s]loads jsonbuf err\n", __FUNCTION__);
		return FALSE;
	}

	if (!GetConfNumLimitFromJsonObj(pjRoot, cLimit))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[%s]GetConfNumLimitFromJson fail\n", __FUNCTION__);
		return FALSE;
	}

	json_t_ptr pjMcuType = json_object_get_s(pjRoot, BMCLICENSE_MCU_TYPE);
	if (NULL == pjMcuType)
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[%s]json_object_get MCU Type is NULL, return FALSE!\n", __FUNCTION__);
		return FALSE;
	}
	strncpy(pchMcuType, json_string_value(pjMcuType), dwMcuTypeLen);

	return TRUE;
}

void CBmcLicenseInstance::DaemonProcBmcLicenseMsgCB( CMessage * const pcMsg )
{
	CPlatformConfNumLimit cConfNumLimit;
	s8 achMcuType[256 + 1] = { 0 };
	if (!GetPlatformLicenseInfoFromCipher(pcMsg->content, pcMsg->length, cConfNumLimit, achMcuType, sizeof(achMcuType)-1))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s]GetPlatformConfNumLimitFormcipher Fail\n", __FUNCTION__);
		return;
	}
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[%s]smallLimit: %u, largeLimit: %u, LicenseType: %s\n", __FUNCTION__,
		cConfNumLimit.GetSmallConfNumLimit(), cConfNumLimit.GetLargeConfNumLimit(), achMcuType);
	if (!strncmp(achMcuType, EDU_OCCU, 4))
	{
		g_cMauVCApp.SetIsEduVer(TRUE);
	}
	else
	{
		g_cMauVCApp.SetIsEduVer(FALSE);
	}

	m_bIsRcvLisence = TRUE;
	KillTimer(BMC_MEETING_LICENSE_TIMER);
	g_cBmcLicenseApp.m_strLicenseInfo = string((const s8*)pcMsg->content, pcMsg->length);

	OspPost( MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), BMC_MAU_CONF_LICENSE_NUM,
		(u8*)&cConfNumLimit, sizeof(cConfNumLimit));
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[%s]smallLimit: %u, largeLimit: %u\n", __FUNCTION__,
		cConfNumLimit.GetSmallConfNumLimit(), cConfNumLimit.GetLargeConfNumLimit());
}

BOOL32 ConstructBmcLicenseJson(const s8* pchEncryptLicense, const u32 dwEncryptLicenseLen,
	json_str& strJson, BOOL32 bUpdate)
{
	json_t_ptr pjRoot = json_object();
	if (!pjRoot) return FALSE;
	ReturnIfFailed(SetStringToJson(pjRoot, "event", bUpdate ? "BMC_LICENSE_UPDATE" : "BMC_LICENSE"));
	ReturnIfFailed(SetStringToJson(pjRoot, "data", pchEncryptLicense));
	strJson = pjRoot;
	return (strJson == NULL) ? FALSE : TRUE;
}

void CBmcLicenseInstance::DaemonProcBmcLicenseAskCB(CMessage * const pcMsg)
{
	const s8* pbuf = (const s8*)pcMsg->content;
	s8 achDomainMoid[OPERATION_MAXLEN_MOID] = { 0 };
	s8 achRspKey[100 + 1] = { 0 };
	if (!GetDomainAndKeyFromJson((s8*)pbuf, achDomainMoid, sizeof(achDomainMoid), achRspKey, sizeof(achRspKey)))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[DaemonProcBmcLicenseAskCB]GetDomainAndKeyFromJson Fail\n");
		return;
	}
	CKdvDomainInfo cDomainInfo;
	if( !g_cMauVCApp.IsDomainExist(achDomainMoid) 
		|| !g_cMauVCApp.GetDomainInfo( achDomainMoid,cDomainInfo ) 
		|| !cDomainInfo.IsUsedFlag()
		|| cDomainInfo.GetDomainLevel() != CKdvDomainInfo::em_KdvPlatformDomain)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[DaemonProcBmcLicenseAskCB]Domain from requester is invalid\n");
		return ;
	}

	if(!g_cBmcLicenseApp.m_strLicenseInfo.empty())
	{
		json_str strJson;
		ConstructBmcLicenseJson(
			g_cBmcLicenseApp.m_strLicenseInfo.c_str(), 
			g_cBmcLicenseApp.m_strLicenseInfo.length(),
			strJson, FALSE);

		if (RT_OK == PostMsg(g_cBmcLicenseApp.s_nBmcLicenseRabbitMq,
			strJson, strlen(strJson),
			achRspKey, MQ_PLATFORM_BMC_LICENSE_EX, 0, 0, NULL, NULL, TIMER_SPAN_LICENSE_REQ))
		{
			LogPrint( LOG_LVL_DETAIL, MID_MAU_BMC, "[DaemonProcBmcLicenseAskCB]"
				"producer: %u produce msg to ex: %s key: %s\n", g_cBmcLicenseApp.s_nBmcLicenseRabbitMq, MQ_PLATFORM_BMC_LICENSE_EX, achRspKey);
		}
		else
		{
			LogPrint( LOG_LVL_DETAIL, MID_MAU_BMC, "PostMsg bmclicense info err\n");
		}
	}
	else
	{
			LogPrint( LOG_LVL_DETAIL, MID_MAU_BMC, "bmclicense info is empty\n");
	}
}

void CBmcLicenseInstance::DaemonProcBmcLicenseUpdateCB(CMessage * const pcMsg)
{
	CPlatformConfNumLimit cConfNumLimit;
	s8 achMcuType[256 + 1] = { 0 };
	if (!GetPlatformLicenseInfoFromCipher(pcMsg->content, pcMsg->length, cConfNumLimit, achMcuType, sizeof(achMcuType) - 1))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s]GetPlatformConfNumLimitFormcipher Fail\n", __FUNCTION__);
		return;
	}
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[%s]smallLimit: %u, largeLimit: %u, McuType: %s\n", __FUNCTION__,
		cConfNumLimit.GetSmallConfNumLimit(), cConfNumLimit.GetLargeConfNumLimit(), achMcuType);
	if (!strncmp(achMcuType, EDU_OCCU, 4))
	{
		g_cMauVCApp.SetIsEduVer(TRUE);
	}
	else
	{
		g_cMauVCApp.SetIsEduVer(FALSE);
	}

	m_bIsRcvLisence = TRUE;
	KillTimer(BMC_MEETING_LICENSE_TIMER);
	g_cBmcLicenseApp.m_strLicenseInfo = string((const s8*)pcMsg->content, pcMsg->length);
	if(!g_cBmcLicenseApp.m_strLicenseInfo.empty())
	{
		OspPost( MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), BMC_MAU_CONF_LICENSE_NUM,
			(u8*)&cConfNumLimit, sizeof(cConfNumLimit));

		json_str strJson;
		ConstructBmcLicenseJson(
			g_cBmcLicenseApp.m_strLicenseInfo.c_str(),
			g_cBmcLicenseApp.m_strLicenseInfo.length(),
			strJson, TRUE);
		if (RT_OK == PostMsg(g_cBmcLicenseApp.s_nBmcLicenseRabbitMq,
			strJson, strlen(strJson),
			MQ_PLATFORM_BMC_LICENSE_NOTIFY_K,
			MQ_PLATFORM_BMC_LICENSE_EX,
			0, 0, NULL, NULL, TIMER_SPAN_LICENSE_REQ))
		{
			LogPrint( LOG_LVL_DETAIL, MID_MAU_BMC, "[DaemonProcBmcLicenseUpdateCB]producer: %u produce msg to ex: %s key: %s\n",
				g_cBmcLicenseApp.s_nBmcLicenseRabbitMq, MQ_PLATFORM_BMC_LICENSE_EX, MQ_PLATFORM_BMC_LICENSE_NOTIFY_K);
		}
		else
		{
			LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "PostMsg bmclicense info err\n");
		}
	}
	else
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "bmclicense info is empty\n");
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
2015/08/25  5.0     ���ڽ�                  ����
=============================================================================*/

void CBmcLicenseInstance::DaemonProcPowerOn( CMessage * const pcMsg )
{
	NextState( STATE_IDLE );
	
	g_cBmcLicenseApp.Init_MQPC();
	LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[CBmcLicenseData::DaemonProcPowerOn]Init license about mq.\n" );
	return;
}

void CBmcLicenseInstance::InstanceEntry( CMessage * const pcMsg ){}

void CBmcLicenseInstance::RequestBmcLicense()
{
	g_cBmcLicenseApp.SendLicenseReqMsgToBMC();
	SetTimer(BMC_MEETING_LICENSE_TIMER, TIMER_SPAN_LICENSE_REQ);
}

void CBmcLicenseInstance::HandleLicenseStatus()
{
	if (!m_bIsRcvLisence)
	{
		RequestBmcLicense();
	}
	else
	{
		KillTimer(BMC_MEETING_LICENSE_TIMER);
	}
}
