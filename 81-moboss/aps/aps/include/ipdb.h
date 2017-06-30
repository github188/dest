


/*****************************************************************************
   ģ����      : 
   �ļ���      : ipdb.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: IP��ַ����������Ӫ�̲�ѯ
   ����        : 
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2014/06/11  1.0				       ����
******************************************************************************/


#ifndef _XAP_IP_DB_H_
#define _XAP_IP_DB_H_

#include "basedb.h"
#include "kdvtype.h"
#include <string>

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

class CIpDB : public CBaseDB
{
public:
    CIpDB();
    virtual ~CIpDB();
    
public:
    //��ȡIP��ַ����Ӫ������
    u16 GetIpISPName(IN u32 dwClientIp/*������*/, OUT std::string& strISPName);	

private:
    u16 QueryForeignISPName(IN u32 dwClientIp/*������*/, OUT std::string& strISPName);
};



#endif /*_XAP_IP_DB_H_*/




