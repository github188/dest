/*****************************************************************************
模块名      : MAU
文件名      : bmclicense.cpp
创建时间    : 2015年 08月 25日
实现功能    : 
作者        : 吕冠娇
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/08/25  5.0        吕冠娇          创建
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

//平台域
s32 CBmcLicenseData::s_nBmcLicenseRabbitMq = 0;
std::string CBmcLicenseData::s_strPlatformRoutingKey;
std::string CBmcLicenseData::s_strPlatformQName;

//核心域
s32 CBmcLicenseData::s_nTopLevelDomainRabbitMq = 0;
std::string CBmcLicenseData::s_strTopLevelRoutingKey;
std::string CBmcLicenseData::s_strTopLevelQName;

#define PLATFORM_LICENSE_AES_KEY					   "DKshxQgZpLMNGx7P"
#define PLATFORM_LICENSE_AES_IV				     	   "JpxFCig2bUahtnEO"
#define JSON_ROUTING_KEY           "routingKey"					  //Routing Key
#define JSON_DOMAIN_MOID           "platformDomainMoid"			  //平台 moid
#define TIMER_EVENT_LICENSE_REQ   6000									  //会议授权数定时器
#define TIMER_SPAN_LICENSE_REQ    60000                                  //会议授权数定时器间隔数
#define TIMER_SPAN_GET_GUID_MOID  1000									 //获取平台MOID&&GUID定时器

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
函 数 名： Init_MQPC
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/08/25  5.0     吕冠娇                  创建
=============================================================================*/
BOOL32 CBmcLicenseData::Init_MQPC()
{
	//创建生产者和消费者以获取核心域的license
	s_nTopLevelDomainRabbitMq = SafeCreateProducer(
		g_cMauVCApp.m_achTopLevelMqIP, 5672, "/", "dev", "dev",
		ProducerSuccCB, ProducerFailCB);
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
					NULL, NULL, TopLevelConsumer_ConSuc_MQCB, ConsumerFailCB, 0 );
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
					NULL, NULL, ConsumerSuccCB, ConsumerFailCB, 0 );
	if (FALSE == nRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s]CreateConSumer<%s> False\n",
			__FUNCTION__, topLevelBmcLicenseNotifyQ.c_str());
	}

	// 创建生产者和消费者以发布从核心域获取的license
	s_nBmcLicenseRabbitMq = SafeCreateP( ProducerSuccCB, ProducerFailCB );
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
						NULL, NULL, ConsumerSuccCB, ConsumerFailCB, 0 );
	if (FALSE == nRet)
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s]CreateConSumer<platform.license.q> False\n", 
			__FUNCTION__);
	}

	return TRUE;
}

/*=============================================================================
函 数 名： Content_MQCB
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/08/25  5.0     吕冠娇                  创建
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

/*=============================================================================
函 数 名： Consumer_ConSuc_MQCB
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/08/25  5.0     吕冠娇                  创建
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
			//改由向核心域请求License
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
函 数 名： DaemonInstanceEntry
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/08/25  5.0     吕冠娇                  创建
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
			//发送会议授权信息请求
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
			//如果规定时间未收到LISENCE则继续发送请求和定时检查
			HandleLicenseStatus();
		}
		break;
	default:
		break;
	}	
	return;
}

BOOL32 GetPlatformConfNumLimitFromCipher(const u8* pchCipher, const u32 dwCipherLen, CPlatformConfNumLimit& cLimit)
{
	u8 achBinaryCipher[10240 + 1] = { 0 };
	u32 dwBinaryCipherLen = 0;
	//bmc根据license加密的密文是二进制数据
	//通过mq下发之后会序列化成字符串的形式
	HexStr2Byte(pchCipher, dwCipherLen, achBinaryCipher, dwBinaryCipherLen);

	u8 achLicenseJson[10240 + 1] = { 0 };
	s32 nRes = KdvAES(PLATFORM_LICENSE_AES_KEY, strlen(PLATFORM_LICENSE_AES_KEY),
		MODE_CBC, DIR_DECRYPT, PLATFORM_LICENSE_AES_IV,
		achBinaryCipher, dwBinaryCipherLen, achLicenseJson);
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[%s]KdvAES res: %d\n", __FUNCTION__, nRes);

	const s8* achJsonBuf = (const s8*)achLicenseJson;
	LogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[CBmcLicenseData::Content_MQCB]eventcontent is\n");
	LongLogPrint(LOG_LVL_KEYSTATUS, MID_MAU_BMC, achJsonBuf, strlen(achJsonBuf));
	if (!GetConfNumLimitFromJson(achJsonBuf, cLimit))
	{
		LogPrint(LOG_LVL_ERROR, MID_MAU_BMC, "[%s]GetConfNumLimitFromJson fail\n", __FUNCTION__);
		return FALSE;
	}
	return TRUE;
}

void CBmcLicenseInstance::DaemonProcBmcLicenseMsgCB( CMessage * const pcMsg )
{
	CPlatformConfNumLimit cConfNumLimit;
	if (!GetPlatformConfNumLimitFromCipher(pcMsg->content, pcMsg->length, cConfNumLimit))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s]GetPlatformConfNumLimitFormcipher Fail\n", __FUNCTION__);
		return;
	}
	LogPrint( LOG_LVL_KEYSTATUS, MID_MAU_BMC, "[%s]smallLimit: %u, largeLimit: %u\n", __FUNCTION__,
		cConfNumLimit.GetSmallConfNumLimit(), cConfNumLimit.GetLargeConfNumLimit());

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
	if (!GetPlatformConfNumLimitFromCipher(pcMsg->content, pcMsg->length, cConfNumLimit))
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_BMC, "[%s]GetPlatformConfNumLimitFormcipher Fail\n", __FUNCTION__);
		return;
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
函 数 名： DaemonProcPowerOn
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/08/25  5.0     吕冠娇                  创建
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
