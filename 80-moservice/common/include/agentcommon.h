/*****************************************************************************
   ģ����      : Mcu Agent
   �ļ���      : Agentcommon.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: mcu ���ý��滯��Ӧ����
   ����        : 
   �汾        : V4.0  Copyright( C) 2006-2008 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���        �޸�����
   2005/08/17  4.0         liuhuiyun     ����
   2006/11/02  4.0         �ű���        ���ݽṹ�Ż�
******************************************************************************/
#ifndef  _AGENT_COMMONSTRUCT_H_
#define  _AGENT_COMMONSTRUCT_H_

#include "evagtsvc.h"
#include "agtcomm.h"
#include "mcuconst.h"
#include "mcustruct.h"
//#include "ftpc_api.h"

#ifdef WIN32
#pragma pack( push )
#pragma pack( 1 )
#endif

#ifdef _LINUX_
#ifndef min
#define min(a,b) ((a)>(b)?(b):(a))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#endif

#define  SUCCESS_AGENT				(u16)0		// ��ȷ

// �����붨��
#define  ERR_AGENT_ERRNO_BGN        (u8)(100)

#define  ERR_AGENT_SETNODEVALUE		(u16)(ERR_AGENT_ERRNO_BGN+1)		// ���ýڵ����
#define  ERR_AGENT_GETNODEVALUE		(u16)(ERR_AGENT_ERRNO_BGN+2)		// ȡ�ڵ�ֵ����
#define  ERR_AGENT_CANNOTCHANGE		(u16)(ERR_AGENT_ERRNO_BGN+3)		// �Ľڵ㲻���޸�
#define  ERR_AGENT_WRITEFILE		(u16)(ERR_AGENT_ERRNO_BGN+4)		// д�ļ�ʧ��
#define  ERR_AGENT_SETMASKIP		(u16)(ERR_AGENT_ERRNO_BGN+5)		// ���������ַʧ��
#define  ERR_AGENT_SETGWIP			(u16)(ERR_AGENT_ERRNO_BGN+6)		// �������ص�ַʧ��
#define  ERR_AGENT_SETMPCIP			(u16)(ERR_AGENT_ERRNO_BGN+7)		// ����MpcIp��ַʧ��
#define  ERR_AGENT_GETCPURATE		(u16)(ERR_AGENT_ERRNO_BGN+8)		// ȡCpuռ����
#define  ERR_AGENT_GETQOSIPTYPE		(u16)(ERR_AGENT_ERRNO_BGN+9)		// ȡQosIp�������ʹ���
#define  ERR_AGENT_GETTABLEVALUE	(u16)(ERR_AGENT_ERRNO_BGN+10)		// ȡ��ֵ����
#define  ERR_AGENT_GETPERIINFO		(u16)(ERR_AGENT_ERRNO_BGN+11)		// ȡ������Ϣʧ��
#define  ERR_AGENT_SEMTAKE			(u16)(ERR_AGENT_ERRNO_BGN+12)		// ȡ�ź�������
#define  ERR_AGENT_VALUEBESET       (u16)(ERR_AGENT_ERRNO_BGN+13)       // ���õĲ������Ϸ�
#define  ERR_AGENT_SETLOGININFO		(u16)(ERR_AGENT_ERRNO_BGN+14)		// ����LoginInfo����
#define  ERR_AGENT_GETLOGININFO		(u16)(ERR_AGENT_ERRNO_BGN+15)		// ȡLoginInfo����



#define	 MAX_UPDATE_NUM				(u8)5

// ������Ϣ
struct TBoardInfo
{
protected:
    u8  m_byBrdId;					// ���
    u8  m_byLayer;					// ���
    u8  m_bySlot;					// �ۺ�
    u8  m_byType;					// ����
    u32 m_dwBrdIp;					// ����Ip(������)
    u8  m_byPortChoice;				// ����ѡ��
    u8  m_byCastChoice;				// �鲥ѡ��
    u8  m_byPeriNum;			    // ���е��������
    u8  m_abyPeriId[MAXNUM_BRD_EQP];// �����ڸõ����ϵ�����ID
    u8  m_byBrdState;				// �Ƿ�����
    u8  m_byOSType;                 // �����OS���� OS_TYPE_WIN32 ...
    s8  m_szAlias[MAXLEN_BRDALIAS]; // ���������� BRD_ALIAS_MPC
public:
	void Print()
	{
		OspPrintf(TRUE, FALSE, "m_byBrdId.%d\n", m_byBrdId);
		OspPrintf(TRUE, FALSE, "m_byLayer.%d\n", m_byLayer);
		OspPrintf(TRUE, FALSE, "m_bySlot.%d\n", m_bySlot);
		OspPrintf(TRUE, FALSE, "m_byType.%d\n", m_byType);
		OspPrintf(TRUE, FALSE, "m_dwBrdIp.0x%x\n", m_dwBrdIp);
		OspPrintf(TRUE, FALSE, "m_byPortChoice.%d\n", m_byPortChoice);
		OspPrintf(TRUE, FALSE, "m_byCastChoice.%d\n", m_byCastChoice);
	}
public:    
    TBoardInfo() : m_byBrdId(0),
                   m_byLayer(0),
                   m_bySlot(0),
                   m_byType(0),
                   m_dwBrdIp(0),
                   m_byPortChoice(1),
                   m_byCastChoice(0),
                   m_byPeriNum(0),
                   m_byBrdState(BOARD_STATUS_UNKNOW),
                   m_byOSType(0) 
    {
        memset( &m_abyPeriId, 0, sizeof(m_abyPeriId) );
        memset( m_szAlias, 0, sizeof(m_szAlias));
    }
    void SetBrdId(u8 byId) { m_byBrdId = byId;    }
    u8   GetBrdId(void) const { return m_byBrdId;    }
    void SetLayer(u8 byLayer) { m_byLayer = byLayer;    }
    u8   GetLayer(void) const { return m_byLayer;    }
    void SetSlot(u8 bySlot) { m_bySlot = bySlot;    }
    u8   GetSlot(void) const { return m_bySlot;    }
    void SetType(u8 byType) { m_byType = byType;    }
    u8   GetType(void) const { return m_byType;    }
    void SetBrdIp(u32 dwIp) { m_dwBrdIp = htonl(dwIp);    }
    u32  GetBrdIp(void) const { return ntohl(m_dwBrdIp);    }
    void SetPortChoice(u8 byChoice) { m_byPortChoice = byChoice;    }
    u8   GetPortChoice(void) const { return m_byPortChoice;    }
    void SetCastChoice(u8 byChoice) { m_byCastChoice = byChoice;    }
    u8   GetCastChoice(void) const { return m_byCastChoice;    }
    void SetState(u8 byState) { m_byBrdState = byState;    }
    u8   GetState(void) const { return m_byBrdState;    }
    void SetOSType(u8 byType) { m_byOSType = byType;    }
    u8   GetOSType(void) const { return m_byOSType;    }
    void SetAlias(const s8* pszAlias)
    {
        memset( m_szAlias, 0, sizeof(m_szAlias) );
        strncpy( m_szAlias, pszAlias,  sizeof(m_szAlias) - 1);
    }
    const s8* GetAlias() const
    {
        return m_szAlias;
    }

