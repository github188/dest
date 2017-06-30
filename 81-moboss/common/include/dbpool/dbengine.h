/*****************************************************************************
   模块名      : DBPool
   文件名      : dbengine.h
   相关文件    : dbengine.cpp
   文件实现功能: 数据库操作引擎的封装类的定义
   作者        : 
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人        修改内容
   2006/02/20  1.0         万春雷        创建
******************************************************************************/

#ifndef _DBPOOL_DBENGINE_H_
#define _DBPOOL_DBENGINE_H_

///////////////////////////////////////////////////////////

#include <memory.h>
#include <time.h>
#include "dbinterface.h"
#include "dberrcode.h"
#include "dbpoolconst.h"
#include <string>

using namespace std;

//数据库类型
enum EmDBEngineType
{
	Em_DBEngineType_Sybase		= DBTYPE_SYBASE,		//1
	Em_DBEngineType_Mysql		= DBTYPE_MYSQL,			//2
//	Em_DBEngineType_Oracle		= DBTYPE_ORACLE,		//3
};

//字符集定义
enum EmDBCharSetType
{
	Em_DBCharSetType_ISO1       = 1,
	Em_DBCharSetType_CP850      = 2,
	Em_DBCharSetType_EUCGB      = 3,
	Em_DBCharSetType_GB18030    = 4,
	Em_DBCharSetType_UTF8       = 5,
	Em_DBCharSetType_BIG5       = 6,
};

// 数据库连接信息
class TDBConnectInfo
{
public:
    TDBConnectInfo( void )
    {
        SetNull();
    }
    //置空
    void SetNull()
    {
        memset(m_achDBName, 0, sizeof(m_achDBName));
        memset(m_achDBIP, 0, sizeof(m_achDBIP));
        m_wDBPort = 0;
        memset(m_achDBUserName, 0, sizeof(m_achDBUserName));
        memset(m_achDBUserPwd, 0, sizeof(m_achDBUserPwd));
        m_emDBEngineType = Em_DBEngineType_Mysql;
    }
    
public:
    const s8* GetDBName() const { return m_achDBName; }
    void SetDBName(const s8* szName) 
    {
        if (NULL == szName)
        {
            memset(m_achDBName, 0, sizeof(m_achDBName));
            return;
        }

        strncpy(m_achDBName, szName, sizeof(m_achDBName)-1);
        return; 
    }

    const s8* GetDBIP() const { return m_achDBIP; }
    void SetDBIP(const s8* szDBIp) 
    {
        if (NULL == szDBIp)
        {
            memset(m_achDBIP, 0, sizeof(m_achDBIP));
            return;
        }

        strncpy(m_achDBIP, szDBIp, sizeof(m_achDBIP)-1);
        return; 
    }

    u16 GetDBPort() const { return m_wDBPort; }
    void SetDBPort(u16 wPort) { m_wDBPort = wPort; }

    const s8* GetDBUserName() const { return m_achDBUserName; }
    void SetDBUserName(const s8* szUsername) 
    { 
        if (NULL == szUsername)
        {
            memset(m_achDBUserName, 0, sizeof(m_achDBUserName));
            return;
        }

        strncpy(m_achDBUserName, szUsername, sizeof(m_achDBUserName)-1);
        return; 
    }

    const s8* GetDBUserPwd() const { return m_achDBUserPwd; }
    void SetDBUserPwd(const s8* szPwd)
    { 
        if (NULL == szPwd)
        {
            memset(m_achDBUserPwd, 0, sizeof(m_achDBUserPwd));
            return;
        }

        strncpy(m_achDBUserPwd, szPwd, sizeof(m_achDBUserPwd)-1);
        return; 
    }

    EmDBEngineType GetDBEngineType() const { return m_emDBEngineType; }
    void SetDBEngineType(EmDBEngineType emType) { m_emDBEngineType = emType; }

public:
    s8   m_achDBName[MAXLEN_DB_USER_NAME+1];//数据库名称
    s8   m_achDBIP[MAXLEN_DB_IPADDR+1];   //数据库连接IP字串
    u16  m_wDBPort;       //数据库连接PORTS
    s8   m_achDBUserName[MAXLEN_DB_USER_NAME+1];//数据库连接用户名
    s8   m_achDBUserPwd[MAXLEN_DB_USER_PWD+1];  //数据库连接密码
	EmDBEngineType m_emDBEngineType;		//数据库引擎类型
 };

// 数据库状态
class CDBState
{
public:
    CDBState()
    {
        Init();
    }

    void Init()
    {
        memset(m_szDBName, 0, sizeof(m_szDBName));
        memset(m_szDBIP, 0, sizeof(m_szDBIP));
        m_bConnectState = FALSE;
    }

    const s8* GetDBName() const { return m_szDBName; }
    void SetDBName(const s8* szName) 
    {
        if (NULL == szName)
        {
            memset(m_szDBName, 0, sizeof(m_szDBName));
            return;
        }

        strncpy(m_szDBName, szName, sizeof(m_szDBName)-1);
        m_szDBName[sizeof(m_szDBName)-1] = '\0'; 
    }

