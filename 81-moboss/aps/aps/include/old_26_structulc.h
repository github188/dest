

/*=======================================================================
模块名      :ULS
文件名      :old_26_structulc.h
相关文件    :
文件实现功能:ulc与uls通信结构体

作者        :
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/07/26  1.0                       创建
=======================================================================*/
#ifndef _OLD_STRUCT_ULC_H_
#define _OLD_STRUCT_ULC_H_

#include "kdvtype.h"
#include "constuls.h"
#include "operationconst.h"
#include "kdvplist.h"
#include "structulc.h"

#ifdef _JSON_CONVERT_
#include "jsonconvert.h"
#endif //_JSON_CONVERT_

#ifdef WIN32
#ifndef vsnprintf
#define vsnprintf   _vsnprintf
#endif
#ifndef snprintf
#define snprintf    _snprintf
#endif
#endif

//...........................................
#ifdef WIN32
#pragma pack( push )
#pragma pack( 1 )
#ifndef PACKED
#define PACKED
#endif
#endif//.....................................

//...........................................
#ifdef _LINUX_
#ifndef PACKED
#define PACKED __attribute__ ((packed)) 
#endif
#endif//.....................................

class COldSrvAddrInfo
{
public:    
    COldSrvAddrInfo()
    {
        Clear();
    }

    void Clear( void )
    {
        memset( m_szNetDomainName, 0, sizeof(m_szNetDomainName) );
        m_dwIpAddr = 0;
    }

    //操作符重载
    bool operator == ( const COldSrvAddrInfo& tOther ) const
    {
        if (GetIpAddr() == tOther.GetIpAddr() && strcmp(GetNetDomainName(), tOther.GetNetDomainName()) == 0)
        {
            return true;
        }

        return false;		
    }

    bool operator < ( const COldSrvAddrInfo& other ) const
    {
        if (GetIpAddr() < other.GetIpAddr())
        {
            return true;
        }

        if ( strcmp(GetNetDomainName(), other.GetNetDomainName()) < 0 )
        {
            return true;
        }

        return false;
    }

public:
    u32 GetIpAddr( void ) const 
    {
        return ntohl( m_dwIpAddr );
    }
    void SetIpAddr( u32 dwIpAddr )
    {
        m_dwIpAddr = htonl( dwIpAddr );
    }

    const s8* GetNetDomainName( void ) const { return m_szNetDomainName; }
    void SetNetDomainName( const s8 * pchSrvDomainName )
    {
        memset( m_szNetDomainName, 0, sizeof(m_szNetDomainName) );
        if (NULL != pchSrvDomainName)
        {
            strncpy( m_szNetDomainName, pchSrvDomainName, sizeof(m_szNetDomainName)-1 );
            m_szNetDomainName[sizeof(m_szNetDomainName)-1] = '\0';
        }
        return;
    }

    //判断IP是否有效
    BOOL32 IsIpValid( void ) const
    {
        return ( 0 == m_dwIpAddr ) ? FALSE : TRUE;
    }

    //判断域名是否有效
    BOOL32 IsNetDomainNameValid( void )
    {
        return ( 0 == strncmp( m_szNetDomainName, "", sizeof(m_szNetDomainName)) ) ? FALSE : TRUE;
    }

public:
#ifdef _JSON_CONVERT_
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;

        jsonTool.SetValue("m_dwIpAddr", GetIpAddr());
        jsonTool.SetValue("m_szNetDomainName", GetNetDomainName());

        return jsonTool.GetJson();
    }

    BOOL32 JsonToStruct(const s8* szJson)
    {
        if (NULL == szJson)
        {
            return FALSE;
        }

        CJsonConvert jsonTool;        
        BOOL32 bRet = jsonTool.ParseJsonStr(szJson);
        if (!bRet)
        {
            return FALSE;
        }

        //IP地址
        u32 dwIp = 0;
        jsonTool.GetValue("m_dwIpAddr", dwIp);
        SetIpAddr(dwIp);

        //域名
        s8 achDomainName[64 + 1] = {0};
        jsonTool.GetValue("m_szNetDomainName", achDomainName, sizeof(achDomainName));
        SetNetDomainName(achDomainName);

        return TRUE;
    }
#endif //_JSON_CONVERT_

private:
    u32	m_dwIpAddr;				//IP地址
    s8  m_szNetDomainName[64 + 1];	//域名地址
}
PACKED;

static COldSrvAddrInfo Convert(const CSrvAddrInfo& tAddr)
{
    COldSrvAddrInfo tTemp;
    tTemp.SetIpAddr(tAddr.GetIpAddr());
    tTemp.SetNetDomainName(tAddr.GetNetDomainName());

    return tTemp;
}

