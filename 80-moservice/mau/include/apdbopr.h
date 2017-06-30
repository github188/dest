/*****************************************************************************
模块名      : MAU
文件名      : apdbopr.h
创建时间    : 2013年 12月 05日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
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
		PRINT_CONNECT_STATUS,	//获取连接状态
		GETHDUMOIDANDUSERDOMAINMOID, //从server_info表中获取HDU的moid以及用户域moid
		GETPLATFORMISRENT,    //获取平台是自建还是租赁
	};
	
public:
	CAPInfoDBOpr();
	~CAPInfoDBOpr();
	
public:
	//初始化函数
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
