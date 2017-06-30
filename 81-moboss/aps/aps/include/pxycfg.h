/*=======================================================================
模块名      :
文件名      :pxycfg.h
相关文件    :
文件实现功能:读写配置文件

作者        : 
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/19  5.0					      创建
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

//1.1 路径与名称
#ifdef _PPC_
#define PXY_CFG_PATH                    (const s8*)"/usr/etc/config/conf"                 //配置文件路径
#else
#define PXY_CFG_PATH                    (const s8*)"conf"
#endif
#define PXY_CFGFILE_NAME		        (const s8*)"pxycfg.json"              //配置文件名称


#define MAXLEN_CFG_VALUE				(u16)255


class CPxyCfg
{
public:
	CPxyCfg();

//private:
	void Init();

public:
    //配置文件是否存在
    BOOL32 IsConfigFileExist();
    const s8* GetCfgFileName() const { return m_szCfgFileName; }
        
    BOOL32 ReadConfigInfo(void);
 
	std::set<CPxyPlatformAddr> GetPxyPlatformSrvAddr() const { return m_setPxyPlatformSrvAddr; }

private:
	s8 m_szCfgFileName[MAXLEN_CFG_VALUE + 1];      //配置文件保存路径
	std::set<CPxyPlatformAddr> m_setPxyPlatformSrvAddr;      //sa所支持的所有的逻辑服务器及其模块列表
};

extern CPxyCfg g_cPxyCfg;


#endif	// _PXY_CFG_H_
// END FILE



