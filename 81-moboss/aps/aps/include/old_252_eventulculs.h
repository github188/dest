
/*****************************************************************************
ģ����      : UserLoginServer
�ļ���      : old_evulculs.h
����ʱ��    : 
ʵ�ֹ���    : 
����        : 
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����

******************************************************************************/
#ifndef _OLD_252_EV_ULC_ULS_H_
#define _OLD_252_EV_ULC_ULS_H_

#include "osp.h"
#include "eventid.h"

//2.5.2�汾��֮ǰ�汾����Ϣ����(��������Ҫ)
//ULC��½������Ϣ
OSPEVENT( OLD_2_5_2_ULC_ULS_LOGIN_REQ,				EV_ULC_ULS_BGN + 0);

//ULC��½������Ϣ�ɹ���
OSPEVENT( OLD_2_5_2_ULS_ULC_LOGIN_ACK,				EV_ULC_ULS_BGN + 1);

//ULC��½������Ϣʧ�ܴ�
OSPEVENT( OLD_2_5_2_ULS_ULC_LOGIN_NACK,				EV_ULC_ULS_BGN + 2);


#endif  //end of _OLD_252_EV_ULC_ULS_H_
