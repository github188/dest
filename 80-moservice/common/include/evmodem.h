/*****************************************************************************
   ģ����      : 
   �ļ���      : evmodem.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ����MODEM��Ϣ
   ����        : 
   �汾        : V0.9  Copyright(C) 2001-2004 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2004/10/25  0.9         zhangsh        ����
******************************************************************************/
#ifndef _EV_MODEM_H_
#define _EV_MODEM_H_

#include "osp.h"
#include "eventid.h"

//��MODEMģ��ע��TMt + TTransportAddr + type(u8)
OSPEVENT( MODEM_MCU_REG_REQ,				EV_MODEM_BGN );
//ע��ȷ��
OSPEVENT( MODEM_MCU_REG_ACK,				EV_MODEM_BGN + 1 );
//ע��ܾ�
OSPEVENT( MODEM_MCU_REG_NACK,				EV_MODEM_BGN + 2 );

//MODEM�������״̬
OSPEVENT( MCU_MODEM_RECEIVE,				EV_MODEM_BGN + 10 );
//ȷ��
OSPEVENT( MODEM_MCU_REV_ACK,				EV_MODEM_BGN + 11 );
//�ܾ�
OSPEVENT( MODEM_MCU_REV_NACK,				EV_MODEM_BGN + 12 );
//֪ͨ
OSPEVENT( MODEM_MCU_REV_NOTIF,				EV_MODEM_BGN + 13 );

//MODEM���뷢��״̬
OSPEVENT( MCU_MODEM_SEND,				    EV_MODEM_BGN + 20 );
//ȷ��
OSPEVENT( MODEM_MCU_SEND_ACK,				EV_MODEM_BGN + 21 );
//�ܾ�
OSPEVENT( MODEM_MCU_SEND_NACK,				EV_MODEM_BGN + 22 );
//֪ͨ
OSPEVENT( MODEM_MCU_SEND_NOTIF,				EV_MODEM_BGN + 23 );

//MODEM����ֹͣ����״̬
OSPEVENT( MCU_MODEM_STOPSEND,				EV_MODEM_BGN + 30 );
//ȷ��
OSPEVENT( MODEM_MCU_STOPSEND_ACK,			EV_MODEM_BGN + 31 );
//�ܾ�
OSPEVENT( MODEM_MCU_STOPSEND_NACK,			EV_MODEM_BGN + 32 );
//֪ͨ
OSPEVENT( MODEM_MCU_STOPSEND_NOTIF,			EV_MODEM_BGN + 33 );

//MODEM����ʱ���趨
OSPEVENT( MCU_MODEM_SETSRC,				    EV_MODEM_BGN + 40 );
//ȷ��
OSPEVENT( MODEM_MCU_SETSRC_ACK,				EV_MODEM_BGN + 41 );
//�ܾ�
OSPEVENT( MODEM_MCU_SETSRC_NACK,			EV_MODEM_BGN + 42 );
OSPEVENT( MODEM_MCU_SETSRCCLK_NOTIF,		EV_MODEM_BGN + 43 );
OSPEVENT( MODEM_MCU_SETREVCLK_NOTIF,		EV_MODEM_BGN + 44 );

//MODEM�ͷ�
OSPEVENT( MCU_MODEM_RELEASE,				EV_MODEM_BGN + 50 );
//ȷ��
OSPEVENT( MODEM_MCU_RELEASE_ACK,			EV_MODEM_BGN + 51 );
//�ܾ�
OSPEVENT( MODEM_MCU_RELEASE_NACK,			EV_MODEM_BGN + 52 );
OSPEVENT( MODEM_MCU_RELEASE_NOTIF,			EV_MODEM_BGN + 53 );

OSPEVENT( MODEM_MCU_ERRSTAT_NOTIF,			EV_MODEM_BGN + 60 );
OSPEVENT( MODEM_MCU_RIGHTSTAT_NOTIF,		EV_MODEM_BGN + 70 );
//MODEM�ͷ�
OSPEVENT( MCU_MODEM_SETBITRATE,				EV_MODEM_BGN + 80 );
//ȷ��
OSPEVENT( MODEM_MCU_SETBITRATE_ACK,			EV_MODEM_BGN + 81 );
//�ܾ�
OSPEVENT( MODEM_MCU_SETBITRATE_NACK,		EV_MODEM_BGN + 82 );
OSPEVENT( MODEM_MCU_SETBITRATE_NOTIF,		EV_MODEM_BGN + 83 );

#endif /* _EV_MCU_H_ */
