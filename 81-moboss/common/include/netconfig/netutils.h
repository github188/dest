#ifndef _NET_UTILS_H_
#define _NET_UTILS_H_

#include "osp.h"
#include "kdvtype.h"
#include "kdvsys.h"
#include "netstruct.h"
#include "netconst.h"

#ifdef WIN32
#include   <setupapi.h>
#endif

#ifndef OUT
#define OUT
#endif
#ifndef IN
#define IN
#endif

 
#ifdef NETCFG_DLL_EXPORT
#define NETCFG_API   __declspec( dllexport )
#else
#define NETCFG_API
#endif	//WIN32


/*=============================================================================
函 数 名： GetIpMaskLength
功    能： 计算子网掩码长度
算法实现： 
全局变量： 
参    数： u32 dwIpMask 主机序
返 回 值： u8 子网掩码长度
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2010/10/27  2.0			陈文灿                  创建
=============================================================================*/
u8	GetIpMaskLength( u32 dwIpMask );

/*=============================================================================
函 数 名： CreatDir
功    能： 创建目录
算法实现： 
全局变量： 
参    数： const s8* lpszDirPath
返 回 值： BOOL32 
=============================================================================*/
API BOOL32 CreatDir(const s8* lpszDirPath);

/*=============================================================================
函 数 名： CreatFile
功    能： 创建任意路径下的文件，同时自动创建需要的目录
算法实现： 
全局变量： 
参    数： const s8 * lpszFileFullPath  // 文件全路径，含文件名
返 回 值： BOOL32 
=============================================================================*/
API BOOL32 CreatFile( const s8 * lpszFileFullPath );

/*=============================================================================
函 数 名： MoveToSectionStart
功    能： 将游标移到指定的段名下一行开始处
算法实现： 
全局变量： 
参    数： FILE *stream, Profile名柄   
           const s8* lpszSectionName, Profile中的段名
           BOOL32 bCreate, 如果没有该段名是否创建
返 回 值： 返回偏移量Offset，如果失败返回-1
=============================================================================*/
API s32	MoveToSectionStart( FILE *stream, const s8* lpszSectionName, BOOL32 bCreate );

/*=============================================================================
函 数 名： MoveToSectionEnd
功    能： 将游标移到指定的段名的下一段开始处或是文件结尾
算法实现： 
全局变量： 
参    数： FILE *stream, Profile名柄   
           const s8* lpszSectionName, Profile中的段名
           BOOL32 bCreate, 如果没有该段名是否创建
返 回 值： 返回偏移量Offset，如果失败返回-1
=============================================================================*/
API s32	MoveToSectionEnd( FILE *stream, const s8* lpszSectionName, BOOL32 bCreate );

/*=============================================================================
  函 数 名： strofip 
  功    能： 得到Ip字符串
  算法实现： 
  全局变量： 
  参    数： u32 dwIp
             bBONet -- 传入参数是否为网络序，默认为主机序
  返 回 值： s8 * 
=============================================================================*/
API s8* strofip(u32 dwIp, BOOL32 bBONet = FALSE);


/*=============================================================================
函 数 名： IsMacAddrInvalid
功    能： 
算法实现： 
全局变量： 
参    数： s8* pchMacAddr
           s8 chSep //MAC地址字符串的分割字符
返 回 值： BOOL32 
=============================================================================*/
API BOOL32 IsMacAddrInvalid(const s8* pchMacAddr, s8 chSep);


/*=============================================================================
函 数 名： MacToStr
功    能： 把二进值数组形式的MAC地址转换为以chSep分隔的字符串形式
算法实现： 
全局变量： 
参    数： const u8* pbyMacAddr
           s8 chSep
返 回 值： API BOOL32 
=============================================================================*/
API s8 * MacToStr(const u8* pbyMacAddr, s8 chSep);

