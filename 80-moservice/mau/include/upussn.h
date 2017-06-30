/*****************************************************************************
模块名      : MAU
文件名      : upussn.h
创建时间    : 2014年 08月 26日
实现功能    : 
作者        : 王也
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/08/26  5.0         王也          创建
******************************************************************************/
#ifndef	_UPUSSN_INSTANCE_H_
#define	_UPUSSN_INSTANCE_H_

#include "osp.h"
#include "operationconst.h"
#include "vccommon.h"
#include "ztemplatedaemon.h"
#include <map>
#include <string>
using namespace std;
#include "upu_client.h"
#include "upu_struct.h"

struct TUpuConfInfo
{
	// 注意，采用按位形式
	enum EmConfType
	{
		emNULL = 0,
		emScheConf = 0x1<<0,
		emOnGoingConf = 0x1<<1,
		emTempConf = 0x1<<2,
	};

	TUpuConfInfo()
	{
		m_byConfType = 0;
		m_dwMcuIp = 0;
		memset(m_achConfE164, 0, sizeof(m_achConfE164));
		memset(m_achDomainMOID, 0, sizeof(m_achDomainMOID));
		memset(m_achPaltformMOID, 0, sizeof(m_achPaltformMOID));
	}

	void AddConfType(u8 byType){ m_byConfType |= byType; }
	void RemoveConfType(u8 byType) { m_byConfType &= (~byType); }
	BOOL32 IsOngoingConf() const{ return (m_byConfType & emOnGoingConf) == 0 ? FALSE : TRUE; }
	BOOL32 IsScheduleConf() const { return (m_byConfType & emScheConf) == 0 ? FALSE : TRUE; }
	BOOL32 IsTempConf() const { return (m_byConfType & emTempConf) == 0 ? FALSE : TRUE; }
	u8 GetConfType() const{ return m_byConfType; }
	void SetE164(const s8* pchE164){ strncpy(m_achConfE164, pchE164, sizeof(m_achConfE164) - 1); }
	const s8* GetE164() const{ return m_achConfE164; }
	void SetDomainMoid(const s8* pchMoid){ strncpy(m_achDomainMOID, pchMoid, sizeof(m_achDomainMOID) - 1); }
	void SetPlatformMoid(const s8* pchMoid) { strncpy(m_achPaltformMOID, pchMoid, sizeof(m_achPaltformMOID) - 1); }
	u32 GetMcuIp() const { return m_dwMcuIp; }
	void SetMcuIp(u32 dwMcuIp){ m_dwMcuIp = dwMcuIp; }

	u8			m_byConfType;
	s8			m_achConfE164[MAXLEN_CONFE164 + 1];
	s8			m_achDomainMOID[OPERATION_MAXLEN_MOID + 1];
	s8			m_achPaltformMOID[OPERATION_MAXLEN_MOID + 1];
	u32			m_dwMcuIp;
};

class CUpuSsnInstance : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,
		STATE_NORMAL,
	};

public:
	CUpuSsnInstance();
	~CUpuSsnInstance();

public:
	//守护实例消息处理入口
	void DaemonInstanceEntry( CMessage * const pcMsg, CApp *pcApp );
	//守护实例处理初始化消息
	void DaemonProcPowerOn( const CMessage * pcMsg, CApp *pcApp );

	void DaemonConnect(const CMessage * pcMsg, CApp* pcApp);

	void DaemonDisConnect( const CMessage * pcMsg, CApp* pcApp);

	void DaemonAddConf( CMessage * const pcMsg, CApp* pcApp);

	void DaemonDelConf( CMessage * const pcMsg, CApp* pcApp);

	void DaemonSendAllInfo(CMessage * const pcMsg, CApp* pcApp);

	void InstanceEntry(  CMessage * const pcMsg );
	
	void Print(CApp* pcApp, u32 /* = 0 */);

private:

};

class CUpuClient
{
public:
	CUpuClient()
	{
		m_pClient = NULL;
		m_hUpuTask = NULL;
		m_bConnected = FALSE;
		m_bTerminal = FALSE;
	}
	virtual ~CUpuClient()
	{
		Stop();
		Destory();
	}

public:
	static void* UpuInitProc(void * pParam);
	BOOL32 Init( upu_callback pfCallBack );
	void Destory();
	BOOL32 Start();
	void Stop();
	upuclient_t * GetClient(void);
	BOOL32 SetClientId(const s8* pchClientId);
	BOOL32 RemoveConfByDevid(const s8* pchDevid);
	BOOL32 RemoveConfByE164(const s8* pchE164);
	BOOL32 AddConf(const TUpuConfInfo& tConfInfo);
	void SetConnectState(BOOL32 bState);
	BOOL32 IsConnected();
	BOOL32 IsTerminal();
protected:
	upuclient_t * m_pClient;
	BOOL32 m_bConnected;
	TASKHANDLE m_hUpuTask;
	BOOL32 m_bTerminal;
};

typedef  std::map<std::string, TUpuConfInfo> TUpuConfInfoSet;
class CUpuSsnData
{
public:
	CUpuSsnData() {}
	virtual ~CUpuSsnData() {}
	void Quit();
	TUpuConfInfoSet  m_vctConfInfo;
	CUpuClient	m_cUpuClient;
};

typedef zTemplate<CUpuSsnInstance, 0, CUpuSsnData, 0> CUpuSsnApp;
extern CUpuSsnApp g_cUpuSsnApp;

BOOL32 UpuAddConf(const s8* pchConfE164, 
	const TUpuConfInfo::EmConfType emConfType,
	const s8* pchDomainMOID, 
	const s8* pchPlatformDomainMOID,
	const s8* pchMcuIp = NULL);
BOOL32 UpuDelConf(const s8* pchConfE164, const TUpuConfInfo::EmConfType emConfType);

#endif  // _UPUSSN_INSTANCE_H_

