/*=======================================================================
模块名      :ULS
文件名      :structself.h
相关文件    :
文件实现功能:自己内部使用的一些结构体

作者        : 
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容

=======================================================================*/


#ifndef _STRUCT_SELF_H_
#define _STRUCT_SELF_H_


#include "kdvtype.h"
#include "kdvplist.h"
#include "old_26_structulc.h"
#include "operationconst.h"
#include "serverdevice.h"
#include "oemmark.h"
#include "bossutils.h"
#include "osp.h"




class CNetAddrInfo
{	
public:	
    CNetAddrInfo()
    {
        Clear();
    }  

    void Clear()
    {
        SetNetAddr(0, NULL);
    }

    bool operator == (CNetAddrInfo& tOther)
    {
        if ( GetIP() == tOther.GetIP() && GetOperatorName() == tOther.GetOperatorName() )
        {
            return TRUE;
        }

        return FALSE;
    }

public:	
    BOOL32 IsValid() const
    {
        return ( 0 == m_dwIp ) ? FALSE : TRUE ;
    }

    void SetNetAddr(u32 dwIP, const s8 * pchOperatorName )
    {
        SetIP(dwIP);
        SetOperatorName(pchOperatorName);
    }

    void SetIP(u32 dwIP) { m_dwIp = htonl(dwIP); }
    u32 GetIP() const { return ntohl(m_dwIp); }

    const s8 * GetOperatorName(void) const { return m_szOperatorName; }
    void SetOperatorName( const s8 * pchOperatorName ) 
    {
        memset( m_szOperatorName, 0, sizeof(m_szOperatorName) );
        if ( NULL != pchOperatorName )
        {            
            strncpy( m_szOperatorName, pchOperatorName, sizeof(m_szOperatorName)-1 );
            m_szOperatorName[sizeof(m_szOperatorName)-1] = '\0';
        }
        return;
    }

private:
    u32 m_dwIp;
    s8 m_szOperatorName[OPERATION_MAXLEN_DNS_DOMAINNAME + 1];    //运营商名称	
};


typedef CKdvpList<COemMark, OPERATION_MAXNUM_OEMTYPE>			COemMarkList;

//服务器简要信息
class CBriefSrvInfo
{
public:
    CBriefSrvInfo()
    {
        Init();
    }

    void Init()
    {
        memset( m_atSrvNetAddr, 0, sizeof(m_atSrvNetAddr) );
        memset( m_szNetDomainName, 0, sizeof(m_szNetDomainName) );
        memset( m_szMOID, 0, sizeof(m_szMOID) );
        memset( m_szPlatformDomainMOID, 0, sizeof(m_szPlatformDomainMOID) );
    }

    bool operator < ( const CBriefSrvInfo & tOther ) const
    {
        if ( strncmp( m_szMOID, tOther.GetMOID(), sizeof(m_szMOID)-1 ) < 0 )
        {
            return true;
        }

        return false;
    }

    const s8* GetNetDomainName( void ) const { return m_szNetDomainName; }
    void SetNetDomainName( const s8 * szNetDomainName )
    {
        memset( m_szNetDomainName, 0, sizeof(m_szNetDomainName) );
        if (NULL != szNetDomainName)
        {
            strncpy( m_szNetDomainName, szNetDomainName, sizeof(m_szNetDomainName)-1 );
            m_szNetDomainName[sizeof(m_szNetDomainName)-1] = '\0';
        }
        return;
    }

    const s8* GetMOID( void ) const { return m_szMOID; }
    void SetMOID( const s8 * szMOID )
    {
        memset( m_szMOID, 0, sizeof(m_szMOID) );
        if (NULL != szMOID)
        {
            //此处不能用strncpy,GUID中可能包含连续的0 可能导致被截断
            memcpy( m_szMOID, szMOID, sizeof(m_szMOID)-1 );
            m_szMOID[sizeof(m_szMOID)-1] = '\0';
        }
        return;
    }

    //参数nIndex从0开始
    BOOL32 GetSrvAddr(u8 nIndex, CNetAddrInfo& tSrvAddr)const
    {
        if (nIndex >= OPERATION_MAXNUM_ISP)
        {
            return FALSE;
        }

        tSrvAddr = m_atSrvNetAddr[nIndex];
        return TRUE;
    }

