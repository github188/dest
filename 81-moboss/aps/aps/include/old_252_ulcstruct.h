/*****************************************************************************
ģ����      : UserLoginClientLib
�ļ���      : ulcstruct.h
����ʱ��    : 2011�� 08�� 18��
ʵ�ֹ���    : 
����        : ���Ĳ�
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2011/08/18  1.0         ���Ĳ�        ����
******************************************************************************/
#ifndef _ULC_STRUCT_H_
#define _ULC_STRUCT_H_


#include "osp.h"
#include "kdvsys.h"
#include "kdvtype.h"
#include "old_26_structulc.h"
#include "structulc.h"

#ifdef WIN32

#include <WinSock2.h>
#pragma comment( lib, "ws2_32.lib" ) 
#pragma pack( push )
#pragma pack( 1 )


#ifndef PACKED
#define PACKED
#endif

#endif	/* end WIN32 */



#ifdef _LINUX_

#ifndef PACKED
#define PACKED __attribute__ ((packed)) 
#endif

#endif  /* end _LINUX_ */

#ifdef WIN32
#ifndef vsnprintf
#define vsnprintf   _vsnprintf
#endif
#ifndef snprintf
#define snprintf    _snprintf
#endif
#endif

#define			MAX_ULSLIST_NUM					(u8)10
//#define         MAX_ULSIPLIST_NUM               (u8)255

#define			MAX_MTNAME_LEN					(u16)16
#define			MAX_MTPASSWORD_LEN				(u16)32
#define			MAX_DOMAINNAME_LEN				(u16)64


#define			ULC_CONNECTULS_TASK_PRIORITY	(u8)90
#define			ULC_CONNECTULS_TASK_STACKSIZE	(u32)(100<10)


#define			ERR_CONNECTULS_FAILED		1
#define			ERR_INVALID_E164			2

// wy[2012/08/08] add �ն˵�¼ver����2.5.0.2.0��2.5��SP2�汾��ʼ���԰汾��Ϣ����У�飬��ǰ����
// ע��֮���·��汾����Ver1����ӣ���uls����а汾ʶ��
typedef enum tagEULSVER
{
    emVerBgn = 0,
	emVer1,      // 2.5.0.2.0��2.5��SP2�汾
    emVerEnd
}EUlsVer;
// end

//wy[2012/08/13] add ����������
typedef enum tagESRVTYPE
{
	emSrvTypeBgn = 0,
	emSrvTypeTui,
	emSrvTypeNma,
	emSrvTypeSus,
	emSrvTypeUls,
	emSrvTypeNts,
	emSrvTypeEnd
}ESrvType;
//end// 

//OEM��ʶ����󳤶�
#define MAXLEN_OLD252_SUS_OEM_MARK                     (u16)16

struct TOld252MtLoginInfo
{
protected:
	u16	m_wLoginType;                              // �ն����ͣ�������
	s8	m_achLoginName[MAX_MTNAME_LEN+1];          // �ն˵�¼���ƣ��ն�E164NO
	s8	m_achPassword[MAX_MTPASSWORD_LEN+1];       // �ն˵�¼���룺������
	s8  m_achOemRemark[MAXLEN_OLD252_SUS_OEM_MARK + 1];   // �ն�OEM��ʾ��OEM�汾���ն�ʹ�ã�Ϊ��ȡOEM��ʾ��SUS
    u32 m_dwUlsIP;      //������

public:
	TOld252MtLoginInfo()
	{
		Clear();
	}

	void Clear( void )
	{
		m_wLoginType = 0;
		memset( m_achLoginName, 0, sizeof(m_achLoginName) );
		memset( m_achPassword, 0, sizeof(m_achPassword) );
		memset( m_achOemRemark, 0, sizeof( m_achOemRemark ) );
        m_dwUlsIP = 0;
	}

	void SetLoginType( u16 wType )
	{
		m_wLoginType = wType;
	}
	u16 GetLoginType( void )
	{
		return m_wLoginType;
	}

    const s8* GetPassword( ) const { return m_achPassword; }
	void SetPassword( const s8 * pchPassword )
	{
		memset( m_achPassword, 0, sizeof(m_achPassword) );
		if( NULL != pchPassword )
		{
			memcpy( m_achPassword, pchPassword, sizeof(m_achPassword)-1 );
			m_achPassword[sizeof(m_achPassword)-1] = '\0';
		}
		return ;
	}

