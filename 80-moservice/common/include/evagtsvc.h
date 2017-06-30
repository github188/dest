 /*****************************************************************************
   ģ����      : �·���ҵ��
   �ļ���      : evagtsvc.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: �·���ҵ��ʹ���ӿ���Ϣ����
   ����        : ����
   �汾        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2002/07/29  0.9         ����        ����
******************************************************************************/
#ifndef _EV_AGTSVC_H_
#define _EV_AGTSVC_H_

#include "osp.h"
#include "eventid.h"

/**********************************************************
 ����Ϊ�����ҵ��ӿ���Ϣ��20001-22000��
**********************************************************/

//����״̬�ı䣬MCU->AGENT����Ϣ��Ϊ�ṹCDevStatus
OSPEVENT( MCU_AGT_BOARD_STATUSCHANGE,		EV_AGTSVC_BGN );
//DRI��DSI����E1״̬�ı䣬MCU->AGENT����Ϣ��Ϊ�ṹCDevStatus
OSPEVENT( MCU_AGT_LINK_STATUSCHANGE,		EV_AGTSVC_BGN + 1 );
//MTE1״̬�ı䣬MT->AGENT����Ϣ��Ϊ�ṹCDevStatus
OSPEVENT( MT_AGT_E1_STATUSCHANGE,			EV_AGTSVC_BGN + 2 );    //�Ѿ����ã�����ɾ��
//ͬ��Դ״̬�ı䣬MCU->AGENT����Ϣ��Ϊ�ṹCSyncSourceAlarm
OSPEVENT( MCU_AGT_SYNCSOURCE_STATUSCHANGE,  EV_AGTSVC_BGN + 3 );
//��ǰͬ��Դ�����л���MCU->AGENT����Ϣ��Ϊ�ṹCCurrentMasterSync
OSPEVENT( MCU_AGT_SYNCSOURCE_SWITCH,		EV_AGTSVC_BGN + 4 );    //�Ѿ����ã�����ɾ��
//�ڴ�״̬�ı䣬SVC->AGENT����Ϣ��Ϊ���ֽڣ�0������1�쳣
OSPEVENT( SVC_AGT_MEMORY_STATUS,			EV_AGTSVC_BGN + 100 );
//�ļ�ϵͳ״̬�ı䣬SVC->AGENT����Ϣ��Ϊ���ֽڣ�0������1�쳣
OSPEVENT( SVC_AGT_FILESYSTEM_STATUS,		EV_AGTSVC_BGN + 101 );
//����״̬�ı䣬SVC->AGENT����Ϣ��Ϊ�ṹCTaskStatus
OSPEVENT( SVC_AGT_TASK_STATUS,		        EV_AGTSVC_BGN + 102 );
//MAP״̬�ı䣬SVC->AGENT����Ϣ��Ϊ�ṹCMapStatus
OSPEVENT( SVC_AGT_MAP_STATUS,		        EV_AGTSVC_BGN + 103 );  //�Ѿ����ã�����ɾ��
//CCI״̬�ı䣬SVC->AGENT����Ϣ��Ϊ�ṹCCCIStatus
OSPEVENT( SVC_AGT_CCI_STATUS,		        EV_AGTSVC_BGN + 104 );  //�Ѿ����ã�����ɾ��
//��̫��״̬��SVC->AGENT����Ϣ��Ϊ�ṹCEthernetCardStatus
OSPEVENT( SVC_AGT_ETHERNETCARD_STATUS,		EV_AGTSVC_BGN + 105 );  //�Ѿ����ã�����ɾ��
//ý����״̬��SVC->AGENT����Ϣ��Ϊ�ṹCMediaStreamStatus
OSPEVENT( SVC_AGT_MEDIASTREAM_STATUS,		EV_AGTSVC_BGN + 106 );  //�Ѿ����ã�����ɾ��
//�豸����״̬�ı䣬SVC->AGENT����Ϣ��Ϊ�ṹCEqpConnStatus
OSPEVENT( SVC_AGT_EQPCONNECT_STATUS,		EV_AGTSVC_BGN + 107 );  //�Ѿ����ã�����ɾ��
//��Ƶ�����״̬, SVC->AGENT, ��Ϣ��Ϊ���ֽڣ�0������1�쳣
OSPEVENT( SVC_AGT_VIDEOSOURCE_STATUS,		EV_AGTSVC_BGN + 108 );  //�Ѿ����ã�����ɾ��
//����״̬�ı䣬SVC->AGENT����Ϣ��Ϊ�ṹCFanStatus
OSPEVENT( SVC_AGT_FAN_STATUS,		        EV_AGTSVC_BGN + 109 );  //�Ѿ����ã�����ɾ��
//V35״̬�ı䣬SVC->AGENT, ��Ϣ��Ϊ���ֽ� 0������bit0�ز����ʧ��
//bit1�������ʧ��, bit2������ʧ��,bit3�����ն�δ����,bit4����δ׼����
OSPEVENT( SVC_AGT_V35_STATUS,		        EV_AGTSVC_BGN + 110 );  //�Ѿ����ã�����ɾ��
//SDH״̬�ı䣬Guard->AGENT����Ϣ��Ϊ�ṹTSDHStatus
OSPEVENT( SVC_AGT_SDH_STATUS,		        EV_AGTSVC_BGN + 111 );