/*=============================================================================
函 数 名： StrToMac
功    能： 把字符串形式的MAC地址转换成二进制数组形式
算法实现： 
全局变量： 
参    数： const s8* pchMacAddr
返 回 值： API BOOL32 
=============================================================================*/
API u8 * StrToMac(const s8* pchMacAddr, s8 chSep);


/*=============================================================================
函 数 名： IpIsLocalHost
功    能： 
算法实现： 
全局变量： 
参    数： u32 dwIp : 网络序
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/6/16   4.0			周广程                  创建
=============================================================================*/
BOOL32 IpIsLocalHost( u32 dwIp );


API BOOL32 GetAdpRouteTable( u32 dwIfIdx, TRoute * ptRoute, u32 * pdwTableSize );
#if 0
API BOOL32 SetAdpRoute( u32 dwIfIdx, u32 dwDestIp, u32 dwNetMask, u32 dwGatewayIp, u32 dwMetric = 1, BOOL32 bPreserved = FALSE );
#endif
API BOOL32 AddAdpRoute( u32 dwIfIdx, u32 dwDestIp, u32 dwNetMask, u32 dwGatewayIp, u32 dwMetric = 1, BOOL32 bPreserved = FALSE );
API BOOL32 DelAdpRoute( u32 dwIfIdx, u32 dwDestIp, u32 dwNetMask );

/*=============================================================================
函 数 名： AddRoute
功    能： 添加一个路由
算法实现： 
全局变量： 
参    数： u32 dwDestIp
           u32 dwNetMask
           u32 dwGatewayIp
           u32 dwMetric = 1
           BOOL32 bPreserved = FALSE
返 回 值： API BOOL32 
=============================================================================*/
API BOOL32 AddRoute( u32 dwDestIp, u32 dwNetMask, u32 dwGatewayIp, u32 dwMetric = 1, BOOL32 bPreserved = FALSE );

/*=============================================================================
函 数 名： DelRoute
功    能： 删除一个路由
算法实现： 
全局变量： 
参    数： u32 dwDestIp
           u32 dwNetMask
           u32 dwGatewayIp
返 回 值： API BOOL32 
=============================================================================*/
API BOOL32 DelRoute( u32 dwDestIp, u32 dwNetMask, u32 dwGatewayIp );


API void   AddSysDefaultGW( u32 dwGatewayIp );  // 主机序
API void   DelSysDefaultGW( void );

/*=============================================================================
函 数 名： GetNetAdapterInfo
功    能： 
算法实现： 
全局变量： 
参    数：	[out]	TNetAdaptInfoAll * ptNetAdaptInfoAll
			[in]	BOOL32 bActive -- TRUE	表示调用GetNetAdapterInfoActive接口
									  FALSE 表示调用GetNetAdapterInfoAll接口

返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/4/1	4.0			周广程                  创建
=============================================================================*/
BOOL32 GetNetAdapterInfo( TNetAdaptInfoAll * ptNetAdaptInfoAll, BOOL32 bActive = TRUE );

/*=============================================================================
函 数 名： GetNetAdapterInfoActive
功    能： 
算法实现： 
全局变量： 
参    数：	[out]	TNetAdaptInfoAll * ptNetAdaptInfoAll
			[in]	BOOL32 bOnlyEthBegin -- TRUE 表示支持网卡名仅以Eth开头
											FALSE 表示网卡名可以为任意数字大小写字符组合
			[in]	BOOL32 bCheckIPInCfg -- TRUE 检测IP是否能够在对应的网络配置文件中找到
											FALSE 不检测

返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/4/1	4.0			周广程                  创建
=============================================================================*/
NETCFG_API BOOL32 GetNetAdapterInfoActive( TNetAdaptInfoAll * ptNetAdaptInfoAll , BOOL32 bOnlyEthBegin = TRUE , BOOL32 bCheckIPInCfg = FALSE );

