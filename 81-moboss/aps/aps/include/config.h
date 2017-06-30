/*=======================================================================
ģ����      :
�ļ���      :config.h
����ļ�    :
�ļ�ʵ�ֹ���:��д�����ļ�

����        :
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/07/05  1.0                       ����
=======================================================================*/
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "kdvtype.h"
#include "dbengine.h"
#include <vector>
#include <string>
#include <map>
#include "../mqwrapper/mqconfig.h"
#include "structulc.h"
#include "structextend.h"

//1.1·��������
#define APS_CONFIG_PATH                 (const s8*)"conf"                   //�����ļ�·��
#define APS_CONFIG_NAME                 (const s8*)"apscfg.ini"             //aps�����ļ�����

//1.2
#define SECTION_LOCALINFO               (const s8*)"LocalInfo"              //������ϢSECTION
#define SECTION_AP_DBCON                (const s8*)"APDBCon"                //AP���ݿ�SECTION
#define SECTION_IP_DBCON                (const s8*)"IPDBCon"                //IP���ݿ�SECTION
#define SECTION_MQCON                   (const s8*)"MqCon"                  //MQ SECTION
#define SECTION_SRVPORT                 (const s8*)"SrvPort"                //�������Ķ˿� SECTION
#define SECTION_PXYCONFIG               (const s8*)"PxyConfig"              //HaProxy�ĵ�ַ����Ӫ����Ϣ������֧��UTF8��GBK��
#define SECTION_LOCALISPCONFIG          (const s8*)"LocalIspConfig"         //����BGM������Ӫ�̵�IP��ַ
#define	SECTION_DMZCONFIG				(const s8*)"DmzInfo"				//����DMZ��Ϣ


//1.3������Ϣ
#define KEY_OSP_LISTEN_PORT             (const s8*)"OspListenPort"          //Osp�����˿� key
#define KEY_HTTP_LISTEN_PORT            (const s8*)"HttpListenPort"         //Http�����˿� key
#define KEY_HTTPS_LISTEN_PORT            (const s8*)"HttpsListenPort"         //Https�����˿� key
#define KEY_TELNET_NAME                 (const s8*)"TelnetName"             //OSP telnet�û��� key
#define KEY_TELNET_PWD                  (const s8*)"TelnetPwd"              //OSP telnet���� key
#define KEY_LOACLISP_NAME               (const s8*)"LoaclIspName"           //������Ӫ����
#define KEY_FOREIGNISP_PWD              (const s8*)"ForeignIspName"         //������Ӫ����
#define KEY_ISPXYMODE                   (const s8*)"IsPxyMode"              //�Ƿ�������ģʽ
#define KEY_ISDMZMODE					(const s8*)"IsDmzMode"              //�Ƿ�dmzģʽ

//1.3.2 ������Ϣ  Ĭ��ֵ
#define DEFAULT_TELNET_NAME             (const s8*)"admin"                  //Ĭ�� OSP telnet�û���
#define DEFAULT_TELNET_PWD              (const s8*)"admin"                  //Ĭ�� OSP telnet����
#define DEFAULT_LOACLISP_NAME           (const s8*)"LoaclIspName"           //Ĭ�� ������Ӫ����
#define DEFAULT_FOREIGNISP_PWD          (const s8*)"ForeignIspName"         //Ĭ�� ������Ӫ����

//1.4.1 ���ݿ���Ϣ
#define KEY_DBNAME                      (const s8*)"DBName"                 //���ݿ��� key
#define KEY_DBIP                        (const s8*)"DBIP"                   //���ݿ�IP key
#define KEY_DBPORT                      (const s8*)"DBPort"                 //���ݿ�˿� key
#define KEY_DBUSERNAME                  (const s8*)"DBUserName"             //���ݿ��¼�û��� key
#define KEY_DBUSERPWD                   (const s8*)"DBUserPwd"              //���ݿ��¼���� key
#define KEY_DBENGINETYPE                (const s8*)"DBType"                 //���ݿ����� key

