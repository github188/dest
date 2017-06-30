
/*****************************************************************************
   模块名      : zkwrapper
   功能        : zookeeper.h的C++封装,方便C++客户调用,已支持多线程
                 本库只是在"\b0-thirdparty\zkiflib"的基础上做了点滴改动
   作者        : 
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/06/06  1.0		        	     创建
******************************************************************************/

#ifndef _ZK_WRAPPER_H_
#define _ZK_WRAPPER_H_


#ifdef  ZKWRAPPER_EXPORT
#define ZKWRAPPER_API __declspec( dllexport )
#else
#define ZKWRAPPER_API
#endif

#include <stdlib.h>
#include "kdvtype.h"
#include "zkconst.h"
#include "zkerrcode.h"
#include "zkstruct.h"


class CZkImpl;
class ZKWRAPPER_API CZkWrapper
{
public:
    CZkWrapper();
    ~CZkWrapper();

private:    //不允许进行拷贝构造与赋值构造
    CZkWrapper(const CZkWrapper& tOther);
    CZkWrapper& operator = (const CZkWrapper& tOther);


public: /********************** 连接接口 *********************/
    /*=============================================================================
    函 数 名：	ConnectSrv
    功    能：	连接zk服务器
    参    数：	dwIp,         zk服务器的ip地址(网络序)
                wPort,        zk服务器的port端口
                funDisConCB,  断开连接后的回调函数，不允许为NULL
                byTimeOut     超时时间，单位秒. 超时时间必须大于1s
                szHostPort    zk服务器的ip:port, e.g. "127.0.0.1:3000,127.0.0.1:3001,127.0.0.1:3002"

    返 回 值：	错误码
    =============================================================================*/
    s32 ConnectSrv(IN u32 dwIp, IN u16 wPort, IN DisConnectCallBackFunc funDisConCB, IN u8 byTimeOut = 5);
    s32 ConnectSrv(IN const s8* szHostPort, IN DisConnectCallBackFunc funDisConCB, IN  u8 byTimeOut = 5);

    /*=============================================================================
    函 数 名：	DisConnect
    功    能：	关闭与zk服务器的连接
    参    数： 
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    s32 DisConnect( );

    /*=============================================================================
    函 数 名：	IsConnected
    功    能：	查询是否已经连上ZK服务器 
    参    数：	
    返 回 值：	TRUE:连接; FALSE:未连接
    =============================================================================*/
    BOOL32 IsConnected( );

    /*=============================================================================
    函 数 名：	SetAuthToDigest
    功    能：	设置ACL模式为digest
    参    数：	szUserPwd           用户名密码，格式为  user:psw
                szUser              用户
                szPwd               密码
    返 回 值：  s32,参见错误码 
    =============================================================================*/
    s32 SetAuthToDigest(IN const s8* szUserPwd);
    s32 SetAuthToDigest(IN const s8* szUser, IN const s8* szPwd);


public:   /********************** Node操作接口 *********************/  
    /*=============================================================================
    函 数 名：	CreateNode
    功    能：	创建结点(注意：节点已经存在时，返回成功；如果)
    参    数：	szNodePath      结点路径
			    bAlwaysCreate   是否级联创建
			    nCreateMode     CREATE_MODE_NORMAL为永久结点，CREATE_MODE_SEQUENCE为顺序永久结点，
								CREATE_MODE_EPHEMERAL为临时结点，CREATE_MODE_SEQUENCE_EPHEMERAL为临时顺序结点
				bErrWhenExist	如果节点已经存在时，是否return错误(顺序型节点不会出现名字相同的情况)
			    szNodeContent   结点内容
			    nContentLen     结点内容长度
			    bUnSafe         结点权限是否为任何人可访问
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    s32 CreateNode(IN const s8* szNodePath, IN BOOL32 bAlwaysCreate, IN EMNodeMode emCreateMode = EmNodeMode_Normal, IN BOOL32 bErrWhenExist = FALSE, IN const u8* byNodeContent = NULL, IN s32 nContentLen = 0, IN BOOL32 bUnSafe = TRUE);

    /*=============================================================================
    函 数 名：	DeleteNode
    功    能：	删除结点
    参    数：	szNodePath      结点路径
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    s32 DeleteNode(IN const s8* szNodePath);

    /*=============================================================================
    函 数 名：	IsNodeExist
    功    能：	判断结点是否存在
    参    数：	szNodePath  结点路径
    返 回 值：	TRUE:存在; FALSE:不存在 
    =============================================================================*/
    BOOL32 IsNodeExist(IN const s8* szNodePath);

