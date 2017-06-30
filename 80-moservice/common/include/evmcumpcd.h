/*****************************************************************************
ģ����      : MPCD
�ļ���      : evmcumpcd.h
����ʱ��    : 2012�� 01�� 05��
ʵ�ֹ���    : 
����        : ���Ĳ�
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/01/05  KDVP2.5     ���Ĳ�        ����
******************************************************************************/

#ifndef		_EV_MCU_MPCD_H_
#define		_EV_MCU_MPCD_H_

#include "osp.h"
#include "eventid.h"

//MPC��MPCDע��
OSPEVENT( MPC_MPCD_REGIST_REQ,                EV_MPC_MPCD_BGN );
//MPCDͬ��MPCע��
OSPEVENT( MPCD_MPC_REGIST_ACK,				  EV_MPC_MPCD_BGN + 1 );
//MPCD�ܾ�MPCע��
OSPEVENT( MPCD_MPC_REGIST_NACK,				  EV_MPC_MPCD_BGN + 2 );
//MPCD��MPC���ʹ�������
OSPEVENT( MPCD_MPC_CREATECONF_REQ,			  EV_MPC_MPCD_BGN + 3 );
//MPCD��������ACK��
OSPEVENT( MPC_MPCD_CREATECONF_ACK,			  EV_MPC_MPCD_BGN + 4 );
//MPCD��������NACK��
OSPEVENT( MPC_MPCD_CREATECONF_NACK,			  EV_MPC_MPCD_BGN + 5 );
//MPCD����MPC����ն�
OSPEVENT( MPCD_MPC_ADDMT_REQ,				  EV_MPC_MPCD_BGN + 6 );
//MPCD����ն�ACK��
OSPEVENT( MPC_MPCD_ADDMT_ACK,				  EV_MPC_MPCD_BGN + 7 );
//MPCD����ն�NACK��
OSPEVENT( MPC_MPCD_ADDMT_NACK,				  EV_MPC_MPCD_BGN + 8 );
//MPCD����MPC��������
OSPEVENT( MPCD_MPC_RELEASECONF_CMD,			  EV_MPC_MPCD_BGN + 9 );
//MPC�������֪ͨ
OSPEVENT( MPC_MPCD_CONFRELEASE_NTF,			  EV_MPC_MPCD_BGN + 10 );
//MPC����״̬�ı�֪ͨ
OSPEVENT( MPC_MPCD_CONFSTATECHANGE_NTF,		  EV_MPC_MPCD_BGN + 11 );
//MPC������л���
OSPEVENT( MPC_MPCD_CONFCALLADDR_REQ,		  EV_MPC_MPCD_BGN + 12 );
//MPCD�ظ�MPC ���������ĺ��е�ַ
OSPEVENT( MPCD_MPC_CONFCALLADDR_ACK,		  EV_MPC_MPCD_BGN + 13 );
//MPCD NACK�ظ�MPC ���������ĺ��е�ַ
OSPEVENT( MPCD_MPC_CONFCALLADDR_NACK,		  EV_MPC_MPCD_BGN + 14 );
//MPCD��MPC������е�ַ
OSPEVENT( MPCD_MPC_CALLADDR_REQ,			  EV_MPC_MPCD_BGN + 15 );
//MPC ACK�ظ�MPCD ���е�ַ
OSPEVENT( MPC_MPCD_CALLADDR_ACK,			  EV_MPC_MPCD_BGN + 16 );
//MPC NACK�ظ�MPCD ���е�ַ
OSPEVENT( MPC_MPCD_CALLADDR_NACK,			  EV_MPC_MPCD_BGN + 17 );

//MPC �ϱ�MPCD ���ý�������
OSPEVENT( MPC_MPCD_MAXMEDIACONFNUM_NTF,		  EV_MPC_MPCD_BGN + 18 );

//MPC �ϱ���ǰ�����б�
OSPEVENT( MPC_MPCD_CONFLIST_NTF,              EV_MPC_MPCD_BGN + 19 );

