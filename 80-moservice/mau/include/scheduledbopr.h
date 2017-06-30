/*****************************************************************************
模块名      : MAU
文件名      : scheduledconfinfodbopr.h
创建时间    : 2013年 12月 05日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/12/05  KDVP2.6     王也        创建
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
		PRINT_CONNECT_STATUS,	//获取连接状态
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

	//打印连接状态
	BOOL32 ProcPrintConnectStatus(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	
};

#endif // _SCHEDULEDCONFINFO_DB_OPR_H_
