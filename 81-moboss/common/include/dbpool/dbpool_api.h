
/*=======================================================================
ģ����      :
�ļ���      :dbpool_api.h
����ļ�    :
�ļ�ʵ�ֹ���:DbPool�ӿ�

����        : 
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/04/23  1.0					      ����
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
// ��ʼ��DB���ݿ�
BOOL32 InitDbEngine(EmDBEngineType emDBEngineType);
// �ͷ�DB���ݿ�
BOOL32 ExitDbEngine(void);






// ���ݿ��̳߳���Ϣ�ص�����ָ��
typedef void (*DBCallBackFunc)(u16 wEventID, u8 * pMsgBuf, u16 wMsgLen, IDbOpr* pDBOper);


template <class TDBOpr = IDbOpr, u8 m_byDBConNum = 6>
class CDBPool
{
public:
	CDBPool();

	~CDBPool();

public:		
	// �����̳߳�(�������ݿ�)
	BOOL32 StartDBPool(const TDBConnectInfo& tDBConnectInfo, DBCallBackFunc pCallBackFun, u32 dwMaxMsgQueueSize=1024, const s8* szThreadName=NULL);
	// �����̳߳�(������ݿ⡢Ҳ�����ڵ������ݿ�)
	BOOL32 StartDBPool(const std::vector<TDBConnectInfo>& vcDBConnectInfo, DBCallBackFunc pCallBackFun, u32 dwMaxMsgQueueSize=1024, const s8* szThreadName=NULL);

	// �����ݿ��߳�Ͷ����Ϣ
	BOOL32 InsertMsgToDBPool(u32 dwEvent, const u8* pbyMsg, u32 dwLen);

public:		//�����Խӿ�

    //��ȡ���ݿ������״̬
    //������StartDBPool֮����ã���Ȼ���ص�vector sizeΪ0
    std::vector<CDBState> GetDBState() const;

	u32 GetMaxMsgQueueSize();
	u32 GetCurMsgQueueSize();
	u32 GetIdleMsgQueueSize();
    void DumpState();

private:
	TDBOpr m_tDBOpr[m_byDBConNum];
	TDBOprTaskInfo m_tDBTaskPool[m_byDBConNum];	// ���ݿ�����̳߳�
    TDBMsgQueue m_tDBMsgQueue;  // ��Ϣ����

	BOOL32 m_bInited;	// �Ƿ��Ѿ���ʼ��	
private:
    u32 m_dwMsgInCome;  //��Ϣ��������
    u32 m_dwMsgdropped;     //��Ϣ��ʧ����
    u32 m_dwMsgWaitingTop;  //���ȴ���������

	TTaskParm m_tTaskParm[m_byDBConNum];
};










void* DbOprTaskProc( void * pParam );	// ���ݿ��߳���ں���

struct TTaskParm
{
	DBCallBackFunc pCallBackFun;
	std::vector<TDBConnectInfo> vcDBConnectInfo;
	IDbOpr* pDBOpr;
	TDBOprTaskInfo* pDBOprTask;	//���ݿ�����߳�
	TDBMsgQueue* pDBMsgQueue;	//��Ϣ����
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
	DBPoolLog(DP_LOGLEVEL_INFO, "��ʼ����DBPool....\n");
	
	u32 dwLoop=0;
	for (dwLoop=0; dwLoop<m_byDBConNum; dwLoop++)
	{
		m_tDBTaskPool[dwLoop].SetTerminateTask(TRUE);
	}	
	OspDelay(1*1000);
	
	for (dwLoop=0; dwLoop<m_byDBConNum; dwLoop++)
	{
		//����ID����
		TASKHANDLE hTaskId = m_tDBTaskPool[dwLoop].GetTaskId();
		if (0 != hTaskId)
		{
			OspTaskTerminate(hTaskId);
			m_tDBTaskPool[dwLoop].SetTaskId(0);
		}
	}

	ExitDbEngine();
}

// �����̳߳�
template <class TDBOpr , u8 m_byDBConNum>
BOOL32 CDBPool<TDBOpr, m_byDBConNum>::StartDBPool(const TDBConnectInfo& tDBConnectInfo, DBCallBackFunc pCallBackFun, u32 dwMaxMsgQueueSize/*=1024*/, const s8* szThreadName/*NULL*/)
{
	std::vector<TDBConnectInfo> vcDBConnectInfo;
	vcDBConnectInfo.push_back(tDBConnectInfo);

	return StartDBPool(vcDBConnectInfo, pCallBackFun, dwMaxMsgQueueSize, szThreadName);
}


// �����̳߳�
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
		//�Ѿ���ʼ����
		DBPoolLog(DP_LOGLEVEL_WARNING, "[StartDBPool] DBpool�Ѿ�����ʼ����, return TRUE !\n");
		return TRUE;
	}
	
	
	//LINUX����ע����Դ�ӡ����
	DBPoolAPIRegInLinux();
	

    // ��ʼ�����ݿ�
    BOOL32 bRet = InitDbEngine(vcDBConnectInfo[0].m_emDBEngineType);
    if (FALSE == bRet)
    {
        DBPoolLog(DP_LOGLEVEL_WARNING, "[StartDBPool] InitDbEngine fail! \n");
        //return FALSE;
    }
    DBPoolLog(DP_LOGLEVEL_WARNING, "[StartDBPool] InitDbEngine succ !\n");

	
	//
	m_tDBMsgQueue.SetQueueMaxSize(dwMaxMsgQueueSize);
	
	//�����߳�����
	TASKHANDLE hTaskId = 0;
	for (u32 dwLoop=0; dwLoop<m_byDBConNum; dwLoop++)
	{	
		//�߳�����
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
		
		//��������
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

// �����ݿ��߳�Ͷ����Ϣ
template <class TDBOpr , u8 m_byDBConNum>
BOOL32 CDBPool<TDBOpr, m_byDBConNum>::InsertMsgToDBPool( u32 dwEvent, const u8* pbyMsg, u32 dwLen )
{
    //������Ϣ����Ϊ��
// 	if (NULL == pbyMsg)
// 	{
// 		DBPoolLog(DP_LOGLEVEL_ERROR, "[InsertMsgToDBPool] Insert Message %u(%s) Fail:NULL == pbyMsg\n", dwEvent, OspEventDesc(dwEvent));
// 		
// 		return FALSE;
// 	}

    //����
    ++m_dwMsgInCome;
		
	// ����Ϣ�����в�����Ϣ
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
    //�Ե�һ���̵߳�״̬���������̵߳�״̬
    //DBConnectState��DBEngine�е�һ����Ա������ֵ���̳߳��е��߳����ã������ô˺����Ŀ϶�������һ���̣߳����߳������ݲ�����;
    //������StartDBPool֮����ã���Ȼ���ص�vector sizeΪ0
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




