/*=======================================================================
模块名      : deploycfg
文件名      : 
相关文件    :
文件实现功能: node的程序部署情况

作者        :
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/07/03  1.0                     创建
=======================================================================*/
#ifndef _DEPLOY_CFG_H_
#define _DEPLOY_CFG_H_

#pragma warning (disable:4786 )

#include <set>
#include "kdvtype.h"
#include "structdeploy.h"
#include <string.h>
#include "guid.h"
#include "moid.h"

#ifdef  DEPLOYCFG_EXPORTS
#define DeployCfg_API __declspec( dllexport )
#else
#define DeployCfg_API
#endif

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

class CDeployMpl;
class DeployCfg_API CDeployCfg
{
public:
	CDeployCfg();
	~CDeployCfg();

public:
	//文件路径
	const s8* GetCfgFileName() const;
	//配置文件是否存在
	BOOL32 IsConfigFileExist();

	//是否安装
	//可通过GetLogicSrvRecipeName获取名称
	BOOL32 IsInstall(const s8* szRecipeName);

	//安装标识
	BOOL32 Install(const CDeployInfo& tIntallInfo);
	BOOL32 UnInstall(const s8* szRecipeName);

	//CDeployInfo
	BOOL32 GetDeployInfo(const s8* szRecipeName, OUT CDeployInfo& tInfo);
	BOOL32 GetAllDeploy(OUT std::set<CDeployInfo>& setInfo);

	//GUID
	BOOL32 GetGUID(const s8* szRecipeName, OUT TGUID& tGUID);
	BOOL32 SetGUID(const s8* szRecipeName, const s8* szGUID);

	//获取物理服务器的GUID(其实就是获取guard的GUID,业务上规定:安装guard时,用户必须填写物理服务器的GUID)
	BOOL32 GetPhysicsSrvGUID(OUT TGUID& tGUID);     
	
	//MOID
	BOOL32 GetMOID(const s8* szRecipeName, OUT TMOID& tMOID);
	BOOL32 SetMOID(const s8* szRecipeName, const s8* szMOID);
private:
	CDeployMpl* m_pDeployMpl;
};

#endif  //_DEPLOY_CFG_H_
