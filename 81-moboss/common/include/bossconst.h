/*****************************************************************************
   模块名      :
   文件名      : bossconst.h
   相关文件    :
   文件实现功能: 定义81-moboss目录下程序公用的一些常量
   作者        :
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2012/04/23  1.0                     创建
******************************************************************************/

#ifndef _MO_BOSS_CONST_H_
#define _MO_BOSS_CONST_H_

//APP线程优先级
#define MOBOSS_APP_PRI_HIGH                 (u8) 80     //高优先级
#define MOBOSS_APP_PRI_NORMAL               (u8) 90     //正常优先级
#define MOBOSS_APP_PRI_LOW                  (u8) 100    //低优先级

#define MOBOSS_THREAD_STACKSIZE				(u32)(512*1024)


//OSP心跳检查
#define MOBOSS_HB_TIME                      (u16)10     //心跳时间
#define MOBOSS_HB_NUM                       (u16)3      //心跳次数

//OSP授权定义
#define MOBOSS_TEL_USRNAME                  (const s8*)"admin"
#define MOBOSS_TEL_PWD                      (const s8*)"admin"

//地址分割符
#ifdef _LINUX_
#define PATH_SEPARATOR                      (s8)'/'
#else
#define PATH_SEPARATOR                      (s8)'\\'
#endif



#endif //_MO_BOSS_CONST_H_
//End of file





