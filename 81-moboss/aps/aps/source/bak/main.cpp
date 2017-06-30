
/////////////
#include "rabbitmqwrapper.h"
///////////
#include "kdvtype.h"
#include "bossutils.h"
#include "bossversion.h"
//#include "watchdog.h"
#include "bossconst.h"
#include "config.h"
#include "telnetportconst.h"
#include "lognameconst.h"
#include "kdvlog.h"
#include "loguserdef.h"
#include "operationconst.h"

#include "debugprint.h"
#include "centerctrl.h"
#include "old_ulcssn.h"
// #ifndef _APS_PXY_
// ApsPxy不需要sa认证，但还需要入网，待规格确认要不要新加一个ApsPxy的入网类型，还是和以前aps共用
#include "connectsa_api.h"
// #endif
#include "consassn.h"
#include "httpssn.h"

#ifdef WIN32
#include <direct.h>
#endif

#ifdef _LINUX_
#include "captureexception.h"

#endif

using namespace BossUtils;


using namespace rabbitmqwrap;

BOOL32 g_bQuit = FALSE;

BOOL32 InitRBMQ( CMqConfig &tMqConf,BOOL32 bIsNeedInit);

void Content_MQCB( amqp_connection_state_t conn, void *pbuf, size_t len, response_type *prsp_type, char* pchTag )
{
if ( NULL == pbuf )
	{
		*prsp_type = RT_NACK;
		return;
	}

	*prsp_type = RT_ACK;


	//g_cCenterCtrlApp.TakeInitLock();

	g_cCenterCtrlApp.ParseMqMsg( pbuf,len );
//	printf( "[Content_MQCB]   body: %s\n", (char *)pbuf );
}

void Consumer_ConSuc_MQCB( amqp_connection_state_t conn, char *desc, char* pchTag )
{
	char achSucDec[128] = {0};
	if ( NULL != desc )
	{
		strncpy( achSucDec, desc, sizeof(achSucDec) - 1 );
	}
	
	printf( "[Consumer_ConSuc_MQCB]%s.\n", achSucDec );
}

void Consumer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv, char* pchTag )
{
	char achExpect[128] = {0};
	char achRecv[128] = {0};
	if ( NULL != expect  )
	{
		strncpy( achExpect, expect, sizeof(achExpect) - 1 );
	}
	if ( NULL != precv  )
	{
		strncpy( achRecv, precv, sizeof(achRecv) - 1 );
	}
	
	printf( "[Consumer_ConFail_MQCB]expect.%s.\n", achExpect );
	printf( "[Consumer_ConFail_MQCB]recv.%s.\n", achRecv );
/*
	CMqConfig tMqConf = g_cCfgFile.GetMqConInfo();
	OspDelay( 2000 );
	if(!InitRBMQ(tMqConf,FALSE))
	{
		printf( "[Consumer_ConFail_MQCB] InitRBMQ fial!!!\n" );	
		LogPrint(LOG_LVL_KEYSTATUS,MID_ENTRANCE_APS, "[Consumer_ConFail_MQCB] InitRBMQ fial!!!\n" );
	}
*/
}


BOOL32 InitRBMQ( CMqConfig &tMqConf,BOOL32 bIsNeedInit)
{
	if ( !CCfg::IsValidIp(tMqConf.GetIP()))
	{
        printf("[%s:%d]Mq Ip:%s is Not Valid\n",__FUNCTION__, __LINE__, tMqConf.GetIP());        
		return FALSE;
	}

	std::string strQueueFullName = tMqConf.GetQueueName();
    strQueueFullName += ":";
    strQueueFullName += GetSrvGUID().GetNOString();
	strQueueFullName += "20202000000000000000000000000002";
    tMqConf.SetQueueName(strQueueFullName.c_str());
	
	printf("[apsstart]Mq Ip:%s port.%d user.%s pass.%s host.%s  bIsNeedInit.%d\n", tMqConf.GetIP(),
					 tMqConf.GetPort(),	tMqConf.GetUsername(), tMqConf.GetUserPwd(),tMqConf.GetVHost(),bIsNeedInit  );     

	printf("[apsstart]Mq qname.%s exname.%s bindkey%s\n", tMqConf.GetQueueName(), tMqConf.GetExchangeName(), 
					tMqConf.GetBindingkey() );

		
	s32 nRet = -1;

	if( bIsNeedInit )
	{
		nRet = RabbitmqClntInit( tMqConf.GetIP(), tMqConf.GetPort(), tMqConf.GetVHost(), 
					tMqConf.GetUsername(), tMqConf.GetUserPwd() );

		if ( RT_OK == nRet )
		{
			printf( "[apsstart]RabbitmqClntInit ok!\n" );
		}
		else
		{
			printf( "[apsstart]RabbitmqClntInit failed!\n" );
			return FALSE;
		}
	}

	nRet = SafeCreateC( tMqConf.GetQueueName(), tMqConf.GetExchangeName(), 
					tMqConf.GetBindingkey(),
					Content_MQCB, NULL, NULL, Consumer_ConSuc_MQCB, Consumer_ConFail_MQCB, 0 );	

	if ( RT_OK == nRet )
	{
		printf( "[apsstart]CreateC ok!\n" );
	}
	else
	{
		printf( "[[apsstart]]CreateC failed!\n" );
		return FALSE;
	}

	return TRUE;
}

