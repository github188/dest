


#include "httpssn.h"
#include "constuls.h"
#include "debugprint.h"
#include "structnack.h"
#include "config.h"
#include "bossutils.h"
#include "structlogin.h"
#include "eventself.h"
#include "structextend.h"
#include "structulc.h"

#undef WIN32 

CHttpSsnApp g_cHttpSsnApp;

#ifndef WIN32
const char cert_pem[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIICpjCCAZCgAwIBAgIESEPtjjALBgkqhkiG9w0BAQUwADAeFw0wODA2MDIxMjU0\n"
    "MzhaFw0wOTA2MDIxMjU0NDZaMAAwggEfMAsGCSqGSIb3DQEBAQOCAQ4AMIIBCQKC\n"
    "AQC03TyUvK5HmUAirRp067taIEO4bibh5nqolUoUdo/LeblMQV+qnrv/RNAMTx5X\n"
    "fNLZ45/kbM9geF8qY0vsPyQvP4jumzK0LOJYuIwmHaUm9vbXnYieILiwCuTgjaud\n"
    "3VkZDoQ9fteIo+6we9UTpVqZpxpbLulBMh/VsvX0cPJ1VFC7rT59o9hAUlFf9jX/\n"
    "GmKdYI79MtgVx0OPBjmmSD6kicBBfmfgkO7bIGwlRtsIyMznxbHu6VuoX/eVxrTv\n"
    "rmCwgEXLWRZ6ru8MQl5YfqeGXXRVwMeXU961KefbuvmEPccgCxm8FZ1C1cnDHFXh\n"
    "siSgAzMBjC/b6KVhNQ4KnUdZAgMBAAGjLzAtMAwGA1UdEwEB/wQCMAAwHQYDVR0O\n"
    "BBYEFJcUvpjvE5fF/yzUshkWDpdYiQh/MAsGCSqGSIb3DQEBBQOCAQEARP7eKSB2\n"
    "RNd6XjEjK0SrxtoTnxS3nw9sfcS7/qD1+XHdObtDFqGNSjGYFB3Gpx8fpQhCXdoN\n"
    "8QUs3/5ZVa5yjZMQewWBgz8kNbnbH40F2y81MHITxxCe1Y+qqHWwVaYLsiOTqj2/\n"
    "0S3QjEJ9tvklmg7JX09HC4m5QRYfWBeQLD1u8ZjA1Sf1xJriomFVyRLI2VPO2bNe\n"
    "JDMXWuP+8kMC7gEvUnJ7A92Y2yrhu3QI3bjPk8uSpHea19Q77tul1UVBJ5g+zpH3\n"
    "OsF5p0MyaVf09GTzcLds5nE/osTdXGUyHJapWReVmPm3Zn6gqYlnzD99z+DPIgIV\n"
    "RhZvQx74NQnS6g==\n" "-----END CERTIFICATE-----\n";

const char key_pem[] =
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "MIIEowIBAAKCAQEAtN08lLyuR5lAIq0adOu7WiBDuG4m4eZ6qJVKFHaPy3m5TEFf\n"
    "qp67/0TQDE8eV3zS2eOf5GzPYHhfKmNL7D8kLz+I7psytCziWLiMJh2lJvb2152I\n"
    "niC4sArk4I2rnd1ZGQ6EPX7XiKPusHvVE6VamacaWy7pQTIf1bL19HDydVRQu60+\n"
    "faPYQFJRX/Y1/xpinWCO/TLYFcdDjwY5pkg+pInAQX5n4JDu2yBsJUbbCMjM58Wx\n"
    "7ulbqF/3lca0765gsIBFy1kWeq7vDEJeWH6nhl10VcDHl1PetSnn27r5hD3HIAsZ\n"
    "vBWdQtXJwxxV4bIkoAMzAYwv2+ilYTUOCp1HWQIDAQABAoIBAArOQv3R7gmqDspj\n"
    "lDaTFOz0C4e70QfjGMX0sWnakYnDGn6DU19iv3GnX1S072ejtgc9kcJ4e8VUO79R\n"
    "EmqpdRR7k8dJr3RTUCyjzf/C+qiCzcmhCFYGN3KRHA6MeEnkvRuBogX4i5EG1k5l\n"
    "/5t+YBTZBnqXKWlzQLKoUAiMLPg0eRWh+6q7H4N7kdWWBmTpako7TEqpIwuEnPGx\n"
    "u3EPuTR+LN6lF55WBePbCHccUHUQaXuav18NuDkcJmCiMArK9SKb+h0RqLD6oMI/\n"
    "dKD6n8cZXeMBkK+C8U/K0sN2hFHACsu30b9XfdnljgP9v+BP8GhnB0nCB6tNBCPo\n"
    "32srOwECgYEAxWh3iBT4lWqL6bZavVbnhmvtif4nHv2t2/hOs/CAq8iLAw0oWGZc\n"
    "+JEZTUDMvFRlulr0kcaWra+4fN3OmJnjeuFXZq52lfMgXBIKBmoSaZpIh2aDY1Rd\n"
    "RbEse7nQl9hTEPmYspiXLGtnAXW7HuWqVfFFP3ya8rUS3t4d07Hig8ECgYEA6ou6\n"
    "OHiBRTbtDqLIv8NghARc/AqwNWgEc9PelCPe5bdCOLBEyFjqKiT2MttnSSUc2Zob\n"
    "XhYkHC6zN1Mlq30N0e3Q61YK9LxMdU1vsluXxNq2rfK1Scb1oOlOOtlbV3zA3VRF\n"
    "hV3t1nOA9tFmUrwZi0CUMWJE/zbPAyhwWotKyZkCgYEAh0kFicPdbABdrCglXVae\n"
    "SnfSjVwYkVuGd5Ze0WADvjYsVkYBHTvhgRNnRJMg+/vWz3Sf4Ps4rgUbqK8Vc20b\n"
    "AU5G6H6tlCvPRGm0ZxrwTWDHTcuKRVs+pJE8C/qWoklE/AAhjluWVoGwUMbPGuiH\n"
    "6Gf1bgHF6oj/Sq7rv/VLZ8ECgYBeq7ml05YyLuJutuwa4yzQ/MXfghzv4aVyb0F3\n"
    "QCdXR6o2IYgR6jnSewrZKlA9aPqFJrwHNR6sNXlnSmt5Fcf/RWO/qgJQGLUv3+rG\n"
    "7kuLTNDR05azSdiZc7J89ID3Bkb+z2YkV+6JUiPq/Ei1+nDBEXb/m+/HqALU/nyj\n"
    "P3gXeQKBgBusb8Rbd+KgxSA0hwY6aoRTPRt8LNvXdsB9vRcKKHUFQvxUWiUSS+L9\n"
    "/Qu1sJbrUquKOHqksV5wCnWnAKyJNJlhHuBToqQTgKXjuNmVdYSe631saiI7PHyC\n"
    "eRJ6DxULPxABytJrYCRrNqmXi5TCiqR2mtfalEMOPxz8rUU8dYyx\n"
    "-----END RSA PRIVATE KEY-----\n";


int answer_to_connection (void *cls, struct MHD_Connection *connection,
                const char *url, const char *method,
                const char *version, const char *upload_data, size_t *upload_data_size, 
                uint32_t client_ip_addr, uint32_t local_ip_addr, uint16_t local_ip_port, void **con_cls)
{
    client_ip_addr = ntohl(client_ip_addr);  //转主机序
    HttpSsnLog(LOG_LVL_KEYSTATUS, "ClientIp:%s, url:%s, method name:%s local_ip_port.%d\n", BossUtils::GetIpString(client_ip_addr).c_str(), url, method, local_ip_port);

    //健康监测
    if ( 0 == strcmp(url, "/status") )
    {
        const char *szStatus = "<html><body>health status OK!</body></html>";
        struct MHD_Response* response = MHD_create_response_from_buffer (strlen (szStatus), (void *) szStatus, MHD_RESPMEM_PERSISTENT);
        int ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
        MHD_destroy_response (response);

        return ret;
    }

    //不支持的函数
    if ( 0 != strcmp(url, "/v5/aps/login") || 0 != strcmp(method, "GET") )
    {
        HttpSsnLog(LOG_LVL_WARNING, "不识别的method或URL\n");	

        const char *szError = "<html><body>only support method:GET url:/v5/aps/login !</body></html>";
        struct MHD_Response* response = MHD_create_response_from_buffer (strlen (szError), (void *) szError, MHD_RESPMEM_PERSISTENT);
        int ret = MHD_queue_response (connection, MHD_HTTP_METHOD_NOT_ALLOWED, response);
        MHD_destroy_response (response);

        return ret;
    }

    //////////////////////////////////////////登录参数分析/////////////////////////////////////////////////////
    BOOL32 bParse = TRUE;
    const s8* szVersion     = MHD_lookup_connection_value (connection, MHD_GET_ARGUMENT_KIND, HTTP_ARGUMENT_VERSION);
    if (NULL == szVersion)
    {
        bParse = FALSE;
        HttpSsnLog(LOG_LVL_WARNING, "参数:%s 不存在\n", HTTP_ARGUMENT_VERSION);	
    }
    
    const s8* szUserName    = MHD_lookup_connection_value (connection, MHD_GET_ARGUMENT_KIND, HTTP_ARGUMENT_USERNAME);
    if (NULL == szUserName || strlen(szUserName) <= 0)
    {
        bParse = FALSE;
        HttpSsnLog(LOG_LVL_WARNING, "参数:%s 不存在\n", HTTP_ARGUMENT_USERNAME);	
    }

    const s8* szPassWord    = MHD_lookup_connection_value (connection, MHD_GET_ARGUMENT_KIND, HTTP_ARGUMENT_PASSWORD);
    if (NULL == szPassWord)
    {
        bParse = FALSE;
        HttpSsnLog(LOG_LVL_WARNING, "参数:%s 不存在\n", HTTP_ARGUMENT_PASSWORD);	
    }

    const s8* szOemMark     = MHD_lookup_connection_value (connection, MHD_GET_ARGUMENT_KIND, HTTP_ARGUMENT_OEMMARK);
    if (NULL == szOemMark)
    {
        //bParse = FALSE;   //可以不带OemMark
        HttpSsnLog(LOG_LVL_WARNING, "参数:%s 不存在\n", HTTP_ARGUMENT_OEMMARK);	
    }

    const s8* szDeviceType  = MHD_lookup_connection_value (connection, MHD_GET_ARGUMENT_KIND, HTTP_ARGUMENT_DEVICETYPE);
    if (NULL == szDeviceType)
    {
        bParse = FALSE;
        HttpSsnLog(LOG_LVL_WARNING, "参数:%s 不存在\n", HTTP_ARGUMENT_DEVICETYPE);	
    }

    const s8* szApsLocalIp    = MHD_lookup_connection_value (connection, MHD_GET_ARGUMENT_KIND, HTTP_ARGUMENT_APSLOCALIP);
    if (NULL == szApsLocalIp)
    {
        bParse = FALSE;
        HttpSsnLog(LOG_LVL_WARNING, "参数:%s 不存在\n", HTTP_ARGUMENT_APSLOCALIP);	
    }

    if ( !bParse )
    { 
        const char *szError = "<html><body>only support method:GET url:/aps/v5/login !</body></html>";
        struct MHD_Response* response = MHD_create_response_from_buffer (strlen (szError), (void *) szError, MHD_RESPMEM_PERSISTENT);
        int ret = MHD_queue_response (connection, MHD_HTTP_BAD_REQUEST, response);
        MHD_destroy_response (response);

        return ret;
    }

    //////////////////////////////////////////cookie分析/////////////////////////////////////////////////////
    const s8* szClientIP = MHD_lookup_connection_value (connection, MHD_HEADER_KIND, HTTP_COOKIE_CLIENT_IP);
    if (NULL != szClientIP)
    {
        HttpSsnLog(LOG_LVL_WARNING, "client ip:%s \n", szClientIP);
        //已cookie中的IP地址为准(因为Haproxy会篡改源地址)
        client_ip_addr = BossUtils::GetIpByString(szClientIP);
    }
    else
    {
        HttpSsnLog(LOG_LVL_WARNING, "MHD_COOKIE_KIND(%s) 不存在\n", HTTP_COOKIE_CLIENT_IP);	
    }    
    //////////////////////////////////////////cookie分析/////////////////////////////////////////////////////
    
    //
    CLoginInfo tLoginInfo;
    tLoginInfo.SetUserName(szUserName);
    tLoginInfo.SetEncryptedAccountPwd(szPassWord);
    tLoginInfo.SetDeviceType(szDeviceType);
    tLoginInfo.SetOemMark(szOemMark);
    tLoginInfo.SetApsLocalIp(BossUtils::GetIpByString(szApsLocalIp));
    tLoginInfo.SetClientIp(client_ip_addr); 
    
    if( g_cHttpSsnApp.IsHttpsPort(ntohs(local_ip_port)) )
    {
    	tLoginInfo.SetIsHttpsClient( TRUE );
    }

    CLoginExtend tExtend;
    tExtend.SetHttpConnection(connection);    //记住把Http句柄
    tExtend.SetDetailBody(tLoginInfo);
    //启动http
    ::OspPost(MAKEIID(AID_APS_HTTP_SSN, CInstance::DAEMON), MT_APS_LOGIN_REQ, &tExtend, sizeof(tExtend));
    HttpSsnLog(LOG_LVL_KEYSTATUS, "MT:%s; %s:%s; %s:%s; %s:%s; %s:%s; %s:%s; %s:%s IsHttps.%d\n", 
        BossUtils::GetIpString(client_ip_addr).c_str(), 
        HTTP_ARGUMENT_VERSION, szVersion,
        HTTP_ARGUMENT_USERNAME, tLoginInfo.GetUserName(), 
        HTTP_ARGUMENT_PASSWORD, tLoginInfo.GetPassWord(), 
        HTTP_ARGUMENT_DEVICETYPE, tLoginInfo.GetDeviceType(), 
        HTTP_ARGUMENT_OEMMARK, tLoginInfo.GetOemMark(), 
        HTTP_ARGUMENT_APSLOCALIP, BossUtils::GetIpString(tLoginInfo.GetApsLocalIp()).c_str(),
        	tLoginInfo.IsHttpsClient() );	

    return MHD_YES;
}
#endif  //WIN32

CHttpSsnData::CHttpSsnData()
{
#ifndef WIN32
    m_pHttpDaemon = NULL;
    m_pHttpsDaemon = NULL;
#endif  //WIN32
}

CHttpSsnData::~CHttpSsnData()
{
#ifndef WIN32
    if (NULL != m_pHttpDaemon)
    {
        MHD_stop_daemon (m_pHttpDaemon);
        m_pHttpDaemon = NULL;
    }
    if (NULL != m_pHttpsDaemon)
    {
        MHD_stop_daemon (m_pHttpsDaemon);
        m_pHttpsDaemon = NULL;
    }
#endif  //WIN32
}

#ifndef WIN32
void CHttpSsnData::StartHttpService()
{
    m_pHttpDaemon ;

    while ( NULL == (m_pHttpsDaemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY|MHD_USE_DEBUG|MHD_USE_SSL, 
        g_cCfgFile.GetHttpsListenPort(),     
        NULL, NULL,                           //IP过滤回调函数
        &answer_to_connection, NULL,          //连接回调函数
        MHD_OPTION_CONNECTION_TIMEOUT, 10,    //超时时间(秒)
        MHD_OPTION_HTTPS_MEM_KEY,key_pem,
        MHD_OPTION_HTTPS_MEM_CERT, cert_pem,
        MHD_OPTION_END)) )
    {
    	HttpSsnLog(LOG_LVL_WARNING, "MHD_start_daemon(ListenPort.%u) Err, application program will try late...\n", g_cCfgFile.GetHttpsListenPort());	
      OspTaskDelay(3 * 1000);   
  	}


     while ( NULL == (m_pHttpDaemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY|MHD_USE_DEBUG, 
                                       g_cCfgFile.GetHttpListenPort(),       
                                       NULL, NULL,                           //IP过滤回调函数
                                       &answer_to_connection, NULL,          //连接回调函数
                                       MHD_OPTION_CONNECTION_TIMEOUT, 30,    //超时时间(秒)                                       
                                       MHD_OPTION_END)) )
    {
        HttpSsnLog(LOG_LVL_WARNING, "MHD_start_daemon(ListenPort.%u) Err, application program will try late...\n", g_cCfgFile.GetHttpListenPort());	
        OspTaskDelay(3 * 1000);       
    }

    HttpSsnLog(LOG_LVL_KEYSTATUS, "MHD_start_daemon(ListenPort.Http.%u Https.%u) succeed!\n", g_cCfgFile.GetHttpListenPort(),g_cCfgFile.GetHttpsListenPort()  );
}

