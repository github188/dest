/*****************************************************************************
模块名      : MAU
文件名      : hdussn.h
创建时间    : 2015年 01月 26日
实现功能    : 
作者        : 阮楠楠
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/01/26  5.0         阮楠楠          创建
******************************************************************************/
#ifndef	_HDUSSN_INSTANCE_H_
#define	_HDUSSN_INSTANCE_H_

#include "osp.h"
#include "hdustruct.h"
#include "evhduacs.h"


#define CONNECTHDUPOOL_TIMEOUT         (u16)3*1000
#define REGISTHDUPOOL_TIMEOUT          (u16)3*1000

struct THduPoolInfo
{
	u32 m_dwIp;
	u32 m_wPort;
};

class CHduPoolSsnInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_NORMAL,
	};

public:
	CHduPoolSsnInstance();
	~CHduPoolSsnInstance();

public:
	void DaemonInstanceEntry( CMessage *const pcMsg, CApp *pcApp );
	//上电初始化函数
	void DaemonProcPowerOn( CMessage * const pcMsg );
	//断链处理
	void DaemonProcDisconnect( CMessage * const pcMsg );
	//连接HduPool定时器消息
	void DaemonProcConnectHduPool(CMessage * const pcMsg);
	//连接HduPool定时器消息
	void DaemonProcConnectTimer( CMessage * const pcMsg );
	//注册HduPool定时器消息处理
	void DaemonProcRegHduPool( CMessage * const pcMsg );
	//HduPool注册答复消息处理
	void DaemonProcHduPoolRegRsp( CMessage * const pcMsg );
	//处理HduPool发送给mau的hdu通知
	void DaemonProcHduInfoNtf( CMessage * const pcMsg );
	//处理mau给HduPool的hdu信息请求答复
	void DaemonProcHduInfoReqRsp( CMessage * const pcMsg );
	//处理会管设置hdu通道音量
	void DaemonProcSetHduChnVolume( CMessage * const pcMsg );

	void InstanceEntry( CMessage * const pcMsg );
	
	//连接hdupool
	BOOL32 ConnectHduPool();
	//注册hdupool
	void RegistHduPool();

public:
	u32	m_dwHduPoolNode;
	u32 m_dwHduPoolIId;
	u32 m_dwIp;
	u32 m_wPort;
};

typedef zTemplate<CHduPoolSsnInstance, 1> CHduPoolSsnApp;

#endif
