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
	
	//省略了其他成员

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
	char		*pchC_Q;								//接收消息的Queue name
	//TSndMsgQ	Servmsg_mq;								//发送消息的队列
	int			m_nRabbitmq;								// rabbitmq
	char		*apMapInst2alias[MAXNUM_MCU_PERIEQP];	//外设昵称和普通实例号映射表

 	map<u64, TMsgHead> m_mapPublishMsg;				//记录发送消息的map（配合publisher confirm机制用）

public:
	//回调函数需声明为类的静态函数
	static	void content_cb( amqp_connection_state_t conn, void *buf, size_t len, response_type *rsp_type );
	static	void disconnect_cb( amqp_connection_state_t conn, void *buf, size_t len);
	static	void publisher_confirm_cb( amqp_connection_state_t conn, void *rsp_struct, response_type rsp_type );
	
	void	initPC();
	u16		OccupyInstID(const void* content=0, u16 length=0);

	CEqpConfig()
	{
		memset(apMapInst2alias, 0, sizeof(apMapInst2alias));

		//确定Consumer的Queue
		pchC_Q = new char(64);
		memset(pchC_Q,0,64);
		sprintf(pchC_Q,"%s\0","MPU_CMU1_Q");

		m_nRabbitmq = 0;
		//业务消息队列
		//mq_init( &Servmsg_mq );

		//don't call add_consumer and add_producer at construct, since event_base hasn't been initialized.

	}

	virtual ~CEqpConfig()
	{
		
		
	}
};

typedef zTemplate< CEqpSsnInst, MAXNUM_MCU_PERIEQP, CEqpConfig, sizeof( unsigned char ) > CEqpSsnApp;

extern CEqpSsnApp	g_cEqpSsnApp;	//外设会话应用实例


#endif
