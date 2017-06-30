/*****************************************************************************
   ģ����      : 
   �ļ���      : evnms.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: 
   ����        : 
   �汾        : V4.6  Copyright(C) 2009-2012 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��       �汾        �޸���          �޸�����
   2004/10/25   0.9         zhangsh         ����
   2009/08/28   4.6          �ű���         �����ǻ�����ֲ����    
******************************************************************************/
#ifndef _EV_NMS_H_
#define _EV_NMS_H_

#include "osp.h"
#include "eventid.h"
//MCU�����ܵǼ�ԤԼ������Ϣ,��Ϣ��ΪTSchConfInfoHeader + ���ն�IP
OSPEVENT( MCU_NMS_SCHEDULE_CONF_REQ,				EV_SATMCUNMS_BGN + 1 );
//���ܻ�ӦTSchConfNotif + ��ͻ�ն�IP
OSPEVENT( NMS_MCU_SCHEDULE_CONF_NOTIF,				EV_SATMCUNMS_BGN + 2 );

//�ն����MCU������,TMtOperateToNms
OSPEVENT( MCU_NMS_MTJIONCONF_REQ,				    EV_SATMCUNMS_BGN + 3 );
//���ܸ�MCU�Ĺ����ն�����Ӧ��TMtOperateNotif
OSPEVENT( NMS_MCU_MTJIONCONF_NOTIF,				    EV_SATMCUNMS_BGN + 4 );

//�ն��˻�MCU������,TMtOperateToNms
OSPEVENT( MCU_NMS_MTQUITCONF_REQ,				    EV_SATMCUNMS_BGN + 5 );
//���ܹ����˻��MCU��Ӧ,TMtOperateNotif
OSPEVENT( NMS_MCU_MTQUITCONF_NOTIF,				    EV_SATMCUNMS_BGN + 6 );

//���鿪ʼʱMCU����������Ƶ��,TMcuApplyFreq
OSPEVENT( MCU_NMS_APPLYCONFFREQ_REQ,				EV_SATMCUNMS_BGN + 7 );
//���ܸ�MCU����Ļ�Ӧ,TFreqGetMsg
OSPEVENT( NMS_MCU_APPLYCONFFREQ_NOTIF,				EV_SATMCUNMS_BGN + 8 );

//�������ʱ�ͷ�Ƶ��,TMcuReleaseFreq
OSPEVENT( MCU_NMS_RELEASECONFFREQ_REQ,				EV_SATMCUNMS_BGN + 9 );
//���ܸ�MCU�ͷŵĻ�Ӧ,TFreqReleaseMsg
OSPEVENT( NMS_MCU_RELEASECONFFREQ_NOTIF,			EV_SATMCUNMS_BGN + 10 );

//���Ļ��鿪ʼʱ��/�ӳ�����ʱ��,TConfInfoNotif
OSPEVENT( MCU_NMS_RESCHEDCONF_REQ,				    EV_SATMCUNMS_BGN + 11);
//��Ӧ,TSchConfNotif
OSPEVENT( NMS_MCU_RESCHEDCONF_NOTIF,				EV_SATMCUNMS_BGN + 12 );


#endif /* _EV_NMS_H_ */