//MPCD�������������Ϣ����Ϣ�壺u8(E164 len)+����E164��
OSPEVENT( MPCD_MPC_CONFINFO_REQ,              EV_MPC_MPCD_BGN + 20 );
//MPC��Ӧ����������Ϣ����Ϣ�壺TConfInfo
OSPEVENT( MPC_MPCD_CONFINFO_ACK,              EV_MPC_MPCD_BGN + 21 );
//MPC�ܾ���Ӧ����������Ϣ����Ϣ�壺u8(E164 len)+����E164��
OSPEVENT( MPC_MPCD_CONFINFO_NACK,             EV_MPC_MPCD_BGN + 22 );

// wy[2012/08/21]
//MPC��MPCDͬ���ȱ���Ϣ����Ϣ�壺ͬ����Ϣ����(EHBType)+ConfE164NO+�ȱ���ϢBuf
OSPEVENT( MPC_MPCD_BAKEUP_CONFINFO_NTF,       EV_MPC_MPCD_BGN + 23 );
//MPCD��MPC������л��������Ϣ�ָ�����Ϣ�壺TReqHeadInfo+TConfInfo+AliasLen(u16)+AliasBuf
//                                           +TVmpModule+MtInConfList(192)+MtInviteList(192)
OSPEVENT( MPCD_MPC_RESUME_CONFINFO_REQ,       EV_MPC_MPCD_BGN + 24 );
//MPC�ظ�MPCD���������Ϣ�ָ����
OSPEVENT( MPC_MPCD_RESUME_CONFINFO_ACK,       EV_MPC_MPCD_BGN + 25 );
OSPEVENT( MPC_MPCD_RESUME_CONFINFO_NACK,      EV_MPC_MPCD_BGN + 26 );
//MPCD��MPC������л��������Ϣ�ָ�����Ϣ�壺TReqHeadInfo+TBDBConfProtectInfo+TBDBConfPollParam
//                                           +TMt(VidBrdSrc)+TMt(AudBrdSrc)+TMt(LastSpeaker)
//                                           +TMt(VacLast)+TMt(DSSrc)+MtDecoderMuteList(192)
//                                           +MtCaptureMuteList(192)+SelectVideoMt(192*TMt)+SelectAudioMt(192*TMt)
OSPEVENT( MPCD_MPC_RESUME_CONFINFOEX_REQ,     EV_MPC_MPCD_BGN + 27 );
//MPC�ظ�MPCD���������Ϣ�ָ����
OSPEVENT( MPC_MPCD_RESUME_CONFINFOEX_ACK,     EV_MPC_MPCD_BGN + 28 );
OSPEVENT( MPC_MPCD_RESUME_CONFINFOEX_NACK,    EV_MPC_MPCD_BGN + 29 );

//mpc�������̻�������ʱ�䣬��Ϣ��:TReqHeadInfo+u16(wDelayTime)
OSPEVENT( MPC_MPCD_SUBCONFDURATION_REQ,       EV_MPC_MPCD_BGN + 30 );
OSPEVENT( MPCD_MPC_SUBCONFDURATION_ACK,	      EV_MPC_MPCD_BGN + 31 );
OSPEVENT( MPCD_MPC_SUBCONFDURATION_NACK,	  EV_MPC_MPCD_BGN + 32 );

//MPC �ϱ�MPCD MTADP/MP/PRS�Ƚ���ģ��״̬, ��Ϣ�壺TMtsMdStatus
OSPEVENT( MPC_MPCD_MTSMDSTATUS_NTF,			  EV_MPC_MPCD_BGN + 35 );

//mpc�����ӳ�����,��Ϣ�壺TReqHeadInfo+u16(wMtId)+u16(wMccId)+u16(wDelayTime)
OSPEVENT( MPC_MPCD_DELAYCONF_REQ,			  EV_MPC_MPCD_BGN + 36 );
OSPEVENT( MPCD_MPC_DELAYCONF_ACK,			  EV_MPC_MPCD_BGN + 37 );
OSPEVENT( MPCD_MPC_DELAYCONF_NACK,			  EV_MPC_MPCD_BGN + 38 );

//mpcd����MCU�ϵĻ����б���ϸ��Ϣ����Ϣ�壺��
OSPEVENT( MPCD_MCU_LISTALLCONF_REQ,		      EV_MPC_MPCD_BGN + 39 );
//Ӧ��MPCD_MCU_LISTALLCONF_REQ����Ϣ�壺��
OSPEVENT( MCU_MPCD_LISTALLCONF_ACK,	          EV_MPC_MPCD_BGN + 40 );
//MCU��û�л���,��Ϣ�壺��
OSPEVENT( MCU_MPCD_LISTALLCONF_NACK,         EV_MPC_MPCD_BGN + 41 ); 

