#include <stdio.h>
#include <string.h>

// #ifdef HAVE_SYS_TIME_H
// #include <sys/time.h>
// #else
// #include <sys/_libevent_time.h>
// #end


//#include "amqp.h"
//#include "amqp_framing.h"
//
//#include "amqp_mq.h"
//#include "event.h"



#include "osp.h"

#include "commondef.h"
#include "ssn.h"
#include "mpuapp.h"
#include "rabbitmqwrapper.h"
using namespace rabbitmqwrap;



//struct event_base *event_base;				//global base



//API

extern "C" void sndmsg2()
{
	OspPost( MAKEIID(27, 1), INNER_SNDMSG_CMD, "aa", 3 );
	OspPost( MAKEIID(27, 1), INNER_SNDMSG_CMD, "aa", 3 );
	OspPost( MAKEIID(27, 1), INNER_SNDMSG_CMD, "aa", 3 );
	OspPost( MAKEIID(27, 1), INNER_SNDMSG_CMD, "aa", 3 );
	OspPost( MAKEIID(27, 1), INNER_SNDMSG_CMD, "aa", 3 );

	OspPost( MAKEIID(27, 2), INNER_SNDMSG_CMD, "bb", 3 );
	OspPost( MAKEIID(27, 2), INNER_SNDMSG_CMD, "bb", 3 );
	OspPost( MAKEIID(27, 2), INNER_SNDMSG_CMD, "bb", 3 );
	OspPost( MAKEIID(27, 2), INNER_SNDMSG_CMD, "bb", 3 );
	OspPost( MAKEIID(27, 2), INNER_SNDMSG_CMD, "bb", 3 );

	OspPost( MAKEIID(127, 1), INNER_SNDMSG_CMD, "11", 3 );
	OspPost( MAKEIID(127, 1), INNER_SNDMSG_CMD, "11", 3 );
	OspPost( MAKEIID(127, 1), INNER_SNDMSG_CMD, "11", 3 );
	OspPost( MAKEIID(127, 1), INNER_SNDMSG_CMD, "11", 3 );
}

//send msg
extern "C" void sndmsg( u16 wAppid, char* content, u8 byInstId )
{
	if( 0 == wAppid  || NULL == content || 0 == byInstId)
	{
		OspPrintf(1, 0, "invalid argument\n");
		return;
	}
	//构造一条记录加到map中，该记录在publisher confirm 回调中作判断处理（RT_ACK则删记录）。另外如果记录达到上限MAXNUM_MSG则不允许再publisher消息
	OspPost( MAKEIID(wAppid, byInstId), INNER_SNDMSG_CMD, (void*)content, strlen(content)+1 );
}