BOOL32 GetNetAdapterInfoFromCfg( TNetAdaptInfoAll * ptNetAdaptInfoAll );
/*=============================================================================
函 数 名： GetNetAdapterInfoAll
功    能： 该接口会先调用GetNetAdapterInfoActive接口，默认第三个参数为true
算法实现： 
全局变量： 
参    数：	[out]	TNetAdaptInfoAll * ptNetAdaptInfoAll
			[in]	BOOL32 bOnlyEthBegin -- TRUE 表示支持网卡名仅以Eth开头
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/4/1	4.0			周广程                  创建
=============================================================================*/
NETCFG_API BOOL32 GetNetAdapterInfoAll( TNetAdaptInfoAll * ptNetAdaptInfoAll , BOOL32 bOnlyEthBegin = TRUE );

#ifdef WIN32
/*=============================================================================
函 数 名： RegEnableDHCP
功    能： 
算法实现： 
全局变量： 
参    数： s8* lpszAdpName
           BOOL32 bEnable
返 回 值： BOOL32 
=============================================================================*/
BOOL32 RegEnableDHCP( const s8* lpszAdpName, BOOL32 bEnable );

/*=============================================================================
函 数 名： RegIsEnableDHCP
功    能： 
算法实现： 
全局变量： 
参    数： s8* lpszAdpName
返 回 值： BOOL32 
=============================================================================*/
BOOL32 RegIsEnableDHCP( const s8* lpszAdpName );

/*=============================================================================
  函 数 名： RegGetAdpName
  功    能： 
  算法实现： 
  全局变量： 
  参    数： s8* lpszAdpName
             u8 byLen   //数组长度
  返 回 值： BOOL32 
=============================================================================*/
BOOL32 RegGetAdpName(s8* lpszAdpName, u16 wLen);

/*=============================================================================
函 数 名： RegGetIpAdr
功    能： 返回IP字符串，以'\0'分割，以两个连续的'\0'表示结束
算法实现： 
全局变量： 
参    数： s8* lpszAdapterName
           s8* pIpAddr
           u32 &dwLen [IN/OUT]//IP数组长度
返 回 值： u16 成功返回ERROR_SUCCESS，如果返回ERROR_NOT_ENOUGH_MEMORY，则wlen返回实际需要的u32个数
=============================================================================*/
u32 RegGetIpAdr(s8* lpszAdapterName, s8* pIpAddr, u32 &dwLen);

/*=============================================================================
函 数 名： RegGetIpAdr
功    能： 获得以u32形式表示的IP地址
算法实现： 
全局变量： 
参    数： s8* lpszAdapterName
           u32* pdwIpAddr       ：缓存地址
           u32 &dwLen            ：缓存空间长度
返 回 值： u32 ：成功返回ERROR_SUCCESS，如果返回ERROR_NOT_ENOUGH_MEMORY，则wlen返回实际需要的u32个数
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/6/24   4.0			周广程                  创建
=============================================================================*/
u32 RegGetIpAdr(s8* lpszAdapterName, u32* pdwIpAddr, u32 &dwLen);

/*=============================================================================
函 数 名： RegGetLocalLinkName
功    能： 
算法实现： 
全局变量： 
参    数： s8* pasAdapterName
           s8* pasLocalLinkName
		   u16 wLocalLinkNameLen
返 回 值： 
u32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2010/5/28   4.0			周广程                  创建
=============================================================================*/
u32 RegGetLocalLinkName(s8* pasAdapterName, s8* pasLocalLinkName, u16 wLocalLinkNameLen);

/*=============================================================================
  函 数 名： RegGetNetMask
  功    能： 
  算法实现： 
  全局变量： 
  参    数： s8* lpszAdapterName
             s8* pNetMask
             u32 &dwLen  [IN/OUT] //NetMask数组长度
  返 回 值： u32 成功返回ERROR_SUCCESS，如果返回ERROR_NOT_ENOUGH_MEMORY，则wlen返回实际需要的u32个数 
=============================================================================*/
u32 RegGetNetMask(s8* lpszAdapterName, s8* pNetMask, u32 &dwLen);