//��Դ״̬�ı䣬Guard->AGENT����Ϣ��Ϊ�ṹTPowerStatus
OSPEVENT( SVC_AGT_POWER_STATUS,		        EV_AGTSVC_BGN + 112 );
//ģ��״̬�ı䣬Guard->AGENT����Ϣ��Ϊ�ṹTModuleStatus
OSPEVENT( SVC_AGT_MODULE_STATUS,		    EV_AGTSVC_BGN + 113 );
//��Դ����״̬�ı䣬Guard->AGENT����Ϣ��Ϊ�ṹTPowerFanStatus
OSPEVENT( SVC_AGT_POWER_FAN_STATUS,		    EV_AGTSVC_BGN + 114 );

//���鿪ʼ֪ͨ��Ϣ, ��Ϣ��ΪTConfNotify
OSPEVENT( SVC_AGT_CONFERENCE_START,		    EV_AGTSVC_BGN + 115 );
//�������֪ͨ��Ϣ, ��Ϣ��ΪTConfNotify
OSPEVENT( SVC_AGT_CONFERENCE_STOP,		    EV_AGTSVC_BGN + 116 );

//�����ն�֪ͨ��Ϣ, ��Ϣ��ΪTMtNotify 
OSPEVENT( SVC_AGT_ADD_MT,		            EV_AGTSVC_BGN + 117 );
//ɾ���ն�֪ͨ��Ϣ, ��Ϣ��ΪTMtNotify
OSPEVENT( SVC_AGT_DEL_MT,		            EV_AGTSVC_BGN + 118 );
OSPEVENT( SVC_AGT_LED_STATUS,		        EV_AGTSVC_BGN + 119 );

// MCU�������͵������, zgc, 2007-08-28
// ��Ϣ��: u8 byBrdLayer, u8 byBrdSlot
OSPEVENT( SVC_AGT_DISCONNECTBRD_CMD,		EV_AGTSVC_BGN + 120 );

// miaoqingsong  NMS5.0 add
// �������״̬�澯�ϱ���Guard->AGENT����Ϣ��Ϊ�ṹTBoxFanStatus
OSPEVENT( SVC_AGT_BOX_FAN_STATUS,           EV_AGTSVC_BGN + 121 );      
//CPU״̬�ı䣬SVC->AGENT����Ϣ��Ϊ�ṹTMPCCpuStatus
OSPEVENT( SVC_AGT_CPU_STATUS,               EV_AGTSVC_BGN + 122 );    
//�ڴ�״̬�ı䣬SVC->AGENT����Ϣ��Ϊ�ṹTMPCMemoryStatus
OSPEVENT( SVC_AGT_MPCMEMORY_STATUS,         EV_AGTSVC_BGN + 123 );
//��Դ���¶�״̬�ı䣬SVR->AGENT,��Ϣ��Ϊ�ṹTPowerBrdTempStatus
OSPEVENT( SVC_AGT_POWERTEMP_STATUS,         EV_AGTSVC_BGN + 124 );
//MPC2���¶�״̬�ı䣬SVC->AGENT,��Ϣ��Ϊ�ṹTMPC2TempStatus
OSPEVENT( SVC_AGT_MPC2TEMP_STATUS,          EV_AGTSVC_BGN + 125 );
//CPU����״̬�澯�ϱ� Guard->AGENT
OSPEVENT( SVC_AGT_CPU_FAN_STATUS,          EV_AGTSVC_BGN + 126 );
//CPU�¶�״̬�澯�ϱ� Guard->AGENT
OSPEVENT( SVC_AGT_CPUTEMP_STATUS,          EV_AGTSVC_BGN + 127 );
//����״̬�澯�ϱ� Guard->AGENT
OSPEVENT( SVC_AGT_DISKUSE_STATUS,          EV_AGTSVC_BGN + 128 );
//ý��˿ں��ն�license�ϱ�,��Ϣ��:u8:ý����Դ������
OSPEVENT( SVC_AGT_MEDIAPORTRES_STATUS, EV_AGTSVC_BGN + 129 );
//ý��˿ں��ն�license��ȡ
OSPEVENT( AGT_SVC_GETMEDIAPORTRES_CMD, EV_AGTSVC_BGN + 133 );

