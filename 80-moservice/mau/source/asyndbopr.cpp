#include "inc.h"
#include "asyndbopr.h"
#include "osp.h"

CAsynDBOpr::CAsynDBOpr() :
m_ptDBTaskPool(NULL),
m_ptDBMsgQueue(NULL),
m_dwDBTaskNum(DEFAULT_DBTHREAD_NUM),
m_dwQueueSize(DEFAULT_DBQUEUEMSG_NUM),
m_hSemTask(NULL),
m_hSemQueue(NULL)
{

}

//lint -sem(CAsynDBOpr::Quit,cleanup)
//lint -sem(CAsynDBOpr::ClearMemData,cleanup)
CAsynDBOpr::~CAsynDBOpr()
{
	Quit();
	ClearMemData();
}

const TDBConnectInfo& CAsynDBOpr::GetDBConnectInfo() const
{
	return m_tDbConnectInfo;
}

/*=============================================================================
�� �� ���� Initial
��    �ܣ� ��ʼ��
�㷨ʵ�֣�
ȫ�ֱ�����
��    ���� void
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2014/08/26  5.0      ��Ҳ                   ����
2015/08/10  5.0		�޿���					�ع�
=============================================================================*/
BOOL32 CAsynDBOpr::Initial(const TDBConnectInfo& tDBConnectInfo, u32 dwTaskNum, u32 dwQueueSize)
{
	m_dwDBTaskNum = dwTaskNum;
	m_dwQueueSize = dwQueueSize;
	m_tDbConnectInfo = tDBConnectInfo;

	//���ݿ��߳��� �� ��Ϣ���д�С ��Ч�Լ���
	if (0 == m_dwDBTaskNum || 0 == m_dwQueueSize)
	{
		LogPrint(LOG_LVL_ERROR, GetModuleId(), "[%s::Initial] Initial failed! Task Num<%d>, Queue Size<%d>\n", GetModuleName(), m_dwDBTaskNum, m_dwQueueSize);
		return FALSE;
	}

	//�����̳߳�
	m_ptDBTaskPool = new TDBOprTaskInfo[m_dwDBTaskNum];
	if (NULL == m_ptDBTaskPool)
	{
		LogPrint(LOG_LVL_ERROR, GetModuleId(), "[%s::Initial] Creat Task Pool failed!!\n", GetModuleName());
		//������
		ClearMemData();
		return FALSE;
	}

	//������Ϣ����
	m_ptDBMsgQueue = new TDBMsgQueue(m_dwQueueSize);
	if (NULL == m_ptDBMsgQueue)
	{
		LogPrint(LOG_LVL_ERROR, GetModuleId(), "[%s::Initial] Creat DB Msg Queue failed!!\n", GetModuleName());
		//������
		ClearMemData();
		return FALSE;
	}

	if (FALSE == OspSemBCreate(&m_hSemQueue))
	{
		LogPrint(LOG_LVL_ERROR, GetModuleId(), "[%s::Initial] Creat DB Queue BSem failed!!\n", GetModuleName());
		ClearMemData();
		return FALSE;
	}

	m_ptDBMsgQueue->SetQueueSem(m_hSemQueue);

	if (FALSE == OspSemCCreate(&m_hSemTask, 0, 65536))
	{
		LogPrint(LOG_LVL_ERROR, GetModuleId(), "[%s::Initial] Creat DB Task CSem failed!!\n", GetModuleName());
		return FALSE;
	}

	//�������ݿ��߳�
	TASKHANDLE hTaskID = 0;
	if (NULL != m_ptDBTaskPool)
	{
		u32 dwLoop = 0;
		for (dwLoop = 0; dwLoop < m_dwDBTaskNum; dwLoop++)
		{
			m_ptDBTaskPool[dwLoop].SetUserData(this);
			hTaskID = OspTaskCreate(DBOprTaskProc, GetModuleName(), DBOPRTASK_PRIORITY, DBOPRTASK_STACKSIZE, (u32)&m_ptDBTaskPool[dwLoop], 0);
			if (0 == hTaskID)
			{
				LogPrint(LOG_LVL_ERROR, GetModuleId(), "[%s::Initial] Creat DB Task failed!!\n", GetModuleName());
				ClearMemData();
				return FALSE;
			}

			m_ptDBTaskPool[dwLoop].SetTaskID(hTaskID);
		}
	}

	return TRUE;
}

