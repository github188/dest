/*****************************************************************************
   ģ����      : KDVMCU Agent
   �ļ���      : mcuagentstruc.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: KDVMCU agent struct
   ����        : ����
   �汾        : V4.0  Copyright( C) 2006-2008 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾     �޸���       �޸�����
   2002/01/25  0.9      ����         ����
   2003/07/15  1.1      jianghy      ��������
   2003/11/11  3.0      jianghy      3.0ʵ��
   2004/11/10  3.6      libo         �½ӿ��޸�
   2004/11/29  3.6      libo         ��ֲ��Linux
   2005/08/17  4.0      liuhuiyun    ����
   2006/11/02  4.0      �ű���       ���ݽṹ�Ż�
******************************************************************************/
#ifndef H_MCUAGTSTRUC_H
#define H_MCUAGTSTRUC_H

#include "agentcommon.h"

#ifdef WIN32
#pragma pack( push )
#pragma pack( 1 )
#endif


#define  VERSION_CONFIGURE36            (u8)36   // 3.6�汾
#define  VERSION_CONFIGURE40            (u8)40   // 4.0�汾
#define  VERSION_CONFIGURE60			(u8)60	 // 6.0�汾	

#define  LOG_OFF                        (u8)0    
#define  LOG_ERROR                      (u8)1
#define  LOG_WARN                       (u8)2
#define  LOG_INFORM                     (u8)3
#define  LOG_VERBOSE                    (u8)4

// �����ַ���
#define ERR_STRING_SYS                  (LPCSTR)"Fail to read system infomation"
#define ERR_STRING_LOACL                (LPCSTR)"Fail to read local infomation"
#define ERR_STRING_NETWORK              (LPCSTR)"Fail to read network infomation"
#define ERR_STRING_TRAP                 (LPCSTR)"Fail to read snmp infomation"
#define ERR_STRING_BOARD                (LPCSTR)"Fail to read board infomation"
#define ERR_STRING_MIX                  (LPCSTR)"Fail to read mixer infomation"
#define ERR_STRING_TVWALL               (LPCSTR)"Fail to read tvwall infomation"
#define ERR_STRING_RECORDER             (LPCSTR)"Fail to read recorder infomation" 
#define ERR_STRING_BAS                  (LPCSTR)"Fail to read bas infomation"
#define ERR_STRING_BASHD                (LPCSTR)"Fail to read basHD infomation"
#define ERR_STRING_VMP                  (LPCSTR)"Fail to read vmp infomation"
#define ERR_STRING_MPW                  (LPCSTR)"Fail to read mpw infomation"
#define ERR_STRING_PRS                  (LPCSTR)"Fail to read prs infomation"
#define ERR_STRING_DATACONF             (LPCSTR)"Fail to read data conf infomation"
#define ERR_STRING_NETSYSC              (LPCSTR)"Fail to read netsysc infomation"
#define ERR_STRING_QOS                  (LPCSTR)"Fail to read Qos infomation"
#define ERR_STRING_VMPATTACH            (LPCSTR)"Fail to read vmp attach infomation" 

// �ڲ������붨��

#define     ERR_AGENT_BGN               (u8)100

#define  ERR_AGENT_READSYSCFG		    (ERR_AGENT_BGN + 7)  // ��ϵͳ��Ϣ����
#define  ERR_AGENT_READNETCFG		    (ERR_AGENT_BGN + 8)  // �������������
#define  ERR_AGENT_READLOCALCFG		    (ERR_AGENT_BGN + 9)  // ��������Ϣ����
#define  ERR_AGENT_READTRAPCFG		    (ERR_AGENT_BGN + 10) // ��Trap��Ϣ����
#define  ERR_AGENT_READBOARDCFG		    (ERR_AGENT_BGN + 11) // ��������Ϣ����
#define  ERR_AGENT_READMIXCFG		    (ERR_AGENT_BGN + 12) // ������������
#define  ERR_AGENT_READTVCFG		    (ERR_AGENT_BGN + 13) // ������ǽ����
#define  ERR_AGENT_READRECORDERCFG      (ERR_AGENT_BGN + 14) // ��¼�����Ϣ����
#define  ERR_AGENT_READBASCFG           (ERR_AGENT_BGN + 15) // �������������
#define  ERR_AGENT_READVMPCFG           (ERR_AGENT_BGN + 16) // �����渴�ϴ���
#define  ERR_AGENT_READMPWCFG           (ERR_AGENT_BGN + 17) // ���໭�渴��������
#define  ERR_AGENT_READVMPATTACHCFG     (ERR_AGENT_BGN + 18) // �����渴�ϸ�����Ϣ����
#define  ERR_AGENT_READPRSCFG           (ERR_AGENT_BGN + 19) // ��Prs��Ϣ����
#define  ERR_AGENT_READQOSCFG           (ERR_AGENT_BGN + 20) // ��Qos��Ϣ����
#define  ERR_AGENT_READNETSYSCCFG       (ERR_AGENT_BGN + 21) // ����ͬ����Ϣ����
#define  ERR_AGENT_READDATACONFCFG      (ERR_AGENT_BGN + 22) // �����ݻ�����Ϣ����
#define  ERR_AGENT_READ8000BINFO        (ERR_AGENT_BGN + 23) // ��8000B DSC���ô���
#define  ERR_AGENT_READBASHDCFG         (ERR_AGENT_BGN + 24) // ������bas����
#define  ERR_AGENT_READHDUSCHEMECFG     (ERR_AGENT_BGN + 25) // ��hduԤ����Ϣ����     //4.6   jlb
#define  ERR_AGENT_READHDUCFG		    (ERR_AGENT_BGN + 13) // ��hdu����


