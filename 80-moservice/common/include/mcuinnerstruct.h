/*****************************************************************************
   ģ����      : mcu
   �ļ���      : mcuinnerstruct.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: MCU�ڲ�ʹ�õĽṹ
   ����        : �´�ΰ
   �汾        : V4.7  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2012/05/17  4.7         chendaiei    ����
******************************************************************************/
#ifndef __MCUINNERSTRUCT_H_
#define __MCUINNERSTRUCT_H_

#include "osp.h"
#include <string.h>
#include <stdlib.h>
#include "kdvsys.h"
#include "mcuconst.h"
#include "kdvdef.h"
#include "vccommon.h"
#include "mcustruct.h"
#include "mediaapi.h"
#include <map>
#include <vector>

#ifdef WIN32
    #pragma comment( lib, "ws2_32.lib" ) 
    #pragma pack( push )
    #pragma pack( 1 )
    #define window( x )	x
#else
    #include <netinet/in.h>
    #define window( x )
#endif

#if 0
// 5.0���� ͳһ��MAXNUM_PRS_CHNNL
#if defined(_8KE_) || defined(_8KH_) || defined(_8KI_)
#define  MAXNUM_CONF_PRS_CHNNL			30				//����ʹ�õ����PRSͨ��
#else
#define  MAXNUM_CONF_PRS_CHNNL			250				//����ʹ�õ����PRSͨ��
#endif
#endif

// ��̬��Ϣ����
#define MAX_STATICMSG_LEN	255

#define MTPORT_SEM_KEY_STR				"/tmp/mtport"
#define MTPORT_SHAREMEM_KEY_STR			"/tmp/mtportshm"
#define MTLICENSE_SEM_KEY_STR			"/tmp/mtlicense"
#define MTLICENSE_SHAREMEM_KEY_STR		"/tmp/mtlicenseshm"
#define CONMAXNUM_SEM_KEY_STR			"/tmp/connum"
#define CONMAXNUM_SHAREMEM_KEY_STR		"/tmp/connumshm"

#define MEDIAPORT_SEM_KEY_STR			"/tmp/mediaport"
#define MEDIAPORT_SHAREMEM_KEY_STR		"/tmp/mediaportshm"

#define PRSPORT_SEM_KEY_STR				"/tmp/prsport"
#define PRSPORT_SHAREMEM_KEY_STR		"/tmp/prsportshm"


#define	 MAXNUM_BASBRD					3					//�㲥Basͨ������
#define  MPU2_BASCHN_MAXENCABILITY		3000				//MPU2 BAS����������
#define	 MPU2_BASCHN_DIVISION_ABILITY	1000				//MPU2 BAS�����ֶ�����(��Ӧý�ز㰴ÿ1000�����ֶ�)
#define	 MPU2_BASCHN_ENCARRAY_NUM		3					//MPU2_BASCHN_MAXENCABILITY/MPU2_BASCHN_DIVISION_ABILITY
#define  MCS_MAXNUM_PLAN                8					//MCS���Ԥ����
#define  MAXNUM_CONF_VMP                16					//����֧�ֵ����vmp����

#define  MAXNUM_EQP_ENCCHL				11					//��������������ͨ������(max(vbas/vmp/dsbas)) vmp11 vbas10 dvbas8


#if defined _LINUX
#endif

#ifdef _VXWORKS_
	#ifdef _MINIMCU_
		#define     MAXNUM_MCU_SCHEDULE_CONF         4   //ԤԼ��������ܺ� 
	#else
		#define     MAXNUM_MCU_SCHEDULE_CONF            4  //ԤԼ��������ܺ� 
	#endif 
#elif defined(_LINUX_)
	#ifdef _MINIMCU_
		#define     MAXNUM_MCU_SCHEDULE_CONF            4 //ԤԼ��������ܺ� 
	#else
		#if defined(_LINUX12_) || defined(_8KE_) || defined(_8KH_) || defined(_8KI_)
			#define     MAXNUM_MCU_SCHEDULE_CONF        16 //MPC2 //ԤԼ��������ܺ� 
		#else
			#define     MAXNUM_MCU_SCHEDULE_CONF            4  //ԤԼ��������ܺ� 
		#endif 
	#endif
#else   // WIN32
	#define     MAXNUM_MCU_SCHEDULE_CONF                4 //ԤԼ��������ܺ� 
#endif


enum emFakeMask                      //�ն����������ٵ�maskֵ
{
	emMainStreamFakedMask = 0x01,   //����Ƶ����������mask
		emDoubleStreamFakedMask = 0x02, //˫������������mask
		emAudioDescFakedMask = 0x04     //��Ƶ����������mask
};

#define IS_MAINSTREAMCAP_FAKE(byMask)   !((byMask & emMainStreamFakedMask) == 0)									
#define IS_DOUBLESTREAMCAP_FAKE(byMask)   !( (byMask & emDoubleStreamFakedMask) == 0)
#define IS_AUDIODESC_FAKE(byMask)  !((byMask & emAudioDescFakedMask) == 0)

#define SET_MAINSTREAM_FAKED(byMask)  byMask |= emMainStreamFakedMask;
#define SET_DOUBLESTREAM_FAKED(byMask)  byMask |= emDoubleStreamFakedMask;
#define SET_AUDIODESC_FAKED(byMask)  byMask |= emAudioDescFakedMask; 


/*------------------------------------------------------------- 
*�����鳣�� */
#define KEY_mcunetIsGKRRQUsePwd					(const s8*)"mcunetIsGkUseRRQPwd"
#define KEY_mcunetGKRRQPwd					    (const s8*)"mcunetGKRRQPwd"

//DMZ�������ó���
#define SECTION_ProxyLocalInfo                  (const s8*)"LocalInfo"
#define KEY_mcuUseDMZ                           (const s8*)"UseDmz"
#define KEY_mcuIpDmzAddr                        (const s8*)"IpDmzAddr"

// [pengguofeng 4/12/2013] ���뷽ʽ������������/conf/xxx.ini
#define SECTION_McuEncode						(const s8*)"mcuEncode"
#define KEY_encoding							(const s8*)"encoding"

//8KIVMP����ǰ����ͨ����Ŀ
#define		MAXNUM_8KIVMP_ADPHDCHL_2				(u8)2	//8KIVMP����ǰ����ͨ����Ŀ
#define		MAXNUM_8KIVMP_ADPHDCHL_4				(u8)4	//8KIVMP����ǰ����ͨ����Ŀ
#define		MAXNUM_8KIVMP_ADPHDCHL_9				(u8)9	//8KIVMP����ǰ����ͨ����Ŀ
#define		MAXNUM_8KIVMP_ADPHDCHL_16				(u8)16	//8KIVMP����ǰ����ͨ����Ŀ

//���崫ͳvmp����
#define		MAX_NUM_VMP								(u8)48
//����˿�vmp����
#define		MAX_NUM_PORTVMP							(u8)96
//����˿�vmp������������ٿ�����vmp�ĸ���
#define		MAX_NUM_PORTMTVMP						(u8)8

//vmpͨ���ն�����  �绰�նˣ���Ƶ�նˣ��������ն˵�
#define     VMP_MEMBERPIC_NOMT						(u8)0		//���ն�
#define     VMP_MEMBERPIC_NOSTRMMT					(u8)1		//��ͨ�ն�
#define     VMP_MEMBERPIC_ONLYAUDIOMT				(u8)2		//��Ƶ�ն�
#define     VMP_MEMBERPIC_PHONEMT					(u8)3		//�绰�ն�
#define     VMP_MEMBERPIC_NOVIDEOMT					(u8)4		//����ƵԴ�ն�
#define     VMP_MEMBERPIC_INVALID					(u8)0xff	//��Чֵ

// 4.8 acs����mcsapp������vcsapp���ӣ��ֱ�ת���ض�srcssnid������
#define ACS_MCU_DEFAULT_SRCSSNID				(u8)50		//Ĭ�ϵ�acsappsrcssnid��ʶΪ50-149
#define ACS_MCU_VCS_SRCSSNID					(u8)150		//vcsʹ�õ�app��ʶΪ150-249

struct TMixMsgInfo
{
private:
	u8 m_byMtNum;
	u8 m_byIsMMcuMix;
	u8 m_byUseEqpId;
	TMt *m_ptMtArray;
	u16 m_wErrorCode;
	u8  m_bySpecMixReplace;
	HMEDIARES m_hUserMixHandle;
	
public:
	TMixMsgInfo ( void )
	{
		//Clear(); //�������clear����,ָ���п�ʱ�Ƿ������ڴ� wsk
		memset(this,0,sizeof(*this));
		m_ptMtArray = NULL;
	}
	~TMixMsgInfo ()
	{
		Clear();
	}
	
	void Clear( void )
	{
		if ( NULL != m_ptMtArray )
		{
			delete [] m_ptMtArray;
		}
		memset(this,0,sizeof(*this));
		m_ptMtArray = NULL;
	}
	
	void SetMixMtNum ( u8 byMtNUm )
	{
		m_byMtNum = byMtNUm;
	}

	u8	GetMixMtNum ( void )
	{
		return m_byMtNum;
	}
	u8 GetEqpId(void)
	{
		return m_byUseEqpId;
	}
	void SetEqpId(u8 byEqpId)
	{
		m_byUseEqpId = byEqpId;
	}
	HMEDIARES GetMixHandle ( void )
	{
		return m_hUserMixHandle;
	}

	void SetMixHandle ( HMEDIARES dwMixHandle )
	{
		m_hUserMixHandle = dwMixHandle;
	}
	
	BOOL32 IsCascadeMixMsg ( void )
	{
		return (m_byIsMMcuMix == 1);
	}

	void SetCascadeMixMsg ( void )
	{
		m_byIsMMcuMix = 1;
	}
	
	emMcuMixMode GetMixMode ( void )
	{
		return m_byMtNum > 0 ? mcuPartMix:mcuWholeMix;
	}

	void SetMixMemberPointer ( TMt *pMt, u8 byMtNum )
	{
		if ( pMt != NULL && byMtNum > 0 )
		{
			m_ptMtArray = new TMt[byMtNum];
			memcpy( m_ptMtArray, pMt, sizeof(TMt)*byMtNum );
		}
	}
	
	TMt * GetMixMemberPointer ( void )
	{
		return m_ptMtArray;
	}

	void SetErrorCode ( u16 wErrorcode)
	{
		m_wErrorCode = wErrorcode;
	}

	u16 GetErrorCode ( void )
	{
		return m_wErrorCode;
	}

	void SetReplaceMemberFlag( u8 bySpecMixReplace)
	{
		m_bySpecMixReplace = bySpecMixReplace;
	}

	u8 GetReplaceMemberFlag ( void )
	{
		return m_bySpecMixReplace;
	}
};


struct TProxyDMZInfo
{
private:
	u8   m_byIsUseDMZ;						//�Ƿ�����DMZ
	u32  m_dwDMZIp;							//������DMZ IP���ã�
//	u8   m_byEthIndx;						//DMZ���õ�����indx
public:
	TProxyDMZInfo ( void )
	{
		memset(this,0,sizeof(*this));
	}

	~TProxyDMZInfo ( void )
	{
		memset(this,0,sizeof(*this));
	}

	void SetIsUseDMZ ( u8 byIsUseDMZ )
	{
		m_byIsUseDMZ = byIsUseDMZ;
	}

	BOOL32 IsUseDMZ ( void )
	{
		return (m_byIsUseDMZ == 1);
	}
	
	//dwIP ������
	void SetDMZIpAddr ( u32 dwIp)
	{
		m_dwDMZIp = htonl(dwIp);
	}
	
	//����������
	u32 GetDMZIpAddr ( void )
	{
		return ntohl(m_dwDMZIp);
	}

// 	void SetDMZEthIndx ( u8 byEthIndx)
// 	{
// 		m_byEthIndx = byEthIndx;
// 	}
// 	
// 	u8 GetDMZEthIndx ( void )
// 	{
// 		return m_byEthIndx;
// 	}

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//MCU����������Ϣ
//�̳���MCU general��Ϣ����չGK�û���������[3/19/2013 chendaiwei]
struct TMcuNewGeneralCfg: public TMcuGeneralCfg
{
public:
	TMcuNewGeneralCfg ( void ){memset(this,0,sizeof(*this));}
	~TMcuNewGeneralCfg (){memset(this,0,sizeof(*this));}
	void SetGkRRQUsePwdFlag( u8 byIsUseGKPwd)
    {
		m_byIsGkUseRRQPwd = byIsUseGKPwd;
    }
    
    u8	GetGkRRQUsePwdFlag() const
    {
        return m_byIsGkUseRRQPwd;
	}
	
    void SetGkRRQPassword(LPCSTR lpszAlias)
    {
        if(lpszAlias != NULL)
            strncpy(m_achRRQPassword, lpszAlias, GK_RRQ_PWD_LEN);
    }
    
    LPCSTR GetGkRRQPassword() const
    {
        return m_achRRQPassword;
	}

	void SetMcuGeneralCfg(TMcuGeneralCfg *ptGenralCfg)
	{
		if( ptGenralCfg != NULL)
		{
			memcpy(this,ptGenralCfg,sizeof(*ptGenralCfg));
		}
	}

	void SetDMZInfo ( TProxyDMZInfo *ptDMz)
	{
		if(ptDMz != NULL)
		{
			memcpy(&m_tProxyDMZInfo,ptDMz,sizeof(m_tProxyDMZInfo));
		}
	}

	TProxyDMZInfo GetDMZInfo ( void )
	{
		return m_tProxyDMZInfo;
	}

	void Print( void )
	{
		TMcuGeneralCfg::Print();
		StaticLog("Is GK RRQ Use Pwd:%d\n",m_byIsGkUseRRQPwd);
		StaticLog("GK RRQ password:%s\n",m_achRRQPassword);
		StaticLog("IsUseDMZ:%d\n",m_tProxyDMZInfo.IsUseDMZ());
		StaticLog("DMZ IP:0x%x\n",m_tProxyDMZInfo.GetDMZIpAddr());
	}

public:
	u8	 m_byIsGkUseRRQPwd;					//�Ƿ�����GK����RRQע��
	s8	 m_achRRQPassword[GK_RRQ_PWD_LEN];	//GK RRQ��Կ
	TProxyDMZInfo m_tProxyDMZInfo;
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//ÿ��ip��Ϣ
struct TNetCfg:public TIpRouteCfg
{
protected:
    s8 m_achName[MAXLEN_NOTES];		//��ע��Ϣ

public:
	TNetCfg()
    {
        Clear();
    }

	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	void SetNotes( LPCSTR lpzNotes )
	{
		if(lpzNotes != NULL )
		{
            memset( m_achName, 0, sizeof(m_achName));
			strncpy( m_achName, lpzNotes, MAXLEN_NOTES -1 );
		}
	}
	
	const s8 *GetNotes(void)
	{
		return m_achName;
	}

	void Print(void)
	{
		u32 dwIp = GetIpAddr();
		if(dwIp != 0)
		{
			u32 dwMask = GetIpMask();
			u32 dwGW = GetGwIp();
			s8	achIp[32] = {0};
			sprintf(achIp, "%d.%d.%d.%d%c", (dwIp>>24)&0xFF, (dwIp>>16)&0xFF, (dwIp>>8)&0xFF, dwIp&0xFF, 0);
			s8	achMask[32] = {0};
			sprintf(achMask, "%d.%d.%d.%d%c", (dwMask>>24)&0xFF, (dwMask>>16)&0xFF, (dwMask>>8)&0xFF, dwMask&0xFF, 0);
			s8	achGw[32] = {0};
			sprintf(achGw, "%d.%d.%d.%d%c", (dwGW>>24)&0xFF, (dwGW>>16)&0xFF, (dwGW>>8)&0xFF, dwGW&0xFF, 0);
			
			OspPrintf( TRUE, FALSE, "Notes:%s\nachIp:%s\nachMask:%s\nachGw:%s\n",m_achName,achIp,achMask,achGw);
		}		
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//ÿ����������
struct TEthCfg
{
protected:
	u32  m_dwDMZIp;								//������DMZ IP���ã�
	TNetCfg	 m_atNetCfg[MCU_MAXNUM_ADAPTER_IP];	//IP��Ŀ


public:
	TEthCfg()
    {
        Clear();
    }
	
	void Clear()
	{
		 memset(this, 0, sizeof(*this));
	}
	
	BOOL32 IsUseDMZ ( void )
	{
		return (m_dwDMZIp != 0);
	}
	
	//dwIP ������
	void SetDMZIpAddr ( u32 dwIp)
	{
		m_dwDMZIp = htonl(dwIp);
	}
	
	//����������
	u32 GetDMZIpAddr ( void )
	{
		return ntohl(m_dwDMZIp);
	}

	void SetNetCfg(u8 byIdx, const TNetCfg &tNetCfg)
	{
		if( byIdx >= MCU_MAXNUM_ADAPTER_IP )
			return;
		
		m_atNetCfg[byIdx] = tNetCfg;
	}
	
	BOOL32 GetNetCfg(u8 byIdx, TNetCfg &tNetCfg) const
	{
		tNetCfg.Clear();
		if( byIdx >= MCU_MAXNUM_ADAPTER_IP )
			return  FALSE;

		tNetCfg = m_atNetCfg[byIdx];		
		return TRUE;
	}	

	void Print(void)
	{
		s8	achDMZIp[32] = {0};
		u32 dwDMZIp = GetDMZIpAddr();
		sprintf(achDMZIp, "%d.%d.%d.%d%c", (dwDMZIp>>24)&0xFF, (dwDMZIp>>16)&0xFF, (dwDMZIp>>8)&0xFF, dwDMZIp&0xFF, 0);
		
		OspPrintf( TRUE, FALSE, "m_dwDMZIp:%s\n",achDMZIp);
		for(u8 byIdx = 0; byIdx < MCU_MAXNUM_ADAPTER_IP; byIdx++)
		{
			if(m_atNetCfg[byIdx].GetIpAddr() == 0)
			{
				continue;
			}

			OspPrintf( TRUE, FALSE, "[Ip-%d]:\n", byIdx);
			m_atNetCfg[byIdx].Print();
		}
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//�µ���������
struct TNewNetCfg
{
protected:
	enUseModeType	m_enUseModeType;	// 3����ģʽ || ����ģʽ
	enNetworkType	m_enNetworkType;    // �����ַ���ͣ�LAN or WAN 
	BOOL32			m_bIsUseSip;		//�Ƿ�����sip
	u8				m_bySipInEthIdx;	// SipIp���ĸ�������
	u32				m_dwSipIpAddr;		// SipIp
	u32				m_dwSipMaskAddr;	// SipMask
	u32				m_dwSipGwAddr;		// SipGw
	TEthCfg	m_atEthCfg[MAXNUM_ETH_INTERFACE];	//��������
	
public:
	TNewNetCfg()
    {
        Clear();
    }

	void Clear()
	{
		memset(this, 0, sizeof(*this));
		m_enNetworkType = enLAN;			//Ĭ������Ϊ˽��
		m_enUseModeType = MODETYPE_3ETH;	//Ĭ������Ϊ3����ģʽ
	}

	//ģʽ����
	void SetUseModeType (enUseModeType enType)
	{
		m_enUseModeType = enType;	
	}
	
	enUseModeType GetUseModeType( void ) const
	{
		return  m_enUseModeType;
	}

	// ������������
	void SetNetworkType(enNetworkType enType) 
    {

		m_enNetworkType = enType;

	}
	
	// ��ȡ��������
    enNetworkType GetNetworkType() const
    {
        return m_enNetworkType;
    }
	
    BOOL32 IsWan() const
    {
        return (m_enNetworkType == enWAN);
    }
	
    BOOL32 IsLan() const
    {
        return (m_enNetworkType == enLAN);
    }
	
	BOOL32 IsLanAndWan() const
    {
        return (m_enNetworkType == enBOTH);
    }

	void SetIsUseSip ( BOOL32 bIsUseSip )
	{
		m_bIsUseSip = bIsUseSip;
	}
	
	BOOL32 IsUseSip ( void ) const
	{
		return m_bIsUseSip;
	}

	void SetSipInEthIdx( u8 bySipInEthIdx )
	{
		m_bySipInEthIdx = bySipInEthIdx;
	}
	
	u8 GetSipInEthIdx ( void ) const
	{
		return m_bySipInEthIdx;
	}	
	//SipIp����
	void SetSipIpAddr(u32 dwSipIpAddr) { m_dwSipIpAddr = htonl(dwSipIpAddr); }
	u32  GetSipIpAddr()const {return ntohl(m_dwSipIpAddr); }

	void SetSipMaskAddr(u32 dwSipMaskAddr) { m_dwSipMaskAddr = htonl(dwSipMaskAddr); }
	u32  GetSipMaskAddr()const {return ntohl(m_dwSipMaskAddr); }
	
	void SetSipGwAddr(u32 dwSipGwAddr) { m_dwSipGwAddr = htonl(dwSipGwAddr); }
	u32  GetSipGwAddr()const {return ntohl(m_dwSipGwAddr); }

	//��������
	void SetEthCfg(u8 byIdx, const TEthCfg &tEthCfg)
    {
		if( byIdx >= MAXNUM_ETH_INTERFACE )
			return;
		m_atEthCfg[byIdx] = tEthCfg;
    }
	
    BOOL32 GetEthCfg(u8 byIdx, TEthCfg &tEthCfg) const
    {
		tEthCfg.Clear();
		if( byIdx >= MAXNUM_ETH_INTERFACE )
			return  FALSE;

		tEthCfg = m_atEthCfg[byIdx];
		return TRUE;
    }

	void Print(void)
	{
		s8	achSipIpAddr[32] = {0};
		u32 dwSipIpAddr = GetSipIpAddr();
		sprintf(achSipIpAddr, "%d.%d.%d.%d%c", (dwSipIpAddr>>24)&0xFF, (dwSipIpAddr>>16)&0xFF, (dwSipIpAddr>>8)&0xFF, dwSipIpAddr&0xFF, 0);

		OspPrintf( TRUE, FALSE, "\n[NewNetCfgInfo]:\nm_enUseModeType:%s\nm_enNetworkType:%s\nm_dwSipIpAddr:%s\n",	\
					(m_enUseModeType == MODETYPE_3ETH ? "MODETYPE_3ETH":"MODETYPE_BAK"),	\
					(m_enNetworkType == enLAN ? "enLAN":(m_enNetworkType == enWAN ? "enWAN":"enBOTH")),	\
					achSipIpAddr);
		for(u8 byIdx = 0; byIdx < MAXNUM_ETH_INTERFACE; byIdx++)
		{
			OspPrintf( TRUE, FALSE, "\n[8KI-Eth%d]:\n", byIdx);
			m_atEthCfg[byIdx].Print();
		}
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TPlayFileHpAttrib 
{
protected:
    emProfileAttrb m_emFirstVideoHpAttrib;
	emProfileAttrb m_emSecVideoHpAttrib;
	
public:
    TPlayFileHpAttrib(void)
    {
        m_emFirstVideoHpAttrib = emBpAttrb;
		m_emSecVideoHpAttrib = emBpAttrb;
    }
	
    void Reset()
    {
        m_emFirstVideoHpAttrib = emBpAttrb;
		m_emSecVideoHpAttrib = emBpAttrb;
    }
	
    emProfileAttrb GetFirstVideHpAttrib(void) const
    {
		//���õĲ���emHpAttrb,����Ϊ��bp
        return (ntohl(m_emFirstVideoHpAttrib) == 1) ? emHpAttrb :emBpAttrb ;
    }
    void SetFirstVideHpAttrib(emProfileAttrb emFirstVidHpAtt)
    {
        m_emFirstVideoHpAttrib = (emProfileAttrb)htonl(emFirstVidHpAtt);
    }
	
	emProfileAttrb GetSecVideHpAttrib(void) const
    {
		//���õĲ���emHpAttrb,����Ϊ��bp
        return (ntohl(m_emSecVideoHpAttrib) == 1)?emHpAttrb:emBpAttrb;
    }
    void SetSecVideHpAttrib(emProfileAttrb emSecVidHpAtt)
    {
        m_emSecVideoHpAttrib = (emProfileAttrb)htonl(emSecVidHpAtt);
    }
	
    void Print() const
    {
        StaticLog( "m_emFirstVideoHpAttrib: %d\n", ntohl(m_emFirstVideoHpAttrib));
		StaticLog( "m_emSecVideoHpAttrib: %d\n", ntohl(m_emSecVideoHpAttrib));
    }    
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

// ¼���ն�
struct TRecMtToCM : public TMtIdentifyToCM
{
	u8 m_byMtType;
	u8 m_byChnIdx;
	TRecMtToCM()
	{
		TMtIdentifyToCM::Clear();
		m_byMtType = MT_TYPE_NONE;
		m_byChnIdx = INVALID_CHNNL;
	}
	~TRecMtToCM()
	{
		
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TVrsRecChnnlStatusToCM
{
private:
	BOOL32 m_bIsRecMainVideo;
	BOOL32 m_bIsRecDStream;
	
	u8     m_byPublicMode;
	
	u8          m_byRecMode;    //¼��ʽ:

protected:
	u8		m_byVrsMtId;
	u8      m_byIsMerge;
	//֧�ֱ������ݣ�����129. 129-64+1=66
	char	m_achRecordName[MAXLEN_RECORD_NAME*2+2];//��¼��
	
public:
	u8  		m_byType;		//ͨ�����ͣ�ERecChnnlType
	u8  		m_byState;		//ͨ��״̬��ERecChnnlState
	
	TRecProg	m_tProg;		//��ǰ¼���������
	u16			m_wRecMtNum;
	TRecMtToCM  m_atRecMtInfo[MAXNUM_VMP_MEMBER];	
	u32			m_dwFileID;		//�����ļ�IDm_dwFileID
	u8   m_byIsAnonymous;     //�Ƿ�ѡ���½�ۿ�ֱ����0=��֧�� 1=֧�������ۿ�
	
public:
	TVrsRecChnnlStatusToCM(){ Clear(); }
	void Clear(){ memset(this, 0, sizeof(TVrsRecChnnlStatusToCM)); }

	void SetRecFrameMode(u8 byMode)
	{
		m_byRecMode = ((m_byRecMode & 0xFC) | (byMode & 0x03));
	}
	
	//ERecFrameMode
	u8 GetRecFrameMode()
	{
		return m_byRecMode & 0x03;
	}
	
	void SetRecMode(u8 byMode)
	{
		m_byRecMode = ((m_byRecMode & 0xF3) | ((byMode & 0x03) << 2));
	}
	
	//ERecAndLiveMode
	u8 GetRecMode(void) const
	{
		return (m_byRecMode & 0x0C) >> 2;
	}

	u8 GetVrsMtId() { return m_byVrsMtId; }
	void SetVrsMtId(u8 byVrsMtId) { m_byVrsMtId = byVrsMtId; }
	//��ȡ��¼��
	BOOL32 GetRecordName( char *szAlias, u16 wBufLen )	
	{
		if (wBufLen < KDV_NAME_MAX_LENGTH+1)
		{
			return FALSE;
		}
		strncpy( szAlias, m_achRecordName, sizeof(m_achRecordName)-1 );//�����ƽ�����
		return TRUE; 
	}
	const s8 * GetRecordName(void)
	{
		return m_achRecordName;
	}
	//���ü�¼��
	void SetRecordName( LPCSTR lpszName ) 
	{
		memset(m_achRecordName, 0, sizeof(m_achRecordName));
		if ( NULL != lpszName )
		{
			strncpy( m_achRecordName, lpszName, sizeof(m_achRecordName)-1 );
		}
		return;
	}
	
	BOOL32 IsMerge() const
	{
		return ( m_byIsMerge != 0 );
	}
	
	void SetIsMerge( BOOL32 bIsMerge )
	{
		m_byIsMerge = ( u8 )bIsMerge;
	}
	void SetPublicMode(u8 byMode)
	{
		m_byPublicMode = byMode;
	}
	void SetIsRecMainVideo(BOOL32 bIsRecMain)
	{
		m_bIsRecMainVideo = bIsRecMain;
	}
	void SetIsRecDStream(BOOL32 bIsRecD)
	{
		m_bIsRecDStream = bIsRecD;
	}
	u8 GetPublicMode(){ return m_byPublicMode;}
	BOOL32 GetIsRecMainVideo() {return m_bIsRecMainVideo;}
	BOOL32 GetIsRecDStream() {return m_bIsRecDStream;}
	void SetIsAnonymous(u8 byIsAnonymous) { m_byIsAnonymous = byIsAnonymous;}
	u8 GetIsAnonymous() { return m_byIsAnonymous;}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


// ¼���ն�
struct TRecMt : public TMt
{
	u8 m_byChnIdx;
	TRecMt()
	{
		TMt::SetNull();
		m_byChnIdx = INVALID_CHNNL;
	}
	~TRecMt()
	{

	}
}
// add pack,2014-04-14,zgc
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


struct TVrsRecChnnlStatus : public TRecChnnlStatus
{
private:
	u8 m_byIsRecMainVideo;
	u8 m_byIsRecDStream;
	u8 m_byPublishMode;
	u8 m_byIsRecMainAudio;	
	u32 m_dwStartTick;	//��ʼtick
protected:
	u8		m_byVrsMtId;
	u8      m_byIsMerge;
	//֧�ֱ������ݣ�����129. 129-64+1=66
	char	m_achRecordExName[MAXLEN_RECORD_NAME+2];//��¼��

public:
	u16     m_wRecMtNum;
	TRecMt  m_atRecMtInfo[MAXNUM_VMP_MEMBER];	
	u32 m_dwFileID;		//�����ļ�ID

	u8   m_byIsAnonymous;     //�Ƿ�ѡ���½�ۿ�ֱ����0=��֧�� 1=֧�������ۿ�

public:
	TVrsRecChnnlStatus(){ Clear(); }
	void Clear()
	{ 
		memset(this, 0, sizeof(TVrsRecChnnlStatus)); 
	}
	u8 GetVrsMtId() { return m_byVrsMtId; }
	void SetVrsMtId(u8 byVrsMtId) { m_byVrsMtId = byVrsMtId; }
	void SetRecMtNum(u16 wRecMtNum)
	{
		m_wRecMtNum = htons(wRecMtNum);
	}
	u16 GetRecMtNum(void) const {return ntohs(m_wRecMtNum);}
	void SetStartTick(u32 dwStartTick){m_dwStartTick = dwStartTick;}
	u32  GetStartTick(){return m_dwStartTick;}
	//��ȡ��¼��
	BOOL32 GetRecordName( char *szAlias, u16 wBufLen )	
	{
		if (wBufLen < KDV_NAME_MAX_LENGTH+1)
		{
			return FALSE;
		}
		strncpy( szAlias, m_achRecordName, sizeof(m_achRecordName)-1 );//�����ƽ�����
		strncpy( szAlias+sizeof(m_achRecordName)-1, m_achRecordExName, sizeof(m_achRecordExName) );
		return TRUE; 
	}
	//���ü�¼��
	void SetRecordName( LPCSTR lpszName, u16 wStrLen = 0 ) 
	{
		if (0 == wStrLen)
		{
			wStrLen = strlen(lpszName);
		}
		strncpy( m_achRecordName, lpszName, MIN(strlen(lpszName), sizeof( m_achRecordName )) );//�ȿ�ǰ��45���ַ�
		m_achRecordName[sizeof( m_achRecordName ) - 1] ='\0';//��֤�����ַ�����ȷ
		memset(m_achRecordExName, 0, sizeof(m_achRecordExName));
		if (wStrLen >= MAXLEN_RECORD_NAME)
		{
			// �ٴӵ�46���ַ���ʼ����ʣ���ַ�����m_achRecordExName��
			strncpy( m_achRecordExName, lpszName+sizeof( m_achRecordName )-1, getmin(wStrLen-sizeof( m_achRecordName )+1, sizeof(m_achRecordExName)-1));
		}
	}

	BOOL32 IsMerge() const
	{
		return ( m_byIsMerge != 0 );
	}

	void SetIsMerge( BOOL32 bIsMerge )
	{
		m_byIsMerge = ( u8 )bIsMerge;
	}
	void SetIsRecMainAudio(BOOL32 bRecAudio) { m_byIsRecMainAudio = bRecAudio;}
	BOOL32 GetIsRecMainAudio(void) const {return (m_byIsRecMainAudio == TRUE);}
	void SetIsRecMainVideo(BOOL32 bIsRecMain)
	{
		m_byIsRecMainVideo = bIsRecMain;
	}
	BOOL32 GetIsRecMainVideo(void) { return (m_byIsRecMainVideo == TRUE);}
	void SetIsRecDStream(BOOL32 brecD = TRUE)
	{
		m_byIsRecDStream = brecD;
	}
	BOOL32 IsRecDStream(void) { return (m_byIsRecDStream == TRUE);}
	void SetPublicMode(u8 byMode)
	{
		if (byMode != PUBLISH_MODE_NONE && byMode != PUBLISH_MODE_IMMED && byMode != PUBLISH_MODE_FIN && byMode != PUBLISH_MODE_BOTH)
		{
			StaticLog("error! publish mode is error\n");
			return;
		}
		m_byPublishMode = byMode;
	}
	u8 GetPublicMode(void) { return m_byPublishMode;}
	
	void SetChnnlState( u8 byState)
	{
		m_byState = byState;
	}
	u8 GetRecChnnlState(void) const
	{
		return m_byState;
	}
	void SetRecChnnlType(u8 byType) { m_byType = byType; }
	u8   GetRecChnnlType(void) const { return m_byType;}

	void SetRecSrcMt(TMt tMt) 
	{
		if (tMt.IsNull())
		{
			StaticLog("The RecSrcMt is NULL!\n");
		}
		m_tSrc = tMt;
	}
	TMt GetRecSrcMt(void) const
	{
		return m_tSrc;
	}

	void SetIsAnonymous(u8 byIsAnonymous) { m_byIsAnonymous = byIsAnonymous;}
	u8 GetIsAnonymous() { return m_byIsAnonymous;}

	void SetFileID(u32 dwFileID) { m_dwFileID = dwFileID; }
	u32   GetFileID(void) const { return m_dwFileID;}

	void print() const
	{
		StaticLog("m_bIsRecMainVideo:        %d\n", m_byIsRecMainVideo);
		StaticLog("m_bIsRecMainAudeo:        %d\n", m_byIsRecMainAudio);
		StaticLog("m_bIsRecDStream:          %d\n", m_byIsRecDStream);
		StaticLog("m_byPublishMode:           %d\n", m_byPublishMode);
		StaticLog("m_byVrsMtId:              %d\n", m_byVrsMtId);
		StaticLog("m_byIsMerge:              %d\n", m_byIsMerge);
		StaticLog("m_achRecordName:          %s\n", m_achRecordName);
		StaticLog("m_byType:                 %d\n", m_byType);
		StaticLog("m_byState:                %d\n", m_byState);
		StaticLog("RecFrameMode:              %d\n", GetRecFrameMode());
		StaticLog("m_byRecMode:              %d\n", GetRecMode());
		StaticLog("m_byIsAnonymous:           %d\n", m_byIsAnonymous);
		
		StaticLog("m_tProg:		Cur.%d Total.%d\n", m_tProg.GetCurProg(), m_tProg.GetTotalTime());
		if (m_wRecMtNum > 0)
		{
			StaticLog("Not Conf Record :\n");
			StaticLog("m_wRecMtNum:              %d\n", m_wRecMtNum);
			u8 byRecNum = 0;
			for (; byRecNum < m_wRecMtNum; byRecNum++)
			{
				StaticLog("Rec<%d> :\n", byRecNum);
				StaticLog("m_byMainType:  %d  ", m_atRecMtInfo->GetMcuType());
				StaticLog("m_bySubType:  %d  ", m_atRecMtInfo->GetEqpType());
				StaticLog("m_byMcuId:  %d  ", m_atRecMtInfo->GetMcuId());
				StaticLog("m_byEqpId:  %d  ", m_atRecMtInfo->GetEqpId());
				StaticLog("m_byConfDriId:  %d  ", m_atRecMtInfo->GetDriId());
				StaticLog("m_byConfIdx:  %d\n", m_atRecMtInfo->GetConfIdx());
			}
		}
		else
		{
			StaticLog("Conf Record \n");
		}
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TPlayEqpAttrib
{
	/*��������*/
	enum EPlayReqType
	{
		TYPE_NOREQ		= 0,
		TYPE_FILELIST	= 1,
		TYPE_STARTPLAY	= 2
	};
protected:
    u8 m_byIsDStreamPlay;
// 	u8 m_byAudTrackNum;
	s8 m_szRecName[255];
// 	u8 m_byNackTryNum;
// 	TEqp m_tEqp;
	u8 m_byReqType;//������Ϣ����(0������/1�ļ�����/2��������)
// 	u32 m_dwGroupId;//����id
// 	u32 m_dwPageNo;//�ڼ�ҳ
// 	u32 m_dwListNum;//��ҳ����
	u32 m_dwFileId;//�����ļ����ļ�ID,����
	//s8 m_szVrsPlayData[135];//���濪������������ݣ�vrsʵ�����ߺ���Ϣ�ã���������ʶ�����޸�,Ŀǰ����128+u32=132
	//u16 m_wVrsPlayDataLen;
	u16 m_wMStremBR;//��¼�������ʣ���ͣ����ʱ��Flowcontrol 0���ָ�ʱ����������Ϣ
// 	u8  m_byListRecordSrcSsnId;//��¼��ǰ�����ļ��б��
	
public:
    TPlayEqpAttrib(void)
    {
		memset(this, 0, sizeof(TPlayEqpAttrib));
// 		m_tEqp.SetNull();
    }
	
    void Reset()
    {
		memset(this, 0, sizeof(TPlayEqpAttrib));
//         m_byFileType = RECFILE_NORMAL;
//         m_byAudioType = MEDIA_TYPE_NULL;
//         m_byVideoType = MEDIA_TYPE_NULL;
//         m_byDVideoType = MEDIA_TYPE_NULL;
// 		m_tEqp.SetNull();
	}
	
    BOOL32 IsDStreamPlay(void) const
    {
        return (m_byIsDStreamPlay == 1) ? TRUE : FALSE;
    }
    void SetDStreamPlay(u8 byIsDStreamPlay)
    {
        m_byIsDStreamPlay = byIsDStreamPlay;
    }

// 	u8 GetAudTrackNum(void) const
//     {
//         return m_byAudTrackNum;
//     }
//     void SetAudTrackNum(u8 byAudTrackNum)
//     {
//         m_byAudTrackNum = byAudTrackNum;
//     }

	s8* GetRecName(void) 
    {
        return &m_szRecName[0];
    }
    void SetRecName(s8* pRecName)
    {
        u16 wStrLen = strlen(pRecName);
		if( wStrLen <= 255 )
		{
			strncpy( m_szRecName,pRecName,255 - 1 );
		}
    }
	
// 	u8 GetNackTryNum(void) const
//     {
//         return m_byNackTryNum;
//     }
// 
//     void SetNackTryNum(u8 byNackTryNum)
//     {
//         m_byNackTryNum = byNackTryNum;
//     }
	
// 	u8 GetListRecordSrcSsnId(void) const
//     {
//         return m_byListRecordSrcSsnId;
//     }
// 	
//     void SetListRecordSrcSsnId(u8 byListRecordSrcSsnId)
//     {
//         m_byListRecordSrcSsnId = byListRecordSrcSsnId;
//     }

// 	TEqp GetEqp(void) 
//     {
//         return m_tEqp;
//     }
// 
//     void SetEqp(TEqp tEqp)
//     {
//         m_tEqp = tEqp;
//     }
	
	u8 GetReqType(void) const
    {
        return m_byReqType;
    }
    void SetReqType(u8 byReqType)
    {
        m_byReqType = byReqType;
    }
	
// 	u32 GetGroupId(void) const
//     {
//         return ntohl(m_dwGroupId);
//     }
//     void SetGroupId(u32 dwGroupId)
//     {
//         m_dwGroupId = htonl(dwGroupId);
//     }
	
// 	u32 GetPageNo(void) const
//     {
//         return ntohl(m_dwPageNo);
//     }
//     void SetPageNo(u32 dwPageNo)
//     {
//         m_dwPageNo = htonl(dwPageNo);
//     }
	
// 	u32 GetListNum(void) const
//     {
//         return ntohl(m_dwListNum);
//     }
//     void SetListNum(u32 dwListNum)
//     {
//         m_dwListNum = htonl(dwListNum);
//     }
	
	u32 GetFileId(void) const
    {
        return ntohl(m_dwFileId);
    }
    void SetFileId(u32 dwFileId)
    {
        m_dwFileId = htonl(dwFileId);
    }
	
	u16 GetMStremBR(void) const
    {
        return ntohs(m_wMStremBR);
    }
    void SetMStremBR(u16 wMStremBR)
    {
        m_wMStremBR = htons(wMStremBR);
    }
	
    void Print() const
    {
//         TPlayFileAttrib::Print();
        StaticLog( "m_byIsDStreamPlay: %d\n", m_byIsDStreamPlay);
// 		StaticLog( "m_byAudTrackNum: %d\n", m_byAudTrackNum);
		StaticLog( "m_szRecName: %s\n", m_szRecName);
// 		StaticLog( "m_byNackTryNum: %d\n", m_byNackTryNum);
// 		StaticLog( "m_tEqp: %d.%d\n", m_tEqp.GetMcuId(),m_tEqp.GetMtId() );
		StaticLog( "m_byReqType: %d\n", m_byReqType);
// 		StaticLog( "m_dwGroupId: %d\n", GetGroupId());
// 		StaticLog( "m_dwPageNo: %d\n", GetPageNo());
// 		StaticLog( "m_dwListNum: %d\n", GetListNum());
		StaticLog( "m_dwFileId: %d\n", GetFileId());
		StaticLog( "m_wMStremBR: %d\n", GetMStremBR());
// 		StaticLog( "m_byListRecordSrcSsnId: %d\n", m_byListRecordSrcSsnId);
    }    
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;


//���廭��ϳɺ������б� (len:35)
struct TKDVVMPOutParam
{
protected:
	u8				m_abyVmpEncIdx[MAXNUM_MPU2_OUTCHNNL];	//����ͨ��(��¼��Դ���Ǳߵ�idx)
	TVideoStreamCap m_atVmpOutMember[MAXNUM_MPU2_OUTCHNNL];	//��������
	u16				m_awPort[MAXNUM_MPU2_OUTCHNNL];			//����˿�
public:
	void Initialize()
	{
		for (u8 byIdx=0;byIdx<MAXNUM_MPU2_OUTCHNNL;byIdx++)
		{
			m_abyVmpEncIdx[byIdx] = byIdx;	//yrl_todo
			m_atVmpOutMember[byIdx].Clear();
			m_awPort[byIdx] = 0;
		}
	}
	TKDVVMPOutParam()
	{
		Initialize();
	}
	
	//����vmp����idx
	BOOL32 SetVmpEncIdx(u8 byIdx, u8 byVmpEncIdx)
	{
		if (byIdx >= MAXNUM_MPU2_OUTCHNNL)
		{
			return FALSE;
		}
		
		m_abyVmpEncIdx[byIdx] = byVmpEncIdx;
		return TRUE;
	}
	
	//����ָ��ͨ����idx
	u8 GetVmpEncIdx(u8 byIdx) const
	{
		if (byIdx >= MAXNUM_MPU2_OUTCHNNL)
		{
			return INVALID_CHNNL;
		}
		
		return m_abyVmpEncIdx[byIdx];
	}

	//����ָ��ͨ��������
	TVideoStreamCap GetVmpOutCapIdx(u8 byIdx) const
	{
		return m_atVmpOutMember[byIdx];
	}
	
	//���غ�����ͨ������
	u8 GetVmpOutCount() const
	{
		u8 byCount = 0;
		for (u8 byIndx = 0; byIndx<MAXNUM_MPU2_OUTCHNNL; byIndx++)
		{
			if (MEDIA_TYPE_NULL == m_atVmpOutMember[byIndx].GetMediaType())
			{
				continue;
			}
			byCount++;
		}
		return byCount;
	}

	/*==============================================================================
	������    :  GetCorrectChnnlByStrCap
	����      :  ���������������ʵ�mpu2������ͨ����(���ڻ�С������ӽ���������ͨ����)���hdu
	�㷨ʵ��  :  
	����˵��  :  TVideoStreamCap &tStrCap	[in]
				 u8 byOnlyAttrb	[in] �޶�ֻ����BP��HP���͵�ͨ��
	����ֵ˵��:  u8 ͨ����
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��       �汾          �޸���          �߶���          �޸ļ�¼
	2011-12-5				yanghuaizhi
	==============================================================================*/
	u8 GetCorrectChnnlByStrCap(const TVideoStreamCap &tStrCap, u8 byOnlyAttrb = 0xFF) const
	{
		u8 byChnNum = ~0;
		u8 byRes = tStrCap.GetResolution();
		u8 byFps = tStrCap.GetUserDefFrameRate();
		TVideoStreamCap tTmpCap;
		//��֧��H264
		if (MEDIA_TYPE_H264 != tStrCap.GetMediaType())
		{
			return byChnNum;
		}
		// ���Һ�������
		for (u8 byNum=0; byNum<MAXNUM_MPU2_OUTCHNNL; byNum++)
		{
			//����δ�ҵ�
			if (MEDIA_TYPE_H264 != m_atVmpOutMember[byNum].GetMediaType())
			{
				//�Ƿ�264��ʽ��ͨ��
				if (MEDIA_TYPE_NULL != m_atVmpOutMember[byNum].GetMediaType())
				{
					byChnNum = byNum;
				}
				break;
			}
			tTmpCap = m_atVmpOutMember[byNum];
			//Bug00166445 1080/30BPȫ������飬HD3��ᣬ����ϳ�ȡ���㲥�ٹ�ѡ�㲥������ϳɷ��ʹ�������
			//��������Ϊ0��ͨ����8kivmp��0·������ʱ����Ϊ0
			//�������ҵ�������� �˴���������
// 			if (tTmpCap.GetMaxBitRate() == 0)
// 			{
// 				continue;
// 			}
			//ֻ����HP��BPʱ,������ͬ��ʽ,֧��ֻƥ��BP��HP
			if (0XFF != byOnlyAttrb && byOnlyAttrb != tTmpCap.GetH264ProfileAttrb())
			{
				continue;
			}
			// �ҵ�һ��С�ڵ��ڴ�������ͨ��
			if ((tTmpCap.IsH264CapEqual(byRes,byFps) && (tStrCap.IsSupportHP() || !tTmpCap.IsSupportHP()))
				|| tTmpCap.IsH264CapLower(byRes,byFps))//�ҵ�һ������С�ڵ���tStrCap��
			{
				byChnNum = byNum;
				break;
			}
		}
		
		return byChnNum;
	}

	/*==============================================================================
	������    :  GetVmpOutChnnlByStrCap
	����      :  ����������ö�Ӧmpu2������ͨ����
	�㷨ʵ��  :  
	����˵��  :  TVideoStreamCap &tStrCap	[in]
	����ֵ˵��:  u8 ͨ����
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��       �汾          �޸���          �߶���          �޸ļ�¼
	2011-12-5				yanghuaizhi
	==============================================================================*/
	u8 GetVmpOutChnnlByStrCap(const TVideoStreamCap &tStrCap) const
	{
		u8 byChnNum = ~0;
		BOOL32 bIs1080P = FALSE;
		// 1080��Ҫ��֡��,�����ʽ������Ҫ�Ƚ�֡��
		if (MEDIA_TYPE_H264 == tStrCap.GetMediaType() &&
			VIDEO_FORMAT_HD1080 == tStrCap.GetResolution())
		{
			bIs1080P = TRUE;
		}

		u16 byTmpRate    = ~0;	//�������ʲ�
		u16 byMinTmpRate = ~0;	//������С���ʲ�
		u16 byTmpCapRate =  0;	//ͨ������
		u16 byStrCapRate =  0;  //�ն�����
		TVideoStreamCap tTmpCap;
		// �����������
		for (u8 byNum=0; byNum<MAXNUM_MPU2_OUTCHNNL; byNum++)
		{
			//����δ�ҵ�
			if (MEDIA_TYPE_NULL == m_atVmpOutMember[byNum].GetMediaType())
			{
				break;
			}
			byTmpRate = ~0;
			tTmpCap = m_atVmpOutMember[byNum]; 
			if (tTmpCap.GetMediaType() == tStrCap.GetMediaType() &&
				tTmpCap.GetResolution() == tStrCap.GetResolution() &&
				(!bIs1080P || tTmpCap.GetUserDefFrameRate() == tStrCap.GetUserDefFrameRate()) &&
				(tTmpCap.IsSupportHP() == tStrCap.IsSupportHP()))
			{
				byTmpCapRate = tTmpCap.GetMaxBitRate();
				byStrCapRate = tStrCap.GetMaxBitRate();
				
				//ȡ��
				byTmpRate = (byTmpCapRate - byStrCapRate) >= 0 ? (byTmpCapRate - byStrCapRate) : (byStrCapRate - byTmpCapRate);
				//������Сֵ	
				if (byTmpRate <= byMinTmpRate)
				{
					byMinTmpRate = byTmpRate;
					byChnNum = byNum;
				}
				
			}
		}

		return byChnNum;
	}
	
	/*==============================================================================
	������    :  GetStrCapByVmpOutChnnl
	����      :  ����mpu2������ͨ���Ż�ö�Ӧ����
	�㷨ʵ��  :  
	����˵��  :  u8 byChnnl		[in]
				 TVideoStreamCap &tStrCap	[out]
	����ֵ˵��:  BOOL32
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��       �汾          �޸���          �߶���          �޸ļ�¼
	2011-12-5				yanghuaizhi
	==============================================================================*/
	BOOL32 GetStrCapByVmpOutChnnl(u8 byChnnl, TVideoStreamCap &tStrCap)
	{
		if (MEDIA_TYPE_NULL == m_atVmpOutMember[byChnnl].GetMediaType() || MAXNUM_MPU2_OUTCHNNL <= byChnnl)
		{
			LogPrint(LOG_LVL_ERROR, MID_MCU_VMP, "[GetStrCapByVmpOutChnnl]Err,the channel is null.\n");
			return FALSE;
		}

		tStrCap = m_atVmpOutMember[byChnnl];
		return TRUE;
	}
	
	//���vmp����������������ʵıȽϣ�
	BOOL32 AddVmpOutCap(const TVideoStreamCap &tStrCap)
	{
		// ������Idx���������ͨ���б�
		for (u8 byNum = 0; byNum < MAXNUM_MPU_OUTCHNNL; byNum++)
		{
			// ��Idx�Ѿ����б���
			if (m_atVmpOutMember[byNum] == tStrCap)
			{
				//��ͬ��������ͬ��������
				if(m_atVmpOutMember[byNum].GetMaxBitRate() != tStrCap.GetMaxBitRate())
				{
					continue;
				}

				LogPrint(LOG_LVL_WARNING, MID_MCU_VMP, "[AddVmpOutMember]Warning, this CapIdx is already exists.\n");
				return FALSE;	
			}
			// �ҵ���λ,�������
			else if (MEDIA_TYPE_NULL == m_atVmpOutMember[byNum].GetMediaType())
			{
				m_atVmpOutMember[byNum] = tStrCap;
				return TRUE;
			}	
		}

		LogPrint(LOG_LVL_ERROR, MID_MCU_VMP, "[AddVmpOutMember]Err,Add Member failed because channel is full.\n");
		return FALSE;
	}

	/*==============================================================================
	������    :  AddVmpOutMember
	����      :  ��Ӻ�����ͨ��
	�㷨ʵ��  :  
	����˵��  :  TVideoStreamCap &tStrCap		[in]
			 
	����ֵ˵��:  BOOL32
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��       �汾          �޸���          �߶���          �޸ļ�¼
	2011-12-5				yanghuaizhi
	==============================================================================*/
	BOOL32 AddVmpOutMember(const TVideoStreamCap &tStrCap)
	{
		// ������Idx���������ͨ���б�
		for (u8 byNum=0; byNum<MAXNUM_MPU2_OUTCHNNL; byNum++)
		{
			// ��Idx�Ѿ����б���
			if (m_atVmpOutMember[byNum] == tStrCap)
			{
				LogPrint(LOG_LVL_WARNING, MID_MCU_VMP, "[AddVmpOutMember]Warning, this CapIdx is already exists.\n");
				return FALSE;
			}
			// �ҵ���λ,�������
			else if (MEDIA_TYPE_NULL == m_atVmpOutMember[byNum].GetMediaType())
			{
				m_atVmpOutMember[byNum] = tStrCap;
				return TRUE;
			}	
		}
		LogPrint(LOG_LVL_ERROR, MID_MCU_VMP, "[AddVmpOutMember]Err,Add Member failed because channel is full.\n");
		return FALSE;
	}
	
	//����ָ��ͨ������
	BOOL32 UpdateVmpOutMember(u8 byNum, const TVideoStreamCap &tStrCap) 
	{ 
		m_atVmpOutMember[byNum] = tStrCap; 
		return TRUE;
	}

	// ���ָ��ͨ������,����MPU2 BASIC vicp��Դ����ʱ,���1080p60fps
	void ClearOneVmpOutChnnl(u8 byIdx)
	{
		if (byIdx >= MAXNUM_MPU2_OUTCHNNL)
		{
			return;
		}
		// ������ĳ�Ա��Ǩ��
		for (u8 byTmpIdx=byIdx+1; byTmpIdx < MAXNUM_MPU2_OUTCHNNL; byTmpIdx++)
		{
			m_atVmpOutMember[byTmpIdx-1] = m_atVmpOutMember[byTmpIdx];
		}
		// ������һ��ͨ��
		m_atVmpOutMember[MAXNUM_MPU2_OUTCHNNL-1].Clear();

	}

	//����vmp����˿�
	BOOL32 SetVmpEncPort(u8 byIdx, u16 wVmpEncPort)
	{
		if (byIdx >= MAXNUM_MPU2_OUTCHNNL)
		{
			return FALSE;
		}
		
		m_awPort[byIdx] = htons(wVmpEncPort);
		return TRUE;
	}
	
	//����ָ��ͨ���Ķ˿�
	u16 GetVmpEncPort(u8 byIdx) const
	{
		if (byIdx >= MAXNUM_MPU2_OUTCHNNL)
		{
			return 0;
		}
		
		return ntohs(m_awPort[byIdx]);
	}

	void print(void)//FIXME
	{
		u8 byOutNum = GetVmpOutCount();
		StaticLog("Vmp Out Chnnl Num: %u\n", byOutNum);
		StaticLog("---------Vmp Out Channel Members------------\n");
		for(u8 byLoop = 0; byLoop < MAXNUM_MPU2_OUTCHNNL; byLoop++)
		{
			if (MEDIA_TYPE_NULL == m_atVmpOutMember[byLoop].GetMediaType())
			{
				continue;
			}
			StaticLog("Channel:%u ", byLoop);
			StaticLog("Idx:%d ", m_abyVmpEncIdx[byLoop]);
			StaticLog("Port:%d ", GetVmpEncPort(byLoop));
			m_atVmpOutMember[byLoop].Print();
		}
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct THduKey
{
	u8 m_byHduId;	//����ǽid
	u8 m_byHduChnId; //����ǽ��ͨ��ID
	
public:
	THduKey()
	{
		clear();
	}
	void SetHduKeyInfo(u8 byHduId, u8 byHduChnId)
	{
		if (byHduId < HDUID_MAX && byHduChnId < HDU_MODEFOUR_MAX_SUBCHNNUM)
		{
			m_byHduId = byHduId;
			m_byHduChnId = byHduChnId;
		}
	}
	void clear()
	{
		u8 m_byHduId = 0;
		u8 m_byHduChnId = 0;
	}
};

typedef	 std::vector<TVideoStreamCap> THDUVideoCap;
//HDU״̬�ṹ
struct THduUnit
{
private:
	THduKey    m_tHduKey;

    u8         m_byHduMode;  	                       
	u8         m_byChnCurVmpMode;        //��ǰ����ͨ�����ں��ַ��
	u8         m_bySchemeIdx;//Ԥ��
	u8		   m_atHduNewMode[HDU_MODEFOUR_MAX_SUBCHNNUM];//
	THduMember    m_atHduVmpChnMt[HDU_MODEFOUR_MAX_SUBCHNNUM];   //�ɹ��󱣴��ͨ���ն���Ϣ
    THduMember m_atNewHudMt[HDU_MODEFOUR_MAX_SUBCHNNUM];//�����������ĵ�ͨ���ն���Ϣ
	THduChnStatus m_atHduVmpChnStatus[HDU_MODEFOUR_MAX_SUBCHNNUM]; //ͨ����Ϣ	

	INTERACTIONID m_HduAPairdId[HDU_MODEFOUR_MAX_SUBCHNNUM];
	INTERACTIONID m_HduVPairdId[HDU_MODEFOUR_MAX_SUBCHNNUM];
	THDUVideoCap m_vcVidCap;  //ǿ�Ƶ����ֱ���(HDU�໭��ʹ��)

public:
	
	THduUnit(){Clear();}

	void Clear()
	{
		m_byHduMode = 0;
		m_byChnCurVmpMode = 0;
		m_bySchemeIdx = 0;

		memset(m_atHduNewMode, 0, sizeof(u8)*HDU_MODEFOUR_MAX_SUBCHNNUM);
		memset(m_atHduVmpChnMt, 0, sizeof(THduMember)*HDU_MODEFOUR_MAX_SUBCHNNUM);
		memset(m_atNewHudMt, 0, sizeof(THduMember)*HDU_MODEFOUR_MAX_SUBCHNNUM);
		memset(m_atHduVmpChnStatus, 0, sizeof(THduChnStatus)*HDU_MODEFOUR_MAX_SUBCHNNUM);
		memset(m_HduAPairdId, INVALID_INTERACTIONID, sizeof(INTERACTIONID)*HDU_MODEFOUR_MAX_SUBCHNNUM);
		memset(m_HduVPairdId, INVALID_INTERACTIONID, sizeof(INTERACTIONID)*HDU_MODEFOUR_MAX_SUBCHNNUM);

		m_vcVidCap.clear();


	}

	void SetHduScHemeIdx( u8 bySchemeIdx)
	{
		m_bySchemeIdx = bySchemeIdx;
	}
	u8 GetHduScHemeIdx()
	{
		return m_bySchemeIdx;
	}

	INTERACTIONID GetHduPairdId(u8 byMode, u8 bySubChnIdx = 0)
	{
		INTERACTIONID IntempId;

		if (bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM )
		{
			if (MODE_AUDIO == byMode)
			{
				IntempId = m_HduAPairdId[bySubChnIdx];
				
			}
			else
			{
				IntempId = m_HduVPairdId[bySubChnIdx];
			}
		
		}
		return IntempId;
	}

	void SetHduPairdId(INTERACTIONID hduPairdId, u8 byMode, u8 bySubChnIdx = 0)
	{
		if (bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM)
		{
			if( MODE_AUDIO == byMode )
			{	
				m_HduAPairdId[bySubChnIdx] = hduPairdId;
				
			}
			else
			{
				m_HduVPairdId[bySubChnIdx] = hduPairdId;
			}
		}
	}

	void SetChnMt(u8 bySubChnIdx, TMt tSrcMt)
	{
		if ( bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM )
		{
			memcpy(&m_atHduVmpChnMt[bySubChnIdx], &tSrcMt, sizeof(TMt));
		}
		return;

	}

	TMt GetChnMt(u8 bySubChnIdx = 0)
	{
		TMt tMt;
		tMt.SetNull();
		if ( bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM )
		{
			tMt = m_atHduVmpChnMt[bySubChnIdx];
		}
		return tMt;
	}

	void SetHduKey(const THduKey tHduKey) {m_tHduKey = tHduKey; }
	THduKey GetHduKey() const {return m_tHduKey; }

	BOOL32 IsSameHduOwner(const THduKey tHduKey)
	{
		if(tHduKey.m_byHduId == m_tHduKey.m_byHduId
			&&tHduKey.m_byHduChnId == m_tHduKey.m_byHduChnId)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	u8 GetHduMode( void )
	{
		return m_byHduMode;
	}

	void SetHduMode( u8 byMode )
	{
		m_byHduMode = byMode;
		return;
	}

	// [2013/03/11 chenbing] ���õ�ǰͨ����� 
	void SetChnCurVmpMode(u8 byStyle)
	{
		if (byStyle != HDUCHN_MODE_ONE && byStyle != HDUCHN_MODE_FOUR)
		{
			LogPrint( LOG_LVL_ERROR, MID_PUB_ALWAYS, "[SetChnCurVmpMode] byStyle(%d) invalid param\n",byStyle);
			return;
		}
		m_byChnCurVmpMode = byStyle;
	}

	// [2013/03/11 chenbing] ��ȡ��ǰͨ�����
	u8 GetChnCurVmpMode()
	{	
		return m_byChnCurVmpMode;
	}


	// [2013/03/11 chenbing] ��ȡ��ǰͨ����Ա
	THduMember GetHduMember( u8 bySubChnIdx = 0)
	{
		THduMember tMmb;
		memset( &tMmb, 0, sizeof(tMmb) );
		if ( bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM )
		{
			tMmb = m_atHduVmpChnMt[bySubChnIdx];
		}
		return tMmb;
	}

	void SetHduMember( const THduMember &tHduMember, u8 bySubChnIdx = 0 )
	{
		if ( bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM )
		{
			m_atHduVmpChnMt[bySubChnIdx] = tHduMember;
		}
		return;
	}

	THduChnStatus GetHduChnStatus(  u8 bySubChnIdx = 0 )
	{
		THduChnStatus tStatus;
		memset( &tStatus, 0, sizeof(tStatus) );
		if (  bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM )
		{
			tStatus = m_atHduVmpChnStatus[bySubChnIdx];
		}
		return tStatus;
	}

	void SetHduChnStatus( const THduChnStatus &tHduChnStatus, u8 bySubChnIdx = 0 )
	{
		if ( bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM )
		{
			m_atHduVmpChnStatus[bySubChnIdx] = tHduChnStatus;
		}
		return;
	}


	THduMember GetNewHduMt( u8 bySubChnIdx = 0 )
	{
		THduMember tMmb;
		memset( &tMmb, 0, sizeof(tMmb) );
		if (  bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM )
		{
			tMmb = m_atNewHudMt[bySubChnIdx];
		}
		return tMmb;
	}

	void SetNewHduMt( const THduMember &tHduMember, u8 bySubChnIdx = 0 )
	{
		if ( bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM )
		{
			m_atNewHudMt[bySubChnIdx] = tHduMember;
		}
		return;
	}

	void SetHduNewMode( u8 byNewMode, u8 bySubChnIdx = 0 )
	{	
		if ( bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM )
		{
			m_atHduNewMode[bySubChnIdx] = byNewMode;
		}
	}

	u8 GetHduNewMode(u8 bySubChnIdx = 0)
	{
		if ( bySubChnIdx < HDU_MODEFOUR_MAX_SUBCHNNUM )
		{
			return m_atHduNewMode[bySubChnIdx];
		}
		return 0;
	}

	void SetHduCapSet(THDUVideoCap tVideoStreamCap)
	{
		m_vcVidCap = tVideoStreamCap;
	}

	void GetHduCapSet(THDUVideoCap& tVideoStreamCap)
	{
		tVideoStreamCap = m_vcVidCap;
	}

}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TVmpOwner
{	
	enum emVmpType
	{
		NONE_CREATE,	//��Ч
		CONF_OWNER,		//���鴴��
		MT_OWNER,		//�ն˴���
		HDU_OWNER		//hdu����
	};

	u8 m_byCreaterType;	//����������
	u8 m_byConfIdx;		//����idx

	union UCreaterId
	{
		u8 m_byMtId;	//�ն�id
		u8 m_byHduId;	//����ǽid
	}m_byCreaterId;

	union UCreaterChnId
	{
		u8 m_byMtChnIdx;//�ն�ͨ��idx ������
		u8 m_byHduChnIdx;//����ǽͨ��idx
	}m_byCreaterChnIdx;

public:
	TVmpOwner(){Clear();}
	void Clear()
	{
		m_byCreaterType	= NONE_CREATE;
		m_byConfIdx		= 0xFF;
		m_byCreaterId.m_byMtId = 0;
		m_byCreaterId.m_byHduId = 0;
		m_byCreaterChnIdx.m_byMtChnIdx = 0xff;
		m_byCreaterChnIdx.m_byHduChnIdx = 0xff;
	}
	
	BOOL32 IsNull() 
	{
		if ( m_byCreaterType == NONE_CREATE )
		{
			return TRUE;
		}
		return FALSE;
	}
	BOOL32 operator == (const TVmpOwner &tVmpOwner) const
	{
		if (m_byConfIdx == tVmpOwner.GetConfIdx() &&
			m_byCreaterType == tVmpOwner.GetOwnerType() &&
			GetMtId() == tVmpOwner.GetMtId() &&
			GetMtChnId() == tVmpOwner.GetMtChnId() &&
			GetHduId() == tVmpOwner.GetHduId() &&
			GetHduChl() == tVmpOwner.GetHduChl())
		{
			return TRUE;
		}
		return FALSE;
	}

	void	SetOwnerType(u8 byCreaterType){m_byCreaterType = byCreaterType;}
	u8		GetOwnerType() const {return m_byCreaterType;} 

	//���ô���vmp�Ĵ�����id
	BOOL32	SetOwnerId(u8 byConfIdx, u8 byCreaterId = 0, u8 byChlIdx = 0xFF)
	{
		if (byConfIdx < MIN_CONFIDX || byConfIdx > MAX_CONFIDX)
		{
			StaticLog("SetOwnerId:byConfIdx.%d is invalid!\n", byConfIdx);
			return FALSE;
		}

		if (m_byCreaterType == NONE_CREATE)
		{
			StaticLog("SetOwnerId:CreaterType is NONE_CREATE!\n");
			return FALSE;
		}
		
		if (m_byCreaterType == CONF_OWNER && byCreaterId == 0 && byChlIdx == 0xFF)
		{
			//���鴴��
			m_byConfIdx = byConfIdx;
		}
		else if (m_byCreaterType == CONF_OWNER && byCreaterId != 0 && byChlIdx == 0xFF)
		{
			//��ϯ�ն˴���
			m_byConfIdx = byConfIdx;
			m_byCreaterId.m_byMtId = byCreaterId;
		}
		else if (m_byCreaterType == MT_OWNER && byCreaterId != 0 /*&& byChlIdx == 0xFF*/)
		{
			//�ն�����
			m_byConfIdx = byConfIdx;
			m_byCreaterId.m_byMtId = byCreaterId;
			m_byCreaterChnIdx.m_byMtChnIdx = byChlIdx;
		}
		else if (m_byCreaterType == HDU_OWNER && byCreaterId != 0 && byChlIdx != 0xFF)
		{
			//hdu����
			m_byConfIdx = byConfIdx;
			m_byCreaterId.m_byHduId = byCreaterId;
			m_byCreaterChnIdx.m_byHduChnIdx = byChlIdx;
		}
		else
		{
			StaticLog("Wrong Param: byCreaterType.%d, byCreaterId.%d, byChlIdx.%d!\n",
				m_byCreaterType, byCreaterId, byChlIdx);
			return FALSE;
		}

		return TRUE;
	}

	//��ȡ����vmp��confidx
	BOOL32	GetConfOwnerId(u8 &byConfIdx) const
	{
		byConfIdx = 0xFF;
		if (m_byCreaterType != CONF_OWNER)
		{
			StaticLog("GetCreaterId: CreaterType is %d!\n", m_byCreaterType);
			return FALSE;
		}
		
		byConfIdx = m_byConfIdx;
		return TRUE;
	}

	//��ȡ����vmp���ն�id
	BOOL32	GetMtOwnerId(u8 &byMtId/*, u8 &byChnId*/) const
	{
		byMtId = 0;
		u8 byChnId = 0;
		if (m_byCreaterType != MT_OWNER)
		{
			StaticLog("GetCreaterId: CreaterType is %d!\n", m_byCreaterType);
			return FALSE;
		}
		
		byMtId = m_byCreaterId.m_byMtId;
		byChnId = m_byCreaterChnIdx.m_byMtChnIdx;
		return TRUE;
	}
	
	//��ȡ����vmp��HduId+ChnIdx
	BOOL32	GetHduOwnerId(u8 &byHduId, u8 &byHduChlIdx) const
	{
		byHduId = 0;
		byHduChlIdx = 0xFF;
		if (m_byCreaterType != HDU_OWNER)
		{
			StaticLog("GetCreaterId: CreaterType is %d!\n", m_byCreaterType);
			return FALSE;
		}
		
		byHduId = m_byCreaterId.m_byHduId;
		byHduChlIdx = m_byCreaterChnIdx.m_byHduChnIdx;
		return TRUE;
	}

	u8 GetConfIdx() const{ return m_byConfIdx; }
	u8 GetMtId() const { return m_byCreaterId.m_byMtId;}
	u8 GetHduId() const { return m_byCreaterId.m_byHduId;}
	u8 GetMtChnId() const {return m_byCreaterChnIdx.m_byMtChnIdx; }
	u8 GetHduChl() const { return m_byCreaterChnIdx.m_byHduChnIdx;}
};


struct TVmpBackUpInfo
{
private:
	TVMPParam   m_tBackVmpParam;		//vmppara
	TVmpOwner	m_tVmpOwner;			//vmp������
	TVmpStyleCfgInfo	m_tVmpStyleCfgInfo;		//vmp������Ϣ
	TVMPExCfgInfo		m_tVmpExCfgInfo;		//vmp��չ��Ϣ

public:
	TVmpBackUpInfo(){Clear();}

	void Clear()
	{
		m_tBackVmpParam.Clear();
		m_tVmpOwner.Clear();
		m_tVmpStyleCfgInfo.Clear();
		m_tVmpExCfgInfo.Init();
	}
	
	//�ж�VmpCreater�Ƿ���ͬ
	BOOL32 IsSameVmpCreater(const TVmpOwner &tVmpOwner) const
	{
		if (tVmpOwner.GetOwnerType() == m_tVmpOwner.GetOwnerType()
			&& tVmpOwner.GetConfIdx() == m_tVmpOwner.GetConfIdx()
			&& tVmpOwner.GetMtId() == m_tVmpOwner.GetMtId()
			&& tVmpOwner.GetHduId() == m_tVmpOwner.GetHduId()
			&& tVmpOwner.GetHduChl() == m_tVmpOwner.GetHduChl())
		{
			return TRUE;
		}
		return FALSE;
	}

	void SetVmpOwner(const TVmpOwner &tVmpOnwer) {m_tVmpOwner = tVmpOnwer; }
	TVmpOwner GetVmpOwner() const {return m_tVmpOwner; }

	void SetVmpParam(const TVMPParam &tVmpParam) { m_tBackVmpParam = tVmpParam;}
	TVMPParam GetVmpParam() const {return m_tBackVmpParam; }

	void SetVmpStyleCfgInfo(const TVmpStyleCfgInfo &tVmpStyleCfgInfo){m_tVmpStyleCfgInfo = tVmpStyleCfgInfo;}
	TVmpStyleCfgInfo GetVmpStyleCfgInfo() const {return m_tVmpStyleCfgInfo;}
	
	void SetVmpExCfgInfo(const TVMPExCfgInfo &tVmpExCfgInfo){m_tVmpExCfgInfo = tVmpExCfgInfo;}
	TVMPExCfgInfo GetVmpExCfgInfo() const {return m_tVmpExCfgInfo;}

	void print()
	{
		if (m_tVmpOwner.GetOwnerType() == TVmpOwner::CONF_OWNER)
		{
			StaticLog("BackUpVmpCreater:CONF_OWNER:Conf.%d\n", m_tVmpOwner.GetConfIdx());
		}
		else if (m_tVmpOwner.GetOwnerType() == TVmpOwner::HDU_OWNER)
		{
			StaticLog("BackUpVmpCreater:HDU_OWNER:Conf.%d Hdu[%d.%d]\n", 
				m_tVmpOwner.GetConfIdx(), m_tVmpOwner.GetHduId(), m_tVmpOwner.GetHduChl());
		}
		else if (m_tVmpOwner.GetOwnerType() == TVmpOwner::MT_OWNER)
		{
			StaticLog("BackUpVmpCreater:MT_OWNER:Conf.%d Mt.%d\n", 
				m_tVmpOwner.GetConfIdx(), m_tVmpOwner.GetMtId());
		}
		GetVmpParam().Print(LOG_LVL_KEYSTATUS);
		StaticLog("\n"); 
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

typedef enum 
{
	em_vmpInvalid,
	em_vmp1080p,
	em_vmp4k2k
}emVmpBaseMap;

struct TVmpAdaptChnInfo
{
private:
	u8 m_byVmpBaseMap;
	u8 m_byMaxStyleNum;
	u8 m_byMaxNumHdChnnl;

public:
	TVmpAdaptChnInfo(void)
	{
		Clear();
	}
	void Clear()
	{
		SetVmpBaseMap(em_vmpInvalid);	
		SetMaxStyleNum(0);
		SetMaxHdChnlNum(0);
	}

	void SetVmpBaseMap(u8 byVmpBaseMap) { m_byVmpBaseMap = byVmpBaseMap; }
	u8   GetVmpBaseMap() const { return m_byVmpBaseMap;}

	void SetMaxStyleNum(u8 byMaxStyleNum) { m_byMaxStyleNum = byMaxStyleNum; }
	u8   GetMaxStyleNum()const {return m_byMaxStyleNum;}

	void SetMaxHdChnlNum(u8 byMaxHdChnlNum) { m_byMaxNumHdChnnl = byMaxHdChnlNum; }
	u8   GetMaxHdChnlNum()const { return m_byMaxNumHdChnnl; }
};

//vmp��ͨ����Ϣ
struct TVmpAdpChnnlMgr
{	
private:
	u8 m_byHDChnnlNum;		//����ͨ����ռ��Ŀ
	u8 m_byMaxNumHdChnnl;	//���ǰ������Ŀ
	u8 m_byMaxStyleNum;		//֧���������(��������������轵�ֱ���)
	//u8 m_byCifChnnlNum;		//Ŀǰ����
	//u8 m_byAllChnnlNum;		//���б��õ�ͨ������Ŀǰ����
	//u8 m_byHdChnnlIdx;		//ͨ��idx
	TChnnlMemberInfo	m_tHdChnnlInfo[MAXNUM_VMP_MEMBER];

public:
	TVmpAdpChnnlMgr(void)
	{
		clear();
	}
	void clear()
	{
		SetMaxNumHdChnnl(0);
		SetMaxStyleNum(0);
		clearHdChnnl();
	}
	void clearHdChnnl() //���ͨ����Ϣ,���������ǰ�������,���ڷ�ֹͣvmpʱ
	{
		SetHDChnnlNum(0);
		memset(m_tHdChnnlInfo, 0, sizeof(m_tHdChnnlInfo));
	}
	void SetHDChnnlNum(u8 byHDChnnlNum){m_byHDChnnlNum = byHDChnnlNum;}
	u8	 GetHDChnnlNum( void ){ return m_byHDChnnlNum; }
	void SetMaxNumHdChnnl(u8 byMaxNumHdChnnl){m_byMaxNumHdChnnl = byMaxNumHdChnnl;}
	u8	 GetMaxNumHdChnnl( void ){ return m_byMaxNumHdChnnl; }
	void SetMaxStyleNum(u8 byMaxStyleNum){m_byMaxStyleNum = byMaxStyleNum;}
	u8	 GetMaxStyleNum( void ){ return m_byMaxStyleNum; }
	//void SetCifChnnlNum(u8 byCifChnnlNum){m_byCifChnnlNum = byCifChnnlNum;}
	//u8	 GetCifChnnlNum( void ){ return m_byCifChnnlNum; }
	//void SetAllChnnlNum(u8 byAllChnnlNum){m_byAllChnnlNum = byAllChnnlNum;}
	
	void SetHdChnnlInfo(u8 byChnnlIdx, const TChnnlMemberInfo *ptChnnlMemberInfo)
	{
		if (NULL == ptChnnlMemberInfo)
		{
			LogPrint(LOG_LVL_ERROR, MID_MCU_VMP, "[SetHdChnnlInfo]Err:ptChnnlMemberInfo is null.\n");
			return;
		}

		if (m_tHdChnnlInfo[byChnnlIdx].GetMt().IsNull() &&
			!ptChnnlMemberInfo->GetMt().IsNull())
		{
			m_byHDChnnlNum++;
		}
		else if (!m_tHdChnnlInfo[byChnnlIdx].GetMt().IsNull() &&
			ptChnnlMemberInfo->GetMt().IsNull())
		{
			m_byHDChnnlNum--;
		}
		m_tHdChnnlInfo[byChnnlIdx] = *ptChnnlMemberInfo;
	}
	
	/*==============================================================================
	������    :  GetHdChnnlInfo
	����      :  ��ǰ����Idx��ö�Ӧ������Ϣ
	�㷨ʵ��  :  
	����˵��  :  u8 byChnnlIdx		[in]
				 TChnnlMemberInfo *ptChnnlMemberInfo	[out]
	����ֵ˵��:  void
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��       �汾          �޸���          �߶���          �޸ļ�¼
	2011-12-5				yanghuaizhi
	==============================================================================*/
	void GetHdChnnlInfo(u8 byChnnlIdx, TChnnlMemberInfo *ptChnnlMemberInfo)
	{
		if (NULL == ptChnnlMemberInfo)
		{
			LogPrint(LOG_LVL_ERROR, MID_MCU_VMP, "[GetHdChnnlInfo]Err:ptChnnlMemberInfo is null.\n");
			return;
		}

		u8 byTmpIdx = 0;
		for(u8 byLoop=0; byLoop< MAXNUM_VMP_MEMBER; byLoop++)
		{
			if(!m_tHdChnnlInfo[byLoop].GetMt().IsNull())
			{
				if (byTmpIdx == byChnnlIdx)
				{
					*ptChnnlMemberInfo = m_tHdChnnlInfo[byLoop];
					return;
				}
				byTmpIdx++;
			}
		}
		//�Ҳ������
		ptChnnlMemberInfo->SetNull();
	}
	
	/*==============================================================================
	������    :  GetHdChnnlInfoByPos
	����      :  ��ǰ����Idx��ö�Ӧ������Ϣ
	�㷨ʵ��  :  
	����˵��  :  u8 byChnnlIdx		[in]
				 TChnnlMemberInfo *ptChnnlMemberInfo	[out]
	����ֵ˵��:  void
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��       �汾          �޸���          �߶���          �޸ļ�¼
	2011-12-5				yanghuaizhi
	==============================================================================*/
	void GetHdChnnlInfoByPos(u8 byPos, TChnnlMemberInfo *ptChnnlMemberInfo)
	{
		if (NULL == ptChnnlMemberInfo || byPos >= MAXNUM_VMP_MEMBER)
		{
			LogPrint(LOG_LVL_ERROR, MID_MCU_VMP, "[GetHdChnnlInfoByPos]Err:ptChnnlMemberInfo is null or byPos[%d] is wrong.\n",byPos);
			return;
		}

		*ptChnnlMemberInfo = m_tHdChnnlInfo[byPos];
	}
	
	/*�ҵ���һ�����е�ͨ��,û���򷵻�-1
	s32 FindFreeHdChnnl()
	{
		for(u8 byLoop=0; byLoop< MAXNUM_SVMPB_HDCHNNL; byLoop++)
		{
			if(m_tHdChnnlInfo[byLoop].GetMt().IsNull())
			{
				return byLoop;
			}
		}
		return -1;
	}*/
	/*==============================================================================
	������    :  GetChnlCountByMt
	����      :  ���Mtռ��ǰ����ͨ������
	�㷨ʵ��  :  
	����˵��  :  const TMt &tMt		[in]
			 
	����ֵ˵��:  ͨ������
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��       �汾          �޸���          �߶���          �޸ļ�¼
	2011-12-5				yanghuaizhi
	==============================================================================*/
	u8 GetChnlCountByMt(const TMt &tMt)
	{
		u8 byCount = 0;
		if (tMt.IsNull())
		{
			return byCount;
		}

		for (u8 byLoop=0; byLoop<MAXNUM_VMP_MEMBER; byLoop++)
		{
			//����˫������ͨ��
			if (m_tHdChnnlInfo[byLoop].IsAttrDstream())
			{
				continue;
			}

			if (m_tHdChnnlInfo[byLoop].GetMt().GetMcuIdx() == tMt.GetMcuIdx() &&
				m_tHdChnnlInfo[byLoop].GetMt().GetMtId() == tMt.GetMtId())
			{
				byCount++;
			}
		}

		return byCount;
	}

	/*==============================================================================
	������    :  UpdateInfoToHdChnl
	����      :  ��ͨ���Ÿ���ǰ������Ϣ
	�㷨ʵ��  :  
	����˵��  :  TChnnlMemberInfo *ptChnnlMemberInfo [in]
				 u8 byVmpPos [in]
			 
	����ֵ˵��:  BOOL32 ����TRUE �쳣FALSE
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��		�汾			�޸���		�߶���		�޸ļ�¼
	2011-11-29					�־
	==============================================================================*/
	BOOL32 UpdateInfoToHdChnl(TChnnlMemberInfo *ptChnnlMemberInfo, u8 byVmpPos)
	{
		if (NULL == ptChnnlMemberInfo || byVmpPos >= MAXNUM_VMP_MEMBER)
		{
			LogPrint(LOG_LVL_ERROR, MID_MCU_VMP, "[UpdateInfoToHdChnl]Err:ptChnnlMemberInfo is null or byVmpPos[%d] is wrong.\n",byVmpPos);
			return FALSE;
		}

		if (m_byHDChnnlNum >= m_byMaxNumHdChnnl &&
			m_tHdChnnlInfo[byVmpPos].GetMt().IsNull())
		{
			LogPrint(LOG_LVL_ERROR, MID_MCU_VMP, "[UpdateInfoToHdChnl]Err:HdChnl is fool.\n");
			return FALSE;
		}

		if (!m_tHdChnnlInfo[byVmpPos].GetMt().IsNull())//����
		{
			m_tHdChnnlInfo[byVmpPos] = *ptChnnlMemberInfo;
		} 
		else { //ռ�ø�ͨ��
			m_tHdChnnlInfo[byVmpPos] = *ptChnnlMemberInfo;
			m_byHDChnnlNum++;
		}

		return TRUE;
	}

	/*==============================================================================
	������    :  ClearChnlByMt
	����      :  �����ն����������ͨ��,������Ȩ��ȡ��
	�㷨ʵ��  :  
	����˵��  :  const TMt &tMt		[in]
	����ֵ˵��:  void
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��		�汾			�޸���		�߶���		�޸ļ�¼
	2011-11-29					�־
	==============================================================================*/
	void ClearChnlByMt(const TMt &tMt)
	{
		if (tMt.IsNull())
		{
			return;
		}

		for (u8 byLoop=0; byLoop<MAXNUM_VMP_MEMBER; byLoop++)
		{
			// ˫��Դ��֧�ְ��ն���ǰ����ͨ��
			if (m_tHdChnnlInfo[byLoop].IsAttrDstream())
			{
				continue;
			}

			if (m_tHdChnnlInfo[byLoop].GetMt().GetMcuIdx() == tMt.GetMcuIdx() &&
				m_tHdChnnlInfo[byLoop].GetMt().GetMtId() == tMt.GetMtId())
			{
				m_tHdChnnlInfo[byLoop].SetNull();
				m_byHDChnnlNum--;
			}
		}
	}

	/*==============================================================================
	������    :  ClearChnlByVmpPos
	����      :  ����vmpͨ�������������ǰ����ͨ��
	�㷨ʵ��  :  
	����˵��  :  const TMt &tMt		[in]
	����ֵ˵��:  BOOL32 ����ɹ�TRUE ʧ��FALSE
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��		�汾			�޸���		�߶���		�޸ļ�¼
	2011-11-29					�־
	==============================================================================*/
	BOOL32 ClearChnlByVmpPos(u8 byVmpPos)
	{
		if (byVmpPos >= MAXNUM_VMP_MEMBER)
		{
			return FALSE;
		}
		if (m_tHdChnnlInfo[byVmpPos].GetMt().IsNull())
		{
			return FALSE;
		}
		else {
			m_tHdChnnlInfo[byVmpPos].SetNull();
			m_byHDChnnlNum--;
		}
		
		return TRUE;
	}
	
	/*==============================================================================
	������    :  GetChnlInfoList
	����      :  ��ð��ն˻��ֵ�ǰ������Ϣ,ͬһ�ն˿���ռ���ǰ����ͨ��
	�㷨ʵ��  :  
	����˵��  :  u8 &byMtNum	[o] �ն˸���
				 TChnnlMemberInfo *ptChnlInfo [o] �ն˶�Ӧǰ������Ϣ
				 u8 *abyChnlNo	[o] ��Ӧ�ն�ռ��ǰ����ͨ������
	����ֵ˵��:  void
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��		�汾			�޸���		�߶���		�޸ļ�¼
	2011-12-13					�־
	==============================================================================*/
	void GetChnlInfoList(u8 &byMtNum,u8 *abyChnlNo, TChnnlMemberInfo *ptChnlInfo)
	{
		if (NULL == abyChnlNo || NULL == ptChnlInfo)
		{
			LogPrint(LOG_LVL_ERROR, MID_MCU_VMP, "[GetChnlInfoList]Err:abyChnlNo or ptChnlInfo is null.\n");
			return;
		}
		byMtNum = 0;
		TVmpAdpChnnlMgr tTmpChnlInfo = *this;
		for(u8 byPos = 0; byPos < MAXNUM_VMP_MEMBER; byPos++)	//����ռ��HD��ͨ����MT��Ϣ
		{
			TMt tTmpMt = m_tHdChnnlInfo[byPos].GetMt();
			if (!tTmpMt.IsNull())
			{
				// �Ѿ��������
				if (0 == tTmpChnlInfo.GetChnlCountByMt(tTmpMt))
				{
					continue;
				}
				*ptChnlInfo = m_tHdChnnlInfo[byPos];
				*abyChnlNo = GetChnlCountByMt(tTmpMt);
				ptChnlInfo++;
				abyChnlNo++;
				byMtNum++;
				//������Ϣ�������ͳ�ƹ����ն�
				tTmpChnlInfo.ClearChnlByMt(tTmpMt);
			}
		}
	}
	
	/*==============================================================================
	������    :  GetWeakestVmpVipChl
	����      :  �������vip����ͨ��
	�㷨ʵ��  :  
	����˵��  :  
	����ֵ˵��:  
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��       �汾          �޸���          �߶���          �޸ļ�¼
	2013-07					yanghuaizhi							
	==============================================================================*/
	u8 GetWeakestVmpVipChl()
	{
		u8 byWeakestChl = 0XFF;
		TChnnlMemberInfo tWeakestChlInfo;
		// �ҵ�����ǰ��������
		for (u8 byLoop=0; byLoop<MAXNUM_VMP_MEMBER; byLoop++)
		{
			if (!m_tHdChnnlInfo[byLoop].IsAttrNull())
			{
				// ѡ���������ҵ�ѡ��vip����ֹͣ
				if (m_tHdChnnlInfo[byLoop].IsAttrSelected())
				{
					byWeakestChl = byLoop;
					break;
				}

				// �������벻�ɽ��ֱ���vip����
				if (byWeakestChl >= MAXNUM_VMP_MEMBER)
				{
					tWeakestChlInfo = m_tHdChnnlInfo[byLoop];// ��¼��һ���������Ƿ�����Ҳ�������⳧��Mt
					byWeakestChl = byLoop;
				}
				else
				{
					// ���ɽ��ֱ���vip�ȷ�������
					if (tWeakestChlInfo.IsAttrSpeaker() && !m_tHdChnnlInfo[byLoop].IsCanAdjResMt())
					{
						tWeakestChlInfo = m_tHdChnnlInfo[byLoop];
						byWeakestChl = byLoop;
					}
				}
			}
		}

		return byWeakestChl;
	}

	/*==============================================================================
	������    :  GetReplaceVmpVipChl
	����      :  ����ǰ����ͨ����Ϣ����ñȸ�����С�Ŀ�ռ��ͨ��
	�㷨ʵ��  :  ������vip > ���ɽ��ֱ���vip > ��ѡ��vip
	����˵��  :  
	����ֵ˵��:  
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��       �汾          �޸���          �߶���          �޸ļ�¼
	2013-07					yanghuaizhi							
	==============================================================================*/
	void GetReplaceVmpVipChl(const TChnnlMemberInfo &tChlMemInfo, u8 &byChlPos, TChnnlMemberInfo &tReplaceMemInfo)
	{
		byChlPos = 0XFF;
		if (GetMaxNumHdChnnl() == 0 || tChlMemInfo.IsAttrNull())
		{
			return;
		}
		// ѡ��������ͣ���֧�ֺ�̨��ռ
		if (tChlMemInfo.IsAttrSelected())
		{
			return;
		}
		u8 byWeakestChlPos = GetWeakestVmpVipChl();
		if (byWeakestChlPos < MAXNUM_VMP_MEMBER)
		{
			// ������vip��ѡ�����ֱ�ӷ���
			if (m_tHdChnnlInfo[byWeakestChlPos].IsAttrSelected())
			{
				byChlPos = byWeakestChlPos;
				tReplaceMemInfo = m_tHdChnnlInfo[byWeakestChlPos];
			}
			// �����˱Ȳ��ɽ��ֱ���vip��ǿ
			else if (tChlMemInfo.IsAttrSpeaker() && !m_tHdChnnlInfo[byWeakestChlPos].IsCanAdjResMt())
			{
				byChlPos = byWeakestChlPos;
				tReplaceMemInfo = m_tHdChnnlInfo[byWeakestChlPos];
			}
			else
			{
				// �Ҳ����ȴ�����������ͨ��
			}
		}

		return;
	}

	void print(void)//FIXME
	{
		LogPrint(LOG_LVL_DETAIL, MID_MCU_VMP, "MaxStyleNum: %u\nMax HdChnnl Num: %u\nSeized HdChnnl Num: %u\n", 
			m_byMaxStyleNum, m_byMaxNumHdChnnl, m_byHDChnnlNum);
		LogPrint(LOG_LVL_DETAIL, MID_MCU_VMP, "---------Mt info in Hd Channels------------\n");
		for(u8 byLoop = 0; byLoop < MAXNUM_VMP_MEMBER; byLoop++)
		{
			LogPrint(LOG_LVL_DETAIL, MID_MCU_VMP, "Hd Channel[%u]-->Mt.(%u,%u)\n",
				byLoop, m_tHdChnnlInfo[byLoop].GetMt().GetMcuId(), m_tHdChnnlInfo[byLoop].GetMt().GetMtId()  );
		}

	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//��¼��ռ������Ҫ�Ĳ���
struct TVmpAdaptChnSeizeOpr
{
public:
	TMt m_tMt;		//��ռ���ն�
	u16 m_wEventId;
	TSwitchInfo m_tSwitchInfo;
	u8  m_bySrcSsnId;  //Դ�Ự��,��������ѡ���Ƿ���MCS��קѡ��,MCS��ק��������ʾС�۾�
public:
	TVmpAdaptChnSeizeOpr(void)
	{
		Clear();
	}
	void Clear( void )
	{
		memset(this, 0, sizeof(TVmpAdaptChnSeizeOpr));
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TSwitchChannelAddorRemove : public TSwitchChannel
{
	TSwitchChannelAddorRemove()
    {
        Clear( );
    }
protected:	
	u8	m_byIsAdd;//�Ƿ�ʱ��ӵĽ�������ɾ���Ľ���
	u32	m_dwMapIp;
	u16 m_wMapPort;
public:
	void Clear( ){ memset( this, 0, sizeof(TSwitchChannelAddorRemove) ); }
	BOOL32  IsAdd( )const { return m_byIsAdd==1; }
	void SetIsAdd(BOOL32 bIsAdd ){ m_byIsAdd = (bIsAdd ? 1:0); }

	u32 GetMapIp() const {return htonl(m_dwMapIp);}
	void SetMapIp( u32 dwMapIp){ m_dwMapIp = ntohl(dwMapIp);}

	u16 GetMapPort() const {return htons(m_wMapPort);}
	void SetMapPort( u16 wMapPort ){ m_wMapPort = ntohs(wMapPort); }
};


struct TSwitchChannelExt:public TSwitchChannel
{
    TSwitchChannelExt()
    {
        Clear( );
    }
protected:
	
	u32  m_dwMappedIP;        //��Ҫ���͵�Ŀ��IP��ַ��Ӧ��Դ����ip(������)
	u16  m_wMappedPort;       //��Ҫ���͵�Ŀ��˿ڵ�Դ����port (������)
	
public:
	
	void Clear( ){ memset( this, 0, sizeof(TSwitchChannelExt) ); }
	void SetMapIp(u32  dwMappedIp){m_dwMappedIP = htonl(dwMappedIp); }
	u32  GetMapIp()const { return ntohl(m_dwMappedIP); }
	void SetMapPort(u16 wMappedPort){m_wMappedPort = htons(wMappedPort);}
	u16  GetMapPort()const{return ntohs(m_wMappedPort);}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//���������ṹ
struct TSendMutePack
{
	u32 m_dwDstIp;//Ҫ����������Ŀ��ip
	u16 m_wDstPort;//Ҫ����������Ŀ��port
	u8  m_byConfIdx;//��������
	u8  m_byMtId;//���������mtid
	u8  m_byPayloadType;    //Ҫ����������payloadtype
	u8  m_byAudioTrackNum;//������
	void Clear()
	{	
		m_dwDstIp = 0;
		m_wDstPort = 0;
		m_byConfIdx = 0xFF;
		m_byMtId = 0;
		m_byPayloadType = MEDIA_TYPE_NULL;
		m_byAudioTrackNum = 1;//Ĭ�ϵ�����
	}
	TSendMutePack(){Clear();}
	~TSendMutePack(){Clear();}
	void SetConfIdx(const u8 &byConfIdx)
	{
		m_byConfIdx = byConfIdx;
	}
	u8 GetConfIdx()const
	{
		return m_byConfIdx;
	}
	void SetMtId(const u8 & byMtId)
	{
		m_byMtId = byMtId;
	}
	u8 GetMtId()const
	{
		return m_byMtId;
	}
	void SetDstIp(const u32 &dwDstIp)
	{
		m_dwDstIp = htonl(dwDstIp);
	}
	u32 GetDstIp()const
	{
		return ntohl(m_dwDstIp);
	}
	void SetDstPort(const u16 &wDstport)
	{
		m_wDstPort = htons(wDstport);
	}
	u16 GetDstPort() const
	{
		return ntohs(m_wDstPort);
	}

	void SetPayloadType(const u8 &byPayload)
	{
		m_byPayloadType = byPayload;
	}
	u8  GetPayloadType()const
	{
		return m_byPayloadType;
	} 
	void SetAudioTrackNum(const u8 &byAudioTrackNum)
	{
		m_byAudioTrackNum = byAudioTrackNum;
	}
	u8  GetAudioTrackNum()const
	{
		return m_byAudioTrackNum;
	} 

	BOOL32 IsNull()
	{
		if ( m_dwDstIp == 0 || m_wDstPort == 0 || m_byConfIdx > MAX_CONFIDX || m_byPayloadType == MEDIA_TYPE_NULL )
		{
			return TRUE;
		}
		return FALSE;
	}
	void OspPrint()
	{
		OspPrintf(TRUE,FALSE,"m_dwDstIp:0x%x\n",GetDstIp());
		OspPrintf(TRUE,FALSE,"m_wDstPort:%d\n",GetDstPort());
		OspPrintf(TRUE,FALSE,"m_byConfIdx:%d\n",GetConfIdx());
		OspPrintf(TRUE,FALSE,"m_byMtId:%d\n",GetMtId());
		OspPrintf(TRUE,FALSE,"m_byPayloadType:%d\n",GetPayloadType());
		OspPrintf(TRUE,FALSE,"m_byAudioTrackNum:%d\n",GetAudioTrackNum());
	}
	void Print()
	{
		StaticLog("m_dwDstIp:0x%x\n",GetDstIp());
		StaticLog("m_wDstPort:%d\n",GetDstPort());
		StaticLog("m_byConfIdx:%d\n",GetConfIdx());
		StaticLog("m_byMtId:%d\n",GetMtId());
		StaticLog("m_byPayloadType:%d\n",GetPayloadType());
		StaticLog("m_byAudioTrackNum:%d\n",GetAudioTrackNum());
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


//���ڼ���V4R6B2��8000H��յ�Ԥ�������ṹ[11/14/2012 chendaiwei]
struct TMtVCSPlanAliasV4R6B2
{
public:
    u8				m_AliasType;               //��������
    s8				m_achAlias[32];  //�����ַ���
    TTransportAddr	m_tTransportAddr;          //�����ַ
	u8				m_byReserve;
public:
	TMtVCSPlanAliasV4R6B2( void ){ memset( this, 0, sizeof(TMtVCSPlanAliasV4R6B2) ); }
	BOOL32 operator ==( const TMtVCSPlanAliasV4R6B2 & tObj ) const;
	BOOL32 IsNull( void ) const { if( m_AliasType == 0 )return TRUE; return FALSE; }
	void SetNull( void ){ memset(this, 0, sizeof(TMtVCSPlanAliasV4R6B2)); }
	BOOL32 IsAliasNull( void ) const { return 0 == strlen(m_achAlias); }
	void SetE164Alias( LPCSTR lpzAlias )
	{
		if( lpzAlias != NULL )
		{
			m_AliasType = mtAliasTypeE164;
            memset( m_achAlias, 0, sizeof(m_achAlias));
			strncpy( m_achAlias, lpzAlias, 32-1 );
		}
	}
	void SetH323Alias( LPCSTR lpzAlias )
	{
		if( lpzAlias != NULL )
		{
			m_AliasType = mtAliasTypeH323ID;
            memset( m_achAlias, 0, sizeof(m_achAlias));
			strncpy( m_achAlias, lpzAlias, 32-1);
		}
	}
	void SetH320Alias( u8 byLayer, u8 bySlot, u8 byChannel )
	{
		m_AliasType = mtAliasTypeH320ID;
		sprintf( m_achAlias, "��%d-��%d-ͨ��%d%c", byLayer, bySlot, byChannel, 0 );
	}
	BOOL32 GetH320Alias ( u8 &byLayer, u8 &bySlot, u8 &byChannel )
	{
		BOOL32 bRet = FALSE;
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
	void Print( void ) const
	{
		if( m_AliasType == 0 )
		{
			StaticLog("null alias" );
		}
		else if( m_AliasType == mtAliasTypeTransportAddress )
		{
			StaticLog("IP: 0x%x:%d", 
				     m_tTransportAddr.GetIpAddr(), m_tTransportAddr.GetPort() );
		}		
		else if( m_AliasType == mtAliasTypeE164 )
		{
			StaticLog("E164: %s", m_achAlias ); 
		}
		else if( m_AliasType == mtAliasTypeH323ID )
		{
			StaticLog("H323ID: %s", m_achAlias ); 
		}
		else if( m_AliasType == mtAliasTypeH320ID )
		{
			StaticLog("H320ID: %s", m_achAlias ); 
		}
        else if( m_AliasType == mtAliasTypeH320Alias )
        {
            StaticLog("H320Alias: %s", m_achAlias );
        }
		else
		{
			StaticLog("Other type alias!" ); 
		}
		StaticLog("\n" );
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// mcu������ʹ��vmp��ͨ��Ϣ
struct TVmpCommonAttrb
{
	u8	m_byVmpStyle;	//�ϳɷ��
	u8	m_byMemberNum;	//��Ա����
	u8	m_byNeedPrs;	//�Ƿ���ҪPRS
	TVMPMemberEx		m_atMtMember[MAXNUM_VMP_MEMBER];		//���ϳ�Ա
	TMediaEncrypt		m_tVideoEncrypt[MAXNUM_VMP_MEMBER];     //��Ƶ���ܲ���
	TDoublePayload		m_tDoublePayload[MAXNUM_VMP_MEMBER];	//˫�غ�
	u8					m_abyRcvH264DependInMark[MAXNUM_VMP_MEMBER];//����8ke/h/i vmp�����ⷽʽ�������ã���Ի�Ϊ�նˣ�
	TVmpStyleCfgInfo	m_tVmpStyleCfgInfo;		//������Ϣ
	TVMPExCfgInfo		m_tVmpExCfgInfo;		//��չ������Ϣ
	
	TVmpCommonAttrb()
	{
		memset(this, 0, sizeof(TVmpCommonAttrb));
	}
	
    void Print(void)
    {
        StaticLog( "VmpStyle:%d, MemberNum:%d, NeedPrs:%d\n",
			m_byVmpStyle, m_byMemberNum, m_byNeedPrs);
        for(u8 byIndex = 0; byIndex < MAXNUM_VMP_MEMBER; byIndex++)
        {
            if(0 != m_atMtMember[byIndex].GetMcuId() && 0 != m_atMtMember[byIndex].GetMtId())
            {
                StaticLog( "McuId:%d, MtId:%d, MemType:%d, MemStatus:%d, RealPayLoad:%d, ActPayLoad:%d, EncrptMode:%d\n", 
					m_atMtMember[byIndex].GetMcuId(), m_atMtMember[byIndex].GetMtId(), 
					m_atMtMember[byIndex].GetMemberType(), m_atMtMember[byIndex].GetMemStatus(),
					m_tDoublePayload[byIndex].GetRealPayLoad(), m_tDoublePayload[byIndex].GetActivePayload(),
					m_tVideoEncrypt[byIndex].GetEncryptMode());
            }            
        }        
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// mpu2����ʹ�ù�ͨ������Ϣ
struct TMPU2CommonAttrb
{
	u8	m_byVmpStyle;	//�ϳɷ��
	u8	m_byMemberNum;	//��Ա����
	u8	m_byNeedPrs;	//�Ƿ���ҪPRS
	TVMPMemberEx		m_atMtMember[MAXNUM_MPU2VMP_MEMBER];		//���ϳ�Ա
	TMediaEncrypt		m_tVideoEncrypt[MAXNUM_MPU2VMP_MEMBER];     //��Ƶ���ܲ���
	TDoublePayload		m_tDoublePayload[MAXNUM_MPU2VMP_MEMBER];	//˫�غ�
	TVmpStyleCfgInfo	m_tVmpStyleCfgInfo;		//������Ϣ
	TVMPExCfgInfo		m_tVmpExCfgInfo;		//��չ������Ϣ
	
	TMPU2CommonAttrb()
	{
		memset(this, 0, sizeof(TMPU2CommonAttrb));
	}
	
    void Print(void)
    {
        StaticLog( "VmpStyle:%d, MemberNum:%d, NeedPrs:%d\n",
			m_byVmpStyle, m_byMemberNum, m_byNeedPrs);
        for(u8 byIndex = 0; byIndex < MAXNUM_MPU2VMP_MEMBER; byIndex++)
        {
            if(0 != m_atMtMember[byIndex].GetMcuId() && 0 != m_atMtMember[byIndex].GetMtId())
            {
                StaticLog( "McuId:%d, MtId:%d, MemType:%d, MemStatus:%d, RealPayLoad:%d, ActPayLoad:%d, EncrptMode:%d\n", 
					m_atMtMember[byIndex].GetMcuId(), m_atMtMember[byIndex].GetMtId(), 
					m_atMtMember[byIndex].GetMemberType(), m_atMtMember[byIndex].GetMemStatus(),
					m_tDoublePayload[byIndex].GetRealPayLoad(), m_tDoublePayload[byIndex].GetActivePayload(),
					m_tVideoEncrypt[byIndex].GetEncryptMode());
            }            
        }        
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

/* ����ϳɳ�Ա�ṹ(Mcu -- Vmp) 8kh/8ki�²��ع�׼��
struct TVMPMember128Ex : public TVMPMember
{
public:
	void  SetMember( TVMPMember& tMember )
	{
		memcpy(this, &tMember, sizeof(tMember));
	}
	
    BOOL32  SetMbAlias( u8 byAliasLen, const s8 *pAlias ) 
    { 
        if( NULL == pAlias )
        {
            return FALSE;
        }
        memset(m_achMbAlias, 0, sizeof(m_achMbAlias));
        memcpy(m_achMbAlias, pAlias, min(byAliasLen, MAXLEN_ALIAS-1));
        m_achMbAlias[MAXLEN_ALIAS-1] = '\0';
        return TRUE; 
    }
	const s8* GetMbAlias(void) { return m_achMbAlias; }
	
protected:
	s8   m_achMbAlias[MAXLEN_ALIAS];
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// 8ke8kh8ki����ʹ�ù�ͨ������Ϣ,һ�������Ҫ���Ǽ���
struct T8KEVmpCommonAttrb
{
	u8	m_byVmpStyle;	//�ϳɷ��
	u8	m_byMemberNum;	//��Ա����
	u8	m_byNeedPrs;	//�Ƿ���ҪPRS
	TVMPMember128Ex		m_atMtMember[MAXNUM_SDVMP_MEMBER];		//���ϳ�Ա,������128
	TMediaEncrypt		m_tVideoEncrypt[MAXNUM_SDVMP_MEMBER];     //��Ƶ���ܲ���
	TDoublePayload		m_tDoublePayload[MAXNUM_SDVMP_MEMBER];	//˫�غ�
	u8					m_abyRcvH264DependInMark[MAXNUM_SDVMP_MEMBER];//����8ke/h/i vmp�����ⷽʽ�������ã���Ի�Ϊ�նˣ�
	TVmpStyleCfgInfo	m_tVmpStyleCfgInfo;		//������Ϣ
	TVMPExCfgInfo		m_tVmpExCfgInfo;		//��չ������Ϣ
	
	T8KEVmpCommonAttrb()
	{
		memset(this, 0, sizeof(T8KEVmpCommonAttrb));
	}
	
    void Print(void)
    {
        StaticLog( "VmpStyle:%d, MemberNum:%d, NeedPrs:%d\n",
			m_byVmpStyle, m_byMemberNum, m_byNeedPrs);
        for(u8 byIndex = 0; byIndex < MAXNUM_SDVMP_MEMBER; byIndex++)
        {
            if(0 != m_atMtMember[byIndex].GetMcuId() && 0 != m_atMtMember[byIndex].GetMtId())
            {
                StaticLog( "McuId:%d, MtId:%d, MemType:%d, MemStatus:%d, RealPayLoad:%d, ActPayLoad:%d, EncrptMode:%d\n", 
					m_atMtMember[byIndex].GetMcuId(), m_atMtMember[byIndex].GetMtId(), 
					m_atMtMember[byIndex].GetMemberType(), m_atMtMember[byIndex].GetMemStatus(),
					m_tDoublePayload[byIndex].GetRealPayLoad(), m_tDoublePayload[byIndex].GetActivePayload(),
					m_tVideoEncrypt[byIndex].GetEncryptMode());
            }            
        }        
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
*/

// Vmp׷����Ϣ������ [7/3/2013 liaokang]
enum emVmpCommType
{
    emVmpStart = (u8)0,
    emVmpMbAlias,          // ��Ա����
};

//��Ƶ������Ϣ
struct TAudioCapInfo
{
private:
	TAudioTypeDesc m_tAudioTypeDesc;
	u8             m_byActivePayload;
	u32            reserved;
public:
	TAudioCapInfo( void )
	{
		Clear();
	}
	void Clear( void )
	{
		m_tAudioTypeDesc.Clear();
		m_byActivePayload = 0;
	}
	void SetAudioMediaType( u8 byAudioType)
	{
		m_tAudioTypeDesc.SetAudioMediaType(byAudioType);
	}
	void SetAudioTrackNum( u8 byAudioTrackNum)
	{
		m_tAudioTypeDesc.SetAudioTrackNum(byAudioTrackNum);
	}
	void SetActivePayLoad( u8 byActivePayload)
	{
		m_byActivePayload = byActivePayload;
	}
	u8 GetAudioMediaType( void )
	{
		return m_tAudioTypeDesc.GetAudioMediaType();
	}
	u8 GetAudioTrackNum( void )
	{
		return m_tAudioTypeDesc.GetAudioTrackNum();
	}
	u8 GetActivePayLoad( void )
	{
		return m_byActivePayload;
	}
}
#ifndef WIN32
__attribute__((packed)) 
#endif
;

//ֹͣ��������Ϣ
struct H460StopHoleInfo
{
public:
    u16 wLocalPort;
    u32 dwRemoteIp;
    u16 wRemotePort;    
    
public:
    u16  GetLocalPort(){ return ntohs(wLocalPort); }
    void SetLocalPort( u16 wPort ){ wLocalPort = htons(wPort); }
    
    u32  GetRemoteIp(){ return ntohl(dwRemoteIp); }
    void SetRemoteIp( u32 dwIp ){ dwRemoteIp = htonl(dwIp); }
    
    u16  GetRemotePort(){ return ntohs(wRemotePort); }
    void SetRemotePort( u16 wPort ){ wRemotePort = htons(wPort); }
    
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


//�ն�ͨ����ϢǨ��
struct TMtMgrVidSndChnStatus : public TChnStatus
{
private:
	u8 m_bySendVideo;
	u8 m_byVideoLose;               //�Ƿ���ƵԴ��ʧ
	
	TLogicalChannel m_tChannel;		//�߼�ͨ��
	
	u16 m_wBandWidth;				//��ƴ���
	u8	m_byCurrVidNo;				//��ǰ���ն���ƵԴ��
	
	//HMEDIARES m_hMediaH;			//�������Դ���
	
	TTransportAddr m_tSwitchAddr;	//���յ�ַ
	
public:
	TMtMgrVidSndChnStatus() : m_bySendVideo(0), m_byVideoLose(0), m_wBandWidth(0), m_byCurrVidNo(0)//, m_hMediaH(0)
	{
		m_tChannel.Clear();
		//m_hMediaH = INVALID_HMEDIARES;
		m_tSwitchAddr.SetNull();
	}
	
	BOOL32 IsSendVideo(void) const { return ( 0 == m_bySendVideo ) ? FALSE : TRUE; }
	void SetIsSendVideo(BOOL32 bIsSendVideo) { m_bySendVideo = bIsSendVideo ? 1 : 0; }
	
	BOOL32 IsVideoLose(void) const { return ( 0 == m_byVideoLose ) ? FALSE : TRUE; }
	void SetIsVideoLose(BOOL32 bIsVideoLose) { m_byVideoLose = bIsVideoLose ? 1 : 0; }
	
	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }
	
	u16 GetBandWidth(void) const { return m_wBandWidth; }
	void SetBandWidth(u16 wBandWidth) { m_wBandWidth = wBandWidth; }
	
	u8	GetCurrVidNo(void) const { return m_byCurrVidNo; }
	void SetCurrVidNo(u8 byVidNo) { m_byCurrVidNo = byVidNo; }
	
	// 	HMEDIARES GetMediaH(void) const { return m_hMediaH; }
	// 	void SetMediaH(const HMEDIARES &hMediaH) { m_hMediaH = hMediaH; }
	
	TTransportAddr GetSwitchAddr(void) const { return m_tSwitchAddr; }
	void SetSwitchAddr(const TTransportAddr &tAddr) { m_tSwitchAddr = tAddr; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrVidRcvChnStatus : public TChnStatus
{
private:
	u8 m_byReceiveVideo;            //�Ƿ����ڽ�����Ƶ
    //HMEDIARES m_hMediaH;			//�������Դ���
	TLogicalChannel m_tChannel;		//�߼�ͨ��
	u16 m_wBandWidth;				//��ƴ���

	TTransportAddr m_tRtcpSwitchAddr;	//���ո��ն˵�RTCP�ش��������ַ(�߼�ͨ����ĵ�ַ)
	TTransportAddr m_tRtcpDstAddr;       //���ն���ƵRTCPĿ�ĵ�ַ(ҵ���ϵ�Ŀ�ĵ�ַ)

public:
	TMtMgrVidRcvChnStatus() : m_byReceiveVideo(0), m_wBandWidth(0)//, m_hMediaH(INVALID_HMEDIARES)
	{
		m_tChannel.Clear();
		m_tRtcpSwitchAddr.SetNull();
		m_tRtcpDstAddr.SetNull();
	}

public:
	BOOL32 IsRecvVideo(void) const { return ( 0 == m_byReceiveVideo ) ? FALSE : TRUE; }
	void SetIsRecvVideo(BOOL32 bIsRecvVideo) { m_byReceiveVideo = bIsRecvVideo ? 1 : 0; }
	
// 	HMEDIARES GetMediaH(void) const { return m_hMediaH; }
// 	void SetMediaH(const HMEDIARES hMediaH) { m_hMediaH = hMediaH; }

	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }

	u16 GetBandWidth(void) const { return m_wBandWidth; }
	void SetBandWidth(u16 wBandWidth) { m_wBandWidth = wBandWidth; }

	TTransportAddr GetRtcpSwitchAddr(void) const { return m_tRtcpSwitchAddr; }
	void SetRtcpSwitchAddr(const TTransportAddr &tAddr) { m_tRtcpSwitchAddr = tAddr; }

	TTransportAddr GetRtcpDstAddr(void) const { return m_tRtcpDstAddr; }
	void SetRtcpDstAddr(const TTransportAddr &tAddr) { m_tRtcpDstAddr = tAddr; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrDVidSndChnStatus : public TChnStatus
{
private:
	u8      m_bySndVideo2;               //�Ƿ��ڷ��͵ڶ���Ƶ
	u8      m_byH239VideoFormat;         //�ն˵ڶ�·��Ƶ�ֱ���
	u8      m_byBindedAudChnIdx;		 //�͸�·˫������ͨ���󶨵�˫����Ƶ����ͨ��

	TLogicalChannel m_tChannel;   //�߼�ͨ��
	u16 m_wBandWidth;				//��ƴ���

	//HMEDIARES m_hMediaH;			//�������Դ���
	TTransportAddr m_tSwitchAddr;	//���յ�ַ

public:
	TMtMgrDVidSndChnStatus() : m_bySndVideo2(0), m_byH239VideoFormat(0), m_byBindedAudChnIdx(INVALID_CHNNL)//, m_hMediaH(INVALID_HMEDIARES)
	{
		m_wBandWidth = 0;
		m_tChannel.Clear();
		m_tSwitchAddr.SetNull();
	}

	BOOL32 IsSendVideo2(void) const { return ( 0 == m_bySndVideo2 ) ? FALSE : TRUE; }
	void SetIsSendVideo2(BOOL32 bIsSendVideo2) { m_bySndVideo2 = bIsSendVideo2 ? 1 : 0; }
	
	u8 GetH239VideoFormat(void) const { return m_byH239VideoFormat; }
	void SetH239VideoFormat(u8 byH239VideoFormat) { m_byH239VideoFormat = byH239VideoFormat; }

	u8 GetBindedAudChnIdx(void) const { return m_byBindedAudChnIdx; }
	void SetBindedAudChnIdx(u8 byChnIdx) { m_byBindedAudChnIdx = byChnIdx; }

	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }

	u16 GetBandWidth(void) const { return m_wBandWidth; }
	void SetBandWidth(u16 wBandWidth) { m_wBandWidth = wBandWidth; }

// 	HMEDIARES GetMediaH(void) const { return m_hMediaH; }
// 	void SetMediaH(const HMEDIARES hMediaH) { m_hMediaH = hMediaH; }

	TTransportAddr GetSwitchAddr(void) const { return m_tSwitchAddr; }
	void SetSwitchAddr(const TTransportAddr &tAddr) { m_tSwitchAddr = tAddr; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrDVidRcvChnStatus : public TChnStatus
{
private:
	u8      m_byRcvVideo2;				 //�Ƿ��ڽ��յڶ���Ƶ 
	//HMEDIARES m_hMediaH;			//�������Դ���

	TLogicalChannel m_tChannel;   //�߼�ͨ��
	u16 m_wBandWidth;				//��ƴ���
	u8      m_byBindedAudChnIdx;		 //�͸�·˫������ͨ���󶨵�˫����Ƶ����ͨ��

	TTransportAddr m_tRtcpSwitchAddr;	//���ո��ն˵�RTCP�ش��������ַ
public:
	TMtMgrDVidRcvChnStatus() : m_byRcvVideo2(0), m_byBindedAudChnIdx(INVALID_CHNNL)//, m_hMediaH(INVALID_HMEDIARES)
	{
		m_wBandWidth = 0;
		m_tChannel.Clear();
		m_tRtcpSwitchAddr.SetNull();
	}
	
public:
	BOOL32 IsRcvVideo2(void) const { return ( 0 == m_byRcvVideo2 ) ? FALSE : TRUE; }
	void SetIsRcvVideo2(BOOL32 bIsRcvVideo2) { m_byRcvVideo2 = bIsRcvVideo2 ? 1 : 0; }
	
// 	HMEDIARES GetMediaH(void) const { return m_hMediaH; }
// 	void SetMediaH(const HMEDIARES hMediaH) { m_hMediaH = hMediaH; }

	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }

	u16 GetBandWidth(void) const { return m_wBandWidth; }
	void SetBandWidth(u16 wBandWidth) { m_wBandWidth = wBandWidth; }

	u8 GetBindedAudChnIdx(void) const { return m_byBindedAudChnIdx; }
	void SetBindedAudChnIdx(u8 byChnIdx) { m_byBindedAudChnIdx = byChnIdx; }

	TTransportAddr GetRtcpSwitchAddr(void) const { return m_tRtcpSwitchAddr; }
	void SetRtcpSwitchAddr(const TTransportAddr &tAddr) { m_tRtcpSwitchAddr = tAddr; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrAudSndChnStatus : public TChnStatus
{
private:
	u8 m_bySendAudio;           //�Ƿ����ڴ�����Ƶ
	u8 m_byCaptureMute;			//����
	u8 m_byCaptureVolume;       //�ɼ�����
	TLogicalChannel m_tChannel;   //�߼�ͨ��
	u8 m_byCurrAudNo;			//��ǰ�ն���Ƶ����Դ
	//HMEDIARES m_hMediaH;			//�������Դ���
	TTransportAddr m_tSwitchAddr;	//���յ�ַ

public:
	TMtMgrAudSndChnStatus() : m_bySendAudio(0), m_byCaptureMute(0), m_byCaptureVolume(0), m_byCurrAudNo(0)//, m_hMediaH(INVALID_HMEDIARES)
	{
		m_tChannel.Clear();
		m_tSwitchAddr.SetNull();
	}

public:
	BOOL32 IsSendAudio(void) const { return ( 0 == m_bySendAudio ) ? FALSE : TRUE; }
	void SetIsSendAudio(BOOL32 bIsSendAudio) { m_bySendAudio = bIsSendAudio ? 1 : 0; }

	BOOL32 IsCaptureMute(void) const { return ( 0 == m_byCaptureMute ) ? FALSE : TRUE; }
	void SetIsCaptureMute(BOOL32 bIsCaptureMutee) { m_byCaptureMute = bIsCaptureMutee ? 1 : 0; }

	u8 GetCaptureVolume(void) const { return m_byCaptureVolume; }
	void SetCaptureVolume(u8 byVolume) { m_byCaptureVolume = byVolume; }

	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }

	u8	GetCurrAudNo(void) const { return m_byCurrAudNo; }
	void SetCurrAudNo(u8 byAudNo) { m_byCurrAudNo = byAudNo; }

// 	HMEDIARES GetMediaH(void) const { return m_hMediaH; }
// 	void SetMediaH(const HMEDIARES hMediaH) { m_hMediaH = hMediaH; }

	TTransportAddr GetSwitchAddr(void) const { return m_tSwitchAddr; }
	void SetSwitchAddr(const TTransportAddr &tAddr) { m_tSwitchAddr = tAddr; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrDVAudSndChnStatus : public TChnStatus
{
private:
	u8 m_bySendAudio;           //�Ƿ����ڴ�����Ƶ
	u8 m_byCaptureMute;			//����
	u8 m_byCaptureVolume;       //�ɼ�����
	u8 m_byBindedDVChnIdx;		//�󶨵�˫������ͨ��������
	TLogicalChannel m_tChannel;   //�߼�ͨ��
	//HMEDIARES m_hMediaH;			//�������Դ���
	TTransportAddr m_tSwitchAddr;	//���յ�ַ

public:
	TMtMgrDVAudSndChnStatus() : m_bySendAudio(0), m_byCaptureMute(0), m_byCaptureVolume(0), m_byBindedDVChnIdx(INVALID_CHNNL)//, m_hMediaH(INVALID_HMEDIARES)
	{
		m_tChannel.Clear();
		m_tSwitchAddr.SetNull();
	}
	
public:
	BOOL32 IsSendAudio(void) const { return ( 0 == m_bySendAudio ) ? FALSE : TRUE; }
	void SetIsSendAudio(BOOL32 bIsSendAudio) { m_bySendAudio = bIsSendAudio ? 1 : 0; }
	
	BOOL32 IsCaptureMute(void) const { return ( 0 == m_byCaptureMute ) ? FALSE : TRUE; }
	void SetIsCaptureMute(BOOL32 bIsCaptureMutee) { m_byCaptureMute = bIsCaptureMutee ? 1 : 0; }
	
	u8 GetCaptureVolume(void) const { return m_byCaptureVolume; }
	void SetCaptureVolume(u8 byVolume) { m_byCaptureVolume = byVolume; }

	u8 GetBindedDVChnIdx(void) const { return m_byBindedDVChnIdx; }
	void SetBindedDVChnIdx(u8 byChnIdx) { m_byBindedDVChnIdx = byChnIdx; }

// 	HMEDIARES GetMediaH(void) const { return m_hMediaH; }
// 	void SetMediaH(const HMEDIARES hMediaH) { m_hMediaH = hMediaH; }

	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }

	TTransportAddr GetSwitchAddr(void) const { return m_tSwitchAddr; }
	void SetSwitchAddr(const TTransportAddr &tAddr) { m_tSwitchAddr = tAddr; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrAudRcvChnStatus : public TChnStatus
{
private:
    //HMEDIARES m_hMediaH;			//�������Դ���
	u8 m_byReceiveAudio;        //�Ƿ����ڽ�����Ƶ
	u8 m_byDecoderMute;			//����
	u8 m_byDecoderVolume;       //��������
	TLogicalChannel m_tChannel;   //�߼�ͨ��

	TTransportAddr m_tRtcpSwitchAddr;	//���ո��ն˵�RTCP�ش��������ַ
	TTransportAddr m_tRtcpDstAddr;       //���ն���ƵRTCPĿ�ĵ�ַ(ҵ���ϵ�Ŀ�ĵ�ַ)

public:
	TMtMgrAudRcvChnStatus() : m_byReceiveAudio(0), m_byDecoderMute(0), m_byDecoderVolume(0)//, m_hMediaH(INVALID_HMEDIARES)
	{
		m_tChannel.Clear();
		m_tRtcpSwitchAddr.SetNull();
		m_tRtcpDstAddr.SetNull();
	}

public:
// 	HMEDIARES GetMediaH(void) const { return m_hMediaH; }
// 	void SetMediaH(const HMEDIARES hMediaH) { m_hMediaH = hMediaH; }

	BOOL32 IsDecoderMute(void) const { return ( 0 == m_byDecoderMute ) ? FALSE : TRUE; }
	void SetIsDecoderMute(BOOL32 bIsDecoderMute) { m_byDecoderMute = bIsDecoderMute ? 1 : 0; }

	BOOL32 IsRecvAudio(void) const { return ( 0 == m_byReceiveAudio ) ? FALSE : TRUE; }
	void SetIsRecvAudio(BOOL32 bIsRecvAudio) { m_byReceiveAudio = bIsRecvAudio ? 1 : 0; }

	u8 GetDecoderVolume(void) const { return m_byDecoderVolume; }
	void SetDecoderVolume(u8 byVolume) { m_byDecoderVolume = byVolume; }

	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }

	TTransportAddr GetRtcpSwitchAddr(void) const { return m_tRtcpSwitchAddr; }
	void SetRtcpSwitchAddr(const TTransportAddr &tAddr) { m_tRtcpSwitchAddr = tAddr; }

	TTransportAddr GetRtcpDstAddr(void) const { return m_tRtcpDstAddr; }
	void SetRtcpDstAddr(const TTransportAddr &tAddr) { m_tRtcpDstAddr = tAddr; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrDVAudRcvChnStatus : public TChnStatus
{
private:
    //HMEDIARES m_hMediaH;			//�������Դ���
	u8 m_byBindedDVChnIdx;		//�󶨵�˫����Ƶ����ͨ��������
	u8 m_byReceiveAudio;        //�Ƿ����ڽ�����Ƶ
	u8 m_byDecoderMute;			//����
	u8 m_byDecoderVolume;       //��������
	TLogicalChannel m_tChannel;   //�߼�ͨ��

	TTransportAddr m_tRtcpSwitchAddr;	//���ո��ն˵�RTCP�ش��������ַ
	
public:
	TMtMgrDVAudRcvChnStatus() : m_byReceiveAudio(0), m_byDecoderMute(0), m_byDecoderVolume(0), m_byBindedDVChnIdx(INVALID_CHNNL)//, m_hMediaH(INVALID_HMEDIARES)
	{
		m_tChannel.Clear();
		m_tRtcpSwitchAddr.SetNull();
	}
	
public:
// 	HMEDIARES GetMediaH(void) const { return m_hMediaH; }
// 	void SetMediaH(const HMEDIARES hMediaH) { m_hMediaH = hMediaH; }
	
	BOOL32 IsDecoderMute(void) const { return ( 0 == m_byDecoderMute ) ? FALSE : TRUE; }
	void SetIsDecoderMute(BOOL32 bIsDecoderMute) { m_byDecoderMute = bIsDecoderMute ? 1 : 0; }
	
	BOOL32 IsRecvAudio(void) const { return ( 0 == m_byReceiveAudio ) ? FALSE : TRUE; }
	void SetIsRecvAudio(BOOL32 bIsRecvAudio) { m_byReceiveAudio = bIsRecvAudio ? 1 : 0; }
	
	u8 GetDecoderVolume(void) const { return m_byDecoderVolume; }
	void SetDecoderVolume(u8 byVolume) { m_byDecoderVolume = byVolume; }

	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }

	TTransportAddr GetRtcpSwitchAddr(void) const { return m_tRtcpSwitchAddr; }
	void SetRtcpSwitchAddr(const TTransportAddr &tAddr) { m_tRtcpSwitchAddr = tAddr; }

	u8 GetBindedDVChnIdx(void) const { return m_byBindedDVChnIdx; }
	void SetBindedDVChnIdx(u8 byChnIdx) { m_byBindedDVChnIdx = byChnIdx; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrT120DataSndChnStatus : public TChnStatus
{
private:
	TLogicalChannel m_tChannel;   //�߼�ͨ��
	
public:
	TMtMgrT120DataSndChnStatus()
	{
		m_tChannel.Clear();
	}

	void SetStatus(const TMtMgrT120DataSndChnStatus& tStatus)
	{
		SetChnStatus( (TChnStatus)tStatus );
		SetLogicalChannel( tStatus.GetLogicalChannel() );
	}

	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }

	void Clear()
	{
		TChnStatus::Clear();
		m_tChannel.Clear();
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrT120DataRcvChnStatus : public TChnStatus
{
private:
	TLogicalChannel m_tChannel;   //�߼�ͨ��
	
public:
	TMtMgrT120DataRcvChnStatus()
	{
		m_tChannel.Clear();
	}

	void SetStatus(const TMtMgrT120DataRcvChnStatus& tStatus)
	{
		SetChnStatus( (TChnStatus)tStatus );
		SetLogicalChannel( tStatus.GetLogicalChannel() );
	}
	
	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }

	void Clear()
	{
		TChnStatus::Clear();
		m_tChannel.Clear();
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrH224DataSndChnStatus : public TChnStatus
{
private:
	TLogicalChannel m_tChannel;   //�߼�ͨ��
	TTransportAddr m_tSwitchAddr;	//���յ�ַ

public:
	TMtMgrH224DataSndChnStatus()
	{
		Clear();
	}

	void Clear(void)
	{
		TChnStatus::Clear();
		m_tChannel.Clear();
		m_tSwitchAddr.SetNull();
	}

	void SetStatus(const TMtMgrH224DataSndChnStatus& tStatus)
	{
		SetChnStatus( (TChnStatus)tStatus );
		SetLogicalChannel( tStatus.GetLogicalChannel() );
		SetSwitchAddr( tStatus.GetSwitchAddr() );
	}
	
	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }

	TTransportAddr GetSwitchAddr(void) const { return m_tSwitchAddr; }
	void SetSwitchAddr(const TTransportAddr &tAddr) { m_tSwitchAddr = tAddr; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrH224DataRcvChnStatus : public TChnStatus
{
private:
	TLogicalChannel m_tChannel;   //�߼�ͨ��

	TTransportAddr m_tRtcpSwitchAddr;	//���ո��ն˵�RTCP�ش��������ַ
	
public:
	TMtMgrH224DataRcvChnStatus()
	{
		Clear();
	}

	void Clear(void)
	{
		TChnStatus::Clear();
		m_tChannel.Clear();
		m_tRtcpSwitchAddr.SetNull();
	}

	void SetStatus(const TMtMgrH224DataRcvChnStatus& tStatus)
	{
		SetChnStatus( (TChnStatus)tStatus );
		SetLogicalChannel( tStatus.GetLogicalChannel() );
		SetRtcpSwitchAddr( tStatus.GetRtcpSwitchAddr() );
	}
	
	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }

	TTransportAddr GetRtcpSwitchAddr(void) const { return m_tRtcpSwitchAddr; }
	void SetRtcpSwitchAddr(const TTransportAddr &tAddr) { m_tRtcpSwitchAddr = tAddr; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrMMcuDataSndChnStatus : public TChnStatus
{
private:
	TLogicalChannel m_tChannel;   //�߼�ͨ��
	
public:
	TMtMgrMMcuDataSndChnStatus()
	{
		m_tChannel.Clear();
	}

	void Clear(void)
	{
		TChnStatus::Clear();
		m_tChannel.Clear();
	}

	void SetStatus(const TMtMgrMMcuDataSndChnStatus& tStatus)
	{
		SetChnStatus( (TChnStatus)tStatus );
		SetLogicalChannel( tStatus.GetLogicalChannel() );
	}
	
	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMtMgrMMcuDataRcvChnStatus : public TChnStatus
{
private:
	TLogicalChannel m_tChannel;   //�߼�ͨ��
	
public:
	TMtMgrMMcuDataRcvChnStatus()
	{
		m_tChannel.Clear();;
	}

	void Clear(void)
	{
		TChnStatus::Clear();
		m_tChannel.Clear();
	}
	
	void SetStatus(const TMtMgrMMcuDataRcvChnStatus& tStatus)
	{
		SetChnStatus( (TChnStatus)tStatus );
		SetLogicalChannel( tStatus.GetLogicalChannel() );
	}

	TLogicalChannel GetLogicalChannel(void) const { return m_tChannel; }
	void SetLogicalChannel( const TLogicalChannel& tChannel ) { m_tChannel = tChannel; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


#ifdef WIN32
#pragma pack( pop )
#endif


/************************************************************************/
/*                                                                      */
/*                        ����ģ����Ϣ����			                    */
/*                     zjj20140513����mcuinnerstruct.h                  */
/************************************************************************/


//����ǽ�໭��ͨ����ϸ��Ϣ(len:43bytes)
struct THduVmpChannlInfo
{
public:
	u8 m_byHduEqpId;
	u8 m_byChIdx;
	u8 m_byScheIdx;		//��������
	u8 m_byVmpStyle;   //����ϳɷ�� ֵ1��ʾ4�����4����ͨ����
	u8 m_abyTvWallMember[HDUCHN_MODE_MAXCHNNUM]; //�ն�ID�������ն�ID��
	u8 m_abyMemberType[HDUCHN_MODE_MAXCHNNUM];   //����ǽ��Ա��������,TW_MEMBERTYPE_MCSSPEC,TW_MEMBERTYPE_SPEAKER...
	u8 m_abyHduMemChnIdx[HDUCHN_MODE_MAXCHNNUM]; //�ն�ͨ���ţ�Ϊ��֧�ֶ���

public:
	void Clear( void ){memset(this,0,sizeof(THduVmpChannlInfo));}
	
	BOOL32 IsValid ( void ) const
	{
		return (m_byHduEqpId != 0/* && m_byVmpStyle != 0*/);
	}

	void Print( void )
	{
		if(m_byHduEqpId != 0)
		{
			StaticLog("HduVmpChnnlInfo:EqpId<%d> ChnnIdx<%d> VmpStyle<%d>\n",m_byHduEqpId,m_byChIdx,m_byVmpStyle);
			for(u8 byIdx = 0; byIdx < HDU_MODEFOUR_MAX_SUBCHNNUM; byIdx++ )
			{
				if( m_abyTvWallMember[byIdx]!=0 || m_abyMemberType[byIdx]!=0 )
				{
					StaticLog("\tSubChnIdx:%d MemberId:%d MemberType:%d, MemChnIdx.%d\n",
						byIdx,m_abyTvWallMember[byIdx],m_abyMemberType[byIdx], m_abyHduMemChnIdx[byIdx]);
				}
			}
		}
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TEqpMaxMinCap
{
public:
	TVideoStreamCap m_atMaxCap[MAXNUM_EQP_ENCCHL];
	TVideoStreamCap m_atMinCap[MAXNUM_EQP_ENCCHL];
public:
	LPCSTR StringOfMedia( u8 byMediaType )
	{
		static s8 szMediaType[16];
		switch( byMediaType ) 
		{
		case MEDIA_TYPE_H261:
			sprintf( szMediaType, "%s%c", "H.261", 0 );
			break;
		case MEDIA_TYPE_H262:
			sprintf( szMediaType, "%s%c", "H.262", 0 );
			break;
		case MEDIA_TYPE_H263:
			sprintf( szMediaType, "%s%c", "H.263", 0 );
			break;
		case MEDIA_TYPE_H263PLUS:
			sprintf( szMediaType, "%s%c", "H.263+", 0 );
			break;
		case MEDIA_TYPE_H264:
			sprintf( szMediaType, "%s%c", "H.264", 0 );
			break;
		case MEDIA_TYPE_H265:
			sprintf( szMediaType, "%s%c", "H.265", 0 );
			break;
		case MEDIA_TYPE_MP4:
			sprintf( szMediaType, "%s%c", "MPEG-4", 0 );
			break;
		case MEDIA_TYPE_PCMA:
			sprintf( szMediaType, "%s%c", "PCMA", 0 );
			break;
		case MEDIA_TYPE_PCMU:
			sprintf( szMediaType, "%s%c", "PCMU", 0 );
			break;
		case MEDIA_TYPE_G722:
			sprintf( szMediaType, "%s%c", "G.722", 0 );
			break;
		case MEDIA_TYPE_G7221C:
			sprintf( szMediaType, "%s%c", "G.7221", 0 );
			break;
		case MEDIA_TYPE_G7231:
			sprintf( szMediaType, "%s%c", "G.7231", 0 );
			break;
		case MEDIA_TYPE_G728:
			sprintf( szMediaType, "%s%c", "G.728", 0 );
			break;
		case MEDIA_TYPE_G729:
			sprintf( szMediaType, "%s%c", "G.729", 0 );
			break;
		case MEDIA_TYPE_MP3:
			sprintf( szMediaType, "%s%c", "MP3", 0 );
			break;
		case MEDIA_TYPE_T120:
			sprintf( szMediaType, "%s%c", "T.120", 0 );
			break;
		case MEDIA_TYPE_H224:
			sprintf( szMediaType, "%s%c", "H.224", 0 );
			break;
		case MEDIA_TYPE_H239:
			sprintf( szMediaType, "%s%c", "H.239", 0 );
			break;
		case MEDIA_TYPE_MMCU:
			sprintf( szMediaType, "%s%c", "MMCU", 0 );
			break;
		default:
			sprintf( szMediaType, "%s(%u)%c", "Unknow", byMediaType, 0 );
			break;
		}
		return szMediaType;
	}

	LPCSTR StringOfRes( u8 byResolution )
	{
		static char szResType[16];
		
		switch( byResolution )
		{
		//��Ƶ��ʽ����
		case VIDEO_FORMAT_SQCIF:
			sprintf( szResType, "%s%c", "SQCIF", 0 );
			break;
		case VIDEO_FORMAT_QCIF:	
			sprintf( szResType, "%s%c", "QCIF", 0 );
			break;
		case VIDEO_FORMAT_CIF:              
			sprintf( szResType, "%s%c", "CIF", 0 );
			break;
		case VIDEO_FORMAT_2CIF:              
			sprintf( szResType, "%s%c", "2CIF", 0 );
			break;
		case VIDEO_FORMAT_4CIF:         
			sprintf( szResType, "%s%c", "4CIF", 0 );
			break;
		case VIDEO_FORMAT_16CIF:			
			sprintf( szResType, "%s%c", "16CIF", 0 );
			break;
		case VIDEO_FORMAT_AUTO:			
			sprintf( szResType, "%s%c", "AUTO", 0 );
			break;
    
		case VIDEO_FORMAT_SIF:             
			sprintf( szResType, "%s%c", "SIF", 0 );
			break;
		case VIDEO_FORMAT_2SIF:               
			sprintf( szResType, "%s%c", "2SIF", 0 );
			break;
		case VIDEO_FORMAT_4SIF:           
			sprintf( szResType, "%s%c", "4SIF", 0 );
			break;
    
		case VIDEO_FORMAT_VGA:            
			sprintf( szResType, "%s%c", "VGA", 0 );
			break;
		case VIDEO_FORMAT_SVGA:             
			sprintf( szResType, "%s%c", "SVGA", 0 );
			break;
		case VIDEO_FORMAT_XGA:             
			sprintf( szResType, "%s%c", "XGA", 0 );
			break;

		case VIDEO_FORMAT_WCIF:             
			sprintf( szResType, "%s%c", "WCIF", 0 );
			break;
    
		//�������ն˷ֱ��ʸı�
		case VIDEO_FORMAT_SQCIF_112x96:     
			sprintf( szResType, "%s%c", "SQCIF112x96", 0 );
			break;
		case VIDEO_FORMAT_SQCIF_96x80:     
			sprintf( szResType, "%s%c", "SQCIF96x80", 0 );
			break;
    
		// ����ֱ���
		case VIDEO_FORMAT_W4CIF:            
			sprintf( szResType, "%s%c", "W4CIF", 0 );
			break;
		case VIDEO_FORMAT_HD720:           
			sprintf( szResType, "%s%c", "HD720", 0 );
			break;
		case VIDEO_FORMAT_SXGA:             
			sprintf( szResType, "%s%c", "SXGA", 0 );
			break;
		case VIDEO_FORMAT_UXGA:             
			sprintf( szResType, "%s%c", "UXGA", 0 );
			break;
		case VIDEO_FORMAT_HD1080:             
			sprintf( szResType, "%s%c", "HD1080", 0 );
			break;
		case VIDEO_FORMAT_4K:             
			sprintf( szResType, "%s%c", "4K2K", 0 );
			break;

		//�Ǳ�ֱ���
		case VIDEO_FORMAT_1440x816:
			sprintf( szResType, "%s%c", "1440x816",0 );
			break;
		case VIDEO_FORMAT_1280x720:
			sprintf( szResType, "%s%c", "1280x720",0 );
			break;
		case VIDEO_FORMAT_960x544:
			sprintf( szResType, "%s%c", "960x544",0 );
			break;
		case VIDEO_FORMAT_640x368:
			sprintf( szResType, "%s%c", "640x368",0 );
			break;
		case VIDEO_FORMAT_480x272:
			sprintf( szResType, "%s%c", "480x272",0 );
			break;
		case VIDEO_FORMAT_384x272:
			sprintf( szResType, "%s%c", "384x272",0 );
			break;

		case VIDEO_FORMAT_720_960x544:
			sprintf( szResType, "%s%c", "960x544",0 );
			break;
		case VIDEO_FORMAT_720_864x480:
			sprintf( szResType, "%s%c", "864x480",0 );
			break;
		case VIDEO_FORMAT_720_640x368:
			sprintf( szResType, "%s%c", "640x368",0 );
			break;
		case VIDEO_FORMAT_720_432x240:
			sprintf( szResType, "%s%c", "432x240",0 );
			break;
		case VIDEO_FORMAT_720_320x192:
			sprintf( szResType, "%s%c", "320x192",0 );
			break;
		case VIDEO_FORMAT_640x544:
			sprintf( szResType, "%s%c", "640x544",0 );
			break;
		case VIDEO_FORMAT_320x272:
			sprintf( szResType, "%s%c", "320x272",0 );
			break;
		case VIDEO_FORMAT_WXGA:
			sprintf( szResType, "%s%c", "WXGA",0 );
			break;
		default:
			sprintf( szResType, "%s(%d)%c", "Unknown", byResolution, 0 );
			break;
		}

		return szResType;
	}
	void print(BOOL32 bMax=TRUE)
	{
		u8 byIdx = 0;
		BOOL32 bPrintProfile = TRUE;
		while(byIdx < MAXNUM_EQP_ENCCHL)
		{
			bPrintProfile = TRUE;
			if (bMax)
			{
				if (m_atMaxCap[byIdx].GetMediaType() != MEDIA_TYPE_NULL)
				{
					if (m_atMaxCap[byIdx].GetMediaType() == MEDIA_TYPE_H265)
					{
						bPrintProfile = FALSE;
					}
					if (bPrintProfile)
					{
						StaticLog( " %-2d   %-6d  %-12s  %-12s  %-4d    %-2d\n",
							byIdx, m_atMaxCap[byIdx].GetMaxBitRate(), StringOfMedia( m_atMaxCap[byIdx].GetMediaType() ), 
							StringOfRes(m_atMaxCap[byIdx].GetResolution() ), m_atMaxCap[byIdx].GetUserDefFrameRate(),
							m_atMaxCap[byIdx].GetH264ProfileAttrb());
					}
					else
					{
						StaticLog( " %-2d   %-6d  %-12s  %-12s  %-4d \n",
							byIdx, m_atMaxCap[byIdx].GetMaxBitRate(), StringOfMedia( m_atMaxCap[byIdx].GetMediaType() ), 
							StringOfRes(m_atMaxCap[byIdx].GetResolution() ), m_atMaxCap[byIdx].GetUserDefFrameRate());
					}
					
				}
			}
			else
			{
				if (m_atMinCap[byIdx].GetMediaType() != MEDIA_TYPE_NULL)
				{
					if (m_atMinCap[byIdx].GetMediaType() == MEDIA_TYPE_H265)
					{
						bPrintProfile = FALSE;
					}

					if (bPrintProfile)
					{
						StaticLog( " %-2d   %-6d  %-12s  %-12s  %-4d    %-2d\n",
							byIdx, m_atMinCap[byIdx].GetMaxBitRate(), StringOfMedia( m_atMinCap[byIdx].GetMediaType() ), 
							StringOfRes(m_atMinCap[byIdx].GetResolution() ), m_atMinCap[byIdx].GetUserDefFrameRate(),
						m_atMinCap[byIdx].GetH264ProfileAttrb());
					}
					else
					{
						StaticLog( " %-2d   %-6d  %-12s  %-12s  %-4d \n",
							byIdx, m_atMinCap[byIdx].GetMaxBitRate(), StringOfMedia( m_atMinCap[byIdx].GetMediaType() ), 
							StringOfRes(m_atMinCap[byIdx].GetResolution() ), m_atMinCap[byIdx].GetUserDefFrameRate());
					}
				}
			}
			byIdx++;
		}
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// ��Ƶ�����������ģ��ṹ
struct TConfEqpEncInfo
{
public:
	// xmpu xmpu5
	TEqpMaxMinCap m_tXVmpCap;
	TEqpMaxMinCap m_tXVBasCap;
	TEqpMaxMinCap m_tXDSBasCap;
	// mpu
	TEqpMaxMinCap m_tVmpCap;
	TEqpMaxMinCap m_tVBasCap;
	TEqpMaxMinCap m_tDSBasCap;
public:


	BOOL32 GetEqpCap(TEqpMaxMinCap* pVidCap, u8 &byCapNum, u8 byMode, u8 byEqpType,u16 wConfBR, BOOL32 bXmpu=TRUE)
	{
		if (pVidCap == NULL)
		{
			return FALSE;
		}

		byCapNum = 0;

		if (bXmpu)
		{
			if (byMode == MODE_VIDEO)
			{
				if (byEqpType == EQP_TYPE_BAS)
				{
					memcpy(pVidCap, &m_tXVBasCap, sizeof(m_tXVBasCap));
				}
				else if (byEqpType == EQP_TYPE_VMP)
				{
					memcpy(pVidCap, &m_tXVmpCap, sizeof(m_tXVmpCap));
				}
				else
				{
					return FALSE;
				}
				
			}
			else if (byMode == MODE_SECVIDEO)
			{
				memcpy(pVidCap, &m_tXDSBasCap, sizeof(m_tXDSBasCap));
			}
			else
			{
				return FALSE;
			}

		} 
		else
		{
			if (byMode == MODE_VIDEO)
			{
				if (byEqpType == EQP_TYPE_BAS)
				{
					memcpy(pVidCap, &m_tVBasCap, sizeof(m_tVBasCap));
				}
				else if (byEqpType == EQP_TYPE_VMP)
				{
					memcpy(pVidCap, &m_tVmpCap, sizeof(m_tVmpCap));
				}
			}
			else if (byMode == MODE_SECVIDEO)
			{
				memcpy(pVidCap, &m_tDSBasCap, sizeof(m_tDSBasCap));
			}
			else
			{
				return FALSE;
			}
		}
		u8 byIdx = 0;
		while(byIdx < MAXNUM_EQP_ENCCHL)
		{
			if (pVidCap->m_atMaxCap[byIdx].GetMediaType() != MEDIA_TYPE_NULL)
			{
				pVidCap->m_atMaxCap[byIdx].SetMaxBitRate(wConfBR);
				byCapNum++;
			}
			byIdx++;
		}
		
		return TRUE;
	}

	void print(BOOL32 bPrintMpu = TRUE)
	{
		StaticLog( "----------------------XMPU or XMPU5---------------------------\n");
		StaticLog( "VMP:\n");
		m_tXVmpCap.print(TRUE);
		StaticLog( "\n");
		m_tXVmpCap.print(FALSE);
		StaticLog( "VBAS:\n");
		m_tXVBasCap.print(TRUE);
		StaticLog( "\n");
		m_tXVBasCap.print(FALSE);
		StaticLog( "DSBAS:\n");
		m_tXDSBasCap.print(TRUE);
		StaticLog( "\n");
		m_tXDSBasCap.print(FALSE);

		if (bPrintMpu)
		{
			StaticLog( "----------------------MPU----------------------------\n");
			StaticLog( "VMP:\n");
			m_tVmpCap.print(TRUE);
			StaticLog( "\n");
			m_tVmpCap.print(FALSE);
			StaticLog( "VBAS:\n");
			m_tVBasCap.print(TRUE);
			StaticLog( "\n");
			m_tVBasCap.print(FALSE);
			StaticLog( "DSBAS:\n");
			m_tDSBasCap.print(TRUE);
			StaticLog( "\n");
			m_tDSBasCap.print(FALSE);
		}
		
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TAllConfEqpEncCfg
{
public:
	TConfEqpEncInfo m_tHD1080_60;
	TConfEqpEncInfo m_tHD1080_30;
	TConfEqpEncInfo m_tHD720_60;
	TConfEqpEncInfo m_tHD720_30;
	TConfEqpEncInfo m_tSD;

	//֧��H265��ʽ
	TConfEqpEncInfo m_t265HD4k2k_30;
	TConfEqpEncInfo m_t265HD1080_60;
	TConfEqpEncInfo m_t265HD1080_30;
	TConfEqpEncInfo m_t265HD720_60;
	TConfEqpEncInfo m_t265HD720_30;
	TConfEqpEncInfo m_t265SD;
public:
	//zyq[2016/5/3] ����Խ�����ı�������
	TAllConfEqpEncCfg( BOOL32 bIsHired = FALSE )
	{
		if ( bIsHired )
		{
			SetHiredConfEqpEnc();
		}
		else
		{
			SetNoHiredConfEqpEnc();
		}

		SetH265ConfEqpEnc();
	}

	void SetHiredConfEqpEnc()
	{

		u16 wMaxBitRate= 8192;

		// 1080
		TVideoStreamCap t264_1080_60_HP_HB;
		t264_1080_60_HP_HB.SetUserDefFrameRate(60);
		t264_1080_60_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_1080_60_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_60_HP_HB.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_60_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_1080_30_HP_HB;
		t264_1080_30_HP_HB.SetUserDefFrameRate(30);
		t264_1080_30_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_1080_30_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_30_HP_HB.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_30_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_1080_25_HP_1472;
		t264_1080_25_HP_1472.SetUserDefFrameRate(25);
		t264_1080_25_HP_1472.SetMaxBitRate(1472);	// ??xmpuΪ��Ҫ2486
		t264_1080_25_HP_1472.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_25_HP_1472.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_25_HP_1472.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_1080_25_HP_2486;
		t264_1080_25_HP_2486.SetUserDefFrameRate(25);
		t264_1080_25_HP_2486.SetMaxBitRate(2486);	// ??xmpuΪ��Ҫ2486
		t264_1080_25_HP_2486.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_25_HP_2486.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_25_HP_2486.SetH264ProfileAttrb(emHpAttrb);

		// 720
		TVideoStreamCap t264_720_60_HP_HB;
		t264_720_60_HP_HB.SetUserDefFrameRate(60);
		t264_720_60_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_720_60_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_720_60_HP_HB.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_60_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_720_30_HP_HB;
		t264_720_30_HP_HB.SetUserDefFrameRate(30);
		t264_720_30_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_720_30_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_720_30_HP_HB.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_30_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_720_50_HP_1472;
		t264_720_50_HP_1472.SetUserDefFrameRate(50);
		t264_720_50_HP_1472.SetMaxBitRate(1472);
		t264_720_50_HP_1472.SetMediaType(MEDIA_TYPE_H264);
		t264_720_50_HP_1472.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_50_HP_1472.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_720_30_HP_448;
		t264_720_30_HP_448.SetUserDefFrameRate(30);
		t264_720_30_HP_448.SetMaxBitRate(448);
		t264_720_30_HP_448.SetMediaType(MEDIA_TYPE_H264);
		t264_720_30_HP_448.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_30_HP_448.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_720_25_HP_448;
		t264_720_25_HP_448.SetUserDefFrameRate(25);
		t264_720_25_HP_448.SetMaxBitRate(448);
		t264_720_25_HP_448.SetMediaType(MEDIA_TYPE_H264);
		t264_720_25_HP_448.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_25_HP_448.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_720_25_HP_1472;
		t264_720_25_HP_1472.SetUserDefFrameRate(25);
		t264_720_25_HP_1472.SetMaxBitRate(1472);
		t264_720_25_HP_1472.SetMediaType(MEDIA_TYPE_H264);
		t264_720_25_HP_1472.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_25_HP_1472.SetH264ProfileAttrb(emHpAttrb);

		// w4cif
		TVideoStreamCap t264_W4CIF_25_HP_HB;
		t264_W4CIF_25_HP_HB.SetUserDefFrameRate(25);
		t264_W4CIF_25_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_W4CIF_25_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_W4CIF_25_HP_HB.SetResolution(VIDEO_FORMAT_W4CIF);
		t264_W4CIF_25_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_W4CIF_25_HP_704;
		t264_W4CIF_25_HP_704.SetUserDefFrameRate(25);
		t264_W4CIF_25_HP_704.SetMaxBitRate(704);
		t264_W4CIF_25_HP_704.SetMediaType(MEDIA_TYPE_H264);
		t264_W4CIF_25_HP_704.SetResolution(VIDEO_FORMAT_W4CIF);
		t264_W4CIF_25_HP_704.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_W4CIF_25_HP_256;
		t264_W4CIF_25_HP_256.SetUserDefFrameRate(25);
		t264_W4CIF_25_HP_256.SetMaxBitRate(256);
		t264_W4CIF_25_HP_256.SetMediaType(MEDIA_TYPE_H264);
		t264_W4CIF_25_HP_256.SetResolution(VIDEO_FORMAT_W4CIF);
		t264_W4CIF_25_HP_256.SetH264ProfileAttrb(emHpAttrb);
		
		// 4cif
		TVideoStreamCap t264_4CIF_25_BP_HB;
		t264_4CIF_25_BP_HB.SetUserDefFrameRate(25);
		t264_4CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_4CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_4CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_4CIF);
		t264_4CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_4CIF_25_HP_192;
		t264_4CIF_25_HP_192.SetUserDefFrameRate(25);
		t264_4CIF_25_HP_192.SetMaxBitRate(192);
		t264_4CIF_25_HP_192.SetMediaType(MEDIA_TYPE_H264);
		t264_4CIF_25_HP_192.SetResolution(VIDEO_FORMAT_4CIF);
		t264_4CIF_25_HP_192.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_4CIF_25_HP_448;
		t264_4CIF_25_HP_448.SetUserDefFrameRate(25);
		t264_4CIF_25_HP_448.SetMaxBitRate(448);
		t264_4CIF_25_HP_448.SetMediaType(MEDIA_TYPE_H264);
		t264_4CIF_25_HP_448.SetResolution(VIDEO_FORMAT_4CIF);
		t264_4CIF_25_HP_448.SetH264ProfileAttrb(emHpAttrb);

		// wcif
		TVideoStreamCap t264_WCIF_25_HP_HB;
		t264_WCIF_25_HP_HB.SetUserDefFrameRate(25);
		t264_WCIF_25_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_WCIF_25_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_WCIF_25_HP_HB.SetResolution(VIDEO_FORMAT_WCIF);
		t264_WCIF_25_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_WCIF_25_HP_192;
		t264_WCIF_25_HP_192.SetUserDefFrameRate(25);
		t264_WCIF_25_HP_192.SetMaxBitRate(192);
		t264_WCIF_25_HP_192.SetMediaType(MEDIA_TYPE_H264);
		t264_WCIF_25_HP_192.SetResolution(VIDEO_FORMAT_WCIF);
		t264_WCIF_25_HP_192.SetH264ProfileAttrb(emHpAttrb);

		// cif HP
		TVideoStreamCap t264_CIF_25_HP_HB;
		t264_CIF_25_HP_HB.SetUserDefFrameRate(25);
		t264_CIF_25_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_CIF_25_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_HP_HB.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_CIF_25_HP_64;
		t264_CIF_25_HP_64.SetUserDefFrameRate(25);
		t264_CIF_25_HP_64.SetMaxBitRate(64);
		t264_CIF_25_HP_64.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_HP_64.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_HP_64.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_CIF_25_HP_192;
		t264_CIF_25_HP_192.SetUserDefFrameRate(25);
		t264_CIF_25_HP_192.SetMaxBitRate(192);
		t264_CIF_25_HP_192.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_HP_192.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_HP_192.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_CIF_25_BP_64;
		t264_CIF_25_BP_64.SetUserDefFrameRate(25);
		t264_CIF_25_BP_64.SetMaxBitRate(64);
		t264_CIF_25_BP_64.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_BP_64.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_BP_64.SetH264ProfileAttrb(emBpAttrb);


		// mp4
		TVideoStreamCap tMP4_4CIF_25_BP_HB;
		tMP4_4CIF_25_BP_HB.SetFrameRate(25);
		tMP4_4CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		tMP4_4CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_MP4);
		tMP4_4CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_4CIF);
		tMP4_4CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap tMP4_CIF_25_BP_HB;
		tMP4_CIF_25_BP_HB.SetFrameRate(25);
		tMP4_CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		tMP4_CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_MP4);
		tMP4_CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_CIF);
		tMP4_CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap tMP4_CIF_25_BP_64;
		tMP4_CIF_25_BP_64.SetFrameRate(25);
		tMP4_CIF_25_BP_64.SetMaxBitRate(64);
		tMP4_CIF_25_BP_64.SetMediaType(MEDIA_TYPE_MP4);
		tMP4_CIF_25_BP_64.SetResolution(VIDEO_FORMAT_CIF);
		tMP4_CIF_25_BP_64.SetH264ProfileAttrb(emBpAttrb);

		// h263
		TVideoStreamCap t263_CIF_25_BP_HB;
		t263_CIF_25_BP_HB.SetFrameRate(25);
		t263_CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		t263_CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_H263);
		t263_CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_CIF);
		t263_CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t263_CIF_25_BP_64;
		t263_CIF_25_BP_64.SetFrameRate(25);
		t263_CIF_25_BP_64.SetMaxBitRate(64);
		t263_CIF_25_BP_64.SetMediaType(MEDIA_TYPE_H263);
		t263_CIF_25_BP_64.SetResolution(VIDEO_FORMAT_CIF);
		t263_CIF_25_BP_64.SetH264ProfileAttrb(emBpAttrb);

		//XVMP 1080*60
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[0] = t264_1080_60_HP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[1] = t264_720_30_HP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[3] = t264_W4CIF_25_HP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_BP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[5] = tMP4_4CIF_25_BP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[6] = t263_CIF_25_BP_HB;
		
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[0] = t264_1080_25_HP_1472;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[1] = t264_720_25_HP_448;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[2] = t264_4CIF_25_HP_192;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[3] = t264_CIF_25_HP_64;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[4] = t264_CIF_25_BP_64;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[5] = tMP4_CIF_25_BP_64;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[6] = t263_CIF_25_BP_64;
		//XVMP 1080*30
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[0] = t264_1080_30_HP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[1] = t264_720_30_HP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[3] = t264_W4CIF_25_HP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_BP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[5] = tMP4_4CIF_25_BP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[6] = t263_CIF_25_BP_HB;

		m_tHD1080_30.m_tXVmpCap.m_atMinCap[0] = t264_1080_25_HP_1472;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[1] = t264_720_25_HP_448;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[2] = t264_4CIF_25_HP_192;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[3] = t264_CIF_25_HP_64;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[4] = t264_CIF_25_BP_64;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[5] = tMP4_CIF_25_BP_64;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[6] = t263_CIF_25_BP_64;

		//XVMP 720*60
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[0] = t264_720_60_HP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[1] = t264_720_30_HP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[3] = t264_W4CIF_25_HP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_BP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[5] = tMP4_4CIF_25_BP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[6] = t263_CIF_25_BP_HB;
		
		m_tHD720_60.m_tXVmpCap.m_atMinCap[0] = t264_720_50_HP_1472;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[1] = t264_720_25_HP_448;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[2] = t264_4CIF_25_HP_192;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[3] = t264_CIF_25_HP_64;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[4] = t264_CIF_25_BP_64;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[5] = tMP4_CIF_25_BP_64;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[6] = t263_CIF_25_BP_64;

		//XVMP 720*30
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[0] = t264_720_30_HP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[1] = t264_720_30_HP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[3] = t264_W4CIF_25_HP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_BP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[5] = tMP4_4CIF_25_BP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[6] = t263_CIF_25_BP_HB;
		
		m_tHD720_30.m_tXVmpCap.m_atMinCap[0] = t264_720_25_HP_1472;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[1] = t264_720_25_HP_448;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[2] = t264_4CIF_25_HP_192;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[3] = t264_CIF_25_HP_64;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[4] = t264_CIF_25_BP_64;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[5] = tMP4_CIF_25_BP_64;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[6] = t263_CIF_25_BP_64;

		//XVMP SD
		m_tSD.m_tXVmpCap.m_atMaxCap[0] = t264_W4CIF_25_HP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[1] = t264_W4CIF_25_HP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[2] = t264_WCIF_25_HP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[3] = t264_WCIF_25_HP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_BP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[5] = tMP4_4CIF_25_BP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[6] = t263_CIF_25_BP_HB;
		
		m_tSD.m_tXVmpCap.m_atMinCap[0] = t264_W4CIF_25_HP_704;
		m_tSD.m_tXVmpCap.m_atMinCap[1] = t264_4CIF_25_HP_192;
		m_tSD.m_tXVmpCap.m_atMinCap[2] = t264_CIF_25_HP_192;
		m_tSD.m_tXVmpCap.m_atMinCap[3] = t264_CIF_25_HP_64;
		m_tSD.m_tXVmpCap.m_atMinCap[4] = t264_CIF_25_BP_64;
		m_tSD.m_tXVmpCap.m_atMinCap[5] = tMP4_CIF_25_BP_64;
		m_tSD.m_tXVmpCap.m_atMinCap[6] = t263_CIF_25_BP_64;

		// VMP ��XVMP����ֻ�����ֵ��֡�ʣ�������ֵ
		memcpy (m_tHD1080_60.m_tVmpCap.m_atMaxCap, m_tHD1080_60.m_tXVmpCap.m_atMaxCap, sizeof(m_tHD1080_60.m_tXVmpCap.m_atMaxCap) );
		m_tHD1080_60.m_tVmpCap.m_atMaxCap[0].SetUserDefFrameRate(30);
		memcpy (m_tHD1080_60.m_tVmpCap.m_atMinCap, m_tHD1080_60.m_tXVmpCap.m_atMinCap, sizeof(m_tHD1080_60.m_tXVmpCap.m_atMinCap) );
		m_tHD1080_60.m_tVmpCap.m_atMinCap[0] = t264_1080_25_HP_1472;

		memcpy (m_tHD1080_30.m_tVmpCap.m_atMaxCap, m_tHD1080_30.m_tXVmpCap.m_atMaxCap, sizeof(m_tHD1080_30.m_tXVmpCap.m_atMaxCap) );
		memcpy (m_tHD1080_30.m_tVmpCap.m_atMinCap, m_tHD1080_30.m_tXVmpCap.m_atMinCap, sizeof(m_tHD1080_30.m_tXVmpCap.m_atMinCap) );

		memcpy (m_tHD720_30.m_tVmpCap.m_atMaxCap, m_tHD720_30.m_tXVmpCap.m_atMaxCap, sizeof(m_tHD720_30.m_tXVmpCap.m_atMaxCap) );
		memcpy (m_tHD720_30.m_tVmpCap.m_atMinCap, m_tHD720_30.m_tXVmpCap.m_atMinCap, sizeof(m_tHD720_30.m_tXVmpCap.m_atMinCap) );

		memcpy (m_tSD.m_tVmpCap.m_atMaxCap, m_tSD.m_tXVmpCap.m_atMaxCap, sizeof(m_tSD.m_tXVmpCap.m_atMaxCap) );
		memcpy (m_tSD.m_tVmpCap.m_atMinCap, m_tSD.m_tXVmpCap.m_atMinCap, sizeof(m_tSD.m_tXVmpCap.m_atMinCap) );
		// ���� mpu ���� w4cif �������ն˲��ή�ֱ��ʣ��ն���ʾ���кڱߣ���ҵ�񲻲����ý��������������֡��ʵ�֣�ͬʱ��������� [6/1/2017 wangshengke]
		m_tSD.m_tVmpCap.m_atMinCap[0] = t264_W4CIF_25_HP_256;
		m_tSD.m_tVmpCap.m_atMinCap[2] = t264_WCIF_25_HP_192;

		// XBAS 1080*60
		m_tHD1080_60.m_tXVBasCap.m_atMaxCap[0] = t264_1080_60_HP_HB;
		m_tHD1080_60.m_tXVBasCap.m_atMaxCap[1] = t264_720_30_HP_HB;
		m_tHD1080_60.m_tXVBasCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD1080_60.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_BP_HB;
		m_tHD1080_60.m_tXVBasCap.m_atMaxCap[4] = tMP4_CIF_25_BP_HB;
		
		m_tHD1080_60.m_tXVBasCap.m_atMinCap[0] = t264_1080_25_HP_1472;
		m_tHD1080_60.m_tXVBasCap.m_atMinCap[1] = t264_720_25_HP_448;
		m_tHD1080_60.m_tXVBasCap.m_atMinCap[2] = t264_CIF_25_HP_64;
		m_tHD1080_60.m_tXVBasCap.m_atMinCap[3] = t264_CIF_25_BP_64;
		m_tHD1080_60.m_tXVBasCap.m_atMinCap[4] = tMP4_CIF_25_BP_64;
		//XBAS 1080*30
		m_tHD1080_30.m_tXVBasCap.m_atMaxCap[0] = t264_1080_30_HP_HB;
		m_tHD1080_30.m_tXVBasCap.m_atMaxCap[1] = t264_720_30_HP_HB;
		m_tHD1080_30.m_tXVBasCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD1080_30.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_BP_HB;
		m_tHD1080_30.m_tXVBasCap.m_atMaxCap[4] = tMP4_CIF_25_BP_HB;

		m_tHD1080_30.m_tXVBasCap.m_atMinCap[0] = t264_1080_25_HP_1472;
		m_tHD1080_30.m_tXVBasCap.m_atMinCap[1] = t264_720_25_HP_448;
		m_tHD1080_30.m_tXVBasCap.m_atMinCap[2] = t264_CIF_25_HP_64;
		m_tHD1080_30.m_tXVBasCap.m_atMinCap[3] = t264_CIF_25_BP_64;
		m_tHD1080_30.m_tXVBasCap.m_atMinCap[4] = tMP4_CIF_25_BP_64;

		//XBAS 720*60
		m_tHD720_60.m_tXVBasCap.m_atMaxCap[0] = t264_720_60_HP_HB;
		m_tHD720_60.m_tXVBasCap.m_atMaxCap[1] = t264_720_30_HP_HB;
		m_tHD720_60.m_tXVBasCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD720_60.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_BP_HB;
		m_tHD720_60.m_tXVBasCap.m_atMaxCap[4] = tMP4_CIF_25_BP_HB;
		
		m_tHD720_60.m_tXVBasCap.m_atMinCap[0] = t264_720_50_HP_1472;
		m_tHD720_60.m_tXVBasCap.m_atMinCap[1] = t264_720_25_HP_448;
		m_tHD720_60.m_tXVBasCap.m_atMinCap[2] = t264_CIF_25_HP_64;
		m_tHD720_60.m_tXVBasCap.m_atMinCap[3] = t264_CIF_25_BP_64;
		m_tHD720_60.m_tXVBasCap.m_atMinCap[4] = tMP4_CIF_25_BP_64;

		//XBAS 720*30
		m_tHD720_30.m_tXVBasCap.m_atMaxCap[0] = t264_720_30_HP_HB;
		m_tHD720_30.m_tXVBasCap.m_atMaxCap[1] = t264_720_30_HP_HB;
		m_tHD720_30.m_tXVBasCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD720_30.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_BP_HB;
		m_tHD720_30.m_tXVBasCap.m_atMaxCap[4] = tMP4_CIF_25_BP_HB;
		
		m_tHD720_30.m_tXVBasCap.m_atMinCap[0] = t264_720_25_HP_1472;
		m_tHD720_30.m_tXVBasCap.m_atMinCap[1] = t264_720_25_HP_448;
		m_tHD720_30.m_tXVBasCap.m_atMinCap[2] = t264_CIF_25_HP_64;
		m_tHD720_30.m_tXVBasCap.m_atMinCap[3] = t264_CIF_25_BP_64;
		m_tHD720_30.m_tXVBasCap.m_atMinCap[4] = tMP4_CIF_25_BP_64;

		//XBAS SD
		m_tSD.m_tXVBasCap.m_atMaxCap[0] = t264_W4CIF_25_HP_HB;
		m_tSD.m_tXVBasCap.m_atMaxCap[1] = t264_WCIF_25_HP_HB;
		m_tSD.m_tXVBasCap.m_atMaxCap[2] = t264_WCIF_25_HP_HB;
		m_tSD.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_BP_HB;
		m_tSD.m_tXVBasCap.m_atMaxCap[4] = tMP4_CIF_25_BP_HB;
		
		m_tSD.m_tXVBasCap.m_atMinCap[0] = t264_4CIF_25_HP_192;
		m_tSD.m_tXVBasCap.m_atMinCap[1] = t264_CIF_25_HP_192;
		m_tSD.m_tXVBasCap.m_atMinCap[2] = t264_CIF_25_HP_64;
		m_tSD.m_tXVBasCap.m_atMinCap[3] = t264_CIF_25_BP_64;
		m_tSD.m_tXVBasCap.m_atMinCap[4] = tMP4_CIF_25_BP_64;

		// BAS
		memcpy (m_tHD1080_60.m_tVBasCap.m_atMaxCap, m_tHD1080_60.m_tXVBasCap.m_atMaxCap, sizeof(m_tHD1080_60.m_tXVBasCap.m_atMaxCap) );
		m_tHD1080_60.m_tVBasCap.m_atMaxCap[0].SetUserDefFrameRate(30);
		memcpy (m_tHD1080_60.m_tVBasCap.m_atMinCap, m_tHD1080_60.m_tXVBasCap.m_atMinCap, sizeof(m_tHD1080_60.m_tXVBasCap.m_atMinCap) );
		m_tHD1080_60.m_tVBasCap.m_atMinCap[0] = t264_1080_25_HP_1472;

		memcpy (m_tHD1080_30.m_tVBasCap.m_atMaxCap, m_tHD1080_30.m_tXVBasCap.m_atMaxCap, sizeof(m_tHD1080_30.m_tXVBasCap.m_atMaxCap) );
		memcpy (m_tHD1080_30.m_tVBasCap.m_atMinCap, m_tHD1080_30.m_tXVBasCap.m_atMinCap, sizeof(m_tHD1080_30.m_tXVBasCap.m_atMinCap) );
		
		memcpy (m_tHD720_30.m_tVBasCap.m_atMaxCap, m_tHD720_30.m_tXVBasCap.m_atMaxCap, sizeof(m_tHD720_30.m_tXVBasCap.m_atMaxCap) );
		memcpy (m_tHD720_30.m_tVBasCap.m_atMinCap, m_tHD720_30.m_tXVBasCap.m_atMinCap, sizeof(m_tHD720_30.m_tXVBasCap.m_atMinCap) );
		
		memcpy (m_tSD.m_tVBasCap.m_atMaxCap, m_tSD.m_tXVBasCap.m_atMaxCap, sizeof(m_tSD.m_tXVBasCap.m_atMaxCap) );
		memcpy (m_tSD.m_tVBasCap.m_atMinCap, m_tSD.m_tXVBasCap.m_atMinCap, sizeof(m_tSD.m_tXVBasCap.m_atMinCap) );
		// ���� mpu ���� w4cif �������ն˲��ή�ֱ��ʣ��ն���ʾ���кڱߣ���ҵ�񲻲����ý��������������֡��ʵ�֣�ͬʱ��������� [6/1/2017 wangshengke]
		m_tSD.m_tVBasCap.m_atMinCap[0] = t264_W4CIF_25_HP_256;
		m_tSD.m_tVBasCap.m_atMinCap[1] = t264_WCIF_25_HP_192;

		// ˫����������
		// SXGA
// 		TVideoStreamCap t264_SXGA_30_HP_HB;
// 		t264_SXGA_30_HP_HB.SetUserDefFrameRate(30);
// 		t264_SXGA_30_HP_HB.SetMaxBitRate(wMaxBitRate);
// 		t264_SXGA_30_HP_HB.SetMediaType(MEDIA_TYPE_H264);
// 		t264_SXGA_30_HP_HB.SetResolution(VIDEO_FORMAT_SXGA);
// 		t264_SXGA_30_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_SXGA_20_HP_HB;
		t264_SXGA_20_HP_HB.SetUserDefFrameRate(20);
		t264_SXGA_20_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_SXGA_20_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_SXGA_20_HP_HB.SetResolution(VIDEO_FORMAT_SXGA);
		t264_SXGA_20_HP_HB.SetH264ProfileAttrb(emHpAttrb);
// 		TVideoStreamCap t264_SXGA_25_HP_448;
// 		t264_SXGA_25_HP_448.SetUserDefFrameRate(25);
// 		t264_SXGA_25_HP_448.SetMaxBitRate(448);
// 		t264_SXGA_25_HP_448.SetMediaType(MEDIA_TYPE_H264);
// 		t264_SXGA_25_HP_448.SetResolution(VIDEO_FORMAT_SXGA);
// 		t264_SXGA_25_HP_448.SetH264ProfileAttrb(emHpAttrb);
// 		TVideoStreamCap t264_SXGA_20_BP_448;
// 		t264_SXGA_20_BP_448.SetUserDefFrameRate(20);
// 		t264_SXGA_20_BP_448.SetMaxBitRate(448);
// 		t264_SXGA_20_BP_448.SetMediaType(MEDIA_TYPE_H264);
// 		t264_SXGA_20_BP_448.SetResolution(VIDEO_FORMAT_SXGA);
// 		t264_SXGA_20_BP_448.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_SXGA_5_HP_192;
		t264_SXGA_5_HP_192.SetUserDefFrameRate(5);
		t264_SXGA_5_HP_192.SetMaxBitRate(192);
		t264_SXGA_5_HP_192.SetMediaType(MEDIA_TYPE_H264);
		t264_SXGA_5_HP_192.SetResolution(VIDEO_FORMAT_SXGA);
		t264_SXGA_5_HP_192.SetH264ProfileAttrb(emHpAttrb);
		// UXGA
// 		TVideoStreamCap t264_UXGA_20_HP_448;
// 		t264_UXGA_20_HP_448.SetUserDefFrameRate(20);
// 		t264_UXGA_20_HP_448.SetMaxBitRate(448);
// 		t264_UXGA_20_HP_448.SetMediaType(MEDIA_TYPE_H264);
// 		t264_UXGA_20_HP_448.SetResolution(VIDEO_FORMAT_UXGA);
// 		t264_UXGA_20_HP_448.SetH264ProfileAttrb(emHpAttrb);
		// XGA
// 		TVideoStreamCap t264_XGA_30_HP_HB;
// 		t264_XGA_30_HP_HB.SetUserDefFrameRate(30);
// 		t264_XGA_30_HP_HB.SetMaxBitRate(wMaxBitRate);
// 		t264_XGA_30_HP_HB.SetMediaType(MEDIA_TYPE_H264);
// 		t264_XGA_30_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
// 		t264_XGA_30_HP_HB.SetH264ProfileAttrb(emHpAttrb);
// 		TVideoStreamCap t264_XGA_30_BP_HB;
// 		t264_XGA_30_BP_HB.SetUserDefFrameRate(30);
// 		t264_XGA_30_BP_HB.SetMaxBitRate(wMaxBitRate);
// 		t264_XGA_30_BP_HB.SetMediaType(MEDIA_TYPE_H264);
// 		t264_XGA_30_BP_HB.SetResolution(VIDEO_FORMAT_XGA);
// 		t264_XGA_30_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_XGA_20_HP_HB;
		t264_XGA_20_HP_HB.SetUserDefFrameRate(20);
		t264_XGA_20_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_20_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_20_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_20_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_XGA_10_HP_HB;
		t264_XGA_10_HP_HB.SetUserDefFrameRate(10);
		t264_XGA_10_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_10_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_10_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_10_HP_HB.SetH264ProfileAttrb(emHpAttrb);
// 		TVideoStreamCap t264_XGA_20_BP_448;
// 		t264_XGA_20_BP_448.SetUserDefFrameRate(20);
// 		t264_XGA_20_BP_448.SetMaxBitRate(448);
// 		t264_XGA_20_BP_448.SetMediaType(MEDIA_TYPE_H264);
// 		t264_XGA_20_BP_448.SetResolution(VIDEO_FORMAT_XGA);
// 		t264_XGA_20_BP_448.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_XGA_20_HP_320;
		t264_XGA_20_HP_320.SetUserDefFrameRate(20);
		t264_XGA_20_HP_320.SetMaxBitRate(320);
		t264_XGA_20_HP_320.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_20_HP_320.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_20_HP_320.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_XGA_5_HP_HB;
		t264_XGA_5_HP_HB.SetUserDefFrameRate(5);
		t264_XGA_5_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_5_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_5_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_5_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_XGA_5_HP_192;
		t264_XGA_5_HP_192.SetUserDefFrameRate(5);
		t264_XGA_5_HP_192.SetMaxBitRate(192);
		t264_XGA_5_HP_192.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_5_HP_192.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_5_HP_192.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_XGA_5_HP_64;
		t264_XGA_5_HP_64.SetUserDefFrameRate(5);
		t264_XGA_5_HP_64.SetMaxBitRate(64);
		t264_XGA_5_HP_64.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_5_HP_64.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_5_HP_64.SetH264ProfileAttrb(emHpAttrb);

		// h263+
		TVideoStreamCap t263P_XGA_5_BP_HB;
		t263P_XGA_5_BP_HB.SetFrameRate(5);
		t263P_XGA_5_BP_HB.SetMaxBitRate(wMaxBitRate);
		t263P_XGA_5_BP_HB.SetMediaType(MEDIA_TYPE_H263PLUS);
		t263P_XGA_5_BP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t263P_XGA_5_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t263P_XGA_5_BP_64;
		t263P_XGA_5_BP_64.SetFrameRate(5);
		t263P_XGA_5_BP_64.SetMaxBitRate(64);
		t263P_XGA_5_BP_64.SetMediaType(MEDIA_TYPE_H263PLUS);
		t263P_XGA_5_BP_64.SetResolution(VIDEO_FORMAT_XGA);
		t263P_XGA_5_BP_64.SetH264ProfileAttrb(emBpAttrb);
		// XDBAS
		// XBAS 1080*60
		m_tHD1080_60.m_tXDSBasCap.m_atMaxCap[0] = t264_1080_60_HP_HB;
		m_tHD1080_60.m_tXDSBasCap.m_atMaxCap[1] = t264_SXGA_20_HP_HB;
		m_tHD1080_60.m_tXDSBasCap.m_atMaxCap[2] = t264_XGA_20_HP_HB;
		m_tHD1080_60.m_tXDSBasCap.m_atMaxCap[3] = t263P_XGA_5_BP_HB;
		
		m_tHD1080_60.m_tXDSBasCap.m_atMinCap[0] = t264_1080_25_HP_1472;
		m_tHD1080_60.m_tXDSBasCap.m_atMinCap[1] = t264_SXGA_5_HP_192;
		m_tHD1080_60.m_tXDSBasCap.m_atMinCap[2] = t264_XGA_5_HP_64;
		m_tHD1080_60.m_tXDSBasCap.m_atMinCap[3] = t263P_XGA_5_BP_64; 
		//XBAS 1080*30
		m_tHD1080_30.m_tXDSBasCap.m_atMaxCap[0] = t264_1080_30_HP_HB;
		m_tHD1080_30.m_tXDSBasCap.m_atMaxCap[1] = t264_SXGA_20_HP_HB;
		m_tHD1080_30.m_tXDSBasCap.m_atMaxCap[2] = t264_XGA_20_HP_HB;
		m_tHD1080_30.m_tXDSBasCap.m_atMaxCap[3] = t263P_XGA_5_BP_HB;
		
		m_tHD1080_30.m_tXDSBasCap.m_atMinCap[0] = t264_1080_25_HP_1472;
		m_tHD1080_30.m_tXDSBasCap.m_atMinCap[1] = t264_SXGA_5_HP_192;
		m_tHD1080_30.m_tXDSBasCap.m_atMinCap[2] = t264_XGA_5_HP_64;
		m_tHD1080_30.m_tXDSBasCap.m_atMinCap[3] = t263P_XGA_5_BP_64;

		//XBAS 720*60
		m_tHD720_60.m_tXDSBasCap.m_atMaxCap[0] = t264_720_60_HP_HB;
		m_tHD720_60.m_tXDSBasCap.m_atMaxCap[1] = t264_SXGA_20_HP_HB;
		m_tHD720_60.m_tXDSBasCap.m_atMaxCap[2] = t264_XGA_20_HP_HB;
		m_tHD720_60.m_tXDSBasCap.m_atMaxCap[3] = t263P_XGA_5_BP_HB;
		
		m_tHD720_60.m_tXDSBasCap.m_atMinCap[0] = t264_720_30_HP_448;
		m_tHD720_60.m_tXDSBasCap.m_atMinCap[1] = t264_SXGA_5_HP_192;
		m_tHD720_60.m_tXDSBasCap.m_atMinCap[2] = t264_XGA_5_HP_64;
		m_tHD720_60.m_tXDSBasCap.m_atMinCap[3] = t263P_XGA_5_BP_64;

		//XBAS 720*30
		m_tHD720_30.m_tXDSBasCap.m_atMaxCap[0] = t264_720_30_HP_HB;
		m_tHD720_30.m_tXDSBasCap.m_atMaxCap[1] = t264_SXGA_20_HP_HB;
		m_tHD720_30.m_tXDSBasCap.m_atMaxCap[2] = t264_XGA_5_HP_HB;
		m_tHD720_30.m_tXDSBasCap.m_atMaxCap[3] = t263P_XGA_5_BP_HB;
		
		m_tHD720_30.m_tXDSBasCap.m_atMinCap[0] = t264_720_30_HP_448;
		m_tHD720_30.m_tXDSBasCap.m_atMinCap[1] = t264_SXGA_5_HP_192;
		m_tHD720_30.m_tXDSBasCap.m_atMinCap[2] = t264_XGA_5_HP_64;
		m_tHD720_30.m_tXDSBasCap.m_atMinCap[3] = t263P_XGA_5_BP_64;
		
		//XBAS SD
		m_tSD.m_tXDSBasCap.m_atMaxCap[0] = t264_XGA_20_HP_HB;
		m_tSD.m_tXDSBasCap.m_atMaxCap[1] = t264_XGA_10_HP_HB;
		m_tSD.m_tXDSBasCap.m_atMaxCap[2] = t264_XGA_5_HP_HB;
		m_tSD.m_tXDSBasCap.m_atMaxCap[3] = t263P_XGA_5_BP_HB;
		
		m_tSD.m_tXDSBasCap.m_atMinCap[0] = t264_XGA_20_HP_320;
		m_tSD.m_tXDSBasCap.m_atMinCap[1] = t264_XGA_5_HP_192;
		m_tSD.m_tXDSBasCap.m_atMinCap[2] = t264_XGA_5_HP_64;
		m_tSD.m_tXDSBasCap.m_atMinCap[3] = t263P_XGA_5_BP_64;

		// DSBAS 1080*60
// 		m_tHD1080_60.m_tDSBasCap.m_atMaxCap[0] = t264_SXGA_20_HP_HB;
// 		m_tHD1080_60.m_tDSBasCap.m_atMaxCap[1] = t264_XGA_5_HP_HB;
// 		m_tHD1080_60.m_tDSBasCap.m_atMaxCap[2] = t263P_XGA_5_BP_HB;
// 		
// 		m_tHD1080_60.m_tDSBasCap.m_atMinCap[0] = t264_SXGA_20_BP_448;
// 		m_tHD1080_60.m_tDSBasCap.m_atMinCap[1] = t264_VGA_5_BP_64;
// 		m_tHD1080_60.m_tDSBasCap.m_atMinCap[2] = t263P_XGA_5_BP_64;
		// DSBAS 1080*30
		m_tHD1080_30.m_tDSBasCap.m_atMaxCap[0] = t264_SXGA_20_HP_HB;
		m_tHD1080_30.m_tDSBasCap.m_atMaxCap[1] = t264_XGA_5_HP_HB;
		m_tHD1080_30.m_tDSBasCap.m_atMaxCap[2] = t263P_XGA_5_BP_HB;

		m_tHD1080_30.m_tDSBasCap.m_atMinCap[0] = t264_SXGA_5_HP_192;
		m_tHD1080_30.m_tDSBasCap.m_atMinCap[1] = t264_XGA_5_HP_64;
		m_tHD1080_30.m_tDSBasCap.m_atMinCap[2] = t263P_XGA_5_BP_64;

		// DSBAS 720*30
		m_tHD720_30.m_tDSBasCap.m_atMaxCap[0] = t264_SXGA_20_HP_HB;
		m_tHD720_30.m_tDSBasCap.m_atMaxCap[1] = t264_XGA_5_HP_HB;
		m_tHD720_30.m_tDSBasCap.m_atMaxCap[2] = t263P_XGA_5_BP_HB;
		
		m_tHD720_30.m_tDSBasCap.m_atMinCap[0] = t264_SXGA_5_HP_192;
		m_tHD720_30.m_tDSBasCap.m_atMinCap[1] = t264_XGA_5_HP_64;
		m_tHD720_30.m_tDSBasCap.m_atMinCap[2] = t263P_XGA_5_BP_64;
		// DSBAS SD
		m_tSD.m_tDSBasCap.m_atMaxCap[0] = t264_XGA_5_HP_HB;
		m_tSD.m_tDSBasCap.m_atMaxCap[1] = t264_XGA_5_HP_HB;
		m_tSD.m_tDSBasCap.m_atMaxCap[2] = t263P_XGA_5_BP_HB;
		
		m_tSD.m_tDSBasCap.m_atMinCap[0] = t264_XGA_5_HP_192;
		m_tSD.m_tDSBasCap.m_atMinCap[1] = t264_XGA_5_HP_64;
		m_tSD.m_tDSBasCap.m_atMinCap[2] = t263P_XGA_5_BP_64;

	}

	// zyq[2016/5/3] �Խ�����ý���������
	void SetNoHiredConfEqpEnc()
	{

		u16 wMaxBitRate= 8192;

		// 1080
		TVideoStreamCap t264_1080_60_HP_HB;
		t264_1080_60_HP_HB.SetUserDefFrameRate(60);
		t264_1080_60_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_1080_60_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_60_HP_HB.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_60_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_1080_30_HP_HB;
		t264_1080_30_HP_HB.SetUserDefFrameRate(30);
		t264_1080_30_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_1080_30_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_30_HP_HB.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_30_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_1080_25_HP_1472;
		t264_1080_25_HP_1472.SetUserDefFrameRate(25);
		t264_1080_25_HP_1472.SetMaxBitRate(1472);	// ??xmpuΪ��Ҫ2486
		t264_1080_25_HP_1472.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_25_HP_1472.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_25_HP_1472.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_1080_30_BP_HB;
		t264_1080_30_BP_HB.SetUserDefFrameRate(30);
		t264_1080_30_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_1080_30_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_30_BP_HB.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_30_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		// 720
		TVideoStreamCap t264_720_60_HP_HB;
		t264_720_60_HP_HB.SetUserDefFrameRate(60);
		t264_720_60_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_720_60_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_720_60_HP_HB.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_60_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_720_30_HP_HB;
		t264_720_30_HP_HB.SetUserDefFrameRate(30);
		t264_720_30_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_720_30_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_720_30_HP_HB.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_30_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_720_50_HP_1472;
		t264_720_50_HP_1472.SetUserDefFrameRate(50);
		t264_720_50_HP_1472.SetMaxBitRate(1472);
		t264_720_50_HP_1472.SetMediaType(MEDIA_TYPE_H264);
		t264_720_50_HP_1472.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_50_HP_1472.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_720_30_HP_448;
		t264_720_30_HP_448.SetUserDefFrameRate(30);
		t264_720_30_HP_448.SetMaxBitRate(448);
		t264_720_30_HP_448.SetMediaType(MEDIA_TYPE_H264);
		t264_720_30_HP_448.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_30_HP_448.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_720_25_HP_1472;
		t264_720_25_HP_1472.SetUserDefFrameRate(25);
		t264_720_25_HP_1472.SetMaxBitRate(1472);
		t264_720_25_HP_1472.SetMediaType(MEDIA_TYPE_H264);
		t264_720_25_HP_1472.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_25_HP_1472.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_720_25_HP_448;
		t264_720_25_HP_448.SetUserDefFrameRate(25);
		t264_720_25_HP_448.SetMaxBitRate(448);
		t264_720_25_HP_448.SetMediaType(MEDIA_TYPE_H264);
		t264_720_25_HP_448.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_25_HP_448.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_720_30_BP_HB;
		t264_720_30_BP_HB.SetUserDefFrameRate(30);
		t264_720_30_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_720_30_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_720_30_BP_HB.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_30_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_720_25_BP_1472;
		t264_720_25_BP_1472.SetUserDefFrameRate(25);
		t264_720_25_BP_1472.SetMaxBitRate(1472);
		t264_720_25_BP_1472.SetMediaType(MEDIA_TYPE_H264);
		t264_720_25_BP_1472.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_25_BP_1472.SetH264ProfileAttrb(emBpAttrb);

		// w4cif
		TVideoStreamCap t264_W4CIF_25_HP_HB;
		t264_W4CIF_25_HP_HB.SetUserDefFrameRate(25);
		t264_W4CIF_25_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_W4CIF_25_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_W4CIF_25_HP_HB.SetResolution(VIDEO_FORMAT_W4CIF);
		t264_W4CIF_25_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_W4CIF_25_HP_704;
		t264_W4CIF_25_HP_704.SetUserDefFrameRate(25);
		t264_W4CIF_25_HP_704.SetMaxBitRate(704);
		t264_W4CIF_25_HP_704.SetMediaType(MEDIA_TYPE_H264);
		t264_W4CIF_25_HP_704.SetResolution(VIDEO_FORMAT_W4CIF);
		t264_W4CIF_25_HP_704.SetH264ProfileAttrb(emHpAttrb);
		
		// 4cif
		TVideoStreamCap t264_4CIF_25_HP_192;
		t264_4CIF_25_HP_192.SetUserDefFrameRate(25);
		t264_4CIF_25_HP_192.SetMaxBitRate(192);
		t264_4CIF_25_HP_192.SetMediaType(MEDIA_TYPE_H264);
		t264_4CIF_25_HP_192.SetResolution(VIDEO_FORMAT_4CIF);
		t264_4CIF_25_HP_192.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_4CIF_25_BP_HB;
		t264_4CIF_25_BP_HB.SetUserDefFrameRate(25);
		t264_4CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_4CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_4CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_4CIF);
		t264_4CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_4CIF_25_BP_960;
		t264_4CIF_25_BP_960.SetUserDefFrameRate(25);
		t264_4CIF_25_BP_960.SetMaxBitRate(960);
		t264_4CIF_25_BP_960.SetMediaType(MEDIA_TYPE_H264);
		t264_4CIF_25_BP_960.SetResolution(VIDEO_FORMAT_4CIF);
		t264_4CIF_25_BP_960.SetH264ProfileAttrb(emBpAttrb);

		// wcif
		TVideoStreamCap t264_WCIF_25_HP_HB;
		t264_WCIF_25_HP_HB.SetUserDefFrameRate(25);
		t264_WCIF_25_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_WCIF_25_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_WCIF_25_HP_HB.SetResolution(VIDEO_FORMAT_WCIF);
		t264_WCIF_25_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		// cif HP
		TVideoStreamCap t264_CIF_25_HP_HB;
		t264_CIF_25_HP_HB.SetUserDefFrameRate(25);
		t264_CIF_25_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_CIF_25_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_HP_HB.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_CIF_25_HP_64;
		t264_CIF_25_HP_64.SetUserDefFrameRate(25);
		t264_CIF_25_HP_64.SetMaxBitRate(64);
		t264_CIF_25_HP_64.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_HP_64.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_HP_64.SetH264ProfileAttrb(emHpAttrb);
// 		TVideoStreamCap t264_CIF_25_HP_LB2;
// 		t264_CIF_25_HP_LB2.SetUserDefFrameRate(25);
// 		t264_CIF_25_HP_LB2.SetMaxBitRate(192);
// 		t264_CIF_25_HP_LB2.SetMediaType(MEDIA_TYPE_H264);
// 		t264_CIF_25_HP_LB2.SetResolution(VIDEO_FORMAT_CIF);
// 		t264_CIF_25_HP_LB2.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_CIF_25_BP_HB;
		t264_CIF_25_BP_HB.SetUserDefFrameRate(25);
		t264_CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_CIF_25_BP_448;
		t264_CIF_25_BP_448.SetUserDefFrameRate(25);
		t264_CIF_25_BP_448.SetMaxBitRate(448);
		t264_CIF_25_BP_448.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_BP_448.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_BP_448.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_CIF_25_BP_64;
		t264_CIF_25_BP_64.SetUserDefFrameRate(25);
		t264_CIF_25_BP_64.SetMaxBitRate(64);
		t264_CIF_25_BP_64.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_BP_64.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_BP_64.SetH264ProfileAttrb(emBpAttrb);

		// mp4
		TVideoStreamCap tMP4_4CIF_25_BP_HB;
		tMP4_4CIF_25_BP_HB.SetFrameRate(25);
		tMP4_4CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		tMP4_4CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_MP4);
		tMP4_4CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_4CIF);
		tMP4_4CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap tMP4_CIF_25_BP_HB;
		tMP4_CIF_25_BP_HB.SetFrameRate(25);
		tMP4_CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		tMP4_CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_MP4);
		tMP4_CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_CIF);
		tMP4_CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap tMP4_CIF_25_BP_64;
		tMP4_CIF_25_BP_64.SetFrameRate(25);
		tMP4_CIF_25_BP_64.SetMaxBitRate(64);
		tMP4_CIF_25_BP_64.SetMediaType(MEDIA_TYPE_MP4);
		tMP4_CIF_25_BP_64.SetResolution(VIDEO_FORMAT_CIF);
		tMP4_CIF_25_BP_64.SetH264ProfileAttrb(emBpAttrb);

		// h263
		TVideoStreamCap t263_CIF_25_BP_HB;
		t263_CIF_25_BP_HB.SetFrameRate(25);
		t263_CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		t263_CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_H263);
		t263_CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_CIF);
		t263_CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t263_CIF_25_BP_64;
		t263_CIF_25_BP_64.SetFrameRate(25);
		t263_CIF_25_BP_64.SetMaxBitRate(64);
		t263_CIF_25_BP_64.SetMediaType(MEDIA_TYPE_H263);
		t263_CIF_25_BP_64.SetResolution(VIDEO_FORMAT_CIF);
		t263_CIF_25_BP_64.SetH264ProfileAttrb(emBpAttrb);

		//XVMP 1080*60
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[0] = t264_1080_60_HP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[1] = t264_720_30_HP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[3] = t264_1080_30_BP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_BP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[5] = tMP4_4CIF_25_BP_HB;
		m_tHD1080_60.m_tXVmpCap.m_atMaxCap[6] = t263_CIF_25_BP_HB;
		
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[0] = t264_1080_25_HP_1472;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[1] = t264_720_25_HP_448;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[2] = t264_CIF_25_HP_64;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[3] = t264_720_25_BP_1472;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[4] = t264_CIF_25_BP_64;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[5] = tMP4_CIF_25_BP_64;
		m_tHD1080_60.m_tXVmpCap.m_atMinCap[6] = t263_CIF_25_BP_64;
		//XVMP 1080*30
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[0] = t264_1080_30_HP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[1] = t264_720_30_HP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[3] = t264_720_30_BP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_BP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[5] = tMP4_4CIF_25_BP_HB;
		m_tHD1080_30.m_tXVmpCap.m_atMaxCap[6] = t263_CIF_25_BP_HB;

		m_tHD1080_30.m_tXVmpCap.m_atMinCap[0] = t264_1080_25_HP_1472;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[1] = t264_720_25_HP_448;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[2] = t264_CIF_25_HP_64;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[3] = t264_720_25_BP_1472;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[4] = t264_CIF_25_BP_64;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[5] = tMP4_CIF_25_BP_64;
		m_tHD1080_30.m_tXVmpCap.m_atMinCap[6] = t263_CIF_25_BP_64;

		//XVMP 720*60
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[0] = t264_720_60_HP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[1] = t264_W4CIF_25_HP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[3] = t264_720_30_BP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_BP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[5] = tMP4_4CIF_25_BP_HB;
		m_tHD720_60.m_tXVmpCap.m_atMaxCap[6] = t263_CIF_25_BP_HB;
		
		m_tHD720_60.m_tXVmpCap.m_atMinCap[0] = t264_720_50_HP_1472;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[1] = t264_W4CIF_25_HP_704;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[2] = t264_CIF_25_HP_64;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[3] = t264_720_25_BP_1472;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[4] = t264_CIF_25_BP_64;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[5] = tMP4_CIF_25_BP_64;
		m_tHD720_60.m_tXVmpCap.m_atMinCap[6] = t263_CIF_25_BP_64;

		//XVMP 720*30
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[0] = t264_720_30_HP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[1] = t264_W4CIF_25_HP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[3] = t264_720_30_BP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_BP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[5] = tMP4_4CIF_25_BP_HB;
		m_tHD720_30.m_tXVmpCap.m_atMaxCap[6] = t263_CIF_25_BP_HB;
		
		m_tHD720_30.m_tXVmpCap.m_atMinCap[0] = t264_720_25_HP_1472;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[1] = t264_W4CIF_25_HP_704;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[2] = t264_CIF_25_HP_64;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[3] = t264_720_25_BP_1472;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[4] = t264_CIF_25_BP_64;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[5] = tMP4_CIF_25_BP_64;
		m_tHD720_30.m_tXVmpCap.m_atMinCap[6] = t263_CIF_25_BP_64;

		//XVMP SD
		m_tSD.m_tXVmpCap.m_atMaxCap[0] = t264_W4CIF_25_HP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[1] = t264_W4CIF_25_HP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[2] = t264_WCIF_25_HP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[3] = t264_4CIF_25_BP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[4] = t264_CIF_25_BP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[5] = tMP4_4CIF_25_BP_HB;
		m_tSD.m_tXVmpCap.m_atMaxCap[6] = t263_CIF_25_BP_HB;
		
		m_tSD.m_tXVmpCap.m_atMinCap[0] = t264_W4CIF_25_HP_704;
		m_tSD.m_tXVmpCap.m_atMinCap[1] = t264_4CIF_25_HP_192;
		m_tSD.m_tXVmpCap.m_atMinCap[2] = t264_CIF_25_HP_64;
		m_tSD.m_tXVmpCap.m_atMinCap[3] = t264_CIF_25_BP_448;
		m_tSD.m_tXVmpCap.m_atMinCap[4] = t264_CIF_25_BP_64;
		m_tSD.m_tXVmpCap.m_atMinCap[5] = tMP4_CIF_25_BP_64;
		m_tSD.m_tXVmpCap.m_atMinCap[6] = t263_CIF_25_BP_64;

		// VMP ��XVMP����ֻ�����ֵ��֡�ʣ�������ֵ
		memcpy (m_tHD1080_60.m_tVmpCap.m_atMaxCap, m_tHD1080_60.m_tXVmpCap.m_atMaxCap, sizeof(m_tHD1080_60.m_tXVmpCap.m_atMaxCap) );
		m_tHD1080_60.m_tVmpCap.m_atMaxCap[0].SetUserDefFrameRate(30);
		memcpy (m_tHD1080_60.m_tVmpCap.m_atMinCap, m_tHD1080_60.m_tXVmpCap.m_atMinCap, sizeof(m_tHD1080_60.m_tXVmpCap.m_atMinCap) );
		m_tHD1080_60.m_tVmpCap.m_atMinCap[0] = t264_1080_25_HP_1472;

		memcpy (m_tHD1080_30.m_tVmpCap.m_atMaxCap, m_tHD1080_30.m_tXVmpCap.m_atMaxCap, sizeof(m_tHD1080_30.m_tXVmpCap.m_atMaxCap) );
		memcpy (m_tHD1080_30.m_tVmpCap.m_atMinCap, m_tHD1080_30.m_tXVmpCap.m_atMinCap, sizeof(m_tHD1080_30.m_tXVmpCap.m_atMinCap) );

		memcpy (m_tHD720_30.m_tVmpCap.m_atMaxCap, m_tHD720_30.m_tXVmpCap.m_atMaxCap, sizeof(m_tHD720_30.m_tXVmpCap.m_atMaxCap) );
		memcpy (m_tHD720_30.m_tVmpCap.m_atMinCap, m_tHD720_30.m_tXVmpCap.m_atMinCap, sizeof(m_tHD720_30.m_tXVmpCap.m_atMinCap) );

		memcpy (m_tSD.m_tVmpCap.m_atMaxCap, m_tSD.m_tXVmpCap.m_atMaxCap, sizeof(m_tSD.m_tXVmpCap.m_atMaxCap) );
		memcpy (m_tSD.m_tVmpCap.m_atMinCap, m_tSD.m_tXVmpCap.m_atMinCap, sizeof(m_tSD.m_tXVmpCap.m_atMinCap) );

		// XBAS 1080*60
		m_tHD1080_60.m_tXVBasCap.m_atMaxCap[0] = t264_1080_30_HP_HB;
		m_tHD1080_60.m_tXVBasCap.m_atMaxCap[1] = t264_W4CIF_25_HP_HB;
		m_tHD1080_60.m_tXVBasCap.m_atMaxCap[2] = t264_1080_30_BP_HB;
		m_tHD1080_60.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_BP_HB;
		m_tHD1080_60.m_tXVBasCap.m_atMaxCap[4] = tMP4_CIF_25_BP_HB;
		
		m_tHD1080_60.m_tXVBasCap.m_atMinCap[0] = t264_720_25_HP_448;
		m_tHD1080_60.m_tXVBasCap.m_atMinCap[1] = t264_CIF_25_HP_64;
		m_tHD1080_60.m_tXVBasCap.m_atMinCap[2] = t264_720_25_BP_1472;
		m_tHD1080_60.m_tXVBasCap.m_atMinCap[3] = t264_CIF_25_BP_64;
		m_tHD1080_60.m_tXVBasCap.m_atMinCap[4] = tMP4_CIF_25_BP_64;
		//XBAS 1080*30
		m_tHD1080_30.m_tXVBasCap.m_atMaxCap[0] = t264_1080_30_HP_HB;
		m_tHD1080_30.m_tXVBasCap.m_atMaxCap[1] = t264_W4CIF_25_HP_HB;
		m_tHD1080_30.m_tXVBasCap.m_atMaxCap[2] = t264_720_30_BP_HB;
		m_tHD1080_30.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_BP_HB;
		m_tHD1080_30.m_tXVBasCap.m_atMaxCap[4] = tMP4_CIF_25_BP_HB;

		m_tHD1080_30.m_tXVBasCap.m_atMinCap[0] = t264_720_25_HP_448;
		m_tHD1080_30.m_tXVBasCap.m_atMinCap[1] = t264_CIF_25_HP_64;
		m_tHD1080_30.m_tXVBasCap.m_atMinCap[2] = t264_720_25_BP_1472;
		m_tHD1080_30.m_tXVBasCap.m_atMinCap[3] = t264_CIF_25_BP_64;
		m_tHD1080_30.m_tXVBasCap.m_atMinCap[4] = tMP4_CIF_25_BP_64;

		//XBAS 720*60
		m_tHD720_60.m_tXVBasCap.m_atMaxCap[0] = t264_720_60_HP_HB;
		m_tHD720_60.m_tXVBasCap.m_atMaxCap[1] = t264_W4CIF_25_HP_HB;
		m_tHD720_60.m_tXVBasCap.m_atMaxCap[2] = t264_720_30_BP_HB;
		m_tHD720_60.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_BP_HB;
		m_tHD720_60.m_tXVBasCap.m_atMaxCap[4] = tMP4_CIF_25_BP_HB;
		
		m_tHD720_60.m_tXVBasCap.m_atMinCap[0] = t264_720_25_HP_1472;
		m_tHD720_60.m_tXVBasCap.m_atMinCap[1] = t264_CIF_25_HP_64;
		m_tHD720_60.m_tXVBasCap.m_atMinCap[2] = t264_720_25_BP_1472;
		m_tHD720_60.m_tXVBasCap.m_atMinCap[3] = t264_CIF_25_BP_64;
		m_tHD720_60.m_tXVBasCap.m_atMinCap[4] = tMP4_CIF_25_BP_64;

		//XBAS 720*30
		m_tHD720_30.m_tXVBasCap.m_atMaxCap[0] = t264_720_30_HP_HB;
		m_tHD720_30.m_tXVBasCap.m_atMaxCap[1] = t264_W4CIF_25_HP_HB;
		m_tHD720_30.m_tXVBasCap.m_atMaxCap[2] = t264_720_30_BP_HB;
		m_tHD720_30.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_BP_HB;
		m_tHD720_30.m_tXVBasCap.m_atMaxCap[4] = tMP4_CIF_25_BP_HB;
		
		m_tHD720_30.m_tXVBasCap.m_atMinCap[0] = t264_720_25_HP_1472;
		m_tHD720_30.m_tXVBasCap.m_atMinCap[1] = t264_CIF_25_HP_64;
		m_tHD720_30.m_tXVBasCap.m_atMinCap[2] = t264_720_25_BP_1472;
		m_tHD720_30.m_tXVBasCap.m_atMinCap[3] = t264_CIF_25_BP_64;
		m_tHD720_30.m_tXVBasCap.m_atMinCap[4] = tMP4_CIF_25_BP_64;

		//XBAS SD
		m_tSD.m_tXVBasCap.m_atMaxCap[0] = t264_W4CIF_25_HP_HB;
		m_tSD.m_tXVBasCap.m_atMaxCap[1] = t264_CIF_25_HP_HB;
		m_tSD.m_tXVBasCap.m_atMaxCap[2] = t264_4CIF_25_BP_HB;
		m_tSD.m_tXVBasCap.m_atMaxCap[3] = t264_CIF_25_BP_HB;
		m_tSD.m_tXVBasCap.m_atMaxCap[4] = tMP4_CIF_25_BP_HB;
		
		m_tSD.m_tXVBasCap.m_atMinCap[0] = t264_4CIF_25_HP_192;
		m_tSD.m_tXVBasCap.m_atMinCap[1] = t264_CIF_25_HP_64;
		m_tSD.m_tXVBasCap.m_atMinCap[2] = t264_4CIF_25_BP_960;
		m_tSD.m_tXVBasCap.m_atMinCap[3] = t264_CIF_25_BP_64;
		m_tSD.m_tXVBasCap.m_atMinCap[4] = tMP4_CIF_25_BP_64;

		// BAS
		memcpy (m_tHD1080_60.m_tVBasCap.m_atMaxCap, m_tHD1080_60.m_tXVBasCap.m_atMaxCap, sizeof(m_tHD1080_60.m_tXVBasCap.m_atMaxCap) );
		m_tHD1080_60.m_tVBasCap.m_atMaxCap[0].SetUserDefFrameRate(30);
		memcpy (m_tHD1080_60.m_tVBasCap.m_atMinCap, m_tHD1080_60.m_tXVBasCap.m_atMinCap, sizeof(m_tHD1080_60.m_tXVBasCap.m_atMinCap) );
// 		m_tHD1080_60.m_tVBasCap.m_atMinCap[0] = t264_1080_25_HP_1472;

		memcpy (m_tHD1080_30.m_tVBasCap.m_atMaxCap, m_tHD1080_30.m_tXVBasCap.m_atMaxCap, sizeof(m_tHD1080_30.m_tXVBasCap.m_atMaxCap) );
		memcpy (m_tHD1080_30.m_tVBasCap.m_atMinCap, m_tHD1080_30.m_tXVBasCap.m_atMinCap, sizeof(m_tHD1080_30.m_tXVBasCap.m_atMinCap) );
		
		memcpy (m_tHD720_30.m_tVBasCap.m_atMaxCap, m_tHD720_30.m_tXVBasCap.m_atMaxCap, sizeof(m_tHD720_30.m_tXVBasCap.m_atMaxCap) );
		memcpy (m_tHD720_30.m_tVBasCap.m_atMinCap, m_tHD720_30.m_tXVBasCap.m_atMinCap, sizeof(m_tHD720_30.m_tXVBasCap.m_atMinCap) );
		
		memcpy (m_tSD.m_tVBasCap.m_atMaxCap, m_tSD.m_tXVBasCap.m_atMaxCap, sizeof(m_tSD.m_tXVBasCap.m_atMaxCap) );
		memcpy (m_tSD.m_tVBasCap.m_atMinCap, m_tSD.m_tXVBasCap.m_atMinCap, sizeof(m_tSD.m_tXVBasCap.m_atMinCap) );

		// ˫����������
		// SXGA
		TVideoStreamCap t264_SXGA_20_HP_HB;
		t264_SXGA_20_HP_HB.SetUserDefFrameRate(20);
		t264_SXGA_20_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_SXGA_20_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_SXGA_20_HP_HB.SetResolution(VIDEO_FORMAT_SXGA);
		t264_SXGA_20_HP_HB.SetH264ProfileAttrb(emHpAttrb);
// 		TVideoStreamCap t264_SXGA_25_HP_448;
// 		t264_SXGA_25_HP_448.SetUserDefFrameRate(25);
// 		t264_SXGA_25_HP_448.SetMaxBitRate(448);
// 		t264_SXGA_25_HP_448.SetMediaType(MEDIA_TYPE_H264);
// 		t264_SXGA_25_HP_448.SetResolution(VIDEO_FORMAT_SXGA);
// 		t264_SXGA_25_HP_448.SetH264ProfileAttrb(emHpAttrb);
// 		TVideoStreamCap t264_SXGA_20_BP_448;
// 		t264_SXGA_20_BP_448.SetUserDefFrameRate(20);
// 		t264_SXGA_20_BP_448.SetMaxBitRate(448);
// 		t264_SXGA_20_BP_448.SetMediaType(MEDIA_TYPE_H264);
// 		t264_SXGA_20_BP_448.SetResolution(VIDEO_FORMAT_SXGA);
// 		t264_SXGA_20_BP_448.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_SXGA_5_HP_192;
		t264_SXGA_5_HP_192.SetUserDefFrameRate(5);
		t264_SXGA_5_HP_192.SetMaxBitRate(192);
		t264_SXGA_5_HP_192.SetMediaType(MEDIA_TYPE_H264);
		t264_SXGA_5_HP_192.SetResolution(VIDEO_FORMAT_SXGA);
		t264_SXGA_5_HP_192.SetH264ProfileAttrb(emHpAttrb);
		// UXGA
// 		TVideoStreamCap t264_UXGA_20_HP_448;
// 		t264_UXGA_20_HP_448.SetUserDefFrameRate(20);
// 		t264_UXGA_20_HP_448.SetMaxBitRate(448);
// 		t264_UXGA_20_HP_448.SetMediaType(MEDIA_TYPE_H264);
// 		t264_UXGA_20_HP_448.SetResolution(VIDEO_FORMAT_UXGA);
// 		t264_UXGA_20_HP_448.SetH264ProfileAttrb(emHpAttrb);
		// XGA
// 		TVideoStreamCap t264_XGA_30_HP_HB;
// 		t264_XGA_30_HP_HB.SetUserDefFrameRate(30);
// 		t264_XGA_30_HP_HB.SetMaxBitRate(wMaxBitRate);
// 		t264_XGA_30_HP_HB.SetMediaType(MEDIA_TYPE_H264);
// 		t264_XGA_30_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
// 		t264_XGA_30_HP_HB.SetH264ProfileAttrb(emHpAttrb);
// 		TVideoStreamCap t264_XGA_30_BP_HB;
// 		t264_XGA_30_BP_HB.SetUserDefFrameRate(30);
// 		t264_XGA_30_BP_HB.SetMaxBitRate(wMaxBitRate);
// 		t264_XGA_30_BP_HB.SetMediaType(MEDIA_TYPE_H264);
// 		t264_XGA_30_BP_HB.SetResolution(VIDEO_FORMAT_XGA);
// 		t264_XGA_30_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_XGA_20_HP_HB;
		t264_XGA_20_HP_HB.SetUserDefFrameRate(20);
		t264_XGA_20_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_20_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_20_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_20_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_XGA_20_HP_320;
		t264_XGA_20_HP_320.SetUserDefFrameRate(20);
		t264_XGA_20_HP_320.SetMaxBitRate(320);
		t264_XGA_20_HP_320.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_20_HP_320.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_20_HP_320.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_XGA_20_BP_HB;
		t264_XGA_20_BP_HB.SetUserDefFrameRate(20);
		t264_XGA_20_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_20_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_20_BP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_20_BP_HB.SetH264ProfileAttrb(emBpAttrb);
// 		TVideoStreamCap t264_XGA_20_BP_448;
// 		t264_XGA_20_BP_448.SetUserDefFrameRate(20);
// 		t264_XGA_20_BP_448.SetMaxBitRate(448);
// 		t264_XGA_20_BP_448.SetMediaType(MEDIA_TYPE_H264);
// 		t264_XGA_20_BP_448.SetResolution(VIDEO_FORMAT_XGA);
// 		t264_XGA_20_BP_448.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_XGA_10_HP_HB;
		t264_XGA_10_HP_HB.SetUserDefFrameRate(10);
		t264_XGA_10_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_10_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_10_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_10_HP_HB.SetH264ProfileAttrb(emHpAttrb);
// 		TVideoStreamCap t264_XGA_10_HP_320;
// 		t264_XGA_10_HP_320.SetUserDefFrameRate(10);
// 		t264_XGA_10_HP_320.SetMaxBitRate(320);
// 		t264_XGA_10_HP_320.SetMediaType(MEDIA_TYPE_H264);
// 		t264_XGA_10_HP_320.SetResolution(VIDEO_FORMAT_XGA);
// 		t264_XGA_10_HP_320.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_XGA_5_HP_HB;
		t264_XGA_5_HP_HB.SetUserDefFrameRate(5);
		t264_XGA_5_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_5_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_5_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_5_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_XGA_5_BP_HB;
		t264_XGA_5_BP_HB.SetUserDefFrameRate(5);
		t264_XGA_5_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_5_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_5_BP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_5_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t264_XGA_5_HP_64;
		t264_XGA_5_HP_64.SetUserDefFrameRate(5);
		t264_XGA_5_HP_64.SetMaxBitRate(64);
		t264_XGA_5_HP_64.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_5_HP_64.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_5_HP_64.SetH264ProfileAttrb(emHpAttrb);
		TVideoStreamCap t264_XGA_5_BP_64;
		t264_XGA_5_BP_64.SetUserDefFrameRate(5);
		t264_XGA_5_BP_64.SetMaxBitRate(64);
		t264_XGA_5_BP_64.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_5_BP_64.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_5_BP_64.SetH264ProfileAttrb(emBpAttrb);
		
		// h263+
		TVideoStreamCap t263P_XGA_5_BP_HB;
		t263P_XGA_5_BP_HB.SetFrameRate(5);
		t263P_XGA_5_BP_HB.SetMaxBitRate(wMaxBitRate);
		t263P_XGA_5_BP_HB.SetMediaType(MEDIA_TYPE_H263PLUS);
		t263P_XGA_5_BP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t263P_XGA_5_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap t263P_XGA_5_BP_64;
		t263P_XGA_5_BP_64.SetFrameRate(5);
		t263P_XGA_5_BP_64.SetMaxBitRate(64);
		t263P_XGA_5_BP_64.SetMediaType(MEDIA_TYPE_H263PLUS);
		t263P_XGA_5_BP_64.SetResolution(VIDEO_FORMAT_XGA);
		t263P_XGA_5_BP_64.SetH264ProfileAttrb(emBpAttrb);
		// XDBAS
		// XBAS 1080*60
		m_tHD1080_60.m_tXDSBasCap.m_atMaxCap[0] = t264_1080_60_HP_HB;
		m_tHD1080_60.m_tXDSBasCap.m_atMaxCap[1] = t264_SXGA_20_HP_HB;
		m_tHD1080_60.m_tXDSBasCap.m_atMaxCap[2] = t264_XGA_20_BP_HB;
		m_tHD1080_60.m_tXDSBasCap.m_atMaxCap[3] = t263P_XGA_5_BP_HB;
		
		m_tHD1080_60.m_tXDSBasCap.m_atMinCap[0] = t264_1080_25_HP_1472;
		m_tHD1080_60.m_tXDSBasCap.m_atMinCap[1] = t264_SXGA_5_HP_192;
		m_tHD1080_60.m_tXDSBasCap.m_atMinCap[2] = t264_XGA_5_BP_64;
		m_tHD1080_60.m_tXDSBasCap.m_atMinCap[3] = t263P_XGA_5_BP_64;
		//XBAS 1080*30
		m_tHD1080_30.m_tXDSBasCap.m_atMaxCap[0] = t264_1080_30_HP_HB;
		m_tHD1080_30.m_tXDSBasCap.m_atMaxCap[1] = t264_SXGA_20_HP_HB;
		m_tHD1080_30.m_tXDSBasCap.m_atMaxCap[2] = t264_XGA_20_BP_HB;
		m_tHD1080_30.m_tXDSBasCap.m_atMaxCap[3] = t263P_XGA_5_BP_HB;
		
		m_tHD1080_30.m_tXDSBasCap.m_atMinCap[0] = t264_1080_25_HP_1472;
		m_tHD1080_30.m_tXDSBasCap.m_atMinCap[1] = t264_SXGA_5_HP_192;
		m_tHD1080_30.m_tXDSBasCap.m_atMinCap[2] = t264_XGA_5_BP_64;
		m_tHD1080_30.m_tXDSBasCap.m_atMinCap[3] = t263P_XGA_5_BP_64;

		//XBAS 720*60
		m_tHD720_60.m_tXDSBasCap.m_atMaxCap[0] = t264_720_60_HP_HB;
		m_tHD720_60.m_tXDSBasCap.m_atMaxCap[1] = t264_SXGA_20_HP_HB;
		m_tHD720_60.m_tXDSBasCap.m_atMaxCap[2] = t264_XGA_20_BP_HB;
		m_tHD720_60.m_tXDSBasCap.m_atMaxCap[3] = t263P_XGA_5_BP_HB;
		
		m_tHD720_60.m_tXDSBasCap.m_atMinCap[0] = t264_720_30_HP_448;
		m_tHD720_60.m_tXDSBasCap.m_atMinCap[1] = t264_SXGA_5_HP_192;
		m_tHD720_60.m_tXDSBasCap.m_atMinCap[2] = t264_XGA_5_BP_64;
		m_tHD720_60.m_tXDSBasCap.m_atMinCap[3] = t263P_XGA_5_BP_64;

		//XBAS 720*30
		m_tHD720_30.m_tXDSBasCap.m_atMaxCap[0] = t264_720_30_HP_HB;
		m_tHD720_30.m_tXDSBasCap.m_atMaxCap[1] = t264_SXGA_20_HP_HB;
		m_tHD720_30.m_tXDSBasCap.m_atMaxCap[2] = t264_XGA_5_BP_HB;
		m_tHD720_30.m_tXDSBasCap.m_atMaxCap[3] = t263P_XGA_5_BP_HB;
		
		m_tHD720_30.m_tXDSBasCap.m_atMinCap[0] = t264_720_30_HP_448;
		m_tHD720_30.m_tXDSBasCap.m_atMinCap[1] = t264_SXGA_5_HP_192;
		m_tHD720_30.m_tXDSBasCap.m_atMinCap[2] = t264_XGA_5_BP_64;
		m_tHD720_30.m_tXDSBasCap.m_atMinCap[3] = t263P_XGA_5_BP_64;
		
		//XBAS SD
		m_tSD.m_tXDSBasCap.m_atMaxCap[0] = t264_XGA_20_HP_HB;
		m_tSD.m_tXDSBasCap.m_atMaxCap[1] = t264_XGA_10_HP_HB;
		m_tSD.m_tXDSBasCap.m_atMaxCap[2] = t264_XGA_5_BP_HB;
		m_tSD.m_tXDSBasCap.m_atMaxCap[3] = t263P_XGA_5_BP_HB;
		
		m_tSD.m_tXDSBasCap.m_atMinCap[0] = t264_XGA_20_HP_320;
		m_tSD.m_tXDSBasCap.m_atMinCap[1] = t264_XGA_5_HP_64;
		m_tSD.m_tXDSBasCap.m_atMinCap[2] = t264_XGA_5_BP_64;
		m_tSD.m_tXDSBasCap.m_atMinCap[3] = t263P_XGA_5_BP_64;

		// DSBAS 1080*60 ���mpsֻ�ܿ�һ������ȥ��
// 		m_tHD1080_60.m_tDSBasCap.m_atMaxCap[0] = t264_SXGA_20_HP_HB;
// 		m_tHD1080_60.m_tDSBasCap.m_atMaxCap[1] = t264_XGA_5_HP_HB;
// 		m_tHD1080_60.m_tDSBasCap.m_atMaxCap[2] = t263P_XGA_5_BP_HB;
// 		
// 		m_tHD1080_60.m_tDSBasCap.m_atMinCap[0] = t264_SXGA_20_BP_448;
// 		m_tHD1080_60.m_tDSBasCap.m_atMinCap[1] = t264_VGA_5_BP_64;
// 		m_tHD1080_60.m_tDSBasCap.m_atMinCap[2] = t263P_XGA_5_BP_64;
		// DSBAS 1080*30
		m_tHD1080_30.m_tDSBasCap.m_atMaxCap[0] = t264_SXGA_20_HP_HB;
		m_tHD1080_30.m_tDSBasCap.m_atMaxCap[1] = t264_XGA_5_BP_HB;
		m_tHD1080_30.m_tDSBasCap.m_atMaxCap[2] = t263P_XGA_5_BP_HB;

		m_tHD1080_30.m_tDSBasCap.m_atMinCap[0] = t264_SXGA_5_HP_192;
		m_tHD1080_30.m_tDSBasCap.m_atMinCap[1] = t264_XGA_5_BP_64;
		m_tHD1080_30.m_tDSBasCap.m_atMinCap[2] = t263P_XGA_5_BP_64;

		// DSBAS 720*30
		m_tHD720_30.m_tDSBasCap.m_atMaxCap[0] = t264_SXGA_20_HP_HB;
		m_tHD720_30.m_tDSBasCap.m_atMaxCap[1] = t264_XGA_5_BP_HB;
		m_tHD720_30.m_tDSBasCap.m_atMaxCap[2] = t263P_XGA_5_BP_HB;
		
		m_tHD720_30.m_tDSBasCap.m_atMinCap[0] = t264_SXGA_5_HP_192;
		m_tHD720_30.m_tDSBasCap.m_atMinCap[1] = t264_XGA_5_BP_64;
		m_tHD720_30.m_tDSBasCap.m_atMinCap[2] = t263P_XGA_5_BP_64;
		// DSBAS SD
		m_tSD.m_tDSBasCap.m_atMaxCap[0] = t264_XGA_5_HP_HB;
		m_tSD.m_tDSBasCap.m_atMaxCap[1] = t264_XGA_5_BP_HB;
		m_tSD.m_tDSBasCap.m_atMaxCap[2] = t263P_XGA_5_BP_HB;
		
		m_tSD.m_tDSBasCap.m_atMinCap[0] = t264_XGA_5_HP_64;
		m_tSD.m_tDSBasCap.m_atMinCap[1] = t264_XGA_5_BP_64;
		m_tSD.m_tDSBasCap.m_atMinCap[2] = t263P_XGA_5_BP_64;

	}

	void SetH265ConfEqpEnc()
	{
		u16 wMaxBitRate= 8192;
		
		// H2654k2k
		TVideoStreamCap t265_4k2k_30_HB;
		t265_4k2k_30_HB.SetUserDefFrameRate(30);
		t265_4k2k_30_HB.SetMaxBitRate(wMaxBitRate);
		t265_4k2k_30_HB.SetMediaType(MEDIA_TYPE_H265);
		t265_4k2k_30_HB.SetResolution(VIDEO_FORMAT_4K);
		
		TVideoStreamCap t265_4k2k_10_1984;
		t265_4k2k_10_1984.SetUserDefFrameRate(10);
		t265_4k2k_10_1984.SetMaxBitRate(1984);
		t265_4k2k_10_1984.SetMediaType(MEDIA_TYPE_H265);
		t265_4k2k_10_1984.SetResolution(VIDEO_FORMAT_4K);
		
		// H265 1080
		TVideoStreamCap t265_1080_60_HB;
		t265_1080_60_HB.SetUserDefFrameRate(60);
		t265_1080_60_HB.SetMaxBitRate(wMaxBitRate);
		t265_1080_60_HB.SetMediaType(MEDIA_TYPE_H265);
		t265_1080_60_HB.SetResolution(VIDEO_FORMAT_HD1080);

		TVideoStreamCap t265_1080_50_1984;
		t265_1080_50_1984.SetUserDefFrameRate(50);
		t265_1080_50_1984.SetMaxBitRate(1984);
		t265_1080_50_1984.SetMediaType(MEDIA_TYPE_H265);
		t265_1080_50_1984.SetResolution(VIDEO_FORMAT_HD1080);

		TVideoStreamCap t265_1080_30_HB;
		t265_1080_30_HB.SetUserDefFrameRate(30);
		t265_1080_30_HB.SetMaxBitRate(wMaxBitRate);
		t265_1080_30_HB.SetMediaType(MEDIA_TYPE_H265);
		t265_1080_30_HB.SetResolution(VIDEO_FORMAT_HD1080);

		TVideoStreamCap t265_1080_25_960;
		t265_1080_25_960.SetUserDefFrameRate(25);
		t265_1080_25_960.SetMaxBitRate(960);
		t265_1080_25_960.SetMediaType(MEDIA_TYPE_H265);
		t265_1080_25_960.SetResolution(VIDEO_FORMAT_HD1080);

		// H264 1080
		TVideoStreamCap t264_1080_60_HP_HB;
		t264_1080_60_HP_HB.SetUserDefFrameRate(60);
		t264_1080_60_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_1080_60_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_60_HP_HB.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_60_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		
		TVideoStreamCap t264_1080_30_HP_HB;
		t264_1080_30_HP_HB.SetUserDefFrameRate(30);
		t264_1080_30_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_1080_30_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_30_HP_HB.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_30_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		
		TVideoStreamCap t264_1080_25_HP_1472;
		t264_1080_25_HP_1472.SetUserDefFrameRate(25);
		t264_1080_25_HP_1472.SetMaxBitRate(1472);
		t264_1080_25_HP_1472.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_25_HP_1472.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_25_HP_1472.SetH264ProfileAttrb(emHpAttrb);
		
		TVideoStreamCap t264_1080_30_BP_HB;
		t264_1080_30_BP_HB.SetUserDefFrameRate(30);
		t264_1080_30_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_1080_30_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_30_BP_HB.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_30_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t264_1080_25_BP_2496;
		t264_1080_25_BP_2496.SetUserDefFrameRate(25);
		t264_1080_25_BP_2496.SetMaxBitRate(2496);
		t264_1080_25_BP_2496.SetMediaType(MEDIA_TYPE_H264);
		t264_1080_25_BP_2496.SetResolution(VIDEO_FORMAT_HD1080);
		t264_1080_25_BP_2496.SetH264ProfileAttrb(emBpAttrb);

		// H265 720
		TVideoStreamCap t265_720_60_HB;
		t265_720_60_HB.SetUserDefFrameRate(60);
		t265_720_60_HB.SetMaxBitRate(wMaxBitRate);
		t265_720_60_HB.SetMediaType(MEDIA_TYPE_H265);
		t265_720_60_HB.SetResolution(VIDEO_FORMAT_HD720);
		
		TVideoStreamCap t265_720_50_960;
		t265_720_50_960.SetUserDefFrameRate(50);
		t265_720_50_960.SetMaxBitRate(960);
		t265_720_50_960.SetMediaType(MEDIA_TYPE_H265);
		t265_720_50_960.SetResolution(VIDEO_FORMAT_HD720);
		
		TVideoStreamCap t265_720_30_HB;
		t265_720_30_HB.SetUserDefFrameRate(30);
		t265_720_30_HB.SetMaxBitRate(wMaxBitRate);
		t265_720_30_HB.SetMediaType(MEDIA_TYPE_H265);
		t265_720_30_HB.SetResolution(VIDEO_FORMAT_HD720);

		TVideoStreamCap t265_720_25_448;
		t265_720_25_448.SetUserDefFrameRate(25);
		t265_720_25_448.SetMaxBitRate(448);
		t265_720_25_448.SetMediaType(MEDIA_TYPE_H265);
		t265_720_25_448.SetResolution(VIDEO_FORMAT_HD720);
		
		// H264 720
		TVideoStreamCap t264_720_60_HP_HB;
		t264_720_60_HP_HB.SetUserDefFrameRate(60);
		t264_720_60_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_720_60_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_720_60_HP_HB.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_60_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		//Ϊ�˱�֤720����Сֵ����һ��sxga 20��������720 30
		TVideoStreamCap t264_720_30_HP_HB;
		t264_720_30_HP_HB.SetUserDefFrameRate(30);
		t264_720_30_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_720_30_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_720_30_HP_HB.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_30_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_720_25_HP_448;
		t264_720_25_HP_448.SetUserDefFrameRate(25);
		t264_720_25_HP_448.SetMaxBitRate(448);
		t264_720_25_HP_448.SetMediaType(MEDIA_TYPE_H264);
		t264_720_25_HP_448.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_25_HP_448.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_720_30_HP_448;
		t264_720_30_HP_448.SetUserDefFrameRate(30);
		t264_720_30_HP_448.SetMaxBitRate(448);
		t264_720_30_HP_448.SetMediaType(MEDIA_TYPE_H264);
		t264_720_30_HP_448.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_30_HP_448.SetH264ProfileAttrb(emHpAttrb);
		
		TVideoStreamCap t264_720_30_BP_HB;
		t264_720_30_BP_HB.SetUserDefFrameRate(30);
		t264_720_30_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_720_30_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_720_30_BP_HB.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_30_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t264_720_25_BP_1472;
		t264_720_25_BP_1472.SetUserDefFrameRate(25);
		t264_720_25_BP_1472.SetMaxBitRate(1472);
		t264_720_25_BP_1472.SetMediaType(MEDIA_TYPE_H264);
		t264_720_25_BP_1472.SetResolution(VIDEO_FORMAT_HD720);
		t264_720_25_BP_1472.SetH264ProfileAttrb(emBpAttrb);

		// H265 w4cif
		TVideoStreamCap t265_w4cif_25_HB;
		t265_w4cif_25_HB.SetUserDefFrameRate(25);
		t265_w4cif_25_HB.SetMaxBitRate(wMaxBitRate);
		t265_w4cif_25_HB.SetMediaType(MEDIA_TYPE_H265);
		t265_w4cif_25_HB.SetResolution(VIDEO_FORMAT_W4CIF);
		
		TVideoStreamCap t265_w4cif_25_384;
		t265_w4cif_25_384.SetUserDefFrameRate(25);
		t265_w4cif_25_384.SetMaxBitRate(384);
		t265_w4cif_25_384.SetMediaType(MEDIA_TYPE_H265);
		t265_w4cif_25_384.SetResolution(VIDEO_FORMAT_W4CIF);

		// H264 w4cif
		TVideoStreamCap t264_W4CIF_25_HP_HB;
		t264_W4CIF_25_HP_HB.SetUserDefFrameRate(25);
		t264_W4CIF_25_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_W4CIF_25_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_W4CIF_25_HP_HB.SetResolution(VIDEO_FORMAT_W4CIF);
		t264_W4CIF_25_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_W4CIF_25_HP_704;
		t264_W4CIF_25_HP_704.SetUserDefFrameRate(25);
		t264_W4CIF_25_HP_704.SetMaxBitRate(704);
		t264_W4CIF_25_HP_704.SetMediaType(MEDIA_TYPE_H264);
		t264_W4CIF_25_HP_704.SetResolution(VIDEO_FORMAT_W4CIF);
		t264_W4CIF_25_HP_704.SetH264ProfileAttrb(emHpAttrb);

		// H265 4cif
		TVideoStreamCap t265_4cif_25_HB;
		t265_4cif_25_HB.SetUserDefFrameRate(25);
		t265_4cif_25_HB.SetMaxBitRate(wMaxBitRate);
		t265_4cif_25_HB.SetMediaType(MEDIA_TYPE_H265);
		t265_4cif_25_HB.SetResolution(VIDEO_FORMAT_4CIF);
		
		TVideoStreamCap t265_4cif_25_256;
		t265_4cif_25_256.SetUserDefFrameRate(25);
		t265_4cif_25_256.SetMaxBitRate(256);
		t265_4cif_25_256.SetMediaType(MEDIA_TYPE_H265);
		t265_4cif_25_256.SetResolution(VIDEO_FORMAT_4CIF);

		// H264 4cif
		TVideoStreamCap t264_4CIF_25_HP_HB;
		t264_4CIF_25_HP_HB.SetUserDefFrameRate(25);
		t264_4CIF_25_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_4CIF_25_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_4CIF_25_HP_HB.SetResolution(VIDEO_FORMAT_4CIF);
		t264_4CIF_25_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		
		TVideoStreamCap t264_4CIF_25_HP_192;
		t264_4CIF_25_HP_192.SetUserDefFrameRate(25);
		t264_4CIF_25_HP_192.SetMaxBitRate(192);
		t264_4CIF_25_HP_192.SetMediaType(MEDIA_TYPE_H264);
		t264_4CIF_25_HP_192.SetResolution(VIDEO_FORMAT_4CIF);
		t264_4CIF_25_HP_192.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_4CIF_25_BP_HB;
		t264_4CIF_25_BP_HB.SetUserDefFrameRate(25);
		t264_4CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_4CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_4CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_4CIF);
		t264_4CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t264_4CIF_25_BP_1472;
		t264_4CIF_25_BP_1472.SetUserDefFrameRate(25);
		t264_4CIF_25_BP_1472.SetMaxBitRate(1472);
		t264_4CIF_25_BP_1472.SetMediaType(MEDIA_TYPE_H264);
		t264_4CIF_25_BP_1472.SetResolution(VIDEO_FORMAT_4CIF);
		t264_4CIF_25_BP_1472.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t264_4CIF_25_BP_960;
		t264_4CIF_25_BP_960.SetUserDefFrameRate(25);
		t264_4CIF_25_BP_960.SetMaxBitRate(960);
		t264_4CIF_25_BP_960.SetMediaType(MEDIA_TYPE_H264);
		t264_4CIF_25_BP_960.SetResolution(VIDEO_FORMAT_4CIF);
		t264_4CIF_25_BP_960.SetH264ProfileAttrb(emBpAttrb);

		// H265 cif
		TVideoStreamCap t265_cif_25_HB;
		t265_cif_25_HB.SetUserDefFrameRate(25);
		t265_cif_25_HB.SetMaxBitRate(wMaxBitRate);
		t265_cif_25_HB.SetMediaType(MEDIA_TYPE_H265);
		t265_cif_25_HB.SetResolution(VIDEO_FORMAT_CIF);
		
		TVideoStreamCap t265_cif_10_64;
		t265_cif_10_64.SetUserDefFrameRate(10);
		t265_cif_10_64.SetMaxBitRate(64);
		t265_cif_10_64.SetMediaType(MEDIA_TYPE_H265);
		t265_cif_10_64.SetResolution(VIDEO_FORMAT_CIF);

		// H264 cif
		TVideoStreamCap t264_CIF_25_HP_HB;
		t264_CIF_25_HP_HB.SetUserDefFrameRate(25);
		t264_CIF_25_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_CIF_25_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_HP_HB.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_HP_HB.SetH264ProfileAttrb(emHpAttrb);
		
		TVideoStreamCap t264_CIF_25_HP_64;
		t264_CIF_25_HP_64.SetUserDefFrameRate(25);
		t264_CIF_25_HP_64.SetMaxBitRate(64);
		t264_CIF_25_HP_64.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_HP_64.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_HP_64.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_CIF_25_BP_HB;
		t264_CIF_25_BP_HB.SetUserDefFrameRate(25);
		t264_CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t264_CIF_25_BP_64;
		t264_CIF_25_BP_64.SetUserDefFrameRate(25);
		t264_CIF_25_BP_64.SetMaxBitRate(64);
		t264_CIF_25_BP_64.SetMediaType(MEDIA_TYPE_H264);
		t264_CIF_25_BP_64.SetResolution(VIDEO_FORMAT_CIF);
		t264_CIF_25_BP_64.SetH264ProfileAttrb(emBpAttrb);

		// mp4
		TVideoStreamCap tMP4_4CIF_25_BP_HB;
		tMP4_4CIF_25_BP_HB.SetFrameRate(25);
		tMP4_4CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		tMP4_4CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_MP4);
		tMP4_4CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_4CIF);
		tMP4_4CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);
		TVideoStreamCap tMP4_CIF_25_BP_HB;
		tMP4_CIF_25_BP_HB.SetFrameRate(25);
		tMP4_CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		tMP4_CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_MP4);
		tMP4_CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_CIF);
		tMP4_CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap tMP4_CIF_25_BP_64;
		tMP4_CIF_25_BP_64.SetFrameRate(25);
		tMP4_CIF_25_BP_64.SetMaxBitRate(64);
		tMP4_CIF_25_BP_64.SetMediaType(MEDIA_TYPE_MP4);
		tMP4_CIF_25_BP_64.SetResolution(VIDEO_FORMAT_CIF);
		tMP4_CIF_25_BP_64.SetH264ProfileAttrb(emBpAttrb);
		
		// h263
		TVideoStreamCap t263_CIF_25_BP_HB;
		t263_CIF_25_BP_HB.SetFrameRate(25);
		t263_CIF_25_BP_HB.SetMaxBitRate(wMaxBitRate);
		t263_CIF_25_BP_HB.SetMediaType(MEDIA_TYPE_H263);
		t263_CIF_25_BP_HB.SetResolution(VIDEO_FORMAT_CIF);
		t263_CIF_25_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t263_CIF_25_BP_64;
		t263_CIF_25_BP_64.SetFrameRate(25);
		t263_CIF_25_BP_64.SetMaxBitRate(64);
		t263_CIF_25_BP_64.SetMediaType(MEDIA_TYPE_H263);
		t263_CIF_25_BP_64.SetResolution(VIDEO_FORMAT_CIF);
		t263_CIF_25_BP_64.SetH264ProfileAttrb(emBpAttrb);

		//H265 SXGA
		TVideoStreamCap t265_SXGA_20_HB;
		t265_SXGA_20_HB.SetFrameRate(20);
		t265_SXGA_20_HB.SetMaxBitRate(wMaxBitRate);
		t265_SXGA_20_HB.SetMediaType(MEDIA_TYPE_H265);
		t265_SXGA_20_HB.SetResolution(VIDEO_FORMAT_SXGA);

		TVideoStreamCap t265_SXGA_5_128;
		t265_SXGA_5_128.SetFrameRate(5);
		t265_SXGA_5_128.SetMaxBitRate(128);
		t265_SXGA_5_128.SetMediaType(MEDIA_TYPE_H265);
		t265_SXGA_5_128.SetResolution(VIDEO_FORMAT_SXGA);

		//H264 SXGA
		TVideoStreamCap t264_SXGA_20_HP_HB;
		t264_SXGA_20_HP_HB.SetFrameRate(20);
		t264_SXGA_20_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_SXGA_20_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_SXGA_20_HP_HB.SetResolution(VIDEO_FORMAT_SXGA);
		t264_SXGA_20_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_SXGA_5_HP_192;
		t264_SXGA_5_HP_192.SetFrameRate(5);
		t264_SXGA_5_HP_192.SetMaxBitRate(192);
		t264_SXGA_5_HP_192.SetMediaType(MEDIA_TYPE_H264);
		t264_SXGA_5_HP_192.SetResolution(VIDEO_FORMAT_SXGA);
		t264_SXGA_5_HP_192.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_SXGA_20_BP_HB;
		t264_SXGA_20_BP_HB.SetFrameRate(20);
		t264_SXGA_20_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_SXGA_20_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_SXGA_20_BP_HB.SetResolution(VIDEO_FORMAT_SXGA);
		t264_SXGA_20_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t264_SXGA_5_BP_256;
		t264_SXGA_5_BP_256.SetFrameRate(5);
		t264_SXGA_5_BP_256.SetMaxBitRate(256);
		t264_SXGA_5_BP_256.SetMediaType(MEDIA_TYPE_H264);
		t264_SXGA_5_BP_256.SetResolution(VIDEO_FORMAT_SXGA);
		t264_SXGA_5_BP_256.SetH264ProfileAttrb(emBpAttrb);

		//H265 XGA
		TVideoStreamCap t265_XGA_20_HB;
		t265_XGA_20_HB.SetFrameRate(20);
		t265_XGA_20_HB.SetMaxBitRate(wMaxBitRate);
		t265_XGA_20_HB.SetMediaType(MEDIA_TYPE_H265);
		t265_XGA_20_HB.SetResolution(VIDEO_FORMAT_XGA);

		TVideoStreamCap t265_XGA_5_HB;
		t265_XGA_5_HB.SetFrameRate(5);
		t265_XGA_5_HB.SetMaxBitRate(wMaxBitRate);
		t265_XGA_5_HB.SetMediaType(MEDIA_TYPE_H265);
		t265_XGA_5_HB.SetResolution(VIDEO_FORMAT_XGA);


		TVideoStreamCap t265_XGA_5_128;
		t265_XGA_5_128.SetFrameRate(5);
		t265_XGA_5_128.SetMaxBitRate(128);
		t265_XGA_5_128.SetMediaType(MEDIA_TYPE_H265);
		t265_XGA_5_128.SetResolution(VIDEO_FORMAT_XGA);

		TVideoStreamCap t265_XGA_5_64;
		t265_XGA_5_64.SetFrameRate(5);
		t265_XGA_5_64.SetMaxBitRate(64);
		t265_XGA_5_64.SetMediaType(MEDIA_TYPE_H265);
		t265_XGA_5_64.SetResolution(VIDEO_FORMAT_XGA);
		//H264 XGA
		TVideoStreamCap t264_XGA_30_HP_HB;
		t264_XGA_30_HP_HB.SetFrameRate(30);
		t264_XGA_30_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_30_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_30_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_30_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_XGA_20_HP_HB;
		t264_XGA_20_HP_HB.SetFrameRate(20);
		t264_XGA_20_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_20_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_20_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_20_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_XGA_15_HP_HB;
		t264_XGA_15_HP_HB.SetFrameRate(15);
		t264_XGA_15_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_15_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_15_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_15_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_XGA_5_HP_HB;
		t264_XGA_5_HP_HB.SetFrameRate(5);
		t264_XGA_5_HP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_5_HP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_5_HP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_5_HP_HB.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_XGA_20_HP_320;
		t264_XGA_20_HP_320.SetFrameRate(20);
		t264_XGA_20_HP_320.SetMaxBitRate(320);
		t264_XGA_20_HP_320.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_20_HP_320.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_20_HP_320.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_XGA_10_HP_192;
		t264_XGA_10_HP_192.SetFrameRate(10);
		t264_XGA_10_HP_192.SetMaxBitRate(192);
		t264_XGA_10_HP_192.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_10_HP_192.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_10_HP_192.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_XGA_5_HP_64;
		t264_XGA_5_HP_64.SetFrameRate(5);
		t264_XGA_5_HP_64.SetMaxBitRate(64);
		t264_XGA_5_HP_64.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_5_HP_64.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_5_HP_64.SetH264ProfileAttrb(emHpAttrb);

		TVideoStreamCap t264_XGA_30_BP_HB;
		t264_XGA_30_BP_HB.SetFrameRate(30);
		t264_XGA_30_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_30_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_30_BP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_30_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t264_XGA_20_BP_HB;
		t264_XGA_20_BP_HB.SetFrameRate(20);
		t264_XGA_20_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_20_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_20_BP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_20_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t264_XGA_15_BP_HB;
		t264_XGA_15_BP_HB.SetFrameRate(15);
		t264_XGA_15_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_15_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_15_BP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_15_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t264_XGA_5_BP_HB;
		t264_XGA_5_BP_HB.SetFrameRate(5);
		t264_XGA_5_BP_HB.SetMaxBitRate(wMaxBitRate);
		t264_XGA_5_BP_HB.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_5_BP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_5_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t264_XGA_20_BP_704;
		t264_XGA_20_BP_704.SetFrameRate(20);
		t264_XGA_20_BP_704.SetMaxBitRate(704);
		t264_XGA_20_BP_704.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_20_BP_704.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_20_BP_704.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t264_XGA_5_BP_64;
		t264_XGA_5_BP_64.SetFrameRate(5);
		t264_XGA_5_BP_64.SetMaxBitRate(64);
		t264_XGA_5_BP_64.SetMediaType(MEDIA_TYPE_H264);
		t264_XGA_5_BP_64.SetResolution(VIDEO_FORMAT_XGA);
		t264_XGA_5_BP_64.SetH264ProfileAttrb(emBpAttrb);

		// h263+
		TVideoStreamCap t263P_XGA_5_BP_HB;
		t263P_XGA_5_BP_HB.SetFrameRate(5);
		t263P_XGA_5_BP_HB.SetMaxBitRate(wMaxBitRate);
		t263P_XGA_5_BP_HB.SetMediaType(MEDIA_TYPE_H263PLUS);
		t263P_XGA_5_BP_HB.SetResolution(VIDEO_FORMAT_XGA);
		t263P_XGA_5_BP_HB.SetH264ProfileAttrb(emBpAttrb);

		TVideoStreamCap t263P_XGA_5_BP_64;
		t263P_XGA_5_BP_64.SetFrameRate(5);
		t263P_XGA_5_BP_64.SetMaxBitRate(64);
		t263P_XGA_5_BP_64.SetMediaType(MEDIA_TYPE_H263PLUS);
		t263P_XGA_5_BP_64.SetResolution(VIDEO_FORMAT_XGA);
		t263P_XGA_5_BP_64.SetH264ProfileAttrb(emBpAttrb);

		//vmp
		//4k*30
		m_t265HD4k2k_30.m_tXVmpCap.m_atMaxCap[0] = t265_4k2k_30_HB;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMaxCap[1] = t265_1080_30_HB;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMaxCap[2] = t265_720_30_HB;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMaxCap[3] = t265_4cif_25_HB;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMaxCap[4] = t264_1080_30_HP_HB;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMaxCap[5] = t264_720_30_HP_HB;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMaxCap[6] = t264_4CIF_25_HP_HB;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMaxCap[7] = t264_720_30_BP_HB;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMaxCap[8] = t264_4CIF_25_BP_HB;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMaxCap[9] = tMP4_4CIF_25_BP_HB;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMaxCap[10] = t263_CIF_25_BP_HB;
		
		m_t265HD4k2k_30.m_tXVmpCap.m_atMinCap[0] = t265_4k2k_10_1984;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMinCap[1] = t265_1080_25_960;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMinCap[2] = t265_720_25_448;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMinCap[3] = t265_cif_10_64;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMinCap[4] = t264_1080_25_HP_1472;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMinCap[5] = t264_720_25_HP_448;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMinCap[6] = t264_CIF_25_HP_64;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMinCap[7] = t264_720_25_BP_1472;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMinCap[8] = t264_CIF_25_BP_64;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMinCap[9] = tMP4_CIF_25_BP_64;
		m_t265HD4k2k_30.m_tXVmpCap.m_atMinCap[10] = t263_CIF_25_BP_64;

		//1080*60
		m_t265HD1080_60.m_tXVmpCap.m_atMaxCap[0] = t265_1080_60_HB;
		m_t265HD1080_60.m_tXVmpCap.m_atMaxCap[1] = t265_1080_30_HB;
		m_t265HD1080_60.m_tXVmpCap.m_atMaxCap[2] = t265_720_30_HB;
		m_t265HD1080_60.m_tXVmpCap.m_atMaxCap[3] = t265_4cif_25_HB;
		m_t265HD1080_60.m_tXVmpCap.m_atMaxCap[4] = t264_1080_30_HP_HB;
		m_t265HD1080_60.m_tXVmpCap.m_atMaxCap[5] = t264_720_30_HP_HB;
		m_t265HD1080_60.m_tXVmpCap.m_atMaxCap[6] = t264_4CIF_25_HP_HB;
		m_t265HD1080_60.m_tXVmpCap.m_atMaxCap[7] = t264_720_30_BP_HB;
		m_t265HD1080_60.m_tXVmpCap.m_atMaxCap[8] = t264_4CIF_25_BP_HB;
		m_t265HD1080_60.m_tXVmpCap.m_atMaxCap[9] = tMP4_4CIF_25_BP_HB;
		m_t265HD1080_60.m_tXVmpCap.m_atMaxCap[10] = t263_CIF_25_BP_HB;

		m_t265HD1080_60.m_tXVmpCap.m_atMinCap[0] = t265_1080_50_1984;
		m_t265HD1080_60.m_tXVmpCap.m_atMinCap[1] = t265_1080_25_960;
		m_t265HD1080_60.m_tXVmpCap.m_atMinCap[2] = t265_720_25_448;
		m_t265HD1080_60.m_tXVmpCap.m_atMinCap[3] = t265_cif_10_64;
		m_t265HD1080_60.m_tXVmpCap.m_atMinCap[4] = t264_1080_25_HP_1472;
		m_t265HD1080_60.m_tXVmpCap.m_atMinCap[5] = t264_720_25_HP_448;
		m_t265HD1080_60.m_tXVmpCap.m_atMinCap[6] = t264_CIF_25_HP_64;
		m_t265HD1080_60.m_tXVmpCap.m_atMinCap[7] = t264_720_25_BP_1472;
		m_t265HD1080_60.m_tXVmpCap.m_atMinCap[8] = t264_CIF_25_BP_64;
		m_t265HD1080_60.m_tXVmpCap.m_atMinCap[9] = tMP4_CIF_25_BP_64;
		m_t265HD1080_60.m_tXVmpCap.m_atMinCap[10] = t263_CIF_25_BP_64;

		//1080*30
		m_t265HD1080_30.m_tXVmpCap.m_atMaxCap[0] = t265_1080_30_HB;
		m_t265HD1080_30.m_tXVmpCap.m_atMaxCap[1] = t265_720_30_HB;
		m_t265HD1080_30.m_tXVmpCap.m_atMaxCap[2] = t265_4cif_25_HB;
		m_t265HD1080_30.m_tXVmpCap.m_atMaxCap[3] = t264_1080_30_HP_HB;
		m_t265HD1080_30.m_tXVmpCap.m_atMaxCap[4] = t264_720_30_HP_HB;
		m_t265HD1080_30.m_tXVmpCap.m_atMaxCap[5] = t264_4CIF_25_HP_HB;
		m_t265HD1080_30.m_tXVmpCap.m_atMaxCap[6] = t264_1080_30_BP_HB;
		m_t265HD1080_30.m_tXVmpCap.m_atMaxCap[7] = t264_720_30_BP_HB;
		m_t265HD1080_30.m_tXVmpCap.m_atMaxCap[8] = t264_4CIF_25_BP_HB;
		m_t265HD1080_30.m_tXVmpCap.m_atMaxCap[9] = tMP4_4CIF_25_BP_HB;
		m_t265HD1080_30.m_tXVmpCap.m_atMaxCap[10] = t263_CIF_25_BP_HB;
		
		m_t265HD1080_30.m_tXVmpCap.m_atMinCap[0] = t265_1080_25_960;
		m_t265HD1080_30.m_tXVmpCap.m_atMinCap[1] = t265_720_25_448;
		m_t265HD1080_30.m_tXVmpCap.m_atMinCap[2] = t265_cif_10_64;
		m_t265HD1080_30.m_tXVmpCap.m_atMinCap[3] = t264_1080_25_HP_1472;
		m_t265HD1080_30.m_tXVmpCap.m_atMinCap[4] = t264_720_25_HP_448;
		m_t265HD1080_30.m_tXVmpCap.m_atMinCap[5] = t264_CIF_25_HP_64;
		m_t265HD1080_30.m_tXVmpCap.m_atMinCap[6] = t264_1080_25_BP_2496;
		m_t265HD1080_30.m_tXVmpCap.m_atMinCap[7] = t264_720_25_BP_1472;
		m_t265HD1080_30.m_tXVmpCap.m_atMinCap[8] = t264_CIF_25_BP_64;
		m_t265HD1080_30.m_tXVmpCap.m_atMinCap[9] = tMP4_CIF_25_BP_64;
		m_t265HD1080_30.m_tXVmpCap.m_atMinCap[10] = t263_CIF_25_BP_64;

		//720*60
		m_t265HD720_60.m_tXVmpCap.m_atMaxCap[0] = t265_720_60_HB;
		m_t265HD720_60.m_tXVmpCap.m_atMaxCap[1] = t265_720_30_HB;
		m_t265HD720_60.m_tXVmpCap.m_atMaxCap[2] = t265_4cif_25_HB;
		m_t265HD720_60.m_tXVmpCap.m_atMaxCap[3] = t264_720_30_HP_HB;
		m_t265HD720_60.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_HP_HB;
		m_t265HD720_60.m_tXVmpCap.m_atMaxCap[5] = t264_CIF_25_HP_HB;
		m_t265HD720_60.m_tXVmpCap.m_atMaxCap[6] = t264_720_30_BP_HB;
		m_t265HD720_60.m_tXVmpCap.m_atMaxCap[7] = t264_4CIF_25_BP_HB;
		m_t265HD720_60.m_tXVmpCap.m_atMaxCap[8] = t264_CIF_25_BP_HB;
		m_t265HD720_60.m_tXVmpCap.m_atMaxCap[9] = tMP4_4CIF_25_BP_HB;
		m_t265HD720_60.m_tXVmpCap.m_atMaxCap[10] = t263_CIF_25_BP_HB;
		
		m_t265HD720_60.m_tXVmpCap.m_atMinCap[0] = t265_720_50_960;
		m_t265HD720_60.m_tXVmpCap.m_atMinCap[1] = t265_720_25_448;
		m_t265HD720_60.m_tXVmpCap.m_atMinCap[2] = t265_cif_10_64;
		m_t265HD720_60.m_tXVmpCap.m_atMinCap[3] = t264_720_25_HP_448;
		m_t265HD720_60.m_tXVmpCap.m_atMinCap[4] = t264_4CIF_25_HP_192;
		m_t265HD720_60.m_tXVmpCap.m_atMinCap[5] = t264_CIF_25_HP_64;
		m_t265HD720_60.m_tXVmpCap.m_atMinCap[6] = t264_720_25_BP_1472;
		m_t265HD720_60.m_tXVmpCap.m_atMinCap[7] = t264_4CIF_25_BP_960;
		m_t265HD720_60.m_tXVmpCap.m_atMinCap[8] = t264_CIF_25_BP_64;
		m_t265HD720_60.m_tXVmpCap.m_atMinCap[9] = tMP4_CIF_25_BP_64;
		m_t265HD720_60.m_tXVmpCap.m_atMinCap[10] = t263_CIF_25_BP_64;

		//720*30
		m_t265HD720_30.m_tXVmpCap.m_atMaxCap[0] = t265_720_30_HB;
		m_t265HD720_30.m_tXVmpCap.m_atMaxCap[1] = t265_4cif_25_HB;
		m_t265HD720_30.m_tXVmpCap.m_atMaxCap[2] = t265_cif_25_HB;
		m_t265HD720_30.m_tXVmpCap.m_atMaxCap[3] = t264_720_30_HP_HB;
		m_t265HD720_30.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_HP_HB;
		m_t265HD720_30.m_tXVmpCap.m_atMaxCap[5] = t264_CIF_25_HP_HB;
		m_t265HD720_30.m_tXVmpCap.m_atMaxCap[6] = t264_720_30_BP_HB;
		m_t265HD720_30.m_tXVmpCap.m_atMaxCap[7] = t264_4CIF_25_BP_HB;
		m_t265HD720_30.m_tXVmpCap.m_atMaxCap[8] = t264_CIF_25_BP_HB;
		m_t265HD720_30.m_tXVmpCap.m_atMaxCap[9] = tMP4_4CIF_25_BP_HB;
		m_t265HD720_30.m_tXVmpCap.m_atMaxCap[10] = t263_CIF_25_BP_HB;
		
		m_t265HD720_30.m_tXVmpCap.m_atMinCap[0] = t265_720_25_448;
		m_t265HD720_30.m_tXVmpCap.m_atMinCap[1] = t265_4cif_25_256;
		m_t265HD720_30.m_tXVmpCap.m_atMinCap[2] = t265_cif_10_64;
		m_t265HD720_30.m_tXVmpCap.m_atMinCap[3] = t264_720_25_HP_448;
		m_t265HD720_30.m_tXVmpCap.m_atMinCap[4] = t264_4CIF_25_HP_192;
		m_t265HD720_30.m_tXVmpCap.m_atMinCap[5] = t264_CIF_25_HP_64;
		m_t265HD720_30.m_tXVmpCap.m_atMinCap[6] = t264_720_25_BP_1472;
		m_t265HD720_30.m_tXVmpCap.m_atMinCap[7] = t264_4CIF_25_BP_960;
		m_t265HD720_30.m_tXVmpCap.m_atMinCap[8] = t264_CIF_25_BP_64;
		m_t265HD720_30.m_tXVmpCap.m_atMinCap[9] = tMP4_CIF_25_BP_64;
		m_t265HD720_30.m_tXVmpCap.m_atMinCap[10] = t263_CIF_25_BP_64;

		//����
		m_t265SD.m_tXVmpCap.m_atMaxCap[0] = t265_w4cif_25_HB;
		m_t265SD.m_tXVmpCap.m_atMaxCap[1] = t265_4cif_25_HB;
		m_t265SD.m_tXVmpCap.m_atMaxCap[2] = t265_cif_25_HB;
		m_t265SD.m_tXVmpCap.m_atMaxCap[3] = t264_W4CIF_25_HP_HB;
		m_t265SD.m_tXVmpCap.m_atMaxCap[4] = t264_4CIF_25_HP_HB;
		m_t265SD.m_tXVmpCap.m_atMaxCap[5] = t264_CIF_25_HP_HB;
		m_t265SD.m_tXVmpCap.m_atMaxCap[6] = t264_4CIF_25_BP_HB;
		m_t265SD.m_tXVmpCap.m_atMaxCap[7] = t264_4CIF_25_BP_HB;
		m_t265SD.m_tXVmpCap.m_atMaxCap[8] = t264_CIF_25_BP_HB;
		m_t265SD.m_tXVmpCap.m_atMaxCap[9] = tMP4_4CIF_25_BP_HB;
		m_t265SD.m_tXVmpCap.m_atMaxCap[10] = t263_CIF_25_BP_HB;
		
		m_t265SD.m_tXVmpCap.m_atMinCap[0] = t265_w4cif_25_384;
		m_t265SD.m_tXVmpCap.m_atMinCap[1] = t265_4cif_25_256;
		m_t265SD.m_tXVmpCap.m_atMinCap[2] = t265_cif_10_64;
		m_t265SD.m_tXVmpCap.m_atMinCap[3] = t264_W4CIF_25_HP_704;
		m_t265SD.m_tXVmpCap.m_atMinCap[4] = t264_4CIF_25_HP_192;
		m_t265SD.m_tXVmpCap.m_atMinCap[5] = t264_CIF_25_HP_64;
		m_t265SD.m_tXVmpCap.m_atMinCap[6] = t264_4CIF_25_BP_1472;
		m_t265SD.m_tXVmpCap.m_atMinCap[7] = t264_4CIF_25_BP_960;
		m_t265SD.m_tXVmpCap.m_atMinCap[8] = t264_CIF_25_BP_64;
		m_t265SD.m_tXVmpCap.m_atMinCap[9] = tMP4_CIF_25_BP_64;
		m_t265SD.m_tXVmpCap.m_atMinCap[10] = t263_CIF_25_BP_64;


		// vbas 4k30
		m_t265HD4k2k_30.m_tXVBasCap.m_atMaxCap[0] = t265_1080_30_HB;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMaxCap[1] = t265_720_30_HB;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMaxCap[2] = t265_4cif_25_HB;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMaxCap[3] = t264_1080_30_HP_HB;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMaxCap[4] = t264_720_30_HP_HB;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMaxCap[5] = t264_4CIF_25_HP_HB;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMaxCap[6] = t264_1080_30_BP_HB;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMaxCap[7] = t264_4CIF_25_BP_HB;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMaxCap[8] = tMP4_CIF_25_BP_HB;

		m_t265HD4k2k_30.m_tXVBasCap.m_atMinCap[0] = t265_1080_25_960;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMinCap[1] = t265_720_25_448;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMinCap[2] = t265_cif_10_64;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMinCap[3] = t264_1080_25_HP_1472;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMinCap[4] = t264_720_25_HP_448;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMinCap[5] = t264_CIF_25_HP_64;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMinCap[6] = t264_720_25_BP_1472;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMinCap[7] = t264_CIF_25_BP_64;
		m_t265HD4k2k_30.m_tXVBasCap.m_atMinCap[8] = tMP4_CIF_25_BP_64;

		//1080*60
		m_t265HD1080_60.m_tXVBasCap.m_atMaxCap[0] = t265_1080_30_HB;
		m_t265HD1080_60.m_tXVBasCap.m_atMaxCap[1] = t265_720_30_HB;
		m_t265HD1080_60.m_tXVBasCap.m_atMaxCap[2] = t265_4cif_25_HB;
		m_t265HD1080_60.m_tXVBasCap.m_atMaxCap[3] = t264_1080_30_HP_HB;
		m_t265HD1080_60.m_tXVBasCap.m_atMaxCap[4] = t264_720_30_HP_HB;
		m_t265HD1080_60.m_tXVBasCap.m_atMaxCap[5] = t264_4CIF_25_HP_HB;
		m_t265HD1080_60.m_tXVBasCap.m_atMaxCap[6] = t264_1080_30_BP_HB;
		m_t265HD1080_60.m_tXVBasCap.m_atMaxCap[7] = t264_4CIF_25_BP_HB;
		m_t265HD1080_60.m_tXVBasCap.m_atMaxCap[8] = tMP4_CIF_25_BP_HB;
		
		m_t265HD1080_60.m_tXVBasCap.m_atMinCap[0] = t265_1080_25_960;
		m_t265HD1080_60.m_tXVBasCap.m_atMinCap[1] = t265_720_25_448;
		m_t265HD1080_60.m_tXVBasCap.m_atMinCap[2] = t265_cif_10_64;
		m_t265HD1080_60.m_tXVBasCap.m_atMinCap[3] = t264_1080_25_HP_1472;
		m_t265HD1080_60.m_tXVBasCap.m_atMinCap[4] = t264_720_25_HP_448;
		m_t265HD1080_60.m_tXVBasCap.m_atMinCap[5] = t264_CIF_25_HP_64;
		m_t265HD1080_60.m_tXVBasCap.m_atMinCap[6] = t264_720_25_BP_1472;
		m_t265HD1080_60.m_tXVBasCap.m_atMinCap[7] = t264_CIF_25_BP_64;
		m_t265HD1080_60.m_tXVBasCap.m_atMinCap[8] = tMP4_CIF_25_BP_64;

		//1080*30
		m_t265HD1080_30.m_tXVBasCap.m_atMaxCap[0] = t265_1080_30_HB;
		m_t265HD1080_30.m_tXVBasCap.m_atMaxCap[1] = t265_720_30_HB;
		m_t265HD1080_30.m_tXVBasCap.m_atMaxCap[2] = t265_4cif_25_HB;
		m_t265HD1080_30.m_tXVBasCap.m_atMaxCap[3] = t264_1080_30_HP_HB;
		m_t265HD1080_30.m_tXVBasCap.m_atMaxCap[4] = t264_720_30_HP_HB;
		m_t265HD1080_30.m_tXVBasCap.m_atMaxCap[5] = t264_4CIF_25_HP_HB;
		m_t265HD1080_30.m_tXVBasCap.m_atMaxCap[6] = t264_1080_30_BP_HB;
		m_t265HD1080_30.m_tXVBasCap.m_atMaxCap[7] = t264_4CIF_25_BP_HB;
		m_t265HD1080_30.m_tXVBasCap.m_atMaxCap[8] = tMP4_CIF_25_BP_HB;
		
		m_t265HD1080_30.m_tXVBasCap.m_atMinCap[0] = t265_1080_25_960;
		m_t265HD1080_30.m_tXVBasCap.m_atMinCap[1] = t265_720_25_448;
		m_t265HD1080_30.m_tXVBasCap.m_atMinCap[2] = t265_cif_10_64;
		m_t265HD1080_30.m_tXVBasCap.m_atMinCap[3] = t264_1080_25_HP_1472;
		m_t265HD1080_30.m_tXVBasCap.m_atMinCap[4] = t264_720_25_HP_448;
		m_t265HD1080_30.m_tXVBasCap.m_atMinCap[5] = t264_CIF_25_HP_64;
		m_t265HD1080_30.m_tXVBasCap.m_atMinCap[6] = t264_720_25_BP_1472;
		m_t265HD1080_30.m_tXVBasCap.m_atMinCap[7] = t264_CIF_25_BP_64;
		m_t265HD1080_30.m_tXVBasCap.m_atMinCap[8] = tMP4_CIF_25_BP_64;

		//720*60
		m_t265HD720_60.m_tXVBasCap.m_atMaxCap[0] = t265_720_30_HB;
		m_t265HD720_60.m_tXVBasCap.m_atMaxCap[1] = t265_4cif_25_HB;
		m_t265HD720_60.m_tXVBasCap.m_atMaxCap[2] = t264_720_30_HP_HB;
		m_t265HD720_60.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_HP_HB;
		m_t265HD720_60.m_tXVBasCap.m_atMaxCap[4] = t264_CIF_25_HP_HB;
		m_t265HD720_60.m_tXVBasCap.m_atMaxCap[5] = t264_720_30_BP_HB;
		m_t265HD720_60.m_tXVBasCap.m_atMaxCap[6] = t264_4CIF_25_BP_HB;
		m_t265HD720_60.m_tXVBasCap.m_atMaxCap[7] = t264_CIF_25_BP_HB;
		m_t265HD720_60.m_tXVBasCap.m_atMaxCap[8] = tMP4_CIF_25_BP_HB;
		
		m_t265HD720_60.m_tXVBasCap.m_atMinCap[0] = t265_720_25_448;
		m_t265HD720_60.m_tXVBasCap.m_atMinCap[1] = t265_cif_10_64;
		m_t265HD720_60.m_tXVBasCap.m_atMinCap[2] = t264_720_25_HP_448;
		m_t265HD720_60.m_tXVBasCap.m_atMinCap[3] = t264_4CIF_25_HP_192;
		m_t265HD720_60.m_tXVBasCap.m_atMinCap[4] = t264_CIF_25_HP_64;
		m_t265HD720_60.m_tXVBasCap.m_atMinCap[5] = t264_720_25_BP_1472;
		m_t265HD720_60.m_tXVBasCap.m_atMinCap[6] = t264_4CIF_25_BP_960;
		m_t265HD720_60.m_tXVBasCap.m_atMinCap[7] = t264_CIF_25_BP_64;
		m_t265HD720_60.m_tXVBasCap.m_atMinCap[8] = tMP4_CIF_25_BP_64;

		//720*30
		m_t265HD720_30.m_tXVBasCap.m_atMaxCap[0] = t265_720_30_HB;
		m_t265HD720_30.m_tXVBasCap.m_atMaxCap[1] = t265_4cif_25_HB;
		m_t265HD720_30.m_tXVBasCap.m_atMaxCap[2] = t264_720_30_HP_HB;
		m_t265HD720_30.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_HP_HB;
		m_t265HD720_30.m_tXVBasCap.m_atMaxCap[4] = t264_CIF_25_HP_HB;
		m_t265HD720_30.m_tXVBasCap.m_atMaxCap[5] = t264_720_30_BP_HB;
		m_t265HD720_30.m_tXVBasCap.m_atMaxCap[6] = t264_4CIF_25_BP_HB;
		m_t265HD720_30.m_tXVBasCap.m_atMaxCap[7] = t264_CIF_25_BP_HB;
		m_t265HD720_30.m_tXVBasCap.m_atMaxCap[8] = tMP4_CIF_25_BP_HB;
		
		m_t265HD720_30.m_tXVBasCap.m_atMinCap[0] = t265_720_25_448;
		m_t265HD720_30.m_tXVBasCap.m_atMinCap[1] = t265_cif_10_64;
		m_t265HD720_30.m_tXVBasCap.m_atMinCap[2] = t264_720_25_HP_448;
		m_t265HD720_30.m_tXVBasCap.m_atMinCap[3] = t264_4CIF_25_HP_192;
		m_t265HD720_30.m_tXVBasCap.m_atMinCap[4] = t264_CIF_25_HP_64;
		m_t265HD720_30.m_tXVBasCap.m_atMinCap[5] = t264_720_25_BP_1472;
		m_t265HD720_30.m_tXVBasCap.m_atMinCap[6] = t264_4CIF_25_BP_960;
		m_t265HD720_30.m_tXVBasCap.m_atMinCap[7] = t264_CIF_25_BP_64;
		m_t265HD720_30.m_tXVBasCap.m_atMinCap[8] = tMP4_CIF_25_BP_64;

		//biaoqing
		m_t265SD.m_tXVBasCap.m_atMaxCap[0] = t265_4cif_25_HB;
		m_t265SD.m_tXVBasCap.m_atMaxCap[1] = t265_cif_25_HB;
		m_t265SD.m_tXVBasCap.m_atMaxCap[2] = t264_W4CIF_25_HP_HB;
		m_t265SD.m_tXVBasCap.m_atMaxCap[3] = t264_4CIF_25_HP_HB;
		m_t265SD.m_tXVBasCap.m_atMaxCap[4] = t264_CIF_25_HP_HB;
		m_t265SD.m_tXVBasCap.m_atMaxCap[5] = t264_4CIF_25_BP_HB;
		m_t265SD.m_tXVBasCap.m_atMaxCap[6] = t264_4CIF_25_BP_HB;
		m_t265SD.m_tXVBasCap.m_atMaxCap[7] = t264_CIF_25_BP_HB;
		m_t265SD.m_tXVBasCap.m_atMaxCap[8] = tMP4_CIF_25_BP_HB;
		
		m_t265SD.m_tXVBasCap.m_atMinCap[0] = t265_4cif_25_256;
		m_t265SD.m_tXVBasCap.m_atMinCap[1] = t265_cif_10_64;
		m_t265SD.m_tXVBasCap.m_atMinCap[2] = t264_W4CIF_25_HP_704;
		m_t265SD.m_tXVBasCap.m_atMinCap[3] = t264_4CIF_25_HP_192;
		m_t265SD.m_tXVBasCap.m_atMinCap[4] = t264_CIF_25_HP_64;
		m_t265SD.m_tXVBasCap.m_atMinCap[5] = t264_4CIF_25_BP_1472;
		m_t265SD.m_tXVBasCap.m_atMinCap[6] = t264_4CIF_25_BP_960;
		m_t265SD.m_tXVBasCap.m_atMinCap[7] = t264_CIF_25_BP_64;
		m_t265SD.m_tXVBasCap.m_atMinCap[8] = tMP4_CIF_25_BP_64;

		//dvbas
		//4k2k 30
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMaxCap[0] = t265_1080_30_HB;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMaxCap[1] = t265_SXGA_20_HB;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMaxCap[2] = t265_XGA_20_HB;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMaxCap[3] = t264_1080_30_HP_HB;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMaxCap[4] = t264_SXGA_20_HP_HB;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMaxCap[5] = t264_SXGA_20_BP_HB;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMaxCap[6] = t264_XGA_5_BP_HB;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMaxCap[7] = t263P_XGA_5_BP_HB;
		
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMinCap[0] = t265_1080_25_960;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMinCap[1] = t265_SXGA_5_128;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMinCap[2] = t265_XGA_5_64;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMinCap[3] = t264_720_30_HP_448;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMinCap[4] = t264_XGA_5_HP_64;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMinCap[5] = t264_SXGA_5_BP_256;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMinCap[6] = t264_XGA_5_BP_64;
		m_t265HD4k2k_30.m_tXDSBasCap.m_atMinCap[7] = t263P_XGA_5_BP_64;

		//1080 60
		m_t265HD1080_60.m_tXDSBasCap.m_atMaxCap[0] = t265_1080_30_HB;
		m_t265HD1080_60.m_tXDSBasCap.m_atMaxCap[1] = t265_SXGA_20_HB;
		m_t265HD1080_60.m_tXDSBasCap.m_atMaxCap[2] = t265_XGA_20_HB;
		m_t265HD1080_60.m_tXDSBasCap.m_atMaxCap[3] = t264_1080_30_HP_HB;
		m_t265HD1080_60.m_tXDSBasCap.m_atMaxCap[4] = t264_SXGA_20_HP_HB;
		m_t265HD1080_60.m_tXDSBasCap.m_atMaxCap[5] = t264_SXGA_20_BP_HB;
		m_t265HD1080_60.m_tXDSBasCap.m_atMaxCap[6] = t264_XGA_5_BP_HB;
		m_t265HD1080_60.m_tXDSBasCap.m_atMaxCap[7] = t263P_XGA_5_BP_HB;
		
		m_t265HD1080_60.m_tXDSBasCap.m_atMinCap[0] = t265_1080_25_960;
		m_t265HD1080_60.m_tXDSBasCap.m_atMinCap[1] = t265_SXGA_5_128;
		m_t265HD1080_60.m_tXDSBasCap.m_atMinCap[2] = t265_XGA_5_64;
		m_t265HD1080_60.m_tXDSBasCap.m_atMinCap[3] = t264_720_30_HP_448;
		m_t265HD1080_60.m_tXDSBasCap.m_atMinCap[4] = t264_XGA_5_HP_64;
		m_t265HD1080_60.m_tXDSBasCap.m_atMinCap[5] = t264_SXGA_5_BP_256;
		m_t265HD1080_60.m_tXDSBasCap.m_atMinCap[6] = t264_XGA_5_BP_64;
		m_t265HD1080_60.m_tXDSBasCap.m_atMinCap[7] = t263P_XGA_5_BP_64;

		//1080 30
		m_t265HD1080_30.m_tXDSBasCap.m_atMaxCap[0] = t265_1080_30_HB;
		m_t265HD1080_30.m_tXDSBasCap.m_atMaxCap[1] = t265_SXGA_20_HB;
		m_t265HD1080_30.m_tXDSBasCap.m_atMaxCap[2] = t265_XGA_20_HB;
		m_t265HD1080_30.m_tXDSBasCap.m_atMaxCap[3] = t264_1080_30_HP_HB;
		m_t265HD1080_30.m_tXDSBasCap.m_atMaxCap[4] = t264_SXGA_20_HP_HB;
		m_t265HD1080_30.m_tXDSBasCap.m_atMaxCap[5] = t264_SXGA_20_BP_HB;
		m_t265HD1080_30.m_tXDSBasCap.m_atMaxCap[6] = t264_XGA_5_BP_HB;
		m_t265HD1080_30.m_tXDSBasCap.m_atMaxCap[7] = t263P_XGA_5_BP_HB;
		
		m_t265HD1080_30.m_tXDSBasCap.m_atMinCap[0] = t265_1080_25_960;
		m_t265HD1080_30.m_tXDSBasCap.m_atMinCap[1] = t265_SXGA_5_128;
		m_t265HD1080_30.m_tXDSBasCap.m_atMinCap[2] = t265_XGA_5_64;
		m_t265HD1080_30.m_tXDSBasCap.m_atMinCap[3] = t264_720_30_HP_448;
		m_t265HD1080_30.m_tXDSBasCap.m_atMinCap[4] = t264_XGA_5_HP_64;
		m_t265HD1080_30.m_tXDSBasCap.m_atMinCap[5] = t264_SXGA_5_BP_256;
		m_t265HD1080_30.m_tXDSBasCap.m_atMinCap[6] = t264_XGA_5_BP_64;
		m_t265HD1080_30.m_tXDSBasCap.m_atMinCap[7] = t263P_XGA_5_BP_64;

		//720 60
		m_t265HD720_60.m_tXDSBasCap.m_atMaxCap[0] = t265_SXGA_20_HB;
		m_t265HD720_60.m_tXDSBasCap.m_atMaxCap[1] = t265_XGA_20_HB;
		m_t265HD720_60.m_tXDSBasCap.m_atMaxCap[2] = t264_720_30_HP_HB;
		m_t265HD720_60.m_tXDSBasCap.m_atMaxCap[3] = t264_SXGA_20_HP_HB;
		m_t265HD720_60.m_tXDSBasCap.m_atMaxCap[4] = t264_XGA_20_HP_HB;
		m_t265HD720_60.m_tXDSBasCap.m_atMaxCap[5] = t264_SXGA_20_BP_HB;
		m_t265HD720_60.m_tXDSBasCap.m_atMaxCap[6] = t264_XGA_20_BP_HB;
		m_t265HD720_60.m_tXDSBasCap.m_atMaxCap[7] = t263P_XGA_5_BP_HB;
		
		m_t265HD720_60.m_tXDSBasCap.m_atMinCap[0] = t265_SXGA_5_128;
		m_t265HD720_60.m_tXDSBasCap.m_atMinCap[1] = t265_XGA_5_64;
		m_t265HD720_60.m_tXDSBasCap.m_atMinCap[2] = t264_720_30_HP_448;
		m_t265HD720_60.m_tXDSBasCap.m_atMinCap[3] = t264_SXGA_5_HP_192;
		m_t265HD720_60.m_tXDSBasCap.m_atMinCap[4] = t264_XGA_5_HP_64;
		m_t265HD720_60.m_tXDSBasCap.m_atMinCap[5] = t264_SXGA_5_BP_256;
		m_t265HD720_60.m_tXDSBasCap.m_atMinCap[6] = t264_XGA_5_BP_64;
		m_t265HD720_60.m_tXDSBasCap.m_atMinCap[7] = t263P_XGA_5_BP_64;

		//720 30
		m_t265HD720_30.m_tXDSBasCap.m_atMaxCap[0] = t265_SXGA_20_HB;
		m_t265HD720_30.m_tXDSBasCap.m_atMaxCap[1] = t265_XGA_20_HB;
		m_t265HD720_30.m_tXDSBasCap.m_atMaxCap[2] = t264_720_30_HP_HB;
		m_t265HD720_30.m_tXDSBasCap.m_atMaxCap[3] = t264_SXGA_20_HP_HB;
		m_t265HD720_30.m_tXDSBasCap.m_atMaxCap[4] = t264_XGA_20_HP_HB;
		m_t265HD720_30.m_tXDSBasCap.m_atMaxCap[5] = t264_SXGA_20_BP_HB;
		m_t265HD720_30.m_tXDSBasCap.m_atMaxCap[6] = t264_XGA_5_BP_HB;
		m_t265HD720_30.m_tXDSBasCap.m_atMaxCap[7] = t263P_XGA_5_BP_HB;
		
		m_t265HD720_30.m_tXDSBasCap.m_atMinCap[0] = t265_SXGA_5_128;
		m_t265HD720_30.m_tXDSBasCap.m_atMinCap[1] = t265_XGA_5_64;
		m_t265HD720_30.m_tXDSBasCap.m_atMinCap[2] = t264_720_30_HP_448;
		m_t265HD720_30.m_tXDSBasCap.m_atMinCap[3] = t264_SXGA_5_HP_192;
		m_t265HD720_30.m_tXDSBasCap.m_atMinCap[4] = t264_XGA_5_HP_64;
		m_t265HD720_30.m_tXDSBasCap.m_atMinCap[5] = t264_SXGA_5_BP_256;
		m_t265HD720_30.m_tXDSBasCap.m_atMinCap[6] = t264_XGA_5_BP_64;
		m_t265HD720_30.m_tXDSBasCap.m_atMinCap[7] = t263P_XGA_5_BP_64;

		//����
		m_t265SD.m_tXDSBasCap.m_atMaxCap[0] = t265_XGA_20_HB;
		m_t265SD.m_tXDSBasCap.m_atMaxCap[1] = t265_XGA_5_HB;
		m_t265SD.m_tXDSBasCap.m_atMaxCap[2] = t264_XGA_30_HP_HB;
		m_t265SD.m_tXDSBasCap.m_atMaxCap[3] = t264_XGA_15_HP_HB;
		m_t265SD.m_tXDSBasCap.m_atMaxCap[4] = t264_XGA_5_HP_HB;
		m_t265SD.m_tXDSBasCap.m_atMaxCap[5] = t264_XGA_30_BP_HB;
		m_t265SD.m_tXDSBasCap.m_atMaxCap[6] = t264_XGA_15_BP_HB;
		m_t265SD.m_tXDSBasCap.m_atMaxCap[7] = t263P_XGA_5_BP_HB;
		
		m_t265SD.m_tXDSBasCap.m_atMinCap[0] = t265_XGA_5_128;
		m_t265SD.m_tXDSBasCap.m_atMinCap[1] = t265_XGA_5_64;
		m_t265SD.m_tXDSBasCap.m_atMinCap[2] = t264_XGA_20_HP_320;
		m_t265SD.m_tXDSBasCap.m_atMinCap[3] = t264_XGA_10_HP_192;
		m_t265SD.m_tXDSBasCap.m_atMinCap[4] = t264_XGA_5_HP_64;
		m_t265SD.m_tXDSBasCap.m_atMinCap[5] = t264_XGA_20_BP_704;
		m_t265SD.m_tXDSBasCap.m_atMinCap[6] = t264_XGA_5_BP_64;
		m_t265SD.m_tXDSBasCap.m_atMinCap[7] = t263P_XGA_5_BP_64;

	}

	public:
		void print()
		{
			StaticLog( "========================= H264 ============================\n");
			StaticLog( " Id   BR      Type          Res           FR      BP/HP\n" );
			StaticLog("********HD1080_60:\n");
			m_tHD1080_60.print();
			StaticLog("\n********HD1080_30:\n");
			m_tHD1080_30.print();
			StaticLog("\n********HD720_60:\n");
			m_tHD720_60.print();
			StaticLog("\n********HD720_30:\n");
			m_tHD720_30.print();
			StaticLog("\n********SD:\n");
			m_tSD.print();
			StaticLog( "\n========================== H265 ===========================\n");
			StaticLog( " Id   BR      Type          Res           FR      BP/HP\n" );
			StaticLog("********265 4k2k_30:\n");
			m_t265HD4k2k_30.print(FALSE);
			StaticLog("********265 HD1080_60:\n");
			m_t265HD1080_60.print(FALSE);
			StaticLog("\n********265 HD1080_30:\n");
			m_t265HD1080_30.print(FALSE);
			StaticLog("\n********265 HD720_60:\n");
			m_t265HD720_60.print(FALSE);
			StaticLog("\n********265 HD720_30:\n");
			m_t265HD720_30.print(FALSE);
			StaticLog("\n********265 SD:\n");
			m_t265SD.print(FALSE);
		}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// ����MIXģ�� [2015/5/19/wsk]
struct TMixModule
{
public:
	u8 m_byMixMode;			// 
	u8 m_byVACTime;			// a period of time to change vacmt (s)
	u8 m_bySpecMixMember[MAXNUM_CONF_MT]; // specmix member list,saving the idx of mtlist(createconf)

public:
	TMixModule(void)
	{
		Clear();
	}
private:

public:

	void Clear(void)
	{
		memset(this, 0 , sizeof(TMixModule));
	}
	void SetVACPeriodTime(u8 byTime)
	{
		m_byVACTime = byTime;
		return;
	}
	u8 GetVACPeriodTime(void)
	{
		return m_byVACTime;
	}
	void SetModuleMixMode(u8 byMode)
	{
		m_byMixMode = byMode;
		return;
	}
	u8 GetModuleMixMode(void)
	{
		return m_byMixMode;
	}
	void SetMixMtIdx(u8 byArridx ,u8 byMtIdx)
	{
		if ( 0 == byMtIdx || byMtIdx > MAXNUM_CONF_MT || byArridx >= MAXNUM_CONF_MT)
		{
			return;
		}
		m_bySpecMixMember[byArridx] = byMtIdx;
	}
	u8 GetMixMtIdx(u8 byArrIdx)	// byArrIdx[0,MAXNUM_CONF_MT)
	{
		if ( byArrIdx >= MAXNUM_CONF_MT )
		{
			return 0;
		}
		return m_bySpecMixMember[byArrIdx];
	}
	void PrintMixModule ( void )
	{
		OspPrintf(TRUE, FALSE, "[TMixModule]: MixMode:%d , VacTime:%d , MtList: \n", m_byMixMode, m_byVACTime);

		u8 byIdx = 0;
		u8 byPOutNum = 0;
		while(byIdx < MAXNUM_CONF_MT)
		{
			if (0 == m_bySpecMixMember[byIdx])
			{
				byIdx++;
				continue;
			}
			byPOutNum++;
			OspPrintf(TRUE, FALSE, " [%d] ",m_bySpecMixMember[byIdx]);
			if ((byPOutNum%10) == 0)
			{
				OspPrintf(TRUE, FALSE, "\n");
			}
			byIdx++;
		}
		
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//����ǽ�໭��ͨ��ģ��Ԥ�������������ǽ������(len:15303bytes)
struct THduVmpModuleOrTHDTvWall
{
public:
	u8 m_byUnionType;	//��ʶ������UHduModuleUnion���ô洢THDTvWall�ṹ����THduVmpChannlInfo����
			  			    //1��ʾTHDTvWall��2��ʾTHduVmpChannlInfo����
	union UHduModuleUnion
	{
		THDTvWall  m_tHDTvWall; //�洢VCS�������ǽģ����Ϣ, �Ѿ�����
		THduVmpChannlInfo m_atHduVmpChnInfo[(HDUID_MAX-HDUID_MIN+1)*MAXNUM_HDU_CHANNEL]; //����ǽ�໭��ͨ����Ϣ(6020 byte)
	}m_union;
public:
	THduVmpModuleOrTHDTvWall ( void )
	{
		Clear();
	}

	void Clear( void )
	{
		memset(this,0,sizeof(THduVmpModuleOrTHDTvWall));
	}

	//�ж��������Ƿ��ŵ���HDU�໭��ͨ����Ϣ
	BOOL32 IsUnionStoreHduVmpInfo( void ) const
	{
		return (m_byUnionType==2);
	}

	//�ж��������Ƿ��ŵ���HDTvwall��Ϣ
	BOOL32 IsUnionStoreHDTvwall( void ) const
	{
		return (m_byUnionType==1);
	}

	/*==============================================================================
	������    :  SetHduVmpChnnlInfo
	����      :  ����HDU�໭��ͨ����Ϣ
	�㷨ʵ��  :  
	����˵��  :  THduVmpChannlInfo *ptVmpChnInfo[IN]�໭��ͨ�������׵�ַ
				 u8 byChnnlNum[IN] ͨ����Ŀ
	����ֵ˵��:  �ɹ�����TRUE������FALSE
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��       �汾          �޸���          �߶���          �޸ļ�¼
	2013-3-8	 4.7.2         chendaiwei						create        
	==============================================================================*/
	BOOL32 SetHduVmpChnnlInfo( THduVmpChannlInfo *ptVmpChnInfo, u8 byChnnlNum)
	{
		BOOL32 bResult = FALSE;	
		
		if ( ptVmpChnInfo == NULL || byChnnlNum == 0 || byChnnlNum > (HDUID_MAX-HDUID_MIN+1)*MAXNUM_HDU_CHANNEL )
		{ 
			StaticLog("[SetVmpChnnlInfo] invalid param!\n");
		}
		else
		{
			m_byUnionType = 2;
			memset(&m_union,0,sizeof(m_union));
			memcpy(&m_union.m_atHduVmpChnInfo[0],ptVmpChnInfo,sizeof(THduVmpChannlInfo)*byChnnlNum);

			bResult = TRUE;
		}

		return bResult;
	}

	/*==============================================================================
	������    :  SetHduVmpSubChnlMember
	����      :  ����HDU�໭����ͨ����ԱΪmtAliasidx
	�㷨ʵ��  :  
	����˵��  :  u8 byLoopChIdx ͨ�������±�
				 u8 bySubChIdx  ��ͨ�������±�
				 u8 byMtaliasIdx �ն˱��������±�
	����ֵ˵��:  �ɹ�����TRUE������FALSE
	-------------------------------------------------------------------------------
	�޸ļ�¼  :  
	��  ��       �汾          �޸���          �߶���          �޸ļ�¼
	2013-3-8	 4.7.2         chendaiwei						create        
	==============================================================================*/
	BOOL32 SetHduVmpSubChnlMember(u8 byLoopChIdx, u8 bySubChIdx, u8 byMtAliasIdx)
	{
		if(byLoopChIdx >= (HDUID_MAX-HDUID_MIN+1)*MAXNUM_HDU_CHANNEL 
			|| bySubChIdx >=HDU_MODEFOUR_MAX_SUBCHNNUM
			|| byMtAliasIdx > MAXNUM_CONF_MT)
		{
			return FALSE;
		}
		else
		{
			m_union.m_atHduVmpChnInfo[byLoopChIdx].m_abyTvWallMember[bySubChIdx] = byMtAliasIdx;

			return TRUE;
		}
	}

	void Print ( void )
	{
		if( m_byUnionType == 2)
		{
			for( u8 byIdx = 0; byIdx <(HDUID_MAX-HDUID_MIN+1)*MAXNUM_HDU_CHANNEL; byIdx++)
			{
				m_union.m_atHduVmpChnInfo[byIdx].Print();
			}
		}
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// wy[2014/12/10] �ýṹ��Ų��mcustruct.h��
// ����ϳ�ģ����Ϣ(len: 58)
// struct TVmpModuleInfo : public TVmpModule
// {
// public:
// /*	u8	m_byVMPAuto;      //�Ƿ����Զ�����ϳ� 0-�� 1-��
// 	u8	m_byVMPBrdst;     //�ϳ�ͼ���Ƿ����ն˹㲥 0-�� 1-�� 
// 	u8  m_byVMPStyle;     //����ϳɷ��,�μ�mcuconst.h�л���ϳɷ����
//     u8  m_byVMPSchemeId;  //�ϳɷ�񷽰����,���֧��5�׷���,1-5
//     u8  m_byVMPMode;      //ͼ�񸴺Ϸ�ʽ: 0-��ͼ�񸴺� 1-��ػ���ϯ����ͼ�񸴺� 2-�Զ�ͼ�񸴺�(��̬���������ó�Ա)
// 	u8  m_byRimEnabled;	  //�Ƿ�ʹ�ñ߿�: 0-��ʹ��(Ĭ��) 1-ʹ��;
// 	u8	m_byVMPBatchPoll; // xliang [12/18/2008] �Ƿ���������ѯ 0-��
// 	u8	m_byVMPSeeByChairman;	// �Ƿ���ϯMTѡ��	0-��
// 
// 	u8  m_abyVmpMember[MAXNUM_MPU2VMP_MEMBER];        //���ָ���Ļ���ϳɳ�Ա������������ʱTMtAlias��������+1��
//     u8  m_abyMemberType[MAXNUM_MPU2VMP_MEMBER];       //����ϳɳ�Ա�ĸ��淽ʽ, VMP_MEMBERTYPE_MCSSPEC, VMP_MEMBERTYPE_SPEAKER...
// */
// 	TMsgVmpCfgInfo m_tVmpCfgInfo;					  // ����ϳ�������Ϣ TMsgVmpCfgInfo��ͬ��TVmpStyleCfgInfo+TVMPExCfgInfo
// public:
// 	TVmpModuleInfo ( void )
// 	{
// 		Clear();
// 	}
// 
// 	~TVmpModuleInfo( void )
// 	{
// 		Clear();
// 	}
// 
// 	void Clear ( void )
// 	{
// 		memset(this,0,sizeof(*this));
// 	}
// 
// 	/*====================================================================
// 	������      ��SetVmpChnnlMember
// 	����        ������vmpģ���Ա����
// 	�㷨ʵ��    ��
// 	����ȫ�ֱ�����
// 	�������˵����[IN]byNum �����TVmpChnnlMember��Ŀ��
// 				  [IN]ptMember �����TVmpChnnlMember�����׵�ַ 
// 	����ֵ˵��  ������ɹ�����TRUE�����򷵻�FALSE
// 	----------------------------------------------------------------------
// 	�޸ļ�¼    ��
// 	��  ��      �汾        �޸���        �޸�����
// 	2013/03/15  4.0         chendaiwei       ����
// 	====================================================================*/
// 	BOOL32 SetVmpChnnlMember ( TVmpChnnlMember * ptMember, u8 byNum)
// 	{
// 		BOOL32 bResult = FALSE;	
// 		
// 		if ( ptMember == NULL || byNum == 0 || byNum > (MAXNUM_MPU2VMP_MEMBER-MAXNUM_MPUSVMP_MEMBER) )
// 		{ 
// 			StaticLog("[SetVmpChnnlMember] invalid param!\n");
// 		}
// 		else
// 		{
// 			for( u8 byIdx = 0; byIdx < byNum;  byIdx++ )
// 			{
// 				if(ptMember[byIdx].m_byChnIdx < MAXNUM_MPU2VMP_MEMBER)
// 				{
// 					m_abyVmpMember[ptMember[byIdx].m_byChnIdx] = ptMember[byIdx].m_byVmpMember;
// 					m_abyMemberType[ptMember[byIdx].m_byChnIdx] = ptMember[byIdx].m_byMemberType;
// 				}
// 			}
// 
// 			bResult = TRUE;
// 		}
// 
// 		return bResult;
// 	}
// 		
// 	void EmptyAllVmpMember ( void )
// 	{
// 		for( u8 byIdx = 0; byIdx < sizeof(m_abyVmpMember);byIdx++ )
// 		{
// 			//�Ǹ�������
// 			if(m_abyVmpMember[byIdx] != MAXNUM_CONF_MT+1)
// 			{
// 				m_abyVmpMember[byIdx] = 0;
// 			}
// 		}
// 	}
// }
// #ifndef WIN32
// __attribute__ ( (packed) ) 
// #endif
// ;


// VCS����ģ��
struct THDUModule 
{
public:
	THDUModule()
	{
		SetNull();
	}

	void SetNull() 
	{
		m_byHduChnlNum = 0;
		EmptyHduChnlInfo();
	}

	void EmptyHduChnlInfo()
	{
		for (u8 byIdx = 0; byIdx < MAXNUM_CONF_HDUBRD * MAXNUM_HDU_CHANNEL; byIdx++)
		{
			m_atHduChnlInfo[byIdx].SetNull();
		}
	}

	void   SetHduChnlNum(u8 byHduChnlNum)       { m_byHduChnlNum = byHduChnlNum; }
	
	u8     GetHduChnlNum( const BOOL32 bVcsConf ) const 
	{
		if (!bVcsConf)
		{
			return m_byHduChnlNum;
		}else
		{
			u8 byVcsChnnlNum = 0;
			for ( u8 byLoop = 0; byLoop < m_byHduChnlNum && byLoop < (MAXNUM_CONF_HDUBRD * MAXNUM_HDU_CHANNEL); byLoop++ )
			{
				if( m_atHduChnlInfo[byLoop].GetMemberType() == TW_MEMBERTYPE_VCSAUTOSPEC )
				{
					byVcsChnnlNum++;
				}
			}
			
			return byVcsChnnlNum;
		}		
	}

	void   SetHduModuleInfo(u8 byHduChnlNum, const s8* pbyHduChnlInfo)
	{
		if (byHduChnlNum >= MAXNUM_HDUBRD * MAXNUM_CONF_HDUBRD)
		{			
			return;
		}

		EmptyHduChnlInfo();
		m_byHduChnlNum = byHduChnlNum;
		memcpy(m_atHduChnlInfo, pbyHduChnlInfo, byHduChnlNum * sizeof(THduModChnlInfo));
	}
	u8*   GetHduModuleInfo()  { return (u8*)m_atHduChnlInfo; }
	const THduModChnlInfo& GetOneHduChnlInfo(u8 byChnlIdx) { return m_atHduChnlInfo[byChnlIdx]; }

private:
	u8                m_byHduChnlNum;                                             //���ø�����ĵ���ǽͨ������
    THduModChnlInfo   m_atHduChnlInfo[MAXNUM_CONF_HDUBRD * MAXNUM_HDU_CHANNEL];        //����ͨ����Ϣ
	
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//�˿�������Ϣ
struct TPortDebugInfo
{
	u16  m_wMpStartPortForMt;    //�ն˽���˿�
	u16  m_wMpStartPortForEqp;   //�������˿�
	u16  m_wSpyStartPort;        //��ش���ʼ�˿�
	u16  m_wBridgeStartPort;     //����ʼ�˿�
	u16  m_wPrsStartPort;        //prs��ʼ�˿�
	u16  m_wConfBrdStartPort;    //�㲥ƽ����ʼ�˿�

	void Clear()
	{
		m_wMpStartPortForMt  = 0;
		m_wMpStartPortForEqp = 0;
		m_wSpyStartPort      = 0;
		m_wBridgeStartPort   = 0;
		m_wPrsStartPort      = 0;
		m_wConfBrdStartPort  = 0;
	}

	void SetMpStartPortForMt( u16 wMpStartPort )  { m_wMpStartPortForMt = wMpStartPort; }
	u16  GetMpStartPortForMt(void) { return m_wMpStartPortForMt; }
	void SetMpStartPortForEqp( u16 wMpStartPortForEqp )  { m_wMpStartPortForEqp = wMpStartPortForEqp; }
	u16  GetMpStartPortForEqp(void) { return m_wMpStartPortForEqp; }
	void SetSpyStartPort( u16 wSpyStartPort )  { m_wSpyStartPort = wSpyStartPort; }
	u16  GetSpyStartPort(void) { return m_wSpyStartPort; }
	void SetBridgeStartPort( u16 wBridgeStartPort )  { m_wBridgeStartPort = wBridgeStartPort; }
	u16  GetBridgeStartPort(void) { return m_wBridgeStartPort; }
	void SetPrsStartPort( u16 wPrsStartPort )  { m_wPrsStartPort = wPrsStartPort; }
	u16  GetPrsStartPort(void) { return m_wPrsStartPort; }
	void SetConfBrdStartPort( u16 wConfBrdStartPort )  { m_wConfBrdStartPort = wConfBrdStartPort; }
	u16  GetConfBrdStartPort(void) { return m_wConfBrdStartPort; }

	BOOL32 IsPortInfoValid()
	{
		if ( ( 0 == m_wMpStartPortForMt) || ( 0 == m_wMpStartPortForEqp) 
			|| ( 0 == m_wPrsStartPort) || ( 0 == m_wBridgeStartPort)
			|| ( 0 == m_wSpyStartPort) || ( 0 == m_wConfBrdStartPort)
			|| (m_wMpStartPortForMt > m_wMpStartPortForEqp) || (m_wMpStartPortForEqp > m_wSpyStartPort)
			|| (m_wSpyStartPort > m_wBridgeStartPort) || (m_wBridgeStartPort > m_wPrsStartPort)
			|| (m_wPrsStartPort > m_wConfBrdStartPort))
		{
			return FALSE;
		}
		return TRUE;
	}

	void SetDefaultPortToAll()
	{
		m_wMpStartPortForMt  = MT_MCU_STARTPORT;
		m_wMpStartPortForEqp = MEDIA_EQP_STARTPORT;
		m_wSpyStartPort      = CASCADE_SPY_STARTPORT;
		m_wBridgeStartPort   = SWITCH_BRG_STARTPORT;
		m_wPrsStartPort      = PRS_EQP_STARTPORT;
		m_wConfBrdStartPort  = RESV_MCU_STARTPORT/*CONFBRD_STARTPORT*/;    //CONFBRD_STARTPORT���������mpģ���mp.h��
	}

	void Print() const
	{
		StaticLog("PortDebugInfo Information:\n");
		StaticLog("MpStartPortForMt = %d\n", m_wMpStartPortForMt);
		StaticLog("MpStartPortForEqp = %d\n", m_wMpStartPortForEqp);
		StaticLog("SpyStartPort = %d\n", m_wSpyStartPort);
		StaticLog("BridgeStartPort = %d\n", m_wBridgeStartPort);
		StaticLog("PrsStartPort = %d\n", m_wPrsStartPort);
		StaticLog("ConfBrdStartPort = %d\n", m_wConfBrdStartPort);
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//�����ն����ݶ˿���Ϣ
struct TRecvMtPort
{
	u8  m_byPortInUse;
    u8  m_byConfIdx;
	u8  m_byMtId;
	u8	m_byMode;
	u8  m_byChnnlIdx;
	u8	m_byIsRtp;
	
	BOOL32 IsInUse()
	{
		return ((m_byPortInUse & 1) == 1);
	}
	
	void SetInUse( BOOL32 bIsInUse )
	{
		if( bIsInUse )
		{
			m_byPortInUse |= 1;
		}
		else
		{
			m_byPortInUse &= 0xfe;
		}		
	}
	
	BOOL32 IsApplyByMcuMedia()
	{
		return ((m_byPortInUse & 2) >> 1) == 1;
	}
	
	void SetIsApplyByMcuMedia( BOOL32 bIsApplyByMcuMedia )
	{
		if( bIsApplyByMcuMedia )
		{
			m_byPortInUse |= 2;
		}
		else
		{
			m_byPortInUse &= 0xfd;
		}	
	}
	void Print()
	{
		StaticLog( "Confidx.%d Mt.%d Mode.%d IsRtp.%d mediamcu.%d\n",
				m_byConfIdx, m_byMtId, m_byMode, m_byIsRtp, IsApplyByMcuMedia() );
	}
};
enum EnumMediaPreiType
{ 
	MEDIAVMP = 1,		
		MEDIAMIXER,
		MEDIABAS,
		MEDIAPRS,
		MEDIAHDU
		
};

struct TMediaPortMgr
{
	u32 dwHandle;		//������
	u8  byType;			//��������
	u16 wNextIdx;		//ͬ�����裬�¸�������Ķ˿�����
	u8  wNextIdxOwer;	//�¸��������������ڵĶ�(��01���ξ���0����2���ξ���1)
	u8  byApplyByMediaMcu;//�Ƿ��Ƕ˿�mcu�����
	u8  byConfIdx;		//����Idx

	void Clear()
	{
		byType				= 0;
		wNextIdx			= 0;
		wNextIdxOwer		= 0;
		byApplyByMediaMcu	= 0;
		byConfIdx			= 0;
		dwHandle			= INVALID_HMEDIARES;
	}

	void Print()
	{
		StaticLog( "ConfIdx.%d Handle.0x%x NextPortIdx.%d NextIdxOwer.%d Type:",
					byConfIdx, dwHandle,wNextIdx,wNextIdxOwer );
		switch( byType )
		{
		case MEDIAVMP:
			StaticLog( "VMP\n" );
			break;
		case MEDIAMIXER:
			StaticLog( "MIXER\n" );
			break;
		case MEDIABAS:
			StaticLog( "BAS\n" );
			break;
		case MEDIAPRS:
			StaticLog( "PRS\n" );
			break;
		case MEDIAHDU:
			StaticLog( "HDU\n" );
			break;
		default:
			StaticLog( "UNKNOW.%d\n",byType );
			break;
		}
	}
};


//�ϳ����������rtp��rtcp�˿ڷ���
struct TMediaRecvPort
{
	
	TMediaPortMgr dwRtpPort1[CASCADE_SPY_STARTPORT - MEDIA_EQP_STARTPORT];  // ���� 45500-56999(vmp7+mix196+vbas6+abas4+dsbas4 = 229*44 =10076)
// 	TMediaPortMgr dwRtpPort1[PROXY_STARTPORT - CASCADE_SPY_ENDPORT];  //48000 - 38000
// 	TMediaPortMgr dwRtpPort2[PRS_EQP_STARTPORT - PROXY_ENDPORT];   //58000 - 53000

	void Clear()
	{
		memset( &dwRtpPort1[0],0,sizeof(dwRtpPort1) );
// 		memset( &dwRtpPort2[0],0,sizeof(dwRtpPort2) );
		u16 wLoop = 0;
		u16 wCount = sizeof(dwRtpPort1) / sizeof(dwRtpPort1[0]);
		while( wLoop < wCount )
		{
			dwRtpPort1[wLoop].dwHandle = INVALID_HMEDIARES;
			++wLoop;
		}

// 		wLoop = 0;
// 		wCount = sizeof(dwRtpPort2) / sizeof(dwRtpPort2[0]);
// 		while( wLoop < wCount )
// 		{
// 			dwRtpPort2[wLoop].dwHandle = INVALID_HMEDIARES;
// 			++wLoop;
// 		}
		
	}
};

#if 0
//5.0 ����
struct TCommonPrsInfo
{
	u8		m_atPrsPort[MAXNUM_CONF_PRS_CHNNL/8 + 1];
	u8		m_atIsMediaMcuApply[MAXNUM_CONF_PRS_CHNNL/8 + 1];
	
	void Clear()
	{
		memset(m_atPrsPort, 0, sizeof(m_atPrsPort));
		memset(m_atIsMediaMcuApply, 0, sizeof(m_atIsMediaMcuApply));
	}
};
#endif

struct TConfNumInfo
{
	u8 byConfIdx;
	u8 byIsMediaMcuApply;
};

//����Pack�����Ļ���洢��Ϣ 
#ifdef WIN32
    #pragma pack( push )
    #pragma pack( 1 )
#endif

struct TPackConfStore
{
    TPackConfStore()
    {
        m_byMtNum = 0;
        m_wAliasBufLen = 0;
    }
    TConfInfo m_tConfInfo;
	u8        m_byMtNum;
	u16       m_wAliasBufLen;       //(������ �洢���ļ���) ����Packʱָ������� m_pbyAliasBuf �ֽڳ���
	//u8       *m_pbyAliasBuf;        //m_wAliasBufLen �ֽ�
	//TTvWallModule m_tTvWallModule;  //(��ѡ��ȡ���ڻ�������) 
	//TVmpModule    m_tVmpModule;     //(��ѡ��ȡ���ڻ�������) 
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

#ifdef WIN32
#pragma pack( pop )
#endif
//8*sizeof(u8)+8*sizeof(u16)+




// ������� [pengguofeng 5/15/2013]
#ifdef _UTF8
	#define CONFINFO_EX_BUFFER_LENGTH (sizeof(u16)+\
	sizeof(u8) + sizeof(u16) + sizeof(TVideoStreamCap)*MAX_CONF_CAP_EX_NUM+\
	sizeof(u8) + sizeof(u16) + sizeof(TVideoStreamCap)*MAX_CONF_CAP_EX_NUM+\
	sizeof(u8) + sizeof(u16) + MAXNUM_CONF_AUDIOTYPE*sizeof(TAudioTypeDesc))+\
	sizeof(u8) + sizeof(u16) + sizeof(THduVmpChnnl)*(HDUID_MAX-HDUID_MIN+1)*1+\
	sizeof(u8) + sizeof(u16) + sizeof(THduVmpSubChnnl)*(HDUID_MAX-HDUID_MIN+1)*1*HDU_MODEFOUR_MAX_SUBCHNNUM+\
	sizeof(u8) + sizeof(u16) + sizeof(TVmpChnnlMember)*(MAXNUM_MPU2VMP_MEMBER-MAXNUM_MPUSVMP_MEMBER)+\
	sizeof(u8) + sizeof(u16) + sizeof(u8)+\
	sizeof(u8) + sizeof(u16) + sizeof(TMsgVmpCfgInfo)+\
	sizeof(u8) + sizeof(u16) + sizeof(TMtAlias)+\
	sizeof(u8) + sizeof(u16) + sizeof(s8)*(MAXLEN_PWD+1)+\
	sizeof(u8) + sizeof(u16) + sizeof(s8)*(MAXLEN_MCS_USRNAME+1)+\
	sizeof(u8) + sizeof(u16) + sizeof(u16)
	//sizeof(u8) + sizeof(u16) + sizeof(TDurationDate)+zjj20140529 ԤԼ�������ɾ��
	//sizeof(u8) + sizeof(u16) + sizeof(TKdvTime)+
#else
#define CONFINFO_EX_BUFFER_LENGTH (sizeof(u16)+8*sizeof(u8)+8*sizeof(u16)+\
	sizeof(u8) + sizeof(u16) + sizeof(TVideoStreamCap)*MAX_CONF_CAP_EX_NUM+\
	sizeof(u8) + sizeof(u16) + sizeof(TVideoStreamCap)*MAX_CONF_CAP_EX_NUM+\
	sizeof(u8) + sizeof(u16) + MAXNUM_CONF_AUDIOTYPE*sizeof(TAudioTypeDesc))+\
	sizeof(u8) + sizeof(u16) + sizeof(THduVmpChnnl)*(HDUID_MAX-HDUID_MIN+1)*1+\
	sizeof(u8) + sizeof(u16) + sizeof(THduVmpSubChnnl)*(HDUID_MAX-HDUID_MIN+1)*1*HDU_MODEFOUR_MAX_SUBCHNNUM+\
	sizeof(u8) + sizeof(u16) + sizeof(TVmpChnnlMember)*(MAXNUM_MPU2VMP_MEMBER-MAXNUM_MPUSVMP_MEMBER)+\
	sizeof(u8) + sizeof(u16) + sizeof(s8)*(MAXLEN_MCS_USRNAME+1)
	//sizeof(u8) + sizeof(u16) + sizeof(TDurationDate)+zjj20140529 ԤԼ�������ɾ��
	//sizeof(u8) + sizeof(u16) + sizeof(TKdvTime)+
#endif
//!ע�⣺����CMcuVcData::GetVcDeamonTemplateData�������漰��С�жϣ��˴�����������

//�����µĽ�����ʽ������m_tConfInfoEx��Ϣ����������
//	ռ��u16(EX��Ϣ�ܴ�С)+u8(emMainStreamCapEx)+u16(emMainStreamCapEx�������ݴ�С)+sizoeof(TVideoStreamCap)*20
//  +u8(emDoubleStreamCapEx)+u16(emDoubleStreamCapEx�������ݴ�С)+sizoeof(TVideoStreamCap)*20
//  +u8(emMainAudioTypeDesc)+u16(emMainAudioTypeDesc�������ݴ�С)+sizeof(TAudioTypeDesc)
//  +u8(emHduVmpChnnl)+u16(emHduVmpChnnl�������ݴ�С)+sizeof(THduVmpChnnl)*140
//  +u8(emHduVmpSubChnnl)+u16(emHduVmpSubChnnl�������ݴ�С)+sizeof(THduVmpSubChnnl)*140*20 
//  +u8(emVmpChnnlMember)+u16(emVmpChnnlmember�������ݴ�С)+sizeof(TVmpChnnlMember)*5(len:18bytes)

//  +u8(emEncoding)     +u16(emEncoding���ʹ�С) ��������+sizeof(u8) (len:4bytes)[pengguofeng 4/15/2013]

//  +u8(emVmpCfgInfo)+u16(emVmpCfgInfo�������ݴ�С)+sizeof(TMsgVmpCfgInfo)//����ϳ�������Ϣ TMsgVmpCfgInfo��ͬ��TVmpStyleCfgInfo+TVMPExCfgInfo
//  +u8(emAutoRecAlias)+u16(emAutoRecAlias�������ݴ�С)+sizeof(TMtAlias)//ģ���Զ�¼��vrs������Ϣ
//  +u8(emMCSPassword)+u16(emMCSPassword�������ݴ�С)+sizeof(s8[MAXLEN_PWD+1])//ģ���Զ�¼��vrs������Ϣ
//	+u8(emConfStartUserName)+u16(emConfStartUserName�������ݴ�С)+sizeof(s8[MAXLEN_MCS_USRNAME+1])//����������û�����
//	+u8(emDefaultConfCallMtBitRate)+u16(emDefaultConfCallMtBitRate�������ݴ�С)+sizeof(u16)//Ĭ���ն˺�������

//����洢��Ϣ 
struct TConfStore : public TPackConfStore 
{
    TMtAlias  m_atMtAlias[MAXNUM_CONF_MT];
	u16       m_awMtDialBitRate[MAXNUM_CONF_MT];
    TMultiTvWallModule m_tMultiTvWallModule;
	TVmpModule m_atVmpModule;
	THDTvWall   m_tHDTWInfo;     // VCS �������ǽ������Ϣ
	TVCSSMCUCfg m_tVCSSMCUCfg;   // VCS ����ʵ���������Ϣ	
	THDUModule  m_tHduModule;     // HDU ����ǽ������Ϣ
	u8          m_byMTPackExist; // �Ƿ������˷��� 
	TMtAlias    m_tVCSBackupChairMan;//VCS������ַ����
	u8			m_byVCAutoMode;		//�Զ�����ĳģʽ
	u8			m_byConInfoExBuf[CONFINFO_EX_BUFFER_LENGTH];	
public:
    
	TConfStore ()
	{
		memset(m_byConInfoExBuf,0,sizeof(m_byConInfoExBuf));
	}

    void EmptyAllTvMember(void)
    {
        for(u8 byTvLp = 0; byTvLp < MAXNUM_PERIEQP_CHNNL; byTvLp++)
        {
            m_tMultiTvWallModule.m_atTvWallModule[byTvLp].EmptyTvMember();
        }    
    }

    void EmptyAllVmpMember(void)
    {
		for( u8 byIdx = 0; byIdx < sizeof(m_atVmpModule.m_abyVmpMember);byIdx++ )
		{
			//�Ǹ�������
			if(m_atVmpModule.m_abyVmpMember[byIdx] != MAXNUM_CONF_MT+1)
			{
				m_atVmpModule.m_abyVmpMember[byIdx] = 0;
			}
		}
    }

    void SetMtInTvChannel(u8 byTvId, u8 byChannel, u8 byMtIdx, u8 byMemberType)
    {
        for(u8 byTvLp = 0; byTvLp < MAXNUM_PERIEQP_CHNNL; byTvLp++)
        {
            if(m_tMultiTvWallModule.m_atTvWallModule[byTvLp].m_tTvWall.GetEqpId() == byTvId)
            {
				//zjl[20091208]����ԭ�����õ�SetTvChannelͨ��������byTvLp��ΪbyChannel
                m_tMultiTvWallModule.m_atTvWallModule[byTvLp].SetTvChannel(byChannel, byMtIdx, byMemberType);
                break;
            }
        }
    }
    
	// �Ƿ������˸������ǽ
	BOOL32 IsHDTWCfg()   { return m_tHDTWInfo.IsHDTWCfg(); }
	// �Ƿ�������VCS�¼�mcu
	BOOL32 IsVCSMCUCfg() { return m_tVCSSMCUCfg.IsVCSMCUCfg(); }

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//����ģ����Ϣ
struct TTemplateInfo : public TConfStore
{
    u8   m_byConfIdx;           //����id

public:
    TTemplateInfo(void) { Clear(); }
    void Clear(void) { memset(this, 0, sizeof(TTemplateInfo)); }

    BOOL32 IsEmpty(void) const { return (0 == m_byConfIdx); }
}
;

struct TConfMapData
{
protected:
    u8  m_byTemIndex;           //ģ����������
    u8  m_byInsId;              //��ʱ����ʵ��id   
    
public:
    TConfMapData(void) { Clear(); }
    void Clear(void) 
    {
        m_byTemIndex = MAXNUM_MCU_TEMPLATE;
        m_byInsId = 0;
    }

    void   SetInsId(u8 byInsId) { m_byInsId = byInsId; }
    u8     GetInsId(void)  { return m_byInsId; }
    void   SetTemIndex(u8 byIndex) { m_byTemIndex = byIndex; }
    u8     GetTemIndex(void) { return m_byTemIndex; }
    
    BOOL32 IsValidConf(void) { return (m_byInsId>=MIN_CONFIDX && m_byInsId <=MAXNUM_MCU_CONF); }//�Ƿ�Ϊռ�û���ʵ������Ч���� ��ʱ��ԤԼ
    BOOL32 IsTemUsed(void) { return (m_byTemIndex < MAXNUM_MCU_TEMPLATE); }
};


typedef u32 PairId;

struct TVcMtMediaH
{
	u32				m_dwPairID;									//ý��󶨹�ϵID��ʶ
public:
	TVcMtMediaH()
	{
		memset(this , 0 , sizeof(TVcMtMediaH));
	}
};

struct TVidSndChnStatus : public TChnStatus
{
private:
	u32 m_dwLastRefreshTick;

public:
	TVidSndChnStatus() : m_dwLastRefreshTick(0)
	{
		;
	}
	TVidSndChnStatus(const TVidSndChnStatus& tChn) : TChnStatus( tChn )
	{
		SetLastRefreshTick( tChn.GetLastRefreshTick() );
	}

	u32 GetLastRefreshTick(void) const { return m_dwLastRefreshTick; }
	void SetLastRefreshTick(u32 dwTick) { m_dwLastRefreshTick = dwTick; }
};

struct TVidRcvChnStatus : public TChnStatus
{
private:
	u8 m_byIsSelByMcsDrag;
	//��ǰ���յ���ƵԴ
	TVcMtChnObj m_tVideoSrc;
	// ѡ����������	
	TVcMtChnObj m_tSelectMt;
    // �󶨵Ľ�����
    TVcMtMediaH m_tMediaH;
public:
	TVidRcvChnStatus() : m_byIsSelByMcsDrag(0)
	{
		;
	}
	TVidRcvChnStatus(const TVidRcvChnStatus& tChn) : TChnStatus( tChn )
	{	
		SetIsSelbyMcsDrag( tChn.IsSelByMcsDrag() );
		SetVidSrc( tChn.GetVidSrc() );
		SetSelectMt( tChn.GetSelectMt() );
		SetMediaH( tChn.GetMediaH() );
	}

public:
	BOOL32 IsSelByMcsDrag(void) const { return ( 0 == m_byIsSelByMcsDrag ) ? FALSE : TRUE; }
	void SetIsSelbyMcsDrag(BOOL32 bIsSel) { m_byIsSelByMcsDrag = bIsSel ? 1 : 0; }

	TVcMtChnObj GetVidSrc(void) const { return m_tVideoSrc; }
	void SetVidSrc(const TVcMtChnObj tVidSrc) { m_tVideoSrc = tVidSrc; }
	
	TVcMtChnObj GetSelectMt(void) const { return m_tSelectMt; }
	void SetSelectMt(const TVcMtChnObj tSelectMt) { m_tSelectMt = tSelectMt; }
	
	TVcMtMediaH GetMediaH(void) const { return m_tMediaH; }
	void SetMediaH(const TVcMtMediaH tMediaH) { m_tMediaH = tMediaH; }
};

struct TDVidSndChnStatus : public TChnStatus
{
private:
	u32 m_dwLastRefreshTick;
	
public:
	TDVidSndChnStatus() : m_dwLastRefreshTick(0)
	{
		;
	}
	TDVidSndChnStatus(const TDVidSndChnStatus& tChn) : TChnStatus( tChn )
	{	
		SetLastRefreshTick( tChn.GetLastRefreshTick() );
	}
	
	u32 GetLastRefreshTick(void) const { return m_dwLastRefreshTick; }
	void SetLastRefreshTick(u32 dwTick) { m_dwLastRefreshTick = dwTick; }
};

struct TDVidRcvChnStatus : public TChnStatus
{
private:
	u8 m_byIsSelByMcsDrag;
	//��ǰ���յ�˫��Դ;
	TVcMtChnObj m_tDVideoSrc; 
	// ѡ����������
	TVcMtChnObj m_tSelectMt; 
	// �����԰󶨹�ϵ
    TVcMtMediaH m_tMediaH;
public:
	TDVidRcvChnStatus() : m_byIsSelByMcsDrag(0)
	{
		;
	}

	TDVidRcvChnStatus(const TDVidRcvChnStatus& tChn) : TChnStatus( tChn )
	{
		SetIsSelbyMcsDrag( tChn.IsSelByMcsDrag() );
		SetDVidSrc( tChn.GetDVidSrc() );
		SetSelectMt( tChn.GetSelectMt() );
		SetMediaH( tChn.GetMediaH() );
	}
	
public:
	BOOL32 IsSelByMcsDrag(void) const { return ( 0 == m_byIsSelByMcsDrag ) ? FALSE : TRUE; }
	void SetIsSelbyMcsDrag(BOOL32 bIsSel) { m_byIsSelByMcsDrag = bIsSel ? 1 : 0; }
	
	TVcMtChnObj GetDVidSrc(void) const { return m_tDVideoSrc; }
	void SetDVidSrc(const TVcMtChnObj tDVidSrc) { m_tDVideoSrc = tDVidSrc; }
	
	TVcMtChnObj GetSelectMt(void) const { return m_tSelectMt; }
	void SetSelectMt(const TVcMtChnObj tSelectMt) { m_tSelectMt = tSelectMt; }
	
	TVcMtMediaH GetMediaH(void) const { return m_tMediaH; }
	void SetMediaH(const TVcMtMediaH tMediaH) { m_tMediaH = tMediaH; }
};

struct TAudSndChnStatus : public TChnStatus
{
private:
	u8 m_byIsMtManualInMix;//�Զ�������
	u8 m_byIsMute; //����
public:
	TAudSndChnStatus() : m_byIsMtManualInMix(0), m_byIsMute(0)
	{
		;
	}

	TAudSndChnStatus(const TAudSndChnStatus& tChn) : TChnStatus( tChn )
	{
		SetIsMtManuInMix( tChn.IsMtManuInMix() );
		SetIsMute( tChn.IsMute() );
	}

public:
	BOOL32 IsMtManuInMix(void) const { return ( 0 == m_byIsMtManualInMix ) ? FALSE : TRUE; }
	void SetIsMtManuInMix(BOOL32 bIsManuInMix) { m_byIsMtManualInMix = bIsManuInMix ? 1 : 0; }

	BOOL32 IsMute(void) const { return ( 0 == m_byIsMute ) ? FALSE : TRUE; }
	void SetIsMute(BOOL32 bIsMute) { m_byIsMute = bIsMute ? 1 : 0; }
};

struct TDVAudSndChnStatus : public TChnStatus
{
private:
	u8 m_byIsMute; //����
	u8 m_byBindedDVChnIdx;
public:
	TDVAudSndChnStatus() : m_byIsMute(0), m_byBindedDVChnIdx(INVALID_CHNNL)
	{
		;
	}

	TDVAudSndChnStatus(const TDVAudSndChnStatus& tChn) : TChnStatus( tChn )
	{
		SetBindedDVChnIdx( tChn.GetBindedDVChnIdx() );
		SetIsMute( tChn.IsMute() );
	}
	
public:
	BOOL32 IsMute(void) const { return ( 0 == m_byIsMute ) ? FALSE : TRUE; }
	void SetIsMute(BOOL32 bIsMute) { m_byIsMute = bIsMute ? 1 : 0; }

	u8 GetBindedDVChnIdx(void) const { return m_byBindedDVChnIdx; }
	void SetBindedDVChnIdx(u8 byBindedDVChnIdx) { m_byBindedDVChnIdx = byBindedDVChnIdx; }
};

struct TAudRcvChnStatus : public TChnStatus
{
private:
	u8 m_byIsSelByMcsDrag;
	//��ǰ���յ���ƵԴ
	TVcMtChnObj m_tAudioSrc;
	// ѡ����������	
	TVcMtChnObj m_tSelectMt;
    // �󶨵Ľ�����
    TVcMtMediaH m_tMediaH;
	u8 m_byIsDeaf; // ����

public:
	TAudRcvChnStatus() : m_byIsSelByMcsDrag(0), m_byIsDeaf(0)
	{
		;
	}

	TAudRcvChnStatus(const TAudRcvChnStatus& tChn) : TChnStatus( tChn )
	{
		SetIsSelbyMcsDrag( tChn.IsSelByMcsDrag() );
		SetAudSrc( tChn.GetAudSrc() );
		SetSelectMt( tChn.GetSelectMt() );
		SetMediaH( tChn.GetMediaH() );
		SetIsDeaf( tChn.IsDeaf() );
	}

public:
	BOOL32 IsSelByMcsDrag(void) const { return ( 0 == m_byIsSelByMcsDrag ) ? FALSE : TRUE; }
	void SetIsSelbyMcsDrag(BOOL32 bIsSel) { m_byIsSelByMcsDrag = bIsSel ? 1 : 0; }

	TVcMtChnObj GetAudSrc(void) const { return m_tAudioSrc; }
	void SetAudSrc(const TVcMtChnObj tAudSrc) { m_tAudioSrc = tAudSrc; }

	TVcMtChnObj GetSelectMt(void) const { return m_tSelectMt; }
	void SetSelectMt(const TVcMtChnObj tSelectMt) { m_tSelectMt = tSelectMt; }

	TVcMtMediaH GetMediaH(void) const { return m_tMediaH; }
	void SetMediaH(const TVcMtMediaH tMediaH) { m_tMediaH = tMediaH; }

	BOOL32 IsDeaf(void) const { return ( 0 == m_byIsDeaf ) ? FALSE : TRUE; }
	void SetIsDeaf(BOOL32 bIsDeaf) { m_byIsDeaf = bIsDeaf ? 1 : 0; }
};

struct TDVAudRcvChnStatus : public TChnStatus
{
private:
	u8 m_byIsDeaf; // ����
	u8 m_byBindedDVChnIdx;
public:
	TDVAudRcvChnStatus() : m_byIsDeaf(0), m_byBindedDVChnIdx(INVALID_CHNNL)
	{
		;
	}
	TDVAudRcvChnStatus(const TDVAudRcvChnStatus& tChn) : TChnStatus( tChn )
	{
		SetBindedDVChnIdx( tChn.GetBindedDVChnIdx() );
		SetIsDeaf( tChn.IsDeaf() );
	}
	
public:
	BOOL32 IsDeaf(void) const { return ( 0 == m_byIsDeaf ) ? FALSE : TRUE; }
	void SetIsDeaf(BOOL32 bIsDeaf) { m_byIsDeaf = bIsDeaf ? 1 : 0; }

	u8 GetBindedDVChnIdx(void) const { return m_byBindedDVChnIdx; }
	void SetBindedDVChnIdx(u8 byBindedDVChnIdx) { m_byBindedDVChnIdx = byBindedDVChnIdx; }
};

//�ն�״̬�ṹ(len:64)
struct TVcMtStatus
{
protected:
	//��������Ƶ����ͨ��
	u8 m_byTopPRIVidSndChnIdx;
	//�ն˸�����/����ͨ��״̬
	//Ϊͳһ��ͨ�ն˺ͼ������鴦�����ݽṹ
	//�޸�Ϊmap��keyΪͨ����
	std::map<u8, TVidSndChnStatus>   m_mapVidSndChnStatus;
	std::map<u8, TDVidSndChnStatus>  m_mapDVidSndChnStatus;
	std::map<u8, TAudSndChnStatus>   m_mapAudSndChnStatus;
	std::map<u8, TDVAudSndChnStatus>   m_mapDVAudSndChnStatus;
	std::map<u8, TVidRcvChnStatus>   m_mapVidRcvChnStatus;
	std::map<u8, TDVidRcvChnStatus>  m_mapDVidRcvChnStatus;
    std::map<u8, TAudRcvChnStatus>   m_mapAudRcvChnStatus;
	std::map<u8, TDVAudRcvChnStatus>   m_mapDVAudRcvChnStatus;

public:
	const TVidSndChnStatus * GetVidSndChn( const u8 byChnIdx ) const
	{
		std::map<u8, TVidSndChnStatus>::const_iterator iter;
		if ( !m_mapVidSndChnStatus.empty() && ( (iter = m_mapVidSndChnStatus.find( byChnIdx )) != m_mapVidSndChnStatus.end()) )
		{
			return &iter->second;
		}
		else
		{
			return NULL; 
		}
	}

	const TVidRcvChnStatus * GetVidRcvChn( const u8 byChnIdx ) const
	{
		std::map<u8, TVidRcvChnStatus>::const_iterator iter;
		if ( !m_mapVidRcvChnStatus.empty() && ( (iter = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end()) )
		{
			return &iter->second;
		}
		else
		{
			return NULL; 
		}
	}
	const TAudSndChnStatus * GetAudSndChn( const u8 byChnIdx ) const
	{
		std::map<u8, TAudSndChnStatus>::const_iterator iter;
		if ( !m_mapAudSndChnStatus.empty() && ( (iter = m_mapAudSndChnStatus.find( byChnIdx )) != m_mapAudSndChnStatus.end()) )
		{
			return &iter->second;
		}
		else
		{
			return NULL; 
		}
	}

	const TAudRcvChnStatus * GetAudRcvChn( const u8 byChnIdx ) const
	{
		std::map<u8, TAudRcvChnStatus>::const_iterator iter;
		if ( !m_mapAudRcvChnStatus.empty() && ( (iter = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end()) )
		{
			return &iter->second;
		}
		else
		{
			return NULL; 
		}
	}

	const TDVAudSndChnStatus * GetDVAudSndChn( const u8 byChnIdx ) const
	{
		std::map<u8, TDVAudSndChnStatus>::const_iterator iter;
		if ( !m_mapDVAudSndChnStatus.empty() && ( (iter = m_mapDVAudSndChnStatus.find( byChnIdx )) != m_mapDVAudSndChnStatus.end()) )
		{
			return &iter->second;
		}
		else
		{
			return NULL; 
		}
	}

	const TDVAudRcvChnStatus * GetDVAudRcvChn( const u8 byChnIdx ) const
	{
		std::map<u8, TDVAudRcvChnStatus>::const_iterator iter;
		if ( !m_mapDVAudRcvChnStatus.empty() && ( (iter = m_mapDVAudRcvChnStatus.find( byChnIdx )) != m_mapDVAudRcvChnStatus.end()) )
		{
			return &iter->second;
		}
		else
		{
			return NULL; 
		}
	}

	const TDVidSndChnStatus * GetDVidSndChn( const u8 byChnIdx ) const
	{
		std::map<u8, TDVidSndChnStatus>::const_iterator iter;
		if ( !m_mapDVidSndChnStatus.empty() && ( (iter = m_mapDVidSndChnStatus.find( byChnIdx )) != m_mapDVidSndChnStatus.end()) )
		{
			return &iter->second;
		}
		else
		{
			return NULL; 
		}
	}

	const TDVidRcvChnStatus* GetDVidRcvChn( const u8 byChnIdx ) const
	{
		std::map<u8, TDVidRcvChnStatus>::const_iterator iter;
		if ( !m_mapDVidRcvChnStatus.empty() && ( (iter = m_mapDVidRcvChnStatus.find( byChnIdx )) != m_mapDVidRcvChnStatus.end()) )
		{
			return &iter->second;
		}
		else
		{
			return NULL; 
		}
	}

public:
	TVcMtStatus( void )
	{
        Clear();
    }

	TVcMtStatus & operator =(const TVcMtStatus& tStatus)
	{
		SetMtStatus(tStatus);
		return (*this);
	}

	u32 GetChnNum( u8 byMode, BOOL32 bIsSend ) const
	{
		u32 dwNum = 0;
		switch( byMode )
		{
		case MODE_VIDEO:
			{
				if ( bIsSend )
				{
					dwNum = m_mapVidSndChnStatus.size();
				}
				else
				{
					dwNum = m_mapVidRcvChnStatus.size();
				}
			}
			break;
		case MODE_AUDIO:
			{
				if ( bIsSend )
				{
					dwNum = m_mapAudSndChnStatus.size();
				}
				else
				{
					dwNum = m_mapAudRcvChnStatus.size();
				}
			}
			break;
		case MODE_SECVIDEO:
			{
				if ( bIsSend )
				{
					dwNum = m_mapDVidSndChnStatus.size();
				}
				else
				{
					dwNum = m_mapDVidRcvChnStatus.size();
				}
			}
			break;
		case MODE_SECAUDIO:
			{
				if ( bIsSend )
				{
					dwNum = m_mapDVAudSndChnStatus.size();
				}
				else
				{
					dwNum = m_mapDVAudRcvChnStatus.size();
				}
			}
			break;
		default:
			break;
		}
		
		return dwNum;
	}
	//dwBufLen�����ã��������ýӿڣ����ȳ�ʼ��len
	BOOL32 GetChnIdxList( u8 byMode, BOOL32 bIsSend, /*OUT*/u8 * pbyBuf, /*OUT*/u32 &dwBufLen ) const
	{
		BOOL32 bSuccess = FALSE;
		if ( NULL == pbyBuf || 0 == dwBufLen )
		{
			return bSuccess;
		}
		u32 dwChnNum = 0;
		switch( byMode )
		{
		case MODE_VIDEO:
			{
				if ( bIsSend )
				{
					dwChnNum = m_mapVidSndChnStatus.size();
					if ( dwBufLen >= dwChnNum )
					{
						dwBufLen = 0;
						std::map<u8, TVidSndChnStatus>::const_iterator iter = m_mapVidSndChnStatus.begin();
						while( iter != m_mapVidSndChnStatus.end() )
						{
							*(pbyBuf+dwBufLen) = iter->second.GetChnIdx();
							dwBufLen++;
							iter++;
						}
						bSuccess = TRUE;
					}
				}
				else
				{
					dwChnNum = m_mapVidRcvChnStatus.size();
					if ( dwBufLen >= dwChnNum )
					{
						dwBufLen = 0;
						std::map<u8, TVidRcvChnStatus>::const_iterator iter = m_mapVidRcvChnStatus.begin();
						while( iter != m_mapVidRcvChnStatus.end() )
						{
							*(pbyBuf+dwBufLen) = iter->second.GetChnIdx();
							dwBufLen++;
							iter++;
						}
						bSuccess = TRUE;
					}
				}
			}
			break;
		case MODE_AUDIO:
			{
				if ( bIsSend )
				{
					dwChnNum = m_mapAudSndChnStatus.size();
					if ( dwBufLen >= dwChnNum )
					{
						dwBufLen = 0;
						std::map<u8, TAudSndChnStatus>::const_iterator iter = m_mapAudSndChnStatus.begin();
						while( iter != m_mapAudSndChnStatus.end() )
						{
							*(pbyBuf+dwBufLen) = iter->second.GetChnIdx();
							dwBufLen++;
							iter++;
						}
						bSuccess = TRUE;
					}
				}
				else
				{
					dwChnNum = m_mapAudRcvChnStatus.size();
					if ( dwBufLen >= dwChnNum )
					{
						dwBufLen = 0;
						std::map<u8, TAudRcvChnStatus>::const_iterator iter = m_mapAudRcvChnStatus.begin();
						while( iter != m_mapAudRcvChnStatus.end() )
						{
							*(pbyBuf+dwBufLen) = iter->second.GetChnIdx();
							dwBufLen++;
							iter++;
						}
						bSuccess = TRUE;
					}
				}
			}
			break;
		case MODE_SECVIDEO:
			{
				if ( bIsSend )
				{
					dwChnNum = m_mapDVidSndChnStatus.size();
					if ( dwBufLen >= dwChnNum )
					{
						dwBufLen = 0;
						std::map<u8, TDVidSndChnStatus>::const_iterator iter = m_mapDVidSndChnStatus.begin();
						while( iter != m_mapDVidSndChnStatus.end() )
						{
							*(pbyBuf+dwBufLen) = iter->second.GetChnIdx();
							dwBufLen++;
							iter++;
						}
						bSuccess = TRUE;
					}
				}
				else
				{
					dwChnNum = m_mapDVidRcvChnStatus.size();
					if ( dwBufLen >= dwChnNum )
					{
						dwBufLen = 0;
						std::map<u8, TDVidRcvChnStatus>::const_iterator iter = m_mapDVidRcvChnStatus.begin();
						while( iter != m_mapDVidRcvChnStatus.end() )
						{
							*(pbyBuf+dwBufLen) = iter->second.GetChnIdx();
							dwBufLen++;
							iter++;
						}
						bSuccess = TRUE;
					}
				}
			}
			break;
		case MODE_SECAUDIO:
			{
				if ( bIsSend )
				{
					dwChnNum = m_mapDVAudSndChnStatus.size();
					if ( dwBufLen >= dwChnNum )
					{
						dwBufLen = 0;
						std::map<u8, TDVAudSndChnStatus>::const_iterator iter = m_mapDVAudSndChnStatus.begin();
						while( iter != m_mapDVAudSndChnStatus.end() )
						{
							*(pbyBuf+dwBufLen) = iter->second.GetChnIdx();
							dwBufLen++;
							iter++;
						}
						bSuccess = TRUE;
					}
				}
				else
				{
					dwChnNum = m_mapDVAudRcvChnStatus.size();
					if ( dwBufLen >= dwChnNum )
					{
						dwBufLen = 0;
						std::map<u8, TDVAudRcvChnStatus>::const_iterator iter = m_mapDVAudRcvChnStatus.begin();
						while( iter != m_mapDVAudRcvChnStatus.end() )
						{
							*(pbyBuf+dwBufLen) = iter->second.GetChnIdx();
							dwBufLen++;
							iter++;
						}
						bSuccess = TRUE;
					}
				}
			}
			break;
		default:
			break;
		}
		
		return bSuccess;
	}

	void SetMtStatus( const TVcMtStatus& tStatus )
	{
		Clear();

		SetTopPRIVidSndChnIdx( tStatus.GetTopPRIVidSndChnIdx() );

		u32 dwChnNum = 0;	// ͨ����
		u32 dwLop = 0;
		u8 * pbyChnIdxList = NULL;	// ͨ�������б�ָ��

		// ��Ƶ����ͨ��
		dwChnNum = tStatus.GetChnNum( MODE_VIDEO, TRUE );
		if ( dwChnNum > 0 )
		{
			pbyChnIdxList = new u8[dwChnNum];
			if ( tStatus.GetChnIdxList( MODE_VIDEO, TRUE, pbyChnIdxList, dwChnNum ) )
			{	
				const TVidSndChnStatus * ptChn = NULL;
				for( dwLop = 0; dwLop < dwChnNum; dwLop++ )
				{
					ptChn = tStatus.GetVidSndChn( *(pbyChnIdxList+dwLop) );
					if ( NULL != ptChn )
					{
						AddVidSndChn( ptChn->GetChnIdx(), *ptChn );
					}
				}
			}
			delete [] pbyChnIdxList;
			pbyChnIdxList = NULL;
		}

		// ��Ƶ����ͨ��
		dwChnNum = tStatus.GetChnNum( MODE_AUDIO, TRUE );
		if ( dwChnNum > 0 )
		{
			pbyChnIdxList = new u8[dwChnNum];
			if ( tStatus.GetChnIdxList( MODE_AUDIO, TRUE, pbyChnIdxList, dwChnNum ) )
			{
				const TAudSndChnStatus * ptChn = NULL;
				for( dwLop = 0; dwLop < dwChnNum; dwLop++ )
				{
					ptChn = tStatus.GetAudSndChn( *(pbyChnIdxList+dwLop) );
					if ( NULL != ptChn )
					{
						AddAudSndChn( ptChn->GetChnIdx(), *ptChn );
					}
				}
			}
			delete [] pbyChnIdxList;
			pbyChnIdxList = NULL;
		}

		// ˫����Ƶ����ͨ��
		dwChnNum = tStatus.GetChnNum( MODE_SECVIDEO, TRUE );
		if ( dwChnNum > 0 )
		{
			pbyChnIdxList = new u8[dwChnNum];
			if ( tStatus.GetChnIdxList( MODE_SECVIDEO, TRUE, pbyChnIdxList, dwChnNum ) )
			{
				const TDVidSndChnStatus * ptChn = NULL;
				for( dwLop = 0; dwLop < dwChnNum; dwLop++ )
				{
					ptChn = tStatus.GetDVidSndChn( *(pbyChnIdxList+dwLop) );
					if ( NULL != ptChn )
					{
						AddDVidSndChn( ptChn->GetChnIdx(), *ptChn );
					}
				}
			}
			delete [] pbyChnIdxList;
			pbyChnIdxList = NULL;
		}

		// ˫����Ƶ����ͨ��
		dwChnNum = tStatus.GetChnNum( MODE_SECAUDIO, TRUE );
		if ( dwChnNum > 0 )
		{
			pbyChnIdxList = new u8[dwChnNum];
			if ( tStatus.GetChnIdxList( MODE_SECAUDIO, TRUE, pbyChnIdxList, dwChnNum ) )
			{
				const TDVAudSndChnStatus * ptChn = NULL;
				for( dwLop = 0; dwLop < dwChnNum; dwLop++ )
				{
					ptChn = tStatus.GetDVAudSndChn( *(pbyChnIdxList+dwLop) );
					if ( NULL != ptChn )
					{
						AddDVAudSndChn( ptChn->GetChnIdx(), *ptChn );
					}
				}
			}
			delete [] pbyChnIdxList;
			pbyChnIdxList = NULL;
		}

		// ��Ƶ����ͨ��
		dwChnNum = tStatus.GetChnNum( MODE_VIDEO, FALSE );
		if ( dwChnNum > 0 )
		{
			pbyChnIdxList = new u8[dwChnNum];
			if ( tStatus.GetChnIdxList( MODE_VIDEO, FALSE, pbyChnIdxList, dwChnNum ) )
			{
				const TVidRcvChnStatus * ptChn = NULL;
				for( dwLop = 0; dwLop < dwChnNum; dwLop++ )
				{
					ptChn = tStatus.GetVidRcvChn( *(pbyChnIdxList+dwLop) );
					if ( NULL != ptChn )
					{
						AddVidRcvChn( ptChn->GetChnIdx(), *ptChn );
					}
				}
			}
			delete [] pbyChnIdxList;
			pbyChnIdxList = NULL;
		}

		// ��Ƶ����ͨ��
		dwChnNum = tStatus.GetChnNum( MODE_AUDIO, FALSE );
		if ( dwChnNum > 0 )
		{
			pbyChnIdxList = new u8[dwChnNum];
			if ( tStatus.GetChnIdxList( MODE_AUDIO, FALSE, pbyChnIdxList, dwChnNum ) )
			{
				const TAudRcvChnStatus * ptChn = NULL;
				for( dwLop = 0; dwLop < dwChnNum; dwLop++ )
				{
					ptChn = tStatus.GetAudRcvChn( *(pbyChnIdxList+dwLop) );
					if ( NULL != ptChn )
					{
						AddAudRcvChn( ptChn->GetChnIdx(), *ptChn );
					}
				}
			}
			delete [] pbyChnIdxList;
			pbyChnIdxList = NULL;
		}
		
		// ˫����Ƶ����ͨ��
		dwChnNum = tStatus.GetChnNum( MODE_SECVIDEO, FALSE );
		if ( dwChnNum > 0 )
		{
			pbyChnIdxList = new u8[dwChnNum];
			if ( tStatus.GetChnIdxList( MODE_SECVIDEO, FALSE, pbyChnIdxList, dwChnNum ) )
			{
				const TDVidRcvChnStatus * ptChn = NULL;
				for( dwLop = 0; dwLop < dwChnNum; dwLop++ )
				{
					ptChn = tStatus.GetDVidRcvChn( *(pbyChnIdxList+dwLop) );
					if ( NULL != ptChn )
					{
						AddDVidRcvChn( ptChn->GetChnIdx(), *ptChn );
					}
				}
			}
			delete [] pbyChnIdxList;
			pbyChnIdxList = NULL;
		}
		
		// ˫����Ƶ����ͨ��
		dwChnNum = tStatus.GetChnNum( MODE_SECAUDIO, FALSE );
		if ( dwChnNum > 0 )
		{
			pbyChnIdxList = new u8[dwChnNum];
			if ( tStatus.GetChnIdxList( MODE_SECAUDIO, FALSE, pbyChnIdxList, dwChnNum ) )
			{
				const TDVAudRcvChnStatus * ptChn = NULL;
				for( dwLop = 0; dwLop < dwChnNum; dwLop++ )
				{
					ptChn = tStatus.GetDVAudRcvChn( *(pbyChnIdxList+dwLop) );
					if ( NULL != ptChn )
					{
						AddDVAudRcvChn( ptChn->GetChnIdx(), *ptChn );
					}
				}
			}
			delete [] pbyChnIdxList;
			pbyChnIdxList = NULL;
		}
	}

    void Clear()
    {
		SetTopPRIVidSndChnIdx(0);
		if ( !m_mapVidSndChnStatus.empty() )
		{
			m_mapVidSndChnStatus.clear( );
		}
		if ( !m_mapDVidSndChnStatus.empty() )
		{
			m_mapDVidSndChnStatus.clear( );
		}
		if ( !m_mapAudSndChnStatus.empty() )
		{
			m_mapAudSndChnStatus.clear( );
		}
		if ( !m_mapDVAudSndChnStatus.empty() )
		{
			m_mapDVAudSndChnStatus.clear( );
		}
		if ( !m_mapVidRcvChnStatus.empty() )
		{
			m_mapVidRcvChnStatus.clear( );
		}
		if ( !m_mapDVidRcvChnStatus.empty() )
		{
			m_mapDVidRcvChnStatus.clear( );
		}
		if ( !m_mapAudRcvChnStatus.empty() )
		{
			m_mapAudRcvChnStatus.clear( );
		}
		if ( !m_mapDVAudRcvChnStatus.empty() )
		{
			m_mapDVAudRcvChnStatus.clear( );
		}
	}

	void SetTopPRIVidSndChnIdx( u8 byChnIdx )
	{
		m_byTopPRIVidSndChnIdx = byChnIdx;
		return;
	}
	u8	GetTopPRIVidSndChnIdx( void ) const
	{
		return m_byTopPRIVidSndChnIdx;
	}

	BOOL32 AddVidSndChn( const u8 byChnIdx, const TVidSndChnStatus &tStatus )
	{
		if( byChnIdx != tStatus.GetChnIdx() )
		{
			return FALSE;
		}
		std::pair<std::map<u8, TVidSndChnStatus>::iterator, bool> Insert_Pair;
		Insert_Pair = m_mapVidSndChnStatus.insert( std::map<u8, TVidSndChnStatus>::value_type( byChnIdx, TVidSndChnStatus(tStatus) ) );
		return ( Insert_Pair.second == true ) ? TRUE : FALSE;
	}
	void RmvVidSndChn( const u8 byChnIdx ) 
	{
		m_mapVidSndChnStatus.erase( byChnIdx );
		return;
	}

	BOOL32 AddVidRcvChn( const u8 byChnIdx, const TVidRcvChnStatus &tStatus )
	{
		if( byChnIdx != tStatus.GetChnIdx() )
		{
			return FALSE;
		}
		std::pair<std::map<u8, TVidRcvChnStatus>::iterator, bool> Insert_Pair;
		Insert_Pair = m_mapVidRcvChnStatus.insert( std::map<u8, TVidRcvChnStatus>::value_type( byChnIdx, TVidRcvChnStatus(tStatus) ) );
		return ( Insert_Pair.second == true ) ? TRUE : FALSE;
	}
	void RmvVidRcvChn( const u8 byChnIdx ) 
	{
		m_mapVidRcvChnStatus.erase( byChnIdx );
		return;
	}

	BOOL32 AddAudSndChn( const u8 byChnIdx, const TAudSndChnStatus &tStatus )
	{
		if( byChnIdx != tStatus.GetChnIdx() )
		{
			return FALSE;
		}
		std::pair<std::map<u8, TAudSndChnStatus>::iterator, bool> Insert_Pair;
		Insert_Pair = m_mapAudSndChnStatus.insert( std::map<u8, TAudSndChnStatus>::value_type( byChnIdx, TAudSndChnStatus(tStatus) ) );
		return ( Insert_Pair.second == true ) ? TRUE : FALSE;
	}
	void RmvAudSndChn( const u8 byChnIdx ) 
	{
		m_mapAudSndChnStatus.erase( byChnIdx );
		return;
	}

	BOOL32 AddDVAudSndChn( const u8 byChnIdx, const TDVAudSndChnStatus &tStatus )
	{
		if( byChnIdx != tStatus.GetChnIdx() )
		{
			return FALSE;
		}
		std::pair<std::map<u8, TDVAudSndChnStatus>::iterator, bool> Insert_Pair;
		Insert_Pair = m_mapDVAudSndChnStatus.insert( std::map<u8, TDVAudSndChnStatus>::value_type( byChnIdx, TDVAudSndChnStatus(tStatus) ) );
		return ( Insert_Pair.second == true ) ? TRUE : FALSE;
	}
	void RmvDVAudSndChn( const u8 byChnIdx ) 
	{
		m_mapDVAudSndChnStatus.erase( byChnIdx );
		return;
	}
	
	BOOL32 AddAudRcvChn( const u8 byChnIdx, const TAudRcvChnStatus &tStatus )
	{
		if( byChnIdx != tStatus.GetChnIdx() )
		{
			return FALSE;
		}
		std::pair<std::map<u8, TAudRcvChnStatus>::iterator, bool> Insert_Pair;
		Insert_Pair = m_mapAudRcvChnStatus.insert( std::map<u8, TAudRcvChnStatus>::value_type( byChnIdx, TAudRcvChnStatus(tStatus) ) );
		return ( Insert_Pair.second == true ) ? TRUE : FALSE;
	}
	void RmvAudRcvChn( const u8 byChnIdx ) 
	{
		m_mapAudRcvChnStatus.erase( byChnIdx );
		return;
	}

	BOOL32 AddDVAudRcvChn( const u8 byChnIdx, const TDVAudRcvChnStatus &tStatus )
	{
		if( byChnIdx != tStatus.GetChnIdx() )
		{
			return FALSE;
		}
		std::pair<std::map<u8, TDVAudRcvChnStatus>::iterator, bool> Insert_Pair;
		Insert_Pair = m_mapDVAudRcvChnStatus.insert( std::map<u8, TDVAudRcvChnStatus>::value_type( byChnIdx, TDVAudRcvChnStatus(tStatus) ) );
		return ( Insert_Pair.second == true ) ? TRUE : FALSE;
	}
	void RmvDVAudRcvChn( const u8 byChnIdx ) 
	{
		m_mapDVAudRcvChnStatus.erase( byChnIdx );
		return;
	}

	BOOL32 AddDVidSndChn( const u8 byChnIdx, const TDVidSndChnStatus &tStatus )
	{
		if( byChnIdx != tStatus.GetChnIdx() )
		{
			return FALSE;
		}
		std::pair<std::map<u8, TDVidSndChnStatus>::iterator, bool> Insert_Pair;
		Insert_Pair = m_mapDVidSndChnStatus.insert( std::map<u8, TDVidSndChnStatus>::value_type( byChnIdx, TDVidSndChnStatus(tStatus) ) );
		return ( Insert_Pair.second == true ) ? TRUE : FALSE;
	}
	void RmvDVidSndChn( const u8 byChnIdx ) 
	{
		m_mapDVidSndChnStatus.erase( byChnIdx );
		return;
	}
	
	BOOL32 AddDVidRcvChn( const u8 byChnIdx, const TDVidRcvChnStatus &tStatus )
	{
		if( byChnIdx != tStatus.GetChnIdx() )
		{
			return FALSE;
		}
		std::pair<std::map<u8, TDVidRcvChnStatus>::iterator, bool> Insert_Pair;
		Insert_Pair = m_mapDVidRcvChnStatus.insert( std::map<u8, TDVidRcvChnStatus>::value_type( byChnIdx, TDVidRcvChnStatus(tStatus) ) );
		return ( Insert_Pair.second == true ) ? TRUE : FALSE;
	}
	void RmvDVidRcvChn( const u8 byChnIdx ) 
	{
		m_mapDVidRcvChnStatus.erase( byChnIdx );
		return;
	}

	void SetDVChnIdxOfDVAudSnd( u8 byDVChnIdx, u8 byChnIdx = 0 )
	{
		std::map<u8, TDVAudSndChnStatus>::iterator iter;
		if ( !m_mapDVAudSndChnStatus.empty() && ( (iter = m_mapDVAudSndChnStatus.find( byChnIdx )) != m_mapDVAudSndChnStatus.end()) )
		{
			iter->second.SetBindedDVChnIdx( byDVChnIdx );
		}
		return;
	}
	u8  GetDVChnIdxOfDVAudSnd( u8 byChnIdx = 0 )
	{
		std::map<u8, TDVAudSndChnStatus>::const_iterator iter;
		if ( !m_mapDVAudSndChnStatus.empty() && ( (iter = m_mapDVAudSndChnStatus.find( byChnIdx )) != m_mapDVAudSndChnStatus.end()) )
		{
			return iter->second.GetBindedDVChnIdx();
		}
		else
		{
			return FALSE; 
		}
	}

	void SetDVChnIdxOfAudRcv( u8 byDVChnIdx, u8 byChnIdx = 0 )
	{
		std::map<u8, TDVAudRcvChnStatus>::iterator iter;
		if ( !m_mapDVAudRcvChnStatus.empty() && ( (iter = m_mapDVAudRcvChnStatus.find( byChnIdx )) != m_mapDVAudRcvChnStatus.end()) )
		{
			iter->second.SetBindedDVChnIdx( byDVChnIdx );
		}
		return;
	}
	u8  GetDVChnIdxOfAudRcv( u8 byChnIdx = 0 )
	{
		std::map<u8, TDVAudRcvChnStatus>::const_iterator iter;
		if ( !m_mapDVAudRcvChnStatus.empty() && ( (iter = m_mapDVAudRcvChnStatus.find( byChnIdx )) != m_mapDVAudRcvChnStatus.end()) )
		{
			return iter->second.GetBindedDVChnIdx();
		}
		else
		{
			return FALSE; 
		}
	}
	
	BOOL32 IsChannelRcvOpen( const u8 &byChlIdx,const u8 &byMode )
	{
		// 		std::map<u8, TMcMtVidSndChnStatus> m_mapVidSndChnStatus;
		// 		std::map<u8, TMcMtDVidSndChnStatus> m_mapDVidSndChnStatus;
		// 		std::map<u8, TMcMtAudSndChnStatus> m_mapAudSndChnStatus;
		// 		std::map<u8, TMcMtVidRcvChnStatus > m_mapVidRcvChnStatus;
		// 		std::map<u8, TMcMtDVidRcvChnStatus> m_mapDVidRcvChnStatus;
		// 		std::map<u8, TMcMtAudRcvChnStatus > m_mapAudRcvChnStatus;
		
		if( MODE_VIDEO == byMode )
		{
			std::map<u8, TVidRcvChnStatus >::iterator iter;
			if( !m_mapVidRcvChnStatus.empty() && ( (iter = m_mapVidRcvChnStatus.find(byChlIdx)) != m_mapVidRcvChnStatus.end() ) )
			{
				return iter->second.IsOpen();
			}
		}
		else if( MODE_AUDIO == byMode )
		{
			std::map<u8, TAudRcvChnStatus >::iterator iter;
			if( !m_mapAudRcvChnStatus.empty() && ( (iter = m_mapAudRcvChnStatus.find(byChlIdx)) != m_mapAudRcvChnStatus.end() ) )
			{
				return iter->second.IsOpen();
			}
		}
		else if( MODE_SECVIDEO == byMode )
		{
			std::map<u8, TDVidRcvChnStatus>::iterator iter;
			if( !m_mapDVidRcvChnStatus.empty() && ( (iter = m_mapDVidRcvChnStatus.find(byChlIdx)) != m_mapDVidRcvChnStatus.end() ) )
			{
				return iter->second.IsOpen();
			}
		}
		else if( MODE_SECAUDIO == byMode )
		{
			std::map<u8, TDVAudRcvChnStatus>::iterator iter;
			if( !m_mapDVAudRcvChnStatus.empty() && ( (iter = m_mapDVAudRcvChnStatus.find(byChlIdx)) != m_mapDVAudRcvChnStatus.end() ) )
			{
				return iter->second.IsOpen();
			}
		}
		else
		{
			
		}
		return FALSE;
	}

	BOOL32 IsChannelSndOpen( const u8 &byChlIdx,const u8 &byMode )
	{
		// 		std::map<u8, TMcMtVidSndChnStatus> m_mapVidSndChnStatus;
		// 		std::map<u8, TMcMtDVidSndChnStatus> m_mapDVidSndChnStatus;
// 		std::map<u8, TMcMtAudSndChnStatus> m_mapAudSndChnStatus;
		if( MODE_VIDEO == byMode )
		{
			std::map<u8, TVidSndChnStatus >::iterator iter;
			if( !m_mapVidSndChnStatus.empty() && ( (iter = m_mapVidSndChnStatus.find(byChlIdx)) != m_mapVidSndChnStatus.end() ) )
			{
				return iter->second.IsOpen();
			}
		}
		else if( MODE_AUDIO == byMode )
		{
			std::map<u8, TAudSndChnStatus >::iterator iter;
			if( !m_mapAudSndChnStatus.empty() && ( (iter = m_mapAudSndChnStatus.find(byChlIdx)) != m_mapAudSndChnStatus.end() ) )
			{
				return iter->second.IsOpen();
			}
		}
		else if( MODE_SECVIDEO == byMode )
		{
			std::map<u8, TDVidSndChnStatus>::iterator iter;
			if( !m_mapDVidSndChnStatus.empty() && ( (iter = m_mapDVidSndChnStatus.find(byChlIdx)) != m_mapDVidSndChnStatus.end() ) )
			{
				return iter->second.IsOpen();
			}
		}
		else if( MODE_SECAUDIO == byMode )
		{
			std::map<u8, TDVAudSndChnStatus>::iterator iter;
			if( !m_mapDVAudSndChnStatus.empty() && ( (iter = m_mapDVAudSndChnStatus.find(byChlIdx)) != m_mapDVAudSndChnStatus.end() ) )
			{
				return iter->second.IsOpen();
			}
		}
		else
		{
			
		}
		return FALSE;
	}

	void SetVLastTick( u32 dwTick, u8 byChnIdx = 0 )
	{
		std::map<u8, TVidSndChnStatus>::iterator iter;
		if( !m_mapVidSndChnStatus.empty() && ( (iter = m_mapVidSndChnStatus.find(byChnIdx)) != m_mapVidSndChnStatus.end() ) )
		{
			iter->second.SetLastRefreshTick( dwTick );
		}
		return;
	}
	u32 GetVLastTick( u8 byChnIdx = 0 ) const
	{
		std::map<u8, TVidSndChnStatus>::const_iterator iter;
		if( !m_mapVidSndChnStatus.empty() && ( (iter = m_mapVidSndChnStatus.find(byChnIdx)) != m_mapVidSndChnStatus.end() ) )
		{
			return iter->second.GetLastRefreshTick();
		}
		else
		{
			return 0;
		}
	}

	void SetDVLastTick( u32 dwTick, u8 byChnIdx = 0 )
	{
		std::map<u8, TDVidSndChnStatus>::iterator iter;
		if( !m_mapDVidSndChnStatus.empty() && ( (iter = m_mapDVidSndChnStatus.find(byChnIdx)) != m_mapDVidSndChnStatus.end() ) )
		{
			iter->second.SetLastRefreshTick( dwTick );
		}
		return;
	}
	u32 GetDVLastTick( u8 byChnIdx = 0 ) const
	{
		std::map<u8, TDVidSndChnStatus>::const_iterator iter;
		if( !m_mapDVidSndChnStatus.empty() && ( (iter = m_mapDVidSndChnStatus.find(byChnIdx)) != m_mapDVidSndChnStatus.end() ) )
		{
			return iter->second.GetLastRefreshTick();
		}
		else
		{
			return 0;
		}
	}

    void   SetDecoderMute(BOOL32 bDecoderMute, u8 byChnIdx = 0)
	{
		std::map<u8, TAudRcvChnStatus>::iterator iter;
		if ( !m_mapAudRcvChnStatus.empty() && ( (iter = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
		{
			iter->second.SetIsDeaf( bDecoderMute );
		}
	} 
    BOOL32   IsDecoderMute( u8 byChnIdx = 0 ) const 
	{ 
		std::map<u8, TAudRcvChnStatus>::const_iterator iter;
		if ( !m_mapAudRcvChnStatus.empty() && ( (iter = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
		{
			return iter->second.IsDeaf();
		}
		else
		{
			return FALSE;
		}
	}
	void   SetDVAudDecoderMute(BOOL32 bDecoderMute, u8 byChnIdx = 0)
	{
		std::map<u8, TDVAudRcvChnStatus>::iterator iter;
		if ( !m_mapDVAudRcvChnStatus.empty() && ( (iter = m_mapDVAudRcvChnStatus.find( byChnIdx )) != m_mapDVAudRcvChnStatus.end() ) )
		{
			iter->second.SetIsDeaf( bDecoderMute );
		}
	} 
    BOOL32   IsDVAudDecoderMute( u8 byChnIdx = 0 ) const 
	{ 
		std::map<u8, TDVAudRcvChnStatus>::const_iterator iter;
		if ( !m_mapDVAudRcvChnStatus.empty() && ( (iter = m_mapDVAudRcvChnStatus.find( byChnIdx )) != m_mapDVAudRcvChnStatus.end() ) )
		{
			return iter->second.IsDeaf();
		}
		else
		{
			return FALSE;
		}
	}
    void   SetCaptureMute(BOOL32 bCaptureMute, u8 byChnIdx = 0)
	{
		std::map<u8, TAudSndChnStatus>::iterator iter;
		if ( !m_mapAudSndChnStatus.empty() && ( (iter = m_mapAudSndChnStatus.find( byChnIdx )) != m_mapAudSndChnStatus.end() ) )
		{
			iter->second.SetIsMute(bCaptureMute);
		}
	}
    BOOL32   IsCaptureMute( u8 byChnIdx = 0 ) const 
	{
		std::map<u8, TAudSndChnStatus>::const_iterator iter;
		if ( !m_mapAudSndChnStatus.empty() && ( (iter = m_mapAudSndChnStatus.find( byChnIdx )) != m_mapAudSndChnStatus.end() ) )
		{
			return iter->second.IsMute();
		}
		else
		{
			return FALSE;
		}
	}
	void   SetDVAudCaptureMute(BOOL32 bCaptureMute, u8 byChnIdx = 0)
	{
		std::map<u8, TDVAudSndChnStatus>::iterator iter;
		if ( !m_mapDVAudSndChnStatus.empty() && ( (iter = m_mapDVAudSndChnStatus.find( byChnIdx )) != m_mapDVAudSndChnStatus.end() ) )
		{
			iter->second.SetIsMute(bCaptureMute);
		}
	}
    BOOL32   IsDVAudCaptureMute( u8 byChnIdx = 0 ) const 
	{
		std::map<u8, TDVAudSndChnStatus>::const_iterator iter;
		if ( !m_mapDVAudSndChnStatus.empty() && ( (iter = m_mapDVAudSndChnStatus.find( byChnIdx )) != m_mapDVAudSndChnStatus.end() ) )
		{
			return iter->second.IsMute();
		}
		else
		{
			return FALSE;
		}
	}
    void   SetInMixing(BOOL32 bInMixing, u8 byChnIdx = 0)
	{
		std::map<u8, TAudSndChnStatus>::iterator iter;
		if ( !m_mapAudSndChnStatus.empty() && ( (iter = m_mapAudSndChnStatus.find( byChnIdx )) != m_mapAudSndChnStatus.end() ) )
		{
			iter->second.SetIsMtManuInMix(bInMixing);
		}
		return;
	} 
    BOOL32   IsInMixing( u8 byChnIdx = 0 ) const 
	{
		std::map<u8, TAudSndChnStatus>::const_iterator iter;
		if ( !m_mapAudSndChnStatus.empty() && ( (iter = m_mapAudSndChnStatus.find( byChnIdx )) != m_mapAudSndChnStatus.end() ) )
		{
			return iter->second.IsMtManuInMix();
		}
		else
		{
			return FALSE;
		}
	}
    void   SetIsInMixGrp(BOOL32 bDiscuss, u8 byChnIdx = 0) 
	{ 
		return;
	}
    BOOL32   IsInMixGrp(u8 byChnIdx = 0) const 
	{ 
		return FALSE;
	}    
	void    SetInTvWall(BOOL32 bInTvWall){ ; }
    BOOL32  IsInTvWall( void ) const { return FALSE; }
	void   SetInHdu(BOOL32 bInHdu, u8 byMode = MODE_VIDEO, u8 byChnIdx = 0)
	{
		;
	} 
    BOOL32   IsInHdu( u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 ) const 
	{ 
		return FALSE;
	}
    
    void   SetVideoMt(TVcMtChnObj tMtChnObj, u8 byChnIdx = 0)
	{ 
		std::map<u8, TVidRcvChnStatus>::iterator iter;
		if ( !m_mapVidRcvChnStatus.empty() && ( (iter = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
		{
			iter->second.SetVidSrc( tMtChnObj );
		}
	} 
    TVcMtChnObj GetVideoMt( u8 byChnIdx = 0 ,u8 byMode = MODE_VIDEO ) const 
	{ 
		TVcMtChnObj tRet;
		if( MODE_VIDEO == byMode )
		{
			std::map<u8, TVidRcvChnStatus>::const_iterator iter;
			if ( !m_mapVidRcvChnStatus.empty() && ( (iter = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
			{
				tRet = iter->second.GetVidSrc();
			}
		}
		else if( MODE_SECVIDEO == byMode )
		{
			std::map<u8, TDVidRcvChnStatus>::const_iterator iter;
			iter = m_mapDVidRcvChnStatus.find( byChnIdx );
			if( iter != m_mapDVidRcvChnStatus.end() )
			{
				tRet = iter->second.GetDVidSrc();
			}
		}
		else
		{

		}
		
		return tRet; 
	}
    void SetAudioMt(TVcMtChnObj tAudioMt, u8 byChnIdx = 0)
	{ 
		std::map<u8, TAudRcvChnStatus>::iterator iter;
		if ( !m_mapAudRcvChnStatus.empty() && ( (iter = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
		{
			iter->second.SetAudSrc( tAudioMt );
		}
	} 
    TVcMtChnObj GetAudioMt( u8 byChnIdx = 0,u8 byMode = MODE_AUDIO ) const 
	{
		TVcMtChnObj tRet;
		if( MODE_AUDIO == byMode )
		{
			std::map<u8, TAudRcvChnStatus>::const_iterator iter;
			if ( !m_mapAudRcvChnStatus.empty() && ( (iter = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
			{
				tRet = iter->second.GetAudSrc();
			}
		}
		else if( MODE_SECAUDIO == byMode)
		{
			std::map<u8, TDVAudRcvChnStatus>::const_iterator iter;
			iter = m_mapDVAudRcvChnStatus.find(byChnIdx);
			if( iter != m_mapDVAudRcvChnStatus.end() )
			{				
				std::map<u8, TDVidRcvChnStatus>::const_iterator iterDv;
				iterDv = m_mapDVidRcvChnStatus.find( iter->second.GetBindedDVChnIdx() );
				if( iterDv != m_mapDVidRcvChnStatus.end() )
				{
					tRet = iterDv->second.GetDVidSrc();
				}
			}
		}
		return tRet;
	}

	void   SetSelectMt(TVcMtChnObj tSeleteMt, u8 byMode, u8 byChnIdx = 0 ) 
    {
		if ( byMode == MODE_AUDIO || byMode == MODE_BOTH )
		{
			std::map<u8, TAudRcvChnStatus>::iterator iter;
			if ( !m_mapAudRcvChnStatus.empty() && ( (iter = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
			{
				iter->second.SetSelectMt( tSeleteMt );
			}
		}
		if ( byMode == MODE_VIDEO || byMode == MODE_BOTH )
		{
			std::map<u8, TVidRcvChnStatus>::iterator iter;
			if ( !m_mapVidRcvChnStatus.empty() && ( (iter = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
			{
				iter->second.SetSelectMt( tSeleteMt );
			}
		}  
		if ( byMode == MODE_SECVIDEO )
		{
			std::map<u8, TDVidRcvChnStatus>::iterator iter;
			if ( !m_mapDVidRcvChnStatus.empty() && ( (iter = m_mapDVidRcvChnStatus.find( byChnIdx )) != m_mapDVidRcvChnStatus.end() ) )
			{
				iter->second.SetSelectMt( tSeleteMt );
			}
		}
		return;
    }
	BOOL32  GetSelectMt( TVcMtChnObj& tVcMtChnObj, u8 byMode, u8 byChnIdx = 0 ) const 
    { 
        if ( byMode == MODE_AUDIO )
        {
            std::map<u8, TAudRcvChnStatus>::const_iterator iter;
			if ( !m_mapAudRcvChnStatus.empty() && ( (iter = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
			{
				tVcMtChnObj = iter->second.GetSelectMt();
				return TRUE;
			}
        }
        else if ( byMode == MODE_VIDEO )
        {
            std::map<u8, TVidRcvChnStatus>::const_iterator iter;
			if ( !m_mapVidRcvChnStatus.empty() && ( (iter = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
			{
				tVcMtChnObj = iter->second.GetSelectMt();
				return TRUE;
			}
        }    
		else if ( byMode == MODE_SECVIDEO )
		{
			std::map<u8, TDVidRcvChnStatus>::const_iterator iter;
			if ( !m_mapDVidRcvChnStatus.empty() && ( (iter = m_mapDVidRcvChnStatus.find( byChnIdx )) != m_mapDVidRcvChnStatus.end() ) )
			{
				tVcMtChnObj = iter->second.GetSelectMt();
				return TRUE;
			}
		}
        return FALSE;
    }

	void   SetSelByMcsDrag( u8 bySelMediaMode, u8 byChnIdx = 0 )
	{ 
		switch(bySelMediaMode)
		{
		case MODE_VIDEO:
			{
				std::map<u8, TVidRcvChnStatus>::iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					iter_v->second.SetIsSelbyMcsDrag( TRUE );
				}
				std::map<u8, TAudRcvChnStatus>::iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					iter_a->second.SetIsSelbyMcsDrag( FALSE );
				}
				std::map<u8, TDVidRcvChnStatus>::iterator iter_dv;
				if ( !m_mapDVidRcvChnStatus.empty() && ( (iter_dv = m_mapDVidRcvChnStatus.find( byChnIdx )) != m_mapDVidRcvChnStatus.end() ) )
				{
					iter_dv->second.SetIsSelbyMcsDrag( FALSE );
				}
			}
			break;
		case MODE_AUDIO:
			{
				std::map<u8, TAudRcvChnStatus>::iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					iter_a->second.SetIsSelbyMcsDrag( TRUE );
				}
				std::map<u8, TVidRcvChnStatus>::iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					iter_v->second.SetIsSelbyMcsDrag( FALSE );
				}
				std::map<u8, TDVidRcvChnStatus>::iterator iter_dv;
				if ( !m_mapDVidRcvChnStatus.empty() && ( (iter_dv = m_mapDVidRcvChnStatus.find( byChnIdx )) != m_mapDVidRcvChnStatus.end() ) )
				{
					iter_dv->second.SetIsSelbyMcsDrag( FALSE );
				}
			}
			break;
		case MODE_BOTH:
			{
				std::map<u8, TAudRcvChnStatus>::iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					iter_a->second.SetIsSelbyMcsDrag( TRUE );
				}
				std::map<u8, TVidRcvChnStatus>::iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					iter_v->second.SetIsSelbyMcsDrag( TRUE );
				}
				std::map<u8, TDVidRcvChnStatus>::iterator iter_dv;
				if ( !m_mapDVidRcvChnStatus.empty() && ( (iter_dv = m_mapDVidRcvChnStatus.find( byChnIdx )) != m_mapDVidRcvChnStatus.end() ) )
				{
					iter_dv->second.SetIsSelbyMcsDrag( FALSE );
				}
			}
			break;
		case MODE_SECVIDEO:
			{
				std::map<u8, TDVidRcvChnStatus>::iterator iter_dv;
				if ( !m_mapDVidRcvChnStatus.empty() && ( (iter_dv = m_mapDVidRcvChnStatus.find( byChnIdx )) != m_mapDVidRcvChnStatus.end() ) )
				{
					iter_dv->second.SetIsSelbyMcsDrag( TRUE );
				}
				std::map<u8, TAudRcvChnStatus>::iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					iter_a->second.SetIsSelbyMcsDrag( FALSE );
				}
				std::map<u8, TVidRcvChnStatus>::iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					iter_v->second.SetIsSelbyMcsDrag( FALSE );
				}
			}
			break;
		case MODE_NONE:
		default:
			{
				std::map<u8, TAudRcvChnStatus>::iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					iter_a->second.SetIsSelbyMcsDrag( FALSE );
				}
				std::map<u8, TVidRcvChnStatus>::iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					iter_v->second.SetIsSelbyMcsDrag( FALSE );
				}
				std::map<u8, TDVidRcvChnStatus>::iterator iter_dv;
				if ( !m_mapDVidRcvChnStatus.empty() && ( (iter_dv = m_mapDVidRcvChnStatus.find( byChnIdx )) != m_mapDVidRcvChnStatus.end() ) )
				{
					iter_dv->second.SetIsSelbyMcsDrag( FALSE );
				}
			}
			break;
		}
		return;
	} 
	void  AddSelByMcsMode(u8 byMode, u8 byChnIdx = 0)
	{
		switch(byMode)
		{
		case MODE_VIDEO:
			{
				std::map<u8, TVidRcvChnStatus>::iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					iter_v->second.SetIsSelbyMcsDrag( TRUE );
				}
			}
			break;
		case MODE_AUDIO:
			{
				std::map<u8, TAudRcvChnStatus>::iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					iter_a->second.SetIsSelbyMcsDrag( TRUE );
				}
			}
			break;
		case MODE_BOTH:
			{
				std::map<u8, TAudRcvChnStatus>::iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					iter_a->second.SetIsSelbyMcsDrag( TRUE );
				}
				std::map<u8, TVidRcvChnStatus>::iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					iter_v->second.SetIsSelbyMcsDrag( TRUE );
				}
			}
			break;
		case MODE_SECVIDEO:
			{
				std::map<u8, TDVidRcvChnStatus>::iterator iter_dv;
				if ( !m_mapDVidRcvChnStatus.empty() && ( (iter_dv = m_mapDVidRcvChnStatus.find( byChnIdx )) != m_mapDVidRcvChnStatus.end() ) )
				{
					iter_dv->second.SetIsSelbyMcsDrag( TRUE );
				}
			}
			break;
		default:
			break;
		}
		return;
	}

	BOOL32 IsInSelMode(u8 byMode, u8 byChnIdx = 0)
	{
		BOOL32 bRet = FALSE;
		switch(byMode)
		{
		case MODE_VIDEO:
			{
				std::map<u8, TVidRcvChnStatus>::const_iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					bRet = iter_v->second.IsSelByMcsDrag();
				}
			}
			break;
		case MODE_AUDIO:
			{
				std::map<u8, TAudRcvChnStatus>::const_iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					bRet = iter_a->second.IsSelByMcsDrag();
				}
			}
			break;
		case MODE_BOTH:
			{
				BOOL32 bRet_v = FALSE;
				BOOL32 bRet_a = FALSE;
				std::map<u8, TVidRcvChnStatus>::const_iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					bRet_v = iter_v->second.IsSelByMcsDrag();
				}
				std::map<u8, TAudRcvChnStatus>::const_iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					bRet_a = iter_a->second.IsSelByMcsDrag();
				}
				bRet = (bRet_v&&bRet_a);
			}
			break;
		case MODE_SECVIDEO:
			{
				std::map<u8, TDVidRcvChnStatus>::const_iterator iter_dv;
				if ( !m_mapDVidRcvChnStatus.empty() && ( (iter_dv = m_mapDVidRcvChnStatus.find( byChnIdx )) != m_mapDVidRcvChnStatus.end() ) )
				{
					bRet = iter_dv->second.IsSelByMcsDrag();
				}
			}
			break;
		default:
			bRet = FALSE;
			break;
		}
		return bRet;
	}
	
	void RemoveSelByMcsMode(u8 byMode, u8 byChnIdx = 0)
	{
		switch(byMode)
		{
		case MODE_VIDEO:
			{
				std::map<u8, TVidRcvChnStatus>::iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					iter_v->second.SetIsSelbyMcsDrag( FALSE );
				}
			}
			break;
		case MODE_AUDIO:
			{
				std::map<u8, TAudRcvChnStatus>::iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					iter_a->second.SetIsSelbyMcsDrag( FALSE );
				}
			}
			break;
		case MODE_BOTH:
			{
				std::map<u8, TVidRcvChnStatus>::iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChnIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					iter_v->second.SetIsSelbyMcsDrag( FALSE );
				}
				std::map<u8, TAudRcvChnStatus>::iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChnIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					iter_a->second.SetIsSelbyMcsDrag( FALSE );
				}
			}
			break;
		case MODE_SECVIDEO:
			{
				std::map<u8, TDVidRcvChnStatus>::iterator iter_dv;
				if ( !m_mapDVidRcvChnStatus.empty() && ( (iter_dv = m_mapDVidRcvChnStatus.find( byChnIdx )) != m_mapDVidRcvChnStatus.end() ) )
				{
					iter_dv->second.SetIsSelbyMcsDrag( FALSE );
				}
			}
			break;
		default:
			break;
		}
		return;
	}

	PairId	GetViewerPairid(u8 byChlMode , u8 byChlIdx = 0) const
	{
		PairId dwPairId = INVALID_INTERACTIONID;
		switch(byChlMode)
		{
		case MODE_VIDEO://������Ƶ
			{
				std::map<u8, TVidRcvChnStatus>::const_iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChlIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					dwPairId =  iter_v->second.GetMediaH().m_dwPairID;
				}
			} 
			break;
		case MODE_AUDIO://������Ƶ
			{
				std::map<u8, TAudRcvChnStatus>::const_iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChlIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					dwPairId = iter_a->second.GetMediaH().m_dwPairID;
				}
			}
			break;
		case MODE_SECVIDEO://˫����Ƶ
			{
				std::map<u8, TDVidRcvChnStatus>::const_iterator iter_dv;
				if ( !m_mapDVidRcvChnStatus.empty() && ( (iter_dv = m_mapDVidRcvChnStatus.find( byChlIdx )) != m_mapDVidRcvChnStatus.end() ) )
				{
					dwPairId = iter_dv->second.GetMediaH().m_dwPairID;
				}
			}
			break;
		default:
			break;
		}
		return dwPairId;
	}
	BOOL32	SetViewerPairid(PairId dwPairId , u8 byChlMode , u8 byChlIdx = 0)
	{
		switch(byChlMode)
		{
		case MODE_VIDEO://������Ƶ
			{
				std::map<u8, TVidRcvChnStatus>::iterator iter_v;
				if ( !m_mapVidRcvChnStatus.empty() && ( (iter_v = m_mapVidRcvChnStatus.find( byChlIdx )) != m_mapVidRcvChnStatus.end() ) )
				{
					TVcMtMediaH tMediaH = iter_v->second.GetMediaH();
					tMediaH.m_dwPairID = dwPairId;
					iter_v->second.SetMediaH( tMediaH );
				}
			}
			
			break;
		case MODE_AUDIO://������Ƶ
			{
				std::map<u8, TAudRcvChnStatus>::iterator iter_a;
				if ( !m_mapAudRcvChnStatus.empty() && ( (iter_a = m_mapAudRcvChnStatus.find( byChlIdx )) != m_mapAudRcvChnStatus.end() ) )
				{
					TVcMtMediaH tMediaH = iter_a->second.GetMediaH();
					tMediaH.m_dwPairID = dwPairId;
					iter_a->second.SetMediaH( tMediaH );
				}
			}
			break;
		case MODE_SECVIDEO://˫����Ƶ
			{
				std::map<u8, TDVidRcvChnStatus>::iterator iter_dv;
				if ( !m_mapDVidRcvChnStatus.empty() && ( (iter_dv = m_mapDVidRcvChnStatus.find( byChlIdx )) != m_mapDVidRcvChnStatus.end() ) )
				{
					TVcMtMediaH tMediaH = iter_dv->second.GetMediaH();
					tMediaH.m_dwPairID = dwPairId;
					iter_dv->second.SetMediaH( tMediaH );
				}
			}
			break;
		default:
			break;
		}

		return TRUE;
	}
/*//del userless code 		
	//Recording status
	void SetMtRecording( u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 )
	{
		return;
	}
	BOOL32 IsMtRecording( u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 )
	{
		return FALSE;
	}
	void SetMtNoRecording( u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 )
	{
		return;
	}
	BOOL32 IsMtNoRecording( u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 )
	{
		return FALSE;
	}
	void SetMtRecPause( u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 )
	{
		return;
	}
	BOOL32 IsMtRecPause( u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 )
	{
		return FALSE;
	}

	BOOL32 GetMtRecState( TRecState * ptRecState, u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 )
	{
		return FALSE;
	}

	BOOL32 IsMtRecSkipFrame( u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 ) const
	{
		return FALSE;
	}
	void SetMtRecSkipFrame( BOOL32 bSkipFrame, u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 )
	{
		return;
	}
	BOOL32 IsMtRecPublish( u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 ) const 
	{ 
		return FALSE;
	}
	void SetMtRecPublish( BOOL32 bPublish, u8 byMode = MODE_VIDEO, u8 byChnIdx = 0 )
	{ 
		return;
	}
	*/
	BOOL32 IsMtManuInMix( u8 byChnIdx = 0 )
	{
		std::map<u8, TAudSndChnStatus>::iterator iter;
		if ( !m_mapAudSndChnStatus.empty() && ( (iter = m_mapAudSndChnStatus.find( byChnIdx )) != m_mapAudSndChnStatus.end() ) )
		{
			return iter->second.IsMtManuInMix();
		}
		else
		{
			return FALSE;
		}
	}
	void SetIsMtManuInMix( BOOL32 bManuInMix, u8 byChnIdx = 0 )
	{
		std::map<u8, TAudSndChnStatus>::iterator iter;
		if ( !m_mapAudSndChnStatus.empty() && ( (iter = m_mapAudSndChnStatus.find( byChnIdx )) != m_mapAudSndChnStatus.end() ) )
		{
			iter->second.SetIsMtManuInMix( bManuInMix );
		}
		return;
	}

	std::map<u8, TVidRcvChnStatus>  GetVidRcvChnStatus()
	{
		return m_mapVidRcvChnStatus;
	}

	std::map<u8, TDVidRcvChnStatus> GetDVidRcvChnStatus()
	{
		return m_mapDVidRcvChnStatus;
	}

    std::map<u8, TAudRcvChnStatus>  GetAudRcvChnStatus()
	{
		return m_mapAudRcvChnStatus;
	}

	std::map<u8, TDVAudRcvChnStatus> GetDVAudRcvChnStatus()
	{
		return m_mapDVAudRcvChnStatus;
	}

	void Print( void ) const
	{
		StaticLog( "TopPRIVidSndChnIdx = %u\n", GetTopPRIVidSndChnIdx() );

		StaticLog( "Vid Send Chn Status --\n" );
		std::map<u8, TVidSndChnStatus>::const_iterator iter_vs;
		for ( iter_vs = m_mapVidSndChnStatus.begin(); iter_vs != m_mapVidSndChnStatus.end(); iter_vs++ )
		{
			StaticLog( "\tChn%u - %s\n", iter_vs->second.GetChnIdx(), iter_vs->second.GetAlias() );
		}
		
		StaticLog( "Aud Send Chn Status --\n" );
		std::map<u8, TAudSndChnStatus>::const_iterator iter_as;
		for ( iter_as = m_mapAudSndChnStatus.begin(); iter_as != m_mapAudSndChnStatus.end(); iter_as++ )
		{
			StaticLog( "\tChn%u - %s : IsManuInMix.%u, IsMute.%u\n", iter_as->second.GetChnIdx(), iter_as->second.GetAlias(), iter_as->second.IsMtManuInMix(), iter_as->second.IsMute() );
		}

		StaticLog( "DVid Send Chn Status --\n" );
		std::map<u8, TDVidSndChnStatus>::const_iterator iter_dvs;
		for ( iter_dvs = m_mapDVidSndChnStatus.begin(); iter_dvs != m_mapDVidSndChnStatus.end(); iter_dvs++ )
		{
			StaticLog( "\tChn%u - %s\n", iter_dvs->second.GetChnIdx(), iter_dvs->second.GetAlias() );
		}
		
		StaticLog( "Vid Receive Chn Status --\n" );
		std::map<u8, TVidRcvChnStatus>::const_iterator iter_vr;
		for ( iter_vr = m_mapVidRcvChnStatus.begin(); iter_vr != m_mapVidRcvChnStatus.end(); iter_vr++ )
		{
			StaticLog( "\tChn%u - %s :\n", iter_vr->second.GetChnIdx(), iter_vr->second.GetAlias() );
			StaticLog( "\t\tIsSelByMcsDrag.%u, BindPairId.%u\n", iter_vr->second.IsSelByMcsDrag(), iter_vr->second.GetMediaH().m_dwPairID );
			StaticLog( "\t\tMediaSrc<Mcu.%u, Mt.%u, Chn.%u>\n", iter_vr->second.GetVidSrc().GetMt().GetMcuIdx(), iter_vr->second.GetVidSrc().GetMt().GetMtId(),
										 iter_vr->second.GetVidSrc().GetChlIdx() );
			StaticLog( "\t\tSelectSrc<Mcu.%u, Mt.%u, Chn.%u>\n", iter_vr->second.GetSelectMt().GetMt().GetMcuIdx(), iter_vr->second.GetSelectMt().GetMt().GetMtId(),
										 iter_vr->second.GetSelectMt().GetChlIdx() );
		}
		
		StaticLog( "Aud Receive Chn Status --\n" );
		std::map<u8, TAudRcvChnStatus>::const_iterator iter_ar;
		for ( iter_ar = m_mapAudRcvChnStatus.begin(); iter_ar != m_mapAudRcvChnStatus.end(); iter_ar++ )
		{
			StaticLog( "\tChn%u - %s :\n", iter_ar->second.GetChnIdx(), iter_ar->second.GetAlias() );
			StaticLog( "\t\tIsSelByMcsDrag.%u, IsDeaf.%u, BindPairId.%u\n", iter_ar->second.IsSelByMcsDrag(), iter_ar->second.IsDeaf(), iter_ar->second.GetMediaH().m_dwPairID );
			StaticLog( "\t\tMediaSrc<Mcu.%u, Mt.%u, Chn.%u>\n", iter_ar->second.GetAudSrc().GetMt().GetMcuIdx(), iter_ar->second.GetAudSrc().GetMt().GetMtId(),
				iter_ar->second.GetAudSrc().GetChlIdx() );
			StaticLog( "\t\tSelectSrc<Mcu.%u, Mt.%u, Chn.%u>\n", iter_ar->second.GetSelectMt().GetMt().GetMcuIdx(), iter_ar->second.GetSelectMt().GetMt().GetMtId(),
				iter_ar->second.GetSelectMt().GetChlIdx() );
		}

		StaticLog( "DVid Receive Chn Status --\n" );
		std::map<u8, TDVidRcvChnStatus>::const_iterator iter_dvr;
		for ( iter_dvr = m_mapDVidRcvChnStatus.begin(); iter_dvr != m_mapDVidRcvChnStatus.end(); iter_dvr++ )
		{
			StaticLog( "\tChn%u - %s :\n", iter_dvr->second.GetChnIdx(), iter_dvr->second.GetAlias() );
			StaticLog( "\t\tIsSelByMcsDrag.%u, BindPairId.%u\n", iter_dvr->second.IsSelByMcsDrag(), iter_dvr->second.GetMediaH().m_dwPairID );
			StaticLog( "\t\tMediaSrc<Mcu.%u, Mt.%u, Chn.%u>\n", iter_dvr->second.GetDVidSrc().GetMt().GetMcuIdx(), iter_dvr->second.GetDVidSrc().GetMt().GetMtId(),
				iter_dvr->second.GetDVidSrc().GetChlIdx() );
			StaticLog( "\t\tSelectSrc<Mcu.%u, Mt.%u, Chn.%u>\n", iter_dvr->second.GetSelectMt().GetMt().GetMcuIdx(), iter_dvr->second.GetSelectMt().GetMt().GetMtId(),
				iter_dvr->second.GetSelectMt().GetChlIdx() );
		}
	}

}
// #ifndef WIN32
// __attribute__ ( (packed) ) 
// #endif
;

enum ERecType 
{
	em_InValid = 0,
	em_RecConf ,
	em_RecMt
};

// 
struct TVrsRecParam : public TRecStartPara
{
public:
    u8   m_byIsAnonymous;     //�Ƿ�ѡ���½�ۿ�ֱ����0=��֧�� 1=֧�������ۿ�

	TMtAlias m_tVrsAlias;	  //vrs����

public:
	TVrsRecParam()
	{
		memset(this, 0, sizeof(TVrsRecParam));
	}

	void Reset()
	{
		memset(this, 0, sizeof(TVrsRecParam));

		TRecStartPara::Reset();
		m_tVrsAlias.SetNull();
	}

	void SetIsAnonymous(u8 byIsAnonymous)
	{
		m_byIsAnonymous = byIsAnonymous;
	}

	u8 GetIsAnonymous() const
	{
		return m_byIsAnonymous;
	}

    void Print() const
    {
		TRecStartPara::Print();
        StaticLog("m_byIsAnonymous:    %d\n", m_byIsAnonymous);
		m_tVrsAlias.Print();
    }
};

struct TConfAutoRecInfo
{    
protected:
    u8     m_byAutoRec;         //�Ƿ���鿪ʼ�Զ�¼��0-��֧�֣�1-֧��
	
    // ��ʼʱ��ͽ���ʱ�䶼��֧�ֿ��죬����ʱ������ڿ�ʼʱ��֮��
    TKdvTime m_tTimeStart;      //��ʼ¼�������ʱ��.ֻ���� ʱ/��/�룬year����Ϊ0������Ҫ����ʱ�䣬���鿪ʼ���Զ���ʼ
    TKdvTime m_tTimeEnd;        //�Զ�¼�������ʱ��.ֻ���� ʱ/��/�룬year����Ϊ0������Ҫ����ʱ�䣬��Ҫ�û��ֶ�ֹͣ
	
    u8     m_byWeekdays;        //��ʼ¼����������ͣ�bit0(Sunday)-6(Sat)��1��ʾ������Ҫ¼��
	
public:
    TConfAutoRecInfo()
    {
        memset(this, 0, sizeof(TConfAutoRecInfo));
		SetRecAnyDay(); // Ĭ��ÿ�춼¼
    }
    
    void     SetAutoRec(BOOL32 bEnableAuto) { m_byAutoRec = bEnableAuto ?  1 : 0; }
    BOOL32   IsAutoRec() const { return (m_byAutoRec == 1); }
	
	u8     GetWeekDays()   //����ÿ�춼¼��
    {
        return m_byWeekdays;
    }
	
    void     SetRecAnyDay()   //����ÿ�춼¼��
    {
        m_byWeekdays = 0x7F;
    }
    void     SetRecNoDay()    //����ÿ�춼��¼,ע�⣺���ָ����m_byAutoRec=1��m_byWeekdays = 0�ǲ��Ϸ���,��������ʱ���
    {
        m_byWeekdays = 0;
    }
    void     SetRecWeekDay(u8 byDay, BOOL32 bEnable)  // ָ��ĳ��¼�� 0 - Sunday, 1 - Monday,  6- Satday
    {
        if (bEnable)
            m_byWeekdays |= (1<<byDay);
        else
            m_byWeekdays &= ~(1<<byDay);
    }
    BOOL32 IsRecAnyDay() const  // �Ƿ�ÿ�춼¼��
    {
        return (m_byWeekdays == 0x7F);
    }
    BOOL32 IsRecNoDay() const   // �Ƿ�ÿ�춼��¼
    {
        return (m_byWeekdays == 0);
    }
    BOOL32 IsRecWeekDay(u8 byDay) const //�Ƿ��ض���¼��
    {
        return  ((m_byWeekdays & (1<<byDay)) >> byDay) ? TRUE : FALSE;
    }
	
    void     SetStartTime(const TKdvTime &tStartTime) { m_tTimeStart = tStartTime; }
    TKdvTime GetStartTime() const { return m_tTimeStart; }
    void     SetEndTime(const TKdvTime &tEndTime) { m_tTimeEnd = tEndTime; }
    TKdvTime GetEndTime() const { return m_tTimeEnd; }
	
    void     Print() const
    {   
        StaticLog("Conf Auto Rec Attrib:\n");
        StaticLog("m_byAutoRec: %d\n", IsAutoRec());
        if (IsAutoRec())
        {
            StaticLog("m_byWeekdays: 0x%x\n", m_byWeekdays);        
            StaticLog("m_tTimeStart: ");
            m_tTimeStart.Print();
            StaticLog("\n");        
            StaticLog("m_tTimeEnd: ");
            m_tTimeEnd.Print();
            StaticLog("\n");
            //        StaticLog("m_szRecName: %s\n", GetRecName());
            StaticLog("m_tRecParam: \n");
        }
    }
};
#endif
