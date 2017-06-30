/*
sumulate mcu thread dealing with eqp thread
*/
#ifndef _MCU_EQPSSN_H
#define _MCU_EQPSSN_H


#include <map>
using namespace std;

#include "rabbitmqwrapper.h"
using namespace rabbitmqwrap;

// function macro
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

class CEqpSsnInst : public CInstance  
{
public:
	enum 
	{ 
		STATE_IDLE,
		STATE_NORMAL
	};

public:
	
	char    m_achP_Qname[64];		// P_C_Queue name
	
	//ʡ����������Ա

public:
	void DaemonInstanceEntry( CMessage * const pcMsg, CApp* pcApp );
	void InstanceEntry( CMessage * const pcMsg );
	void PublishMsg(const void* content=0, u16 length=0);



	CEqpSsnInst();
	virtual ~CEqpSsnInst();
};


class CEqpConfig
{
public:
	char		*pchC_Q;								//������Ϣ��Queue name
	//TSndMsgQ	Servmsg_mq;								//������Ϣ�Ķ���
	int			m_nRabbitmq;								// rabbitmq
	char		*apMapInst2alias[MAXNUM_MCU_PERIEQP];	//�����ǳƺ���ͨʵ����ӳ���

 	map<u64, TMsgHead> m_mapPublishMsg;				//��¼������Ϣ��map�����publisher confirm�����ã�

public:
	//�ص�����������Ϊ��ľ�̬����
	static	void content_cb( amqp_connection_state_t conn, void *buf, size_t len, response_type *rsp_type );
	static	void disconnect_cb( amqp_connection_state_t conn, void *buf, size_t len);
	static	void publisher_confirm_cb( amqp_connection_state_t conn, void *rsp_struct, response_type rsp_type );
	
	void	initPC();
	u16		OccupyInstID(const void* content=0, u16 length=0);

	CEqpConfig()
	{
		memset(apMapInst2alias, 0, sizeof(apMapInst2alias));

		//ȷ��Consumer��Queue
		pchC_Q = new char(64);
		memset(pchC_Q,0,64);
		sprintf(pchC_Q,"%s\0","MPU_CMU1_Q");

		m_nRabbitmq = 0;
		//ҵ����Ϣ����
		//mq_init( &Servmsg_mq );

		//don't call add_consumer and add_producer at construct, since event_base hasn't been initialized.

	}

	virtual ~CEqpConfig()
	{
		
		
	}
};

typedef zTemplate< CEqpSsnInst, MAXNUM_MCU_PERIEQP, CEqpConfig, sizeof( unsigned char ) > CEqpSsnApp;

extern CEqpSsnApp	g_cEqpSsnApp;	//����ỰӦ��ʵ��


#endif
