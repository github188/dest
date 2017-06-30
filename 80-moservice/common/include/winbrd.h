#ifndef WINBRD_H
#define WINBRD_H

#include "osp.h"
#include "kdvtype.h"
#ifdef ERROR
#undef ERROR
#endif
#include "brdwrapperdef.h"
#include "nipwrapperdef.h"
#include "kdvsys.h"

#define	FILENAME_CONFIG				(s8*)"brdcfgdebug.ini"

#define SECTION_BoardConfig			(s8*)"BoardConfig"
#define KEY_Layer					(s8*)"Layer"
#define KEY_Slot					(s8*)"Slot"
#define KEY_Type					(s8*)"Type"
#define KEY_BoardIpAddr				(s8*)"BoardIpAddr"

#define SECTION_IpConfig			(s8*)"IpConfig"
#define ENTRYPART_ETHERNET			(s8*)"Eth"
#define ENTRYPART_IPADDR			(s8*)"IpAddr"
#define ENTRYPART_IPNUM				(s8*)"IpNum"
#define	ENTRYPART_MAC				(s8*)"Mac"

#define SECTION_IpRoute				(s8*)"IpRoute"
#define KEY_IpRouteNum				(s8*)"IpRouteNum"
#define ENTRYPART_IpRoute			(s8*)"IpRoute"				

#define BOARDHWVER					(u8)9
#define BOARDFPGAVER				(u8)4
#define BSP15SPEED					(u8)393
#define OSVERSION					(s8*)"windows"

#define MCUTYPE_UNKOWN              (u8)0
#define MCUTYPE_MCU                 (u8)1
#define MCUTYPE_MINIMCU             (u8)2

#ifdef __cplusplus 
extern "C" 
#endif /* __cplusplus */

#include "kdvtype.h"
#include "brdwrapperdef.h"
#include "nipwrapperdef.h"
#include "boarddhcp.h"
#include "boardsntp.h"
#include <time.h>

/*��غ궨��*/

#define BRD_KDV7810                 0x7b   /* KDV7810��Ƶ�ն� */
#define BRD_KDV7910                 0x7c   /* KDV7910��Ƶ�ն� */
#define BRD_KDV7820                 0x7d   /* KDV7820��Ƶ�ն� */
#define BRD_KDV7920                 0x7e   /* KDV7920��Ƶ�ն� */
#define BRD_HWID_KDV7820            0x0021 /* KDV7820��Ƶ�ն� */


/*�������ӵİ汾�Ŷ���*/
#define VER_MTLinuxOsWrapperForKDV8010A  ( const s8 * )"WinBoardWrapperForKDV8010A 40.01.00.02.070413"
#define VER_LinuxOsWrapperForMDSC        ( const s8 * )"WinBoardWrapperForMDSC 40.01.00.01.070413"
#define VER_LinuxOsWrapperForHDSC        ( const s8 * )"WinBoardWrapperForHDSC 40.01.00.01.070413"   
#define VER_LinuxOsWrapperForKDV8010C    ( const s8 * )"WinBoardWrapperForKDV8010C 40.01.00.01.070413" 
#define VER_LinuxOsWrapperForKDV8010C1    ( const s8 * )"WinBoardWrapperForKDV8010C1 40.01.00.01.070413"     
#define VER_LinuxOsWrapperForCRI         ( const s8 * )"WinBoardWrapperForCRI 40.01.00.01.070413" 
#define VER_LinuxOsWrapperForMPC         ( const s8 * )"WinBoardWrapperForMPC 40.01.00.00.070413" 
#define VER_LinuxOsWrapperForDRI         ( const s8 * )"WinBoardWrapperForDRI 40.01.00.00.070413" 
#define VER_LinuxOsWrapperForAPU         ( const s8 * )"WinBoardWrapperForAPU 40.01.00.00.070413" 
#define VER_LinuxOsWrapperForKDM2000PLUS         ( const s8 * )"BWinoardWrapperForKDM2000PLUS 40.01.00.00.070413" 
#define VER_MTLinuxOsWrapperForKDV8000B  ( const s8 * )"WinBoardWrapperForKDV8000B 40.01.00.00.070413"
#define VER_MTLinuxOsWrapperForKDV8000C  ( const s8 * )"WinBoardWrapperForKDV8000C 40.01.00.00.070413"
#define VER_MTLinuxOsWrapperForMMP       ( const s8 * )"WinBoardWrapperForMMP 40.01.00.00.070413"
#define VER_MTLinuxOsWrapperForDSC       ( const s8 * )"WinBoardWrapperForDSC 40.01.00.00.070413"
#define VER_MTLinuxOsWrapperForKDV8005   ( const s8 * )"WinBoardWrapperForKDV8005 40.01.00.00.070413"
#define VER_MTLinuxOsWrapperForDSI       ( const s8 * )"WinBoardWrapperForDSI 40.01.00.00.070413"
#define VER_MTLinuxOsWrapperForKDV8003   ( const s8 * )"WinBoardWrapperForKDV8003 40.01.00.00.070413"
#define VER_MTLinuxOsWrapperForVAS   ( const s8 * )"WinBoardWrapperForVAS 40.01.00.00.070413"
#define VER_MTLinuxOsWrapperFor16E1   ( const s8 * )"WinBoardWrapperForDRI16 40.01.00.00.070413"
#define VER_MTLinuxOsWrapperForKDM2428   ( const s8 * )"WinBoardWrapperForKDM2428 40.01.00.00.070413"
#define VER_MTLinuxOsWrapperForVRI   ( const s8 * )"WinBoardWrapperForVRI 40.01.00.00.070413"