// NMS5.0 end     

// MCU�����������/�޸�DSC info, zgc, 2007-07-16
OSPEVENT( SVC_AGT_SETDSCINFO_REQ,			EV_AGTSVC_BGN + 130 );
// ���������/�޸�DSC infoӦ��, zgc, 2007-07-16
OSPEVENT( AGT_SVC_SETDSCINFO_ACK,			EV_AGTSVC_BGN + 131 );
// ���������/�޸�DSC info��Ӧ��, zgc, 2007-07-16
OSPEVENT( AGT_SVC_SETDSCINFO_NACK,			EV_AGTSVC_BGN + 132 );

// MCU֪ͨ����GK info, zgc, 2007-07-21
OSPEVENT( SVC_AGT_DSCGKINFO_UPDATE_CMD,			EV_AGTSVC_BGN + 134 );
// ����֪ͨMCU DSC���Ѿ�ע��ɹ�, zgc, 2007-07-21
OSPEVENT( AGT_SVC_DSCREGSUCCEED_NOTIF,		EV_AGTSVC_BGN + 135 );
// MCU֪ͨ�������LoginInfo, zgc, 2007-10-12
OSPEVENT( SVC_AGT_DSCTELNETLOGININFO_UPDATE_CMD,	EV_AGTSVC_BGN + 136 );

// ����״̬֪ͨ,(u8)0:������1:����, 2:δ֪
OSPEVENT( AGT_SVC_BOARDSTATUS_NOTIFY,       EV_AGTSVC_BGN + 150 );
// ����MPC״̬֪ͨ,��MSӦ�÷���MCU����,(u8)1:������2:����
OSPEVENT( SVC_AGT_STANDBYMPCSTATUS_NOTIFY,  EV_AGTSVC_BGN + 151 );


//Mcu ֪ͨ�������ܸı�֪ͨ
OSPEVENT( MCU_AGT_PFM_INFOCHANGE,			EV_AGTSVC_BGN + 152 );

//��������ϣ�SVC->AGENT
OSPEVENT( SVC_AGT_COLD_RESTART,				EV_AGTSVC_BGN + 200 );
//�ػ���SVC->AGENT
OSPEVENT( SVC_AGT_POWEROFF,					EV_AGTSVC_BGN + 201 );
//���ó���
OSPEVENT( SVC_AGT_CONFIG_ERROR,				EV_AGTSVC_BGN + 202 );

//mcu֪ͨ������µ���汾(�����Լ�)
//��Ϣ�壺u8(��������) + u8(Դ�ļ�������) + u8(�ļ�������) + s8[](�ļ���)  //ע��(�ļ�����������·��)
OSPEVENT( SVC_AGT_UPDATEBRDVERSION_CMD,     EV_AGTSVC_BGN + 205 );
//���µ�DSC��ע������(8000B), zgc, 2007-03-05
//��Ϣ�壺u8(��������) + u32(DSC��IP,������) + u32(DSC�����ӵı���MCU��IP, ������) + u32(����IP MASK, ������) 
OSPEVENT( AGT_SVC_NEWDSCREGREQ_NOTIFY,		EV_AGTSVC_BGN + 206);
//�汾���½��֪ͨ����Ϣ�壺u8(��������) + u8(1-�ɹ���0-ʧ��)
OSPEVENT( AGT_SVC_UPDATEBRDVERSION_NOTIF,   EV_AGTSVC_BGN + 208 );

//����DSC������°汾, zgc, 2007-08-20
//��Ϣ�壺u8(��������) + u8(��ػỰ����) + u8(Դ�ļ�������) + u8(�ļ�������) + s8[](�ļ���)  //ע��(�ļ�����������·��)
OSPEVENT( SVC_AGT_STARTUPDATEDSCVERSION_REQ, EV_AGTSVC_BGN + 209 );
//��Ϣ�壺ͬ��
OSPEVENT( AGT_SVC_STARTUPDATEDSCVERSION_ACK, EV_AGTSVC_BGN + 210 );
//��Ϣ�壺ͬ��
OSPEVENT( AGT_SVC_STARTUPDATEDSCVERSION_NACK,EV_AGTSVC_BGN + 211 );

