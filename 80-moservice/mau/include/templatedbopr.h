/*****************************************************************************
ģ����      : MPCD
�ļ���      : confstoreopr.h
����ʱ��    : 2012�� 01�� 05��
ʵ�ֹ���    : 
����        : ���Ĳ�
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
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

	//��ʼ������
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

