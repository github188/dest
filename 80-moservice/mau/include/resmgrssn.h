/*****************************************************************************
模块名      : MAU
文件名      : resmgrssn.h
创建时间    : 2014年 08月 26日
实现功能    :
作者        : 王也
版本        :
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifndef	_RESMGRSSN_INSTANCE_H_
#define	_RESMGRSSN_INSTANCE_H_

#include "osp.h"
#include "mauvc.h"
#include "ztemplatedaemon.h"
#include "IZkClient.h"
using namespace ZkClient;

class CResSsnData
{
public:
	CResSsnData();
	virtual ~CResSsnData();

public:
	BOOL32 ConnectResource();
	BOOL32 IsConnectedZK();
	const s8* GetZkState();
	void Quit();

	static void ResourceCBFunc(CallbackParam* pParam);

private:
	IZkApplyClient * m_pzkClient;
	static BOOL32 bZkConnected;
};

class CResMgrSsnInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_NORMAL,
	};

public:
	CResMgrSsnInstance();
	~CResMgrSsnInstance();

public:
	//守护实例消息处理入口
	void DaemonInstanceEntry(CMessage * const pcMsg, CApp *pcApp);
	//守护实例处理初始化消息
	void DaemonProcPowerOn(const CMessage *pcMsg, CApp *pcApp);

	void InstanceEntry(CMessage * const pcMsg);
};

typedef zTemplate<CResMgrSsnInstance, 0, CResSsnData, 0> CResMgrSsnApp;

#endif  // _RESMGRSSN_INSTANCE_H_
