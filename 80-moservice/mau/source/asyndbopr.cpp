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
函 数 名： Initial
功    能： 初始化
算法实现：
全局变量：
参    数： void
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2014/08/26  5.0      王也                   创建
2015/08/10  5.0		邹俊龙					重构
=============================================================================*/
BOOL32 CAsynDBOpr::Initial(const TDBConnectInfo& tDBConnectInfo, u32 dwTaskNum, u32 dwQueueSize)
{
	m_dwDBTaskNum = dwTaskNum;
	m_dwQueueSize = dwQueueSize;
	m_tDbConnectInfo = tDBConnectInfo;

	//数据库线程数 和 消息队列大小 有效性检验
	if (0 == m_dwDBTaskNum || 0 == m_dwQueueSize)
	{
		LogPrint(LOG_LVL_ERROR, GetModuleId(), "[%s::Initial] Initial failed! Task Num<%d>, Queue Size<%d>\n", GetModuleName(), m_dwDBTaskNum, m_dwQueueSize);
		return FALSE;
	}

	//创建线程池
	m_ptDBTaskPool = new TDBOprTaskInfo[m_dwDBTaskNum];
	if (NULL == m_ptDBTaskPool)
	{
		LogPrint(LOG_LVL_ERROR, GetModuleId(), "[%s::Initial] Creat Task Pool failed!!\n", GetModuleName());
		//出错返回
		ClearMemData();
		return FALSE;
	}

	//创建消息队列
	m_ptDBMsgQueue = new TDBMsgQueue(m_dwQueueSize);
	if (NULL == m_ptDBMsgQueue)
	{
		LogPrint(LOG_LVL_ERROR, GetModuleId(), "[%s::Initial] Creat DB Msg Queue failed!!\n", GetModuleName());
		//出错返回
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

	//创建数据库线程
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
			//放一个空信号量，让线程退出等待
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
函 数 名： PostOneMsgToDBOprTask
功    能： 投递一条消息给数据库线程
算法实现：
全局变量：
参    数： TDBOprTaskMsgItem & tDBTaskMsgItem
BOOL32 bPushQueue
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6  王也                   创建
2015/08/10  5.0		邹俊龙					重构
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
函 数 名： CancelQueueDBOpr
功    能：
算法实现：
全局变量：
参    数： u32 dwSessionID
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
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
函 数 名： DBTaskClientCallBack
功    能：
算法实现：
全局变量：
参    数： u16 wErrNum
LPCSTR lpSrvMsg
void * pContext
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
=============================================================================*/
void DBTaskClientCallBack(u16 wErrNum, LPCSTR lpSrvMsg, void * pContext)
{
	if (NULL == lpSrvMsg)
	{
		return;
	}

	//当wErrNum == 0,表示数据库连接已经断链
// 	if (NULL != pContext && 0 == wErrNum)
// 	{
// 		TDBOprTaskInfo * ptDBOprTaskInfo = (TDBOprTaskInfo*)pContext;
// 	}
	return;
}

/*=============================================================================
函 数 名： DBTaskSrvCallBack
功    能：
算法实现：
全局变量：
参    数： u16 wErrNum
LPCSTR lpSrvMsg
void * pContext
返 回 值： void
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
=============================================================================*/
void DBTaskSrvCallBack(u16 wErrNum, LPCSTR lpSrvMsg, void * pContext)
{
	if (NULL == lpSrvMsg)
	{
		return;
	}

	//当wErrNum == 0,表示数据库连接已经断链
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
函 数 名： DBOprTaskProc
功    能： 数据库线程入口函数
算法实现：
全局变量：
参    数： void * pParam
返 回 值： void *
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2015/04/09  KDVP5.0     阮楠楠					创建
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

	//动态转换
	CAsynDBOpr * pDBOpr = (CAsynDBOpr*)ptDBOprTaskInfo->GetUserData();
	if (NULL == pDBOpr)
	{
// 		LogPrint(LOG_LVL_ERROR, pDBOpr->GetModuleId(), "[%s] ConfInfoOpr point dynamic translate failed!\n", pDBOpr->GetModuleName());
		return 0;
	}

	//创建数据库连接对象
	pOprDB = new CDBEngine;
	if (NULL == pOprDB)
	{
		LogPrint(LOG_LVL_ERROR, pDBOpr->GetModuleId(), "[%s] new CDBEngine Err - TaskID(0x%x) return!!!\n", pDBOpr->GetModuleName(), ptDBOprTaskInfo->GetTaskID());
		return 0;
	}

	//设置回调函数
	pOprDB->SetCallBackUserData(DBTaskSrvCallBack, DBTaskClientCallBack, (u32)ptDBOprTaskInfo);

	BOOL32 bHasTask = FALSE;
	//进入线程循环
	while (!ptDBOprTaskInfo->IsTerminateTask())
	{
		pDBOpr->PreprocDBOpr(ptDBOprTaskInfo, pOprDB, dwTaskTimeOutNum, bHasTask);
		bHasTask = OspSemTakeByTime(pDBOpr->m_hSemTask, DB_CHECKDBTASK_SPAN);
		ptDBOprTaskInfo->SetIsIdle(!bHasTask);
	}

	//收尾
	if (pOprDB->IsConnectDB())
	{
		pOprDB->DisconnectDB();
	}

	//清理数据库连接对象
	if (NULL != pOprDB)
	{
		delete pOprDB;
	}

	LogPrint(LOG_LVL_DETAIL, pDBOpr->GetModuleId(), "[%s] is Running over - TaskID(0x%x)......\n", pDBOpr->GetModuleName(), ptDBOprTaskInfo->GetTaskID());

	return 0;
}

/*=============================================================================
函 数 名： PreprocDBOpr
功    能：
算法实现：
全局变量：
参    数： TDBOprTaskInfo *ptDBOprTaskInfo,
CDBEngine * ptDBEngine,
u32 &dwTaskTimeOutNum,
BOOL32 bTaskActive
返 回 值： BOOL32
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6 王也                    创建
2015/08/10  5.0		邹俊龙					重构
=============================================================================*/
void CAsynDBOpr::PreprocDBOpr(TDBOprTaskInfo *ptDBOprTaskInfo,
	CDBEngine * ptDBEngine,
	u32 &dwTaskTimeOutNum,
	BOOL32 bTaskActive)
{
	//重新检测数据库连接状态
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
	// 如果有消息需要处理，处理相应消息
	if (bTaskActive)
	{
		dwTaskTimeOutNum = 0;
		if (m_ptDBMsgQueue != NULL)
		{
			m_ptDBMsgQueue->PopFrontOneMsg(&tDBMsg);
			// 如果连接OK，直接处理
			if (bCurDBConOK || !tDBMsg.IsDurable())
			{
				DBOprReqEntry(&tDBMsg, ptDBEngine);
			}
			else
			{
				//如果消息持久化，则压回队列，否则消耗掉
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

	// 如果连接正常
	if (!bCurDBConOK)
	{
		TDBConnectInfo tDBInfo = GetDBConnectInfo();
		BOOL32 bConnectRet = (ERR_DB_SUCCESS == ptDBEngine->ConnectDB(&tDBInfo));
		if (bConnectRet)
		{
			//连接成功后设置编码格式为UTF-8
			LogPrint(LOG_LVL_WARNING, GetModuleId(), "[%s::PreprocDBOpr] task<0x%x> connect to db!\n", GetModuleName(), ptDBOprTaskInfo->GetTaskID());
			BOOL32 bRet = ptDBEngine->SetCharacterSet(DB_CHARSET_UTF8);
			LogPrint(LOG_LVL_WARNING, GetModuleId(), "[%s::PreprocDBOpr] task<0x%x> Set CharacterSet %s!\n", GetModuleName(), ptDBOprTaskInfo->GetTaskID(), bRet ? "Successed" : "Failed");
			//修改数据库连接状态
			ptDBOprTaskInfo->SetDBConState(TRUE);
		}
		else
		{
			// 由于有任务的话，信号量会被触发，因此需要在这里延迟
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

	//是否需要返回结果集改为false
	u16 wRet = ptDBEnging->ExecSql(szSql);
	if (ERR_DB_SUCCESS != wRet)
	{
		return FALSE;
	}

	return TRUE;

}

/*=============================================================================
函 数 名： ClearMemData
功    能：
算法实现：
全局变量：
参    数：
返 回 值：
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/12/05  KDVP2.6     王也                    创建
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
