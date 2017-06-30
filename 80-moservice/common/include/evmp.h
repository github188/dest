 /*****************************************************************************
   ģ����      : MP
   �ļ���      : evmp.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: MP��MCU����Ϣ�ӿڶ���
   ����        : ������
   �汾        : V0.1  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2003/07/11  0.1         ������        ����
******************************************************************************/
#ifndef __EVMP_H_
#define __EVMP_H_

#include "osp.h"
#include "eventid.h"

//MP��MCU��ע������,��Ϣ��ΪTMp
OSPEVENT(MP_MCU_REG_REQ,                    EV_MP_BGN);
//MCU����MP��ע������,��Ϣ��ΪTMp
OSPEVENT(MCU_MP_REG_ACK,                    EV_MP_BGN+1);
//MCU�ܾ�MP��ע������,��Ϣ����
OSPEVENT(MCU_MP_REG_NACK,                   EV_MP_BGN+2);

//MCU����MP�����ӽ�������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MCU_MP_ADDSWITCH_REQ,              EV_MP_BGN+4);
//MP����MCU�����ӽ�������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_ADDSWITCH_ACK,              EV_MP_BGN+5);
//MP�ܾ�MCU�����ӽ�������,��Ϣ����
OSPEVENT(MP_MCU_ADDSWITCH_NACK,             EV_MP_BGN+6);

//MCU����MP���Ƴ���������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MCU_MP_REMOVESWITCH_REQ,           EV_MP_BGN+8);
//MP����MCU���Ƴ���������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_REMOVESWITCH_ACK,           EV_MP_BGN+9);
//MP�ܾ�MCU���Ƴ���������,��Ϣ����
OSPEVENT(MP_MCU_REMOVESWITCH_NACK,          EV_MP_BGN+10);

//MCU����MP�ĵõ�����״̬����,��Ϣ��ΪTMp
OSPEVENT(MCU_MP_GETSWITCHSTATUS_REQ,        EV_MP_BGN+12);
//MP����MCU�ĵõ�״̬Ӧ��,��Ϣ��ΪTSwitchChannel����
OSPEVENT(MP_MCU_GETSWITCHSTATUS_ACK,        EV_MP_BGN+13);
//MP�ܾ�Ӧ��MCU�ĵõ�״̬����,��Ϣ����
OSPEVENT(MP_MCU_GETSWITCHSTATUS_NACK,       EV_MP_BGN+14);

//MP��ʼ����MCU,��Ϣ����
OSPEVENT(EV_MP_CONNECT_MCU,                 EV_MP_BGN+16);
//MP��ʼ����MCU,��Ϣ����
OSPEVENT(EV_MP_CONNECT_MCUB,                EV_MP_BGN+17);
//MP��ʼ��MCUע�ᣬ��Ϣ����
OSPEVENT(EV_MP_REGISTER_MCU,                EV_MP_BGN+18);
//MP��ʼ��MCUע�ᣬ��Ϣ����
OSPEVENT(EV_MP_REGISTER_MCUB,               EV_MP_BGN+19);

//MP�Ự֪ͨMCU����
OSPEVENT(MCU_MP_DISCONNECTED_NOTIFY,        EV_MP_BGN+20);

//MCU֪ͨMP�������
OSPEVENT(MCU_MP_RELEASECONF_NOTIFY,         EV_MP_BGN+22);

// �����һ��֪ͨ
OSPEVENT(MCU_MP_CONFUNIFORMMODE_NOTIFY,     EV_MP_BGN+23);

// MP ȡ��������״̬
OSPEVENT(MP_MCU_GETMSSTATUS_REQ,            EV_MP_BGN+25);
// Mcu֪ͨMp��������״̬ ��Ϣ�壺TMcuMsStatus
OSPEVENT(MCU_MP_GETMSSTATUS_ACK,            EV_MP_BGN+26);
// ��Ϣ������
OSPEVENT(MCU_MP_GETMSSTATUS_NACK,           EV_MP_BGN+27);

//������mp�Լ���������֪ͨ
OSPEVENT(MCU_MP_SELFRELEASECONF_NOTIFY,     EV_MP_BGN+28);

//MCU����MP�����Ӷ�㵽һ�㽻������,��Ϣ��ΪTSwitchChannel + dwMapIpAddr + wMapPort
OSPEVENT(MCU_MP_ADDMULTITOONESWITCH_REQ,    EV_MP_BGN+30);
//MP����MCU�����Ӷ�㵽һ�㽻������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_ADDMULTITOONESWITCH_ACK,    EV_MP_BGN+31);
//MP�ܾ�MCU�����Ӷ�㵽һ�㽻������,��Ϣ����
OSPEVENT(MP_MCU_ADDMULTITOONESWITCH_NACK,   EV_MP_BGN+32);