typedef CKdvpList<COldSrvAddrInfo, MAXNUM_ULSIPLIST>  COld26ApsSrvIpList;



class COld26UlcMtInfo
{
public:
    COld26UlcMtInfo()
    {
        Init();
    }

    void Init()
    {
        memset(m_szMoid, 0, sizeof(m_szMoid));
        memset(m_szE164NO, 0, sizeof(m_szE164NO));
        memset(m_szEmail, 0, sizeof(m_szEmail));
        memset(m_szPrivilegeList, 0, sizeof(m_szPrivilegeList));
    }

    const s8* GetMoid() const { return m_szMoid; }
    void SetMoid(const s8* szMoid) 
    { 
        if (NULL == szMoid)
        {
            memset(m_szMoid, 0, sizeof(m_szMoid));
            return;
        }

        strncpy(m_szMoid, szMoid, sizeof(m_szMoid)-1);
        m_szMoid[sizeof(m_szMoid)-1] = '\0';
    }

    const s8* GetE164NO() const { return m_szE164NO; }
    void SetE164NO(const s8* szE164NO) 
    { 
        if (NULL == szE164NO)
        {
            memset(m_szE164NO, 0, sizeof(m_szE164NO));
            return;
        }
        
        strncpy(m_szE164NO, szE164NO, sizeof(m_szE164NO)-1);
        m_szE164NO[sizeof(m_szE164NO)-1] = '\0';
    }

    const s8* GetEmail() const { return m_szEmail; }
    void SetEmail(const s8* szEmail) 
    { 
        if (NULL == szEmail)
        {
            memset(m_szEmail, 0, sizeof(m_szEmail));
            return;
        }
        
        strncpy(m_szEmail, szEmail, sizeof(m_szEmail)-1);
        m_szEmail[sizeof(m_szEmail)-1] = '\0';
    }

    const s8* GetPrivilegeList() const { return m_szPrivilegeList; }
    void SetPrivilegeList(const s8* szPrivilegeList) 
    { 
        if (NULL == szPrivilegeList)
        {
            memset(m_szPrivilegeList, 0, sizeof(m_szPrivilegeList));
            return;
        }
        
        strncpy(m_szPrivilegeList, szPrivilegeList, sizeof(m_szPrivilegeList)-1);
        m_szPrivilegeList[sizeof(m_szPrivilegeList)-1] = '\0';
    }

    BOOL32 IsHasPrivilege(const s8* szPrivilege) const
    {
        if (NULL == szPrivilege || 0 == strlen(szPrivilege))
        {
            return FALSE;
        }
        
        s8 szTempPriList[MAXLEN_PRIVILEGE_LIST+1] = {0};
        strncpy(szTempPriList, m_szPrivilegeList, sizeof(szTempPriList)-1);

        //以";"分隔
        const s8 seps[] = ";";
        s8* pToken = strtok(szTempPriList, seps);
        while (NULL != pToken)
        {
            if (0 == strcmp(szPrivilege, pToken))
            {
                return TRUE;
            }
                        
            pToken = strtok( NULL, seps );
		}

        return FALSE;
    }

public:
#ifdef _JSON_CONVERT_
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;
        
        jsonTool.SetValue("m_szMoid", m_szMoid);
        jsonTool.SetValue("m_szE164NO", m_szE164NO);
        jsonTool.SetValue("m_szEmail", m_szEmail);
        jsonTool.SetValue("m_szPrivilegeList", m_szPrivilegeList);
        
        return jsonTool.GetJson();
    }
    
    BOOL32 JsonToStruct(const s8* szJson)
    {
        if (NULL == szJson)
        {
            return FALSE;
        }
        
        CJsonConvert jsonTool;        
        BOOL32 bRet = jsonTool.ParseJsonStr(szJson);
        if (!bRet)
        {
            return FALSE;
        }
                
        jsonTool.GetValue("m_szMoid", m_szMoid, sizeof(m_szMoid));
        jsonTool.GetValue("m_szE164NO", m_szE164NO, sizeof(m_szE164NO));
        jsonTool.GetValue("m_szEmail", m_szEmail, sizeof(m_szEmail));
        jsonTool.GetValue("m_szPrivilegeList", m_szPrivilegeList, sizeof(m_szPrivilegeList));
        
        return TRUE;
    }
#endif //_JSON_CONVERT_