#define  ERR_AGENT_BOARDDEXIST          (ERR_AGENT_BGN + 30) // �õ����Ѿ�����
#define  ERR_AGENT_BOARDDNOTEXIST       (ERR_AGENT_BGN + 31) // �õ��岻����
#define  ERR_AGENT_BOARDDUPREG          (ERR_AGENT_BGN + 32) // �ظ�ע��
#define  ERR_AGENT_CREATECFGFILE	    (ERR_AGENT_BGN + 33) // ���������ļ�ʧ��
#define  ERR_AGENT_WRITECFGFILE		    (ERR_AGENT_BGN + 34) // д�����ļ�ʧ��
#define  ERR_AGENT_BRDNOTEXIST		    (ERR_AGENT_BGN + 35) // ���岻����
#define  ERR_AGENT_EQPNOTEXIST		    (ERR_AGENT_BGN + 36) // ���費����
#define  ERR_AGENT_NULLPARAM		    (ERR_AGENT_BGN + 37) // ��������
#define  ERROR_AGENT_SNMP_GETVALUE	    (ERR_AGENT_BGN + 38) // Snmpȡֵ����
#define  ERR_AGENT_CREATECONFFILE	    (ERR_AGENT_BGN + 39) // ���������ļ�����

// �ڲ��궨��
#define  ENTRY_KEY					    (LPCSTR)"Entry"      // Entry �ַ���
#define  ENTRY_NUM					    (LPCSTR)"EntryNum"   // Entry num �ַ���

#define  MAX_ERROR_NUM					(u8)20		// ��������
#define  MAX_PRIEQP_NUM					(u8)16      // ��������� 
//4.6�¼����� jlb
#define  MAX_PERIHDU_NUM                (u8)70      // ���HDU����
//#define  MAX_PERIDVMP_NUM                (u8)8       //���Dvmp����
#ifndef _MINIMCU_
//  [12/6/2010 chendaiwei]����������һ����֧��IS2.2��
#define  MAX_BOARD_NUM					(u8)(((16*4) + (7*10))+1)  // ��󵥰���
#else
#define  MAX_BOARD_NUM					(u8)(16 + (7*10))      // ��󵥰���
#endif

#define  MAX_ALIAS_LENGTH				(u8)32      // ����������
#define  MAX_PORT_LENGTH				(u8)5       // ���˿��ַ�������

#define  CONF_MONTH_OLD					(u8)10      // �������ļ��·�
#define  CONF_YEAR_OLD					(u16)2005   // �������ļ����


#define  MAX_SOFT_VER_LEN				(u8)128      // ����汾��Ϣ
#define  MAX_CONFIGURE_LENGTH			(u8)64       // Mcu �����ļ��汾����

#define  MAX_IPSTR_LEN					(u8)64		 // ��ַ�ַ�����󳤶�
// change for IS2.2 [05/04/2012 liaokang]
#define  MAX_BOARD_LED_NUM				(u8)32       // ����������Ŀ
#define  MAX_MODULE_NAME_LEN			(u8)256		 // ����ģ��������
// add for IS2.2 [05/04/2012 liaokang]
#define  MAX_BOARD_ETHPORT_NUM			(u8)27		 // �������������Ŀ

#define  QOS_IP_NOALL					(u8)0        // ȫ����ѡ
#define  QOS_IP_LEASTDELAY				(u8)1        // ��С�ӳ�
#define  QOS_IP_THRUPUT					(u8)2        // ���������
#define  QOS_IP_RELIABLITY				(u8)3        // ��߿ɿ���
#define  QOS_IP_EXPENSES				(u8)4        // ��С����

#define  DATA_LVL_IP					(u8)0        // QosΪIp ʱ����Ĭ�ϵȼ�
#define  VIDEO_LVL_IP					(u8)3        // QosΪIp ʱ��ƵĬ�ϵȼ�
#define  AUDIO_LVL_IP					(u8)4        // QosΪIp ʱ��ƵĬ�ϵȼ�
#define  SIGNAL_LVL_IP					(u8)7        // QosΪIp ʱ�ź�Ĭ�ϵȼ�

#define  MAX_IPSERVICE_LVL				(u8)4        // QosΪIp ����ʱIp����ȼ����ֵ
#define  MIN_IPSERVICE_LVL				(u8)0        // QosΪIp ����ʱIp����ȼ���Сֵ
#define  MIN_IPSERVICE_STR_LEN			(u8)10		 // QosΪIp ����ʱIp�����ַ�������

#define  MAX_ERROR_NUM			        (u8)20		  // �����ô�����Ŀ
#define  MAX_ERROR_STR_LEN		        (u8)128		  // ����������󳤶�

#define  MAX_ALARMOBJECT_NUM            (u8)5		  // �澯������Ŀ
#define  MAX_ALARM_TIME_LEN             (u8)32        // �澯ʱ�䳤��
#define  MAX_FTPFILENAME_LEN            (u8)80	      // ftp �ļ�����󳤶�
#define  MAXNUM_MCU_ALARM		        (u32)((64*32+128)/2) // MCU�е����澯��(ȡʵ�����澯����һ��)

#define  UPDATE_NMS                     (u8)1         // ���ܷ��𵥰�����
#define  UPDATE_MCS                     (u8)2         // ��ط��𵥰�����

#define  BRD_LAYER_CUSTOM               (u8)0xFE      // �ر�ĵ����ţ���ʾ�ǻ���ʽ����(����Ϊ0-3)

// ���������ַ���
#define BOARD_TYPE_MPC                  (LPCSTR)"MPC"
//  [1/21/2011 chendaiwei]����MPC2���������ַ���
#define BOARD_TYPE_MPC2                 (LPCSTR)"MPC2"
#define BOARD_TYPE_CRI                  (LPCSTR)"CRI"
//  [1/21/2011 chendaiwei]����CRI2���������ַ���
#define BOARD_TYPE_CRI2                 (LPCSTR)"CRI2"
#define BOARD_TYPE_DTI                  (LPCSTR)"DTI"
#define BOARD_TYPE_DIC                  (LPCSTR)"DIC"
#define BOARD_TYPE_DRI                  (LPCSTR)"DRI"
//  [1/21/2011 chendaiwei]����DRI2���������ַ���
#define BOARD_TYPE_DRI2                 (LPCSTR)"DRI2"
#define BOARD_TYPE_DRI16                (LPCSTR)"DRI16"
#define BOARD_TYPE_MMP                  (LPCSTR)"MMP"
#define BOARD_TYPE_VPU                  (LPCSTR)"VPU"
#define BOARD_TYPE_DEC5                 (LPCSTR)"DEC5"
#define BOARD_TYPE_VAS                  (LPCSTR)"VAS"
#define BOARD_TYPE_IMT                  (LPCSTR)"IMT"
#define BOARD_TYPE_APU                  (LPCSTR)"APU"
#define BOARD_TYPE_EAPU                 (LPCSTR)"EAPU"
#define BOARD_TYPE_DSI                  (LPCSTR)"DSI"
#define BOARD_TYPE_MPCB                 (LPCSTR)"MPCB"
#define BOARD_TYPE_MPCC                 (LPCSTR)"MPCC"
#define BOARD_TYPE_MDSC			        (LPCSTR)"MDSC"
#define BOARD_TYPE_HDSC                 (LPCSTR)"HDSC"
#define BOARD_TYPE_DSC			        (LPCSTR)"DSC"
//4.6�汾�¼� jlb
#define BOARD_TYPE_HDU			        (LPCSTR)"HDU"
#define BOARD_TYPE_HDU_L			    (LPCSTR)"HDU_L"
#define BOARD_TYPE_MPU			        (LPCSTR)"MPU"
#define BOARD_TYPE_EBAP			        (LPCSTR)"EBAP"
#define BOARD_TYPE_EVPU			        (LPCSTR)"EVPU"
#define BOARD_TYPE_UNKNOW               (LPCSTR)"Unknow"
//[2011/01/25 zhushz]IS2.x����֧��
#define BOARD_TYPE_IS21					(LPCSTR)"IS21"
#define BOARD_TYPE_IS22					(LPCSTR)"IS22"
// 4.7֧�� [1/31/2012 chendaiwei]
#define BOARD_TYPE_HDU2					(LPCSTR)"HDU2"
#define BOARD_TYPE_MPU2					(LPCSTR)"MPU2"
#define BOARD_TYPE_MPU2ECARD	        (LPCSTR)"MPU2_ECARD"
#define BOARD_TYPE_APU2					(LPCSTR)"APU2"
#define BOARD_TYPE_HDU2_L				(LPCSTR)"HDU2-L"
#define BOARD_TYPE_HDU2_S				(LPCSTR)"HDU2-S"

/* ������ݽṹ���� */
struct tTagBrdPos{
    u8 byBrdID;                      /* ����ID�� */
    u8 byBrdLayer;                   /* �������ڲ�ţ��豸��֧��ʱ����0������0�㴦�� */
    u8 byBrdSlot;                    /* �������ڲ�λ�ţ��豸��֧��ʱ����0������0�۴���;
									 �����豸������֮�֣���slot�����֣�0=����1=�� */
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
typedef tTagBrdPos TBrdPos;

// add EthPort [05/04/2012 liaokang]
// �������ñ���ṹ�� �ڷ�Ƕ��ʽ(8260)�����ֻ�б�ź�IP��Ч
struct TEqpBrdCfgEntry : public TBoardInfo
{
protected:    
    s8  m_achVersion[MAX_SOFT_VER_LEN+1];		/*��������汾*/
    s8  m_achModule[MAX_MODULE_NAME_LEN+1];	    /*�����ģ����Ϣ*/
    s8  m_achPanelLed[MAX_BOARD_LED_NUM+1];		/*����ĵ�״̬ 1 �� 2 �� 3 ���� 4 ���� 5 ÿ��2������*/    
    s8  m_achEthPort[MAX_BOARD_ETHPORT_NUM+1];  /*���������˫��״̬*/  // add [05/04/2012 liaokang]
public:
    TEqpBrdCfgEntry() {	SetEntParamNull();	}
    void SetEntParamNull()
    {
        memset(m_achVersion, 0, sizeof(m_achVersion));
        memset(m_achModule, 0, sizeof(m_achModule));
        memset(m_achPanelLed, 0, sizeof(m_achPanelLed));
        memset(m_achEthPort, 0, sizeof(m_achEthPort));
    }
    void SetVersion(LPCSTR lpszVerion) 
    {
        if ( NULL != lpszVerion )
        {
            strncpy( m_achVersion, lpszVerion, sizeof(m_achVersion) );
            m_achVersion[sizeof(m_achVersion)-1] = '\0';
        }
    }
    LPCSTR GetVersion(void) { return m_achVersion;    }
    void SetModule(LPCSTR lpszModule) 
    {
        if ( NULL != lpszModule )
        {
            strncpy( m_achModule, lpszModule, sizeof(m_achModule) );
            m_achModule[sizeof(m_achModule)-1] = '\0';
        }
    }
    LPCSTR GetModule(void) { return m_achModule;    }      