//��Ϣ��: u8(��������) + u8(��ػỰ����) + TDscUpdateReqHead + [filecontent]����
OSPEVENT( SVC_AGT_DSCUPDATEFILE_REQ,		EV_AGTSVC_BGN + 213 );
//��Ϣ��: u8(��������) + u8(��ػỰ����) + TDscUpdateRsp
OSPEVENT( AGT_SVC_DSCUPDATEFILE_ACK,		EV_AGTSVC_BGN + 214 );
//��Ϣ��: u8(��������) + u8(��ػỰ����) + TDscUpdateRsp
OSPEVENT( AGT_SVC_DSCUPDATEFILE_NACK,	EV_AGTSVC_BGN + 215 );

//MCU֪ͨ������ע��ɹ�DSC�彨��·��, zq, 2008-07-14
//��Ϣ�壺u8(������) + u8(����ۺ�) + u32(IP����) + u32(���IP1 ������) + u32(���IP2 ������) + ��������������������
OSPEVENT( SVC_AGT_DSCCREATEROUTE_CMD,		EV_AGTSVC_BGN + 216 );

//MCU֪ͨ�����ض���DSC��ɾ��·��, zq, 2008-07-14
//��Ϣ�壺u8(������) + u8(����ۺ�) + u32(���IP ������)
OSPEVENT( SVC_AGT_DSCDELROUTE_CMD,		EV_AGTSVC_BGN + 218 );

//��mcu֪ͨ����mcu��LED���Ե�notify
OSPEVENT( SVC_AGT_LEDSTATUS_NOTIFY,		EV_AGTSVC_BGN + 219 );

//��������֪ͨ
//��Ϣ��: u8(������) + u8(����ۺ�) + u8(�������)+[u8(��������)+����������Ϣ�ṹ+����]+u8(E1_CLK_OUTPUT_ENABLE)+u8(��ͬ��ʱ�Ӳο�ѡ��)+u32(����IP)+u8(ʹ�ÿ��Ź���־)
OSPEVENT( SVC_AGT_BOARDCFGMODIFY_NOTIF,		EV_AGTSVC_BGN + 217 );
//E1��·����֪ͨ
OSPEVENT( AGT_SVC_E1BANDWIDTH_NOTIF,		EV_AGTSVC_BGN + 218 );

//����������������Ϣ������Ϣ��
OSPEVENT( AGT_SVC_REBOOT,					EV_AGTSVC_BGN + 300 );
//�������������Ϣ������Ϣ��
OSPEVENT( AGT_SVC_POWEROFF,					EV_AGTSVC_BGN + 301 );
//�������迪����Ϣ������Ϣ��
OSPEVENT( AGT_SVC_POWERON,					EV_AGTSVC_BGN + 302 );

//�����ģ�����Ϣ��
OSPEVENT( AGT_SVC_PFMINFO_REQ,				EV_AGTSVC_BGN + 303 );


/*������MCU AGENT����������Ϣ*/

//�����ע����Ϣ����Ϣ��ΪTBrdPosition+u32(����IP)+u8(���ں�)+u8(OsType)
OSPEVENT( BOARD_MPC_REG,					EV_AGTSVC_BGN + 401 );
//�����ע��Ӧ��
OSPEVENT( MPC_BOARD_REG_ACK,				EV_AGTSVC_BGN + 402 );
//�����ע���Ӧ��
OSPEVENT( MPC_BOARD_REG_NACK,				EV_AGTSVC_BGN + 403 );


//����ȡ���õ���Ϣ
OSPEVENT( BOARD_MPC_GET_CONFIG,				EV_AGTSVC_BGN + 404 );
//����ȡ����Ӧ����Ϣ
//��Ϣ��Ϊ u8(�������)+[u8(��������)+����������Ϣ�ṹ+����]+u8(E1_CLK_OUTPUT_ENABLE)+u8(��ͬ��ʱ�Ӳο�ѡ��)+u32(����IP)+u8(ʹ�ÿ��Ź���־)
OSPEVENT( MPC_BOARD_GET_CONFIG_ACK,			EV_AGTSVC_BGN + 405 );
//����ȡ���÷�Ӧ����Ϣ
OSPEVENT( MPC_BOARD_GET_CONFIG_NACK,		EV_AGTSVC_BGN + 406 );

//����澯ͬ����Ϣ
OSPEVENT( MPC_BOARD_ALARM_SYNC_REQ,			EV_AGTSVC_BGN + 407 );
//����澯ͬ����Ϣ��Ӧ��
//��Ϣ��Ϊ sizeof(TBrdPosition)  + sizeof(u16)[ �澯����] + �澯���� * sizeof(TBoardAlarmMsgInfo)
OSPEVENT( BOARD_MPC_ALARM_SYNC_ACK,			EV_AGTSVC_BGN + 408 );
//�����MPC�ĸ澯֪ͨ
//��Ϣ��Ϊ sizeof(TBrdPosition)  + sizeof(u16)[ �澯����] + �澯���� * sizeof(TBoardAlarmMsgInfo)
OSPEVENT( BOARD_MPC_ALARM_NOTIFY,			EV_AGTSVC_BGN + 409 );


