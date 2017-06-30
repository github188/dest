

#include "osp.h"
//#include "amqp.h"
//#include "amqp_mq.h"
//#include "event.h"
//#include "rabbitmqwrapper.h"
//using namespace rabbitmqwrap;

#include "commondef.h"

#include "ssn.h"


CEqpSsnApp	g_cEqpSsnApp;


void CEqpConfig::content_cb( amqp_connection_state_t conn, void *buf, size_t len, response_type *rsp_type )
{
	char achBuf[64] = {0};
	memcpy(achBuf, buf, len);
	OspPrintf(TRUE, FALSE,"[CEqpConfig::%s]!!!!recv content:%s!!\n",__FUNCTION__,(char*)achBuf);
	
	*rsp_type = RT_ACK;
	//post to daemoninst

	//OspPost(MAKEIID(AID_MCU_PERIEQPSSN, 1), INNER_RECVFRMMQ_NTF);

	
	return;
}

void CEqpConfig::disconnect_cb( amqp_connection_state_t conn, void *buf, size_t len)
{
	printf("EqpSsn disconnect from rabbitmq-server\n");

	//add logic dealing with this situation:
	return;
}

void CEqpConfig::publisher_confirm_cb( amqp_connection_state_t conn, void *rsp_struct, response_type rsp_type )
{

	/***note:
	采用publisher confirm机制，程序需要自己记录msg.id,从1开始累加，每投递一条消息，id++. 本程序用map记录了消息id和消息对应的相关描述。 
	由于本程序起了多个线程分别初始化producer(即调add_producer接口)，每个线程对应一个rabbitmq channel，每个channel的msg.id是独立累计的。
	上面提到的msg.id即和delivery_tag对应
	*/
	
	switch ( rsp_type )
	{
	case RT_ACK:
		{
			// 执行到这里说明 message 被 RabbitMQ 所 accept 了，Producer 可以放心将当前消息移除
			amqp_basic_ack_t *basic_ack_p = (amqp_basic_ack_t *)rsp_struct;
			OspPrintf( TRUE,FALSE, "##[CEqpConfig::publisher_confirm_cb] -- [Basic.Ack]  Delivery_Tag:[%d]  multiple:[%d]\n", 
				basic_ack_p->delivery_tag, basic_ack_p->multiple );

			//operate on map
			u64 ddwMsgId = basic_ack_p->delivery_tag;
			map<u64, TMsgHead>::iterator itFound;
			itFound = g_cEqpSsnApp.m_mapPublishMsg.find(ddwMsgId);
			if( g_cEqpSsnApp.m_mapPublishMsg.end() != itFound )
			{
				//remove the msg from the map if the msg in the map
				u16 wEventid = itFound->second.wEventid;
				char achMsgDesc[64] = {0};
				memcpy(achMsgDesc, itFound->second.achEventDes, min(63, strlen(itFound->second.achEventDes)));
								
				OspPrintf(TRUE, FALSE, "##[CEqpConfig::publisher_confirm_cb] the msg.%llu [eventid:%u][%s] \
									   has been delivered to all queues that have bindings matching the message's routing key.\n", 
					ddwMsgId, wEventid, achMsgDesc);

				g_cEqpSsnApp.m_mapPublishMsg.erase(itFound);

			}
			else
			{
				//msg not found in the map, we do nothing
				OspPrintf(TRUE, FALSE, "##[CEqpConfig::publisher_confirm_cb] unexpected msg.%llu RT_ACK\n", ddwMsgId);
			}
			
			

		}
		break;
	case RT_NACK:
		{
			// 执行到这里意味着 message 由于 RabbitMQ 的内部原因丢失了，可能需要做消息重传处理
			amqp_basic_nack_t *basic_nack_p = (amqp_basic_nack_t *)rsp_struct;
			OspPrintf( TRUE,FALSE, "### CB: Publisher Confirm -- [Basic.Nack]  Delivery_Tag:[%d]  multiple:[%d]  requeue:[%d]\n", 
				basic_nack_p->delivery_tag, basic_nack_p->multiple, basic_nack_p->requeue );

			u64 ddwMsgId = basic_nack_p->delivery_tag;
			map<u64, TMsgHead>::iterator itFound;
			itFound = g_cEqpSsnApp.m_mapPublishMsg.find(ddwMsgId);
			if( g_cEqpSsnApp.m_mapPublishMsg.end() != itFound )
			{
				u16 wEventid = itFound->second.wEventid;
				char achMsgDesc[64] = {0};
				memcpy(achMsgDesc, itFound->second.achEventDes, min(63, strlen(itFound->second.achEventDes)));

				OspPrintf(TRUE,FALSE, "the msg.%llu [eventid:%u][%s] has been lost.\n", ddwMsgId,wEventid, achMsgDesc);
				
				//写到文件日志，省略
			}
			else
			{
				OspPrintf(TRUE, FALSE, "lost unexpected msg.%llu \n", ddwMsgId);
			}

		}
		break;
	case RT_REJECT:
		{
			// 执行到这里意味着 message 由于 RabbitMQ 的内部原因丢失了
			amqp_basic_reject_t *basic_reject_p = (amqp_basic_reject_t *)rsp_struct;
			OspPrintf( TRUE,FALSE, "### CB: Publisher Confirm -- [Basic.Reject]  Delivery_Tag:[%d]  requeue:[%d]\n", 
				basic_reject_p->delivery_tag, basic_reject_p->requeue );

			u64 ddwMsgId = basic_reject_p->delivery_tag;
			map<u64, TMsgHead>::iterator itFound;
			itFound = g_cEqpSsnApp.m_mapPublishMsg.find(ddwMsgId);
			if( g_cEqpSsnApp.m_mapPublishMsg.end() != itFound )
			{
				u16 wEventid = itFound->second.wEventid;
				char achMsgDesc[64] = {0};
				memcpy(achMsgDesc, itFound->second.achEventDes, min(63, strlen(itFound->second.achEventDes)));

				OspPrintf(TRUE,FALSE, "the msg.%llu [eventid:%u][%s] has been lost.\n", ddwMsgId,wEventid, achMsgDesc);

				//写到文件日志，省略
			}
			else
			{
				OspPrintf(TRUE, FALSE, "lost unexpected msg.%llu \n", ddwMsgId);
			}
		}
		break;
	default:
		OspPrintf(TRUE,FALSE,"in %s unexpected Rsp_type.%d\n",__FUNCTION__, rsp_type);
		break;
	}	
}

