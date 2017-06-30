/*****************************************************************************
模块名      : MAU
文件名      : main.cpp
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
#include "jansson.h"
#include "operationconst.h"
#include "appdef.h"
#include "osp.h"
#include "kdvtype.h"
#include "kdvlog.h"
#include "loguserdef.h"
#include "mauvc.h"
#include "mauconst.h"
#include "mcuver.h"
#include "evmau.h"
#include "captureexception.h"
#include "mautest.h"
#include "bmclicense.h"
#include "connectsa_api.h"

#include "bmcssn.h"
#include "cmussn.h"
#include "bmcssn.h"
#include "hdussn.h"
#include "nussn.h"
#include "upussn.h"
#include "resmgrssn.h"
#include "cmmaussn.h"
#include "cmcmussn.h"
#include "connmsadpssn.h"
#include "bmclicense.h"
#include "nppssn.h"
//rnn test
#include "jsontranslate.h"
#include "eventnumau.h"
#include "commonclass.h"

SEMHANDLE	g_hQuitSem = NULL;
SOCKHANDLE	g_nListenSock = INVALID_SOCKET;

//测试q是否有两个消费者信号量
SEMHANDLE g_hTestQConsumerSem = NULL;

CMauVCApp g_cMauVCApp;
CCMUSsnApp g_cCMUSsnApp;
CCMMauSsnApp g_cCMMauSsnApp;
CUpuSsnApp g_cUpuSsnApp;
CHduPoolSsnApp g_cHduPoolSsnApp;
CNUSsnApp g_cNUSsnApp;
CResMgrSsnApp g_cResMgrSsnApp;
CConNmsAdpApp g_cConAdpApp;
CCMCmuSsnApp g_cCMCmuSsnApp;
CBmcSsnApp g_cBmcSsnApp;
CNppSsnApp g_cNppSsnApp;

API void csshelp( void )
{
	OspPrintf(TRUE, FALSE, "MPCD Ver: %s   Compile Time: %s, %s\n", KDV_MCU_PREFIX, __TIME__, __DATE__);
	OspPrintf(TRUE, FALSE, "\nFollowing are all commands in mau module.\n");
	OspPrintf(TRUE, FALSE, "showalldomain:         print all domain info in mau.\n");
	OspPrintf(TRUE, FALSE, "showallmt:             print all mt info in mau.\n");
	OspPrintf(TRUE, FALSE, "showconf:              print all conf info in memory.\n");
	OspPrintf(TRUE, FALSE, "showtempconf:          print all temp info in memory.\n");
	OspPrintf(TRUE, FALSE, "showallreqinfo:        print all require info in memory.\n");
	OspPrintf(TRUE, FALSE, "showcmu:               print current regist cmu info.\n");
	OspPrintf(TRUE, FALSE, "showgk:                print current regist gk info.\n");
	OspPrintf(TRUE, FALSE, "showcfg:               print mau config info.\n");
	OspPrintf(TRUE, FALSE, "showhdu:               print hdu info.\n");
	OspPrintf(TRUE, FALSE, "showupu:               print upu info.\n");
	OspPrintf(TRUE, FALSE,  "sli:                   print bmc license info\n");
	OspPrintf(TRUE, FALSE, "showmt [x] [y]:	       print mt info by e164.\n");
	OspPrintf(TRUE, FALSE, "                       [x]-prefix(6bits)\n");
	OspPrintf(TRUE, FALSE, "                       [y]-serialno(7bits)\n");
	OspPrintf(TRUE, FALSE, "reloadlua              reload redis lua script\n");
	OspPrintf(TRUE, FALSE, "showpt        print all personal template e164.\n");
	OspPrintf(TRUE, FALSE, "showptinfo [x] [y]:     print all personal tempalteinfo by e164.\n");
	OspPrintf(TRUE, FALSE, "showaphdu:              print apdb hdu info.\n");
	OspPrintf(TRUE, FALSE, "setrecspan:             set recover conf timespan\n");
	OspPrintf(TRUE, FALSE, "getrecspan:             get recover conf timespan\n");
	OspPrintf(TRUE, FALSE, "setcmuspan:             get mcu heartbeat timespan\n");
	OspPrintf(TRUE, FALSE, "getmcuspan:             get mcu heartbeat timespan\n");
	OspPrintf(TRUE, FALSE, "splatinfo:              print platform guid,moid,confnum limit\n");
	OspPrintf(TRUE, FALSE, "srent:                  print platform is rent or self build\n");
	OspPrintf(TRUE, FALSE, "conflistloglvl:         print h323/sip convert msg\n");
	OspPrintf(TRUE, FALSE, "showc:                  print rabbitmq consumer list\n");
	OspPrintf(TRUE, FALSE, "showp:                  print rabbitmq producer list\n");
	OspPrintf(TRUE, FALSE, "setrabbitmqlvl:         set rabbitmqwrap log level\n");

	OspPrintf(TRUE, FALSE, "\n--------------------------PRINT INFO---------------------------\n");
	OspPrintf(TRUE, FALSE, "ptempdbmsg:            print db-temp msg info.\n");
	OspPrintf(TRUE, FALSE, "nptempdbmsg:           not print db-temp msg info.\n");
	OspPrintf(TRUE, FALSE, "pvmdbmsg:              print db-vm msg info.\n");
	OspPrintf(TRUE, FALSE, "npvmdbmsg:             not print db-vm msg info.\n");
	OspPrintf(TRUE, FALSE, "pschedbmsg:            print db-sche msg info.\n");
	OspPrintf(TRUE, FALSE, "npschedbmsg:           not print db-sche msg info.\n");
	OspPrintf(TRUE, FALSE, "papdbmsg:              print db-ap msg info.\n");
	OspPrintf(TRUE, FALSE, "npapdbmsg:             not print db-ap msg info.\n");
	OspPrintf(TRUE, FALSE, "pmauvcmsg:             print mau control msg info.\n");
	OspPrintf(TRUE, FALSE, "npmauvcmsg:            not print mau control msg info.\n");
	OspPrintf(TRUE, FALSE, "ppublicmsg:            print public msg info.\n");
	OspPrintf(TRUE, FALSE, "nppublicmsg:           not print public msg info.\n");
	OspPrintf(TRUE, FALSE, "pcmumsg:               print cmussn msg info.\n");
	OspPrintf(TRUE, FALSE, "npcmumsg:              not print cmussn msg info.\n");

	OspPrintf(TRUE, FALSE, "pcmcmumsg:             print cmcmussn msg info.\n");
	OspPrintf(TRUE, FALSE, "npcmcmumsg:            not print cmcmussn msg info.\n");
	OspPrintf(TRUE, FALSE, "pcmmaumsg:             print cmmaussn msg info.\n");
	OspPrintf(TRUE, FALSE, "npcmmaumsg:            not print cmmaussn msg info.\n");

	OspPrintf(TRUE, FALSE, "pbmcmsg:              print bmc ssn msg info.\n");
	OspPrintf(TRUE, FALSE, "npbmcmsg:             not print bmc ssn msg info.\n");
	OspPrintf(TRUE, FALSE, "pnumsg:               print nu ssn msg info.\n");
	OspPrintf(TRUE, FALSE, "npnumsg:              not print nu ssn msg info.\n");
	OspPrintf(TRUE, FALSE, "pnppmsg:              print npp ssn msg info.\n");
	OspPrintf(TRUE, FALSE, "npnppmsg:             not print npp ssn msg info.\n");
	OspPrintf(TRUE, FALSE, "presmgrmsg:           print res mgr msg info.\n");
	OspPrintf(TRUE, FALSE, "npresmgrmsg:          not print resmgr msg info.\n");
	OspPrintf(TRUE, FALSE, "pupumsg:              print upu ssn msg info.\n");
	OspPrintf(TRUE, FALSE, "npupumsg:             not print upu ssn msg info.\n");
	OspPrintf(TRUE, FALSE, "predismsg:             print redis msg info.\n");
	OspPrintf(TRUE, FALSE, "npredismsg:            not print redis ssn msg info.\n");

	OspPrintf(TRUE, FALSE, "\n--------------------------MPCD MEETING MANAGEMENT---------------------------\n");
	OspPrintf(TRUE, FALSE, "showdb:               print db connect status.\n");
	OspPrintf(TRUE, FALSE, "sb:                   print book conf by param(null/0:all|1:today|2:within_2_day)\n");
	OspPrintf(TRUE, FALSE, "showallbook:          print all book conf\n");
	OspPrintf(TRUE, FALSE, "showbookconf:         print book conf cache data.\n");
	OspPrintf(TRUE, FALSE, "showallmonitor:       print all monitor data.\n");

	OspPrintf(TRUE, FALSE, "\n---------------------------------UPU API------------------------------------\n");
	OspPrintf(TRUE, FALSE, "addconf:              add conf to upu\n");
	OspPrintf(TRUE, FALSE, "findconf:             find conf from upu\n");
	OspPrintf(TRUE, FALSE, "delconf:              del conf to upu\n");
	OspPrintf(TRUE, FALSE, "deldev:               del conf to upu by dev guid\n");
}

API void showalldomain( void )
{
// 	u16 wType = DUMPALLDOMAININFO;
// 	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16) );
	g_cMauVCApp.PrintDomainList();
}

API void showpt(void)
{
	u16 wType = DUMPALLPERSONALTEMPE164;
	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16));
}

API void srent(void)
{
	OspPrintf( TRUE, FALSE, "platform is: %s\n", 
		g_cMauVCApp.GetPlatformIsRent() ? "Rent" : "self build");
}

API void showptinfo(u32 dwE164)
{
	if (0 == dwE164)
	{
		OspPrintf(TRUE, FALSE, "[showptinfo]Income value err, return!\n");
		return;
	}
	s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
	sprintf(achConfE164, "%d", dwE164);
	g_cMauVCApp.PrintPersonalTempByE164(achConfE164);
}

API void showallmt( void )
{
	u16 wType = DUMPALLMTINFO;
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16) );
}

//通过e164打印对应终端信息
API void showmt( u32 dwGkPreFix, u32 dwSerialNO )
{
	if ( dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > MAXVALUE_MT_SERIALNO )
	{
		OspPrintf( TRUE, FALSE, "[showmtinfo]Income value err, return!\n" );
		return;
	}
	
	s8 achMtE164NO[13 + 1] = { 0 };
	snprintf( achMtE164NO, sizeof( achMtE164NO ), "%06d%07d", dwGkPreFix, dwSerialNO );

	CMauMtInfo cMtOutInfo;
	
	OspPrintf(TRUE, FALSE, "mt.%s info:\n", achMtE164NO);
	
	if (g_cMauVCApp.GetMtInfo(achMtE164NO, cMtOutInfo))
	{
		cMtOutInfo.Print();
	}
	
	return;
}

API void showconf( void )
{
	u16 wType = DUMPALLCONFCACHEINFO;
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16) );
}

API void showtempconf( void )
{
	u16 wType = DUMPALLTEMPCACHEINFO;
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16) );
}

API void showcmu( void )
{
	u16 wType = DUMPCMUINFO;
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16) );
}

API void showgk( void )
{
	u16 wType = DUMPNUINFO;
	OspPost( MAKEIID(AID_MAU_NUSSN_APPID,CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16) );
}

API void showhdu( void )
{
	u16 wType = DUMPALLHDUINFO;
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16) );
}
API void showaphdu(void)
{
	u16 wType = DUMPAPDBHDUINFO;
	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16));
}

API void showcfg( void )
{
	u16 wType = DUMPDBCFGINFO;
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16) );
}

API void sb(const u8 byPrintParam, const u8 byPrintDetailJson)
{
	g_cMauVCApp.PrintBookConfData(byPrintParam, byPrintDetailJson);
}

API void sr()
{
	g_cMauVCApp.PrintNeedRecorverConf();
}

API void showbookconf(void)
{
	g_cMauVCApp.PrintBookConfData(1);
}

API void showallbook(void)
{
	g_cMauVCApp.PrintBookConfData();
}

API void showdb( void )
{
	u16 wType = DUMPDBSTATUS;
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID,CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16) );
}

API void showallmonitor( void )
{	
	u16 wType = DUMPALLMONITORINFO;
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16) );
}

API void showupu(void)
{
	u16 wType = DUMPALLUPUINFO;
	OspPost(MAKEIID(AID_MAU_UPUSSN_APPID, CInstance::DAEMON), MAU_PRINTINFO_CMD, &wType, sizeof(u16));
}

API void sli(void)
{
	g_cBmcLicenseApp.PrintBmcLicenseInfo();
}

API void reloadlua(void)
{
	g_cMauVCApp.LoadRedisLua();
}

API void quit(void)
{
	OspSemGive(g_hQuitSem);
}

API void splatlimit()
{
	const CPlatformConfNumLimit& cPlatformConfNumLimit(g_cMauVCApp.GetPlatConfLimitNum());
	OspPrintf(TRUE, FALSE, "platform maxSmallConfNum: %u, maxLargeConfNUm: %u\n",
		cPlatformConfNumLimit.GetSmallConfNumLimit(), 
		cPlatformConfNumLimit.GetLargeConfNumLimit());
}

API void splatinfo()
{
	OspPrintf(TRUE, FALSE, "platform moid:%s,\n guid:%s\n",
		g_cMauVCApp.m_achPlatformMoid, g_cMauVCApp.m_achPlatformGuid);
	splatlimit();
}

API void check()
{
	g_cMauVCApp.CheckEnv();
}

API void setrecspan(const u32 dwTimeSpan)
{
	g_cMauVCApp.SetRecoverTimeSpan(dwTimeSpan);
}

API void getrecspan()
{
	OspPrintf(TRUE, FALSE, "current recovertime span: %u\n", g_cMauVCApp.GetRecoverTimeSpan());
}

API void setmcuspan(const u32 dwTimeSpan)
{
	g_cMauVCApp.SetMcuHeartBeatTimeSpan(dwTimeSpan);
}

API void getmcuspan()
{
	OspPrintf(TRUE, FALSE, "current mcu heartbeat time span: %u\n", g_cMauVCApp.GetMcuHeartBeatTimeSpan());
}

API void setconflistspan(const u32 dwTimeSpan)
{
	g_cMauVCApp.SetConfListTimeSpan(dwTimeSpan);
}

API void getconflistspan()
{
	OspPrintf(TRUE, FALSE, "current get conflist time span: %u\n", g_cMauVCApp.GetConfListTimeSpan());
}

void APIRegInLinux( void )
{
	
#ifdef _LINUX_
	// 将void func()转为void* func时pclint会出错
	/*lint -save -e611*/
	OspRegCommand("csshelp",			(void*)csshelp,					"show mau help command.\n");
    OspRegCommand("showalldomain",		(void*)showalldomain,       "show all domain info.\n");
    OspRegCommand("showallmt",			(void*)showallmt,           "show all mt info.\n");
	OspRegCommand("showconf",			(void*)showconf,				"show all conf data in memory.\n");
	OspRegCommand("showtempconf",		(void*)showtempconf,			"show all temp data in memory.\n");
	OspRegCommand("showcmu",			(void*)showcmu,					"show all cmu data info.\n");
	OspRegCommand("showgk",				(void*)showgk,					"show gk info\n");
	OspRegCommand("showhdu",			(void*)showhdu,					"show hdu info\n");
	OspRegCommand("showupu",			(void*)showupu,					"print upu info");
	OspRegCommand("showcfg",			(void*)showcfg,					"show mau config info\n");
	OspRegCommand("showmt"	,		  	(void*)showmt,					"show mt info by e164.\n");
	OspRegCommand("showallmonitor",     (void*)showallmonitor,          "show all monitor.\n");
	OspRegCommand("reloadlua",			(void*)reloadlua,				"reload redis lua script.\n");
	OspRegCommand("sli",                (void*)sli,                     "show license info.\n");
	OspRegCommand("sb",                (void*)sb,                       "show book conf info.\n");
	OspRegCommand("sr",                (void*)sr,                       "show need recover conf info.\n");
	OspRegCommand("splatlimit",        (void*)splatlimit,               "show platform conf num limit\n");
	OspRegCommand("splatinfo",        (void*)splatinfo,                 "show platform moid, guid, confnum limit\n");
	OspRegCommand("srent",              (void*)srent,                   "show platfrom is rent or self build\n");
	OspRegCommand("ptempdbmsg",			(void*)ptempdbmsg,				"print db-temp msg info\n");
	OspRegCommand("nptempdbmsg",		(void*)nptempdbmsg,				"not print db-temp msg info\n");
	OspRegCommand("pvmdbmsg",			(void*)pvmdbmsg,				"print db-vm msg info\n");
	OspRegCommand("npvmdbmsg",			(void*)npvmdbmsg,				"not print db-vm msg info\n");
	OspRegCommand("pschedbmsg",			(void*)pschedbmsg,				"print db-sche msg info\n");
	OspRegCommand("npschedbmsg",		(void*)npschedbmsg,				"not print db-sche msg info\n");
	OspRegCommand("papdbmsg",			(void*)papdbmsg,				"print db-ap msg info\n");
	OspRegCommand("npapdbmsg",			(void*)npapdbmsg,				"not print db-ap msg info\n");
	OspRegCommand("pmauvcmsg",			(void*)pmauvcmsg,				"print mau control msg info\n");
	OspRegCommand("npmauvcmsg",		    (void*)npmauvcmsg,				"not print mau control msg info\n");
	OspRegCommand("ppublicmsg",			(void*)ppublicmsg,				"print public msg info\n");
	OspRegCommand("nppublicmsg",		(void*)nppublicmsg,				"not print public msg info\n");
	OspRegCommand("pconfdbmsg",			(void*)pconfdbmsg,				"print confdb msg\n");
	OspRegCommand("npconfdbmsg",		(void*)npconfdbmsg,				"not print confdb msg\n");
	OspRegCommand("pconfdbmsg",			(void*)pconfbackmsg,			"print confback msg\n");
	OspRegCommand("npconfdbmsg",		(void*)npconfbackmsg,			"not print confback msg\n");
	OspRegCommand("pcmumsg",			(void*)pcmumsg,					"print cmussn msg info\n");
	OspRegCommand("npcmumsg",			(void*)npcmumsg,				"not print cmussn msg info\n");
	OspRegCommand("pcmcmumsg",			(void*)pcmcmumsg,				"print cmcmussn msg info\n");
	OspRegCommand("npcmcmumsg",			(void*)npcmcmumsg,				"not print cmcmussn msg info\n");
	OspRegCommand("pcmmaumsg",			(void*)pcmmaumsg,				"print cmmaussn msg info\n");
	OspRegCommand("npcmmaumsg",			(void*)npcmmaumsg,				"not print cmmaussn msg info\n");
	OspRegCommand("pbmcmsg",			(void*)pbmcmsg,					"print bmc ssn msg info\n");
	OspRegCommand("npbmcmsg",			(void*)npbmcmsg,				"not print bmc ssn msg info\n");
	OspRegCommand("predismsg",			(void*)predismsg,				"print redis msg\n");
	OspRegCommand("npredismsg",			(void*)npredismsg,				"not print redis msg\n");

	
	OspRegCommand("pnumsg",			    (void*)pnumsg,					"print nu ssn msg info\n");
	OspRegCommand("npnumsg",			(void*)npnumsg,					"not print nu ssn msg info\n");
	OspRegCommand("pnppmsg",            (void*)pnppmsg,                 "print npp ssn msg info\n");
	OspRegCommand("npnppmsg",           (void*)npnppmsg,                "not print npp ssn msg info\n");
	OspRegCommand("presmgrmsg",		    (void*)presmgrmsg,				"not print res mgr msg info\n");
	OspRegCommand("npresmgrmsg",		(void*)npresmgrmsg,				"print res mgr msg info\n");
	OspRegCommand("pupumsg",		    (void*)pupumsg,			        "print bdb msg\n");
	OspRegCommand("npupumsg",		    (void*)npupumsg,		     	"not print bdb msg\n");
    OspRegCommand("phdumsg",		    (void*)phdumsg,			        "print hdu msg\n");
    OspRegCommand("nphdumsg",		    (void*)nphdumsg,		     	"not print hdu msg\n");
	
	OspRegCommand("showbookconf",       (void*)showbookconf,            "print book conf cache data.\n");
	OspRegCommand("showdb",             (void*)showdb,                  "print db connect status.\n");
	OspRegCommand("showallbook",        (void*)showallbook,             "print all book conf data.\n");
	
	OspRegCommand("createconfbycm",     (void*)createconfbycm,           "createconfbycm info.\n");
	OspRegCommand("releaseconf",        (void*)releaseconf,              "releaseconf info.\n");
	OspRegCommand("addmt",              (void*)addmt,                    "addmt info.\n");

	OspRegCommand("addconf",         (void*)addconf,            "add conf to upu.\n");
	OspRegCommand("delconf",         (void*)delconf,            "find conf from upu.\n");
	OspRegCommand("deldev",          (void*)deldev,             "find conf from upu by dev guid.\n");
	OspRegCommand("findconf",        (void*)findconf,           "del conf to upu.\n");

	OspRegCommand("initcmu",		(void*)initcmu,           "init cmu.\n");
	OspRegCommand("enablecmu",		(void*)enablecmu,		"enable cmu, param: mcuid\n");
	OspRegCommand("disablecmu",		(void*)disablecmu,		"disable cmu, param: mcuid\n");
	OspRegCommand("quit",			(void*)quit,			"quit css\n");
	OspRegCommand("showpt",			(void*)showpt,			"showpersonaltemplatee164\n");
	OspRegCommand("showptinfo",   (void*)showptinfo,    "show personaltemplateinfo by e164\n");
	OspRegCommand("check",        (void*)check,			"check css env\n");
	OspRegCommand("showaphdu",    (void*)showaphdu,     "show apdb HDUinfo\n");

	OspRegCommand("setrecspan",    (void*)setrecspan,     "set recover conf timespan\n");
	OspRegCommand("getrecspan",    (void*)getrecspan,     "show recover conf timespan\n");
	OspRegCommand("setmcuspan",    (void*)setmcuspan,     "set mcu heartbeat timespan\n");
	OspRegCommand("getmcuspan",    (void*)getmcuspan,     "show mcu heartbeat timespan\n");
	OspRegCommand("setconflistspan",    (void*)setconflistspan,     "set getconflist timespan\n");
	OspRegCommand("getconflistspan",    (void*)getconflistspan,     "show getconflist timespan\n");
	/*lint -restore*/
