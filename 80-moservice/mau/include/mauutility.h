/*****************************************************************************
ģ����      : MAU
�ļ���      : mauutility.h
����ʱ��    : 2014�� 08�� 26��
ʵ�ֹ���    : 
����        : ��Ҳ
�汾        : 
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/08/26  5.0         ��Ҳ          ����
******************************************************************************/
#ifndef	_MAUUTILITY_H_
#define	_MAUUTILITY_H_

#pragma once
#pragma warning( disable:4786 )

#include "osp.h"
#include "mauconst.h"
#include "appdef.h"
#include "kdvlog.h"
#include "loguserdef.h"
#include "stlstruct.h"
#include "vccommon.h"
#include "maustruct.h"
#include "operationconst.h"
#include "jsontranslate.h"
#include "nppmaucommon.h"
#include "commonlib.h"
#include "mauerrcode.h"
#include "redisadapter.h"
#include <vector>

#ifdef WIN32
    #pragma pack( push )
    #pragma pack( 1 )
    #define window( x )	x
#endif

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#define MAXVALUE_GK_PREFIX   (u32)999999
#define MAXVALUE_MT_SERIALNO (u32)9999999

const u16 DUMPALLDOMAININFO = 2;
const u16 DUMPALLMTINFO = 3;
const u16 DUMPALLCONFCACHEINFO = 4;
const u16 DUMPALLTEMPCACHEINFO = 5;
const u16 DUMPALLREQINFO = 6;
const u16 DUMPCMUINFO = 7;
const u16 DUMPMCCINFO = 8;
const u16 DUMPNUINFO = 9;
const u16 DUMPAGWINFO = 10;
const u16 DUMPDBCFGINFO = 11;
const u16 DUMPBDBCONFINFO = 12;
const u16 DUMPBDBMTINFO = 13;
const u16 DUMPBDBCONFCACHEINFO = 14;
const u16 DUMPBDBDOMAININFO = 15;
const u16 DUMPBDBACCOUNTINFO = 16;
const u16 DUMPBDBAUTHMTINFO = 17;
const u16 DUMPINSTANCESTATE = 18;		//instance״̬
const u16 DUMPDBSTATUS = 20;	    	//DB״̬��Ϣ
const u16 DUMPALLHDUINFO = 23;
const u16 DUMPALLMONITORINFO = 24;     //���м����Ϣ
const u16 DUMPALLUPUINFO = 25;
const u16 DUMPALLPERSONALTEMPE164 = 26; //���и���ģ��E164��
const u16 DUMPAPDBHDUINFO = 27;         //AP���ݿ�serverinfo����HDU��Ϣ
////�����ʼ��ֱ��ʵó���Ƶģʽ(���塢�����)
//u8 GetVideoMode(const u16 wBitrate, const u8 byResolution);

//ת�������ʶ
BOOL32 GetNeedPswFlag(const s8 *pszConfPsw);

void LongLogPrint(u8 byLogLvl, u16 wModuleId, const s8* achJsonBuf, 
	u32 dwPrintBuffLen, BOOL32 bIsAutoLineFeed = TRUE);
////ָ��YYYYMMDDHH24MISS�͵�ʱ�䣬��ʽ��Ϊtime_t�͵�ʱ�� 
//TKdvTime FormatStr2KdvTime(const char * szTime);


/*
//�����ʵó���Ƶģʽ(���塢�����)
u8 GetVideoModeByBitrate( u16 wBitrate );
//ת�������ʶ
BOOL32 GetNeedPswFlag(const s8 *pszConfPsw);
*/

#define MAXLEN_TELNET_NAME				64
#define MAXLEN_TELNET_PWD				64
#define MAU_CONFINFO_HOLDTIME           30

#define      MAU_MSG_LEN					65535  //��Ϣ����
#define      MAU_MSGHEAD_LEN				6    //��Ϣͷ����
#define      MAXLEN_MAU_MSG				MAU_MSG_LEN - MAU_MSGHEAD_LEN

#define CSSCFGFILENAME				"csscfg.ini"
#define	SECTION_CSS					"CSS"
#define SECTION_TEMPLATEDB			"TemplateDB"
#define	SECTION_CONFINFODB			"ConfInfoDB"
#define SECTION_LOCALINFO			"LocalInfo"
#define SECTION_MQINFO				"MqInfo"
#define SECTION_UPUINFO				"UpuInfo"
#define SECTION_ZKINFO				"ZkInfo"
#define SECTION_MEETINGDB			"MeetingDB"
#define	SECTION_SCHEDULEDCONFINFODB	"ScheduledConfInfoDB"
#define	SECTION_APINFODB			"APInfoDB"
#define SECTION_REDIS				"RedisInfo"

#define KEY_TELNETNAME				"TelnetName"
#define KEY_TELNETPASS				"TelnetPass"
#define KEY_CONFINFOHOLDTIME		"ConfInfoHoldTime"

#define KEY_MQIPADDR				"MqIpAddr"
#define KEY_MQPORT                  "MqPort"
#define KEY_TOPLEVELMQIPADDR		"TopLevelMqIpAddr"
#define KEY_UPUIPADDR				"UpuIpAddr"
#define KEY_ZKIPADDR				"ZkIpAddr"
#define KEY_ZKPORT					"ZkPort"
#define DEF_ZKPORT					(u16)2181   //zk�˿�
#define DEF_MQPORT					(u16)5672  
#define DEF_MQCONSUMER_HEARTBEAT    (u16)30

#define KEY_IPADDR					"IpAddr"
#define KEY_PORT					"Port"
#define KEY_REDISPWD				"Pwd"
#define DEF_REDISPORT				6379

#define	KEY_TEMPSTORETYPE			"TempStoreType"
#define KEY_DBCONNTREAHDNUM			"DBConnThreadNum"
#define KEY_DBMSGQUEUENUM			"DBMsgQueueNum"
#define KEY_SERVER_RENT             "ServerRent"   // �����Ƿ�����
#define	KEY_DBIPAddr				"DBIpAddr"
#define KEY_DBPORT					"DBPort"
#define	KEY_DBNAME					"DBName"
#define KEY_DBENGINETYPE			"DBEngineType"
#define KEY_DBUSERNAME				"DBUserName"
#define KEY_DBUSERPWD				"DBUserPwd"
#define KEY_RECOVERTIMESPAN         "recovertime"
#define DEF_RECOVERTIMESPAN         (10)
#define KEY_MCUHEARTBEATTIMESPAN    "mcuheartbeattime"
#define KEY_GETCONFLISTIMESPAN      "getconflisttime"
#define DEF_MCUHEARTBEATTIMESPAN    (12)
#define DEF_GETCONFLISTTIMESPAN     (60)

#define MAU_DEF_DBIP				"127.0.0.1"
#define MAU_DEF_DBNAME				"mpcddb"//���ݿ���
#define MAU_DEF_DBPORT				(u16)3306   //���ݿ�˿�
#define MAU_DEF_DBENGINETYPE		(u16)1
#define MAU_DEF_DBUSERNAME			"kedacom"
#define MAU_DEF_DBUSERPWD			"KedaMysql16#"

#define AP_DEF_DBNAME				"ap"
#define AP_DEF_DBUSERNAME			"kedacom"
#define AP_DEF_DBUSERPWD			"KedaMysql16#"

#define MAU_DEF_CONFDBNAME			"confinfodb"
#define MAU_DEF_BOOKDBNAME			"meeting"

//rnn[2014/04/29]����hdupool��Ϣ
#define	SECTION_HDUPOOLINFO			"HduPoolInfo"
#define KEY_HDUPOOL_IPADDRESS		"HduPoolIpAddr"
#define	KEY_HDUPOOL_PORT			"HduPoolPort"

/************************************** MQ���ն��ж��� *************************************************/
// ���ջ�ܷ��͸�MAU������
#define MQ_MAU_CMMAU_EX				"mau.cmmau.ex"
#define MQ_MAU_CMMAU_K				"mau.cmmau.k"
#define MQ_MAU_CMMAU_Q				"mau.cmmau.q"
#define MQ_MAU_CMMAU_RELAY_Q		"meeting.rpcmau.q"

//����CSS���͸���ݵ�����
#define MQ_CM_CSSCM_EX				"meeting.css.ex"
#define MQ_CM_CSSCM_K				"meeting.css.k"
#define MQ_CM_CSSCM_Q				"meeting.css.q"

// ����תͶ��MCU������
#define MQ_MAU_CMMCU_EX				"mau.cmmcu.ex"
#define MQ_MAU_CMMCU_K				"mau.cmmcu.k"
#define MQ_MAU_CMMCU_Q				"mau.cmmcu.q"
#define MQ_MAU_CMMCU_REPLY_EX		"meeting.mccntf.ex"
#define MQ_MAU_CMMCU_REPLY_RK		"meeting.mccntf.k"

// ����MCU���͸�MAU��Q
#define MQ_MAU_CONF_Q				"mau.conf.q"

// ����BMC�޸�������Ϣ
#define MQ_BMC_NTF_EX				"modb.notify.ex"
#define MQ_BMC_NTF_Q				"mau.notify.q"

// ���պ�����BMC������Ȩ��Ϣ
#define MQ_TOPLEVEL_BMC_LICENSE_Q			"css.license.q:"
#define MQ_TOPLEVEL_BMC_LICENSE_NTF_Q		"css.license.ntf.q:"
#define MQ_TOPLEVEL_BMC_LICENSE_EX          "movision.license.ex"
#define MQ_TOPLEVEL_BMC_LICENSE_K           "mau.license.k:"
#define MQ_TOPLEVEL_BMC_LICENSE_PRODUCER_K  "movision.license.k"
#define MQ_TOPLEVEL_BMC_LICENSE_NOTIFY_K    "platform.license.ntf.k:"

