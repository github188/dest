/*****************************************************************************
ģ����      : ���ݿ�
�ļ���      : dbopr.h
����ʱ��    : 2012�� 02�� 01��
ʵ�ֹ���    : 
����        : ���Ĳ�
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/02/01  KDVP2.5     ���Ĳ�        ����
******************************************************************************/
#ifndef	_DB_OPR_
#define	_DB_OPR_

#include "vccommon.h"
#include "mcuconst.h"
#include "dbinterface.h"

#include <queue>

#define		MAXLEN_DB_NAME			(u8)64
#define		MAXLEN_DB_IPADDR		(u8)20
#define		MAXLEN_DB_USERNAME		(u8)64
#define		MAXLEN_DB_USERPWD		(u8)32

#define		ERR_DB_BGN					(u32)0
#define		ERR_DB_SUCCESS				ERR_DB_BGN
#define		ERR_DB_INVALID_PARAM		ERR_DB_BGN + 1
#define		ERR_DB_ALLOC_MEMORY			ERR_DB_BGN + 2
#define		ERR_DB_SET_CBDATA			ERR_DB_BGN + 3
#define		ERR_DB_CONNECT_FAILED		ERR_DB_BGN + 4
#define		ERR_DB_DISCONNECT_FAILED	ERR_DB_BGN + 5
#define		ERR_DB_EXECSQL_FAILED		ERR_DB_BGN + 6
#define		ERR_DB_NO_RESULESET			ERR_DB_BGN + 7
#define		ERR_DB_NO_ENOUGH_RESULTSET	ERR_DB_BGN + 8
#define		ERR_DB_NO_ASSIGN_NO_FAILED	ERR_DB_BGN + 9

#define		DBOPRTASK_PRIORITY		(u8)(90)
#define		DBOPRTASK_STACKSIZE		(u32)(512 * 1024)

//����Ƿ�����Ϣ�ȴ�����ĳ�ʱ���(��λ������)
#define		DB_CHECKDBTASK_SPAN		(u32)(10000)	//10��
//���ݿ����Ӽ��Ķ�ʱ��Ƶ��(��λ������)
#define		DB_CHECKDBCONNECT_SPAN	(u32)(120*1000)	//120��
//���ݿ����Ӽ��ļ������
#define		DB_CHECKDBCONNECT_TIMES	(u32)(DB_CHECKDBCONNECT_SPAN/DB_CHECKDBTASK_SPAN)
//
#define		DB_TIMEOUT_DEFAULTSPAN	(u8)20

//���ݿ�����ʱʱ��(10����)
#define	DBREQ_TIMEROUT_SPAN		(u32)10*60

#define	DEFAULT_DBTHREAD_NUM	(u32)5
#define	DEFAULT_DBQUEUEMSG_NUM	(u32)1000


//���ݿ�������Ϣ
struct TDBConnectInfo
{
public:
	TDBConnectInfo();
	~TDBConnectInfo();

public:
	void SetNull();

	void SetDBName( LPCSTR pchDBName, u16 wNameLen );
	LPCSTR  GetDBName( void ) const;
	u16	 GetDBNameLen( void ) const;

	void SetDBIp( LPCSTR pchDBIp, u16 wIpLen );
	LPCSTR	 GetDBIp( void ) const;
	u16  GetDBIpLen( void ) const;

	void SetDBUserName( LPCSTR pchDBUserName, u16 wUserNameLen );
	LPCSTR	 GetDBUserName( void ) const;
	u16  GetDBUserNameLen( void ) const;

	void SetDBUserPwd( LPCSTR pchDBUserPwd, u16 wUserNameLen );
	LPCSTR	 GetDBUserPwd( void ) const;
	u16  GetDBUserPwdLen( void ) const;

	void SetDBPort( u16 wPort );
	u16  GetDBPort( void ) const;

	void SetDBEngineType( u8 byEngineType );
	u8   GetDBEngineType( void ) const;

private:
	s8	m_achDBName[MAXLEN_DB_NAME+1];			//���ݿ������
	s8	m_achDBIP[MAXLEN_DB_IPADDR+1];			//���ݿ����ӵ�IP
	s8	m_achDBUserName[MAXLEN_DB_USERNAME+1];	//���ݿ����ӵ��û���
	s8	m_achDBUserPwd[MAXLEN_DB_USERPWD+1];		//���ݿ����ӵ�����
	u16 m_wDBPort;								//���ݿ����ӵĶ˿�
	u8	m_byDBEngineType;						//���ݿ������
	u8	m_byReserved;
};

//���ݿ���������
class CResultSet
{
public:
	CResultSet();
	~CResultSet();


public:
	BOOL32	MoveNext( void );
	BOOL32	MoveNNext( u32 dwNStep, u32 &dwRealStep );
	u16		GetFieldCount( void );
	BOOL32	IsEof( void );

public:
	CDBRs	m_cDBRs;
};


//���ݿ��������
class CDBEngine
{
public:
	CDBEngine();
	virtual ~CDBEngine();

public:
	void	SetCallBackUserData( DBCbFunc pServerCbFunc, DBCbFunc pClientCbFunc, u32 dwUserData );
	u16		ConnectDB( const TDBConnectInfo * ptDBConnectInfo );
	u16		DisconnectDB( void );
	BOOL32	IsConnectDB( void );

	u8 GetDBEngineType( void )	const;
	u32	GetDBUserData( void ) const;

	u32	ExecSql( LPCSTR pchSql, BOOL32 bNeedRet = FALSE );
	u64 EscapeString(LPSTR lpTo,LPCSTR lpFrom,BOOL32 isHandle=FALSE,u64 nLen=0);

