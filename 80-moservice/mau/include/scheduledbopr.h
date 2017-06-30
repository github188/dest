/*****************************************************************************
ģ����      : MAU
�ļ���      : scheduledconfinfodbopr.h
����ʱ��    : 2013�� 12�� 05��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/12/05  KDVP2.6     ��Ҳ        ����
******************************************************************************/
#ifndef	_SCHEDULEDCONFINFO_DB_OPR_H_
#define	_SCHEDULEDCONFINFO_DB_OPR_H_

#include "asyndbopr.h"

class CScheduledConfInfoDBOpr : public CAsynDBOpr
{
	enum
	{
		CLEARDBDATAOPR = 1,
		GETALLSCHEDULEDCONFINFOOPR,
		PRINT_CONNECT_STATUS,	//��ȡ����״̬
	};

public:
	CScheduledConfInfoDBOpr();
	~CScheduledConfInfoDBOpr();

public:
	virtual const s8* GetModuleName();
	virtual u16 GetModuleId();

	u8 GetAllScheduledConfInfoThenClear();
	void   PrintConnectStatus();

protected:

	BOOL32 DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine);
    BOOL32 ProcGetAllScheduledConfInfoThenClear( TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine );

	const s8 * GetDBOprNameByTransId( u32 dwTransId );

	//��ӡ����״̬
	BOOL32 ProcPrintConnectStatus(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	
};

#endif // _SCHEDULEDCONFINFO_DB_OPR_H_