//����������Ϣ֪ͨ,��Ϣ��: CServMsgHead+TConfInfo+TConfAllMtInfo
OSPEVENT( MCU_MPCD_CONFINFO_NOTIF,            EV_MPC_MPCD_BGN + 42 );
//MCU����MPCD���ն�������Ϣ֪ͨ, ��Ϣ�壺TConfAllMtInfo
OSPEVENT( MCU_MPCD_CONFALLMTINFO_NOTIF,       EV_MPC_MPCD_BGN + 43 );
//MCU����MPCD���ն��б�֪ͨ, ��Ϣ�壺u8(ʵ���ն˸���)+(TMtStatus+TMtExt+TLogicalChannel)
OSPEVENT( MCU_MPCD_MTLIST_NOTIF,              EV_MPC_MPCD_BGN + 44 );

//������ϯ����Ϣ��:TReqHeadInfo+TMt
OSPEVENT( MPCD_MCU_SETCHAIRMAN_REQ,           EV_MPC_MPCD_BGN + 45 );
//������ϯ�ɹ�����Ϣ��:
OSPEVENT( MCU_MPCD_SETCHAIRMAN_ACK,           EV_MPC_MPCD_BGN + 46 );
//������ϯʧ�ܣ���Ϣ��:
OSPEVENT( MCU_MPCD_SETCHAIRMAN_NACK,          EV_MPC_MPCD_BGN + 47 );

//ȡ����ϯ����Ϣ��:TReqHeadInfo + TMt
OSPEVENT( MPCD_MCU_CANCELCHAIRMAN_REQ,        EV_MPC_MPCD_BGN + 48 );
//ȡ����ϯ�ɹ�����Ϣ��:
OSPEVENT( MCU_MPCD_CANCELCHAIRMAN_ACK,        EV_MPC_MPCD_BGN + 49 );
//ȡ����ϯʧ�ܣ���Ϣ��:
OSPEVENT( MCU_MPCD_CANCELCHAIRMAN_NACK,       EV_MPC_MPCD_BGN + 50 );

//������������Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MPCD_MCU_SETSPEAKER_REQ,            EV_MPC_MPCD_BGN + 51 );
//���������ɹ�����Ϣ��:
OSPEVENT( MCU_MPCD_SETSPEAKER_ACK,            EV_MPC_MPCD_BGN + 52 );
//��������ʧ�ܣ���Ϣ��:
OSPEVENT( MCU_MPCD_SETSPEAKER_NACK,           EV_MPC_MPCD_BGN + 53 );

//ȡ����������Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MPCD_MCU_CANCELSPEAKER_REQ,         EV_MPC_MPCD_BGN + 54 );
//ȡ�������ɹ�����Ϣ��:
OSPEVENT( MCU_MPCD_CANCELSPEAKER_ACK,         EV_MPC_MPCD_BGN + 55 );
//ȡ������ʧ�ܣ���Ϣ��:
OSPEVENT( MCU_MPCD_CANCELSPEAKER_NACK,        EV_MPC_MPCD_BGN + 56 );

//MPCD����MPCɾ���նˣ���Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MPCD_MPC_DELMT_REQ,				  EV_MPC_MPCD_BGN + 57 );
//MPCDɾ���ն�ACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_DELMT_ACK,				  EV_MPC_MPCD_BGN + 58 );
//MPCDɾ���ն�NACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_DELMT_NACK,				  EV_MPC_MPCD_BGN + 59 );

//�����նˣ���Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MPCD_MPC_CALLMT_REQ,				  EV_MPC_MPCD_BGN + 60 );
//�����ն�ACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_CALLMT_ACK,				  EV_MPC_MPCD_BGN + 61 );
//�����ն�NACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_CALLMT_NACK,				  EV_MPC_MPCD_BGN + 62 );


//�Ҷ��նˣ���Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MPCD_MPC_DROPMT_REQ,				  EV_MPC_MPCD_BGN + 63 );
//�Ҷ��ն�ACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_DROPMT_ACK,				  EV_MPC_MPCD_BGN + 64 );
//�Ҷ��ն�NACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_DROPMT_NACK,				  EV_MPC_MPCD_BGN + 65 );

