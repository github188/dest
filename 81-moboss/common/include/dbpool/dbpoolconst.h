#ifndef _BD_POOL_CONST_H_
#define _BD_POOL_CONST_H_

#include "kdvtype.h"


//数据库连接
#define MAXLEN_DB_ALIAS							(u8)64
#define MAXLEN_DB_NAME							(u8)64
#define MAXLEN_DB_USER_NAME						(u8)64
#define MAXLEN_DB_USER_PWD						(u8)32
#define MAXLEN_DB_IPADDR						(u8)32

#define MAXLEN_SQL_STRING						(u32)(1024*5)   //最大Sql语句长度
#define MAXLEN_DB_VALUE						    (u32)(1024)     //最大value值长度

//3 时间定义
//数据库操作超时(单位：秒)
#define DBOPR_TIMEOUT_DEFAULTSPAN				(u8)120//120秒

//DBTask 数据库连接检测的间隔次数(按挂起超时次数算)
#define DBPOOL_CHECKDBCONNECT_TIMES				(u32)(2000) //次




//4 程序有先级与堆栈大小定义
//数据库操作线程优先级
#define DBPOOL_TASK_PRIORITY					(u8)90
//数据库操作线程堆栈大小
#define DBPOOL_TASK_STACKSIZE					(u32)(512*1024)


#endif // _BD_POOL_CONST_H_
// END OF FILE

