/*****************************************************************************
   ģ����      : KDV system
   �ļ���      : kdvmcugk.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: KDVRAS������ؽṹ
   ����        : 
   �汾        : V0.9  Copyright(C) 2002-2003 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2003/05/06  1.0         zhangsh        
******************************************************************************/
#include "mcustruct.h"
#include "gkmmcuerr.h"
#include "mcuconst.h"
#include "evmcugk.h"
//#include "gkconst.h"
///////////////////////////////////////////
//userage: 
//	this header file is depended on the header file listed
//		osp.h
//		mcustruct.h
//			
///////////////////////////////////////////

#ifndef _KDV_RAS_H
#define _KDV_RAS_H

#ifdef WIN32
#pragma pack( push )
#pragma pack ( 1 )
#endif

///////////////////////////////////////////
//��������
///////////////////////////////////////////
const u16 MAX_REG_ALIAS_BYTELEN = 512;	//���ı������ȣ�512���ֽ�
const u8  MAX_REG_CONFNUM = 65; //��ע�����������,MCU����һ��,64������.
//
// class CKDVAlias
// ��װЭ��ջ�����cmAlias�ṹ,���������Զ�������,��ο�mcustruct.h���������
//
// ����ʹ�õ��ǹ̶����ȵ�char���飬ʹ��ʱע��alias���Ȳ��ܳ���
// MAX_REG_ALIAS_BYTELEN
//
// 
//
class CKDVAlias
{
public:
	CKDVAlias()
	{
		Reset();
	}

	////////////////////////////////////////
	// ���
	void Reset()
	{
 		memset(&m_alias.string,0,sizeof(m_alias.string));
		memset(m_aliasString,0,sizeof(m_aliasString));
		m_alias.string = m_aliasString;
		m_alias.length = 0;
	}

	//////////////////////////////////////////
	// ����Alias(��cmAliasTypeTransportAddress��cmAliasTypePartyNumber֮��)
	// ����ֵ���ɹ�true, ����false
	BOOL SetAlias(const char * buf, int len, cmAliasType type)
		{
		assert(type!=cmAliasTypeTransportAddress && type!=cmAliasTypePartyNumber);
		if ( len > MAX_REG_ALIAS_BYTELEN )
			return FALSE;

		Reset();
		m_alias.type = type;
		memcpy(m_aliasString, buf, len);
		m_alias.length = len;

		return TRUE;
		}

	//////////////////////////////////////////
	// ����cmAliasTypeTransportAddress��Alias
	BOOL SetAlias(const cmTransportAddress & tpaddr)
		{
		if ( tpaddr.type != cmTransportTypeIP )
			return FALSE;

		Reset();
		m_alias.type = cmAliasTypeTransportAddress;
		m_alias.transport.type = tpaddr.type;
		m_alias.transport.ip = tpaddr.ip;
		m_alias.transport.port = tpaddr.port;
		m_alias.transport.distribution = tpaddr.distribution;

		return TRUE;
		}

	//////////////////////////////////////////
	// ����cmAliasTypeTransportAddress��Alias
	// @ ֻ����cmTransportTypeIP���͵�transportaddress
	// unsigned long ip : ip
	// unsigned short port : port
	BOOL SetAlias(unsigned long ip, unsigned short port, cmDistribution dsb)
		{
		Reset();
		m_alias.type = cmAliasTypeTransportAddress;
		m_alias.transport.type = cmTransportTypeIP;
		m_alias.transport.ip = (UINT32)ip;
		m_alias.transport.port = (UINT16)port;
		m_alias.transport.distribution = dsb;

		return TRUE;
		}

	///////////////////////////////////////////
	// ����cmAliasTypeTransportAddress��Alias
	// @ ֻ����cmTransportTypeIP���͵�transportaddress
	// const sockaddr_in & addr : IP address
	BOOL SetAlias(const sockaddr_in & addr, cmDistribution dsb)
		{
		Reset();
		m_alias.type = cmAliasTypeTransportAddress;
		m_alias.transport.type = cmTransportTypeIP;
		m_alias.transport.ip = (UINT32)addr.sin_addr.s_addr;
		m_alias.transport.port = (UINT16)addr.sin_port;
		m_alias.transport.distribution = dsb;

		return TRUE;
		}