/*=============================================================================
函 数 名： RegGetNetMask
功    能： 获得以u32形式表示的掩码
算法实现： 
全局变量： 
参    数： s8* lpszAdapterName
           u32* pdwNetMask
           u32 &dwLen
返 回 值： u16 成功返回ERROR_SUCCESS，如果返回ERROR_NOT_ENOUGH_MEMORY，则dwlen返回实际需要的u32个数 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/6/24   4.0			周广程                  创建
=============================================================================*/
u32 RegGetNetMask(s8* lpszAdapterName, u32* pdwNetMask, u32 &dwLen);

/*=============================================================================
函 数 名： RegGetNetGate
功    能： 
算法实现： 
全局变量： 
参    数： s8* lpszAdapterName
           s8* pNetGate
           u32 &dwLen
返 回 值： u32 
=============================================================================*/
u32 RegGetNetGate(s8* lpszAdapterName, s8* pNetGate, u32 &dwLen);

/*=============================================================================
函 数 名： RegGetNetGate
功    能： 
算法实现： 
全局变量： 
参    数： s8* lpszAdapterName
           u32* pdwNetGate
           u32 &dwLen
返 回 值： u32 
=============================================================================*/
u32 RegGetNetGate(s8* lpszAdapterName, u32* pdwNetGate, u32 &dwLen);

/*=============================================================================
  函 数 名： RegSetIpAdr
  功    能： 
  算法实现： 
  全局变量： 
  参    数： s8* pIpAddr
             u32 dwLen //IP数组长度
  返 回 值： u32 
=============================================================================*/
u32 RegSetIpAdr(s8* lpszAdapterName, s8* pIpAddr, u32 dwLen);

/*=============================================================================
函 数 名： RegSetIpAdr
功    能： 
算法实现： 
全局变量： 
参    数： s8* lpszAdapterName
           u32 * pdwIpAddr
           u32 dwLen
返 回 值： u32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/6/24   4.0			周广程                  创建
=============================================================================*/
u32 RegSetIpAdr(s8* lpszAdapterName, u32 * pdwIpAddr, u32 dwLen);

/*=============================================================================
  函 数 名： RegSetNetMask
  功    能： 
  算法实现： 
  全局变量： 
  参    数： s8* pNetMask
             u32 dwLen   [IN] //NetMask数组长度
  返 回 值： u32 
=============================================================================*/
u32 RegSetNetMask(s8* lpszAdapterName, s8* pNetMask, u32 dwLen);

/*=============================================================================
函 数 名： RegSetNetMask
功    能： 
算法实现： 
全局变量： 
参    数： s8* lpszAdapterName
           u32* pdwNetMask
           u32 dwLen
返 回 值： u32 
=============================================================================*/
u32 RegSetNetMask(s8* lpszAdapterName, u32* pdwNetMask, u32 dwLen);

/*=============================================================================
函 数 名： RegSetNetGate
功    能： 
算法实现： 
全局变量： 
参    数： const s8* lpszAdapterName
           s8* pNetGate
           u32 dwLen
返 回 值： u32 
=============================================================================*/
u32 RegSetNetGate(const s8* lpszAdapterName, s8* pNetGate, u32 dwLen);

/*=============================================================================
函 数 名： RegSetNetGate
功    能： 
算法实现： 
全局变量： 
参    数： const s8* lpszAdapterName
           u32* pdwNetGate
           u32 dwLen
返 回 值： u32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/6/24   4.0			周广程                  创建
=============================================================================*/
u32 RegSetNetGate(const s8* lpszAdapterName, u32* pdwNetGate, u32 dwLen);

BOOL32 RegGetRoute(TRoute * ptRoute, u32 wLen);
BOOL32 RegAddRoute(TRoute * ptRoute );
BOOL32 RegDelRoute(TRoute * ptRoute );
BOOL32 RegDelRouteAll(void);
u32    RegGetRouteNum(void);


