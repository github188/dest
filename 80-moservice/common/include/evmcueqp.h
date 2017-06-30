/*****************************************************************************
   ģ����      : �·���ҵ��
   �ļ���      : evmcueqp.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: �·���ҵ��MCU������ҵ��ӿ���Ϣ����
                 *_REQ: ��Ӧ������
				 *_ACK, *_NACK: ����֮��
				 *_CMD: ����Ӧ������
				 *_NOTIF: ״̬֪ͨ
   ����        : ����
   �汾        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2003/05/15  1.0         LI Yi       ��evsvc.h�зֳ���
******************************************************************************/
#ifndef _EV_MCUEQP_H_
#define _EV_MCUEQP_H_

#include "osp.h"
#include "eventid.h"

//ע��Ϊ��֤������ȷ���У����гɹ���ʧ��Ӧ���ԭ��Ϣ�ŷֱ��1��2

/**********************************************************
 ����ΪMCU������֮��ҵ��ӿ� (26001-27000)
**********************************************************/

//ǰ��100Ϊ�ڲ�����

/*MCU��REC��ҵ��ӿ���Ϣ(26101-26300)*/
#define EV_MCUREC_BGN			EV_MCUEQP_BGN + 100
#define EV_MCUREC_END			EV_MCUREC_BGN + 199

/*MCU��MIXER��ҵ��ӿ���Ϣ(26301-26400)*/
#define EV_MCUMIXER_BGN			EV_MCUREC_END	+ 1
#define EV_MCUMIXER_END			EV_MCUMIXER_BGN + 99

/*MCU��TVWALL��ҵ��ӿ���Ϣ(26401-26440)*/
#define EV_MCUTVWALL_BGN		EV_MCUMIXER_END	 + 1
#define EV_MCUTVWALL_END		EV_MCUTVWALL_BGN + 39

/*MCU��HDU��ҵ��ӿ���Ϣ(26441-26500)*/
#define EV_MCUHDU_BGN		EV_MCUTVWALL_END + 1
#define EV_MCUHDU_END		EV_MCUHDU_BGN + 59

/*MCU��DCS�Ự��ҵ��ӿ���Ϣ(26501-26600)*/
#define EV_MCUDCS_BGN			EV_MCUHDU_END + 1
#define EV_MCUDCS_END			EV_MCUDCS_BGN + 99

/*MCU��BAS��ҵ��ӿ���Ϣ(26601-26700)*/
#define EV_MCUBAS_BGN			EV_MCUDCS_END + 1
#define EV_MCUBAS_END			EV_MCUBAS_BGN + 99

/*���渴����ҵ����Ϣ(26701-26800)*/
#define EV_MCUVMP_BGN			EV_MCUBAS_END + 1
#define EV_MCUVMP_END			EV_MCUVMP_BGN + 99

/*���ϵ���ǽҵ����Ϣ(26801-26900)*/
#define EV_MCUVMPTW_BGN			EV_MCUVMP_END   + 1
#define EV_MCUVMPTW_END			EV_MCUVMPTW_BGN + 99

/*�����ش���ҵ����Ϣ(26901-27000)*/
#define EV_MCUPRS_BGN			EV_MCUVMPTW_END + 1
#define EV_MCUPRS_END			EV_MCUPRS_BGN   + 99

/*MpuBasҵ����Ϣ(39601-39650)*/
#define EV_MCUMPUBAS_BGN	    EV_MPU_BGN
#define EV_MCUMPUBAS_END	    EV_MCUMPUBAS_BGN + 49

/*MpuEBapҵ����Ϣ(39651-39700)*/
#define EV_MCUMPUEBAP_BGN       EV_MCUMPUBAS_END + 1
#define EV_MCUMPUEBAP_END       EV_MCUMPUEBAP_BGN + 49

/*MpuEVpuҵ����Ϣ(39701-39800)*/
#define EV_MCUMPUEVPU_BGN       EV_MCUMPUEBAP_END + 1
#define EV_MCUMPUEVPU_END       EV_MCUMPUEVPU_BGN + 99

/*MpuSVmpҵ����Ϣ(39801-39900)*/
#define EV_MCUMPUSVMP_BGN       EV_MCUMPUEVPU_END + 1
#define EV_MCUMPUSVMP_END       EV_MCUMPUSVMP_BGN + 99

/*MpuDVmpҵ����Ϣ(39901-40000)*/
#define EV_MCUMPUDVMP_BGN       EV_MCUMPUSVMP_END + 1
#define EV_MCUMPUDVMP_END       EV_MCUMPUDVMP_BGN + 99

/*�ļ�������ҵ����Ϣ(26901-27000)*/
//#define EV_MCUFILEENC_BGN	EV_MCUPRS_END     + 1
//#define EV_MCUFILEENC_END	EV_MCUFILEENC_BGN + 1

//����ΪMCU������֮�������������к��ŵ������Ϣ���ŵ��ž�λ����Ϣͷ��


/*-------- ����ΪMCU���°�REC��Ϣ�Ŷ���(26101-26300) --------*/

//¼�����MCU����ע������REC->MCU����Ϣ��ΪTPeriEqpRegReq�ṹ
OSPEVENT( REC_MCU_REG_REQ,					EV_MCUREC_BGN );
//MCU����Ӧ��MCU->REC����Ϣ��Ϊ��Ϣ��ΪTPeriEqpRegAck
OSPEVENT( MCU_REC_REG_ACK,					EV_MCUREC_BGN + 1 );
//MCU�ܾ�Ӧ��MCU->REC������Ϣ��
OSPEVENT( MCU_REC_REG_NACK,					EV_MCUREC_BGN + 2 );

//MCU������¼���״̬����, MCU->REC����Ϣ��ΪTEqp
OSPEVENT( MCU_REC_GETRECSTATUS_REQ,			EV_MCUREC_BGN + 5 );
//����¼���״̬��REC->MCU����Ϣ��TPeriEqpStatus
OSPEVENT( REC_MCU_GETRECSTATUS_ACK,			EV_MCUREC_BGN + 6 );
//�ܾ�����¼���״̬��REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_GETRECSTATUS_NACK,		EV_MCUREC_BGN + 7 );
//¼���״̬֪ͨ��REC->MCU����Ϣ��ΪTPeriEqpStatus
OSPEVENT( REC_MCU_RECSTATUS_NOTIF,			EV_MCUREC_BGN + 8 );

// ��¼����Ŀ���б��ϱ���TEqp + TMtAlias����¼����+ �ܵ������n(u32)+(����(s8*49)+��Id(u32)+�����ļ�����(u32))* n 
OSPEVENT( REC_MCU_FILEGROUPLIST_NOTIF,		EV_MCUREC_BGN + 14 );


//MCU������¼���¼��ͨ��״̬����, MCU->REC, ��Ϣ��ΪTEqp���ŵ���λ����Ϣͷ��
OSPEVENT( MCU_REC_GETRECORDCHNSTATUS_REQ,	EV_MCUREC_BGN + 15 );
//����¼����ŵ�״̬��REC->MCU����Ϣ��TEqp+TRecChnnlStatus
OSPEVENT( REC_MCU_GETRECORDCHNSTATUS_ACK,	EV_MCUREC_BGN + 16 );
//�ܾ�����¼����ŵ�״̬��REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_GETRECORDCHNSTATUS_NACK,	EV_MCUREC_BGN + 17 );

//MCU������¼�������ͨ��״̬����, MCU->REC, ��Ϣ��ΪTEqp���ŵ���λ����Ϣͷ��
OSPEVENT( MCU_REC_GETPLAYCHNSTATUS_REQ,		EV_MCUREC_BGN + 20 );
//����¼����ŵ�״̬��REC->MCU����Ϣ��TEqp+TRecChnnlStatus
OSPEVENT( REC_MCU_GETPLAYCHNSTATUS_ACK,		EV_MCUREC_BGN + 21 );
//�ܾ�����¼����ŵ�״̬��REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_GETPLAYCHNSTATUS_NACK,	EV_MCUREC_BGN + 22 );

//��ȡ¼����ȣ���Ϣ�壺�� (�ŵ���λ����Ϣͷ)
OSPEVENT( MCU_REC_GETRECPROG_CMD,           EV_MCUREC_BGN + 24 );

//��ȡ������ȣ���Ϣ�壺�� (�ŵ���λ����Ϣͷ)
OSPEVENT( MCU_REC_GETPLAYPROG_CMD,          EV_MCUREC_BGN + 26 );
//¼����쳣֪ͨ��Ϣ��REC->MCU����Ϣ��ΪTEqp���ŵ��źʹ�����λ����Ϣͷ
OSPEVENT( REC_MCU_EXCPT_NOTIF,				EV_MCUREC_BGN + 28 );

/////////////////
//������Ϣ

//MCU������ͣ��MCU->REC����Ϣ��ΪTEqp
OSPEVENT( MCU_REC_PAUSEPLAY_REQ,			EV_MCUREC_BGN + 35 );
//RECͬ����ͣ����REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_PAUSEPLAY_ACK,			EV_MCUREC_BGN + 36 );
//REC�ܾ���ͣ����REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_PAUSEPLAY_NACK,			EV_MCUREC_BGN + 37 );

//������������MCU->REC����Ϣ��ΪTEqp
OSPEVENT( MCU_REC_RESUMEPLAY_REQ,			EV_MCUREC_BGN + 40 );
//ͬ��������ţ�REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_RESUMEPLAY_ACK,			EV_MCUREC_BGN + 41 );
//�ܾ��������ţ�REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_RESUMEPLAY_NACK,			EV_MCUREC_BGN + 42 );

//ֹͣ��������MCU->REC����Ϣ��ΪTEqp
OSPEVENT( MCU_REC_STOPPLAY_REQ,				EV_MCUREC_BGN + 45 );
//ͬ��ֹͣ����Ӧ��,REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_STOPPLAY_ACK,				EV_MCUREC_BGN + 46 );
//�ܾ�ֹͣ����Ӧ��,REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_STOPPLAY_NACK,			EV_MCUREC_BGN + 47 );

//�����Կ����ʽ���ţ�MCU->REC����Ϣ��ΪTEqp+�������(u8)
OSPEVENT( MCU_REC_FFPLAY_REQ,				EV_MCUREC_BGN + 50 );
//ͬ������ʽ����Ӧ��REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_FFPLAY_ACK,				EV_MCUREC_BGN + 51 );
//�ܾ������ʽ����Ӧ��,REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_FFPLAY_NACK,				EV_MCUREC_BGN + 52 );

