


#ifndef _MQ_CONSUMER_
#define _MQ_CONSUMER_

#include "kdvtype.h"
#include "mqconfig.h"


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

class CConsumerThreadParm;

//消费者
class MQWRAPPER_API CMqConsumerMsgReceiver
{
public:
    virtual ~CMqConsumerMsgReceiver()
    {
    }

    // 回调函数会在另一个线程中回调上层业务，上层业务需根据情况加锁
    virtual void MqConsumerMsgCB(void* const pMsgBody, u32 dwMsgLen) = 0;
};

class MQWRAPPER_API CMqConsumer
{
public:
    CMqConsumer();
    virtual ~CMqConsumer();

public:
    // 启动MQ客户端
    // 注意: 如果queue不存在，会自动创建queue,  但Exchange必须已经存在，不会自动创建Exchange
    // 回调函数会在另一个线程中回调上层业务，上层业务需根据情况加锁
    BOOL32 StartMq(IN const CMqConfig& tMqConfigInfo, IN CMqConsumerMsgReceiver* pMqMsgReceiver, IN BOOL32 bQueueDurable = TRUE);
    
    // 与MQ服务器的连接状态
    BOOL32 GetConnectState();

private:
    BOOL32 m_bInited;
    CConsumerThreadParm* m_pThreadParm;
};

#endif  //_MQ_CONSUMER_


