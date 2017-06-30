/*****************************************************************************
ģ����      : updatelib
�ļ���      : vccommon_old.h
����ļ�    : 
�ļ�ʵ�ֹ���: ��ȡ������������Ϣ�Ľṹͷ�ļ�
����        : �ű���
�汾        : V1.0  Copyright(C) 2006-2008 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���		�߶���      �޸�����
2006/02/22  1.0         �ű���					����
******************************************************************************/
#ifndef _VCCOMMON_040602_OLD_H_
#define _VCCOMMON_040602_OLD_H_

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "kdvtype.h"
#include "kdvdef.h"


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

#define MAXNUM_PERIEQP_CHNNL_OLD		16
#define MAXNUM_CONF_MT_OLD				192

//�����MCU���ն˻�����ṹ (len:6)
struct TMtOld
{
protected:
    u8          m_byMainType;   //�豸����
    u8  		m_bySubType;	//�豸����
	u8  		m_byMcuId;		//MCU��
	u8  		m_byEqpId;	    //�豸��	
	u8          m_byConfDriId;  //DRI���
    u8          m_byConfIdx;    //���������ţ�������ʵ���� 1 - MAXNUM_MCU_CONF

public:
	//�����������ͺ���
	void   SetType( u8   byType ){ m_byMainType = byType; }         //���ýṹ����
	u8     GetType( void ) const { return m_byMainType; }           //�õ��ṹ����
	void   SetMcuId( u8   byMcuId ){ m_byMcuId = byMcuId; }         //����McuId
	u8     GetMcuId( void ) const { return m_byMcuId; }             //�õ�McuId
	BOOL   operator ==( const TMtOld & tObj ) const;                  //�ж��Ƿ����

	//����MCU����
	void   SetMcu( u8   byMcuId );                         //����MCU
	void   SetMcuType( u8  byMcuType ){ m_byMainType = TYPE_MCU; m_bySubType = byMcuType; }
	u8     GetMcuType( void ) const { return m_bySubType; }

	//�������躯��
	void   SetMcuEqp( u8   byMcuId, u8   byEqpId, u8   byEqpType );            //����MCU����
	void   SetEqpType( u8   byEqpType ){ m_byMainType = TYPE_MCUPERI; m_bySubType = byEqpType; }                                       //����MCU��������
	u8     GetEqpType( void ) const { return m_bySubType; }
	void   SetEqpId( u8   byEqpId ){ m_byMainType = TYPE_MCUPERI; m_byEqpId = byEqpId; }
	u8     GetEqpId( void ) const { return m_byEqpId; }
	
