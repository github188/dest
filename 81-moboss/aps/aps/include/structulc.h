

/*=======================================================================
模块名      :ULS
文件名      :structulc.h
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
#ifndef _STRUCT_ULC_H_
#define _STRUCT_ULC_H_

#include "kdvtype.h"
#include "constuls.h"
#include "operationconst.h"
#include "kdvplist.h"
#include "bossutils.h"
#include "osp.h"
#include "srvmoduleinfo_nu.h"

#include <map>

#ifdef _JSON_CONVERT_
#include "jsonconvert.h"
#endif //_JSON_CONVERT_

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

class CSrvAddrInfo
{
public:    
    CSrvAddrInfo()
    {
        Clear();
    }
	CSrvAddrInfo(const u32 dwIp, const u16 wPort, const u16 wTlsPort, const s8* pchDomainName)
		:m_dwIpAddr(htonl(dwIp)), m_wPort(wPort), m_wTlsPort(wTlsPort)
	{
		if (pchDomainName != NULL)
		{
			strncpy(m_szNetDomainName, pchDomainName, sizeof(m_szNetDomainName) - 1);
			m_szNetDomainName[sizeof(m_szNetDomainName) - 1] = '\0';
		}
		else
		{
			memset(m_szNetDomainName, 0, sizeof(m_szNetDomainName));
		}
	}
    
    void Clear( void )
    {
        m_dwIpAddr = 0;
        m_wPort = 0;
		m_wTlsPort = 0;
        memset( m_szNetDomainName, 0, sizeof(m_szNetDomainName) );
#ifdef _APS_PXY_
		m_byIspType = (u8)emTypeQT;
		memset( m_achPlatformMoId, 0, sizeof(m_achPlatformMoId));
#endif
    }

	//操作符重载
    bool operator == ( const CSrvAddrInfo& tOther ) const
	{
		if (GetIpAddr() == tOther.GetIpAddr() && strcmp(GetNetDomainName(), tOther.GetNetDomainName()) == 0)
		{
			return true;
		}

		return false;		
	}
	
    bool operator < ( const CSrvAddrInfo& other ) const
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
    
    u16 GetPort() const { return m_wPort; }
    void SetPort(u16 wPort) { m_wPort = wPort; }

	u16 GetTlsPort() const { return m_wTlsPort; }
	void SetTlsPort(u16 wTlsPort) { m_wTlsPort = wTlsPort; }

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
#ifdef _APS_PXY_
	void SetIspType(EOperatorsType emIspType)
	{
		m_byIspType = (u8)emIspType;
	}

	u8   GetIspType() const
	{
		return m_byIspType;
	}

	void SetPlatformMoId(s8 *pszMoId)
	{
		if ( pszMoId )
		{
			u32 dwLen = strlen(pszMoId);
			if ( dwLen > sizeof(m_achPlatformMoId)-1 )
			{
				dwLen = sizeof(m_achPlatformMoId)-1;
			}
			memcpy(m_achPlatformMoId, pszMoId, dwLen);
			m_achPlatformMoId[dwLen] = 0;
		}
		return ;
	}

	s8 *GetPlatformMoId() const
	{
		return (s8*)m_achPlatformMoId;
	}
#endif
    
public:
#ifdef _JSON_CONVERT_
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;
        
        //以字符串的形式传输IP地址
        std::string strIP = BossUtils::GetIpString(GetIpAddr());    
        jsonTool.SetValue("m_szIpAddr", strIP.c_str());

        jsonTool.SetValue("m_szNetDomainName", GetNetDomainName());
        jsonTool.SetValue("m_wPort", GetPort());
        
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
        
        //以字符串的形式传输IP地址
        s8 szIP[OPERATION_MAXLEN_IPADDRESS+1] = {0};
        jsonTool.GetValue("m_szIpAddr", szIP, sizeof(szIP));
        u32 dwIp = BossUtils::GetIpByString(szIP);
        SetIpAddr(dwIp);
        
        //
        u16 wPort = 0;
        jsonTool.GetValue("m_wPort", wPort);
        SetPort(wPort);

        //域名
        s8 achDomainName[OPERATION_MAXLEN_DNS_DOMAINNAME + 1] = {0};
        jsonTool.GetValue("m_szNetDomainName", achDomainName, sizeof(achDomainName));
        SetNetDomainName(achDomainName);
        
        return TRUE;
    }
#endif //_JSON_CONVERT_

public:
	void Print() const
	{
		printf("Ip:%15s Port:%u Name:%20s ",
			BossUtils::GetIpString(ntohl(m_dwIpAddr)).c_str(), m_wPort, m_szNetDomainName);
		OspPrintf(TRUE, FALSE, "Ip:%15s Port:%u Name:%20s ",
			BossUtils::GetIpString(ntohl(m_dwIpAddr)).c_str(), m_wPort, m_szNetDomainName);
#ifdef _APS_PXY_
		printf("emIspType: %u PlatformMoId:%s\n", m_byIspType, m_achPlatformMoId);
		OspPrintf(TRUE, FALSE, "emIspType: %u PlatformMoId:%s\n", m_byIspType, m_achPlatformMoId);
#else
		printf("\n");
		OspPrintf(TRUE, FALSE, "\n");
#endif
	}
    
private:
    u32	m_dwIpAddr;				   //IP地址
    u16 m_wPort;
	u16 m_wTlsPort;
    s8  m_szNetDomainName[OPERATION_MAXLEN_DNS_DOMAINNAME + 1];	//域名地址
#ifdef _APS_PXY_
	u8  m_byIspType;              //运营商的类型，参照srvmodule_nu.h定义
	s8  m_achPlatformMoId[OPERATION_MAXLEN_MOID+1];      //平台域的MoId
#endif
}
PACKED;



typedef CKdvpList<CSrvAddrInfo, MAXNUM_ULSIPLIST>  CApsSrvIpList;

class CPlatformSrvAddrInfo
{
public:
    CPlatformSrvAddrInfo()
    {
        Init();
    }

    void Init()
    {
        m_tSusAddrInfo.Clear();
        m_tNmsAddrInfo.Clear();
        m_tNdsAddrInfo.Clear();

        m_tStdH323AddrInfo.Clear();
        m_tNonH323AddrInfo.Clear();
        m_tSipAddrInfo.Clear();
        m_tPasAddrInfo.Clear();
        
        m_tPlatApiAddrInfo.Clear(); 
        m_tWeiboAddrInfo.Clear(); 
        m_tMeetingAddrInfo.Clear();
        m_tXmppAddrInfo.Clear();
		m_tRestAddrInfo.Clear();

        m_tApsSrvIpList.Init();
		m_tMainPlatApsList.Init();
		m_tBackUpPlatApsList.Init();

    }

	void SetServerAddrInfo(const u32 dwType, CSrvAddrInfo& tAddrInfo);

    CSrvAddrInfo GetSusAddrInfo() const { return m_tSusAddrInfo; }
    void SetSusAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tSusAddrInfo = tAddrInfo; }

    CSrvAddrInfo GetNmsAddrInfo() const { return m_tNmsAddrInfo; }
    void SetNmsAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tNmsAddrInfo = tAddrInfo; }

    CSrvAddrInfo GetNdsAddrInfo() const { return m_tNdsAddrInfo; }
    void SetNdsAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tNdsAddrInfo = tAddrInfo; }

    CSrvAddrInfo GetStdH323AddrInfo() const { return m_tStdH323AddrInfo; }
    void SetStdH323AddrInfo(const CSrvAddrInfo& tAddrInfo) 
    { 
        m_tStdH323AddrInfo = tAddrInfo; 
        m_tPasAddrInfo = tAddrInfo;
    }

    CSrvAddrInfo GetNonH323AddrInfo() const { return m_tNonH323AddrInfo; }
    void SetNonH323AddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tNonH323AddrInfo = tAddrInfo; }

    CSrvAddrInfo GetSipAddrInfo() const { return m_tSipAddrInfo; }
    void SetSipAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tSipAddrInfo = tAddrInfo; }

    CSrvAddrInfo GetPlatApiAddrInfo() const { return m_tPlatApiAddrInfo; }
    void SetPlatApiAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tPlatApiAddrInfo = tAddrInfo; }

    CSrvAddrInfo GetWeiboAddrInfo() const { return m_tWeiboAddrInfo; }
    void SetWeiboAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tWeiboAddrInfo = tAddrInfo; }

    CSrvAddrInfo GetMeetingAddrInfo() const { return m_tMeetingAddrInfo; }
    void SetMeetingAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tMeetingAddrInfo = tAddrInfo; }

    CSrvAddrInfo GetXmppAddrInfo() const { return m_tXmppAddrInfo; }
    void SetXmppAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tXmppAddrInfo = tAddrInfo; }

    CSrvAddrInfo GetNtpAddrInfo() const { return m_tNtpAddrInfo; }
    void SetNtpAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tNtpAddrInfo = tAddrInfo; }

    CSrvAddrInfo GetVrsAddrInfo() const { return m_tVrsAddrInfo; }
    void SetVrsAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tVrsAddrInfo = tAddrInfo; }

    CSrvAddrInfo GetDcsAddrInfo() const { return m_tDcsAddrInfo; }
    void SetDcsAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tDcsAddrInfo = tAddrInfo; }

	CSrvAddrInfo GetRestAddrInfo() const { return m_tRestAddrInfo; }
	void SetRestAddrInfo(const CSrvAddrInfo& tAddrInfo) { m_tRestAddrInfo = tAddrInfo; }

    const CApsSrvIpList& GetApsList() const { return m_tApsSrvIpList; }
    void SetApsList(const CApsSrvIpList& tList) { m_tApsSrvIpList = tList; }

    const CApsSrvIpList& GetMainPlatApsList() const { return m_tMainPlatApsList; }
    void SetMainPlatApsList(const CApsSrvIpList& tList) { m_tMainPlatApsList = tList; }
    const CApsSrvIpList& GetBackUpApsList() const { return m_tBackUpPlatApsList; }
    void SetBackUpApsList(const CApsSrvIpList& tList) { m_tBackUpPlatApsList = tList; }


#ifdef _JSON_CONVERT_
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;

        jsonTool.SetNestedValue("m_tNmsAddrInfo", m_tNmsAddrInfo);
        jsonTool.SetNestedValue("m_tNdsAddrInfo", m_tNdsAddrInfo);
        jsonTool.SetNestedValue("m_tSusAddrInfo", m_tSusAddrInfo);

        jsonTool.SetNestedValue("m_tStdH323AddrInfo", m_tStdH323AddrInfo);
        jsonTool.SetNestedValue("m_tNonH323AddrInfo", m_tNonH323AddrInfo);
        jsonTool.SetNestedValue("m_tSipAddrInfo", m_tSipAddrInfo);
        jsonTool.SetNestedValue("m_tPasAddrInfo", m_tPasAddrInfo);

        jsonTool.SetNestedValue("m_tPlatApiAddrInfo", m_tPlatApiAddrInfo);
        jsonTool.SetNestedValue("m_tWeiboAddrInfo", m_tWeiboAddrInfo);
        jsonTool.SetNestedValue("m_tMeetingAddrInfo", m_tMeetingAddrInfo);
        jsonTool.SetNestedValue("m_tXmppAddrInfo", m_tXmppAddrInfo);

        jsonTool.SetNestedValue("m_tNtpAddrInfo", m_tNtpAddrInfo);
        jsonTool.SetNestedValue("m_tVrsAddrInfo", m_tVrsAddrInfo);
        jsonTool.SetNestedValue("m_tDcsAddrInfo", m_tDcsAddrInfo);
		jsonTool.SetNestedValue("m_tRestAddrInfo", m_tRestAddrInfo );

        jsonTool.SetNestedValue("m_tApsSrvIpList", m_tApsSrvIpList);

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

        jsonTool.GetNestedValue("m_tNmsAddrInfo", m_tNmsAddrInfo);
        jsonTool.GetNestedValue("m_tNdsAddrInfo", m_tNdsAddrInfo);
        jsonTool.GetNestedValue("m_tSusAddrInfo", m_tSusAddrInfo);

        jsonTool.GetNestedValue("m_tStdH323AddrInfo", m_tStdH323AddrInfo);
        jsonTool.GetNestedValue("m_tNonH323AddrInfo", m_tNonH323AddrInfo);
        jsonTool.GetNestedValue("m_tSipAddrInfo", m_tSipAddrInfo);
        jsonTool.GetNestedValue("m_tPasAddrInfo", m_tPasAddrInfo);

        jsonTool.GetNestedValue("m_tPlatApiAddrInfo", m_tPlatApiAddrInfo);
        jsonTool.GetNestedValue("m_tWeiboAddrInfo", m_tWeiboAddrInfo);
        jsonTool.GetNestedValue("m_tMeetingAddrInfo", m_tMeetingAddrInfo);
        jsonTool.GetNestedValue("m_tXmppAddrInfo", m_tXmppAddrInfo);

        jsonTool.GetNestedValue("m_tNtpAddrInfo", m_tNtpAddrInfo);
        jsonTool.GetNestedValue("m_tVrsAddrInfo", m_tVrsAddrInfo);
        jsonTool.GetNestedValue("m_tDcsAddrInfo", m_tDcsAddrInfo);
		jsonTool.GetNestedValue("m_tRestAddrInfo", m_tRestAddrInfo );

        jsonTool.GetNestedValue("m_tApsSrvIpList", m_tApsSrvIpList);

        return TRUE;
    }
#endif //_JSON_CONVERT_
private:

    CSrvAddrInfo m_tNmsAddrInfo;     //NMS的地址信息
    CSrvAddrInfo m_tNdsAddrInfo;     //NTS的地址信息
    CSrvAddrInfo m_tSusAddrInfo;     //SUS的地址信息

    //将PAS服务器分成3个服务器告知终端
    CSrvAddrInfo m_tStdH323AddrInfo ; //标准H323地址信息
    CSrvAddrInfo m_tNonH323AddrInfo;  //摩云非标H323地址信息
    CSrvAddrInfo m_tSipAddrInfo;      //SIP协议的地址信息
    CSrvAddrInfo m_tPasAddrInfo;      //兼容5.0的实验局版本

    CSrvAddrInfo m_tPlatApiAddrInfo; //PlatApi的地址信息
    CSrvAddrInfo m_tWeiboAddrInfo;   //weibo的地址信息
    CSrvAddrInfo m_tMeetingAddrInfo; //Meeting的地址信息
    CSrvAddrInfo m_tXmppAddrInfo;    //Xmpp的地址信息

    CSrvAddrInfo m_tNtpAddrInfo;    //时间同步服务器
    CSrvAddrInfo m_tVrsAddrInfo;    //视频录播服务器
    CSrvAddrInfo m_tDcsAddrInfo;    //数据会议服务器
	CSrvAddrInfo m_tRestAddrInfo;   //RestApi服务器

    CApsSrvIpList m_tApsSrvIpList;   //其它APS(Xap)列表
	CApsSrvIpList m_tMainPlatApsList; //主平台aps列表
	CApsSrvIpList m_tBackUpPlatApsList; //灾备平台aps列表
};


class CMtInfo
{
public:
    CMtInfo()
    {
        Init();
    }

    void Init()
    {
        memset(m_szMoid, 0, sizeof(m_szMoid));
        memset(m_szAccount, 0, sizeof(m_szAccount));
        memset(m_szE164NO, 0, sizeof(m_szE164NO));
        memset(m_szEmail, 0, sizeof(m_szEmail));
        memset(m_szPrivilegeList, 0, sizeof(m_szPrivilegeList));
        memset(m_szJID, 0, sizeof(m_szJID));
        memset(m_szXmppPwd, 0, sizeof(m_szXmppPwd));
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

    const s8* GetAccount() const { return m_szAccount; }
    void SetAccount(const s8* szAccount) 
    {
        if (NULL == szAccount)
        {
            memset(m_szAccount, 0, sizeof(m_szAccount));
            return;
        }

        strncpy(m_szAccount, szAccount, sizeof(m_szAccount)-1);
        m_szAccount[sizeof(m_szAccount)-1] = '\0';
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

    const s8* GetXmppPwd() const { return m_szXmppPwd; }
    void SetXmppPwd(const s8* szPwd) 
    { 
        if (NULL == szPwd)
        {
            memset(m_szXmppPwd, 0, sizeof(m_szXmppPwd));
            return;
        }

        strncpy(m_szXmppPwd, szPwd, sizeof(m_szXmppPwd)-1);
        m_szXmppPwd[sizeof(m_szXmppPwd)-1] = '\0';
    }

public:
#ifdef _JSON_CONVERT_
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;

        jsonTool.SetValue("m_szMoid", m_szMoid);
        jsonTool.SetValue("m_szAccount", m_szAccount);
        jsonTool.SetValue("m_szE164NO", m_szE164NO);
        jsonTool.SetValue("m_szEmail", m_szEmail);
        jsonTool.SetValue("m_szPrivilegeList", m_szPrivilegeList);
        jsonTool.SetValue("m_szJID", m_szJID);
        jsonTool.SetValue("m_szXmppPwd", m_szXmppPwd);

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
        jsonTool.GetValue("m_szAccount", m_szAccount, sizeof(m_szAccount));
        jsonTool.GetValue("m_szE164NO", m_szE164NO, sizeof(m_szE164NO));
        jsonTool.GetValue("m_szEmail", m_szEmail, sizeof(m_szEmail));
        jsonTool.GetValue("m_szPrivilegeList", m_szPrivilegeList, sizeof(m_szPrivilegeList));
        jsonTool.GetValue("m_szJID", m_szJID, sizeof(m_szJID));
        jsonTool.GetValue("m_szXmppPwd", m_szXmppPwd, sizeof(m_szXmppPwd));

        return TRUE;
    }
#endif //_JSON_CONVERT_

private:
    s8 m_szMoid[OPERATION_MAXLEN_MOID + 1];
    s8 m_szAccount[OPERATION_MAXLEN_ACCOUNT + 1];   //自定义账号信息
    s8 m_szE164NO[OPERATION_MAXLEN_DEVE164 + 1];
    s8 m_szEmail[OPERATION_MAXLEN_FULLMAIL + 1];
    s8 m_szPrivilegeList[MAXLEN_PRIVILEGE_LIST+1];    //权限列表,以;分割
    s8 m_szJID[MAXLEN_ULS_JID + 1];
    s8 m_szXmppPwd[OPERATION_MAXLEN_PWD + 1];
};

//平台属性
class CPlatformAttr
{
public:
    CPlatformAttr()
    {
        Init();
    }

    void Init()
    {
        m_bSupportPortMedia = false;
        m_bSelfBuilt = false;
    }

    bool IsPortMedia() const { return m_bSupportPortMedia; }
    void SetPortMedia(bool bSupport) { m_bSupportPortMedia = bSupport; }

    bool GetSelfBuilt() const { return m_bSelfBuilt; }
    void SetSelfBuilt(bool bSelfBuilt) { m_bSelfBuilt = bSelfBuilt; }

public:
#ifdef _JSON_CONVERT_
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;

        jsonTool.SetValue("m_bSupportPortMedia", m_bSupportPortMedia);
        jsonTool.SetValue("m_bSelfBuilt", m_bSelfBuilt);

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

        jsonTool.GetValue("m_bSupportPortMedia", m_bSupportPortMedia);
        jsonTool.GetValue("m_bSelfBuilt", m_bSelfBuilt);

        return TRUE;
    }
#endif //_JSON_CONVERT_

private:
    bool m_bSupportPortMedia;  //是否支持端口媒体
    bool m_bSelfBuilt;  //是否自建(区别于租赁)
};

class CUlcFullInfo
{
public:
    CUlcFullInfo()
    {
        Init();
    }

    void Init()
    {
        m_tMtInfo.Init();
        m_tSrvInfo.Init();
    }

    CMtInfo GetMtInfo() const { return m_tMtInfo; }
    void SetMtInfo(const CMtInfo& tMtInfo) { m_tMtInfo = tMtInfo; }

    CPlatformSrvAddrInfo GetSrvInfo() const { return m_tSrvInfo; }
    void SetSrvInfo(const CPlatformSrvAddrInfo& tInfo) { m_tSrvInfo = tInfo; }

    CPlatformAttr GetPlatformAttr() const { return m_tPlatformAttr; }
    void SetPlatformAttr(const CPlatformAttr& tAttr) { m_tPlatformAttr = tAttr; }

public:
#ifdef _JSON_CONVERT_
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;

        jsonTool.SetNestedValue("m_tMtInfo", m_tMtInfo);
        jsonTool.SetNestedValue("m_tSrvInfo", m_tSrvInfo);
        jsonTool.SetNestedValue("m_tPlatformAttr", m_tPlatformAttr);

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
        jsonTool.GetNestedValue("m_tPlatformAttr", m_tPlatformAttr);

        return TRUE;
    }
#endif //_JSON_CONVERT_

private:
    CMtInfo m_tMtInfo;//终端的信息
    CPlatformSrvAddrInfo m_tSrvInfo;//服务器地址
    CPlatformAttr m_tPlatformAttr;  //平台属性
};


#ifdef WIN32
#pragma pack( pop )
#endif

#endif //end of _STRUCT_ULC_H_