//�����Կ��˷�ʽ���ţ�MCU->REC����Ϣ��ΪTEqp+���˱���(u8)
OSPEVENT( MCU_REC_FBPLAY_REQ,				EV_MCUREC_BGN + 55 );
//ͬ����˷�ʽ����Ӧ��REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_FBPLAY_ACK,				EV_MCUREC_BGN + 56 );
//�ܾ����˷�ʽ����Ӧ��,REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_FBPLAY_NACK,				EV_MCUREC_BGN + 57 );

//ֹͣ¼������MCU->REC����Ϣ��ΪTEqp
OSPEVENT( MCU_REC_STOPREC_REQ,				EV_MCUREC_BGN + 85 );
//ͬ��ֹͣ¼��Ӧ��REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_STOPREC_ACK,				EV_MCUREC_BGN + 86 );
//�ܾ�ֹͣ¼��Ӧ��REC->MCU����Ϣ��ΪTEqp
OSPEVENT( REC_MCU_STOPREC_NACK,				EV_MCUREC_BGN + 87 );
//���̲���,ֹͣ¼������ļ�
OSPEVENT( REC_MCU_ADDVIDFILE_NTF,			EV_MCUREC_BGN + 88 );

//�ı�¼��ģʽ����, ��Ϣ��TMt �� u8( REC_MODE_NORMAL, REC_MODE_SKIPFRAME )
OSPEVENT( MCU_REC_CHANGERECMODE_REQ,		EV_MCUREC_BGN + 90 );
//ͬ�⣬MCU->MCS����Ϣ��Ϊ TMt 
OSPEVENT( REC_MCU_CHANGERECMODE_ACK,		EV_MCUREC_BGN + 91 );
//���ԣ�MCU->MCS����Ϣ��Ϊ TMt 
OSPEVENT( REC_MCU_CHANGERECMODE_NACK,		EV_MCUREC_BGN + 92 );

//MCU ����ɾ��¼����еļ�¼��MCU->REC ,��Ϣ��ΪTEqp+��¼��(0��β�ַ���)
OSPEVENT( MCU_REC_DELETERECORD_REQ,			EV_MCUREC_BGN + 93 );
//¼���ͬ�Ⲣ�ɹ�ɾ��¼���ļ�Ӧ��
//��Ϣ��ΪTEqp
OSPEVENT( REC_MCU_DELETERECORD_ACK,			EV_MCUREC_BGN + 94);
//¼����ܾ�ɾ��¼�� ��Ϣ��TEqp
OSPEVENT( REC_MCU_DELETERECORD_NACK,		EV_MCUREC_BGN + 95 );

//MCU������ļ�¼�ļ�, MCU->REC
//��Ϣ��ΪTEqp + u8(�ɼ�¼������) + �ɼ�¼�� + u8(�¼�¼������) + �¼�¼��
OSPEVENT( MCU_REC_RENAMERECORD_REQ,			EV_MCUREC_BGN + 96 );
//ͬ�⣬��Ϣ��ΪTEqp
OSPEVENT( REC_MCU_RENAMERECORD_ACK,			EV_MCUREC_BGN + 97 );
//���ԣ���Ϣ��ΪTEqp
OSPEVENT( REC_MCU_RENAMERECORD_NACK,		EV_MCUREC_BGN + 98 );

//����¼������ MCU->REC����Ϣ��ΪTEqp + u8(PUBLISH_LEVEL) + ¼����
OSPEVENT( MCU_REC_PUBLISHREC_REQ,			EV_MCUREC_BGN + 100 );
//ͬ�⣬REC->MCU����Ϣ��Ϊ TEqp
OSPEVENT( REC_MCU_PUBLISHREC_ACK,			EV_MCUREC_BGN + 101 );
//���ԣ�REC->MCU����Ϣ��Ϊ TEqp
OSPEVENT( REC_MCU_PUBLISHREC_NACK,			EV_MCUREC_BGN + 102 );

//ȡ������¼������ MCU->REC����Ϣ��ΪTEqp +  ¼����
OSPEVENT( MCU_REC_CANCELPUBLISHREC_REQ,		EV_MCUREC_BGN + 105 );
//ͬ�⣬REC->MCU����Ϣ��Ϊ TEqp
OSPEVENT( REC_MCU_CANCELPUBLISHREC_ACK,		EV_MCUREC_BGN + 106 );
//���ԣ�REC->MCU����Ϣ��Ϊ TEqp
OSPEVENT( REC_MCU_CANCELPUBLISHREC_NACK,	EV_MCUREC_BGN + 107 );
//¼�������ؼ�֡
OSPEVENT( REC_MCU_NEEDIFRAME_CMD,	        EV_MCUREC_BGN + 108 );
//���ý��ղ�����MCU->REC,��Ϣ�壺u8(stream number)+TDoublePayload+TMediaEncrypt
OSPEVENT( MCU_REC_SETRECV_NTF,				EV_MCUREC_BGN + 120 );
//����¼��Դ��BACKRTCP
/*OSPEVENT( MCU_REC_ADJUST_BACKRTCP_CMD,	        EV_MCUREC_BGN + 109 );*/


/*----------  MCU��REC��Ϣ�������(26101-26300)   ------------*/


/*----------  ����Ϊ��������Ϣ�Ŷ���(26301-26400)  -----------*/

//��������MCU����ע������MIXER->MCU����Ϣ��ΪTPeriEqpRegReq�ṹ
OSPEVENT( MIXER_MCU_REG_REQ,				EV_MCUMIXER_BGN );
//MCU����Ӧ��MCU->MIXER����Ϣ��ΪTPeriEqpRegAck + TPrsTimeSpan + u16(MTU size) + u8(IsSendRedundancy)
OSPEVENT( MCU_MIXER_REG_ACK,				EV_MCUMIXER_BGN + 1 );
//MCU�ܾ�Ӧ��MCU->MIXER������Ϣ��
OSPEVENT( MCU_MIXER_REG_NACK,				EV_MCUMIXER_BGN + 2 );
//������״̬֪ͨ��MIXER->MCU����Ϣ��ΪTPeriEqpStatus
OSPEVENT( MIXER_MCU_MIXERSTATUS_NOTIF,		EV_MCUMIXER_BGN + 3 );

//MCU��ʼ��������MCU->MIXER ��Ϣ��: TMixerStart+TDoublePayload+TCapSupportEx
OSPEVENT( MCU_MIXER_STARTMIX_REQ,			EV_MCUMIXER_BGN + 5 );
//ͬ�⿪ʼ����Ӧ��MIXER->MCU����Ϣ��: 1 byte(������Id)+ 1 byte(�������)
OSPEVENT( MIXER_MCU_STARTMIX_ACK,			EV_MCUMIXER_BGN + 6 );
//�ܾ���ʼ����Ӧ��MIXER->MCU������Ϣ��
OSPEVENT( MIXER_MCU_STARTMIX_NACK,			EV_MCUMIXER_BGN + 7 );

//������״̬֪ͨ��MIXER->MCU����Ϣ��ΪTMixerGrpStatus
OSPEVENT( MIXER_MCU_GRPSTATUS_NOTIF,		EV_MCUMIXER_BGN + 10 );

//MCUֹͣ��������MCU->MIXER����Ϣ��: 1 byte(������Id)
OSPEVENT( MCU_MIXER_STOPMIX_REQ,			EV_MCUMIXER_BGN + 15 );
//ͬ��ֹͣ����Ӧ��MIXER->MCU����Ϣ��: 1 byte(������Id)
OSPEVENT( MIXER_MCU_STOPMIX_ACK,			EV_MCUMIXER_BGN + 16 );
//�ܾ�ֹͣ����Ӧ��MIXER->MCU������Ϣ��
OSPEVENT( MIXER_MCU_STOPMIX_NACK,			EV_MCUMIXER_BGN + 17 );

//�����Ա����MCU->MIXER����Ϣ��:1 byte(������Id)+ TMixMember + 1 byte(��Ա����ԭʼ��ʽ)
//  m_tMember,m_tAddr,��Ч
OSPEVENT( MCU_MIXER_ADDMEMBER_REQ,			EV_MCUMIXER_BGN + 20 );
//�����ԱӦ����Ϣ��MIXER->MCU����Ϣ��: 1 byte(������Id)+ TMixMember
OSPEVENT( MIXER_MCU_ADDMEMBER_ACK,			EV_MCUMIXER_BGN + 21 );
//�����ԱӦ����Ϣ��MIXER->MCU������Ϣ��
OSPEVENT( MIXER_MCU_ADDMEMBER_NACK,			EV_MCUMIXER_BGN + 22 );


//ɾ����Ա����MCU->MIXER����Ϣ��:1 byte(������Id)+ TMixMember	
//  m_tMember, m_byGrpId ��Ч
OSPEVENT( MCU_MIXER_REMOVEMEMBER_REQ,		EV_MCUMIXER_BGN + 25 );
//ɾ����ԱӦ����Ϣ��MIXER->MCU����Ϣ��: 1 byte(������Id)+ TMixMember
OSPEVENT( MIXER_MCU_REMOVEMEMBER_ACK,		EV_MCUMIXER_BGN + 26 );
//ɾ����ԱӦ����Ϣ��MIXER->MCU������Ϣ��
OSPEVENT( MIXER_MCU_REMOVEMEMBER_NACK,		EV_MCUMIXER_BGN + 27 );

//����������Ա�ı�֪ͨ,MIXER->MCU ,��Ϣ�� u8    abyActiveMmb[MAXNUM_MIXER_GROUP]
//0��ʾ��Ч��Ա
OSPEVENT( MIXER_MCU_ACTIVEMMBCHANGE_NOTIF,	EV_MCUMIXER_BGN + 30 );

//���û�����Ա��MCU->MIXER����Ϣ��:1 byte(������Id)+ 192 byte(ÿ�ֽڶ�Ӧһ�ն�)	
OSPEVENT( MCU_MIXER_SETMEMBER_CMD,			EV_MCUMIXER_BGN + 32 );

