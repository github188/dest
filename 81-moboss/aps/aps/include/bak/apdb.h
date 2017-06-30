


/*****************************************************************************
   模块名      : 
   文件名      : apdb.h
   相关文件    : 
   文件实现功能: 
   作者        : 
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2014/06/11  1.0				       创建
******************************************************************************/


#ifndef _XAP_AP_DB_H_
#define _XAP_AP_DB_H_

#include <set>
#include "basedb.h"
#include "kdvtype.h"

#include "structlogin.h"
#include "structself.h"
#include "serverdevice.h"
#include "platformdomain.h"

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

class CApDB : public CBaseDB
{
public:
    CApDB();
    virtual ~CApDB();
    
public:
    //通过设备GUID，查询设备的详细信息
    u16 GetSrvDevInfoByGUID(IN const s8* szDevGUID, OUT CServerDevice&  tSrvDevInfo);

    //某服务域下某种设备信息
    u16 GetBriefSrvInfo(IN const s8* ServiceDomainMOID, IN u16 dwDeviceType, OUT std::set<CBriefSrvInfo>& setBriefSrvInfo);

    //获取某个平台域信息
    u16 GetPlatformDomain(IN const s8* szPlatformDomainMOID, OUT CPlatformDomain&  tPlatformDomain);

	u16 GetAllPlatformDoaminMOID(IN const s8* pchServiceDomainMOID, OUT std::set<std::string>& setPlatformDoaminMOID);

public:
    //验证终端登录
    u16 Authentication(IN const CLoginInfo &tLoginInfo, OUT CDBMtInfo& tMtInfo, OUT CPlatformAttr& tPlatformAttr);	

private:
    u16 ReadSrvDeviceInfo( OUT CServerDevice& tSrvDeviceInfo );
    u16 ReadPlatformDomainInfo(OUT CPlatformDomain& tPlatformDomain);
};



#endif /*_XAP_AP_DB_H_*/




