/*****************************************************************************
ģ����      : MAU
�ļ���      : apdbopr.h
����ʱ��    : 2013�� 12�� 05��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
******************************************************************************/
#ifndef	_APINFO_DB_OPR_H_
#define	_APINFO_DB_OPR_H_

#include "asyndbopr.h"

class CAPInfoDBOpr : public CAsynDBOpr
{
public:
	enum
	{
		CLEARDBDATAOPR = 1,
		GETSERVDOMAININFOOPR,
		GETPLATFORMDOMAININFOOPR,
		GETUSERDOMAININFOOPR,
		GETMTINFOOPR,
		GETDOMAINHASXMPU,
		PRINT_CONNECT_STATUS,	//��ȡ����״̬
		GETHDUMOIDANDUSERDOMAINMOID, //��server_info���л�ȡHDU��moid�Լ��û���moid
		GETPLATFORMISRENT,    //��ȡƽ̨���Խ���������
	};
	
public:
	CAPInfoDBOpr();
	~CAPInfoDBOpr();
	
public:
	//��ʼ������
	virtual const s8* GetModuleName();
	virtual u16 GetModuleId();
	u8 GetServDomainInfo();
	u8 GetPlatFormDomainInfo();
	u8 GetUserDomainInfo();
	u8 GetMtInfo();
	u8 GetDomainHasXmpu();
	u8 GetHduMoidAndUserDomainMoid();
	u8 GetPlatformIsRent();
	void PrintConnectStatus();
	
protected:
	BOOL32 DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcGetServDomainInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcGetPlatFormDomainInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcGetUserDomainInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcGetMtInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcGetDomainHasXmpu(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	const s8 * GetDBOprNameByTransId( u32 dwTransId );
	BOOL32 ProcGetHduMoidAndUserMoid(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcGetPlatformIsRent(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine);
};

#endif // _APINFO_DB_OPR_H_
