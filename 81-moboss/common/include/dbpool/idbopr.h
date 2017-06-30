/*=======================================================================
ģ����      : DBPool
�ļ���      : idbopr.h
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
	
	virtual void MaintenanceAllDbLink() = 0;// DB��·ά��
};

#endif // _DBPOOL_INTERFACE_OPRDB_H_
// END OF FILE



