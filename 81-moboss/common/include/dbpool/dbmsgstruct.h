
#ifndef _BD_MSG_QUEUE_H_
#define _BD_MSG_QUEUE_H_

#pragma warning (disable:4786 )

#include "osp.h"
#include "rwlock_api.h"
#include <queue>

//17. ���ݿ⴦�����Ϣ��Ԫ
struct TDBMsgItem
{
public:
    TDBMsgItem( void )
	{
        m_wMsgEventID = 0;
        m_pMsgBuf = new u8[1];  //������1���ռ䣬��ֹ����NULLָ��
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
        m_wDBMsgBufLen = 0; //���ͷ�m_pMsgBuf�Ŀռ�        
	}

    //��������
    TDBMsgItem( const TDBMsgItem& tObj )
    {        
        m_wMsgEventID = tObj.m_wMsgEventID;		    //��Ϣ��

        // �������
        m_pMsgBuf = new u8[tObj.GetDBMsgBodyLen()+1];   //������1���ռ䣬��ֹ����NULLָ��
        memcpy(m_pMsgBuf, tObj.m_pMsgBuf, tObj.GetDBMsgBodyLen()+1);
        m_wDBMsgBufLen = tObj.GetDBMsgBodyLen();  //����
	}

	void operator = ( const TDBMsgItem& tObj )
	{
        if (this == &tObj )
        {
            return;
        }
        
        if ( 0 == tObj.GetDBMsgBodyLen() )
        {
            m_wMsgEventID = tObj.m_wMsgEventID;         //��Ϣ��
            m_wDBMsgBufLen = 0;
            return;
        }

        //�ͷ���buf
        if (NULL != m_pMsgBuf)
        {
            delete[] m_pMsgBuf;
            m_pMsgBuf = NULL;
        }

        // �������
        m_pMsgBuf = new u8[tObj.GetDBMsgBodyLen()+1]; //������1���ռ䣬��ֹ����NULLָ��
        memcpy(m_pMsgBuf, tObj.m_pMsgBuf, tObj.GetDBMsgBodyLen()+1);
		m_wDBMsgBufLen = tObj.GetDBMsgBodyLen();    //����

        m_wMsgEventID = tObj.m_wMsgEventID;         //��Ϣ��
    }
	
	u16 GetEventID() const { return m_wMsgEventID; }
	void SetEventID(u16 wEventID){ m_wMsgEventID = wEventID; }

	u8* GetDBMsgBody(void) { return m_pMsgBuf; }
	u16 GetDBMsgBodyLen(void) const { return m_wDBMsgBufLen; }
	
	//3.������Ϣ
	BOOL32 SetDBMsgBody( const u8* pbyMsg, u16 wMsgLen )
	{
		if (NULL == pbyMsg || 0 == wMsgLen)
		{
            //Init();
			return TRUE;
		}

        //�ͷ���buf
        if (NULL != m_pMsgBuf)
        {
            delete[] m_pMsgBuf;
            m_pMsgBuf = NULL;
        }
        
        //����ռ�
        m_pMsgBuf = new u8[wMsgLen+1];  //������1���ռ䣬��ֹ����NULLָ��
		memcpy(m_pMsgBuf, pbyMsg, wMsgLen);
        m_pMsgBuf[wMsgLen] = '\0';
		m_wDBMsgBufLen = wMsgLen;

		return TRUE;
	}	
	
protected:
	u16 m_wMsgEventID;		//��Ϣ��
	u16 m_wDBMsgBufLen;	//��Ϣ����
    u8*  m_pMsgBuf;//��Ϣ������
};


//18. ���ݿ�����߳�����״̬
struct TDBOprTaskInfo
{
public:
	TDBOprTaskInfo( void )
	{
        m_hTaskId    = 0;//��ЧID
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

	//���� ����ID, ������
    void SetTaskId( TASKHANDLE hTaskId ) { m_hTaskId = hTaskId; }
    //��ȡ ����ID, ������
    TASKHANDLE GetTaskId( void ) const { return m_hTaskId; }//������  	
	
	//���� �߳������Ƿ����ǿ����ֹ
	void  SetTerminateTask( BOOL32 bTerminate ) 
	{ 
		if (NULL != m_hSemTaskLock)
		{
			OspSemTake(m_hSemTaskLock);
			m_byTerminateTask = ((TRUE == bTerminate)?1:0); 
			OspSemGive(m_hSemTaskLock);
		}
	}

	//��ȡ �߳������Ƿ����ǿ����ֹ
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
	SEMHANDLE m_hSemTaskLock; //�߳��ź���
 	u8  m_byTerminateTask;    //�߳������Ƿ����ǿ����ֹ
};


//19. ���ݿ�����߳��޿���ʱ�ȴ��������Ϣ����Ϣ����(ԭ��:FIFO)
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
    //��һ����Ϣѹ�����β����ռ��λ��
    BOOL32 PushBackOneMsg(const TDBMsgItem &tDBMsg)
    {
		CAutoWriteLock cWriteLock(m_tLock);	// д��

        if (m_dwMaxSize == m_cDBMsgQueue.size())
        {
            return FALSE;
        }

        m_cDBMsgQueue.push(tDBMsg);
        return TRUE;
    }
    
    //�Ӷ���ͷ��ȡ��һ����Ϣ���ͷ�λ��
	BOOL32 PopFrontOneMsg( TDBMsgItem& item)
    {
		CAutoWriteLock cWriteLock(m_tLock);	// д��

		if (TRUE == m_cDBMsgQueue.empty())
        {
			item.Init();
            return FALSE;
        }

        item = m_cDBMsgQueue.front();
        m_cDBMsgQueue.pop();
		
		return TRUE;
    }
    
        
    //�����Ƿ�Ϊ�� TRUE-��
    BOOL32 IsEmpty() 
    {
		CAutoReadLock cReadLock(m_tLock);	// ����

        return m_cDBMsgQueue.empty();
    }
    
    //��ն���
    void Clear()
    {
		CAutoWriteLock cWriteLock(m_tLock);	// д��

        while (FALSE == m_cDBMsgQueue.empty()) 
        {
            m_cDBMsgQueue.pop();
        }
        return;
    }
    
    // �õ����еĵ�ǰ��Ч��Ϣ��Ŀ
    u32 GetCurMsgNum()
    {
		CAutoReadLock cReadLock(m_tLock);	// ����

        return m_cDBMsgQueue.size();
    }
    
    // �õ����е������Ϣ��Ŀ
    u32 GetQueueMaxSize() 
    {
		CAutoReadLock cReadLock(m_tLock);	// ����

		return m_dwMaxSize;
    }

	void SetQueueMaxSize(u32 dwMaxSize) 
	{ 
		CAutoWriteLock cWriteLock(m_tLock);	// д��
		m_dwMaxSize = dwMaxSize; 
	}
    
private:
    u32 m_dwMaxSize;//���е������Ϣ��Ŀ, ������
    std::queue<TDBMsgItem> m_cDBMsgQueue;
	RWLock m_tLock;	// ����������е���
};


#endif // _BD_MSG_QUEUE_H_
// END OF FILE