//MPC�õ������������Ե���Ϣ
OSPEVENT( MPC_BOARD_BIT_ERROR_TEST_CMD,		EV_AGTSVC_BGN + 410 );
//MPC�õ������ʱ��ͬ������Ϣ
OSPEVENT( MPC_BOARD_TIME_SYNC_CMD,			EV_AGTSVC_BGN + 411 );
//MPC�õ�������Բ����Ϣ
OSPEVENT( MPC_BOARD_SELF_TEST_CMD,			EV_AGTSVC_BGN + 412 );
//MPC�õ����������������Ϣ
OSPEVENT( MPC_BOARD_RESET_CMD,			    EV_AGTSVC_BGN + 413 );


//MPC��ȡ�����ͳ����Ϣ����Ϣ  (��Ӧ��ͬ�ĵ��壬ͳ����Ϣ�ṹ���ܲ�һ��)
OSPEVENT( MPC_BOARD_GET_STATISTICS_REQ,		EV_AGTSVC_BGN + 414 );
//�����MPC��ͳ����ϢӦ�� 
OSPEVENT( BOARD_MPC_GET_STATISTICS_ACK,		EV_AGTSVC_BGN + 415 );

//MPC��ȡ����ģ����Ϣ����Ϣ
OSPEVENT( MPC_BOARD_GET_MODULE_REQ,			EV_AGTSVC_BGN + 417 );
//�����MPC��ģ����ϢӦ��
OSPEVENT( BOARD_MPC_GET_MODULE_ACK,			EV_AGTSVC_BGN + 418 );
//����E1����ָʾ
OSPEVENT( BOARD_MPC_E1BANDWIDTH_NOTIF,		EV_AGTSVC_BGN + 419 );


//MPC��ȡ����汾��Ϣ����Ϣ
OSPEVENT( MPC_BOARD_GET_VERSION_REQ,		EV_AGTSVC_BGN + 420 );
//�����MPC�İ汾��ϢӦ��
OSPEVENT( BOARD_MPC_GET_VERSION_ACK,		EV_AGTSVC_BGN + 421 );


//�������������Ϣ
OSPEVENT( BOARD_AGENT_POWERON,			    EV_AGTSVC_BGN + 423 );
//����GUARDģ��������Ϣ
OSPEVENT( BOARD_GUARD_POWERON,              EV_AGTSVC_BGN + 424 );

//MPC�õ������������µ���Ϣ
//��Ϣ��: u8(��������) + u8(Դ�ļ�������) + u8(�ļ�������) + s8[](�ļ���)  //ע��(linux�ļ�����������·��)
OSPEVENT( MPC_BOARD_UPDATE_SOFTWARE_CMD,	EV_AGTSVC_BGN + 425 );

//�汾���½��֪ͨ����Ϣ�壺u8(��������) + u8(�ļ���) + u8[](�Ƿ�ɹ�<�Ի��>) 
// + u8(�ļ�����) + s8[](�ļ���) + u8 + s8[] + ...
OSPEVENT( BOARD_MPC_UPDATE_NOTIFY,			EV_AGTSVC_BGN + 426 );

//MPC���͸������E1����������Ϣ
OSPEVENT( MPC_BOARD_E1_LOOP_CMD,			EV_AGTSVC_BGN + 428 );

/************************************************************************/
/*  �����ǵ�������(GUARD)��ʱɨ������ĵ���״̬�ı���Ϣ                 */
/************************************************************************/
//����E1״̬�ı���Ϣ
OSPEVENT( BOARD_LINK_STATUS,			    EV_AGTSVC_BGN + 429 );
//����ģ��״̬�ı���Ϣ
OSPEVENT( BOARD_MODULE_STATUS,              EV_AGTSVC_BGN + 430 );
//��������״̬��Ϣ
OSPEVENT( BOARD_LED_STATUS,                 EV_AGTSVC_BGN + 431 );
//�������״̬��Ϣ
OSPEVENT( MCU_BRD_FAN_STATUS,               EV_AGTSVC_BGN + 450 );

//MPC���͸������ȡ���������״̬��Ϣ
OSPEVENT( MPC_BOARD_LED_STATUS_REQ,         EV_AGTSVC_BGN + 432 );
//�����MPC��ȡ���������״̬Ӧ����Ϣ
OSPEVENT( BOARD_MPC_LED_STATUS_ACK,         EV_AGTSVC_BGN + 433 );
//��������״̬��Ϣ
OSPEVENT( BOARD_MPC_LED_STATUS_NOTIFY,      EV_AGTSVC_BGN + 434 );

