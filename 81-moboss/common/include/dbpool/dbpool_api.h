
/*=======================================================================
模块名      :
文件名      :dbpool_api.h
相关文件    :
文件实现功能:DbPool接口

作者        : 
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/04/23  1.0					      创建
=======================================================================*/

#ifndef _BD_POOL_API_H_
#define _BD_POOL_API_H_

#include "osp.h"
#include "kdvtype.h"
#include "multidbopr.h"
#include "dbinterface.h"
#include "dbmsgstruct.h"
#include "dbpooldebug.h"

struct TTaskParm;
// 初始化DB数据库
BOOL32 InitDbEngine(EmDBEngineType emDBEngineType);
// 释放DB数据库
BOOL32 ExitDbEngine(void);






// 数据库线程池消息回调函数指针
typedef void (*DBCallBackFunc)(u16 wEventID, u8 * pMsgBuf, u16 wMsgLen, IDbOpr* pDBOper);


template <class TDBOpr = IDbOpr, u8 m_byDBConNum = 6>
class CDBPool
{
public:
	CDBPool();

	~CDBPool();

public:		
	// 启动线程池(单个数据库)
	BOOL32 StartDBPool(const TDBConnectInfo& tDBConnectInfo, DBCallBackFunc pCallBackFun, u32 dwMaxMsgQueueSize=1024, const s8* szThreadName=NULL);
	// 启动线程池(多个数据库、也可用于单个数据库)
	BOOL32 StartDBPool(const std::vector<TDBConnectInfo>& vcDBConnectInfo, DBCallBackFunc pCallBackFun, u32 dwMaxMsgQueueSize=1024, const s8* szThreadName=NULL);

	// 向数据库线程投递消息
	BOOL32 InsertMsgToDBPool(u32 dwEvent, const u8* pbyMsg, u32 dwLen);

public:		//辅助性接口

    //获取数据库的连接状态
    //必须在StartDBPool之后调用，不然返回的vector size为0
    std::vector<CDBState> GetDBState() const;

	u32 GetMaxMsgQueueSize();
	u32 GetCurMsgQueueSize();
	u32 GetIdleMsgQueueSize();
    void DumpState();

private:
	TDBOpr m_tDBOpr[m_byDBConNum];
	TDBOprTaskInfo m_tDBTaskPool[m_byDBConNum];	// 数据库操作线程池
    TDBMsgQueue m_tDBMsgQueue;  // 消息队列

	BOOL32 m_bInited;	// 是否已经初始化	
private:
    u32 m_dwMsgInCome;  //消息进入数量
    u32 m_dwMsgdropped;     //消息丢失数量
    u32 m_dwMsgWaitingTop;  //最大等待处理数量

	TTaskParm m_tTaskParm[m_byDBConNum];
};










void* DbOprTaskProc( void * pParam );	// 数据库线程入口函数

struct TTaskParm
{
	DBCallBackFunc pCallBackFun;
	std::vector<TDBConnectInfo> vcDBConnectInfo;
	IDbOpr* pDBOpr;
	TDBOprTaskInfo* pDBOprTask;	//数据库操作线程
	TDBMsgQueue* pDBMsgQueue;	//消息队列
};


template <class TDBOpr , u8 m_byDBConNum>
CDBPool<TDBOpr, m_byDBConNum>::CDBPool()
{
	m_bInited = FALSE;

    m_dwMsgInCome = 0;
    m_dwMsgdropped = 0;
    m_dwMsgWaitingTop = 0;
}

template <class TDBOpr , u8 m_byDBConNum>
CDBPool<TDBOpr, m_byDBConNum>::~CDBPool()
{
	DBPoolLog(DP_LOGLEVEL_INFO, "开始析构DBPool....\n");
	
	u32 dwLoop=0;
	for (dwLoop=0; dwLoop<m_byDBConNum; dwLoop++)
	{
		m_tDBTaskPool[dwLoop].SetTerminateTask(TRUE);
	}	
	OspDelay(1*1000);
	
	for (dwLoop=0; dwLoop<m_byDBConNum; dwLoop++)
	{
		//任务ID置零
		TASKHANDLE hTaskId = m_tDBTaskPool[dwLoop].GetTaskId();
		if (0 != hTaskId)
		{
			OspTaskTerminate(hTaskId);
			m_tDBTaskPool[dwLoop].SetTaskId(0);
		}
	}

	ExitDbEngine();
}

