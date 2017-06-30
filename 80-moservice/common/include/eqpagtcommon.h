/*****************************************************************************
    ģ����      : EqpAgt
    �ļ���      : eqpagtcommon.h
    ����ļ�    : 
    �ļ�ʵ�ֹ���: EqpAgt����
    ����        : liaokang
    �汾        : V4r7  Copyright(C) 2011 KDC, All rights reserved.
-----------------------------------------------------------------------------
    �޸ļ�¼:
    ��  ��      �汾        �޸���          �޸�����
    2012/06/18  4.7         liaokang        ����
******************************************************************************/
#ifndef _EQPAGT_COMMON_H_
#define _EQPAGT_COMMON_H_

#include "osp.h"
#include "kdvtype.h"

// ������
// 0���ɹ�
#define EQPAGT_SUCCESS                  (u16)0    // �ɹ�
// 1����ʼ��������
#define EQPAGT_INITERR_CREATSEM         (u16)1    // �����ź�������
#define EQPAGT_INITERR_CFG              (u16)2    // ���ô���
#define EQPAGT_INITERR_SNMPINIT         (u16)3    // SNMP��ʼ������
#define EQPAGT_INITERR_CREATAPP         (u16)4    // �����̴߳���
#define EQPAGT_INITERR_POSTMSG          (u16)5    // ������Ϣ����

// Ĭ���ļ�·��
#ifdef WIN32
#define DIR_CONFIG                      ( LPCSTR )"./conf"	
#endif

#ifdef _LINUX_
#define DIR_CONFIG                      ( LPCSTR )"/usr/etc/config/conf"
#endif

// ��������
#define EQPAGTCFGFILENAME               ( LPCSTR )"eqpagtcfg.ini"   // eqpagt�����ļ� 
#define MAXLEN_EQPAGTCFG_INI            (u32)5120                   // eqpagtcfg.ini �����ļ�����󳤶�

// ϵͳ״̬��NMS��u32������
#define EQP_SYSSTATE_UNKNOWN            (u32)0   /* δ֪״̬ */
#define EQP_SYSSTATE_RUNNING            (u32)1   /* ����״̬ */
#define EQP_SYSSTATE_REBOOT             (u32)2   /* ����״̬ */
#define EQP_SYSSTATE_STANDY             (u32)3   /* ����״̬ */

// �豸����ϵͳ��NMS��u32������
#define EQP_OSTYPE_UNKNOWN              (u32)0
#define EQP_OSTYPE_VXWORKS              (u32)1
#define EQP_OSTYPE_LINUX                (u32)2
#define EQP_OSTYPE_WIN32                (u32)3
#define EQP_OSTYPE_VX_RAWDISK           (u32)4    //VX�����
#define EQP_OSTYPE_LINUX_RH		        (u32)5

// �豸�����ͣ�NMS��u32������
#define EQP_SUBTYPE_UNKNOWN		        (u32)0	//δ֪ 
#define EQP_SUBTYPE_KDV2K		        (u32)1	//KDV2000
#define EQP_SUBTYPE_KDV2KC		        (u32)2	//��Ȼ������
#define EQP_SUBTYPE_KDV2KE		        (u32)3	//KDV2000E
#define EQPSUBTYPE_KDV2K		        ( LPCSTR )"KDV2K"
#define EQPSUBTYPE_KDV2KC		        ( LPCSTR )"KDV2KC"
#define EQPSUBTYPE_KDV2KE		        ( LPCSTR )"KDV2KE"

#define	MAXLEN_COMMUNITY	            (u8)32	                    // community ��󳤶�(��Чλ32)
#define	MAXNUM_EQPAGTINFO_LIST	        (u8)16	                    // EqpAgt��Ϣ����󳤶�

#define DEF_READCOM                     ( LPCSTR )"public"          // ����ͬ�� 
#define DEF_WRITECOM	                ( LPCSTR )"kdv123"          // д��ͬ��
#define DEF_GSPORT                      (u16)161                    // ��ȡ�˿�(������)
#define	DEF_TRAPPORT		            (u16)162	                // ��Trap�˿�(������)

// EqpAgt��Ϣ
struct TEqpAgtInfo
{
protected:
    u32  m_dwTrapIp;								// ����Trap�����ܷ�����Ip(������)
    s8   m_achReadCom[MAXLEN_COMMUNITY+1];		    // ����ͬ��
    s8   m_achWriteCom[MAXLEN_COMMUNITY+1];         // д��ͬ��
    u16  m_wGetSetPort;								// ��ȡ�˿�(������)
    u16  m_wSendTrapPort;							// ��Trap�˿�(������)
public:
    TEqpAgtInfo()
    {
        memset(this, 0, sizeof(TEqpAgtInfo)); 
        SetReadCom( DEF_READCOM );
        SetWriteCom( DEF_WRITECOM );
        SetGSPort( DEF_GSPORT );
        SetTrapPort( DEF_TRAPPORT );
    }
    void SetTrapIp(u32 dwTrapIp) { m_dwTrapIp = htonl(dwTrapIp);	}
    u32  GetTrapIp(void) const { return ntohl(m_dwTrapIp);	}
    void SetGSPort(u16 wPort) { m_wGetSetPort = wPort;	}
    u16  GetGSPort(void) const { return m_wGetSetPort;	}
    void SetTrapPort(u16 wPort) { m_wSendTrapPort = wPort;	}
    u16  GetTrapPort(void) const { return m_wSendTrapPort;	}
    
    void SetReadCom( LPCSTR lpszReadCom )
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

#endif  // _EQPAGT_COMMON_H_