//ǿ�Ƴ�Ա���� ,MCU->MIXER����Ϣ��:1 byte(������Id)+ TMixMember
OSPEVENT( MCU_MIXER_FORCEACTIVE_REQ,		EV_MCUMIXER_BGN + 35 );
//ǿ�Ƴ�Ա����Ӧ��MIXER->MCU����Ϣ��:1 byte(������Id)+ TMixMember
OSPEVENT( MIXER_MCU_FORCEACTIVE_ACK,		EV_MCUMIXER_BGN + 36 );
//ǿ�Ƴ�Ա�����ܾ���MIXER->MCU������Ϣ��
OSPEVENT( MIXER_MCU_FORCEACTIVE_NACK,		EV_MCUMIXER_BGN + 37 );

//ȡ����Աǿ�ƻ��� ,MCU->MIXER����Ϣ��:1 byte(������Id)+ TMixMember
OSPEVENT( MCU_MIXER_CANCELFORCEACTIVE_REQ,	EV_MCUMIXER_BGN + 40 );
//ȡ����Աǿ�ƻ���Ӧ��MIXER->MCU����Ϣ��:1 byte(������Id)+ TMixMember
OSPEVENT( MIXER_MCU_CANCELFORCEACTIVE_ACK,	EV_MCUMIXER_BGN + 41 );
//ȡ����Աǿ�ƻ����ܾ���MIXER->MCU������Ϣ��
OSPEVENT( MIXER_MCU_CANCELFORCEACTIVE_NACK,	EV_MCUMIXER_BGN + 42 );

//����ĳͨ��������Ϣ��MCU->MIXER����Ϣ��1 byte(������Id)+ TMixMember
//  m_tMember, m_byGrpId ,m_byVolume ��Ч
OSPEVENT( MCU_MIXER_SETCHNNLVOL_CMD,		EV_MCUMIXER_BGN + 45 );
//ĳͨ������֪ͨ��Ϣ��MIXER->MCU����Ϣ��ΪTMixMember
OSPEVENT( MIXER_MCU_CHNNLVOL_NOTIF,			EV_MCUMIXER_BGN + 50 );

//���û������������Ϣ��MCU->MIX����Ϣ��: 1 byte(������Id) + 1 byte(�������)
OSPEVENT( MCU_MIXER_SETMIXDEPTH_REQ,		EV_MCUMIXER_BGN + 55 );
//���û������Ӧ����Ϣ��MIXER->MCU����Ϣ��: 1 byte(������Id) + 1 byte(�������)
OSPEVENT( MIXER_MCU_SETMIXDEPTH_ACK,		EV_MCUMIXER_BGN + 56 );
//���û�����Ⱦܾ���Ϣ��MIXER->MCU��
OSPEVENT( MIXER_MCU_SETMIXDEPTH_NACK,		EV_MCUMIXER_BGN + 57 );

//MCU�����������Ƿ�Ҫ��������֪ͨ,��Ϣ��: 1 byte(������Id) + 1 byte(0:����,1��)
OSPEVENT( MCU_MIXER_SEND_NOTIFY,			EV_MCUMIXER_BGN + 60 );

//��������������ʱ����ʱ����Ϣ, MCU->MIXER, ��Ϣ��: 1 byte(������Id) + 4 byte(u32 ����ʱ�� ������)
OSPEVENT( MCU_MIXER_VACKEEPTIME_CMD,		EV_MCUMIXER_BGN + 65 );

//������ʱ����,��Ϣ�壺u8��������id�� �� u16�����룩
OSPEVENT( MCU_MIXER_CHANGEMIXDELAY_CMD,		EV_MCUMIXER_BGN + 66 );

// stop mixer map, zgc, 2007/04/24
OSPEVENT( MCU_MIXER_STOPEQP_CMD,			EV_MCUMIXER_BGN + 67 );
// start mixer map, zgc, 2007/04/26
OSPEVENT( MCU_MIXER_STARTEQP_CMD,			EV_MCUMIXER_BGN + 68 );

//tianzhiyong 2010/05/19  ���ӿ����͹ر�EAPU����������������������
//����EAPU���������������������MCU->MIXER��
//��Ϣ��: ������������ʱΪTMixerStart+TDoublePayload+TCapSupportEx ����Ϊ��
OSPEVENT( MCU_MIXER_STARTVAC_CMD,		EV_MCUMIXER_BGN + 69 );
//�ر�EAPU���������������������MCU->MIXER����Ϣ��: ��
OSPEVENT( MCU_MIXER_STOPVAC_CMD,		EV_MCUMIXER_BGN + 70 );
/*----------  MCU�ͻ�������Ϣ�Ŷ������(26301-26400)  -----------*/


/*---------  MCU��TVWALL��ҵ����Ϣ�Ŷ���(26401-26440)  ----------*/
/*---------  TVWALL��Ϣȫ�����ã�����������Ϣ����  ----------*/

//����ǽ��MCU����ע������PERIEQP->MCU����Ϣ��ΪTPeriEqpRegReq�ṹ
OSPEVENT( TVWALL_MCU_REG_REQ,				EV_MCUTVWALL_BGN );
//MCU����Ӧ��MCU->PERIEQP������Ϣ��
OSPEVENT( MCU_TVWALL_REG_ACK,				EV_MCUTVWALL_BGN + 1 );
//MCU�ܾ�Ӧ��MCU->PERIEQP������Ϣ��
OSPEVENT( MCU_TVWALL_REG_NACK,				EV_MCUTVWALL_BGN + 2 );

//MCU�����跢����ʼ������Ƶͼ���������ͨ��
//������������Ϣͷ�У�MCU->PERIEQP��
//    ��Ϣ��ΪTTvWallStartPlay + TDoublePayload(video) + TDoublePayload(audio) + TCapSupportEx
OSPEVENT( MCU_TVWALL_START_PLAY_REQ,		EV_MCUTVWALL_BGN + 5 );
//����ǽ����Ӧ��PERIEQP->MCU������Ϣ��
OSPEVENT( TVWALL_MCU_START_PLAY_ACK,		EV_MCUTVWALL_BGN + 6 );
//����ǽ�ܾ�Ӧ��PERIEQP->MCU������Ϣ��	
OSPEVENT( TVWALL_MCU_START_PLAY_NACK,		EV_MCUTVWALL_BGN + 7 );
//��MCU��Ƶ����Ƶ���ݸı�֪ͨ��MCU->PERIEQP����Ϣ��ΪTSwitchInfo
//OSPEVENT( MCU_PERIEQP_MCUSWITCH_NOTIF,	EV_MCUTVWALL_BGN + 8 );

//MCU�����ǽ����ֹͣ������Ƶͼ���������ͨ��
//������������Ϣͷ�У�MCU->PERIEQP������Ϣ��
OSPEVENT( MCU_TVWALL_STOP_PLAY_REQ,			EV_MCUTVWALL_BGN + 10 );
//����ǽ����Ӧ��PERIEQP->MCU������Ϣ��
OSPEVENT( TVWALL_MCU_STOP_PLAY_ACK,			EV_MCUTVWALL_BGN + 11 );
//����ǽ�ܾ�Ӧ��PERIEQP->MCU������Ϣ��
OSPEVENT( TVWALL_MCU_STOP_PLAY_NACK,		EV_MCUTVWALL_BGN + 12 );


//MCU��������ǽ��ѯ״̬����MCU->PERIEQP������Ϣ��
OSPEVENT( MCU_TVWALL_GETSTATUS_REQ,			EV_MCUTVWALL_BGN + 15 );	
//�ɹ�Ӧ��PERIEQP->MCU����Ϣ��ΪTPeriEqpStatus
OSPEVENT( TVWALL_MCU_GETSTATUS_ACK,			EV_MCUTVWALL_BGN + 16 );
//�ܾ�Ӧ��PERIEQP->MCU������Ϣ��
OSPEVENT( TVWALL_MCU_GETSTATUS_NACK,		EV_MCUTVWALL_BGN + 17 );
//����ǽ״̬�ı�֪ͨ��PERIEQP->MCU����Ϣ��ΪTPeriEqpStatus
OSPEVENT( TVWALL_MCU_STATUS_NOTIF,			EV_MCUTVWALL_BGN + 18 );

//����������Դ��Ӧ,֧�ֶ�����裨��������vmp��bas����MediaClient��MCU����Ϣ�壺TPreOccurEqpInfo + TEqpHandleInfo
OSPEVENT( RES_MCU_APPLYEQPRES_RSP,			EV_MCUTVWALL_BGN + 19 );

//�˿ڻ���������Դ�ص�ͳһ��Ϣ��MediaClient��MCU����Ϣ�壺u8(�ص�����)+u8(CBFunId)+u16(ErrCode)+u16(BufLen)+u8*wBufLen(pbyBuf)
OSPEVENT( RES_MCU_EQPRES_RSP,				EV_MCUTVWALL_BGN + 20 );
//�˿ڻ���˿���Դ�ص�ͳһ��Ϣ��MediaClient��MCU����Ϣ�壺u8(�ص�����)+u8(CBFunId)+u16(ErrCode)+u16(BufLen)+u8*wBufLen(pbyBuf)
OSPEVENT( RES_MCU_DECNECRES_RSP,				EV_MCUTVWALL_BGN + 21 );
//ý��ͻ�������Դ�ϱ���Ϣ��MediaClient��MCU����Ϣ�壺u8(�ص�����)+u8(CBFunId)+u16(ErrCode)+u16(BufLen)+u8*wBufLen(pbyBuf)
OSPEVENT( RES_MCU_RESCONNECT_NTF,				EV_MCUTVWALL_BGN + 22 );

/*---------- MCU��TVWALLҵ����Ϣ������� (26401-26440) ----------*/

/*--------  MCU��HDUҵ����Ϣ�Ŷ��� (26441-26500) --------------*/
//HDUע��
OSPEVENT( HDU_MCU_REG_REQ,			        EV_MCUHDU_BGN );
OSPEVENT( MCU_HDU_REG_ACK,			        EV_MCUHDU_BGN + 1 );
OSPEVENT( MCU_HDU_REG_NACK,			        EV_MCUHDU_BGN + 2 );

//ͨ����ʼ����
OSPEVENT( MCU_HDU_START_PLAY_REQ,		    EV_MCUHDU_BGN + 3 );
OSPEVENT( HDU_MCU_START_PLAY_ACK,		    EV_MCUHDU_BGN + 4 );
OSPEVENT( HDU_MCU_START_PLAY_NACK,		    EV_MCUHDU_BGN + 5 );

