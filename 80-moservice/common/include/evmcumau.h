/*****************************************************************************
ģ����      : MAU
�ļ���      : evmcumau.h
����ʱ��    : 2014�� 08�� 26��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
******************************************************************************/

#ifndef		_EV_MCU_MAU_H_
#define		_EV_MCU_MAU_H_

#include "osp.h"
#include "eventid.h"

/*
//MAU��MCU���ʹ�������
OSPEVENT( MAU_MCU_CREATECONF_REQ,			  EV_MPC_MPCD_BGN );
//MAU��������ACK��
OSPEVENT( MCU_MAU_CREATECONF_ACK,			  EV_MPC_MPCD_BGN + 1 );
//MAU��������NACK��
OSPEVENT( MCU_MAU_CREATECONF_NACK,			  EV_MPC_MPCD_BGN + 2 );

//MAU����MCU��������
OSPEVENT( MAU_MCU_RELEASECONF_CMD,			  EV_MPC_MPCD_BGN + 3 );

//MAU����MCU����ն�
OSPEVENT( MAU_MCU_ADDMT_REQ,				  EV_MPC_MPCD_BGN + 4 );
//MAU����ն�ACK��
OSPEVENT( MCU_MAU_ADDMT_ACK,				  EV_MPC_MPCD_BGN + 5 );
//MAU����ն�NACK��
OSPEVENT( MCU_MAU_ADDMT_NACK,				  EV_MPC_MPCD_BGN + 6 );

//MCU�������֪ͨ
OSPEVENT( MCU_MAU_CONFRELEASE_NTF,			  EV_MPC_MPCD_BGN + 10 );
//MCU����״̬�ı�֪ͨ
OSPEVENT( MCU_MAU_CONFSTATECHANGE_NTF,		  EV_MPC_MPCD_BGN + 11 );
//MCU������л���
OSPEVENT( MCU_MAU_CONFCALLADDR_REQ,		  EV_MPC_MPCD_BGN + 12 );
//MAU�ظ�MCU ���������ĺ��е�ַ
OSPEVENT( MAU_MCU_CONFCALLADDR_ACK,		  EV_MPC_MPCD_BGN + 13 );
//MAU NACK�ظ�MCU ���������ĺ��е�ַ
OSPEVENT( MAU_MCU_CONFCALLADDR_NACK,		  EV_MPC_MPCD_BGN + 14 );
//MAU��MCU������е�ַ
OSPEVENT( MAU_MCU_CALLADDR_REQ,			  EV_MPC_MPCD_BGN + 15 );
//MCU ACK�ظ�MAU ���е�ַ
OSPEVENT( MCU_MAU_CALLADDR_ACK,			  EV_MPC_MPCD_BGN + 16 );
//MCU NACK�ظ�MAU ���е�ַ
OSPEVENT( MCU_MAU_CALLADDR_NACK,			  EV_MPC_MPCD_BGN + 17 );

//MCU �ϱ�MAU ���ý�������
OSPEVENT( MCU_MAU_MAXMEDIACONFNUM_NTF,		  EV_MPC_MPCD_BGN + 18 );

//MCU �ϱ���ǰ�����б�
OSPEVENT( MCU_MAU_CONFLIST_NTF,              EV_MPC_MPCD_BGN + 19 );

//MAU�������������Ϣ����Ϣ�壺u8(E164 len)+����E164��
OSPEVENT( MAU_MCU_CONFINFO_REQ,              EV_MPC_MPCD_BGN + 20 );
//MCU��Ӧ����������Ϣ����Ϣ�壺TConfInfo
OSPEVENT( MCU_MAU_CONFINFO_ACK,              EV_MPC_MPCD_BGN + 21 );
//MCU�ܾ���Ӧ����������Ϣ����Ϣ�壺u8(E164 len)+����E164��
OSPEVENT( MCU_MAU_CONFINFO_NACK,             EV_MPC_MPCD_BGN + 22 );

// wy[2012/08/21]
//MCU��MAUͬ���ȱ���Ϣ����Ϣ�壺ͬ����Ϣ����(EHBType)+ConfE164NO+�ȱ���ϢBuf
OSPEVENT( MCU_MAU_BAKEUP_CONFINFO_NTF,       EV_MPC_MPCD_BGN + 23 );
//MAU��MCU������л��������Ϣ�ָ�����Ϣ�壺TReqHeadInfo+TConfInfo+AliasLen(u16)+AliasBuf
//                                           +TVmpModule+MtInConfList(192)+MtInviteList(192)
OSPEVENT( MAU_MCU_RESUME_CONFINFO_REQ,       EV_MPC_MPCD_BGN + 24 );
//MCU�ظ�MAU���������Ϣ�ָ����
OSPEVENT( MCU_MAU_RESUME_CONFINFO_ACK,       EV_MPC_MPCD_BGN + 25 );
OSPEVENT( MCU_MAU_RESUME_CONFINFO_NACK,      EV_MPC_MPCD_BGN + 26 );
//MAU��MCU������л��������Ϣ�ָ�����Ϣ�壺TReqHeadInfo+TBDBConfProtectInfo+TBDBConfPollParam
//                                           +TMt(VidBrdSrc)+TMt(AudBrdSrc)+TMt(LastSpeaker)
//                                           +TMt(VacLast)+TMt(DSSrc)+MtDecoderMuteList(192)
//                                           +MtCaptureMuteList(192)+SelectVideoMt(192*TMt)+SelectAudioMt(192*TMt)
OSPEVENT( MAU_MCU_RESUME_CONFINFOEX_REQ,     EV_MPC_MPCD_BGN + 27 );
//MCU�ظ�MAU���������Ϣ�ָ����
OSPEVENT( MCU_MAU_RESUME_CONFINFOEX_ACK,     EV_MPC_MPCD_BGN + 28 );
OSPEVENT( MCU_MAU_RESUME_CONFINFOEX_NACK,    EV_MPC_MPCD_BGN + 29 );

//MCU�������̻�������ʱ�䣬��Ϣ��:TReqHeadInfo+u16(wDelayTime)
OSPEVENT( MCU_MAU_SUBCONFDURATION_REQ,       EV_MPC_MPCD_BGN + 30 );
OSPEVENT( MAU_MCU_SUBCONFDURATION_ACK,	      EV_MPC_MPCD_BGN + 31 );
OSPEVENT( MAU_MCU_SUBCONFDURATION_NACK,	  EV_MPC_MPCD_BGN + 32 );

//MCU �ϱ�MAU MTADP/MP/PRS�Ƚ���ģ��״̬, ��Ϣ�壺TMtsMdStatus
OSPEVENT( MCU_MAU_MTSMDSTATUS_NTF,			  EV_MPC_MPCD_BGN + 35 );

//MCU�����ӳ�����,��Ϣ�壺TReqHeadInfo+u16(wMtId)+u16(wMccId)+u16(wDelayTime)
OSPEVENT( MCU_MAU_DELAYCONF_REQ,			  EV_MPC_MPCD_BGN + 36 );
OSPEVENT( MAU_MCU_DELAYCONF_ACK,			  EV_MPC_MPCD_BGN + 37 );
OSPEVENT( MAU_MCU_DELAYCONF_NACK,			  EV_MPC_MPCD_BGN + 38 );

//MAU����MCU�ϵĻ����б���ϸ��Ϣ����Ϣ�壺��
OSPEVENT( MAU_MCU_LISTALLCONF_REQ,		      EV_MPC_MPCD_BGN + 39 );
//Ӧ��MAU_MCU_LISTALLCONF_REQ����Ϣ�壺��
OSPEVENT( MCU_MAU_LISTALLCONF_ACK,	          EV_MPC_MPCD_BGN + 40 );
//MCU��û�л���,��Ϣ�壺��
OSPEVENT( MCU_MAU_LISTALLCONF_NACK,         EV_MPC_MPCD_BGN + 41 ); 

//����������Ϣ֪ͨ,��Ϣ��: CServMsgHead+TConfInfo+TConfAllMtInfo
OSPEVENT( MCU_MAU_CONFINFO_NOTIF,            EV_MPC_MPCD_BGN + 42 );
//MCU����MAU���ն�������Ϣ֪ͨ, ��Ϣ�壺TConfAllMtInfo
OSPEVENT( MCU_MAU_CONFALLMTINFO_NOTIF,       EV_MPC_MPCD_BGN + 43 );
//MCU����MAU���ն��б�֪ͨ, ��Ϣ�壺u8(ʵ���ն˸���)+(TMtStatus+TMtExt+TLogicalChannel)
OSPEVENT( MCU_MAU_MTLIST_NOTIF,              EV_MPC_MPCD_BGN + 44 );

//������ϯ����Ϣ��:TReqHeadInfo+TMt
OSPEVENT( MAU_MCU_SETCHAIRMAN_REQ,           EV_MPC_MPCD_BGN + 45 );
//������ϯ�ɹ�����Ϣ��:
OSPEVENT( MCU_MAU_SETCHAIRMAN_ACK,           EV_MPC_MPCD_BGN + 46 );
//������ϯʧ�ܣ���Ϣ��:
OSPEVENT( MCU_MAU_SETCHAIRMAN_NACK,          EV_MPC_MPCD_BGN + 47 );

//ȡ����ϯ����Ϣ��:TReqHeadInfo + TMt
OSPEVENT( MAU_MCU_CANCELCHAIRMAN_REQ,        EV_MPC_MPCD_BGN + 48 );
//ȡ����ϯ�ɹ�����Ϣ��:
OSPEVENT( MCU_MAU_CANCELCHAIRMAN_ACK,        EV_MPC_MPCD_BGN + 49 );
//ȡ����ϯʧ�ܣ���Ϣ��:
OSPEVENT( MCU_MAU_CANCELCHAIRMAN_NACK,       EV_MPC_MPCD_BGN + 50 );

//������������Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MAU_MCU_SETSPEAKER_REQ,            EV_MPC_MPCD_BGN + 51 );
//���������ɹ�����Ϣ��:
OSPEVENT( MCU_MAU_SETSPEAKER_ACK,            EV_MPC_MPCD_BGN + 52 );
//��������ʧ�ܣ���Ϣ��:
OSPEVENT( MCU_MAU_SETSPEAKER_NACK,           EV_MPC_MPCD_BGN + 53 );

//ȡ����������Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MAU_MCU_CANCELSPEAKER_REQ,         EV_MPC_MPCD_BGN + 54 );
//ȡ�������ɹ�����Ϣ��:
OSPEVENT( MCU_MAU_CANCELSPEAKER_ACK,         EV_MPC_MPCD_BGN + 55 );
//ȡ������ʧ�ܣ���Ϣ��:
OSPEVENT( MCU_MAU_CANCELSPEAKER_NACK,        EV_MPC_MPCD_BGN + 56 );

//MAU����MCUɾ���նˣ���Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MAU_MCU_DELMT_REQ,				  EV_MPC_MPCD_BGN + 57 );
//MAUɾ���ն�ACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_DELMT_ACK,				  EV_MPC_MPCD_BGN + 58 );
//MAUɾ���ն�NACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_DELMT_NACK,				  EV_MPC_MPCD_BGN + 59 );

//�����նˣ���Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MAU_MCU_CALLMT_REQ,				  EV_MPC_MPCD_BGN + 60 );
//�����ն�ACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_CALLMT_ACK,				  EV_MPC_MPCD_BGN + 61 );
//�����ն�NACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_CALLMT_NACK,				  EV_MPC_MPCD_BGN + 62 );


//�Ҷ��նˣ���Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MAU_MCU_DROPMT_REQ,				  EV_MPC_MPCD_BGN + 63 );
//�Ҷ��ն�ACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_DROPMT_ACK,				  EV_MPC_MPCD_BGN + 64 );
//�Ҷ��ն�NACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_DROPMT_NACK,				  EV_MPC_MPCD_BGN + 65 );

//MAU����MCU�ն˾�������Ϣ�壺TReqHeadInfo + TMt +1byte(1- MUTE 0-NOMUTE)+1byte(1-DECODER 2-CAPTURE)
OSPEVENT( MAU_MCU_MTAUDMUTE_REQ,			  EV_MPC_MPCD_BGN + 66 );
//MAU�ն˾���ACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_MTAUDMUTE_ACK,			  EV_MPC_MPCD_BGN + 67 );
//MAU�ն˾���NACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_MTAUDMUTE_NACK,			  EV_MPC_MPCD_BGN + 68 );
//���Ͷ���Ϣ����Ϣ�壺2 byte(�ն���,������,ֵΪN,0Ϊ�㲥�������ն�) + N��TMt +TROLLMSG
OSPEVENT( MAU_MCU_SENDRUNMSG_CMD,            EV_MPC_MPCD_BGN + 69 );

//sp2
//��ʼ����ϳɣ���Ϣ�壺TReqHeadInfo + TVMPParam
OSPEVENT( MAU_MCU_STARTVMP_REQ,			  EV_MPC_MPCD_BGN + 70 );
//��ʼ����ϳ�ACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_STARTVMP_ACK,			  EV_MPC_MPCD_BGN + 71 );
//��ʼ����ϳ�NACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_STARTVMP_NACK,			  EV_MPC_MPCD_BGN + 72 );

//�᳡��������Ϣ�壺TReqHeadInfo
OSPEVENT( MAU_MCU_ALLMTMUTE_REQ,			  EV_MPC_MPCD_BGN + 73 );
//�᳡����ACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_ALLMTMUTE_ACK,			  EV_MPC_MPCD_BGN + 74 );
//�᳡����NACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_ALLMTMUTE_NACK,			  EV_MPC_MPCD_BGN + 75 );

//�᳡��������Ϣ�壺TReqHeadInfo
OSPEVENT( MAU_MCU_ALLMTSILENCE_REQ,		  EV_MPC_MPCD_BGN + 76 );
//�᳡����ACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_ALLMTSILENCE_ACK,		  EV_MPC_MPCD_BGN + 77 );
//�᳡����NACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_ALLMTSILENCE_NACK,		  EV_MPC_MPCD_BGN + 78 );

//MCU֪ͨMAU�������ʣ��ʱ��, ��Ϣ��: 1 u16 (������, �������ʣ��ʱ��, ��λ: ����)
OSPEVENT( MCU_MAU_CONFTIMELEFT_NOTIF,        EV_MPC_MPCD_BGN + 79 );
//������ϯ֪ͨ��MAU, ��Ϣ�壺 TMt(������ն�) 
OSPEVENT( MCU_MAU_MTAPPLYCHAIRMANT_NOTIF,    EV_MPC_MPCD_BGN + 80 );
//��������֪ͨ��MAU��MT->MCU����Ϣ��NULL
OSPEVENT( MCU_MAU_MTAPPLYSPEAKER_NOTIF,	  EV_MPC_MPCD_BGN + 81 ); 

//ȡ���᳡��������Ϣ�壺TReqHeadInfo
OSPEVENT( MAU_MCU_CANCELALLMTMUTE_REQ,		  EV_MPC_MPCD_BGN + 82 );
//�᳡����ACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_CANCELALLMTMUTE_ACK,		  EV_MPC_MPCD_BGN + 83 );
//�᳡����NACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_CANCELALLMTMUTE_NACK,	  EV_MPC_MPCD_BGN + 84 );

//ȡ���᳡��������Ϣ�壺TReqHeadInfo
OSPEVENT( MAU_MCU_CANCELALLMTSILENCE_REQ,	  EV_MPC_MPCD_BGN + 85 );
//�᳡����ACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_CANCELALLMTSILENCE_ACK,	  EV_MPC_MPCD_BGN + 86 );
//�᳡����NACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_CANCELALLMTSILENCE_NACK,	  EV_MPC_MPCD_BGN + 87 );

//��ʼ����
OSPEVENT( MAU_MCU_STARTDISCUSS_REQ,    EV_MPC_MPCD_BGN + 88 );
OSPEVENT( MCU_MAU_STARTDISCUSS_ACK,    EV_MPC_MPCD_BGN + 89 );
OSPEVENT( MCU_MAU_STARTDISCUSS_NACK,   EV_MPC_MPCD_BGN + 90 );

//��������
OSPEVENT( MAU_MCU_STOPDISCUSS_REQ,     EV_MPC_MPCD_BGN + 91 );
OSPEVENT( MCU_MAU_STOPDISCUSS_ACK,     EV_MPC_MPCD_BGN + 92 );
OSPEVENT( MCU_MAU_STOPDISCUSS_NACK,    EV_MPC_MPCD_BGN + 93 );

//�޸Ļ���ϳɲ���
OSPEVENT( MAU_MCU_CHANGEVMP_REQ,			  EV_MPC_MPCD_BGN + 94);
//�޸Ļ���ϳɲ���ACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_CHANGEVMP_ACK,			  EV_MPC_MPCD_BGN + 95);
//�޸Ļ���ϳɲ���NACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_CHANGEVMP_NACK,			  EV_MPC_MPCD_BGN + 96);

//��ʼ����ϳɣ���Ϣ�壺TReqHeadInfo
OSPEVENT( MAU_MCU_STOPVMP_REQ,			      EV_MPC_MPCD_BGN + 97 );
//��ʼ����ϳ�ACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_STOPVMP_ACK,			      EV_MPC_MPCD_BGN + 98 );
//��ʼ����ϳ�NACK�𸴣���Ϣ�壺��
OSPEVENT( MCU_MAU_STOPVMP_NACK,			  EV_MPC_MPCD_BGN + 99 );

//MAU��MCU�����ӳ���������
OSPEVENT( MAU_MCU_DELAYCONF_REQ,			  EV_MPC_MPCD_BGN + 100 );
OSPEVENT( MCU_MAU_DELAYCONF_ACK,			  EV_MPC_MPCD_BGN + 101 );
OSPEVENT( MCU_MAU_DELAYCONF_NACK,			  EV_MPC_MPCD_BGN + 102 );

//MCU�ӳ�����֪ͨ, ��Ϣ��: 1 u16 (�������ӳ�ʱ�䣬��λ������)
OSPEVENT( MCU_MAU_DELAYCONF_NOTIF,           EV_MPC_MPCD_BGN + 103 );	

//����ģʽ֪ͨ,��Ϣ�壺TConfMode
OSPEVENT( MCU_MAU_CONFMODE_NOTIF,            EV_MPC_MPCD_BGN + 104 );

//�������Ϣ֪ͨ����Ϣ�壺TBasicConfInfo
OSPEVENT( MCU_MAU_SIMCONFINFO_NOTIF,         EV_MPC_MPCD_BGN + 105 );

//MCU���������̨����Ƶ���ϲ���֪ͨ, ��Ϣ�壺  TVMPParam
OSPEVENT( MCU_MAU_VMPPARAM_NOTIF,	          EV_MPC_MPCD_BGN + 106 );

//MAU��MCU����նˣ��ն˱������,MAU_MCU_ADDMT_REQΪ�ն�������ᣩ,��Ϣ�壺
OSPEVENT( MAU_MCU_ADDMT_PASSIVITY_REQ,	      EV_MPC_MPCD_BGN + 107 );
OSPEVENT( MCU_MAU_ADDMT_PASSIVITY_ACK,		  EV_MPC_MPCD_BGN + 108 );
OSPEVENT( MCU_MAU_ADDMT_PASSIVITY_NACK,	  EV_MPC_MPCD_BGN + 109 );

//MAU��MCU���Ϳ�ʼ��ѯ����,��Ϣ�壺MAU����MCU�ն˾�������Ϣ�壺TReqHeadInfo + TPollInfo + TMtPollParam����
OSPEVENT( MAU_MCU_STARTPOLL_CMD,			  EV_MPC_MPCD_BGN + 110 );
//MAU��MCU����ֹͣ��ѯ����,��Ϣ�壺MAU����MCU�ն˾�������Ϣ�壺TReqHeadInfo
OSPEVENT( MAU_MCU_STOPPOLL_CMD,			  EV_MPC_MPCD_BGN + 111 );
//MAU��MCU������ͣ��ѯ����,��Ϣ�壺MAU����MCU�ն˾�������Ϣ�壺TReqHeadInfo
OSPEVENT( MAU_MCU_PAUSEPOLL_CMD,	          EV_MPC_MPCD_BGN + 112 );
//MAU��MCU���ͼ�����ѯ����,��Ϣ�壺MAU����MCU�ն˾�������Ϣ�壺TReqHeadInfo
OSPEVENT( MAU_MCU_RESUMEPOLL_CMD,	          EV_MPC_MPCD_BGN + 113 );

//MAU��MCU����ָ����ѯλ�������Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MAU_MCU_SPECPOLLPOS_REQ,	          EV_MPC_MPCD_BGN + 114 );
OSPEVENT( MCU_MAU_SPECPOLLPOS_ACK,		      EV_MPC_MPCD_BGN + 115 );
OSPEVENT( MCU_MAU_SPECPOLLPOS_NACK,	      EV_MPC_MPCD_BGN + 116 );

//MAU��MCU�����޸���ѯ���������Ϣ�壺TReqHeadInfo + TPollInfo + TMtPollParam����
OSPEVENT( MAU_MCU_CHANGEPOLLPARAM_CMD,	      EV_MPC_MPCD_BGN + 117 );
//MCU��MAU������ѯ�����ı�֪ͨ����Ϣ�壺TReqHeadInfo + TPollInfo + TMtPollParam����
OSPEVENT( MCU_MAU_POLLPARAMCHANGE_NOTIF,	  EV_MPC_MPCD_BGN + 118 );

//MCU��MAU������ѯ״̬֪ͨ����Ϣ�壺TReqHeadInfo + TPollInfo
OSPEVENT( MCU_MAU_POLLSTATE_NOTIF,	          EV_MPC_MPCD_BGN + 119 );


//������Ա����
//���ӻ�����Ա, ��Ϣ�壺TMt����
OSPEVENT( MAU_MCU_ADDMIXMEMBER_CMD,          EV_MPC_MPCD_BGN + 120 );
//�Ƴ�������Ա, ��Ϣ�壺TMt����
OSPEVENT( MAU_MCU_REMOVEMIXMEMBER_CMD,       EV_MPC_MPCD_BGN + 121 );

//�ն�����������֪ͨ, ��Ϣ�壺TMt(�����ն�)
OSPEVENT( MCU_MAU_MTAPPLYMIX_NOTIF,          EV_MPC_MPCD_BGN + 122 );

//MAU��MCU������������ͨ����ʱ����
OSPEVENT( MAU_MCU_SETVOICEHINT_CMD,		  EV_MPC_MPCD_BGN + 123 );

//����ͨ����ʱ����״̬֪ͨ
OSPEVENT( MCU_MAU_VOICEHINT_NTF,	          EV_MPC_MPCD_BGN + 124 );

//MAU����MCU�ϵĻ����б���ϸ��Ϣ�����Ϣ�壺��
OSPEVENT( MAU_MCU_LISTALLCONF_CMD,		      EV_MPC_MPCD_BGN + 125 );
*/

