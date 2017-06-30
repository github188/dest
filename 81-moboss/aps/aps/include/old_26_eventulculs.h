
/*=======================================================================
ģ����      :uls
�ļ���      :eventulculs.h
����ļ�    :
�ļ�ʵ�ֹ���:���ն�ͨ�ŵ���Ϣ����

����        :
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/07/18  1.0                       ����
=======================================================================*/
#ifndef _EVENT_26_ULC_ULS_H_
#define _EVENT_26_ULC_ULS_H_

#include "osp.h"
#include "eventid.h"

//ULC��½������Ϣ           ����:CUlcLoginInfo
OSPEVENT( OLD_26_ULC_ULS_LOGIN_REQ,				EV_ULC_ULS_BGN + 3);    //+1,+2,+3��Ϣ����2.5.2֮ǰ�汾�����ն˼���

//ULC��½������Ϣ�ɹ���   ����:CUlcFullInfo
OSPEVENT( OLD_26_ULS_ULC_LOGIN_ACK,				EV_ULC_ULS_BGN + 4 );

//ULC��½������Ϣʧ�ܴ�   ����:(u32_ErrorCode
OSPEVENT( OLD_26_ULS_ULC_LOGIN_NACK,			EV_ULC_ULS_BGN + 5 );

#endif  //end of _EVENT_26_ULC_ULS_H_