//Board register information to report to mcu
OSPEVENT( AGENT_MCU_REGISTER_NOTIFY,        EV_AGTSVC_BGN + 435 );
//��ʾ����ע�ᵥ��
OSPEVENT( EV_AGENT_SHOWREGEDBOARD,          EV_AGTSVC_BGN + 436 );

//MPC֪ͨ������Ϣ�޸�
//��Ϣ��: TDSCModuleInfo
OSPEVENT( MPC_BOARD_SETDSCINFO_REQ,         EV_AGTSVC_BGN + 437 );
OSPEVENT( BOARD_MPC_SETDSCINFO_ACK,         EV_AGTSVC_BGN + 438 );
OSPEVENT( BOARD_MPC_SETDSCINFO_NACK,        EV_AGTSVC_BGN + 439 );

//�����¶��쳣״̬֪ͨ(����guard->����agent)
OSPEVENT(BOARD_TEMPERATURE_STATUS_NOTIF,			EV_AGTSVC_BGN + 440);
//�����¶��쳣״̬֪ͨ��0:���� 1:�쳣��
OSPEVENT(SVC_AGT_BRD_TEMPERATURE_STATUS_NOTIFY,        EV_AGTSVC_BGN + 441);
//����CPUռ����״̬֪ͨ(0:���� 1:ռ�ù��� ����guard->����agent)
OSPEVENT(BOARD_CPU_STATUS_NOTIF,			EV_AGTSVC_BGN + 442); 
//����CPUռ����״̬֪ͨ(0:���� 1:ռ�ù���)
OSPEVENT(SVC_AGT_BRD_CPU_STATUS_NOTIF,			EV_AGTSVC_BGN + 443); 

//MPC�������DSCGKINFO, ��Ϣ�壺u32(������), zgc, 2007-07-21
OSPEVENT( MPC_BOARD_DSCGKINFO_UPDATE_CMD,	EV_AGTSVC_BGN + 444 );
//MPC����͵������, zgc, 2007-08-28
OSPEVENT( MPC_BOARD_DISCONNECT_CMD,			EV_AGTSVC_BGN + 445 );
//���������޸�ͳ��, zgc, 2007-09-24
OSPEVENT( MCU_BOARD_CONFIGMODIFY_NOTIF,	EV_AGTSVC_BGN + 451 );
//MPC����DSC����LoginInfo, zgc, 2007-10-12
OSPEVENT( MCU_BOARD_DSCTELNETLOGININFO_UPDATE_CMD, EV_AGTSVC_BGN + 452 );

//MPC֪ͨDSC����ע��ɹ���Ҫ����·��, zq, 2008-07-14
//��Ϣ�壺u8(������) + u8(����ۺ�) + u32(IP����) + u32(���IP1 ������) + u32(���IP2 ������) + ��������������������
OSPEVENT( MPC_BOARD_DSCCREATEROUTE_CMD,	EV_AGTSVC_BGN + 460 );

//MPC֪ͨDSC���ض�����Ҫɾ��·��, zq, 2008-07-14
//��Ϣ�壺u8(������) + u8(����ۺ�) + u32(msc IP��ַ)
OSPEVENT( MPC_BOARD_DSCDELROUTE_CMD,	EV_AGTSVC_BGN + 462 );

//����֪ͨMPC������汾�� [12/13/2011 chendaiwei]
//OSPEVENT( BOARD_MPC_SOFTWARE_VERSION_NOTIFY, EV_AGTSVC_BGN + 463 );
//����֪ͨMPC������汾�� [12/13/2011 chendaiwei]
//OSPEVENT(SVC_AGT_BRD_SOFTWARE_VERSION_NOTIFY, EV_AGTSVC_BGN + 464);

// ��ʼ��������
OSPEVENT( EV_MSGCENTER_POWER_ON,            EV_AGTSVC_BGN + 500 );
// �ػ�֪ͨ
OSPEVENT( EV_MSGCENTER_POWER_OFF,           EV_AGTSVC_BGN + 501 );
// ������
OSPEVENT( EV_AGENT_COLD_RESTART,            EV_AGTSVC_BGN + 502 );
// ��������mcu
OSPEVENT( EV_AGENT_MCU_MCU_RESTART,         EV_AGTSVC_BGN + 503 );
//������������mcu
OSPEVENT( EV_AGENT_MCU_MCU_UPDATE,          EV_AGTSVC_BGN + 504 );
// Led �澯
OSPEVENT( EV_BOARD_LED_ALARM,               EV_AGTSVC_BGN + 505 );
// ɾ������澯
OSPEVENT( EV_DELETEBOARD_ALARM,             EV_AGTSVC_BGN + 506 );
// ��ʼ��Snmp��Ϣ
OSPEVENT( EV_TOMSGCENTER_INITAL_SNMPINFO,   EV_AGTSVC_BGN + 510 );
// �����������Mcu����
OSPEVENT( EV_BOARD_MPC_CONNECT,             EV_AGTSVC_BGN + 511 );
// ���������Mcu����ע��
OSPEVENT( EV_BOARD_MPC_REGISTER,            EV_AGTSVC_BGN + 512 );
// MPC ���������֪ͨ
OSPEVENT( EV_MSGCENTER_MPC_UPDATED_NTF,     EV_AGTSVC_BGN + 513 );
// ��ͨ�����������֪ͨ
OSPEVENT( EV_MSGCENTER_BRD_UPDATED_NTF,     EV_AGTSVC_BGN + 514 );