    const s8* GetDBIP() const { return m_szDBIP; }
    void SetDBIP(const s8* szIpAddr) 
    { 
        if (NULL == szIpAddr)
        {
            memset(m_szDBIP, 0, sizeof(m_szDBIP));
            return;
        }

        strncpy(m_szDBIP, szIpAddr, sizeof(m_szDBIP)-1);
        m_szDBIP[sizeof(m_szDBIP)-1] = '\0'; 
    }

    BOOL32 GetConnectState() const { return m_bConnectState; }
    void SetConnectState(BOOL32 bState) { m_bConnectState = bState; }

private:
    s8 m_szDBName[MAXLEN_DB_USER_NAME+1];//数据库名称
    s8 m_szDBIP[MAXLEN_DB_IPADDR+1];     //数据库连接IP字串
    BOOL32 m_bConnectState;     //状态
};

//回调函数的定义
#define DBPOOL_SERVERCBPROC DBCbFunc
#define DBPOOL_CLIENTCBPROC DBCbFunc
class CResultSet
{
public:
	CResultSet();
	virtual ~CResultSet();
	
public:
	BOOL32 MoveNext(void);
	BOOL32 MoveNNext(u32 dwNStep, u32 &dwRealStep);
	u16    GetFieldCount();
	//u32    GetCount(void);    //20141124太容易误解了，废弃此接口
	BOOL32 IsEof(void);
	u16    CancelCurRs(void);
	
public:
	CDBRs  m_cDBRs;
};

class CDBEngine
{
public:
	CDBEngine();
	virtual ~CDBEngine();

public:
	void   SetCallBackUserData(DBPOOL_SERVERCBPROC pServerCbFunc, DBPOOL_CLIENTCBPROC pClientCbFunc, u32 dwUserData);
	u16    ConnectDB(const TDBConnectInfo *ptDBConnectInfo, EmDBCharSetType emType);
	u16    DisconnectDB();// 断开连接
	BOOL32 IsConnectDB() const; // 是否连接上数据库

	EmDBEngineType GetDBEngineType( void ) const;
	u32    GetDBUserData( void ) const;

	u16    ExecSql(LPCSTR lpszSql, BOOL32 bNeedRet = FALSE);
	u16    UpdateImageBuf(LPCSTR lpszSql, const void *pImageBuf, u16 wImageBufLen);

	BOOL32 MoveNext();
	BOOL32 MoveNNext(u32 dwNStep, BOOL32 bGoOnProcRes = FALSE);// 移动指定条数
	u16    GetFieldCount();       // 取列数

    //20141124太容易误解了，废弃此接口;可以让dbi层通过mysql_num_rows/mysql_store_result函数来实现
	//u32    GetCount(void);        // 取行数-记录总数     
    //u32    GetMaxID(void);  // 获取查询的id字段最大值    

    u16    GetFieldValue(LPCSTR lpszName, s64 &lwValue);
    u16    GetFieldValue(LPCSTR lpszName, u64 &lwValue);
    u16    GetFieldValue(LPCSTR lpszName, s32 &nValue);
	u16    GetFieldValue(LPCSTR lpszName, u32 &dwValue);
	u16    GetFieldValue(LPCSTR lpszName, s16 &sValue); 
	u16    GetFieldValue(LPCSTR lpszName, u16 &wValue); 
	u16    GetFieldValue(LPCSTR lpszName, s8  &chValue);
	u16    GetFieldValue(LPCSTR lpszName, u8  &byValue);
	u16    GetFieldValue(LPCSTR lpszName, LPSTR pszstr, u16 wSize);
	u16    GetFieldValue(LPCSTR lpszName, void *ImagepBuf, u16 wImageBufLen, u16 &wImageOutLen);

    u16    GetFieldValue(u8 byIndex, s64 &lwValue);
    u16    GetFieldValue(u8 byIndex, u64 &lwValue);
    u16    GetFieldValue(u8 byIndex, s32 &nValue);
	u16    GetFieldValue(u8 byIndex, u32 &dwValue);
	u16    GetFieldValue(u8 byIndex, s16 &sValue);
	u16    GetFieldValue(u8 byIndex, u16 &wValue);
	u16    GetFieldValue(u8 byIndex, s8  &chValue);
	u16    GetFieldValue(u8 byIndex, u8  &byValue);
	u16    GetFieldValue(u8 byIndex, LPSTR pszstr, u16 wSize);
	u16    GetFieldValue(u8 byIndex, void *pImageBuf, u16 wImageBufLen, u16 &wImageOutLen);

	u16    GoOnProcRes();   // 继续处理下一个结果集(用于存储过程中)
	BOOL32 IsEof();         // 是否到记录尾或者是否存在记录
	void   ClearResults();  // 清空结果集

	u32 GetDBHandler(void);
    
    //获取sql执行时的时间点
    u64 GetSqlExecTime() const { return m_dwSqlExecTime; }

	std::string GetDBCharSetType(EmDBCharSetType emType);

private:
	CDBHandler     *m_pcDBOprer;
	CResultSet     *m_pcDBResultSet;
	BOOL32          m_bConnectDB;
	DBCbFunc        m_pServerCbFunc;
	DBCbFunc        m_pClientCbFunc;
	u32             m_dwUserData;       //用户连接的回调数据
	EmDBEngineType  m_emDBEngineType;	//数据库引擎类型

    time_t          m_dwSqlExecTime; //sql执行时的时间点
};

#endif // _DBPOOL_DBENGINE_H_
// END OF FILE