#endif
	
	return ;
}

void AddEventStr()
{
#undef OSPEVENT
#define OSPEVENT( x, y ) const u16 x = y; \
	OspAddEventDesc( #x, x ) \
	
#ifdef _EV_MAU_H_
#undef _EV_MAU_H_
#include "evmau.h"
#define _EV_MAU_H_
#else
#include "evmau.h"
#undef _EV_MAU_H_
#endif
	
// #ifdef _EV_MCU_MAU_H_
// #undef _EV_MCU_MAU_H_
// #include "evmcumau.h"
// #define _EV_MCU_MAU_H_
// #else
// #include "evmcumau.h"
// #undef _EV_MCU_MAU_H_
// #endif
	
#ifdef _EVENT_NU_MAU_H_
#undef _EVENT_NU_MAU_H_
#include "eventnumau.h"
#define _EVENT_NU_MAU_H_
#else
#include "eventnumau.h"
#undef _EVENT_NU_MAU_H_
#endif
	
}

/*=============================================================================
函 数 名： CreateSemResource
功    能： 创建锁资源
算法实现： 
全局变量： 
参    数： void
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2011/12/21  KDVP2.5		陈文灿                  创建
=============================================================================*/
BOOL32 CreateSemResource( void )
{
	//测试css消费的q是否有多个消费者信号量
	if (!OspSemBCreate(&g_hTestQConsumerSem))
	{
		OspSemDelete(g_hTestQConsumerSem);
		g_hTestQConsumerSem = NULL;
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC, "[CreateSemResource] create g_hTestQConsumer Failed!\n" );
		return FALSE;
	}

	return TRUE;
}