// ����ftp��ʽ��������
OSPEVENT( EV_MSGCENTER_NMS_UPDATEDMPC_CMD,  EV_AGTSVC_BGN + 515 );
// ����ftp��ʽ��������
//OSPEVENT( EV_SELFCENTER_NMS_UPDATEDMPC_CMD,  EV_AGTSVC_BGN + 531 );

// mpc����DSC���°汾, zgc, 2007-08-20
//��Ϣ��:  u8(��������) + u8(��ػỰ����) + u8(Դ�ļ�������) + u8(�ļ�������) + s8[](�ļ���)  //ע��(linux�ļ�����������·��)
OSPEVENT( MPC_DSC_STARTUPDATE_SOFTWARE_REQ,	EV_AGTSVC_BGN + 516 );
//��Ϣ��:  ͬ��
OSPEVENT( DSC_MPC_STARTUPDATE_SOFTWARE_ACK,	EV_AGTSVC_BGN + 517 );
//��Ϣ��:  ͬ��
OSPEVENT( DSC_MPC_STARTUPDATE_SOFTWARE_NACK,EV_AGTSVC_BGN + 518 );

//��Ϣ��: u8(��������) + u8(��ػỰ����) + TDscUpdateReqHead + [filecontent]����
OSPEVENT( MPC_DSC_UPDATEFILE_REQ,			EV_AGTSVC_BGN + 519 );
//��Ϣ��: u8(��������) + u8(��ػỰ����) + TDscUpdateRsp
OSPEVENT( DSC_MPC_UPDATEFILE_ACK,			EV_AGTSVC_BGN + 520 );
//��Ϣ��: u8(��������) + u8(��ػỰ����) + TDscUpdateRsp
OSPEVENT( DSC_MPC_UPDATEFILE_NACK,			EV_AGTSVC_BGN + 521 );
//��Ϣ��: 
OSPEVENT( SVC_AGT_MCULED_ALARM,		        EV_AGTSVC_BGN + 522 );
// ������ʱ��
OSPEVENT( DSC_UPDATE_SOFTWARE_WAITTIMER,	EV_AGTSVC_BGN + 530 );

// ����ftp��ʽ��������
OSPEVENT( EV_SM_UPDATEDMPC_CMD,				EV_AGTSVC_BGN + 531 );

// ��ʱ��������Ƿ�ϵ�
OSPEVENT( EV_ETH_STATDETECT_TIME,			EV_AGTSVC_BGN + 533 );

// ֪ͨmcucfg���ڷ����б�
OSPEVENT( AGT_MCU_CFG_ETHCHANGE_NOTIF,		EV_AGTSVC_BGN + 534 );

OSPEVENT( BOARD_MPC_CFG_TEST,				EV_AGTSVC_BGN + 1000 );
OSPEVENT( BOARD_MPC_GET_ALARM,				EV_AGTSVC_BGN + 1001 );
OSPEVENT( BOARD_MPC_QUERY_ALARM,			EV_AGTSVC_BGN + 1002 );
OSPEVENT( BOARD_MPC_MANAGERCMD_TEST,        EV_AGTSVC_BGN + 1003 );
OSPEVENT( TEST_BOARDMANAGERCMD_DUMPINST,    EV_AGTSVC_BGN + 1004 );

//  xsl [6/9/2006] ��������ö�ʱ��id
OSPEVENT( BRDAGENT_CONNECT_MANAGERA_TIMER,  EV_AGTSVC_BGN + 1200 );
OSPEVENT( BRDAGENT_CONNECT_MANAGERB_TIMER,  EV_AGTSVC_BGN + 1201 );
OSPEVENT( BRDAGENT_REGISTERA_TIMER,         EV_AGTSVC_BGN + 1202 );
OSPEVENT( BRDAGENT_REGISTERB_TIMER,         EV_AGTSVC_BGN + 1203 );
OSPEVENT( BRDAGENT_GET_CONFIG_TIMER,        EV_AGTSVC_BGN + 1204 );
OSPEVENT( BRDAGENT_SCAN_STATE_TIMER,        EV_AGTSVC_BGN + 1205 );