	//判断Ip是否已经存在
	BOOL32 IsIpExist(u32 dwHostIp) const
	{
		CNetAddrInfo cAddr;
		for (u8 byLoop = 0; byLoop < OPERATION_MAXNUM_ISP; byLoop++)
		{
			GetSrvAddr(byLoop, cAddr);
			if ( dwHostIp == cAddr.GetIP() )
			{
				return TRUE;
			}
		}
		return FALSE;
	}

    //参数nIndex从0开始
    BOOL32 SetSrvIpAddr(u8 nIndex, const CNetAddrInfo& tSrvAddr)
    {
        if (nIndex >= OPERATION_MAXNUM_ISP)
        {
            return FALSE;
        }

        m_atSrvNetAddr[nIndex] = tSrvAddr;

        return TRUE;
    }

    //要求参数achIp格式为：名字:ip;名字:IP;
    void SetSrvIpAddr(const s8* szIp)
    {
        for (u8 i = 0; i < OPERATION_MAXNUM_ISP; i++)
        {
            m_atSrvNetAddr[i].Clear();
        }

        if (NULL == szIp)
        {
            return;
        }

        //存储格式是：名字:ip;名字:IP;
        s8  achIpAddress[1024] = {0};    
        memcpy(achIpAddress, szIp, sizeof(achIpAddress)-1);
        achIpAddress[sizeof(achIpAddress)-1] = '\0';

        s8 seps[]   = ";";		
        s8* pToken = strtok( achIpAddress, seps );

        s8 pchOperatorName[OPERATION_MAXLEN_DNS_DOMAINNAME+1] = {0};
        for(u8 nIndex = 0; pToken != NULL && nIndex < OPERATION_MAXNUM_ISP; ++nIndex)
        {
            memset(pchOperatorName, 0, sizeof(pchOperatorName));

            s8* pchTem = strchr(pToken, ':');//取":"后的为IP
            if (NULL != pchTem)
            {
                *pchTem = '\0';		//将':'替换成'\0'，方便进行strncpy
                strncpy(pchOperatorName, pToken, sizeof(pchOperatorName)-1);
                pchOperatorName[sizeof(pchOperatorName)-1] = '\0';

                m_atSrvNetAddr[nIndex].SetNetAddr(ntohl(inet_addr(pchTem+1)), pchOperatorName);
            }			

            pToken = strtok( NULL, seps );
        }		
    }

    const s8 * GetPlatformDomainMOID(void) const { return (s8*)m_szPlatformDomainMOID; }
    void SetPlatformDomainMOID( const s8 * szPlatformDomainMOID ) 
    {
        memset( m_szPlatformDomainMOID, 0, sizeof(m_szPlatformDomainMOID) );
        if ( NULL != szPlatformDomainMOID )
        {
            strncpy( m_szPlatformDomainMOID, szPlatformDomainMOID, sizeof(m_szPlatformDomainMOID)-1 );
            m_szPlatformDomainMOID[sizeof(m_szPlatformDomainMOID)-1] = '\0';
        }
        return;
    }

    BOOL32 HasValidIp( void )
    {
        for( u8 byIdx = 0; byIdx < OPERATION_MAXNUM_ISP; byIdx++ )
        {
            if( m_atSrvNetAddr[byIdx].IsValid() )
            {
                return TRUE;
            }
        }

        return FALSE;
    }

    BOOL32 IsNetDomainNameValid( void )
    {
        return ( 0 == strncmp( m_szNetDomainName, "", sizeof(m_szNetDomainName)) ) ? FALSE : TRUE;
    }

    COemMarkList GetOemMarkList() const { return m_listOemMark; }
    void SetOemMarkList(const s8* szOemMark) 
    {
        m_listOemMark.Init();
        if (NULL == szOemMark)
        {
            return;
        }

        s8 szTempBuf[(OPERATION_MAXLEN_OEMMARK+1) * OPERATION_MAXNUM_OEMTYPE + 1] = {0};
        strncpy(szTempBuf, szOemMark, sizeof(szTempBuf)-1);

        //以;号分隔
        s8 seps[] = ";";		
        s8* pToken = strtok( szTempBuf, seps );        
        while (NULL != pToken)
        {
            COemMark tMark(pToken);
            m_listOemMark.Append(tMark);

            pToken = strtok( NULL, seps );
        }

        return;
    }