void MauStart( void )
{
	//IZkLogHelp::SetLog( true );
	if( !IsOspInitd() )
	{
#ifdef	NDEBUG
		if( !OspInit( FALSE, MAU_TELNET_LISTEN_PORT))
#else
		if( !OspInit( TRUE, MAU_TELNET_LISTEN_PORT))
#endif
		{
			printf( "[MauStart] Osp Init Failed, return!\n" );
			return ;
		}
	}

	//注册帮助命令函数
	APIRegInLinux();
	OspRegistModule("librabbitmqwrap.dll");

	//添加消息字符串
	AddEventStr();

	OspSetPrompt( "css" );
 	Err_t err = LogInit();
 	logenablemod( MID_MAU_START );
	if(LOG_ERR_NO_ERR != err)
	{
		printf("KdvLog Init failed, err is %d\n", err);
	}

	// 读取配置文件
	g_cMauVCApp.Init();

	// 设置进程异常捕获
#ifdef _LINUX_
#ifdef _X86_
	CaptureExceptionInit( (s8*)EXCEPTION_MAU_FILENAME );
#endif
#endif

	//	OspTelAuthor( MCU_TEL_USRNAME, MCU_TEL_PWD );

#ifdef _LINUX_
	StartConnectSA("css", ConnectSACB);
	strncpy(g_cMauVCApp.m_achPlatformGuid, GetSrvGUID().GetNOString(), 
		sizeof(g_cMauVCApp.m_achPlatformGuid));
	strncpy(g_cMauVCApp.m_achPlatformMoid, GetSrvMOID().GetNOString(),
		sizeof(g_cMauVCApp.m_achPlatformMoid));
#endif

	// 创建锁资源
	if( !CreateSemResource() )
	{
		LogPrint( LOG_LVL_ERROR, MID_MAU_PUBLIC,  "[MpcdStart] Create Sem Resource err, return!\n" );
		return ;
	}

	// 创建APP
	BOOL32 nRet = 0;
	nRet = g_cMauVCApp.CreateApp( "MauVC", AID_MAU_MAUVC_APPID, 85, 1024*10, 500<<10 );
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[MauStart] Creat MauVC app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);

	nRet = g_cCMUSsnApp.CreateApp("CmuSsn", AID_MAU_CMUSSN_APPID, 80, 2000, 500 << 10);
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[MauStart] Creat CmuSsn app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);

	nRet = g_cBmcSsnApp.CreateApp("BmcSsn", AID_MAU_BMCSSN_APPID, 80, 2000, 500 << 10);
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[MauStart] Creat BmcSsn app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);

	nRet = g_cNUSsnApp.CreateApp("NuSsn", AID_MAU_NUSSN_APPID, 80, 1024 * 10, 500 << 10);
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[MauStart] Creat NuSsn app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);

	nRet = g_cResMgrSsnApp.CreateApp("ResMgrSsn", AID_MAU_RESMGRSSN_APPID, 80, 1024 * 10, 500 << 10);
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[MauStart] Creat ResMgrSsn app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);

	nRet = g_cCMMauSsnApp.CreateApp("CMMauSsn", AID_MAU_CMMAUSSN_APPID, 80, 1024 * 10, 500 << 10);
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[MauStart] Creat CMMauSsn app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);

	nRet = g_cCMCmuSsnApp.CreateApp("CMCmuSsn", AID_MAU_CMCMUSSN_APPID, 80, 2000, 500 << 10);
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[MauStart] Creat CMCmuSsn app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);

	nRet = g_cConAdpApp.CreateApp("NmsAdpSsn", AID_MAU_CONNMSADPSSN_APPID, 80, 2000, 500 << 10);
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[MauStart] Creat NmsAdpSsn app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);

	nRet = g_cHduPoolSsnApp.CreateApp("HduSsn", AID_MAU_HDUSSN_APPID, 80, 2000, 500 << 10);
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[MauStart] Creat HduSsn app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);
	
	nRet = g_cUpuSsnApp.CreateApp("UpuSsn", AID_MAU_UPUSSN_APPID, 80, 2000, 500 << 10);
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[MauStart] Creat UpuSsn app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);

	nRet = g_cBmcLicenseApp.CreateApp("BmcLicense", AID_MAU_BMCLICENSE_APPID, 80, 100, 500 << 10);
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcLoadDBResNtf] Creat BmcLicense app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);

	nRet = g_cNppSsnApp.CreateApp("NppSsn", AID_MAU_NPPSSN_APPID, 80, 100, 500 << 10);
	LogPrint(LOG_LVL_DETAIL, MID_MAU_VC, "[DaemonProcLoadDBResNtf] Creat NppSsn app %s, nRet = %d!\n", (nRet == OSP_OK) ? "successful" : "fail", nRet);
	
	//创建侦听端口
	g_nListenSock = OspCreateTcpNode( 0, g_cMauVCApp.m_dwListenPort, TRUE );
	if( INVALID_SOCKET == g_nListenSock )
	{
		printf( "[MpcdStart] Create Listen Socket failed!\n" );
		return ;
	}
	
	//发送上电初始化消息
	OspPost( MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), OSP_POWERON );
}

void MauQuit(void)
{
	g_cMauVCApp.Quit();
	g_cUpuSsnApp.Quit();
	g_cResMgrSsnApp.Quit();
	LogQuit();
}

#ifdef _LINUX_
void OspQuitFunc(int nSignal)
{
	::OspPrintf(TRUE, FALSE, "Resive quit signal: %d\n", nSignal);
	::OspSemGive(g_hQuitSem);
}
#endif

int main( int argc, char** argv )
{
	if (IsProcessOnGoing("/tmp/lockFile.css"))
	{
		printf("process ongoing,quit\n");
		return 0;
	}
	MauStart();
	
#ifdef	_LINUX_
	//注册退出清除函数
	OspRegQuitFunc(OspQuitFunc);
#endif
	
	OspSemBCreate(&g_hQuitSem);
	OspSemTake(g_hQuitSem);

	//等待退出信号量激活
	OspSemTake(g_hQuitSem);
	OspSemDelete(g_hQuitSem);

	MauQuit();

	OspQuit();
	return 0;
}