//MPCD����MPC�ն˾�������Ϣ�壺TReqHeadInfo + TMt +1byte(1- MUTE 0-NOMUTE)+1byte(1-DECODER 2-CAPTURE)
OSPEVENT( MPCD_MPC_MTAUDMUTE_REQ,			  EV_MPC_MPCD_BGN + 66 );
//MPCD�ն˾���ACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_MTAUDMUTE_ACK,			  EV_MPC_MPCD_BGN + 67 );
//MPCD�ն˾���NACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_MTAUDMUTE_NACK,			  EV_MPC_MPCD_BGN + 68 );
//���Ͷ���Ϣ����Ϣ�壺2 byte(�ն���,������,ֵΪN,0Ϊ�㲥�������ն�) + N��TMt +TROLLMSG
OSPEVENT( MPCD_MCU_SENDRUNMSG_CMD,            EV_MPC_MPCD_BGN + 69 );

//sp2
//��ʼ����ϳɣ���Ϣ�壺TReqHeadInfo + TVMPParam
OSPEVENT( MPCD_MPC_STARTVMP_REQ,			  EV_MPC_MPCD_BGN + 70 );
//��ʼ����ϳ�ACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_STARTVMP_ACK,			  EV_MPC_MPCD_BGN + 71 );
//��ʼ����ϳ�NACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_STARTVMP_NACK,			  EV_MPC_MPCD_BGN + 72 );

//�᳡��������Ϣ�壺TReqHeadInfo
OSPEVENT( MPCD_MPC_ALLMTMUTE_REQ,			  EV_MPC_MPCD_BGN + 73 );
//�᳡����ACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_ALLMTMUTE_ACK,			  EV_MPC_MPCD_BGN + 74 );
//�᳡����NACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_ALLMTMUTE_NACK,			  EV_MPC_MPCD_BGN + 75 );

//�᳡��������Ϣ�壺TReqHeadInfo
OSPEVENT( MPCD_MPC_ALLMTSILENCE_REQ,		  EV_MPC_MPCD_BGN + 76 );
//�᳡����ACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_ALLMTSILENCE_ACK,		  EV_MPC_MPCD_BGN + 77 );
//�᳡����NACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_ALLMTSILENCE_NACK,		  EV_MPC_MPCD_BGN + 78 );

//MCU֪ͨMPCD�������ʣ��ʱ��, ��Ϣ��: 1 u16 (������, �������ʣ��ʱ��, ��λ: ����)
OSPEVENT( MCU_MPCD_CONFTIMELEFT_NOTIF,        EV_MPC_MPCD_BGN + 79 );
//������ϯ֪ͨ��mpcd, ��Ϣ�壺 TMt(������ն�) 
OSPEVENT( MCU_MPCD_MTAPPLYCHAIRMANT_NOTIF,    EV_MPC_MPCD_BGN + 80 );
//��������֪ͨ��mpcd��MT->MCU����Ϣ��NULL
OSPEVENT( MCU_MPCD_MTAPPLYSPEAKER_NOTIF,	  EV_MPC_MPCD_BGN + 81 ); 

//ȡ���᳡��������Ϣ�壺TReqHeadInfo
OSPEVENT( MPCD_MPC_CANCELALLMTMUTE_REQ,		  EV_MPC_MPCD_BGN + 82 );
//�᳡����ACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_CANCELALLMTMUTE_ACK,		  EV_MPC_MPCD_BGN + 83 );
//�᳡����NACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_CANCELALLMTMUTE_NACK,	  EV_MPC_MPCD_BGN + 84 );

//ȡ���᳡��������Ϣ�壺TReqHeadInfo
OSPEVENT( MPCD_MPC_CANCELALLMTSILENCE_REQ,	  EV_MPC_MPCD_BGN + 85 );
//�᳡����ACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_CANCELALLMTSILENCE_ACK,	  EV_MPC_MPCD_BGN + 86 );
//�᳡����NACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_CANCELALLMTSILENCE_NACK,	  EV_MPC_MPCD_BGN + 87 );

