/*
sumulate mpu thread dealing with mcu thread
*/
#ifndef _MPU_H
#define _MPU_H

#include "rabbitmqwrapper.h"
using namespace rabbitmqwrap;

class CMpuInst : public CInstance  
{
public:
	enum 
	{ 
		STATE_IDLE,
		STATE_NORMAL
	};

public:
	char    m_achP_Qname[64];		// P_C_Queue name


public:
	void DaemonInstanceEntry( CMessage * const pcMsg, CApp* pcApp );
	void InstanceEntry( CMessage * const pcMsg );
	void PublishMsg(const void* content=0, u16 length=0);

	CMpuInst();
	virtual ~CMpuInst();
};


class CMpuConfig
{
public:
	char		*pchC_Q;
	//TSndMsgQ	Servmsg_mq;
	int			m_nRabbitmq;								// rabbitmq
public:
	//�ص�����������Ϊ��ľ�̬����
	static	void content_cb( amqp_connection_state_t conn, void *buf, size_t len, response_type *rsp_type );
	static	void publisher_confirm_cb( amqp_connection_state_t conn, void *rsp_struct, response_type rsp_type );
	void initPC();


	CMpuConfig()
	{
		//ȷ��Consumer��Queue
		pchC_Q = new char(64);
		memset(pchC_Q,0,64);
		sprintf(pchC_Q,"%s\0","CMU_MPU1_Q");

		m_nRabbitmq = 0;
		//ҵ����Ϣ����
		//mq_init( &Servmsg_mq );

		//don't call add_consumer and add_producer at construct function, since event_base hasn't been initialized.

	}

	virtual ~CMpuConfig()
	{
		
	}
};

typedef zTemplate< CMpuInst, 1, CMpuConfig, sizeof( unsigned char ) > CMpuApp;

extern CMpuApp	g_cMpuApp;	


#endif