BOOL32 apsstart()
{
	//2. OSP 初始化
    if (!IsOspInitd())
    {
        BOOL32 bOspInit = OspInit(TRUE, TELNET_PORT_APS, NULL, (MAXINSTNUM_ULCSSN+100*OPERATION_MAXNUM_ISP));
        if (!bOspInit)
        {
            printf("%s: OspInit fail!\n", MODULE_TYPE_APS);
			LogPrint(LOG_LVL_ERROR, MID_PUB_ALWAYS, "%s: Osp Init fail!\n", MODULE_TYPE_APS);
            return FALSE;
        }
		printf("%s: Osp Init Succeed!\n", MODULE_TYPE_APS);
		LogPrint(LOG_LVL_KEYSTATUS, MID_PUB_ALWAYS, "%s: Osp Init Succeed!\n", MODULE_TYPE_APS);
    }
	else
	{
		printf("%s: Osp is already Init!\n", MODULE_TYPE_APS);
		LogPrint(LOG_LVL_KEYSTATUS, MID_PUB_ALWAYS, "%s: Osp is already Init!\n", MODULE_TYPE_APS);
	}

#ifdef WIN32
	OspRegistModule( "ospdll.dll" );
#endif

	 //3. 启动watchdogclient
// #ifdef NDEBUG
//     WDStart(MODULE_TYPE_APS);
// #endif

	//4.读配置文件
	g_cCfgFile.ReadConfigInfo();



#ifdef _APS_PXY_
	OspSetPrompt("ApsProxy");
#else
	OspSetPrompt("APS");
#endif
    OspTelAuthor(g_cCfgFile.GetTelnetName(), g_cCfgFile.GetTelnetPwd());
	
    APIRegInLinux();
    AddEventStr();
	
    logenablemod(MID_ENTRANCE_APS);
    logsetlvl(LOG_LVL_DETAIL, LOG_LVL_KEYSTATUS);

    //OspOpenLogFile(LOG_APS_FILENAME, 20 * 1024, 20);
    OspPrintf(TRUE, TRUE, "Osp Init SUCC.\n");

    // 打印程序版本
    OspPrintf(TRUE, TRUE, "application program version: %s\n", GetBossBuildVersion().c_str());


#ifdef _TEST_   //自测代码
	//
#else
//  [12/5/2015 pengguofeng]apsPxy不走Sa认证
#ifndef _APS_PXY_
	//StartConnectSA(MODULE_TYPE_APS);
	printf("StartConnectSA succeed!\n");
	OspPrintf(TRUE, TRUE, "StartConnectSA succeed!\n");
#endif
#endif


////////////////////////
//zjj20170209 mq的初始化必须在StartConnectSA后面
//因为mq初始化创建routingkey需要通过sa的接口拿到设备guid
#ifndef _APS_PXY_	
	CMqConfig tMqConf = g_cCfgFile.GetMqConInfo();
	
	if( !InitRBMQ(tMqConf,TRUE) )
	{
		printf("Fail to InitRBMQ\n");
		LogPrint(LOG_LVL_KEYSTATUS,MID_PUB_ALWAYS, "InitRBMQ is failed!\n" );
		return FALSE;
	}
#endif
////////////////////////

    //5.启动其他APP	
    //启动CenterCtrl服务APP
    if (OSP_OK != g_cCenterCtrlApp.CreateApp("CenterCtrApp", AID_APS_CENTERCTRL, MOBOSS_APP_PRI_NORMAL, 512*2, 6<<20 ))
    {
        OspPrintf(TRUE, TRUE, "Create CenterCtrlApp fail, Please restart server!\n");
        return FALSE;
    }
    ::OspPost(MAKEIID(AID_APS_CENTERCTRL, CInstance::DAEMON), OSP_POWERON);

    //启动connectsa APP
    if (OSP_OK != g_cConSaSsnApp.CreateApp("ConSaApp", AID_APS_CONNECTSA, MOBOSS_APP_PRI_NORMAL, 512*2))
    {
        OspPrintf(TRUE, TRUE, "Create ConSaApp fail, Please restart server!\n");
        return FALSE;
    }
    ::OspPost(MAKEIID(AID_APS_CONNECTSA, CInstance::DAEMON), OSP_POWERON);

    //启动UlcSSn服务App
    if (OSP_OK != g_cOldUlcSsnApp.CreateApp("OldSsnApp", AID_OLD_26_APS_ULCSSN, MOBOSS_APP_PRI_NORMAL, 512*2))
    {
        OspPrintf(TRUE, TRUE, "Create UlcSsnApp fail, please restart server!\n");
        return FALSE;
    }

    //启动HttpSSn服务App
    if (OSP_OK != g_cHttpSsnApp.CreateApp("HttpSsnApp", AID_APS_HTTP_SSN, MOBOSS_APP_PRI_NORMAL, 512*2))
    {
        OspPrintf(TRUE, TRUE, "Create UlcSsnApp fail, please restart server!\n");
        return FALSE;
    }

	OspPrintf(TRUE, TRUE, "startup ok...\n");
    printf("startup ok...\n");


   return TRUE;
}