//
//���濪ʼʹ������Ϣ
//
//MAU <--> MCU֮�������Ϣ�����ַ���
OSPEVENT( MAINEV_MAU_MCU,	                  EV_MPC_MPCD_BGN + 126 );

//MAU����MCU�ϵĻ����б���ϸ��Ϣ�����Ϣ�壺��
OSPEVENT( MCU_MAU_ALLMTALIAS_NTF,		      EV_MPC_MPCD_BGN + 127 );


#define MAU_MCU_SUBEV_BEGIN (u32)65535			//����Ϣ��ʼ��

#ifndef MAKESUBEV_u16
//�����u16�������ʹ��
#define  MAKESUBEV_u16(x)          x - MAU_MCU_SUBEV_BEGIN
#endif

#ifndef MAKESUBEV_u32
//�����u32�������ʹ��
#define  MAKESUBEV_u32(x)          x
#endif

#ifndef MAKESUBEV_U16_TO_U32
//u16����ϢתΪu32����Ϣ
#define  MAKESUBEV_U16_TO_U32(x)   x + MAU_MCU_SUBEV_BEGIN 
#endif




const u32 SUBEV_MCU_MAU_MTSTATUS_NTF      = MAU_MCU_SUBEV_BEGIN + 1;

const u32 SUBEV_MCU_MAU_ALLMTALIAS_NTF    = MAU_MCU_SUBEV_BEGIN + 2;

