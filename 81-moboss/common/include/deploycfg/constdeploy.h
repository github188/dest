/*****************************************************************************
   模块名      : 
   文件名      : constdeploy.h
   相关文件    : 
   文件实现功能: 自己需要用到的一些常量
   作者        : 
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2012/04/23  1.0				       创建
******************************************************************************/
#ifndef _CONST_DEPLOY_H_
#define _CONST_DEPLOY_H_

#include "kdvtype.h"


//1.1 路径与名称
#ifdef _PPC_
	#define DEPLOY_CONFIG_PATH				    (const s8*)"/usr/etc/nm/conf"      //默认 配置文件路径
#elif _HISI_
	#define DEPLOY_CONFIG_PATH				    (const s8*)"/usr/etc/nm/conf"      //默认 配置文件路径
#else
#ifdef _LINUX_
	#define DEPLOY_CONFIG_PATH				    (const s8*)"/opt/mcu/nm/conf"      //默认 配置文件路径
#else 
	#define DEPLOY_CONFIG_PATH				    (const s8*)"conf"                              //默认 配置文件路径
#endif
#endif

#define DEPLOY_CONFIG_FILE_NAME		           (const s8*)"deploycfg.json"	    //配置文件名称
#define PHYSICSSRV_RECIPE_NAME                           (const s8*)"guard"                           //物理服务器配方包名称


#define MAXLEN_DEPLOY_CONFIG			           (u16)255

#endif // _CONST_DEPLOY_H_
