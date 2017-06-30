
#ifndef _NET_CONFIG_H_
#define _NET_CONFIG_H_

#include "osp.h"
#include "kdvtype.h"
#include "netconst.h"
#include "netstruct.h"
#include "netutils.h"
 


#ifdef NETCFG_DLL_EXPORT
#define NETCFG_API   __declspec( dllexport )
#else
#define NETCFG_API
#endif	//WIN32


/*=============================================================================
�� �� ���� SetNetParamAll
��    �ܣ� ����ϵͳ������������д���ļ�
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ����	[in]	TMINIMCUNetParamAll_Mcu * ptNetParamAll_Mcu		ϣ�����õ�������Ϣ
			[in]	TNetAdaptInfoAll * ptNetAdaptInfoAll			ϵͳ������Ϣ
			[in]	u32 & dwDefaultRouteIpAddr	Ĭ��·�ɣ����޸�ֵ��ͬ��ᱻ�ĵ�����5�����������Ƿ�Դ˽����޸�
			[out]	const s8* lpszProfileName	�ǿ����޸����ã�������޸�ϵͳ�������
			[in]	BOOL32 IsChangeRouteIp		�Ƿ��޸�·��ѡ��	TRUE  -- �޸ľ�̬·��	Ĭ��ֵΪTRUE
																	FALSE -- ���޸�
�� �� ֵ��  void 
=============================================================================*/
NETCFG_API void SetNetParamAll(TMINIMCUNetParamAll_Mcu * ptNetParamAll, TNetAdaptInfoAll * ptNetAdaptInfoAll, const s8* lpszProfileName = NULL);

/*=============================================================================
�� �� ���� SetNetParamAllFromFile
��    �ܣ� ���ļ��л�ȡ������������Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ����	[out]	TMINIMCUNetParamAll_Mcu * ptNetParamAll_Mcu
			[out]	TNetAdaptInfoAll * ptNetAdaptInfoAll
			[in]	const s8* lpszProfileName
			[out]	u32 & dwDefaultRouteIpAddr
�� �� ֵ��  BOOL32 
=============================================================================*/
NETCFG_API BOOL32 SetNetParamAllFromFile( TMINIMCUNetParamAll_Mcu * ptNetParamAll , TNetAdaptInfoAll * ptNetAdaptInfoAll , const s8* lpszProfileName ,u32 & dwDefaultRouteIpAddr);

/*=============================================================================
�� �� ���� TableMemoryFree
��    �ܣ� �ڴ���ͷ�
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ����  [in]	void **ppMem
			[in]	u32 dwEntryNum
�� �� ֵ��  BOOL32 
=============================================================================*/
NETCFG_API BOOL32 TableMemoryFree( void **ppMem , u32 dwEntryNum );

/*=============================================================================
�� �� ���� GetNetParamAll
��    �ܣ� ���ļ��л�ȡ������Ϣ
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ����	[out]	TMINIMCUNetParamAll_Mcu * ptNetParamAll_Mcu
			[int]	const s8* lpszProfileName
�� �� ֵ��	BOOL32 
=============================================================================*/
NETCFG_API BOOL32 GetNetParamAll( TMINIMCUNetParamAll_Mcu * ptNetParamAll , const s8* lpszProfileName );

/*=============================================================================
�� �� ���� GetSystemDefaultGW
��    �ܣ� ��ϵͳ�л�ȡĬ������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ����	[out]	u32 &dwDefaultGW
�� �� ֵ��	BOOL32 
=============================================================================*/
NETCFG_API BOOL32 GetSystemDefaultGW(u32 &dwDefaultGW);

/*=============================================================================
�� �� ���� IsValidNetParamAll
��    �ܣ� �ж����������Ƿ���Ч
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� [in]	const TMINIMCUNetParamAll &tMINIMCUNetParamAll
�� �� ֵ�� BOOL32 
=============================================================================*/
NETCFG_API BOOL32 IsValidNetParamAll( TMINIMCUNetParamAll * ptMINIMCUNetParamAll);


/*=============================================================================
�� �� ���� WriteNetConfigToFile
��    �ܣ� ����������д�������ļ�
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ����	[in]	TMINIMCUNetParamAll_Mcu * ptNetParamAll	������Ϣ(�������������)
			[out]	const s8* lpszProfileName				д��������ļ���(��·��)
�� �� ֵ��	void 
=============================================================================*/
NETCFG_API void WriteNetConfigToFile( TMINIMCUNetParamAll_Mcu * ptNetParamAll , const s8* lpszProfileName );

/*=============================================================================
�� �� ���� NetParamIn2Out
��    �ܣ� ϵͳʶ�����������ת��ΪӲ����������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ����  [in]	TMINIMCUNetParamAll_Mcu & ptNetParamAll_Mcu
			[out]	TMINIMCUNetParamAll & ptNetParamAll
�� �� ֵ��  void 
=============================================================================*/
NETCFG_API void NetParamIn2Out( TMINIMCUNetParamAll_Mcu * ptNetParamAll_Mcu , TMINIMCUNetParamAll * ptNetParamAll );

/*=============================================================================
�� �� ���� NetParamOut2In
��    �ܣ� Ӳ����������ת��Ϊϵͳʶ�����������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ����	[in]	const TMINIMCUNetParamAll &tMINIMCUNetParamAll
			[in]	TNetAdaptInfoAll &ptNetAdaptInfoAll
			[out]   TMINIMCUNetParamAll_Mcu &tMINIMCUNetParamAll_Mcu
�� �� ֵ��  void 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2009/06/04  1.0			���Ĳ�                  ����
=============================================================================*/
NETCFG_API void NetParamOut2In( TMINIMCUNetParamAll * ptNetParamAll, TNetAdaptInfoAll * ptNetAdaptInfoAll, TMINIMCUNetParamAll_Mcu * ptNetParamAll_Mcu );


#ifdef _HARDWARE_
#ifdef _LINUX_
/*=============================================================================
�� �� ���� EthIdxSys2Hard
��    �ܣ� ϵͳʶ����������ת����Ӳ���������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� [in]	u32 dwSysIdx 
�� �� ֵ�� u32 
=============================================================================*/
NETCFG_API u32 EthIdxSys2Hard( u32 dwSysIdx );


/*=============================================================================
�� �� ���� EthIdxHard2Sys
��    �ܣ� Ӳ���������ת����ϵͳʶ����������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� [in]	u32 dwHardIdx 
�� �� ֵ�� u32 
=============================================================================*/
NETCFG_API u32 EthIdxHard2Sys( u32 dwHardIdx );

#endif
#ifdef WIN32
/*=============================================================================
�� �� ���� AdapterName2EthIdx
��    �ܣ� ����������(�����ͺ�)ת��ΪӲ���������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ����	[in]	const s8 * pchAdapterDiscript
			[out]	u32 &dwEthIdx
�� �� ֵ��  BOOL32 
=============================================================================*/
NETCFG_API BOOL32 AdapterDiscript2EthIdx( const s8 * pchAdapterDiscript , u32 &dwEthIdx );


/*=============================================================================
�� �� ���� EthIdx2AdapterDiscript
��    �ܣ� ������������������������ͺ�
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ����  [int]	const u32 dwEthIdx [in]
			[out]	s8 * pchAdapterDiscript [out]
�� �� ֵ��  BOOL32 
=============================================================================*/
NETCFG_API BOOL32 EthIdx2AdapterDiscript( const u32 dwEthIdx, s8 * pchAdapterDiscript );

#endif
#endif



#endif //_NET_CONFIG_H_

