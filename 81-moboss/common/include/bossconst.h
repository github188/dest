/*****************************************************************************
   ģ����      :
   �ļ���      : bossconst.h
   ����ļ�    :
   �ļ�ʵ�ֹ���: ����81-mobossĿ¼�³����õ�һЩ����
   ����        :
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2012/04/23  1.0                     ����
******************************************************************************/

#ifndef _MO_BOSS_CONST_H_
#define _MO_BOSS_CONST_H_

//APP�߳����ȼ�
#define MOBOSS_APP_PRI_HIGH                 (u8) 80     //�����ȼ�
#define MOBOSS_APP_PRI_NORMAL               (u8) 90     //�������ȼ�
#define MOBOSS_APP_PRI_LOW                  (u8) 100    //�����ȼ�

#define MOBOSS_THREAD_STACKSIZE				(u32)(512*1024)


//OSP�������
#define MOBOSS_HB_TIME                      (u16)10     //����ʱ��
#define MOBOSS_HB_NUM                       (u16)3      //��������

//OSP��Ȩ����
#define MOBOSS_TEL_USRNAME                  (const s8*)"admin"
#define MOBOSS_TEL_PWD                      (const s8*)"admin"

//��ַ�ָ��
#ifdef _LINUX_
#define PATH_SEPARATOR                      (s8)'/'
#else
#define PATH_SEPARATOR                      (s8)'\\'
#endif



#endif //_MO_BOSS_CONST_H_
//End of file