//ͨ��ֹͣ����
OSPEVENT( MCU_HDU_STOP_PLAY_REQ,		    EV_MCUHDU_BGN + 6 );
OSPEVENT( HDU_MCU_STOP_PLAY_ACK,		    EV_MCUHDU_BGN + 7 );
OSPEVENT( HDU_MCU_STOP_PLAY_NACK,		    EV_MCUHDU_BGN + 8 );

//��������״̬
OSPEVENT( MCU_HDU_GETSTATUS_REQ,		    EV_MCUHDU_BGN + 9 );
OSPEVENT( HDU_MCU_GETSTATUS_ACK,		    EV_MCUHDU_BGN + 10 );
OSPEVENT( HDU_MCU_GETSTATUS_NACK,		    EV_MCUHDU_BGN + 11 );
OSPEVENT( HDU_MCU_STATUS_NOTIF,			    EV_MCUHDU_BGN + 12 );

//--------����ؼ�֡-------
OSPEVENT( HDU_MCU_NEEDIFRAME_CMD,		    EV_MCUHDU_BGN + 13 );
//--------��������---------
//��Ϣ�壺FIXME���漰chnlid,vol
OSPEVENT( MCU_HDU_SETCHNNLVOL_CMD,			EV_MCUHDU_BGN + 14 );
//ĳͨ������֪ͨ��Ϣ
OSPEVENT( HDU_MCU_CHNNLVOL_NOTIF,			EV_MCUHDU_BGN + 15 );
//ͨ��״̬֪ͨ
OSPEVENT( HDU_MCU_CHNNLSTATUS_NOTIF,		EV_MCUHDU_BGN + 16 ); 

//�ı����������ʽ����ʾ����֪ͨ
OSPEVENT( MCU_HDU_CHANGEMODEPORT_NOTIF,		EV_MCUHDU_BGN + 17 ); 

// ����������С����
OSPEVENT( MCU_HDU_CHANGEVOLUME_CMD,		    EV_MCUHDU_BGN + 19 ); 

// ���ģʽ
OSPEVENT( MCU_HDU_CHANGEMODE_CMD,		    EV_MCUHDU_BGN + 20 ); 

//ͨ��������ʾ���Ա��
OSPEVENT( MCU_HDU_CHANGEPLAYPOLICY_NOTIF,   EV_MCUHDU_BGN + 21 ); 


//HDUͨ��ģʽ�л�(�໭���ͨ��)
OSPEVENT( MCU_HDU_CHGHDUVMPMODE_REQ,		EV_MCUHDU_BGN + 22 ); 
OSPEVENT( HDU_MCU_CHGHDUVMPMODE_ACK,		EV_MCUHDU_BGN + 23 ); 
OSPEVENT( HDU_MCU_CHGHDUVMPMODE_NACK,		EV_MCUHDU_BGN + 24 ); 

//ˢ����Ƶ�غ�
OSPEVENT( MCU_HDU_FRESHAUDIO_PAYLOAD_CMD, 	EV_MCUHDU_BGN + 25 );  


/*----------MCU��HDUҵ����Ϣ������� (26441-26500) ------------*/

/*--------  MCU��DCS�Ựҵ����Ϣ�Ŷ��� (26501-26600) -----------*/

//MCU����DCS�������飬MCU->DCSSSN����Ϣ��Ϊ TDataConfInfo
OSPEVENT( MCU_DCSSSN_CREATECONF_REQ,		EV_MCUDCS_BGN  );
//DCSSSN ��������ͬ��Ӧ��DCSSSN->MCU����Ϣ��Ϊ NULL
OSPEVENT( DCSSSN_MCU_CREATECONF_ACK,		EV_MCUDCS_BGN + 1 );
//DCSSSN ��������ܾ�Ӧ��DCSSSN->MCU����Ϣ��Ϊ u8(byErrorId)
OSPEVENT( DCSSSN_MCU_CREATECONF_NACK,		EV_MCUDCS_BGN + 2 );
//DCSSSN �������ͻ���֪ͨ,DCSSSN->MCU,��Ϣ��Ϊ TDataConfInfo
OSPEVENT( DCSSSN_MCU_CONFCREATED_NOTIF,		EV_MCUDCS_BGN +	3 );

//MCU����DCS�������飬MCU->DCSSSN����Ϣ��Ϊ NULL
OSPEVENT( MCU_DCSSSN_RELEASECONF_REQ,		EV_MCUDCS_BGN +	4 );
//DCSSSN ��������ͬ��Ӧ��DCSSSN->MCU����Ϣ��Ϊ NULL
OSPEVENT( DCSSSN_MCU_RELEASECONF_ACK,		EV_MCUDCS_BGN + 5 );
//DCSSSN ��������ܾ�Ӧ��DCSSSN->MCU����Ϣ��Ϊ u8(byErrorId)
OSPEVENT( DCSSSN_MCU_RELEASECONF_NACK,		EV_MCUDCS_BGN + 6 );
//DCSSSN �������ͻ������֪ͨ,DCSSSN->MCU,��Ϣ��Ϊ NULL
OSPEVENT( DCSSSN_MCU_CONFRELEASED_NOTIF,	EV_MCUDCS_BGN +	7 );


//MCU����DCS�����նˣ�MCU->DCSSSN����Ϣ��Ϊ TDataMtInfo
OSPEVENT( MCU_DCSSSN_ADDMT_REQ,				EV_MCUDCS_BGN +	10 );
//DCSSSN �����ն�ͬ��Ӧ��DCSSSN->MCU����Ϣ��Ϊ u8(byMtId)
OSPEVENT( DCSSSN_MCU_ADDMT_ACK,				EV_MCUDCS_BGN + 11 );
//DCSSSN �����ն˾ܾ�Ӧ��DCSSSN->MCU����Ϣ��Ϊ u8(byMtId) + u8(byErrorId)
OSPEVENT( DCSSSN_MCU_ADDMT_NACK,			EV_MCUDCS_BGN + 12 );
//DCSSSN �ն˼���֪ͨ��DCSSSN->MCU����Ϣ��Ϊ TDataMtInfo
OSPEVENT( DCSSSN_MCU_MTJOINED_NOTIF,		EV_MCUDCS_BGN + 13 );


//MCU����DCSɾ���նˣ�MCU->DCSSSN����Ϣ��Ϊ u8(byMtId)
OSPEVENT( MCU_DCSSSN_DELMT_REQ,				EV_MCUDCS_BGN +	14 );
//DCSSSN ɾ���ն�ͬ��Ӧ��DCSSSN->MCU����Ϣ��Ϊ u8(byMtId)
OSPEVENT( DCSSSN_MCU_DELMT_ACK,				EV_MCUDCS_BGN + 15 );
//DCSSSN ɾ���ն˾ܾ�Ӧ��DCSSSN->MCU����Ϣ��Ϊ u8(byMtId) + u8(byErrorId)
OSPEVENT( DCSSSN_MCU_DELMT_NACK,			EV_MCUDCS_BGN + 16 );
//DCSSSN �ն����(����, ���Ǳ�ɾ��)֪ͨ��DCSSSN->MCU����Ϣ��Ϊ u8(byMtId)
OSPEVENT( DCSSSN_MCU_MTLEFT_NOTIF,			EV_MCUDCS_BGN + 17 );

/*------    MCU��DCS�Ựҵ����Ϣ�Ŷ������ (26501-26600)   ------*/


/*----------    MCU��BASҵ����Ϣ�Ŷ��� (26601-26700)   ----------*/

//������������MCU����ע������BAS->MCU����Ϣ��ΪTPeriEqpRegReq�ṹ
OSPEVENT( BAS_MCU_REG_REQ,					EV_MCUBAS_BGN );
//MCU����Ӧ��MCU->BAS����Ϣ��Ϊ��Ϣ��ΪTPeriEqpRegAck + TPrsTimeSpan + u16(MTU size)
OSPEVENT( MCU_BAS_REG_ACK,					EV_MCUBAS_BGN + 1 );
//MCU�ܾ�Ӧ��MCU->BAS������Ϣ��
OSPEVENT( MCU_BAS_REG_NACK,					EV_MCUBAS_BGN + 2 );
//������״̬֪ͨ��BAS->MCU����Ϣ��ΪTPeriEqpStatus
OSPEVENT( BAS_MCU_BASSTATUS_NOTIF,			EV_MCUBAS_BGN + 3 );

//MCU������������MCU->BAS����Ϣ��ΪTAdaptParam+TMediaEncrypt(video)+TDoublePayload(video)+
//                       TMediaEncrypt(audio)+TDoublePayload(Audio)�ṹ+TCapSupportEx,
//     ҵ����Ϣ��ͨ��������ָ��ͨ�� 1�� MAXNUM_RTPHDR_CHNNL ΪRTPͷ����ͨ��������Ϊ����ͨ��
//apu2 ��Ƶ��������ʱ����Ϣ��ΪTAudAdaptParam*byOutNum+TMediaEncrypt+TDoublePayload
OSPEVENT( MCU_BAS_STARTADAPT_REQ,			EV_MCUBAS_BGN + 5 );
//��������Ӧ����Ϣ��BAS->MCU����Ϣ��: TEqp
OSPEVENT( BAS_MCU_STARTADAPT_ACK,			EV_MCUBAS_BGN + 6 );
//��������Ӧ����Ϣ��BAS->MCU����Ϣ��: TEqp
OSPEVENT( BAS_MCU_STARTADAPT_NACK,			EV_MCUBAS_BGN + 7 );

//MCUֹͣ�������MCU->BAS������Ϣ�壬ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( MCU_BAS_STOPADAPT_REQ,			EV_MCUBAS_BGN + 10 );
//ֹͣ����Ӧ����Ϣ��BAS->MCU������Ϣ��
OSPEVENT( BAS_MCU_STOPADAPT_ACK,			EV_MCUBAS_BGN + 11 );
//ֹͣ����Ӧ����Ϣ��BAS->MCU������Ϣ��
OSPEVENT( BAS_MCU_STOPADAPT_NACK,			EV_MCUBAS_BGN + 12 );

//MCU�ı������������MCU->BAS����Ϣ��ΪTAdaptParam�ṹ,ҵ����Ϣ��ͨ��������ָ��ͨ��
//TAdaptParam �е�����Ƶ������Ч
OSPEVENT( MCU_BAS_SETADAPTPARAM_CMD,		EV_MCUBAS_BGN + 15 );
OSPEVENT( MCU_BAS_CHANGEAUDPARAM_REQ,		EV_MCUBAS_BGN + 16 );
OSPEVENT( BAS_MCU_CHANGEAUDPARAM_ACK,		EV_MCUBAS_BGN + 17 );
OSPEVENT( BAS_MCU_CHANGEAUDPARAM_NACK,		EV_MCUBAS_BGN + 18 );
//����������ͨ��״̬֪ͨ��BAS-> MCU ����Ϣ��BYTE(����ID) + TBasChnStatus,ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( BAS_MCU_CHNNLSTATUS_NOTIF,		EV_MCUBAS_BGN + 20 );

