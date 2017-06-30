#ifndef _NET_CONST_H_
#define _NET_CONST_H_




#ifdef _LINUX_
#define ROUTE_CFGFILE       "/proc/net/route"
#define NETWORK_CFGFILE     "/etc/sysconfig/network"
#define NETIF_CFGFILE_DIR   "/etc/sysconfig/network-scripts/"  
#define RC_LOCAL            "/etc/rc.d/rc.local"

#define GW_KEY      (LPCSTR)"NMGWK"
#define IP_KEY      (LPCSTR)"IPADDR"
#define HWADDR_KEY  (LPCSTR)"HWADDR"
#define DEVICE_KEY  (LPCSTR)"DEVICE"
#define NETMAS_KEY  (LPCSTR)"NETMASK"
#define TYPE_KEY    (LPCSTR)"TYPE"
#define ONBOOT_KEY  (LPCSTR)"ONBOOT"
#define BOOTPR_KEY  (LPCSTR)"BOOTPROTO"
#endif // _LINUX_



// MINIMCU所在的网络类型
#define	NETTYPE_INVALID						(u8)0
#define NETTYPE_LAN							(u8)1
#define NETTYPE_WAN							(u8)2
#define NETTYPE_MIXING_ALLPROXY				(u8)3
#define NETTYPE_MIXING_NOTALLPROXY			(u8)4


// 网络配置参数的配置文件项
#define SECTION_McuNetParam                     (const s8*)"McuNetParam"
#define KEY_ACTUALNETNUM                        (const s8*)"ActualNetNum"
#define KEY_NetParamNum							(const s8*)"EntryNum"
#define ENTRY_NetParam							(const s8*)"Entry"
#define FIELD_netparamNetworkType               (const s8*)"netparamNetworkType"
#define FIELD_netparamAdapterName               (const s8*)"netparamAdapterName"
#define FIELD_netparamIpAddr                    (const s8*)"netparamIpAddr"
#define FIELD_netparamIpMask                    (const s8*)"netparamIpMask"
#define FIELD_netparamGateWay                   (const s8*)"netparamGateWay"
#define FIELD_netparamServerAlias               (const s8*)"netparamServerAlias"
#define SECTION_VIRINFO                         (const s8*)"VirtualIP"
#define KEY_VIRIPNUM                            (const s8*)"VirtualIPNum"
#define KEY_VIRIPLIST                           (const s8*)"VirtualIP" 


#define MCU_MAX_ADAPTER_NAME_LENGTH         256 // arb.
#define MAXLEN_PWD                          32   //最大密码长度  
#define MCU_MAXNUM_ADAPTER                  (u32)12
#define MCU_MAXNUM_ADAPTER_IP               (u32)16
#define MCU_MAX_ADAPTER_DESCRIPTION_LENGTH  128 // arb.
#define MCU_MAX_ADAPTER_ADDRESS_LENGTH      8   // arb.
#define MCU_MAXNUM_ADAPTER_GW               (u32)16
#define ETH_IP_MAXNUM						16

 


// mcucfg.ini 配置文件的最大长度
#define    MAXLEN_MCUCFG_INI                8192


#define MCU_MIB_IF_TYPE_OTHER               1
#define MCU_MIB_IF_TYPE_ETHERNET            6
#define MCU_MIB_IF_TYPE_TOKENRING           9
#define MCU_MIB_IF_TYPE_FDDI                15
#define MCU_MIB_IF_TYPE_PPP                 23
#define MCU_MIB_IF_TYPE_LOOPBACK            24
#define MCU_MIB_IF_TYPE_SLIP                28

//policyroutecfg.ini配置文件
#define SECTION_McuPolicyParam             (const s8*)"McuPolicyRouteParam"
#define KEY_PolicyParamNum				   (const s8*)"EntryNum"
#define ENTRY_PolicyParam				   (const s8*)"Entry"

#endif




