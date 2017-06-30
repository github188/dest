
#ifndef _DBENGINE_ERR_CODE_H_
#define _DBENGINE_ERR_CODE_H_


//数据库操作的一些错误码

#define   SUCCESS_DBENGINE							(u16)0 //opr success
#define	  COMMON_ERR_DBENGINE                       u16(0xffff)

#define   ERR_DBENGINE_INTER_BGN					(u16)1
#define   ERR_DBENGINE_INTER_INVALID_PARAM			ERR_DBENGINE_INTER_BGN     //invalid param
#define   ERR_DBENGINE_INTER_ALLOC_MEMORY			ERR_DBENGINE_INTER_BGN + 1 //fail to alloc memory
#define   ERR_DBENGINE_INTER_SET_CBDATA				ERR_DBENGINE_INTER_BGN + 2 //fail to set ErrFuc
#define   ERR_DBENGINE_INTER_SET_CHARSET            ERR_DBENGINE_INTER_BGN + 3 //fail to set CharSet

#define   ERR_DBENGINE_INTER_CONNECT_FAILED			ERR_DBENGINE_INTER_BGN + 5 //fail to connect server
#define   ERR_DBENGINE_INTER_DISCONNECT_FAILED		ERR_DBENGINE_INTER_BGN + 6 //fail to disconnect server
#define   ERR_DBENGINE_INTER_EXECSQL_FAILED			ERR_DBENGINE_INTER_BGN + 7 //fail to execute sql

#define   ERR_DBENGINE_INTER_NO_RESULTSET			ERR_DBENGINE_INTER_BGN + 20//fail to find recordset
#define   ERR_DBENGINE_INTER_NO_ENOUGH_RESULTSET	ERR_DBENGINE_INTER_BGN + 21//fail to find enough recordset
#define   ERR_DBENGINE_INTER_NO_ASSIGN_NO_FAILED	ERR_DBENGINE_INTER_BGN + 22//fail to assign new NO



#endif // _DBENGINE_ERR_CODE_H_
//END OF FILE
