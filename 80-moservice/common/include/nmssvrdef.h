  /*****************************************************************************
   ģ����      : KDVNMS
   �ļ���      : NmsSvrDef.h
   ����ļ�    : NmsSvrDef.cpp
   �ļ�ʵ�ֹ���: �������ݽṹ�Ķ���
   ����		   : Ѧ����
   �汾 	   : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾 	�޸���  	�޸�����
   2002/03/05  0.9  	Ѧ����  	����
******************************************************************************/
#ifndef _NMS_SVR_COMMON_H
#define _NMS_SVR_COMMON_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "nmsmacro.h"
#include <list>
#include <afxmt.h>

using namespace std;

#define OPER_UNIMPLEMENTED				FALSE

/***************************************************************************/

//����ģ��Ķ�ʱ��ϵ��
#define SESS_MODULAR		1
#define CM_MODULAR			0.85
#define FM_MODULAR			0.85

//ǰ��̨�����Ƿ�һ��
#define	CONFIG_SAME			0
#define	CONFIG_DIFFERENT	1

//ǰ��̨�澯�Ƿ�һ��
#define	ALARM_SAME			0
#define	ALARM_DIFFERENT		1

//�������ڲ���Ϣ
#define EV_SVR_START					EV_SVR_BGN + 0	//ϵͳ������Ϣ
#define	EV_SVR_CM_NORMAL				EV_SVR_BGN + 1	//CMģ��������Ϣ
#define	EV_SVR_FM_NORMAL				EV_SVR_BGN + 2	//FMģ��������Ϣ
#define EV_SVR_GETVERSION_SUC			EV_SVR_BGN + 3	//��ȡ�汾��Ϣ�ɹ���Ϣ
#define EV_SVR_GETVERSION_FAIL			EV_SVR_BGN + 4	//��ȡ�汾��Ϣʧ����Ϣ
#define EV_SVR_CFG_CHANGE				EV_SVR_BGN + 7	//���øı���Ϣ
#define EV_SVR_LINK_CHANGE				EV_SVR_BGN + 8	//��·�ı���Ϣ
#define EV_AGT_RESTART					EV_SVR_BGN + 9	//Agt����������Ϣ
#define EV_SVR_NEED_FMSYNC				EV_SVR_BGN + 10	//�豸��Ҫͬ���澯��Ϣ
#define EV_SVR_GETALARMSTAMP_SUC		EV_SVR_BGN + 11	//��ȡ�澯��Ϣ�ɹ���Ϣ
#define EV_SVR_GETALARMSTAMP_FAIL		EV_SVR_BGN + 12	//��ȡ�澯��Ϣʧ����Ϣ
#define EV_SVR_GETALARM_SUC				EV_SVR_BGN + 13	//��ȡ�澯�ɹ���Ϣ
#define EV_SVR_GETALARM_FAIL			EV_SVR_BGN + 14	//��ȡ�澯ʧ����Ϣ
#define EV_SVR_ALARM					EV_SVR_BGN + 15	//�澯�ϱ���Ϣ
#define EV_SVR_SHUTDOWN					EV_SVR_BGN + 16	//�澯�ϱ���Ϣ
#define EV_SVR_FTP_PROC					EV_SVR_BGN + 21 //Ftp��չ
#define EV_SVR_FTP_THREAD_EXIT			EV_SVR_BGN + 22 //Ftp�߳��˳�
#define EV_SVR_FTP_PUT_SUC				EV_SVR_BGN + 23 //FTP Put �ɹ�
#define EV_SVR_FIND_DEVICE				EV_SVR_BGN + 24 //�������豸
#define EV_SVR_GETDEVICEINFO_SUC		EV_SVR_BGN + 25 //��ȡ�豸��Ϣ�ɹ�
#define EV_SVR_GETDEVICEINFO_FAIL		EV_SVR_BGN + 26 //��ȡ�豸��Ϣʧ��
#define EV_SVR_ADD_BOARD				EV_SVR_BGN + 27 //��Ӱ忨
#define EV_SVR_BOARD_STATUS				EV_SVR_BGN + 29 //�忨״̬�ı�
#define EV_SVR_QUERY_ALL_BOARD_SUC		EV_SVR_BGN + 31 //��ѯ�忨�ɹ�
#define EV_SVR_QUERY_ALL_BOARD_FAILED	EV_SVR_BGN + 32 //��ѯ�忨ʧ��
#define EV_SVR_MCU_CONFSTATE            EV_SVR_BGN + 33 //Mcu����״̬
#define EV_SVR_MT_CONFSTATE             EV_SVR_BGN + 34 //Mt����״̬
#define EV_SVR_FILE_UPDATE              EV_SVR_BGN + 35 //�ļ�����

