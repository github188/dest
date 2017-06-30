
/*****************************************************************************
模块名      : UserLoginServer
文件名      : old_evulculs.h
创建时间    : 
实现功能    : 
作者        : 
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容

******************************************************************************/
#ifndef _OLD_252_EV_ULC_ULS_H_
#define _OLD_252_EV_ULC_ULS_H_

#include "osp.h"
#include "eventid.h"

//2.5.2版本及之前版本的消息定义(兼容性需要)
//ULC登陆请求消息
OSPEVENT( OLD_2_5_2_ULC_ULS_LOGIN_REQ,				EV_ULC_ULS_BGN + 0);

//ULC登陆请求消息成功答复
OSPEVENT( OLD_2_5_2_ULS_ULC_LOGIN_ACK,				EV_ULC_ULS_BGN + 1);

//ULC登陆请求消息失败答复
OSPEVENT( OLD_2_5_2_ULS_ULC_LOGIN_NACK,				EV_ULC_ULS_BGN + 2);


#endif  //end of _OLD_252_EV_ULC_ULS_H_