    const s8* GetLoginName( ) const { return m_achLoginName; }
	void SetLoginName( const s8 * pchMtLoginName )
	{
		// wy[2012/08/08] add
		SetLoginType( (u16)emVer1 );
		// end

		memset( m_achLoginName, 0, sizeof(m_achLoginName) );
        if (NULL != pchMtLoginName)
        {
            memcpy( m_achLoginName, pchMtLoginName, sizeof(m_achLoginName)-1 );
            m_achLoginName[sizeof(m_achLoginName)-1] = '\0';
        }
        return;
	}

    const s8* GetOemRemark( ) const { return m_achOemRemark; }
	void SetOemRemark( const s8 * pchOemRemark )
	{
		memset( m_achOemRemark, 0, sizeof( m_achOemRemark ) );
		if ( NULL != pchOemRemark )
		{
			memcpy( m_achOemRemark, pchOemRemark, sizeof( m_achOemRemark ) - 1 );
			m_achOemRemark[sizeof(m_achOemRemark) - 1] = '\0';
		}
		return;
	}

    u32 GetUlsIP()/*������*/ const { return m_dwUlsIP; }
    void SetUlsIP(u32 dwUlsIP/*������*/) { m_dwUlsIP = dwUlsIP; }
}
PACKED
;
 
struct TOld252SrvAddrListInfo
{
protected:
	COldSrvAddrInfo m_tGkInfo;		//GK��ַ��Ϣ
	COldSrvAddrInfo m_tNtsInfo;		//NTS��ַ��Ϣ
	COldSrvAddrInfo m_tNmaInfo;		//NMA��ַ��Ϣ
	COldSrvAddrInfo m_tSusInfo;		//SUS��ַ��Ϣ
	
	u8 m_byValidUlsCount;

public:
	COldSrvAddrInfo m_atUlsListInfo[MAX_ULSLIST_NUM];

public:

	TOld252SrvAddrListInfo()
	{
		Clear();
	}

	COldSrvAddrInfo GetGkAddr( ) const { return m_tGkInfo; }
	void SetGkAddr( const COldSrvAddrInfo& tGkInfo )
	{
		m_tGkInfo = tGkInfo;
	}
    void SetGkAddr(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetGkAddr( Convert(tAddrInfo) );
    }

	COldSrvAddrInfo GetNtsAddr( ) const { return m_tNtsInfo; }
	void SetNtsAddr( const COldSrvAddrInfo& tNtsInfo )
	{
        m_tNtsInfo = tNtsInfo;
	}
    void SetNtsAddr(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetNtsAddr( Convert(tAddrInfo) );
    }

	COldSrvAddrInfo GetNmaAddr( ) const { return m_tNmaInfo; }
	void SetNmaAddr( const COldSrvAddrInfo& tNmaInfo )
	{
        m_tNmaInfo = tNmaInfo;
	}
    void SetNmaAddr(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetNmaAddr( Convert(tAddrInfo) );
    }

    //
    COldSrvAddrInfo GetSusAddr( ) { return m_tSusInfo; }
	void SetSusAddr( const COldSrvAddrInfo& tSusInfo )
	{
        m_tSusInfo = tSusInfo;
	}
    void SetSusAddr(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetSusAddr( Convert(tAddrInfo) );
    }

	void SetUlsCount( u8 byUlsCount )
	{
		m_byValidUlsCount = (byUlsCount >= MAX_ULSLIST_NUM) ? MAX_ULSLIST_NUM : byUlsCount ;
	}
	u8  GetUlsCount( void )
	{
		return m_byValidUlsCount;
	}

	void Clear( void )
	{
		m_tGkInfo.Clear();
		m_tNtsInfo.Clear();
		m_tNmaInfo.Clear();
		m_tSusInfo.Clear();

		m_byValidUlsCount = 0;
		memset( m_atUlsListInfo, 0, sizeof(m_atUlsListInfo) );
	}
}
PACKED
;