//ϵͳ�еĶ�ʱ������
//For CM
#define CM_TIMER_FOR_TIMER				EV_SVR_BGN + 80 //��ʼ����������ѯ��ʱ
#define CM_TIMER_FOR_POLL				EV_SVR_BGN + 81 //��ʱ������ѯ
#define CM_TIMER_FOR_VER				EV_SVR_BGN + 82 //������ѯ��ʱ
//#define CM_TIMER_FOR_QUERY_BOARD		EV_SVR_BGN + 83 //��ʼ��ѯ�忨��ʱ
#define CM_TIMER_FOR_OPER				EV_SVR_BGN + 84 //������ʱ
#define CM_TIMER_FOR_DEL_DEVICE			EV_SVR_BGN + 85
#define CM_TIMER_FOR_NEXTPOLL           EV_SVR_BGN + 87

//ϵͳ��ʱ����ʱ����������Ϣ����
//For CM
#define EV_CM_TIMER_FOR_TIMER			EV_SVR_BGN + 80 //��ʼ����������ѯ��ʱ�¼�
#define EV_CM_TIMER_FOR_POLL			EV_SVR_BGN + 81 //��ʱ������ѯ�¼�
#define EV_CM_TIMER_FOR_VER				EV_SVR_BGN + 82 //������ѯ��ʱ�¼�
//#define EV_CM_TIMER_FOR_QUERY_BOARD		EV_SVR_BGN + 83 //��ʼ��ѯ�忨��ʱ�¼�
#define EV_CM_TIMER_FOR_OPER			EV_SVR_BGN + 84 //������ʱ
#define EV_CM_TIMER_FOR_DEL_DEVICE		EV_SVR_BGN + 85 //������ʱ
#define EV_CM_TIMER_QUERY_UPDATE        EV_SVR_BGN + 86 //��ѯ�ļ�����״̬
#define EV_CM_TIMER_FOR_NEXTPOLL        EV_SVR_BGN + 87



//ϵͳ�еĶ�ʱ������
//For FM
#define FM_TIMER_FOR_POLL				EV_SVR_BGN + 100 //�澯��ѯ��ʱ
#define FM_TIMER_FOR_ALARMSTAMP			EV_SVR_BGN + 101 //��ȡ�澯ʱ�����ʱ
#define FM_TIMER_FOR_ALARM				EV_SVR_BGN + 102 //��ȡ�澯��ʱ
#define FM_TIMER_FOR_TIMER				EV_SVR_BGN + 103 //�����趨��ʱ�澯��ѯ��ʱ
#define FM_TIMER_FOR_DEVICEINFO			EV_SVR_BGN + 104 //��ȡ�豸��Ϣ��ʱ
#define FM_TIMER_FOR_NEXTPOLL           EV_SVR_BGN + 105 //��ȡ��һ����ѯ��Ϣ��ʱ

