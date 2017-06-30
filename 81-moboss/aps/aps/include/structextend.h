
/*=======================================================================
ģ����      :
�ļ���      : structextend.h
����ļ�    :
�ļ�ʵ�ֹ���: �ڲ�������Ϣʱʹ�õĽṹ����չ��

����        :
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/07/25  1.0                     ����
=======================================================================*/
#ifndef _STRUCT_EXTEND_H_
#define _STRUCT_EXTEND_H_

#include "kdvtype.h"
#include "structlogin.h"
#include "jsonconvert.h"
#include "structnack.h"
#include "structulc.h"
#include "guid.h"


//��չ��Ϣ�����:����һЩ������Ϣ��ת������չ��Ϣ��
class CBaseExtendMsg
{
public:
    CBaseExtendMsg()
    {
        m_wAppID = 0;
        m_wInstID = 0;
        m_pHttpConnection = 0;
        m_byIsHttpsConnection = 0;
    }
	
    virtual ~CBaseExtendMsg()
    {
    }
	
    void SetSessionID(u16 dwAppID, u16 dwInstID) 
    { 
        m_wAppID = dwAppID; 
        m_wInstID = dwInstID;
    }

    u16  GetAppID(void) const { return m_wAppID; }
    u16  GetInstID(void) const { return m_wInstID; }

    void* GetHttpConnection() const { return m_pHttpConnection; }
    void SetHttpConnection(void* pConnect) { m_pHttpConnection = pConnect; }
    
    BOOL32 IsHttpsConnection() { return (0 != m_byIsHttpsConnection);}
    void SetIsHttpsConnection( BOOL32 bIsHttpsConnection ) { m_byIsHttpsConnection = (bIsHttpsConnection?1:0);}

protected:
    u16 m_wAppID;   	
    u16 m_wInstID;  
    void* m_pHttpConnection;
    u8 m_byIsHttpsConnection;
};


//�ڲ������õ�����չ��Ϣ��ģ��
template<class T>
class CExtendMgs : public CBaseExtendMsg
{
public:
    CExtendMgs(void) : CBaseExtendMsg()
    {
		
    }
	
    ~CExtendMgs(void)
    {
		
    }
public:
    T GetDetailBody() const { return m_cDetailBody; }
    void SetDetailBody(const T &tBody)
    {
        m_cDetailBody = tBody;
    }
	
	Json::Value StructToJson() const
	{
		CJsonConvert jsonTool;
		CBaseExtendMsg tExtend;
		jsonTool.SetBaseClassValue((const CBaseExtendMsg *)&tExtend);

		jsonTool.SetNestedValue("m_cDetailBody", GetDetailBody());

		return jsonTool.GetJson();
	}

	BOOL32 JsonToStruct(const s8* szJson)
	{
		if (NULL == szJson)
		{
			return FALSE;
		}
		
		CJsonConvert jsonTool;        
		BOOL32 bRet = jsonTool.ParseJsonStr(szJson);
		if (!bRet)
		{
			return FALSE;
		}

		CBaseExtendMsg tExtend;
		jsonTool.GetBaseClassValue((CBaseExtendMsg *)&tExtend);

		T tBody;
		jsonTool.GetNestedValue("m_cDetailBody", tBody);
		SetDetailBody(tBody);

		return TRUE;
	}

protected:
    T m_cDetailBody;
};

typedef CExtendMgs<CLoginInfo> CLoginExtend;
typedef CExtendMgs<CUlcFullInfo> CUlcFullInfoExtend;
typedef CExtendMgs<CApsBaseNack> CUlsBaseNackExtend;
typedef CExtendMgs<TGUID> CGuidExtend;


#endif //end of _STRUCT_EXTEND_H_