void CEqpConfig::initPC()
{
	
	m_nRabbitmq = CreateP(NULL, NULL, CEqpConfig::publisher_confirm_cb);
	if ( 0 == m_nRabbitmq)
	{
		//print error
	}
	
	if(! CreateC(pchC_Q, NULL, NULL, CEqpConfig::content_cb))
	{
		//print error
	}

	
}


u16 CEqpConfig::OccupyInstID(const void* content, u16 length)
{
	u16 wResId = MAXNUM_MCU_PERIEQP;
	for (u16 wIdx = 0; wIdx<MAXNUM_MCU_PERIEQP; wIdx++)
	{
		
		if( NULL == apMapInst2alias[wIdx])
		{
			if (wResId == MAXNUM_MCU_PERIEQP)
			{
				printf("[%s]find one available, resid is %d\n", __FUNCTION__,wIdx);
				wResId = wIdx;
				continue;
			}
			else
			{
				continue;
			}
			
		}
				
		if (0 == strncmp(apMapInst2alias[wIdx], (char*)content,length))
		{
			wResId = wIdx;
			break;
		}
	}

	if (MAXNUM_MCU_PERIEQP > wResId && NULL == apMapInst2alias[wResId])
	{
		//if valid Id and hasn't been occupied, we occupy it
		apMapInst2alias[wResId] = (char*)malloc(length+1);
		memset(apMapInst2alias[wResId], 0, (length+1));
		memcpy(apMapInst2alias[wResId],content, length);
		printf("Occupy success!\n");
	}

	return (wResId+1);
}