	//�����ն˺���
	void   SetMt( u8   byMcuId, u8   byMtId, u8   m_byDriId = 0, u8   m_byConfIdx = 0);
	void   SetMt( TMtOld tMt);
	void   SetMtId( u8   byMtId ){ m_byMainType = TYPE_MT; m_byEqpId = byMtId; }
	u8     GetMtId( void ) const { return m_byEqpId; }//�����ն�
	void   SetMtType( u8   byMtType ) { m_byMainType = TYPE_MT; m_bySubType = byMtType; }
	u8     GetMtType( void ) const { return m_bySubType; }  	
    void   SetDriId(u8   byDriId){ m_byConfDriId = byDriId; }
    u8     GetDriId( void ) const { return m_byConfDriId; }
    void   SetConfIdx(u8   byConfIdx){ m_byConfIdx = byConfIdx; } 
    u8     GetConfIdx( void ) const { return m_byConfIdx; }
	void   SetNull( void ){ m_byMcuId = 0; m_byEqpId = 0; m_byConfDriId = 0; }
	BOOL   IsNull( void ){ return m_byMcuId == 0 && m_byEqpId == 0; }
	BOOL   IsLocal() const {	return (m_byMcuId == LOCAL_MCUID );	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//�����ն˽ṹ
typedef TMtOld TEqpOld;

//�ն���ѯ���� (len:8)
struct TMtPollParamOld: public TMtOld
{
protected:
	u16   m_wKeepTime;        //�ն���ѯ�ı���ʱ�� ��λ:��(s)
    u32   m_dwPollNum;        //�ն���ѯ�Ĵ���
public:
	void  SetKeepTime(u16 wKeepTime) { m_wKeepTime = htons(wKeepTime); } 
	u16   GetKeepTime(void) const { return ntohs(m_wKeepTime); }
    void  SetPollNum(u32 dwPollNum) { m_dwPollNum = htonl(dwPollNum); } 
	u32   GetPollNum(void) const { return ntohl(m_dwPollNum); }
	TMtOld   GetTMt(void) const { return *(TMtOld *)this; }
	void  SetTMt(TMtOld tMt) { memcpy( this, &tMt, sizeof(tMt) ); }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//���廭��ϳɳ�Ա�ṹ (len:7)
struct TVMPMemberOld: public TMtOld
{
protected:
    u8   m_byMemberType;     //����ϳɳ�Ա����,�μ�mcuconst.h�л���ϳɳ�Ա���Ͷ���
    u8   m_byMemStatus;      //��Ա״̬(MT_STATUS_CHAIRMAN, MT_STATUS_SPEAKER, MT_STATUS_AUDIENCE)

public:
    void  SetMemberType(u8   byMemberType){ m_byMemberType = byMemberType;} 
    u8    GetMemberType( void ) const { return m_byMemberType; }
    void  SetMemStatus(u8 byStatus) { m_byMemStatus = byStatus; }
    u8    GetMemStatus(void) const { return m_byMemStatus; }
    void  SetMemberTMt( TMtOld tMt ){ memcpy( this, &tMt, sizeof(TMt) ); }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//���廭��ϳɲ����ṹ (len:121)
struct TVMPParamOld
{
protected:
	u8      m_byVMPAuto;      //�Ƿ����Զ�����ϳ� 0-�� 1-��
	u8      m_byVMPBrdst;     //�ϳ�ͼ���Ƿ����ն˹㲥 0-�� 1-�� 
	u8      m_byVMPStyle;     //����ϳɷ��,�μ�mcuconst.h�л���ϳɷ����
    u8      m_byVMPSchemeId;  //�ϳɷ�񷽰����,���֧��5�׷���,1-5
	TVMPMemberOld  m_atVMPMember[MAXNUM_VMP_MEMBER]; //����ϳɳ�Ա
    u8      m_byVMPMode;      //ͼ�񸴺Ϸ�ʽ: 0-��ͼ�񸴺� 1-��ػ���ϯ����ͼ�񸴺� 2-�Զ�ͼ�񸴺�(��̬���������ó�Ա)
	u8		m_byRimEnabled;	  //�Ƿ�ʹ�ñ߿�: 0-��ʹ��(Ĭ��) 1-ʹ��;
							  //���ֶ�Ŀǰֻ�Է���0��Ч, ���������ݲ��漰���ֶε����ú��ж�
	u8		m_byReserved1;	  //�����ֶ�
	u8		m_byReserved2;	  //�����ֶ�
public:
    void   SetVMPAuto(u8   byVMPAuto){ m_byVMPAuto = byVMPAuto;} 
    BOOL   IsVMPAuto( void ) const { return m_byVMPAuto == 0 ? FALSE : TRUE; }
    void   SetVMPBrdst(u8   byVMPBrdst){ m_byVMPBrdst = byVMPBrdst;} 
    BOOL   IsVMPBrdst( void ) const { return m_byVMPBrdst == 0 ? FALSE : TRUE; }
    void   SetVMPStyle(u8   byVMPStyle){ m_byVMPStyle = byVMPStyle;} 
    u8     GetVMPStyle( void ) const { return m_byVMPStyle; }
    void   SetVMPSchemeId(u8 bySchemeId) { m_byVMPSchemeId = bySchemeId; }
    u8     GetVMPSchemeId(void) const { return m_byVMPSchemeId; }
    void   SetVMPMode(u8   byVMPMode){ m_byVMPMode = byVMPMode;} 
    u8     GetVMPMode( void ) const { return m_byVMPMode; }
	void   SetIsRimEnabled(u8 byEnabled){ m_byRimEnabled = byEnabled; }
	BOOL32 GetIsRimEnabled(void) const { return m_byRimEnabled == 1 ? TRUE : FALSE; }
    u8     GetMaxMemberNum( void ) const 
	{
		u8   byMaxMemNum = 1;

		switch( m_byVMPStyle ) 
		{
		case VMP_STYLE_ONE:
			byMaxMemNum = 1;
			break;
		case VMP_STYLE_VTWO:
		case VMP_STYLE_HTWO:
			byMaxMemNum = 2;
			break;
		case VMP_STYLE_THREE:
			byMaxMemNum = 3;
			break;
		case VMP_STYLE_FOUR:
			byMaxMemNum = 4;
			break;
		case VMP_STYLE_SIX:
			byMaxMemNum = 6;
			break;
		case VMP_STYLE_EIGHT:
			byMaxMemNum = 8;
			break;
		case VMP_STYLE_NINE:
			byMaxMemNum = 9;
			break;
		case VMP_STYLE_TEN:
			byMaxMemNum = 10;
			break;
		case VMP_STYLE_THIRTEEN:
			byMaxMemNum = 13;
			break;
		case VMP_STYLE_SIXTEEN:
			byMaxMemNum = 16;
			break;
		case VMP_STYLE_SPECFOUR:
			byMaxMemNum = 4;
			break;
		case VMP_STYLE_SEVEN:
			byMaxMemNum = 7;
			break;
		default:
			byMaxMemNum = 1;
			break;
		}
		return byMaxMemNum;
	}
	void   SetVmpMember( u8   byMemberId, TVMPMemberOld tVMPMember )
	{
		if( byMemberId >= MAXNUM_VMP_MEMBER )return;
		m_atVMPMember[byMemberId] = tVMPMember;
	}
	void   ClearVmpMember( u8 byMemberId )
	{
		if( byMemberId >= MAXNUM_VMP_MEMBER )return;
		m_atVMPMember[byMemberId].SetNull();
        m_atVMPMember[byMemberId].SetMemberType(0);
	}
    TVMPMemberOld *GetVmpMember( u8   byMemberId )
	{
		if( byMemberId >= MAXNUM_VMP_MEMBER )return NULL;
		return &m_atVMPMember[byMemberId];
	}
	BOOL IsMtInMember( TMtOld tMt )
	{
		u8 byLoop = 0;
		u8 byLoopCtrl = 0;
		while( byLoop < GetMaxMemberNum() && byLoopCtrl < MAXNUM_VMP_MEMBER )
		{
			if( tMt.GetMtId() == m_atVMPMember[byLoop].GetMtId() && 
				tMt.GetMcuId() == m_atVMPMember[byLoop].GetMcuId() )
			{
				return TRUE;
			}

			byLoop++;
			byLoopCtrl++;
		}

		return FALSE;
	}
	u8 GetChlOfMtInMember( TMtOld tMt )
	{
		u8 byLoop = 0;
		while( byLoop < GetMaxMemberNum() )
		{
			if( tMt.GetMtId() == m_atVMPMember[byLoop].GetMtId() )
			{
				return byLoop;
			}

			byLoop++;
		}

		return MAXNUM_VMP_MEMBER;
	}
	void   Print( void )
	{
		OspPrintf( TRUE, FALSE, "\nVMPParam:\n" );
		OspPrintf( TRUE, FALSE, "m_byVMPAuto: %d\n", m_byVMPAuto);
		OspPrintf( TRUE, FALSE, "m_byVMPBrdst: %d\n", m_byVMPBrdst);
		OspPrintf( TRUE, FALSE, "m_byVMPStyle: %d\n", m_byVMPStyle);
        OspPrintf( TRUE, FALSE, "m_byVmpSchemeId: %d\n", m_byVMPSchemeId );
		OspPrintf( TRUE, FALSE, "m_byRimEnabled: %d\n", m_byRimEnabled );
        OspPrintf( TRUE, FALSE, "m_byVMPMode: %d\n", m_byVMPMode );
		for( int i=0;i<GetMaxMemberNum();i++ )
		{
			OspPrintf( TRUE, FALSE, "VMP member %d: MtId-%d, Type-%d\n", 
				i,  m_atVMPMember[i].GetMtId(), m_atVMPMember[i].GetMemberType() );
		}
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//����Žṹ (len:16)
struct CConfIdOld
{
protected:
	u8  	m_abyConfId[MAXLEN_CONFID];

public:
	u8   CConfIdOld::GetConfId( u8   * pbyConfId, u8   byBufLen ) const; //��û��������
	void CConfIdOld::CreateConfId( void );									//���������
	void GetConfIdString( LPSTR lpszConfId, u8   byBufLen ) const;//��û�����ַ���
	void SetConfId( const CConfIdOld & cConfId );						//���û����
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

//���崫���ַ�ṹ(len:6)
struct TTransportAddrOld
{
public:
	u32   		m_dwIpAddr;		//IP��ַ, ������
	u16 		m_wPort;		//�˿ں�, ������
public:
    void   SetNetSeqIpAddr(u32    dwIpAddr){ m_dwIpAddr = dwIpAddr; } 
    u32    GetNetSeqIpAddr( void ) const { return m_dwIpAddr; }
    void   SetIpAddr(u32    dwIpAddr){ m_dwIpAddr = htonl(dwIpAddr); } 
    u32    GetIpAddr( void ) const { return ntohl(m_dwIpAddr); }
    void   SetPort(u16  wPort){ m_wPort = htons(wPort); } 
    u16    GetPort( void ) const { return ntohs(m_wPort); }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//(len:22)
struct TMediaEncryptOld
{
protected:
	u8  m_byEncryptMode;    //����ģʽ:CONF_ENCRYPTMODE_NONE,CONF_ENCRYPTMODE_DES, CONF_ENCRYPT_AES
    s32 m_nKeyLen;          //����key�ĳ���
	u8  m_abyEncKey[MAXLEN_KEY]; //����key
	u8  m_byReserve;//����
public:
	TMediaEncryptOld()
	{
		Reset();
	}
	void Reset()
	{
		m_byEncryptMode = CONF_ENCRYPTMODE_NONE;
		m_nKeyLen = 0;
	}

	void SetEncryptMode(u8 byEncMode)
	{
		m_byEncryptMode = byEncMode;
	}
    u8  GetEncryptMode()
	{
		return m_byEncryptMode;
	}
	void SetEncryptKey(u8 *pbyKey, s32 nLen)
	{
		m_nKeyLen = (nLen > MAXLEN_KEY ? MAXLEN_KEY : nLen);
		if(m_nKeyLen > 0)
			memcpy(m_abyEncKey, pbyKey, m_nKeyLen); 
		m_nKeyLen = htonl(m_nKeyLen);
	}
	
	void GetEncryptKey(u8 *pbyKey, s32* pnLen)
	{
		if(pnLen != NULL) *pnLen = ntohl(m_nKeyLen);
		if(pbyKey != NULL) memcpy(pbyKey, m_abyEncKey, ntohl(m_nKeyLen));
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//��¼¼���������(��λ:��)
struct  TRecProgOld
{
protected:
	u32   		m_dwCurProg;	//��ǰ����
	u32   		m_dwTotalTime;	//�ܳ��ȣ����ڷ���ʱ��Ч

public:
	//���õ�ǰ����λ�ã���λ���룩
	void SetCurProg( u32    dwCurProg )	{ m_dwCurProg = htonl( dwCurProg ); }
	//��ȡ��ǰ����λ�ã���λ���룩
	u32    GetCurProg( void ) const	{ return ntohl( m_dwCurProg ); }
	//�����ܳ��ȣ����Է�����Ч����λ���룩
	void SetTotalTime( u32    dwTotalTime )	{ m_dwTotalTime = htonl( dwTotalTime ); }
	//��ȡ�ܳ��ȣ����Է�����Ч����λ���룩
	u32    GetTotalTime( void ) const	{ return ntohl( m_dwTotalTime ); }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//ʱ��ṹ���¶���
struct TKdvTimeOld
{
protected:
	u16 		m_wYear;//��
	u8  		m_byMonth;//��
	u8  		m_byMDay;//��
	u8  		m_byHour;//ʱ
	u8  		m_byMinute;//��
	u8  		m_bySecond;//��

public:
	void SetTime( const time_t * ptTime );//����ʱ��
	void GetTime( time_t & tTime ) const;//�õ�ʱ��ṹ
	BOOL operator == ( const TKdvTimeOld & tTime );//����

	u16  GetYear( void ) const	{ return( ntohs( m_wYear ) ); }
	u8   GetMonth( void ) const	{ return( m_byMonth ); }
	u8   GetDay( void ) const	{ return( m_byMDay ); }
	u8   GetHour( void ) const	{ return( m_byHour ); }
	u8   GetMinute( void ) const	{ return( m_byMinute ); }
	u8   GetSecond( void ) const	{ return( m_bySecond ); }
	void SetYear( u16  wYear )	{ m_wYear = htons( wYear ); }
	void SetMonth( u8   byMonth )	{ m_byMonth = byMonth; }
	void SetDay( u8   byDay )	{ m_byMDay = byDay; }
	void SetHour( u8   byHour )	{ m_byHour = byHour; }
	void SetMinute( u8   byMinute )	{ m_byMinute = byMinute; }
	void SetSecond( u8   bySecond )	{ m_bySecond = bySecond; }
	void Print( void )
	{
		OspPrintf( TRUE, FALSE, "%d-%d-%d %d:%d:%d", ntohs(m_wYear),m_byMonth,m_byMDay,m_byHour,m_byMinute,m_bySecond );
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//��ѯ��Ϣ
struct TPollInfoOld
{
protected:
    u8  m_byMediaMode;           //��ѯģʽ MODE_VIDEO ͼ��  MODE_BOTH ����ͼ��
    u8  m_byPollState;           //��ѯ״̬���μ���ѯ״̬����
    u32 m_dwPollNum;             //�ն���ѯ�Ĵ���
    TMtPollParamOld m_tMtPollParam; //��ǰ����ѯ�㲥���ն˼������
public:
    void SetMediaMode(u8 byMediaMode) { m_byMediaMode = byMediaMode; }
    u8   GetMediaMode(void) { return m_byMediaMode; }
    void SetPollState(u8 byPollState) { m_byPollState = byPollState; }
    u8   GetPollState(void) const { return m_byPollState; }
    void SetPollNum(u32 dwPollNum) { m_dwPollNum = htonl(dwPollNum); }
    u32  GetPollNum(void) const { return ntohl(m_dwPollNum); }
    void SetMtPollParam(TMtPollParamOld tMtPollParam){ m_tMtPollParam = tMtPollParam; }
    TMtPollParamOld GetMtPollParam() { return m_tMtPollParam; }
    void Print(void)
    {
        OspPrintf(TRUE, FALSE, "\nPollInfo:\n");
        OspPrintf(TRUE, FALSE, "m_byMediaMode: %d\n", m_byMediaMode);
        OspPrintf(TRUE, FALSE, "m_byPollState: %d\n", m_byPollState);
        OspPrintf(TRUE, FALSE, "m_dwPollNum: %d\n", m_dwPollNum);
        OspPrintf(TRUE, FALSE, "CurPollMtId: %d\n", m_tMtPollParam.GetMtId());
    }
}
#ifndef WIN32
__attribute__((packed))
#endif
;

//����ǽ��ѯ��Ϣ
struct TTvWallPollInfoOld : public TPollInfoOld
{
protected:
    TEqpOld    m_tTvWall;
    u8      m_byTWChnnl;
    
public:
    void SetTvWall(TEqpOld tTvWall) { m_tTvWall = tTvWall; }
    TEqpOld GetTvWall(void) { return m_tTvWall; } 
    void SetTWChnnl(u8 byChnnl) { m_byTWChnnl = byChnnl; }
    u8   GetTWChnnl(void) { return m_byTWChnnl; }
}
#ifndef WIN32
__attribute__((packed))
#endif
;

//�����ն˱����ṹ(len:135)
struct TMtAliasOld
{
public:
    u8			m_AliasType;               //��������
    s8			m_achAlias[MAXLEN_ALIAS];  //�����ַ���
    TTransportAddrOld	m_tTransportAddr;          //�����ַ
public:
	TMtAliasOld( void ){ memset( this, 0, sizeof(TMtAliasOld) ); }
	BOOL operator ==( const TMtAliasOld & tObj ) const;
	BOOL IsNull( void ){ if( m_AliasType == 0 )return TRUE; return FALSE; }
	void SetNull( void ){ memset(this, 0, sizeof(TMtAliasOld)); }
	void SetE164Alias( LPCSTR lpzAlias )
	{
		if( lpzAlias != NULL )
		{
			m_AliasType = mtAliasTypeE164;
			memcpy( m_achAlias, lpzAlias, MAXLEN_ALIAS );
			m_achAlias[MAXLEN_ALIAS-1] = '\0';
		}
	}
	void SetH323Alias( LPCSTR lpzAlias )
	{
		if( lpzAlias != NULL )
		{
			m_AliasType = mtAliasTypeH323ID;
			memcpy( m_achAlias, lpzAlias, MAXLEN_ALIAS );
			m_achAlias[MAXLEN_ALIAS-1] = '\0';
		}
	}
	void SetH320Alias( u8 byLayer, u8 bySlot, u8 byChannel )
	{
		m_AliasType = mtAliasTypeH320ID;
		sprintf( m_achAlias, "��%d-��%d-ͨ��%d%c", byLayer, bySlot, byChannel, 0 );
	}
	BOOL GetH320Alias( u8 &byLayer, u8 &bySlot, u8 &byChannel )
	{
		BOOL bRet = FALSE;
		byLayer   = 0;
		bySlot    = 0; 
		byChannel = 0; 
		if( mtAliasTypeH320ID != m_AliasType )
		{
			return bRet;
		}

		s8 *pachLayerPrefix   = "��";
		s8 *pachSlotPrefix    = "-��";
		s8 *pachChannelPrefix = "-ͨ��";
		s8 *pachAlias = m_achAlias;

		if( 0 == memcmp( pachAlias, pachLayerPrefix, strlen("��") ) )
		{
			s32  nPos = 0;
			s32  nMaxCHLen = 3; //һ�ֽڵ�����ִ�����
			s8 achLayer[4] = {0};

			//��ȡ���
			pachAlias += strlen("��");
			for( nPos = 0; nPos < nMaxCHLen; nPos++ )
			{
				if( '-' == pachAlias[nPos] )
				{
					break;
				}
				achLayer[nPos] = pachAlias[nPos];
			}
			if( 0 == nPos || nPos >= nMaxCHLen )
			{
				return bRet;
			}
			achLayer[nPos+1] = '\0';
			byLayer = atoi(achLayer);

			//��ȡ�ۺ�
			pachAlias += nPos;
			if( 0 == memcmp( pachAlias, pachSlotPrefix, strlen("-��") ) )
			{
				pachAlias += strlen("-��");
				for( nPos = 0; nPos < nMaxCHLen; nPos++ )
				{
					if( '-' == pachAlias[nPos] )
					{
						break;
					}
					achLayer[nPos] = pachAlias[nPos];
				}
				if( 0 == nPos || nPos >= nMaxCHLen )
				{
					return bRet;
				}
				achLayer[nPos+1] = '\0';
				bySlot = atoi(achLayer);
			
				//��ȡͨ����
				pachAlias += nPos;
				if( 0 == memcmp( pachAlias, pachChannelPrefix, strlen("-ͨ��") ) )
				{
					pachAlias += strlen("-ͨ��");
					memcpy( achLayer, pachAlias, nMaxCHLen );
					achLayer[nMaxCHLen] = '\0';
					byChannel = atoi(achLayer);
					bRet = TRUE;
				}
			}
		}
		return bRet;
	}
	void Print( void )
	{
		if( m_AliasType == 0 )
		{
			OspPrintf( TRUE, FALSE, "null aliase!" );
		}
		else if( m_AliasType == mtAliasTypeTransportAddress )
		{
			OspPrintf( TRUE, FALSE, "IP:0x%x, port:%d", 
				     m_tTransportAddr.GetIpAddr(), m_tTransportAddr.GetPort() );
		}		
		else if( m_AliasType == mtAliasTypeE164 )
		{
			OspPrintf( TRUE, FALSE, "E164: %s", m_achAlias ); 
		}
		else if( m_AliasType == mtAliasTypeH323ID )
		{
			OspPrintf( TRUE, FALSE, "H323ID: %s", m_achAlias ); 
		}
		else if( m_AliasType == mtAliasTypeH320ID )
		{
			OspPrintf( TRUE, FALSE, "H320ID: %s", m_achAlias ); 
		}
        else if( m_AliasType == mtAliasTypeH320Alias )
        {
            OspPrintf( TRUE, FALSE, "H320Alias: %s", m_achAlias );
        }
		else
		{
			OspPrintf( TRUE, FALSE, "Other type aliase!" ); 
		}
		OspPrintf( TRUE, FALSE, "\n" );
	}
} 
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


//�������۲����ṹ
struct TDiscussParamOld
{
public:
    u8  m_byMixMode;   //����ģʽ��mcuNoMix - ������
                       //          mcuWholeMix - ȫ�����
                       //          mcuPartMix - ���ƻ���
    u8  m_byMemberNum; //�μ����۵ĳ�Ա����(�������)
    TMtOld m_atMtMember[MAXNUM_MIXER_DEPTH];  //���۳�Ա
public:
    TDiscussParamOld(void)
    { 
        memset(this, 0, sizeof(TDiscussParamOld));
    }
    BOOL32 IsMtInMember(TMtOld tMt)
    {
        u8 byLoop = 0;
        u8 byLoopCtrl = 0;
        while (byLoop < m_byMemberNum && byLoopCtrl < MAXNUM_MIXER_DEPTH)
        {
            if (tMt == m_atMtMember[byLoop])
            {
                return TRUE;
            }

            byLoop++;
            byLoopCtrl++;
        }

        return FALSE;
    }
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

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
	void   Print( void )
	{
		OspPrintf( TRUE, FALSE, "\nConfAttrb:\n" );
		OspPrintf( TRUE, FALSE, "m_byOpenMode: %d\n", m_byOpenMode);
		OspPrintf( TRUE, FALSE, "m_byEncryptMode: %d\n", m_byEncryptMode);
		OspPrintf( TRUE, FALSE, "m_byMulticastMode: %d\n", m_byMulticastMode);
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

/*����ǽģ��ṹ�ͻ���ϳ�ģ��ṹ˵��:
Ŀǰ����ʱ����Ϣ��: TConfInfoOld + 2 byte(TMtAliasOld����������,�������n)
                  + n byte(TMtAliasOld������) + TMultiTvWallModuleOld(��ѡ) + TVmpModuleOld(��ѡ) */
//����ǽģ��ṹ
struct TTvWallModuleOld
{
	TEqpOld m_tTvWall;                                     //���ָ���ĵ���ǽ
    u8   m_abyTvWallMember[MAXNUM_PERIEQP_CHNNL_OLD];       //���ָ���ĵ���ǽ��Ա������������ʱTMtAliasOld��������+1��
  
public:
    TTvWallModuleOld(void) { memset(this, 0, sizeof(TTvWallModuleOld)); }

    TEqpOld GetTvEqp(void)
    {
        return m_tTvWall;
    }
    
    void SetTvEqp(TEqpOld tTvWall)
    {
        m_tTvWall = tTvWall;
    }
    
    void EmptyTvMember(void)
    {
        memset((void*)m_abyTvWallMember, 0, sizeof(m_abyTvWallMember));
    }

    void SetTvChannel(u8 byTvChl, u8 byMtIdx) // ����ָ��ͨ��
    {
        if(byTvChl < MAXNUM_PERIEQP_CHNNL_OLD)
        {
            m_abyTvWallMember[byTvChl] = byMtIdx;
        }
    }
    
    void RemoveTvMember(u8 byTvChl) // �Ƴ�����ǽָ��ͨ��
    {
        if(byTvChl < MAXNUM_PERIEQP_CHNNL_OLD)
        {
            m_abyTvWallMember[byTvChl] = 0;
        }
    }
    
    void RemoveMtInTvMember(u8 byMtIdx) // �Ƴ�����ǽ�����и��ն�
    {
        for(u8 byTvChlLp = 0; byTvChlLp < MAXNUM_PERIEQP_CHNNL_OLD; byTvChlLp++)
        {
            if(m_abyTvWallMember[byTvChlLp] == byMtIdx)
            {
                m_abyTvWallMember[byTvChlLp] = 0;
            }
        }
    }

    void Clear(void)
    {
        memset(&m_tTvWall, 0, sizeof(m_tTvWall));
        memset(m_abyTvWallMember, 0, sizeof(m_abyTvWallMember) );
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// ����ģ����֧�ֶ����ǽ�ṹ
struct TMultiTvWallModuleOld
{
    u8 m_byTvModuleNum; // ����ǽ��Ŀ
    TTvWallModuleOld  m_atTvWallModule[MAXNUM_PERIEQP_CHNNL_OLD];

public:

    TMultiTvWallModuleOld(void){ memset(this, 0, sizeof(TMultiTvWallModuleOld));}

    void SetTvModuleNum( u8 byTvModuleNum ) { m_byTvModuleNum = (byTvModuleNum <= MAXNUM_PERIEQP_CHNNL_OLD) ? byTvModuleNum : m_byTvModuleNum;}
    u8     GetTvModuleNum( void ) { return m_byTvModuleNum; }
    BOOL32 SetTvModule(u8 byTvModueIdx, TTvWallModuleOld m_tTvWallModule)
    {
        BOOL32 bRet = TRUE;
        if( byTvModueIdx < MAXNUM_PERIEQP_CHNNL_OLD )
        {
            m_atTvWallModule[byTvModueIdx] = m_tTvWallModule;
        }
        else
        {
            bRet = FALSE;
        }
        return bRet;
    }

    BOOL32  GetTvModuleByIdx(u8 byTvIdx, TTvWallModuleOld& tTvWallModule) // �����ȡ����ǽģ��
    {
        BOOL32 bRet = TRUE;
        if( 0 != m_byTvModuleNum && byTvIdx < MAXNUM_PERIEQP_CHNNL_OLD)
        {
            tTvWallModule = m_atTvWallModule[byTvIdx];
        }
        else
        {
            bRet = FALSE;
        }
        return bRet;
    }

    BOOL32  GetTvModuleByTvId(u8 byTvId, TTvWallModuleOld& tTvWallModule)
    {
        BOOL32 bRet = TRUE;
        if( 0 != m_byTvModuleNum )
        {
            for(u8 byTvLp = 0; byTvLp < MAXNUM_PERIEQP_CHNNL_OLD; byTvLp++)
            {
                if( byTvId == m_atTvWallModule[byTvLp].m_tTvWall.GetEqpId() )
                {
                    tTvWallModule = m_atTvWallModule[byTvLp];
                    break;
                }
            }
        }
        else
        {
            bRet = FALSE;
        }
        return bRet;
    }

    void SetTvWallMember(u8 byTvId, u8 byTvChl, u8 byMtIdx)
    {
        for(u8 byTvLp = 0; byTvLp < MAXNUM_PERIEQP_CHNNL_OLD; byTvLp++)
        {
            if(m_atTvWallModule[byTvLp].GetTvEqp().GetEqpId() == byTvId)
            {
                m_atTvWallModule[byTvLp].SetTvChannel(byTvChl, byMtIdx);
                break;
            }
        }
    }

    void RemoveMtByTvId(u8 byTvId, u8 byMtIdx )
    {
        for(u8 byTvLp = 0; byTvLp < MAXNUM_PERIEQP_CHNNL_OLD; byTvLp++)
        {
            if(m_atTvWallModule[byTvLp].GetTvEqp().GetEqpId() == byTvId)
            {
                m_atTvWallModule[byTvLp].RemoveMtInTvMember(byMtIdx);
                break;
            }
        }
    }

    void Clear(void)
    {
        m_byTvModuleNum = 0;
        memset(m_atTvWallModule, 0, sizeof(m_atTvWallModule) );
    }

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//����ϳ�ģ��ṹ 
struct TVmpModuleOld
{                        
	TVMPParamOld m_tVMPParam;                              //���ָ���Ļ���ϳɲ���
	u8        m_abyVmpMember[MAXNUM_VMP_MEMBER];        //���ָ���Ļ���ϳɳ�Ա������������ʱTMtAliasOld��������+1��

    TVmpModuleOld(void) { memset(this, 0, sizeof(TVmpModuleOld)); }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


//������鷽ʽ�ṹ(�����л�ı�)
struct TConfModeOld
{
protected:
	u8     m_byTakeMode;        //������з�ʽ: 0-ԤԼ 1-��ʱ 2-����ģ�� 
	u8     m_byLockMode;        //����������ʽ: 0-������,���л�ؿɼ��ɲ��� 1-������������˻��� 2-ĳ���������̨����
    u8     m_byCallMode;        //�����ն˷�ʽ: 0-�������նˣ��ֶ����� 1-����һ�� 2-��ʱ����δ����ն�
	u8     m_byVACMode;         //����������ʽ: 0-�����������������Ʒ���(���ݿ��Ʒ���) 1-���������������Ʒ���
	u8     m_byAudioMixMode;    //������ʽ:     0-������ 1-���ܻ���
	u8     m_byRecordMode;      //����¼��ʽ: (BIT:0-6)0-��¼�� 1-¼�� 2-¼����ͣ; BIT7 = 0 ʵʱ 1��֡¼��
	u8     m_byPlayMode;        //�������ʽ: 0-������ 1-���� 2-������ͣ 3-��� 4-����
	u8     m_byBasMode;         //�������䷽ʽ: BIT0-�Ƿ�RTPͷ���� BIT1-�Ƿ��������� BIT2-�Ƿ���������
	u8     m_byPollMode;        //������ѯ��ʽ: 0-����ѯ�㲥 1-��ͼ����ѯ�㲥 2-��������
	u8     m_byDiscussMode;     //��������ģʽ: 0-������ģʽ(�ݽ�ģʽ) 1-����ģʽ
	u8     m_byForceRcvSpeaker; //ǿ���տ�����: 0-��ǿ���տ������� 1-ǿ���տ�������
	u8     m_byNoChairMode;     //������ϯģʽ: 0-��������ϯ��ʽ 1-����ϯ��ʽ
	u8     m_byRegToGK;         //����ע��GK���: 0-����δ��GK��ע�� 1-������GK�ϳɹ�ע��
	u8     m_byMixSpecMt;		//�Ƿ�ָ���ն˻���: 0-����ָ���ն˻���(�����ն˲μӻ���) 1-ָ���ն˻���
	u8	   m_byReserved1;		//�����ֶ�
	u8	   m_byReserved2;		//�����ֶ�
	u32    m_dwCallInterval;    //��ʱ����ʱ�ĺ��м����(��λ���룬��СΪ5��)��������
	u32    m_dwCallTimes;       //��ʱ����ʱ�ĺ��д�����������
public:
	TConfModeOld( void ){ memset( this, 0, sizeof(TConfModeOld) ); }
    void   SetTakeMode(u8 byTakeMode){ m_byTakeMode = byTakeMode;} 
    u8     GetTakeMode( void ) const { return m_byTakeMode; }
    void   SetLockMode(u8   byLockMode){ m_byLockMode = byLockMode;} 
    u8     GetLockMode( void ) const { return m_byLockMode; }
    void   SetCallMode(u8   byCallMode){ m_byCallMode = byCallMode;} 
    u8     GetCallMode( void ) const { return m_byCallMode; }
	void   SetCallInterval(u32 dwCallInterval){ m_dwCallInterval = htonl(dwCallInterval);} 
    u32    GetCallInterval( void ) const { return ntohl(m_dwCallInterval); }
	void   SetCallTimes(u32 dwCallTimes){ m_dwCallTimes = htonl(dwCallTimes);} 
    u32    GetCallTimes( void ) const { return ntohl(m_dwCallTimes); }
    void   SetVACMode(BOOL bVACMode){ m_byVACMode = bVACMode;} 
    BOOL   IsVACMode( void ) const { return m_byVACMode == 0 ? FALSE : TRUE; }
    void   SetAudioMixMode(BOOL bAudioMixMode){ m_byAudioMixMode = bAudioMixMode;} 
    BOOL   IsAudioMixMode( void ) const { return m_byAudioMixMode == 0 ? FALSE : TRUE; }
	void   SetRegToGK( BOOL bRegToGK ){ m_byRegToGK = bRegToGK; }
	BOOL   IsRegToGK( void ){ return m_byRegToGK == 0 ? FALSE : TRUE; }
    void   SetRecordMode(u8   byRecordMode)
	{   byRecordMode   &= 0x7f ;
		m_byRecordMode &= 0x80 ;
		m_byRecordMode |= byRecordMode;
	} 
    u8     GetRecordMode( void ) const { return m_byRecordMode&0x7f; }
	
	//�жϵ�ǰ�Ƿ��ڳ�֡¼�����Ƿ���TRUE���򷵻�FALSE
	BOOL   IsRecSkipFrame(){ return m_byRecordMode&0x80 ? TRUE:FALSE;}
	//bSkipFrame = TRUE ���õ�ǰΪ��֡¼��,����ʵʱ¼��
	void   SetRecSkipFrame(BOOL bSkipFrame)
	{
		if(bSkipFrame)
			m_byRecordMode |=0x80;
		else 
			m_byRecordMode &=0x7f;
	}
    void   SetPlayMode( u8  byPlayMode ){ m_byPlayMode = byPlayMode;} 
    u8     GetPlayMode( void ) const { return m_byPlayMode; }
    void   SetBasMode( u8  byBasMode, BOOL32 bHasBasMode )
	{ 
		if( bHasBasMode )
		{
			m_byBasMode |= (1<<(byBasMode-1)) ;
		}
		else
		{
			m_byBasMode &= ~(1<<(byBasMode-1));
		}
	} 
    BOOL   GetBasMode( u8  byBasMode ) const 
	{ 
		return m_byBasMode & (1<<(byBasMode-1));
	}
    void   SetPollMode(u8   byPollMode){ m_byPollMode = byPollMode;} 
    u8     GetPollMode( void ) const { return m_byPollMode; }
    void   SetDiscussMode(u8   byDiscussMode){ m_byDiscussMode = byDiscussMode;} 
    BOOL   IsDiscussMode( void ) const { return m_byDiscussMode == 0 ? FALSE : TRUE; }  
    void   SetForceRcvSpeaker(u8   byForceRcvSpeaker){ m_byForceRcvSpeaker = byForceRcvSpeaker;} 
    BOOL   IsForceRcvSpeaker( void ) const { return m_byForceRcvSpeaker == 0 ? FALSE : TRUE; }
    void   SetNoChairMode(u8   byNoChairMode){ m_byNoChairMode = byNoChairMode;} 
    BOOL   IsNoChairMode( void ) const { return m_byNoChairMode == 0 ? FALSE : TRUE; }
    void   SetMixSpecMt( BOOL bMixSpecMt ){ m_byMixSpecMt = bMixSpecMt;} 
    BOOL   IsMixSpecMt( void ) const { return m_byMixSpecMt == 0 ? FALSE : TRUE; }
	void   Print( void )
	{
		OspPrintf( TRUE, FALSE, "\nConfMode:\n" );
		OspPrintf( TRUE, FALSE, "m_byTakeMode: %d\n", m_byTakeMode);
		OspPrintf( TRUE, FALSE, "m_byLockMode: %d\n", m_byLockMode);
		OspPrintf( TRUE, FALSE, "m_byCallMode: %d\n", m_byCallMode);
		OspPrintf( TRUE, FALSE, "m_dwCallInterval: %d\n", GetCallInterval());
		OspPrintf( TRUE, FALSE, "m_dwCallTimes: %d\n", GetCallInterval());
		OspPrintf( TRUE, FALSE, "m_byVACMode: %d\n", m_byVACMode);
		OspPrintf( TRUE, FALSE, "m_byAudioMixMode: %d\n", m_byAudioMixMode);
		OspPrintf( TRUE, FALSE, "m_byRecordMode: %d\n", m_byRecordMode);
		OspPrintf( TRUE, FALSE, "m_byPlayMode: %d\n", m_byPlayMode);
		OspPrintf( TRUE, FALSE, "m_byBasMode: %d\n", m_byBasMode);
		OspPrintf( TRUE, FALSE, "m_byPollMode: %d\n", m_byPollMode);
		OspPrintf( TRUE, FALSE, "m_byDiscussMode: %d\n", m_byDiscussMode);
		OspPrintf( TRUE, FALSE, "m_byForceRcvSpeaker: %d\n", m_byForceRcvSpeaker);
		OspPrintf( TRUE, FALSE, "m_byNoChairMode: %d\n", m_byNoChairMode);
		OspPrintf( TRUE, FALSE, "m_byRegToGK: %d\n", m_byRegToGK);
		OspPrintf( TRUE, FALSE, "m_byMixSpecMt: %d\n", m_byMixSpecMt);
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//�������״̬�ṹ
struct TConfStatusOld
{
public:
    TMtOld 	      m_tChairman;	      //��ϯ�նˣ�MCU��Ϊ0��ʾ����ϯ
    TMtOld		      m_tSpeaker;		  //�����նˣ�MCU��Ϊ0��ʾ�޷�����
    TConfModeOld     m_tConfMode;        //���鷽ʽ
    TRecProgOld	  m_tRecProg;		  //��ǰ¼�����,��¼��״̬ʱ��Ч
    TRecProgOld	  m_tPlayProg;	      //��ǰ�������,������״̬ʱ��Ч
    TPollInfoOld     m_tPollInfo;        //������ѯ����,����ѯʱ�н�
    TTvWallPollInfoOld     m_tTvWallPollInfo;  //����ǽ��ǰ��ѯ����������ѯʱ��Ч
    TDiscussParamOld m_tDiscussParam;    //��ǰ����������������ʱ��Ч
    TVMPParamOld     m_tVMPParam;        //��ǰ��Ƶ���ϲ���������Ƶ����ʱ��Ч
    TVMPParamOld     m_tVmpTwParam;
    u8            m_byPrs;            //��ǰ���ش�״̬

public:
    BOOL   HasChairman(void) const { return m_tChairman.GetMtId()==0 ? FALSE : TRUE; }	
	TMtOld    GetChairman(void) const { return m_tChairman; }
	void   SetNoChairman(void) { memset( &m_tChairman, 0, sizeof( TMtOld ) ); }
	void   SetChairman(TMtOld tChairman) { m_tChairman = tChairman; }
	BOOL   HasSpeaker(void) const { return m_tSpeaker.GetMtId()==0 ? FALSE : TRUE; }
    TMtOld    GetSpeaker(void) const { return m_tSpeaker; }
	void   SetNoSpeaker(void) { memset( &m_tSpeaker, 0, sizeof( TMtOld ) ); }
	void   SetSpeaker(TMtOld tSpeaker) { m_tSpeaker = tSpeaker; }
    void          SetConfMode(TConfModeOld tConfMode) { m_tConfMode = tConfMode; } 
    TConfModeOld     GetConfMode(void) const { return m_tConfMode; }
    void          SetRecProg(TRecProgOld tRecProg){ m_tRecProg = tRecProg; } 
    TRecProgOld      GetRecProg(void) const { return m_tRecProg; }
    void          SetPlayProg(TRecProgOld tPlayProg){ m_tPlayProg = tPlayProg; } 
    TRecProgOld      GetPlayProg(void) const { return m_tPlayProg; }
    void          SetVmpParam(TVMPParamOld tVMPParam){ m_tVMPParam = tVMPParam;} 
    TVMPParamOld     GetVmpParam(void) const { return m_tVMPParam; }

	//�������״̬
	BOOL IsScheduled( void ) const { return m_tConfMode.GetTakeMode() == CONF_TAKEMODE_SCHEDULED ? TRUE : FALSE; }
	void SetScheduled( void ){ m_tConfMode.SetTakeMode( CONF_TAKEMODE_SCHEDULED ); }
	BOOL IsOngoing( void ) const { return m_tConfMode.GetTakeMode() == CONF_TAKEMODE_ONGOING ? TRUE : FALSE; }
	void SetOngoing( void ){ m_tConfMode.SetTakeMode( CONF_TAKEMODE_ONGOING ); }
	BOOL IsTemplate( void ) const { return m_tConfMode.GetTakeMode() == CONF_TAKEMODE_TEMPLATE ? TRUE : FALSE; }
	void SetTemplate( void ){ m_tConfMode.SetTakeMode( CONF_TAKEMODE_TEMPLATE ); }
	u8   GetTakeMode( void ){ return m_tConfMode.GetTakeMode();	}
	void SetTakeMode( u8 byTakeMode ){ m_tConfMode.SetTakeMode( byTakeMode ); }

	//���鱣��״̬
	u8   GetProtectMode( void ) const { return m_tConfMode.GetLockMode(); }
    void SetProtectMode( u8   byProtectMode ) { m_tConfMode.SetLockMode( byProtectMode ); }
	
	//������в���
	u8   GetCallMode( void ) const { return m_tConfMode.GetCallMode(); }
	void SetCallMode( u8   byCallMode ){ m_tConfMode.SetCallMode( byCallMode ); }
	void SetCallInterval(u32 dwCallInterval){ m_tConfMode.SetCallInterval( dwCallInterval ); } 
    u32  GetCallInterval( void ) const { return m_tConfMode.GetCallInterval( ); }
	void SetCallTimes(u32 dwCallTimes){ m_tConfMode.SetCallTimes( dwCallTimes ); } 
    u32  GetCallTimes( void ) const { return m_tConfMode.GetCallTimes( ); }

    //�������״̬
    BOOL32 IsNoMixing(void) const { return !m_tConfMode.IsAudioMixMode(); }
    void   SetNoMixing(void) { m_tConfMode.SetAudioMixMode(0); }
    BOOL32 IsMixing(void) const { return m_tConfMode.IsAudioMixMode(); }
    void   SetMixing(void) { m_tConfMode.SetAudioMixMode(1); }

    //������ʽ 
    BOOL32 IsMixSpecMt(void) const { return m_tConfMode.IsMixSpecMt(); }
    void   SetMixSpecMt(BOOL bMixSpecMt) { m_tConfMode.SetMixSpecMt(bMixSpecMt); }

    //�������۷�ʽ
    BOOL32 IsDiscussMode(void) const { return m_tConfMode.IsDiscussMode(); }
    void   SetDiscussMode(BOOL bDiscussMode = TRUE) { m_tConfMode.SetDiscussMode(bDiscussMode); }

    //����������ʽ
    BOOL32 IsVACMode(void) const { return m_tConfMode.IsVACMode(); }
    void   SetVACMode(BOOL bVACMode = TRUE) { m_tConfMode.SetVACMode(bVACMode); }

    //���۲���
    BOOL32 GetMixModeParam(void) const { return m_tDiscussParam.m_byMixMode; }
    void   SetMixModeParam(u8 byMixMode) { m_tDiscussParam.m_byMixMode = byMixMode; }
    void   SetDiscussMember(TDiscussParamOld tAudMixParam)
    { memcpy(m_tDiscussParam.m_atMtMember, tAudMixParam.m_atMtMember, sizeof(tAudMixParam.m_atMtMember)); } 
    void   SetDiscussParam(TDiscussParamOld tAudMixParam){ m_tDiscussParam = tAudMixParam; } 
    TDiscussParamOld GetDiscussParam(void) const { return m_tDiscussParam; }

    //����ϳɷ�ʽ
	void   SetVMPMode(u8   byVMPMode){ m_tVMPParam.SetVMPMode(byVMPMode); } 
    u8     GetVMPMode( void ) const { return m_tVMPParam.GetVMPMode(); }
	BOOL   IsBrdstVMP( void ) const { return m_tVMPParam.GetVMPMode() != CONF_VMPMODE_NONE && m_tVMPParam.IsVMPBrdst(); }
	void   SetVmpBrdst( BOOL bBrdst ){ m_tVMPParam.SetVMPBrdst( bBrdst ); }
	u8     GetVmpStyle( void ){ return m_tVMPParam.GetVMPStyle(); }
	void   SetVmpStyle( u8 byVMPStyle ){ m_tVMPParam.SetVMPStyle( byVMPStyle ); }

    //vmp tvwall    
	void      SetVmpTwMode(u8 byVmpTwMode){ m_tVmpTwParam.SetVMPMode(byVmpTwMode); }
    u8        GetVmpTwMode(void) const { return m_tVmpTwParam.GetVMPMode(); }
    void      SetVmpTwParam(TVMPParamOld tVmpTwParam) { m_tVmpTwParam = tVmpTwParam; }
    TVMPParamOld GetVmpTwParam(void) const { return m_tVmpTwParam; }
    u8        GetVmpTwStyle(void) { return m_tVmpTwParam.GetVMPStyle(); }
	void      SetVmpTwStyle(u8 byVmpTwStyle) { m_tVmpTwParam.SetVMPStyle(byVmpTwStyle); }

	//ǿ�ƹ㲥(��ԭ��ǿ�Ʒ����˵ĸ��ͬ���ӿ�������ʱ���޸�)
	BOOL IsMustSeeSpeaker(void) const { return m_tConfMode.IsForceRcvSpeaker(); }
	void SetMustSeeSpeaker(BOOL bMustSeeSpeaker) { m_tConfMode.SetForceRcvSpeaker( bMustSeeSpeaker ); }

	//��������״̬ -- modify bas 2
    BOOL32 IsAudAdapting(void) const { return m_tConfMode.GetBasMode(CONF_BASMODE_AUD); }
    BOOL32 IsVidAdapting(void) const { return m_tConfMode.GetBasMode(CONF_BASMODE_VID); }
    BOOL32 IsBrAdapting(void) const { return m_tConfMode.GetBasMode(CONF_BASMODE_BR); }
    BOOL32 IsCasdAudAdapting(void) const { return m_tConfMode.GetBasMode(CONF_BASMODE_CASDAUD); }
    BOOL32 IsCasdVidAdapting(void) const { return m_tConfMode.GetBasMode(CONF_BASMODE_CASDVID); }
    void SetAdaptMode(u8 byBasMode, BOOL32 bAdapt)
    { 
        m_tConfMode.SetBasMode(byBasMode, bAdapt);
    }

	//������ϯ״̬
	BOOL IsNoChairMode( void ) const { return m_tConfMode.IsNoChairMode(); }
    void SetNoChairMode( BOOL bNoChairMode ) { m_tConfMode.SetNoChairMode( bNoChairMode ); }

	//��ѯ��ʽ
	void   SetPollMode(u8   byPollMode){ m_tConfMode.SetPollMode( byPollMode ); } 
    u8     GetPollMode(){ return m_tConfMode.GetPollMode(); }
	
	//��ѯ����
    void   SetPollInfo(TPollInfoOld tPollInfo){ m_tPollInfo = tPollInfo;} 
    TPollInfoOld  *GetPollInfo( void ) { return &m_tPollInfo; }
	void   SetPollMedia(u8   byMediaMode){ m_tPollInfo.SetMediaMode( byMediaMode ); } 
    u8     GetPollMedia( void ){ return m_tPollInfo.GetMediaMode(); }
	void   SetPollState(u8   byPollState){ m_tPollInfo.SetPollState( byPollState );} 
    u8     GetPollState( void ) const { return m_tPollInfo.GetPollState(); }
    void   SetMtPollParam(TMtPollParamOld tMtPollParam){ m_tPollInfo.SetMtPollParam( tMtPollParam ); } 
    TMtPollParamOld  GetMtPollParam(){ return m_tPollInfo.GetMtPollParam(); }

    //����ǽ��ѯ����
    void   SetTvWallPollInfo(TTvWallPollInfoOld tPollInfo){ m_tTvWallPollInfo = tPollInfo;} 
    TTvWallPollInfoOld  *GetTvWallPollInfo( void ) { return &m_tTvWallPollInfo; }    
    void   SetTvWallPollState(u8   byPollState){ m_tTvWallPollInfo.SetPollState( byPollState );} 
    u8     GetTvWallPollState( void ) const { return m_tTvWallPollInfo.GetPollState(); }
    void   SetTvWallMtPollParam(TMtPollParamOld tMtPollParam){ m_tTvWallPollInfo.SetMtPollParam( tMtPollParam ); } 
    TMtPollParamOld  GetTvWallMtPollParam(){ return m_tTvWallPollInfo.GetMtPollParam(); }

	//����¼��״̬	
	BOOL IsNoRecording( void ) const{ return m_tConfMode.GetRecordMode() == CONF_RECMODE_NONE ? TRUE : FALSE; }
	void SetNoRecording( void ){ m_tConfMode.SetRecordMode( CONF_RECMODE_NONE ); }
	BOOL IsRecording( void ) const{ return m_tConfMode.GetRecordMode() == CONF_RECMODE_REC ? TRUE : FALSE; }
	void SetRecording( void ){ m_tConfMode.SetRecordMode( CONF_RECMODE_REC ); }
	BOOL IsRecPause( void ) const{ return m_tConfMode.GetRecordMode() == CONF_RECMODE_PAUSE ? TRUE : FALSE; }
	void SetRecPause( void ){ m_tConfMode.SetRecordMode( CONF_RECMODE_PAUSE ); }

	//�������״̬
	BOOL IsNoPlaying( void ) const{ return m_tConfMode.GetPlayMode() == CONF_PLAYMODE_NONE ? TRUE : FALSE; }
	void SetNoPlaying( void ){ m_tConfMode.SetPlayMode( CONF_PLAYMODE_NONE ); }
	BOOL IsPlaying( void ) const { return m_tConfMode.GetPlayMode() == CONF_PLAYMODE_PLAY ? TRUE : FALSE; }
	void SetPlaying( void ){ m_tConfMode.SetPlayMode( CONF_PLAYMODE_PLAY ); }
	BOOL IsPlayPause( void )const { return m_tConfMode.GetPlayMode() == CONF_PLAYMODE_PAUSE ? TRUE : FALSE; }
	void SetPlayPause( void ){ m_tConfMode.SetPlayMode( CONF_PLAYMODE_PAUSE ); }
	BOOL IsPlayFF( void )const { return m_tConfMode.GetPlayMode() == CONF_PLAYMODE_FF ? TRUE : FALSE; }
	void SetPlayFF( void ) { m_tConfMode.SetPlayMode( CONF_PLAYMODE_FF ); }
	BOOL IsPlayFB( void )const { return m_tConfMode.GetPlayMode() == CONF_PLAYMODE_FB ? TRUE : FALSE; }
	void SetPlayFB( void ){ m_tConfMode.SetPlayMode( CONF_PLAYMODE_FB ); }

	//���ش�״̬
	void SetPrsing( BOOL bPrs ){ m_byPrs = bPrs; }
	BOOL IsPrsing( void ){ return m_byPrs; }

	//ע��GK���
	void SetRegToGK( BOOL bRegToGK ){ m_tConfMode.SetRegToGK( bRegToGK ); }
	BOOL IsRegToGK( void ){ return m_tConfMode.IsRegToGK(); }

	void Print( void )
	{
		OspPrintf( TRUE, FALSE, "\nConfStatus:\n" );
        if(m_tChairman.IsNull())  
            OspPrintf( TRUE, FALSE, "Chairman: \n" );
        else 
            OspPrintf( TRUE, FALSE, "Chairman: Mcu%dMt%d\n", m_tChairman.GetMcuId(), m_tChairman.GetMtId() );
        if(m_tSpeaker.IsNull())
            OspPrintf( TRUE, FALSE, "Speaker: \n" );
        else
		    OspPrintf( TRUE, FALSE, "Speaker: Mcu%dMt%d\n", m_tSpeaker.GetMcuId(), m_tSpeaker.GetMtId() );
        OspPrintf( TRUE, FALSE, "VmpMode: %d\n", m_tVMPParam.GetVMPMode() );
        OspPrintf( TRUE, FALSE, "TWVmpMode: %d\n", m_tVmpTwParam.GetVMPMode() );
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//������Ƶý���ʽ������(len:5)
struct TVideoStreamCapOld
{
protected:
    u16		m_wMaxBitRate;   
    u8		m_byMediaType;   
    u8      m_byResolution;	//�ֱ��� VIDEO_FORMAT_AUTO, ...
	u8      m_byFrameRate;  //֡�� MPI
    
public:
    TVideoStreamCapOld() { Clear(); }
    
    void    Clear(void)
    {
        m_wMaxBitRate = 0;
        m_byFrameRate = 0;
        m_byMediaType = MEDIA_TYPE_NULL;
        m_byResolution = VIDEO_FORMAT_CIF;
    }   
    
    void	SetMediaType(u8 byMediaType)  {  m_byMediaType = byMediaType; }	
    u8		GetMediaType() const { return m_byMediaType; }
    
    void    SetResolution(u8 byRes) { m_byResolution = byRes; }
    u8      GetResolution(void) const { return m_byResolution; }

    void    SetFrameRate(u8 byFrameRate) { m_byFrameRate = byFrameRate; }
    u8      GetFrameRate(void) const { 	return m_byFrameRate;	}
    
    void	SetMaxBitRate(u16 wMaxRate) {m_wMaxBitRate = htons(wMaxRate); }
    u16		GetMaxBitRate() const { return ntohs(m_wMaxBitRate); }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//˫����������(len:6)
struct TDStreamCapOld : public TVideoStreamCapOld
{
protected:
    u8  m_byIsH239Type;    

public:
    TDStreamCapOld() { Reset(); }

    void SetSupportH239(BOOL32 bSupport) { m_byIsH239Type = (bSupport ? 1:0); }
    BOOL32   IsSupportH239(void) { return (0 != m_byIsH239Type); }

    void Reset(void)
    {
        Clear();
        m_byIsH239Type = 0;
    }
};

//ͬʱ�������ṹ(len:6)
struct TSimCapSetOld
{
protected:
	TVideoStreamCapOld  m_tVideoCap;
	u8  m_byAudioMediaType;  

public:
    TSimCapSetOld( void )	{ Clear(); }

	void SetVideoMediaType( u8 byMediaType ) { m_tVideoCap.SetMediaType(byMediaType); }
    u8   GetVideoMediaType( void ) const { return m_tVideoCap.GetMediaType(); }

	void SetVideoResolution( u8 byRes ) { m_tVideoCap.SetResolution(byRes); }
    u8   GetVideoResolution( void ) const { return m_tVideoCap.GetResolution(); }

	void SetVideoMaxBitRate( u16 wMaxRate ) { m_tVideoCap.SetMaxBitRate(wMaxRate); }
	u16  GetVideoMaxBitRate( void ) const { return m_tVideoCap.GetMaxBitRate(); }

    void SetVideoFrameRate(u8 byFrameRate) { m_tVideoCap.SetFrameRate(byFrameRate); }
    u8   GetVideoFrameRate(void) const { return m_tVideoCap.GetFrameRate(); }
    
    void SetAudioMediaType( u8 byAudioMediaType ) { m_byAudioMediaType = byAudioMediaType;}
    u8   GetAudioMediaType( void ) const { return m_byAudioMediaType; }

    void SetVideoCap( TVideoStreamCapOld &tVidCap ){ memcpy(&m_tVideoCap, &tVidCap, sizeof(TVideoStreamCapOld)); }
    TVideoStreamCapOld   GetVideoCap( void ) const { return m_tVideoCap; }

	BOOL32   operator ==( const TSimCapSet & tSimCapSet ) const  //�ж��Ƿ����(ֻ�ж�����ͼ��)
	{
		if( GetVideoMediaType() == tSimCapSet.GetVideoMediaType() && 
			m_byAudioMediaType == tSimCapSet.GetAudioMediaType() &&
            GetVideoMediaType() != MEDIA_TYPE_NULL && 
            m_byAudioMediaType != MEDIA_TYPE_NULL)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	BOOL32   IsLike(  const TSimCapSet & tSimCapSet )  const  //�ж��Ƿ�����(ֻ�ж�����ͼ��)
	{
		if( *this == tSimCapSet )
		{
			return TRUE;
		}
		
		if( ( GetVideoMediaType() == MEDIA_TYPE_NULL && m_byAudioMediaType != MEDIA_TYPE_NULL && 
			  m_byAudioMediaType == tSimCapSet.GetAudioMediaType() ) || 
			( m_byAudioMediaType == MEDIA_TYPE_NULL && GetVideoMediaType() != MEDIA_TYPE_NULL && 
			  GetVideoMediaType() == tSimCapSet.GetVideoMediaType() ) || 
			( tSimCapSet.GetAudioMediaType() == MEDIA_TYPE_NULL && tSimCapSet.GetVideoMediaType() != MEDIA_TYPE_NULL && 
			  tSimCapSet.GetVideoMediaType() == GetVideoMediaType() ) || 
			( tSimCapSet.GetVideoMediaType() == MEDIA_TYPE_NULL && tSimCapSet.GetAudioMediaType() != MEDIA_TYPE_NULL && 
			  tSimCapSet.GetAudioMediaType() == m_byAudioMediaType ) )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	BOOL32   IsNull( void )
	{
		if(	GetVideoMediaType() == MEDIA_TYPE_NULL && m_byAudioMediaType == MEDIA_TYPE_NULL )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	void  Clear( void )
	{
        m_tVideoCap.Clear();
        m_byAudioMediaType = MEDIA_TYPE_NULL;        
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//�������ṹ(len:23)
struct TCapSupportOld
{
protected:		
	u8          m_bySupportMMcu;        //�Ƿ�֧�ֺϲ�����
    u8          m_bySupportH224;	    //�Ƿ�֧��H224
    u8          m_bySupportT120;	    //�Ƿ�֧��T120  
    u8          m_byEncryptMode;        //����ģʽCONF_ENCRYPTMODE_NONE, CONF_ENCRYPTMODE_DES, CONF_ENCRYPTMODE_AES
    u8          m_byDStreamType;        //VIDEO_DSTREAM_H263PLUS,...

    //˫������
    TDStreamCapOld   m_tDStreamCap; 

	//ͬʱ������
	TSimCapSetOld  m_tMainSimCapSet;    //����ͬʱ������
	TSimCapSetOld  m_tSecondSimCapSet;  //����ͬʱ������

public:
	//���캯��
    TCapSupportOld( void ) { Clear(); }
           
	void    SetSupportMMcu( BOOL32 bMMcu) { m_bySupportMMcu = (bMMcu ? 1:0); }
	BOOL32  IsSupportMMcu() const { return (0 != m_bySupportMMcu); }

    void    SetEncryptMode(u8 byMode) { m_byEncryptMode = byMode; }
    u8      GetEncryptMode(void) { return m_byEncryptMode; }
    
    void    SetSupportH224(BOOL32  bSupportH224){ m_bySupportH224 = (bSupportH224 ? 1:0); } 
    BOOL32  IsSupportH224( void ) { return (0 != m_bySupportH224); }  
    
    void    SetSupportT120(BOOL32   bSupportT120) { m_bySupportT120 = (bSupportT120 ? 1:0);} 
    BOOL32  IsSupportT120(void) { return (0 != m_bySupportT120); }   

	//�ڶ�·��Ƶ��˫��������������
	void    SetDStreamResolution(u8 byRes) { m_tDStreamCap.SetResolution(byRes); }
	u8      GetDStreamResolution(void) const { return m_tDStreamCap.GetResolution(); }
	
	void    SetDStreamMaxBitRate(u16 wMaxRate) { m_tDStreamCap.SetMaxBitRate(wMaxRate); }
	u16     GetDStreamMaxBitRate(void) const { return m_tDStreamCap.GetMaxBitRate(); }

    void    SetDStreamFrameRate(u8 byFrameRate) { m_tDStreamCap.SetFrameRate(byFrameRate); }
    u8      GetDStreamFrameRate(void) const { return m_tDStreamCap.GetFrameRate(); }

    void    SetDStreamType( u8 byDStreamType ); //�����ʹ��    
    u8      GetDStreamType( void ) const { return m_byDStreamType; } //�����ʹ��

	void    SetDStreamSupportH239( BOOL32 bSupport ) { m_tDStreamCap.SetSupportH239(bSupport); } //��MCU�ڲ�ʹ��
    BOOL32  IsDStreamSupportH239( void ) { return m_tDStreamCap.IsSupportH239(); } //�ڶ�·��Ƶ�Ƿ�֧��H239

    void    SetDStreamMediaType( u8 byMediaType ) { m_tDStreamCap.SetMediaType(byMediaType); } //��MCU�ڲ�ʹ��
    u8      GetDStreamMediaType( void ) const { return m_tDStreamCap.GetMediaType(); }	

    TDStreamCapOld GetDStreamCapSet( void ) const { return m_tDStreamCap; } 

	//��һ·����Ƶ����������
    void    SetMainSimCapSet( TSimCapSetOld &tSimCapSet ) { memcpy(&m_tMainSimCapSet, &tSimCapSet, sizeof(TSimCapSetOld)); }    
    TSimCapSetOld  GetMainSimCapSet( void ) const { return m_tMainSimCapSet; }

    void    SetSecondSimCapSet( TSimCapSetOld &tSimCapSet ) { memcpy(&m_tSecondSimCapSet, &tSimCapSet, sizeof(TSimCapSetOld)); }
    TSimCapSetOld  GetSecondSimCapSet( void ) const { return m_tSecondSimCapSet; }
   
	BOOL32  IsSupportMediaType( u8 byMediaType ); //��һ·����Ƶ�Ƿ�֧��ĳ��ý������	

	void Clear( void )
	{
        m_bySupportMMcu = 0;
        m_bySupportH224 = 0;
        m_bySupportT120 = 0;
        m_byEncryptMode = CONF_ENCRYPTMODE_NONE;
        m_byDStreamType = VIDEO_DSTREAM_MAIN;
        m_tMainSimCapSet.Clear(); 
        m_tSecondSimCapSet.Clear(); 
        m_tDStreamCap.Reset();
	}
	
	void Print( void )
	{	
		OspPrintf( TRUE, FALSE, "main encrypt mode:%d\n", m_byEncryptMode);
		OspPrintf( TRUE, FALSE, "main simul support:\n");
		OspPrintf( TRUE, FALSE, " VideoType :%d, Resolution :%d, Fps :%d\n", m_tMainSimCapSet.GetVideoMediaType(),
                   m_tMainSimCapSet.GetVideoResolution(), m_tMainSimCapSet.GetVideoFrameRate() );
		OspPrintf( TRUE, FALSE, " AudioType :%d\n", m_tMainSimCapSet.GetAudioMediaType() );
		OspPrintf( TRUE, FALSE, "second simul support:\n");
		OspPrintf( TRUE, FALSE, " VideoType :%d, Resolution :%d, Fps :%d\n", m_tSecondSimCapSet.GetVideoMediaType(),
                   m_tSecondSimCapSet.GetVideoResolution(), m_tSecondSimCapSet.GetVideoFrameRate());
		OspPrintf( TRUE, FALSE, " AudioType :%d\n", m_tSecondSimCapSet.GetAudioMediaType() );
        OspPrintf( TRUE, FALSE, "double stream support:\n" );
        OspPrintf( TRUE, FALSE, " VideoType :%d, Resolution :%d, Fps :%d\n", m_tDStreamCap.GetMediaType(),
                   m_tDStreamCap.GetResolution(), m_tDStreamCap.GetFrameRate());
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
    CConfIdOld       m_cConfId;                        //����ţ�ȫ��Ψһ
    TKdvTimeOld      m_tStartTime;                     //��ʼʱ�䣬����̨��0Ϊ������ʼ
    u16			     m_wDuration;                      //����ʱ��(����)��0��ʾ���Զ�ֹͣ
    u16              m_wBitRate;                       //��������(��λ:Kbps,1K=1024)
    u16              m_wSecBitRate;                    //˫�ٻ���ĵ�2����(��λ:Kbps,Ϊ0��ʾ�ǵ��ٻ���)
	u16			     m_wDcsBitRate;					   //���ݻ�������(��λ:Kbps,Ϊ0��ʾ��֧�����ݻ���)
    u16              m_wReserved;                      //����
    u8               m_byTalkHoldTime;                 //��С���Գ���ʱ��(��λ:��)
    u16              m_wMixDelayTime;                  //������ʱʱ��
    TCapSupportOld   m_tCapSupport;                    //����֧�ֵ�ý��
    s8               m_achConfPwd[MAXLEN_PWD+1];       //��������
    s8               m_achConfName[MAXLEN_CONFNAME+1]; //������
    s8               m_achConfE164[MAXLEN_E164+1];     //�����E164����
    TMtAliasOld      m_tChairAlias;                    //��������ϯ�ı���
    TMtAliasOld      m_tSpeakerAlias;                  //�����з����˵ı���
    TConfAttrbOld    m_tConfAttrb;                     //��������

    TMediaEncryptOld m_tMediaKey;                      //��һ�汾������Կһ������֧�ָ���

public:
    TConfStatusOld   m_tStatus;                        //����״̬

public:	
    TConfInfoOld( void ){ memset( this, 0, sizeof( TConfInfoOld ) ); m_tCapSupport.Clear(); }
    CConfIdOld GetConfId( void ) const { return m_cConfId; }
    void    SetConfId( CConfIdOld cConfId ){ m_cConfId = cConfId; }
    TKdvTimeOld GetKdvStartTime( void ) const { return( m_tStartTime ); }
    void   SetKdvStartTime( TKdvTimeOld tStartTime ){ m_tStartTime = tStartTime; }
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
        TSimCapSetOld tSim = m_tCapSupport.GetMainSimCapSet();
        tSim.SetVideoResolution(byVideoFormat); 
        m_tCapSupport.SetMainSimCapSet(tSim);
    } 
    u8     GetMainVideoFormat( void ) const { return m_tCapSupport.GetMainSimCapSet().GetVideoResolution(); }
    void   SetSecVideoFormat(u8 byVideoFormat)
    { 
        TSimCapSetOld tSim = m_tCapSupport.GetSecondSimCapSet();
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
    void   SetCapSupport(TCapSupportOld tCapSupport){ m_tCapSupport = tCapSupport;} 
    TCapSupportOld GetCapSupport( void ) const { return m_tCapSupport; }
    void   SetConfPwd( LPCSTR lpszConfPwd );
    LPCSTR GetConfPwd( void ) const { return m_achConfPwd; }
    void   SetConfName( LPCSTR lpszConfName );
    LPCSTR GetConfName( void ) const { return m_achConfName; }
    void   SetConfE164( LPCSTR lpszConfE164 );
    LPCSTR GetConfE164( void ) const { return m_achConfE164; }
    void   SetChairAlias(TMtAliasOld tChairAlias){ m_tChairAlias = tChairAlias;} 
    TMtAliasOld  GetChairAlias( void ) const { return m_tChairAlias; }
    void   SetSpeakerAlias(TMtAliasOld tSpeakerAlias){ m_tSpeakerAlias = tSpeakerAlias;} 
    TMtAliasOld  GetSpeakerAlias( void ) const { return m_tSpeakerAlias; }
    void   SetConfAttrb(TConfAttrbOld tConfAttrb){ m_tConfAttrb = tConfAttrb;} 
    TConfAttrbOld  GetConfAttrb( void ) const { return m_tConfAttrb; }
    void   SetStatus(TConfStatusOld tStatus){ m_tStatus = tStatus;} 
    TConfStatusOld  GetStatus( void ) const { return m_tStatus; }	

	TMediaEncryptOld& GetMediaKey(void) { return m_tMediaKey; };
	void SetMediaKey(TMediaEncryptOld& tMediaEncrypt){ memcpy(&m_tMediaKey, &tMediaEncrypt, sizeof(tMediaEncrypt));}

	void   GetVideoScale(u8 byVideoType, u16 &wVideoWidth, u16 &wVideoHeight, u8* pbyVideoResolution = NULL);
	BOOL   HasChairman( void ) const{ if(m_tStatus.m_tChairman.GetMtId()==0)return FALSE; return TRUE; }	
	TMtOld    GetChairman( void ) const{ return m_tStatus.m_tChairman; }
	void   SetNoChairman( void ){ memset( &m_tStatus.m_tChairman, 0, sizeof( TMtOld ) ); }
	void   SetChairman( TMtOld tChairman ){ m_tStatus.m_tChairman = tChairman; }
	BOOL   HasSpeaker( void ) const{ if(m_tStatus.m_tSpeaker.GetMtId()==0)return FALSE; return TRUE; }
    TMtOld    GetSpeaker( void ) const{ return m_tStatus.m_tSpeaker; }
	void   SetNoSpeaker( void ){ memset( &m_tStatus.m_tSpeaker, 0, sizeof( TMtOld ) ); }
	void   SetSpeaker( TMtOld tSpeaker ){ m_tStatus.m_tSpeaker = tSpeaker; }
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

/*-------------------------------------------------------------------
                               TMtOld                                  
--------------------------------------------------------------------*/

/*====================================================================
    ������      ��operator==
    ����        ������ID�ж��Ƿ����
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����const TMtOld & tObj, �ж϶���
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    03/01/02    1.0         LI Yi         ����
	04/06/28    3.0         ������        �޸�
====================================================================*/
inline BOOL TMtOld::operator ==( const TMtOld & tObj ) const
{
	if( tObj.GetType() != GetType() )
	{
		return FALSE;
	}

	switch( tObj.GetType() ) 
	{
	case TYPE_MCU:
		if( tObj.GetMcuId() == GetMcuId() ) 
		{
			return TRUE;
		}
		break;
	case TYPE_MCUPERI:
		if( tObj.GetMcuId() == GetMcuId() && tObj.GetEqpType() == GetEqpType() 
			&& tObj.GetEqpId() ==  GetEqpId() )
		{
			return TRUE;
		}
		break;
	case TYPE_MT:
        if( tObj.GetMcuId() == GetMcuId() && tObj.GetMtId() ==  GetMtId() 
			&& tObj.GetConfIdx() == GetConfIdx() )
		{
			return TRUE;
		}
		break;
	default:
		break;
	}

	return FALSE;
}

/*====================================================================
    ������      ��SetMcu
    ����        ������MCU
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 byMcuId, MCU��
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    2002/07/26  1.0         LI Yi         ����
    2003/10/28  3.0         ������        �޸�  
====================================================================*/
inline void TMtOld::SetMcu( u8 byMcuId )
{
	m_byMainType = TYPE_MCU;
	m_bySubType = 0;
	m_byMcuId = byMcuId;
	m_byEqpId = 0;	
	m_byConfDriId = 0;  
    m_byConfIdx = 0;    
}

/*====================================================================
    ������      ��SetMcuEqp
    ����        ������MCU����
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 byMcuId, MCU��
				  u8 byEqpId, �����
				  u8 byEqpType, ��������
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    2002/07/26  1.0         LI Yi         ����
    2003/06/06  1.0         LI Yi         �����������Ͳ���
    2003/10/28  3.0         ������        �޸�  
====================================================================*/
inline void TMtOld::SetMcuEqp( u8 byMcuId, u8 byEqpId, u8 byEqpType )
{
	m_byMainType = TYPE_MCUPERI;
	m_bySubType = byEqpType;
	m_byMcuId = byMcuId;
	m_byEqpId = byEqpId;	
	m_byConfDriId = 0;  
    m_byConfIdx = 0; 
}

/*====================================================================
    ������      ��SetMt
    ����        �������ն�
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 byMcuId, MCU��
				  u8 byMtId, �ն˺�
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    2002/07/26  1.0         LI Yi         ����
    2003/10/28  3.0         ������        �޸�  
====================================================================*/
inline void TMtOld::SetMt( u8 byMcuId, u8 byMtId, u8 byDriId, u8 byConfIdx )
{
	m_byMainType = TYPE_MT;
	m_bySubType = 0;
	m_byMcuId = byMcuId;
	m_byEqpId = byMtId;	
	m_byConfDriId = byDriId;  
    m_byConfIdx = byConfIdx; 
}

/*====================================================================
    ������      ��SetMt
    ����        �������ն�
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����TMtOld tMt 
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    2005/01/24  3.6         κ�α�        ����
====================================================================*/
inline void TMtOld::SetMt( TMtOld tMt )
{
	SetMt(tMt.GetMcuId(), tMt.GetMtId(), tMt.GetDriId(), tMt.GetConfIdx());
}

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

/*-------------------------------------------------------------------
                               TCapSupportOld                             
--------------------------------------------------------------------*/
/*=============================================================================
  �� �� ���� SetDStreamType
  ��    �ܣ� ����˫������
  �㷨ʵ�֣� 
  ȫ�ֱ����� 
  ��    ���� u8 byDStreamType
  �� �� ֵ�� inline void 
=============================================================================*/
inline void TCapSupportOld::SetDStreamType( u8 byDStreamType ) 
{ 
    m_byDStreamType = byDStreamType;
    
    switch(byDStreamType)
    {
    case VIDEO_DSTREAM_MAIN:
        m_tDStreamCap.SetMediaType(m_tMainSimCapSet.GetVideoMediaType());
        m_tDStreamCap.SetSupportH239(FALSE);
        break;
    case VIDEO_DSTREAM_H263PLUS:
        m_tDStreamCap.SetMediaType(MEDIA_TYPE_H263PLUS);
        m_tDStreamCap.SetSupportH239(FALSE);
        break;
    case VIDEO_DSTREAM_H263PLUS_H239:
        m_tDStreamCap.SetMediaType(MEDIA_TYPE_H263PLUS);
        m_tDStreamCap.SetSupportH239(TRUE);
        break;
    case VIDEO_DSTREAM_H263_H239:
        m_tDStreamCap.SetMediaType(MEDIA_TYPE_H263);
        m_tDStreamCap.SetSupportH239(TRUE);
        break;
    case VIDEO_DSTREAM_H264_H239:
        m_tDStreamCap.SetMediaType(MEDIA_TYPE_H264);
        m_tDStreamCap.SetSupportH239(TRUE);
        break;
    default:
        break;
    }
}

/*====================================================================
    ������      ��IsSupportMediaType
    ����        ���жϵ�һ·����Ƶ�Ƿ�֧��ĳ��ý������
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8   byMediaType ý������       
    ����ֵ˵��  ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    04/03/18    3.0         ������          ����             
====================================================================*/
inline BOOL32  TCapSupportOld::IsSupportMediaType( u8 byMediaType )
{
    BOOL32  bSupport = FALSE;
	switch( byMediaType ) 
	{
	case MEDIA_TYPE_H261:		
	case MEDIA_TYPE_H262:		
	case MEDIA_TYPE_H263:
	case MEDIA_TYPE_H264:		
	case MEDIA_TYPE_MP4:
//  case MEDIA_TYPE_H263PLUS:	
        bSupport = (m_tMainSimCapSet.GetVideoMediaType() == byMediaType ||
                    m_tSecondSimCapSet.GetVideoMediaType() == byMediaType);
        break;   
		
	case MEDIA_TYPE_PCMA:	
	case MEDIA_TYPE_PCMU:	
	case MEDIA_TYPE_G722:	
	case MEDIA_TYPE_G7231:	
	case MEDIA_TYPE_G728:	
	case MEDIA_TYPE_G729:	
	case MEDIA_TYPE_MP3:
        bSupport = (m_tMainSimCapSet.GetAudioMediaType() == byMediaType ||
                    m_tSecondSimCapSet.GetAudioMediaType() == byMediaType);
        break;
    default:
        break;	
	}

	return bSupport;
}

/*-------------------------------------------------------------------
                               TConfInfoOld                             
--------------------------------------------------------------------*/

/*====================================================================
    ������      ��GetVideoFormat
    ����        ����ȡͼ��ķֱ���
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 byVideoType,
                  u8 byChanNo
    ����ֵ˵��  ��ͼ��ķֱ���
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    05/09/21    4.0         libo          ����
====================================================================*/
inline u8 TConfInfoOld::GetVideoFormat(u8 byVideoType, u8 byChanNo)
{
    if (LOGCHL_VIDEO == byChanNo)
    {
        if (m_tCapSupport.GetMainSimCapSet().GetVideoMediaType() == byVideoType)
        {
            return m_tCapSupport.GetMainSimCapSet().GetVideoResolution();
        }
        else
        {
            return m_tCapSupport.GetSecondSimCapSet().GetVideoResolution();
        }
    }
    else
    {
        return m_tCapSupport.GetDStreamResolution();
    }
}

/*====================================================================
    ������      ��GetVideoScale
    ����        ���õ���Ƶ��ģ
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u16 &wVideoWidth ��Ƶ���
                  u16 &wVideoHeight ��Ƶ�߶�	              
    ����ֵ˵��  ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    03/12/05    3.0         ������          ����             
====================================================================*/
inline void  TConfInfoOld::GetVideoScale(u8 byVideoType, u16 &wVideoWidth, u16 &wVideoHeight, u8* pbyVideoResolution/* = NULL */)
{
   
    u8 byVideoFormat;
    if (m_tCapSupport.GetMainSimCapSet().GetVideoMediaType() == byVideoType)
    {
        byVideoFormat = m_tCapSupport.GetMainSimCapSet().GetVideoResolution();
    }
    else
    {
        byVideoFormat = m_tCapSupport.GetSecondSimCapSet().GetVideoResolution();
    }
    if( NULL != pbyVideoResolution )
    {
        byVideoFormat = *pbyVideoResolution;
    }

    switch (byVideoFormat)
	{
    case VIDEO_FORMAT_SQCIF:
		wVideoWidth = 128;
        wVideoHeight = 96;
    	break;
		
    case VIDEO_FORMAT_QCIF:
		wVideoWidth = 176;
        wVideoHeight = 144;
    	break;

    case VIDEO_FORMAT_CIF:
		wVideoWidth = 352;
        wVideoHeight = 288;
    	break;

    case VIDEO_FORMAT_2CIF:
        wVideoWidth = 352;
        wVideoHeight = 576;
        break;

	case VIDEO_FORMAT_4CIF:
		wVideoWidth = 704;
        wVideoHeight = 576;
		break;

	case VIDEO_FORMAT_16CIF:
		wVideoWidth = 1408;
        wVideoHeight = 1152;
		break;

    case VIDEO_FORMAT_SIF:
        wVideoWidth = 352;
        wVideoHeight = 240;
        break;

    case VIDEO_FORMAT_4SIF:
        wVideoWidth = 704;
        wVideoHeight = 480;
        break;

    case VIDEO_FORMAT_VGA:
        wVideoWidth = 640;
        wVideoHeight = 480;
        break;

    case VIDEO_FORMAT_SVGA:
        wVideoWidth = 800;
        wVideoHeight = 600;
        break;

    case VIDEO_FORMAT_XGA:
        wVideoWidth = 1024;
        wVideoHeight = 768;
        break;

	default:
		wVideoWidth = 352;
        wVideoHeight = 288;
		break;
    }

}

/*====================================================================
    ������      ��GetStartTime
    ����        ����øû��鿪ʼʱ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ��time_t����ʱ�䣨local Time��������̨��0Ϊ������ʼ
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/30    1.0         LI Yi         ����
    02/08/23    1.0         LI Yi         ��time_t��ʱ��ת��Ϊ�ַ����ͱ���
====================================================================*/
inline time_t TConfInfoOld::GetStartTime( void ) const
{
	time_t	dwStartTime;
	
	if( m_tStartTime.GetYear() != 0 )	//���Ǽ�ʱ����
		m_tStartTime.GetTime( dwStartTime );
	else
		dwStartTime = 0;

	return( dwStartTime );
}

/*====================================================================
    ������      ��SetStartTime
    ����        �����øû��鿪ʼʱ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����time_t dwStartTime, ��ʼʱ�䣬0Ϊ������ʼ
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/31    1.0         LI Yi         ����
    02/08/23    1.0         LI Yi         ��time_t��ʱ��ת��Ϊ�ַ����ͱ���
====================================================================*/
inline void TConfInfoOld::SetStartTime( time_t dwStartTime )
{
	if( dwStartTime != 0 )	//���Ǽ�ʱ����
		m_tStartTime.SetTime( &dwStartTime );
	else
		memset( &m_tStartTime, 0, sizeof( TKdvTimeOld ) );
}


/*====================================================================
    ������      ��SetConfPwd
    ����        �����ñ���
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����LPCSTR lpszConfPwd, ����
    ����ֵ˵��  ���ַ���ָ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/26    1.0         LI Yi         ����
====================================================================*/
inline void TConfInfoOld::SetConfPwd( LPCSTR lpszConfPwd )
{
	if( lpszConfPwd != NULL )
	{
		strncpy( m_achConfPwd, lpszConfPwd, sizeof( m_achConfPwd ) );
		m_achConfPwd[sizeof( m_achConfPwd ) - 1] = '\0';
	}
	else
	{
		memset( m_achConfPwd, 0, sizeof( m_achConfPwd ) );
	}
}

/*====================================================================
    ������      ��SetConfName
    ����        �����ñ���
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����LPCSTR lpszConfName, ����
    ����ֵ˵��  ���ַ���ָ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/26    1.0         LI Yi         ����
====================================================================*/
inline void TConfInfoOld::SetConfName( LPCSTR lpszConfName )
{
	if( lpszConfName != NULL )
	{
		strncpy( m_achConfName, lpszConfName, sizeof( m_achConfName ) );
		m_achConfName[sizeof( m_achConfName ) - 1] = '\0';
	}
	else
	{
		memset( m_achConfName, 0, sizeof( m_achConfName ) );
	}
}

/*====================================================================
    ������      ��SetConfE164
    ����        �����ñ���
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����LPCSTR lpszConfE164, ����
    ����ֵ˵��  ���ַ���ָ��
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/07/26    1.0         LI Yi         ����
====================================================================*/
inline void TConfInfoOld::SetConfE164( LPCSTR lpszConfE164 )
{
	if( lpszConfE164 != NULL )
	{
		strncpy( m_achConfE164, lpszConfE164, sizeof( m_achConfE164 ) );
		m_achConfE164[sizeof( m_achConfE164 ) - 1] = '\0';
	}
	else
	{
		memset( m_achConfE164, 0, sizeof( m_achConfE164 ) );
	}
}

/*-------------------------------------------------------------------
                               TMtAliasOld                             
--------------------------------------------------------------------*/
/*====================================================================
    ������      ��operator ==
    ����        ���Ƚ������ն˱����Ƿ���� 
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����void
    ����ֵ˵��  ��TRUE - ��� FALSE - �����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    2003/11/13  3.0         ������         ����
====================================================================*/
inline  BOOL   TMtAliasOld::operator ==( const TMtAliasOld & tObj ) const
{
    if( tObj.m_AliasType == 0 || tObj.m_AliasType != m_AliasType ) 
	{
		return FALSE ;
	}

	if( tObj.m_AliasType == mtAliasTypeTransportAddress )
	{
		if( (tObj.m_tTransportAddr.GetIpAddr() == m_tTransportAddr.GetIpAddr()) && 
			(tObj.m_tTransportAddr.GetPort() == m_tTransportAddr.GetPort()) && 
			(tObj.m_tTransportAddr.GetIpAddr() != 0 ))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if( (memcmp( tObj.m_achAlias, m_achAlias, sizeof( m_achAlias ) ) == 0) && 
			(m_achAlias[0] != 0) )
		{
            return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

/*====================================================================
    ������      ��SetTime
    ����        ������ʱ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����const time_t * ptTime, Ҫ���õ�ʱ��
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    03/02/07    1.0         ���ؾ�         ����
====================================================================*/
inline void TKdvTimeOld::SetTime( const time_t * ptTime )
{
	tm *ptLocalTime;
	
	ptLocalTime = ::localtime( ptTime );

	m_wYear = htons( (u16)ptLocalTime->tm_year + 1900 );
	m_byMonth = (u8)ptLocalTime->tm_mon + 1;
	m_byMDay = (u8)ptLocalTime->tm_mday;
	m_byHour = (u8)ptLocalTime->tm_hour;
	m_byMinute = (u8)ptLocalTime->tm_min;
	m_bySecond = (u8)ptLocalTime->tm_sec;
}

/*====================================================================
    ������      ��GetTime
    ����        ���õ�ʱ��ṹ
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����time_t & tTime, ���ص�ʱ��
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    03/02/07    1.0         ���ؾ�         ����
====================================================================*/
inline void TKdvTimeOld::GetTime( time_t & tTime ) const
{
	tm tTmCurTime;

	tTmCurTime.tm_year = ntohs( m_wYear ) - 1900;
	tTmCurTime.tm_mon = m_byMonth - 1;
	tTmCurTime.tm_mday = m_byMDay;
	tTmCurTime.tm_hour = m_byHour;
	tTmCurTime.tm_min = m_byMinute;
	tTmCurTime.tm_sec = m_bySecond;

	tTime = ::mktime( &tTmCurTime );
}

/*====================================================================
    ������      ��operator ==
    ����        ��==����
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����const TKdvTimeOld & tTime, �Ƚϵ�ʱ��
    ����ֵ˵��  ����
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    03/06/07    1.0         ����          ����
====================================================================*/
inline BOOL TKdvTimeOld::operator == ( const TKdvTimeOld & tTime )
{
	if( memcmp( this, &tTime, sizeof( TKdvTimeOld ) ) == 0 )
		return( TRUE );
	else
		return( FALSE );
}


//����Pack�����Ļ���洢��Ϣ 
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

//����洢��Ϣ 
struct TConfStoreOld : public TPackConfStoreOld
{
    TMtAliasOld		m_atMtAlias[MAXNUM_CONF_MT_OLD];
	u16				m_awMtDialBitRate[MAXNUM_CONF_MT_OLD];
    TMultiTvWallModuleOld m_tMultiTvWallModule;
	TVmpModuleOld	m_atVmpModule;
public:
    
    void EmptyAllTvMember(void)
    {
        for(u8 byTvLp = 0; byTvLp < MAXNUM_PERIEQP_CHNNL_OLD; byTvLp++)
        {
            m_tMultiTvWallModule.m_atTvWallModule[byTvLp].EmptyTvMember();
        }    
    }
	
    void EmptyAllVmpMember(void)
    {
        memset((void*)m_atVmpModule.m_abyVmpMember, 0, sizeof(m_atVmpModule.m_abyVmpMember));
    }
	
    void SetMtInTvChannel(u8 byTvId, u8 byChannel, u8 byMtIdx)
    {
        for(u8 byTvLp = 0; byTvLp < MAXNUM_PERIEQP_CHNNL_OLD; byTvLp++)
        {
            if(m_tMultiTvWallModule.m_atTvWallModule[byTvLp].m_tTvWall.GetEqpId() == byTvId)
            {
                m_tMultiTvWallModule.m_atTvWallModule[byTvLp].SetTvChannel(byTvLp, byMtIdx);
                break;
            }
        }
    }
    
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

#ifdef WIN32
#pragma pack( pop )
#endif

#endif //_VCCOMMON_040602_OLD_H_
