
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
函 数 名： SetNetParamAll
功    能： 设置系统的网卡参数并写入文件
算法实现： 
全局变量： 
参    数：	[in]	TMINIMCUNetParamAll_Mcu * ptNetParamAll_Mcu		希望设置的网卡信息
			[in]	TNetAdaptInfoAll * ptNetAdaptInfoAll			系统网卡信息
			[in]	u32 & dwDefaultRouteIpAddr	默认路由，与修改值不同则会被改掉，第5个参数决定是否对此进行修改
			[out]	const s8* lpszProfileName	非空则修改配置，否则仅修改系统网络参数
			[in]	BOOL32 IsChangeRouteIp		是否修改路由选项	TRUE  -- 修改静态路由	默认值为TRUE
																	FALSE -- 不修改
返 回 值：  void 
=============================================================================*/
NETCFG_API void SetNetParamAll(TMINIMCUNetParamAll_Mcu * ptNetParamAll, TNetAdaptInfoAll * ptNetAdaptInfoAll, const s8* lpszProfileName = NULL);

/*=============================================================================
函 数 名： SetNetParamAllFromFile
功    能： 从文件中获取并设置网卡信息
算法实现： 
全局变量： 
参    数：	[out]	TMINIMCUNetParamAll_Mcu * ptNetParamAll_Mcu
			[out]	TNetAdaptInfoAll * ptNetAdaptInfoAll
			[in]	const s8* lpszProfileName
			[out]	u32 & dwDefaultRouteIpAddr
返 回 值：  BOOL32 
=============================================================================*/
NETCFG_API BOOL32 SetNetParamAllFromFile( TMINIMCUNetParamAll_Mcu * ptNetParamAll , TNetAdaptInfoAll * ptNetAdaptInfoAll , const s8* lpszProfileName ,u32 & dwDefaultRouteIpAddr);

/*=============================================================================
函 数 名： TableMemoryFree
功    能： 内存表释放
算法实现： 
全局变量： 
参    数：  [in]	void **ppMem
			[in]	u32 dwEntryNum
返 回 值：  BOOL32 
=============================================================================*/
NETCFG_API BOOL32 TableMemoryFree( void **ppMem , u32 dwEntryNum );

/*=============================================================================
函 数 名： GetNetParamAll
功    能： 从文件中获取网卡信息
算法实现： 
全局变量： 
参    数：	[out]	TMINIMCUNetParamAll_Mcu * ptNetParamAll_Mcu
			[int]	const s8* lpszProfileName
返 回 值：	BOOL32 
=============================================================================*/
NETCFG_API BOOL32 GetNetParamAll( TMINIMCUNetParamAll_Mcu * ptNetParamAll , const s8* lpszProfileName );

/*=============================================================================
函 数 名： GetSystemDefaultGW
功    能： 从系统中获取默认网关
算法实现： 
全局变量： 
参    数：	[out]	u32 &dwDefaultGW
返 回 值：	BOOL32 
=============================================================================*/
NETCFG_API BOOL32 GetSystemDefaultGW(u32 &dwDefaultGW);

/*=============================================================================
函 数 名： IsValidNetParamAll
功    能： 判定网卡参数是否有效
算法实现： 
全局变量： 
参    数： [in]	const TMINIMCUNetParamAll &tMINIMCUNetParamAll
返 回 值： BOOL32 
=============================================================================*/
NETCFG_API BOOL32 IsValidNetParamAll( TMINIMCUNetParamAll * ptMINIMCUNetParamAll);


/*=============================================================================
函 数 名： WriteNetConfigToFile
功    能： 将网卡参数写入配置文件
算法实现： 
全局变量： 
参    数：	[in]	TMINIMCUNetParamAll_Mcu * ptNetParamAll	网卡信息(单网卡或多网卡)
			[out]	const s8* lpszProfileName				写入的配置文件名(含路径)
返 回 值：	void 
=============================================================================*/
NETCFG_API void WriteNetConfigToFile( TMINIMCUNetParamAll_Mcu * ptNetParamAll , const s8* lpszProfileName );

/*=============================================================================
函 数 名： NetParamIn2Out
功    能： 系统识别的网卡参数转换为硬件网卡参数
算法实现： 
全局变量： 
参    数：  [in]	TMINIMCUNetParamAll_Mcu & ptNetParamAll_Mcu
			[out]	TMINIMCUNetParamAll & ptNetParamAll
返 回 值：  void 
=============================================================================*/
NETCFG_API void NetParamIn2Out( TMINIMCUNetParamAll_Mcu * ptNetParamAll_Mcu , TMINIMCUNetParamAll * ptNetParamAll );

/*=============================================================================
函 数 名： NetParamOut2In
功    能： 硬件网卡参数转换为系统识别的网卡参数
算法实现： 
全局变量： 
参    数：	[in]	const TMINIMCUNetParamAll &tMINIMCUNetParamAll
			[in]	TNetAdaptInfoAll &ptNetAdaptInfoAll
			[out]   TMINIMCUNetParamAll_Mcu &tMINIMCUNetParamAll_Mcu
返 回 值：  void 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/06/04  1.0			陈文灿                  创建
=============================================================================*/
NETCFG_API void NetParamOut2In( TMINIMCUNetParamAll * ptNetParamAll, TNetAdaptInfoAll * ptNetAdaptInfoAll, TMINIMCUNetParamAll_Mcu * ptNetParamAll_Mcu );


#ifdef _HARDWARE_
#ifdef _LINUX_
/*=============================================================================
函 数 名： EthIdxSys2Hard
功    能： 系统识别的网卡序号转换成硬件网卡序号
算法实现： 
全局变量： 
参    数： [in]	u32 dwSysIdx 
返 回 值： u32 
=============================================================================*/
NETCFG_API u32 EthIdxSys2Hard( u32 dwSysIdx );


/*=============================================================================
函 数 名： EthIdxHard2Sys
功    能： 硬件网卡序号转换成系统识别的网卡序号
算法实现： 
全局变量： 
参    数： [in]	u32 dwHardIdx 
返 回 值： u32 
=============================================================================*/
NETCFG_API u32 EthIdxHard2Sys( u32 dwHardIdx );

#endif
#ifdef WIN32
/*=============================================================================
函 数 名： AdapterName2EthIdx
功    能： 适配器描述(网卡型号)转换为硬件网卡序号
算法实现： 
全局变量： 
参    数：	[in]	const s8 * pchAdapterDiscript
			[out]	u32 &dwEthIdx
返 回 值：  BOOL32 
=============================================================================*/
NETCFG_API BOOL32 AdapterDiscript2EthIdx( const s8 * pchAdapterDiscript , u32 &dwEthIdx );


/*=============================================================================
函 数 名： EthIdx2AdapterDiscript
功    能： 根据网卡号索引获得网卡的型号
算法实现： 
全局变量： 
参    数：  [int]	const u32 dwEthIdx [in]
			[out]	s8 * pchAdapterDiscript [out]
返 回 值：  BOOL32 
=============================================================================*/
NETCFG_API BOOL32 EthIdx2AdapterDiscript( const u32 dwEthIdx, s8 * pchAdapterDiscript );

#endif
#endif



#endif //_NET_CONFIG_H_

