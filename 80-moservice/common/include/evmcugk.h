/*****************************************************************************
   ģ����      : �·���ҵ��
   �ļ���      : evras.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: �·���ҵ��RAS��Ϣ����
   ����        : zhangsh
   �汾        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2003/10/15  1.0         zhangsh        ����
******************************************************************************/
#ifndef _EV_MCUGK_H_
#define _EV_MCUGK_H_

#include "osp.h"
#include "eventid.h"

/**********************************************************
 ����Ϊҵ��RAS�ڲ���Ϣ
**********************************************************/
//Э��ջ����������Ϣ,��Ϣ����BUF��
OSPEVENT( STACK_GKM_REQ,			  EV_RAS_BGN );
//Э��ջ����ȷ����Ϣ,��Ϣ����BUF��
OSPEVENT( STACK_GKM_CFM,			  EV_RAS_BGN + 1 );
//Э��ջ���ؾܾ���Ϣ,��Ϣ����BUF��
OSPEVENT( STACK_GKM_REJ,			  EV_RAS_BGN + 2 );
//Э��ջ���س�ʱ��Ϣ,��Ϣ����BUF��
OSPEVENT( STACK_GKM_EXP,			  EV_RAS_BGN + 3 );
//�ⲿ��ʱ������Ϣ
OSPEVENT( MTADP_GKM_TIMER,			  EV_RAS_BGN + 4 );
/*-----------����Ϊ�ⲿ��Ϣ�ӿ�-----------------------------*/
//ע��MCU����MCU->GKM,��Ϣ��Ϊ��
OSPEVENT( MCU_GKM_REGISTERMCU_REQ,		  EV_RAS_BGN + 10 );
//ע���������MCU->GKM,��Ϣ��ΪCRegisterRequest
OSPEVENT( MCU_GKM_REGISTERCOF_REQ,		  EV_RAS_BGN + 20 );
//ע��MCUȷ��,GKM->MCU,��Ϣ��Ϊ��
OSPEVENT( GKM_MCU_REGISTER_ACK,		  	  EV_RAS_BGN + 11 );
//ע��MCU�ܾ�,GKM->MCU,��Ϣ��Ϊ�������
OSPEVENT( GKM_MCU_REGISTER_NACK,		  EV_RAS_BGN + 12 );

//��ʼ����͵�ַ��������MCU->GKM,��Ϣ��ΪCAdmissinRequest
OSPEVENT( MCU_GKM_ADMISSION_REQ,		  EV_RAS_BGN + 30 );
//��ʼ����͵�ַ����ȷ��,GKM->MCU,��Ϣ��ΪTTransportAddr
OSPEVENT( GKM_MCU_ADMISSION_ACK,		  EV_RAS_BGN + 31 );
//��ʼ����͵�ַ�����ܾ�,GKM->MCU,��Ϣ��Ϊ�������
OSPEVENT( GKM_MCU_ADMISSION_NACK,		  EV_RAS_BGN + 32 );

//����ı�����MCU->GKM,��Ϣ��ΪCBandWidthRequest
OSPEVENT( MCU_GKM_BANDWIDTH_REQ,		  EV_RAS_BGN + 40 );
//����ı�ȷ��,GKM->MCU,��Ϣ��Ϊ��
OSPEVENT( GKM_MCU_BANDWIDTH_ACK,		  EV_RAS_BGN + 41 );
//����ı�ܾ�,GKM->MCU,��Ϣ��Ϊ�������
OSPEVENT( GKM_MCU_BANDWIDTH_NACK,		  EV_RAS_BGN + 42 );

//�����ͷ�����MCU->GKM,��Ϣ��ΪCDisengageRequest
OSPEVENT( MCU_GKM_DISENGAGE_REQ,		  EV_RAS_BGN + 50 );
//�����ͷ�ȷ��,GKM->MCU,��Ϣ��Ϊ��
OSPEVENT( GKM_MCU_DISENGAGE_ACK,		  EV_RAS_BGN + 51 );
//�����ͷžܾ�,GKM->MCU,��Ϣ��Ϊ�������
OSPEVENT( GKM_MCU_DISENGAGE_NACK,		  EV_RAS_BGN + 52 );

//ע����������MCU->GKM,��Ϣ��ΪCUnregisterRequest
OSPEVENT( MCU_GKM_UNREGISTERCOF_REQ,	  	  EV_RAS_BGN + 60 );
//ע��MCU����MCU->GKM,��Ϣ��Ϊ��
OSPEVENT( MCU_GKM_UNREGISTERMCU_REQ,		  EV_RAS_BGN + 70 );
//ע������ȷ��,GKM->MCU,��Ϣ��Ϊ��
OSPEVENT( GKM_MCU_UNREGISTER_ACK,	  	  EV_RAS_BGN + 61 );
//ע������ܾ�,GKM->MCU,��Ϣ��Ϊ�������
OSPEVENT( GKM_MCU_UNREGISTER_NACK,		  EV_RAS_BGN + 62 );

/*------------------������Ϣ-----------------------------------*/
//��ʱ��Ϣ,GKM->MCU,��Ϣ��Ϊ�������
OSPEVENT( GKM_MCU_TIMEOUT,		  	  EV_RAS_BGN + 80 );
//����ע��MCU���߻���,GKM->MCU,��Ϣ��Ϊ��
OSPEVENT( GKM_MCU_UNREGISTER_NOTIFY,	  	  EV_RAS_BGN + 81 );
//�����ͷŴ���֪ͨ,GKM->MCU,��Ϣ��ΪCDisengageRequest
OSPEVENT( GKM_MCU_DISENGAGE_NOTIFY,		  EV_RAS_BGN + 82 );

#endif /* _EV_RAS_H_ */