/*��ؽṹ�嶨��*/

/*SysSetUsrAppParam()�����õ��Ľṹ��*/

/* Ӧ��������Ϣ�ṹ��*/
typedef struct{
    u8 abyAppName[APP_NAME_MAX_LEN+1];  /* Ӧ�ó����ļ��� */
    u8 byAppType;                       /* Ӧ�ó�������,��:LOCAL_UNCOMPRESS_FILE�� */
}TAppInf;

typedef struct{
    u32 dwAppNum; /* Ӧ�ó���ĸ��� */
    TAppInf tAppInf[APP_MAX_NUM];
}TAppLoadInf;


/*������ݽṹ����*/
/* ������ݽṹ���� */
typedef struct{
    u8 byBrdID;                      /* ����ID�� */
    u8 byBrdLayer;                   /* �������ڲ�� */
    u8 byBrdSlot;                    /* �������ڲ�λ�� */
}TBrdPosition;

/*��̫����صĽṹ����*/
/* ��̫�������ṹ */
typedef struct{
    u32 dwIpAdrs;/*�����ֽ���*/
    u32 dwIpMask; /*�����ֽ���*/
    u8  byMacAdrs[6];
}TBrdEthParam;

/* ��̫�������ṹ */
typedef struct{
    u32 dwIpNum;/*��Ч��IP��ַ��*/
    TBrdEthParam atBrdEthParam[IP_ADDR_MAX_NUM];/*���IP��ַ����Ϣ������*/
}TBrdEthParamAll;

/* ����·�ɲ����ṹ */
typedef struct{
    u32 dwDesIpNet;      /* Ŀ�������������ֽ���	*/
    u32 dwDesIpMask;     /* ���룬�����ֽ���	*/
    u32 dwGwIpAdrs;      /* ����ip��ַ�������ֽ���*/
    u32 dwRoutePri;		/* ·�����ȼ�(1-255)��Ĭ��Ϊ0*/
}TBrdIpRouteParam;

/*���·�ɲ����ṹ */
typedef struct{
    u32 dwIpRouteNum;    /* �õ���ʵ��·�ɸ��� */
    TBrdIpRouteParam tBrdIpRouteParam[IP_ROUTE_MAX_NUM];
}TBrdAllIpRouteInfo;

/* ��̫��ƽ������æ״̬��Ϣ */
typedef struct{
    u8 byEthId;
    BOOL32  bSndBusy;
}TBrdEthSndBusyParam;

/*��̫��ƽ������æ�ص��������Ͷ���*/
typedef void ( *TEthSndBusyCallBack )(u32 dwUsrArg, TBrdEthSndBusyParam *ptParam);

/*ע���豸�����ص������Ļص���������---VxWorks�ϸú���������ϵͳ����ģ���Linux��Ϊ�˼���VxWorks���صض���ú�������*/
#ifdef _LINUX_
typedef void 		(*VOIDFUNCPTR) (void);	/* pfunction returning void */
#endif

/*IP��ַ��ͻ�ص��������Ͷ���*/
typedef void (*TIpConflictCallBack)(u8 *pbyMacAddr, u32 dwIpAdrs);

/*Ping�ص��������Ͷ���*/
/*����˵����
    nResult��ping���:0--�ɹ���1 ---��ʱ��2--����
    nTtl��ping�յ�ICMP�ذ���ttl����ttlֵ���Է�Ӧ����ping������������
    nTripTime��ping�յ�ICMP�ذ�������ʱ�䣬��λΪ΢�us��
    nUserID��ping��Я�����û����ݣ����������ֲ�ͬ���û�
    nErrType���յ�ICMP�ذ������ͣ��������ͽ϶࣬��ʱû�ж����������Ҫʱ���Բ��Ҷ���
    nErrCode���յ�ICMP�ذ��Ĵ��룬�������ͽ϶࣬��ʱû�ж����������Ҫʱ���Բ��Ҷ���
*/
typedef s32 (*TPingCallBack)(s32 nResult,s32 nTtl,s32 nTripTime,s32 nUserID,s32 nErrType,s32 nErrCode);


