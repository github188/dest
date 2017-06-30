/*****************************************************************************
   ģ����      : 4.8���ܶ���
   �ļ���      : snmpclt.h
   ����ʱ��    : 2014�� 5�� 26��
   ʵ�ֹ���    : ��Ϣ���Ŀͻ���ͷ�ļ�
   ����        : �ܾ���
   �汾        : 0.1
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   14/06/12    0.1         �ܾ���      ���� 
******************************************************************************/


#ifndef _SNMPCLT_H_
#define _SNMPCLT_H_

#include "osp.h"
#include "kdvlog.h"
#include "loguserdef.h"
#include "kdvsys.h"
#include "mcuconst.h"
#include "mcustruct.h"
#include "commonlib.h"
#include "mcuver.h"
#include "evagtsvc.h"
#include "mcuagtstruct.h"
#include "evbrdmgrsvc.h"
#include "evconfsvc.h"
#include "appdef.h"
#include "agtcomm.h"
#include "ipmi_api.h"
#include "evmcu.h"

const u16 DUMPCLTINFO = 1;


//���ܷ���˱�ʶ����
#define BOARD_NONE			(u8)0//ceu����
#define BOARD_CEU			(u8)1//ceu����
#define BOARD_UMU			(u8)2//umu����
#define BOARD_SMU			(u8)7//smu����	//�����������ܶ����id��ͻ���޸�Ϊ�ϴ��ֵ
#define BOARD_XMPU			(u8)4//xmpu����

#define	 FAN_MCU_OBJECT				(u8)5	//�澯��ʶ1,2,3,4�Ķ�����agtcomm.h��
#define	 POWER_MCU_OBJECT			(u8)6
#define	 POWERTEMP_MCU_OBJECT		(u8)7
#define	 FANTEMP_MCU_OBJECT			(u8)8
#define	 DISKCOUNT_MCU_OBJECT		(u8)9

#define SNMPSVC_BRDMGR		(u8)1//����������
#define SNMPSVC_MEDIARES	(u8)2//ý����Դ����
#define SNMPSVC_MTACCESSRES	(u8)3//�ն˽������
// #define	SNMPSVC_CONF		(u8)4//��ͳý��������
// #define	SNMPSVC_MEDIACONF	(u8)5//��ͳý��������
#define	SNMPSVC_ACS			(u8)4//ACS�������

//typedef void (*SnmpMsgCenterConnected)(void);

typedef void (*SnmpMsgRecvCallBack)( const u32 &dwSrcNode,const u32 &dwSrcIId,
							const u16 &wEvent,const u8* pbyMsg,
							const u16 &wLen);





BOOL32 SnmpCltInit( SnmpMsgRecvCallBack CltSnmpMsgRecv,const u8 &byBoardType = 0,const u8 &bySlot = 0,const u8 &byMsState = 0xff );

BOOL32 SnmpCltQuit();

API void snmpcltver();

void snmpclthelp();



#endif