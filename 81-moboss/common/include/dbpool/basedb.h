/*=======================================================================
ģ����      : DBPool
�ļ���      : basedb.h
����ļ�    :
�ļ�ʵ�ֹ���:

����        :
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/04/23  1.0                     ����
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
	BOOL32 GetConnectState() const;   // �õ�����״̬	
	u16 ClearQryResults(void);

    void DbLinkMaintenance(); // DB��·ά��

    void SetDBCharSetType(EmDBCharSetType emType) { m_emDBCharSetType = emType; }

private:
    void TryCheckDBConState();  // ���� �������״̬
    u16  DoCheckDBConState();   // ʵ��ִ�� �������״̬

protected:
	CDBEngine m_cDBEngine;

private:
    TDBConnectInfo m_tDBConnectInfo;    //
    BOOL32 m_bDbiCheckState;		//dbi��������鵽��״̬���
	u32 m_dwDisConnectNum;		//�Ͽ�����
	EmDBCharSetType m_emDBCharSetType;
};

#endif // _DBPOOL_BASE_DB_H_
// END OF FILE
