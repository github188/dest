
/*****************************************************************************
   模块名      : zkwrapper
   功能        : 错误码定义
   作者        : 
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/06/06  1.0				       创建
******************************************************************************/

#ifndef _ZK_ERROR_CODE_H_
#define _ZK_ERROR_CODE_H_


//错误定义
enum ZK_ERROR
{
    ZKOK                        = 0,
    ZKSYSTEMERROR               = -1,	//System and server-side errors.
    ZKRUNTIMEINCONSISTENCY      = -2,	//A runtime inconsistency was found 
    ZKDATAINCONSISTENCY         = -3,	//A data inconsistency was found
    ZKCONNECTIONLOSS            = -4,	//Connection to the server has been lost
    ZKMARSHALLINGERROR          = -5,	//Error while marshalling or unmarshalling data
    ZKUNIMPLEMENTED             = -6,	//Operation is unimplemented
    ZKOPERATIONTIMEOUT          = -7,	//Operation timeout
    ZKBADARGUMENTS              = -8,	//Invalid arguments
    ZKINVALIDSTATE              = -9,	//Invliad zhandle state 

    //以下是API错误
    ZKAPIERROR                  = -100, 
    ZKNONODE                    = -101,	//Node does not exist
    ZKNOAUTH                    = -102,	//Not authenticated
    ZKBADVERSION                = -103,	//Version conflict 
    ZKNOCHILDRENFOREPHEMERALS   = -108,	//Ephemeral nodes may not have children 
    ZKNODEEXISTS                = -110,	//The node already exists 
    ZKNOTEMPTY                  = -111,	//The node has children 
    ZKSESSIONEXPIRED            = -112,	//The session has been expired by the server 
    ZKINVALIDCALLBACK           = -113,	//Invalid callback specified 
    ZKINVALIDACL                = -114,	//Invalid ACL specified 
    ZKAUTHFAILED                = -115,	//Client authentication failed 
    ZKCLOSING                   = -116,	//ZooKeeper is closing 
    ZKNOTHING                   = -117,	//(not error) no server responses to process 
    ZKSESSIONMOVED              = -118,	//session moved to another server, so operation is ignored 

    //以上是zookeeper内部定义的错误,以下是zkiflib定义的错误
    ZKINVALIDPATH               = -201, //无效路径
    ZKINVLIDIPPORT              = -202,	//无效IP:端口
    ZKWSASTARTUPERROR           = -203,	//WSAStartup失败
    ZKINITERROR                 = -204,	//zookeeper初始化错误
    ZKTIMEOUTERROR              = -205,	//超时时间错误
    ZKUNCONNECTED               = -206,	//ZK未连接
    ZKSESSIONERROR              = -207,	//ZK Session错误
    ZKBUFFERLENERROR            = -208,	//buffer长度错误
    ZKPOINTERERROR              = -209,	//buffer指针错误
    ZKINVALIDNODENAME           = -210,	//节点名错误
    ZKErr_InvalidCBFun          = -210,	//无效的回调函数
    ZKQUEUENOTEXIST             = -211,	//队列不存在
    ZKCHILDNOTEXIST             = -212,	//子节点不存在
    ZKEND
};


#endif  //_ZK_ERROR_CODE_H_


