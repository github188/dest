
/*****************************************************************************
   模块名      : SusMgr
   文件名      : dbtabledef.h
   相关文件    :
   文件实现功能: ap-数据库具体数据表单的定义
   作者        :
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/07/08  1.0                     创建
******************************************************************************/
#ifndef _DBTABLE_DEFINE_ULS_H_
#define _DBTABLE_DEFINE_ULS_H_

/************************************************************************/
/*							AP数据库                   
/************************************************************************/

//#define TABLE_SERVICE_DOMAIN_INFO		    (LPCSTR)"service_domain"            // 服务域信息
#define TABLE_PLATFOEM_DOMAIN_INFO		    "platform_domain"           // 平台域信息
#define TABLE_USER_DOMAIN_INFO			    (LPCSTR)"user_domain"               // 用户域信息
#define TABLE_USERDOMAIN_PRIVILEGE_DATA		(LPCSTR)"user_domain_privilege_data"// 用户域权限列表

#define TABLE_SERVER_DEVICE_INFO			(LPCSTR)"server_info"		        // 服务器设备信息

#define TABLE_USER_INFO						(LPCSTR)"user_info"					// 用户信息
#define TABLE_USER_PROFILE				    (LPCSTR)"user_profile"				// 用户属性
#define TABLE_USER_PRIVILEGE_DATA			(LPCSTR)"user_privilege_data"		// 用户权限列表

#define TABLE_SYS_CFG           		    (LPCSTR)"t_sys_cfg"                 // 系统配置信息(平台核心域配置信息)

//2. 字段名
// 服务域表(service_domain)
//#define SERVICE_DOMAIN_MOID				    (LPCSTR)"service_domain_moid"       //MOID
//#define SERVICE_SELFBUILT_PLATFORM          (LPCSTR)"self_built_platform"       //是否自建平台（相对于租赁）

// 平台域表(platform_domain)
#define PLATFOEM_DOMAIN_MOID				"platform_domain_moid"      //MOID
#define PLATFOEM_DOMAIN_NAME				(LPCSTR)"platform_domain_name"      //域名
#define PLATFOEM_DOMAIN_SERVICEDOMAIN_MOID  (LPCSTR)"service_domain_moid"       //所属服务域
#define PLATFORM_DOMAIN_DISASTER_BACKUP     "disaster_backup"           //灾备配置（0代表关闭，1代表手动，2代表自动）
#define PLATFORM_DOMAIN_USE_BACKUP_PLATFORM "use_backup_platform"       //是否使用备平台（1代表是，0代表否）

//platform_domain_disaster（灾备配置关系表）
#define TABLE_PLATFORM_DOMAIN_DISASTER      "platform_domain_disaster"  
#define BACKUP_PLATFORM_DOMAIN_MOID         "backup_platform_domain_moid"

// 用户域表(user_domain)
#define USERDOMAIN_MOID					    (LPCSTR)"user_domain_moid"      //MOID
#define USERDOMAIN_NAME				        (LPCSTR)"user_domain_name"      //域名
#define USERDOMAIN_SERVICEDOMAIN_MOID       (LPCSTR)"service_domain_moid"   //所属服务域
#define USERDOMAIN_PLATFOEMDOMAIN_MOID		(LPCSTR)"platform_domain_moid"  //优先注册的平台域
#define USERDOMAIN_USED_FLAG                (LPCSTR)"used_flag"             //启用停用标识
#define USERDOMAIN_MAXNUM_CONF              (LPCSTR)"concurrent_number"     //最大并发会议召开数
#define USERDOMAIN_MAXNUM_ROAM              (LPCSTR)"roam_call_count"       //最大并发漫游数量
#define USERDOMAIN_MAXNUM_OUT               (LPCSTR)"out_call_count"        //最大并发出局数量
#define USERDOMAIN_MAXNUM_INCOMING          (LPCSTR)"incoming_call_count"   //最大并发入局数量
#define USERDOMAIN_MAXNUM_PHONECALL         (LPCSTR)"concurrent_phone_call_count"//最大并发电话会议数量
#define USERDOMAIN_PHONECONF_NUMBER         (LPCSTR)"phone_conf_num"        //电话会议号码
#define USERDOMAIN_MAXNUM_SATELLITE_UPLOAD  (LPCSTR)"satellite_meeting_max_number"//卫星会议最大上传路数
#define USERDOMAIN_SATELLITE_MULTICAST_IP   (LPCSTR)"multicast_ip"          //卫星组播地址

// 用户域权限列表(user_domain_privilege_data)
#define USERDOMAIN_PRIVILEGE_MOID			(LPCSTR)"user_domain_moid"	    // 用户域moid
#define USERDOMAIN_PRIVILEGE_KEY			(LPCSTR)"privilege_key"	        // 用户域拥有的权限