    void Print( void ) const
    {
        for( u8 byIdx = 0; byIdx < OPERATION_MAXNUM_ISP; byIdx++ )
        {
            if( !m_atSrvNetAddr[byIdx].IsValid() )
            {
                continue;
            }

			OspPrintf(TRUE, FALSE, "NetParam%d: Name<%s> IP<%s>\n",
				byIdx + 1, m_atSrvNetAddr[byIdx].GetOperatorName(), BossUtils::GetIpString(m_atSrvNetAddr[byIdx].GetIP()).c_str());
        }

        if( strlen(m_szNetDomainName) > 0 )
        {
            OspPrintf( TRUE, FALSE, "Server NetDomain Name is %s\n", m_szNetDomainName );
        }
        else
        {
            OspPrintf( TRUE, FALSE, "Server NetDomain Name is NULL\n" );
        }

        if( strlen(m_szMOID) > 0 )
        {
            OspPrintf( TRUE, FALSE, "Server MOID is %s\n", m_szMOID );
        }
        else
        {
            OspPrintf( TRUE, FALSE, "Server MOID is NULL\n" );
        }

        OspPrintf( TRUE, FALSE, "\n" );
    }

protected:
    s8			 m_szMOID[OPERATION_MAXLEN_MOID+1]; //MOID 全局唯一ID
    s8			 m_szNetDomainName[OPERATION_MAXLEN_DNS_DOMAINNAME+1]; //网络域名
    s8           m_szPlatformDomainMOID[OPERATION_MAXLEN_MOID+1];     //所属平台域MOID
    CNetAddrInfo m_atSrvNetAddr[OPERATION_MAXNUM_ISP];	//服务器地址
    COemMarkList m_listOemMark;     //OemMark (sus服务器独有)
};

static CBriefSrvInfo Convert(IN const CServerDevice& tServerInfo)
{
    CBriefSrvInfo tBriefInfo;
    tBriefInfo.SetSrvIpAddr( tServerInfo.GetIpAddress() );
    tBriefInfo.SetNetDomainName( tServerInfo.GetNetDomainName() );
    tBriefInfo.SetMOID( tServerInfo.GetMOID() );
    tBriefInfo.SetPlatformDomainMOID( tServerInfo.GetPlatformDomainMOID() );
    tBriefInfo.SetOemMarkList( tServerInfo.GetOemMark() );

    return tBriefInfo;
}

class CStateBase
{
public:
    CStateBase()
    {
        InitState();
    }

    void InitState()
    {
        for (u32 dwIndex = 0; dwIndex < OPERATION_MAXNUM_ISP; ++dwIndex)
        {
            m_atSrvOspNode[dwIndex] = INVALID_NODE;
        }
    }

    BOOL32 GetIpState(u32 dwIndex) const
    {
        if (dwIndex >=  OPERATION_MAXNUM_ISP)
        {
            return FALSE;
        }
    
        u32 dwNode = m_atSrvOspNode[dwIndex];
        return OspIsValidTcpNode(dwNode);
    }

    u32 GetIpOspNode(u32 dwIndex) const
    {
        if (dwIndex >=  OPERATION_MAXNUM_ISP)
        {
            return INVALID_NODE;
        }
        
        return m_atSrvOspNode[dwIndex];
    }

    void SetIpOspNode(u32 dwIndex, u32 dwOspNOde)
    {
        if (dwIndex >=  OPERATION_MAXNUM_ISP)
        {
            return;
        }
        
        m_atSrvOspNode[dwIndex] = dwOspNOde;
    }

private:
    u32	m_atSrvOspNode[OPERATION_MAXNUM_ISP];	//服务器状态
    
};

//内部传输用到的扩展消息类模板
template<class T>
class CStateEx : public CStateBase
{
public:
    CStateEx(void) : CStateBase()
    {
        
    }
    
    ~CStateEx(void)
    {
        
    }

    bool operator < (const CStateEx& tOther) const
    {
        if (m_cDetailBody < tOther.m_cDetailBody)
        {
            return true;
        }
        
        return false;
    }

public:
    T GetDetailBody() const { return m_cDetailBody; }
    void SetDetailBody(const T &tBody)
    {
        m_cDetailBody = tBody;
    }
    
protected:
    T m_cDetailBody;
};


typedef CStateEx<CBriefSrvInfo>    CBriefSrvStateEx;


class CDBMtInfo
{
public:
    CDBMtInfo()
    {
        Init();
    }

