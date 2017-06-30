/*=======================================================================
ģ����      : deploycfg
�ļ���      : 
����ļ�    :
�ļ�ʵ�ֹ���: node�ĳ��������

����        :
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/07/03  1.0                     ����
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
	//�ļ�·��
	const s8* GetCfgFileName() const;
	//�����ļ��Ƿ����
	BOOL32 IsConfigFileExist();

	//�Ƿ�װ
	//��ͨ��GetLogicSrvRecipeName��ȡ����
	BOOL32 IsInstall(const s8* szRecipeName);

	//��װ��ʶ
	BOOL32 Install(const CDeployInfo& tIntallInfo);
	BOOL32 UnInstall(const s8* szRecipeName);

	//CDeployInfo
	BOOL32 GetDeployInfo(const s8* szRecipeName, OUT CDeployInfo& tInfo);
	BOOL32 GetAllDeploy(OUT std::set<CDeployInfo>& setInfo);

	//GUID
	BOOL32 GetGUID(const s8* szRecipeName, OUT TGUID& tGUID);
	BOOL32 SetGUID(const s8* szRecipeName, const s8* szGUID);

	//��ȡ�����������GUID(��ʵ���ǻ�ȡguard��GUID,ҵ���Ϲ涨:��װguardʱ,�û�������д�����������GUID)
	BOOL32 GetPhysicsSrvGUID(OUT TGUID& tGUID);     
	
	//MOID
	BOOL32 GetMOID(const s8* szRecipeName, OUT TMOID& tMOID);
	BOOL32 SetMOID(const s8* szRecipeName, const s8* szMOID);
private:
	CDeployMpl* m_pDeployMpl;
};

#endif  //_DEPLOY_CFG_H_
