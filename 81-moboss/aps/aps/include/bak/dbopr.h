
/*****************************************************************************
   ģ����      : 
   �ļ���      : dbopr.h
   ����ļ�    : dbopr.cpp
   �ļ�ʵ�ֹ���: ����ap���ݿ�Ľӿ�
   ����        :
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/07/08  1.0                     ����
******************************************************************************/
#ifndef _XAP_DBOPR_H_
#define _XAP_DBOPR_H_

#include "multidbopr.h"
#include "kdvtype.h"

#include "apdb.h"
#include "ipdb.h"
#include "serverdevice.h"

class CDBOpr : public CMultiDBOpr
{
public:
	CDBOpr();
	~CDBOpr();

    //ע��:��ȡDBItem��˳�������CDBPool::StartDBPoolʱ����std::vector<TDBConnectInfo>��˳��һ��
    virtual void GetAllDBEngine(std::vector<CBaseDB*>& vcDBItem);
    void   ClearResults();  // ��ս����

public:
    //ͨ���豸GUID����ѯ�豸����ϸ��Ϣ
    u16 GetSrvDevInfoByGUID(IN const s8* szDevGUID, OUT CServerDevice&  tSrvDevInfo);

    //ĳ��������ĳ�ַ������豸��Ϣ
    u16 GetBriefSrvInfo(IN const s8* ServiceDomainMOID, IN u16 dwDeviceType, OUT std::set<CBriefSrvInfo>& setBriefSrvInfo);

    //��ȡĳ��ƽ̨����Ϣ
    u16 GetPlatformDomain(IN const s8* szPlatformDomainMOID, OUT CPlatformDomain&  tPlatformDomain);

	//��ȡĳ��������������ƽ̨����Ϣ
    u16 GetAllPlatformDomainMOID(IN const s8* pchServiceDoaminMOID, OUT std::set<std::string>&  vecPlatformDoaminMOID);


	//��֤�ն˵�¼
	u16 Authentication(IN const CLoginInfo &tLoginInfo, OUT CDBMtInfo& tMtInfo, OUT CPlatformAttr& tPlatformAttr);	

    //��ȡIP��ַ����Ӫ������(�ӿ�2��ȡ��������)
    u16 GetIpISPName(IN u32 dwClientIp/*������*/, OUT std::string& strISPName);
#ifdef _APS_PXY_
	u16 GetIpISPName(IN u32 dwClientIp/*������*/, OUT EOperatorsType& emIspType);
#endif

private:	
    CApDB m_tApDB;
    CIpDB m_tIpDB;
};

#endif //end of _XAP_DBOPR_H_