	///////////////////////////////////////////
	// ����cmAliasTypePartyNumber��Alias
	// const char * buf : buffer
	// int len : ����
	// cmPartyNumberType type : type
	BOOL SetAlias(const char * buf, int len, cmPartyNumberType type)
		{
		if ( len > MAX_REG_ALIAS_BYTELEN )
			return FALSE;

		Reset();
		m_alias.type = cmAliasTypePartyNumber;
		m_alias.pnType = type;
		memcpy(m_aliasString, buf, len);
		m_alias.length = len;

		return TRUE;
		}
	
	///////////////////////////////////////
	// �����Ƿ���alias
	// ����ֵ�������alias����true, ����false
	BOOL HasAlias() const
		{
		if ( m_alias.length > 0 ) // ��cmAliasTypeTransportAddress
			return TRUE;

		return ( m_alias.type == cmAliasTypeTransportAddress
			&& m_alias.transport.ip != 0 );
		}
	///////////////////////////////////////
	// �����Ƿ���alias
	// ����ֵ�������alias����true, ����false
	BOOL IsEmpty() const
		{
		if ( m_alias.length == 0 ) // ��cmAliasTypeTransportAddress
			return TRUE;

		return ( m_alias.type == cmAliasTypeTransportAddress
			&& m_alias.transport.ip == 0 );
		}

	///////////////////////////////////////
	// �õ�cmAlias
	// ����ֵ��Alias��const����
	const cmAlias & GetcmAlias() const { return m_alias; }

	////////////////////////////////////////////
	// �õ�TransportAddress
	// @ ʹ��ʱҪע��Alias������һ��Ҫ��cmAliasTypeTransportAddress
	// ����ֵ�� cmTransportAddress��const����
	const cmTransportAddress & GetTransportAdress() const
		{
		assert(m_alias.type==cmAliasTypeTransportAddress);
		return m_alias.transport;
		}
	
	///////////////////////////////////////
	// �õ���д��cmAlias
	// ����ֵ��Alias������
	cmAlias & GetWriteablecmAlias() { return m_alias; }
	
	////////////////////////////////////////////
	// �õ���д��TransportAddress
	// @ ʹ��ʱҪע��Alias������һ��Ҫ��cmAliasTypeTransportAddress
	// ����ֵ�� cmTransportAddress������
	cmTransportAddress & GetWriteableTransportAdress()
		{
		assert(m_alias.type==cmAliasTypeTransportAddress);
		return m_alias.transport;
		}
	
	//////////////////////////////////////
	// �õ�Alias������
	// ����ֵ��Alias����cmAliasType
	cmAliasType GetType() const { return m_alias.type; }

	//////////////////////////////////////
	// �õ�PartyNumber����Alias������
	// ʹ��ʱҪע��Alias�����ͱ�����cmAliasTypePartyNumber
	// ����ֵ��PartyNumber����Alias������
	cmPartyNumberType GetPartyNumberType() const
		{
		assert(m_alias.type==cmAliasTypePartyNumber);
		return m_alias.pnType;
		}

	//////////////////////////////////////
	// ��aliasת�����ַ���
	// @ �����cmAliasTypeTransportAddress���ͣ���������100.3.1.4:1719���ַ���
	//    ���򷵻�����¼���ַ���
	// [out] char *buf : ָ��ת������ַ���
	// [in] int len : buf���ȡ������cmAliasTypeTransportAddress���ͣ�����Ӧ�ô��ڵ���22
	void ToString(char *buf, int len)
	{
		if (buf == NULL)
		{
			return;
		}

		// cmAliasTypeTransportAddress
		if ( m_alias.type == cmAliasTypeTransportAddress )
			{
			if ( len < 22 ) //" 255.255.255.255:65536" 
				{
				buf[0] = '\0';
				return;
				}
			
			in_addr Addr;
			Addr.s_addr = m_alias.transport.ip;
#ifdef WIN32
			char * psAddr = inet_ntoa(Addr);
			strcpy(buf, psAddr);
#else
#ifdef _VXWORKS_
			inet_ntoa_b(Addr,buf);
#else
#error "This version only surpport WIN32 and VXWORKS!"
#endif // _VXWORKS_
#endif // WIN32

			char sPort[16];
			sprintf(sPort,"%d",m_alias.transport.port);
			strcat(buf, sPort);
			
			return;
			}

		// other type...
		int stringLen = (len < m_alias.length) ? len : m_alias.length;
		int actLen = 0;
		int i;
		char *pPtr = m_aliasString;

		for(i=0; i<stringLen; i++, pPtr++)
		{
			if(((*pPtr)>=0x21)&&((*pPtr)<= 0x7e))
			{
				actLen += sprintf(buf+actLen, "%1c", *pPtr);
			}
			else
			{
				actLen += sprintf(buf+actLen, ".");
			} 
		}  				

		if(actLen < len)
		{
			buf[actLen] = '\0';
		}
		else
		{
			buf[len-1] = '\0';
		}
	}