// ����ƽ̨��BMC������Ȩ��Ϣ
#define MQ_PLATFORM_BMC_LICENSE_Q			"platform.license.q"
#define MQ_PLATFORM_BMC_LICENSE_EX          "platform.license.ex"
#define MQ_PLATFORM_BMC_LICENSE_K           "platform.license.k"
#define MQ_PLATFORM_BMC_LICENSE_NOTIFY_K    "platform.license.ntf.k"

/************************************** MQ���ݿ�����ֶζ��� *************************************************/
//���ݿ����
#define		ONGINGCONFINFO		"ongoingconfinfo"
#define		TTEMPLATEINFO		"TTemplateInfo"
#define		TCONFINFO			"TConfInfo"
#define		SCHEDULEDINFO		"tscheduledconfinfo"
#define     SERVDOMAIN          "service_domain"
#define     PLATFORMDOMAIN      "platform_domain"
#define     USERDOMAIN          "user_domain"
#define     USERINFO            "user_info"
#define		SERVERINFO			"server_info"
//�ֶ���
#define		TTEMPLATEKEY		"TemplateKey"
#define		TTEMPLATENAME		"TemplateName"
#define		TDOMAINNAME			"DomainName"
#define		TDOMAINGUID			"DomainGUID"
#define		TCREATETIME			"CreateTime"
#define		TLASTMODIFYTIME		"LastMdfTime"
#define		TTEMPLATEDATA		"TemplateData"
#define     TDOMAINMOID         "DomainMoID"

#define		TCONFE164			"ConfE164"
#define		TCONFNAME			"ConfName"
#define		TNEEDPASSWORD		"NeedPassword"
#define		TCONFPASSWORD		"ConfPassword"
#define		TCONFGUID		    "Guid"
#define     TCONFID             "ConfId"

#define     TMEETINGID          "MeetingID"
#define     TSHOTID             "ShotID"
#define     TSTARTTIME          "StartTime"
#define     TENDTIME            "EndTime"
#define     TCONFDATA           "ConfData"

// APDB
#define     DOMAIN_GUID         "domain_guid"
#define     USEDENABLE          "enable"
#define     SERVERMOID			"server_moid"
// APDB-������
#define     SERVDOMAINMOID      "service_domain_moid"
#define     SERVDOMAINNAME      "service_domain_name"

// ƽ̨��
#define     PLATFORMDOMAINMOID  "platform_domain_moid"
#define     PLATFORMDOMAINNAME  "platform_domain_name"
#define     PLATFOMRDOMAINPREFIX "number_segment"

#define     BMCLICENSE_192_1080P_CONFNUM_LIMIT "KEY_192_1080P_HD_CONFERENCE_LIMIT"
#define     BMCLICENSE_192_720P_CONFNUM_LIMIT  "KEY_192_720P_HD_CONFERENCE_LIMIT"
#define     BMCLICENSE_64_1080P_CONFNUM_LIMIT  "KEY_64_1080P_HD_CONFERENCE_LIMIT"
#define     BMCLICENSE_64_720P_CONFNUM_LIMIT   "KEY_64_720P_HD_CONFERENCE_LIMIT"
#define     BMCLICENSE_32_1080P_CONFNUM_LIMIT  "KEY_32_1080P_HD_CONFERENCE_LIMIT"
#define     BMCLICENSE_32_720P_CONFNUM_LIMIT   "KEY_32_720P_HD_CONFERENCE_LIMIT"
#define     BMCLICENSE_8_1080P_CONFNUM_LIMIT   "KEY_8_1080P_HD_CONFERENCE_LIMIT"
#define     BMCLICENSE_8_720P_CONFNUM_LIMIT    "KEY_8_720P_HD_CONFERENCE_LIMIT"
#define     BMCLICENSE_MCU_TYPE				   "KEY_MCU_TYPE"
// �û���
#define		USERMOID			 "moid"
#define     USERDOMAINMOID		 "user_domain_moid"
#define     USERDOMAINNAME       "user_domain_name"
#define     USERDOMAINUSEDFLAG   "used_flag"
#define     USERDOMAINMAXCONFNUM "concurrent_number"

#define     APDB_192_1080P_CONFNUM_LIMIT "large_1080_hd_conference"
#define     APDB_192_720P_CONFNUM_LIMIT  "large_720_hd_conference"
#define     APDB_64_1080P_CONFNUM_LIMIT  "medium_64_1080_conference"
#define     APDB_64_720P_CONFNUM_LIMIT   "medium_64_720_conference"
#define     APDB_32_1080P_CONFNUM_LIMIT  "medium_32_1080_conference"
#define     APDB_32_720P_CONFNUM_LIMIT   "medium_32_720_conference"
#define     APDB_8_1080P_CONFNUM_LIMIT   "small_1080_hd_conference"
#define     APDB_8_720P_CONFNUM_LIMIT    "small_720_hd_conference"

#define     BMC_192_1080P_CONFNUM_LIMIT "large1080PHDConference"
#define     BMC_192_720P_CONFNUM_LIMIT  "large720PHDConference"
#define     BMC_64_1080P_CONFNUM_LIMIT  "medium64_1080PHDConference"
#define     BMC_64_720P_CONFNUM_LIMIT   "medium64_720PHDConference"
#define     BMC_32_1080P_CONFNUM_LIMIT  "medium32_1080PHDConference"
#define     BMC_32_720P_CONFNUM_LIMIT   "medium32_720PHDConference"
#define     BMC_8_1080P_CONFNUM_LIMIT   "small1080PHDConference"
#define     BMC_8_720P_CONFNUM_LIMIT    "small720PHDConference"

// ��������Ϣ
#define		SERVERTYPE			"server_type"

// MT
#define     MTE164NO             "e164"
#define     MTTYPE               "device_type"
#define     MTLIMITED            "limited"


#define     JSON_REQID			  "reqid"            //����ģ�����������ַ���
/************************************** ��ܽ������� ***************************************************/
// ���ģ��
#define EV_CM_MAU_CREATETEMPLATE_REQ				"CM_MAU_CREATETEMPLATE_REQ"
#define EV_MAU_CM_CREATETEMPLATE_ACK				"MAU_CM_CREATETEMPLATE_ACK"
#define EV_MAU_CM_CREATETEMPLATE_NACK				"MAU_CM_CREATETEMPLATE_NACK"
// �޸�ģ��
#define EV_CM_MAU_MDFTEMPLATE_REQ					"CM_MAU_MDFTEMPLATE_REQ"
#define EV_MAU_CM_MDFTEMPLATE_ACK					"MAU_CM_MDFTEMPLATE_ACK"
#define EV_MAU_CM_MDFTEMPLATE_NACK					"MAU_CM_MDFTEMPLATE_NACK"
// ɾ��ģ��
#define EV_CM_MAU_DELTEMPLATE_REQ					"CM_MAU_DELTEMPLATE_REQ"
#define EV_MAU_CM_DELTEMPLATE_ACK					"MAU_CM_DELTEMPLATE_ACK"
#define EV_MAU_CM_DELTEMPLATE_NACK					"MAU_CM_DELTEMPLATE_NACK"
//����ģ��
#define EV_MAU_CM_ALLPERSONALCONFTEMPLATE_REQ		"MAU_CM_ALLPERSONALCONFTEMPLATE_REQ"
#define EV_CM_MAU_ALLPERSONALCONFTEMPLATE_ACK		"CM_MAU_ALLPERSONALCONFTEMPLATE_ACK"
#define EV_CM_MAU_PERSONALCONFTEMPLATE_NTF          "CM_MAU_PERSONALCONFTEMPLATE_NTF"
#define EV_CM_MAU_DELPERSONALCONFTEMPLATE_NTF		"CM_MAU_DELPERSONALCONFTEMPLATE_NTF"
// ���ԤԼ����
#define EV_CM_MAU_ADDBOOKCONF_REQ					"CM_MAU_ADDBOOKCONF_REQ"
#define EV_MAU_CM_ADDBOOKCONF_ACK					"MAU_CM_ADDBOOKCONF_ACK"
#define EV_MAU_CM_ADDBOOKCONF_NACK					"MAU_CM_ADDBOOKCONF_NACK"
// �޸�ԤԼ����
#define EV_CM_MAU_MDFBOOKCONF_REQ					"CM_MAU_MDFBOOKCONF_REQ"
#define EV_MAU_CM_MDFBOOKCONF_ACK					"MAU_CM_MDFBOOKCONF_ACK"
#define EV_MAU_CM_MDFBOOKCONF_NACK					"MAU_CM_MDFBOOKCONF_NACK"
// ɾ��ԤԼ����
#define EV_CM_MAU_DELBOOKCONF_REQ					"CM_MAU_DELBOOKCONF_REQ"
#define EV_MAU_CM_DELBOOKCONF_ACK					"MAU_CM_DELBOOKCONF_ACK"
#define EV_MAU_CM_DELBOOKCONF_NACK					"MAU_CM_DELBOOKCONF_NACK"
// ԤԼ�������ٿ�
#define EV_CM_MAU_BOOKCONFONGOING_NTF				"CM_MAU_BOOKCONFONGOING_NTF"
// ��������
#define EV_CM_MAU_CREATECONF_REQ					"CM_MAU_CREATECONF_REQ"
#define EV_MAU_CM_CREATECONF_ACK					"MAU_CM_CREATECONF_ACK"
#define EV_MAU_CM_CREATECONF_NACK					"MAU_CM_CREATECONF_NACK"

