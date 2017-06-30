/*****************************************************************************
ģ����      : mpcd
�ļ���      : dbopr.cpp
����ʱ��    : 2012�� 02�� 01��
ʵ�ֹ���    : 
����        : ���Ĳ�
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/02/01  KDVP2.5     ���Ĳ�        ����
******************************************************************************/
#ifdef WIN32
#pragma warning( disable:4786 )
#endif

#include "inc.h"
#include "osp.h"
#include "dbopr.h"

/*=============================================================================
�� �� ���� TDBConnectInfo
��    �ܣ� ���ݿ�������Ϣ���캯��
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
//lint -sem(TDBConnectInfo::SetNull,initializer)
TDBConnectInfo::TDBConnectInfo()
{
	SetNull();
}


/*=============================================================================
�� �� ���� ~TDBConnectInfo
��    �ܣ� ���ݿ�������Ϣ��������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
TDBConnectInfo::~TDBConnectInfo() {}


/*=============================================================================
�� �� ���� SetNull
��    �ܣ� ������ݿ�ṹ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBConnectInfo::SetNull( void )
{
	memset( m_achDBName, 0, sizeof(m_achDBName) );
	memset( m_achDBIP, 0, sizeof(m_achDBIP) );
	memset( m_achDBUserName, 0, sizeof(m_achDBUserName) );
	memset( m_achDBUserPwd, 0, sizeof(m_achDBUserPwd) );
	m_wDBPort = 0;
	m_byDBEngineType = 0;
	m_byReserved = 0;
}

/*=============================================================================
�� �� ���� SetDBName
��    �ܣ� �������ݿ�����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchDBName		���ݿ�����
		   u16 wNameLen				���ݿ����Ƴ���
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBConnectInfo::SetDBName( LPCSTR pchDBName, u16 wNameLen )
{
	if( NULL == pchDBName || 0 == wNameLen )
	{
		return ;
	}

	u16 wDBNameLen = wNameLen > MAXLEN_DB_NAME ? MAXLEN_DB_NAME : wNameLen;
	memcpy( m_achDBName, pchDBName, wDBNameLen );

	return ;
}

/*=============================================================================
�� �� ���� GetDBName
��    �ܣ� ��ȡ���ݿ�����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� LPCSTR
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
LPCSTR TDBConnectInfo::GetDBName( void ) const
{
	return m_achDBName;
}

/*=============================================================================
�� �� ���� GetDBNameLen
��    �ܣ� ��ȡ���ݿ����Ƴ���
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u16
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u16 TDBConnectInfo::GetDBNameLen( void ) const
{
	return strlen(m_achDBName);
}

/*=============================================================================
�� �� ���� SetDBIp
��    �ܣ� �������ݿ�IP��ַ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchDBIp		���ݿ�IP��ַ
		   u16 wIpLen				���ݿ�IP��ַ����
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBConnectInfo::SetDBIp( LPCSTR pchDBIp, u16 wIpLen )
{
	if( NULL == pchDBIp || 0 == wIpLen )
	{
		return ;
	}

	u16 wDBIpLen = wIpLen > MAXLEN_DB_IPADDR ? MAXLEN_DB_IPADDR : wIpLen;
	memcpy( m_achDBIP, pchDBIp, wDBIpLen );

	return ;
}

/*=============================================================================
�� �� ���� GetDBIp
��    �ܣ� ��ȡ���ݿ�IP��ַ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� LPCSTR
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
LPCSTR TDBConnectInfo::GetDBIp( void ) const
{
	return m_achDBIP;
}

/*=============================================================================
�� �� ���� GetDBIpLen
��    �ܣ� ��ȡ���ݿ�IP��ַ����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u16
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u16 TDBConnectInfo::GetDBIpLen( void ) const
{
	return strlen(m_achDBIP);
}


/*=============================================================================
�� �� ���� SetDBUserName
��    �ܣ� �������ݿ��û���
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchDBUserName		���ݿ��û���
		   u16 wUserNameLen			���ݿ��û�����
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBConnectInfo::SetDBUserName( LPCSTR pchDBUserName, u16 wUserNameLen )
{
	if( NULL == pchDBUserName || 0 == wUserNameLen )
	{
		return ;
	}

	u16 wDBUserNameLen = 0;
	wDBUserNameLen = wUserNameLen > MAXLEN_DB_USERNAME ? MAXLEN_DB_USERNAME : wUserNameLen;
	memcpy( m_achDBUserName, pchDBUserName, wDBUserNameLen );

	return ;
}

/*=============================================================================
�� �� ���� GetDBUserName
��    �ܣ� ��ȡ���ݿ��û���
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� LPCSTR
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
LPCSTR TDBConnectInfo::GetDBUserName( void ) const
{
	return m_achDBUserName;
}

/*=============================================================================
�� �� ���� GetDBIpLen
��    �ܣ� ��ȡ���ݿ��û�������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u16
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u16 TDBConnectInfo::GetDBUserNameLen( void ) const
{
	return strlen(m_achDBUserName);
}


/*=============================================================================
�� �� ���� SetDBUserPwd
��    �ܣ� �������ݿ��û�����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchDBUserPwd		���ݿ��û�����
		   u16 wUserNameLen				���ݿ��û����볤��
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBConnectInfo::SetDBUserPwd( LPCSTR pchDBUserPwd, u16 wUserNameLen )
{
	if( NULL == pchDBUserPwd || 0 == wUserNameLen )
	{
		return ;
	}

	u16 wDBUserPwdLen = wUserNameLen > MAXLEN_DB_USERPWD ? MAXLEN_DB_USERPWD : wUserNameLen;
	memcpy( m_achDBUserPwd, pchDBUserPwd, wDBUserPwdLen );

	return ;
}

/*=============================================================================
�� �� ���� GetDBUserName
��    �ܣ� ��ȡ���ݿ��û�����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� LPCSTR
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
LPCSTR TDBConnectInfo::GetDBUserPwd( void ) const
{
	return m_achDBUserPwd;
}

/*=============================================================================
�� �� ���� GetDBUserPwdLen
��    �ܣ� ��ȡ���ݿ��û����볤��
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u16
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u16 TDBConnectInfo::GetDBUserPwdLen( void ) const
{
	return strlen(m_achDBUserPwd);
}


/*=============================================================================
�� �� ���� SetDBPort
��    �ܣ� �������ݿ�˿�
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u16			���ݿ�˿�
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBConnectInfo::SetDBPort( u16 wDBPort )
{
	m_wDBPort = wDBPort;
}

/*=============================================================================
�� �� ���� GetDBPort
��    �ܣ� ��ȡ���ݿ�˿�
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u16
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u16 TDBConnectInfo::GetDBPort( void ) const
{
	return m_wDBPort;
}

/*=============================================================================
�� �� ���� SetDBEngineType
��    �ܣ� �������ݿ�����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u16			���ݿ�˿�
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBConnectInfo::SetDBEngineType( u8 byEngineType )
{
	m_byDBEngineType = byEngineType;
}

/*=============================================================================
�� �� ���� GetDBEngineType
��    �ܣ� ��ȡ���ݿ�����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u16
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/29  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u8 TDBConnectInfo::GetDBEngineType( void ) const
{
	return m_byDBEngineType;
}


/*=============================================================================
�� �� ���� CResultSet
��    �ܣ� ���ݿ��������캯��
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/28  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
CResultSet::CResultSet()
{

}


/*=============================================================================
�� �� ���� ~CResultSet
��    �ܣ� ���ݿ�������������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ��  
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/28  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
CResultSet::~CResultSet()
{

}


/*=============================================================================
�� �� ���� MoveNext
��    �ܣ� �ƶ�����һ����¼
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ�� TRUE		�����һ����¼
		   FALSE	δ�����һ����¼
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/28  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 CResultSet::MoveNext( void )
{
	return m_cDBRs.MoveNext();
}


/*=============================================================================
�� �� ���� MoveNNext
��    �ܣ� �ƶ���ָ��λ�õļ�¼
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u32 dwNStep         �ƶ��Ĳ���
           u32 &dwRealStep     ʵ���ƶ��Ĳ���
�� �� ֵ�� TRUE		�����һ����¼
		   FALSE	δ�����һ����¼
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/28  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 CResultSet::MoveNNext( u32 dwNStep, u32 &dwRealStep )
{
	dwRealStep = 0;
	if( dwNStep > 0 )
	{
		while( (dwNStep > 0) && (!m_cDBRs.IsEof()) )
		{
			m_cDBRs.MoveNext();
			dwNStep--;
			dwRealStep++;
		}
	}

	return ( !m_cDBRs.IsEof() );
}


/*=============================================================================
�� �� ���� GetFieldCount
��    �ܣ� ��ȡ�����������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u16      ����
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/28  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u16 CResultSet::GetFieldCount( void )
{
	return m_cDBRs.GetFieldCount();
}

/*=============================================================================
�� �� ���� IsEof
��    �ܣ� �Ƿ����һ����¼�����жϼ�¼�Ƿ����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/28  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 CResultSet::IsEof( void )
{
	return m_cDBRs.IsEof();
}

/*=============================================================================
�� �� ���� CDBEngine
��    �ܣ� ���ݿ�������湹�캯��
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/28  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
CDBEngine::CDBEngine( void )
{
	m_pcDBOprer = NULL;
	m_pcDBResultSet = NULL;
	m_bConnectDB = FALSE;
	m_pServerCbFunc = NULL;
	m_pClientCbFunc = NULL;
	m_dwUserData = 0;
	m_byDBEngineType = 0;
}


/*=============================================================================
�� �� ���� ~CDBEngine
��    �ܣ� ���ݿ����������������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� 
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/28  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
CDBEngine::~CDBEngine( void )
{
	DisconnectDB();
	if( NULL != m_pcDBOprer )
	{
		delete m_pcDBOprer;
		m_pcDBOprer = NULL;
	}

	if( NULL != m_pcDBResultSet )
	{
		delete m_pcDBResultSet;
		m_pcDBResultSet = NULL;
	}
}


/*=============================================================================
�� �� ���� SetCallBackUserData
��    �ܣ� ����û����ӵĻص�����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� DBCbFunc pServerCbFunc
		   DBCbFunc pClientCbFunc
		   u32 dwUserData
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void CDBEngine::SetCallBackUserData( DBCbFunc pServerCbFunc, DBCbFunc pClientCbFunc, u32 dwUserData )
{
	m_dwUserData = dwUserData;
	m_pServerCbFunc = pServerCbFunc;
	m_pClientCbFunc = pClientCbFunc;
}


/*=============================================================================
�� �� ���� ConnectDB
��    �ܣ� ������Ӧ����,���������ݿ�
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� const TDBConnectInfo * ptDBConnectInfo
�� �� ֵ�� u16	������
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u16	CDBEngine::ConnectDB( const TDBConnectInfo * ptDBConnectInfo )
{
	if( IsConnectDB() )
	{
		return ERR_DB_SUCCESS;
	}

	if( NULL == ptDBConnectInfo )
	{
		return ERR_DB_INVALID_PARAM;
	}

	if( NULL == m_pcDBOprer )
	{
		m_pcDBOprer = new CDBHandler;
		if( NULL == m_pcDBOprer )
		{
			return ERR_DB_ALLOC_MEMORY;
		}
	}

	//FIXME: m_pcDBOprer �� m_pcDBResultSet ʲôɾ��
	if( NULL == m_pcDBResultSet )
	{
		m_pcDBResultSet = new CResultSet;
		if( NULL == m_pcDBResultSet )
		{
			return ERR_DB_ALLOC_MEMORY;
		}
	}

	
	BOOL32 bRet = FALSE;
	bRet = m_pcDBOprer->SetupSrvErrFuc( m_pServerCbFunc, (void*)m_dwUserData );
	if( !bRet )
	{
		return ERR_DB_SET_CBDATA;
	}

	bRet = m_pcDBOprer->SetupCtErrFuc( m_pClientCbFunc, (void*)m_dwUserData );
	if( !bRet )
	{
		return ERR_DB_SET_CBDATA;
	}


	bRet = m_pcDBOprer->ConnecttoSrv( ptDBConnectInfo->GetDBUserName(),
		                              ptDBConnectInfo->GetDBUserPwd(),
									  ptDBConnectInfo->GetDBName(),
									  ptDBConnectInfo->GetDBIp(),
									  ptDBConnectInfo->GetDBPort() );
	if( !bRet )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	m_bConnectDB = TRUE;
	m_byDBEngineType = ptDBConnectInfo->GetDBEngineType();

	return ERR_DB_SUCCESS;
}


/*=============================================================================
�� �� ���� DisconnectDB
��    �ܣ� �Ͽ����ݿ�
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u16	������
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u16	CDBEngine::DisconnectDB( void )
{
	u16 wRet = ERR_DB_SUCCESS;

	if( IsConnectDB() )
	{
		BOOL32 bRet = m_pcDBOprer->CloseConnection();
		wRet = (TRUE == bRet) ? ERR_DB_SUCCESS : ERR_DB_DISCONNECT_FAILED ;
	}

	//����Ϊ�Ͽ�״̬
	m_bConnectDB = FALSE;
	m_byDBEngineType = 0;

	return wRet ;
}


/*=============================================================================
�� �� ���� IsConnectDB
��    �ܣ� �ж��Ƿ����������ݿ�
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 CDBEngine::IsConnectDB( void )
{
	if( NULL == m_pcDBOprer || NULL == m_pcDBResultSet )
	{
		return FALSE;
	}

	return m_bConnectDB;
}

/*=============================================================================
�� �� ���� GetDBEngineType
��    �ܣ� ��ȡ���ݿ���������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u8
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u8 CDBEngine::GetDBEngineType( void ) const
{
	return m_byDBEngineType;
}

/*=============================================================================
�� �� ���� GetDBUserData
��    �ܣ� ��ȡ���ݿ�����û�����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetDBUserData( void ) const
{
	return m_dwUserData;
}

/*=============================================================================
�� �� ���� ExecSql
��    �ܣ� ִ��Sql���
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchSql		SQL���
		   BOOL32 bNeedRet		�Ƿ���Ҫ���ؽ����
�� �� ֵ�� u32					������
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32	CDBEngine::ExecSql( LPCSTR pchSql, BOOL32 bNeedRet )
{
	if( NULL == pchSql )
	{
		return ERR_DB_INVALID_PARAM;
	}

	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	u16 wRet = DB_EXEC_ERRID_FAIL;
	if ( TRUE == bNeedRet )
	{
		wRet = m_pcDBOprer->ExecSql( pchSql, &m_pcDBResultSet->m_cDBRs );
		switch ( wRet )
		{
		case DB_EXEC_ERRID_SUCC:
			{
				//Ų����һ��
				m_pcDBResultSet->MoveNext();
			}
			break;
		case DB_EXEC_ERRID_FAIL:
		case DB_EXEC_ERRID_LOST_CON:
			// something bad happened!
			break;
		default:
			// should not be here!
			break;
		}
	}
	else
	{
		wRet = m_pcDBOprer->ExecSql( pchSql );
	}

	if ( DB_EXEC_ERRID_SUCC != wRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� MoveNext
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 CDBEngine::MoveNext( void )
{
	if( !IsConnectDB() )
	{
		return FALSE;
	}

	return m_pcDBResultSet->MoveNext();
}

/*=============================================================================
�� �� ���� GetFiledCount
��    �ܣ� ��ȡ��������ֶ�����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u16
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFiledCount( void )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_NO_RESULESET;
	}

	return m_pcDBResultSet->GetFieldCount();
}

/*=============================================================================
�� �� ���� MoveNNext
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 CDBEngine::MoveNNext( u32 dwNStep, BOOL32 bGoOnProcRes )
{
	if( !IsConnectDB() )
	{
		return FALSE;
	}

	if( 0 == dwNStep )
	{
		return FALSE;
	}

	BOOL32 bRet = FALSE;
	u32	dwRealStep = 0;
	u32 dwNextNStep = 0;
	bRet = m_pcDBResultSet->MoveNNext( dwNStep, dwRealStep );
	//�������ݼ����ܰ�������,ǰһ�����ݼ���ת���,������ת��һ�����ݼ�
	if( FALSE == bRet && TRUE == bGoOnProcRes )
	{
		if( TRUE == IsEof() && ERR_DB_SUCCESS != GoOnProcRes() )
		{
			return bRet ;
		}

		if( dwNStep >= dwRealStep )
		{
			dwNextNStep = dwNStep - dwRealStep;
			bRet = m_pcDBResultSet->MoveNNext( dwNextNStep, dwNextNStep );
		}
	}

	return bRet;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchName
		   s32 &nValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, s32 &nValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, nValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchName
		   u32 &dwValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, u32 &dwValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, dwValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchName
		   s16 &sValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, s16 &sValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, sValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchName
		   u16 &wValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, u16 &wValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, wValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchName
		   s8 &chValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, s8 &chValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, chValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchName
		   u8 &byValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, u8 &byValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, byValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchName
		   LPSTR pchStr
		   u16 wSize
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, LPSTR pchStr, u16 wSize )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, pchStr, wSize );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchName
		   void* pImageBuf
		   u16 wImageBufLen
		   u16 &wImageOutLen
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( LPCSTR pchName, void* pImageBuf, u16 wImageBufLen, u16 &wImageOutLen )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( pchName, pImageBuf, wImageBufLen, wImageOutLen );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u8 byIndex
		   s32 &nValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, s32 &nValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, nValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u8 byIndex
		   u32 &dwValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, u32 &dwValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, dwValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u8 byIndex
		   s16 &sValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, s16 &sValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, sValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u8 byIndex
		   u16 &wValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, u16 &wValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, wValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u8 byIndex
		   s8 &chValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, s8 &chValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, chValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u8 byIndex
		   u8 &byValue
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, u8 &byValue )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, byValue );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u8 byIndex
		   LPSTR pStr
		   u16 wSize
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, LPSTR pStr, u16 wSize )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, pStr, wSize );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� GetFieldValue
��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u8 byIndex
		   void *pImageBuf
		   u16 wImageBufLen
		   u16 &wImageOutLen
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 CDBEngine::GetFieldValue( u8 byIndex, void *pImageBuf, u16 wImageBufLen, u16 &wImageOutLen )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_CONNECT_FAILED;
	}

	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue( byIndex, pImageBuf, wImageBufLen, wImageOutLen );
	if( !bRet )
	{
		return ERR_DB_EXECSQL_FAILED;
	}

	return ERR_DB_SUCCESS;
}

/*=============================================================================
�� �� ���� IsEof
��    �ܣ� �Ƿ񵽼�¼β���߼�¼�Ƿ����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32	CDBEngine::IsEof( void )
{
	if( !IsConnectDB() )
	{
		return TRUE;
	}

	return m_pcDBResultSet->IsEof();
}

/*=============================================================================
�� �� ���� GoOnProcRes
��    �ܣ� ������������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u16
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2011/12/30  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u16 CDBEngine::GoOnProcRes( void )
{
	if( !IsConnectDB() )
	{
		return ERR_DB_NO_RESULESET;
	}

	u16	wRet = ERR_DB_SUCCESS;
	EOpCode eRet = m_pcDBOprer->GoOnProcRes();
	if( EOpCode_FindRs != eRet )
	{
		return ERR_DB_NO_RESULESET;
	}
	if( FALSE == m_pcDBResultSet->MoveNext() )
	{
		return ERR_DB_NO_RESULESET;
	}

	return wRet;
}


/*=============================================================================
�� �� ���� TDBOprTaskMsgItem
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
//lint -sem(TDBOprTaskMsgItem::SetNull,initializer)
TDBOprTaskMsgItem::TDBOprTaskMsgItem( )
{
	SetNull();
}

/*=============================================================================
�� �� ���� ~TDBOprTaskMsgItem
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
TDBOprTaskMsgItem::~TDBOprTaskMsgItem( )
{
	SetNull();
}

/*=============================================================================
�� �� ���� SetNull
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBOprTaskMsgItem::SetNull( void )
{
	m_dwSessionID = 0;
	m_dwTransactionID = 0;
	m_dwEventID = 0;
	m_dwMsgLen = 0;
	m_bDurable = FALSE;
	memset( m_abyReqMsgBuf, 0, sizeof(m_abyReqMsgBuf) );
}

/*=============================================================================
�� �� ���� SetTransactionID
��    �ܣ� ��������ID
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u32		����ID(������)
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBOprTaskMsgItem::SetTransactionID( u32 dwTransactionID )
{
	m_dwTransactionID = htonl( dwTransactionID );
}

/*=============================================================================
�� �� ���� GetTransactionID
��    �ܣ� ��ȡ����ID
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 TDBOprTaskMsgItem::GetTransactionID( void ) const
{
	return ntohl( m_dwTransactionID );
}

/*=============================================================================
�� �� ���� SetSessionID
��    �ܣ� ���ûỰID
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u32
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBOprTaskMsgItem::SetSessionID( u32 dwSessionID )
{
	m_dwSessionID = htonl( dwSessionID );
}

/*=============================================================================
�� �� ���� GetSessionID
��    �ܣ� ���ûỰID
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 TDBOprTaskMsgItem::GetSessionID( void ) const 
{
	return ntohl( m_dwSessionID );
}

/*=============================================================================
�� �� ���� SetEventID
��    �ܣ� ������ϢID
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� u32
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBOprTaskMsgItem::SetEventID( u32 dwEventID )
{
	m_dwEventID = htonl( dwEventID );
}

/*=============================================================================
�� �� ���� GetEventID
��    �ܣ� ���ûỰID
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 TDBOprTaskMsgItem::GetEventID( void ) const 
{
	return ntohl( m_dwEventID );
}

void TDBOprTaskMsgItem::SetDurable(BOOL32 bDurable)
{
	m_bDurable = bDurable;
}

BOOL32 TDBOprTaskMsgItem::IsDurable()
{
	return m_bDurable;
}

/*=============================================================================
�� �� ���� SetRecordTime
��    �ܣ� ��������ʱ��
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� const TKdvTime &tRecordTime
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBOprTaskMsgItem::SetRecordTime( const TKdvTime &tRecordTime )
{
	m_tRecordTime = tRecordTime;
}

/*=============================================================================
�� �� ���� GetRecordTime
��    �ܣ� ��ȡ����ʱ��
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� TKdvTime
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
TKdvTime TDBOprTaskMsgItem::GetRecordTime( void ) const
{
	return m_tRecordTime;
}

/*=============================================================================
�� �� ���� SetMsgBody
��    �ܣ� ��������ʱ��
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� const u8* pbyMsg
		   u32 wMsgLen
�� �� ֵ�� BOOL32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 TDBOprTaskMsgItem::SetMsgBody( const void* pbyMsg, u32 wMsgLen )
{
	if( NULL == pbyMsg || 0 >= wMsgLen || wMsgLen > SERV_MSG_LEN )
	{
		return FALSE;
	}

	memcpy( m_abyReqMsgBuf, pbyMsg, wMsgLen );
	m_dwMsgLen = wMsgLen;

	return TRUE;
}

BOOL32 TDBOprTaskMsgItem::CatMsgBody(const void* pbyMsg, u32 wMsgLen)
{
	if (NULL == pbyMsg || 0 >= wMsgLen || wMsgLen > SERV_MSG_LEN - m_dwMsgLen)
	{
		return FALSE;
	}

	memcpy(m_abyReqMsgBuf + m_dwMsgLen, pbyMsg, wMsgLen);
	m_dwMsgLen += wMsgLen;

	return TRUE;
}

/*=============================================================================
�� �� ���� GetMsgBodyLen
��    �ܣ� ��ȡ������Ϣ����
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 TDBOprTaskMsgItem::GetMsgBodyLen( void ) const
{
	return m_dwMsgLen;
}

/*=============================================================================
�� �� ���� GetMsgBody
��    �ܣ� ��ȡ������Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u32
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
const u8* TDBOprTaskMsgItem::GetMsgBody( void ) const
{
	return m_abyReqMsgBuf;
}

/*=============================================================================
�� �� ���� TDBOprTaskInfo
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
//lint -sem(TDBOprTaskInfo::SetNull,initializer)
TDBOprTaskInfo::TDBOprTaskInfo( void )
{
	SetNull();
}

/*=============================================================================
�� �� ���� ~TDBOprTaskInfo
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
TDBOprTaskInfo::~TDBOprTaskInfo(void) {}

/*=============================================================================
�� �� ���� SetNull
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBOprTaskInfo::SetNull( void )
{
	m_byDBConState = 0;
	m_byCancelTransaction = 0;
	m_byTerminateTask = 0;
	m_pData = NULL;
	m_hTaskID = 0;
	m_bIsIdle = TRUE;
}

/*=============================================================================
�� �� ���� SetDBConState
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBOprTaskInfo::SetDBConState( BOOL32 bDBConOK )
{
	m_byDBConState = (TRUE == bDBConOK) ? 1 : 0;
}

/*=============================================================================
�� �� ���� IsDBConOK
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 TDBOprTaskInfo::IsDBConOK( void ) const
{
	return ( 1 == m_byDBConState ) ? TRUE : FALSE;
}

/*=============================================================================
�� �� ���� SetTerminateTask
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� BOOL32 bTerminate
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBOprTaskInfo::SetTerminateTask( BOOL32 bTerminate )
{
	m_byTerminateTask = ( TRUE == bTerminate ) ? 1 : 0;
}

/*=============================================================================
�� �� ���� IsTerminateTask
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 TDBOprTaskInfo::IsTerminateTask( void ) const
{
	return ( 1 == m_byTerminateTask ) ? TRUE : FALSE ;
}


void TDBOprTaskInfo::SetUserData(void* pData)
{
	m_pData = pData;
}

void* TDBOprTaskInfo::GetUserData(void)
{
	return m_pData;
}

/*=============================================================================
�� �� ���� SetTaskID
��    �ܣ� ������ϢID
�㷨ʵ�֣�
ȫ�ֱ�����
��    ���� u32
�� �� ֵ�� void
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBOprTaskInfo::SetTaskID(TASKHANDLE hTaskID)
{
	m_hTaskID = hTaskID;
}

/*=============================================================================
�� �� ���� GetTaskID
��    �ܣ� ��ȡ�߳�����ID
�㷨ʵ�֣�
ȫ�ֱ�����
��    ���� void
�� �� ֵ�� TASKHANDLE
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
TASKHANDLE TDBOprTaskInfo::GetTaskID(void)
{
	return m_hTaskID;
}

void TDBOprTaskInfo::SetIsIdle(BOOL32 bIdle)
{
	m_bIsIdle = bIdle;
}

BOOL32 TDBOprTaskInfo::IsIdle() const
{
	return m_bIsIdle;
}

/*=============================================================================
�� �� ���� TDBMsgQueue
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
TDBMsgQueue::TDBMsgQueue( u32 dwMaxSize )
{
	m_dwMaxSize = dwMaxSize;
	m_dwLostMsg = 0;
	m_hSemQueueLock = NULL;
}

/*=============================================================================
�� �� ���� ~TDBMsgQueue
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
//lint -sem(TDBMsgQueue::Clear,cleanup)
TDBMsgQueue::~TDBMsgQueue( )
{
	Clear();
}

/*=============================================================================
�� �� ���� SetTaskSem
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/06/18  KDVP2.5.2	����                  ����
=============================================================================*/
BOOL32 TDBMsgQueue::SetQueueSem( SEMHANDLE hSemTaskLock )
{
	if( NULL == hSemTaskLock )
	{
		return FALSE;
	}
	
	m_hSemQueueLock = hSemTaskLock;
	return TRUE;
}

