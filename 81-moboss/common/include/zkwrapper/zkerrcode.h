
/*****************************************************************************
   ģ����      : zkwrapper
   ����        : �����붨��
   ����        : 
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/06/06  1.0				       ����
******************************************************************************/

#ifndef _ZK_ERROR_CODE_H_
#define _ZK_ERROR_CODE_H_


//������
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

    //������API����
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

    //������zookeeper�ڲ�����Ĵ���,������zkiflib����Ĵ���
    ZKINVALIDPATH               = -201, //��Ч·��
    ZKINVLIDIPPORT              = -202,	//��ЧIP:�˿�
    ZKWSASTARTUPERROR           = -203,	//WSAStartupʧ��
    ZKINITERROR                 = -204,	//zookeeper��ʼ������
    ZKTIMEOUTERROR              = -205,	//��ʱʱ�����
    ZKUNCONNECTED               = -206,	//ZKδ����
    ZKSESSIONERROR              = -207,	//ZK Session����
    ZKBUFFERLENERROR            = -208,	//buffer���ȴ���
    ZKPOINTERERROR              = -209,	//bufferָ�����
    ZKINVALIDNODENAME           = -210,	//�ڵ�������
    ZKErr_InvalidCBFun          = -210,	//��Ч�Ļص�����
    ZKQUEUENOTEXIST             = -211,	//���в�����
    ZKCHILDNOTEXIST             = -212,	//�ӽڵ㲻����
    ZKEND
};


#endif  //_ZK_ERROR_CODE_H_