OSPEVENT( MCU_BAS_FASTUPDATEPIC_CMD,		EV_MCUBAS_BGN + 25 );
OSPEVENT( BAS_MCU_NEEDIFRAME_CMD,			EV_MCUBAS_BGN + 26 );

// ����BAS��Ϣ, zgc, 2008-08-02
//MCU����������������MCU->HDBAS
//��Ϣ�壺THDAdaptParam+THDAdaptParam+TMediaEncrypt(video)+TDoublePayload(video)
//        ҵ����Ϣ��ͨ��������ָ��ͨ��, ����˫��ͨ��������TAdaptParam��ȫһ��
OSPEVENT( MCU_HDBAS_STARTADAPT_REQ,         EV_MCUBAS_BGN + 30 );
//��������Ӧ����Ϣ��HDBAS->MCU����Ϣ��: TEqp, ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( HDBAS_MCU_STARTADAPT_ACK,         EV_MCUBAS_BGN + 31 );
//��������Ӧ����Ϣ��HDBAS->MCU����Ϣ��: TEqp, ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( HDBAS_MCU_STARTADAPT_NACK,        EV_MCUBAS_BGN + 32 );
//������״̬֪ͨ��HDBAS->MCU����Ϣ��: TPeriEqpStatus
OSPEVENT( HDBAS_MCU_BASSTATUS_NOTIF,		EV_MCUBAS_BGN + 33 );
//����ˢ��ͨ��֪ͨ
OSPEVENT( HDBAS_MCU_REFRESHBAS_NOTIF,		EV_MCUBAS_BGN + 34 );


//MCUֹͣ�������MCU->HDBAS������Ϣ�壬ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( MCU_HDBAS_STOPADAPT_REQ,          EV_MCUBAS_BGN + 35 );
//ֹͣ����Ӧ����Ϣ��HDBAS->MCU������Ϣ��
OSPEVENT( HDBAS_MCU_STOPADAPT_ACK,          EV_MCUBAS_BGN + 36 );
//ֹͣ����Ӧ����Ϣ��HDBAS->MCU������Ϣ��
OSPEVENT( HDBAS_MCU_STOPADAPT_NACK,         EV_MCUBAS_BGN + 37 );

//��Ϣ�壺THDAdaptParam+THDAdaptParam+TMediaEncrypt(video)+TDoublePayload(video)
//        ҵ����Ϣ��ͨ��������ָ��ͨ��, ����˫��ͨ��������TAdaptParam��ȫһ��
OSPEVENT( MCU_HDBAS_SETADAPTPARAM_CMD,		EV_MCUBAS_BGN + 40 );
//����������ͨ��״̬֪ͨ��HDBAS-> MCU ����Ϣ��: BYTE(ͨ������) + THDBasVidChnStatus/THDBasDVidChnStatus,ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( HDBAS_MCU_CHNNLSTATUS_NOTIF,		EV_MCUBAS_BGN + 45 );

//��Ƶ������ͨ��״̬֪ͨ AUDBAS->MCU����Ϣ��u8(�������ͣ�APU2_BAS_CHN) + TAudBasChnStatus
OSPEVENT( AUDBAS_MCU_CHNNLSTATUS_NOTIF,		EV_MCUBAS_BGN + 46 );

//HDBas����������޸�
//��Ϣ�壺THDAdaptParam+THDAdaptParam+TMediaEncrypt(video)+TDoublePayload(video)
//        ҵ����Ϣ��ͨ��������ָ��ͨ��, ����˫��ͨ��������TAdaptParam��ȫһ��
OSPEVENT( MCU_HDBAS_CHANGEPARAM_REQ,		EV_MCUBAS_BGN + 50 );
OSPEVENT( MCU_HDBAS_CHANGEPARAM_ACK,		EV_MCUBAS_BGN + 51 );
OSPEVENT( MCU_HDBAS_CHANGEPARAM_NACK,		EV_MCUBAS_BGN + 52 );

/*---------- MCU��BASҵ����Ϣ������� (26601-26700) ----------*/


/*----------    ����Ϊ���渴������Ϣ(26701-26800)   ----------*/

//VMP��MCUע������VMP->MCU����Ϣ��ΪTPeriEqpRegReq�ṹ
OSPEVENT( VMP_MCU_REGISTER_REQ,				EV_MCUVMP_BGN );
//MCU��VMPע��ȷ��,MCU->VMP����Ϣ��ΪTPeriEqpRegAck + TPrsTimeSpan + u16(MTU size)��
OSPEVENT( MCU_VMP_REGISTER_ACK,				EV_MCUVMP_BGN + 1 );
//MCU��VMPע��ܾ�,MCU->VMP������Ϣ��
OSPEVENT( MCU_VMP_REGISTER_NACK,			EV_MCUVMP_BGN + 2 );
//VMP��MCU��״̬�ϱ�,VMP->MCU����Ϣ��:TPeriEqpStatus
OSPEVENT( VMP_MCU_VMPSTATUS_NOTIF,			EV_MCUVMP_BGN + 3 );

//MCU���û���ϳɷ��, ��Ϣ��:TVmpStyleCfgInfo
OSPEVENT( MCU_VMP_SETSTYLE_CMD,				EV_MCUVMP_BGN + 4 );
//MCU����VMPֹͣMAP, ��Ϣ��: ��, zgc, 2007/04/24
OSPEVENT( MCU_VMP_STOPMAP_CMD,				EV_MCUVMP_BGN + 5 );
//MCU���������޸ķ���IP��ַ, ��Ϣ�壺u32
OSPEVENT( MCU_EQP_MODSENDADDR_CMD,          EV_MCUVMP_BGN + 6 );

//MCU����VMP��ʼ����,MCU->VMP����Ϣ��ΪCKDVVMPParam+CKDVVMPParam+u8(�Ƿ�prs)+TVmpStyleCfgInfo+TCapSupportEx
OSPEVENT( MCU_VMP_STARTVIDMIX_REQ,			EV_MCUVMP_BGN + 11 );
//VMP��MCU��ʼ����ȷ��,VMP->MCU������Ϣ��
OSPEVENT( VMP_MCU_STARTVIDMIX_ACK,			EV_MCUVMP_BGN + 12 );
//VMP��MCU��ʼ�����ܾ�,VMP->MCU������Ϣ��
OSPEVENT( VMP_MCU_STARTVIDMIX_NACK,			EV_MCUVMP_BGN + 13 );
//VMP��MCU�Ŀ�ʼ�����VMP->MCU,��Ϣ���
OSPEVENT( VMP_MCU_STARTVIDMIX_NOTIF,		EV_MCUVMP_BGN + 14 );

//MCU����VMPֹͣ����,MCU->VMP������Ϣ��
OSPEVENT( MCU_VMP_STOPVIDMIX_REQ,			EV_MCUVMP_BGN + 21 );
//VMP��MCUֹͣ����ȷ��,VMP->MCU������Ϣ��
OSPEVENT( VMP_MCU_STOPVIDMIX_ACK,			EV_MCUVMP_BGN + 22 );
//VMP��MCUֹͣ�����ܾ�,VMP->MCU������Ϣ��
OSPEVENT( VMP_MCU_STOPVIDMIX_NACK,			EV_MCUVMP_BGN + 23 );
//VMP��MCU��ֹͣ�����VMP->MCU,��Ϣ���
OSPEVENT( VMP_MCU_STOPVIDMIX_NOTIF,			EV_MCUVMP_BGN + 24 );

//MCU����VMP�ı临�ϲ���,MCU->VMP����Ϣ��ΪCKDVVMPParam+CKDVVMPParam+u8(�Ƿ�prs)+TVmpStyleCfgInfo
OSPEVENT( MCU_VMP_CHANGEVIDMIXPARAM_CMD,	EV_MCUVMP_BGN + 31 );
//VMP��MCU�ı临�ϲ���ȷ��,VMP->MCU������Ϣ��
OSPEVENT( VMP_MCU_CHANGEVIDMIXPARAM_ACK,	EV_MCUVMP_BGN + 32 );
//VMP��MCU�ı临�ϲ����ܾ�,VMP->MCU������Ϣ��
OSPEVENT( VMP_MCU_CHANGEVIDMIXPARAM_NACK,	EV_MCUVMP_BGN + 33 );
//VMP��MCU��״̬�ı�����VMP->MCU,��Ϣ��CKDVVMPParam�ṹ
OSPEVENT( VMP_MCU_CHANGESTATUS_NOTIF,		EV_MCUVMP_BGN + 34 );

OSPEVENT( MCU_VMP_UPDATAVMPENCRYPTPARAM_REQ,EV_MCUVMP_BGN + 35 );

OSPEVENT( MCU_VMP_FASTUPDATEPIC_CMD,		EV_MCUVMP_BGN + 36 );

//MCU�Ժϳ�ͨ��LOG�Ĳ�������, ��Ϣ�壺u8( 0 - ȡ��LOG�� 1 - ����LOG )
OSPEVENT( MCU_VMP_CHNNLLOGOPR_CMD,          EV_MCUVMP_BGN + 37 );
//MCU ��VMPĳͨ������̨����� ��Ϣ�壺[s8 * aliasname + byChlIdx] * ͨ����
OSPEVENT( MCU_VMP_CHANGEMEMALIAS_CMD,		EV_MCUVMP_BGN + 38 );