/*E1��صĽṹ����*/
/* E1��·����serial�ӿڲ����ṹ */
typedef struct{
    u32 dwUsedFlag;                  /* ��ǰserialͬ���ӿ�ʹ�ñ�־,��1ʹ�ã����������Ч��0��ʹ�ã����������Ч */
    u32 dwSerialId;                  /* serialͬ���ӿڱ�ţ�e1��·��16��ʼ����Ч��Χ16~62,���ɳ�ͻ */
    u32 dwE1Id;                      /* ��serialͬ���ӿڶ�Ӧ��e1��ţ������÷�Χ0~E1_MAXNUM-1, E1_MAXNUMȡ������ӦӲ��֧�ֵ�e1����,��kdv8010�ն� #define BRD_MT_E1_MAXNUM 4 */
    u32 dwE1TSMask;                  /* E1ʱ϶����˵��,����� */
    u32 dwE1ChanGroupId;             /* E1ʱ϶�������ڵı���ţ������÷�Χ0~7,��������һ��E1ʹ�õĻ���0�������ֵĻ������ÿ��ʱ϶�ν������α�� */
    u32 dwProtocolType;		/* �ӿ�Э���װ����,����ǵ�E1���ӿ���ָ��PPP/HDLC������Ƕ�E1�������ӱ�����PPPЭ�� */
    u32 dwEchoInterval;		/* ��Ӧserialͬ���ӿڵ�echo����ʱ��������Ϊ��λ����Ч��Χ0-256����Ĭ��ֵ2���� */
    u32 dwEchoMaxRetry;		/* ��Ӧserialͬ���ӿڵ�echo����ط���������Ч��Χ0-256����Ĭ��ֵ2���� */
}TBrdE1SerialInfo;
/******************************************************************************
* dwE1TSMaskʱ϶���䲹��˵����
*    ʱ����������룬��Ts31 Ts30...Ts0����32λ���룬TSxΪ1ʱ��ʾʹ�ø�ʱ϶��Ϊ0ʱ��ʹ��
* Ts0��Ϊͬ���źţ��û�����ʹ��, 0x0��0x1���ǷǷ��Ĳ�������Ч��Χ��0x2~0xfffffffe��
* ��: ʹ��TS1~TS5 5��ʱ϶ʱdwE1TSMask = 0x0000003e,ע��:Ts0=0; ��ʱ����= 64x5 = 320kbit
      ʹ��TS1~TS3,TS30~31 5��ʱ϶ʱdwE1TSMask = 0xc000000e,ע��:Ts0=0; ��ʱ����= 64x5 = 320kbit
******************************************************************************/

/* ��E1��·����ͨ�������ṹ */
typedef struct{
    u32 dwUsedFlag;                  /* ��ǰE1����·����ͨ��ʹ�ñ�־,��1ʹ�ã����������Ч��0��ʹ�ã����������Ч */
    u32 dwIpAdrs;                    /* ip��ַ�������� */
    u32 dwIpMask;                    /* ���룬������ */
    TBrdE1SerialInfo tE1SerialInfo;     /* ��Ӧserialͬ���ӿ���·���� */
}TBrdE1SingleLinkChanInfo;

/* ��E1��·��������ͨ�������ṹ */
typedef struct{
    u32 dwUsedFlag;                      /* ��ǰE1����·��������ͨ��ʹ�ñ�־,��1ʹ�ã����������Ч��0��ʹ�ã����������Ч */
    u32 dwIpAdrs;                        /* ip��ַ�������� */
    u32 dwIpMask;                        /* ���룬������ */
    u32 dwAuthenticationType;            /* PPP���ӵ���֤����PAP/CHAP,Ĭ����MP_AUTHENTICATION_PAP */
    u32 dwFragMinPackageLen;             /* ��С��Ƭ�������ֽ�Ϊ��λ����Χ20~1500��Ĭ����20 */
    s8  chSvrUsrName[MP_STRING_MAX_LEN]; /* ������û����ƣ�������֤�Զ˵����� */
    s8  chSvrUsrPwd[MP_STRING_MAX_LEN];  /* ������û����룬������֤�Զ˵����� */
    s8  chSentUsrName[MP_STRING_MAX_LEN];/* �ͻ����û����ƣ��������Զ���֤ */
    s8  chSentUsrPwd[MP_STRING_MAX_LEN]; /* �ͻ����û����룬�������Զ���֤ */
    TBrdE1SerialInfo tE1SerialInfo[E1_SINGLE_LINK_CHAN_MAX_NUM];/* ÿ���������serialͬ���ӿ���·���� */
}TBrdE1MultiLinkChanInfo;

/* ����һ��ͨ����Ҫ����Ĳ����ṹ���� */
typedef struct{
    u32  dwBufNum;           /* ��Ϣ������buf������ */
    u16  wBufLength;         /* ��Ϣ������ÿ��buf�ĳ���,��Χ������ĺ궨�壬�ұ�����8�������� */
    u32  dwTimeSlotMask;     /* �������ʱ������ʹ��˵��*/
} TE1TransChanParam;

/********************************************************************************* 
* dwTimeSlotMask ʱ������ʹ��˵����
*    ʱ����������룬��Ts31 Ts30...Ts0����32λ���룬TSxΪ1ʱ��ʾʹ�ø�ʱ϶��Ϊ0ʱ��ʹ��
* Ts0��Ϊͬ���źţ��û�����ʹ�ã�Ts16����Ϊ���ݻ�����ڱ�ģ���и�ʱ϶���û�����ͨ������
* ʱ϶����ѡ���Ƿ�ʹ�ã�ʱ϶���佨��ӵ͵�����������ʱ϶����Ҫ��������������������ͨ��ϵ
* ͳ���ȶ���
* ��: ʹ��5��ʱ϶ʱdwTimeSlotMask = 0x0000003e,��ʱ����= 64x5 = 320kbit�� ע��:Ts0=0; 
**********************************************************************************/


/* �û�����ķ������ݲ����ṹ���� */
typedef struct{
    u8* pbyMsg;                  /* �û���Ϣָ�� */
    u32 dwMsgLen;                /* �û���Ϣ���� */
} TE1TransMsgParam;

