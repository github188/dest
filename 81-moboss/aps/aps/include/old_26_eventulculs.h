
/*=======================================================================
模块名      :uls
文件名      :eventulculs.h
相关文件    :
文件实现功能:与终端通信的消息定义

作者        :
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/07/18  1.0                       创建
=======================================================================*/
#ifndef _EVENT_26_ULC_ULS_H_
#define _EVENT_26_ULC_ULS_H_

#include "osp.h"
#include "eventid.h"

//ULC登陆请求消息           内容:CUlcLoginInfo
OSPEVENT( OLD_26_ULC_ULS_LOGIN_REQ,				EV_ULC_ULS_BGN + 3);    //+1,+2,+3消息用于2.5.2之前版本的老终端兼容

//ULC登陆请求消息成功答复   内容:CUlcFullInfo
OSPEVENT( OLD_26_ULS_ULC_LOGIN_ACK,				EV_ULC_ULS_BGN + 4 );

//ULC登陆请求消息失败答复   内容:(u32_ErrorCode
OSPEVENT( OLD_26_ULS_ULC_LOGIN_NACK,			EV_ULC_ULS_BGN + 5 );

#endif  //end of _EVENT_26_ULC_ULS_H_
