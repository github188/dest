

/*=======================================================================
模块名      :
文件名      :structpxy.h
相关文件    :
文件实现功能:Pxy相关的结构体定义

作者        :
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/07/26  1.0                       创建
=======================================================================*/
#ifndef _STRUCT_PXY_H_
#define _STRUCT_PXY_H_

#include "operationconst.h"
#include "jsonconvert.h"
#include "structulc.h"


class CPxyIspSrvAddr
{
public:
    CPxyIspSrvAddr()
    {
        Init();
    }

    ~CPxyIspSrvAddr()
    {

    }

    void Init()
    {
        m_setNmsAddrInfo.clear();
        m_setNdsAddrInfo.clear();
        m_setSusAddrInfo.clear();

        m_setStdH323AddrInfo.clear();
        m_setNonH323AddrInfo.clear();
        m_setSipAddrInfo.clear();

        m_setSnsAddrInfo.clear();
        m_setCmcAddrInfo.clear();
        m_setXnsAddrInfo.clear();

        m_setNtpAddrInfo.clear();
        m_setVrsAddrInfo.clear();
        m_setDcsAddrInfo.clear();
    }

public:
    //
    std::set<CSrvAddrInfo> GetNmsAddrInfo() const { return m_setNmsAddrInfo; }
    void SetNmsAddrInfo(const std::set<CSrvAddrInfo>& setSrvAddr) { m_setNmsAddrInfo = setSrvAddr; }

    std::set<CSrvAddrInfo> GetNdsAddrInfo() const { return m_setNdsAddrInfo; }
    void SetNdsAddrInfo(const std::set<CSrvAddrInfo>& setSrvAddr) { m_setNdsAddrInfo = setSrvAddr; }

    std::set<CSrvAddrInfo> GetSusAddrInfo() const { return m_setSusAddrInfo; }
    void SetSusAddrInfo(const std::set<CSrvAddrInfo>& setSrvAddr) { m_setSusAddrInfo = setSrvAddr; }


    //
    std::set<CSrvAddrInfo> GetStdH323AddrInfo() const { return m_setStdH323AddrInfo; }
    void SetStdH323AddrInfo(const std::set<CSrvAddrInfo>& setSrvAddr) { m_setStdH323AddrInfo = setSrvAddr; }

    std::set<CSrvAddrInfo> GetNonH323AddrInfo() const { return m_setNonH323AddrInfo; }
    void SetNonH323AddrInfo(const std::set<CSrvAddrInfo>& setSrvAddr) { m_setNonH323AddrInfo = setSrvAddr; }

    std::set<CSrvAddrInfo> GetSipAddrInfo() const { return m_setSipAddrInfo; }
    void SetSipAddrInfo(const std::set<CSrvAddrInfo>& setSrvAddr) { m_setSipAddrInfo = setSrvAddr; }

    //
    std::set<CSrvAddrInfo> GetSnsAddrInfo() const { return m_setSnsAddrInfo; }
    void SetSnsAddrInfo(const std::set<CSrvAddrInfo>& val) { m_setSnsAddrInfo = val; }

    std::set<CSrvAddrInfo> GetCmcAddrInfo() const { return m_setCmcAddrInfo; }
    void SetCmcAddrInfo(const std::set<CSrvAddrInfo>& val) { m_setCmcAddrInfo = val; }

    std::set<CSrvAddrInfo> GetXnsAddrInfo() const { return m_setXnsAddrInfo; }
    void SetXnsAddrInfo(const std::set<CSrvAddrInfo>& val) { m_setXnsAddrInfo = val; }

    //
    std::set<CSrvAddrInfo> GetNtpAddrInfo() const { return m_setNtpAddrInfo; }
    void SetNtpAddrInfo(const std::set<CSrvAddrInfo>& val) { m_setNtpAddrInfo = val; }

    std::set<CSrvAddrInfo> GetVrsAddrInfo() const { return m_setVrsAddrInfo; }
    void SetVrsAddrInfo(const std::set<CSrvAddrInfo>& val) { m_setVrsAddrInfo = val; }

    std::set<CSrvAddrInfo> GetDcsAddrInfo() const { return m_setDcsAddrInfo; }
    void SetDcsAddrInfo(const std::set<CSrvAddrInfo>& val) { m_setDcsAddrInfo = val; }

public:
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;

        jsonTool.SetNestedSetValue("m_setNmsAddrInfo", m_setNmsAddrInfo);
        jsonTool.SetNestedSetValue("m_setNdsAddrInfo", m_setNdsAddrInfo);
        jsonTool.SetNestedSetValue("m_setSusAddrInfo", m_setSusAddrInfo);