// api��������
#define EV_CM_MAU_CREATEAPICONF_REQ					"CM_MAU_CREATEAPICONF_REQ"
#define EV_MAU_CM_CREATEAPICONF_ACK					"MAU_CM_CREATEAPICONF_ACK"
#define EV_MAU_CM_CREATEAPICONF_NACK				"MAU_CM_CREATEAPICONF_NACK"

// ��������
#define EV_CM_MAU_RELEASECONF_REQ					"CM_MAU_RELEASECONF_REQ"
#define EV_MAU_CM_RELEASECONF_ACK					"MAU_CM_RELEASECONF_ACK"
#define EV_MAU_CM_RELEASECONF_NACK					"MAU_CM_RELEASECONF_NACK"
// ���ͨ��ģ�崴������
#define EV_CM_MAU_CREATECONFBYTEMP_REQ				"CM_MAU_CREATECONFBYTEMP_REQ"
#define EV_MAU_CM_CREATECONFBYTEMP_ACK				"MAU_CM_CREATECONFBYTEMP_ACK"
#define EV_MAU_CM_CREATECONFBYTEMP_NACK				"MAU_CM_CREATECONFBYTEMP_NACK"
// �����������б�CMD
#define EV_CM_MAU_GETCONFLIST_CMD					"CM_MAU_GETCONFLIST_CMD"
#define EV_CM_MAU_GETCONFLIST_REQ					"CM_MAU_GETCONFLIST_REQ"
#define EV_MAU_CM_GETCONFLIST_ACK					"MAU_CM_GETCONFLIST_ACK"
#define EV_MAU_CM_GETCONFLIST_NACK					"MAU_CM_GETCONFLIST_NACK"


// MAU���ܷ��ͻ����б�NTF
#define EV_MAU_CM_CONFLIST_NTF						"MAU_CM_CONFLIST_NTF"

// �������֪ͨ
#define EV_MAU_CM_RELEASECONF_NTF					"MAU_CM_RELEASECONF_NTF"
	
/************************************** MAU����Эͬ�������� ***************************************************/
// ����ʱ�����б�CMD
#define EV_MAU_MAU_GETCONFINFO_CMD					"MAU_MAU_GETCONFINFO_CMD"
// ��ʱ�����б�NTF
#define EV_MAU_MAU_ADDCONFINFO_NTF					"MAU_MAU_ADDCONFINFO_NTF"
// �޸ļ�ʱ�����б�NTF
#define EV_MAU_MAU_MDFCONFINFO_NTF					"MAU_MAU_MDFCONFINFO_NTF"

// ����ԤԼ�����б�CMD
#define EV_MAU_MAU_GETSCHECONFINFO_CMD				"MAU_MAU_GETSCHECONFINFO_CMD"
// ԤԼ�����б�NTF
#define EV_MAU_MAU_ADDSCHECONFINFO_NTF				"MAU_MAU_ADDSCHECONFINFO_NTF"
// �޸�ԤԼ�����б�NTF
#define EV_MAU_MAU_MDFSCHECONFINFO_NTF				"MAU_MAU_MDFSCHECONFINFO_NTF"

// ���ģ��NTF
#define EV_MAU_MAU_ADDTEMPLATE_NTF					"MAU_MAU_ADDTEMPLATE_NTF"
// �޸�ģ��NTF
#define EV_MAU_MAU_MDFTEMPLATE_NTF					"MAU_MAU_MDFTEMPLATE_NTF"

// hdu��Ϣ�ϱ����
#define EV_MAU_CM_HDUINFO_NTF						"MAU_CM_HDUINFO_NTF"
// ���������ȡhdu��Ϣ
#define EV_CM_MAU_GETHDUINFO_CMD					"CM_MAU_GETHDUINFO_CMD"
// �ϱ�����hdu��Ϣ�����
#define EV_MAU_CM_ALLHDUINFO_NTF					"MAU_CM_ALLHDUINFO_NTF"

#define EV_CM_MAU_GETHDUINFOBYMOID_REQ				"CM_MAU_GETHDUINFOBYMOID_REQ"
#define EV_MAU_CM_GETHDUINFOBYMOID_ACK				"MAU_CM_GETHDUINFOBYMOID_ACK"
#define EV_MAU_CM_GETHDUINFOBYMOID_NACK				"MAU_CM_GETHDUINFOBYMOID_NACK"

// ���õ���ǽ����
#define EV_CM_MAU_SETHDUVOLUME_REQ					"CM_MAU_SETHDUVOLUME_REQ"
#define	EV_MAU_CM_SETHDUVOLUME_ACK					"MAU_CM_SETHDUVOLUME_ACK"
#define	EV_MAU_CM_SETHDUVOLUME_NACK					"MAU_CM_SETHDUVOLUME_NACK"

// �ӳ�����
#define EV_CM_MAU_DELAYCONF_REQ                     "CM_MAU_DELAYCONF_REQ"
#define EV_MAU_CM_DELAYCONF_ACK                     "MAU_CM_DELAYCONF_ACK"
#define EV_MAU_CM_DELAYCONF_NACK                    "MAU_CM_DELAYCONF_NACK"

//CM����MCU�б�
#define EV_CM_MAU_GETMCULIST_REQ					"CM_MAU_GETMCULIST_REQ"
#define EV_MAU_CM_GETMCULIST_ACK					"MAU_CM_GETMCULIST_ACK"
#define EV_MAU_CM_GETMCULIST_NACK					"MAU_CM_GETMCULIST_NACK"
#define EV_MAU_CM_MCULIST_NTF						"MAU_CM_MCULIST_NTF"

//��ܼ�ر���֪ͨ
#define EV_CM_MAU_MONITORKEEPALIVE_NTF				"CM_MAU_MONITORKEEPALIVE_NTF"

#define	MAX_REQ_NUM				(u32)10000000

//����ʱʱ�� 5����
#define	REQ_TIMEOUT_SPAN		(u32)300

//ԤԼ�����������ʱ����
#define OrderConfCheckTime      (u32)(30 * 1000)
//ԤԼ����E164���ʱ����
#define OrderConfE164CheckTime  (u32)(10 * 1000)
//��ʱ�������ʱ����ʱ����
#define CHECKCONF_TIME_SPAN     (u32)(60 * 1000)

//�������ģ����ʱ��
#define PERSONALTMPACK_TIME_SPAN (u32)(60 * 1000)

//���redis�����Ƿ�һ��
#define CHECK_REDISDATA_SPAN    (u32)(180 * 1000)

#define INIT_REDISCONF_SPAN     (u32)(1000)
#define INIT_UPUADDBOOKCONF_SPAN     (u32)(1000)

// Ĭ�ϻ���ʱ��
#define DEFAULT_CONF_DURATION	(4 * 60)
//ԤԼ�����ȡʱ����
#define OrderConfReadTime      (u32)(86400 * 1000)

#define CREATECONF_TIMEOUT_SPAN 10

/*
enum en_MpcdVcPrintLevel
{
	em_PrintExcept = (u8)0,
	em_PrintNormal = (u8)1,
	em_PrintDetail = (u8)2
};
*/


//���IP�ַ�������
#define MAXLEN_IPADDR (u16)16

//��ǰϵͳʱ��buf��󳤶�
#define MAXLEN_CURTIME		   (u16)255

//���ݿ�����
enum EMDBEngineType
{
    enumDBEngineTypeSybase      = (u8)1,
	enumDBEngineTypeMySql       = (u8)2,
	enumDBEngineTypeOracle      = (u8)3,
};

class CPlatformConfNumLimit
{
public:
	CPlatformConfNumLimit(u32 dwSmallLimit = 0, u32 dwLargeLimit = 0) 
		:m_dwMaxSmallConfNum(dwSmallLimit),
		m_dwMaxLargeConfNum(dwLargeLimit) {}
	u32 GetSmallConfNumLimit() const
	{
		return m_dwMaxSmallConfNum;
	}
	void SetSmallConfNumLimit(const u32 dwSmallConfNumLimit)
	{
		m_dwMaxSmallConfNum= dwSmallConfNumLimit;
	}
	u32 GetLargeConfNumLimit() const
	{
		return m_dwMaxLargeConfNum;
	}
	void SetLargeConfNumLimit(const u32 dwLargeConfNumLimit)
	{
		m_dwMaxLargeConfNum= dwLargeConfNumLimit;
	}
private:
	u32 m_dwMaxSmallConfNum;
	u32 m_dwMaxLargeConfNum;
};

//MAU������״̬
enum EMConfState
{
	enumInvalidState			= (u8)0,
	//׼��״̬,MAU�Ѿ���MPC���ʹ�������
	enumPrepareState			= (u8)1,
	//����״̬,�յ�MPC�ɹ������֪ͨ
	enumRunningState			= (u8)2,
    //�ȴ��ָ�״̬����ӦMPC�������
    enumConnectWaitingState     = (u8)3,
	//��ʼ��״̬����Ӧ�ָ����״̬
	enumInitedState				= (u8)4
};

static const s8* const s_confstate[] = {
	"Invalid",
	"Prepare",
	"Running",
	"ConnectWaiting",
	"Inited"
};

static const s8* ConfStateToString(u8 byState)
{
	if (byState < _countof(s_confstate))
	{
		return s_confstate[byState];
	}
	return "";
}

static const s8* const s_achConfNumType[] = {
	"licensed",
	"maxlicensed",
	"unlicensed",
	"licensed_small",
	"licensed_large"
};