//�ն���Ϣ�����������,�ն��߼�ͨ����Ϣ֪ͨ
const u32 SUBEV_MCU_MAU_MTLOGICCHNNL_NTF  = MAU_MCU_SUBEV_BEGIN + 3;

const u32 SUBEV_MCU_MAU_CALLMTFAILURE_NTF = MAU_MCU_SUBEV_BEGIN + 4;		//�����ն�ʧ��֪ͨ
const u32 SUBEV_MCU_MAU_CALLSUCCESS_NTF   = MAU_MCU_SUBEV_BEGIN + 5;      //�����ն˳ɹ�֪ͨ

//ָ���Ի���
const u32 SUBEV_MAU_MCU_SPECDIALOGIST_REQ     = MAU_MCU_SUBEV_BEGIN + 6;
const u32 SUBEV_MCU_MAU_SPECDIALOGIST_ACK     = MAU_MCU_SUBEV_BEGIN + 7;
const u32 SUBEV_MCU_MAU_SPECDIALOGIST_NACK    = MAU_MCU_SUBEV_BEGIN + 8;

//ȡ���Ի���
const u32 SUBEV_MAU_MCU_CANCELDIALOGIST_REQ   = MAU_MCU_SUBEV_BEGIN + 9;
const u32 SUBEV_MCU_MAU_CANCELDIALOGIST_ACK   = MAU_MCU_SUBEV_BEGIN + 10;
const u32 SUBEV_MCU_MAU_CANCELDIALOGIST_NACK  = MAU_MCU_SUBEV_BEGIN + 11; 