int main(int argc, char const * const *argv)
{
	if ( argc < 2)
	{
		printf("input server ip");
		return 1;
	}
	printf("begin test!\n");
	//	init osp
#ifdef WIN32
	if ( !IsOspInitd() )
	{
		int bOspInit = OspInit(TRUE, 0, NULL );
		if ( !bOspInit )
		{
			printf( "[Mcu Start] OspInit failed!\n" );
		}
	}
	//OspRegistModule("kdv323.dll");
#else
	//if ( !IsOspInitd() )
	//{
		
		//bOspInit = OspInit( TRUE, wMcuTelnetPort );
		int bOspInit = OspInit( TRUE, 2500, NULL/*, 512, 2048 */);    //兼容新的osp库
		if ( !bOspInit )
		{
			printf( "[Mcu Start] OspInit failed!\n" );
		}
	//}
#endif	



	char *pIpAddr = const_cast<char*>(argv[1]);

	//RabbitmqClntInit("192.168.25.149");
	RabbitmqClntInit(pIpAddr);
	
#ifndef WIN32
	OspRegCommand("sndmsg",		(void*)sndmsg, "snd msg");
	OspRegCommand("sndmsg2",	(void*)sndmsg2, "snd msg2");	//just for batch test

#endif
	//create app
	g_cMpuApp.CreateApp( "Mpu", AID_MPU, 80, 1000, 2<<20 );
	OspPost(MAKEIID(AID_MPU, CInstance::DAEMON), POWERON);	

	g_cEqpSsnApp.CreateApp( "EqpSsn", AID_MCU_PERIEQPSSN, 80, 1000, 2<<20 );
	OspPost(MAKEIID(AID_MCU_PERIEQPSSN, CInstance::DAEMON), POWERON);			// 让该app初始化Producer和consumer
	
	//假设从zk获取到了一个空闲的MPU，其alias为MPU1，为同该MPU1通信，做准备工作，确定发消息给对端应该发到哪个Queue
	char achAlias[MAXLEN_ALIAS] = {0};
	snprintf(achAlias, MAXLEN_ALIAS-1,"%s", "MPU1");
	OspPost(MAKEIID(AID_MCU_PERIEQPSSN, CInstance::DAEMON), INNER_PRE_REQ,achAlias,strlen(achAlias));	

	//假设要给MPU1发开启命令
	//cServmsg的头需要有MPU1的alias信息
	//OspPost(MAKEIID(AID_MCU_PERIEQPSSN, CInstance::DAEMON), INNER_SNDMSG_CMD,cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	//假设从zk获取到了一个空闲的MPU，其alias为MPU2，为同该MPU1通信，做准备工作，确定发消息给对端应该发到哪个Queue
	snprintf(achAlias, MAXLEN_ALIAS-1,"%s", "MPU2");
	OspPost(MAKEIID(AID_MCU_PERIEQPSSN, CInstance::DAEMON), INNER_PRE_REQ,achAlias,strlen(achAlias));

	
	/*
	//////////////////////////////////////////////////
	int		flag = -1;

	MQ msgQ;
	mq_init( &msgQ );

 	//struct event_base *event_base;
 	//event_base = event_init();
 	//event_base = event_base_new();

	//default exchange(direct)
	char *pchX			= NULL;
	char *pchBindingKey = NULL;
	char *pchConsumertag ="xltest";

 	//flag = add_producer( "192.168.25.138",PORT,"/","guest","guest",
 	//	NULL,NULL,EMPTY, &msgQ,
 	//	CT_JSON, 0, 0,
 	//	connection_timeout_cb, connection_disc_cb,NULL);
 
 	flag = add_consumer( "192.168.25.138", PORT, "/", "guest","guest",
 		"test_queue", NULL, NULL, Q_DECLARE|Q_BIND|B_QOS|B_CONSUME, &msgQ,
 		0, 1, pchConsumertag,
 		connection_timeout_cb, connection_disc_cb, content_body_cb );


	// 	flag = add_producer( "172.16.81.111", PORT, VHOST, USERNAME, PASSWORD, 
	// 		"amq.direct", "test", C_CONFIRM_SELECT, CT_JSON, 0, 0,
	// 		connection_timeout_cb, connection_disc_cb, publisher_confirm_cb );
	

	//send msg
	//MQ_ITEM mq_item;
	//char *pchContent = (char*)malloc(16);
	//memset(pchContent,0,sizeof(*pchContent));
	//sprintf(pchContent,"imusingrabbit");
	//mq_item.content_arg = pchContent;
	//mq_item.content_len = strlen("imusingrabbit");

	//mq_item.exchange_arg = NULL;
	//mq_item.exchange_len = 0;

	//char *pchRkey = (char*)malloc(16);
	//memset(pchRkey,0,sizeof(*pchRkey));
	//sprintf(pchRkey, "test_queue");
	//mq_item.routingkey_arg = pchRkey;
	//mq_item.routingkey_len = strlen("test_queue");
	//mq_push(&msgQ,&mq_item);


	//
	// 	flag = add_consumer( "172.16.217.217", PORT, VHOST, USERNAME, PASSWORD,
	// 		"test_queue", "amq.direct", "test", Q_DECLARE|Q_BIND|B_QOS|B_CONSUME, 0, 1, "Betty",
	// 		connection_timeout_cb, connection_disc_cb, content_body_cb );

	// 	flag = add_manager( "172.16.81.111", PORT, VHOST, USERNAME, PASSWORD, "test_queue", NULL, NULL, Q_PURGE );

	*/
	/*
	//如果执行event_base_loop时，发现还没有注册的事件，则会退出loop
	if ( event_base_loop( event_base, 0 ) != 0 ) 
	{
	fprintf( stderr, "main: event_base_loop exit abnormal!\n" );
	}
	*/


	// start event loop thread
	//OspTaskCreate(eventloop,"eventloop",60, 1<<20, (u32)event_base, 0);
	
	while(1);
	return 0;
}