    void SetPanelLed(LPCSTR lpszPanelLed) 
    {
        if ( NULL != lpszPanelLed )
        {
            /*strncpy( m_achPanelLed, lpszPanelLed, sizeof(m_achPanelLed) );*/
            memcpy( m_achPanelLed, lpszPanelLed, sizeof(m_achPanelLed) ); // MLNK��ʼΪ0 [2012/05/30 liaokang]
            m_achPanelLed[sizeof(m_achPanelLed)-1] = '\0';
        }
    }

	/*
	void SetPanelLed(u8 *pbyPanelLed, u8 byCount) 
    {
		if (byCount > MAX_BOARD_LED_NUM)
		{
			return;
		}

        if ( NULL != pbyPanelLed )
        {
            memcpy( m_achPanelLed, pbyPanelLed, byCount * sizeof(u8) );
            m_achPanelLed[MAX_BOARD_LED_NUM] = '\0';
        }
    }
	*/

    LPCSTR GetPanelLed(void) { return m_achPanelLed; }
    
    //  add EthPort [05/04/2012 liaokang]
    void SetEthPort(LPCSTR lpszNetPort) 
    {
        if ( NULL != lpszNetPort )
        {
            memcpy( m_achEthPort, lpszNetPort, sizeof(m_achEthPort) );  // ����״̬
            m_achEthPort[sizeof(m_achEthPort)-1] = '\0'; // ������            
        }
    }
    