//MCU����VMP���͸��ϲ���,MCU->VMP������Ϣ��
OSPEVENT( MCU_VMP_GETVIDMIXPARAM_REQ,		EV_MCUVMP_BGN + 41 );
//VMP��MCU���͸��ϲ���ȷ��,VMP->MCU����Ϣ��ΪCKDVVMPParam�ṹ
OSPEVENT( VMP_MCU_GETVIDMIXPARAM_ACK,		EV_MCUVMP_BGN + 42 );
//VMP��MCU���͸��ϲ����ܾ�,VMP->MCU������Ϣ��
OSPEVENT( VMP_MCU_GETVIDMIXPARAM_NACK,		EV_MCUVMP_BGN + 43 );
//����VMP��ʼ��
OSPEVENT( EV_VMP_INIT,	  	  				EV_MCUVMP_BGN + 50 );
//��ʾ���е�ͳ�ƺ�״̬
OSPEVENT( EV_VMP_DISPLAYALL,	  			EV_MCUVMP_BGN + 60 );
//��ʾVMP�ڲ��ĺϳɲ���
OSPEVENT( EV_VMP_SHOWPARAM,					EV_MCUVMP_BGN + 61 );
//MCU����VMP���͸��ϲ���,MCU->VMP����Ϣ��u16�����ʣ�CServMsg.SetChnIndex()[1 or 2]
OSPEVENT( MCU_VMP_SETCHANNELBITRATE_REQ,	EV_MCUVMP_BGN + 65 );
//VMP��MCU���͸��ϲ���ȷ��,VMP->MCU����Ϣ��ΪCKDVVMPParam�ṹ
OSPEVENT( VMP_MCU_SETCHANNELBITRATE_ACK,	EV_MCUVMP_BGN + 66 );
//VMP��MCU���͸��ϲ����ܾ�,VMP->MCU������Ϣ��
OSPEVENT( VMP_MCU_SETCHANNELBITRATE_NACK,	EV_MCUVMP_BGN + 67 );
//����I֡,CServMsg.GetChnIndexΪ�����ͨ����
OSPEVENT( VMP_MCU_NEEDIFRAME_CMD,			EV_MCUVMP_BGN + 70 );
//��������Ƿ��������VMP�ڲ�ʹ�ã���Ϣ�壺u8: 0�����ã�1����
OSPEVENT( EV_VMP_BACKBOARD_OUT_CMD,         EV_MCUVMP_BGN + 71 );
//����VMP֡��
OSPEVENT( EV_VMP_SETFRAMERATE_CMD,			EV_MCUVMP_BGN + 72 );
//MCU֪ͨVMP���ӻ�ɾ��ͨ��(������ͣ��Ӧͨ���Ľ���)�����Ϣ�壺u8: channelidx, u8 byAdd
OSPEVENT( MCU_VMP_ADDREMOVECHNNL_CMD,		EV_MCUVMP_BGN + 73 );
//MCU֪ͨVMPͣ��ĳһ���ͨ�������Ϣ�壺u8 chnnelIdx,u8 byActive
OSPEVENT( MCU_VMP_STARTSTOPCHNNL_CMD,		EV_MCUVMP_BGN + 74 );


/*----------    ���渴������Ϣ�������(26701-26800)  ----------*/


/*----------     ����Ϊ���ϵ���ǽ��Ϣ(26701-26800)   ----------*/

//VMP��MCUע������VMP->MCU����Ϣ��ΪTPeriEqpRegReq�ṹ
OSPEVENT(VMPTW_MCU_REGISTER_REQ,			EV_MCUVMPTW_BGN);
//MCU��VMPע��ȷ��,MCU->VMP������Ϣ��
OSPEVENT(MCU_VMPTW_REGISTER_ACK,            EV_MCUVMPTW_BGN + 1 );
//MCU��VMPע��ܾ�,MCU->VMP������Ϣ��
OSPEVENT(MCU_VMPTW_REGISTER_NACK,           EV_MCUVMPTW_BGN + 2 );
//VMP��MCU��״̬�ϱ�,VMP->MCU����Ϣ��:TPeriEqpStatus
OSPEVENT(VMPTW_MCU_VMPTWSTATUS_NOTIF,       EV_MCUVMPTW_BGN + 3 );

//MCU����VMP��ʼ����,MCU->VMP����Ϣ��ΪCKDVVMPParam�ṹ+u8(needprs)+TCapSupportEx
OSPEVENT(MCU_VMPTW_STARTVIDMIX_REQ,         EV_MCUVMPTW_BGN + 11 );
//VMP��MCU��ʼ����ȷ��,VMP->MCU������Ϣ��
OSPEVENT(VMPTW_MCU_STARTVIDMIX_ACK,         EV_MCUVMPTW_BGN + 12 );
//VMP��MCU��ʼ�����ܾ�,VMP->MCU������Ϣ��
OSPEVENT(VMPTW_MCU_STARTVIDMIX_NACK,        EV_MCUVMPTW_BGN + 13 );
//VMP��MCU�Ŀ�ʼ�����VMP->MCU,��Ϣ���
OSPEVENT(VMPTW_MCU_STARTVIDMIX_NOTIF,       EV_MCUVMPTW_BGN + 14 );

//MCU����VMPֹͣ����,MCU->VMP������Ϣ��
OSPEVENT(MCU_VMPTW_STOPVIDMIX_REQ,          EV_MCUVMPTW_BGN + 21 );
//VMP��MCUֹͣ����ȷ��,VMP->MCU������Ϣ��
OSPEVENT(VMPTW_MCU_STOPVIDMIX_ACK,          EV_MCUVMPTW_BGN + 22 );
//VMP��MCUֹͣ�����ܾ�,VMP->MCU������Ϣ��
OSPEVENT(VMPTW_MCU_STOPVIDMIX_NACK,         EV_MCUVMPTW_BGN + 23 );
//VMP��MCU��ֹͣ�����VMP->MCU,��Ϣ���
OSPEVENT(VMPTW_MCU_STOPVIDMIX_NOTIF,        EV_MCUVMPTW_BGN + 24 );

//MCU����VMP�ı临�ϲ���,MCU->VMP����Ϣ��ΪCKDVVMPParam�ṹ
OSPEVENT(MCU_VMPTW_CHANGEVIDMIXPARAM_REQ,   EV_MCUVMPTW_BGN + 31 );
//VMP��MCU�ı临�ϲ���ȷ��,VMP->MCU������Ϣ��
OSPEVENT(VMPTW_MCU_CHANGEVIDMIXPARAM_ACK,   EV_MCUVMPTW_BGN + 32 );
//VMP��MCU�ı临�ϲ����ܾ�,VMP->MCU������Ϣ��
OSPEVENT(VMPTW_MCU_CHANGEVIDMIXPARAM_NACK,  EV_MCUVMPTW_BGN + 33 );
//VMP��MCU��״̬�ı�����VMP->MCU,��Ϣ��CKDVVMPParam�ṹ
OSPEVENT(VMPTW_MCU_CHANGESTATUS_NOTIF,      EV_MCUVMPTW_BGN + 34 );

OSPEVENT(MCU_VMPTW_UPDATAVMPENCRYPTPARAM_REQ, EV_MCUVMPTW_BGN + 35 );

OSPEVENT(MCU_VMPTW_FASTUPDATEPIC_CMD,       EV_MCUVMPTW_BGN + 36 );

//MCU����VMP���͸��ϲ���,MCU->VMP������Ϣ��
OSPEVENT(MCU_VMPTW_GETVIDMIXPARAM_REQ,      EV_MCUVMPTW_BGN + 41 );
//VMP��MCU���͸��ϲ���ȷ��,VMP->MCU����Ϣ��ΪCKDVVMPParam�ṹ
OSPEVENT(VMPTW_MCU_GETVIDMIXPARAM_ACK,      EV_MCUVMPTW_BGN + 42 );
//VMP��MCU���͸��ϲ����ܾ�,VMP->MCU������Ϣ��
OSPEVENT(VMPTW_MCU_GETVIDMIXPARAM_NACK,     EV_MCUVMPTW_BGN + 43 );
//����VMP��ʼ��
OSPEVENT(EV_VMPTW_INIT,                     EV_MCUVMPTW_BGN + 50 );
//��ʾ���е�ͳ�ƺ�״̬
OSPEVENT(EV_VMPTW_DISPLAYALL,               EV_MCUVMPTW_BGN + 60 );
//MCU����VMP���͸��ϲ���,MCU->VMP����Ϣ��u16�����ʣ�CServMsg.SetChnIndex()[1 or 2]
OSPEVENT(MCU_VMPTW_SETCHANNELBITRATE_REQ,   EV_MCUVMPTW_BGN + 65 );
//VMP��MCU���͸��ϲ���ȷ��,VMP->MCU����Ϣ��ΪCKDVVMPParam�ṹ
OSPEVENT(VMPTW_MCU_SETCHANNELBITRATE_ACK,   EV_MCUVMPTW_BGN + 66 );
//VMP��MCU���͸��ϲ����ܾ�,VMP->MCU������Ϣ��
OSPEVENT(VMPTW_MCU_SETCHANNELBITRATE_NACK,  EV_MCUVMPTW_BGN + 67 );
//����I֡,CServMsg.GetChnIndexΪ�����ͨ����
OSPEVENT(VMPTW_MCU_NEEDIFRAME_CMD,          EV_MCUVMPTW_BGN + 70 );

/*----------     ���ϵ���ǽ��Ϣ�������(26701-26800)   ----------*/


/*----------     ����Ϊ���ܶ����ָ���Ϣ(26801-26900)   ----------*/

//PRS��MCUע������PRS->MCU����Ϣ��ΪTPeriEqpRegReq�ṹ
OSPEVENT( PRS_MCU_REGISTER_REQ,				EV_MCUPRS_BGN );
//MCU��PRSע��ȷ��,MCU->PRS����Ϣ��TRegAck
OSPEVENT( MCU_PRS_REGISTER_ACK,				EV_MCUPRS_BGN + 1 );
//MCU��PRSע��ܾ�,MCU->PRS������Ϣ��
OSPEVENT( MCU_PRS_REGISTER_NACK,			EV_MCUPRS_BGN + 2 );
//VMP��MCU��״̬�ϱ�,PRS->MCU����Ϣ��:TPeriEqpStatus
OSPEVENT( PRS_MCU_PRSSTATUS_NOTIF,			EV_MCUPRS_BGN + 3 );

//MCU����PRS��ʼ������ϢԴ,MCU->PRS����Ϣ��TPrsParam��ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( MCU_PRS_SETSRC_REQ,				EV_MCUPRS_BGN + 5 );
//PRS��MCU������ϢԴȷ��,PRS->MCU������Ϣ�壬 mcu��eqpSsn����PrsId
OSPEVENT( PRS_MCU_SETSRC_ACK,				EV_MCUPRS_BGN + 6 );
//PRS��MCU������ϢԴ�ܾ�,PRS->MCU������Ϣ��
OSPEVENT( PRS_MCU_SETSRC_NACK,				EV_MCUPRS_BGN + 7 );
//PRS��MCU�ı�����ϢԴ�����PRS->MCU,��Ϣ��TPeriEqpStatus
OSPEVENT( PRS_MCU_SETSRC_NOTIF,				EV_MCUPRS_BGN + 8 );