//1.4.2 ���ݿ���Ϣ Ĭ��ֵ
#define DEFAULT_APDBNAME                (const s8*)"APDB"                   //Ĭ�� AP���ݿ���
#define DEFAULT_IPDBNAME                (const s8*)"IPDB"                   //Ĭ�� IP���ݿ���
#define DEFAULT_DBIP                    (const s8*)"0.0.0.0"                //Ĭ�� ���ݿ�IP
#define DEFAULT_DBPORT                  (u16)3306                           //Ĭ�� ���ݿ�˿�
#define DEFAULT_DB_USERNAME             (const s8*)"root"                   //Ĭ�� ���ݿ��¼�û���
#define DEFAULT_DB_USERPWD              (const s8*)"kedacom"                //Ĭ�� ���ݿ��¼����
#define DEFAULT_DB_ENGINETYPE           (u8)Em_DBEngineType_Mysql           //Ĭ�� ���ݿ�����

#define DEFAULT_MTTRYAPSSPAN            30000 //ms

//1.5.1 MQ��Ϣ
#define KEY_MQIP                        (const s8*)"MqIP"                   //MQ������IP
#define KEY_MQPORT                      (const s8*)"MqPort"                 //MQ�������˿�
#define KEY_MQVHOST                     (const s8*)"Vhost"                  //��������
#define KEY_MQUSERNAME                  (const s8*)"UserName"               //�û���
#define KEY_MQUSERPWD                   (const s8*)"UserPwd"                //����
#define KEY_MQEXCHANGENAME              (const s8*)"ExchangeName"           //Exchange��
#define KEY_MQQueueName                 (const s8*)"QueueName"              //Queue��

//1.5.2 MQ��Ϣ Ĭ��ֵ
#define DEFAULT_MQIP                    (const s8*)"0.0.0.0"                //MQ������IP
#define DEFAULT_MQPORT                  (u16)5672                           //MQ�������˿�
#define DEFAULT_MQVHOST                 (const s8*)"/Vhost"                 //��������
#define DEFAULT_MQUSERNAME              (const s8*)"guest"                  //�û���
#define DEFAULT_MQUSERPWD               (const s8*)"guest"                  //����
#define DEFAULT_MQEXCHANGENAME          (const s8*)"unKnow"                 //Exchange��
#define DEFAULT_MQQueueName             (const s8*)"unKnow"                 //Queue��

//1.6 �������Ķ˿� ��Ϣ
#define KEY_SRVPORT_NDS                 (const s8*)"NdsPort"                //NDS�˿�
#define KEY_SRVPORT_NMS                 (const s8*)"NmsPort"                //NMS�˿�
#define KEY_SRVPORT_SNS                 (const s8*)"SnsPort"                //SNS�˿�
#define KEY_SRVPORT_CMC                 (const s8*)"CmcPort"                //CMC�˿�
#define KEY_SRVPORT_XNS                 (const s8*)"XnsPort"                //Xns�˿�
#define KEY_SRVPORT_NTP                 (const s8*)"NtpPort"                //Ntp�˿�
#define KEY_SRVPORT_VRS                 (const s8*)"VrsPort"                //VRS�˿�
#define KEY_SRVPORT_DCS                 (const s8*)"DcsPort"                //DCS�˿�
#define KEY_SRVPORT_SUS                 (const s8*)"SusPort"                //SUS�˿�
#define KEY_SRVPORT_STDH323             (const s8*)"StdH323Port"            //��׼323�˿�
#define KEY_SRVPORT_NONH323             (const s8*)"NonH323Port"            //�Ǳ�323�˿�
#define KEY_SRVPORT_SIP                 (const s8*)"SipPort"                //SIP�˿�
#define KEY_USESRVTLSPORT_SIP           (const s8*)"UseSipTlsPort"          //�Ƿ�����SIPTls�˿ڣ�1��ʾ���ã�0��ʾ�ر�
#define KEY_SRVTLSPORT_SIP              (const s8*)"SipTlsPort"             //SIP Tls�˿�
#define KEY_SRVPORT_RESTAPI				(const s8*)"RestApiPort"			//RestApi�˿�

