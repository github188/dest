


/*****************************************************************************
   ģ����      : 
   �ļ���      : apdb.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: 
   ����        : 
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2014/06/11  1.0				       ����
******************************************************************************/


#ifndef _XAP_AP_DB_H_
#define _XAP_AP_DB_H_

#include <set>
#include "basedb.h"
#include "kdvtype.h"

#include "structlogin.h"
#include "structself.h"
#include "serverdevice.h"
#include "platformdomain.h"

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

class CApDB : public CBaseDB
{
public:
    CApDB();
    virtual ~CApDB();
    
public:
    //ͨ���豸GUID����ѯ�豸����ϸ��Ϣ
    u16 GetSrvDevInfoByGUID(IN const s8* szDevGUID, OUT CServerDevice&  tSrvDevInfo);

    //ĳ��������ĳ���豸��Ϣ
    u16 GetBriefSrvInfo(IN const s8* ServiceDomainMOID, IN u16 dwDeviceType, OUT std::set<CBriefSrvInfo>& setBriefSrvInfo);

    //��ȡĳ��ƽ̨����Ϣ
    u16 GetPlatformDomain(IN const s8* szPlatformDomainMOID, OUT CPlatformDomain&  tPlatformDomain);

	u16 GetAllPlatformDoaminMOID(IN const s8* pchServiceDomainMOID, OUT std::set<std::string>& setPlatformDoaminMOID);

public:
    //��֤�ն˵�¼
    u16 Authentication(IN const CLoginInfo &tLoginInfo, OUT CDBMtInfo& tMtInfo, OUT CPlatformAttr& tPlatformAttr);	

private:
    u16 ReadSrvDeviceInfo( OUT CServerDevice& tSrvDeviceInfo );
    u16 ReadPlatformDomainInfo(OUT CPlatformDomain& tPlatformDomain);
};



#endif /*_XAP_AP_DB_H_*/