/*=============================================================================
�� �� ���� GetTaskSem
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2013/06/18  KDVP2.5.2	����                  ����
=============================================================================*/
SEMHANDLE TDBMsgQueue::GetQueueSem( void ) const
{
	return m_hSemQueueLock;
}


/*=============================================================================
�� �� ���� PushBackOneMsg
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 TDBMsgQueue::PushBackOneMsg( const TDBOprTaskMsgItem& tDBMsg )
{
	BOOL32 bRet = FALSE;
	OspSemTake(m_hSemQueueLock);

	if( m_dwMaxSize > GetCurMsgNum() )
	{
		m_cDBMsgQueue.push( tDBMsg );
		bRet =  TRUE;
	}

	OspSemGive(m_hSemQueueLock);
	return bRet;
}

/*=============================================================================
�� �� ���� PopFrontOneMsg
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 TDBMsgQueue::PopFrontOneMsg(TDBOprTaskMsgItem* ptDBMsg)
{
	BOOL32 bRet = TRUE;
	OspSemTake(m_hSemQueueLock);

	if( TRUE == IsEmpty() )
	{
		bRet = FALSE;
	}
	else
	{
		if (ptDBMsg)
		{
			*ptDBMsg = m_cDBMsgQueue.front();
		}
		m_cDBMsgQueue.pop();
	}
	OspSemGive(m_hSemQueueLock);
	return bRet;
}

/*=============================================================================
�� �� ���� GetFirstMsg
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
TDBOprTaskMsgItem TDBMsgQueue::GetFirstMsg( void )
{
	OspSemTake(m_hSemQueueLock);
	TDBOprTaskMsgItem& tDBMsgItem = m_cDBMsgQueue.front();
	OspSemGive(m_hSemQueueLock);
	return tDBMsgItem;
}

/*=============================================================================
�� �� ���� GetNextMsg
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
TDBOprTaskMsgItem TDBMsgQueue::GetNextMsg( u32 dwPos )
{
	if( 1 == dwPos )
	{
		return ( TDBOprTaskMsgItem )m_cDBMsgQueue.front();
	}
	return (TDBOprTaskMsgItem)m_cDBMsgQueue.front();
}

/*=============================================================================
�� �� ���� RemoveOneMsg
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 TDBMsgQueue::RemoveOneMsg( u32 dwPos )
{
	if( 1 == dwPos )
	{
		return PopFrontOneMsg();
	}

	return FALSE;
}

/*=============================================================================
�� �� ���� IsEmpty
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
BOOL32 TDBMsgQueue::IsEmpty( void ) const
{
	return m_cDBMsgQueue.empty();
}

/*=============================================================================
�� �� ���� Clear
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
void TDBMsgQueue::Clear( void )
{
	while( FALSE == IsEmpty() )
	{
		PopFrontOneMsg();
	}
	return ;
}

/*=============================================================================
�� �� ���� GetCurMsgNum
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 TDBMsgQueue::GetCurMsgNum( void ) const
{
	return m_cDBMsgQueue.size();
}

/*=============================================================================
�� �� ���� GetQueueSize
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2012/01/04  KDVP2.5		���Ĳ�                  ����
=============================================================================*/
u32 TDBMsgQueue::GetQueueSize( void ) const
{
	return m_dwMaxSize;
}

/*=============================================================================
�� �� ���� SetCharacterSet
��    �ܣ� ���ñ����ַ���
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� LPCSTR pchCharSet
�� �� ֵ�� BOOL32	�����Ƿ�ɹ�
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2015/03/31  5.0			����                  ����
=============================================================================*/
BOOL32	CDBEngine::SetCharacterSet( LPCSTR pchCharSet )
{
	if( NULL == pchCharSet )
	{
		return FALSE;
	}

	if( !IsConnectDB() )
	{
		return FALSE;
	}

	return m_pcDBOprer->SetCharacterSet( pchCharSet );
}

u64 CDBEngine::EscapeString( LPSTR lpTo,LPCSTR lpFrom,BOOL32 isHandle/*=FALSE*/,u64 nLen/*=0*/ )
{
	if (NULL == m_pcDBOprer)
	{
		return 0;
	}

	return m_pcDBOprer->EscapeStr(lpTo, lpFrom, isHandle, nLen);
}