BOOL32 CHttpSsnData::IsHttpsPort(const u16 &wPort)
{
	if( NULL == m_pHttpsDaemon )
	{
		return FALSE;
	}
	
	if( 0 != IsMHD_DaemonListenPort(m_pHttpsDaemon,wPort) )
	{
		return TRUE;
	}
	
	return FALSE;
}
#endif  //WIN32



/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
CHttpSsnInst::CHttpSsnInst()
{

}

CHttpSsnInst::~CHttpSsnInst()
{

}

void CHttpSsnInst::DaemonInstanceEntry( CMessage *const pcMsg, CApp* pcApp )
{
    if (NULL == pcMsg || NULL == pcApp)
    {
        HttpSsnLog(LOG_LVL_ERROR, "[DaemonInstanceEntry] The received msg's pointer is NULL!\n");
        return;
    }
    HttpSsnLog(LOG_LVL_KEYSTATUS, "[DaemonInstanceEntry]Receive event  (%u)%s \n", pcMsg->event, ::OspEventDesc(pcMsg->event));

    switch (pcMsg->event)
    {
    case OSP_POWERON:   //上电处理
        DaemonProcPowerOn(pcMsg);
        break;

    case MT_APS_LOGIN_REQ:
        ProcUlcLoginReq(pcMsg);
        break;

    case APS_MT_LOGIN_ACK:
        ProcUlcLoginAck(pcMsg);
        break;

    case APS_MT_LOGIN_NACK:
        ProcUlcLoginNack(pcMsg);
        break;

    default:
        HttpSsnLog(LOG_LVL_ERROR, "[DaemonInstanceEntry] received wrong message %u(%s)!\n", pcMsg->event, ::OspEventDesc(pcMsg->event));
        break;
    }

    return;
}

