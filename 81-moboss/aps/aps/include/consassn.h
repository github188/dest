
/*****************************************************************************
   ģ����      :
   �ļ���      : consassn.h
   ����ļ�    : consassn.cpp
   �ļ�ʵ�ֹ���: ��3A��ͨ�ţ���ø�����������Ϣ
   ����        :
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/07/08  1.0                     ����
******************************************************************************/
#ifndef _CON_SA_SSN_H_
#define _CON_SA_SSN_H_

#pragma warning( disable : 4786 )


#include "osp.h"
#include "connectsa_api.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////



class CConSaSsnInst : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_CONNECT,
		STATE_NORMAL,
	};

	CConSaSsnInst();
	~CConSaSsnInst();
    void ClearInst();

public:
	void DaemonInstanceEntry( CMessage *const pcMsg, CApp* pcApp);
	void InstanceEntry(CMessage *const pMsg);

	//
	void DaemonProcPowerOn(CMessage *const pcMsg);

    //��������״̬
    //������Ӧ�÷ŵ�CCenterCtrlApp��,���ǵ��ܶ���·����ͨʱ��
    //�˺�������ɺܳ�ʱ��Ķ���, ����CCenterCtrlApp��,�ᵼ�����������ܵ������½�
    void CheckSrvStateTimeOut(CMessage *const pcMsg);

};

class CConSaSsnConfig
{

};

typedef zTemplate<CConSaSsnInst, 1, CConSaSsnConfig> CConSaSsnApp;

extern CConSaSsnApp g_cConSaSsnApp;

#endif  //end of _CON_SA_SSN_H
