/*****************************************************************************
ģ����      : MAU
�ļ���      : resmgrssn.h
����ʱ��    : 2014�� 08�� 26��
ʵ�ֹ���    :
����        : ��Ҳ
�汾        :
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
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
	//�ػ�ʵ����Ϣ�������
	void DaemonInstanceEntry(CMessage * const pcMsg, CApp *pcApp);
	//�ػ�ʵ�������ʼ����Ϣ
	void DaemonProcPowerOn(const CMessage *pcMsg, CApp *pcApp);

	void InstanceEntry(CMessage * const pcMsg);
};

typedef zTemplate<CResMgrSsnInstance, 0, CResSsnData, 0> CResMgrSsnApp;

#endif  // _RESMGRSSN_INSTANCE_H_