void CAsynDBOpr::Quit()
{
	u32 i = 0;
	for (; i < m_dwDBTaskNum; ++i)
	{
		if (m_ptDBTaskPool != NULL && m_ptDBTaskPool[i].GetTaskID())
		{
			m_ptDBTaskPool[i].SetTerminateTask(TRUE);
			//��һ�����ź��������߳��˳��ȴ�
			OspSemGive(m_hSemTask);
		}
	}

	i = 0;
	for (; i < m_dwDBTaskNum; ++i)
	{
		if (m_ptDBTaskPool != NULL)
		{
			WaitForThreadExit(m_ptDBTaskPool[i].GetTaskID());
			m_ptDBTaskPool[i].SetTaskID(0);
		}
	}

	if (m_hSemTask != NULL)
	{
		OspSemDelete(m_hSemTask);
		m_hSemTask = NULL;
	}

	if (m_hSemQueue != NULL)
	{
		OspSemDelete(m_hSemQueue);
		m_hSemQueue = NULL;
	}
}

u32 CAsynDBOpr::GetDBTaskNum(void)
{
	return m_dwDBTaskNum;
}

u32 CAsynDBOpr::GetQueueSize(void)
{
	return m_dwQueueSize;
}

/*=============================================================================
�� �� ���� PostOneMsgToDBOprTask
��    �ܣ� Ͷ��һ����Ϣ�����ݿ��߳�
�㷨ʵ�֣�
ȫ�ֱ�����
��    ���� TDBOprTaskMsgItem & tDBTaskMsgItem
BOOL32 bPushQueue
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6  ��Ҳ                   ����
2015/08/10  5.0		�޿���					�ع�
=============================================================================*/
BOOL32 CAsynDBOpr::PostOneMsgToDBOprTask(TDBOprTaskMsgItem & tDBTaskMsgItem, BOOL32 bPushQueue)
{
	if (NULL == m_ptDBMsgQueue)
	{
		return FALSE;
	}
	tDBTaskMsgItem.SetDurable(bPushQueue);
	m_ptDBMsgQueue->PushBackOneMsg(tDBTaskMsgItem);
	OspSemGive(m_hSemTask);
	return TRUE;
}

/*=============================================================================
�� �� ���� CancelQueueDBOpr
��    �ܣ�
�㷨ʵ�֣�
ȫ�ֱ�����
��    ���� u32 dwSessionID
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
=============================================================================*/
BOOL32 CAsynDBOpr::CancelQueueDBOpr(u32 dwSessionID)
{
	if (0 == dwSessionID || NULL == m_ptDBMsgQueue)
	{
		return FALSE;
	}

	u32 dwMsgNum = m_ptDBMsgQueue->GetCurMsgNum();
	for (u32 dwLoop = 1; dwLoop <= dwMsgNum; dwLoop++)
	{
		TDBOprTaskMsgItem tDBMsg = m_ptDBMsgQueue->GetNextMsg(dwLoop);
		if (dwSessionID == tDBMsg.GetSessionID())
		{
			m_ptDBMsgQueue->RemoveOneMsg(dwLoop);
		}
	}
	return TRUE;
}

BOOL32 CAsynDBOpr::PrintStatus(CDBEngine * ptDBEnging)
{
	if (!ptDBEnging) return FALSE;
	
	OspPrintf(TRUE, FALSE, "%s DB connect status = %d.\n",m_tDbConnectInfo.GetDBName(), CheckDBConnectState(ptDBEnging));

	return TRUE;
}