void CHttpSsnInst::InstanceEntry( CMessage *const pcMsg )
{

}

void CHttpSsnInst::DaemonProcPowerOn( CMessage *const pcMsg )
{
    if (NULL == pcMsg)
    {
        HttpSsnLog(LOG_LVL_ERROR, "[DaemonProcPowerOn] The received msg's pointer is NULL!\n");
        return;
    }
 
#ifndef WIN32
    // 启动http服务
    g_cHttpSsnApp.StartHttpService();
#endif //WIN32
}

void CHttpSsnInst::ProcUlcLoginReq( CMessage *const pcMsg )
{
    if (NULL == pcMsg || NULL == pcMsg->content)
    {
        HttpSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginReq] NULL == pcMsg || NULL == pcMsg->content! \n");
        return;
    }

    CLoginExtend tExtend = *(CLoginExtend*)pcMsg->content;
    tExtend.SetSessionID(GetAppID(), GetInsID());

    PostMsgToCtrl(pcMsg->event, (u8 *)&tExtend, sizeof(tExtend));
}

void CHttpSsnInst::ProcUlcLoginAck( CMessage *const pcMsg )
{
    if (NULL == pcMsg)
    {
        HttpSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginAck] The receive msg's pointer is NULL!\n");
        return;
    }

    if (NULL == pcMsg->content || sizeof(CUlcFullInfoExtend) != pcMsg->length)
    {
        HttpSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginAck]NULL == pcMsg->content || sizeof(CUlcFullInfoExtend) != pcMsg->length!\n");
        return;
    }

    CUlcFullInfoExtend* pExtend = (CUlcFullInfoExtend*)pcMsg->content;
    CUlcFullInfo tFullInfo = pExtend->GetDetailBody();
    Json::Value jsonValue = tFullInfo.StructToJson();
    std::string strJson = jsonValue.toStyledString();