	////////////////////////////////////////
	// ��ֵ
	const CKDVAlias & operator = (const CKDVAlias & alias)
	{
		if ( this != &alias )
			{
			memcpy(&m_alias,&(alias.m_alias),sizeof(m_alias));
			m_alias.string = m_aliasString;
			if ( m_alias.type != cmAliasTypeTransportAddress ) // �������ַ������������
				memcpy(&m_aliasString,&(alias.m_aliasString),sizeof(m_aliasString));
			}
		
		return *this;
	}

	/////////////////////////////////////////
	// == 
	BOOL operator==( const CKDVAlias & another ) const
		{
		if ( this == &another )
			return TRUE;
		
		if ( m_alias.type != another.m_alias.type )
			return FALSE;
		
		switch ( m_alias.type )
			{
			case cmAliasTypePartyNumber:
			case cmAliasTypeE164:
			case cmAliasTypeH323ID:
			case cmAliasTypeEndpointID:
			case cmAliasTypeGatekeeperID:
			case cmAliasTypeURLID:
			case cmAliasTypeEMailID:
				if ( m_alias.type == cmAliasTypePartyNumber
					&& m_alias.pnType != another.m_alias.pnType )
					return FALSE;
				if ( m_alias.length != another.m_alias.length )
					return FALSE;
				return !memcmp(m_aliasString, another.m_aliasString, m_alias.length);
				break;
			case cmAliasTypeTransportAddress:
				return ( m_alias.transport.type == another.m_alias.transport.type
					&&  m_alias.transport.ip == another.m_alias.transport.ip
					&&  m_alias.transport.port == another.m_alias.transport.port
					&&  m_alias.transport.distribution == another.m_alias.transport.distribution
					);
				break;
			default:
				assert(false);
				return FALSE;
				break;
			}

		}
	/////////////////////////////////////////////
	// !=
	BOOL operator!=( const CKDVAlias & another ) const
		{
		if ( this == &another )
			return FALSE;
		
		if ( m_alias.type != another.m_alias.type )
			return TRUE;
		
		switch ( m_alias.type )
			{
			case cmAliasTypePartyNumber:
			case cmAliasTypeE164:
			case cmAliasTypeH323ID:
			case cmAliasTypeEndpointID:
			case cmAliasTypeGatekeeperID:
			case cmAliasTypeURLID:
			case cmAliasTypeEMailID:
				if ( m_alias.type == cmAliasTypePartyNumber
					&& m_alias.pnType != another.m_alias.pnType )
					return TRUE;
				if ( m_alias.length != another.m_alias.length )
					return TRUE;
				return memcmp(m_aliasString, another.m_aliasString, m_alias.length);
				break;
			case cmAliasTypeTransportAddress:
				return ( m_alias.transport.type != another.m_alias.transport.type
					||  m_alias.transport.ip != another.m_alias.transport.ip
					||  m_alias.transport.port != another.m_alias.transport.port
					||  m_alias.transport.distribution != another.m_alias.transport.distribution
					);
				break;
			default:
				assert(false);
				return FALSE;
				break;
			}
		}

public:
	cmAlias m_alias; //Э��ջAlias�ṹ
	char	m_aliasString[MAX_REG_ALIAS_BYTELEN];//������ַ�ڴ���
}
#ifdef _VXWORKS_
__attribute__ ( (packed) ) 
#endif // _VXWORKS_
;