/*=============================================================================
�� �� ���� DBTaskClientCallBack
��    �ܣ�
�㷨ʵ�֣�
ȫ�ֱ�����
��    ���� u16 wErrNum
LPCSTR lpSrvMsg
void * pContext
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
void DBTaskClientCallBack(u16 wErrNum, LPCSTR lpSrvMsg, void * pContext)
{
	if (NULL == lpSrvMsg)
	{
		return;
	}

	//��wErrNum == 0,��ʾ���ݿ������Ѿ�����
// 	if (NULL != pContext && 0 == wErrNum)
// 	{
// 		TDBOprTaskInfo * ptDBOprTaskInfo = (TDBOprTaskInfo*)pContext;
// 	}
	return;
}

/*=============================================================================
�� �� ���� DBTaskSrvCallBack
��    �ܣ�
�㷨ʵ�֣�
ȫ�ֱ�����
��    ���� u16 wErrNum
LPCSTR lpSrvMsg
void * pContext
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
void DBTaskSrvCallBack(u16 wErrNum, LPCSTR lpSrvMsg, void * pContext)
{
	if (NULL == lpSrvMsg)
	{
		return;
	}

	//��wErrNum == 0,��ʾ���ݿ������Ѿ�����
	if (NULL != pContext && 0 == wErrNum)
	{
	
		TDBOprTaskInfo * ptDBOprTaskInfo = (TDBOprTaskInfo*)pContext;
		CAsynDBOpr* pDBOpr = (CAsynDBOpr*)ptDBOprTaskInfo->GetUserData();
		if (pDBOpr)
		{
			LogPrint(LOG_LVL_KEYSTATUS, pDBOpr->GetModuleId(), "[%s::DBTaskSrvCallBack] disconnect.\n", pDBOpr->GetModuleName());
		}
	}
}


/*=============================================================================
�� �� ���� DBOprTaskProc
��    �ܣ� ���ݿ��߳���ں���
�㷨ʵ�֣�
ȫ�ֱ�����
��    ���� void * pParam
�� �� ֵ�� void *
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/04/09  KDVP5.0     ����					����
=============================================================================*/
void* CAsynDBOpr::DBOprTaskProc(void * pParam)
{
	if (NULL == pParam)
	{
		return 0;
	}

	TDBOprTaskInfo * ptDBOprTaskInfo = (TDBOprTaskInfo *)pParam;
	CDBEngine * pOprDB = NULL;
	u32 dwTaskTimeOutNum = 0;

	//��̬ת��
	CAsynDBOpr * pDBOpr = (CAsynDBOpr*)ptDBOprTaskInfo->GetUserData();
	if (NULL == pDBOpr)
	{
// 		LogPrint(LOG_LVL_ERROR, pDBOpr->GetModuleId(), "[%s] ConfInfoOpr point dynamic translate failed!\n", pDBOpr->GetModuleName());
		return 0;
	}

	//�������ݿ����Ӷ���
	pOprDB = new CDBEngine;
	if (NULL == pOprDB)
	{
		LogPrint(LOG_LVL_ERROR, pDBOpr->GetModuleId(), "[%s] new CDBEngine Err - TaskID(0x%x) return!!!\n", pDBOpr->GetModuleName(), ptDBOprTaskInfo->GetTaskID());
		return 0;
	}

	//���ûص�����
	pOprDB->SetCallBackUserData(DBTaskSrvCallBack, DBTaskClientCallBack, (u32)ptDBOprTaskInfo);

	BOOL32 bHasTask = FALSE;
	//�����߳�ѭ��
	while (!ptDBOprTaskInfo->IsTerminateTask())
	{
		pDBOpr->PreprocDBOpr(ptDBOprTaskInfo, pOprDB, dwTaskTimeOutNum, bHasTask);
		bHasTask = OspSemTakeByTime(pDBOpr->m_hSemTask, DB_CHECKDBTASK_SPAN);
		ptDBOprTaskInfo->SetIsIdle(!bHasTask);
	}

	//��β
	if (pOprDB->IsConnectDB())
	{
		pOprDB->DisconnectDB();
	}

	//�������ݿ����Ӷ���
	if (NULL != pOprDB)
	{
		delete pOprDB;
	}

	LogPrint(LOG_LVL_DETAIL, pDBOpr->GetModuleId(), "[%s] is Running over - TaskID(0x%x)......\n", pDBOpr->GetModuleName(), ptDBOprTaskInfo->GetTaskID());

	return 0;
}