//ϵͳ��ʱ����ʱ����������Ϣ����
//For FM
#define EV_FM_TIMER_FOR_POLL			EV_SVR_BGN + 100 //�澯��ѯ��ʱ�¼�
#define EV_FM_TIMER_FOR_ALARMSTAMP		EV_SVR_BGN + 101 //��ȡ�澯ʱ�����ʱ�¼�
#define EV_FM_TIMER_FOR_ALARM			EV_SVR_BGN + 102 //��ȡ�澯�¼���ʱ
#define EV_FM_TIMER_FOR_TIMER			EV_SVR_BGN + 103 //�����趨��ʱ�澯��ѯ�¼�
#define EV_FM_TIMER_FOR_DEVICEINFO		EV_SVR_BGN + 104 //��ȡ�豸��Ϣ��ʱ�¼�
#define EV_FM_TIMER_FOR_NEXTPOLL        EV_SVR_BGN + 105 //��һ����ѯ��ʼ �¼�

//����ID
#define OPER_GETVERSION_ID			1		//��ȡ�豸�����ļ��汾
#define OPER_GETBOARD_ID			2		//��ѯ�忨
#define OPER_SYNCFILE_ID			3		//��ǰ̨���͵�Set�����ļ���SNMP����
#define OPER_SYNCTIME_ID			4		//��ǰ̨���͵�Setʱ��ͬ��SNMP����
#define OPER_DEVICERESTART_ID		5		//��ǰ̨��������������
#define OPER_SETPERISTATUS_ID		6		//�����豸״̬
#define	OPER_SETPERISYNCTIME_ID		7		//ͬ���豸ʱ��
#define OPER_SETPERIBITERRTEST_ID	8		//�����豸�������
#define OPER_SETPERISELFTEST_ID		9		//�����豸�Լ�
#define OPER_PERIE1LINKCFG_ID		10		//ͬ���豸E1��·״̬
#define OPER_GETALARMSTAMP_ID		11		//Snmp Get�澯ʱ���
#define OPER_GETALARM_ID			12		//Snmp Get�澯
#define OPER_GETDEVICEINFO_ID		13		//��ȡ�豸��Ϣ
#define OPER_GETSINGLEBOARD_ID		14		//��ѯ����忨


//ϵͳ�еĶ�ʱ������
//For Session mamage
#define SM_TIMER_FOR_OPER				EV_SVR_BGN + 120

//ϵͳ��ʱ����ʱ����������Ϣ����
//For Session mamage
#define EV_SM_TIMER_FOR_OPER			EV_SVR_BGN + 120

const BYTE SVR_STATUS_IDL = 0x0;
const BYTE SVR_STATUS_CCMAPP_NORMAL = 0x1;
const BYTE SVR_STATUS_CFMAPP_NORMAL = 0x2;
const BYTE SVR_STATUS_CSMAPP_NORMAL = 0x4;
const BYTE SVR_STATUS_SERVICE = 0x07;

//�������Ķ˿ں�
#define	SERVER_PORT			2000

//��������
#define ALIAS_MAX_LEN		(s32)32

//�����������
#define MAX_PROC_NAME_LEN		(u8)32
//ʵ���������
#define MAX_INS_NAME_LEN		MAX_PROC_NAME_LEN

//�汾��Ϣ�ַ����ĳ���
#define	VERINFO_LEN			(s32)20

//�ļ����ĳ���
#define FILE_LEN			(s32)100
//Database Name
#define DBNAME_LEN			(s32)17
//�ļ�·������
#define PATH_LEN			(s32)256
//Entry���ĳ���
#define	ENTRY_LEN			(s32)20
//Community����
#define COMMUNITY_LEN		(s32)50
//OID Name����
#define OID_NAME_LEN		(s32)100

//SnmpĬ�϶˿�
#define SNMP_DEFAULT_PORT	(u16)161
//���ո澯�˿�
#define TRAP_DEFAULT_PORT   (u16)162

//���ݿ��к�Mcu/Mt��صı����Ŀ
#define MCU_TABLE_NUM		9
#define MT_TABLE_NUM		5

//ϵͳʵ������
#define MAX_INSTACE_NUM			16448
#define NORMAL_INSTACE_NUM		10
#define MAX_SESSION_NUM			8

