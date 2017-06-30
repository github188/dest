 /*****************************************************************************
   ģ����      : KDVMT
   �ļ���      : mtstruc.h
   ����ļ�    : kdvmt.mib
   �ļ�ʵ�ֹ���: KDVMT struct
   ����        : ����
   �汾        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2002/02/18  0.9         ����        ����
   2002/07/29  1.0         ����        �·�������
   2003/07/15  1.1         jianghy     ��������
   2003/11/05  3.0         jianghy     3.0ʵ��
******************************************************************************/
#if !defined( _MT_STRUC_H_ )
#define _MT_STRUC_H_ 

#include "kdvsys.h"
#include "mtleaf.h"

/* config file name*/
#define FILENAME_MTCFG      (LPCSTR)"mtcfg.ini"

/* maximum alarm number in MCU */
#define MAXNUM_MT_E1                    (u8   )4
#define MAXNUM_MT_ALARM                 MAXNUM_MT_E1 * 6 + 2

/* index length */
#define MTSYS_ALARM_TBL_INDEX_LEN		OID_ID_LEN
#define MTNET_TRAPRCV_TBL_INDEX_LEN		OID_IP_ADDR_LEN
#define MTNET_PERIEQP_TBL_INDEX_LEN		OID_ID_LEN
#define MTEQP_E1_TBL_INDEX_LEN			OID_ID_LEN
#define MTEQP_TS_TBL_INDEX_LEN			OID_ID_LEN * 2
#define MTNET_ETHERNET_CARD_TBL_INDEX_LEN OID_ID_LEN
#define MTNET_IP_ROUTE_TBL_INDEX_LEN		OID_IP_ADDR_LEN
#define MTEQP_CAMERA_TBL_INDEX_LEN			OID_ID_LEN
#define MTEQP_MATRIX_TBL_INDEX_LEN			OID_ID_LEN
#define MTPFM_ALARM_TBL_INDEX_LEN			OID_ID_LEN
#define MTNET_MAP_TBL_INDEX_LEN		OID_ID_LEN


/*---------------------------------------------------------
*������Ϣ�ṹ
*--------------------------------------------------------*/
typedef struct 
{
    char            mtAlias[MAXSIZE_mtAlias + 1];            /*�ն˱���*/
	char			mtE164Number[MAXSIZE_mtE164Number + 1];  /*�ն˺���*/
	UCHAR			mtType;                                  /*�ն�����*/
	UCHAR           mtPosition;                              /*�ն˵�λ*/
}STRUCT_mtLocalInfo, STRUCT_mtLocalInfo_mem;


/*---------------------------------------------------------
*�������ò���
*--------------------------------------------------------*/
typedef struct 
{
	UCHAR			mtnetNmIpIndex;       /*�ն�����IP����*/
	UCHAR			mtnetVcIpIndex;       /*�ն�VC IP����*/
	UCHAR			mtnetGwIpIndex;	      /*�ն�GW IP����*/
	u32			mtnetGKIpAddr;	      /*GateKeeper��IP��ַ*/
	u32			mtnetBandwidth;		  /*�������(��λkbps)*/
	u16 			mtnetRcvStartPort;    /*�ն˽�����ʼ�˿�*/
	u16 			mtnetSendStartPort;	  /*�ն˷�����ʼ�˿�*/
}STRUCT_mtNetwork,STRUCT_mtNetwork_mem;


/*---------------------------------------------------------
*������������ã����ܲ�Ҫ
*--------------------------------------------------------*/
typedef struct
{
	UCHAR			mtnetEthernetCardEntIndex;		/*����������*/
	u32			mtnetEthernetCardEntIpAddr;		/*������IP��ַ*/
	u32			mtnetEthernetCardEntMask;		/*��������������*/
	PHYADDR_STRUCT	mtnetEthernetCardEntPhyAddr;	/*�����������ַ*/
	UCHAR			mtnetEthernetCardEntExist;		/*�Ƿ�ʹ�ø�����*/
}STRUCT_mtnetEthernetCardEntry, STRUCT_mtnetEthernetCardEntry_mem;


