
/*****************************************************************************
   ģ����      :
   �ļ���      : centerctrl.h
   ����ļ�    : centerctrl.cpp
   �ļ�ʵ�ֹ���: ����APP
   ����        :
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/07/08  1.0                     ����
******************************************************************************/
#ifndef _CENTER_CTRL_H_
#define _CENTER_CTRL_H_

#include "osp.h"
#include "dbpool_api.h"
#include "dbopr.h"
#include "constuls.h"
#include "mqmsgparse.h"
#include "srvmoduleinfo_nu.h"
#include "../../../module/mqmsgparse/include/jsonmsgparse.h"
class CCenterCtrlInst : public CInstance
{
public:
	CCenterCtrlInst();
	~CCenterCtrlInst();

public:
	void DaemonInstanceEntry( CMessage *const pcMsg, CApp* pcApp);
	void InstanceEntry(CMessage *const pMsg);

protected:
    void DaemonProcPowerOn(CMessage *const pcMsg);
    void DaemonInstanceDump(u32 param = 0);

    void QueryServerDevListTimeOut(CMessage *const pcMsg);
    void QueryServerDevListAck(CMessage *const pcMsg);
    void QueryServerDevListNack(CMessage *const pcMsg);

    //�����͵�DB����Ϣ
    void ProcPostToDBMsg(CMessage *const pcMsg);
};

class CCenterCtrlData : public CMqParseReceiver
{
public:
	CCenterCtrlData();
    ~CCenterCtrlData();

public:
    CDBPool<CDBOpr, MAXNUM_DB_CONNECT> &GetDBPool() { return m_tDBPool; }

    BOOL32 StarMqMsgParse();
    virtual void MqMsgParseCB( MqMsgCBEvent emEventID, void* pMsgBody, u32 wMsgLen);

    //��MQ������������״̬
    BOOL32 GetConnectRmqState() { return TRUE; }
    
    //�����ݿ������״̬
    std::vector<CDBState> GetDBPoolConnectState() const { return m_tDBPool.GetDBState(); }

	BOOL32 AddMtInfo(TMtRegNetInfo& tMtInfo);

	void TakeInitLock();

	void ParseMqMsg(void *pbuf, u32 len);

	void ReportAllMtInfo2Nms();

	void Print() const;
private:
    CDBPool<CDBOpr, MAXNUM_DB_CONNECT> m_tDBPool;
    //CMqMsgParse m_tMqMsgParse;
	TMtRegNetInfo  m_atMtInfo[MAXINSTNUM_ULCSSN];
	CJsonMsgParse m_cJsonMsgParse;
	SEMHANDLE   m_hInitLock;
	BOOL32		m_bInited;
};

typedef zTemplate<CCenterCtrlInst, 1, CCenterCtrlData> CCenterCtrlApp;

extern CCenterCtrlApp g_cCenterCtrlApp;


#endif //end of _CENTER_CTRL_H_