/* ͨ�������շ�ͳ�ƽṹ���� */
typedef struct{
    u32 dwRxOnePacketLen;        /* ���յ��ĵ������ĳ��� */
    u32 dwRxPackets;             /* ��E1��·���յ��İ��� */
    u32 dwRxErrPackets;          /* �յ�����İ��� */
    u32 dwRxDropPackets;         /* �����ϲ��û������ж����İ��� */
    u32 dwRxSucSndPackets;       /* �ɹ������ϲ��û��İ��� */
    u32 dwTxSndPackets;          /* �ϲ�Ҫ���͵İ��� */
    u32 dwTxSucSndPackets;       /* �ϲ�ɹ����͵��İ��� */
    u32 dwTxSucSndPacketLen;     /* �ϲ�ɹ����͵����ܰ������ֽ�Ϊ��λ */
}TE1TransChanStat;

/* �ص��������Ͷ��� */
typedef void ( * TE1MsgCallBack )(u8 byE1ChanID, u32 dwState);

/*Flash��صĽṹ����*/
/* flash������Ϣ�ṹ���� */
typedef struct{
    u32  dwDiskSize;        /* flash�̵���������ByteΪ��λ */
    s8   achDiskNameStr[32];         /* flash�̵����� */
}TBrdDiskInfoDesc;

typedef struct{
    u8  byDiskNum;           /* ʵ��flash�̵ĸ��� */
    TBrdDiskInfoDesc atBrdDiskInfo[DISK_MAX_NUMBER];
}TBrdAllDiskInfo;

/*�ײ�澯��صĽṹ����*/
/* �ײ�V35�澯��Ϣ�ṹ���� */
typedef struct{
    BOOL32 bAlarmV35CDDown;     /* Carrier Detect, �ز����ʧ�� */
    BOOL32 bAlarmV35CTSDown;    /* Clear To Send, �������ʧ�� */
    BOOL32 bAlarmV35RTSDown;    /* Request To Send, ������ʧ�� */
    BOOL32 bAlarmV35DTRDown;    /* �����ն�δ���� */
    BOOL32 bAlarmV35DSRDown;    /* ����δ׼���� */
}TBrdV35AlarmDesc;

/* �ײ�E1�澯��Ϣ�ṹ���� */
typedef struct{
    BOOL32 bAlarmE1RUA1;     /* E1ȫ1�澯 */
    BOOL32 bAlarmE1RLOS;     /* E1ʧͬ���澯 */
    BOOL32 bAlarmE1RRA;      /* E1Զ�˸澯 */
    BOOL32 bAlarmE1RCL;      /* E1ʧ�ز��澯 */
    BOOL32 bAlarmE1RCMF;     /* E1��CRC��֡����澯 */
    BOOL32 bAlarmE1RMF;      /* E1��·��֡����澯 */
}TBrdE1AlarmDesc;

/* �����Դ�澯���ݽṹ */
typedef struct{
    BOOL32 bAlarmPowerDC48VLDown;     /* ���Դ-48V�쳣 */
    BOOL32 bAlarmPowerDC48VRDown;     /* �ҵ�Դ-48V�쳣 */
    BOOL32 bAlarmPowerDC5VLDown;      /* ���Դ+5V�쳣 */
    BOOL32 bAlarmPowerDC5VRDown;      /* �ҵ�Դ+5V�쳣 */
}TBrdMPCAlarmPowerDesc;

/* �����Դ���ȸ澯���ݽṹ */
typedef struct{
    BOOL32 bAlarmPowerFanLLDown;      /* ���Դ�����ͣת */
    BOOL32 bAlarmPowerFanLRDown;      /* ���Դ�ҷ���ͣת */
    BOOL32 bAlarmPowerFanRLDown;      /* �ҵ�Դ�����ͣת */
    BOOL32 bAlarmPowerFanRRDown;      /* �ҵ�Դ�ҷ���ͣת */
}TBrdMPCAlarmPowerFanDesc;

/* MPC����澯���ݽṹ���� */
typedef struct{
    BOOL32 bAlarmNetClkLockFailed;     /*  ���໷û����ס�ο�ʱ�� */
    TBrdMPCAlarmPowerDesc tBrdMPCAlarmPower; /* ��Դ�澯 */
    TBrdMPCAlarmPowerFanDesc tBrdMPCAlarmPowerFan; /* ��Դ���ȸ澯 */
}TBrdMPCAlarmVeneer;

