/*****************************************************************************
ģ����      : connmsadpssnͨ��ģ��
�ļ���      : connmsadpssn.h
����ļ�    : 
�ļ�ʵ�ֹ���: connmsadpssnͨ��ģ��ʵ������
����        : ����
�汾        : V5.0  Copyright(C) 2009-2010 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2015/01/15  5.0         ����        ����
******************************************************************************/
#ifndef _MAU_CONNMSADPSSN_H_
#define _MAU_CONNMSADPSSN_H_

#include "evmau.h"
#include "connectadp_api.h"
#include "mauvc.h"
#include "cmussn.h"
#include "nussn.h"
#include "upussn.h"
#include "mcuver.h"
#include "resmgrssn.h"
#include "ztemplatedaemon.h"

extern CCMUSsnApp g_cCMUSsnApp;
extern CNUSsnApp g_cNUSsnApp;
extern CMauVCApp g_cMauVCApp;
extern CUpuSsnApp g_cUpuSsnApp;

//��������֪ͨ������Ϣ�Ƿ���ȫ�Ļص�����
void SetCssPfmCallBackFunc( CAdpServMsg & cAdpServMsg );
//�������ӳɹ��ص�֪ͨ
void ConnNMSSuccessCallBackFunc();

class CConNmsAdpInst : public CInstance  
{
public:
    //�ػ�ʵ�����
    void DaemonInstanceEntry( CMessage * const pcMsg, CApp * pcApp );
    //��ͨʵ�����
    void InstanceEntry( CMessage * const pcMsg );
	
    void DaemonProcPowerOn( CMessage * const pcMsg );
	void DaemonProcOspDisconnect( CMessage * const pcMsg );
	void DaemonProcTransMsgToAdp( CMessage* const pcMsg );
	
};

typedef zTemplate< CConNmsAdpInst, 0, CAppNoData, 0 > CConNmsAdpApp;

#endif