/*=============================================================================
函 数 名： GetLocalIp
功    能： 
算法实现： 
全局变量： 
参    数： 
返 回 值： u32 
=============================================================================*/
u32 GetLocalIp();

/*=============================================================================
函 数 名： CheckIPAddress
功    能： 
算法实现： 
全局变量： 
参    数： s8 *apchIPAddr[32]   [in] 待校验的ip地址 
		   const u8 byIpNum     [in] 传入ip数量，最大32个
           const u16 dConPort   [in] socket连接端口
		   const u16 dTimeOut)  [in] 连接超时时间，单位毫秒
返 回 值： s8*                  [ret] 如果为空，则没有能连上的地址；否则返回连接时间最短的ip
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/7/20   1.0		    曾捷                  创建
=============================================================================*/
s8* CheckIPAddress(s8 *apchIPAddr[], const u8 byIpNum, const u16 dConPort, const u16 dTimeOut);

/*=============================================================================
函 数 名： AddAdapterIp
功    能： 
算法实现： 
全局变量： 
参    数：  u32 dwIpAddr
           u32 dwIpMask
           s8 * pchAdapterName
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/3/31   4.0		    周广程                  创建
=============================================================================*/
BOOL32 AddAdapterIp( u32 dwIpAddr, u32 dwIpMask, s8 * pchAdapterName );

/*=============================================================================
函 数 名： SetAdapterIp
功    能： 
算法实现： 
全局变量： 
参    数：  TNetParam * ptNetParam
           u16 wParamNum
           s8 * pchAdapterName
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/3/31   4.0		周广程                  创建
=============================================================================*/
BOOL32 SetAdapterIp( TNetParam * ptNetParam, u16 wParamNum, s8 * pchAdapterName );

/*=============================================================================
函 数 名： AddDefaultGW
功    能： 
算法实现： 
全局变量： 
参    数： u32 dwGWAddr
           s8 * pchAdapterName
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/3/31   4.0		周广程                  创建
=============================================================================*/
BOOL32 AddDefaultGW( u32 dwGWAddr, s8 * pchAdapterName );

/*=============================================================================
函 数 名： DelDefaultGW
功    能： 
算法实现： 
全局变量： 
参    数： s8 * pchAdapterName
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/5/13   4.0		周广程                  创建
=============================================================================*/
BOOL32 DelDefaultGW( const s8 * pchAdapterName );

/*=============================================================================
函 数 名： SetDefaultGW
功    能： 
算法实现： 
全局变量： 
参    数： u32 *pdwGWAddr
           u16 wGWNum
           s8 * pchAdapterName
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/3/31   4.0		周广程                  创建
=============================================================================*/
BOOL32 SetDefaultGW( u32 *pdwGWAddr, u16 wGWNum, s8 * pchAdapterName );

/*=============================================================================
函 数 名： IsIpAddrIn
功    能： 
算法实现： 
全局变量： 
参    数：  u32 dwIpAddr
           s8 * pchAdapterName
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/3/31   4.0		    周广程                  创建
=============================================================================*/
BOOL32 IsIpAddrIn( u32 dwIpAddr, s8 * pchAdapterName );

/*=============================================================================
函 数 名： DelAdapterIp
功    能： 
算法实现： 
全局变量： 
参    数：  u32 dwIpAddr
           s8 * pchAdapterName
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/3/31   4.0		周广程                  创建
=============================================================================*/
BOOL32 DelAdapterIp( u32 dwIpAddr, s8 * pchAdapterName );

/*=============================================================================
函 数 名： SetAdapterRoute
功    能： 
算法实现： 
全局变量： 
参    数：  TRoute * ptRoute
           u32 wRouteNum
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/4/9   4.0		周广程                  创建
=============================================================================*/
BOOL32 SetAdapterRoute( TRoute * ptRoute, u32 dwRouteNum );
BOOL32 GetAdapterRoute( TRoute * ptRoute, u32 dwRouteNum );

