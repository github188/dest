/*****************************************************************************
ģ����      : MAU
�ļ���      : hdussn.h
����ʱ��    : 2015�� 01�� 26��
ʵ�ֹ���    : 
����        : ����
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2015/01/26  5.0         ����          ����
******************************************************************************/
#ifndef	_HDUSSN_INSTANCE_H_
#define	_HDUSSN_INSTANCE_H_

#include "osp.h"
#include "hdustruct.h"
#include "evhduacs.h"


#define CONNECTHDUPOOL_TIMEOUT         (u16)3*1000
#define REGISTHDUPOOL_TIMEOUT          (u16)3*1000

struct THduPoolInfo
{
	u32 m_dwIp;
	u32 m_wPort;
};

class CHduPoolSsnInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_NORMAL,
	};

public:
	CHduPoolSsnInstance();
	~CHduPoolSsnInstance();

public:
	void DaemonInstanceEntry( CMessage *const pcMsg, CApp *pcApp );
	//�ϵ��ʼ������
	void DaemonProcPowerOn( CMessage * const pcMsg );
	//��������
	void DaemonProcDisconnect( CMessage * const pcMsg );
	//����HduPool��ʱ����Ϣ
	void DaemonProcConnectHduPool(CMessage * const pcMsg);
	//����HduPool��ʱ����Ϣ
	void DaemonProcConnectTimer( CMessage * const pcMsg );
	//ע��HduPool��ʱ����Ϣ����
	void DaemonProcRegHduPool( CMessage * const pcMsg );
	//HduPoolע�����Ϣ����
	void DaemonProcHduPoolRegRsp( CMessage * const pcMsg );
	//����HduPool���͸�mau��hdu֪ͨ
	void DaemonProcHduInfoNtf( CMessage * const pcMsg );
	//����mau��HduPool��hdu��Ϣ�����
	void DaemonProcHduInfoReqRsp( CMessage * const pcMsg );
	//����������hduͨ������
	void DaemonProcSetHduChnVolume( CMessage * const pcMsg );

	void InstanceEntry( CMessage * const pcMsg );
	
	//����hdupool
	BOOL32 ConnectHduPool();
	//ע��hdupool
	void RegistHduPool();

public:
	u32	m_dwHduPoolNode;
	u32 m_dwHduPoolIId;
	u32 m_dwIp;
	u32 m_wPort;
};

typedef zTemplate<CHduPoolSsnInstance, 1> CHduPoolSsnApp;

#endif
