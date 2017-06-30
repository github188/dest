
/*****************************************************************************
   ģ����      :
   �ļ���      : httpssn.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ���ն˵�httpͨ��
   ����        :
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/07/08  1.0                     ����
******************************************************************************/
#ifndef _XAP_HTTP_SSN_H_
#define _XAP_HTTP_SSN_H_


#include "osp.h"
#include "kdvtype.h"
#include "microhttpd.h"

class CHttpSsnInst : public CInstance
{
public:
	CHttpSsnInst();
	~CHttpSsnInst();

public:
	void DaemonInstanceEntry( CMessage *const pcMsg, CApp* pcApp);
    void InstanceEntry(CMessage *const pcMsg);
	void DaemonProcPowerOn(CMessage *const pcMsg);


	//http��¼
    void ProcUlcLoginReq(CMessage *const pcMsg);
	void ProcUlcLoginAck(CMessage *const pcMsg);
	void ProcUlcLoginNack(CMessage *const pcMsg);

protected:
	BOOL32 PostMsgToCtrl(u16 wEvent, u8 *const pbyMsg, u16 wLen);
	BOOL32 PostMsgToMT(struct MHD_Daemon* pHttpDaemon,struct MHD_Connection* handleConnect, u16 wHttpCode, const void* pbyMsg, u16 wLen);
    BOOL32 PostNackToMT(struct MHD_Connection* handleConnect, u32 dwErrID);
};

class CHttpSsnData
{
public:
	CHttpSsnData();
	virtual ~CHttpSsnData();

public:	
    struct MHD_Daemon* GetHttpDaemon() const { return m_pHttpDaemon; }
    struct MHD_Daemon* GetHttpsDaemon() const { return m_pHttpsDaemon; }
    void StartHttpService();
    BOOL32 IsHttpsPort(const u16 &wPort);
	
private:
    struct MHD_Daemon* m_pHttpDaemon;
    struct MHD_Daemon* m_pHttpsDaemon;
};

typedef zTemplate<CHttpSsnInst, 1, CHttpSsnData> CHttpSsnApp;

extern CHttpSsnApp g_cHttpSsnApp;




#endif //end of _XAP_HTTP_SSN_H_
