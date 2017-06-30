
/*=======================================================================
ģ����      :XAP
�ļ���      :eventself.h
����ļ�    :
�ļ�ʵ�ֹ���:XAP�ڲ�ͨ����Ϣ����

����        :
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/07/12  1.0                       ����
=======================================================================*/
#ifndef _EVENT_APS_SELF_H_
#define _EVENT_APS_SELF_H_

#include "osp.h"
#include "eventid.h"

#define EV_APS_SELF_BEG                          5000


/************************************************************************/
/*								Dump                    
/************************************************************************/
//dump dbpool��״̬
OSPEVENT(APS_DBPOOL_STATE_DUMP,                 EV_APS_SELF_BEG + 1);

//dump ulc��״̬
OSPEVENT(APS_DUMP_ULC_STATE_DUMP,               EV_APS_SELF_BEG + 2);

//dump ������״̬
OSPEVENT(APS_DUMP_SRV_STATE_DUMP,               EV_APS_SELF_BEG + 4);


/************************************************************************/
/*								                    
/************************************************************************/
//������ⶨʱ��
OSPEVENT(CHECK_HEALTH_TIMER,                    EV_APS_SELF_BEG + 10);
           

/************************************************************************/
/*								����������б�                   
/************************************************************************/
//����������б�ʱ��
OSPEVENT(QUERY_SERVERDEV_LIST_TIMER,            EV_APS_SELF_BEG + 30);
OSPEVENT(QUERY_SERVERDEV_LIST_REQ,              EV_APS_SELF_BEG + 31);
OSPEVENT(QUERY_SERVERDEV_LIST_ACK,              EV_APS_SELF_BEG + 32);
OSPEVENT(QUERY_SERVERDEV_LIST_NACk,             EV_APS_SELF_BEG + 33);


/************************************************************************/
/*								��½                    
/************************************************************************/
//ULC��½������Ϣ           ����:CUlcLoginInfo
OSPEVENT( MT_APS_LOGIN_REQ,				        EV_APS_SELF_BEG + 40);

//ULC��½������Ϣ�ɹ���   ����:CUlcFullInfo
OSPEVENT( APS_MT_LOGIN_ACK,				        EV_APS_SELF_BEG + 41 );

//ULC��½������Ϣʧ�ܴ�   ����:(u32_ErrorCode)
OSPEVENT( APS_MT_LOGIN_NACK,			        EV_APS_SELF_BEG + 42 );

OSPEVENT( SEARCH_PLATFORM_BACKUP_CONFIG_REQ,	EV_APS_SELF_BEG + 43 );

#endif //end of _EVENT_APS_SELF_H_
