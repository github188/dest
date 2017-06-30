
/*****************************************************************************
   模块名      :
   文件名      : consassn.h
   相关文件    : consassn.cpp
   文件实现功能: 与3A的通信，获得各服务器的信息
   作者        :
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/07/08  1.0                     创建
******************************************************************************/
#ifndef _CON_SA_SSN_H_
#define _CON_SA_SSN_H_

#pragma warning( disable : 4786 )


#include "osp.h"
#include "connectsa_api.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////



class CConSaSsnInst : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_CONNECT,
		STATE_NORMAL,
	};

	CConSaSsnInst();
	~CConSaSsnInst();
    void ClearInst();

public:
	void DaemonInstanceEntry( CMessage *const pcMsg, CApp* pcApp);
	void InstanceEntry(CMessage *const pMsg);

	//
	void DaemonProcPowerOn(CMessage *const pcMsg);

    //检查服务器状态
    //理论上应该放到CCenterCtrlApp中,但是当很多线路都不通时，
    //此函数会造成很长时间的堵塞, 放在CCenterCtrlApp中,会导整个程序性能的严重下降
    void CheckSrvStateTimeOut(CMessage *const pcMsg);

};

class CConSaSsnConfig
{

};

typedef zTemplate<CConSaSsnInst, 1, CConSaSsnConfig> CConSaSsnApp;

extern CConSaSsnApp g_cConSaSsnApp;

#endif  //end of _CON_SA_SSN_H
