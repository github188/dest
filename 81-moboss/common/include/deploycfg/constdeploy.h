/*****************************************************************************
   ģ����      : 
   �ļ���      : constdeploy.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: �Լ���Ҫ�õ���һЩ����
   ����        : 
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2012/04/23  1.0				       ����
******************************************************************************/
#ifndef _CONST_DEPLOY_H_
#define _CONST_DEPLOY_H_

#include "kdvtype.h"


//1.1 ·��������
#ifdef _PPC_
	#define DEPLOY_CONFIG_PATH				    (const s8*)"/usr/etc/nm/conf"      //Ĭ�� �����ļ�·��
#elif _HISI_
	#define DEPLOY_CONFIG_PATH				    (const s8*)"/usr/etc/nm/conf"      //Ĭ�� �����ļ�·��
#else
#ifdef _LINUX_
	#define DEPLOY_CONFIG_PATH				    (const s8*)"/opt/mcu/nm/conf"      //Ĭ�� �����ļ�·��
#else 
	#define DEPLOY_CONFIG_PATH				    (const s8*)"conf"                              //Ĭ�� �����ļ�·��
#endif
#endif

#define DEPLOY_CONFIG_FILE_NAME		           (const s8*)"deploycfg.json"	    //�����ļ�����
#define PHYSICSSRV_RECIPE_NAME                           (const s8*)"guard"                           //����������䷽������


#define MAXLEN_DEPLOY_CONFIG			           (u16)255

#endif // _CONST_DEPLOY_H_
