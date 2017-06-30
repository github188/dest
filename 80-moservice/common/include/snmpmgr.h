/*****************************************************************************
   ģ����      : 4.8���ܶ���
   �ļ���      : snmpmgr.h
   ����ʱ��    : 2014�� 5�� 26��
   ʵ�ֹ���    : 
   ����        : �ܾ���
   �汾        : 
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����   0
   14/05/26    4.8         �ܾ���      ���� 
******************************************************************************/


#ifndef _SNMPMGR_H_
#define _SNMPMGR_H_


#include "osp.h"
#include "kdvlog.h"
#include "loguserdef.h"
#include "kdvsys.h"
#include "mcuconst.h"
#include "mcustruct.h"
#include "commonlib.h"
#include "mcuver.h"
#include "snmpadp.h"
#include "evagtsvc.h"
#include "mcuagtstruct.h"
#include "evbrdmgrsvc.h"
#include "evconfsvc.h"
#include "mediasvc.h"
#include "mtadpsvc.h"
//#include "evacssvc.h"
#include "snmpclt.h"
#include "evboardmanager.h"
#ifdef _PPC1010
#include "snmpmgrinstance.h"
#endif

const u16 DUMPCONFIG = 1;
const u16 DUMPCLIENT = 2;

#define CPURATEINVALID 0xFFFF

#define  MEDIALICENSENUMINVALID 0xFFFFFFFF
#define  MTLICENSENUMINVALID 0xFFFF

enum emSnmpAbility
{
	emCpuAllocateRate = 0,
	emMemAllocateRate,
	emDiskAllocateRate,
	emMediaPortRes,	
	emMtAccessRes,
	emTemperature,
	emAlarm,
	emGKRegStatus,
	emAllCardsInfo,
	emAllLedState,
};

void snmpmgrtlibhelp();
void snmpmgrlibhelp();
API void showallcards();
void ShowAllCards();
BOOL32 SnmpMgrInit( u32 dwTrapRcvIp );
BOOL32 SnmpMgrQuit();


#endif