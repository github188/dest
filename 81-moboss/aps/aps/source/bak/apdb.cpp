
#pragma warning (disable:4786 )

#include <set>
#include "apdb.h"
#include "sqlstring.h"
#include "dbtabledef.h"
#include "debugprint.h"
#include "errorcode.h"
#include "bossutils.h"

#ifdef WIN32
#ifndef strcasecmp
#define strcasecmp		_stricmp  
#endif
#endif


CApDB::CApDB()
{

}

CApDB::~CApDB()
{

}

u16 CApDB::GetSrvDevInfoByGUID( IN const s8* szDevGUID, OUT CServerDevice& tSrvDevInfo )
{
    if (NULL == szDevGUID)
    {
        DBOprLog( LOG_LVL_WARNING, "[CApDB::GetSrvDevInfoByGUID] NULL == szDevGUID\n");
        return COMMON_ERR_DBENGINE;
    }

    u16 wErrId = COMMON_ERR_DBENGINE;
    TSqlString tSqlString;

    tSqlString.Format("SELECT DISTINCT * FROM %s WHERE %s = \"%s\"",
        TABLE_SERVER_DEVICE_INFO, SRV_DEVICE_GUID, szDevGUID);

    wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }

    if ( m_cDBEngine.IsEof() )
    {
        return COMMON_ERR_DBENGINE;
    }

    wErrId = ReadSrvDeviceInfo(tSrvDevInfo);
    if ( SUCCESS_DBENGINE != wErrId )
    {
        DBOprLog(LOG_LVL_WARNING, "[CApDB::GetSrvDevInfoByGUID] wErrId.%u, ReadSrvDeviceInfo fial!\n", wErrId);
        return wErrId;
    }

    return SUCCESS_DBENGINE;	
}

//某服务域下某种设备信息
u16 CApDB::GetBriefSrvInfo(IN const s8* ServiceDomainMOID, IN u16 dwDeviceType, OUT std::set<CBriefSrvInfo>& setBriefSrvInfo)
{
    setBriefSrvInfo.clear();
    if (NULL == ServiceDomainMOID)
    {
        DBOprLog( LOG_LVL_WARNING, "[CApDB::GetBriefSrvInfo] NULL == ServiceDomainMOID\n");
        return COMMON_ERR_DBENGINE;
    }

    u16 wErrId = COMMON_ERR_DBENGINE;
    TSqlString tSqlString;
    TSqlString tSubSqlString;

    //服务域下的多个平台域
    tSubSqlString.Format("SELECT %s FROM %s WHERE %s = \"%s\"",
        PLATFOEM_DOMAIN_MOID, TABLE_PLATFOEM_DOMAIN_INFO, PLATFOEM_DOMAIN_SERVICEDOMAIN_MOID, ServiceDomainMOID);

    tSqlString.Format("SELECT DISTINCT * FROM %s WHERE %s = %d AND %s IN (%s) AND %s = %d",
        TABLE_SERVER_DEVICE_INFO, 
        SRV_DEVICE_USED_FLAG, 1, 
        SRV_DEVICE_PLATFORMDOMAIN_MOID, tSubSqlString.GetBuf(), 
        SRV_DEVICE_SERVER_TYPE, dwDeviceType);

    wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }

    //读取
    CServerDevice tInfo;
    CBriefSrvInfo tBriefInfo;
    while( !m_cDBEngine.IsEof() )
    {
        //tInfo.Init();	
        wErrId = ReadSrvDeviceInfo(tInfo);
        if (SUCCESS_DBENGINE != wErrId)
        {
            DBOprLog(LOG_LVL_WARNING, "[CApDB::GetBriefSrvInfo] wErrId.%u, ReadSrvDeviceInfo fial!\n", wErrId);
            return wErrId;
        }

        tBriefInfo = Convert(tInfo);
        setBriefSrvInfo.insert(tBriefInfo);		
        m_cDBEngine.MoveNext();		
    }

    m_cDBEngine.ClearResults();
    return SUCCESS_DBENGINE;
}