	BOOL32	MoveNext( void );
	BOOL32	MoveNNext( u32 dwNStep, BOOL32 bGoOnProcRes = FALSE );
	u32		GetFiledCount( void );
	u32		GetFieldValue( LPCSTR pchName, s32 &nValue );
	u32		GetFieldValue( LPCSTR pchName, u32 &dwValue );
	u32		GetFieldValue( LPCSTR pchName, s16 &sValue );
	u32		GetFieldValue( LPCSTR pchName, u16 &wValue );
	u32		GetFieldValue( LPCSTR pchName, s8  &chValue );
	u32		GetFieldValue( LPCSTR pchName, u8  &byValue );
	u32		GetFieldValue( LPCSTR pchName, LPSTR pchStr, u16 wSize );
	u32		GetFieldValue( LPCSTR pchName, void* pImageBuf, u16 wImageBufLen, u16 &wImageOutLen );

	u32		GetFieldValue( u8 byIndex, s32 &nValue );
	u32		GetFieldValue( u8 byIndex, u32 &dwValue );
	u32		GetFieldValue( u8 byIndex, s16 &sValue );
	u32		GetFieldValue( u8 byIndex, u16 &wValue );
	u32		GetFieldValue( u8 byIndex, s8  &chValue );
	u32		GetFieldValue( u8 byIndex, u8  &byValue );
	u32		GetFieldValue( u8 byIndex, LPSTR pStr, u16 wSize );
	u32		GetFieldValue( u8 byIndex, void *pImageBuf, u16 wImageBufLen, u16 &wImageOutLen );
	u16		GoOnProcRes( void );
	BOOL32	IsEof( void );
	BOOL32	SetCharacterSet( LPCSTR pchCharSet );


protected:
	CDBHandler	*m_pcDBOprer;
	CResultSet	*m_pcDBResultSet;
	BOOL32		m_bConnectDB;
	DBCbFunc	m_pServerCbFunc;
	DBCbFunc	m_pClientCbFunc;
	u32			m_dwUserData;
	u8			m_byDBEngineType;
	//FILE		*m_hDataFile;
};

struct TDBOprTaskMsgItem
{
public:
	TDBOprTaskMsgItem();
	~TDBOprTaskMsgItem();

public:
	void SetNull();

	void SetTransactionID( u32 dwTransactionID /*������*/);
	u32	 GetTransactionID( void ) const;

	void SetSessionID( u32 dwSessionID /*������*/);
	u32	 GetSessionID( void ) const;

	void SetEventID( u32 dwEventID /*������*/);
	u32	 GetEventID( void ) const;

	void SetDurable(BOOL32 bDurable);
	BOOL32 IsDurable();

	void SetRecordTime( const TKdvTime &tRecordTime );
	TKdvTime GetRecordTime( void ) const;

	BOOL32 SetMsgBody( const void* pbyMsg, u32 wMsgLen );
	BOOL32 CatMsgBody( const void* pbyMsg, u32 wMsgLen );
	u32  GetMsgBodyLen(void) const;
	const u8* GetMsgBody( void ) const;

protected:
	u32			m_dwSessionID;						//�ỰID(������)
	u32			m_dwTransactionID;					//����ID(������)
	u32			m_dwEventID;						//��ϢID(������)

	TKdvTime	m_tRecordTime;						//������ļ�¼ʱ��
	u32			m_dwMsgLen;							//���������
	u8			m_abyReqMsgBuf[SERV_MSG_LEN];		//�����������Ϣ������
	BOOL32		m_bDurable;                         //�־û�
};


struct	TDBOprTaskInfo
{
public:
	TDBOprTaskInfo();
	~TDBOprTaskInfo();

public:
	void	SetNull( void );

	void	SetDBConState( BOOL32 bDBConOK );
	BOOL32	IsDBConOK( void ) const;

	void	SetTerminateTask( BOOL32 bTerminate );
	BOOL32	IsTerminateTask( void ) const;

	void	SetUserData(void* pData);
	void*	GetUserData(void);

	void SetTaskID(TASKHANDLE hTaskID);
	TASKHANDLE GetTaskID(void);

	void SetIsIdle(BOOL32 bIdle);
	BOOL32 IsIdle() const;

protected:
	u8	m_byDBConState;			//���ݿ�����״̬ 0-δ����
	u8	m_byCancelTransaction;	//������ִ�����Ƿ�ǿ����ֹ
	u8	m_byTerminateTask;		//�߳������Ƿ����ǿ����ֹ
	TASKHANDLE	m_hTaskID;							//��Ϣ����ID
	s32	m_bIsIdle;
	void* m_pData;
};


struct TDBMsgQueue
{
public:
	TDBMsgQueue( u32 dwMaxSize );
	~TDBMsgQueue();

public:
	BOOL32	SetQueueSem( SEMHANDLE hSemQueueLock );
	SEMHANDLE GetQueueSem( void ) const;

	BOOL32	PushBackOneMsg( const TDBOprTaskMsgItem& tDBMsg );
	BOOL32  PopFrontOneMsg(TDBOprTaskMsgItem* ptDBMsg = NULL);
	TDBOprTaskMsgItem GetFirstMsg(void);
	TDBOprTaskMsgItem GetNextMsg( u32 dwPos );
	BOOL32	RemoveOneMsg( u32 dwPos );
	BOOL32	IsEmpty( void ) const;
	void	Clear( void );
	u32		GetCurMsgNum( void ) const;
	u32		GetQueueSize( void ) const;

public:
	u32	m_dwLostMsg;

protected:
	SEMHANDLE m_hSemQueueLock;	//���в����ź���

private:
	u32	m_dwMaxSize;
	std::queue< TDBOprTaskMsgItem > m_cDBMsgQueue;
};

#endif