static const s8* ConfNumTypeToString(const u8 byConfNumType)
{
	return (byConfNumType < _countof(s_achConfNumType) ? s_achConfNumType[byConfNumType] : "");
}

// ���AddrΪ������
const s8* iptostr(const void *pAddr, s8 *pchIpAddr, u32 dwIpAddrLen);

//MAU��CMU��״̬
enum EMMpcState
{
    enumMpcDisconnectState       = 0,
    enumMpcConnectState,
    enumMpcConnectWaitingState
};

//ģ����Ϣ�ڴ滺��ṹ
class CTempCacheData
{
private:
	s8  m_achConfName[MAXLEN_CONFNAME+1]; //������
	s8  m_achConfE164[MAXLEN_CONFE164+1];     //�����E164����
	s8	m_achDomainGUID[OPERATION_MAXLEN_GUID+1];	//�����������GUID
	s8	m_achDomainName[OPERATION_MAXLEN_DOMAINNAME+1];	//���������������
	u8  m_byResolution;							//�ֱ��� VIDEO_FORMAT_AUTO, ...
	u16 m_wBitRate;					            //��������(��λ:Kbps,1K=1024)
	s8  m_achConfPwd[MAXLEN_PWD+1];             //��������
	u8  m_bySatDCastMode;                       //�Ƿ����Ƿ�ɢ���飺0-���ǣ�1-��
	u8  m_byPublicConf;							//�Ƿ񿪷Ż��飺0-���ǣ�1-��

	//rnn[2014/03/13]���ӻ�����Ϣ�������ն˻�ȡģ����ϸ��Ϣʱֱ�Ӵӻ����ȡ
	//����Ƶ����ȡ���ݿ����ӵ��
	s8  m_achDomainMOID[OPERATION_MAXLEN_MOID+1];	//���MOID
	u8        m_byOpenMode;						//����ģʽ��1-����������� 2-��ȫ����
	u8        m_byMixMode;
    u16       m_wDuration;						//����ʱ��(����)
	u8	      m_byDualMode;						//�����˫������ʽ: 0-������ 1-�����ն�
	u8		  m_byEncryptMode;					//�������ģʽ
	u8		  m_byHasXmpuSer;					//��ģ���������Ƿ�����xmpu������
	u8		  m_byConfNoDisturb;				//�����Ƿ��ն������
	u8        m_byPortMode;						// �Ƿ�Ϊ�˿ڻ���
	u8        m_byMaxJoinMtNum;					// �������ն���
	u8        m_byMediaType;					// ý������

public:
	CTempCacheData()
	{
		SetNull();
	}

	//�ÿ�
	void SetNull( void )
	{
		memset( m_achConfName,   0, sizeof(m_achConfName) );
		memset( m_achConfE164,   0, sizeof(m_achConfE164) );
		memset( m_achDomainGUID, 0, sizeof(m_achDomainGUID) );
		memset( m_achDomainName, 0, sizeof(m_achDomainName) );
		memset( m_achConfPwd,    0, sizeof(m_achConfPwd) );
		m_byResolution   = VIDEO_FORMAT_INVALID;
		m_wBitRate       = 0;
		m_bySatDCastMode = 0;
		m_byPublicConf = 0;

		memset( m_achDomainMOID, 0, sizeof(m_achDomainMOID) );
		m_byOpenMode = CONF_OPENMODE_OPEN;
		m_byMixMode = 0;
		m_byDualMode = 0;
		m_byEncryptMode = 0;
		m_wDuration = 0;
		m_byHasXmpuSer = 0;
		m_byConfNoDisturb = 0;
		m_byMaxJoinMtNum = 0;
		m_byMediaType = 0;
	}

	void ToSimpleConfInfo(TSimpleConfInfo& tSimpleConfInfo) const
	{
		tSimpleConfInfo.SetConfE164( m_achConfE164, MAXLEN_CONFE164 );
		tSimpleConfInfo.SetConfName( m_achConfName );
		tSimpleConfInfo.SetConfMode(GetVideoMode(GetMediaType(), GetBitRate(), GetResolution()));
		tSimpleConfInfo.SetNeedPsw( GetNeedPswFlag(m_achConfPwd) );
		tSimpleConfInfo.SetPublicConf(m_byPublicConf);

		if( IsSatDCastMode() )
		{
			tSimpleConfInfo.SetSatDCastMode( TRUE );
		} 
		else
		{
			tSimpleConfInfo.SetSatDCastMode( FALSE );
		}
	}

	//rnn[2014/03/13]
	void ToConfDetailInfo(TConfDetailInfo& tDetailConf)
	{
		tDetailConf.m_tNewConfBaseInfo.SetConfE164( m_achConfE164 );
		tDetailConf.m_tNewConfBaseInfo.SetConfName( m_achConfName );
		tDetailConf.m_tNewConfBaseInfo.SetDomainGUID( m_achDomainGUID );
		tDetailConf.m_tNewConfBaseInfo.SetDomainMOID( m_achDomainMOID );
		tDetailConf.m_tNewConfBaseInfo.SetDomainName( m_achDomainName );
		
		tDetailConf.SetConfStyle(CONF_STYLE_TEMPLATE);
		tDetailConf.SetEncryptMode( m_byEncryptMode );
		//zengjie[2013/05/25] ��Ƶ������24K����Ϊ64K���������ն���ʾ����ȷ������
		u16 wConfBitRate = GetBitRate();
		if (0 != wConfBitRate%64)
		{
			wConfBitRate = wConfBitRate - 40;		//40 = 64 - 24
		}
		tDetailConf.SetBitRate( wConfBitRate );
		tDetailConf.SetResolution(m_byResolution);
		tDetailConf.SetOpenMode( m_byOpenMode );
		tDetailConf.SetMixMode(m_byMixMode);
		tDetailConf.SetDiscuss(m_byMixMode == 0 ? 0 : 1);

		tDetailConf.SetDuration( DEFAULT_CONF_DURATION );
		tDetailConf.SetDualMode( m_byDualMode );
		tDetailConf.SetConfMode(GetVideoMode(GetMediaType(), GetBitRate(), GetResolution()));
		tDetailConf.SetSatDCastMode( 1 == m_bySatDCastMode );
		tDetailConf.SetPublicConf( m_byPublicConf );
		tDetailConf.SetPortModeConf(m_byPortMode);
		tDetailConf.SetNeedPsw(m_achConfPwd[0] != 0);
		tDetailConf.SetCloseMode(IsConfNoDisturb());
		tDetailConf.SetMaxJoinMtNum(GetMaxJoinMtNum());
	}

	// �������ն���
	void   SetMaxJoinMtNum(u8 byMaxJoinMtNum)
	{
		m_byMaxJoinMtNum = byMaxJoinMtNum;
	}
	u8	   GetMaxJoinMtNum(void) const
	{
		return m_byMaxJoinMtNum;
	}

	//DomainMOID
	void SetDomainMOID( LPCSTR lpszDomainMOID )
    {
        if (NULL != lpszDomainMOID)
        {
            strncpy( m_achDomainMOID, lpszDomainMOID, sizeof(m_achDomainMOID) );
			m_achDomainMOID[sizeof( m_achDomainMOID ) - 1] = '\0';
        }
		else
		{
			memset( m_achDomainMOID, 0, sizeof( m_achDomainMOID ) );
		}
        return;
    }
	LPCSTR GetDomainMOID( void ) const 
	{ 
		return m_achDomainMOID; 
	}

	//EncryptMode
 	u8 GetEncryptMode() const 
 	{ 
	 	return m_byEncryptMode; 
 	}
 	void SetEncryptMode(u8 byMode)
 	{ 
 		m_byEncryptMode = byMode; 
 	}

	void SetMixMode(u8 byMixMode)
	{
		m_byMixMode = byMixMode;
	}
	u8 GetMixMode(void) const
	{
		return m_byMixMode;
	}
	
	//����ģʽ
	void SetOpenMode(u8   byOpenMode)
	{ 
		m_byOpenMode = byOpenMode;
	} 
    u8 GetOpenMode( void ) const 
	{ 
		return m_byOpenMode; 
	}

	//Duration
	u16 GetDuration() const 
	{ 
		return m_wDuration; 
	}
	void SetDuration(u16 wDuration) 
	{
		m_wDuration = wDuration; 
	}

	//�����˫������ʽ
	void   SetDualMode(	u8 byDualMode )
	{ 
		m_byDualMode = byDualMode;	
	}
	u8	   GetDualMode( void ) const 
	{ 
		return m_byDualMode;	
	}
	//end

	void   SetBitRate(u16  wBitRate){ m_wBitRate = wBitRate;} 
    u16    GetBitRate( void ) const { return m_wBitRate; }
	
	void    SetResolution(u8 byRes) { m_byResolution = byRes; }
    u8      GetResolution(void) const { return m_byResolution; }

	//m_bySatDCastMode
	void SetSatDCastMode( BOOL32 bSatDCastMode )
	{ 
		if ( bSatDCastMode )
		{
			m_bySatDCastMode = 1;
		}
		else
		{
			m_bySatDCastMode = 0;
		}
	}
	BOOL32 IsSatDCastMode( void ) const 
	{ 
		return ( 1 == m_bySatDCastMode ) ? TRUE:FALSE; 
	}

	//����
	const s8* GetConfPsw() const
	{ 
		return m_achConfPwd; 
	}
	void SetConfPsw(const s8* pszConfPsw) 
	{ 
		memset( m_achConfPwd, 0, sizeof(m_achConfPwd) );
        if (NULL != pszConfPsw)
        {
            strncpy( m_achConfPwd, pszConfPsw, sizeof(m_achConfPwd)-1 );
			m_achConfPwd[sizeof( m_achConfPwd ) - 1] = '\0';
        }
        return;
	}

