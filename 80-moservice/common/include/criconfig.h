/*****************************************************************************
   ģ����      : Board Agent
   �ļ���      : criagent.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ������ú�������
   ����        : jianghy
   �汾        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2003/08/25  1.0         jianghy       ����
   2004/11/11  3.5         �� ��         �½ӿڵ��޸�
******************************************************************************/
#ifndef CRICONFIG_H
#define CRICONFIG_H

#include "mcuconst.h"
#include "vccommon.h"
#include "mcuagtstruct.h"
#include "eqpcfg.h"
#include "agtcomm.h"

///*------------------------------------------------------------- 
//*���ش������� 
//*-------------------------------------------------------------*/
//#define SECTION_mcueqpPrsConfig			(const s8*)"mcueqpPrsConfig"
//#define KEY_mcueqpPrsId					(const s8*)"mcueqpPrsId"
//#define KEY_mcueqpPrsMCUStartPort		(const s8*)"mcueqpPrsMCUStartPort"
//#define KEY_mcueqpPrsSwitchBrdId		(const s8*)"mcueqpPrsSwitchBrdId"
//#define KEY_mcueqpPrsAlias				(const s8*)"mcueqpPrsAlias"
//#define KEY_mcueqpPrsRunningBrdId		(const s8*)"mcueqpPrsIpAddr"
//#define KEY_mcueqpPrsStartPort			(const s8*)"mcueqpPrsStartPort"
//#define KEY_mcueqpPrsFirstTimeSpan		(const s8*)"mcueqpPrsFirstTimeSpan"
//#define KEY_mcueqpPrsSecondTimeSpan		(const s8*)"mcueqpPrsSecondTimeSpan"
//#define KEY_mcueqpPrsThirdTimeSpan      (const s8*)"mcueqpPrsThirdTimeSpan"
//#define KEY_mcueqpPrsRejectTimeSpan		(const s8*)"mcueqpPrsRejectTimeSpan"


class CCriConfig
{
public:
	CCriConfig();
	~CCriConfig();
    friend class CBoardAgent;
	/*====================================================================
	���ܣ���ȡ����Ĳ�λ��
	��������
	����ֵ��IDֵ��0��ʾʧ��
	====================================================================*/
	u8	GetBoardId();

	/*====================================================================
	���ܣ���ȡҪ���ӵ�MCU��IP��ַ(������)
	��������
	����ֵ��MCU��IP��ַ(������)��0��ʾʧ��
	====================================================================*/
	u32 GetConnectMcuIpAddr();

	/*====================================================================
	���ܣ���ȡ���ӵ�MCU�Ķ˿ں�
	��������
	����ֵ��MCU�Ķ˿ںţ�0��ʾʧ��
	====================================================================*/
	u16  GetConnectMcuPort();
	
	/*====================================================================
	���ܣ��õ�����IP��ַ
	��������
	����ֵ������IP��ַ(������)
	====================================================================*/
	u32 GetBrdIpAddr( );

	/*====================================================================
	���ܣ��Ƿ�����Prs
	��������
	����ֵ�����з���TRUE����֮FALSE
	====================================================================*/
	BOOL IsRunPrs();

	/*====================================================================
	���ܣ���ȡprs������Ϣ
	��������
	����ֵ��MCU�Ķ˿ںţ�0��ʾʧ��
	====================================================================*/
	BOOL GetPrsCfg( TPrsCfg* ptCfg );

	/*====================================================================
	���ܣ����õ���IP��ַ
	��������
	����ֵ��TRUE/FALSE
	====================================================================*/
	BOOL SetBrdIpAddr( u32 dwIp );

	/*====================================================================
	���ܣ�����PRS��������Ϣ
	��������
	����ֵ��TRUE/FALSE
	====================================================================*/
	BOOL SetPrsConfig( TEqpPrsEntry *ptPrsCfg );
	

	/*====================================================================
	���ܣ����ļ���������Ϣ
	��������
	����ֵ��TRUE/FALSE
	====================================================================*/
	BOOL ReadConnectMcuInfo();
    
    u32  GetMpcBIp(void);
    u16  GetMpcBPort(void);
public:
    u32	 m_dwDstMcuNode;
    u32  m_dwDstMcuNodeB;
    
    u16  m_wDiscHeartBeatTime;
    u8   m_byDiscHeartBeatNum;

private:
	BOOL    bIsRunPrs;      //�Ƿ�����PRS
	TPrsCfg m_prsCfg;
	u32	    m_dwMpcIpAddr;
	u16 	m_wMpcPort;
    u32     m_dwMpcIpAddrB;
    u16     m_wMpcPortB;

	u8	m_byBrdId;
	u32 m_dwBrdIpAddr;
    u8 m_byChoice;
};

#endif /* CRICONFIG_H */