//////////////////////////////////////////////////////////////////////////////////////////////////
//                      ��mcustruct.h�ļ��� ��ֲCServMsg
//////////////////////////////////////////////////////////////////////////////////////////////////
//����Žṹ (len:16)
#define     MAXLEN_CONFID                   16
//��Ϣ�������
#define      SERV_MSG_LEN					0x7000  //��Ϣ����
#define      SERV_MSGHEAD_LEN				0x30    //��Ϣͷ����

#ifndef getmin
#define getmin(a,b) ((a)>(b)?(b):(a))
#endif
#ifndef getmax
#define getmax(a,b) ((a)>(b)?(a):(b))
#endif

struct CConfId
{
protected:
    u8  	m_abyConfId[MAXLEN_CONFID];

public:
    u8   GetConfId( u8   * pbyConfId, u8   byBufLen ) const; //��û��������
    void CreateConfId( u8 byUsrGrpId );                               //���������
    void GetConfIdString( LPSTR lpszConfId, u8   byBufLen ) const;//��û�����ַ���
    void SetConfId( const CConfId & cConfId );                  //���û����
    u8   SetConfId( const u8   * pbyConfId, u8   byBufLen );          //���û��������
    u8   SetConfId( LPCSTR lpszConfId );                          //���û��������	
    BOOL32 IsNull( void ) const;                                  //�жϻ�����Ƿ�Ϊ��
    void SetNull( void );                                       //���û����Ϊ��
    BOOL32 operator == ( const CConfId & cConfId ) const;         //����������
    // ���񻪣���CConfId�������û������Ϣ
    u8   GetUsrGrpId() const 
    {
        return m_abyConfId[MAXLEN_CONFID-1];
    }
    void SetUsrGrpId(u8 byGrpId)
    {
        m_abyConfId[MAXLEN_CONFID-1] = byGrpId;
    }

    //fuxiuhua,���ӻ��鷢����Ϣ
    //����ID����ɣ�MCU��(2Byte)/��������(1Byte)/��������ֵ(1Byte)/����Ų�������(4Byte)/
    //              δ��(6Byte)/��������(1Byte)/���(1Byte)
    void SetConfSource( u8 byConfSource )
    {
        m_abyConfId[MAXLEN_CONFID - 2] = byConfSource;
    }
    u8   GetConfSource() const
    {
        return m_abyConfId[MAXLEN_CONFID - 2];
    }

    u8 GetConfIdx() const
    {
        return m_abyConfId[3];
    }
}
/*
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
*/
PACKED
    ;

