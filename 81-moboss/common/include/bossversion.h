

/*****************************************************************************
   模块名      :
   文件名      : bossversion.h
   相关文件    :
   文件实现功能: 81-moboss目录下版本与版权信息
   作者        :
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2012/04/23  1.0                     创建
******************************************************************************/

#ifndef _MO_BOSS_VERSION_H_
#define _MO_BOSS_VERSION_H_

#include <string>
#include <string.h>
#include "kdvtype.h"

#define MAXLEN_MO_BOSS_VER              (u16)32 //版本字串的最大长度
//#define MO_BOSS_VER_PREFIX				"5.0.0.0.0"
#define MO_BOSS_VER_PREFIX				"5.0.0.0.0"		//Ver.Release.Build.Sp.Fix.Time
#define MO_BOSS_KEDA_COPYRIGHT			(const s8*)"版权所有(C) 1995-%d 苏州科达科技有限公司"

std::string GetBossBuildVersion();
std::string GetBossCopyRight();



#endif // _MO_BOSS_VERSION_H_
// End of file