//webmcc�ܾ��ն˼������֪ͨ
const u32 SUBEV_MAU_MCU_REFUSEMIX_NTF		   = MAU_MCU_SUBEV_BEGIN + 12;

//webmcc�޸Ļ��鿪����������
const u32 SUBEV_MAU_MCU_MDFPUBLICCONF_CMD	   = MAU_MCU_SUBEV_BEGIN + 13;

//rnn[2014/04/22]����ǽ���
const u32 SUBEV_MAU_MCU_STARTTVS_REQ		   = MAU_MCU_SUBEV_BEGIN + 14;
const u32 SUBEV_MCU_MAU_STARTTVS_ACK		   = MAU_MCU_SUBEV_BEGIN + 15;
const u32 SUBEV_MCU_MAU_STARTTVS_NACK		   = MAU_MCU_SUBEV_BEGIN + 16;

const u32 SUBEV_MAU_MCU_CHANGETVSPARAM_REQ	   = MAU_MCU_SUBEV_BEGIN + 17;
const u32 SUBEV_MCU_MAU_CHANGETVSPARAM_ACK	   = MAU_MCU_SUBEV_BEGIN + 18;
const u32 SUBEV_MCU_MAU_CHANGETVSPARAM_NACK   = MAU_MCU_SUBEV_BEGIN + 19;

const u32 SUBEV_MAU_MCU_STOPTVS_REQ		   = MAU_MCU_SUBEV_BEGIN + 20;
const u32 SUBEV_MCU_MAU_STOPTVS_ACK		   = MAU_MCU_SUBEV_BEGIN + 21;
const u32 SUBEV_MCU_MAU_STOPTVS_NACK		   = MAU_MCU_SUBEV_BEGIN + 22;

const u32 SUBEV_MCU_MAU_HDUINFO_NTF		   = MAU_MCU_SUBEV_BEGIN + 23;

const u32 SUBEV_MAU_MCU_SETHDUVOLUME_NTF	   = MAU_MCU_SUBEV_BEGIN + 24;	

const u32 SUBEV_MCU_MAU_PLAYHDUFAIL_NTF	   = MAU_MCU_SUBEV_BEGIN + 25;
//end


#endif

