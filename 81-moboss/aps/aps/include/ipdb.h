


/*****************************************************************************
   模块名      : 
   文件名      : ipdb.h
   相关文件    : 
   文件实现功能: IP地址归属地与运营商查询
   作者        : 
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2014/06/11  1.0				       创建
******************************************************************************/


#ifndef _XAP_IP_DB_H_
#define _XAP_IP_DB_H_

#include "basedb.h"
#include "kdvtype.h"
#include <string>

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

class CIpDB : public CBaseDB
{
public:
    CIpDB();
    virtual ~CIpDB();
    
public:
    //获取IP地址的运营商名称
    u16 GetIpISPName(IN u32 dwClientIp/*主机序*/, OUT std::string& strISPName);	

private:
    u16 QueryForeignISPName(IN u32 dwClientIp/*主机序*/, OUT std::string& strISPName);
};



#endif /*_XAP_IP_DB_H_*/