        jsonTool.SetNestedSetValue("m_setStdH323AddrInfo", m_setStdH323AddrInfo);
        jsonTool.SetNestedSetValue("m_setNonH323AddrInfo", m_setNonH323AddrInfo);
        jsonTool.SetNestedSetValue("m_setSipAddrInfo", m_setSipAddrInfo);

        jsonTool.SetNestedSetValue("m_setSnsAddrInfo", m_setSnsAddrInfo);
        jsonTool.SetNestedSetValue("m_setCmcAddrInfo", m_setCmcAddrInfo);
        jsonTool.SetNestedSetValue("m_setXnsAddrInfo", m_setXnsAddrInfo);
        jsonTool.SetNestedSetValue("m_setNtpAddrInfo", m_setNtpAddrInfo);
        jsonTool.SetNestedSetValue("m_setVrsAddrInfo", m_setVrsAddrInfo);
        jsonTool.SetNestedSetValue("m_setDcsAddrInfo", m_setDcsAddrInfo);

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
       
        jsonTool.GetNestedSetValue("m_setNmsAddrInfo", m_setNmsAddrInfo);
        jsonTool.GetNestedSetValue("m_setNdsAddrInfo", m_setNdsAddrInfo);
        jsonTool.GetNestedSetValue("m_setSusAddrInfo", m_setSusAddrInfo);

        jsonTool.GetNestedSetValue("m_setStdH323AddrInfo", m_setStdH323AddrInfo);
        jsonTool.GetNestedSetValue("m_setNonH323AddrInfo", m_setNonH323AddrInfo);
        jsonTool.GetNestedSetValue("m_setSipAddrInfo", m_setSipAddrInfo);

        jsonTool.GetNestedSetValue("m_setSnsAddrInfo", m_setSnsAddrInfo);
        jsonTool.GetNestedSetValue("m_setCmcAddrInfo", m_setCmcAddrInfo);
        jsonTool.GetNestedSetValue("m_setXnsAddrInfo", m_setXnsAddrInfo);
        jsonTool.GetNestedSetValue("m_setNtpAddrInfo", m_setNtpAddrInfo);
        jsonTool.GetNestedSetValue("m_setVrsAddrInfo", m_setVrsAddrInfo);
        jsonTool.GetNestedSetValue("m_setDcsAddrInfo", m_setDcsAddrInfo);

        return TRUE;
    }

public:
	void Print() const
	{
		std::set<CSrvAddrInfo>::iterator iter = m_setNmsAddrInfo.begin();
		while (iter != m_setNmsAddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}

		iter = m_setNdsAddrInfo.begin();
		while (iter != m_setNdsAddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}

		iter = m_setSusAddrInfo.begin();
		while (iter != m_setSusAddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}
		
		iter = m_setStdH323AddrInfo.begin();
		while (iter != m_setStdH323AddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}
		
		iter = m_setNonH323AddrInfo.begin();
		while (iter != m_setNonH323AddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}
		
		iter = m_setSipAddrInfo.begin();
		while (iter != m_setSipAddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}
		
		iter = m_setSnsAddrInfo.begin();
		while (iter != m_setSnsAddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}
		
		iter = m_setCmcAddrInfo.begin();
		while (iter != m_setCmcAddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}
		
		iter = m_setXnsAddrInfo.begin();
		while (iter != m_setXnsAddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}
		
		iter = m_setNtpAddrInfo.begin();
		while (iter != m_setNtpAddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}
		
		iter = m_setVrsAddrInfo.begin();
		while (iter != m_setVrsAddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}
		
		iter = m_setDcsAddrInfo.begin();
		while (iter != m_setDcsAddrInfo.end() )
		{
			(*iter).Print();
			iter++;
		}
		
	}

private:
    std::set<CSrvAddrInfo> m_setNmsAddrInfo;     //NMS的地址信息
    std::set<CSrvAddrInfo> m_setNdsAddrInfo;     //NTS的地址信息
    std::set<CSrvAddrInfo> m_setSusAddrInfo;     //SUS的地址信息

    //将PAS服务器分成3个服务器告知终端
    std::set<CSrvAddrInfo> m_setStdH323AddrInfo ; //标准H323地址信息
    std::set<CSrvAddrInfo> m_setNonH323AddrInfo;  //摩云非标H323地址信息
    std::set<CSrvAddrInfo> m_setSipAddrInfo;      //SIP协议的地址信息

    std::set<CSrvAddrInfo> m_setSnsAddrInfo;     //SNS(weibo)的地址信息
    std::set<CSrvAddrInfo> m_setCmcAddrInfo;     //CMC(Meeting)的地址信息
    std::set<CSrvAddrInfo> m_setXnsAddrInfo;     //Xmpp的地址信息
    std::set<CSrvAddrInfo> m_setNtpAddrInfo;     //时间同步服务器
    std::set<CSrvAddrInfo> m_setVrsAddrInfo;     //视频录播服务器
    std::set<CSrvAddrInfo> m_setDcsAddrInfo;     //数据会议服务器
};

