

/*****************************************************************************
   模块名      : zkwrapper
   功能        : 结构体定义
   作者        : 
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/06/06  1.0				       创建
******************************************************************************/

#ifndef _ZK_STRUCT_H_
#define _ZK_STRUCT_H_

#include "zkconst.h"
#include "kdvtype.h"

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif


//创建结点模式定义
enum EMNodeMode
{
    EmNodeMode_Normal               = 0,	//普通模式
    EmNodeMode_Sequence             = 1,	//顺序模式
    EmNodeMode_Ephemeral            = 2,	//临时模式
    EmNodeMode_SequenceEphemeral    = 3	    //顺序临时模式
};

//watch触发类型
enum EMWatchType
{
    EmWatchType_Create              = 1,    //创建结点
    EmWatchType_Delete              = 2,	//删除结点
    EmWatchType_Change              = 3,	//数据变化
    EmWatchType_Child               = 4		//子节点变化
};
class CZkWrapper;

//节点名称
typedef s8 NodeName[MAXLEN_ZK_NODENAME+1];

/*=============================================================================
函 数 名：	DisConnectCallBackFunc
功    能：	disconnect回调函数 
参    数：	
返 回 值：	 
=============================================================================*/
typedef void (*DisConnectCallBackFunc)(CZkWrapper* pZKWrapper); 

/*=============================================================================
函 数 名：	WatchCallBackFunc
功    能：	watch回调函数 
参    数：	szPath      watch触发路径
			nEventType  watch触发类型
返 回 值：	 
=============================================================================*/
typedef void (*WatchCallBackFunc)( CZkWrapper* pZKWrapper, const s8* szWatchPath, EMWatchType emEventType );

typedef void (*QueueCallBackFunc)( CZkWrapper* pZKWrapper, const s8* szQueuePath, const u8* szData, s32 nLen );

#endif  //_ZK_STRUCT_H_


