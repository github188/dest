/*=======================================================================
ģ����      : DBPool
�ļ���      : multidbopr.h
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

	virtual void MaintenanceAllDbLink(); // DB��·ά��
	
public:
	//ע��:��ȡDBItem��˳�������CDBPool::StartDBPoolʱ����std::vector<TDBConnectInfo>��˳��һ��
	virtual void GetAllDBEngine(std::vector<CBaseDB*>& vcDBItem) = 0;

private:
    BOOL32 SetCallBackUserData();

private:
	std::vector<CBaseDB*> m_vcDBItem;   //���ݿ�
};

#endif // _DBPOOL_MULTI_OPRDB_H_
// END OF FILE