//MCU����MP���Ƴ���㵽һ�㽻������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MCU_MP_REMOVEMULTITOONESWITCH_REQ, EV_MP_BGN+35);
//MP����MCU���Ƴ���㵽һ�㽻������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_REMOVEMULTITOONESWITCH_ACK, EV_MP_BGN+36);
//MP�ܾ�MCU���Ƴ���㵽һ�㽻������,��Ϣ����
OSPEVENT(MP_MCU_REMOVEMULTITOONESWITCH_NACK,EV_MP_BGN+37);

//MCU����MP��ֹͣ��㵽һ�㽻������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MCU_MP_STOPMULTITOONESWITCH_REQ,   EV_MP_BGN+40);
//MP����MCU��ֹͣ��㵽һ�㽻������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_STOPMULTITOONESWITCH_ACK,   EV_MP_BGN+41);
//MP�ܾ�MCU��ֹͣ��㵽һ�㽻������,��Ϣ����
OSPEVENT(MP_MCU_STOPMULTITOONESWITCH_NACK,  EV_MP_BGN+42);

//MCU����MP������ֻ���ս�������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MCU_MP_ADDRECVONLYSWITCH_REQ,      EV_MP_BGN+45);
//MP����MCU������ֻ���ս�������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_ADDRECVONLYSWITCH_ACK,      EV_MP_BGN+46);
//MP�ܾ�MCU������ֻ���ս�������,��Ϣ����
OSPEVENT(MP_MCU_ADDRECVONLYSWITCH_NACK,     EV_MP_BGN+47);

//MCU����MP���Ƴ�ֻ���ս�������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MCU_MP_REMOVERECVONLYSWITCH_REQ,   EV_MP_BGN+50);
//MP����MCU���Ƴ�ֻ���ս�������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_REMOVERECVONLYSWITCH_ACK,   EV_MP_BGN+51);
//MP�ܾ�MCU���Ƴ�ֻ���ս�������,��Ϣ����
OSPEVENT(MP_MCU_REMOVERECVONLYSWITCH_NACK,  EV_MP_BGN+52);

//MCU����MP�ı�����ս���ssrc����,��Ϣ��ΪTSwitchChannel+byChangeSSRC(u8)
OSPEVENT(MCU_MP_SETRECVSWITCHSSRC_REQ,      EV_MP_BGN+55);
//MP����MCU�ı�����ս���ssrc����,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_SETRECVSWITCHSSRC_ACK,      EV_MP_BGN+56);
//MP�ܾ�MCU�ı�����ս���ssrc����,��Ϣ����
OSPEVENT(MP_MCU_SETRECVSWITCHSSRC_NACK,     EV_MP_BGN+57);

//MCU����MP�����ӹ㲥Դ����,��Ϣ��ΪTSwitchChannel
OSPEVENT(MCU_MP_ADDBRDSRCSWITCH_REQ,        EV_MP_BGN+60);
//MP����MCU�����ӹ㲥Դ����,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_ADDBRDSRCSWITCH_ACK,        EV_MP_BGN+61);
//MP�ܾ�MCU�����ӹ㲥Դ����,��Ϣ����
OSPEVENT(MP_MCU_ADDBRDSRCSWITCH_NACK,       EV_MP_BGN+62);

//MP ĳ��ת�㲥Դ �ͷź��MCU��֪ͨ,��Ϣ��Ϊ TMt(�ͷ�ǰ����ת������ƵԴ)
OSPEVENT( MP_MCU_BRDSRCSWITCHCHANGE_NTF,        EV_MP_BGN + 49 );

//MCU����MP���Ƴ��㲥Դ����,��Ϣ��ΪTSwitchChannel
OSPEVENT(MCU_MP_REMOVEBRDSRCSWITCH_REQ,     EV_MP_BGN+65);
//MP����MCU���Ƴ��㲥Դ����,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_REMOVEBRDSRCSWITCH_ACK,     EV_MP_BGN+66);
//MP�ܾ�MCU���Ƴ��㲥Դ����,��Ϣ����
OSPEVENT(MP_MCU_REMOVEBRDSRCSWITCH_NACK,    EV_MP_BGN+67);

