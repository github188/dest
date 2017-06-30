

/*=======================================================================
模块名      :ULS
文件名      :structlogin.h
相关文件    :
文件实现功能:终端结构体定义

作者        :
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/07/22  1.0                       创建
=======================================================================*/
#ifndef _STRUCT_XAP_LOGIN_INFO_H_
#define _STRUCT_XAP_LOGIN_INFO_H_

#include "kdvtype.h"
#include "constuls.h"
#include "operationconst.h"
#include "md5.h"

#ifdef WIN32
#include<Winsock2.h>
#endif

#ifdef _LINUX_
#include <arpa/inet.h>
#endif

#ifdef _JSON_CONVERT_
#include "jsonconvert.h"
#endif  //_JSON_CONVERT_

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

class CLoginInfo
{
public:
	CLoginInfo();
	void Init();

	const s8* GetUserName() const;
	void SetUserName(const s8* pchUserName);

	const s8* GetPassWord() const;
	void SetPassWord(const s8* pchPassWord);
	void SetEncryptedAccountPwd(const s8*pchPwd);

    const s8* GetOemMark() const;
    void SetOemMark(const s8* pchOemMark);

    const s8* GetDeviceType() const;
    void SetDeviceType(const s8* szDevType);

    u32 GetApsLocalIp() const { return ntohl(m_dwLocalUlsIp); }
    void SetApsLocalIp(u32 dwIp) { m_dwLocalUlsIp = htonl(dwIp); }

	u32 GetClientIp() const { return ntohl(m_dwMtIp); }
    void SetClientIp(u32 dwIp) { m_dwMtIp = htonl(dwIp); }
    
  BOOL32 IsHttpsClient() const { return ( 0 != m_byIsHttpsClient ); }
  void SetIsHttpsClient( BOOL32 bIsHttpsClient ) { m_byIsHttpsClient = (bIsHttpsClient ? 1 : 0); }

public:
#ifdef _JSON_CONVERT_
    Json::Value StructToJson() const;
	BOOL32 JsonToStruct(const s8* szJson);
#endif  //_JSON_CONVERT_

private:
	s8 m_szUserName[OPERATION_MAXLEN_ACCOUNTNAME + 1];          //终端登录名称
	s8 m_szPassWord[OPERATION_MAXLEN_PWD + 1];                  //终端登录密码
	s8 m_szOemMark[OPERATION_MAXLEN_OEMMARK + 1];               //终端oem标识
    s8 m_szDeviceType[OPERATION_MAXLEN_DEVICETYPE+1];          //终端类型
    u32 m_dwLocalUlsIp;     //本地XAp的Ip地址
	u32 m_dwMtIp;           //终端的IP地址
	u8 m_byIsHttpsClient;
}
PACKED;

inline CLoginInfo::CLoginInfo()
{
	Init();
}

inline void CLoginInfo::Init()
{
	memset(m_szUserName, 0, sizeof(m_szUserName));
	memset(m_szPassWord, 0, sizeof(m_szPassWord));
    memset(m_szOemMark, 0, sizeof(m_szOemMark));
    memset(m_szDeviceType, 0, sizeof(m_szDeviceType));
	m_dwLocalUlsIp = 0;
	m_dwMtIp = 0;
	m_byIsHttpsClient = 0;
	
}

inline const s8* CLoginInfo::GetUserName() const
{
	return m_szUserName;
}

inline void CLoginInfo::SetUserName(const s8* pchUserName)
{
	if (NULL == pchUserName)
	{
		memset(m_szUserName, 0, sizeof(m_szUserName));
		return;
	}

	strncpy(m_szUserName, pchUserName, sizeof(m_szUserName)-1 );
	m_szUserName[sizeof(m_szUserName)-1] = '\0';
}

//只能返回加密后的密码
inline const s8* CLoginInfo::GetPassWord() const
{
	return m_szPassWord;
}

//输入的密码为明文
inline void CLoginInfo::SetPassWord(const s8* pchPassWord)
{
	if (NULL == pchPassWord)
	{
		memset(m_szPassWord, 0, sizeof(m_szPassWord));
		return;
	}

    s8 szTemp[OPERATION_MAXLEN_PWD + 1] = {0};
    strncpy(szTemp, pchPassWord, sizeof(szTemp)-1);
    szTemp[sizeof(szTemp)-1] = '\0';

	CMD5Encrypt cMd5;
	cMd5.GetEncrypted(szTemp,m_szPassWord);
	m_szPassWord[sizeof(m_szPassWord)-1] = '\0';

	return;
}

//输入的密码已经加密
inline void CLoginInfo::SetEncryptedAccountPwd(const s8*pchPwd)
{
	if (NULL == pchPwd)
	{
		memset(m_szPassWord, 0, sizeof(m_szPassWord));
		return;
	}

	strncpy(m_szPassWord, pchPwd, sizeof(m_szPassWord) - 1);
	m_szPassWord[sizeof(m_szPassWord) - 1] = '\0';
}

inline const s8* CLoginInfo::GetOemMark() const
{
    return m_szOemMark;
}

inline void CLoginInfo::SetOemMark(const s8* szOEMMark)
{
	if (NULL == szOEMMark)
	{
		memset(m_szOemMark, 0, sizeof(m_szOemMark));
		return;
	}
	
	strncpy(m_szOemMark, szOEMMark, sizeof(m_szOemMark)-1);
	m_szOemMark[sizeof(m_szOemMark)-1] = '\0';
   
}

inline const s8* CLoginInfo::GetDeviceType() const
{
    return m_szDeviceType;
}

inline void CLoginInfo::SetDeviceType(const s8* szDevType)
{
    if (NULL == szDevType)
    {
        memset(m_szDeviceType, 0, sizeof(m_szDeviceType));
        return;
    }
    
    strncpy(m_szDeviceType, szDevType, sizeof(m_szDeviceType)-1);
    m_szDeviceType[sizeof(m_szDeviceType)-1] = '\0';    
}

#ifdef _JSON_CONVERT_

inline Json::Value CLoginInfo::StructToJson() const
{
	CJsonConvert jsonTool;

	jsonTool.SetValue("m_szOemMark", m_szOemMark);
	jsonTool.SetValue("m_szUserName", m_szUserName);
	jsonTool.SetValue("m_szPassWord", m_szPassWord);
	jsonTool.SetValue("m_szDeviceType", m_szDeviceType);
    jsonTool.SetValue("m_dwLocalUlsIp", GetApsLocalIp());
	jsonTool.SetValue("m_dwMtIp", GetClientIp());

	return jsonTool.GetJson();
}

inline BOOL32 CLoginInfo::JsonToStruct(const s8* szJson)
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

	jsonTool.GetValue("m_szUserName", m_szUserName, sizeof(m_szUserName));
	jsonTool.GetValue("m_szPassWord", m_szPassWord, sizeof(m_szPassWord));
	jsonTool.GetValue("m_szOemMark", m_szOemMark, sizeof(m_szOemMark));
	jsonTool.GetValue("m_szDeviceType", m_szDeviceType, sizeof(m_szDeviceType));
    u32 dwTemp;
    jsonTool.GetValue("m_dwLocalUlsIp", dwTemp);
    SetApsLocalIp(dwTemp);

	jsonTool.GetValue("m_dwMtIp", dwTemp);
	SetClientIp(dwTemp);

	return TRUE;
}

#endif  //_JSON_CONVERT_


#ifdef WIN32
#pragma pack( pop )
#endif


#endif //end of _STRUCT_XAP_LOGIN_INFO_H_

