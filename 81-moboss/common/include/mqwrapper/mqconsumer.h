


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

//������
class MQWRAPPER_API CMqConsumerMsgReceiver
{
public:
    virtual ~CMqConsumerMsgReceiver()
    {
    }

    // �ص�����������һ���߳��лص��ϲ�ҵ���ϲ�ҵ��������������
    virtual void MqConsumerMsgCB(void* const pMsgBody, u32 dwMsgLen) = 0;
};

class MQWRAPPER_API CMqConsumer
{
public:
    CMqConsumer();
    virtual ~CMqConsumer();

public:
    // ����MQ�ͻ���
    // ע��: ���queue�����ڣ����Զ�����queue,  ��Exchange�����Ѿ����ڣ������Զ�����Exchange
    // �ص�����������һ���߳��лص��ϲ�ҵ���ϲ�ҵ��������������
    BOOL32 StartMq(IN const CMqConfig& tMqConfigInfo, IN CMqConsumerMsgReceiver* pMqMsgReceiver, IN BOOL32 bQueueDurable = TRUE);
    
    // ��MQ������������״̬
    BOOL32 GetConnectState();

private:
    BOOL32 m_bInited;
    CConsumerThreadParm* m_pThreadParm;
};

#endif  //_MQ_CONSUMER_