/*=============================================================================
�� �� ���� PreprocDBOpr
��    �ܣ�
�㷨ʵ�֣�
ȫ�ֱ�����
��    ���� TDBOprTaskInfo *ptDBOprTaskInfo,
CDBEngine * ptDBEngine,
u32 &dwTaskTimeOutNum,
BOOL32 bTaskActive
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6 ��Ҳ                    ����
2015/08/10  5.0		�޿���					�ع�
=============================================================================*/
void CAsynDBOpr::PreprocDBOpr(TDBOprTaskInfo *ptDBOprTaskInfo,
	CDBEngine * ptDBEngine,
	u32 &dwTaskTimeOutNum,
	BOOL32 bTaskActive)
{
	//���¼�����ݿ�����״̬
	BOOL32 bCurDBConOK = ptDBEngine->IsConnectDB();

	if (bCurDBConOK)
	{
		if (dwTaskTimeOutNum >= DB_CHECKDBCONNECT_TIMES || bTaskActive)
		{
			dwTaskTimeOutNum = 0;
			if (!CheckDBConnectState(ptDBEngine))
			{
				bCurDBConOK = FALSE;
				LogPrint(LOG_LVL_WARNING, GetModuleId(), "[%s::PreprocDBOpr] task<0x%x> disconnect with db, clear status!\n", GetModuleName(), ptDBOprTaskInfo->GetTaskID());
				ptDBEngine->DisconnectDB();
			}
		}
	}

	TDBOprTaskMsgItem tDBMsg;
	// �������Ϣ��Ҫ����������Ӧ��Ϣ
	if (bTaskActive)
	{
		dwTaskTimeOutNum = 0;
		if (m_ptDBMsgQueue != NULL)
		{
			m_ptDBMsgQueue->PopFrontOneMsg(&tDBMsg);
			// �������OK��ֱ�Ӵ���
			if (bCurDBConOK || !tDBMsg.IsDurable())
			{
				DBOprReqEntry(&tDBMsg, ptDBEngine);
			}
			else
			{
				//�����Ϣ�־û�����ѹ�ض��У��������ĵ�
				m_ptDBMsgQueue->PushBackOneMsg(tDBMsg);
				OspSemGive(m_hSemTask);
			}		
		}
		else
		{
			LogPrint(LOG_LVL_ERROR, GetModuleId(), "[%s]m_ptDBMsgQueue is NULL\n", GetModuleName());
		}
	}
	else
	{
		++dwTaskTimeOutNum;
	}

	// �����������
	if (!bCurDBConOK)
	{
		TDBConnectInfo tDBInfo = GetDBConnectInfo();
		BOOL32 bConnectRet = (ERR_DB_SUCCESS == ptDBEngine->ConnectDB(&tDBInfo));
		if (bConnectRet)
		{
			//���ӳɹ������ñ����ʽΪUTF-8
			LogPrint(LOG_LVL_WARNING, GetModuleId(), "[%s::PreprocDBOpr] task<0x%x> connect to db!\n", GetModuleName(), ptDBOprTaskInfo->GetTaskID());
			BOOL32 bRet = ptDBEngine->SetCharacterSet(DB_CHARSET_UTF8);
			LogPrint(LOG_LVL_WARNING, GetModuleId(), "[%s::PreprocDBOpr] task<0x%x> Set CharacterSet %s!\n", GetModuleName(), ptDBOprTaskInfo->GetTaskID(), bRet ? "Successed" : "Failed");
			//�޸����ݿ�����״̬
			ptDBOprTaskInfo->SetDBConState(TRUE);
		}
		else
		{
			// ����������Ļ����ź����ᱻ�����������Ҫ�������ӳ�
			if (bTaskActive)
			{
				OspDelay(DB_CHECKDBTASK_SPAN);
			}
		}
	}
}

BOOL32 CAsynDBOpr::CheckDBConnectState(CDBEngine * ptDBEnging)
{
	if (NULL == ptDBEnging)
	{
		return FALSE;
	}

	s8  szSql[50] = { 0 };
	sprintf(szSql, "select 2");

	//�Ƿ���Ҫ���ؽ������Ϊfalse
	u16 wRet = ptDBEnging->ExecSql(szSql);
	if (ERR_DB_SUCCESS != wRet)
	{
		return FALSE;
	}

	return TRUE;

}

/*=============================================================================
�� �� ���� ClearMemData
��    �ܣ�
�㷨ʵ�֣�
ȫ�ֱ�����
��    ����
�� �� ֵ��
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/12/05  KDVP2.6     ��Ҳ                    ����
=============================================================================*/
void CAsynDBOpr::ClearMemData(void)
{
	if (NULL != m_ptDBTaskPool)
	{
		delete[] m_ptDBTaskPool;
		m_ptDBTaskPool = NULL;
	}

	if (NULL != m_ptDBMsgQueue)
	{
		delete m_ptDBMsgQueue;
		m_ptDBMsgQueue = NULL;
	}

}
