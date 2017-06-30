/*=======================================================================
模块名      : DBPool
文件名      : basedb.h
相关文件    :
文件实现功能:

作者        :
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/04/23  1.0                     创建
=======================================================================*/

#ifndef _DBPOOL_BASE_DB_H_
#define _DBPOOL_BASE_DB_H_

#include "kdvtype.h"
#include "dbengine.h"

class CBaseDB
{
public:
    CBaseDB();
    virtual ~CBaseDB();
    
public:
    void SetCallBackUserData(DBPOOL_SERVERCBPROC pServerCbFunc, DBPOOL_CLIENTCBPROC pClientCbFunc);
    BOOL32 ConnectDBServer(const TDBConnectInfo& tDBConnectInfo);
    BOOL32 DisconnectDBServer(void);

    TDBConnectInfo GetDBConnectInfo() const { return m_tDBConnectInfo; }
	BOOL32 GetConnectState() const;   // 得到连接状态	
	u16 ClearQryResults(void);

    void DbLinkMaintenance(); // DB链路维护

    void SetDBCharSetType(EmDBCharSetType emType) { m_emDBCharSetType = emType; }

private:
    void TryCheckDBConState();  // 尝试 检测连接状态
    u16  DoCheckDBConState();   // 实际执行 检测连接状态

protected:
	CDBEngine m_cDBEngine;

private:
    TDBConnectInfo m_tDBConnectInfo;    //
    BOOL32 m_bDbiCheckState;		//dbi层主动检查到的状态结果
	u32 m_dwDisConnectNum;		//断开次数
	EmDBCharSetType m_emDBCharSetType;
};

#endif // _DBPOOL_BASE_DB_H_
// END OF FILE