    /*=============================================================================
    函 数 名：	GetNodeDataLen
    功    能：	获取结点数据的长度
    参    数：	szNodePath      结点路径
                nDataLen        数据的长度
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    s32 GetNodeDataLen(IN const s8* szNodePath, OUT s32& nDataLen);

    /*=============================================================================
    函 数 名：	GetNodeInfo
    功    能：	获取结点内容 
    参    数：	szNodePath      结点路径
			    byBuffer        内容
			    nBufferLen      传入Buffer长度,传入实际长度
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    s32 GetNodeInfo(IN const s8* szNodePath,  OUT u8* byBuffer, IN OUT s32& nbufferLen);

    /*=============================================================================
    函 数 名：	SetNodeInfo
    功    能：	设置结点内容
    参    数：	szNodePath      结点路径
			    byBuffer        内容
			    nBufferLen      内容长度
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    s32 SetNodeInfo(IN const s8* szNodePath,  IN const u8* byBuffer, IN s32 nBufferLen);
    s32 SetNodeInfo(IN const s8* szNodePath,  IN const s8* szData);

    /*=============================================================================
    函 数 名：	GetChildNodeNum
    功    能：	获取子节点数量
    参    数：	szNodePath      结点路径
			    nChildNum       结点个数
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    s32 GetChildNodeNum(IN const s8* szNodePath,  OUT s32& nChildNum);
    
    /*=============================================================================
    函 数 名：	GetChildNodeName
    功    能：	获取各子节点的名称
    参    数：	NodeNameArray       存放子节点名称的数组
			    nArrayNum           数组数量
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    s32 GetChildNodeName(IN const s8* szNodePath,  OUT NodeName NodeNameArray[], IN OUT s32& nArrayNum);

    /*=============================================================================
    函 数 名：	SetWatch
    功    能：	设置watch(连接断开后，关注自动失效)
    参    数：	szWatchNode         watch结点
			    funCallBackk        回调函数(不同的Node,回调函数可以不同)
                                    回调函数会在另一个线程中回调上层业务，上层业务需根据情况加锁
                bConsistentWatch    是否持续关注; FALSE:只关注一次,收到一次回调后,关注自动失效;
                                    TRUE:持续关注,只要有变化,都能收到watch消息。(受限于zk自身机制,如果一个节点,2次改变之间的时间很短(毫秒级),
                                    上层可能只收到第一次回调;如果这2次改变为同一类型,例如同为节点内容发生改变,虽然只回调一次,还是能确保上层获取
                                    到的是此节点的最新内容;如果这2次改变为不同类型,上层很可能收不到第二次的回调;上层应用需要在设计层面考虑到此情况)
                bWatchChild         是否关注子节点数量的变化
    返 回 值：  s32,参见错误码 
    =============================================================================*/
    s32 SetWatch(IN const s8* szWatchNode, IN WatchCallBackFunc funCallBack, IN BOOL32 bConsistentWatch = TRUE, IN BOOL32 bWatchChild = TRUE);


public:   /********************** 队列操作接口 *********************/  
    /*=============================================================================
    函 数 名：	QueueCreate
    功    能：	创建一个队列(连接断开后，队列自动失效)
    参    数：	szQueuePath         队列路径(key)
                funReadCB           读取队列节点后的回调函数;
                                    为NULL时,表示不读取队列上的节点;
                                    非NULL时,一旦队列上有节点，就会读取节点内容并回调上层; 
                                    回调函数会在另一个线程中回调上层业务，上层业务需根据情况加锁
    返 回 值：	s32,参见错误码 (如果队列已经存在,返回成功)
    =============================================================================*/
    s32 QueueCreate(IN const s8* szQueuePath, IN QueueCallBackFunc funReadCB);
    
    /*=============================================================================
    函 数 名：	QueueDestroy
    功    能：	释放队列
    参    数：	szQueuePath         队列路径(key)
    返 回 值：	s32,参见错误码 (如果队列本来就不存在,返回成功)
    =============================================================================*/
    s32 QueueDestroy(IN const s8* szQueuePath);

    /*=============================================================================
    函 数 名：	ZKQueueAdd
    功    能：	增加一个队列结点
    参    数：	szQueuePath         队列路径(key)
                byData              数据
			    s32 nLen            数据长度
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    s32 QueuePush(IN const s8* szQueuePath, IN const u8* byData, IN s32 nLen);
    s32 QueuePush(IN const s8* szQueuePath, IN const s8* szData);

    /*=============================================================================
    函 数 名：	IsQueueExist
    功    能：	判断队列是否存在
    参    数：	szQueuePath  队列路径(key)
    返 回 值：	TRUE:存在; FALSE:不存在 
    =============================================================================*/
    BOOL32 IsQueueExist(IN const s8* szQueuePath);


public:  /********************** 锁操作接口 *********************/  
    /*=============================================================================
    函 数 名：	LockCreate
    功    能：	创建一个锁(连接断开后，锁自动失效)
    参    数：	szLockPath          Lock路径(key)
    返 回 值：	s32,参见错误码 (如果锁已经存在,返回成功)
    =============================================================================*/
    s32 LockCreate(IN const s8* szLockPath);

    /*=============================================================================
    函 数 名：	ZKLockDestory
    功    能：	lock销毁
    参    数：	szLockPath          Lock路径(key)
    返 回 值：	s32,参见错误码 (如果Lock本来就不存在,返回成功)
    =============================================================================*/
    s32 LockDestory( IN const s8* szLockPath );

    /*=============================================================================
    函 数 名：	LockGet
    功    能：	获取lock
    参    数：	szLockPath          Lock路径(key)
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    s32 LockGet( IN const s8* szLockPath );

    /*=============================================================================
    函 数 名：	LockFree
    功    能：	释放lock
    参    数：	szLockPath          Lock路径(key)
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    s32 LockFree( IN const s8* szLockPath );

    /*=============================================================================
    函 数 名：	IsQueueExist
    功    能：	判断lock是否存在
    参    数：	szLockPath          Lock路径(key)
    返 回 值：	TRUE:存在; FALSE:不存在 
    =============================================================================*/
    BOOL32 IsLockExist(IN const s8* szLockPath);

    /*=============================================================================
    函 数 名：	IsLockOwner
    功    能：	判断是否锁的拥有者
    参    数：	szLockPath          Lock路径(key)
    返 回 值：	s32,参见错误码 
    =============================================================================*/
    BOOL32 IsLockOwner( IN const s8* szLockPath );

public:  /********************** 调试打印操作接口 *********************/  
	    /*=============================================================================
    函 数 名：	Setdebuglevel
	功    能：	设置调试打印级别
	参    数：	nDbgLvl           调试打印级别
	返 回 值：	 
	=============================================================================*/
	void SetDebugLevel(IN s32 nDbgLvl);

private:
    CZkImpl* m_pZkImpl;
};

#endif  //_ZK_WRAPPER_H_