	//����/��ȡ ��������
	void SetConfName( LPCSTR lpszConfName )
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
    LPCSTR GetConfName( void ) const { return m_achConfName; }

	//����/��ȡ����E164
	void   SetConfE164( LPCSTR lpszConfE164 )
	{
		if( lpszConfE164 != NULL )
		{
			strncpy( m_achConfE164, lpszConfE164, sizeof( m_achConfE164 ) );
			m_achConfE164[sizeof( m_achConfE164 ) - 1] = '\0';
		}
		else
		{
			memset( m_achConfE164, 0, sizeof( lpszConfE164 ) );
		}
	}
	LPCSTR GetConfE164( void ) const { return m_achConfE164; }

	//����/��ȡ �����������GUID
	void   SetDomainGUID( LPCSTR lpszDomainGUID )
	{
		if( lpszDomainGUID != NULL )
		{
			strncpy( m_achDomainGUID, lpszDomainGUID, sizeof( m_achDomainGUID ) );
			m_achDomainGUID[sizeof( m_achDomainGUID ) - 1] = '\0';
		}
		else
		{
			memset( m_achDomainGUID, 0, sizeof( m_achDomainGUID ) );
		}
	}
	LPCSTR GetDomainGUID( void ) const { return m_achDomainGUID; }

	//����/��ȡ ���������������
	void   SetDomainName( LPCSTR lpszDomainName )
	{
		if( lpszDomainName != NULL )
		{
			strncpy( m_achDomainName, lpszDomainName, sizeof( m_achDomainName ) );
			m_achDomainName[sizeof( m_achDomainName ) - 1] = '\0';
		}
		else
		{
			memset( m_achDomainName, 0, sizeof( m_achDomainName ) );
		}
	}
	LPCSTR GetDomainName( void ) const { return m_achDomainName; }

	//��ȡ��������к� #����λ����
// 	u32 GetConfSerialCode( void ) const
//     {
//         u32 dwSerialCode = 0;
//         if ( strlen(m_achConfE164) != MAXLEN_CONFE164 )
//         {
//             return dwSerialCode;
//         }
//         s8 * pchSerial = (s8 *)m_achConfE164+strlen(m_achConfE164)-5;
//         dwSerialCode = atol( pchSerial );
//         return dwSerialCode;
//     }

	//rnn[2014/10/17]��ȡģ������кź�4λ
	u32 GetTempSerialCode( void ) const
    {
        u32 dwSerialCode = 0;
        if ( strlen(m_achConfE164) != MAXLEN_CONFE164 )
        {
            return dwSerialCode;
        }

		s8 achSerial[OPERATION_MAXLEN_SERIALNO+1] = {0};
		memcpy( achSerial, m_achConfE164 + OPERATION_MAXLEN_PLATFORMPREFIX, OPERATION_MAXLEN_SERIALNO );
		achSerial[OPERATION_MAXLEN_SERIALNO] = '\0';
        dwSerialCode = atol( achSerial );
        return dwSerialCode;
    }

	//rnn
	void SetPublicConf(u8 byPublicConf) 
	{ 
		m_byPublicConf = byPublicConf;
	}
    u8 GetPublicConf(void) const
	{ 
		return m_byPublicConf; 
	}

	//HasXmpuSer
	u8 GetHasXmpuSer() const 
	{ 
		return m_byHasXmpuSer; 
	}
	void SetHasXmpuSer(u8 byHasXmpuSer)
	{ 
		m_byHasXmpuSer = byHasXmpuSer; 
 	}

	//ConfClosed
	BOOL32 IsConfNoDisturb() const
	{
		return m_byConfNoDisturb == 1 ? TRUE : FALSE;
	}
	void SetConfNoDisturb(u8 byConfNoDisturb)
	{ 
		m_byConfNoDisturb = byConfNoDisturb;
	}
	//end

	BOOL32 IsPortModeConf() const
	{
		return m_byPortMode == 1 ? TRUE : FALSE;
	}
	void SetPortModeConf(BOOL32 byPortMode)
	{
		m_byPortMode = (TRUE == byPortMode) ? 1 : 0;
	}

	void SetMediaType(const u8 byMediaType)
	{
		m_byMediaType = byMediaType;
	}
	u8 GetMediaType() const
	{
		return m_byMediaType;
	}

	void Print( void ) const
	{
		OspPrintf( TRUE, FALSE, "Template Name is:%s\n", m_achConfName );
		OspPrintf( TRUE, FALSE, "Template E164 is:%s\n", m_achConfE164 );
		OspPrintf( TRUE, FALSE, "Template's Domain Name is:%s\n", m_achDomainName );
		OspPrintf( TRUE, FALSE, "Template's Domain GUID is:%s\n", m_achDomainGUID );
		OspPrintf( TRUE, FALSE, "Template's Domain MOID is:%s\n", m_achDomainMOID );
		OspPrintf( TRUE, FALSE, "Template's bitrate is:%d\n", GetBitRate() );
		OspPrintf( TRUE, FALSE, "Template's resolution is:%d\n", GetResolution() );
		OspPrintf( TRUE, FALSE, "Template's psw is:%s\n", GetConfPsw() );
		OspPrintf( TRUE, FALSE, "Template's is sat:%d\n", IsSatDCastMode() );
		OspPrintf( TRUE, FALSE, "Template's is public:%d\n", GetPublicConf() );
		OspPrintf( TRUE, FALSE, "Template's is open:%d\n", GetOpenMode() );
		OspPrintf(TRUE, FALSE,  "Template's is mixmode:%d\n", GetMixMode());
		OspPrintf( TRUE, FALSE, "Template's is duration:%d\n", GetDuration() );
		OspPrintf( TRUE, FALSE, "Template's dual mode:%d\n", GetDualMode() );
		OspPrintf( TRUE, FALSE, "Template's encrypt mode:%d\n", GetEncryptMode() );
		OspPrintf( TRUE, FALSE, "Template's Domain has Xmpu Server:%d\n", GetHasXmpuSer() );
		OspPrintf( TRUE, FALSE, "Template's Conf is No Disturb:%d\n", IsConfNoDisturb() );	
		OspPrintf( TRUE, FALSE, "Template's Conf is PortMode:%d\n", IsPortModeConf() );
		OspPrintf(TRUE, FALSE, "Template's Max Join Mt Num:%d\n", GetMaxJoinMtNum());
		OspPrintf(TRUE, FALSE, "Template's MediaType:%d\n\n", GetMediaType());
	}
};

typedef struct tagMonitorAliveInfo
{
	s8 m_achIpAddr[20];
	s8 m_achPort[6];
	s8 m_achConfE164[MAXLEN_CONFE164 + 1];
	s8 m_achMcuIpAddr[20];
	time_t m_tAliveTime;
	u8 m_byMediaMode;
	tagMonitorAliveInfo()
	{
		memset(this, 0, sizeof(tagMonitorAliveInfo));
	}
} TMonitorAliveInfo;

typedef struct tagDBConf
{
	s8 m_achConfPwd[MAXLEN_PWD + 1];       //��������
	s8 m_achConfName[MAXLEN_CONFNAME + 1]; //������
	s8 m_achConfE164[MAXLEN_CONFE164 + 1]; //�����E164����
	tagDBConf()
	{
		memset(this, 0, sizeof(tagDBConf));
	}
	void SetConfPwd(LPCSTR lpszConfPwd)
	{
		if (lpszConfPwd != NULL)
		{
			strncpy(m_achConfPwd, lpszConfPwd, sizeof(m_achConfPwd) - 1);
		}
		else
		{
			memset(m_achConfPwd, 0, sizeof(m_achConfPwd));
		}
	}
	const s8 * GetConfPwd(void) const
	{
		return m_achConfPwd;
	}
	BOOL32 HasPwd() const
	{
		return m_achConfPwd[0] == 0 ? FALSE : TRUE;
	}
	void SetConfName(LPCSTR lpszConfName)
	{
		if (lpszConfName != NULL)
		{
			strncpy(m_achConfName, lpszConfName, sizeof(m_achConfName) - 1);
		}
		else
		{
			memset(m_achConfName, 0, sizeof(m_achConfName));
		}
	}
	const s8 * GetConfName(void) const
	{
		return m_achConfName;
	}
	void SetConfE164(LPCSTR lpszConfE164)
	{
		if (lpszConfE164 != NULL)
		{
			strncpy(m_achConfE164, lpszConfE164, sizeof(m_achConfE164) - 1);
		}
		else
		{
			memset(m_achConfE164, 0, sizeof(m_achConfE164));
		}
	}
	const s8 * GetConfE164(void) const
	{
		return m_achConfE164;
	}
}TDBConf;

//MPC��������Ϣ
struct TCmuData
{
	TCmuInfo m_tInfo;
	u8	    m_byConnectState;   //����״̬
	BOOL32	m_bDisable;
public:
	TCmuData()
	{
		SetNull();
	}

	void SetNull( void )
	{
		m_tInfo.SetNull();
		m_byConnectState = enumMpcDisconnectState;
		m_bDisable = FALSE;
	}

	void SetConnectState( u8 byConnectState ) { m_byConnectState = byConnectState; }
    u8   ConnectState( void ) const { return m_byConnectState; }