/*---------------------------------------------------------
*����TRAP����ķ�������
*--------------------------------------------------------*/
typedef struct
{
    u32           mtnetTrapRcvEntIpAddr;			/*TRAP������IP��ַ*/
    char            mtnetTrapRcvEntCommunity[32]; /*COMMUNITY*/
	UCHAR			mtnetTrapRcvEntExist;			/*�Ƿ�ʹ�øñ���*/
} STRUCT_mtnetTrapRcvEntry,STRUCT_mtnetTrapRcvEntry_mem;

typedef struct
{
	u32 mtnetTrapEntryAddIp;
	char  mtnetTrapEntryAddCommunity[32];
	UCHAR mtnetTrapEntryAddEnable;
}STRUCT_mtnetTrapRcvTable;

/*---------------------------------------------------------
*���������
*--------------------------------------------------------*/
typedef struct
{
	UCHAR	mteqpCameraEntId;			/*��������*/
	UCHAR	mteqpCameraEntType;			/*���������(1:SONY�����;2:PELCO�����;3:SAMSUNG�����)*/
	u32	mteqpCameraEntSvrIpAddr;	/*�������Ӧ�����������IP��ַ(���������������Ϊ0.0.0.0)*/
	u16 	mteqpCameraEntPort;			/*�������Ӧ(����)���ں�*/
}STRUCT_mteqpCameraEntry_mem;

typedef struct
{
	UCHAR	mteqpCameraEntId;			/*��������*/
	UCHAR	mteqpCameraEntType;			/*���������(1:SONY�����;2:PELCO�����;3:SAMSUNG�����)*/
	u32	mteqpCameraEntSvrIpAddr;	/*�������Ӧ�����������IP��ַ(���������������Ϊ0.0.0.0)*/
	u16 	mteqpCameraEntPort;			/*�������Ӧ(����)���ں�*/
	UCHAR	mteqpCameraEntExist;		/*�Ƿ�ʹ�ø���*/
}STRUCT_mteqpCameraEntry;

/*---------------------------------------------------------
*����Ƶ��������
*--------------------------------------------------------*/
typedef struct
{
	UCHAR	mteqpMatrixEntId;			/*������*/
	UCHAR	mteqpMatrixEntType;			/*��������(����Ϊ1)*/
	u32	mteqpMatrixEntSvrIpAddr;	/*�����Ӧ�����������IP��ַ(���������������Ϊ0.0.0.0)*/
	u16 	mteqpMatrixEntPort;			/*�����Ӧ(����)���ں�*/
}STRUCT_mteqpMatrixEntry_mem;

typedef struct
{
	UCHAR	mteqpMatrixEntId;			/*������*/
	UCHAR	mteqpMatrixEntType;			/*��������(����Ϊ1)*/
	u32	mteqpMatrixEntSvrIpAddr;	/*�����Ӧ�����������IP��ַ(���������������Ϊ0.0.0.0)*/
	u16 	mteqpMatrixEntPort;			/*�����Ӧ(����)���ں�*/
	UCHAR	mteqpMatrixEntExist;		/*�Ƿ�ʹ�ø���*/
}STRUCT_mteqpMatrixEntry;

/*---------------------------------------------------------
*�������������
*--------------------------------------------------------*/
typedef struct
{
	UCHAR	mteqpNetEncoderId;			/*������������豸���*/
}STRUCT_mteqpNetEncoder,STRUCT_mteqpNetEncoder_mem;


