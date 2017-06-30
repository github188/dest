/*****************************************************************************
   ģ����      : 
   �ļ���      : constcommon.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: node���õ���һЩ����
   ����        : 
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2014/03/06  1.0				       ����
******************************************************************************/
#ifndef _CONST_NODE_COMMON_H_
#define _CONST_NODE_COMMON_H_

#include "kdvtype.h"


/************************************************************************/
/*							����                    
/************************************************************************/

#define MAXLEN_ACTION                          (u16)32
// Ӧ������󳤶�
#define MAXLEN_APP_NAME                     (u16)64
// �汾����󳤶�
#define MAXLEN_VERSION                        (u16)32
// ʱ�����󳤶�
#define MAXLEN_TIMESTAMP                   (u16)32
// �ڵ�������󳤶�
#define MAXLEN_NODETYPE                     (u16)32
// ·����󳤶�
#define MAXLEN_FILE_PATH                    (u16)512

// �ڵ�����󳤶�
#define MAXLEN_NODE                             (u16)128

//�����ļ����� ��󳤶�
#define MAXLEN_CONF_PARM                  (u16)1024

//��ͨMSG��󳤶�
#define MAXLEN_MSG                               (u16)4096

// ����MSG��󳤶�
#define MAXLEN_ERR_MSG                       (u16)1024


// shell������󳤶�
#define MAXLEN_SHELL_CMD                   (u16)1024


#endif // _CONST_NODE_COMMON_H_
