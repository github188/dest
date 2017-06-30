
/*****************************************************************************
   模块名      : 
   文件名      : dbopr.h
   相关文件    : dbopr.cpp
   文件实现功能: 操作ap数据库的接口
   作者        :
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/07/08  1.0                     创建
******************************************************************************/
#ifndef _XAP_DBOPR_H_
#define _XAP_DBOPR_H_

#include "multidbopr.h"
#include "kdvtype.h"

#include "apdb.h"
#include "ipdb.h"
#include "serverdevice.h"

class CDBOpr : public CMultiDBOpr
{
public:
	CDBOpr();
	~CDBOpr();

    //注意:获取DBItem的顺序必须与CDBPool::StartDBPool时参数std::vector<TDBConnectInfo>的顺序一致
    virtual void GetAllDBEngine(std::vector<CBaseDB*>& vcDBItem);
    void   ClearResults();  // 清空结果集

public:
    //通过设备GUID，查询设备的详细信息
    u16 GetSrvDevInfoByGUID(IN const s8* szDevGUID, OUT CServerDevice&  tSrvDevInfo);

    //某服务域下某种服务器设备信息
    u16 GetBriefSrvInfo(IN const s8* ServiceDomainMOID, IN u16 dwDeviceType, OUT std::set<CBriefSrvInfo>& setBriefSrvInfo);

    //获取某个平台域信息
    u16 GetPlatformDomain(IN const s8* szPlatformDomainMOID, OUT CPlatformDomain&  tPlatformDomain);

	//获取某个服务域下所有平台域信息
    u16 GetAllPlatformDomainMOID(IN const s8* pchServiceDoaminMOID, OUT std::set<std::string>&  vecPlatformDoaminMOID);


	//验证终端登录
	u16 Authentication(IN const CLoginInfo &tLoginInfo, OUT CDBMtInfo& tMtInfo, OUT CPlatformAttr& tPlatformAttr);	

    //获取IP地址的运营商名称(接口2获取的是类型)
    u16 GetIpISPName(IN u32 dwClientIp/*主机序*/, OUT std::string& strISPName);
#ifdef _APS_PXY_
	u16 GetIpISPName(IN u32 dwClientIp/*主机序*/, OUT EOperatorsType& emIspType);
#endif

private:	
    CApDB m_tApDB;
    CIpDB m_tIpDB;
};

#endif //end of _XAP_DBOPR_H_
