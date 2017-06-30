

#ifndef _MQ_MSG_PARSE_H_
#define _MQ_MSG_PARSE_H_

#ifdef  MQMSGPARSE_EXPORTS
#define MQMSGPARSE_API __declspec( dllexport )
#else
#define MQMSGPARSE_API
#endif

#include "kdvtype.h"
#include "structmqmsgparse.h"
#include "../mqwrapper/mqconfig.h"
#include "operationconst.h"

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

//pengguofeng 2016-01-06 消费者（或生产者）的最大数量
#define MAX_CONSUMERNUM     3

class CMqParseReceiver
{
public:
    virtual ~CMqParseReceiver()
    {
    }

    // 回调函数会在另一个线程中回调上层业务，上层业务需根据情况加锁
    virtual void MqMsgParseCB( MqMsgCBEvent emEventID, void* pMsgBody, u32 wMsgLen) = 0;
};


class CMqConsumer;
class CMqProducer;
class CJsonMsgParse;
class MQMSGPARSE_API CMqMsgParse
{
public:
    CMqMsgParse();
    ~CMqMsgParse();

public:
    //业务注册自己关注的事件
    BOOL32 RegisterEvent(MqMsgCBEvent emEvent);

    //启动MQ客户端
    //注意: 如果queue不存在，会自动创建queue,  但Exchange必须已经存在，不会自动创建Exchange
    //      回调函数会在另一个线程中回调上层业务，上层业务需根据情况加锁
	// 以下两个接口只能使用其中的一个
    BOOL32 StarParse(IN const CMqConfig& tMqConfigInfo, IN CMqParseReceiver* pMqMsgReceiver);

	//启动MQ客户端２：只初始化一个Mq客户端，但是内部的生产者、消费者由前两个参数控制
	BOOL32 StartParse(IN const CMqCfgWithProducer* patMqInfo, IN const u8 byMqInfoNum, IN CMqParseReceiver* pMqMsgReceiver);

    //与MQ服务器的连接状态
    BOOL32 GetConnectState();

private:
    BOOL32 m_bInited;
	u8 m_byMqConsumerNum;   // 消费者数量
	u8 m_byMqProducerNum;   // 生产者数量
    CMqConsumer* m_pMqConsumer[MAX_CONSUMERNUM]; // 消费者对象链表
	CMqProducer* m_pMqProducer[MAX_CONSUMERNUM]; // 生产者对象链表
    CJsonMsgParse* m_pJsonMsgParse;
	s8 m_aszPlatformMoId[OPERATION_MAXLEN_MOID+1];
	s8 m_aszPasGuid[OPERATION_MAXLEN_GUID+1];
};


#endif  //_MQ_MSG_PARSE_H_


