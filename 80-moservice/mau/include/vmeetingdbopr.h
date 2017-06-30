/*****************************************************************************
模块名      : MAU
文件名      : vmeetingdbopr.h
创建时间    : 2013年 12月 05日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifndef	_VMEETING_DB_OPR_H_
#define	_VMEETING_DB_OPR_H_

#include "asyndbopr.h"

//数据库表名定义
#define MEETING_TABLE	             "meeting"
#define VMEETING_TABLE               "v_meeting"

//meeting数据库,meeting表字段名定义
#define M_ID						   "id"
#define M_SUBJECT                      "subject"
#define M_COMPANY_MOID                 "company_moid"
#define M_START_TIME                   "start_time"
#define M_END_TIME                     "end_time"
#define M_STATE                        "state"
#define M_CREATER_BY                   "created_by"
#define M_CREATER_AT                   "created_at"
#define M_ORGANIZER_NAME               "organizer_name"

//meeting数据库,v_meeting表字段名定义
#define VM_MEETING_ID                   "meeting_id"
#define VM_E164							"e164"
#define VM_COMPANY_MOID					"company_moid"
#define	VM_STATE						"state"
#define VM_BITERATE_VALUE				"biterate_value"
#define VM_PUBLIC_MEETING				"public_meeting"
#define VM_DOBULEFLOW_LIMIT				"doubleflow_limit"
#define VM_PASSWORD						"password"
#define	VM_ENCRYPTED_TYPE				"encrypted_type"
#define VM_ENCRYPTED_KEY				"encrypted_key"
#define VM_MEETING_TYPE					"meeting_type"
#define VM_MTTING_CLOSED				"closed_meeting"
#define VM_MEETING_OPENMODE				"meeting_safe"

class CVMeetingDBOpr : public CAsynDBOpr
{
public:
	enum
	{
		CLEARDBDATAOPR = 1,
		STATE_CHECK,        	            //会议状态校验
		CREATE_MEETING_OPR,	                //非模板创会
		CHANGE_VM_STATE,                    //修改vm状态
		CREATE_MEETING_FAILED,              //创会失败
		AHEAD_MEETING,	                    //提前召开
		SET_MEETING_E164_OPR,	            //设置会议数据库中E164
		RELEASE_CONF_OPR,	             	//结束会议操作
		CREATE_FAIL_CHANGE_STATE_OPR,		//创会失败修改状态
		DELAY_CONFTIME_ORP,					//延长会议
		CHANGE_CONF_NAME,					//修改会议名称
		SUB_CONF_DURATION_ORP,      		//缩短会议运行时间操作
		UPDATE_VMEETING_STATE,				//修改VMeeting状态
		PRINT_CONNECT_STATUS,	   //获取连接状态
		PRINT_PROCEDURE_STATUS,			//获取存储过程状态
	};
	

	CVMeetingDBOpr();
	~CVMeetingDBOpr();
	
public:
	//初始化函数
	void   StateCheck();
	void   CreateMeeting( TDBOprTaskMsgItem & tDBMsg );
	void   ChangeVmState( TDBOprTaskMsgItem & tDBMsg );
	void   CreateMeetingFailed(const u32 nMeetingID);
	void   AheadMeeting( TDBOprTaskMsgItem & tDBMsg );
	void   SetDBMeetingE164(const s8* pchConfE164, const u32 dwMeetingID);
	void   ReleaseConf(const s8* pchConfE164, const s8* pchDomainMOID, const u32 nMeetingID);
	void   ReleaseConf(TDBOprTaskMsgItem & tDBMsg);
	void   DelayConf( TDBOprTaskMsgItem & tDBMsg );
	void   ChangeConfName( TDBOprTaskMsgItem & tDBMsg );
	void   SubConfDuration( TDBOprTaskMsgItem & tDBMsg );
	void   UpdateVMeetingState(TDBOprTaskMsgItem & tDBMsg);
	void   PrintProcedureStatus();

	void   PrintConnectStatus();
	virtual const s8* GetModuleName();
	virtual u16 GetModuleId();
	
protected:
	BOOL32 DBOprReqEntry(TDBOprTaskMsgItem *ptTaskInfo, CDBEngine * ptDBEngine);
	const s8 * GetDBOprNameByTransId(u32 dwTransId);
	BOOL32 ProcStateCheck(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcCreateMeetingOpr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcChangeVmState(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcCreateMeetingFailed(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcAheadMeeting(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcSetMeetingE164Opr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcReleaseConfOpr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcDelayConfOpr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcChangeConfNameOpr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcSubConfDurationOpr(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcUpdateVMeetingState(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	BOOL32 ProcPrintProcedureStatus(TDBOprTaskMsgItem * ptTaskInfo, CDBEngine * ptDBEngine);
	//数据库中会议加密模式适配
	u8 EncryptModeAdapt( u8 bySrcMode );

};

#endif // _VMEETING_DB_OPR_H_
