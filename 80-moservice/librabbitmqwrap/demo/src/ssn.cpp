

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
	����publisher confirm���ƣ�������Ҫ�Լ���¼msg.id,��1��ʼ�ۼӣ�ÿͶ��һ����Ϣ��id++. ��������map��¼����Ϣid����Ϣ��Ӧ����������� 
	���ڱ��������˶���̷ֱ߳��ʼ��producer(����add_producer�ӿ�)��ÿ���̶߳�Ӧһ��rabbitmq channel��ÿ��channel��msg.id�Ƕ����ۼƵġ�
	�����ᵽ��msg.id����delivery_tag��Ӧ
	*/
	
	switch ( rsp_type )
	{
	case RT_ACK:
		{
			// ִ�е�����˵�� message �� RabbitMQ �� accept �ˣ�Producer ���Է��Ľ���ǰ��Ϣ�Ƴ�
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
			// ִ�е�������ζ�� message ���� RabbitMQ ���ڲ�ԭ��ʧ�ˣ�������Ҫ����Ϣ�ش�����
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
				
				//д���ļ���־��ʡ��
			}
			else
			{
				OspPrintf(TRUE, FALSE, "lost unexpected msg.%llu \n", ddwMsgId);
			}

		}
		break;
	case RT_REJECT:
		{
			// ִ�е�������ζ�� message ���� RabbitMQ ���ڲ�ԭ��ʧ��
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

				//д���ļ���־��ʡ��
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
	//��content����ȡeventid�������ؼ����ݸ���tMsgHead��Ӧ�ֶΣ��˴�ʡ������ȡ���̣�ֱ�Ӹ�tMsgHead��ֵ
	tMsgHead.wEventid = 12000;
	snprintf(tMsgHead.achEventDes, 64,"%s\0","tell mpu mpaddr");

	//OspPrintf(TRUE,FALSE,"[%s] achEventDes is %s\n", __FUNCTION__,tMsgHead.achEventDes);
	if (g_cEqpSsnApp.m_mapPublishMsg.size() > MAXNUM_MSG)
	{
		OspPrintf(TRUE,FALSE,"too many messages haven't been routed to queues. We cann't send more messages at present! \n");
		return;
	}

	//��map�ϸ�����Ҫ�����ź����������˴�ʡ����
	g_cEqpSsnApp.m_mapPublishMsg.insert( pair<u64,TMsgHead>(s_ddwMsgID++,tMsgHead) );

	printf("in %s\n",__FUNCTION__);
	//Ͷ����Ϣ
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
					//��¼P_Q:���ݴ�������Alias����Queue name
					char achAlias[MAXLEN_ALIAS+1] = {0};
					memcpy(achAlias, (char*)pcMsg->content, pcMsg->length);
					sprintf(m_achP_Qname, "CMU_%s_Q",achAlias);

					// ʡ�Ի�ack

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
			//Ͷ����ͨʵ��
			OspPost(MAKEIID(AID_MCU_PERIEQPSSN, wInstId), pcMsg->event,pchAlias,nAliasLen);	
			break;
		}
	}
	
	//event_base_dispatch(g_cEqpSsnApp.event_base);	// event_base loop shouldn't be here. It conflict with osp loop mechanism
}