private:
    s8 m_szMoid[OPERATION_MAXLEN_MOID + 1];
    s8 m_szE164NO[OPERATION_MAXLEN_DEVE164 + 1];
    s8 m_szEmail[OPERATION_MAXLEN_FULLMAIL + 1];
    s8 m_szPrivilegeList[MAXLEN_PRIVILEGE_LIST+1];    //权限列表,以;分割
}
PACKED;

class COld26XnuInfo
{
public:
    COld26XnuInfo()
    {
        Init();
    }

    void Init()
    {
        memset(m_szXmppUrl, 0, sizeof(m_szXmppUrl));
        memset(m_szXmppPort, 0, sizeof(m_szXmppPort));
        memset(m_szJID, 0, sizeof(m_szJID));
        memset(m_szPassWord, 0, sizeof(m_szPassWord));
    }

    const s8* GetXmppUrl() const { return m_szXmppUrl; }
    void SetXmppUrl(const s8* szUrl) 
    { 
        if (NULL == szUrl)
        {
            memset(m_szXmppUrl, 0, sizeof(m_szXmppUrl));
            return;
        }
        
        strncpy(m_szXmppUrl, szUrl, sizeof(m_szXmppUrl)-1);
        m_szXmppUrl[sizeof(m_szXmppUrl)-1] = '\0';
    }

    const s8* GetXmppPort() const { return m_szXmppPort; }
    void SetXmppPort(const s8* szPort) 
    { 
        if (NULL == szPort)
        {
            memset(m_szXmppPort, 0, sizeof(m_szXmppPort));
            return;
        }
        
        strncpy(m_szXmppPort, szPort, sizeof(m_szXmppPort)-1);
        m_szXmppPort[sizeof(m_szXmppPort)-1] = '\0';
    }
    void SetXmppPort(u16 wPort) 
    { 
        s8 str[16] = {0};
        snprintf(str, sizeof(str)-1, "%u", wPort);
        SetXmppPort(str);
    }

    const s8* GetXmppJID() const { return m_szJID; }
    void SetXmppJID(const s8* szJid) 
    { 
        if (NULL == szJid)
        {
            memset(m_szJID, 0, sizeof(m_szJID));
            return;
        }
        
        strncpy(m_szJID, szJid, sizeof(m_szJID)-1);
        m_szJID[sizeof(m_szJID)-1] = '\0';
    }

    const s8* GetPwd() const { return m_szPassWord; }
    void SetPwd(const s8* szPwd) 
    { 
        if (NULL == szPwd)
        {
            memset(m_szPassWord, 0, sizeof(m_szPassWord));
            return;
        }
        
        strncpy(m_szPassWord, szPwd, sizeof(m_szPassWord)-1);
        m_szPassWord[sizeof(m_szPassWord)-1] = '\0';
    }

public:
#ifdef _JSON_CONVERT_
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;
        
        jsonTool.SetValue("m_szXmppUrl", m_szXmppUrl);
        jsonTool.SetValue("m_szXmppPort", m_szXmppPort);
        jsonTool.SetValue("m_szJID", m_szJID);
        jsonTool.SetValue("m_szPassWord", m_szPassWord);
        
        return jsonTool.GetJson();
    }
    
    BOOL32 JsonToStruct(const s8* szJson)
    {
        if (NULL == szJson)
        {
            return FALSE;
        }
        
        CJsonConvert jsonTool;        
        BOOL32 bRet = jsonTool.ParseJsonStr(szJson);
        if (!bRet)
        {
            return FALSE;
        }
        
        jsonTool.GetValue("m_szXmppUrl", m_szXmppUrl, sizeof(m_szXmppUrl));
        jsonTool.GetValue("m_szXmppPort", m_szXmppPort, sizeof(m_szXmppPort));
        jsonTool.GetValue("m_szJID", m_szJID, sizeof(m_szJID));
        jsonTool.GetValue("m_szPassWord", m_szPassWord, sizeof(m_szPassWord));
        
        return TRUE;
    }
#endif //_JSON_CONVERT_

private:
    s8 m_szXmppUrl[MAXLEN_URL + 1];
    s8 m_szXmppPort[MAXLEN_PORT + 1];
    s8 m_szJID[MAXLEN_ULS_JID + 1];
    s8 m_szPassWord[OPERATION_MAXLEN_PWD + 1];
}
PACKED;

class COld26UlcPlatformSrvInfo
{
public:
    COld26UlcPlatformSrvInfo()
    {
        Init();
    }
    
