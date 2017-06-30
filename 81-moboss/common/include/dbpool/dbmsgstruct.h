
#ifndef _BD_MSG_QUEUE_H_
#define _BD_MSG_QUEUE_H_

#pragma warning (disable:4786 )

#include "osp.h"
#include "rwlock_api.h"
#include <queue>

//17. 数据库处理的消息单元
struct TDBMsgItem
{
public:
    TDBMsgItem( void )
	{
        m_wMsgEventID = 0;
        m_pMsgBuf = new u8[1];  //至少有1个空间，防止返回NULL指针
        m_pMsgBuf[0] = '\0';
        m_wDBMsgBufLen = 0;
	}

    ~TDBMsgItem()
    {
        if (NULL != m_pMsgBuf)
        {
            delete[] m_pMsgBuf;
            m_pMsgBuf = NULL;
        }
    }

	void Init()
	{
		m_wMsgEventID = 0;
        m_wDBMsgBufLen = 0; //不释放m_pMsgBuf的空间        
	}

    //拷贝构造
    TDBMsgItem( const TDBMsgItem& tObj )
    {        
        m_wMsgEventID = tObj.m_wMsgEventID;		    //消息号

        // 进行深拷贝
        m_pMsgBuf = new u8[tObj.GetDBMsgBodyLen()+1];   //至少有1个空间，防止返回NULL指针
        memcpy(m_pMsgBuf, tObj.m_pMsgBuf, tObj.GetDBMsgBodyLen()+1);
        m_wDBMsgBufLen = tObj.GetDBMsgBodyLen();  //长度
	}

	void operator = ( const TDBMsgItem& tObj )
	{
        if (this == &tObj )
        {
            return;
        }
        
        if ( 0 == tObj.GetDBMsgBodyLen() )
        {
            m_wMsgEventID = tObj.m_wMsgEventID;         //消息号
            m_wDBMsgBufLen = 0;
            return;
        }

        //释放老buf
        if (NULL != m_pMsgBuf)
        {
            delete[] m_pMsgBuf;
            m_pMsgBuf = NULL;
        }

        // 进行深拷贝
        m_pMsgBuf = new u8[tObj.GetDBMsgBodyLen()+1]; //至少有1个空间，防止返回NULL指针
        memcpy(m_pMsgBuf, tObj.m_pMsgBuf, tObj.GetDBMsgBodyLen()+1);
		m_wDBMsgBufLen = tObj.GetDBMsgBodyLen();    //长度

        m_wMsgEventID = tObj.m_wMsgEventID;         //消息号
    }
	
	u16 GetEventID() const { return m_wMsgEventID; }
	void SetEventID(u16 wEventID){ m_wMsgEventID = wEventID; }

	u8* GetDBMsgBody(void) { return m_pMsgBuf; }
	u16 GetDBMsgBodyLen(void) const { return m_wDBMsgBufLen; }
	
	//3.拷贝消息
	BOOL32 SetDBMsgBody( const u8* pbyMsg, u16 wMsgLen )
	{
		if (NULL == pbyMsg || 0 == wMsgLen)
		{
            //Init();
			return TRUE;
		}

        //释放老buf
        if (NULL != m_pMsgBuf)
        {
            delete[] m_pMsgBuf;
            m_pMsgBuf = NULL;
        }
        
        //申请空间
        m_pMsgBuf = new u8[wMsgLen+1];  //至少有1个空间，防止返回NULL指针
		memcpy(m_pMsgBuf, pbyMsg, wMsgLen);
        m_pMsgBuf[wMsgLen] = '\0';
		m_wDBMsgBufLen = wMsgLen;

		return TRUE;
	}	
	
protected:
	u16 m_wMsgEventID;		//消息号
	u16 m_wDBMsgBufLen;	//消息长度
    u8*  m_pMsgBuf;//消息缓冲区
};


//18. 数据库操作线程任务状态
struct TDBOprTaskInfo
{
public:
	TDBOprTaskInfo( void )
	{
        m_hTaskId    = 0;//无效ID
		m_byTerminateTask     = 0;
		
		m_hSemTaskLock = NULL;
		OspSemBCreate(&m_hSemTaskLock);
	}

