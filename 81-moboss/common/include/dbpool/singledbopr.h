/*=======================================================================
模块名      : DBPool
文件名      : singledbopr.h
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

#ifndef _DBPOOL_SINGLE_OPRDB_H_
#define _DBPOOL_SINGLE_OPRDB_H_

#include "basedb.h"
#include "idbopr.h"

class CSingleDBOpr : public IDbOpr, public CBaseDB
{
public:
	CSingleDBOpr();
	virtual ~CSingleDBOpr();

public:
	virtual BOOL32 Init();
    virtual void ConnectAllDB(const std::vector<TDBConnectInfo>& vcDBConnectInfo);
    virtual void DisconnectAllDB(void);
	virtual std::vector<CDBState> GetDBState() const;

	virtual void MaintenanceAllDbLink(); // 维护DB链路

private:
	TDBConnectInfo m_tDBConnectInfo;
};

#endif // _DBPOOL_SINGLE_OPRDB_H_
// END OF FILE






