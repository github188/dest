/*****************************************************************************
模块名      : MPCD
文件名      : confstoreopr.h
创建时间    : 2012年 01月 05日
实现功能    : 
作者        : 陈文灿
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifndef	_CONFSTORE_DB_OPR_H_
#define	_CONFSTORE_DB_OPR_H_

#include "asyndbopr.h"

class CTemplateDBOpr : public CAsynDBOpr
{
public:
	enum
	{
		LOADTEMPLATEOPR = 1,
		ADDTEMPLATEOPR,
		DELTEMPLATEOPR,
		FINDTEMPLATEOPR,
		MODTEMPLATEOPR,
		PRINT_CONNECT_STATUS,
	};

	CTemplateDBOpr();
	virtual ~CTemplateDBOpr();

public:
	virtual const s8* GetModuleName();
	virtual u16 GetModuleId();

	//初始化函数
	void LoadDBData( void );

	virtual u8 AddTemplate( TDBOprTaskMsgItem & tDBMsg );
	virtual u8 DelTemplate( TDBOprTaskMsgItem & tDBMsg );
	virtual u8 GetTemplateInfo(TReqHeadInfo& tHeadInfo, const u16 wConfDuration);
	virtual u8 ModTemplate( TDBOprTaskMsgItem & tDBMsg );
	virtual void PrintConnectStatus();

protected:
	BOOL32 DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine);

	BOOL32 ProcLoadDBData( CDBEngine * ptDBEngine );
	BOOL32 ProcAddTemplate(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcDelTemplate(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcGetTemplate(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcModTemplate(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
};

#endif

