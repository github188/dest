

/*****************************************************************************
   ģ����      :
   �ļ���      : bossversion.h
   ����ļ�    :
   �ļ�ʵ�ֹ���: 81-mobossĿ¼�°汾���Ȩ��Ϣ
   ����        :
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2012/04/23  1.0                     ����
******************************************************************************/

#ifndef _MO_BOSS_VERSION_H_
#define _MO_BOSS_VERSION_H_

#include <string>
#include <string.h>
#include "kdvtype.h"

#define MAXLEN_MO_BOSS_VER              (u16)32 //�汾�ִ�����󳤶�
//#define MO_BOSS_VER_PREFIX				"5.0.0.0.0"
#define MO_BOSS_VER_PREFIX				"5.0.0.0.0"		//Ver.Release.Build.Sp.Fix.Time
#define MO_BOSS_KEDA_COPYRIGHT			(const s8*)"��Ȩ����(C) 1995-%d ���ݿƴ�Ƽ����޹�˾"

std::string GetBossBuildVersion();
std::string GetBossCopyRight();



#endif // _MO_BOSS_VERSION_H_
// End of file