/* SDH�澯�ṹ���� */
typedef struct{
    BOOL32 bAlarmLOS;         /* �źŶ�ʧ */
    BOOL32 bAlarmLOF;         /* ֡��ʧ */
    BOOL32 bAlarmOOF;         /* ֡ʧ�� */
    BOOL32 bAlarmLOM;         /* ��֡��ʧ */
    BOOL32 bAlarmAU_LOP;      /* ����Ԫָ�붪ʧ */
    BOOL32 bAlarmMS_RDI;      /* ���ö�Զ��ȱ��ָʾ */
    BOOL32 bAlarmMS_AIS;      /* ���öα���ָʾ */
    BOOL32 bAlarmHP_RDI;      /* �߽�ͨ��Զ��ȱ��ָʾ */
    BOOL32 bAlarmHP_AIS;      /* �߽�ͨ������ָʾ */
    BOOL32 bAlarmRS_TIM;      /* ������ͨ���ټ��ֽڲ�ƥ�� */
    BOOL32 bAlarmHP_TIM;      /* ��ͨ���ټ��ֽڲ�ƥ�� */
    BOOL32 bAlarmHP_UNEQ;     /* �߽�ͨ���źű���ֽ�δװ�� */
    BOOL32 bAlarmHP_PLSM;     /* �߽�ͨ���źű���ֽڲ�ƥ�� */

    BOOL32 bAlarmTU_LOP;      /* ֧·��Ԫָ�붪ʧ */
    BOOL32 bAlarmLP_RDI;      /* �ͽ�ͨ��Զ��ȱ��ָʾ */
    BOOL32 bAlarmLP_AIS;      /* �ͽ�ͨ������ָʾ */
    BOOL32 bAlarmLP_TIM;      /* �ͽ�ͨ���ټ��ֽڲ�ƥ�� */
    BOOL32 bAlarmLP_PLSM;     /* �ͽ�ͨ���źű���ֽڲ�ƥ�� */
}TBrdMPCAlarmSDH;

/* MPC�澯���ݽṹ���� */
typedef struct{
    TBrdMPCAlarmVeneer tBrdMPCAlarmVeneer;
    TBrdMPCAlarmSDH    tBrdMPCAlarmSDH;
}TBrdMPCAlarmAll;

typedef struct{
    BOOL32 bAlarmModuleOffLine;     /* ģ������ */
}TBrdCRIAlarmAll;

/* DRI�ײ�E1�澯��Ϣ�ṹ���� */
typedef struct{
    TBrdE1AlarmDesc tBrdE1AlarmDesc[8]; 
}TBrdDRIAlarmAll;

/* DRI16�ײ�E1�澯��Ϣ�ṹ���� */
typedef struct{
    TBrdE1AlarmDesc tBrdE1AlarmDesc[16]; 
}TBrdDRI16AlarmAll;

/* DSI�ײ�E1�澯��Ϣ�ṹ���� */
typedef struct{
    TBrdE1AlarmDesc tBrdE1AlarmDesc[4];
}TBrdDSIAlarmAll;

/* IMT�ײ�澯��Ϣ�ṹ���� */
typedef struct{
    BOOL32 bAlarmDSP1FanStop;     /* ͼ��������ͣת */
    BOOL32 bAlarmDSP2FanStop;     /* ͼ��������ͣת */
    BOOL32 bAlarmDSP3FanStop;     /* ��Ƶ��������ͣת */
}TBrdIMTAlarmAll;

/* KDV8010A�ײ�澯��Ϣ�ṹ���� */
typedef struct{
    BOOL32 bAlarmEth1LnkDown;     /* ��̫��1������ */
    BOOL32 bAlarmDSP1FanStop;     /* ͼ��������ͣת */
    BOOL32 bAlarmDSP2FanStop;     /* ͼ��������ͣת */
    BOOL32 bAlarmDSP3FanStop;     /* ��Ƶ��������ͣת */
    BOOL32 bAlarmSYS1FanStop;     /* �������1ͣת */
    BOOL32 bAlarmSYS2FanStop;     /* �������2ͣת */
    TBrdE1AlarmDesc tBrdE1AlarmDesc[4];     /* E1�澯 */
    TBrdV35AlarmDesc tBrdV35AlarmDesc;    /* V.35�澯 */
}TBrdMTAlarmAll;

/* KDV8018�ײ�澯��Ϣ�ṹ���� */
typedef struct{
    TBrdE1AlarmDesc tBrdE1Alarm;     /* E1�澯 */
}TBrdSMTAlarmAll;

/* KDM2400 2500�ײ�澯��Ϣ�ṹ���� */
typedef struct{
    TBrdE1AlarmDesc tBrdE1Alarm;     /* E1�澯 */
}TBrdKDM2000AlarmAll;


/* �澯״̬�����ṹ */
typedef struct {
    u32 dwAlmNum;             /* ʵ�ʸ澯��Ԫ���� */
    union {
        BOOL32 bAlmNo[BRD_ALM_NUM_MAX];
        TBrdMPCAlarmAll tBrdMPCAlarmAll;
        TBrdCRIAlarmAll tBrdCRIAlarmAll;
        TBrdDRIAlarmAll tBrdDRIAlarmAll;
        TBrdDRI16AlarmAll tBrdDRI16AlarmAll;
        TBrdDSIAlarmAll tBrdDSIAlarmAll;
        TBrdIMTAlarmAll tBrdIMTAlarmAll;
        TBrdMTAlarmAll  tBrdMTAlarmAll;
        TBrdSMTAlarmAll tBrdSMTAlarmAll;
        TBrdKDM2000AlarmAll tBrdKDM2000AlarmAll;
    } nlunion;
} TBrdAlarmState;