    BOOL32 SetPeriId( u8 byPeriId )
    {
        u8 byIndex = 0;
        BOOL32 bExist = FALSE;
        BOOL32 bSet   = FALSE;
        if ( 0 != byPeriId)
        {
            for ( ; byIndex < MAXNUM_BRD_EQP; byIndex ++ )
            {
                if ( byPeriId == m_abyPeriId[byIndex] )
                {
                    bExist = TRUE;
                    bSet   = TRUE;  // FIXME
                    break;
                }                
            }
            if ( !bExist )
            {
                for ( byIndex = 0; byIndex < MAXNUM_BRD_EQP; byIndex ++ )
                {
                    if ( 0 == m_abyPeriId[byIndex] )
                    {
                        m_abyPeriId[byIndex] = byPeriId;
                        m_byPeriNum ++;
                        bSet = TRUE;
                        break;
                    }
                }
            }
        }
        return bSet;
    }
    void SetPeriId(u8 *pbyPerid, u8 byPriNum)
    {
        if ( NULL != pbyPerid && 0 != byPriNum && byPriNum <= MAXNUM_BRD_EQP )
        {
            memcpy( &m_abyPeriId, pbyPerid, byPriNum );
            m_byPeriNum = byPriNum;
        }
    }
    void GetPeriId(u8 *pbyPerid, u8* pbyPriNum)
    {
        *pbyPriNum = m_byPeriNum;
        if ( 0 != m_byPeriNum && NULL != pbyPerid )
        {
            memcpy(pbyPerid, &m_abyPeriId, m_byPeriNum);
        }
    }
    void SetPeriIdNull()
    {
        m_byPeriNum = 0;
        memset( &m_abyPeriId, 0, sizeof(m_abyPeriId) );
    }
	BOOL32 IsEqual( TBoardInfo &tBrdInfo )
	{
		if( GetBrdId()      != tBrdInfo.GetBrdId()      ||	
			GetLayer()      != tBrdInfo.GetLayer() 	    ||
			GetSlot()       != tBrdInfo.GetSlot()	    ||	
			GetType()       != tBrdInfo.GetType()       ||	
			GetBrdIp()      != tBrdInfo.GetBrdIp() 	    ||	
			GetPortChoice() != tBrdInfo.GetPortChoice() ||
			GetCastChoice() != tBrdInfo.GetCastChoice() )		 
		{
			return FALSE;
		}
		u8 abyPeriId[MAXNUM_BRD_EQP];
		u8 byPriNum;
		tBrdInfo.GetPeriId( &abyPeriId[0], &byPriNum );
		if( m_byPeriNum != byPriNum )
		{
			return FALSE;
		}
		if( 0 != memcmp( &abyPeriId[0],m_abyPeriId,m_byPeriNum) )
		{
			return FALSE;
		}		
		const char* pszAlias = tBrdInfo.GetAlias();
		if( 0 != memcmp( pszAlias,m_szAlias,strlen(m_szAlias) ) )
		{
			return FALSE;
		}
		return TRUE;
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;



struct TLocalInfo
{
protected:
    u8  m_byMcuId;                    // Mcu ID
    u8  m_bySaved;
    u16 m_wSaved;

    s8  m_achAlias[MAXLEN_ALIAS+1];   // Mcu ����
    s8  m_achE164Num[MAXLEN_E164+1];  // E164��

    u16 m_wCheckMtLinkTimes;          // �ն˶���������(������)
    u16 m_wCheckLinkTime;             // �������ʱ��ms (������)
    u16 m_wListRefreshTime;           // ����ʱ�ն��б�ˢ��ʱ��ms(������)
    u16 m_wAudioRefreshTime;          // ����ʱ��Ƶ��Ϣˢ��ʱ��ms(������)
    u16 m_wVideoRefreshTime;          // ����ʱ��Ƶ��Ϣˢ��ʱ��ms(������)
    u8  m_byIsSaveBand;               // �Ƿ��ʡ����(0:����ʡ 1:��ʡ)

    u8  m_byNPlusMode;                      //�Ƿ�֧��N+1����
    u8  m_byNPlusBackupMode;                //�Ƿ�N+1���ݹ���ģʽ
    u32 m_dwNPlusMcuIp;                     //��֧��N+1�Ҳ��Ǳ��ݹ���ģʽ����Ҫ����N+1����mcu�ĵ�ַ
    u16 m_wNPlusRtdTime;                    //N+1����rtdʱ��(s)
    u16 m_wNPlusRtdNum;                     //N+1����rtd����
    u8  m_byNPlusRollBack;                  //�Ƿ�֧�ֹ��ϻָ�

	u8 m_byIsHoldDefaultConf;				//�Ƿ�֧��ȱʡ�����ٿ�(0-��֧�֣�1-֧��)
	u8 m_byIsShowMMcuMtList;				//�Ƿ�֧����ʾ�ϼ�mcu�ն��б�(0-��֧�֣�1-֧��)
	u8 m_byMaxMcsOnGoingConfNum;			//��������ٿ���mcs��ʱ������
	u8 m_byAdminLevel;						//�����ٿ��Ļ������������(1��4 1���)
	u8 m_byConfNameShowType;				//���¼���������ʾ�ı����Ļ������Ƶ�����(1:��������(Ĭ��ֵ) 2:mcu���ƣ���������)
	u8 m_byIsMMcuSpeaker;					//�Ƿ�Ĭ�Ͻ��ϼ�mcu��Ϊ������(0:���� 1:��

	u32 m_dwReserve;						//����	

public:
    TLocalInfo() : m_byMcuId(0),
                   m_bySaved(0),
                   m_wSaved(0),
                   m_wCheckMtLinkTimes(0),
                   m_wCheckLinkTime(0),
                   m_wListRefreshTime(0),
                   m_wAudioRefreshTime(0),
                   m_wVideoRefreshTime(0),
                   m_byIsSaveBand(0),
                   m_byNPlusMode(0),
                   m_byNPlusBackupMode(0),
                   m_dwNPlusMcuIp(0),
                   m_wNPlusRtdTime(0),
                   m_wNPlusRtdNum(0),
                   m_byNPlusRollBack(1),
				   m_byIsHoldDefaultConf(0),
				   m_byIsShowMMcuMtList(0),
				   m_byMaxMcsOnGoingConfNum(DEF_MCSONGOINGCONFNUM_MAX),
				   m_byAdminLevel(DEF_ADMINLVEL),
				   m_byConfNameShowType(1),
				   m_byIsMMcuSpeaker(1),
				   m_dwReserve(0)

    {
        memset( m_achAlias, 0, sizeof(m_achAlias) );
        memset( m_achE164Num, 0, sizeof(m_achE164Num) );
    }
    void SetMcuId(u8 byId){ m_byMcuId = byId;    }
    u8   GetMcuId(void) const  { return m_byMcuId;    }
    void SetCheckTimes(u16 wTimes) { m_wCheckMtLinkTimes = wTimes;    }
    u16  GetCheckTimes(void) const { return m_wCheckMtLinkTimes;    }
    void SetCheckTime(u16 wTime){ m_wCheckLinkTime = wTime; }
    u16  GetCheckTime(void) const { return m_wCheckLinkTime;    }
    void SetListRefreshTime(u16 wTime) { m_wListRefreshTime = wTime;    }
    u16  GetListRefreshTime(void) const { return m_wListRefreshTime;    }
    void SetAudioRefreshTime(u16 wTime) { m_wAudioRefreshTime = wTime;    }
    u16  GetAudioRefreshTime(void) const { return m_wAudioRefreshTime;    }
    void SetVideoRefreshTime(u16 wTime) { m_wVideoRefreshTime = wTime;    }
    u16  GetVideoRefreshTime(void) const { return m_wVideoRefreshTime;    }
    void SetIsSaveBand(u8 bySave){ m_byIsSaveBand = bySave;    }
    u8   GetIsSaveBand(void) const { return m_byIsSaveBand;    }

    void    SetIsNPlusMode(BOOL32 bNPlus) { m_byNPlusMode = bNPlus ? 1 : 0; }   //�Ƿ�֧��N+1����
    BOOL32  IsNPlusMode(void) { return (1 == m_byNPlusMode ? TRUE : FALSE); }
    void    SetIsNPlusBackupMode(BOOL32 bBackupMode) { m_byNPlusBackupMode = bBackupMode ? 1 : 0; }
    BOOL32  IsNPlusBackupMode(void) { return (1 == m_byNPlusBackupMode ? TRUE : FALSE); } //�Ƿ�N+1���ݹ���ģʽ
    void    SetNPlusMcuIp(u32 dwIp) { m_dwNPlusMcuIp = htonl(dwIp); }   
    u32     GetNPlusMcuIp(void) { return ntohl(m_dwNPlusMcuIp); } //N+1����mcu�ĵ�ַ(host order)
    void    SetNPlusRtdTime(u16 wTime) { m_wNPlusRtdTime = htons(wTime); }
    u16     GetNPlusRtdTime(void) { return ntohs(m_wNPlusRtdTime); }
    void    SetNPlusRtdNum(u16 wNum) { m_wNPlusRtdNum = htons(wNum); }
    u16     GetNPlusRtdNum(void) { return ntohs(m_wNPlusRtdNum); }
    void    SetIsNPlusRollBack(BOOL32 bRollBack) { m_byNPlusRollBack = bRollBack ? 1 : 0; }
    BOOL32  GetIsNPlusRollBack(void) { return (1 == m_byNPlusRollBack ? TRUE : FALSE); }

	void	SetIsHoldDefaultConf( u8 byIsHold ){ m_byIsHoldDefaultConf = (0 == byIsHold) ? 0 : 1; }
	u8		GetIsHoldDefaultConf( void ){ return m_byIsHoldDefaultConf; }
	void	SetIsShowMMcuMtList( u8 byIsShow ){ m_byIsShowMMcuMtList = (0 == byIsShow) ? 0 : 1;  }
	u8		GetIsShowMMcuMtList( void ){ return m_byIsShowMMcuMtList; }
	void	SetMaxMcsOnGoingConfNum( u8 byNum ){ m_byMaxMcsOnGoingConfNum = byNum; }
	u8		GetMaxMcsOnGoingConfNum( void ){ return m_byMaxMcsOnGoingConfNum; };
	void	SetAdminLevel( u8 byAdminLevel ){ m_byAdminLevel = byAdminLevel; };
	u8		GetAdminLevel( void ){ return m_byAdminLevel; }
	void	SetConfNameShowType( u8 byType ) { m_byConfNameShowType = ( 2 == byType ) ? 2 : 1; }
	u8		GetConfNameShowType(void) const{ return m_byConfNameShowType; }
	void	SetIsMMcuSpeaker( u8 byIsMMcuSpeaker ){ m_byIsMMcuSpeaker = (0 == byIsMMcuSpeaker) ? 0 : 1; }
	u8		GetIsMMcuSpeaker( ){ return m_byIsMMcuSpeaker; }

    void SetAlias(LPCSTR lpszAlias)
    {
        if ( NULL != lpszAlias )
        {
            strncpy( m_achAlias, lpszAlias, sizeof(m_achAlias) );
            m_achAlias[sizeof(m_achAlias)-1] = '\0';
        }
    }
    LPCSTR GetAlias(void){ return m_achAlias;    }
    void SetE164Num(LPCSTR lpszE164Num)
    {
        if ( NULL != lpszE164Num )
        {
            strncpy( m_achE164Num, lpszE164Num, sizeof(m_achE164Num) );
            m_achE164Num[sizeof(m_achE164Num)-1] = '\0';
        }
    }
    LPCSTR GetE164Num(void){ return m_achE164Num;    }
    void Print()
    {
        OspPrintf( TRUE, FALSE, "[TLocalInfo] McuId.%d, Alias.%s, E164Num.%s \n\t CheckLinkTimes.%d,CheckLinkTime.%d, \n\t ListRefresh.%d, AudioRefresh.%d, VideoRefreshTime.%d, IsSaveBand.%d\n\t IsNPlus.%d, IsNPlusBackMode.%d, NPlusMcuIp.0x%x NPlusRtdTime.%d RtdNum.%d  IsHoldDefaultConf:%d IsShowMMcuMtList:%d MaxMcsOnGoingConfNum:%d\n AdminLevel:%d LocalConfNameShowType:%d m_byIsMMcuSpeaker:%d\n",
                                 m_byMcuId, m_achAlias, m_achE164Num, 
                                 m_wCheckMtLinkTimes, m_wCheckLinkTime,
                                 m_wListRefreshTime, m_wAudioRefreshTime, m_wVideoRefreshTime, 
                                 m_byIsSaveBand,
                                 IsNPlusMode(), IsNPlusBackupMode(), GetNPlusMcuIp(),
								 GetNPlusRtdTime(), GetNPlusRtdNum(),
								 m_byIsHoldDefaultConf,m_byIsShowMMcuMtList,
									m_byMaxMcsOnGoingConfNum,m_byAdminLevel,
									m_byConfNameShowType,m_byIsMMcuSpeaker
									);
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// ������Ϣ 
struct TNetWorkInfo
{
protected:
    u32 m_dwGKIpAddr;                 // Gk ��ַ(������)
    u32 m_dwRRQMtadpIp;               // ע��gk����Э��������ַ(������)
    u32 m_dwMulticastIpAddr ;         // �鲥��ַ(������)
    u16 m_wMulticastPort;             // �鲥�˿�(������)
    u16 m_wRecvStartPort;             // ������ʼ�˿�(������)
    u16 m_w225245StartPort;           // 245�˿�(������)
    u16 m_w225245MtNum;               // 245 Mt��(������)
	u16 m_wMTUSize;					  // MTU��С, zgc, 2007-04-02
	u8	m_byMTUSetupMode;			  // MTU����ģʽ, zgc, 2007-04-05
    u8  m_byUseMPCTransData;          // �Ƿ�ʹ��Mpc������
    u8  m_byUseMPCStack;              // �Ƿ�ʹ��Mpc����Э��ջ
    u8  m_byGkCharge;                 // �Ƿ�֧��GK�Ʒ�
    u8  m_byReserved;                 // �����ֶ�
	u8  m_byIsGkUseRRQPwd;			  // GK�Ƿ��������뷢��RRQ
	s8	m_achRRQPassword[GK_RRQ_PWD_LEN];	    //GK RRQ��Կ

public:
    TNetWorkInfo() : m_dwGKIpAddr(0),
					 m_dwRRQMtadpIp(0),
					 m_dwMulticastIpAddr(0),
					 m_wMulticastPort(0),
					 m_wRecvStartPort(0),
					 m_w225245StartPort(0),
					 m_w225245MtNum(0),
					 m_wMTUSize(0),
					 m_byMTUSetupMode(0),
					 m_byUseMPCTransData(0),
                     m_byUseMPCStack(0),
                     m_byGkCharge(0),
                     m_byReserved(0),
					 m_byIsGkUseRRQPwd(0)
					{
						memset(m_achRRQPassword,0,sizeof(m_achRRQPassword));
					}
	void SetGKIp(u32 dwIp){ m_dwGKIpAddr = htonl(dwIp);	}
	u32  GetGkIp(void) {	return ntohl(m_dwGKIpAddr);	}
	void SetRRQMtadpIp(u32 dwIp) { m_dwRRQMtadpIp = htonl(dwIp);	}
	u32  GetRRQMtadpIp(void) {	return ntohl(m_dwRRQMtadpIp);	}
	void SetCastIp(u32 dwIp){ m_dwMulticastIpAddr = htonl(dwIp);	}
	u32  GetCastIp(void){ return ntohl(m_dwMulticastIpAddr);	}
	void SetCastPort(u16 wPort){ m_wMulticastPort = wPort;	}
	u16  GetCastPort(void) { return m_wMulticastPort;	}
	void SetRecvStartPort(u16 wPort) { m_wRecvStartPort = wPort;	}
	u16  GetRecvStartPort(void) { return m_wRecvStartPort;	}
	void Set225245StartPort(u16 wPort) { m_w225245StartPort = wPort;	}
	u16  Get225245StartPort(void) { return m_w225245StartPort;	}
	void Set225245MtNum(u16 wMtNum) { m_w225245MtNum = wMtNum;	}
	u16  Get225245MtNum(void) { return m_w225245MtNum;	}
	void SetMTUSize( u16 wMTUSize ) { m_wMTUSize = wMTUSize; }
	u16  GetMTUSize(void) { return m_wMTUSize; }
	void SetMTUSetupMode( u8 byMode ) { m_byMTUSetupMode = byMode; }
	u8	 GetMTUSetupMode( void ) { return m_byMTUSetupMode; }
	void SetMpcTransData(u8 byStyle){ m_byUseMPCTransData = byStyle;	}
	u8	 GetMpcTransData(void) { return m_byUseMPCTransData;	}
    void SetMpcStack(BOOL32 bUse){ m_byUseMPCStack = bUse ? 1 : 0;	}
	u8	 GetMpcStack(void) { return m_byUseMPCStack;	}
    void   SetIsGKCharge(BOOL32 bCharge) { m_byGkCharge = ( bCharge ? 1 : 0 );  }
    BOOL32 GetIsGKCharge(void) { return m_byGkCharge == 1 ? TRUE : FALSE;    }
   
	
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
		else
			memset(&m_achRRQPassword,0,sizeof(m_achRRQPassword));
    }
    
    LPCSTR GetGkRRQPassword() const
    {
        return m_achRRQPassword;
	}

	void Print()
	{
		OspPrintf( TRUE, FALSE, "[NetInfo]: GkIp.0x%x, bCharge.%d, RRQMtAdpIp.0x%x, MultiCastIp.0x%x, MultiCastPort.%d, RecvStartPort.%d, 225245StartPort.%d, 225245MtNum.%d, MTUSize.%d, MTUSetupMode.%d, MpcTransData.%d, MpcStack.%d,Use RRQ Pwd.%d,GK RRQ Pwd.%s !\n",
								 m_dwGKIpAddr, m_byGkCharge,
                                 m_dwRRQMtadpIp, m_dwMulticastIpAddr, 
								 m_wMulticastPort, m_wRecvStartPort, m_w225245StartPort,
								 m_w225245MtNum, m_wMTUSize, m_byMTUSetupMode, m_byUseMPCTransData, m_byUseMPCStack,
								 m_byIsGkUseRRQPwd,m_achRRQPassword);
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// Trap ����Ϣ
struct TTrapInfo
{
protected:
    u8   m_byIndex;									// ���
    u32  m_dwTrapIp;								// ����Trap�����ܷ�����Ip(������)
    s8   m_achReadCom[MAXLEN_TRAP_COMMUNITY+1];		// ����ͬ��
    s8   m_achWriteCom[MAXLEN_TRAP_COMMUNITY+1];    // д��ͬ��
    u16  m_wGetSetPort;								// ��ȡ�˿�(������)
    u16  m_wSendTrapPort;							// ��Trap�˿�(������)
public:
    TTrapInfo() : m_byIndex(0),
				  m_dwTrapIp(0),
				  m_wGetSetPort(0),
				  m_wSendTrapPort(0)
	{
		memset( m_achReadCom, 0, sizeof(m_achReadCom) );
		memset( m_achWriteCom, 0, sizeof(m_achWriteCom) );
	}
	void SetIndex(u8 byIndex) { m_byIndex = byIndex;	}
	u8   GetIndex(void) const { return m_byIndex;	}
	void SetTrapIp(u32 dwTrapIp) { m_dwTrapIp = htonl(dwTrapIp);	}
	u32  GetTrapIp(void) const { return ntohl(m_dwTrapIp);	}
	void SetGSPort(u16 wPort) { m_wGetSetPort = wPort;	}
	u16  GetGSPort(void) const { return m_wGetSetPort;	}
	void SetTrapPort(u16 wPort) { m_wSendTrapPort = wPort;	}
	u16  GetTrapPort(void) const { return m_wSendTrapPort;	}

    void SetReadCom(LPCSTR lpszReadCom)
    {
        if ( NULL != lpszReadCom)
        {
            strncpy(m_achReadCom, lpszReadCom, sizeof(m_achReadCom));
            m_achReadCom[sizeof(m_achReadCom)-1] = '\0';
        }
    }
    LPCSTR GetReadCom(void){ return m_achReadCom; }

	void SetWriteCom(LPCSTR lpszWriteCom)
	{
		if ( NULL != lpszWriteCom )
		{
			strncpy(m_achWriteCom, lpszWriteCom, sizeof(m_achWriteCom));
			m_achWriteCom[sizeof(m_achWriteCom)-1] = '\0';
		}
	}
	LPCSTR GetWriteCom(void) { return m_achWriteCom;	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//���������Ϣ
struct TEqpBasicInfo
{
protected:
    u8 m_byEqpId;                       //�豸ID
    s8 m_achAlias[MAXLEN_EQP_ALIAS+1];  //�豸����
public:
    TEqpBasicInfo() : m_byEqpId(0)	{ memset( m_achAlias, 0, sizeof(m_achAlias) );	}
    void SetEqpId(u8 byEqpId) { m_byEqpId = byEqpId;  }
    u8   GetEqpId(void) const { return m_byEqpId;  }
    void SetAlias(LPCSTR lpszAlias)
    {	
		s8 achAlias[MAXLEN_EQP_ALIAS+1] = {0};
	
        if ( NULL != lpszAlias)
		{
			strncpy(achAlias, lpszAlias, strlen(lpszAlias));
			
			//1.ȥ���ַ�����ǰ���������ո��Լ�TAB��
			Trim(achAlias); 
			achAlias[sizeof(achAlias)-1] = '\0';
			
			//2.��ȡ�Ƿ��ַ�֮ǰ���ַ���
			for(u8 byIdx = 0; byIdx < MAXLEN_EQP_ALIAS +1; byIdx++)
			{
				if(achAlias[byIdx] == ' ' || achAlias[byIdx] == ',' || achAlias[byIdx] == '\t')
				{
					achAlias[byIdx] = '\0';
					
					break;
				}
			}

			strncpy(m_achAlias, achAlias, sizeof(m_achAlias));
			m_achAlias[sizeof(m_achAlias)-1] = '\0';
		}
    }
    LPCSTR GetAlias(void){ return m_achAlias; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//�������а������Ϣ
struct TEqpRunBrdInfo
{
protected:
    u8  m_byRunBrdId;       //���а�ID
    u16 m_wEqpRecvPort;     //���а�(����)�������ʼ�˿ں� ������
    u32 m_dwIpAddr;         //���а�Ip
public:
    TEqpRunBrdInfo() : m_byRunBrdId(0),
					   m_wEqpRecvPort(0),
                       m_dwIpAddr(0){}
    void SetRunBrdId(u8 byBrdId){ m_byRunBrdId = byBrdId;  }
    u8   GetRunBrdId(void) const { return m_byRunBrdId;  }
    void SetEqpRecvPort(u16 wRecvPort)  { m_wEqpRecvPort = htons(wRecvPort);  }
    u16  GetEqpRecvPort(void) const { return ntohs(m_wEqpRecvPort);  }   
    void SetIpAddr(u32 dwIp){ m_dwIpAddr = htonl(dwIp);    }
    u32  GetIpAddr(void) const { return ntohl(m_dwIpAddr);    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//���轻���������Ϣ
struct TEqpSwitchBrdInfo
{
protected:
    u8  m_bySwitchBrdId;     //������ID
    u16 m_wMcuRecvPort;      //������(MCU)�������ʼ�˿ں� ������
public:
    TEqpSwitchBrdInfo() : m_bySwitchBrdId(0),
						  m_wMcuRecvPort(0)	{}
    void SetSwitchBrdId(u8 byBrdId) { m_bySwitchBrdId = byBrdId;  }
    u8   GetSwitchBrdId(void) const { return m_bySwitchBrdId;  }
    void SetMcuRecvPort(u16 wPort) { m_wMcuRecvPort = htons(wPort);  }
    u16  GetMcuRecvPort(void) const { return ntohs(m_wMcuRecvPort);  }

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//����map�����Ϣ
struct TEqpMapInfo
{
protected:
    u8 m_abyUsedMapId[MAXNUM_MAP];
    u8 m_byMapNum;
public:
    TEqpMapInfo() : m_byMapNum(0){ memset(m_abyUsedMapId, 0, sizeof(m_abyUsedMapId));  }

	// ��set����ֻ��Ӧ˳�δ�С����д��
    void SetUsedMapId(u8 byMapId )
    {
		if ( m_byMapNum < MAXNUM_MAP )
		{
			m_abyUsedMapId[m_byMapNum] = byMapId;
			m_byMapNum ++;
		}
    }
    u8 GetUsedMapId( u8 byIdx )
	{
		return m_abyUsedMapId[byIdx];
	}

    BOOL32 GetUsedMap( u8 * pbyMap, u8 &byMapCount )
    {
        if ( NULL == pbyMap ) 
        {
            return FALSE;
        }
        memcpy(pbyMap, m_abyUsedMapId, m_byMapNum);
        byMapCount = m_byMapNum;
        return TRUE;
    }
    BOOL32 SetUsedMap( u8 * pbyMap, u8 byMapCount )
    {
        if ( NULL == pbyMap || 0 == byMapCount )
        {
            return FALSE;
        }
        memcpy(m_abyUsedMapId, pbyMap, byMapCount);
        m_byMapNum = byMapCount;
        return TRUE;
    }
    void GetUsedMapStr( s8 * pszMapStr )
    {
        if( NULL == pszMapStr )
        {
            return;
        }
		for(u8 nLp = 0; nLp < m_byMapNum; nLp++)
		{
			sprintf(pszMapStr, "%s,%d", pszMapStr, ((s8*)m_abyUsedMapId)[nLp]);
		}
		if ( pszMapStr[0] == ',')
		{
			pszMapStr[0] = ' ';
		}
        return;
    }
    u8   GetUsedMapNum(void) 
    {
        return m_byMapNum;
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// 1.Bas ��Ϣ
struct TEqpBasInfo : public TEqpBasicInfo,
                     public TEqpRunBrdInfo,
                     public TEqpSwitchBrdInfo,
                     public TEqpMapInfo
{
    void Print(void)
    {                           
        OspPrintf(TRUE, FALSE, "BasId: %d, BasAlias:%s, RunBrdId: %d, RunBrdIp.0x%x, BasRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d, UsedMapNum: %d !\n",
                                GetEqpId(), GetAlias(), 
                                GetRunBrdId(), GetIpAddr(), GetEqpRecvPort(), 
                                GetSwitchBrdId(), GetMcuRecvPort(), GetUsedMapNum());
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//zw[08/06/2008]
// 1.1.BasHD ��Ϣ
struct TEqpBasHDInfo : public TEqpBasicInfo,
                       public TEqpSwitchBrdInfo
{
protected:
    u16 m_wRecvStartPort;				// ��ʼ���ն˿ں�(������) δʹ��
    u32 m_dwAddr;						// Ip��ַ        (������)
    u8  m_byType;                       // Ӳ������: ��ͨ���ͻ���263+
public:
    TEqpBasHDInfo(void) : m_wRecvStartPort(0),
						m_dwAddr(0){}
    void SetEqpRecvPort(u16 wRecvPort) { m_wRecvStartPort = htons(wRecvPort);   }
    u16  GetEqpRecvPort(void) const { return ntohs(m_wRecvStartPort);    }
    void SetIpAddr(u32 dwRecIp) { m_dwAddr = htonl(dwRecIp);    }
    u32  GetIpAddr(void) const { return ntohl(m_dwAddr);  }
    void SetHDBasType(u8 byType) { m_byType = byType; }
    u8   GetHDBasType(void) const { return m_byType; }
    void Print(void)
    {
        OspPrintf( TRUE, FALSE, "RecId: %d, RecAlias:%s, RecIpAddr: 0x%x, RecRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d, Type: %d!\n",
                                GetEqpId(), GetAlias(), 
                                GetIpAddr(), GetEqpRecvPort(), 
                                GetSwitchBrdId(), GetMcuRecvPort(),
                                GetHDBasType() );
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// 2.Mixer ��Ϣ
struct TEqpMixerInfo : public TEqpBasicInfo,
                       public TEqpRunBrdInfo,
                       public TEqpSwitchBrdInfo,
                       public TEqpMapInfo
{
protected:
    u8  m_byMaxChannelInGrp;			// ���������ͨ����
	u8	m_byIsSendRedundancy;			// �Ƿ����෢��, zgc, 2007-0725

public:
    TEqpMixerInfo() : m_byMaxChannelInGrp(0){}
    void SetMaxChnInGrp(u8 byChnInGrp) { m_byMaxChannelInGrp = byChnInGrp;   }
    u8   GetMaxChnInGrp(void) { return m_byMaxChannelInGrp;   }
	void SetIsSendRedundancy( BOOL32 IsRedundancy ) { m_byIsSendRedundancy = IsRedundancy ? 1 : 0; }
	BOOL32 IsSendRedundancy(void) const { return ( 1 == m_byIsSendRedundancy ) ? TRUE : FALSE; } 
    
    void Print(void)
    {
        OspPrintf(TRUE, FALSE, "MixerId: %d, MixerAlias:%s, RunBrdId: %d, RunBrdIp.0x%x, MixerRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d, UsedMapNum: %d, MaxChnInGrp: %d, IsSendRedundancy: %d!\n",
                                GetEqpId(), GetAlias(), 
                                GetRunBrdId(), GetIpAddr(), GetEqpRecvPort(), 
                                GetSwitchBrdId(), GetMcuRecvPort(), 
                                GetUsedMapNum(), GetMaxChnInGrp(), IsSendRedundancy() );
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


// 3.���ָ��ϵ���ǽ��Ϣ
struct TEqpMpwInfo : public TEqpBasicInfo,
                     public TEqpRunBrdInfo,
                     public TEqpMapInfo
{
    void Print(void)
    {
        OspPrintf(TRUE, FALSE, "MpwId: %d, MpwAlias:%s, RunBrdId: %d, RunBrdIp.0x%x, MpwRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d, UsedMapNum: %d !\n",
                                GetEqpId(), GetAlias(), 
                                GetRunBrdId(), GetIpAddr(), GetEqpRecvPort(), GetUsedMapNum() );
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// 4.����ǽ��Ϣ
struct TEqpTVWallInfo : public TEqpBasicInfo,
                        public TEqpRunBrdInfo,
                        public TEqpMapInfo
{
    void Print(void)
    {
        OspPrintf(TRUE, FALSE, "TVWallId: %d, TVWallAlias:%s, RunBrdId: %d, RunBrdIp.0x%x, TVWallRecvPort: %d, UsedMapNum: %d !\n",
                                GetEqpId(), GetAlias(), 
                                GetRunBrdId(), GetIpAddr(), GetEqpRecvPort(), GetUsedMapNum() );
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// 5.Prs��Ϣ
struct TEqpPrsInfo : public TEqpBasicInfo,
                     public TEqpRunBrdInfo,
                     public TEqpSwitchBrdInfo
{
protected:
    u16 m_wFirstTimeSpan;			// ��һ���ش�����(ms)
    u16 m_wSecondTimeSpan;		    // �ڶ����ش�����(ms)
    u16 m_wThirdTimeSpan;			// �������ش�����(ms)
    u16 m_wRejectTimeSpan;		    // ���ڶ�����ʱ����(ms)
public:
    TEqpPrsInfo() : m_wFirstTimeSpan(0),
					m_wSecondTimeSpan(0),
					m_wThirdTimeSpan(0),
					m_wRejectTimeSpan(0){}
    void SetFirstTimeSpan(u16 wFirstTimeSpan) { m_wFirstTimeSpan = htons(wFirstTimeSpan); }
    u16  GetFirstTimeSpan(void) { return ntohs(m_wFirstTimeSpan);   }
    void SetSecondTimeSpan(u16 wSecondTimeSpan) { m_wSecondTimeSpan = htons(wSecondTimeSpan); }
    u16  GetSecondTimeSpan(void) { return ntohs(m_wSecondTimeSpan);   }
    void SetThirdTimeSpan(u16 wThirdTimeSpan) { m_wThirdTimeSpan = htons(wThirdTimeSpan); }
    u16  GetThirdTimeSpan(void) { return ntohs(m_wThirdTimeSpan);   }
    void SetRejectTimeSpan(u16 wRejectTimeSpan) { m_wRejectTimeSpan = htons(wRejectTimeSpan); }
    u16  GetRejectTimeSpan(void) { return ntohs(m_wRejectTimeSpan);   }
    void Print(void)
    {
        OspPrintf(TRUE, FALSE, "PrsId: %d, PrsAlias:%s, RunBrdId: %d, RunBrdIp.0x%x, PrsRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d !\n",
                                GetEqpId(), GetAlias(), 
                                GetRunBrdId(), GetIpAddr(), GetEqpRecvPort(), 
                                GetSwitchBrdId(), GetMcuRecvPort() );
        OspPrintf(TRUE, FALSE, "PrsIpAddr: 0x%x, FirstTimeSpan: %d, SecondTimeSpan: %d, ThirdTimeSpan: %d, RejectTimeSpan: %d!\n",
                                GetIpAddr(), 
                                GetFirstTimeSpan(), GetSecondTimeSpan(), 
                                GetThirdTimeSpan(), GetRejectTimeSpan());
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// 6.Vmp ��Ϣ
struct TEqpVMPInfo : public TEqpBasicInfo,
                     public TEqpRunBrdInfo,
                     public TEqpSwitchBrdInfo,
                     public TEqpMapInfo
{
protected:
    u8  m_byEncodeNum;			// ͬʱ֧�ֵ���Ƶ����·��
public:
    TEqpVMPInfo(void) : m_byEncodeNum(0){}
    void SetEncodeNum(u8 byEncNum){ m_byEncodeNum = byEncNum;   }
    u8   GetEncodeNum(void) const { return m_byEncodeNum;    }
    
    void Print(void)
    {
        OspPrintf(TRUE, FALSE, "VMPId: %d, VMPAlias:%s, RunBrdId: %d, RunBrdIp.0x%x, VMPRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d, UsedMapNum: %d !\n",
                                GetEqpId(), GetAlias(), 
                                GetRunBrdId(), GetIpAddr(), GetEqpRecvPort(), 
                                GetSwitchBrdId(), GetMcuRecvPort(), GetUsedMapNum() );
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// 7.¼�����Ϣ
struct TEqpRecInfo : public TEqpBasicInfo,
                     public TEqpSwitchBrdInfo
{
protected:
    u16 m_wRecvStartPort;				// ���ַš�¼�������ʼ���ն˿ں�(������) δʹ��
    u32 m_dwAddr;						// ���ַš�¼�����Ip��ַ        (������)
public:
    TEqpRecInfo(void) : m_wRecvStartPort(0),
						m_dwAddr(0){}
    void SetEqpRecvPort(u16 wRecvPort) { m_wRecvStartPort = htons(wRecvPort);   }
    u16  GetEqpRecvPort(void) const { return ntohs(m_wRecvStartPort);    }
    void SetIpAddr(u32 dwRecIp) { m_dwAddr = htonl(dwRecIp);    }
    u32  GetIpAddr(void) const { return ntohl(m_dwAddr);  }
    void Print(void)
    {
        OspPrintf(TRUE, FALSE, "RecId: %d, RecAlias:%s, RecIpAddr: 0x%x, RecRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d !\n",
                                GetEqpId(), GetAlias(), 
                                GetIpAddr(), GetEqpRecvPort(), 
                                GetSwitchBrdId(), GetMcuRecvPort() );
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//4.6������
struct THduChnlModePortAgt
{
public:
    THduChnlModePortAgt()
	{
		memset(this, 0x0, sizeof(THduChnlModePortAgt));
	}

	u8 GetZoomRate() const { return byZoomRate; }
	void SetZoomRate(u8 val) { byZoomRate = val; }

    u8 GetOutPortType() const { return byOutPortType; }
    void SetOutPortType(u8 val) { byOutPortType = val; }
	
    u8 GetOutModeType() const { return byOutModeType; }
    void SetOutModeType(u8 val) { byOutModeType = val; }
	
	u8 GetScalingMode() const { return (reserved & 0x03);}
	void SetScalingMode(u8 byMode) { reserved = (reserved | 0x03) & (byMode | 0xFC); }

private:
    u8 byOutPortType;
    u8 byOutModeType;
	u8 byZoomRate;

	// 1)���ֶε�2λ��ʹ�ã����ڱ�ʶScalingMode [11/29/2011 chendaiwei]
    u8 reserved;
}
; 

//Hdu��Ϣ
struct TEqpHduInfo: public TEqpBasicInfo,
                    public TEqpRunBrdInfo,
					public TEqpMapInfo
{
protected:

THduChnlModePortAgt atHduChnlModePort[MAXNUM_HDU_CHANNEL];    
u8  m_byStartMode; //����ģʽ(0: hdu_2, 1: hdu_1, 2: hdu_L)	
public:
	TEqpHduInfo()
	{
        memset(&atHduChnlModePort, 0X0, sizeof(atHduChnlModePort));
		SetStartMode(STARTMODE_HDU_M); //Ĭ��Ϊhdu_2
	}
	
	void SetStartMode(u8 byStartMode){ m_byStartMode = byStartMode; }
	u8   GetStartMode(){ return m_byStartMode;}

	void GetHduChnlModePort( u8 byNum, THduChnlModePortAgt &tHduChnModePort )
	{
		if (byNum == 0 || byNum == 1)
		{
			tHduChnModePort.SetOutModeType( atHduChnlModePort[byNum].GetOutModeType() );
			tHduChnModePort.SetOutPortType( atHduChnlModePort[byNum].GetOutPortType() );
			tHduChnModePort.SetZoomRate( atHduChnlModePort[byNum].GetZoomRate() );
			tHduChnModePort.SetScalingMode(atHduChnlModePort[byNum].GetScalingMode());
		}
		else
		{
			OspPrintf(TRUE, FALSE, "[TEqpHduInfo]:GetHduChnlModePort()'s byNum wrong!\n");
			return;			
		}
		
		return;
	}
	
    void SetHduChnlModePort( u8 byNum, THduChnlModePortAgt &tHduChnModePort )
	{
		if (byNum == 0 || byNum == 1)
		{
			atHduChnlModePort[byNum].SetOutModeType( tHduChnModePort.GetOutModeType() );
			atHduChnlModePort[byNum].SetOutPortType( tHduChnModePort.GetOutPortType() );
            atHduChnlModePort[byNum].SetZoomRate( tHduChnModePort.GetZoomRate() );
			atHduChnlModePort[byNum].SetScalingMode(tHduChnModePort.GetScalingMode() );
		}
		else
		{
			OspPrintf(TRUE, FALSE, "[TEqpHduInfo]:SetHduChnlModePort()'s byNum wrong!\n");
			return;	
		}
		
		return;
	}
	
	void Print(void)
	{
		OspPrintf(TRUE, FALSE, "\nHdu:%s\nEqpId:%d, RunBrd:%d\n, ZoomRate[0] = %d, ZoomRate[1] = %d\nHduOutPortType[0]:%d, HduOutModeType[0]:%d\n,HduOutPortType[1]:%d, HduOutModeType[1]%d:,HduScalingMode[0]:%d,HduScalingMode[1]:%d\n",
			GetAlias(), GetEqpId(), GetRunBrdId(), 
			atHduChnlModePort[0].GetZoomRate(), atHduChnlModePort[1].GetZoomRate(),
			atHduChnlModePort[0].GetOutPortType(), atHduChnlModePort[0].GetOutModeType(),
			atHduChnlModePort[1].GetOutPortType(), atHduChnlModePort[1].GetOutModeType(),
			atHduChnlModePort[0].GetScalingMode(), atHduChnlModePort[1].GetScalingMode());
        
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//��VMP��Ϣ
struct TEqpSvmpInfo: public TEqpBasicInfo,
                     public TEqpRunBrdInfo,
					 public TEqpSwitchBrdInfo,
					 public TEqpMapInfo
{
	u8  m_byVmpSubType;  //��ʾ��vmp�������ͣ�TYPE_MPUSVMP��TYPE_MPU2VMP_ENHANCED,TYPE_MPU2VMP_BASIC

	void SetVmpType( u8 byVmpSubType ) { m_byVmpSubType = byVmpSubType;}	
	u8	 GetVmpType() const { return m_byVmpSubType; }

	TEqpSvmpInfo()
	{
		m_byVmpSubType = TYPE_MPUSVMP;//Ĭ����mpu��vmp
	}

	void Print(void)
    {
        OspPrintf(TRUE, FALSE, "SVMPId: %d, SVMPAlias:%s, RunBrdId: %d, RunBrdIp.0x%x, VMPRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d, UsedMapNum: %d, VmpSubType:%d!\n",
			GetEqpId(), GetAlias(), 
			GetRunBrdId(), GetIpAddr(), GetEqpRecvPort(), 
			GetSwitchBrdId(), GetMcuRecvPort(), GetUsedMapNum(),GetVmpType());
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
//˫VMP�е���VMP����Ϣ
struct TEqpDvmpBasicInfo: public TEqpBasicInfo,
                          public TEqpRunBrdInfo,
					      public TEqpSwitchBrdInfo,
					      public TEqpMapInfo
{
	void Print(void)
    {
        OspPrintf(TRUE, FALSE, "DVMPId: %d, DVMPAlias:%s, RunBrdId: %d, RunBrdIp.0x%x, VMPRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d, UsedMapNum: %d !\n",
			GetEqpId(), GetAlias(), 
			GetRunBrdId(), GetIpAddr(), GetEqpRecvPort(), 
			GetSwitchBrdId(), GetMcuRecvPort(), GetUsedMapNum() );
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//˫VMP��Ϣ�ṹ
//struct TEqpDvmpInfo
//{
//public:
//    TEqpDvmpBasicInfo m_atEqpDvmpBasicInfo[2];    //�������VMP����Ϣ

//public:
//    TEqpDvmpInfo()
//	{
//		memset(m_atEqpDvmpBasicInfo, 0 ,sizeof(m_atEqpDvmpBasicInfo));
//	}
//}
//#ifndef WIN32
//__attribute__ ( (packed) ) 
//#endif
//;

//MpuBas��Ϣ�ṹ
struct TEqpMpuBasInfo: public TEqpBasicInfo,
                       public TEqpRunBrdInfo,
                       public TEqpSwitchBrdInfo,
					   public TEqpMapInfo
{
	u8 m_byStartMode; //����ģʽ(0: bap_4, 1:bap_2)
	
	TEqpMpuBasInfo()
	{
		SetStartMode (TYPE_MPUBAS); //Ĭ��ΪTYPE_MPUBAS
	}
	void SetStartMode(u8 byStartMode){ m_byStartMode = byStartMode; }
	u8   GetStartMode(){ return m_byStartMode; }
	void Print(void)
    {                           
        OspPrintf(TRUE, FALSE, "MpuBasId: %d, MpuBasAlias:%s, RunBrdId: %d, RunBrdIp.0x%x, MpuBasRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d, UsedMapNum: %d,StartMode:%d !\n",
			GetEqpId(), GetAlias(), 
			GetRunBrdId(), GetIpAddr(), GetEqpRecvPort(), 
			GetSwitchBrdId(), GetMcuRecvPort(), GetUsedMapNum(),GetStartMode());
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//EBAP��Ϣ�ṹ
struct TEqpEbapInfo: public TEqpBasicInfo,
                     public TEqpRunBrdInfo,
                     public TEqpSwitchBrdInfo,
					 public TEqpMapInfo
{
	void Print(void)
    {                           
        OspPrintf(TRUE, FALSE, "EbapId: %d, EbapAlias:%s, RunBrdId: %d, RunBrdIp.0x%x, EbapRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d, UsedMapNum: %d !\n",
			GetEqpId(), GetAlias(), 
			GetRunBrdId(), GetIpAddr(), GetEqpRecvPort(), 
			GetSwitchBrdId(), GetMcuRecvPort(), GetUsedMapNum());
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//EVPU��Ϣ�ṹ
struct TEqpEvpuInfo: public TEqpBasicInfo,
                     public TEqpRunBrdInfo,
                     public TEqpSwitchBrdInfo,
                     public TEqpMapInfo
{
	void Print(void)
    {                           
        OspPrintf(TRUE, FALSE, "EvpuId: %d, EvpuAlias:%s, RunBrdId: %d, RunBrdIp.0x%x, EvpuRecvPort: %d, SwitchBrdId: %d, McuRecvPort: %d, UsedMapNum: %d !\n",
			GetEqpId(), GetAlias(), 
			GetRunBrdId(), GetIpAddr(), GetEqpRecvPort(), 
			GetSwitchBrdId(), GetMcuRecvPort(), GetUsedMapNum());
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
struct TVmpAttachCfg
{
protected:
    u8   m_byIndex;					// �������(��1��ʼ, ���ֵΪ5)
    u8   m_byRemain;				// ����1
    u16  m_wRemain;					// ����2
    u32  m_dwBackgroudColor;		// ͼ�񱳾�ɫ(������)
    u32  m_dwFrameColor;			// ͼ��߿�ɫ(������)
    u32  m_dwSpeakerFrameColor;		// �����˱߿�ɫ(������)
    u32  m_dwChairFrameColor;		// ��ϯ�߿�ɫ(������)
	u8   m_byFontType;				// �ϳ���������
	u8   m_byFontSize;				// �ϳ������ֺ�
	u8   m_byAlignType;				// ���뷽ʽ
	u32  m_dwTextColor;			    // �ϳ�����������ɫ
	u32  m_dwTopicBGDColor;         // �ϳ����ⱳ��ɫ
	u32  m_dwDiaphaneity;			// �ϳ�����͸����
    s8   m_achAlias[MAX_VMPSTYLE_ALIASLEN]; // ����
public:
    TVmpAttachCfg() : m_byIndex(0),
                      m_byRemain(0),
                      m_wRemain(0),
                      m_dwBackgroudColor(0),
                      m_dwFrameColor(0),
                      m_dwSpeakerFrameColor(0),
                      m_dwChairFrameColor(0),
                      m_byFontType(0),
                      m_byFontSize(0),
                      m_byAlignType(0),
                      m_dwTextColor(0),
                      m_dwTopicBGDColor(0),
                      m_dwDiaphaneity(0)
    {
        memset(m_achAlias, 0, sizeof(m_achAlias)); 
    }
    void SetIndex(u8 byIndex){ m_byIndex = byIndex;    }
    u8   GetIndex(void) const { return m_byIndex;    }
    void SetBGDColor(u32 dwColor) { m_dwBackgroudColor = dwColor;    }
    u32  GetBGDColor(void) const { return m_dwBackgroudColor;    }
    void SetFrameColor(u32 dwColor){ m_dwFrameColor = dwColor;    }
    u32  GetFrameColor(void) const { return m_dwFrameColor;    }
    void SetSpeakerFrameColor(u32 dwColor){ m_dwSpeakerFrameColor = dwColor;    }
    u32  GetSpeakerFrameColor(void) const { return m_dwSpeakerFrameColor;    }
    void SetChairFrameColor(u32 dwColor){ m_dwChairFrameColor = dwColor;    }
    u32  GetChairFrameColor(void) const { return m_dwChairFrameColor;    }
    void SetFontType(u8 byType) { m_byFontType = byType;    }
    u8   GetFontType(void) const { return m_byFontType;    }
    void SetFontSize(u8 bySize) { m_byFontSize = bySize;    }
    u8   GetFontSize(void) const { return m_byFontSize;    }
    void SetAlignType(u8 byType) { m_byAlignType = byType;    }
    u8   GetAlignType(void) const { return m_byAlignType;    }
    void SetTextColor(u32 dwColor) { m_dwTextColor = dwColor;    }
    u32  GetTextColor(void) const { return m_dwTextColor;    }
    void SetTopicBGDColor(u32 dwColor) { m_dwTopicBGDColor = dwColor;    }
    u32  GetTopicBGDColor(void) const { return m_dwTopicBGDColor;    }
    void SetDiaphaneity(u32 dwDiaphaneity){ m_dwDiaphaneity = dwDiaphaneity;    }
    u32  GetDiaphaneity(void) const { return m_dwDiaphaneity;    }
    void SetAlias( LPCSTR lpszAlias)
    {
        if ( NULL != lpszAlias )
        {
            memcpy( m_achAlias, lpszAlias, MAX_VMPSTYLE_ALIASLEN );
            m_achAlias[sizeof(m_achAlias)-1] = '\0';
        }
    }
    LPCSTR GetAlias(void){ return m_achAlias;    }
}
#ifndef WIN32
__attribute__( (packed) )
#endif
;

//Ԥ������ͨ������������Դ�ṹ   jlb
struct THduChnlInfo
{
protected:
	u8   m_byChnlIdx;               //ͨ��������
	u8   m_byEqpId;					//�豸ID
	u8   m_byChnlVolumn;            //ͨ������
	BOOL32 m_bIsChnlBeQuiet;          //ͨ������
	s8   m_achEqpAlias[MAXLEN_EQP_ALIAS];   //�������
public:
	THduChnlInfo(){ SetNull(); }
    void SetNull(void)
	{
        memset( this, 0, sizeof(THduChnlInfo) );
	    return;
	}

	u8   GetChnlIdx(void) const { return m_byChnlIdx; }
	void SetChnlIdx(u8 val) { m_byChnlIdx = val; }

	u8   GetEqpId(void) const { return m_byEqpId; }
	void SetEqpId(u8 val) { m_byEqpId = val; }

	u8   GetChnlVolumn(void) const { return m_byChnlVolumn; }
	void SetChnlVolumn(u8 val) { m_byChnlVolumn = val; }

	BOOL32 GetIsChnlBeQuiet(void) const { return m_bIsChnlBeQuiet; }
	void SetIsChnlBeQuiet(BOOL32 val) { m_bIsChnlBeQuiet = val; }

	void SetSchemeAlias(LPCSTR pchEqpAlias)
    {
        memcpy(m_achEqpAlias, pchEqpAlias, MAXLEN_EQP_ALIAS);
        m_achEqpAlias[sizeof(m_achEqpAlias) - 1] = '\0';
    }

	LPCSTR GetSchemeAlias(void){ return m_achEqpAlias; }

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
; 

//HDUԤ�跽���ṹ   jlb
struct THduStyleInfo 
{
protected:
	u8   m_byStyleIdx;          //����Id��

	u8   m_byTotalChnlNum;      //������Ҫ��ͨ�������������п����õ����ͨ����
                                //���ݲ�ͬ�ķ�����Ϊ1��2��4������56�����56��
	THduChnlInfo m_atHduChnlTable[MAXNUM_HDUCFG_CHNLNUM];  //��ͨ����Ҫ��������Դ
	// m_bywidth * m_byHeight <= 56
	u8   m_byWidth;             //�������õĿ��
	u8   m_byHeight;            //�������õĸ߶�
    u8   m_byVolumn;            //����ͨ��ͳһ������С
	BOOL32 m_bIsBeQuiet;          //����ͨ���Ƿ�ͳһ����
	s8  m_achSchemeAlias[MAX_HDUSTYLE_ALIASLEN];   //Ԥ��������
	
public:
    THduStyleInfo(void) { SetNull(); }
	void	SetNull(void)  { memset(this, 0, sizeof(THduStyleInfo)); }
	BOOL32  IsNull(void) 
    { 
        THduStyleInfo tInfo;
        return (0 == memcmp(this, &tInfo, sizeof(THduStyleInfo)));
    }
    //patHduChnlTable  Ϊ����ָ�룬Ԫ�ظ���ΪMAXNUM_HDUCFG_CHNLNUM
	void GetHduChnlTable(THduChnlInfo *patHduChnlTable)    
	{
        memcpy(patHduChnlTable, m_atHduChnlTable, MAXNUM_HDUCFG_CHNLNUM*sizeof(THduChnlInfo));
        return;
	}
    //patHduChnlTable  Ϊ����ָ�룬Ԫ�ظ���ΪMAXNUM_HDUCFG_CHNLNUM
	BOOL32 SetHduChnlTable(THduChnlInfo *patHduChnlTable)
	{
		BOOL32 bRet = TRUE;
		if (NULL == patHduChnlTable)
		{
			bRet = FALSE;
			OspPrintf(TRUE, FALSE, "[mcustruct.h]:SetHduChnlTable()-->patHduChnlTable is NULL\n");
		}
		memcpy(m_atHduChnlTable, patHduChnlTable, MAXNUM_HDUCFG_CHNLNUM*sizeof(THduChnlInfo));
		return bRet;
	}

	u8   GetStyleIdx(void) const { return m_byStyleIdx; }
	void SetStyleIdx(u8 val) { m_byStyleIdx = val; }

	u8   GetTotalChnlNum(void) const { return m_byTotalChnlNum; }
	void SetTotalChnlNum(u8 val) { m_byTotalChnlNum = val; }

	u8   GetWidth(void) const { return m_byWidth; }
	void SetWidth(u8 val) { m_byWidth = val; }

	u8   GetHeight(void) const { return m_byHeight; }
	void SetHeight(u8 val) { m_byHeight = val; }

	u8   GetVolumn(void) const { return m_byVolumn; }
	void SetVolumn(u8 val) { m_byVolumn = val; }

	BOOL32 GetIsBeQuiet(void) const { return m_bIsBeQuiet; }
	void SetIsBeQuiet(BOOL32 val) { m_bIsBeQuiet = val; }

	void SetSchemeAlias(LPCSTR pchSchemeAlias)
    {
        memcpy(m_achSchemeAlias, pchSchemeAlias, MAX_HDUSTYLE_ALIASLEN);
        m_achSchemeAlias[sizeof(m_achSchemeAlias) - 1] = '\0';
    }
	LPCSTR GetSchemeAlias(void){ return m_achSchemeAlias; }
//     void    GetSchemeAlias(s8* pchSchemeAlias, u8 byBufferLen)
//     {
//         u8 byAliasLen = (byBufferLen < sizeof(m_achSchemeAlias) ) ? byBufferLen : sizeof(m_achSchemeAlias);
//         memcpy(pchSchemeAlias, m_achSchemeAlias, byAliasLen);
//     }

	inline BOOL32 operator == (const THduStyleInfo& tHduStyle )
	{
		BOOL32 bRet = TRUE;
		if( this == &tHduStyle )
		{
			return bRet;
		}
		
		if ( 0 != memcmp(this, &tHduStyle, sizeof(THduStyleInfo)) )
		{
			bRet = FALSE;
		}
		return bRet;
	}

    void Print(void)
	{
		OspPrintf(TRUE, FALSE, "[THduStyleInfo]: StyleIdx:%d, TotalChnlNum:%d, Width:%d, Height:%d, Volumn:%d, IsBeQuiet:%d\n",
			      GetStyleIdx(), GetTotalChnlNum(), GetWidth(), GetHeight(), GetVolumn(), GetIsBeQuiet());
		OspPrintf(TRUE, FALSE, "[THduStyleInfo]: Hdu Scheme Alias is:%s \n",m_achSchemeAlias);
	    
		OspPrintf(TRUE, FALSE, "[THduStyleInfo]:\n");
		u16 wIndex;
		for ( wIndex=0; wIndex<GetTotalChnlNum(); wIndex++)
	    {
			OspPrintf(TRUE, FALSE, "[%d]:  ChnlIdx:%d, EqpId:%d, ChnlVolumn:%d, IsChnlBeQuiet:%d\n",
				       m_atHduChnlTable[wIndex].GetChnlIdx(),
					   m_atHduChnlTable[wIndex].GetEqpId(),
					   m_atHduChnlTable[wIndex].GetChnlVolumn(),
    				   m_atHduChnlTable[wIndex].GetIsChnlBeQuiet());
	    }

		return;
	}
}
#ifndef WIN32
__attribute__( (packed) )
#endif
;

// ��ͬ����Ϣ
struct TNetSyncInfo
{
protected:
    u8	m_byMode;					// ��ͬ��ģʽ
    u8	m_byDTSlot;					// ��ͬ��ʹ�õ�DT���
    u8	m_byE1Index;				// ��ͬ��DT���ϵ�E1��
public:
    TNetSyncInfo() : m_byMode(0),
					 m_byDTSlot(0), 
                     m_byE1Index(0) {}
	void SetMode(u8 byMode) { m_byMode = byMode;	}
	u8   GetMode(void) const { return m_byMode;	}
	void SetDTSlot(u8 bySlot){ m_byDTSlot = bySlot;	}
	u8   GetDTSlot(void) const { return m_byDTSlot;	}
	void SetE1Index(u8 byIndex) { m_byE1Index = byIndex;	}
	u8   GetE1Index(void) const { return m_byE1Index;	}
	void Print()
	{
		OspPrintf( TRUE, FALSE, "Mode.%d, DTSlot.%d, E1Index.%d !\n",
								m_byMode, m_byDTSlot, m_byE1Index );
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TDSCModuleInfo
{
protected:
    u32 m_dwMcuInnerIp;          // DSC Ҫ���ӵ�MCU�ڲ�IP 
    u32 m_dwDscInnerIp;		     // DSC ���ڲ�IP
    u32 m_dwInnerIpMask;         // DSC �ڲ�IP����
    u16 m_wMcuListenPort;		 // DSC Ҫ���ӵ�MCU�˿�,һ����60000
	
	u8  m_byNetType;			 // MCU���ڵ���������
	u8  m_byLanMtProxyIpPos;	 // �����ն˴��������IP������λ��
	TMINIMCUNetParamAll m_tDscParamAll;	// ���е�ַ����DSC��IP��
	TMINIMCUNetParamAll m_tMcuParamAll;	// ������ӵ�ַ����MCU��0��IP��
	
    u8 m_byStartMp;
    u8 m_byStartMtAdp;
    u8 m_byStartGk;
    u8 m_byStartProxy;
    u8 m_byStartDcs;

public:
    TDSCModuleInfo():m_byStartMp(0),
                     m_byStartMtAdp(0),
                     m_byStartGk(0),
                     m_byStartProxy(0),
                     m_byStartDcs(0),
                     m_wMcuListenPort(MCU_LISTEN_PORT),
                     m_dwMcuInnerIp(0),
                     m_dwDscInnerIp(0),
                     m_dwInnerIpMask(0),
					 m_byNetType(NETTYPE_INVALID),
					 m_byLanMtProxyIpPos(0)
    {
		memset( &m_tDscParamAll, 0, sizeof(m_tDscParamAll) );
		memset( &m_tMcuParamAll, 0, sizeof(m_tMcuParamAll) );
	}
    
    void   SetMcuInnerIp(u32 dwIp) { m_dwMcuInnerIp = htonl(dwIp); }
    u32    GetMcuInnerIp() const { return ntohl(m_dwMcuInnerIp); }
    void   SetMcuPort(u16 wPort) { m_wMcuListenPort = htons(wPort); }
    u16    GetMcuPort() const { return ntohs(m_wMcuListenPort); }
    void   SetDscInnerIp(u32 dwIp) { m_dwDscInnerIp = htonl(dwIp); }
    u32    GetDscInnerIp() const { return ntohl(m_dwDscInnerIp); }
    void   SetInnerIpMask(u32 dwMask) { m_dwInnerIpMask = htonl(dwMask); }
    u32    GetInnerIpMask() const {return ntohl(m_dwInnerIpMask); }
	void   SetNetType(u8 byNetType) { m_byNetType = byNetType; }
	u8	   GetNetType(void) const { return m_byNetType; }
	void   SetLanMtProxyIpPos(u8 byLanMtProxyIpPos) { m_byLanMtProxyIpPos = byLanMtProxyIpPos; }
	u8	   GetLanMtProxyIpPos(void) const { return m_byLanMtProxyIpPos; }
    
    void SetCallAddrAll( TMINIMCUNetParamAll &tDscParamAll)
    {
        if ( tDscParamAll.IsValid() )
        {
            memcpy( &m_tDscParamAll, &tDscParamAll, sizeof(TMINIMCUNetParamAll) );
        }
    }
    void SetMcsAccessAddrAll( TMINIMCUNetParamAll &tMcuParamAll)
    {
        if ( tMcuParamAll.IsValid() )
        {
            memcpy( &m_tMcuParamAll, &tMcuParamAll, sizeof(TMINIMCUNetParamAll) );
        }
    }
    void GetCallAddrAll(TMINIMCUNetParamAll &tDscParamAll) const 
    { 
        memcpy( &tDscParamAll, &m_tDscParamAll, sizeof(TMINIMCUNetParamAll) ); 
    }
    void GetMcsAccessAddrAll(TMINIMCUNetParamAll &tMcuParamAll) const 
    { 
        memcpy( &tMcuParamAll, &m_tMcuParamAll, sizeof(TMINIMCUNetParamAll) ); 
    }
    u8 GetCallAddrNum(void) const 
    { 
        return m_tDscParamAll.GetNetParamNum(); 
    }
    u8 GetLanCallAddrNum(void) const
    {
        return m_tDscParamAll.GetLANParamNum();
    }
    u8 GetMcsAccessAddrNum(void) const 
    { 
        return m_tMcuParamAll.GetNetParamNum(); 
    }
    u8 GetLanMcsAccessAddrNum(void) const
    {
        return m_tMcuParamAll.GetLANParamNum();
    }
    
    void   SetStartMp(BOOL32 bStart){ m_byStartMp = bStart ? 1 : 0;    }
    BOOL32 IsStartMp() const { return m_byStartMp == 1 ? TRUE : FALSE;    }
    void   SetStartMtAdp(BOOL32 bStart){ m_byStartMtAdp = bStart ? 1 : 0;    }
    BOOL32 IsStartMtAdp() const { return m_byStartMtAdp == 1 ? TRUE : FALSE;    }
    void   SetStartGk(BOOL32 bStart) { m_byStartGk = bStart ? 1 : 0; }
    BOOL32 IsStartGk() const { return m_byStartGk == 1 ? TRUE : FALSE; }
    void   SetStartProxy(BOOL32 bStart) { m_byStartProxy = bStart ? 1 : 0; }
    BOOL32 IsStartProxy() const { return m_byStartProxy == 1 ? TRUE : FALSE; }
    void   SetStartDcs(BOOL32 bStart) { m_byStartDcs = bStart ? 1 : 0; }
    BOOL32 IsStartDcs() const { return m_byStartDcs == 1 ? TRUE : FALSE; }

    void   Print() const
    {
		u8 byLop = 0;
        OspPrintf( TRUE, FALSE, "TDSCModuleInfo: DscInnerIp: 0x%x, McuInnerIp: 0x%x:%d, InnerMask: 0x%x. Module: Mp.%d, MtAdp.%d, GK.%d, Proxy.%d, Dcs.%d\n", 
            m_dwDscInnerIp, m_dwMcuInnerIp, m_wMcuListenPort, m_dwInnerIpMask,
            m_byStartMp, m_byStartMtAdp, m_byStartGk, m_byStartProxy, m_byStartDcs);
		OspPrintf( TRUE, FALSE, "Net Type: ");
		switch( m_byNetType )
		{
		case NETTYPE_INVALID:
			OspPrintf(TRUE, FALSE, "INVALID\n");
			break;
		case NETTYPE_LAN:
			OspPrintf(TRUE, FALSE, "LAN\n");
			break;
		case NETTYPE_WAN:
			OspPrintf(TRUE, FALSE, "WAN\n");
			break;
		case NETTYPE_MIXING_ALLPROXY:
			OspPrintf(TRUE, FALSE, "MIXING_ALLPROXY\n");
			break;
		case NETTYPE_MIXING_NOTALLPROXY:
			OspPrintf(TRUE, FALSE, "MIXING_NOTALLPROXY\n" );
			break;
		default:
			break;
		}
		OspPrintf( TRUE, FALSE, "LanMtProxyIpPos: %d\n", m_byLanMtProxyIpPos );
		OspPrintf( TRUE, FALSE, "TDSCModuleInfo: DSC NetParam--\n");
		TMINIMCUNetParam tTemp;
		for ( byLop = 0; byLop < m_tDscParamAll.GetNetParamNum(); byLop++ )
		{
			if ( m_tDscParamAll.GetNetParambyIdx(byLop, tTemp) )
			{
				if ( tTemp.IsValid() )
				{
					OspPrintf(TRUE, FALSE, "NetType: %s\tIp: 0x%x\tIpMask: 0x%x\tGateway: 0x%x\tServerAlias: %s\n",
						tTemp.IsLan() ? "LAN" : "WAN",
						tTemp.GetIpAddr(), tTemp.GetIpMask(),
						tTemp.GetGatewayIp(), tTemp.GetServerAlias() );
				}
			}
		}
		OspPrintf( TRUE, FALSE, "TDSCModuleInfo: MCU NetParam--\n");
		for ( byLop = 0; byLop < ETH_IP_MAXNUM; byLop++ )
		{
			if ( m_tMcuParamAll.GetNetParambyIdx(byLop, tTemp) )
			{
				if ( tTemp.IsValid() )
				{
					OspPrintf(TRUE, FALSE, "NetType: %s\tIp: 0x%x\tIpMask: 0x%x\tGateway: 0x%x\tServerAlias: %s\n",
						tTemp.IsLan() ? "LAN" : "WAN",
						tTemp.GetIpAddr(), tTemp.GetIpMask(),
						tTemp.GetGatewayIp(), tTemp.GetServerAlias() );
				}
			}
		}
    }

	void  cosPrint() const
    {
		u8 byLop = 0;
        printf( "TDSCModuleInfo: DscInnerIp: 0x%x, McuInnerIp: 0x%x:%d, InnerMask: 0x%x. Module: Mp.%d, MtAdp.%d, GK.%d, Proxy.%d, Dcs.%d\n", 
            m_dwDscInnerIp, m_dwMcuInnerIp, m_wMcuListenPort, m_dwInnerIpMask,
            m_byStartMp, m_byStartMtAdp, m_byStartGk, m_byStartProxy, m_byStartDcs);
		printf( "Net Type: ");
		switch( m_byNetType )
		{
		case NETTYPE_INVALID:
			printf( "INVALID\n");
			break;
		case NETTYPE_LAN:
			printf( "LAN\n");
			break;
		case NETTYPE_WAN:
			printf( "WAN\n");
			break;
		case NETTYPE_MIXING_ALLPROXY:
			printf( "MIXING_ALLPROXY\n");
			break;
		case NETTYPE_MIXING_NOTALLPROXY:
			printf( "MIXING_NOTALLPROXY\n" );
			break;
		default:
			break;
		}
		printf( "LanMtProxyIpPos: %d\n", m_byLanMtProxyIpPos );
		printf( "TDSCModuleInfo: DSC NetParam--\n");
		TMINIMCUNetParam tTemp;
		for ( byLop = 0; byLop < m_tDscParamAll.GetNetParamNum(); byLop++ )
		{
			if ( m_tDscParamAll.GetNetParambyIdx(byLop, tTemp) )
			{
				if ( tTemp.IsValid() )
				{
					printf( "NetType: %s\tIp: 0x%x\tIpMask: 0x%x\tGateway: 0x%x\tServerAlias: %s\n",
						tTemp.IsLan() ? "LAN" : "WAN",
						tTemp.GetIpAddr(), tTemp.GetIpMask(),
						tTemp.GetGatewayIp(), tTemp.GetServerAlias() );
				}
			}
		}
		printf( "TDSCModuleInfo: MCU NetParam--\n");
		for ( byLop = 0; byLop < ETH_IP_MAXNUM; byLop++ )
		{
			if ( m_tMcuParamAll.GetNetParambyIdx(byLop, tTemp) )
			{
				if ( tTemp.IsValid() )
				{
					printf( "NetType: %s\tIp: 0x%x\tIpMask: 0x%x\tGateway: 0x%x\tServerAlias: %s\n",
						tTemp.IsLan() ? "LAN" : "WAN",
						tTemp.GetIpAddr(), tTemp.GetIpMask(),
						tTemp.GetGatewayIp(), tTemp.GetServerAlias() );
				}
			}
		}
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// ���� ��������MCS��IP��Ϣ  [04/24/2012 liaokang]
//��TRegedMcsIpInfo�ṹ�����ΪAgent�ڲ��������������ݴ���
struct TRegedMcsIpInfo
{
private:
    u32   m_dwMcsIpAddr;        //�������̨��Ip��ַ��������
public:
    void  SetMcsIpAddr(u32 dwMcsIpAddr){ m_dwMcsIpAddr = htonl(dwMcsIpAddr);}
    u32   GetMcsIpAddr( void ) const { return ntohl(m_dwMcsIpAddr); }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TBrdRegAck
{
private:
	u32 dwMpcIpAddrOther;
	u16 wMpcPortOther;
public:
	void SetOtherMpcIp(u32 dwMpcIp) { dwMpcIpAddrOther = htonl(dwMpcIp); }
	u32  GetOtherMpcIp(void) const { return ntohl(dwMpcIpAddrOther); }
	void SetOtherMpcPort(u16 wMpcPort) { wMpcPortOther = htons(wMpcPort); }
	u16  GetOtherMpcPort(void) const { return ntohs(wMpcPortOther); }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMpuEBapEVpuConfigAck
{
private:
	u8   m_byEqpType;
	u8   m_byEqpId;
    u8   m_byEqpId2;
    u16  m_wEqpRcvStartPort;
    u16  m_wEqpRcvStartPort2;
    s8 m_achAlias[MAXLEN_EQP_ALIAS+1]; // zw 20090309 ��ʱû���õ�
public:
	void SetEqpType( u8 byEqpType ) { m_byEqpType = byEqpType; }
	u8   GetEqpType( void ) const { return m_byEqpType; }
	void SetEqpId( u8 byEqpId) { m_byEqpId = byEqpId; }
	u8   GetEqpId( void ) const { return m_byEqpId; }
    void SetEqpId2( u8 byEqpId) { m_byEqpId2 = byEqpId; }
	u8   GetEqpId2( void ) const { return m_byEqpId2; }
    void SetEqpRcvStartPort( u16 wEqpRcvStartPort) { m_wEqpRcvStartPort = htons(wEqpRcvStartPort); }
	u16  GetEqpRcvStartPort( void ) const { return ntohs(m_wEqpRcvStartPort); }
    void SetEqpRcvStartPort2( u16 wEqpRcvStartPort) { m_wEqpRcvStartPort2 = htons(wEqpRcvStartPort); }
	u16  GetEqpRcvStartPort2( void ) const { return ntohs(m_wEqpRcvStartPort2); }
    void SetAlias( LPCSTR lpszAlias )
    {
        if ( NULL != lpszAlias )
        {
            strncpy( m_achAlias, lpszAlias, sizeof(m_achAlias) );
            m_achAlias[sizeof(m_achAlias)-1] = '\0';
        }
    }
    LPCSTR GetAlias( void ) { return m_achAlias; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;



//��TPfmConfInfo�ṹ�����ΪAgent�ڲ��������������ݴ���
struct TPfmConfInfo
{
	CConfId m_tConfId;
	u16 m_wMtNum;
	u8 m_byUsed;
	u8 m_byCfgIdx;
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//��TPfmEqpStat�ṹ�����ΪAgent�ڲ��������������ݴ���
struct TPfmEqpStat
{
	u16 m_wEqpUsedNum;
	u16 m_wEqpTotal;
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//��TMcuPfmInfo�ṹ�����ΪAgent�ڲ��������������ݴ���
struct TMcuPfmInfo
{
private:
	u8 m_byConfNum;
	TPfmConfInfo m_atPfmConfInfo[MAXNUM_MCU_CONF];
	TPfmEqpStat m_atEqpStat[SM_STAT_EQP_TYPE_MAX];
	u16 m_wAuthMTNum;
	u16 m_wAuthMTTotal;
	u8 m_byReserved1;
	u16 m_wReserved2;
public:

	u8 GetConfNum()
	{
		return m_byConfNum;
	}
	BOOL32 AddConf(CConfId tConfId)
	{
		u8 byIndex = 0;

		BOOL32 bRet = GetIdleConf(byIndex);

		if (bRet)
		{
			m_atPfmConfInfo[byIndex].m_byUsed = 1;
			m_atPfmConfInfo[byIndex].m_tConfId.SetConfId(tConfId);
			++m_byConfNum;
		}

		return bRet;
	}

	void DelConf(CConfId tConfId)
	{
		u8 byIndex = 0;
		if (FindConf(tConfId, byIndex))
		{
			memset(&m_atPfmConfInfo[byIndex], 0, sizeof(TPfmConfInfo));
			--m_byConfNum;
		}
	}

	//�ҵ�����True��������Ӧ��Index
	BOOL32 FindConf(CConfId tConfId, u8 &byIndexRet)
	{
		byIndexRet = 0;

		for (u8 byIndex = 0; byIndex < MAXNUM_MCU_CONF; ++byIndex)
		{
			if (tConfId == m_atPfmConfInfo[byIndex].m_tConfId)
			{
				byIndexRet = byIndex;
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL32 GetIdleConf(u8 &byIndexRet)
	{
		byIndexRet = 0;

		for (u8 byIndex = 0; byIndex < MAXNUM_MCU_CONF; ++byIndex)
		{
			if (m_atPfmConfInfo[byIndex].m_byUsed == 0)
			{
				byIndexRet = byIndex;
				return TRUE;
			}
		}

		return FALSE;
	}

	void SetMtNum(u8 byConfIndex, u16 wMtNum)
	{
		if (byConfIndex >= MAXNUM_MCU_CONF)
		{
			return;
		}

		m_atPfmConfInfo[byConfIndex].m_wMtNum = wMtNum;
	}

	void IncMtNum(u8 byConfIndex)
	{
		if (byConfIndex >= MAXNUM_MCU_CONF)
		{
			return;
		}
		
		++m_atPfmConfInfo[byConfIndex].m_wMtNum;
	}

	void DecMtNum(u8 byConfIndex)
	{
		if (byConfIndex >= MAXNUM_MCU_CONF)
		{
			return;
		}
		
		--m_atPfmConfInfo[byConfIndex].m_wMtNum;
	}

	u16 GetMtNum(u8 byConfIndex)
	{
		if (byConfIndex >= MAXNUM_MCU_CONF)
		{
			return 0;
		}

		return m_atPfmConfInfo[byConfIndex].m_wMtNum;
	}

	u16 GetTotalMtNum()
	{
		u16 wMtNum = 0;
		for (u8 byIndex = 0; byIndex < MAXNUM_MCU_CONF; ++byIndex)
		{
			if (m_atPfmConfInfo[byIndex].m_byUsed == 1)
			{
				wMtNum += m_atPfmConfInfo[byIndex].m_wMtNum;
			}
		}

		return wMtNum;
	}

	void SetEqpStat(u8 byEqpType, u16 wEqpUsedNum, u16 wEqpTotal)
	{
		if (byEqpType >= SM_STAT_EQP_TYPE_MAX)
		{
			return;
		}

		m_atEqpStat[byEqpType].m_wEqpUsedNum = wEqpUsedNum;
		m_atEqpStat[byEqpType].m_wEqpTotal = wEqpTotal;
	}


/*=============================================================================
  �� �� ���� GetEqpBuffer
  ��    �ܣ� ��ȡMCU����ʹ������Ϣ
  �㷨ʵ�֣� 
  ȫ�ֱ����� 
  ��    ���� s8* pEqpBuffer
  �� �� ֵ�� u16
  -----------------------------------------------------------------------------
  �޸ļ�¼:
  ��  ��        �汾        �޸���           �޸�����
  2010/11/29    5.0         ������             ����
=============================================================================*/
	u16 GetEqpBuffer(s8* pEqpBuffer)
	{
		u16 wSize = 0;
		if (pEqpBuffer == NULL)
		{
			return 0;
		}

		s8 *pBuff = pEqpBuffer;
		u8 byU32Len = sizeof(u32);

		//ǰ�棺ͳ���豸��������(4Byte)
		*(u32*)pBuff = MAKEDWORD((u16)SM_STAT_EQP_TYPE_MAX, 0);
		wSize += byU32Len;
		pBuff += byU32Len;

		//���棺������������ͳ�Ƹ��豸ʹ�����������(4Byte) + ʹ��״̬(4Byte = EqpTotal(H2B) + EqpUsedNum(L2B))
		for (u8 byIndex = 0; byIndex < SM_STAT_EQP_TYPE_MAX; ++byIndex)
		{
			*(u32*)pBuff = MAKEDWORD((u16)byIndex, 0);
			wSize += byU32Len;
			pBuff += byU32Len;
			*(u32*)pBuff = MAKEDWORD(m_atEqpStat[byIndex].m_wEqpUsedNum, m_atEqpStat[byIndex].m_wEqpTotal);
			wSize += byU32Len;
			pBuff += byU32Len;
		}

		return wSize;
	}

	void GetEqpStat(u8 byEqpType, u16 &wEqpUsedNum, u16 &wEqpTotal)
	{
		if (byEqpType >= SM_STAT_EQP_TYPE_MAX)
		{
			return;
		}
		
		wEqpUsedNum = m_atEqpStat[byEqpType].m_wEqpUsedNum;
		wEqpTotal = m_atEqpStat[byEqpType].m_wEqpTotal;
	}

	void SetAuthMTNum(u16 wAuthMTNum){ m_wAuthMTNum = wAuthMTNum; }
	u16 GetAuthMTNum() { return m_wAuthMTNum; }
	void SetAuthMTTotal(u16 wAuthTotal){ m_wAuthMTTotal = wAuthTotal; }
	u16 GetAuthMTTotal() { return m_wAuthMTTotal; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// miaoqingsong [2010-11-30] MCU8000G cpu��memory��Ϣ�ṹ��
typedef struct
{
	// Cpu���аٷֱ�0��100
	u8 m_by8KEIdlePercent;
}T8KECpuInfo;

typedef struct
{
	// �����ڴ��С(KByte)
	u32 m_dw8KEPhysicsSize;
	// ��ʹ���ڴ�(KBytes)
	u32 m_dw8KEAllocSize;
	// �����ڴ�(KBytes)
	u32 m_dw8KEFreeSize;
}T8KEMemInfo;

struct TUpdateFileInfo
{
private:
	u32 m_dwSvrIp; //ftp server Ip
	u16 m_wSrvPort;//ftp server port
	s8  m_achVerServUsername[MAXLEN_PWD + 4];    //�汾��������½�û���
	s8  m_achVerServPassword[MAXLEN_PWD + 4];     //�汾��������½����
	s8 m_aszSvrFilePath[KDV_MAX_PATH];
	s8 m_aszLocalFilePath[KDV_MAX_PATH];
	u32 m_dwUpdateState;
	u32 m_dwTransPercent;
	
public:
	TUpdateFileInfo()
		:m_dwSvrIp(0)
		,m_wSrvPort(0)
		,m_dwUpdateState(0)
		,m_dwTransPercent(0)
	{
		memset(m_aszSvrFilePath, 0, sizeof(m_aszSvrFilePath));
		memset(m_aszLocalFilePath, 0, sizeof(m_aszLocalFilePath));
		memset(m_achVerServUsername, 0, sizeof(m_achVerServUsername));
		memset(m_achVerServPassword, 0, sizeof(m_achVerServPassword));
	}
	
	void ChgUpdateState(u32 dwState)
	{
		m_dwUpdateState = dwState;
	}

	void ChgTransPercent(u32 dwTransPercent)
	{
		m_dwTransPercent = dwTransPercent;
	}

	u32 GetUpdateState()
	{
		return m_dwUpdateState;
	}

	u32 GetTransPercent()
	{
		return m_dwTransPercent;
	}

	void SetSvrFilePath(s8 *pszFilePath)
	{
		memcpy(m_aszSvrFilePath, pszFilePath, min(strlen(pszFilePath), KDV_MAX_PATH));
	}

	s8* GetSvrFilePath()
	{
		return m_aszSvrFilePath;
	}

	void SetLocalFilePath(s8 *pszFilePath)
	{
		memcpy(m_aszLocalFilePath, pszFilePath, min(strlen(pszFilePath), KDV_MAX_PATH));
	}
	
	s8* GetLocalFilePath()
	{
		return m_aszLocalFilePath;
	}

	BOOL32 IsSvrSameFile(s8 *pszFilePath)
	{
		return (memcmp(m_aszSvrFilePath, pszFilePath, strlen(pszFilePath)) == 0);
	}

	void SetServ(u32 dwSvrIp, u16 wSvrPort)
	{
		m_dwSvrIp = dwSvrIp;
		m_wSrvPort = wSvrPort;
	}
	
	u32 GetSvrIp()
	{
		return m_dwSvrIp;
	}
	
	u16 GetSvrPort()
	{
		return m_wSrvPort;
	}

	void setSvrUser(s8 *pszUser, s8 *pszPassword)
	{
		memcpy(m_achVerServUsername, pszUser, min(MAXLEN_PWD, strlen(pszUser)));
		memcpy(m_achVerServPassword, pszPassword, min(MAXLEN_PWD, strlen(pszPassword)));
	}

	s8 * GetSvrUser()
	{
		return m_achVerServUsername;
	}

	s8* GetSvrPassword()
	{
		return m_achVerServPassword;
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMcuUpdateInfo
{
private:
	u8 m_byUpdate; // �Ƿ������ı�ʾ 1������ 0��û������
	u8 m_byFileNum;// �����ļ���
	u8 m_byFtpGetEnd;

	u8 m_byLayerID;
	u8 m_bySlotID;
	u8 m_byType;
	TUpdateFileInfo m_tUpdateFile[MAX_UPDATE_NUM];

public:
	TMcuUpdateInfo()
		:m_byUpdate(0)
		,m_byFileNum(0)
		,m_byFtpGetEnd(0)
		,m_byLayerID(0)
		,m_bySlotID(0)
		,m_byType(0)
	{
		memset(m_tUpdateFile, 0, sizeof(m_tUpdateFile));
	}

	void SetBrdPosition(u8 byLayer, u8 bySlot, u8 byType)
	{
		m_byLayerID = byLayer;
		m_bySlotID = bySlot;
		m_byType = byType;
	}

	u8 GetLayerID()
	{
		return m_byLayerID;
	}

	u8 GetSlotID()
	{
		return m_bySlotID;
	}

	u8 GetType()
	{
		return m_byType;
	}

	void SetUpdateState(u8 bUpdate)
	{
		m_byUpdate = bUpdate;
	}

	u8 GetUpdateState()
	{
		return m_byUpdate;
	}

// 	BOOL32 IsUpdate()
// 	{
// 		return (m_byUpdate != SNMP_FILE_UPDATE_INIT) ? TRUE : FALSE;
// 	}
// 
// 	BOOL32 IsUpdateFail()
// 	{
// 		return (m_byUpdate == SNMP_FILE_UPDATE_FAILED) ? TRUE : FALSE;
// 	}
// 
// 	BOOL32 IsUpdateSuc()
// 	{
// 		return (m_byUpdate == SNMP_FILE_UPDATE_SUCCEED) ? TRUE : FALSE;
// 	}

	void AddUpdateInfo(TUpdateFileInfo& tUpdateInfo)
	{
		if (m_byFileNum >= MAX_UPDATE_NUM)
		{
			return;
		}
		memcpy(&m_tUpdateFile[m_byFileNum++], &tUpdateInfo, sizeof(TUpdateFileInfo));
	}

	u8 GetFileNum()
	{
		return m_byFileNum;
	}

	TUpdateFileInfo* GetUpdateFile(u8 byIndex)
	{
		if (byIndex >= m_byFileNum)
		{
			return NULL;
		}

		return &m_tUpdateFile[byIndex];
	}

	s8* GetLocalUpdateFileName(u8 byIndex)
	{
		if (byIndex >= m_byFileNum)
		{
			return NULL;
		}
		
		return m_tUpdateFile[byIndex].GetLocalFilePath();
	}

	void Clear()
	{
		m_byUpdate = 0;
		m_byFileNum = 0;
		m_byFtpGetEnd = 0;
		m_byLayerID = 0;
		m_bySlotID = 0;
		m_byType = 0;
		memset(m_tUpdateFile, 0, sizeof(m_tUpdateFile));
	}

	BOOL32 IsFtpEnd()
	{
		return (m_byFtpGetEnd == m_byFileNum);
	}

	BOOL32 FtpGet();
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
//[2011/02/11 zhushz] mcs�޸�mcu��������
struct TMcuNewNetCfg
{
private:
	u32 m_dwNewMpcIp;
	u8	m_byNewMpcInterface;
	u32 m_dwNewMpcMaskIp;
	u32 m_wNewMpcDefGateWay;
public:
	//new ip
	u32	GetNewMpcIP()
	{
		return m_dwNewMpcIp;
	}
	void SetNewMpcIP(u32 dwNewIP)
	{
		m_dwNewMpcIp = dwNewIP;
		return;
	}
	//new interface
	u8 GetNewMpcInterface()
	{
		return m_byNewMpcInterface;
	}
	void SetNewMpcInterface(u8 byNewInterface)
	{
		m_byNewMpcInterface = byNewInterface;
		return;
	}
	//new Mask Ip
	u32 GetNewMpcMaskIp()
	{
		return m_dwNewMpcMaskIp;
	}
	void SetNewMpcMaskIp(u32 dwNewMpcMaskIp)
	{
		m_dwNewMpcMaskIp = dwNewMpcMaskIp;
		return;
	}
	//new GateWay
	u32 GetNewMpcDefGateWay()
	{
		return m_wNewMpcDefGateWay;
	}
	void SetNewMpcDefGateWay(u32 wNewMpcDefGateWay)
	{
		m_wNewMpcDefGateWay = wNewMpcDefGateWay;
		return;
	}

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

#ifdef WIN32
#pragma pack( pop )
#endif

#endif  // _AGENT_COMMONSTRUCT_H_
