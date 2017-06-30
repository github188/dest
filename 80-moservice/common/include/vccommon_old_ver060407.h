/*****************************************************************************
ģ����      : updatelib
�ļ���      : vccommon_old.h
����ļ�    : 
�ļ�ʵ�ֹ���: ��ȡ������������Ϣ�Ľṹͷ�ļ�
����        : ����
�汾        : V1.0  Copyright(C) 2006-2008 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���		�߶���      �޸�����
2006/04/07  1.0         ����					����
******************************************************************************/


#ifndef _VCCOMMON_OLD_060407_H_
#define _VCCOMMON_OLD_060407_H_


#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "kdvtype.h"
#include "kdvdef.h"
// �����°汾�Ķ��� 
#include "vccommon.h"

#ifdef _LINUX_

#ifndef min
#define min(a,b) ((a)>(b)?(b):(a))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#endif//!_LINUX_



#ifdef WIN32
    #pragma comment( lib, "ws2_32.lib" ) 
    #pragma pack( push )
    #pragma pack( 1 )
    #define window( x )	x
    #define PACKED 
#else
    #include <netinet/in.h>
    #define window( x )
    #define PACKED __attribute__((__packed__))	// ȡ�����������Ż�����
#endif


//����Žṹ (len:16)
struct CConfIdOld
{
protected:
	u8  	m_abyConfId[MAXLEN_CONFID];

public:
	u8   GetConfId( u8   * pbyConfId, u8   byBufLen ) const; //��û��������
	void CreateConfId( void );                         //���������
	void GetConfIdString( LPSTR lpszConfId, u8   byBufLen ) const;//��û�����ַ���
	void SetConfId( const CConfIdOld & cConfId );                  //���û����
	u8   SetConfId( const u8   * pbyConfId, u8   byBufLen );          //���û��������
	u8   SetConfId( LPCSTR lpszConfId );                          //���û��������	
	BOOL IsNull( void ) const;                                  //�жϻ�����Ƿ�Ϊ��
	void SetNull( void );                                       //���û����Ϊ��
	BOOL operator == ( const CConfIdOld & cConfId ) const;         //����������
	void Print( void )
	{
		s8 achBuf[MAXLEN_CONFID*2+1];
		memset(achBuf, 0, sizeof(achBuf));
		GetConfIdString( achBuf, sizeof(achBuf) );
		OspPrintf( TRUE, FALSE, "%s\n", achBuf);
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
/*-------------------------------------------------------------------
                               CConfIdOld                                  
--------------------------------------------------------------------*/

/*====================================================================
    ������      ��GetConfId
    ����        ����û��������
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * pbyConfId, �����BUFFER����ָ�룬�������ػ����
			      u8 byBufLen, BUFFER��С
    ����ֵ˵��  ��ʵ�ʷ��������С
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/12/31    1.0         LI Yi         ����
====================================================================*/
inline u8 CConfIdOld::GetConfId( u8 * pbyConfId, u8 byBufLen ) const
{
	u8	byLen = min( sizeof( m_abyConfId ), byBufLen );
	memcpy( pbyConfId, m_abyConfId, byLen );

	return( byLen );
}

/*====================================================================
    ������      ��GetConfIdString
    ����        ����û�����ַ���
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����LPSTR lpszConfId, ������ַ���BUFFER����ָ�룬����
	                   ����0��β������ַ���
			      u8 byBufLen, BUFFER��С
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/12/31    1.0         LI Yi         ����
====================================================================*/
inline void CConfIdOld::GetConfIdString( LPSTR lpszConfId, u8 byBufLen ) const
{
	u8	byLoop;

	for( byLoop = 0; byLoop < sizeof( m_abyConfId ) && byLoop < ( byBufLen - 1 ) / 2; byLoop++ )
	{
		sprintf( lpszConfId + byLoop * 2, "%.2x", m_abyConfId[byLoop] );
	}
	lpszConfId[byLoop * 2] = '\0';
}

/*====================================================================
    ������      ��SetConfId
    ����        �����û��������
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����const CConfIdOld & cConfId, ��������
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/12/31    1.0         LI Yi         ����
====================================================================*/
inline void CConfIdOld::SetConfId( const CConfIdOld & cConfId )
{
	cConfId.GetConfId( m_abyConfId, sizeof( m_abyConfId ) );
}

/*====================================================================
    ������      ��SetConfId
    ����        �����û��������
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����const u8 * pbyConfId, ���������ָ��
			      u8 byBufLen, BUFFER��С
    ����ֵ˵��  ��ʵ�����������С
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/12/31    1.0         LI Yi         ����
====================================================================*/
inline u8 CConfIdOld::SetConfId( const u8 * pbyConfId, u8 byBufLen )
{
	u8	bySize = min( byBufLen, sizeof( m_abyConfId ) );
	
	memset( m_abyConfId, 0, sizeof( m_abyConfId ) );
	memcpy( m_abyConfId, pbyConfId, bySize );

	return( bySize );
}
	
/*====================================================================
    ������      ��SetConfId
    ����        �����û��������
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����LPCSTR lpszConfId, ����Ļ�����ַ���
    ����ֵ˵��  ��ʵ�����������С
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/12/31    1.0         LI Yi         ����
====================================================================*/
inline u8 CConfIdOld::SetConfId( LPCSTR lpszConfId )
{
	LPCSTR	lpszTemp = lpszConfId;
	s8	achTemp[3], *lpszStop;
	u8	byCount = 0;

	memset( m_abyConfId, 0, sizeof( m_abyConfId ) );
	while( lpszTemp != NULL && lpszTemp + 1 != NULL && byCount < sizeof( m_abyConfId ) )
	{
		memcpy( achTemp, lpszTemp, 2 );
		achTemp[2] = '\0';
		m_abyConfId[byCount] = ( u8 )strtoul( achTemp, &lpszStop, 16 );
		byCount++;
		lpszTemp += 2;
	}

	return( byCount );
}

/*====================================================================
    ������      ��CreateConfId
    ����        �����������
    �㷨ʵ��    �����ݵ�ǰʱ������ֵ
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    03/06/06    1.0         LI Yi         ����
====================================================================*/
inline void CConfIdOld::CreateConfId( void )
{
	struct tm	*ptmCurTime;
	time_t		tCurTime = time( NULL );
	u8	byTemp;
	u16	wTemp;

	memset( m_abyConfId, 0, sizeof( m_abyConfId ) );
	
	//get current time
	ptmCurTime = localtime( &tCurTime );
	//year
	wTemp = ptmCurTime->tm_year + 1900;
	memcpy( m_abyConfId, &wTemp, sizeof( u16 ) );
	//month
	byTemp = ptmCurTime->tm_mon + 1;
	memcpy( m_abyConfId + 2, &byTemp, sizeof( u8 ) );
	//day
	byTemp = ptmCurTime->tm_mday;
	memcpy( m_abyConfId + 3, &byTemp, sizeof( u8 ) );
	//hour
	byTemp = ptmCurTime->tm_hour;
	memcpy( m_abyConfId + 4, &byTemp, sizeof( u8 ) );
	//minute
	byTemp = ptmCurTime->tm_min;
	memcpy( m_abyConfId + 5, &byTemp, sizeof( u8 ) );
	//second
	byTemp = ptmCurTime->tm_sec;
	memcpy( m_abyConfId + 6, &byTemp, sizeof( u8 ) );
	//msec
	wTemp = 0;
	memcpy( m_abyConfId + 7, &wTemp, sizeof( u16 ) );

	//rand
	wTemp = rand();
	memcpy( m_abyConfId + 9, &wTemp, sizeof( u16 ) );
}

/*====================================================================
    ������      ��IsNull
    ����        ���жϻ�����Ƿ�Ϊ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    03/03/31    1.0         LI Yi         ����
====================================================================*/
inline BOOL CConfIdOld::IsNull( void ) const
{
	u8	abyNull[sizeof( m_abyConfId )];

	memset( abyNull, 0, sizeof( abyNull ) );
	if( memcmp( m_abyConfId, abyNull, sizeof( m_abyConfId ) ) == 0 )
		return( TRUE );
	else
		return( FALSE );
}

/*====================================================================
    ������      ��SetNull
    ����        �����û����Ϊ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    03/03/31    1.0         LI Yi         ����
====================================================================*/
inline void CConfIdOld::SetNull( void )
{
	memset( m_abyConfId, 0, sizeof( m_abyConfId ) );
}

/*====================================================================
    ������      ��operator==
    ����        ������������
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����const CConfIdOld & cConfId, ��������
    ����ֵ˵��  ����ȷ���TRUE�����򷵻�FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/12/31    1.0         LI Yi         ����
====================================================================*/
inline BOOL CConfIdOld::operator == ( const CConfIdOld & cConfId ) const
{
	u8	abyTemp[sizeof( m_abyConfId )];

	cConfId.GetConfId( abyTemp, sizeof( abyTemp ) );
	if( memcmp( abyTemp, m_abyConfId, sizeof( m_abyConfId ) ) == 0 )
		return( TRUE );
	else
		return( FALSE );
}


//����������Խṹ(�����в���ı�)
struct TConfAttrbOld
{
protected:
	u8     m_byOpenMode;        //���鿪�ŷ�ʽ: 0-������,�ܾ��б�������ն� 1-����������� 2-��ȫ����
	u8     m_byEncryptMode;     //�������ģʽ: 0-������, 1-des����,2-aes����
	u8     m_byMulticastMode;   //�鲥���鷽ʽ: 0-�����鲥���� 1-�鲥����
	u8     m_byDataMode;        //���ݻ��鷽ʽ: 0-���������ݵ�����Ƶ���� 1-�������ݵ�����Ƶ���� 2-ֻ�����ݵĻ���
	u8     m_byReleaseMode;     //���������ʽ: 0-�����Զ����� 1-���ն�ʱ�Զ�����
    u8     m_byVideoMode;       //������Ƶģʽ: 0-�ٶ����� 1-��������
	u8     m_byCascadeMode;     //���鼶����ʽ: 0-��֧�ֺϲ�����, 1-֧�ֺϲ�����
	u8     m_byPrsMode;         //�����ش���ʽ: 0-���ش� 1-��
    u8     m_byHasTvWallModule; //����ǽģ��:   0-�޵���ǽģ��  1-�е���ǽģ��
	u8     m_byHasVmpModule;    //����ϳ�ģ��: 0-�޻���ϳ�ģ��  1-�л���ϳ�ģ��
	u8     m_byDiscussConf;     //�Ƿ����ۻ���: 0-�������ۻ���(�ݽ�����) 1-���ۻ��� (��һ�ֶν��������ƻ��鿪ʼ���Ƿ���������)
	u8     m_byUseAdapter;		//�Ƿ���������: 0-�������κ����� 2-����Ҫ����(����������)���� 
	u8     m_bySpeakerSrc;		//�����˵�Դ:   0-���Լ� 1-����ϯ 2-����һ�η����� 
	u8     m_bySpeakerSrcMode;  //�����˵�Դ�ķ�ʽ: 0-��Чֵ 1-����Ƶ 2-����Ƶ 3-����Ƶ
	u8     m_byMulcastLowStream;//�Ƿ��鲥��������: 0-�����鲥��������(�鲥��������) 1-�鲥��������
	u8     m_byAllInitDumb;     //�ն������Ƿ��ʼ���� 0-������ 1-����
	u8	   m_byDualMode;		//�����˫������ʽ: 0-������ 1-�����ն�
	u8     m_byUniformMode;     //����ת��ʱ�Ƿ�֧�ֹ�һ������ʽ: CONF_UNIFORMMODE_NONE-������ CONF_UNIFORMMODE_valid-������
	                            //�������ʽ�붪���ش���ʽ����
	u8	   m_byReserve1;		//�����ֶ�
	u8	   m_byReserve2;		//�����ֶ�
    
//    u32    m_dwMulticastIp;     //�����鲥��ַ������������鲥���飬��Ҫ��д�鲥��ַ
//    u16    m_wMulticastPort;    //�����鲥��ʼ�˿ڣ�����������鲥���飬��Ҫ��д�鲥��ַ
//	u8	   m_byReserve3;		//�����ֶ�
//	u8	   m_byReserve4;		//�����ֶ�

public:
	TConfAttrbOld( void )
	{ 
		memset( this, 0, sizeof(TConfAttrbOld) );
		m_byOpenMode = 2;
	}

    void   SetOpenMode(u8   byOpenMode){ m_byOpenMode = byOpenMode;} 
    u8     GetOpenMode( void ) const { return m_byOpenMode; }
	void   SetEncryptMode(u8   byEncryptMode){ m_byEncryptMode = byEncryptMode;} 
    u8     GetEncryptMode( void ) const { return m_byEncryptMode; }
    void   SetMulticastMode(BOOL bMulticastMode){ m_byMulticastMode = bMulticastMode;} 
    BOOL   IsMulticastMode( void ) const { return m_byMulticastMode == 0 ? FALSE : TRUE; }
    void   SetDataMode(u8   byDataMode){ m_byDataMode = byDataMode;} 
    u8     GetDataMode( void ) const { return m_byDataMode; }
    void   SetReleaseMode(BOOL bReleaseMode){ m_byReleaseMode = bReleaseMode;} 
    BOOL   IsReleaseNoMt( void ) const { return m_byReleaseMode == 0 ? FALSE : TRUE; }
    void   SetQualityPri(BOOL bQualityPri){ m_byVideoMode = bQualityPri;} 
    BOOL   IsQualityPri( void ) const { return m_byVideoMode == 0 ? FALSE : TRUE; }
	void   SetSupportCascade(BOOL bCascadeMode){ m_byCascadeMode = (bCascadeMode==TRUE?1:0); }
	BOOL   IsSupportCascade( void ) const { return (m_byCascadeMode != 0 );}
	void   SetPrsMode( BOOL bResendPack ){ m_byPrsMode = bResendPack; }
	BOOL   IsResendLosePack( void ){ return m_byPrsMode == 0 ? FALSE : TRUE; }
	void   SetHasTvWallModule( BOOL bHasTvWallModule ){ m_byHasTvWallModule = bHasTvWallModule; }
	BOOL   IsHasTvWallModule( void ){ return m_byHasTvWallModule == 0 ? FALSE : TRUE; }
	void   SetHasVmpModule( BOOL bHasVmpModule ){ m_byHasVmpModule = bHasVmpModule; }
	BOOL   IsHasVmpModule( void ){ return m_byHasVmpModule == 0 ? FALSE : TRUE; }
	void   SetDiscussConf( BOOL bDiscussConf ){ m_byDiscussConf = bDiscussConf; }
	BOOL   IsDiscussConf( void ){ return m_byDiscussConf == 0 ? FALSE : TRUE; }
    void   SetUseAdapter( BOOL bUseAdapter ){ m_byUseAdapter = bUseAdapter; } 
    BOOL   IsUseAdapter( void ) const { return m_byUseAdapter == 0 ? FALSE : TRUE; }
    void   SetSpeakerSrc( u8 bySpeakerSrc ){ m_bySpeakerSrc = bySpeakerSrc;} 
    u8     GetSpeakerSrc( void ) const { return m_bySpeakerSrc; }
    void   SetSpeakerSrcMode( u8 bySpeakerSrcMode ){ m_bySpeakerSrcMode = bySpeakerSrcMode;} 
    u8     GetSpeakerSrcMode( void ) const { return m_bySpeakerSrcMode; }
	void   SetMulcastLowStream( BOOL bMulcastLowStream ){ m_byMulcastLowStream = bMulcastLowStream;} 
    BOOL   IsMulcastLowStream( void ) const { return m_byMulcastLowStream == 0 ? FALSE : TRUE; }
	void   SetAllInitDumb( BOOL bAllInitDumb ){ m_byAllInitDumb = bAllInitDumb;} 
    BOOL   IsAllInitDumb( void ) const { return m_byAllInitDumb == 0 ? FALSE : TRUE; }
	void   SetUniformMode( BOOL bUniformMode ){ m_byUniformMode = bUniformMode; }
	u8     IsAdjustUniformPack( void ){ return m_byUniformMode; }
	void   SetDualMode(	u8 byDualMode ){ m_byDualMode = byDualMode;	}
	u8	   GetDualMode( void ) const { return m_byDualMode;	}
//    void   SetMulticastIp (u32 dwIp) { m_dwMulticastIp = htonl(dwIp); }
//    u32    GetMulticastIp ( void ) const { return ntohl(m_dwMulticastIp); }
//    void   SetMulticastPort ( u16 wPort) { m_wMulticastPort = wPort; }
//    u16    GetMulticastPort ( void ) const { return m_wMulticastPort; }
	void   Print( void )
	{
		OspPrintf( TRUE, FALSE, "\nConfAttrb:\n" );
		OspPrintf( TRUE, FALSE, "m_byOpenMode: %d\n", m_byOpenMode);
		OspPrintf( TRUE, FALSE, "m_byEncryptMode: %d\n", m_byEncryptMode);
		OspPrintf( TRUE, FALSE, "m_byMulticastMode: %d\n", m_byMulticastMode);
//        OspPrintf( TRUE, FALSE, "m_byMulticastIp: 0x%x\n", GetMulticastIp());
//        OspPrintf( TRUE, FALSE, "m_byMulticastPort: %d\n", m_wMulticastPort);
		OspPrintf( TRUE, FALSE, "m_byDataMode: %d\n", m_byDataMode);
		OspPrintf( TRUE, FALSE, "m_byReleaseMode: %d\n", m_byReleaseMode);
		OspPrintf( TRUE, FALSE, "m_byVideoMode: %d\n", m_byVideoMode);
		OspPrintf( TRUE, FALSE, "m_byCascadeMode: %d\n", m_byCascadeMode);
		OspPrintf( TRUE, FALSE, "m_byPrsMode: %d\n", m_byPrsMode);
		OspPrintf( TRUE, FALSE, "m_byHasTvWallModule: %d\n", m_byHasTvWallModule);
		OspPrintf( TRUE, FALSE, "m_byHasVmpModule: %d\n", m_byHasVmpModule);	
		OspPrintf( TRUE, FALSE, "m_byDiscussConf: %d\n", m_byDiscussConf);
		OspPrintf( TRUE, FALSE, "m_byUseAdapter: %d\n", m_byUseAdapter);
		OspPrintf( TRUE, FALSE, "m_bySpeakerSrc: %d\n", m_bySpeakerSrc);
		OspPrintf( TRUE, FALSE, "m_bySpeakerSrcMode: %d\n", m_bySpeakerSrcMode);
		OspPrintf( TRUE, FALSE, "m_byMulcastLowStream: %d\n", m_byMulcastLowStream);
		OspPrintf( TRUE, FALSE, "m_byAllInitDumb: %d\n", m_byAllInitDumb);
		OspPrintf( TRUE, FALSE, "m_byUniformMode: %d\n", m_byUniformMode);
		OspPrintf( TRUE, FALSE, "m_byDualMode: %d\n", m_byDualMode);
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


//���������Ϣ�ṹ,�ýṹ�����˻��������Ϣ��״̬ (len:725)
struct TConfInfoOld
{
protected:
    CConfIdOld    m_cConfId;                        //����ţ�ȫ��Ψһ
    TKdvTime      m_tStartTime;                     //��ʼʱ�䣬����̨��0Ϊ������ʼ
    u16           m_wDuration;                      //����ʱ��(����)��0��ʾ���Զ�ֹͣ
    u16           m_wBitRate;                       //��������(��λ:Kbps,1K=1024)
    u16           m_wSecBitRate;                    //˫�ٻ���ĵ�2����(��λ:Kbps,Ϊ0��ʾ�ǵ��ٻ���)
	u16			  m_wDcsBitRate;					//���ݻ�������(��λ:Kbps,Ϊ0��ʾ��֧�����ݻ���)
    u16           m_wReserved;                      //����
    u8            m_byTalkHoldTime;                 //��С���Գ���ʱ��(��λ:��)
    u16            m_wMixDelayTime;                 //������ʱʱ��
    TCapSupport   m_tCapSupport;                    //����֧�ֵ�ý��
    s8            m_achConfPwd[MAXLEN_PWD+1];       //��������
    s8            m_achConfName[MAXLEN_CONFNAME+1]; //������
    s8            m_achConfE164[MAXLEN_E164+1];     //�����E164����
    TMtAlias      m_tChairAlias;                    //��������ϯ�ı���
    TMtAlias      m_tSpeakerAlias;                  //�����з����˵ı���
    TConfAttrbOld m_tConfAttrb;                     //��������

    TMediaEncrypt m_tMediaKey;                      //��һ�汾������Կһ������֧�ָ���

public:
    TConfStatus   m_tStatus;                        //����״̬

public:	
    TConfInfoOld( void ){ memset( this, 0, sizeof( this ) ); m_tCapSupport.Clear(); }
    CConfIdOld GetConfId( void ) const { return m_cConfId; }
    void    SetConfId( CConfIdOld cConfId ){ m_cConfId = cConfId; }
    TKdvTime GetKdvStartTime( void ) const { return( m_tStartTime ); }
    void   SetKdvStartTime( TKdvTime tStartTime ){ m_tStartTime = tStartTime; }
    void   SetDuration(u16  wDuration){ m_wDuration = htons(wDuration);} 
    u16    GetDuration( void ) const { return ntohs(m_wDuration); }
    void   SetBitRate(u16  wBitRate){ m_wBitRate = htons(wBitRate);} 
    u16    GetBitRate( void ) const { return ntohs(m_wBitRate); }
    void   SetSecBitRate(u16 wSecBitRate){ m_wSecBitRate = htons(wSecBitRate);} 
    u16    GetSecBitRate( void ) const { return ntohs(m_wSecBitRate); }
	void   SetDcsBitRate(u16 wDcsBitRate){ m_wDcsBitRate = htons(wDcsBitRate); }
	u16	   GetDcsBitRate( void ) const { return ntohs(m_wDcsBitRate); }

    void   SetMainVideoFormat(u8 byVideoFormat)
    { 
        TSimCapSet tSim = m_tCapSupport.GetMainSimCapSet();
        tSim.SetVideoResolution(byVideoFormat); 
        m_tCapSupport.SetMainSimCapSet(tSim);
    } 
    u8     GetMainVideoFormat( void ) const { return m_tCapSupport.GetMainSimCapSet().GetVideoResolution(); }
    void   SetSecVideoFormat(u8 byVideoFormat)
    { 
        TSimCapSet tSim = m_tCapSupport.GetSecondSimCapSet();
        tSim.SetVideoResolution(byVideoFormat);
        m_tCapSupport.SetSecondSimCapSet(tSim); 
    } 
    u8     GetSecVideoFormat(void) const { return m_tCapSupport.GetSecondSimCapSet().GetVideoResolution(); }
    void   SetDoubleVideoFormat(u8 byVideoFormat) { m_tCapSupport.SetDStreamResolution(byVideoFormat); } 
    u8     GetDoubleVideoFormat(void) const { return m_tCapSupport.GetDStreamResolution(); }
    u8     GetVideoFormat(u8 byVideoType, u8 byChanNo = LOGCHL_VIDEO);

    void   SetMixDelayTime(u16 wDelayTime) { m_wMixDelayTime = htons(wDelayTime); }
    u16     GetMixDelayTime(void) { return ntohs(m_wMixDelayTime); }
    void   SetTalkHoldTime(u8   byTalkHoldTime){ m_byTalkHoldTime = byTalkHoldTime;} 
    u8     GetTalkHoldTime( void ) const { return m_byTalkHoldTime; }
    void   SetCapSupport(TCapSupport tCapSupport){ m_tCapSupport = tCapSupport;} 
    TCapSupport GetCapSupport( void ) const { return m_tCapSupport; }
    void   SetConfPwd( LPCSTR lpszConfPwd );
    LPCSTR GetConfPwd( void ) const { return m_achConfPwd; }
    void   SetConfName( LPCSTR lpszConfName );
    LPCSTR GetConfName( void ) const { return m_achConfName; }
    void   SetConfE164( LPCSTR lpszConfE164 );
    LPCSTR GetConfE164( void ) const { return m_achConfE164; }
    void   SetChairAlias(TMtAlias tChairAlias){ m_tChairAlias = tChairAlias;} 
    TMtAlias  GetChairAlias( void ) const { return m_tChairAlias; }
    void   SetSpeakerAlias(TMtAlias tSpeakerAlias){ m_tSpeakerAlias = tSpeakerAlias;} 
    TMtAlias  GetSpeakerAlias( void ) const { return m_tSpeakerAlias; }
    void   SetConfAttrb(TConfAttrbOld tConfAttrb){ m_tConfAttrb = tConfAttrb;} 
    TConfAttrbOld  GetConfAttrb( void ) const { return m_tConfAttrb; }
    void   SetStatus(TConfStatus tStatus){ m_tStatus = tStatus;} 
    TConfStatus  GetStatus( void ) const { return m_tStatus; }	

	TMediaEncrypt& GetMediaKey(void) { return m_tMediaKey; };
	void SetMediaKey(TMediaEncrypt& tMediaEncrypt){ memcpy(&m_tMediaKey, &tMediaEncrypt, sizeof(tMediaEncrypt));}

	void   GetVideoScale(u8 byVideoType, u16 &wVideoWidth, u16 &wVideoHeight, u8* pbyVideoResolution = NULL);
	BOOL   HasChairman( void ) const{ if(m_tStatus.m_tChairman.GetMtId()==0)return FALSE; return TRUE; }	
	TMt    GetChairman( void ) const{ return m_tStatus.m_tChairman; }
	void   SetNoChairman( void ){ memset( &m_tStatus.m_tChairman, 0, sizeof( TMt ) ); }
	void   SetChairman( TMt tChairman ){ m_tStatus.m_tChairman = tChairman; }
	BOOL   HasSpeaker( void ) const{ if(m_tStatus.m_tSpeaker.GetMtId()==0)return FALSE; return TRUE; }
    TMt    GetSpeaker( void ) const{ return m_tStatus.m_tSpeaker; }
	void   SetNoSpeaker( void ){ memset( &m_tStatus.m_tSpeaker, 0, sizeof( TMt ) ); }
	void   SetSpeaker( TMt tSpeaker ){ m_tStatus.m_tSpeaker = tSpeaker; }
	time_t GetStartTime( void ) const;	
	void SetStartTime( time_t dwStartTime );

	void Print( void )
	{
		OspPrintf( TRUE, FALSE, "\nConfInfo:\n" );
		OspPrintf( TRUE, FALSE, "m_cConfId: " );
		m_cConfId.Print();
		OspPrintf( TRUE, FALSE, "\nm_tStartTime: " );
        m_tStartTime.Print();
		OspPrintf( TRUE, FALSE, "\nm_wDuration: %d\n", ntohs(m_wDuration) );
		OspPrintf( TRUE, FALSE, "m_wBitRate: %d\n", ntohs(m_wBitRate) );
		OspPrintf( TRUE, FALSE, "m_wDcsBitRata: %d\n", ntohs(m_wDcsBitRate) );
		OspPrintf( TRUE, FALSE, "m_byVideoFormat: %d\n", GetMainVideoFormat() );
		OspPrintf( TRUE, FALSE, "m_byTalkHoldTime: %d\n", m_byTalkHoldTime );
		OspPrintf( TRUE, FALSE, "m_tCapSupport:" );
	    m_tCapSupport.Print();
		OspPrintf( TRUE, FALSE, "\nm_achConfPwd: %s\n", m_achConfPwd );
		OspPrintf( TRUE, FALSE, "m_achConfName: %s\n", m_achConfName );
		OspPrintf( TRUE, FALSE, "m_achConfE164: %s\n", m_achConfE164 );
		OspPrintf( TRUE, FALSE, "\n" );
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// ����Pack�����Ļ���洢��Ϣ 
struct TPackConfStoreOld
{
    TConfInfoOld m_tConfInfo;
	u8			 m_byMtNum;
	u16			 m_wAliasBufLen;  //(������ �洢���ļ���)����Packʱָ������� m_pbyAliasBuf �ֽڳ���
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


#ifdef WIN32
#pragma pack( pop )
#endif




#endif _VCCOMMON_OLD_060407_H_