//��ʼ����
OSPEVENT( MPCD_MCU_STARTDISCUSS_REQ,    EV_MPC_MPCD_BGN + 88 );
OSPEVENT( MCU_MPCD_STARTDISCUSS_ACK,    EV_MPC_MPCD_BGN + 89 );
OSPEVENT( MCU_MPCD_STARTDISCUSS_NACK,   EV_MPC_MPCD_BGN + 90 );

//��������
OSPEVENT( MPCD_MCU_STOPDISCUSS_REQ,     EV_MPC_MPCD_BGN + 91 );
OSPEVENT( MCU_MPCD_STOPDISCUSS_ACK,     EV_MPC_MPCD_BGN + 92 );
OSPEVENT( MCU_MPCD_STOPDISCUSS_NACK,    EV_MPC_MPCD_BGN + 93 );

//�޸Ļ���ϳɲ���
OSPEVENT( MPCD_MPC_CHANGEVMP_REQ,			  EV_MPC_MPCD_BGN + 94);
//�޸Ļ���ϳɲ���ACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_CHANGEVMP_ACK,			  EV_MPC_MPCD_BGN + 95);
//�޸Ļ���ϳɲ���NACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_CHANGEVMP_NACK,			  EV_MPC_MPCD_BGN + 96);

//��ʼ����ϳɣ���Ϣ�壺TReqHeadInfo
OSPEVENT( MPCD_MPC_STOPVMP_REQ,			      EV_MPC_MPCD_BGN + 97 );
//��ʼ����ϳ�ACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_STOPVMP_ACK,			      EV_MPC_MPCD_BGN + 98 );
//��ʼ����ϳ�NACK�𸴣���Ϣ�壺��
OSPEVENT( MPC_MPCD_STOPVMP_NACK,			  EV_MPC_MPCD_BGN + 99 );

//Mpcd��mcu�����ӳ���������
OSPEVENT( MPCD_MPC_DELAYCONF_REQ,			  EV_MPC_MPCD_BGN + 100 );
OSPEVENT( MPC_MPCD_DELAYCONF_ACK,			  EV_MPC_MPCD_BGN + 101 );
OSPEVENT( MPC_MPCD_DELAYCONF_NACK,			  EV_MPC_MPCD_BGN + 102 );

//MCU�ӳ�����֪ͨ, ��Ϣ��: 1 u16 (�������ӳ�ʱ�䣬��λ������)
OSPEVENT( MCU_MPCD_DELAYCONF_NOTIF,           EV_MPC_MPCD_BGN + 103 );	

//����ģʽ֪ͨ,��Ϣ�壺TConfMode
OSPEVENT( MCU_MPCD_CONFMODE_NOTIF,            EV_MPC_MPCD_BGN + 104 );

//�������Ϣ֪ͨ����Ϣ�壺TBasicConfInfo
OSPEVENT( MCU_MPCD_SIMCONFINFO_NOTIF,         EV_MPC_MPCD_BGN + 105 );

//MCU���������̨����Ƶ���ϲ���֪ͨ, ��Ϣ�壺  TVMPParam
OSPEVENT( MCU_MPCD_VMPPARAM_NOTIF,	          EV_MPC_MPCD_BGN + 106 );

//mpcd��mcu����նˣ��ն˱������,MPCD_MPC_ADDMT_REQΪ�ն�������ᣩ,��Ϣ�壺
OSPEVENT( MPCD_MPC_ADDMT_PASSIVITY_REQ,	      EV_MPC_MPCD_BGN + 107 );
OSPEVENT( MPC_MPCD_ADDMT_PASSIVITY_ACK,		  EV_MPC_MPCD_BGN + 108 );
OSPEVENT( MPC_MPCD_ADDMT_PASSIVITY_NACK,	  EV_MPC_MPCD_BGN + 109 );

//mpcd��mcu���Ϳ�ʼ��ѯ����,��Ϣ�壺MPCD����MPC�ն˾�������Ϣ�壺TReqHeadInfo + TPollInfo + TMtPollParam����
OSPEVENT( MPCD_MPC_STARTPOLL_CMD,			  EV_MPC_MPCD_BGN + 110 );
//mpcd��mcu����ֹͣ��ѯ����,��Ϣ�壺MPCD����MPC�ն˾�������Ϣ�壺TReqHeadInfo
OSPEVENT( MPCD_MPC_STOPPOLL_CMD,			  EV_MPC_MPCD_BGN + 111 );
//mpcd��mcu������ͣ��ѯ����,��Ϣ�壺MPCD����MPC�ն˾�������Ϣ�壺TReqHeadInfo
OSPEVENT( MPCD_MPC_PAUSEPOLL_CMD,	          EV_MPC_MPCD_BGN + 112 );
//mpcd��mcu���ͼ�����ѯ����,��Ϣ�壺MPCD����MPC�ն˾�������Ϣ�壺TReqHeadInfo
OSPEVENT( MPCD_MPC_RESUMEPOLL_CMD,	          EV_MPC_MPCD_BGN + 113 );

