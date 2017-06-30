/*****************************************************************************
ģ����      : css
�ļ���      : confinfodbopr.h
����ʱ��    : 2015�� 04�� 09��
ʵ�ֹ���    : 
����        : ����
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2015/04/09  KDVP5.0     ����        ����
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
		PRINT_CONNECT_STATUS,	//��ȡ����״̬
	};

public:
	CConfInfoDBOpr();
	~CConfInfoDBOpr();
	virtual const s8* GetModuleName();
	virtual u16 GetModuleId();
public:
	//��ʼ������
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