// 服务器设备表(server_device_info)
#define SRV_DEVICE_MOID						(LPCSTR)"server_moid"               //MOID
#define SRV_DEVICE_GUID						(LPCSTR)"device_guid"               //GUID
#define SRV_DEVICE_SERVER_TYPE				(LPCSTR)"server_type"               //服务器类型
#define SRV_DEVICE_USED_FLAG				(LPCSTR)"enable"                    //启用停用标识
#define SRV_DEVICE_USERDOMAIN_MOID          (LPCSTR)"user_domain_moid"          //所属用户域
#define SRV_DEVICE_PLATFORMDOMAIN_MOID      (LPCSTR)"platform_domain_moid"      //所属平台域
#define SRV_DEVICE_IPADDR					(LPCSTR)"device_ip"                 //IP
#define SRV_DEVICE_NET_DOMAIN_NAME			(LPCSTR)"network_domain"            //网络域名
#define SRV_DEVICE_OEM_MARK					(LPCSTR)"oem_remark"	            //SUS OEM标识


// 用户信息表(user_info)
#define USER_MOID							(LPCSTR)"moid"	              //账号全局唯一ID           	         
#define USER_E164_NO						(LPCSTR)"e164"	              //E164号 
#define USER_ACCOUNT						(LPCSTR)"account"	          //用户自定义账号 
#define USER_MOBILE						    (LPCSTR)"mobile"	          //手机号 
#define USER_JID							(LPCSTR)"jid"                 //xmpp帐号	    
#define USER_EMAIL							(LPCSTR)"email"	              //注册邮箱
#define USER_PASSWORD						(LPCSTR)"password"            //账号加密后密码
#define USER_USERDOMAIN_MOID			    (LPCSTR)"user_domain_moid"	  //所属用户域ID
#define USER_BINDED							(LPCSTR)"binded"              //是否绑定标志	    
#define USER_USED_FLAG						(LPCSTR)"enable"              //账号启用禁用标志	    
#define USER_PAS_MOID						(LPCSTR)"nu_server_id"        //用户优先登录的PAS    

// 用户属性(user_profile)
#define USER_PROFILE_MOID					(LPCSTR)"moid"	              //账号全局唯一ID           	         
#define USER_PROFILE_RESTRICT				(LPCSTR)"restrict_used_on"	  //登陆限制 

// 用户权限列表(user_privilege_data)
#define USER_PRIVILEGE_MOID			        (LPCSTR)"moid"			       // 用户moid
#define USER_PRIVILEGE_KEY					(LPCSTR)"privilege_key"	       // 用户拥有的权限


//系统配置信息(t_sys_cfg)
#define SYSCFG_KEY					        (LPCSTR)"cfg_key"               //key	    
#define SYSCFG_VALUE						(LPCSTR)"cfg_value"             //value   

#define SYSCFG_SYSTEM_MODE                  (LPCSTR)"system.mode"           //系统模式 (1-自建，0-租赁)

/************************************************************************/
/*							IP数据库                   
/************************************************************************/

#define TABLE_CITY_IP				        (LPCSTR)"city_ip"				// 国内IP地址的城市归属
#define TABLE_COUNTRY_IP			        (LPCSTR)"country_ip"		    // IP地址所属国家
#define TABLE_CODE_COUNTRY			        (LPCSTR)"code_country"		    // 国家字符代码

// 地址的城市归属(city_ip)
#define CITYIP_START_IP					    (LPCSTR)"start_ip"	            //起始IP           	         
#define CITYIP_END_IP				        (LPCSTR)"end_ip"	            //结束IP  
#define CITYIP_PROVINCE_NAME				(LPCSTR)"province"	            //所属省  
#define CITYIP_CITY_NAME				    (LPCSTR)"city"	            //所属省  
#define CITYIP_ISP				            (LPCSTR)"isp"	                //电信运营商

// 地址所属国家(country_ip)
#define COUNTRYIP_START_IP					(LPCSTR)"start_ip"	            //起始IP           	         
#define COUNTRYIP_END_IP				    (LPCSTR)"end_ip"	            //结束IP  
#define COUNTRYIP_CODE2				        (LPCSTR)"code2"	                //国家的2位代码

// 国家字符代码(code_country)
#define CODECOUNTRY_CODE2					(LPCSTR)"c2code"	            //国家的2位代码          	         
#define CODECOUNTRY_CHN_NAME				(LPCSTR)"country_cn"	        //中文国家名 
#define CODECOUNTRY_ENG_NAME				(LPCSTR)"country_en"	        //英文国家名 

#endif //end of _DBTABLE_DEFINE_ULS_H_
