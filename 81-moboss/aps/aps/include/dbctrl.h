
/*****************************************************************************
   ģ����      :
   �ļ���      : 
   ����ļ�    : 
   �ļ�ʵ�ֹ���:DB�̻߳ص���Ϣ����
   ����        :
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/07/08  1.0                     ����
******************************************************************************/
#ifndef _DB_CTRL_H_
#define _DB_CTRL_H_

#pragma warning( disable : 4786 )

#include "kdvtype.h"
#include "structextend.h"
#include "dbopr.h"

class IDbOpr;


BOOL32 PostMsgToSSn(u16 wEvent, u8 *const pbyMsg, u16 wLen, u16 wAppID, u16 wInstId);
BOOL32 PostNackToSsn(u16 wEvent, u16 wErrId, u16 wAppID, u16 wInstId);
BOOL32 PostNackExtendToSsn(u16 wEvent, u16 wErrId, void* pHttpConnection, u16 wAppID, u16 wInstId);


void DBCallBack(u16 wEventID, u8 *pMsgBuf, u16 wMsgLen, IDbOpr *pDBOpr);

u16 ProcQuerySrvDevListReq(const CGuidExtend *ptExtend, CDBOpr *pcOprDB);
u16 ProcUlcLoginReq(const CLoginExtend *ptExtend, CDBOpr *pcOprDB);
u16 Proc26UlcLoginReq(const CLoginExtend *ptExtend, CDBOpr *pcOprDB);
u16 Proc252UlcLoginReq(const CLoginExtend *ptExtend, CDBOpr *pcOprDB);
u16 PrintPlatformBackUpConfig(CDBOpr *pcOprDB);


#endif // end of _DB_CTRL_H_