//ҵ����Ϣ�࣬�����28K������Ϣ
class CServMsg
{
protected:
    u16     m_wSerialNO;            //��ˮ��
    u8      m_bySrcDriId;           //ԴDRI���
    u8      m_bySrcMtId;            //Դ�ն˺�
    u16     m_wSrcSsnId;           //Դ�Ự��
    u8      m_byDstDriId;           //Ŀ��DRI���
    u8      m_byDstMtId;            //Ŀ���ն˺�
    u8      m_byMcuId;              //MCU��
    u8      m_byChnIndex;           //ͨ��������
    u32     m_dwConfIdx;            //����������
    CConfId m_cConfId;              //�����
    u16		m_wEventId;             //�¼���
    u16		m_wTimer;               //��ʱ
    u16		m_wErrorCode;           //������
    u16		m_wMsgBodyLen;          //��Ϣ�峤��
    u8      m_byTotalPktNum;        //�ܰ�����������Ҫ�а����͵���Ϣ��
    u8      m_byCurPktIdx;          //��ǰ����������0��ʼ��
    u16     m_wDstSsnId;            //Ŀ�ĻỰ��
    u8      m_abyReserverd[10];     //�����ֽ�
    u8      m_abyMsgBody[SERV_MSG_LEN-SERV_MSGHEAD_LEN];    //��Ϣ��
public:
    void  SetSerialNO(u16  wSerialNO){ m_wSerialNO = htons(wSerialNO);} 
    u16   GetSerialNO( void ) const { return ntohs(m_wSerialNO); }
    void  SetSrcDriId(u8   bySrcDriId){ m_bySrcDriId = bySrcDriId;} 
    u8    GetSrcDriId( void ) const { return m_bySrcDriId; }
    void  SetSrcMtId(u8   bySrcMtId){ m_bySrcMtId = bySrcMtId;} 
    u8    GetSrcMtId( void ) const { return m_bySrcMtId; }
    void  SetSrcSsnId( u16 wSrcSsnId ){ m_wSrcSsnId = htons(wSrcSsnId);} 
    u16   GetSrcSsnId( void ) const { return ntohs(m_wSrcSsnId); }
    void  SetDstSsnId( u16 wDstSsnId ){ m_wDstSsnId = htons(wDstSsnId);} 
    u16   GetDstSsnId( void ) const { return ntohs(m_wDstSsnId); }
    void  SetDstDriId(u8   byDstDriId){ m_byDstDriId = byDstDriId;} 
    u8    GetDstDriId( void ) const { return m_byDstDriId; }  
    void  SetDstMtId(u8   byDstMtId){ m_byDstMtId = byDstMtId;} 
    u8    GetDstMtId( void ) const { return m_byDstMtId; }
    void  SetMcuId(u8   byMcuId){ m_byMcuId = byMcuId;} 
    u8    GetMcuId( void ) const { return m_byMcuId; }
    void  SetChnIndex(u8   byChnIndex){ m_byChnIndex = byChnIndex;} 
    u8    GetChnIndex( void ) const { return m_byChnIndex; }
    void  SetConfIdx( u32 dwConfIdx ){ m_dwConfIdx = dwConfIdx; } 
    u32   GetConfIdx( void ) const { return m_dwConfIdx; }
    void  SetEventId(u16  wEventId){ m_wEventId = htons(wEventId);} 
    u16   GetEventId( void ) const { return ntohs(m_wEventId); }
    void  SetTimer(u16  wTimer){ m_wTimer = htons(wTimer);} 
    u16   GetTimer( void ) const { return ntohs(m_wTimer); }
    void  SetErrorCode(u16  wErrorCode){ m_wErrorCode = htons(wErrorCode);} 
    u16   GetErrorCode( void ) const { return ntohs(m_wErrorCode); }
    void  SetTotalPktNum(u8 byPktNum) { m_byTotalPktNum = byPktNum; }
    u8    GetTotalPktNum( void ) { return m_byTotalPktNum; }
    void  SetCurPktIdx(u8 byPktIdx) { m_byCurPktIdx = byPktIdx; }
    u8    GetCurPktIdx( void ) { return m_byCurPktIdx; }