//
// class  CRegistrationRequest
// ע����������
//
class  CRegistrationRequest
{
public:
	CRegistrationRequest() 
	{
		memset(&m_addrCallAddr,0,sizeof(m_addrCallAddr));
	}
public:
	TMtAlias				m_aliasRegConf;	//ע��������
	TTransportAddr  		m_addrCallAddr;	//GKM�Ĵ����ַ
}
#ifdef _VXWORKS_
__attribute__ ( (packed) ) 
#endif // _VXWORKS_
;
//
// class CAdmissonRequest
// ��ʼ����͵�ַ��������
//
class CAdmissonRequest  //: public CRASMsg
{
public:
	CAdmissonRequest() : m_hsCall(NULL),// m_callType(cmCallTypeP2P),
					m_BandWidth(0)
	{
		memset(&m_tDstAlias,0,sizeof(m_tDstAlias));
	}
public:
	HCALL					m_hsCall;//Э��ջ���о��
	TMtAlias				m_tSrcAlias; //���еĻ������
	TMtAlias				m_tDstAlias;//���б�����ַ(destinationInfo)
	u32						m_BandWidth; // BandWidth
}
#ifdef _VXWORKS_
__attribute__ ( (packed) ) 
#endif // _VXWORKS_
;
//
// class CUnregistrationRequest
// ע���Ѿ�ע��Ļ���
//
class CUnregistrationRequest
{
public:
	CUnregistrationRequest() 
	{
		memset(&m_addrCallAddr,0,sizeof(m_addrCallAddr));
	}
public:
	TMtAlias				m_aliasRegConf;//�������
	TTransportAddr 			m_addrCallAddr;//���鴫���ַ
}
#ifdef _VXWORKS_
__attribute__ ( (packed) ) 
#endif // _VXWORKS_
;
//
// class CDisengageRequest
// �����ͷ�����
//
class CDisengageRequest  
{
public:
	CDisengageRequest() : m_hsCall(NULL)
	{
	}
public:
	HCALL					m_hsCall;//�ͷŴ�����Եĺ���
}
#ifdef _VXWORKS_
__attribute__ ( (packed) ) 
#endif // _VXWORKS_
;
//
// class CBandWidthRequest
// ����ı�����
//
class CBandWidthRequest
{
public:
	CBandWidthRequest() : m_hCall(NULL),m_BandWidth(0)
		{
		}

public:
	HCALL					m_hCall;	 //����ı���Եĺ���
	u32						m_BandWidth; // ����
}
#ifdef _VXWORKS_
__attribute__ ( (packed) ) 
#endif // _VXWORKS_
;
//
// class CLocationRequest
// ��λ����,Ŀǰ�汾��δ��
//
class CLocationRequest //: public CRASMsg
{
public:
	CLocationRequest()
		{
		memset(&m_aliasHasKnown,0,sizeof(m_aliasHasKnown));
		}

public:
	TMtAlias				m_aliasHasKnown; //�Ѿ�֪���ı��б���
}
#ifdef _VXWORKS_
__attribute__ ( (packed) ) 
#endif // _VXWORKS_
;
//
// class CLocationConfirm
// ��λȷ��,Ŀǰ�汾��δ��
//
class CLocationConfirm
{
public:
	CLocationConfirm() 
		{
		memset(&m_addrGetbyAlias,0,sizeof(m_addrGetbyAlias));
		}
	

public:
	TTransportAddr     	m_addrGetbyAlias; //���ؽ������ı��д����ַ
}
#ifdef _VXWORKS_
__attribute__ ( (packed) ) 
#endif // _VXWORKS_
;

//
// class CErrorCode
// ErrorCode
//
class CErrorCode
{
public:
	u32		m_dwErrCode; //������
}
#ifdef _VXWORKS_
__attribute__ ( (packed) ) 
#endif // _VXWORKS_
;

#ifdef WIN32
#pragma pack( pop )
#endif

#endif // _KDV_RAS_H