// 启动线程池
template <class TDBOpr , u8 m_byDBConNum>
BOOL32 CDBPool<TDBOpr, m_byDBConNum>::StartDBPool(const TDBConnectInfo& tDBConnectInfo, DBCallBackFunc pCallBackFun, u32 dwMaxMsgQueueSize/*=1024*/, const s8* szThreadName/*NULL*/)
{
	std::vector<TDBConnectInfo> vcDBConnectInfo;
	vcDBConnectInfo.push_back(tDBConnectInfo);

	return StartDBPool(vcDBConnectInfo, pCallBackFun, dwMaxMsgQueueSize, szThreadName);
}


// 启动线程池
template <class TDBOpr , u8 m_byDBConNum>
BOOL32 CDBPool<TDBOpr, m_byDBConNum>::StartDBPool( const std::vector<TDBConnectInfo>& vcDBConnectInfo, DBCallBackFunc pCallBackFun, u32 dwMaxMsgQueueSize/*=1024*/, const s8* szThreadName/*NULL*/ )
{
	if (!IsOspInitd())
	{
#ifdef _DEBUG
		if (!OspInit(TRUE))
#else
			if (!OspInit(FALSE))
#endif
			{
				DBPoolLog(DP_LOGLEVEL_CRITICAL, "[StartDBPool] Osp Init failed!\n");
				printf( "[StartDBPool] Osp Init failed.\n" );
				return FALSE;
			}
	}
	
	
	if (NULL == pCallBackFun || 0 == dwMaxMsgQueueSize || TRUE == vcDBConnectInfo.empty())
	{
		DBPoolLog(DP_LOGLEVEL_ERROR, "[StartDBPool] NULL == pCallBackFun || 0 == dwMaxMsgQueueSize || TRUE == vcDBConnectInfo.empty()!\n");
		return FALSE;
	}
	
	if (TRUE == m_bInited)	
	{
		//已经初始化过
		DBPoolLog(DP_LOGLEVEL_WARNING, "[StartDBPool] DBpool已经被初始化过, return TRUE !\n");
		return TRUE;
	}
	
	
	//LINUX下需注册调试打印函数
	DBPoolAPIRegInLinux();
	

    // 初始化数据库
    BOOL32 bRet = InitDbEngine(vcDBConnectInfo[0].m_emDBEngineType);
    if (FALSE == bRet)
    {
        DBPoolLog(DP_LOGLEVEL_WARNING, "[StartDBPool] InitDbEngine fail! \n");
        //return FALSE;
    }
    DBPoolLog(DP_LOGLEVEL_WARNING, "[StartDBPool] InitDbEngine succ !\n");

	
	//
	m_tDBMsgQueue.SetQueueMaxSize(dwMaxMsgQueueSize);
	
	//创建线程任务
	TASKHANDLE hTaskId = 0;
	for (u32 dwLoop=0; dwLoop<m_byDBConNum; dwLoop++)
	{	
		//线程名称
		s8 achTaskName[64+1] = {0};
        if (szThreadName != NULL)
        {
            snprintf(achTaskName, sizeof(achTaskName), "%s_%u", szThreadName, dwLoop+1);
            achTaskName[ sizeof(achTaskName)-1 ] = '\0';
        }
        else
        {
            snprintf(achTaskName, sizeof(achTaskName), "DBPoolTask_%u", dwLoop+1);
            achTaskName[ sizeof(achTaskName)-1 ] = '\0';
        }
		
		//创建任务
		m_tTaskParm[dwLoop].pCallBackFun = pCallBackFun;
		m_tTaskParm[dwLoop].vcDBConnectInfo = vcDBConnectInfo;
		m_tTaskParm[dwLoop].pDBOpr = &m_tDBOpr[dwLoop];
		m_tTaskParm[dwLoop].pDBOprTask = &m_tDBTaskPool[dwLoop];
		m_tTaskParm[dwLoop].pDBMsgQueue = &m_tDBMsgQueue;
		
		hTaskId = OspTaskCreate( DbOprTaskProc, achTaskName, DBPOOL_TASK_PRIORITY, DBPOOL_TASK_STACKSIZE, (u32)&m_tTaskParm[dwLoop], 0 );
		if (0 == hTaskId)
		{
			DBPoolLog(DP_LOGLEVEL_ERROR, "[StartDBPool] OspTaskCreate %s Fail!\n", achTaskName);
			return FALSE;
		}
		m_tDBTaskPool[dwLoop].SetTaskId(hTaskId);
		OspTaskDelay(100);
	}
	
	m_bInited = TRUE;
	return TRUE;
}