	u8 GetMaxConfNum() const
	{
		return m_tInfo.byMaxConfNum;
	}
	LPCSTR GetCmuType( void ) const
	{
		return m_tInfo.achMcuType;
	}
	LPCSTR GetCmuIp(void) const
	{
		return m_tInfo.achMcuIP;
	}
	LPCSTR GetCmuPid(void) const
	{
		return m_tInfo.achMcuPid;
	}
	void Enable(BOOL32 bEnable)
	{
		m_bDisable = !bEnable;
	}
	void Print( void ) const
	{
		OspPrintf(TRUE, FALSE, "McuIp %s:%s is %s\n", m_tInfo.achMcuIP, m_tInfo.achMcuPid, m_bDisable ? "Disable" : "Enable");
		OspPrintf(TRUE, FALSE, "Mcu Type:%s, MaxConf:%d.\n", GetCmuType(), GetMaxConfNum());
	}
};
typedef std::map<u32, TCmuData> MapCmuData;

//Gk��������Ϣ
struct TNuData
{
	BOOL32	m_bOnline;   //����״̬
	u32     m_dwNuIp;    //IP��ַ
	
public:
	TNuData()
	{
		SetNull();
	}
	
	void SetNull( void )
	{
		m_bOnline = FALSE;
		m_dwNuIp = 0;
	}
	
	void   SetIsOnline( BOOL32 bOnline ) { m_bOnline = bOnline; }
    BOOL32 IsOnline( void ) const { return m_bOnline; }
    
    void SetIpAddr( u32 dwIpAddr ) { m_dwNuIp = htonl(dwIpAddr); }
    u32  GetIpAddr( void ) const { return ntohl(m_dwNuIp); }
	
	void Print( void )
	{
		OspPrintf( TRUE, FALSE, "Nu Regist Ip is %s\n", strofip(ntohl(m_dwNuIp)) );
	}
};

struct TDBConfInfo
{
	s32 m_dwMeetingId;
	u8 m_byState;
	s8 m_abyE164[MAXLEN_E164 + 1];
};

static void WaitForThreadExit(TASKHANDLE hTask)
{
#ifdef WIN32
	DWORD dwRtn = ::WaitForSingleObject(hTask, INFINITE);
#else
	s32 dwRtn = pthread_join(hTask, NULL);
#endif
}

/*
//shenbiao[2013-08-1]  ��������鲥��ַ��map
typedef std::map<u32, TSatDMulticastAddr> TMapSatDMulticastAddr;

struct TDomSatDMulticastAddrInfo
{
private:
	TMapSatDMulticastAddr m_mapSatdMulticastAddr;	
	s8  m_achDomainGUID[KDVKDM_MAXLEN_GUID+1];      // ��������

public:
	TDomSatDMulticastAddrInfo( )
	{
		memset( m_achDomainGUID, 0, sizeof( m_achDomainGUID ) );
	}

	~TDomSatDMulticastAddrInfo( )
	{
		memset( m_achDomainGUID, 0, sizeof( m_achDomainGUID ) );
	}

	TMapSatDMulticastAddr *GetMapAddr( void )
	{
		return &m_mapSatdMulticastAddr;
	}

	void SetDomainGUID( const s8 *pchDomainGUID )
    {
		memset( m_achDomainGUID, 0, sizeof(m_achDomainGUID) );
        if (NULL != pchDomainGUID)
        {
            strncpy( m_achDomainGUID, pchDomainGUID, sizeof(m_achDomainGUID)-1 );
            m_achDomainGUID[KDVKDM_MAXLEN_GUID] = '\0';
        }
        return;
    }
    const s8* GetDomainGUID( void ) const 
	{ 
		return m_achDomainGUID;
	}
	
	//ͨ������ɾ��map�е�һ���鲥��ַ	
	BOOL32 DelSatdMulticastAddr( u32 dwIndex )
	{
		if( m_mapSatdMulticastAddr.empty() )
		{
			return FALSE;
		}
		
		BOOL32 bResult = FALSE;
		
		TMapSatDMulticastAddr::iterator itFound;
		itFound = m_mapSatdMulticastAddr.find( dwIndex );

		if ( m_mapSatdMulticastAddr.end() == itFound )
		{
			bResult = FALSE;
		}
		else
		{
			m_mapSatdMulticastAddr.erase( dwIndex );

			bResult = TRUE;
		}		
		
		return bResult;
	}
			
	//��ȡһ�����е��鲥��ַ
	BOOL32 GetIdleSatdMulticastAddr( TSatDMulticastAddr &tNewSatDMulticastAddr, u32 dwMulticastIp )
	{
		BOOL32 bResult = FALSE;

		if( m_mapSatdMulticastAddr.empty() 
			&& tNewSatDMulticastAddr.SatDMulticastAddrInit( 1, MULTICAST_STARTPORT, dwMulticastIp )
			)
		{		
			m_mapSatdMulticastAddr.insert( std::pair<u32, TSatDMulticastAddr>( 1, tNewSatDMulticastAddr ) );
			
			return TRUE;
		}		
		
		TMapSatDMulticastAddr::iterator itFound;

		itFound = m_mapSatdMulticastAddr.end();
		itFound--;

		u32 dwIndexCopy = 1;
		u32 dwMapNum = m_mapSatdMulticastAddr.size();
		if ( dwMapNum < itFound->first )
		{
			itFound = m_mapSatdMulticastAddr.begin();
			
			while ( itFound != m_mapSatdMulticastAddr.end() )
			{
				if( dwIndexCopy < itFound->first 
					&& tNewSatDMulticastAddr.SatDMulticastAddrInit( dwIndexCopy, MULTICAST_STARTPORT, dwMulticastIp )
					)
				{
					m_mapSatdMulticastAddr.insert( std::pair<u32, TSatDMulticastAddr>( dwIndexCopy, tNewSatDMulticastAddr ) );
					bResult = TRUE;

					break;
				}
				dwIndexCopy = itFound->first;

				itFound++;
			}	
			
	//		bResult = FALSE;
		}
		else if ( dwMapNum == itFound->first 
				  && tNewSatDMulticastAddr.SatDMulticastAddrInit( dwMapNum + 1, MULTICAST_STARTPORT, dwMulticastIp )
				  )
		{
			m_mapSatdMulticastAddr.insert( std::pair<u32, TSatDMulticastAddr>( dwMapNum + 1, tNewSatDMulticastAddr ) );

			bResult = TRUE;
		}
		
		
		return bResult;
	}

	void AddMapSatdMulticastAddr( u32 dwIndex, TSatDMulticastAddr tSatDMulticastAddr )
	{
		m_mapSatdMulticastAddr.insert( std::pair<u32, TSatDMulticastAddr>( dwIndex, tSatDMulticastAddr ) );
	}
	
};

//shenbiao[2013-08-1] �������������µ��鲥��ַ��map  
typedef std::map<u32, TDomSatDMulticastAddrInfo> TDomMapSatDMulticastAddr;

*/

BOOL32 GetConfInfoFromJsonObj(const json_t_ptr, TSimpleConfInfo&);

BOOL32 GetConfInfoFromJson(const s8*, TConfDetailInfo&);
BOOL32 GetConfInfoFromJson(const json_t_ptr, TConfDetailInfo&);

BOOL32 GetConfListFromJson(const s8*, ConfList&);
BOOL32 GetConfListFromJsonObj(const json_t_ptr, ConfList&);

void ConvertStruct(const CRedisBookConf& cBookConf, TSimpleConfInfo& tSimpleConf);
void ConvertStruct(const CRedisBookConf& cBookConf, TConfDetailInfo& tDetailConf);

void ConvertStruct(const std::vector<CRedisBookConf>& vecBookConf,
	std::vector<TSimpleConfInfo>& vecSimpleConf);
void ConvertStruct(const std::vector<CTempCacheData>& vecBookConf,
	std::vector<TSimpleConfInfo>& vecSimpleConf);

BOOL32 GetRedisConfFromJsonObj(const json_t_ptr& pjRoot, TRedisConfInfo &tConfInfo);
BOOL32 GetRedisConfFromJson(const s8* pchJson, TRedisConfInfo& tConfInfo);
BOOL32 GetRedisConfListFromJsonObj(const json_t_ptr& pjRoot, std::list<TRedisConfInfo>& lstConfList);
BOOL32 GetRedisConfListFromJson(const s8* pchJson, std::list<TRedisConfInfo>& lstConfList);

BOOL32 GetRedisBookConfFromJsonObj(const json_t_ptr& pjRoot, CRedisBookConf &tConfInfo);
BOOL32 GetRedisBookConfListFromJsonObj(const json_t_ptr& pjRoot, vector<CRedisBookConf>& bookConfVec);
BOOL32 GetRedisBookConfListFromJson(const s8* pchJson, vector<CRedisBookConf>& bookConfVec);
BOOL32 GetRedisBookConfFromJson(const s8* pchJson, CRedisBookConf& bookConf);

BOOL32 GetDBConfInfoFromJson(const s8* pchJson, TDBConf& tDBConf);
BOOL32 GetDBConfInfoFromJson(const json_t_ptr& pjRoot, TDBConf& tDBConf);

BOOL32 GetConfMonitorsInfoFromJson(const s8* pchJson, std::vector<TMonitorAliveInfo>& monitorVec);
void GetConfMonitorsInfoFromJsonObj(const json_t_ptr& pjRoot, std::vector<TMonitorAliveInfo>& monitorVec);
void GetMonitorInfoFromJsonObj(const json_t_ptr& pjRoot, TMonitorAliveInfo& tMonitorInfo);
BOOL32 SetStopMonitorsInfoToJsonObj(json_t_ptr& pjRoot, const std::vector<TMonitorAliveInfo>& monitorVec);

BOOL32 GetEnableHDMeetingFromJson(const s8 * pchJsonBuf, BOOL32& bEnableHDMeeting);
BOOL32 GetEnableH265FromJson(const s8 * pchJsonBuf, BOOL32& bEnableH265);