    LPCSTR GetEthPort(void) { return m_achEthPort; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// ��ͬ�����ò��� 
struct TEqpNetSync : public TNetSyncInfo
{
protected:
	u8	m_bySourceType;		/*��ͬ��Դѡ��*/
    u8	m_byClockType;		/*ʱ��ѡ��*/
    u8	m_byState;			/*��ͬ����״̬*/	
public:
	TEqpNetSync() : m_bySourceType(0),
					m_byClockType(0),
					m_byState(0){}
	void SetSourceType(u8 byType) { m_bySourceType = byType;	}
	u8   GetSourceType(void) const { return m_bySourceType;	}
	void SetClockType(u8 byType){ m_byClockType = byType;	}
	u8   GetClockType(void) const { return m_byClockType;	}
	void SetState(u8 byState) { m_byState = byState;	}
	u8   GetState(void) const { return m_byState;	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


/************************************************************************/
/* ���貿��                                                             */
/************************************************************************/

//�����������������
struct TEqpEntryParam
{
protected:
    u32 m_dwMcuIp;		    /*������Ҫ���ӵ�MCU��IP��ַ ������*/
    u16 m_wMcuPort;			/*������Ҫ���ӵ�MCU�Ķ˿ں� ������*/
    u8  m_byMcuId;		    /*������Ҫ���ӵ�MCU���*/
    
    u8  m_byType;			/*�����������ֵ*/
    u8  m_byConnectState;	/*�����������״̬*/
    u8  m_byExist;			/*�Ƿ�ʹ�øñ���*/
    
public:
    TEqpEntryParam() : m_dwMcuIp(0),
                       m_wMcuPort(0),
                       m_byMcuId(0),
                       m_byType(0),
                       m_byConnectState(0),
                       m_byExist(0){}
    void SetMcuIp(u32 dwMcuIp) { m_dwMcuIp = htonl(dwMcuIp);   }
    u32  GetMcuIp(void) const { return ntohl(m_dwMcuIp);  }
    void SetMcuPort(u16 wMcuPort) { m_wMcuPort = htons(wMcuPort);  }
    u16  GetMcuPort(void) const { return ntohs(m_wMcuPort);   }
    void SetMcuId(u8 byMcuId) { m_byMcuId = byMcuId;   }
    u8   GetMcuId(void) const { return m_byMcuId;    }
    void SetType(u8 byType) { m_byType = byType;  }
    u8   GetType(void) const { return m_byType;   }
    void SetState( u8 byState ) {  m_byConnectState = byState;   }
    u8   GetState(void) const { return m_byConnectState;  }
    void SetExist(BOOL32 bExist) { m_byExist = bExist ? 1 : 0;   }
    BOOL32 GetExist(void) { return m_byExist == 1 ? TRUE : FALSE;  }

    void SetDefaultEntParam( u8 byEqpType, u32 dwMcuIp, u8 byState = 0 )
    {
        m_byType   = byEqpType;
        m_dwMcuIp  = dwMcuIp;
        m_byConnectState  = byState;
        m_byMcuId  = LOCAL_MCUID;
        m_wMcuPort = MCU_LISTEN_PORT;
        m_byExist  = 1;
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// [1]. ����������
struct TEqpBasEntry : public TEqpBasInfo, 
                      public TEqpEntryParam
{
protected:
    u8  m_byMaxAdpChannel;		/*������������ÿ��MAP�������ͨ��*/
public:
    TEqpBasEntry() : m_byMaxAdpChannel(0){}
    void SetAdpChnNum(u8 byChnNum){ m_byMaxAdpChannel = byChnNum;  }
    u8   GetAdpChnNum(void) const { return m_byMaxAdpChannel;  }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// [1.1]. ����bas
struct TEqpBasHDEntry : public TEqpBasHDInfo, 
                      public TEqpEntryParam
{
protected:
    u8  m_byMaxAdpChannel;		/*������������ÿ��MAP�������ͨ��*/
public:
    TEqpBasHDEntry() : m_byMaxAdpChannel(0){}
    void SetAdpChnNum(u8 byChnNum){ m_byMaxAdpChannel = byChnNum;  }
    u8   GetAdpChnNum(void) const { return m_byMaxAdpChannel;  }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// [2]. ������
struct TEqpMixerEntry : public TEqpMixerInfo,
                        public TEqpEntryParam 
{
protected:
    u8  m_byMaxMixGroupNum;	/*�����������*/
	u8	m_byIsSendRedundancy;	// �Ƿ����෢��, zgc, 2007-0725
public:
    TEqpMixerEntry() : m_byMaxMixGroupNum(0){}
    void SetMaxMixGrpNum(u8 byMaxGrpNum) { m_byMaxMixGroupNum = byMaxGrpNum;   }
    u8   GetMaxMixGrpNum(void) const { return m_byMaxMixGroupNum;    }
	void SetIsSendRedundancy( BOOL32 IsRedundancy ) { m_byIsSendRedundancy = IsRedundancy ? 1 : 0; }
	BOOL32 IsSendRedundancy(void) const { return ( 1 == m_byIsSendRedundancy ) ? TRUE : FALSE; } 
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// [3]. �໭�����ǽ
struct TEqpMPWEntry : public TEqpMpwInfo,
                      public TEqpEntryParam
{
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// [4]. ���ֵ���ǽ 
struct TEqpTVWallEntry : public TEqpTVWallInfo,
                         public TEqpEntryParam
{
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// [5]. ���ش���
struct TEqpPrsEntry : public TEqpPrsInfo,
                      public TEqpEntryParam
{
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// [6]. ͼ��ϳ���
struct TEqpVMPEntry : public TEqpVMPInfo,
                      public TEqpEntryParam
{
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// [7]. ����¼�����
struct TEqpRecEntry : public TEqpRecInfo,
                      public TEqpEntryParam
{
protected:
    u8  m_byRecordChannelNum;	/*���ַ���¼�����¼��ͨ����*/
    u8  m_byPlayChannelNum;		/*���ַ���¼����ķ���ͨ����*/
public:
    TEqpRecEntry() : m_byRecordChannelNum(0),
                     m_byPlayChannelNum(0){}
    void SetRecChnNum(u8 byRecChnNum) { m_byRecordChannelNum = byRecChnNum;   }
    u8   GetRecChnNum(void) const { return m_byRecordChannelNum;   }
    void SetPlayChnNum(u8 byPlayChnNum) { m_byPlayChannelNum = byPlayChnNum;   }
    u8   GetPlayChnnlNum(void) const { return m_byPlayChannelNum;   }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


// [8]. ���ֻ��������
struct TEqpDCSEntry
{
public:
    TEqpDCSEntry() : m_dwDcsIp(0),
                     m_byDcsConnState(0){}
public:
	u32  m_dwDcsIp;			    /*���ֻ�����������豸ID*/
	u8   m_byDcsConnState;		/*���ֻ��������������״̬*/
};

//4.6���¼��������ṹ
//HDU������뿨
struct TEqpHduEntry : public TEqpHduInfo,
                      public TEqpEntryParam
{
    
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//��VMP
struct TEqpSvmpEntry: public TEqpSvmpInfo,
                      public TEqpEntryParam
{

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//˫VMP
struct TEqpDvmpBasicEntry: public TEqpDvmpBasicInfo,
                      public TEqpEntryParam
{

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//˫VMP
struct TEqpMpuBasEntry: public TEqpMpuBasInfo,
                           public TEqpEntryParam
{

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//Ebap
struct TEqpEbapEntry: public TEqpEbapInfo,
                      public TEqpEntryParam
{
	
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//Evpu
struct TEqpEvpuEntry: public TEqpEvpuInfo,
                      public TEqpEntryParam
{
	
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//Vrs¼��������
struct TEqpVrsRecEntry: public TEqpVrsRecCfgInfo
{

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
/*------------------------------------------------------------- 
*ϵͳ���������Ϣ 
*-------------------------------------------------------------*/
struct TMcuSystem
{
protected:
    u8  m_byState;										/*ϵͳ״̬*/
    s8  m_achTime[MAX_ALARM_TIME_LEN+1];				/*ϵͳʱ��*/
    s8  m_achFtpFileName[MAX_FTPFILENAME_LEN+1];		/*FTP�ļ���*/
    s8  m_achConfigVersion[MAX_CONFIGURE_LENGTH];	    /*���ð汾*/
    s8  m_achSoftwareVersion[MAX_SOFT_VER_LEN+1];		/*����汾*/
	u8  m_byIsMcuConfiged;						        /*MCU�������ñ�ʶ*/
	u8	m_byMcuCfgInfoLevel;							/*MCU������Ϣ����, zgc, 2007-03-19*/
	s8  m_achCompileTime[MAX_ALARM_TIME_LEN+1];				/*ϵͳʱ��*/
    
public:
    TMcuSystem(void){   memset(this, 0, sizeof(TMcuSystem));	}
    void SetState(u8 byState) { m_byState = byState;    }
    u8   GetState(void) const { return m_byState;    }

    void SetTime(s8 *pszTime)
    {
        if ( NULL != pszTime )
        {
            memcpy( &m_achTime, pszTime, sizeof(m_achTime) );
            m_achTime[sizeof(m_achTime)-1] = '\0';
        }
        return;
    }
    LPCSTR GetTime() { return m_achTime;    }
    void SetFtpFileName(LPCSTR lpszStr)
    {
        if ( NULL != lpszStr )
        {
            strncpy( m_achFtpFileName, lpszStr, sizeof(m_achFtpFileName) );
            m_achFtpFileName[sizeof(m_achFtpFileName)-1] = '\0';
        }
    }
    LPCSTR GetFtpFileName() { return m_achFtpFileName;    }
    void SetConfigVer(s8* lpszStr)
    {
        if ( NULL != lpszStr )
        {
            strncpy( m_achConfigVersion, lpszStr, sizeof(m_achConfigVersion) );
            m_achConfigVersion[sizeof(m_achConfigVersion)-1] = '\0';
        }
    }
    s8* GetConfigVer() { return m_achConfigVersion;    }
    void SetSoftWareVer(LPCSTR lpszStr)
    {
        if ( NULL != lpszStr )
        {
            strncpy( m_achSoftwareVersion, lpszStr, sizeof(m_achSoftwareVersion) );
            m_achSoftwareVersion[sizeof(m_achSoftwareVersion)-1] = '\0';
        }
    }
    LPCSTR GetSoftWareVer() { return m_achSoftwareVersion;    }

	//forMCU֧�ֵ�����������zgc06-12-21
	void SetIsMcuConfiged(BOOL32 bIsConfiged)
	{
		m_byIsMcuConfiged = bIsConfiged ? 1 : 0;
		return;
	}
	BOOL32 IsMcuConfiged()
	{
		return ( 1 == m_byIsMcuConfiged ) ? TRUE : FALSE ;   
	}

	// ֧��MCU�����ļ����ݹ��ܣ�zgc , 2007-03-16
	void SetMcuCfgInfoLevel( u8 byLevel )
	{
		m_byMcuCfgInfoLevel = byLevel;
	}
	u8 GetMcuCfgInfoLevel()
	{
		return m_byMcuCfgInfoLevel;
	}

};    


/*------------------------------------------------------------- 
*��������Ϣ 
*-------------------------------------------------------------*/
struct TMcuPerformance
{
    u16 m_wAlarmStamp;   /*�澯��������ɾ���澯ʱ��1*/
public:
    TMcuPerformance() : m_wAlarmStamp(0) {}
};

/*------------------------------------------------------------- 
*MIBʹ�õĸ澯�ṹ 
*-------------------------------------------------------------*/
struct TMcupfmAlarmEntry
{
public:
    TMcupfmAlarmEntry()	: m_dwSerialNo(0),
                          m_dwAlarmCode(0),
                          m_byObjType(0)
    {
        memset( m_achTime, 0, sizeof(m_achTime));
        memset( m_achObject, 0, sizeof(m_achObject));
    }
public: 
	u32 m_dwSerialNo;				        /*�澯�ı��*/
    u32 m_dwAlarmCode;				        /*�澯��*/
    u8  m_byObjType;				        /*�澯��������*/
    s8  m_achObject[MAX_ALARMOBJECT_NUM];	/*�澯����*/
    s8  m_achTime[MAX_ALARM_TIME_LEN+1];	/*�澯����ʱ��*/
};  


/*------------------------------------------------------------- 
*�澯���еĸ澯�ṹ 
*-------------------------------------------------------------*/
struct TMcupfmAlarmTable
{
public:
    TMcupfmAlarmTable() : m_dwAlarmCode(0),
                          m_byObjType(0),
                          m_bExist(FALSE)
    { 
        memset( m_achTime, 0, sizeof(m_achTime));
        memset( m_achObject, 0, sizeof(m_achObject));
    }
public:
	u32   m_dwAlarmCode;					/*�澯��*/
    u8    m_byObjType;						/*�澯��������*/
    s8    m_achObject[MAX_ALARMOBJECT_NUM];	/*�澯����*/
    s8    m_achTime[MAX_ALARM_TIME_LEN+1];	/*�澯����ʱ��*/
    BOOL32  m_bExist;							/*�ø澯�Ƿ���Ч*/
};

struct TMcuConfState
{
public:
    TMcuConfState()	: m_byState(0)
    { 
        memset( &m_abyTime, 0, sizeof(m_abyTime)); 
        memset( &m_abyConfId, 0, sizeof(m_abyConfId)); 
        memset( &m_abyConfName, 0, sizeof(m_abyConfName)); 
    }
public:
	u8  m_abyConfId[MAXLEN_CONFID+1];					/*����Id*/
    u8  m_abyConfName[MAXLEN_CONFNAME];					/*������*/
    u8  m_abyTime[MAX_ALARM_TIME_LEN+1];				/*ʱ��*/
    u8  m_byState;										/*״̬*/
};

struct TMcuMtConfState
{
public:
    TMcuMtConfState() : m_byState(0)
    {
        memset( &m_abyTime, 0, sizeof(m_abyTime));
        memset( &m_abyConfId, 0, sizeof(m_abyConfId));
        memset( &m_abyMtAlias, 0, sizeof(m_abyMtAlias));
    }
public:
	u8  m_abyConfId[MAXLEN_CONFID+1];					/*����Id*/
    u8  m_abyMtAlias[MAXLEN_ALIAS+1];					/*�ն˱���*/
    u8  m_abyTime[MAX_ALARM_TIME_LEN+1];				/*ʱ��*/
    u8  m_byState;										/*״̬*/
};


// MPC��Ϣ(������)
struct TMPCInfo
{
protected:
    u32 m_dwMpcIp;                  // ����MPC��ַ(������)
    u16 m_wMpcPort;                 // ����MPC�˿�(������)
    u8  m_byMpcPortChoice;          // ����MPC�˿�ѡ��
    u8  m_byMpcLayer;               // ����MPC���
    u8  m_byMpcSlot;                // ����MPC�ۺ�
    u8  m_byIsMpcMaster;            // ����MPC�Ƿ�����(ҵ������)
    u8  m_byMpcConfigVer;           // ����MPC���ð汾��
    
    u8  m_byOSType;                 // MPC�Ĳ���ϵͳ����(Ĭ����������ϵͳ����һ��)

    u8  m_byIsHaveOtherMpc;         // �Ƿ��жԶ�MPC
    u8  m_byOtherMpcStatus;         // �Զ�MPC״̬
    
    u32 m_dwOtherMpcIp;             // �Զ�MPC��ַ(������)
    u16 m_wOtherMpcPort;            // �Զ�MPC�˿�(������)
    u8  m_byOtherMpcPortChoice;     // �Զ�MPC�˿�ѡ��
    u8  m_byOtherMpcLayer;          // �Զ�MPC���
    u8  m_byOtherMpcSlot;           // �Զ�MPC�ۺ�
    
public:
    TMPCInfo() : m_dwMpcIp(0),
                 m_wMpcPort(0),
                 m_byMpcPortChoice(0),
                 m_byMpcLayer(0),
                 m_byMpcSlot(0),
                 m_byIsMpcMaster(0),
                 m_byMpcConfigVer(VERSION_CONFIGURE40),
                 m_byIsHaveOtherMpc(0),
                 m_byOtherMpcStatus(BOARD_STATUS_UNKNOW),
                 m_dwOtherMpcIp(0),
                 m_wOtherMpcPort(0),
                 m_byOtherMpcPortChoice(0),
                 m_byOtherMpcLayer(0),
                 m_byOtherMpcSlot(0){}
    void   SetLocalIp(u32 dwIp) { m_dwMpcIp = htonl(dwIp);    }
    u32    GetLocalIp(void) const { return ntohl(m_dwMpcIp);    }
    void   SetLocalPort(u16 wPort) { m_wMpcPort = htons(wPort);    }
    u16    GetLocalPort(void) const { return ntohs(m_wMpcPort);    }
    void   SetLocalPortChoice(u8 byPortChoice) { m_byMpcPortChoice = byPortChoice;    }
    u8     GetLocalPortChoice(void) const { return m_byMpcPortChoice;    }
    void   SetLocalLayer(u8 byLayer) { m_byMpcLayer = byLayer;    }
    u8     GetLocalLayer(void) const { return m_byMpcLayer;    }
    void   SetLocalSlot(u8 bySlot) { m_byMpcSlot = bySlot;    }
    u8     GetLocalSlot(void) const { return m_byMpcSlot;    }
    void   SetIsLocalMaster(BOOL32 bMaster) { m_byIsMpcMaster = bMaster ? 1 : 0; };
    BOOL32 GetIsLocalMaster(void) { return m_byIsMpcMaster == 1 ? TRUE : FALSE;    }
    void   SetLocalConfigVer(u8 byConfigVer) { m_byMpcConfigVer = byConfigVer;    }
    u8     GetLocalConfigVer(void) const { return m_byMpcConfigVer;    }
    void   SetIsHaveOtherMpc(BOOL32 bHave) { m_byIsHaveOtherMpc = bHave ? 1 : 0;    }
    BOOL32 GetIsHaveOtherMpc(void) { return m_byIsHaveOtherMpc == 1 ? TRUE : FALSE;    }
    void   SetOtherMpcStatus(u8 byStatus) { m_byOtherMpcStatus = byStatus;    }
    u8     GetOtherMpcStatus(void) const { return m_byOtherMpcStatus;    }
    void   SetOtherMpcIp(u32 dwIp) { m_dwOtherMpcIp = htonl(dwIp);    }
    u32    GetOtherMpcIp(void) const { return ntohl(m_dwOtherMpcIp);    }
    void   SetOtherMpcPort(u16 wPort) { m_wOtherMpcPort = htons(wPort);    }
    u16    GetOtherMpcPort(void) const { return ntohs(m_wOtherMpcPort);    }
    void   SetOtherPortChoice(u8 byChoice) { m_byOtherMpcPortChoice = byChoice;    }
    u8     GetOtherPortChoice(void) const { return m_byOtherMpcPortChoice;    }
    void   SetOtherMpcLayer(u8 byLayer) { m_byOtherMpcLayer = byLayer;    }
    u8     GetOtherMpcLayer(void) const { return m_byOtherMpcLayer;    }
    void   SetOtherMpcSlot(u8 bySlot) { m_byOtherMpcSlot = bySlot;    }
    u8     GetOtherMpcSlot(void) const { return m_byOtherMpcSlot;    }
    void   SetOSType(u8 byOSType) { m_byOSType = byOSType;    }
    u8     GetOSType(void) const { return m_byOSType;    }
    
    void Print() const
    {
        OspPrintf( TRUE, FALSE, "[TMPCInfo] Config File Version: %d \n", GetLocalConfigVer() );
        OspPrintf( TRUE, FALSE, "[TMPCInfo] OSType: %d <1:VX,2:LINUX,3:WIN32> \n", m_byOSType );
        OspPrintf( TRUE, FALSE, "[LocalMpc] MpcIp.0x%x, MpcPort.%d, MpcPortChoice.%d, \n\t   MpcLayer.%d, MpcSlot.%d, MpcMaster.%d \n",
                                 m_dwMpcIp, m_wMpcPort, m_byMpcPortChoice, 
                                 m_byMpcLayer, m_byMpcSlot, m_byIsMpcMaster );
        if ( TRUE == m_byIsHaveOtherMpc ) 
        {
            OspPrintf( TRUE, FALSE, "[OtherMpc] MpcIp.0x%x, MpcPort.%d, MpcPortChoice.%d, MpcLayer.%d, MpcSlot.%d \n",
                                     m_dwOtherMpcIp, m_wOtherMpcPort, m_byOtherMpcPortChoice, 
                                     m_byOtherMpcLayer, m_byOtherMpcSlot );
        }
        else
        {
            OspPrintf( TRUE, FALSE, "[OtherMpc] Not Configured! \n" );
        }
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

/************************************************************************/
/* McuDebug  ��Ϣ                                                       */
/************************************************************************/
struct TMcuAgentDebugInfo
{
public:
    TMcuAgentDebugInfo() : 
        m_wListenPort(MCU_LISTEN_PORT),
        m_wBroadDiscTimeSpan(DEF_OSPDISC_HBTIME),
        m_byBroadDiscTimes(DEF_OSPDISC_HBNUM),
        m_byMpcRunMp(0)
    {
    }

    void SetListenPort(u16 wPort)
    {
        m_wListenPort = wPort;
    }
    u16  GetListenPort() const
    {
        return m_wListenPort;
    }
    void SetBoardDiscTimeSpan(u16 wTimeSpan)
    {
        m_wBroadDiscTimeSpan = wTimeSpan;
    }
    u16  GetBoardDiscTimeSpan() const
    {
        return m_wBroadDiscTimeSpan;
    }
    void SetBoardDiscTimes(u8 byTimes)
    {
        m_byBroadDiscTimes = byTimes;
    }
    u8 GetBoardDiscTimes() const
    {
        return m_byBroadDiscTimes;
    }
    void SetMpcRunMp(BOOL32 bRun)
    {
        m_byMpcRunMp = bRun ? 1 : 0;
    }
    BOOL32 IsMpcRunMp() const
    {
        return (m_byMpcRunMp == 1);
    }
	void SetMpcRunMtAdp( BOOL32 bRun )
	{
		m_byMpcRunMtAdp = bRun ? 1 : 0;
	}
	BOOL32 IsMpcRunMtAdp() const
	{
		return (m_byMpcRunMtAdp == 1);
	}

    void Print() const
    {
        OspPrintf(TRUE, FALSE, "Mcu Agent Debug Info\n");
        OspPrintf(TRUE, FALSE, "\tm_wListenPort %u\n", m_wListenPort);
        OspPrintf(TRUE, FALSE, "\tm_wBroadDiscTimeSpan %u\n", m_wBroadDiscTimeSpan);
        OspPrintf(TRUE, FALSE, "\tm_byBroadDiscTimes %u\n", m_byBroadDiscTimes);
        OspPrintf(TRUE, FALSE, "\tm_byMpcRunMp %u\n", m_byMpcRunMp);
		OspPrintf(TRUE, FALSE, "\tm_byMpcRunMtAdp %u\n", m_byMpcRunMtAdp);
    }

private:
    u16    m_wListenPort;               // MCU �����˿�
    u16    m_wBroadDiscTimeSpan;        // ������������
    u8     m_byBroadDiscTimes;          // �������������
    u8     m_byMpcRunMp;                // MPC�Ƿ�����MP
	u8	   m_byMpcRunMtAdp;				// MPC�Ƿ�����MtAdp, zgc, 2007-07-30
};

#ifdef WIN32

#define SystemBasicInformation       0
#define SystemPerformanceInformation 2
#define SystemTimeInformation        3

#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))

//���´���Windowsֻ����linux�±������ε�
//lint -save -e10 -e148 -e38 -e40 -e19
typedef struct
{
    DWORD   dwUnknown1;
    ULONG   uKeMaximumIncrement;
    ULONG   uPageSize;
    ULONG   uMmNumberOfPhysicalPages;
    ULONG   uMmLowestPhysicalPage;
    ULONG   uMmHighestPhysicalPage;
    ULONG   uAllocationGranularity;
    PVOID   pLowestUserAddress;
    PVOID   pMmHighestUserAddress;
    ULONG   uKeActiveProcessors;
    BYTE    bKeNumberProcessors;
    BYTE    bUnknown2;
    WORD    wUnknown3;
} SYSTEM_BASIC_INFORMATION;

typedef struct
{
    LARGE_INTEGER   liIdleTime;
    DWORD           dwSpare[76];
} SYSTEM_PERFORMANCE_INFORMATION;

typedef struct
{
    LARGE_INTEGER liKeBootTime;
    LARGE_INTEGER liKeSystemTime;
    LARGE_INTEGER liExpTimeZoneBias;
    ULONG         uCurrentTimeZoneId;
    DWORD         dwReserved;
} SYSTEM_TIME_INFORMATION;

typedef LONG (WINAPI *PROCNTQSI)(UINT,PVOID,ULONG,PULONG);
//lint -restore
#endif

#ifdef WIN32
#pragma pack( pop )
#endif

#endif /* End MCUAGTSTRUC_H */
