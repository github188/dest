
/*****************************************************************************
   模块名      :
   文件名      : ulcssn.h
   相关文件    : ulcssn.cpp
   文件实现功能: 与终端的通信
   作者        :
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/07/08  1.0                     创建
******************************************************************************/
#ifndef _ULC_SSN_H_
#define _ULC_SSN_H_

#pragma warning( disable : 4786 )

#include "osp.h"
#include "kdvtype.h"
#include "constuls.h"
//#include "kdmstruct.h"
#include "bossutils.h"

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
using namespace BossUtils;
class CUlcSsnInst : public CInstance
{
public:
	enum
	{
		STATE_IDLE = 0,    //空闲
		STATE_NORMAL       //工作
	};

	CUlcSsnInst();
	~CUlcSsnInst();

public:
	void DaemonInstanceEntry( CMessage *const pcMsg, CApp* pcApp);
    void InstanceEntry(CMessage *const pcMsg);

	void DaemonProcPowerOn(CMessage *const pcMsg);
	void ProcDisconnect(CMessage *const pcMsg);
	void ClearInst(void);

	//2.6版本终端登录
	void DaemonProcUlcLoginReq(CMessage *const pcMsg,  CApp* pcApp);
    void ProcUlcLoginReq(CMessage *const pcMsg);
	void ProcUlcLoginAck(CMessage *const pcMsg);
	void ProcUlcLoginNack(CMessage *const pcMsg);

    //2.5.2版本终端登录
    void Old252_DaemonProcUlcLoginReq(CMessage *const pcMsg,  CApp* pcApp);
    void Old252_ProcUlcLoginReq(CMessage *const pcMsg);    
    void Proc252UlcLoginAck(CMessage *const pcMsg);
    void Proc252UlcLoginNack(CMessage *const pcMsg);

protected: //dump
	void DaemonInstanceDump(u32 param /*=0*/);
    void DumpAllInstState(void);
    const s8 *GetInsStateStr();

protected:
	BOOL32 PostMsgToCtrl(u16 wEvent, u8 *const pbyMsg, u16 wLen);
	BOOL32 PostMsgToUlc(u16 wEvent, u8 *const pbyMsg, u16 wLen);
    BOOL32 PostNackToUlc(u32 dwErrID);

protected:
    u32 GetUlcNode() const { return m_dwUlcNode; }
    u32 GetUlcIID() const { return m_dwUlcIID; }
    std::string GetUlcIPString() const;
    time_t GetLoginTime() const { return GetSysTime(); }

private:
	u32          m_dwUlcNode;
    u32          m_dwUlcIID;
    u32          m_dwUlcIP;
    time_t       m_tLoginTime;  //登陆时间
};

class CUlcSsnConfig
{
public:
	CUlcSsnConfig();
	virtual ~CUlcSsnConfig();

public:	
    BOOL32 GetIdleInstID(OUT u16 &wIdleInst);
	
private:
    u16     m_wLastInstID;   //上次回话使用到的SessionId(查找idle实例时不要重头开始寻找，提升效率)
};

typedef zTemplate<CUlcSsnInst, MAXINSTNUM_ULCSSN, CUlcSsnConfig> COldUlcSsnApp;

extern COldUlcSsnApp g_cOldUlcSsnApp;




#endif //end of _ULC_SSN_H_
