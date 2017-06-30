/*****************************************************************************
模块名      : css
文件名      : confinfodbopr.h
创建时间    : 2015年 04月 09日
实现功能    : 
作者        : 阮楠楠
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/04/09  KDVP5.0     阮楠楠        创建
******************************************************************************/
#ifndef	_CONFINFO_DB_OPR_H_
#define	_CONFINFO_DB_OPR_H_

#include "asyndbopr.h"

class CConfInfoDBOpr : public CAsynDBOpr
{
	enum
	{
		CLEARDBDATAOPR = 1,
		ADDCONFINFOOPR,
		DELCONFINFOOPR,
		MODCONFNAMEOPR,
		PRINT_CONNECT_STATUS,	//获取连接状态
	};

public:
	CConfInfoDBOpr();
	~CConfInfoDBOpr();
	virtual const s8* GetModuleName();
	virtual u16 GetModuleId();
public:
	//初始化函数
	u8 ClearDBData( void );
	u8 AddConfInfo(const s8* pchConfE164, const s8* pchConfName, const s8* pchPwd);
	u8 DelConfInfo(const s8* pchConfE164);
	u8 ModConfName(const s8* pchConfE164, const s8* pchConfName);
	void PrintConnectStatus();

protected:
	BOOL32 DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine);

	BOOL32 ProcClearConfInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcAddConfInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcDelConfInfo(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcModConfName(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	
	const s8 * GetDBOprNameByTransId( u32 dwTransId );
};

#endif