//MCU����PRS���һ���ش�ͨ��,MCU->PRS����Ϣ��TPrsParam��ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( MCU_PRS_ADDRESENDCH_REQ,			EV_MCUPRS_BGN + 10 );
//PRS��MCUֹͣ����ȷ��,PRS->MCU������Ϣ��
OSPEVENT( PRS_MCU_ADDRESENDCH_ACK,			EV_MCUPRS_BGN + 11 );
//PRS��MCUֹͣ�����ܾ�,PRS->MCU������Ϣ��
OSPEVENT( PRS_MCU_ADDRESENDCH_NACK,			EV_MCUPRS_BGN + 12 );
//PRS��MCU��ֹͣ�����PRS->MCU,��Ϣ��TPeriEqpStatus
OSPEVENT( PRS_MCU_ADDRESENDCH_NOTIF,		EV_MCUPRS_BGN + 13 );

//MCU����PRS�Ƴ�һ���ش�ͨ��,MCU->PRS����Ϣ��TPrsParam��ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( MCU_PRS_REMOVERESENDCH_REQ,		EV_MCUPRS_BGN + 15 );
//PRS��MCU�ı临�ϲ���ȷ��,PRS->MCU������Ϣ��
OSPEVENT( PRS_MCU_REMOVERESENDCH_ACK,		EV_MCUPRS_BGN + 16 );
//PRS��MCU�ı临�ϲ����ܾ�,PRS->MCU������Ϣ��
OSPEVENT( PRS_MCU_REMOVERESENDCH_NACK,		EV_MCUPRS_BGN + 17 );
//PRS��MCU��״̬�ı�����PRS->MCU,��Ϣ��TPeriEqpStatus
OSPEVENT( PRS_MCU_REMOVERESENDCH_NOTIF,		EV_MCUPRS_BGN + 18 );

//MCU����PRS�Ƴ����е��ش�ͨ��,MCU->PRS������Ϣ�壬ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( MCU_PRS_REMOVEALL_REQ,			EV_MCUPRS_BGN + 20 );
//PRS��MCU�ı临�ϲ���ȷ��,PRS->MCU������Ϣ��
OSPEVENT( PRS_MCU_REMOVEALL_ACK,			EV_MCUPRS_BGN + 21 );
//PRS��MCU�ı临�ϲ����ܾ�,PRS->MCU������Ϣ��
OSPEVENT( PRS_MCU_REMOVEALL_NACK,			EV_MCUPRS_BGN + 22 );
//PRS��MCU��״̬�ı�����PRS->MCU,��Ϣ��TPeriEqpStatus
OSPEVENT( PRS_MCU_REMOVEALL_NOTIF,			EV_MCUPRS_BGN + 23 );

//����PRS��ʼ��
OSPEVENT( EV_PRS_INIT,	  	  				EV_MCUPRS_BGN + 25 );
//��ʾͨ��״̬
OSPEVENT( EV_PRS_STATUS,	  	  			EV_MCUPRS_BGN + 30 );

OSPEVENT( MCUVC_OCCUPYPRS_CMD,	  			EV_MCUPRS_BGN + 31 );
OSPEVENT( MCUVC_APPLYPRSSUC_NTF,	  		EV_MCUPRS_BGN + 32 );
OSPEVENT( EV_MCU_CONNECTPRS_TIMER,	  	    EV_MCUPRS_BGN + 33 );
OSPEVENT( EV_MCU_PRS_REG_TIMER,	     		EV_MCUPRS_BGN + 34 );
OSPEVENT( MCU_PRS_REG_REQ,	     	    	EV_MCUPRS_BGN + 35 );
OSPEVENT( PRS_MCU_REG_ACK,	  		    	EV_MCUPRS_BGN + 36 );
OSPEVENT( PRS_MCU_REG_NACK,  		    	EV_MCUPRS_BGN + 37 );
OSPEVENT( EV_MCU_CHECKPRSRES_TIMER,  		EV_MCUPRS_BGN + 38 );
OSPEVENT( EV_MCU_PRSDISCONNECT_NTF,  		EV_MCUPRS_BGN + 39 );


/*----------   ���ܶ����ָ���Ϣ�������(26801-26900)  --------*/


/*----------     ����Ϊ�ļ���������Ϣ(26901-27000)    --------*/

//File Encoder��MCUע������File Encoder->MCU����Ϣ��ΪTPeriEqpRegReq�ṹ
/*OSPEVENT( FILEENC_MCU_REGISTER_REQ,		EV_MCUFILEENC_BGN );
//MCU��File Encoderע��ȷ��,MCU->File Encoder����Ϣ��TPeriEqpRegAck
OSPEVENT( MCU_FILEENC_REGISTER_ACK,			EV_MCUFILEENC_BGN + 1 );
//MCU��File Encoderע��ܾ�,MCU->File Encoder������Ϣ��
OSPEVENT( MCU_FILEENC_REGISTER_NACK,		EV_MCUFILEENC_BGN + 2 );


//�ļ���������ʼ����, MCU->File Encoder(Daemon), ��Ϣ��Ϊ ��Ƶ��������(1B)+��Ƶ��������(1B)+����(2B ������)
OSPEVENT( MCU_FILEENC_STARTENC_REQ,			EV_MCUFILEENC_BGN + 4 );
//�ļ���������ʼ����Ӧ��, File Encoder -> MCU, ��Ϣ��Ϊ ʹ�õ��ļ�������ʵ����(1B)
OSPEVENT( MCU_FILEENC_STARTENC_ACK,			EV_MCUFILEENC_BGN + 5 );
//�ļ���������ʼ�����Ӧ��, File Encoder -> MCU, ����Ϣ��
OSPEVENT( MCU_FILEENC_STARTENC_NACK,		EV_MCUFILEENC_BGN + 6 );

//�ļ�������ֹͣ����, MCU->File Encoder(Daemon), ��Ϣ��Ϊ �ļ�������ʵ����(1B)
OSPEVENT( MCU_FILEENC_STOPENC_REQ,			EV_MCUFILEENC_BGN + 8 );
//�ļ�������ֹͣ����Ӧ��, File Encoder -> MCU, ����Ϣ��
OSPEVENT( MCU_FILEENC_STOPENC_ACK,			EV_MCUFILEENC_BGN + 9 );
//�ļ�������ֹͣ�����Ӧ��, File Encoder -> MCU, ����Ϣ��
OSPEVENT( MCU_FILEENC_STOPENC_NACK,			EV_MCUFILEENC_BGN + 10 );

//File Encoder��MCU��״̬�ϱ�,File Encoder->MCU����Ϣ��:TPeriEqpStatus
OSPEVENT( FILEENC_MCU_FILEENCSTATUS_NOTIF,	EV_MCUFILEENC_BGN + 12 );

//�ڲ���Ϣ
OSPEVENT( EV_FILEENC_SHOW,					EV_MCUFILEENC_BGN + 16 );
OSPEVENT( EV_FILEENC_INI,					EV_MCUFILEENC_BGN + 20 );
OSPEVENT( EV_FILEENC_CONNECT,				EV_MCUFILEENC_BGN + 24 );
OSPEVENT( EV_FILEENC_REGISTER,				EV_MCUFILEENC_BGN + 28 );
OSPEVENT( EV_FILEENC_REGTIMEOUT,			EV_MCUFILEENC_BGN + 32 );*/

/*------------     �ļ���������Ϣ�������(26901-27000)    ---------*/


/*-- �������蹲����Ϣ, ���ڱ�����ǰ100��, ��(26001-26100)  --*/

//MCU�����軷·ά��������Ϣ��:u32���кš�(����ҪCServMsg��װ��
OSPEVENT( MCU_EQP_ROUNDTRIP_REQ,			EV_MCUEQP_BGN + 35 );
//��Ϣ��u32���кš�
OSPEVENT( EQP_MCU_ROUNDTRIP_ACK,			EV_MCUEQP_BGN + 40 );

// ����qos����Ϣ�壺TMcuQosCfgInfo
OSPEVENT( MCU_EQP_SETQOS_CMD,				EV_MCUEQP_BGN + 30 );
// ����ȡMcu�����Ƿ�ɹ�
OSPEVENT( EQP_MCU_GETMSSTATUS_REQ,          EV_MCUEQP_BGN + 31 );
// Mcu �����赹��״̬, ��Ϣ�壺TMcuMsStatus
OSPEVENT( MCU_EQP_GETMSSTATUS_ACK,          EV_MCUEQP_BGN + 32 );
// Mcu �����赹��״̬
OSPEVENT( MCU_EQP_GETMSSTATUS_NACK,         EV_MCUEQP_BGN + 33 );
// Mcu ֪ͨ���轨�������ɹ�
OSPEVENT( MCU_EQP_SWITCHSTART_NOTIF,        EV_MCUEQP_BGN + 34 );

// Mcu ֪ͨ�������� MPCB ��, zgc, 2007/04/30
OSPEVENT( MCU_EQP_RECONNECTB_CMD,			EV_MCUEQP_BGN + 36 );
/*---------- �������蹲����Ϣ�������(26001-26100)  ----------*/

/**********************************************************
 ����ΪMCU������֮��ҵ��ӿڵĲ��� (39601-40000)
**********************************************************/

/*----------    ����ΪMpuBas��Ϣ(39601-39650)   ----------*/

//MPUBAS��MCU����ע������MPUBAS->MCU����Ϣ��ΪTPeriEqpRegReq�ṹ
OSPEVENT( MPUBAS_MCU_REG_REQ,			    EV_MCUMPUBAS_BGN );
//MCU����Ӧ��MCU->MPUBAS����Ϣ��Ϊ��Ϣ��ΪTPeriEqpRegAck + TPrsTimeSpan + u16(MTU size)
OSPEVENT( MCU_MPUBAS_REG_ACK,				EV_MCUMPUBAS_BGN + 1 );
//MCU�ܾ�Ӧ��MCU->MPUBAS������Ϣ��
OSPEVENT( MCU_MPUBAS_REG_NACK,				EV_MCUMPUBAS_BGN + 2 );