    void Init()
    {
        memset(m_szPlatApiUrl, 0, sizeof(m_szPlatApiUrl));
        memset(m_szMooooUrl, 0, sizeof(m_szMooooUrl));
        m_tXnuInfo.Init();
        m_tSusAddrInfo.Clear();
        m_tNmsAddrInfo.Clear();
        m_tNtsAddrInfo.Clear();
        m_tCsuAddrInfo.Clear();
        m_tUlsSrvIpList.Init();
    }

    const s8* GetPlatApiUrl() const { return m_szPlatApiUrl; }
    void SetPlatApiUrl(const s8* szUrl) 
    { 
        if (NULL == szUrl)
        {
            memset(m_szPlatApiUrl, 0, sizeof(m_szPlatApiUrl));
            return;
        }
        
        strncpy(m_szPlatApiUrl, szUrl, sizeof(m_szPlatApiUrl)-1);
        m_szPlatApiUrl[sizeof(m_szPlatApiUrl)-1] = '\0';
    }

    const s8* GetMooooUrl() const { return m_szMooooUrl; }
    void SetMooooUrl(const s8* szUrl) 
    { 
        if (NULL == szUrl)
        {
            memset(m_szMooooUrl, 0, sizeof(m_szMooooUrl));
            return;
        }
        
        strncpy(m_szMooooUrl, szUrl, sizeof(m_szMooooUrl)-1);
        m_szMooooUrl[sizeof(m_szMooooUrl)-1] = '\0';
    }

    COld26XnuInfo GetXnuInfo() const { return m_tXnuInfo; }
    void SetXnuInfo(const COld26XnuInfo& tXnuInfo) { m_tXnuInfo = tXnuInfo; }

    COldSrvAddrInfo GetSusAddrInfo() const { return m_tSusAddrInfo; }
    void SetSusAddrInfo(const COldSrvAddrInfo& tAddrInfo) { m_tSusAddrInfo = tAddrInfo; }
    void SetSusAddrInfo(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetSusAddrInfo( Convert(tAddrInfo) );
    }

    COldSrvAddrInfo GetNmsAddrInfo() const { return m_tNmsAddrInfo; }
    void SetNmsAddrInfo(const COldSrvAddrInfo& tAddrInfo) { m_tNmsAddrInfo = tAddrInfo; }
    void SetNmsAddrInfo(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetNmsAddrInfo( Convert(tAddrInfo) );
    }

    COldSrvAddrInfo GetNtsAddrInfo() const { return m_tNtsAddrInfo; }
    void SetNtsAddrInfo(const COldSrvAddrInfo& tAddrInfo) { m_tNtsAddrInfo = tAddrInfo; }
    void SetNtsAddrInfo(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetNtsAddrInfo( Convert(tAddrInfo) );
    }

    COldSrvAddrInfo GetCsuAddrInfo() const { return m_tCsuAddrInfo; }
    void SetCsuAddrInfo(const COldSrvAddrInfo& tAddrInfo) { m_tCsuAddrInfo = tAddrInfo; }
    void SetCsuAddrInfo(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetCsuAddrInfo( Convert(tAddrInfo) );
    }

    COld26ApsSrvIpList GetXapList() const { return m_tUlsSrvIpList; }
    void SetXapList(const COld26ApsSrvIpList& tList) { m_tUlsSrvIpList = tList; }
    void SetXapList(const CApsSrvIpList& tAddrInfo) 
    { 
        m_tUlsSrvIpList.Init();
        for (u32 dwIdx=0; dwIdx < tAddrInfo.Size(); ++dwIdx)
        {
            CSrvAddrInfo tSrvAddr = tAddrInfo.GetValue(dwIdx);
            COldSrvAddrInfo tOldAddr = Convert(tSrvAddr);
            m_tUlsSrvIpList.Append(tOldAddr);
        }
    }

public:
#ifdef _JSON_CONVERT_
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;
        
        jsonTool.SetValue("m_szPlatApiUrl", m_szPlatApiUrl);
        jsonTool.SetValue("m_szMooooUrl", m_szMooooUrl);

        jsonTool.SetNestedValue("m_tXnuInfo", m_tXnuInfo);
        jsonTool.SetNestedValue("m_tSusAddrInfo", m_tSusAddrInfo);
        jsonTool.SetNestedValue("m_tNmsAddrInfo", m_tNmsAddrInfo);
        jsonTool.SetNestedValue("m_tNtsAddrInfo", m_tNtsAddrInfo);
        jsonTool.SetNestedValue("m_tCsuAddrInfo", m_tCsuAddrInfo);
        jsonTool.SetNestedValue("m_tUlsSrvIpList", m_tUlsSrvIpList);
        
