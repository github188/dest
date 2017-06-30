#pragma once

#include "osp.h"
#include "dbopr.h"

class CAsynDBOpr
{
public:
	CAsynDBOpr();
	virtual ~CAsynDBOpr();
	virtual const s8* GetModuleName() = 0;
	virtual u16 GetModuleId() = 0;
	const TDBConnectInfo& GetDBConnectInfo() const;
	virtual BOOL32 DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine) = 0;
	virtual BOOL32 Initial(const TDBConnectInfo& tDBConnectInfo, u32 dwTaskNum = DEFAULT_DBTHREAD_NUM, u32 dwQueueSize = DEFAULT_DBQUEUEMSG_NUM);
	void Quit();
	virtual u32 GetDBTaskNum(void);
	virtual u32 GetQueueSize(void);
	virtual void PreprocDBOpr(TDBOprTaskInfo *ptDBOprTaskInfo, CDBEngine * ptDBEngine, u32 &dwTaskTimeOutNum, BOOL32 bTaskActive);
	virtual BOOL32 PostOneMsgToDBOprTask(TDBOprTaskMsgItem & tDBTaskMsgItem, BOOL32 bPushQueue);
	virtual BOOL32 CancelQueueDBOpr(u32 dwSessionID);
protected:
	virtual BOOL32 PrintStatus(CDBEngine * ptDBEnging);
	virtual BOOL32 CheckDBConnectState(CDBEngine * ptDBEnging);
	void ClearMemData(void);
	static void* DBOprTaskProc(void * pParam);
protected:
	TDBOprTaskInfo	*m_ptDBTaskPool;			//数据库操作线程池
	TDBMsgQueue		*m_ptDBMsgQueue;			//消息队列
	TDBConnectInfo	m_tDbConnectInfo;
	u32 m_dwDBTaskNum;
	u32 m_dwQueueSize;
	SEMHANDLE m_hSemTask;
	SEMHANDLE m_hSemQueue;
};