	~TDBOprTaskInfo()
	{
		if (NULL != m_hSemTaskLock)
		{
			OspSemDelete(m_hSemTaskLock);
			m_hSemTaskLock = NULL;
		}
	}

	//设置 任务ID, 主机序
    void SetTaskId( TASKHANDLE hTaskId ) { m_hTaskId = hTaskId; }
    //获取 任务ID, 主机序
    TASKHANDLE GetTaskId( void ) const { return m_hTaskId; }//主机序  	
	
	//设置 线程任务是否进行强制终止
	void  SetTerminateTask( BOOL32 bTerminate ) 
	{ 
		if (NULL != m_hSemTaskLock)
		{
			OspSemTake(m_hSemTaskLock);
			m_byTerminateTask = ((TRUE == bTerminate)?1:0); 
			OspSemGive(m_hSemTaskLock);
		}
	}

	//获取 线程任务是否进行强制终止
	BOOL32 IsTerminateTask( void ) const 
	{
		BOOL32 bRet = 0;
		if (TRUE == OspSemTakeByTime(m_hSemTaskLock, 5))
		{
			bRet = m_byTerminateTask; 
			OspSemGive(m_hSemTaskLock);
		}
		else
		{
			return FALSE;
		}
		return bRet;
	}

protected:
    TASKHANDLE m_hTaskId;
	SEMHANDLE m_hSemTaskLock; //线程信号量
 	u8  m_byTerminateTask;    //线程任务是否进行强制终止
};


//19. 数据库操作线程无空闲时等待处理的消息的消息队列(原则:FIFO)
struct TDBMsgQueue
{
public:
    TDBMsgQueue( u32 dwMaxSize )
    {
        m_dwMaxSize = dwMaxSize;
    }

	TDBMsgQueue( )
    {
        m_dwMaxSize = 1024;
    }

    ~TDBMsgQueue()
    {
        Clear();
    }
    
public:
    //将一条消息压入队列尾部，占用位置
    BOOL32 PushBackOneMsg(const TDBMsgItem &tDBMsg)
    {
		CAutoWriteLock cWriteLock(m_tLock);	// 写锁

        if (m_dwMaxSize == m_cDBMsgQueue.size())
        {
            return FALSE;
        }

        m_cDBMsgQueue.push(tDBMsg);
        return TRUE;
    }
    
    //从队列头部取出一条消息，释放位置
	BOOL32 PopFrontOneMsg( TDBMsgItem& item)
    {
		CAutoWriteLock cWriteLock(m_tLock);	// 写锁

		if (TRUE == m_cDBMsgQueue.empty())
        {
			item.Init();
            return FALSE;
        }

        item = m_cDBMsgQueue.front();
        m_cDBMsgQueue.pop();
		
		return TRUE;
    }
    
        
    //队列是否为空 TRUE-空
    BOOL32 IsEmpty() 
    {
		CAutoReadLock cReadLock(m_tLock);	// 读锁

        return m_cDBMsgQueue.empty();
    }
    
    //清空队列
    void Clear()
    {
		CAutoWriteLock cWriteLock(m_tLock);	// 写锁

        while (FALSE == m_cDBMsgQueue.empty()) 
        {
            m_cDBMsgQueue.pop();
        }
        return;
    }
    
    // 得到队列的当前有效消息数目
    u32 GetCurMsgNum()
    {
		CAutoReadLock cReadLock(m_tLock);	// 读锁

        return m_cDBMsgQueue.size();
    }
    
    // 得到队列的最大消息数目
    u32 GetQueueMaxSize() 
    {
		CAutoReadLock cReadLock(m_tLock);	// 读锁

		return m_dwMaxSize;
    }

	void SetQueueMaxSize(u32 dwMaxSize) 
	{ 
		CAutoWriteLock cWriteLock(m_tLock);	// 写锁
		m_dwMaxSize = dwMaxSize; 
	}
    
private:
    u32 m_dwMaxSize;//队列的最大消息数目, 主机序
    std::queue<TDBMsgItem> m_cDBMsgQueue;
	RWLock m_tLock;	// 操作任务队列的锁
};


#endif // _BD_MSG_QUEUE_H_
// END OF FILE