BOOL32 IsMtInConf(const s8* pchMtE164, const s8* pchConfDetailJson);

BOOL32 GetReqidFromJson(const json_t_ptr& pjRoot, s8 * pchReqid, u32 dwReqidLen);
BOOL32 GetReqidFromJson(const s8 * pchJsonBuf, s8 * pchReqid, u32 dwReqidLen);

BOOL32 GetCreatorInfoFromJson(const json_t_ptr& pjRoot, 
	TRedisConfInfo& tConfInfo, const BOOL32 bRedisData = FALSE);

BOOL32 SetChargeInfoToJson(const TRedisConfInfo& tRedisConf, std::string& chargeJson);

s32 B64DecodeAndUnCompressXmlMsg(s8* pchData, u8* pchXmlMsg, u32& dwXmlMsgLen);

BOOL32 IsOldMcuByJson(const s8* pchJsonBuf);
BOOL32 IsOldMcu(const s8* achIp);
BOOL32 GetMaxJoinedMtFromJson(const s8* pchJsonBuf, u32& dwMaxJoinedMt);

BOOL32 GetConfNumLimitFromJson(const s8 * pchJsonBuf,
	CPlatformConfNumLimit& cConfNumLimit, const BOOL32 bBmcLicense = TRUE);
BOOL32 GetConfNumLimitFromJsonObj(const json_t_ptr& pjRoot, 
	CPlatformConfNumLimit& cConfNumLimit, const BOOL32 bBmcLicense = TRUE);

BOOL32 GetCreateConfParamFromJson(const s8* pchJsonBuf, TConfInfo& tConfInfo);
BOOL32 GetCreateConfParamFromJsonObj(const json_t_ptr& pjRoot, TConfInfo& tConfInfo);

BOOL32 CheckConfNumIsReachLimit(const u32 dwMaxJoinedMt,
	const u32 dwSmallConfNum, const u32 dwMaxSmallConfNum,
	const u32 dwLargeConfNum, const u32 dwMaxLargeConfNum,
	BOOL32& bReach);

//const u8 TranslatePollMode(const u8 byPollMode);
const BOOL32 IsMt(const u16 wMsgType);

BOOL32 CheckPassword(const s8* pchReqPassword, const s8* pchBookConfPassword);
BOOL32 CheckPublicTempPassword(const s8* pchReqPassword, const s8* pchTempPwd);
const u8 GetDBConfScale(const u32 dwMaxJoinedMt);
BOOL32 IsSmallScaleConf(const u32 dwMaxJoinedMt);
BOOL32 CheckQComsumerStatus(const s8* pchJsonBuf, const u32 dwJsonLen);
BOOL32 IsNewMcu(const s8* abyMcuType);
BOOL32 IsCreateConfTimeOut(const time_t createTime);
BOOL32 IsCreateConfTimeOut(const s8* pchCreateTime);
BOOL32 IsConfInCmuConfList(const s8* pchConfE164, 
	const TMpcConfData* paCmuConf,
	const u32 dwConfNUm,
	BOOL32& bExist);
BOOL32 IsRedisHasConfExData(const TRedisConfInfo& tRedisConf);
BOOL32 IsExceptionBookConf(const s8* pchBookConfStartTime);

BOOL32 ConstructGetRecoverConfNeedInfoToJson(const s8* pchConfE164, const BOOL32 bHasXmpu, 
	s8* pchJsonBuf, u32& dwJsonBufLen);
BOOL32 GetRecoverConfNeedInfoFromJson(const string& strJson, string& strConfE164, BOOL32& bHasXmpu);
BOOL32 GetConfInfoExFromConfInfo(const TConfInfo& tConfInfo, TConfInfoEx& tConfInfoEx);
BOOL32 TransMiniMixParamToMixModule(const TMiniMixParam& tMiniMixParam, TMixModule& tMixModule);
BOOL32 TransMiniVmpParamToVmpModule(const TMiniVmpParam& tMiniVmpParam, TVmpModuleInfo& tVmpModuleInfo);
BOOL32 GetMtListFromMiniConfInfo(const TMiniConfInfo& tMiniConfInfo, const TMtAlias& tCreatorMt, TMtAlias* atMtList, u32& nMtNum);

// ��ӡ����
API void ptempdbmsg( void );
API void nptempdbmsg( void );

API void pvmdbmsg( void );
API void npvmdbmsg( void );

API void pschedbmsg( void );
API void npschedbmsg( void );

API void papdbmsg( void );
API void npapdbmsg( void );

API void pmauvcmsg( void );
API void npmauvcmsg( void );

API void ppublicmsg( void );
API void nppublicmsg( void );


API void pmccmsg( void );
API void npmccmsg( void );

API void pagwmsg( void );
API void npagwmsg( void );

API void pcmumsg( void );
API void npcmumsg( void );

API void pcmcmumsg( void );
API void npcmcmumsg( void );

API void pcmmaumsg( void );
API void npcmmaumsg( void );

API void phdumsg( void );
API void nphdumsg( void );

API void pbmcmsg( void );
API void npbmcmsg( void );

API void pnumsg( void );
API void npnumsg( void );

API void pnppmsg(void);
API void npnppmsg(void);

API void presmgrmsg( void );
API void npresmgrmsg( void );

API void pupumsg( void );
API void npupumsg( void );

API void pconfdbmsg( void );
API void npconfdbmsg( void );

API void pconfbackmsg( void );
API void npconfbackmsg( void );

API void predismsg(void);
API void npredismsg(void);

class CPrintFunInOut
{
public:
	CPrintFunInOut(const s8* pchFunName, const u16 wModule = MID_MAU_PUBLIC,
		const u8 byLevel = LOG_LVL_KEYSTATUS)
		:m_funName(pchFunName), m_wModule(wModule), m_byLevel(byLevel)
	{
		std::string moduleSeperator(GetModuleSeperator());
		LogPrint(m_byLevel, m_wModule, "%sIN~%s[%s]%sIN~%s\n", moduleSeperator.c_str(),
			moduleSeperator.c_str(), m_funName.c_str(), moduleSeperator.c_str(), 
			moduleSeperator.c_str());
	}
	CPrintFunInOut(const s8* pchFunName, const s8* pchDescribe, ...)
		:m_funName(pchFunName), m_wModule(MID_MAU_PUBLIC), m_byLevel(LOG_LVL_KEYSTATUS)
	{
		va_list args;
		va_start(args, pchDescribe);
		s8 achDescribe[10240 + 1] = { 0 };
		vsnprintf(achDescribe, sizeof(achDescribe), pchDescribe, args);
		m_describe = achDescribe;
		LogFunIn();
		va_end(args);
	}
	CPrintFunInOut(const s8* pchFunName, const u16 wModule, const s8* pchDescribe, ...)
		:m_funName(pchFunName), m_wModule(wModule), m_byLevel(LOG_LVL_KEYSTATUS)
	{
		va_list args;
		va_start(args, pchDescribe);
		s8 achDescribe[10240 + 1] = { 0 };
		vsnprintf(achDescribe, sizeof(achDescribe), pchDescribe, args);
		m_describe = achDescribe;
		LogFunIn();
		va_end(args);
	}
	CPrintFunInOut(const s8* pchFunName, const u16 wModule,
		const u8 byLevel, const s8* pchDescribe, ...)
		:m_funName(pchFunName), m_wModule(wModule), m_byLevel(byLevel)
	{
		va_list args;
		va_start(args, pchDescribe);
		s8 achDescribe[10240 + 1] = { 0 };
		vsnprintf(achDescribe, sizeof(achDescribe), pchDescribe, args);
		m_describe = achDescribe;
		LogFunIn();
		va_end(args);
	}
	~CPrintFunInOut()
	{
		std::string moduleSeperator(GetModuleSeperator());
		LogPrint(m_byLevel, m_wModule, "%sOUT%s[%s]%sOUT%s\n\n", moduleSeperator.c_str(),
			moduleSeperator.c_str(), m_funName.c_str(), moduleSeperator.c_str(), 
			moduleSeperator.c_str());
	}
private:
	void LogFunIn()
	{
		std::string moduleSeperator(GetModuleSeperator());
		LogPrint(m_byLevel, m_wModule, "%sIN~%s[%s]%sIN~%s#Describe:[%s]\n",
			moduleSeperator.c_str(), moduleSeperator.c_str(),
			m_funName.c_str(), moduleSeperator.c_str(), moduleSeperator.c_str(), 
			m_describe.c_str());
	}
	std::string GetModuleSeperator()
	{
		switch (m_wModule)
		{
		case MID_MAU_PUBLIC:
			return string(6, '~');
		case MID_MAU_VC:
			return string(6, '-');
		case MID_MAU_NU:
			return string(6, '=');
		case MID_MAU_CMU:
			return string(6, '+');
		case MID_MAU_CMMAU:
			return string(6, '`');
		case MID_MAU_REDIS:
			return string(6, '<');
		default:
			return string(6, '.');
		}
	}
private:
	std::string m_describe;
	std::string m_funName;
	u16 m_wModule;
	u8 m_byLevel;
};