/*ָʾ�ƿ�����صĽṹ����*/
/* MPC��ָʾ�ƽṹ */
typedef struct{
    u8 byLedRUN;     /* ϵͳ���е� */
    u8 byLedDSP;     /* DSP���е� */
    u8 byLedALM;     /* ϵͳ�澯�� */
    u8 byLedMS;      /* ������ָʾ�� */
    u8 byLedOUS;     /* ousָʾ�� */
    u8 byLedSYN;     /* ͬ��ָʾ�� */
    u8 byLedLNK0;    /* ��̫��0���ӵ� */
    u8 byLedLNK1;    /* ��̫��1���ӵ� */
    u8 byLedNMS;     /* nmsָʾ�� */
    u8 byLedNORM;    /* �ư�������ָʾ�� */
    u8 byLedNALM;    /* �ư���һ��澯ָʾ�� */
    u8 byLedSALM ;   /* �ư������ظ澯ָʾ�� */
}TBrdMPCLedStateDesc;

/* CRIָʾ�ƽṹ */
typedef struct{
    u8 byLedRUN;      /* ϵͳ���е� */
    u8 byLedMRUN;     /* VOIP/DDNģ�����е� */
    u8 byLedSPD0;     /* ����0����ָʾ�� */
    u8 byLedLNK0;     /* ����0����ָʾ�� */
    u8 byLedACT0;     /* ����0�����շ���Чָʾ�� */
    u8 byLedSPD1;     /* ����1����ָʾ�� */
    u8 byLedLNK1;     /* ��̫��1���ӵ� */
    u8 byLedACT1;     /* ����1�����շ���Чָʾ�� */
    u8 byLedWERR;     /* ���������ݴ��� */
    u8 byLedMLNK;     /* ��MPC����ָʾ�� */
}TBrdCRILedStateDesc;

/* DRIָʾ�ƽṹ */
typedef struct{
    u8 byLedRun; 
    u8 byLedE1_ALM0;
    u8 byLedE1_ALM1;
    u8 byLedE1_ALM2;
    u8 byLedE1_ALM3;
    u8 byLedE1_ALM4;
    u8 byLedE1_ALM5;
    u8 byLedE1_ALM6;
    u8 byLedE1_ALM7;
    u8 byLedMLINK;
    u8 byLedLNK0;
    u8 byLedLNK1;
}TBrdDRILedStateDesc;

/* DRI16ָʾ�ƽṹ */
typedef struct{
    u8 byLedRun; 
    u8 byLedE1_ALM0;
    u8 byLedE1_ALM1;
    u8 byLedE1_ALM2;
    u8 byLedE1_ALM3;
    u8 byLedE1_ALM4;
    u8 byLedE1_ALM5;
    u8 byLedE1_ALM6;
    u8 byLedE1_ALM7;
    u8 byLedE1_ALM8;
    u8 byLedE1_ALM9;
    u8 byLedE1_ALM10;
    u8 byLedE1_ALM11;
    u8 byLedE1_ALM12;
    u8 byLedE1_ALM13;
    u8 byLedE1_ALM14;
    u8 byLedE1_ALM15;
    u8 byLedMLINK;
    u8 byLedLNK0;
    u8 byLedLNK1;
}TBrdDRI16LedStateDesc;

/* DSIָʾ�ƽṹ */
typedef struct{
    u8 byLedRun; 
    u8 byLedE1_ALM0;
    u8 byLedE1_ALM1;
    u8 byLedE1_ALM2;
    u8 byLedE1_ALM3;
    u8 byLedE1_ALM4;
    u8 byLedE1_ALM5;
    u8 byLedE1_ALM6;
    u8 byLedE1_ALM7;
    u8 byLedMLINK;
    u8 byLedLNK0;
    u8 byLedLNK1;
}TBrdDSILedStateDesc;

/* IMTָʾ�ƽṹ */
typedef struct{
    u8 byLedRun; 
    u8 byLedDSP1;
    u8 byLedDSP2;
    u8 byLedDSP3;
    u8 byLedSYSALM;
    u8 byLedMLINK;
    u8 byLedLNK0;
    u8 byLedLNK1;
}TBrdIMTLedStateDesc;

/* KDV8000Bģ��DSCָʾ�ƽṹ */
typedef struct{
    u8 byLedRun; 
    u8 byLedLNK0;
    u8 byLedLNK1;
    u8 byLedALM;
}TBrdDSCLedStateDesc;

/* MMPָʾ�ƽṹ */
typedef struct{
    u8 byLedRUN; 
    u8 byLedDSPRUN1;
    u8 byLedDSPRUN2;
    u8 byLedDSPRUN3;
    u8 byLedDSPRUN4;
    u8 byLedDSPRUN5;
    u8 byLedALM;
    u8 byLedLNK;
    u8 byLedETH0;
    u8 byLedETH1;
}TBrdMMPLedStateDesc;

/* VASָʾ�ƽṹ */
typedef struct{
    u8 byLedRUN; 
    u8 byLedALM;
    u8 byLedMLNK;
    u8 byLedETHLNK;
}TBrdVASLedStateDesc;

