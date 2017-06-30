
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

//ĳ��������ĳ���豸��Ϣ
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

    //�������µĶ��ƽ̨��
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

    //��ȡ
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

    //�Ƿ�����
    u8 byUsedFlag = 0;
    wErrId = m_cDBEngine.GetFieldValue(SRV_DEVICE_USED_FLAG, byUsedFlag);
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetUsedFlag( (0 != byUsedFlag ) ? TRUE: FALSE );

    //����ƽ̨��MOID
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(SRV_DEVICE_PLATFORMDOMAIN_MOID, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetPlatformDomainMOID( szValue );

    //�����û���MOID(��������ֱ������ĳ���û���)
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(SRV_DEVICE_USERDOMAIN_MOID, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetUserDomainMOID( szValue );

    //�豸IP
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(SRV_DEVICE_IPADDR, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetIpAddress( szValue );

    //��������
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(SRV_DEVICE_NET_DOMAIN_NAME, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tSrvDeviceInfo.SetNetDomainName( szValue );

    //SUS�豸��OEM��ʶ
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

    //����
    memset(szValue, 0, sizeof(szValue));
    wErrId = m_cDBEngine.GetFieldValue(PLATFOEM_DOMAIN_NAME, szValue, sizeof(szValue));
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }
    tPlatformDomain.SetDomainName( szValue );

    //����������MOID
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

    /**********************************************��֤�˺��Ƿ����*****************************************************/
    TSqlString tSqlString;
    tSqlString.Format("select A.%s, A.%s, A.%s, A.%s, A.%s, A.%s, A.%s, A.%s, B.%s from %s A, %s B where A.%s = B.%s and A.%s = %u and A.%s = %u and (A.%s = \"%s\" or A.%s = \"%s\" or A.%s = \"%s\" or A.%s = \"%s\") ", 
        USER_MOID, USER_ACCOUNT, USER_E164_NO, USER_JID, USER_EMAIL, USER_PASSWORD, USER_USERDOMAIN_MOID, USER_PAS_MOID, USER_PROFILE_RESTRICT, 
        TABLE_USER_INFO, TABLE_USER_PROFILE,
        USER_MOID, USER_PROFILE_MOID, USER_USED_FLAG, 1, USER_BINDED/*�豸�󶨺�,��E164�Ų�ѯ,����2����¼,��Email�ļ�¼Ϊ׼*/, 0, 
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

    /**********************************************     �˺���֤     ****************************************************/
    //1.�Ƿ����
    if (m_cDBEngine.IsEof())
    {
        DBOprLog(LOG_LVL_WARNING, "[Authentication]The Record is not existed!\n");
        return APS_ERR_ACCOUNT_NAME_NOTEXIST;
    }

    s8 szValue[1024 + 1] = {0};

    //2.�����Ƿ���ȷ
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

    //�û��Զ����˺�
    s8 szAccount[OPERATION_MAXLEN_ACCOUNT+1] = {0};
    wErrId = m_cDBEngine.GetFieldValue(USER_ACCOUNT, szAccount, sizeof(szAccount)); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }
    tMtInfo.SetAccount(szAccount);

    //���ȵ�¼��PAS������
    s8 szPasMOID[OPERATION_MAXLEN_MOID+1] = {0};
    wErrId = m_cDBEngine.GetFieldValue(USER_PAS_MOID, szPasMOID, sizeof(szPasMOID)); 
    if (SUCCESS_DBENGINE != wErrId)
    {
        //return APS_ERR_DB_EXEC_FAIL;
    }
    tMtInfo.SetPASMoid(szPasMOID);

    /********************************************�ж��豸�����Ƿ�����**********************************************/	
    if (strlen(szE164NO) != 0)  //��E164�ŵģ���Ҫ���е�¼���������ж�
    {
        //Type restrict
        memset(szValue, 0, sizeof(szValue));
        wErrId = m_cDBEngine.GetFieldValue(USER_PROFILE_RESTRICT, szValue, sizeof(szValue)); 
        if (SUCCESS_DBENGINE != wErrId)
        {
            return APS_ERR_DB_EXEC_FAIL;
        }

        BOOL32 bTypeValid = FALSE;
        //��";"�ָ�
        const s8 seps[] = ";";
        s8* pToken = strtok(szValue, seps);
        while (NULL != pToken)
        {
			DBOprLog(LOG_LVL_DETAIL, "[Authentication] Mt Device %s, Token %s\n", tLoginInfo.GetDeviceType() , pToken );
            if (0 == strcasecmp(tLoginInfo.GetDeviceType(), pToken))    //���Դ�Сд
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

    /*********************************************��ȡ������Ϣ*****************************************************/	   
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

    /*********************************************�����û���*****************************************************/
    tSqlString.Empty();
    tSqlString.Format("SELECT DISTINCT * FROM %s  WHERE %s = \"%s\" ",
        TABLE_USER_DOMAIN_INFO, USERDOMAIN_MOID, szUserDomainMOID);

    wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        DBOprLog(LOG_LVL_ERROR, "[Authentication] SQL exec error: %s\n", tSqlString.GetBuf());
        return APS_ERR_DB_EXEC_FAIL;
    }

    //����ע���ƽ̨��
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

    /*********************************************��ȡ�û�Ȩ����Ϣ*****************************************************/
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

    /*********************************************��ȡ�û���Ȩ��*****************************************************/
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

    /*********************************************�ϲ�Ȩ��*****************************************************/
    //enableCall��enableMeeting��enableSatellite��enableSMS��enableWeibo��5��Ȩ�ޣ�
    //����û���û�У���ʹ�ն�����ЩȨ�ޣ�ҲӦ����Ϊ��Ч
    //�绰����
    if ( setUserDomainPrivilege.end() == setUserDomainPrivilege.find(USERDOMAIN_PRIVILEGE_CALL) )
    {
        setUserPrivilege.erase(USER_PRIVILEGE_CALL);
    }

    //�������
    if ( setUserDomainPrivilege.end() == setUserDomainPrivilege.find(USERDOMAIN_PRIVILEGE_MEETING) )
    {
        setUserPrivilege.erase(USER_PRIVILEGE_MEETING);
        setUserPrivilege.erase(USER_PRIVILEGE_MEETING_ADMIN);
    }

    //������·
    if ( setUserDomainPrivilege.end() == setUserDomainPrivilege.find(USERDOMAIN_PRIVILEGE_SATELLITE) )
    {
        setUserPrivilege.erase(USER_PRIVILEGE_SATELLITE);
        setUserPrivilege.erase(USER_PRIVILEGE_SATELLITE_P2P);
    }

    //��ܶ���Ȩ��
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

    /*********************************************��ȡ��������������*****************************************************/
    tSqlString.Empty();
    tSqlString.Format("select %s from %s where %s = \"%s\" ", 
        SYSCFG_VALUE, TABLE_SYS_CFG, SYSCFG_KEY, SYSCFG_SYSTEM_MODE);

    wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        return APS_ERR_DB_EXEC_FAIL;
    }

    //�Ƿ��Խ�ƽ̨
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

    //��֧�ֶ˿�ý��
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