class CPxyIsp
{
public:
    CPxyIsp()
    {
        Init();
    }

    ~CPxyIsp()
    {

    }

    void Init()
    {
        memset(m_szIspName, 0, sizeof(m_szIspName));
        m_tPxyIspSrvAddr.Init();
    }

    bool operator < (const CPxyIsp& tOther) const
    {
        if ( strcmp(this->GetIspName(), tOther.GetIspName()) < 0 )        //区分大小写
        {
            return true;
        }

        return false;
    }

public:
    //
    const s8* GetIspName() const { return m_szIspName; }
    void SetIspName(const s8* szName) 
    {
        if (NULL == szName)
        {
            memset(m_szIspName, 0, sizeof(m_szIspName));
            return;
        }

        strncpy(m_szIspName, szName, sizeof(m_szIspName)-1);
    }

    CPxyIspSrvAddr GetPxyIspSrvAddr() const { return m_tPxyIspSrvAddr; }
    void SetPxyIspSrvAddr(const CPxyIspSrvAddr& tAddrInfo) { m_tPxyIspSrvAddr = tAddrInfo; }

public:
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;

        jsonTool.SetValue("m_szIspName", m_szIspName);
        jsonTool.SetNestedValue("m_tPxyIspSrvAddr", m_tPxyIspSrvAddr);

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

        jsonTool.GetValue("m_szIspName", m_szIspName, sizeof(m_szIspName));
        jsonTool.GetNestedValue("m_tPxyIspSrvAddr", m_tPxyIspSrvAddr);

        return TRUE;
    }

public:
	void Print() const
	{
		printf("IspName:%s\n", m_szIspName);
		OspPrintf(TRUE, FALSE, "IspName:%s\n", m_szIspName);
		m_tPxyIspSrvAddr.Print();
	}
private:
    s8 m_szIspName[OPERATION_MAXLEN_DNS_DOMAINNAME+1];     //
    CPxyIspSrvAddr m_tPxyIspSrvAddr;
};

class CPxyPlatformAddr
{
public:
    CPxyPlatformAddr()
    {
        Init();
    }

    ~CPxyPlatformAddr()
    {

    }

    void Init()
    {
        memset(m_szPlatformDomainMOID, 0, sizeof(m_szPlatformDomainMOID));
        m_setPxyPlatformSrvAddr.clear();
    }

    bool operator < (const CPxyPlatformAddr& tOther) const
    {
        if ( strcmp(this->GetPlatformDomainMOID(), tOther.GetPlatformDomainMOID()) < 0 )        //区分大小写
        {
            return true;
        }

        return false;
    }

public:
    //
    const s8* GetPlatformDomainMOID() const { return m_szPlatformDomainMOID; }
    void SetPlatformDomainMOID(const s8* szMOID) 
    {
        if (NULL == szMOID)
        {
            memset(m_szPlatformDomainMOID, 0, sizeof(0));
            return;
        }

        strncpy(m_szPlatformDomainMOID, szMOID, sizeof(m_szPlatformDomainMOID)-1);
    }

    //
    std::set<CPxyIsp> GetPxyPlatformSrvAddr() const { return m_setPxyPlatformSrvAddr; }
    void SetPxyPlatformSrvAddr(const std::set<CPxyIsp>& setIsp) { m_setPxyPlatformSrvAddr = setIsp; }

public:
    Json::Value StructToJson() const
    {
        CJsonConvert jsonTool;

        jsonTool.SetValue("m_szPlatformDomainMOID", m_szPlatformDomainMOID);
        jsonTool.SetNestedSetValue("m_setPxyPlatformSrvAddr", m_setPxyPlatformSrvAddr);

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

        jsonTool.GetValue("m_szPlatformDomainMOID", m_szPlatformDomainMOID, sizeof(m_szPlatformDomainMOID));
        jsonTool.GetNestedSetValue("m_setPxyPlatformSrvAddr", m_setPxyPlatformSrvAddr);

        return TRUE;
    }

public:
	void Print() const
	{
		printf("PlatformDomain MoId:%s\n", m_szPlatformDomainMOID);
		OspPrintf(TRUE, FALSE, "PlatformDomain MoId:%s\n", m_szPlatformDomainMOID);
		std::set<CPxyIsp>::iterator iter = m_setPxyPlatformSrvAddr.begin();
		while (iter  != m_setPxyPlatformSrvAddr.end() )
		{
			(*iter).Print();
			iter++;
		}
	}
private:
    s8 m_szPlatformDomainMOID[OPERATION_MAXLEN_MOID+1];     //平台域MOID
    std::set<CPxyIsp> m_setPxyPlatformSrvAddr;   //平台域服务器地址
};

#endif //end of _STRUCT_PXY_H_
