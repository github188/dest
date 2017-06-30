#include "osp.h"
//#include "amqp.h"
//#include "amqp_mq.h"
//#include "event.h"
//#include "rabbitmqwrapper.h"
//using namespace rabbitmqwrap;
#include "commondef.h"
#include "mpuapp.h"

CMpuApp	g_cMpuApp;


void CMpuConfig::content_cb( amqp_connection_state_t conn, void *buf, size_t len, response_type *rsp_type )
{
	char achBuf[64] = {0};
	memcpy(achBuf, buf, len);
	OspPrintf(TRUE,FALSE,"[CMpuConfig][%s]!!!!recv content:%s, len: %d!!\n",__FUNCTION__,(char*)achBuf, len);
	printf("[%s]recv content%s!!\n",__FUNCTION__,(char*)achBuf);
	
	//if content isn't absurd, we send RT_ACK otherwise we send RT_REJECT
	*rsp_type = RT_ACK;
	
	//now we post to inst to further process

	OspPost(MAKEIID(AID_MPU, 1), INNER_RCVMSG_NTF, achBuf,strlen(achBuf));
	return;
}

void CMpuConfig::publisher_confirm_cb( amqp_connection_state_t conn, void *rsp_struct, response_type rsp_type )
{
	OspPrintf(TRUE,FALSE,"in %s\n",__FUNCTION__);
	switch ( rsp_type )
	{
	case RT_ACK:
		{
			// 执行到这里说明 message 被 RabbitMQ 所 accept 了，Producer 可以放心将当前消息移除
			amqp_basic_ack_t *basic_ack_p = (amqp_basic_ack_t *)rsp_struct;
			OspPrintf( TRUE,FALSE, "### CB: Publisher Confirm -- [Basic.Ack]  Delivery_Tag:[%d]  multiple:[%d]\n", 
				basic_ack_p->delivery_tag, basic_ack_p->multiple );
		}
		break;
	case RT_NACK:
		{
			// 执行到这里意味着 message 由于 RabbitMQ 的内部原因丢失了，可能需要做消息重传处理
			amqp_basic_nack_t *basic_nack_p = (amqp_basic_nack_t *)rsp_struct;
			OspPrintf( TRUE,FALSE, "### CB: Publisher Confirm -- [Basic.Nack]  Delivery_Tag:[%d]  multiple:[%d]  requeue:[%d]\n", 
				basic_nack_p->delivery_tag, basic_nack_p->multiple, basic_nack_p->requeue );
		}
		break;
	case RT_REJECT:
		{
			// 执行到这里意味着 message 由于 RabbitMQ 的内部原因丢失了，可能需要做消息重传处理
			amqp_basic_reject_t *basic_reject_p = (amqp_basic_reject_t *)rsp_struct;
			OspPrintf( TRUE,FALSE, "### CB: Publisher Confirm -- [Basic.Reject]  Delivery_Tag:[%d]  requeue:[%d]\n", 
				basic_reject_p->delivery_tag, basic_reject_p->requeue );
		}
		break;
	default:
		OspPrintf(TRUE,FALSE,"in %s unexpected Rsp_type.%d\n",__FUNCTION__, rsp_type);
		break;
	}	
}

void CMpuConfig::initPC()
{
	
	m_nRabbitmq = CreateP(NULL, NULL, CMpuConfig::publisher_confirm_cb);
	if ( 0 == m_nRabbitmq )
	{
		printf("[CMpuConfig]CreateP error！\n");
	}

	CreateC(pchC_Q, NULL, NULL, CMpuConfig::content_cb);


}

CMpuInst::CMpuInst()
{
	memset( m_achP_Qname,0, 64 );
}

CMpuInst::~CMpuInst()
{
}

void CMpuInst::PublishMsg(const void* content/*=0*/, u16 length/*=0*/)
{
	//PostMsg2Q(&g_cMpuApp.Servmsg_mq, m_achP_Qname, strlen(m_achP_Qname), content, length);
	PostMsg(g_cMpuApp.m_nRabbitmq, (char*)content, length, m_achP_Qname);
	
}

void CMpuInst::InstanceEntry( CMessage * const pcMsg )
{
	switch( pcMsg->event )
	{
	case POWERON:
		{
			//记录P_Q
			//MPU1可以是由主控传进来的，这里省略了该步骤。
			sprintf(m_achP_Qname, "MPU_CMU1_Q");

			// 省略回ack

			break;
		}
	case INNER_SNDMSG_CMD:
		{
			OspPrintf(TRUE,FALSE,"in INNER_SNDMSG_REQ \n");

			PublishMsg(pcMsg->content, pcMsg->length);
					
			break;
		}
	case INNER_RCVMSG_NTF:
		{
			//to simplify the process, we just send rsp(ack) here
			OspPrintf(TRUE, FALSE,"process INNER_RECVFRMMQ_NTF\n");
			s8 achBuf[64]= {0};
			s32 nLen = sprintf(achBuf, "%s","mpu_mcu_x_ack");
			PublishMsg(achBuf, nLen);

			//OspPrintf(TRUE, FALSE, "achBuf is %s, buflen is %d\n", achBuf, strlen(achBuf));
			//OspPost(MAKEIID(GetAppID(), 1), INNER_SNDMSG_REQ, achBuf, strlen(achBuf));
			
			break;
		}
	}
}

void CMpuInst::DaemonInstanceEntry( CMessage * const pcMsg, CApp * pcApp )
{

	if( NULL == pcMsg )
	{
		OspPrintf( TRUE, FALSE, "EqpSsn: The received msg's pointer in the msg entry is NULL!" );
		return;
	}

	switch( pcMsg->event )
	{
	case POWERON:
		{
			g_cMpuApp.initPC();
			//投给普通实例，不妨为1
			OspPost(MAKEIID(AID_MPU, 1), pcMsg->event);	
			break;
		}
	}

	//event_base_dispatch(g_cEqpSsnApp.event_base);	// event_base loop shouldn't be here. It conflict with osp loop mechanism
}