//mpcd��mcu����ָ����ѯλ�������Ϣ�壺TReqHeadInfo + TMt
OSPEVENT( MPCD_MPC_SPECPOLLPOS_REQ,	          EV_MPC_MPCD_BGN + 114 );
OSPEVENT( MPC_MPCD_SPECPOLLPOS_ACK,		      EV_MPC_MPCD_BGN + 115 );
OSPEVENT( MPC_MPCD_SPECPOLLPOS_NACK,	      EV_MPC_MPCD_BGN + 116 );

//mpcd��mcu�����޸���ѯ���������Ϣ�壺TReqHeadInfo + TPollInfo + TMtPollParam����
OSPEVENT( MPCD_MPC_CHANGEPOLLPARAM_CMD,	      EV_MPC_MPCD_BGN + 117 );
//mcu��mpcd������ѯ�����ı�֪ͨ����Ϣ�壺TReqHeadInfo + TPollInfo + TMtPollParam����
OSPEVENT( MPC_MPCD_POLLPARAMCHANGE_NOTIF,	  EV_MPC_MPCD_BGN + 118 );

//mcu��mpcd������ѯ״̬֪ͨ����Ϣ�壺TReqHeadInfo + TPollInfo
OSPEVENT( MPC_MPCD_POLLSTATE_NOTIF,	          EV_MPC_MPCD_BGN + 119 );


//������Ա����
//���ӻ�����Ա, ��Ϣ�壺TMt����
OSPEVENT( MPCD_MPC_ADDMIXMEMBER_CMD,          EV_MPC_MPCD_BGN + 120 );
//�Ƴ�������Ա, ��Ϣ�壺TMt����
OSPEVENT( MPCD_MPC_REMOVEMIXMEMBER_CMD,       EV_MPC_MPCD_BGN + 121 );

//�ն�����������֪ͨ, ��Ϣ�壺TMt(�����ն�)
OSPEVENT( MPC_MPCD_MTAPPLYMIX_NOTIF,          EV_MPC_MPCD_BGN + 122 );

//Mpcd��mcu������������ͨ����ʱ����
OSPEVENT( MPCD_MPC_SETVOICEHINT_CMD,		  EV_MPC_MPCD_BGN + 123 );

//����ͨ����ʱ����״̬֪ͨ
OSPEVENT( MPC_MPCD_VOICEHINT_NTF,	          EV_MPC_MPCD_BGN + 124 );

//mpcd����MCU�ϵĻ����б���ϸ��Ϣ�����Ϣ�壺��
OSPEVENT( MPCD_MCU_LISTALLCONF_CMD,		      EV_MPC_MPCD_BGN + 125 );


//
//���濪ʼʹ������Ϣ
//
//mpcd <--> mcu֮�������Ϣ�����ַ���
OSPEVENT( MAINEV_MPCD_MPC,	                  EV_MPC_MPCD_BGN + 126 );

//mpcd����MCU�ϵĻ����б���ϸ��Ϣ�����Ϣ�壺��
OSPEVENT( MCU_MPCD_ALLMTALIAS_NTF,		      EV_MPC_MPCD_BGN + 127 );


#define MPCD_MPC_SUBEV_BEGIN (u32)65535			//����Ϣ��ʼ��

#ifndef MAKESUBEV_u16
//�����u16�������ʹ��
#define  MAKESUBEV_u16(x)          x - MPCD_MPC_SUBEV_BEGIN
#endif

#ifndef MAKESUBEV_u32
//�����u32�������ʹ��
#define  MAKESUBEV_u32(x)          x
#endif

