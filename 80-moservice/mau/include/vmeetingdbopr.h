/*****************************************************************************
ģ����      : MAU
�ļ���      : vmeetingdbopr.h
����ʱ��    : 2013�� 12�� 05��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
******************************************************************************/
#ifndef	_VMEETING_DB_OPR_H_
#define	_VMEETING_DB_OPR_H_

#include "asyndbopr.h"

//���ݿ��������
#define MEETING_TABLE	             "meeting"
#define VMEETING_TABLE               "v_meeting"

//meeting���ݿ�,meeting���ֶ�������
#define M_ID						   "id"
#define M_SUBJECT                      "subject"
#define M_COMPANY_MOID                 "company_moid"
#define M_START_TIME                   "start_time"
#define M_END_TIME                     "end_time"
#define M_STATE                        "state"
#define M_CREATER_BY                   "created_by"
#define M_CREATER_AT                   "created_at"
#define M_ORGANIZER_NAME               "organizer_name"

//meeting���ݿ�,v_meeting���ֶ�������
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
		STATE_CHECK,        	            //����״̬У��
		CREATE_MEETING_OPR,	                //��ģ�崴��
		CHANGE_VM_STATE,                    //�޸�vm״̬
		CREATE_MEETING_FAILED,              //����ʧ��
		AHEAD_MEETING,	                    //��ǰ�ٿ�
		SET_MEETING_E164_OPR,	            //���û������ݿ���E164
		RELEASE_CONF_OPR,	             	//�����������
		CREATE_FAIL_CHANGE_STATE_OPR,		//����ʧ���޸�״̬
		DELAY_CONFTIME_ORP,					//�ӳ�����
		CHANGE_CONF_NAME,					//�޸Ļ�������
		SUB_CONF_DURATION_ORP,      		//���̻�������ʱ�����
		UPDATE_VMEETING_STATE,				//�޸�VMeeting״̬
		PRINT_CONNECT_STATUS,	   //��ȡ����״̬
		PRINT_PROCEDURE_STATUS,			//��ȡ�洢����״̬
	};
	

	CVMeetingDBOpr();
	~CVMeetingDBOpr();
	
public:
	//��ʼ������
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
	//���ݿ��л������ģʽ����
	u8 EncryptModeAdapt( u8 bySrcMode );

};

#endif // _VMEETING_DB_OPR_H_