OSPEVENT( BRDAGENT_CONNECT_TEST_TIMER,      EV_AGTSVC_BGN + 1207 );
OSPEVENT( BRDAGENT_CONNECT_TEST_OVER_TIMER,		EV_AGTSVC_BGN + 1208 ); // zgc, 2007-03-15

// [11/25/2010 xliang] new ID
OSPEVENT( BRDAGENT_CONNECT_MANAGER_TIMER,   EV_AGTSVC_BGN + 1210);
OSPEVENT( BRDAGENT_REGISTER_TIMER,			EV_AGTSVC_BGN + 1211);
OSPEVENT( BRDAGENT_E1CHECK_TIMER,           EV_AGTSVC_BGN + 1212);      //E1��·��ⶨʱ��

//mcu�����ö�ʱ��
OSPEVENT( MCUAGENT_SCAN_STATE_TIMER,        EV_AGTSVC_BGN + 1220 );
OSPEVENT( MCUAGENT_PRINT_CONFIG_ERROR_TIMER,EV_AGTSVC_BGN + 1221 );
OSPEVENT( MCUAGENT_UPDATE_MCUPFMINFO_TIMER, EV_AGTSVC_BGN + 1222 );     // �������pfmInfo
OSPEVENT( MCUAGENT_OPENORCLOSE_MPCLED_TIMER,EV_AGTSVC_BGN + 1223 );     // MPC��NMS��״̬ mqs 20101118 add

// add for IS2.2�� 8548 sys(MASTER) and 8313 sys(SLAVE) ��[05/04/2012 liaokang]
/*  ������IS2.2����ϵͳ������Ϣ��������ע�ᡢ��ʱɨ�輰״̬��ȡ  */
// ����
OSPEVENT( BRDSYS_SLAVE_MASTER_CONNECT_TIMER,EV_AGTSVC_BGN + 1224 );     // 8313
// ע��
OSPEVENT( BRDSYS_SLAVE_MASTER_REG_REQ,      EV_AGTSVC_BGN + 1225 );     // 8313 -> 8548 BoardAgent
OSPEVENT( BRDSYS_MASTER_SLAVE_REG_ACK,      EV_AGTSVC_BGN + 1226 );     // 8548 BoardAgent -> 8313
OSPEVENT( BRDSYS_MASTER_SLAVE_REG_NACK,     EV_AGTSVC_BGN + 1227 );     // 8548 BoardAgent -> 8313
OSPEVENT( BRDSYS_SLAVE_MASTER_REG_TIMER,    EV_AGTSVC_BGN + 1228 );     // 8313
// ��ʱɨ��
OSPEVENT( BRDSYS_SLAVE_SCAN_STATE_TIMER,    EV_AGTSVC_BGN + 1229 );     // 8313
// LED״̬
OSPEVENT( BRDSYS_SLAVE_MASTER_LED_STATUS_NOTIFY,EV_AGTSVC_BGN + 1230 ); // 8313 -> 8548 BoardAgent
// ����״̬	
OSPEVENT( BRDSYS_SLAVE_MASTER_ETHPORT_STATUS_NOTIFY,EV_AGTSVC_BGN + 1231 );// 8313 -> 8548 BoardAgent
/*  ������McuAgent��BoardAgent������Ϣ����ȡ����״̬  */
OSPEVENT( BOARD_ETHPORT_STATUS,             EV_AGTSVC_BGN + 1232 );     // BoardGuard -> BoardAgent
OSPEVENT( MPC_BOARD_ETHPORT_STATUS_REQ,     EV_AGTSVC_BGN + 1233 );     // McuAgent brdmanager -> BoardAgent
OSPEVENT( BOARD_MPC_ETHPORT_STATUS_ACK,     EV_AGTSVC_BGN + 1234 );     // BoardAgent -> McuAgent brdmanager
OSPEVENT( BOARD_MPC_ETHPORT_STATUS_NOTIFY,  EV_AGTSVC_BGN + 1235 );     // BoardAgent -> McuAgent brdmanager
OSPEVENT( EV_BOARD_ETHPORT_ALARM,           EV_AGTSVC_BGN + 1236 );     // McuAgent brdmanager -> McuAgent agentmsgcenter
OSPEVENT( EV_CHANGE_ETH_NOTIF,				EV_AGTSVC_BGN + 1237 );     // 8000H-M: eth has changed [pengguofeng 6/6/2013]

#endif /* _EV_AGTSVC_H_ */
