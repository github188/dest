/*****************************************************************************
   ģ����      : MP
   �ļ���      : bindmp.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: MPģ��ӿڶ���
   ����        : ������
   �汾        : V0.1  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2003/07/25  0.1         ������        ����
******************************************************************************/
#ifndef __BINDMP_H_
#define __BINDMP_H_

#include "cristruct.h"
typedef struct tagStartMpParm
{
    u32 dwMcuIp;
    u16 wMcuPort;
    tagStartMpParm()
    {
        dwMcuIp  = 0;
        wMcuPort = 0;
    }
}TStartMpParam;

//mpstart����
// BOOL32 mpstart(u32 dwMcuIp,
// 			   u16 wMcuPort, 
// 			   TMp *ptMp,
// 			   TStartMpParam* ptMpParamSlave = NULL); 
//zyq 5.0
BOOL32 mpstart(TMp *ptMp, u32 dwZKIpAddr, u16 wZkPort); 

void mpstop(); //ֹͣMP

//mpstart����
BOOL32 mpstart(TMtIpMapTab *ptIpMapTab,
			   s8 *pchV6Ip,
			   u32 dwMcuTcpNode, 
			   u32 dwMcuIp,
			   u16 wMcuPort, 
			   TMp *ptMp,
			   TStartMpParam* ptMpParamSlave = NULL); 

#endif
