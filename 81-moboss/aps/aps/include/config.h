/*=======================================================================
模块名      :
文件名      :config.h
相关文件    :
文件实现功能:读写配置文件

作者        :
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/07/05  1.0                       创建
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

//1.1路径与名称
#define APS_CONFIG_PATH                 (const s8*)"conf"                   //配置文件路径
#define APS_CONFIG_NAME                 (const s8*)"apscfg.ini"             //aps配置文件名称

//1.2
#define SECTION_LOCALINFO               (const s8*)"LocalInfo"              //本地信息SECTION
#define SECTION_AP_DBCON                (const s8*)"APDBCon"                //AP数据库SECTION
#define SECTION_IP_DBCON                (const s8*)"IPDBCon"                //IP数据库SECTION
#define SECTION_MQCON                   (const s8*)"MqCon"                  //MQ SECTION
#define SECTION_SRVPORT                 (const s8*)"SrvPort"                //服务器的端口 SECTION
#define SECTION_PXYCONFIG               (const s8*)"PxyConfig"              //HaProxy的地址、运营商信息（中文支持UTF8和GBK）
#define SECTION_LOCALISPCONFIG          (const s8*)"LocalIspConfig"         //配置BGM本地运营商的IP地址
#define	SECTION_DMZCONFIG				(const s8*)"DmzInfo"				//配置DMZ信息


//1.3本地信息
#define KEY_OSP_LISTEN_PORT             (const s8*)"OspListenPort"          //Osp监听端口 key
#define KEY_HTTP_LISTEN_PORT            (const s8*)"HttpListenPort"         //Http监听端口 key
#define KEY_HTTPS_LISTEN_PORT            (const s8*)"HttpsListenPort"         //Https监听端口 key
#define KEY_TELNET_NAME                 (const s8*)"TelnetName"             //OSP telnet用户名 key
#define KEY_TELNET_PWD                  (const s8*)"TelnetPwd"              //OSP telnet密码 key
#define KEY_LOACLISP_NAME               (const s8*)"LoaclIspName"           //本地运营商名
#define KEY_FOREIGNISP_PWD              (const s8*)"ForeignIspName"         //海外运营商名
#define KEY_ISPXYMODE                   (const s8*)"IsPxyMode"              //是否开启代理模式
#define KEY_ISDMZMODE					(const s8*)"IsDmzMode"              //是否dmz模式

//1.3.2 本地信息  默认值
#define DEFAULT_TELNET_NAME             (const s8*)"admin"                  //默认 OSP telnet用户名
#define DEFAULT_TELNET_PWD              (const s8*)"admin"                  //默认 OSP telnet密码
#define DEFAULT_LOACLISP_NAME           (const s8*)"LoaclIspName"           //默认 本地运营商名
#define DEFAULT_FOREIGNISP_PWD          (const s8*)"ForeignIspName"         //默认 海外运营商名

//1.4.1 数据库信息
#define KEY_DBNAME                      (const s8*)"DBName"                 //数据库名 key
#define KEY_DBIP                        (const s8*)"DBIP"                   //数据库IP key
#define KEY_DBPORT                      (const s8*)"DBPort"                 //数据库端口 key
#define KEY_DBUSERNAME                  (const s8*)"DBUserName"             //数据库登录用户名 key
#define KEY_DBUSERPWD                   (const s8*)"DBUserPwd"              //数据库登录密码 key
#define KEY_DBENGINETYPE                (const s8*)"DBType"                 //数据库类型 key

//1.4.2 数据库信息 默认值
#define DEFAULT_APDBNAME                (const s8*)"APDB"                   //默认 AP数据库名
#define DEFAULT_IPDBNAME                (const s8*)"IPDB"                   //默认 IP数据库名
#define DEFAULT_DBIP                    (const s8*)"0.0.0.0"                //默认 数据库IP
#define DEFAULT_DBPORT                  (u16)3306                           //默认 数据库端口
#define DEFAULT_DB_USERNAME             (const s8*)"root"                   //默认 数据库登录用户名
#define DEFAULT_DB_USERPWD              (const s8*)"kedacom"                //默认 数据库登录密码
#define DEFAULT_DB_ENGINETYPE           (u8)Em_DBEngineType_Mysql           //默认 数据库类型

#define DEFAULT_MTTRYAPSSPAN            30000 //ms

//1.5.1 MQ信息
#define KEY_MQIP                        (const s8*)"MqIP"                   //MQ服务器IP
#define KEY_MQPORT                      (const s8*)"MqPort"                 //MQ服务器端口
#define KEY_MQVHOST                     (const s8*)"Vhost"                  //虚拟主机
#define KEY_MQUSERNAME                  (const s8*)"UserName"               //用户名
#define KEY_MQUSERPWD                   (const s8*)"UserPwd"                //密码
#define KEY_MQEXCHANGENAME              (const s8*)"ExchangeName"           //Exchange名
#define KEY_MQQueueName                 (const s8*)"QueueName"              //Queue名

//1.5.2 MQ信息 默认值
#define DEFAULT_MQIP                    (const s8*)"0.0.0.0"                //MQ服务器IP
#define DEFAULT_MQPORT                  (u16)5672                           //MQ服务器端口
#define DEFAULT_MQVHOST                 (const s8*)"/Vhost"                 //虚拟主机
#define DEFAULT_MQUSERNAME              (const s8*)"guest"                  //用户名
#define DEFAULT_MQUSERPWD               (const s8*)"guest"                  //密码
#define DEFAULT_MQEXCHANGENAME          (const s8*)"unKnow"                 //Exchange名
#define DEFAULT_MQQueueName             (const s8*)"unKnow"                 //Queue名

//1.6 服务器的端口 信息
#define KEY_SRVPORT_NDS                 (const s8*)"NdsPort"                //NDS端口
#define KEY_SRVPORT_NMS                 (const s8*)"NmsPort"                //NMS端口
#define KEY_SRVPORT_SNS                 (const s8*)"SnsPort"                //SNS端口
#define KEY_SRVPORT_CMC                 (const s8*)"CmcPort"                //CMC端口
#define KEY_SRVPORT_XNS                 (const s8*)"XnsPort"                //Xns端口
#define KEY_SRVPORT_NTP                 (const s8*)"NtpPort"                //Ntp端口
#define KEY_SRVPORT_VRS                 (const s8*)"VrsPort"                //VRS端口
#define KEY_SRVPORT_DCS                 (const s8*)"DcsPort"                //DCS端口
#define KEY_SRVPORT_SUS                 (const s8*)"SusPort"                //SUS端口
#define KEY_SRVPORT_STDH323             (const s8*)"StdH323Port"            //标准323端口
#define KEY_SRVPORT_NONH323             (const s8*)"NonH323Port"            //非标323端口
#define KEY_SRVPORT_SIP                 (const s8*)"SipPort"                //SIP端口
#define KEY_USESRVTLSPORT_SIP           (const s8*)"UseSipTlsPort"          //是否启用SIPTls端口，1表示启用，0表示关闭
#define KEY_SRVTLSPORT_SIP              (const s8*)"SipTlsPort"             //SIP Tls端口
#define KEY_SRVPORT_RESTAPI				(const s8*)"RestApiPort"			//RestApi端口

#define DEFAULT_TLS_PORT                (u16)0                             //默认 TLS端口,默认为0表示未启用


// 1.7 运营商的中文名，目前支持UTF8 & GBK
const s8 ISP_CN_TELECOM[]      =        "中国电信";
const s8 ISP_CN_TELECOM_UTF8[] =        {0xe4, 0xb8, 0xad, 0xe5, 0x9b, 0xbd, 0xe7, 0x94, 0xb5, 0xe4, 0xbf, 0xa1, 0x00};
const s8 ISP_CN_MOBILE[]       =        "中国移动";
const s8 ISP_CN_MOBILE_UTF8[]  =        {0xe4, 0xb8, 0xad, 0xe5, 0x9b, 0xbd, 0xe7, 0xa7, 0xbb, 0xe5, 0x8a, 0xa8, 0x00};
const s8 ISP_CN_UNICOM[]       =        "中国联通";
const s8 ISP_CN_UNICOM_UTF8[]  =        {0xe4, 0xb8, 0xad, 0xe5, 0x9b, 0xbd, 0xe8, 0x81, 0x94, 0xe9, 0x80, 0x9a, 0x00};
const s8 ISP_CN_WIRED[]        =        "有线通";
const s8 ISP_CN_WIRED_UTF8[]   =        {0xe6, 0x9c, 0x89, 0xe7, 0xba, 0xbf, 0xe9, 0x80, 0x9a, 0x00};
const s8 ISP_CN_CTT[]          =        "铁通";
const s8 ISP_CN_CTT_UTF8[]     =        {0xe9, 0x93, 0x81, 0xe9, 0x80, 0x9a, 0x00};
const s8 ISP_CN_LOCAL[]        =        "本地";
const s8 ISP_CN_LOCAL_UTF8[]   =        {0xe6, 0x9c, 0xac, 0xe5, 0x9c, 0xb0, 0x00};
const s8 ISP_OVERSEAS[]        =        "海外";
const s8 ISP_OVERSEAS_UTF8[]   =        {0xe6, 0xb5, 0xb7, 0xe5, 0xa4, 0x96, 0x00};
const s8 ISP_OTHER[]           =        "其他";
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

public: //服务器端口
    u16 GetStdH323Port() const { return m_wStdH323Port; }
    u16 GetNonH323Port() const { return m_wNonH323Port; }
    u16 GetSipPort() const { return m_wSipPort; }
	BOOL32 IsUseSipTlsPort() const { return m_bUseSipTlsPort; }
	u16 GetSipTlsPort() const { return m_wSipTlsPort; }
	//pas_type并没有配置port,因为pas的port最终是要作为std323,nostd323,sip发出去
	//在最终设置这些服务器的时候单独区分设置
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
    s8 m_szCfgFileName[MAXLEN_CFG_VALUE + 1];     //配置文件保存路径
	
private:    //具体配置信息
    u16 m_wOspListenPort;
    u16 m_wHttpListenPort;
	u16 m_wHttpsListenPort;

    s8 m_szTelnetName[MAXLEN_CFG_VALUE + 1];       //OSP telnet用户名
    s8 m_szTelnetPwd[MAXLEN_CFG_VALUE + 1];        //OSP telnet密码
	
    u32 m_dwDBConNum;                               //数据库连接数
    TDBConnectInfo m_tApDBConInfo;                  //AP数据库
    TDBConnectInfo m_tIpDbConInfo;                  //IP数据库

    CMqConfig m_tMqConInfo;                         //MQ

    s8 m_szLoaclIspName[MAXLEN_CFG_VALUE + 1];       //本地运营商名
    s8 m_szForeignIspName[MAXLEN_CFG_VALUE + 1];     //海外运营商名

    BOOL32 m_bPxyMode;  //是否代理模式

	std::string m_strApsGUID;
	u32 m_dwMtTryApsSpan; //毫秒 

private: // 服务器端口

    u16 m_wStdH323Port;
    u16 m_wNonH323Port;
    u16 m_wSipPort;
	BOOL32 m_bUseSipTlsPort;
	//SipTlsPort作为单独一项，其他服务器的TlsPort后期可以作为和m_mapServerTypePort一样的map
	u16 m_wSipTlsPort;
	std::map<u32, u16> m_mapServerTypePort;
	CLoginInfo m_cTestLoginInfo;

#ifdef _APS_PXY_
	std::set<CSrvAddrInfo>  m_atHaPxyInfo;     //HaPxy的地址信息
	std::set<CSrvAddrInfo>  m_atLocalIspInfo;  //本地Ip属运营商信息
#endif
};

extern CCfg g_cCfgFile;

extern EOperatorsType TransIspNameToType(IN std::string& strIspName);

#endif //end of _CONFIG_H_