/*=============================================================================
函 数 名： NotifyIPChange
功    能： 
算法实现： 
全局变量： 
参    数： LPCTSTR lpszAdapterName
           u32 nIndex
           LPCTSTR pIPAddress
           LPCTSTR pNetMask
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/3/31   4.0		周广程                  创建
=============================================================================*/
typedef BOOL (WINAPI *BDHCPNOTIFYPROC)( LPWSTR lpwszServerName, // 本地机器为NULL 
                                        LPWSTR lpwszAdapterName, // 适配器名称 
                                        BOOL bNewIpAddress, // TRUE表示更改IP 
                                        DWORD dwIpIndex, // 指明第几个IP地址，如果只有该接口只有一个IP地址则为0 
                                        DWORD dwIpAddress, // IP地址 
                                        DWORD dwSubNetMask, // 子网掩码 
                                        int nDhcpAction ); // 对DHCP的操作 0:不修改, 1:启用 DHCP，2:禁用 DHCP 
BOOL32 NotifyIPChange(LPCTSTR lpszAdapterName, s32 nIndex, LPCTSTR pIPAddress, LPCTSTR pNetMask); 

//网卡操作函数
BOOL StateChange( DWORD NewState, DWORD SelectedItem, HDEVINFO hDevInfo );
u32 GetRegistryProperty( HDEVINFO hDevInfo, PSP_DEVINFO_DATA DeviceInfoData, ULONG Property, PVOID Buffer, PULONG Length );
BOOL IsSpecNetCard( HDEVINFO hDevInfo, PSP_DEVINFO_DATA DeviceInfoData, const s8 * pchAdapterName  );
BOOL32 NetCardOper( const s8 * pchAdapterName, BOOL32 bStart );

#endif //WIN32


#ifdef _LINUX_

/*=============================================================================
函 数 名： GetDefaultGW
功    能： 获得默认网关
算法实现： 
全局变量： 
参    数： u32 &dwDefaultGW
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/4/2   4.0		周广程                  创建
=============================================================================*/
BOOL32 GetDefaultGW( u32 dwIfIndex, u32 &dwDefaultGW );

/*=============================================================================
函 数 名： GetDefaultGW
功    能： 获得默认网关
算法实现： 
全局变量： 
参    数： u32 &dwDefaultGW
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/01/31  KDVP2.5.2   heyisen                 创建
=============================================================================*/
BOOL32 GetDefaultGW(const s8 * pchAdapterName ,u32 &dwDefaultGW ) ;

/*=============================================================================
函 数 名： GetDefaultGW
功    能： 获得默认网关
算法实现： 
全局变量： 
参    数： u32 &dwDefaultGW
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/01/31  KDVP2.5.2   heyisen                 创建
=============================================================================*/
BOOL32 GetDefaultGWFromNetWorkCfg( u32 &dwDefaultGW ); 

/*=============================================================================
函 数 名： SetDefaultGW
功    能： 设置默认网关
算法实现： 
全局变量： 
参    数：  u32 dwIfIndex
           u32 dwDefaultGW
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/4/2   4.0		周广程                  创建
=============================================================================*/
BOOL32 SetDefaultGW( u32 dwIfIndex, u32 dwDefaultGW );

/*=============================================================================
函 数 名： SetDefaultGW
功    能： 设置默认网关
算法实现： 
全局变量： 
参    数：  u32 dwIfIndex
           u32 dwDefaultGW
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/4/2   4.0		周广程                  创建
=============================================================================*/
BOOL32 SetDefaultGW( const s8 * pchAdapterName , u32 dwDefaultGW );

/*=============================================================================
函 数 名： DelDefaultGW
功    能： 
算法实现： 
全局变量： 
参    数：  u32 dwIfIndex
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/4/3   4.0		周广程                  创建
=============================================================================*/
BOOL32 DelDefaultGW( u32 dwIfIndex );