#define DEFAULT_TLS_PORT                (u16)0                             //Ĭ�� TLS�˿�,Ĭ��Ϊ0��ʾδ����


// 1.7 ��Ӫ�̵���������Ŀǰ֧��UTF8 & GBK
const s8 ISP_CN_TELECOM[]      =        "�й�����";
const s8 ISP_CN_TELECOM_UTF8[] =        {0xe4, 0xb8, 0xad, 0xe5, 0x9b, 0xbd, 0xe7, 0x94, 0xb5, 0xe4, 0xbf, 0xa1, 0x00};
const s8 ISP_CN_MOBILE[]       =        "�й��ƶ�";
const s8 ISP_CN_MOBILE_UTF8[]  =        {0xe4, 0xb8, 0xad, 0xe5, 0x9b, 0xbd, 0xe7, 0xa7, 0xbb, 0xe5, 0x8a, 0xa8, 0x00};
const s8 ISP_CN_UNICOM[]       =        "�й���ͨ";
const s8 ISP_CN_UNICOM_UTF8[]  =        {0xe4, 0xb8, 0xad, 0xe5, 0x9b, 0xbd, 0xe8, 0x81, 0x94, 0xe9, 0x80, 0x9a, 0x00};
const s8 ISP_CN_WIRED[]        =        "����ͨ";
const s8 ISP_CN_WIRED_UTF8[]   =        {0xe6, 0x9c, 0x89, 0xe7, 0xba, 0xbf, 0xe9, 0x80, 0x9a, 0x00};
const s8 ISP_CN_CTT[]          =        "��ͨ";
const s8 ISP_CN_CTT_UTF8[]     =        {0xe9, 0x93, 0x81, 0xe9, 0x80, 0x9a, 0x00};
const s8 ISP_CN_LOCAL[]        =        "����";
const s8 ISP_CN_LOCAL_UTF8[]   =        {0xe6, 0x9c, 0xac, 0xe5, 0x9c, 0xb0, 0x00};
const s8 ISP_OVERSEAS[]        =        "����";
const s8 ISP_OVERSEAS_UTF8[]   =        {0xe6, 0xb5, 0xb7, 0xe5, 0xa4, 0x96, 0x00};
const s8 ISP_OTHER[]           =        "����";
const s8 ISP_OTHER_UTF8[]      =        {0xe5, 0x85, 0xb6, 0xe4, 0xbb, 0x96, 0x00};


#define MAXLEN_CFG_VALUE				(u16)255
// #define NMS_26MT_LISTENPORT             (u16)1727
// #define NMS_50MT_LISTENPORT             (u16)1827

class CCfg
{
public:
    CCfg();
    BOOL32 ReadConfigInfo(void);

    u16 GetOspListenPort() const { return m_wOspListenPort; }	
    u16 GetHttpListenPort() const { return m_wHttpListenPort; }	
    u16 GetHttpsListenPort() const { return m_wHttpsListenPort; }	
    
    const s8 *GetTelnetName() const { return m_szTelnetName; }
    const s8 *GetTelnetPwd() const { return m_szTelnetPwd; }
	
    u32 GetDBConNum() const { return m_dwDBConNum;}
    std::vector<TDBConnectInfo> GetDBConInfo() const;
	
    CMqConfig GetMqConInfo() const { return m_tMqConInfo; }

    EmDBEngineType GetDBEngineType(void) const;
    const s8 *GetDBEngineTypeDesc(void) const;
	
    const s8 *GetLoaclIspName() const { return m_szLoaclIspName; }
    const s8 *GetForeignIspName() const { return m_szForeignIspName; }

    BOOL32 IsPxyMode() const { return m_bPxyMode; }

