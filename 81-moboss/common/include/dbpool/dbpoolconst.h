#ifndef _BD_POOL_CONST_H_
#define _BD_POOL_CONST_H_

#include "kdvtype.h"


//���ݿ�����
#define MAXLEN_DB_ALIAS							(u8)64
#define MAXLEN_DB_NAME							(u8)64
#define MAXLEN_DB_USER_NAME						(u8)64
#define MAXLEN_DB_USER_PWD						(u8)32
#define MAXLEN_DB_IPADDR						(u8)32

#define MAXLEN_SQL_STRING						(u32)(1024*5)   //���Sql��䳤��
#define MAXLEN_DB_VALUE						    (u32)(1024)     //���valueֵ����

//3 ʱ�䶨��
//���ݿ������ʱ(��λ����)
#define DBOPR_TIMEOUT_DEFAULTSPAN				(u8)120//120��

//DBTask ���ݿ����Ӽ��ļ������(������ʱ������)
#define DBPOOL_CHECKDBCONNECT_TIMES				(u32)(2000) //��




//4 �������ȼ����ջ��С����
//���ݿ�����߳����ȼ�
#define DBPOOL_TASK_PRIORITY					(u8)90
//���ݿ�����̶߳�ջ��С
#define DBPOOL_TASK_STACKSIZE					(u32)(512*1024)


#endif // _BD_POOL_CONST_H_
// END OF FILE