        return jsonTool.GetJson();
    }
    
    BOOL32 JsonToStruct(const s8* szJson)
    {
        if (NULL == szJson)
        {
            return FALSE;
        }
        
        CJsonConvert jsonTool;        
        BOOL32 bRet = jsonTool.ParseJsonStr(szJson);
        if (!bRet)
        {
            return FALSE;
        }
        
        jsonTool.GetValue("m_szPlatApiUrl", m_szPlatApiUrl, sizeof(m_szPlatApiUrl));
        jsonTool.GetValue("m_szMooooUrl", m_szMooooUrl, sizeof(m_szMooooUrl));
           
        jsonTool.GetNestedValue("m_tXnuInfo", m_tXnuInfo);
        jsonTool.GetNestedValue("m_tSusAddrInfo", m_tSusAddrInfo);
        jsonTool.GetNestedValue("m_tNmsAddrInfo", m_tNmsAddrInfo);
        jsonTool.GetNestedValue("m_tNtsAddrInfo", m_tNtsAddrInfo);
        jsonTool.GetNestedValue("m_tCsuAddrInfo", m_tCsuAddrInfo);
        jsonTool.GetNestedValue("m_tUlsSrvIpList", m_tUlsSrvIpList);

        return TRUE;
    }
#endif //_JSON_CONVERT_

private:
    s8 m_szPlatApiUrl[MAXLEN_URL + 1];
    s8 m_szMooooUrl[MAXLEN_URL + 1];
    COld26XnuInfo m_tXnuInfo;
    COldSrvAddrInfo m_tNmsAddrInfo;    //NMS的地址信息
    COldSrvAddrInfo m_tNtsAddrInfo;    //NTS的地址信息
    COldSrvAddrInfo m_tCsuAddrInfo;    //Csu的地址信息
    COldSrvAddrInfo m_tSusAddrInfo;    //SUS的地址信息
    COld26ApsSrvIpList m_tUlsSrvIpList;     //其它Uls列表
}
PACKED;

class COld26UlcFullInfo
{
public:
    COld26UlcFullInfo()
    {
        Init();
    }

    void Init()
    {
        m_tMtInfo.Init();
        m_tSrvInfo.Init();
		memset(m_szPlatVer, 0, sizeof(m_szPlatVer));
    }

    COld26UlcMtInfo GetMtInfo() const { return m_tMtInfo; }
    void SetMtInfo(const COld26UlcMtInfo& tMtInfo) { m_tMtInfo = tMtInfo; }

    COld26UlcPlatformSrvInfo GetSrvInfo() const { return m_tSrvInfo; }
    void SetSrvInfo(const COld26UlcPlatformSrvInfo& tInfo) { m_tSrvInfo = tInfo; }

	const s8 *GetPlatVer() const { return m_szPlatVer; }
	void SetPlatVer( s8* szVer )
	{
		if (NULL == szVer)
		{
			memset(m_szPlatVer, 0, sizeof(m_szPlatVer));
			return;
		}

		strncpy(m_szPlatVer, szVer, sizeof(m_szPlatVer)-1);
		m_szPlatVer[sizeof(m_szPlatVer)-1] = '\0';
	}

public:
#ifdef _JSON_CONVERT_
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;
                
        jsonTool.SetNestedValue("m_tMtInfo", m_tMtInfo);
        jsonTool.SetNestedValue("m_tSrvInfo", m_tSrvInfo);
        jsonTool.SetValue( "m_tPlatVer", m_szPlatVer );
        return jsonTool.GetJson();
    }
    
    BOOL32 JsonToStruct(const s8* szJson)
    {
        if (NULL == szJson)
        {
            return FALSE;
        }
        
        CJsonConvert jsonTool;        
        BOOL32 bRet = jsonTool.ParseJsonStr(szJson);
        if (!bRet)
        {
            return FALSE;
        }

        jsonTool.GetNestedValue("m_tMtInfo", m_tMtInfo);
        jsonTool.GetNestedValue("m_tSrvInfo", m_tSrvInfo);
        jsonTool.GetValue( "m_tPlatVer", m_szPlatVer, sizeof(m_szPlatVer)  );
        return TRUE;
    }
#endif //_JSON_CONVERT_

private:
    COld26UlcMtInfo m_tMtInfo;//终端的信息
    COld26UlcPlatformSrvInfo m_tSrvInfo;//服务器地址
	s8 m_szPlatVer[OPERATION_MAXLEN_VERSION+1];          //终端类型
}
PACKED;

#ifdef WIN32
#pragma pack( pop )
#endif

#endif //end of _OLD_STRUCT_ULC_H_