/* �ն�KDV8010A�Լ�KDV8000Bǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedPower;
    u8 byLedALM;
    u8 byLedRun; 
    u8 byLedLINK;
    u8 byLedDSP1;
    u8 byLedDSP2;
    u8 byLedDSP3;
    u8 byLedETH1;
    u8 byLedETH2;
    u8 byLedETH3;
    u8 byLedIR;
}TBrdKDV8010LedStateDesc,TBrdMTLedStateDesc;

/* �ն�KDV8010Cǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedPower;
    u8 byLedALM;
    u8 byLedRun; 
    u8 byLedLINK;
    u8 byLedDSP1;
    u8 byLedDSP2;
    u8 byLedETH1;
    u8 byLedIR;
}TBrdKDV8010CLedStateDesc;

/* �ն�KDV8010C1ǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedPower;
    u8 byLedALM;
    u8 byLedRun; 
    u8 byLedLINK;
    u8 byLedDSP1;
    u8 byLedDSP2;
    u8 byLedETH1;
    u8 byLedIR;
}TBrdKDV8010C1LedStateDesc;

/* KDV8005ǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedRun; 
    u8 byLedLINK;
    u8 byLedETH0;
    u8 byLedETH1;
    u8 byLedV35;
}TBrdKDV8005LedStateDesc;

/* KDV8003ǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedRun; 
    u8 byLedLINK;
    u8 byLedETH0;
    u8 byLedE1Alm;
}TBrdKDV8003LedStateDesc;

/* KDM2417ǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedPower;
    u8 byLedALM;
    u8 byLedRun; 
    u8 byLedCDMA;
    u8 byLedWLAN;
    u8 byLedETH;
    u8 byLedDISK;
}TBrdKDM2417LedStateDesc;

/* KDV8008ǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedALM;
    u8 byLedRun;
    u8 byLedETH;
}TBrdKDV8008LedStateDesc;

/* KDM2000PLUSǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedRun; 
    u8 byLedALM; 
    u8 byLedLINK;
    u8 byLedDSP;
    u8 byLedE1Alm;
    u8 byLedETH1;
    u8 byLedSPD1;
    u8 byLedETH2;
}TBrdKDM2000LedStateDesc;

/* KDM2428ǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedPower;
    u8 byLedALM;
    u8 byLedRun; 
    u8 byLedLNK;
    u8 byLedETH;
    u8 byLedDISK;
}TBrdKDM2428LedStateDesc;

/* MDSCǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedRun; 
    u8 byLedALM; 
    u8 byLedETH1;
    u8 byLedETH2;
}TBrdMDSLedStateDesc;

/* HDSCǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedRun; 
    u8 byLedLos; 
    u8 byLedETH1;
    u8 byLedETH2;
}TBrdHDSLedStateDesc;

/* �ն�VRIǰ���ָʾ�ƽṹ */
typedef struct{
    u8 byLedRUN;     /* ϵͳ���е� */
    u8 byLedV35ALM0;  /* E10�澯ָʾ�� */
    u8 byLedV35ALM1;  /* E11�澯ָʾ�� */
    u8 byLedV35ALM2;  /* E12�澯ָʾ�� */
    u8 byLedV35ALM3;  /* E13�澯ָʾ�� */
    u8 byLedV35ALM4;  /* E14�澯ָʾ�� */
    u8 byLedV35ALM5;  /* E15�澯ָʾ�� */
    u8 byLedMLINK;   /* ϵͳ����ָʾ�� */
    u8 byLedLNK0;    /* Eth0������ָʾ�� */
}TVRILedStateDesc;


/* ָʾ��״̬�����ṹ */
typedef struct {
    u32 dwLedNum;                /* ʵ��ָʾ�Ƶĸ��� */
    union {
        u8 byLedNo[BRD_LED_NUM_MAX];
        TBrdMPCLedStateDesc tBrdMPCLedState;
        TBrdCRILedStateDesc tBrdCRILedState;
        TBrdDRILedStateDesc tBrdDRILedState;
        TBrdDRI16LedStateDesc tBrdDRI16LedState;
        TBrdDSILedStateDesc tBrdDSILedState;
        TBrdIMTLedStateDesc tBrdIMTLedState;
        TBrdDSCLedStateDesc tBrdDSCLedStateDesc;
        TBrdMMPLedStateDesc tBrdMMPLedState;
        TBrdVASLedStateDesc tBrdVASLedState;
        TBrdMTLedStateDesc  tBrdMTLedState;
        TBrdKDV8010CLedStateDesc tBrdKDV8010CLedState;
        TBrdKDV8010C1LedStateDesc tBrdKDV8010C1LedState;
        TBrdKDV8010LedStateDesc tBrdKDV8010LedStateDesc;
        TBrdKDV8005LedStateDesc tBrdKDV8005LedState;
        TBrdKDV8003LedStateDesc tBrdKDV8003LedState;
        TBrdKDM2417LedStateDesc tBrdKDM2417LedState;
        TBrdKDV8008LedStateDesc tBrdKDV8008LedState;
        TBrdKDM2000LedStateDesc tBrdKDM2000LedState;
        TBrdKDM2428LedStateDesc tBrdKDM2428LedStateDesc;
        TBrdMDSLedStateDesc     tBrdMDSLedStateDesc;
        TBrdHDSLedStateDesc     tBrdHDSLedStateDesc;
        TVRILedStateDesc        tVRILedStateDesc;
    } nlunion;
} TBrdLedState;

