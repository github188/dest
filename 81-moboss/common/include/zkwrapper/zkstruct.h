

/*****************************************************************************
   ģ����      : zkwrapper
   ����        : �ṹ�嶨��
   ����        : 
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/06/06  1.0				       ����
******************************************************************************/

#ifndef _ZK_STRUCT_H_
#define _ZK_STRUCT_H_

#include "zkconst.h"
#include "kdvtype.h"

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif


//�������ģʽ����
enum EMNodeMode
{
    EmNodeMode_Normal               = 0,	//��ͨģʽ
    EmNodeMode_Sequence             = 1,	//˳��ģʽ
    EmNodeMode_Ephemeral            = 2,	//��ʱģʽ
    EmNodeMode_SequenceEphemeral    = 3	    //˳����ʱģʽ
};

//watch��������
enum EMWatchType
{
    EmWatchType_Create              = 1,    //�������
    EmWatchType_Delete              = 2,	//ɾ�����
    EmWatchType_Change              = 3,	//���ݱ仯
    EmWatchType_Child               = 4		//�ӽڵ�仯
};
class CZkWrapper;

//�ڵ�����
typedef s8 NodeName[MAXLEN_ZK_NODENAME+1];

/*=============================================================================
�� �� ����	DisConnectCallBackFunc
��    �ܣ�	disconnect�ص����� 
��    ����	
�� �� ֵ��	 
=============================================================================*/
typedef void (*DisConnectCallBackFunc)(CZkWrapper* pZKWrapper); 

/*=============================================================================
�� �� ����	WatchCallBackFunc
��    �ܣ�	watch�ص����� 
��    ����	szPath      watch����·��
			nEventType  watch��������
�� �� ֵ��	 
=============================================================================*/
typedef void (*WatchCallBackFunc)( CZkWrapper* pZKWrapper, const s8* szWatchPath, EMWatchType emEventType );

typedef void (*QueueCallBackFunc)( CZkWrapper* pZKWrapper, const s8* szQueuePath, const u8* szData, s32 nLen );

#endif  //_ZK_STRUCT_H_