// 向数据库线程投递消息
template <class TDBOpr , u8 m_byDBConNum>
BOOL32 CDBPool<TDBOpr, m_byDBConNum>::InsertMsgToDBPool( u32 dwEvent, const u8* pbyMsg, u32 dwLen )
{
    //允许消息内容为空
// 	if (NULL == pbyMsg)
// 	{
// 		DBPoolLog(DP_LOGLEVEL_ERROR, "[InsertMsgToDBPool] Insert Message %u(%s) Fail:NULL == pbyMsg\n", dwEvent, OspEventDesc(dwEvent));
// 		
// 		return FALSE;
// 	}

    //计数
    ++m_dwMsgInCome;
		
	// 向消息队列中插入消息
	TDBMsgItem tDBOprTaskMsgItem;
	tDBOprTaskMsgItem.SetEventID(dwEvent);
	tDBOprTaskMsgItem.SetDBMsgBody(pbyMsg, dwLen);
	
	BOOL32 bRet = m_tDBMsgQueue.PushBackOneMsg(tDBOprTaskMsgItem);
	if (FALSE == bRet)
	{
        ++m_dwMsgdropped;

		DBPoolLog(DP_LOGLEVEL_CRITICAL, "[InsertMsgToDBPool] Insert Message %u(%s) into DBMsgQueue Fail! msg Drop!\n", dwEvent, OspEventDesc(dwEvent));
		return bRet;
	}
	
    u32 dwCurNum = m_tDBMsgQueue.GetCurMsgNum();
    if (dwCurNum > m_dwMsgWaitingTop)
    {
        m_dwMsgWaitingTop = dwCurNum;
    }
    
    DBPoolLog(DP_LOGLEVEL_DETAIL, "[InsertMsgToDBPool] Insert Message %u(%s) into DBMsgQueue succeed!\n", dwEvent, OspEventDesc(dwEvent));	
	return bRet;
}

template <class TDBOpr , u8 m_byDBConNum>
std::vector<CDBState> CDBPool<TDBOpr, m_byDBConNum>::GetDBState() const
{
    //以第一个线程的状态代表所有线程的状态
    //DBConnectState是DBEngine中的一个成员变量，值由线程池中的线程设置，但调用此函数的肯定是另外一个线程；多线程问题暂不考虑;
    //必须在StartDBPool之后调用，不然返回的vector size为0
    return m_tDBOpr[0].GetDBState();
}

template <class TDBOpr , u8 m_byDBConNum>
u32 CDBPool<TDBOpr, m_byDBConNum>::GetMaxMsgQueueSize()
{
	return m_tDBMsgQueue.GetQueueMaxSize();
}

template <class TDBOpr , u8 m_byDBConNum>
u32 CDBPool<TDBOpr, m_byDBConNum>::GetCurMsgQueueSize()
{
	return m_tDBMsgQueue.GetCurMsgNum();
}

template <class TDBOpr , u8 m_byDBConNum>
u32 CDBPool<TDBOpr, m_byDBConNum>::GetIdleMsgQueueSize()
{
	u32 dwIdleNum = m_tDBMsgQueue.GetQueueMaxSize() - m_tDBMsgQueue.GetCurMsgNum();
	return dwIdleNum;
}

template <class TDBOpr , u8 m_byDBConNum>
void CDBPool<TDBOpr, m_byDBConNum>::DumpState()
{
    OspPrintf(TRUE, FALSE, "******************DBPool info begin*************** \n");
    OspPrintf(TRUE, FALSE, "    DBThreadNum = %u \n", m_byDBConNum);
    OspPrintf(TRUE, FALSE, "    maxMsgQueueSize = %u \n", m_tDBMsgQueue.GetQueueMaxSize());
    OspPrintf(TRUE, FALSE, "    msgInCome = %u \n", m_dwMsgInCome);
    u32 dwProcessed = m_dwMsgInCome - m_dwMsgdropped - m_tDBMsgQueue.GetCurMsgNum();
    OspPrintf(TRUE, FALSE, "    msgProcessed = %u \n", dwProcessed);
    OspPrintf(TRUE, FALSE, "    msgdropped = %u \n", m_dwMsgdropped);
    OspPrintf(TRUE, FALSE, "    msgWaitingTop = %u \n", m_dwMsgWaitingTop);
    OspPrintf(TRUE, FALSE, "******************DBPool info end****************\n");
}


#endif // _BD_POOL_API_H_ 
// END OF FILE




