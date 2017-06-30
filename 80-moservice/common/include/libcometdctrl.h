#pragma once

#ifdef COMETDCTRLDLL_EXPORT
#ifdef WIN32
	#define COMETDCTRLDLL_EXPORT __declspec(dllexport)
#else
	#define COMETDCTRLDLL_EXPORT
#endif
#else
#ifdef WIN32
	#define COMETDCTRLDLL_EXPORT __declspec(dllimport)
#else
	#define COMETDCTRLDLL_EXPORT
#endif
#endif


#include <string.h>
#ifdef _LINUX_
#include <dlfcn.h>
#endif _LINUX_



enum EmCometdErr
{
	emNoerr =0,
	emInitJVMerr,
	emInitClientErr,
//	emCreateClientErr,
	emClientShakeErr,
};

//消息回调给C++
typedef struct tagCBInfo
{
	char	m_strID[33];//发送的序列号（有cometd的java客户端库自己产生）
	bool	m_bResult;//true 成功  false失败
	int		m_nReason;//暂时未用

	tagCBInfo()
	{
		memset(m_strID, 0, sizeof(m_strID));
		m_bResult = false;
		m_nReason = 0;
	}
}TCBInfo;

///////////////////////////////////////////////
//作用：初始化InitCometdCtrlLib
//参数：[in] bWebSocket 是否是websocket
//返回  返回错误号
//姓名         时间           修改记录 
//沈钦       2014/05/04        创建
///////////////////////////////////////////////////
EmCometdErr COMETDCTRLDLL_EXPORT InitCometdCtrlLib(bool bWebSocket, char *URL);

///////////////////////////////////////////////
//作用：与Cometd服务器握手
//参数：[in] URL 格式："http://172.16.80.49:8080/cometdPrj/cometd"
//返回  返回错误号
//姓名         时间           修改记录 
//沈钦       2014/05/04        创建
///////////////////////////////////////////////////
EmCometdErr COMETDCTRLDLL_EXPORT HandShake();

///////////////////////////////////////////////
//作用：销毁CometdClient
//参数：[in] 空
//返回  返回错误号
//备注： 程序退出时调用
//姓名         时间           修改记录 
//沈钦       2014/05/04        创建
///////////////////////////////////////////////////
EmCometdErr COMETDCTRLDLL_EXPORT DestroyCometdCtrlLib();
///////////////////////////////////////////////
//作用：发送数据
//参数：[in] pVal为指向字符串首地址或者是Json对象的地址
//      [in] IsJson 表明pVal是否是Json
//返回  true 成功,  false 失败
//姓名         时间           修改记录 
//沈钦       2014/05/04        创建
///////////////////////////////////////////////////
bool COMETDCTRLDLL_EXPORT SendData(void *pVal, bool IsJson=true);
///////////////////////////////////////////////
//作用：是否已连接
//返回  
//姓名         时间           修改记录 
//沈钦       2014/05/04        创建
///////////////////////////////////////////////////
bool COMETDCTRLDLL_EXPORT IsConnected();

typedef void (*Myresult)(TCBInfo *);
void COMETDCTRLDLL_EXPORT SetCallback(Myresult pfun);