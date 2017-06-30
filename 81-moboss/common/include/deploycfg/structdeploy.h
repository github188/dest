/*=======================================================================
模块名      : 
文件名      : structdeploy.h
相关文件    : 结构体定义
文件实现功能: 

作者        : 
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/03/06  1.0                     创建
=======================================================================*/
#ifndef _STRUCT_DEPLOY_H_
#define _STRUCT_DEPLOY_H_

#include "kdvtype.h"
#include "constcommon.h"
#include "operationconst.h"
#include <string.h>

#ifdef _USE_JSON_CONVERT_
#include "jsonconvert.h"
#endif	//#_USE_JSON_CONVERT_


#ifdef  DEPLOYCFG_EXPORTS
#define DeployCfg_API __declspec( dllexport )
#else
#define DeployCfg_API
#endif


class DeployCfg_API CDeployInfo
{
public:
	CDeployInfo();

	void Init();

	bool operator < (const CDeployInfo& tOther) const;

	//
	const s8* GetKey() const { return m_szKey; }
	void SetKey(const s8* szKey);

	//
	const s8* GetParentKey() const { return m_szParentKey; }
	void SetParentKey(const s8* szParentKey);

	//
	const s8* GetVersion() const { return m_szVersion; }
	void SetVersion(const s8* szVersion);

	//
	const s8* GetUnzipFileDir() const { return m_szUnzipFileDir; }
	void SetUnzipFileDir(const s8* szPath);

	//
	const s8* GetZipFilePath() const { return m_szZipFilePath; }
	void SetZipFilePath(const s8* szPath);

	const s8* GetShellParmFilePath() const { return m_szShellParmFilePath; }
	void SetShellParmFilePath(const s8* szPath);

	//
	const s8* GetCurCfgFilePath() const { return m_szCurCfgFilePath; }
	void SetCurCfgFilePath(const s8* szPath);

	//
	const s8* GetGUID() const { return m_szGUID; }
	void SetGUID(const s8* szGUID);

	const s8* GetMOID() const { return m_szMOID; }
	void SetMOID(const s8* szMOID);

	const s8* GetTimeStamp() const { return m_szTimeStamp; }
	void SetTimeStamp(const s8* szTimeStamp);

	s32 GetPriority() const { return m_dwPriority; }
	void SetPriority(s32 dwPriority){ m_dwPriority = dwPriority; }
    
public:
#ifdef _USE_JSON_CONVERT_
	BOOL32 JsonToStruct(const s8* szJson);
	Json::Value StructToJson()const;
#endif

private:
	s8 m_szKey[MAXLEN_APP_NAME + 1]; 
	s8 m_szParentKey[MAXLEN_APP_NAME + 1];
	s8 m_szVersion[MAXLEN_VERSION + 1];      

	s8 m_szUnzipFileDir[MAXLEN_FILE_PATH + 1];      
	s8 m_szZipFilePath[MAXLEN_FILE_PATH + 1];      

	s8 m_szShellParmFilePath[MAXLEN_FILE_PATH + 1];	//shell脚本的参数文件路径
	s8 m_szCurCfgFilePath[MAXLEN_FILE_PATH + 1];	//配置文件路径  

	s8 m_szGUID[(OPERATION_MAXLEN_GUID+1)*2];
	s8 m_szMOID[OPERATION_MAXLEN_MOID+1];
	s8 m_szTimeStamp[MAXLEN_TIMESTAMP+1];     //安装或者升级成功的时间戳
	s32 m_dwPriority;
};

#endif // _STRUCT_DEPLOY_H_
