
#ifndef _MQ_PRODUCER_
#define _MQ_PRODUCER_

#include "kdvtype.h"
#include "mqconfig.h"
#ifdef _LINUX_
#include <pthread.h>
#endif


#ifdef  MQWRAPPER_EXPORTS
#define MQWRAPPER_API __declspec( dllexport )
#else
#define MQWRAPPER_API
#endif

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif





class MQWRAPPER_API CMqMsgPostResultReceiver
{
public:
    virtual ~CMqMsgPostResultReceiver()
    {
    }

    // 回调函数会在另一个线程中回调上层业务，上层业务需根据情况加锁
    virtual void MsgPostResultCB(u64 dwMsgSn, BOOL32 bResult,  const s8* szExchangeName, const s8* szRoutingKey, void* const pMsgBody, u32 dwMsgLen) = 0;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//                  生产者(异步发送)
///////////////////////////////////////////////////////////////////////////////////////////////////

class CProducerThreadParm;
class MQWRAPPER_API CMqProducer 
{
public:
    CMqProducer();
    virtual ~CMqProducer();

public:
    //启动MQ客户端
    //注意:不会自动创建Exchange
    //CMqConfig中的ExchangeName与QueueName无意义
    //如果不关心发送结果，可以不设置CMqConfirmMsgReceiver
    // 回调函数会在另一个线程中回调上层业务，上层业务需根据情况加锁
    BOOL32 StartMq(IN const CMqConfig& tMqConfigInfo, IN CMqMsgPostResultReceiver* pMqMsgReceiver = NULL);

    //与MQ服务器的连接状态
    BOOL32 GetConnectState();

    //异步发送  返回消息SN号，为0表示失败， 非0只表示将消息存入了等待队列，并不代表已经发到服务器，最终结果会通过Callback通知
    u64 PostMsg(IN const s8* szExchangeName, IN const s8* szRoutingKey, IN const void* pMsgBody, IN u32 dwMsgLen, IN BOOL32 bDurable = TRUE, IN BOOL32 bRepeatPost = FALSE);

	const s8* GetExchangeName() const
	{
		return m_szExchangeName;
	}

	const s8* GetRoutingKey() const
	{
		return m_szRoutingkey;
	}

	const s8* GetPostBuf() const
	{
		return m_szPostBuf;
	}

	u32 GetPostBufLen() const
	{
		return strlen(m_szPostBuf);
	}

	void SetPostBuf(s8 * szPostBuf, u32 dwBufLen)
	{
		if ( !szPostBuf )
		{
			memset(m_szPostBuf, 0, sizeof(m_szPostBuf));
			return;
		}
		if ( dwBufLen > sizeof(m_szPostBuf) -1 )
		{
			dwBufLen = sizeof(m_szPostBuf) -1;
		}
		memcpy(m_szPostBuf, szPostBuf, dwBufLen);
		m_szPostBuf[dwBufLen] = 0;
	}
	BOOL32 IsRepeatPost() const
	{
		return m_bRepeatPost;
	}
private:
	void  SetExchangeName(const s8 *pszExChangeName)
	{
        if (NULL == pszExChangeName)
        {
            memset(m_szExchangeName, 0, sizeof(m_szExchangeName));
            return;
        }
		
		u32 dwLen = strlen(pszExChangeName);
		if ( dwLen > sizeof(m_szExchangeName)-1)
		{
			dwLen = sizeof(m_szExchangeName)-1;
		}
        strncpy(m_szExchangeName, pszExChangeName, dwLen);
		return;
	}
	void  SetRoutingKey(const s8 *pszRoutingKey)
	{
        if (NULL == pszRoutingKey)
        {
            memset(m_szRoutingkey, 0, sizeof(m_szRoutingkey));
            return;
        }
		
		u32 dwLen = strlen(pszRoutingKey);
		if ( dwLen > sizeof(m_szRoutingkey)-1)
		{
			dwLen = sizeof(m_szRoutingkey)-1;
		}
        strncpy(m_szRoutingkey, pszRoutingKey, dwLen);
        return;
	}

private:
    BOOL32 m_bInited;
	BOOL32 m_bRepeatPost;   //是否重复发送请求
    CProducerThreadParm* m_pThreadParm;
    s8 m_szExchangeName[MQWRAPPER_MAXLEN_EXCHANGE_NAME+1];  //Exchange名    
    s8 m_szRoutingkey[MQWRAPPER_MAXLEN_ROUTINGKEY+1];       //Routingkey(消费者并且队列需要Bind时才有用)
	s8 m_szPostBuf[2048];  // 发送Buf，暂定2048，因为只有License.ex会用
};





///////////////////////////////////////////////////////////////////////////////////////////////////
//                  生产者(同步发送)
///////////////////////////////////////////////////////////////////////////////////////////////////

class MQWRAPPER_API CMqProducerSync : public CMqProducer, CMqMsgPostResultReceiver
{
public:
    CMqProducerSync();
    virtual ~CMqProducerSync();

public:
    //启动MQ客户端
    //注意:不会自动创建Exchange
    //CMqConfig中的ExchangeName与QueueName无意义
    BOOL32 StartMq(IN const CMqConfig& tMqConfigInfo);

    //同步发送  返回消息SN号，为0表示失败
    BOOL32 SendMsg(IN const s8* szExchangeName, IN const s8* szRoutingKey, IN const void* pMsgBody, IN u32 dwMsgLen, IN BOOL32 bDurable = TRUE);

private:
    BOOL32 WaitRspSem();
    BOOL32 GiveRspSem();
    void SetSendResult(BOOL32 bResult) { m_bSendResult = bResult; }

private:
    virtual void MsgPostResultCB(u64 dwMsgSn, BOOL32 bResult,  const s8* szExchangeName, const s8* szRoutingKey, void* const pMsgBody, u32 dwMsgLen);

private:
#ifdef _LINUX_
    pthread_cond_t* m_tSyncSem;
#else
    void* m_tSyncSem;
#endif

    BOOL32 m_bSendResult;
};


#endif  //_MQ_PRODUCER_