#ifdef _LINUX_
		struct MHD_Daemon* pHttpDaemon = NULL;
		if( pExtend->IsHttpsConnection() )
			{
				pHttpDaemon = g_cHttpSsnApp.GetHttpsDaemon();
			}
			else
			{
				pHttpDaemon = g_cHttpSsnApp.GetHttpDaemon();
			}
    PostMsgToMT(pHttpDaemon,(struct MHD_Connection*)pExtend->GetHttpConnection(), MHD_HTTP_OK, strJson.c_str(), strJson.length()+1);
#endif //_LINUX_

    HttpSsnLog(LOG_LVL_KEYSTATUS, "[ProcUlcLoginAck] E164(%s) login succ! MT Full info:\n %s \n", tFullInfo.GetMtInfo().GetE164NO(), strJson.c_str());
    return;
}

void CHttpSsnInst::ProcUlcLoginNack( CMessage *const pcMsg )
{
    if (NULL == pcMsg)
    {
        HttpSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginNack] The receive msg's pointer is NULL!\n");
        return;
    }

    if (NULL == pcMsg->content || sizeof(CUlsBaseNackExtend) != pcMsg->length)
    {
        HttpSsnLog(LOG_LVL_ERROR, "[ProcUlcLoginNack]NULL == pcMsg->content || sizeof(CUlsBaseNackExtend) != pcMsg->length!\n");
        return;
    }

    CUlsBaseNackExtend* pExtend = (CUlsBaseNackExtend*)pcMsg->content;
    CApsBaseNack tNack = pExtend->GetDetailBody();
    Json::Value jsonVale = tNack.StructToJson();
    std::string strJson = jsonVale.toStyledString();

