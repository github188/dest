/*****************************************************************************
   模块名      : mrs配置文件
   文件名      : mrscfg.h
   相关文件    : 
   文件实现功能: 新方案业务MCU内部消息定义
   作者        : 李屹
   版本        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2002/07/24  0.9         李屹        创建
******************************************************************************/
#ifndef _MRS_CFG_H_
#define _MRS_CFG_H_

/*------------------------------------------------------------- 
配置文件mrscfg.ini
*-------------------------------------------------------------*/

#define MRS_CFG_INI		                    	(const s8*)"mrscfg.ini"
#define SECTION_MpInfo                          (const s8*)"MpInfo" 
#define KEY_MpIpAddr                            (const s8*)"MpIpAddr"
#define SECTION_PrsInfo                         (const s8*)"PrsInfo" 
#define KEY_PrsIpAddr                           (const s8*)"PrsIpAddr"
#define SECTION_ZkInfo                          (const s8*)"ZkInfo" 
#define KEY_ZkIpAddr                            (const s8*)"ZkIpAddr"
#define KEY_ZkPort                              (const s8*)"ZkPort"

#endif /* _MRS_CFG_H_ */