    void Init()
    {
        memset(m_szMoid, 0, sizeof(m_szMoid));
        memset(m_szE164NO, 0, sizeof(m_szE164NO));
        memset(m_szEmail, 0, sizeof(m_szEmail));
        memset(m_szJID, 0, sizeof(m_szJID));    
        memset(m_szPrivilegeList, 0, sizeof(m_szPrivilegeList));
        memset(m_szUserDomainMoid, 0, sizeof(m_szUserDomainMoid));
        memset(m_szServiceDomainMoid, 0, sizeof(m_szServiceDomainMoid));
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
    void SetAccount(const s8* szName) 
    { 
        if (NULL == szName)
        {
            memset(m_szAccount, 0, sizeof(m_szAccount));
            return;
        }

        strncpy(m_szAccount, szName, sizeof(m_szAccount)-1);
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
    void AddPrivilege(const s8* szPrivilege)
    {
        if (NULL == szPrivilege || strlen(szPrivilege) == 0)
        {
            return;
        }

        u32 dwLen = sizeof(m_szPrivilegeList) - strlen(m_szPrivilegeList) - 1;
        strncat(m_szPrivilegeList, szPrivilege, dwLen);

        dwLen = sizeof(m_szPrivilegeList) - strlen(m_szPrivilegeList) - 1;
        strncat(m_szPrivilegeList, ";", dwLen);//权限列表,以;分割
    }

    const s8* GetUserDomainMoid() const { return m_szUserDomainMoid; }
    void SetUserDomainMoid(const s8* szMoid) 
    { 
        if (NULL == szMoid)
        {
            memset(m_szUserDomainMoid, 0, sizeof(m_szUserDomainMoid));
            return;
        }
        
        strncpy(m_szUserDomainMoid, szMoid, sizeof(m_szUserDomainMoid)-1);
        m_szUserDomainMoid[sizeof(m_szUserDomainMoid)-1] = '\0';
    }

    const s8* GetPlatformDomainMoid() const { return m_szPlatformDomainMoid; }
    void SetPlatformDomainMoid(const s8* szMoid) 
    { 
        if (NULL == szMoid)
        {
            memset(m_szPlatformDomainMoid, 0, sizeof(m_szPlatformDomainMoid));
            return;
        }

        strncpy(m_szPlatformDomainMoid, szMoid, sizeof(m_szPlatformDomainMoid)-1);
        m_szPlatformDomainMoid[sizeof(m_szPlatformDomainMoid)-1] = '\0';
    }

    const s8* GetPASMoid() const { return m_szPASMoid; }
    void SetPASMoid(const s8* szMoid) 
    {
        if (NULL == szMoid)
        {
            memset(m_szPASMoid, 0, sizeof(m_szPASMoid));
            return;
        }

        strncpy(m_szPASMoid, szMoid, sizeof(m_szPASMoid)-1);
        m_szPASMoid[sizeof(m_szPASMoid)-1] = '\0';
    }

    const s8* GetServiceDomainMoid() const { return m_szServiceDomainMoid; }
    void SetServiceDomainMoid(const s8* szMoid) 
    { 
        if (NULL == szMoid)
        {
            memset(m_szServiceDomainMoid, 0, sizeof(m_szServiceDomainMoid));
            return;
        }

        strncpy(m_szServiceDomainMoid, szMoid, sizeof(m_szServiceDomainMoid)-1);
        m_szServiceDomainMoid[sizeof(m_szServiceDomainMoid)-1] = '\0';
    }

private:
    s8 m_szMoid[OPERATION_MAXLEN_MOID + 1];
    s8 m_szAccount[OPERATION_MAXLEN_ACCOUNT+1];
    s8 m_szE164NO[OPERATION_MAXLEN_DEVE164 + 1];
    s8 m_szEmail[OPERATION_MAXLEN_FULLMAIL + 1];
    s8 m_szJID[MAXLEN_ULS_JID + 1];
    s8 m_szPrivilegeList[MAXLEN_PRIVILEGE_LIST+1];    //权限列表,以;分割
    s8 m_szUserDomainMoid[OPERATION_MAXLEN_MOID + 1];      //用户域
    s8 m_szPlatformDomainMoid[OPERATION_MAXLEN_MOID + 1];  //优先选择的平台域
    s8 m_szPASMoid[OPERATION_MAXLEN_MOID + 1];  //优先选择的PAS服务器
    s8 m_szServiceDomainMoid[OPERATION_MAXLEN_MOID + 1];   //服务域
};



#endif // _STRUCT_SELF_H_
// END FILE