CEqpSsnInst::CEqpSsnInst()
{
	memset( m_achP_Qname,0, 64 );
}

CEqpSsnInst::~CEqpSsnInst()
{
}

void CEqpSsnInst::PublishMsg(const void* content/*=0*/, u16 length/*=0*/)
{
	
	//add map to cooperate with publisher confirm mechanism
	TMsgHead tMsgHead;
	static u64 s_ddwMsgID = 1;				//msgid
	s_ddwMsgID = s_ddwMsgID % ((u64)~0);
	//从content中提取eventid和其他关键内容赋给tMsgHead相应字段，此处省略了提取过程，直接给tMsgHead赋值
	tMsgHead.wEventid = 12000;
	snprintf(tMsgHead.achEventDes, 64,"%s\0","tell mpu mpaddr");

	//OspPrintf(TRUE,FALSE,"[%s] achEventDes is %s\n", __FUNCTION__,tMsgHead.achEventDes);
	if (g_cEqpSsnApp.m_mapPublishMsg.size() > MAXNUM_MSG)
	{
		OspPrintf(TRUE,FALSE,"too many messages haven't been routed to queues. We cann't send more messages at present! \n");
		return;
	}

	//该map严格上需要加上信号量保护，此处省略了
	g_cEqpSsnApp.m_mapPublishMsg.insert( pair<u64,TMsgHead>(s_ddwMsgID++,tMsgHead) );

	printf("in %s\n",__FUNCTION__);
	//投递消息
	PostMsg(g_cEqpSsnApp.m_nRabbitmq, (char*)content, length, m_achP_Qname);
	
}

void CEqpSsnInst::InstanceEntry( CMessage * const pcMsg )
{
	switch( pcMsg->event )
	{
	case INNER_PRE_REQ:
		{
			printf("curstate: %d\n",CurState());
			switch( CurState() )
			{
			case STATE_IDLE:
				{
					//记录P_Q:根据传进来的Alias构造Queue name
					char achAlias[MAXLEN_ALIAS+1] = {0};
					memcpy(achAlias, (char*)pcMsg->content, pcMsg->length);
					sprintf(m_achP_Qname, "CMU_%s_Q",achAlias);

					// 省略回ack

					//
					NextState(STATE_NORMAL);
					break;
				}
			default:
				{
					printf("state wrong\n");
					break;
				}
					
			}
			break;
		}
	case INNER_SNDMSG_CMD:
		{
			printf("in INNER_MEDIAOPR_SNDMSG_REQ \n");
		
			PublishMsg(pcMsg->content, pcMsg->length);
			break;
		}
	case INNER_RCVMSG_NTF:
		{
			//some process logic

			//then call PublishMsg to send msg to the peer if necessary
					
			break;
		}
	default:
		break;
	}
}

void CEqpSsnInst::DaemonInstanceEntry( CMessage * const pcMsg, CApp * pcApp )
{
	// omit state at present
	if( NULL == pcMsg )
	{
		OspPrintf( TRUE, FALSE, "EqpSsn: The received msg's pointer in the msg entry is NULL!" );
		return;
	}

	switch( pcMsg->event )
	{
	case POWERON:
		{
			g_cEqpSsnApp.initPC();
			break;
		}
	case INNER_PRE_REQ:
		{
			char *pchAlias = (char*)pcMsg->content;
			s32 nAliasLen = pcMsg->length;

			printf("begin to getInstId-alias is:%s, len is %d\n",pchAlias,nAliasLen);
			u16 wInstId = g_cEqpSsnApp.OccupyInstID(pchAlias,nAliasLen);

			printf("post to Inst.%d\n", wInstId);
			if (MAXNUM_MCU_PERIEQP < wInstId || wInstId == 0)
			{
				return;
			}
			//投给普通实例
			OspPost(MAKEIID(AID_MCU_PERIEQPSSN, wInstId), pcMsg->event,pchAlias,nAliasLen);	
			break;
		}
	}
	
	//event_base_dispatch(g_cEqpSsnApp.event_base);	// event_base loop shouldn't be here. It conflict with osp loop mechanism
}