/*lint -save -e628*/
void apsquit()
{
	OspPrintf(TRUE, TRUE, "[%s]pid.%u application program quit normally!\n", __FUNCTION__, getpid());
	LogPrint(LOG_LVL_KEYSTATUS, MID_PUB_ALWAYS, "[%s]pid.%u application program quit normally!\n", __FUNCTION__, getpid());
   
	logflush();
    return;
}
/*lint -restore*/
#ifdef _LINUX_
void CaptureExceptionQuitCB()
{
    printf("[CaptureExceptionQuitCB]pid.%u application program quit by CaptureException lib!\n", getpid());
    OspPrintf(TRUE, TRUE, "[CaptureExceptionQuitCB]pid.%u application program quit by CaptureException lib !\n", getpid());
	LogPrint(LOG_LVL_KEYSTATUS, MID_PUB_ALWAYS, "[%s]pid.%u application program quit by CaptureException lib!\n", __FUNCTION__, getpid());

	logflush();
	return;
}
#endif

int main(int argc, char **argv)
{
	// 判断是否已经启动过该程序
    if (IsProcessRunned(MODULE_TYPE_APS))
    {
        printf("%s is already running, so quit!\n", MODULE_TYPE_APS);
        return 0;
    }
	else
	{
		printf("start running this app:%s!\n", MODULE_TYPE_APS);
	}

	// 初始化Kdvlog
    Err_t err = LogInit();
    if(LOG_ERR_NO_ERR != err)
    {
        printf("[%s]KdvLog Init failed, err is %d\n", MODULE_TYPE_APS, err);
    }
	else
	{
        printf("[%s]KdvLog Init succeed\n", MODULE_TYPE_APS);
	}

	//设置进程异常捕获回调
#ifdef _LINUX_
	/*lint -save -e605*/
    if ( 0 == CaptureExceptionInit(EXCEPTION_APS_FILENAME, CaptureExceptionQuitCB) )
	{
        printf("[%s]CaptureExceptionInit succeed\n", MODULE_TYPE_APS);
	}
	else
	{
        printf("[%s]CaptureExceptionInit failed\n", MODULE_TYPE_APS);
	}
	/*lint -restore*/

	//生成core文件
	struct rlimit tCoreCfg;
	if ( getrlimit(RLIMIT_CORE, &tCoreCfg) != 0 )
	{
		tCoreCfg.rlim_cur = ~0UL;
		tCoreCfg.rlim_max = ~0UL;
	}
	else
	{
		tCoreCfg.rlim_cur = tCoreCfg.rlim_max;
	}
	setrlimit(RLIMIT_CORE, &tCoreCfg);
#endif	

    std::string strModulePath = BossUtils::GetModuleFullPath();
    s32 nRet = chdir( strModulePath.c_str() );
    if (nRet != 0)
    {
        printf("[%s] chdir to dir:%s fail!\n", MODULE_TYPE_APS, strModulePath.c_str());
		LogPrint(LOG_LVL_KEYSTATUS, MID_PUB_ALWAYS, "[%s] chdir to dir:%s fail!\n", MODULE_TYPE_APS, strModulePath.c_str());
        return FALSE;
    }

// #ifdef NDEBUG
//     if (argc <= 1)
//     {
//         return 0;
//     }
// 	if( 0 != ( strcmp("guard",argv[1]) ) )
// 	{
//         printf("start with guard please! quit now...!\n");
// 		return 0;
// 	}
// #endif
	
    BOOL32 bRet = apsstart();
    if (!bRet)
    {
        printf("[%s]apsstart fail, this app quit!\n", MODULE_TYPE_APS);
		LogPrint(LOG_LVL_KEYSTATUS, MID_PUB_ALWAYS,"[%s]apsstart fail, quit!\n", MODULE_TYPE_APS);
        return 0;
    }

    while (!g_bQuit)
    {
        OspDelay(1000);
    }
	
	printf("[%s] quit now\n", MODULE_TYPE_APS);
	LogPrint(LOG_LVL_KEYSTATUS, MID_PUB_ALWAYS,"[%s] quit now\n", MODULE_TYPE_APS);
    apsquit();
	
    return 0;
}