// ����״̬�����ṹ
/* �ṹ������ 
link:  0-fail;    1-pass
duplex: 0-half;   1-full
speed: 10-10Mbps; 100-100Mbps; 1000-1000Mbps) 
*/
typedef struct{
    u8 Link;                      /* ��̫���ӿ�����״̬ */
    u8 AutoNeg;                   /* ��̫���ӿ�Э�̣���״̬���ϴ�,ֻ��Ϊ�˼��������豸 */
    u8 Duplex;                    /* ��̫���ӿ�˫�� */
    u32 Speed;                     /* ��̫���ӿ����� */
}TBrdEthInfo;
// IS2.2 8313 ����״̬
#define SW_ETH_MAXNUM 				26
struct SWEthInfoRaw
{
    TBrdEthInfo eth_info[SW_ETH_MAXNUM];
} ;
#define INSIDE_ETH_MAXNUM 		16
#define FRONT_PANE_ETH_MAXNUM 	2
#define BACK_PANE_ETH_MAXNUM 	8
/*����оƬ�ṩ������ӿڷ�Ϊ���ࣺ�ֱ����ڲ�����ӿڣ�ǰ�������ӿڣ������2���ӿڣ��������ΪIS3-8548������ӿڣ�����������ӿ� */
struct SWEthInfo
{
    TBrdEthInfo inside_eth_info[INSIDE_ETH_MAXNUM];
    TBrdEthInfo front_eth_info[FRONT_PANE_ETH_MAXNUM];
    TBrdEthInfo back_eth_info[BACK_PANE_ETH_MAXNUM];
};

/*���ڿ�����صĽṹ����*/
/* 485��ѯ��������Ĳ����ṹ */
typedef struct {
    u32 dwDevType;           /* 485�豸���� */
    u32 dwRcvTimeOut;        /* �ȴ�485�豸��Ӧ�ĳ�ʱ,0ȡĬ��ֵ */
    u8  byDesAddr;           /* 485�豸Ŀ���ַ */
    u8  byCmd;               /* �������� */
} TRS485InParam;


/* IRAY����ֵ�ṹ */
typedef struct{
    u8 byCtrllerType;    /* ң�������� */
    u8 byKeyCode;        /* ���� */
}TIRAYRtnDataDesc;

typedef struct {
    BOOL32 bDevLnked;         /* 485�豸����״̬ */
    union {
        TIRAYRtnDataDesc tIRAYRtnData;
    } nlunion;
} TRS485RtnData;

/*BrdPing�ӿڴ�������ṹ��*/
typedef struct{
    s32 nPktSize;/*Ҫping�����ݱ��Ĵ�С�����ֽ�Ϊ��λ*/
    s32 nTimeOut;/*ping�ĳ�ʱֵ������Ϊ��λ*/
    s32 nTtl;/*TTLֵ*/
}TPingOpt;

/*BrdGetDeviceInfo �ӿڴ�������ṹ��*/
typedef struct{
    s8  achCPUInfo[CPU_TYPE_MAX_LEN];/*CPU����*/
    s8  achUBootInfo[UBOOT_VERSION_MAX_LEN];/*u-boot�汾��Ϣ*/
    s8  achKernelInfo[KERNEL_VERSION_MAX_LEN];/*�ں˲���ϵͳ�İ汾��Ϣ*/
    u32 dwMemSize;/*�����ڴ��С�����ֽ�Ϊ��λ*/
    u32 dwFlashSize;/*FLash��С�����ֽ�Ϊ��λ*/
    u32 dwCpuFreq;/*CPU����Ƶ*/
    u32 dwCpmFreq;/*CPM����Ƶ*/
    u32 dwBusFreq;/*���ߵ���Ƶ*/
    u32 dwEthNum;/*��̫���ĸ���*/
    u32 dwE1Num;/*�豸��ǰ���E1�ĸ���*/
    u32 dwV35Num;/*�豸��ǰ���V35�ĸ���*/    
}TBrdDeviceInfo;

/*BrdAddUser�ӿڴ�����û�����*/
typedef enum UserType
{
  USER_TYPE_CONSOLE = 0,     /*Console�û�*/
  USER_TYPE_CONSOLE_SUPER,     /*Console�߼��û�*/
  USER_TYPE_FTP,         /*FTP �û�*/
  USER_TYPE_TELNET,      /*TELNET�û�*/
  USER_TYPE_TELNET_SUPER,      /*TELNET �߼��û�*/
  USER_TYPE_PPP,         /*PPP��·�û�*/
  USER_TYPE_HTTP,	  /*HTTP�û�*/
  USER_TYPE_MAX,         /*�û����͵����ֵ*/
}TUserType;

/*RawFlash��صĽṹ�嶨��*/

/* ÿ�����������ṹ���� */
typedef struct{
    u32 dwPartitionType;                 /* �������� */
    u32 dwPartitionSize;                 /* �÷�������,�ֽ�Ϊ��λ */
}TOnePartnParam;

/* ���������ṹ���� */
typedef struct{
    BOOL32  bUseDefaultConf;                  /* �Ƿ�ʹ��Ĭ�ϵ����÷���,���ΪTRUE��ʹ��ÿ�����ӵ�Ĭ������������,�ڶ�������Ϣʱ����0����Ч���� */
    TOnePartnParam tOnePartitionParam[PARTITION_MAX_NUM];    /* ÿ��������Ϣ */
    u32 dwRamdiskSize;                   /* RAMDISK �Ĵ�С,�ֽ�Ϊ��λ */
}TFlPartnParam;

#endif