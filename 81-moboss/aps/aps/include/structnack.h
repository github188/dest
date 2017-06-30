
/*=======================================================================
ģ����      : ULS
�ļ���      : structnack.h
����ļ�    :
�ļ�ʵ�ֹ���:

����        : 
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/08/07
=======================================================================*/
#ifndef _STRUCT_APS_NACK_H_
#define _STRUCT_APS_NACK_H_



//...........................................
#ifdef WIN32
#pragma pack( push )
#pragma pack( 1 )
#ifndef PACKED
#define PACKED
#endif
#endif//.....................................

//...........................................
#ifdef _LINUX_
#ifndef PACKED
#define PACKED __attribute__ ((packed)) 
#endif
#endif//.....................................

#include "kdvtype.h"

#ifdef _JSON_CONVERT_
#include "jsonconvert.h"
#endif  //_JSON_CONVERT_

class CApsBaseNack
{
public:
    CApsBaseNack()
	{
		Init();
	}
   	
    void Init()
	{
		m_dwErrorId = 0;
	}
    
    u32 GetErrorId() const { return m_dwErrorId; }
    void SetErrorId(u32 dwErrID) { m_dwErrorId = dwErrID; }  
	
public: 
#ifdef _JSON_CONVERT_ 
	Json::Value StructToJson()
	{
		CJsonConvert jsonTool;
		
		jsonTool.SetValue("m_dwErrorId", GetErrorId());
		
	    return jsonTool.GetJson();
	}

    BOOL32 JsonToStruct(const char* szJson)
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
		
		//������Ϣ
		u32 dwErrId = 0;
		jsonTool.GetValue("m_dwErrorId", dwErrId);
		SetErrorId(dwErrId);
		
    	return TRUE;
	}
#endif //_JSON_CONVERT_
    
private:
    u32 m_dwErrorId;
} 
PACKED;



#ifdef WIN32
#pragma pack( pop )
#endif

#endif//  end of _STRUCT_APS_NACK_H_
