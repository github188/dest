/*=======================================================================
ģ����      :
�ļ���      :pxycfg.h
����ļ�    :
�ļ�ʵ�ֹ���:��д�����ļ�

����        : 
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/19  5.0					      ����
=======================================================================*/

#ifndef _PXY_CFG_H_
#define _PXY_CFG_H_

#include "osp.h"
#include "kdvtype.h"
#include <set>
#include "structpxy.h"




#ifdef WIN32
#ifndef vsnprintf
#define vsnprintf   _vsnprintf
#endif
#ifndef snprintf
#define snprintf    _snprintf
#endif
#endif

//1.1 ·��������
#ifdef _PPC_
#define PXY_CFG_PATH                    (const s8*)"/usr/etc/config/conf"                 //�����ļ�·��
#else
#define PXY_CFG_PATH                    (const s8*)"conf"
#endif
#define PXY_CFGFILE_NAME		        (const s8*)"pxycfg.json"              //�����ļ�����


#define MAXLEN_CFG_VALUE				(u16)255


class CPxyCfg
{
public:
	CPxyCfg();

//private:
	void Init();

public:
    //�����ļ��Ƿ����
    BOOL32 IsConfigFileExist();
    const s8* GetCfgFileName() const { return m_szCfgFileName; }
        
    BOOL32 ReadConfigInfo(void);
 
	std::set<CPxyPlatformAddr> GetPxyPlatformSrvAddr() const { return m_setPxyPlatformSrvAddr; }

private:
	s8 m_szCfgFileName[MAXLEN_CFG_VALUE + 1];      //�����ļ�����·��
	std::set<CPxyPlatformAddr> m_setPxyPlatformSrvAddr;      //sa��֧�ֵ����е��߼�����������ģ���б�
};

extern CPxyCfg g_cPxyCfg;


#endif	// _PXY_CFG_H_
// END FILE