    void Init( void );
    void SetNoSrc(){ SetSrcSsnId( 0 ); }
    void SetMsgBodyLen( u16  wMsgBodyLen );
    CServMsg( void );//constructor
    CServMsg( u8   * const pbyMsg, u16  wMsgLen );//constructor
    ~CServMsg( void );//distructor
    void ClearHdr( void );//��Ϣͷ����
    CConfId GetConfId( void ) const;//��ȡ�������Ϣ
    void SetConfId( const CConfId & cConfId );//���û������Ϣ
    void SetNullConfId( void );//���û������ϢΪ��
    u16  GetMsgBodyLen( void ) const;//��ȡ��Ϣ�峤����Ϣ
    u16  GetMsgBody( u8   * pbyMsgBodyBuf, u16  wBufLen ) const;//��ȡ��Ϣ�壬���û�����BUFFER�������С���ضϴ���
    u8   * const GetMsgBody( void ) const;//��ȡ��Ϣ��ָ�룬�û������ṩBUFFER
    void SetMsgBody( u8   * const pbyMsgBody = NULL, u16  wLen = 0 );//������Ϣ��
    void CatMsgBody( u8   * const pbyMsgBody, u16  wLen );//�����Ϣ��
    u16  GetServMsgLen( void ) const;//��ȡ������Ϣ����
    u16  GetServMsg( u8   * pbyMsgBuf, u16  wBufLen ) const;//��ȡ������Ϣ�����û�����BUFFER�������С���ضϴ���
    u8   * const GetServMsg( void ) const;//��ȡ������Ϣָ�룬�û������ṩBUFFER
    void SetServMsg( u8   * const pbyMsg, u16  wLen );//����������Ϣ
    const CServMsg & operator= ( const CServMsg & cServMsg );//����������
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
    ;


/*-------------------------------------------------------------------
                               CServMsg                              
--------------------------------------------------------------------*/

/*====================================================================
    ������      ��CServMsg
    ����        ��constructor
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����
    ����ֵ˵��  ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline CServMsg::CServMsg( void )
{
	Init();
}

/*====================================================================
    ������      ��CServMsg
    ����        ��constructor
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * const pbyMsg, Ҫ��ֵ����Ϣָ��
				  u16 wMsgLen, Ҫ��ֵ����Ϣ���ȣ�������ڵ��� SERV_MSGHEAD_LEN
    ����ֵ˵��  ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline CServMsg::CServMsg( u8 * const pbyMsg, u16 wMsgLen )
{
	Init();
	
	if( wMsgLen < SERV_MSGHEAD_LEN || pbyMsg == NULL )
		return;
	
	wMsgLen = getmin( wMsgLen, SERV_MSG_LEN );
	memcpy( this, pbyMsg, wMsgLen );
	
	//set length
	SetMsgBodyLen( wMsgLen - SERV_MSGHEAD_LEN );
}

/*====================================================================
    ������      ��~CServMsg
    ����        ��distructor
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����
    ����ֵ˵��  ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline CServMsg::~CServMsg( void )
{

}

/*====================================================================
    ������      ��Init
    ����        ��BUFFER��ʼ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����
    ����ֵ˵��  ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline void CServMsg::Init( void )
{
	memset( this, 0, SERV_MSG_LEN );	//����
}

/*====================================================================
    ������      ��ClearHdr
    ����        ����Ϣͷ����
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����
    ����ֵ˵��  ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline void CServMsg::ClearHdr( void )
{
	u16		wBodyLen = GetMsgBodyLen();

	memset( this, 0, SERV_MSGHEAD_LEN );	//����
	SetMsgBodyLen( wBodyLen );
}

/*====================================================================
    ������      ��GetConfId
    ����        ����ȡ�������Ϣ
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ������ţ�ȫ0��ʾ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
    02/12/24    1.0         LI Yi         �޸Ľӿ�
====================================================================*/
inline CConfId CServMsg::GetConfId( void ) const
{
	return( m_cConfId );
}

/*====================================================================
    ������      ��SetConfId
    ����        �����û������Ϣ
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����const CConfId & cConfId������ţ�ȫ0��ʾ����
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
    02/12/24    1.0         LI Yi         �޸Ľӿ�
====================================================================*/
inline void CServMsg::SetConfId( const CConfId & cConfId )
{
	m_cConfId = cConfId;
}

/*====================================================================
    ������      ��SetNullConfId
    ����        �����û������ϢΪ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    03/06/06    1.0         LI Yi         ����
====================================================================*/
inline void CServMsg::SetNullConfId( void )
{
	CConfId		cConfId;
	
	cConfId.SetNull();
	SetConfId( cConfId );
}

/*====================================================================
    ������      ��GetMsgBodyLen
    ����        ����ȡ��Ϣ�峤����Ϣ
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ����Ϣ�峤��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline u16 CServMsg::GetMsgBodyLen( void ) const
{
	return( ntohs( m_wMsgBodyLen ) );
}

/*====================================================================
    ������      ��SetMsgBodyLen
    ����        ��������Ϣ�峤����Ϣ
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u16 wMsgBodyLen, ��Ϣ�峤��
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline void CServMsg::SetMsgBodyLen( u16 wMsgBodyLen )
{
	m_wMsgBodyLen = htons( wMsgBodyLen );
}

/*====================================================================
    ������      ��GetMsgBody
    ����        ����ȡ��Ϣ��ָ�룬�û������ṩBUFFER
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ��u8 * constָ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline u8 * const CServMsg::GetMsgBody( void ) const
{
	return( ( u8 * const )m_abyMsgBody );
}

/*====================================================================
    ������      ��GetMsgBodyLen
    ����        ����ȡ������Ϣ����
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ��������Ϣ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/26    1.0         LI Yi         ����
====================================================================*/
inline u16 CServMsg::GetServMsgLen( void ) const
{
	return( GetMsgBodyLen() + SERV_MSGHEAD_LEN );
}

/*====================================================================
    ������      ��GetMsgBody
    ����        ����ȡ��Ϣ�壬���û�����BUFFER�������С���ضϴ���
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * pbyMsgBodyBuf, ���ص���Ϣ��
				  u16 wBufLen, BUFFER��С
    ����ֵ˵��  ��ʵ�ʷ��ص���Ϣ�峤��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline u16 CServMsg::GetMsgBody( u8 * pbyMsgBodyBuf, u16 wBufLen ) const
{
    u16 wActLen = getmin( GetMsgBodyLen(), wBufLen );
	memcpy( pbyMsgBodyBuf, m_abyMsgBody, wActLen );
	return wActLen;
}

/*====================================================================
    ������      ��SetMsgBody
    ����        ��������Ϣ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * const pbyMsgBody, �������Ϣ�壬ȱʡΪNULL
				  u16 wLen, �������Ϣ�峤�ȣ�ȱʡΪ0
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline void CServMsg::SetMsgBody( u8 * const pbyMsgBody, u16 wLen )
{
    /*
    if (pbyMsgBody == NULL || wLen == 0)
    {
        SetMsgBodyLen( 0 );
        return;
    }
    */
	wLen = getmin( wLen, SERV_MSG_LEN - SERV_MSGHEAD_LEN );
	memcpy( m_abyMsgBody, pbyMsgBody, wLen );
	SetMsgBodyLen( wLen );
}