//MCU����MP�����ӹ㲥Ŀ������,��Ϣ��ΪTSwitchChannel
OSPEVENT(MCU_MP_ADDBRDDSTSWITCH_REQ,        EV_MP_BGN+70);
//MP����MCU�����ӹ㲥Դ����,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_ADDBRDDSTSWITCH_ACK,        EV_MP_BGN+71);
//MP�ܾ�MCU�����ӹ㲥Դ����,��Ϣ����
OSPEVENT(MP_MCU_ADDBRDDSTSWITCH_NACK,       EV_MP_BGN+72);

/*
//MCU����MP���Ƴ��㲥Դ����,��Ϣ��ΪTSwitchChannel
OSPEVENT(MCU_MP_REMOVEBRDDSTSWITCH_REQ,     EV_MP_BGN+75);
//MP����MCU���Ƴ��㲥Դ����,��Ϣ��ΪTSwitchChannel
OSPEVENT(MP_MCU_REMOVEBRDDSTSWITCH_ACK,     EV_MP_BGN+76);
//MP�ܾ�MCU���Ƴ��㲥Դ����,��Ϣ����
OSPEVENT(MP_MCU_REMOVEBRDDSTSWITCH_NACK,    EV_MP_BGN+77);
*/


//֪ͨMp����Ϣ ��Ϣ��:u32(LocalIp) + u16(LocalPort) + u32(RemoteIp) + u16(RemotePort) + u8(HoleType) + u8(PayLoad)
OSPEVENT( MCU_MP_PINHOLE_NOTIFY,			EV_MP_BGN + 73);
//֪ͨMp������Ϣ ��Ϣ��: u8(������) + u16(LocalPort) + u32(RemoteIp) + u16(RemotePort) + ...�ɽӶ��
OSPEVENT( MCU_MP_STOPPINHOLE_NOTIFY,		EV_MP_BGN + 74);
//mcu��ȡmp���������հ������



OSPEVENT(MCU_MP_GETDSINFO_REQ,				EV_MP_BGN+80);
OSPEVENT(MP_MCU_GETDSINFO_ACK,				EV_MP_BGN+81);
OSPEVENT(MP_MCU_GETDSINFO_NACK,				EV_MP_BGN+82);

//���Լ���������
OSPEVENT(MCU_MP_ADDSENDSELFMUTEPACK_CMD,	EV_MP_BGN+83);
OSPEVENT(MCU_MP_DELSENDSELFMUTEPACK_CMD,	EV_MP_BGN+84);

// �ڲ�״̬ͳ�ƶ�ʱ��
OSPEVENT(EV_MP_NETSTAT,                     EV_MP_BGN+90);

// �ڲ�������ⶨʱ��
OSPEVENT(EV_MP_FLUXOVERRUN,					EV_MP_BGN+91);

//��������֪ͨ���˶�ʱ��
OSPEVENT(EV_MP_SENDFLUXOVERRUNNOTIFY,	    EV_MP_BGN+92);

// MP��ǰʵ������֪ͨ�ϱ�����Ϣ�壺u16 ��ǰ��ʵռ�õ��������
OSPEVENT(MP_MCU_FLUXSTATUS_NOTIFY,			EV_MP_BGN+93);

// ȡ��������״̬��ʱ��
OSPEVENT(EV_MP_GETMSSTATUS_TIEMR,	        EV_MP_BGN+94);
// �������ҳ��ӡ��ʱ��
OSPEVENT(EV_SWITCHTABLE_PRTNEXTPAGE_TEMER,  EV_MP_BGN+97);
// ���ӽ�����(������)
OSPEVENT(EV_MP_ADDTABLE,                    EV_MP_BGN+95);
// ��ս�����(������)
OSPEVENT(EV_MP_CLEARTABLE,                  EV_MP_BGN+96);
// �����������״̬
OSPEVENT( EV_MP_DEAL_CALLBACK_NTF,          EV_MP_BGN + 98 );
// �л���ƵԴ��ʱ��ʱ��
OSPEVENT( EV_MP_CHGSRCTOT_TIMER,            EV_MP_BGN + 99 );

OSPEVENT( EV_MCU_CONNECTMP_TIMER,           EV_MP_BGN + 101 );
OSPEVENT( EV_MCU_MP_REG_TIMER,              EV_MP_BGN + 102 );

OSPEVENT( MCU_MP_REG_REQ,                   EV_MP_BGN + 103 );
OSPEVENT( MP_MCU_REG_ACK,                   EV_MP_BGN + 104 );
OSPEVENT( MP_MCU_REG_NACK,                  EV_MP_BGN + 105 );

OSPEVENT( EV_MCU_CHECKMPRES_TIMER,          EV_MP_BGN + 106 );
OSPEVENT( EV_MP_UPDATE_ZKSTATUS_TIMER,		EV_MP_BGN + 107 );

#endif