	std::string GetApsGUID() const;
	const CLoginInfo& GetTestLoginInfo() const { return m_cTestLoginInfo; }
	BOOL32 LoadTestLoginInfo();

#ifdef _APS_PXY_
	BOOL32 GetPlatformSrvAddrInfo(const u8 byIspType,const s8 *pszMtMoId, CPlatformSrvAddrInfo &cPlatformSvrAddrInfo);

	void PrintHaPxyInfo() const;
#endif

	void PrintCfg() const;

public: //�������˿�
    u16 GetStdH323Port() const { return m_wStdH323Port; }
    u16 GetNonH323Port() const { return m_wNonH323Port; }
    u16 GetSipPort() const { return m_wSipPort; }
	BOOL32 IsUseSipTlsPort() const { return m_bUseSipTlsPort; }
	u16 GetSipTlsPort() const { return m_wSipTlsPort; }
	//pas_type��û������port,��Ϊpas��port������Ҫ��Ϊstd323,nostd323,sip����ȥ
	//������������Щ��������ʱ�򵥶���������
	u16 GetServerPort(const u32 dwServerType) { return m_mapServerTypePort[dwServerType]; }

	void SetMtTryApsSpan(const u32 dwSpan) { m_dwMtTryApsSpan = dwSpan; }
	u32 GetMtTryApsSpan() const { return m_dwMtTryApsSpan; }

	static BOOL32 IsValidIp(const s8* pachIp);

#ifdef _APS_PXY_
	u8 GetIspTypeByLoginApsIp(const u32 & dwApsLocalIp) const;
#endif

	BOOL32 SetGUID(const s8* pchGUID);
private:
    void Init();
	
    BOOL32 LoadCfgLocalInfo(void);
    BOOL32 LoadApDBCfgInfo(void);
    BOOL32 LoadIpDBCfgInfo(void);
    BOOL32 LoadMqCfgInfo(void);
    BOOL32 LoadSrvPortInfo(void);
	BOOL32 LoadHaPxyInfo(void);
	BOOL32 LoadGUID();

private:
    s8 m_szCfgFileName[MAXLEN_CFG_VALUE + 1];     //�����ļ�����·��
	
private:    //����������Ϣ
    u16 m_wOspListenPort;
    u16 m_wHttpListenPort;
	u16 m_wHttpsListenPort;

    s8 m_szTelnetName[MAXLEN_CFG_VALUE + 1];       //OSP telnet�û���
    s8 m_szTelnetPwd[MAXLEN_CFG_VALUE + 1];        //OSP telnet����
	
    u32 m_dwDBConNum;                               //���ݿ�������
    TDBConnectInfo m_tApDBConInfo;                  //AP���ݿ�
    TDBConnectInfo m_tIpDbConInfo;                  //IP���ݿ�

    CMqConfig m_tMqConInfo;                         //MQ

    s8 m_szLoaclIspName[MAXLEN_CFG_VALUE + 1];       //������Ӫ����
    s8 m_szForeignIspName[MAXLEN_CFG_VALUE + 1];     //������Ӫ����

    BOOL32 m_bPxyMode;  //�Ƿ����ģʽ

	std::string m_strApsGUID;
	u32 m_dwMtTryApsSpan; //���� 

private: // �������˿�

    u16 m_wStdH323Port;
    u16 m_wNonH323Port;
    u16 m_wSipPort;
	BOOL32 m_bUseSipTlsPort;
	//SipTlsPort��Ϊ����һ�������������TlsPort���ڿ�����Ϊ��m_mapServerTypePortһ����map
	u16 m_wSipTlsPort;
	std::map<u32, u16> m_mapServerTypePort;
	CLoginInfo m_cTestLoginInfo;

#ifdef _APS_PXY_
	std::set<CSrvAddrInfo>  m_atHaPxyInfo;     //HaPxy�ĵ�ַ��Ϣ
	std::set<CSrvAddrInfo>  m_atLocalIspInfo;  //����Ip����Ӫ����Ϣ
#endif
};

extern CCfg g_cCfgFile;

extern EOperatorsType TransIspNameToType(IN std::string& strIspName);

#endif //end of _CONFIG_H_