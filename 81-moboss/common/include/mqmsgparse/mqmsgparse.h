

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

//pengguofeng 2016-01-06 �����ߣ��������ߣ����������
#define MAX_CONSUMERNUM     3

class CMqParseReceiver
{
public:
    virtual ~CMqParseReceiver()
    {
    }

    // �ص�����������һ���߳��лص��ϲ�ҵ���ϲ�ҵ��������������
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
    //ҵ��ע���Լ���ע���¼�
    BOOL32 RegisterEvent(MqMsgCBEvent emEvent);

    //����MQ�ͻ���
    //ע��: ���queue�����ڣ����Զ�����queue,  ��Exchange�����Ѿ����ڣ������Զ�����Exchange
    //      �ص�����������һ���߳��лص��ϲ�ҵ���ϲ�ҵ��������������
	// ���������ӿ�ֻ��ʹ�����е�һ��
    BOOL32 StarParse(IN const CMqConfig& tMqConfigInfo, IN CMqParseReceiver* pMqMsgReceiver);

	//����MQ�ͻ��ˣ���ֻ��ʼ��һ��Mq�ͻ��ˣ������ڲ��������ߡ���������ǰ������������
	BOOL32 StartParse(IN const CMqCfgWithProducer* patMqInfo, IN const u8 byMqInfoNum, IN CMqParseReceiver* pMqMsgReceiver);

    //��MQ������������״̬
    BOOL32 GetConnectState();

private:
    BOOL32 m_bInited;
	u8 m_byMqConsumerNum;   // ����������
	u8 m_byMqProducerNum;   // ����������
    CMqConsumer* m_pMqConsumer[MAX_CONSUMERNUM]; // �����߶�������
	CMqProducer* m_pMqProducer[MAX_CONSUMERNUM]; // �����߶�������
    CJsonMsgParse* m_pJsonMsgParse;
	s8 m_aszPlatformMoId[OPERATION_MAXLEN_MOID+1];
	s8 m_aszPasGuid[OPERATION_MAXLEN_GUID+1];
};


#endif  //_MQ_MSG_PARSE_H_