class CMqStatus
{
public:
	CMqStatus() :
		m_bMau_CmMauCreated(FALSE), m_bMau_ConfCreated(FALSE),
		m_bMau_CmMcuCreated(FALSE), m_bMau_NotifyCreated(FALSE),
		m_bMovisionLicenseQCreated(FALSE), m_bMovisionLicenseUpdateQCreated(FALSE),
		m_bPlatformLicenseQCreated(FALSE), m_bCmMauPCreated(FALSE), m_bCmCmuPCreated(FALSE),
		m_bCmuPCreated(FALSE), m_bMovisionLicensePCreated(FALSE),
		m_bPlatfromLicensePCreated(FALSE),
		m_bNpp_NgiNppCreated(FALSE), m_bNppNgiPCreated(FALSE){}
	void SetMau_CmMauIsCreated(const BOOL32 bCreated) { m_bMau_CmMauCreated = bCreated; };
	BOOL32 GetMau_CmMauIsCreated() const { return m_bMau_CmMauCreated; }
	void SetMau_ConfIsCreated(const BOOL32 bCreated) { m_bMau_ConfCreated = bCreated; }
	BOOL32 GetMau_ConfIsCreated() const { return m_bMau_ConfCreated; }
	void SetMau_CmMcuIsCreated(const BOOL32 bCreated) { m_bMau_CmMcuCreated = bCreated; }
	BOOL32 GetMau_CmMcuIsCreated() const { return m_bMau_CmMcuCreated; }
	void SetMau_NotifyIsCreated(const BOOL32 bCreated) { m_bMau_NotifyCreated = bCreated; }
	BOOL32 GetMau_NotifyIsCreated() const { return m_bMau_NotifyCreated; }
	void SetMovisionLicenseQIsCreated(const BOOL32 bCreated) { m_bMovisionLicenseQCreated = bCreated; }
	BOOL32 GetMovisionLicenseQIsCreated() const { return m_bMovisionLicenseQCreated; }
	void SetMovisionLicenseUpdateQIsCreated(const BOOL32 bCreated) { m_bMovisionLicenseUpdateQCreated = bCreated; }
	BOOL32 GetMovisionLicenseUpdateQIsCreated() const { return m_bMovisionLicenseUpdateQCreated; }
	void SetPlatformLicenseQIsCreated(const BOOL32 bCreated) { m_bPlatformLicenseQCreated = bCreated; }
	BOOL32 GetPlatformLicenseQIsCreated() const { return m_bPlatformLicenseQCreated; }
	void SetNpp_NgiNppQIsCreated(const BOOL32 bCreated) { m_bNpp_NgiNppCreated = bCreated; }
	BOOL32 GetNpp_NgiNppQIsCreated() const { return m_bNpp_NgiNppCreated; }

	void SetNppNgiPIsCreated(const BOOL32 bCreated) { m_bNppNgiPCreated = bCreated; }
	void SetCmMauPIsCreated(const BOOL32 bCreated) { m_bCmMauPCreated = bCreated; }
	void SetCmCmuPIsCreated(const BOOL32 bCreated) { m_bCmCmuPCreated = bCreated; }
	void SetCmuPIsCreated(const BOOL32 bCreated) { m_bCmuPCreated = bCreated; }
	void SetMovisionLicensePIsCreated(const BOOL32 bCreated) { m_bMovisionLicensePCreated = bCreated; }
	void SetPlatformLicensePIsCreated(const BOOL32 bCreated) { m_bPlatfromLicensePCreated = bCreated; }
	BOOL32 GetCmMauPIsCreated() const { return m_bCmMauPCreated; }
	BOOL32 GetCmCmuPIsCreated() const { return m_bCmCmuPCreated; }
	BOOL32 GetCmuPIsCreated() const { return m_bCmuPCreated; }
	BOOL32 GetMovisionLicensePIsCreated() const { return m_bMovisionLicensePCreated; }
	BOOL32 GetPlatformLicensePIsCreated() const { return m_bPlatfromLicensePCreated; }
	BOOL32 GetNppNgiPIsCreated() const { return m_bNppNgiPCreated; }
private:
	//������״̬
	BOOL32 m_bMau_CmMauCreated;
	BOOL32 m_bMau_ConfCreated;
	BOOL32 m_bMau_CmMcuCreated;
	BOOL32 m_bMau_NotifyCreated;
	BOOL32 m_bMovisionLicenseQCreated;
	BOOL32 m_bMovisionLicenseUpdateQCreated;
	BOOL32 m_bPlatformLicenseQCreated;
	BOOL32 m_bNpp_NgiNppCreated;
	//������״̬
	BOOL32 m_bCmMauPCreated;
	BOOL32 m_bCmCmuPCreated;
	BOOL32 m_bCmuPCreated;
	BOOL32 m_bMovisionLicensePCreated;
	BOOL32 m_bPlatfromLicensePCreated;
	BOOL32 m_bNppNgiPCreated;
};

//ҵ����Ϣ�࣬�����65535�ֽڳ�����Ϣ
class CMauMsg
{
protected:
	u16		m_wEventId;             //�¼���
	u16		m_wErrorCode;           //������
	u16		m_wMsgBodyLen;          //��Ϣ�峤��
	u8      m_abyMsgBody[MAU_MSG_LEN-MAU_MSGHEAD_LEN];    //��Ϣ��
public:
    void  SetEventId(u16  wEventId){ m_wEventId = htons(wEventId);} 
    u16   GetEventId( void ) const { return ntohs(m_wEventId); }
    void  SetErrorCode(u16  wErrorCode){ m_wErrorCode = htons(wErrorCode);} 
    u16   GetErrorCode( void ) const { return ntohs(m_wErrorCode); }

	void Init( void )
	{
		memset( this, 0, sizeof(CMauMsg));	//����
	}
	CMauMsg( void )//constructor
	{
		Init();
	}
	CMauMsg(const u8   * const pbyMsg, u16  wMsgLen)//constructor
	{
		Init();
		
		if( wMsgLen < MAU_MSGHEAD_LEN || pbyMsg == NULL )
			return;
		
		wMsgLen = getmin( wMsgLen, MAU_MSG_LEN );
		memcpy( this, pbyMsg, wMsgLen );
	}
	~CMauMsg(void) {}//distructor
	void SetMsgBodyLen( u16  wMsgBodyLen )
	{
		m_wMsgBodyLen = htons( wMsgBodyLen );
	}
	u16  GetMsgBodyLen( void ) const//��ȡ��Ϣ�峤����Ϣ
	{
		return( ntohs( m_wMsgBodyLen ) );
	}
	u16  GetMsgBody( u8   * pbyMsgBodyBuf, u16  wBufLen ) const//��ȡ��Ϣ�壬���û�����BUFFER�������С���ضϴ���
	{
		u16 wActLen = getmin( GetMsgBodyLen(), wBufLen );
		memcpy( pbyMsgBodyBuf, m_abyMsgBody, wActLen );
		return wActLen;
	}
	u8   * const GetMsgBody( void ) const//��ȡ��Ϣ��ָ�룬�û������ṩBUFFER
	{
		return( ( u8 * const )m_abyMsgBody );
	}
	void SetMsgBody( const void * const pbyMsgBody = NULL, u16  wLen = 0 )//������Ϣ��
	{
		/*
		if (pbyMsgBody == NULL || wLen == 0)
		{
			SetMsgBodyLen( 0 );
			return;
		}
		*/
		wLen = getmin( wLen, MAU_MSG_LEN - MAU_MSGHEAD_LEN );
		memcpy( m_abyMsgBody, pbyMsgBody, wLen );
		SetMsgBodyLen( wLen );
	}
	void CatMsgBody( const void * const pbyMsgBody, u16  wLen )//�����Ϣ��
	{
		/*
		if (pbyMsgBody == NULL || wLen == 0)
		{
			return;
		}
		*/
		u16 wMaxLen = MAU_MSG_LEN - MAU_MSGHEAD_LEN - GetMsgBodyLen();
		if( wLen > wMaxLen )
		{
			OspPrintf( TRUE, FALSE, "CMauMsg: CatMsgBody() Exception -- Msg too long!\n" );
			wLen = wMaxLen;
		}
		memcpy( m_abyMsgBody + GetMsgBodyLen(), pbyMsgBody, wLen );
		SetMsgBodyLen( GetMsgBodyLen() + wLen );
	}
	u16  GetServMsgLen( void ) const//��ȡ������Ϣ����
	{
		return( GetMsgBodyLen() + MAU_MSGHEAD_LEN );
	}
	u16  GetServMsg( u8   * pbyMsgBuf, u16  wBufLen ) const//��ȡ������Ϣ�����û�����BUFFER�������С���ضϴ���
	{
		wBufLen = getmin(MAU_MSG_LEN,wBufLen);
		memcpy( pbyMsgBuf, this, wBufLen );
		return( getmin( GetMsgBodyLen() + MAU_MSGHEAD_LEN, wBufLen ) );
	}
	u8   * const GetServMsg( void ) const//��ȡ������Ϣָ�룬�û������ṩBUFFER
	{
		return( ( u8 * const )( this ) );
	}
	void SetServMsg(const u8   * const pbyMsg, u16  wMsgLen )//����������Ϣ
	{
		if( wMsgLen < MAU_MSGHEAD_LEN )
		{
			OspPrintf( TRUE, FALSE, "CMauMsg: SetServMsg() Exception -- invalid MsgLen!\n" );
			return;
		}

		wMsgLen = getmin( wMsgLen, MAU_MSG_LEN );
		memcpy( this, pbyMsg, wMsgLen );
		SetMsgBodyLen( wMsgLen - MAU_MSGHEAD_LEN );
	}
	const CMauMsg & operator= ( const CMauMsg & cServMsg )//����������
	{
		Init();
		u16	wLen = cServMsg.GetServMsgLen();

		memcpy( this, cServMsg.GetServMsg(), wLen );
		return( *this );
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

#ifdef WIN32
#pragma pack( pop )
#endif

#endif  // _MAUUTILITY_H_