/*---------------------------------------------------------
*��Ƶ���ò���
*--------------------------------------------------------*/
typedef struct 
{
	UCHAR	mtVideoEncSource;		/*��ƵԴ 1:COMP  2:S-VIDEO*/
	UCHAR	mtVideoEncSystem;		/*��Ƶ��ʽ 1:PAL  2:NTSC (�ݲ�������)*/
	UCHAR	mtVideoEncFormat;		/*��Ƶ�����ʽ 1:h2611  2:h263   3:mpeg4*/
	u32	mtVideoEncBitRate;		/*��Ƶ��������(��λ��kbps)*/
	UCHAR	mtVideoEncResolution;	/*��Ƶ�ɼ����*/
	UCHAR	mtVideoEncFrameRate;	/*��Ƶ�ɼ�֡��*/
	UCHAR	mtVideoEncIKeyRate;		/*��Ƶѹ�����ؼ�֡���*/
	UCHAR	mtVideoEncQualMaxValue;	/*��Ƶѹ�������������(1-31)*/
	UCHAR	mtVideoEncQualMinValue;	/*��Ƶѹ����С��������(1-31)*/
	UCHAR	mtVideoEncImageQuality; /*��Ƶͼ������(1-5)*/
}STRUCT_mtVideo, STRUCT_mtVideo_mem;


/*---------------------------------------------------------
*��Ƶ���ò���
*--------------------------------------------------------*/
typedef struct 
{
	UCHAR	mtAudioEncSource;	/*��ƵԴ   1:Line  2:Mic  (�ݲ�������)*/
	UCHAR	mtAudioEncFormat;	/*��Ƶ�����ʽ	mp3(1-4)   5:g711u  6:g711a  7:g723  8:g729   9:g722*/
	UCHAR	mtAudioEncVolume;	/*�������� (��Χ0-255)*/
	UCHAR	mtAudioAEC;			/*��������	1 ����  2 �ر�*/
}STRUCT_mtAudio, STRUCT_mtAudio_mem;


/*---------------------------------------------------------
*��ʾ���ò���
*--------------------------------------------------------*/
typedef struct 
{
	UCHAR mtDisplayLabelOnOff;		/*�Ƿ���ʾ̨�� 1 ��ʾ 2 ����ʾ*/
	UCHAR mtDisplayLabelMode;		/*̨����ʾģʽ*/
	UCHAR mtDisplayPIPMode;         /*���л����� 1:�Զ�����  2:�ֶ�����*/
}STRUCT_mtDisplay, STRUCT_mtDisplayOption_mem;


/*---------------------------------------------------------
*�������ò���
*--------------------------------------------------------*/
typedef struct 
{
	UCHAR mtAnswerMode;			/*���ܺ��з�ʽ 1:�Զ� 2:�ֶ�  3:�����*/
	UCHAR mtCallMode;			/*���з�ʽ     1:�Զ� 2:�ֶ� */
	u32 mtAutoCallIpAddr;		/*�Զ����е�IP��ַ�����ó��Զ�����ʱʹ��*/
}STRUCT_mtCall, STRUCT_mtCallConfig_mem;


/*---------------------------------------------------------
*ϵͳ��
*--------------------------------------------------------*/
typedef struct
{
    u16     mtsysState;												/*����״̬*/
    char    mtsysTime[SIZE_mtsysTime + 1];							/*��ǰʱ��*/
    char    mtsysFtpFileName[MAXSIZE_mtsysFtpFileName + 1];			/*FTP�ļ���*/
    char    mtsysConfigVersion[SIZE_mtsysConfigVersion + 1];		/*���ð汾*/
	char	mtsysSoftwareVersion[MAXSIZE_mtsysSoftwareVersion + 1];	/*����汾*/
} STRUCT_mtSystem,STRUCT_mtSystem_mem;    


/*---------------------------------------------------------
*������
*--------------------------------------------------------*/
typedef struct
{
    u16     mtpfmAlarmStamp;	/*�澯��������ɾ���澯ʱ��1*/
}STRUCT_mtPerformance;


