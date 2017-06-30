/*****************************************************************************
   模块名      : 
   文件名      : constcommon.h
   相关文件    : 
   文件实现功能: node共用到的一些常量
   作者        : 
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2014/03/06  1.0				       创建
******************************************************************************/
#ifndef _CONST_NODE_COMMON_H_
#define _CONST_NODE_COMMON_H_

#include "kdvtype.h"


/************************************************************************/
/*							长度                    
/************************************************************************/

#define MAXLEN_ACTION                          (u16)32
// 应用名最大长度
#define MAXLEN_APP_NAME                     (u16)64
// 版本号最大长度
#define MAXLEN_VERSION                        (u16)32
// 时间戳最大长度
#define MAXLEN_TIMESTAMP                   (u16)32
// 节点类型最大长度
#define MAXLEN_NODETYPE                     (u16)32
// 路径最大长度
#define MAXLEN_FILE_PATH                    (u16)512

// 节点名最大长度
#define MAXLEN_NODE                             (u16)128

//配置文件参数 最大长度
#define MAXLEN_CONF_PARM                  (u16)1024

//普通MSG最大长度
#define MAXLEN_MSG                               (u16)4096

// 错误MSG最大长度
#define MAXLEN_ERR_MSG                       (u16)1024


// shell命令最大长度
#define MAXLEN_SHELL_CMD                   (u16)1024


#endif // _CONST_NODE_COMMON_H_