/*====================================================================
    ������      ��CatMsgBody
    ����        �������Ϣ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * const pbyMsgBody, �������Ϣ�壬ȱʡΪNULL
				  u16 wLen, �������Ϣ�峤�ȣ�ȱʡΪ0
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/11/07    1.0         Liaoweijiang  ����
====================================================================*/
inline void CServMsg::CatMsgBody( u8 * const pbyMsgBody, u16 wLen )
{
    /*
    if (pbyMsgBody == NULL || wLen == 0)
    {
        return;
    }
    */
	wLen = getmin( wLen, SERV_MSG_LEN - SERV_MSGHEAD_LEN - GetMsgBodyLen() );
	memcpy( m_abyMsgBody + GetMsgBodyLen(), pbyMsgBody, wLen );
	SetMsgBodyLen( GetMsgBodyLen() + wLen );
}

/*====================================================================
    ������      ��GetMsgBody
    ����        ����ȡ������Ϣָ�룬�û������ṩBUFFER
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ��u8 * constָ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/26    1.0         LI Yi         ����
====================================================================*/
inline u8 * const CServMsg::GetServMsg( void ) const
{
	return( ( u8 * const )( this ) );
}

/*====================================================================
    ������      ��GetServMsg
    ����        ����ȡ������Ϣ�����û�����BUFFER�������С���ضϴ���
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * pbyMsgBuf, ���ص���Ϣ
				  u16 wBufLen, BUFFER��С
    ����ֵ˵��  ��ʵ�ʷ��ص���Ϣ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline u16 CServMsg::GetServMsg( u8 * pbyMsgBuf, u16 wBufLen ) const
{
	wBufLen = getmin(SERV_MSG_LEN,wBufLen);
	memcpy( pbyMsgBuf, this, wBufLen );
	return( getmin( GetMsgBodyLen() + SERV_MSGHEAD_LEN, wBufLen ) );
}

/*====================================================================
    ������      ��SetServMsg
    ����        ������������Ϣ
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * const pbyMsg, �����������Ϣ
				  u16 wMsgLen, �������Ϣ����
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/15    1.0         LI Yi         ����
====================================================================*/
inline void CServMsg::SetServMsg( u8 * const pbyMsg, u16 wMsgLen )
{
	if( wMsgLen < SERV_MSGHEAD_LEN )
	{
		OspPrintf( TRUE, FALSE, "CServMsg: SetServMsg() Exception -- invalid MsgLen!\n" );
		return;
	}

	wMsgLen = getmin( wMsgLen, SERV_MSG_LEN );
	memcpy( this, pbyMsg, wMsgLen );
	SetMsgBodyLen( wMsgLen - SERV_MSGHEAD_LEN );
}

/*====================================================================
    ������      ��operator=
    ����        ������������
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����CServMsg & cServMsg, ��ֵ����Ϣ����
    ����ֵ˵��  ��CServMsg����Ӧ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/26    1.0         LI Yi         ����
====================================================================*/
inline const CServMsg & CServMsg::operator= ( const CServMsg & cServMsg )
{
	u16	wLen = cServMsg.GetServMsgLen();

	memcpy( this, cServMsg.GetServMsg(), wLen );
	return( *this );
}



#ifdef WIN32
#pragma pack( pop )
#endif


#endif

