#include "config.h"
#include "debugprint.h"
#include "bossversion.h"
#include "eventself.h"
#include "connectsa_api.h"
#include "srvlistmgr.h"
#include "centerctrl.h"
#include "loguserdef.h"
#include "structextend.h"

/*lint -save -e438*/
//数据库
s32 g_nDBPrtLevel = LOG_LVL_KEYSTATUS;
//主控
s32 g_nCCtrlPrtLevel = LOG_LVL_KEYSTATUS;
//ConSa
s32 g_nConSaPrtLevel = LOG_LVL_KEYSTATUS;
//UlcSsn
s32 g_nUlcPrtLevel = LOG_LVL_KEYSTATUS;
//http
s32 g_nHttpPrtLevel = LOG_LVL_KEYSTATUS;
//SrvLIstData
s32 g_nSrvListPartLevel = LOG_LVL_KEYSTATUS;


void AddEventStr()
{

#undef OSPEVENT
#define OSPEVENT( x, y ) const u16 x = y; \
    OspAddEventDesc( #x, x ) \

#ifdef _EVENT_APS_SELF_H_
    #undef _EVENT_APS_SELF_H_
    #include "eventself.h"
#else
    #include "eventself.h"
#endif

#ifdef _EVENT_26_ULC_ULS_H_
    #undef _EVENT_26_ULC_ULS_H_
    #include "old_26_eventulculs.h"
#else
    #include "old_26_eventulculs.h"
#endif

#ifdef _OLD_252_EV_ULC_ULS_H_
#undef _OLD_252_EV_ULC_ULS_H_
#include "old_252_eventulculs.h"
#else
#include "old_252_eventulculs.h"
#endif

}

/*lint -save -e611 */
void APIRegInLinux(void)
{
#ifdef _LINUX_
    OspRegCommand("help",                   (void*)help,                "help");
    OspRegCommand("apsver",                 (void*)apsver,              "show the aps version info");
    OspRegCommand("quit",                   (void*)quit,                "quit the app");
	
    OspRegCommand("cctrlloglevel",          (void*)cctrlloglevel,       "set the centerctrl log level");
    OspRegCommand("dbloglevel",             (void*)dbloglevel,          "set the DB log level");
    OspRegCommand("dbpooldump",	            (void*)dbpooldump,			"show the DBPool state");
		
    OspRegCommand("clientnode",             (void*)clientnode,          "show client info");
    OspRegCommand("clientloglevel",         (void*)clientloglevel,      "set the old version mt seesion log level");
    OspRegCommand("httploglevel",           (void*)httploglevel,        "set the http seesion log level ");

	OspRegCommand("showsrv",                (void*)showsrv,             "show the server state");
	OspRegCommand("showcfg",                (void*)showcfg,             "show config info");
	OspRegCommand("showmtinfo",             (void*)showmtinfo,          "show registered mt info");
	OspRegCommand("apsloglvl",              (void*)apsloglvl,           "set all aps log level");
	OspRegCommand("srvlistmgrloglevel",     (void*)srvlistmgrloglevel,  "set the SrvListMgr log level");
    OspRegCommand("consaloglevel",          (void*)consaloglevel,       "set the ConSA log level");
    OspRegCommand("loglogin",               (void*)loadlogin,           "load login mt info");
    OspRegCommand("login",                  (void*)login,               "send mt request");
    OspRegCommand("settryapsspan",          (void*)settryapsspan,       "set try aps span");
    OspRegCommand("showbackup",             (void*)showbackup,          "show plat backup info");
	
#endif	
    return;
}
/*lint -restore*/

API void help(void)
{
	OspPrintf(TRUE, FALSE, "\n\n");
    OspPrintf(TRUE, FALSE, "************************** Help *******************************\n\n");
    OspPrintf(TRUE, FALSE, "\n");
    OspPrintf(TRUE, FALSE, " apsver:                 show the aps version info\n\n\n");
    OspPrintf(TRUE, FALSE, "\n");
	
    OspPrintf(TRUE, FALSE, "************************** set the log level ***********************\n\n");
    OspPrintf(TRUE, FALSE, " 1 －－－－－－－－－－－ ERROR \n");
    OspPrintf(TRUE, FALSE, " 2 －－－－－－－－－－－ WARNING \n");
    OspPrintf(TRUE, FALSE, " 3 －－－－－－－－－－－ INFO \n");
    OspPrintf(TRUE, FALSE, " 4 －－－－－－－－－－－ DETAIL \n\n\n");
	
    OspPrintf(TRUE, FALSE, " cctrlloglevel:           set the centerctrl log level\n");
    OspPrintf(TRUE, FALSE, " dbloglevel:              set the DB log level\n");
	OspPrintf(TRUE, FALSE, " dbpooldump:              show the DBPool state\n");
    OspPrintf(TRUE, FALSE, "\n");
	
    OspPrintf(TRUE, FALSE, " httploglevel:            set the http seesion log level\n");
    OspPrintf(TRUE, FALSE, "\n");    

    OspPrintf(TRUE, FALSE, " clientloglevel:          set the old version mt seesion log level\n");
    OspPrintf(TRUE, FALSE, "\n");
	
    OspPrintf(TRUE, FALSE, " consaloglevel:           set the ConSA log level\n");
    OspPrintf(TRUE, FALSE, "\n\n");

	OspPrintf(TRUE, FALSE, " showsrv:                 show the server state\n");
	OspPrintf(TRUE, FALSE, " showcfg:                 show config info\n");
	OspPrintf(TRUE, FALSE, " showbackup:              show plat backup info\n");
	OspPrintf(TRUE, FALSE, " showmtinfo:              show registered mt info\n");
	OspPrintf(TRUE, FALSE, " apsloglvl:               set whole aps log level\n");
	OspPrintf(TRUE, FALSE, "\n\n");

	OspPrintf(TRUE, FALSE, " srvlistmgrloglevel:      set the SrvListMgr log level\n");
    OspPrintf(TRUE, FALSE, "\n\n");
	
    OspPrintf(TRUE, FALSE, "************************** the mt register info ***************************\n\n");
    OspPrintf(TRUE, FALSE, " clientnode:              show client infon");
    OspPrintf(TRUE, FALSE, "\n");
    OspPrintf(TRUE, FALSE, "***************************************************************\n\n");
}

API void apsver(void)
{
	OspPrintf(TRUE, FALSE, "*************************** aps version **************************\n");
    OspPrintf(TRUE, FALSE, "\tversion : %s \n", MO_BOSS_VER_PREFIX);
    OspPrintf(TRUE, FALSE, "\tcompile time: %s    %s\n", __TIME__, __DATE__);
    OspPrintf(TRUE, FALSE, "***************************************************************\n");
    OspPrintf(TRUE, FALSE, "\n");

}

extern BOOL32 g_bQuit;

API void quit()
{
    OspPrintf( TRUE, TRUE, "quit this program by 'quit' command.\n" );
	LogPrint(LOG_LVL_KEYSTATUS, MID_PUB_ALWAYS, "APS will quit normally\n");
    g_bQuit = TRUE;
}

/************************************主控****************************************/
API void apsloglvl(s32 nLogLvl )
{
	cctrlloglevel(nLogLvl);
	httploglevel(nLogLvl);
	dbloglevel(nLogLvl);
	consaloglevel(nLogLvl);
	clientloglevel(nLogLvl);
	srvlistmgrloglevel(nLogLvl);
}

API void cctrlloglevel(s32 nDbgLvl)
{
	if (nDbgLvl < LOG_LVL_ERROR)
	{
		nDbgLvl = LOG_LVL_ERROR;
	}

	if (nDbgLvl > LOG_LVL_DETAIL)
	{
		nDbgLvl = LOG_LVL_DETAIL;
	}

	g_nCCtrlPrtLevel = nDbgLvl;
}

void CCtrlLog( s32 nPrtLevel, const s8* fmt, ... )
{
	BOOL32 bScreen = nPrtLevel <= g_nCCtrlPrtLevel;
	BOOL32 bFile = nPrtLevel <= LOG_LVL_KEYSTATUS;
	
	if (FALSE == bScreen && FALSE == bFile)
    {
        return;
    }
	
	
    s8 achBuf[512] = {0};
    s32 nPrefix = snprintf(achBuf, sizeof(achBuf), "[CCtrl]: ");
    achBuf[ sizeof(achBuf) - 1 ] = '\0';
	
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(achBuf + nPrefix, sizeof(achBuf) - nPrefix - 1, fmt, argptr);
    va_end(argptr);
	
    LogPrint( nPrtLevel, MID_ENTRANCE_APS, achBuf );
    //OspPrintf(bScreen, bFile, achBuf);
}

/************************************DB******************************************/
API void dbpooldump()
{
	OspInstDump(AID_APS_CENTERCTRL, CInstance::DAEMON, APS_DBPOOL_STATE_DUMP);
    return;
}

API void dbloglevel(s32 nDbgLvl)
{
	if (nDbgLvl < LOG_LVL_ERROR)
	{
		nDbgLvl = LOG_LVL_ERROR;
	}

	if (nDbgLvl > LOG_LVL_DETAIL)
	{
		nDbgLvl = LOG_LVL_DETAIL;
	}
	g_nDBPrtLevel = nDbgLvl;
}

void DBOprLog(s32 nPrtLevel, const s8 *fmt, ...)
{
	BOOL32 bScreen = nPrtLevel <= g_nDBPrtLevel;
	BOOL32 bFile = nPrtLevel <= LOG_LVL_KEYSTATUS;
	
	if (FALSE == bScreen && FALSE == bFile)
    {
        return;
    }
	
	
    s8 achBuf[512] = {0};
    s32 nPrefix = snprintf(achBuf, sizeof(achBuf), "[DBOpr]: ");
    achBuf[ sizeof(achBuf) - 1 ] = '\0';
	
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(achBuf + nPrefix, sizeof(achBuf) - nPrefix - 1, fmt, argptr);
    va_end(argptr);
	
    LogPrint( nPrtLevel, MID_ENTRANCE_APS, achBuf );
    //OspPrintf(bScreen, bFile, achBuf);	
}

/************************************ULC Ssn****************************************/
API void clientnode()
{
	OspInstDump(AID_OLD_26_APS_ULCSSN, CInstance::DAEMON, APS_DUMP_ULC_STATE_DUMP);
	return;
}

API void clientloglevel(s32 nDbgLvl)
{
	if (nDbgLvl < LOG_LVL_ERROR)
    {
        nDbgLvl = LOG_LVL_ERROR;
    }
	
    if (nDbgLvl > LOG_LVL_DETAIL)
    {
        nDbgLvl = LOG_LVL_DETAIL;
    }
	
    g_nUlcPrtLevel = nDbgLvl;
}

void ClientSsnLog(s32 nPrtLevel, const s8 *fmt, ...)
{
	BOOL32 bScreen = nPrtLevel <= g_nUlcPrtLevel;
	BOOL32 bFile = nPrtLevel <= LOG_LVL_KEYSTATUS;

	if (FALSE == bScreen && FALSE == bFile)
    {
        return;
    }
	
	
    s8 achBuf[512 * 10] = {0};
    s32 nPrefix = snprintf(achBuf, sizeof(achBuf), "[ClientSsn]: ");
    achBuf[ sizeof(achBuf) - 1 ] = '\0';
	
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(achBuf + nPrefix, sizeof(achBuf) - nPrefix - 1, fmt, argptr);
    va_end(argptr);
	
    if (strlen(achBuf) < 108)   //超过一定长度，kdvlog会直接丢弃，不打印出来
    {
        LogPrint( nPrtLevel, MID_ENTRANCE_APS, achBuf );
    }
    else
    {
        OspPrintf(bScreen, FALSE, achBuf);
    }
}

/************************************HTTP Ssn****************************************/

API void httploglevel(s32 nDbgLvl)
{
    if (nDbgLvl < LOG_LVL_ERROR)
    {
        nDbgLvl = LOG_LVL_ERROR;
    }

    if (nDbgLvl > LOG_LVL_DETAIL)
    {
        nDbgLvl = LOG_LVL_DETAIL;
    }

    g_nHttpPrtLevel = nDbgLvl;
}

void HttpSsnLog(s32 nPrtLevel, const s8 *fmt, ...)
{
    BOOL32 bScreen = nPrtLevel <= g_nHttpPrtLevel;
    BOOL32 bFile = nPrtLevel <= LOG_LVL_KEYSTATUS;

    if (FALSE == bScreen && FALSE == bFile)
    {
        return;
    }


    s8 achBuf[512 * 10] = {0};
    s32 nPrefix = snprintf(achBuf, sizeof(achBuf), "[HttpSsn]: ");
    achBuf[ sizeof(achBuf) - 1 ] = '\0';

    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(achBuf + nPrefix, sizeof(achBuf) - nPrefix - 1, fmt, argptr);
    va_end(argptr);

    if (strlen(achBuf) < 108)   //超过一定长度，kdvlog会直接丢弃，不打印出来
    {
        LogPrint( nPrtLevel, MID_ENTRANCE_APS, achBuf );
    }
    else
    {
        OspPrintf(bScreen, FALSE, achBuf);
    }
}

/**********************************Con3As****************************************/

API void consaloglevel(s32 nDbgLvl)
{
	if (nDbgLvl < LOG_LVL_ERROR)
	{
		nDbgLvl = LOG_LVL_ERROR;
	}

	if (nDbgLvl > LOG_LVL_DETAIL)
	{
		nDbgLvl = LOG_LVL_DETAIL;
	}

	g_nConSaPrtLevel = nDbgLvl;
}

void ConSaSsnLog(s32 nPrtLevel, const s8 *fmt, ...)
{
	BOOL32 bScreen = nPrtLevel <= g_nConSaPrtLevel;
	BOOL32 bFile = nPrtLevel <= LOG_LVL_KEYSTATUS;
	
	if (FALSE == bScreen && FALSE == bFile)
    {
        return;
    }
	
	
    s8 achBuf[512] = {0};
    s32 nPrefix = snprintf(achBuf, sizeof(achBuf), "[ConSaSSn]: ");
    achBuf[ sizeof(achBuf) - 1 ] = '\0';
	
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(achBuf + nPrefix, sizeof(achBuf) - nPrefix - 1, fmt, argptr);
    va_end(argptr);
	
    LogPrint( nPrtLevel, MID_ENTRANCE_APS, achBuf );
    //OspPrintf(bScreen, bFile, achBuf);	
}

/**************************************SrvlistMgr*****************************************/
API void srvlistmgrloglevel(s32 nDbglvl)
{
	if (nDbglvl < LOG_LVL_ERROR)
	{
		nDbglvl = LOG_LVL_ERROR;
	}

	if (nDbglvl > LOG_LVL_DETAIL)
	{
		nDbglvl = LOG_LVL_DETAIL;
	}

	g_nSrvListPartLevel = nDbglvl;
}

void SrvListMgrLog(s32 nPrtLevel, const s8 *fmt, ...)
{
	BOOL32 bScreen = nPrtLevel <= g_nSrvListPartLevel;
	BOOL32 bFile = nPrtLevel <= LOG_LVL_KEYSTATUS;
	
	if (FALSE == bScreen && FALSE == bFile)
    {
        return;
    }
	
	
    s8 achBuf[512] = {0};
    s32 nPrefix = snprintf(achBuf, sizeof(achBuf), "[SrvListMgr]: ");
    achBuf[ sizeof(achBuf) - 1 ] = '\0';
	
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(achBuf + nPrefix, sizeof(achBuf) - nPrefix - 1, fmt, argptr);
    va_end(argptr);
	
    LogPrint( nPrtLevel, MID_ENTRANCE_APS, achBuf );
    //OspPrintf(bScreen, bFile, achBuf);	
}

/*打印帮助*/
CPrintFunInOut::CPrintFunInOut(const s8* pchFunName, const u16 wModule/* = MID_ENTRANCE_APS*/,
	const u8 byLevel/* = LOG_LVL_KEYSTATUS*/)
	:m_funName(pchFunName), m_wModule(wModule), m_byLevel(byLevel)
{
	std::string moduleSeperator(GetModuleSeperator());
	LogPrint(m_byLevel, m_wModule, "%sIN~%s[%s]%sIN~%s\n", moduleSeperator.c_str(),
		moduleSeperator.c_str(), m_funName.c_str(), moduleSeperator.c_str(),
		moduleSeperator.c_str());
}
CPrintFunInOut::CPrintFunInOut(const s8* pchFunName, const s8* pchDescribe, ...)
	:m_funName(pchFunName), m_wModule(MID_ENTRANCE_APS), m_byLevel(LOG_LVL_KEYSTATUS)
{
	va_list args;
	va_start(args, pchDescribe);
	s8 achDescribe[10240 + 1] = { 0 };
	vsnprintf(achDescribe, sizeof(achDescribe), pchDescribe, args);
	m_describe = achDescribe;
	LogFunIn();
	va_end(args);
}
CPrintFunInOut::CPrintFunInOut(const s8* pchFunName, const u16 wModule, const s8* pchDescribe, ...)
	:m_funName(pchFunName), m_wModule(wModule), m_byLevel(LOG_LVL_KEYSTATUS)
{
	va_list args;
	va_start(args, pchDescribe);
	s8 achDescribe[10240 + 1] = { 0 };
	vsnprintf(achDescribe, sizeof(achDescribe), pchDescribe, args);
	m_describe = achDescribe;
	LogFunIn();
	va_end(args);
}
CPrintFunInOut::CPrintFunInOut(const s8* pchFunName, const u16 wModule,
	const u8 byLevel, const s8* pchDescribe, ...)
	:m_funName(pchFunName), m_wModule(wModule), m_byLevel(byLevel)
{
	va_list args;
	va_start(args, pchDescribe);
	s8 achDescribe[10240 + 1] = { 0 };
	vsnprintf(achDescribe, sizeof(achDescribe), pchDescribe, args);
	m_describe = achDescribe;
	LogFunIn();
	va_end(args);
}
CPrintFunInOut::~CPrintFunInOut()
{
	std::string moduleSeperator(GetModuleSeperator());
	LogPrint(m_byLevel, m_wModule, "%sOUT%s[%s]%sOUT%s\n\n", moduleSeperator.c_str(),
		moduleSeperator.c_str(), m_funName.c_str(), moduleSeperator.c_str(),
		moduleSeperator.c_str());
}
void CPrintFunInOut::LogFunIn()
{
	std::string moduleSeperator(GetModuleSeperator());
	LogPrint(m_byLevel, m_wModule, "%sIN~%s[%s]%sIN~%s#Describe:[%s]\n",
		moduleSeperator.c_str(), moduleSeperator.c_str(),
		m_funName.c_str(), moduleSeperator.c_str(), moduleSeperator.c_str(),
		m_describe.c_str());
}
std::string CPrintFunInOut::GetModuleSeperator()
{
	return string(6, '-');
}

/********************************* 服务器状态 *********************************************/
API void showsrv()
{
	g_cSrvListMgr.PrintAllSrvsInfo();
	return;
}

API void showmtinfo()
{
	g_cCenterCtrlApp.Print();
	return;
}

API void showcfg()
{
	CPrintFunInOut cPrintFun(__FUNCTION__);
	g_cCfgFile.PrintCfg();
	OspPrintf(TRUE, FALSE, "PlatFormDomainMOID: %s\n\n", g_cSrvListMgr.GetPlatformDomainMOID());
	return;
}

API void loadlogin()
{
	g_cCfgFile.LoadTestLoginInfo();
}

API void login()
{
	CLoginExtend tExtend;
	const CLoginInfo& tLoginInfo = g_cCfgFile.GetTestLoginInfo();
    tExtend.SetDetailBody(tLoginInfo);
    ::OspPost(MAKEIID(AID_APS_HTTP_SSN, CInstance::DAEMON), MT_APS_LOGIN_REQ, &tExtend, sizeof(tExtend));
}

API void settryapsspan(const u32 dwSpan)
{
	g_cCfgFile.SetMtTryApsSpan(dwSpan);
}

API void showbackup()
{
	CLoginExtend tExtend;
	g_cCenterCtrlApp.GetDBPool().InsertMsgToDBPool(SEARCH_PLATFORM_BACKUP_CONFIG_REQ, (const u8*)&tExtend, sizeof(tExtend));
}

/*lint -restore*/