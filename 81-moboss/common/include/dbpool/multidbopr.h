/*=======================================================================
模块名      : DBPool
文件名      : multidbopr.h
相关文件    :
文件实现功能:

作者        :
版本        : V2.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/09/06  2.0                     创建
=======================================================================*/

#ifndef _DBPOOL_MULTI_OPRDB_H_
#define _DBPOOL_MULTI_OPRDB_H_

#include "basedb.h"
#include "idbopr.h"

class CMultiDBOpr : public IDbOpr
{
public:
	CMultiDBOpr();
	virtual ~CMultiDBOpr();

public:
	virtual BOOL32 Init();
    virtual void ConnectAllDB(const std::vector<TDBConnectInfo>& vcDBConnectInfo);
    virtual void DisconnectAllDB(void);
	virtual std::vector<CDBState> GetDBState() const;

	virtual void MaintenanceAllDbLink(); // DB链路维护
	
public:
	//注意:获取DBItem的顺序必须与CDBPool::StartDBPool时参数std::vector<TDBConnectInfo>的顺序一致
	virtual void GetAllDBEngine(std::vector<CBaseDB*>& vcDBItem) = 0;

private:
    BOOL32 SetCallBackUserData();

private:
	std::vector<CBaseDB*> m_vcDBItem;   //数据库
};

#endif // _DBPOOL_MULTI_OPRDB_H_
// END OF FILE