/*=============================================================================
函 数 名： DelDefaultGWBaseonAdapterName
功    能： 
算法实现： 
全局变量： 
参    数： [in]	s8 * const pchAdapterName 网卡名(eth0)
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/1/29   KDVP2.5.2   heyisen                  创建
=============================================================================*/
BOOL32 DelDefaultGWBaseonAdapterName( const s8 *  pchAdapterName );

/*=============================================================================
函 数 名： DelIPBaseonAdapterName
功    能： 
算法实现： 
全局变量： 
参    数：  const s8 *  pchAdapterName 
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/1/29   KDVP2.5.2   heyisen                  创建
=============================================================================*/
BOOL32 DelIPBaseonAdapterName( const s8 *  pchAdapterName );

/*=============================================================================
函 数 名： SetAdapterIp
功    能： 
算法实现： 
全局变量： 
参    数：  u32 dwIfIndex
           TNetParam tIp
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/4/2   4.0		周广程                  创建
=============================================================================*/
BOOL32 SetAdapterIp( u32 dwIfIndex, TNetParam tIp );

/*=============================================================================
函 数 名： SetAdapterIp
功    能： 
算法实现： 
全局变量： 
参    数：  const s8 * pchAdapterName 
           TNetParam tIp
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/1/13   2.5.2       heyisen               Create
=============================================================================*/
BOOL32 SetAdapterIp( const s8 * pchAdapterName , TNetParam tIp );

/*=============================================================================
函 数 名： AddAdapterIp
功    能： 
算法实现： 
全局变量： 
参    数：  u32 dwIfIndex
           TNetParam tIp
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/5/26   4.0			周广程                  创建
=============================================================================*/
BOOL32 AddAdapterIp( u32 dwIfIndex, TNetParam tIp );

/*=============================================================================
函 数 名： AddAdapterIp
功    能： 
算法实现： 
全局变量： 
参    数： const s8 * pchAdapterName 
           TNetParam tIp
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/1/13   2.5.2       heyisen               Create
=============================================================================*/
BOOL32 AddAdapterIp(const s8 * pchAdapterName , TNetParam tIp );

/*=============================================================================
函 数 名： DelAdapterIp
功    能： 
算法实现： 
全局变量： 
参    数：  u32 dwIfIndex
           u32 dwIp
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/4/3   4.0		周广程                  创建
=============================================================================*/
BOOL32 DelAdapterIp( u32 dwIfIndex, u32 dwIp );

/*=============================================================================
函 数 名： SetAdapterMac
功    能： 
算法实现： 
全局变量： 
参    数：  u32 dwIfIndex
const s8* pchMacAddr
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2010/5/28   4.0			周广程                  创建
=============================================================================*/
BOOL32 SetAdapterMac( u32 dwIfIndex, const s8* pchMacAddr );

/*=============================================================================
函 数 名： SetAdapterMac
功    能： 
算法实现： 
全局变量： 
参    数： const s8 * pchAdapterName
const s8* pchMacAddr
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/2/1    2.5.2		heyisen                 创建
=============================================================================*/
BOOL32 SetAdapterMac( const s8 * pchAdapterName, const s8* pchMacAddr );

/*=============================================================================
函 数 名： SetAdapterMac
功    能： 
算法实现： 
全局变量： 
参    数： u32 dwIfIndex
const u8* pbyMacAddr
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2010/5/28   4.0			周广程                  创建
=============================================================================*/
BOOL32 SetAdapterMac( u32 dwIfIndex, const u8* pbyMacAddr, u8 byMacAddrLen );

/*=============================================================================
函 数 名： SetAdapterMac
功    能： 设置网络适配器的mac地址
算法实现： 
全局变量： 
参    数： const s8 * pchAdapterName
           const s8* pchMacAddr
           u8 byMacAddrLen
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/2/1    2.5.2		heyisen                 创建
=============================================================================*/
BOOL32 SetAdapterMac( const s8 * pchAdapterName, const u8 * pbyMacAddr, u8 byMacAddrLen );