#ifndef WIN32
		struct MHD_Daemon* pHttpDaemon = NULL;
		if( pExtend->IsHttpsConnection() )
			{
				pHttpDaemon = g_cHttpSsnApp.GetHttpsDaemon();
			}
			else
			{
				pHttpDaemon = g_cHttpSsnApp.GetHttpDaemon();
			}
    PostMsgToMT(pHttpDaemon,(struct MHD_Connection*)pExtend->GetHttpConnection(), MHD_HTTP_OK, strJson.c_str(), strJson.length()+1);
#endif  //WIN32

    HttpSsnLog(LOG_LVL_WARNING, "[ProcUlcLoginNack] login fail!\n");
    return;
}

BOOL32 CHttpSsnInst::PostMsgToCtrl( u16 wEvent, u8 *const pbyMsg, u16 wLen )
{
    if (OSP_OK != post(MAKEIID(AID_APS_CENTERCTRL, CInstance::DAEMON), wEvent, pbyMsg, wLen))
    {
        HttpSsnLog(LOG_LVL_ERROR, "[PostMsgToCtrl] Failed - Inst.%u CurState(%u) message %u(%s).\n",
            GetInsID(), CurState(), wEvent, ::OspEventDesc(wEvent));
        return FALSE;
    }

    HttpSsnLog(LOG_LVL_DETAIL, "[PostMsgToCtrl] Inst.%u CurState(%u) post message %u(%s) to CCtrl.\n",
        GetInsID(), CurState(), wEvent, ::OspEventDesc(wEvent));

    return TRUE;
}

BOOL32 CHttpSsnInst::PostMsgToMT(struct MHD_Daemon* m_pHttpDaemon,struct MHD_Connection* handleConnect, u16 wHttpCode, const void* pbyMsg, u16 wLen)
{
#ifndef WIN32
    int nRet = MHD_response_from_buffer_async(m_pHttpDaemon, handleConnect, wHttpCode, wLen, pbyMsg, MHD_RESPMEM_MUST_COPY);
    if (nRet != MHD_YES)
    {
        HttpSsnLog(LOG_LVL_ERROR, "[PostMsgToMT] Failed \n");
    }
#endif //WIN32

    return TRUE;
}