u16 CApDB::ReadSrvDeviceInfo( OUT CServerDevice& tSrvDeviceInfo )
{
    tSrvDeviceInfo.Init();

    s8 szValue[MAXLEN_DB_VALUE+1] = {0};
    u16 wErrId = SUCCESS_DBENGINE;

    //GUID
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue( SRV_DEVICE_GUID , szValue , sizeof(szValue) );
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetGUID( szValue );

    //MOID
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue( SRV_DEVICE_MOID , szValue , sizeof(szValue) );
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetMOID( szValue );

    //是否启用
    u8 byUsedFlag = 0;
    wErrId = m_cDBEngine.GetFieldValue(SRV_DEVICE_USED_FLAG, byUsedFlag);
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetUsedFlag( (0 != byUsedFlag ) ? TRUE: FALSE );

    //所属平台域MOID
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(SRV_DEVICE_PLATFORMDOMAIN_MOID, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetPlatformDomainMOID( szValue );

    //所属用户域MOID(服务器可直接属于某个用户域)
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(SRV_DEVICE_USERDOMAIN_MOID, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetUserDomainMOID( szValue );

    //设备IP
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(SRV_DEVICE_IPADDR, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetIpAddress( szValue );

    //网络域名
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(SRV_DEVICE_NET_DOMAIN_NAME, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetNetDomainName( szValue );

    //SUS设备的OEM标识
    memset( szValue, 0, sizeof(szValue) );
    wErrId = m_cDBEngine.GetFieldValue(SRV_DEVICE_OEM_MARK, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetOemMark( szValue );

    return SUCCESS_DBENGINE;
}

u16 CApDB::GetAllPlatformDoaminMOID(IN const s8* pchServiceDomainMOID, OUT std::set<std::string>& setPlatformDoaminMOID)
{
    if (NULL == pchServiceDomainMOID)
    {
        DBOprLog( LOG_LVL_WARNING, "[CApDB::GetAllPlatformDoaminMOID] NULL == pchServiceDoaminMOID\n");
        return COMMON_ERR_DBENGINE;
    }

    TSqlString tSqlString;
    tSqlString.Format("SELECT DISTINCT * FROM %s WHERE %s = \"%s\"",
        TABLE_PLATFOEM_DOMAIN_INFO, PLATFOEM_DOMAIN_SERVICEDOMAIN_MOID, pchServiceDomainMOID);
    u16 wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }

	while (!m_cDBEngine.IsEof())
	{
		s8 szValue[MAXLEN_DB_VALUE+1] = {0};
		wErrId = m_cDBEngine.GetFieldValue( PLATFOEM_DOMAIN_MOID , szValue , sizeof(szValue) );
		if (SUCCESS_DBENGINE != wErrId)
		{
			break;
		}
		setPlatformDoaminMOID.insert(szValue);
		m_cDBEngine.MoveNext();
	}
	return wErrId;
}

u16 CApDB::GetPlatformDomain( IN const s8* szPlatformDomainMOID, OUT CPlatformDomain&  tPlatformDomain )
{
    if (NULL == szPlatformDomainMOID)
    {
        DBOprLog( LOG_LVL_WARNING, "[CApDB::GetPlatformDomain] NULL == szPlatformDomainMOID\n");
        return COMMON_ERR_DBENGINE;
    }

    u16 wErrId = COMMON_ERR_DBENGINE;
    TSqlString tSqlString;

    tSqlString.Format("SELECT DISTINCT * FROM %s WHERE %s = \"%s\"",
        TABLE_PLATFOEM_DOMAIN_INFO, PLATFOEM_DOMAIN_MOID, szPlatformDomainMOID);

    wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }

    if ( m_cDBEngine.IsEof() )
    {
        return COMMON_ERR_DBENGINE;
    }

    wErrId = ReadPlatformDomainInfo(tPlatformDomain);
    if ( SUCCESS_DBENGINE != wErrId )
    {
        DBOprLog(LOG_LVL_WARNING, "[CApDB::GetPlatformDomain] wErrId.%u, ReadPlatformDomainInfo fial!\n", wErrId);
        return wErrId;
    }

    return SUCCESS_DBENGINE;	
}

u16 CApDB::ReadPlatformDomainInfo( OUT CPlatformDomain& tPlatformDomain )
{
    tPlatformDomain.Init();

    s8 szValue[MAXLEN_DB_VALUE+1] = {0};
    u16 wErrId = SUCCESS_DBENGINE;

    //MOID
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue( PLATFOEM_DOMAIN_MOID , szValue , sizeof(szValue) );
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tPlatformDomain.SetMOID( szValue );

    //域名
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(PLATFOEM_DOMAIN_NAME, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tPlatformDomain.SetDomainName( szValue );

    //所属服务域MOID
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(PLATFOEM_DOMAIN_SERVICEDOMAIN_MOID, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tPlatformDomain.SetServiceDomainMOID( szValue );

    return SUCCESS_DBENGINE;
}

u16 CApDB::Authentication(IN const CLoginInfo &tLoginInfo, OUT CDBMtInfo& tMtInfo, OUT CPlatformAttr& tPlatformAttr)
{
    tMtInfo.Init();
    tPlatformAttr.Init();

    /**********************************************验证账号是否存在*****************************************************/
    TSqlString tSqlString;
    tSqlString.Format("select A.%s, A.%s, A.%s, A.%s, A.%s, A.%s, A.%s, A.%s, B.%s from %s A, %s B where A.%s = B.%s and A.%s = %u and A.%s = %u and (A.%s = \"%s\" or A.%s = \"%s\" or A.%s = \"%s\" or A.%s = \"%s\") ", 
        USER_MOID, USER_ACCOUNT, USER_E164_NO, USER_JID, USER_EMAIL, USER_PASSWORD, USER_USERDOMAIN_MOID, USER_PAS_MOID, USER_PROFILE_RESTRICT, 
        TABLE_USER_INFO, TABLE_USER_PROFILE,
        USER_MOID, USER_PROFILE_MOID, USER_USED_FLAG, 1, USER_BINDED/*设备绑定后,以E164号查询,会有2条记录,以Email的记录为准*/, 0, 
        USER_E164_NO, tLoginInfo.GetUserName(), 
        USER_EMAIL, tLoginInfo.GetUserName(),
        USER_ACCOUNT, tLoginInfo.GetUserName(), 
        USER_MOBILE, tLoginInfo.GetUserName() );

    u16 wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        DBOprLog(LOG_LVL_ERROR, "[Authentication]SQL exec error: %s\n", tSqlString.GetBuf());
        return APS_ERR_DB_EXEC_FAIL;
    }

    /**********************************************     账号验证     ****************************************************/
    //1.是否存在
    if (m_cDBEngine.IsEof())
    {
        DBOprLog(LOG_LVL_WARNING, "[Authentication]The Record is not existed!\n");
        return APS_ERR_ACCOUNT_NAME_NOTEXIST;
    }

    s8 szValue[1024 + 1] = {0};

    //2.密码是否正确
    if ( 0 != strcasecmp(tLoginInfo.GetDeviceType(), "TS6610") )
    {
        wErrId = m_cDBEngine.GetFieldValue(USER_PASSWORD, szValue, sizeof(szValue)); 
        if (SUCCESS_DBENGINE != wErrId)
        {
            return APS_ERR_DB_EXEC_FAIL;
        }

        if (strcmp(tLoginInfo.GetPassWord(), szValue) != 0)
        {
            return APS_ERR_ACCOUNT_PWD_ERROR;
        }
    }
    else
    {
        DBOprLog(LOG_LVL_WARNING, "[Authentication]TS6610 do not match the password!\n");
    }

    //E164
    s8 szE164NO[OPERATION_MAXLEN_DEVE164+1] = {0};
    wErrId = m_cDBEngine.GetFieldValue(USER_E164_NO, szE164NO, sizeof(szE164NO)); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }
    tMtInfo.SetE164NO(szE164NO);

    //用户自定义账号
    s8 szAccount[OPERATION_MAXLEN_ACCOUNT+1] = {0};
    wErrId = m_cDBEngine.GetFieldValue(USER_ACCOUNT, szAccount, sizeof(szAccount)); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }
    tMtInfo.SetAccount(szAccount);

    //优先登录的PAS服务器
    s8 szPasMOID[OPERATION_MAXLEN_MOID+1] = {0};
    wErrId = m_cDBEngine.GetFieldValue(USER_PAS_MOID, szPasMOID, sizeof(szPasMOID)); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        //return APS_ERR_DB_EXEC_FAIL;
    }
    tMtInfo.SetPASMoid(szPasMOID);

    /********************************************判断设备类型是否受限**********************************************/	
    if (strlen(szE164NO) != 0)  //有E164号的，都要进行登录类型限制判断
    {
        //Type restrict
        memset(szValue, 0, sizeof(szValue));
        wErrId = m_cDBEngine.GetFieldValue(USER_PROFILE_RESTRICT, szValue, sizeof(szValue)); 
        if (SUCCESS_DBENGINE != wErrId)
        {
            return APS_ERR_DB_EXEC_FAIL;
        }

        BOOL32 bTypeValid = FALSE;
        //以";"分隔
        const s8 seps[] = ";";
        s8* pToken = strtok(szValue, seps);
        while (NULL != pToken)
        {
			DBOprLog(LOG_LVL_DETAIL, "[Authentication] Mt Device %s, Token %s\n", tLoginInfo.GetDeviceType() , pToken );
            if (0 == strcasecmp(tLoginInfo.GetDeviceType(), pToken))    //忽略大小写
            {
                bTypeValid = TRUE;
                break;
            }

            pToken = strtok( NULL, seps );
        }

        if (!bTypeValid)
        {
            return APS_ERR_DEVTYPE_RESTRICT;
        }		
    }

    /*********************************************获取基本信息*****************************************************/	   
    //Email
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(USER_EMAIL, szValue, sizeof(szValue)); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }
    tMtInfo.SetEmail(szValue);

    //MOID
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(USER_MOID, szValue, sizeof(szValue)); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }
    tMtInfo.SetMoid(szValue);

    //XmppJID
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(USER_JID, szValue, sizeof(szValue)); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }
    tMtInfo.SetXmppJID(szValue);

    //UserDomain MOID
    s8 szUserDomainMOID[OPERATION_MAXLEN_MOID + 1] = {0};
    wErrId = m_cDBEngine.GetFieldValue(USER_USERDOMAIN_MOID, szUserDomainMOID, sizeof(szUserDomainMOID)); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }
    tMtInfo.SetUserDomainMoid(szUserDomainMOID);

    /*********************************************所属用户域*****************************************************/
    tSqlString.Empty();
    tSqlString.Format("SELECT DISTINCT * FROM %s  WHERE %s = \"%s\" ",
        TABLE_USER_DOMAIN_INFO, USERDOMAIN_MOID, szUserDomainMOID);

    wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        DBOprLog(LOG_LVL_ERROR, "[Authentication] SQL exec error: %s\n", tSqlString.GetBuf());
        return APS_ERR_DB_EXEC_FAIL;
    }

    //优先注册的平台域
    s8 szPlatfromDomainMOID[OPERATION_MAXLEN_MOID + 1] = {0};
    wErrId = m_cDBEngine.GetFieldValue(USERDOMAIN_PLATFOEMDOMAIN_MOID, szPlatfromDomainMOID, sizeof(szPlatfromDomainMOID)); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }
    tMtInfo.SetPlatformDomainMoid(szPlatfromDomainMOID);
    
    //ServiceDomain MOID
    s8 szServiceDomainMOID[OPERATION_MAXLEN_MOID + 1] = {0};
    wErrId = m_cDBEngine.GetFieldValue(USERDOMAIN_SERVICEDOMAIN_MOID, szServiceDomainMOID, sizeof(szServiceDomainMOID)); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }
    tMtInfo.SetServiceDomainMoid(szServiceDomainMOID);

    /*********************************************获取用户权限信息*****************************************************/
    tSqlString.Empty();
    tSqlString.Format("select %s from %s where %s = \"%s\" ", 
        USER_PRIVILEGE_KEY, TABLE_USER_PRIVILEGE_DATA, USER_PRIVILEGE_MOID, tMtInfo.GetMoid());

    wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        DBOprLog(LOG_LVL_ERROR, "[Authentication] SQL exec error: %s\n", tSqlString.GetBuf());
        return APS_ERR_DB_EXEC_FAIL;
    }

    std::set<std::string> setUserPrivilege;
    while(!m_cDBEngine.IsEof())
    {
        memset(szValue, 0, sizeof(szValue));
        wErrId = m_cDBEngine.GetFieldValue(USER_PRIVILEGE_KEY, szValue, sizeof(szValue)); 
        if (SUCCESS_DBENGINE != wErrId)
        {
            return APS_ERR_DB_EXEC_FAIL;
        }

        setUserPrivilege.insert(szValue);
        m_cDBEngine.MoveNext();
    }

    /*********************************************获取用户域权限*****************************************************/
    tSqlString.Empty();
    tSqlString.Format("select %s from %s where %s = \"%s\" ", 
        USERDOMAIN_PRIVILEGE_KEY, TABLE_USERDOMAIN_PRIVILEGE_DATA, USERDOMAIN_PRIVILEGE_MOID, szUserDomainMOID);

    wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        DBOprLog(LOG_LVL_ERROR, "[Authentication] SQL exec error: %s\n", tSqlString.GetBuf());
        return APS_ERR_DB_EXEC_FAIL;
    }

    std::set<std::string> setUserDomainPrivilege;
    while(!m_cDBEngine.IsEof())
    {
        memset(szValue, 0, sizeof(szValue));
        wErrId = m_cDBEngine.GetFieldValue(USER_PRIVILEGE_KEY, szValue, sizeof(szValue)); 
        if (SUCCESS_DBENGINE != wErrId)
        {
            return APS_ERR_DB_EXEC_FAIL;
        }

        setUserDomainPrivilege.insert(szValue);        
        m_cDBEngine.MoveNext();
    }

    /*********************************************合并权限*****************************************************/
    //enableCall、enableMeeting、enableSatellite、enableSMS、enableWeibo这5个权限，
    //如果用户域没有，即使终端有这些权限，也应该视为无效
    //电话会议
    if ( setUserDomainPrivilege.end() == setUserDomainPrivilege.find(USERDOMAIN_PRIVILEGE_CALL) )
    {
        setUserPrivilege.erase(USER_PRIVILEGE_CALL);
    }

    //会议管理
    if ( setUserDomainPrivilege.end() == setUserDomainPrivilege.find(USERDOMAIN_PRIVILEGE_MEETING) )
    {
        setUserPrivilege.erase(USER_PRIVILEGE_MEETING);
        setUserPrivilege.erase(USER_PRIVILEGE_MEETING_ADMIN);
    }

    //卫星线路
    if ( setUserDomainPrivilege.end() == setUserDomainPrivilege.find(USERDOMAIN_PRIVILEGE_SATELLITE) )
    {
        setUserPrivilege.erase(USER_PRIVILEGE_SATELLITE);
        setUserPrivilege.erase(USER_PRIVILEGE_SATELLITE_P2P);
    }

    //会管短信权限
    if ( setUserDomainPrivilege.end() == setUserDomainPrivilege.find(USERDOMAIN_PRIVILEGE_MEETINGSMS) )
    {
        setUserPrivilege.erase(USER_PRIVILEGE_MEETINGSMS);
    }

    //WeiBo
    if ( setUserDomainPrivilege.end() == setUserDomainPrivilege.find(USERDOMAIN_PRIVILEGE_WEIBO) )
    {
        setUserPrivilege.erase(USER_PRIVILEGE_WEIBO);
        setUserPrivilege.erase(USER_PRIVILEGE_WEIBO_ADMIN);
    }

    std::set<std::string>::iterator iter = setUserPrivilege.begin();
    for ( ; iter != setUserPrivilege.end(); ++iter )
    {
        tMtInfo.AddPrivilege(iter->c_str());
    }    

    /*********************************************获取所属服务域属性*****************************************************/
    tSqlString.Empty();
    tSqlString.Format("select %s from %s where %s = \"%s\" ", 
        SYSCFG_VALUE, TABLE_SYS_CFG, SYSCFG_KEY, SYSCFG_SYSTEM_MODE);

    wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }

    //是否自建平台
    s32 nSelfBuild = 0;
    wErrId = m_cDBEngine.GetFieldValue(SYSCFG_VALUE, nSelfBuild); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }

    if (nSelfBuild != 0)
    {
        tPlatformAttr.SetSelfBuilt(true);
    }
    else
    {
        tPlatformAttr.SetSelfBuilt(false);
    }

    //是支持端口媒体
    if ( setUserDomainPrivilege.end() == setUserDomainPrivilege.find(SERVICEDOMAIN_PRIVILEGE_PORTMEDIA) )
    {
        tPlatformAttr.SetPortMedia(false);
    }
    else
    {
        tPlatformAttr.SetPortMedia(true);
    }


    return SUCCESS_DBENGINE;
}