/*---------------------------------------------------------
*MIBʹ�õĸ澯�ṹ
*--------------------------------------------------------*/
typedef struct
{
    u32   mtpfmAlarmEntSerialNo;   /*�澯�ı��*/
    u32   mtpfmAlarmEntAlarmCode;  /*�澯��*/
    UCHAR   mtpfmAlarmEntObjType;    /*�澯��������*/
    UCHAR   mtpfmAlarmEntObject[3];  /*�澯����*/
    char    mtpfmAlarmEntTime[SIZE_mtpfmAlarmEntTime + 1];  /*�澯����ʱ��*/
} STRUCT_mtpfmAlarmEntry; 


/*---------------------------------------------------------
*�ڴ��еĸ澯�ṹ
*--------------------------------------------------------*/
typedef struct
{
    u32   mtpfmAlarmEntAlarmCode;		/*�澯��*/
    UCHAR   mtpfmAlarmEntObjType;		/*�澯��������*/
    UCHAR   mtpfmAlarmEntObject[3];		/*�澯����*/
    char    mtpfmAlarmEntTime[SIZE_mtpfmAlarmEntTime + 1];  /*�澯����ʱ��*/
    BOOL  	bExist;						/*�ø澯�Ƿ���Ч*/
} STRUCT_mtpfmAlarmEntry_mem;     


/*---------------------------------------------------------
*������ͳ����Ϣ
*--------------------------------------------------------*/
typedef struct
{
	u8    mtpfmEncoderVideoFrameRate;   /*��Ƶ֡��*/
	u32 mtpfmEncoderVideoBitRate;    /*��Ƶ�����ٶ�*/
	u32 mtpfmEncoderAudioBitRate;    /*���������ٶ�*/
	u32 mtpfmEncoderVideoPackLose;   /*��Ƶ��֡��*/
	u32 mtpfmEncoderVideoPackError;  /*��Ƶ��֡��*/
	u32 mtpfmEncoderAudioPackLose;   /*������֡��*/
	u32 mtpfmEncoderAudioPackError;  /*������֡��*/
}STRUCT_mtpfmEncoderStatistics;


/*---------------------------------------------------------
*������ͳ����Ϣ
*--------------------------------------------------------*/
typedef struct
{
	u8    mtpfmDecoderVideoFrameRate;   /*��Ƶ����֡��*/
	u16  mtpfmDecoderVideoBitRate;     /*��Ƶ��������*/
	u16  mtpfmDecoderAudioBitRate;     /*������������*/
	u32 mtpfmDecoderVideoRecvFrame;  /*�յ�����Ƶ֡��*/
	u32 mtpfmDecoderAudioRecvFrame;  /*�յ�������֡��*/
	u32 mtpfmDecoderVideoLoseFrame;  /*��ʧ����Ƶ֡��*/
	u32 mtpfmDecoderAudioLoseFrame;  /*��ʧ������֡��*/
	u32 mtpfmDecoderPackError;       /*��֡��*/
	u32 mtpfmDecoderIndexLose;       /*��Ŷ�֡*/
	u32 mtpfmDecoderSizeLose;        /*��С��֡*/
	u32 mtpfmDecoderFullLose;        /*����֡*/
}STRUCT_mtpfmDecoderStatistics;


/*---------------------------------------------------------
*����ת��ͳ����Ϣ
*--------------------------------------------------------*/
typedef struct 
{
	u32 mtpfmDSP1FanSpeed;    /*ͼ��������ת��*/
	u32 mtpfmDSP2FanSpeed;	/*ͼ��������ת��*/
	u32 mtpfmDSP3FanSpeed;	/*��Ƶ��������ת��*/
	u32 mtpfmSYS1FanSpeed;	/*�������1ת��*/
	u32 mtpfmSYS2FanSpeed;	/*�������2ת��*/
}STRUCT_mtpfmFanSpeedStatistics;

/*---------------------------------------------------------
*����ת��ͳ����Ϣ
*--------------------------------------------------------*/
typedef struct
{
	UCHAR mtpfmLedState[8];
}STRUCT_mtpfmPanelLed;

#endif /* _MT_STRUC_H_ */
/* end of file mtstruc.h */
