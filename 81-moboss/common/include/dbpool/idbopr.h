/*=======================================================================
模块名      : DBPool
文件名      : idbopr.h
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

#ifndef _DBPOOL_INTERFACE_OPRDB_H_
#define _DBPOOL_INTERFACE_OPRDB_H_

#pragma warning (disable:4786 )

#include <vector>
#include "kdvtype.h"
#include "dbengine.h"

class IDbOpr
{  
public:
	virtual ~IDbOpr()
	{
	}

public:
	virtual BOOL32 Init() = 0;
    virtual void ConnectAllDB(const std::vector<TDBConnectInfo>& vcDBConnectInfo) = 0;
    virtual void DisconnectAllDB(void) = 0;
    virtual std::vector<CDBState> GetDBState() const = 0;
	
	virtual void MaintenanceAllDbLink() = 0;// DB链路维护
};

#endif // _DBPOOL_INTERFACE_OPRDB_H_
// END OF FILE