#ifndef MAKESUBEV_U16_TO_U32
//u16����ϢתΪu32����Ϣ
#define  MAKESUBEV_U16_TO_U32(x)   x + MPCD_MPC_SUBEV_BEGIN 
#endif




const u32 SUBEV_MPC_MPCD_MTSTATUS_NTF      = MPCD_MPC_SUBEV_BEGIN + 1;

const u32 SUBEV_MCU_MPCD_ALLMTALIAS_NTF    = MPCD_MPC_SUBEV_BEGIN + 2;

//�ն���Ϣ�����������,�ն��߼�ͨ����Ϣ֪ͨ
const u32 SUBEV_MCU_MPCD_MTLOGICCHNNL_NTF  = MPCD_MPC_SUBEV_BEGIN + 3;

const u32 SUBEV_MCU_MPCD_CALLMTFAILURE_NTF = MPCD_MPC_SUBEV_BEGIN + 4;		//�����ն�ʧ��֪ͨ
const u32 SUBEV_MCU_MPCD_CALLSUCCESS_NTF   = MPCD_MPC_SUBEV_BEGIN + 5;      //�����ն˳ɹ�֪ͨ

//ָ���Ի���
const u32 SUBEV_MPCD_MCU_SPECDIALOGIST_REQ     = MPCD_MPC_SUBEV_BEGIN + 6;
const u32 SUBEV_MCU_MPCD_SPECDIALOGIST_ACK     = MPCD_MPC_SUBEV_BEGIN + 7;
const u32 SUBEV_MCU_MPCD_SPECDIALOGIST_NACK    = MPCD_MPC_SUBEV_BEGIN + 8;

//ȡ���Ի���
const u32 SUBEV_MPCD_MCU_CANCELDIALOGIST_REQ   = MPCD_MPC_SUBEV_BEGIN + 9;
const u32 SUBEV_MCU_MPCD_CANCELDIALOGIST_ACK   = MPCD_MPC_SUBEV_BEGIN + 10;
const u32 SUBEV_MCU_MPCD_CANCELDIALOGIST_NACK  = MPCD_MPC_SUBEV_BEGIN + 11; 

//webmcc�ܾ��ն˼������֪ͨ
const u32 SUBEV_MPCD_MCU_REFUSEMIX_NTF		   = MPCD_MPC_SUBEV_BEGIN + 12;

//webmcc�޸Ļ��鿪����������
const u32 SUBEV_MPCD_MPC_MDFPUBLICCONF_CMD	   = MPCD_MPC_SUBEV_BEGIN + 13;

//rnn[2014/04/22]����ǽ���
const u32 SUBEV_MPCD_MPC_STARTTVS_REQ		   = MPCD_MPC_SUBEV_BEGIN + 14;
const u32 SUBEV_MPC_MPCD_STARTTVS_ACK		   = MPCD_MPC_SUBEV_BEGIN + 15;
const u32 SUBEV_MPC_MPCD_STARTTVS_NACK		   = MPCD_MPC_SUBEV_BEGIN + 16;

const u32 SUBEV_MPCD_MPC_CHANGETVSPARAM_REQ	   = MPCD_MPC_SUBEV_BEGIN + 17;
const u32 SUBEV_MPC_MPCD_CHANGETVSPARAM_ACK	   = MPCD_MPC_SUBEV_BEGIN + 18;
const u32 SUBEV_MPC_MPCD_CHANGETVSPARAM_NACK   = MPCD_MPC_SUBEV_BEGIN + 19;

const u32 SUBEV_MPCD_MPC_STOPTVS_REQ		   = MPCD_MPC_SUBEV_BEGIN + 20;
const u32 SUBEV_MPC_MPCD_STOPTVS_ACK		   = MPCD_MPC_SUBEV_BEGIN + 21;
const u32 SUBEV_MPC_MPCD_STOPTVS_NACK		   = MPCD_MPC_SUBEV_BEGIN + 22;

const u32 SUBEV_MPC_MPCD_HDUINFO_NTF		   = MPCD_MPC_SUBEV_BEGIN + 23;

const u32 SUBEV_MPCD_MPC_SETHDUVOLUME_NTF	   = MPCD_MPC_SUBEV_BEGIN + 24;	

const u32 SUBEV_MPC_MPCD_PLAYHDUFAIL_NTF	   = MPCD_MPC_SUBEV_BEGIN + 25;
//end


#endif