/*=============================================================================
函 数 名： NetCardOper
功    能： 
算法实现： 
全局变量： 
参    数：  u32 dwIfIndex
           BOOL32 bStart
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/6/16   4.0			周广程                  创建
=============================================================================*/
BOOL32 NetCardOper( u32 dwIfIndex, BOOL32 bStart );

/*=============================================================================
函 数 名： IsValidEthIfFileName
功    能： 
算法实现： 
全局变量： 
参    数： const s8 * pchEthIfFileName
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/6/17   4.0			周广程                  创建
=============================================================================*/
BOOL32 IsValidEthIfFileName( const s8 * pchEthIfFileName );

/*=============================================================================
函 数 名： GetEthIdxbyEthIfFileName
功    能： 对形如ifcfg-ethX 或者 ifcfg-ethX:X之类的网口配置文件名，取得网口号
算法实现： 
全局变量： 
参    数： const s8 * pchEthIfFileName
返 回 值： s8  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/6/18   4.0			周广程                  创建
=============================================================================*/
s8  GetEthIdxbyEthIfFileName( const s8 * pchEthIfFileName );

/*=============================================================================
函 数 名： IsValidEthIfName
功    能： 对形如 ethX 或 ethX:Y 的网口名，判断是否合法
算法实现： 
全局变量： 
参    数： const s8 * pchEthIfName
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/6/25   4.0			周广程                  创建
=============================================================================*/
BOOL32 IsValidEthIfName( const s8 * pchEthIfName );

/*=============================================================================
函 数 名： GetEthIdxbyEthIfName
功    能： 对形如 ethX 或 ethX:Y 的网口名，取得网口号
算法实现： 
全局变量： 
参    数： const s8 * pchEthIfName
返 回 值： s8  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/6/25   4.0			周广程                  创建
=============================================================================*/
s8  GetEthIdxbyEthIfName( const s8 * pchEthIfName );

/*=============================================================================
函 数 名： IsValidAdapterName
功    能： 判断网卡名是否合法，网卡名可以为大小写字母或者数字，如果有:则
		   其后面必须为数字 
算法实现： 
全局变量： 
参    数： const s8 * pchEthIfName
返 回 值： s8  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2013/01/29  kdvp2.5.2	heyisen               创建
=============================================================================*/
BOOL32 IsValidAdapterName( const s8 * pchEthIfName );

/*=============================================================================
函 数 名： IsLegalIpAddr
功    能： 判断是否为合法的ip地址
算法实现： 
全局变量： 
参    数： const s8 * pchIpAddr
返 回 值： TRUE-合法ip   FALSE-非法ip  
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2012/05/02  2.5			王也                    创建
=============================================================================*/
BOOL32 IsLegalIpAddr( const s8 * pchIpAddr );

/*=============================================================================
函 数 功 能 : 对形式为 XXX=YYY 的配置文件进行操作的函数组，无需段名
=============================================================================*/
s32 WWriteKeyValueString( FILE *stream, s32 sdwOffsetCurLine, s32 sdwOffsetNextLine, const s8* lpszKeyName, const s8* lpszKeyValue );
s32 WReadWriteKeyValueString( FILE *stream, const s8* lpszKeyName, s8* lpszKeyValue, u32 dwBufSize, BOOL32 bWrite );
BOOL32 WGetRegKeyString2( FILE *stream, const s8* lpszKeyName, s8*  lpszReturnValue, u32 dwBufSize );
BOOL32 WGetRegKeyString( const s8* lpszProfileName, const s8* lpszKeyName, s8*  lpszReturnValue, u32 dwBufSize );
BOOL32 WSetRegKeyString( const s8* lpszProfileName,  const s8* lpszKeyName, const s8* lpszValue);
BOOL32 WDeleteRegKey( const s8* lpszProfileName, const s8* lpszKeyName );
BOOL32 WriteToFile(const s8* lpszProfileName,const s8* lpszValue);
#endif


#endif // _NET_UTILS_H_



