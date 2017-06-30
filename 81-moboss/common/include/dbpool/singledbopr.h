/*=======================================================================
ģ����      : DBPool
�ļ���      : singledbopr.h
����ļ�    :
�ļ�ʵ�ֹ���:

����        :
�汾        : V2.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/09/06  2.0                     ����
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

	virtual void MaintenanceAllDbLink(); // ά��DB��·

private:
	TDBConnectInfo m_tDBConnectInfo;
};

#endif // _DBPOOL_SINGLE_OPRDB_H_
// END OF FILE