//������״̬֪ͨ��MPUBAS->MCU����Ϣ��ΪTPeriEqpStatus
OSPEVENT( MPUBAS_MCU_MPUBASSTATUS_NOTIF,	EV_MCUMPUBAS_BGN + 3 );
//����������ͨ��״̬֪ͨ��MPUBAS->MCU ����Ϣ��: THDBasVidChnStatus,ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( MPUBAS_MCU_CHNNLSTATUS_NOTIF,		EV_MCUMPUBAS_BGN + 4 );

//��Ϣ�壺THDAdaptParam+THDAdaptParam+TMediaEncrypt(video)+TDoublePayload(video)��ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( MCU_MPUBAS_STARTADAPT_REQ,		EV_MCUMPUBAS_BGN + 5 );
OSPEVENT( MCU_MPUBAS_STARTADAPT_ACK,		EV_MCUMPUBAS_BGN + 6 );
OSPEVENT( MCU_MPUBAS_STARTADAPT_NACK,		EV_MCUMPUBAS_BGN + 7 );

//MCUֹͣ�������MCU->MPUBAS������Ϣ�壬ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( MCU_MPUBAS_STOPADAPT_REQ,         EV_MCUMPUBAS_BGN + 8 );
//ֹͣ����Ӧ����Ϣ��MPUBAS->MCU������Ϣ��
OSPEVENT( MPUBAS_MCU_STOPADAPT_ACK,         EV_MCUMPUBAS_BGN + 9 );
//ֹͣ����Ӧ����Ϣ��MPUBAS->MCU������Ϣ��
OSPEVENT( MPUBAS_MCU_STOPADAPT_NACK,        EV_MCUMPUBAS_BGN + 10 );

//��Ϣ�壺THDAdaptParam+THDAdaptParam+TMediaEncrypt(video)+TDoublePayload(video)��ҵ����Ϣ��ͨ��������ָ��ͨ��
OSPEVENT( MCU_MPUBAS_SETADAPTPARAM_CMD,		EV_MCUMPUBAS_BGN + 11 );

OSPEVENT( MCU_MPUBAS_FASTUPDATEPIC_CMD,		EV_MCUMPUBAS_BGN + 12 );
OSPEVENT( MPUBAS_MCU_NEEDIFRAME_CMD,		EV_MCUMPUBAS_BGN + 13 );

/*----------    MpuBas��Ϣ����(39601-39650)   ----------*/

/*----------    ����ΪMpuEBap��Ϣ(39651-39700)   ----------*/
/*----------    MpuEBap��Ϣ����(39651-39700)   ----------*/

/*----------    ����ΪMpuEVpu��Ϣ(39701-39800)   ----------*/
/*----------    MpuEVpu��Ϣ����(39701-39800)   ----------*/

/*----------    ����ΪMpuSVmp��Ϣ(39801-39900)   ----------*/
/*----------    MpuSVmp��Ϣ����(39801-39900)   ----------*/

/*----------    ����ΪMpuDVmp��Ϣ(39901-40000)   ----------*/
//VMP��MCUע������VMP->MCU����Ϣ��ΪTPeriEqpRegReq�ṹ
OSPEVENT( MPUDVMP_MCU_REGISTER_REQ,				EV_MCUMPUDVMP_BGN );
//MCU��VMPע��ȷ��,MCU->VMP����Ϣ��ΪTPeriEqpRegAck + TPrsTimeSpan + u16(MTU size)��
OSPEVENT( MCU_MPUDVMP_REGISTER_ACK,				EV_MCUMPUDVMP_BGN + 1 );
//MCU��VMPע��ܾ�,MCU->VMP������Ϣ��
OSPEVENT( MCU_MPUDVMP_REGISTER_NACK,			EV_MCUMPUDVMP_BGN + 2 );
//VMP��MCU��״̬�ϱ�,VMP->MCU����Ϣ��:TPeriEqpStatus
OSPEVENT( MPUDVMP_MCU_VMPSTATUS_NOTIF,			EV_MCUMPUDVMP_BGN + 3 );

//MCU���û���ϳɷ��, ��Ϣ��:TVmpStyleCfgInfo
OSPEVENT( MCU_MPUDVMP_SETSTYLE_CMD,				EV_MCUMPUDVMP_BGN + 4 );
//MCU����VMPֹͣMAP, ��Ϣ��: ��, zgc, 2007/04/24
OSPEVENT( MCU_MPUDVMP_STOPMAP_CMD,				EV_MCUMPUDVMP_BGN + 5 );

//MCU����VMP��ʼ����,MCU->VMP����Ϣ��ΪCKDVVMPParam+CKDVVMPParam+u8(�Ƿ�prs)+TVmpStyleCfgInfo+TCapSupportEx
OSPEVENT( MCU_MPUDVMP_STARTVIDMIX_REQ,			EV_MCUMPUDVMP_BGN + 11 );
//VMP��MCU��ʼ����ȷ��,VMP->MCU������Ϣ��
OSPEVENT( MPUDVMP_MCU_STARTVIDMIX_ACK,			EV_MCUMPUDVMP_BGN + 12 );
//VMP��MCU��ʼ�����ܾ�,VMP->MCU������Ϣ��
OSPEVENT( MPUDVMP_MCU_STARTVIDMIX_NACK,			EV_MCUMPUDVMP_BGN + 13 );
//VMP��MCU�Ŀ�ʼ�����VMP->MCU,��Ϣ���
OSPEVENT( MPUDVMP_MCU_STARTVIDMIX_NOTIF,		EV_MCUMPUDVMP_BGN + 14 );

//MCU����VMPֹͣ����,MCU->VMP������Ϣ��
OSPEVENT( MCU_MPUDVMP_STOPVIDMIX_REQ,			EV_MCUMPUDVMP_BGN + 21 );
//VMP��MCUֹͣ����ȷ��,VMP->MCU������Ϣ��
OSPEVENT( MPUDVMP_MCU_STOPVIDMIX_ACK,			EV_MCUMPUDVMP_BGN + 22 );
//VMP��MCUֹͣ�����ܾ�,VMP->MCU������Ϣ��
OSPEVENT( MPUDVMP_MCU_STOPVIDMIX_NACK,			EV_MCUMPUDVMP_BGN + 23 );
//VMP��MCU��ֹͣ�����VMP->MCU,��Ϣ���
OSPEVENT( MPUDVMP_MCU_STOPVIDMIX_NOTIF,			EV_MCUMPUDVMP_BGN + 24 );

//MCU����VMP�ı临�ϲ���,MCU->VMP����Ϣ��ΪCKDVVMPParam+CKDVVMPParam+u8(�Ƿ�prs)+TVmpStyleCfgInfo
OSPEVENT( MCU_MPUDVMP_CHANGEVIDMIXPARAM_REQ,	EV_MCUMPUDVMP_BGN + 31 );
//VMP��MCU�ı临�ϲ���ȷ��,VMP->MCU������Ϣ��
OSPEVENT( MPUDVMP_MCU_CHANGEVIDMIXPARAM_ACK,	EV_MCUMPUDVMP_BGN + 32 );
//VMP��MCU�ı临�ϲ����ܾ�,VMP->MCU������Ϣ��
OSPEVENT( MPUDVMP_MCU_CHANGEVIDMIXPARAM_NACK,	EV_MCUMPUDVMP_BGN + 33 );
//VMP��MCU��״̬�ı�����VMP->MCU,��Ϣ��CKDVVMPParam�ṹ
OSPEVENT( MPUDVMP_MCU_CHANGESTATUS_NOTIF,		EV_MCUMPUDVMP_BGN + 34 );

OSPEVENT( MCU_MPUDVMP_UPDATAVMPENCRYPTPARAM_REQ,EV_MCUMPUDVMP_BGN + 35 );

OSPEVENT( MCU_MPUDVMP_FASTUPDATEPIC_CMD,		EV_MCUMPUDVMP_BGN + 36 );

//MCU����VMP���͸��ϲ���,MCU->VMP������Ϣ��
OSPEVENT( MCU_MPUDVMP_GETVIDMIXPARAM_REQ,		EV_MCUMPUDVMP_BGN + 41 );
//VMP��MCU���͸��ϲ���ȷ��,VMP->MCU����Ϣ��ΪCKDVVMPParam�ṹ
OSPEVENT( MPUDVMP_MCU_GETVIDMIXPARAM_ACK,		EV_MCUMPUDVMP_BGN + 42 );
//VMP��MCU���͸��ϲ����ܾ�,VMP->MCU������Ϣ��
OSPEVENT( MPUDVMP_MCU_GETVIDMIXPARAM_NACK,		EV_MCUMPUDVMP_BGN + 43 );
//����VMP��ʼ��
OSPEVENT( EV_MPUDVMP_INIT,	  	  				EV_MCUMPUDVMP_BGN + 50 );
//��ʾ���е�ͳ�ƺ�״̬
OSPEVENT( EV_MPUDVMP_DISPLAYALL,	  			EV_MCUMPUDVMP_BGN + 60 );
//��ʾVMP�ڲ��ĺϳɲ���
OSPEVENT( EV_MPUDVMP_SHOWPARAM,					EV_MCUMPUDVMP_BGN + 61 );
//MCU����VMP���͸��ϲ���,MCU->VMP����Ϣ��u16�����ʣ�CServMsg.SetChnIndex()[1 or 2]
OSPEVENT( MCU_MPUDVMP_SETCHANNELBITRATE_REQ,	EV_MCUMPUDVMP_BGN + 65 );
//VMP��MCU���͸��ϲ���ȷ��,VMP->MCU����Ϣ��ΪCKDVVMPParam�ṹ
OSPEVENT( MPUDVMP_MCU_SETCHANNELBITRATE_ACK,	EV_MCUMPUDVMP_BGN + 66 );
//VMP��MCU���͸��ϲ����ܾ�,VMP->MCU������Ϣ��
OSPEVENT( MPUDVMP_MCU_SETCHANNELBITRATE_NACK,	EV_MCUMPUDVMP_BGN + 67 );
//����I֡,CServMsg.GetChnIndexΪ�����ͨ����
OSPEVENT( MPUDVMP_MCU_NEEDIFRAME_CMD,			EV_MCUMPUDVMP_BGN + 70 );
//��������Ƿ��������VMP�ڲ�ʹ�ã���Ϣ�壺u8: 0�����ã�1����
OSPEVENT( EV_MPUDVMP_BACKBOARD_OUT_CMD,         EV_MCUMPUDVMP_BGN + 71 );
/*----------    MpuDVmp��Ϣ����(39901-40000)   ----------*/

#endif /* _EV_MCUEQP_H_ */