//����InstanceID�ж�ȫ�������е�Index
#define GET_INDEX(x) x != CInstance::DAEMON ? x - 1 : NORMAL_INSTACE_NUM - 1

//һ��ͬ�����澯��Ŀ
#define	MAX_ALARM_NUM		200

inline void STRNCPY(TCHAR * const pszDst, LPCTSTR pszSrc, size_t nCount)
{	
    _tcsncpy( pszDst, pszSrc, nCount - 1 );
    pszDst[nCount - 1] = 0;
}

//����������������ļ�
#define CFG_SECTION_SYSTEM      (LPCSTR)"System"
#define CFG_ITEM_SVR_IP         (LPCSTR)"Server"
#define CFG_ITEM_TRAPPORT       (LPCSTR)"TrapPort"
#define CFG_ITEM_OSP_PORT       (LPCSTR)"OspConnPort"
#define CFG_ITEM_TELNET_PORT    (LPCSTR)"TelnetPort"
#define CFG_ITEM_CM_PERIOD      (LPCSTR)"CmSyncPeriod"
#define CFG_ITEM_FM_PERIOD      (LPCSTR)"FmSyncPeriod"
#define CFG_ITEM_SNMP_TIMEOUT   (LPCSTR)"SnmpTimeout"
#define CFG_ITEM_READ_COMMUNITY (LPCSTR)"ReadCommunity"
#define CFG_ITEM_WRITE_COMMUNITY    (LPCSTR)"WriteCommunity"

#define CFG_SECTION_DB          (LPCSTR)"Database"
#define CFG_ITEM_DB_SERVER      (LPCSTR)"DBServer"
#define CFG_ITEM_DB_NAME        (LPCSTR)"DBName"
#define CFG_ITEM_DB_USER        (LPCSTR)"DBUser"
#define CFG_ITEM_DB_PWD         (LPCSTR)"DBPwd"

#define CFG_SECTION_FTP             (LPCSTR)"Ftp"
#define CFG_ITEM_REMOTE_FTP_USER    (LPCSTR)"FtpRemoteUser"
#define CFG_ITEM_REMOTE_FTP_PWD     (LPCSTR)"FtpRemotePwd"
#define CFG_ITEM_LOCAL_FTP_USER     (LPCSTR)"FtpLocalUser"
#define CFG_ITEM_LOCAL_FTP_PWD      (LPCSTR)"FtpLocalPwd"
#define CFG_ITEM_FTP_LOCAL_PATH     (LPCSTR)"FtpLocalPath"

#define CFG_SECTION_LOG             (LPCSTR)"Log"
#define CFG_ITEM_LOG_PRINTSCREEN    (LPCSTR)"PrintScreenLevel"
#define CFG_ITEM_LOG_PRINTFILE      (LPCSTR)"PrintFileLevel"
#define CFG_ITEM_LOG_FILE           (LPCSTR)"LogFile"

#define CFG_SECTION_LANGUAGE        (LPCSTR)"Language"
#define CFG_ITEM_LANGUAGE_TYPE      (LPCSTR)"Type"


typedef std::list<u32> TAlarmFilterList;

//�ٽ�������
class CSection
{
public:
	CSection(CRITICAL_SECTION *pSection, CEvent *pcEvent = NULL,
             BOOL32 bIsSection = TRUE)
	{
        if ( bIsSection )
        {
		    m_pSection = pSection;
		    m_pcEvent = NULL;
		    EnterCriticalSection( m_pSection );
        }
        else
        {
            m_pSection = NULL;
		    m_pcEvent = pcEvent;
		    m_pcEvent->ResetEvent();
        }
	}

	~CSection()
	{
		if ( m_pSection != NULL )
			LeaveCriticalSection( m_pSection );

		if ( m_pcEvent != NULL )
			m_pcEvent->SetEvent();
	}

private:
    CSection() {}   //  wanghao 2005/11/07 ���������Ĭ�Ϲ��캯��

private:
	CRITICAL_SECTION    *m_pSection;
	CEvent              *m_pcEvent;
};

#endif
