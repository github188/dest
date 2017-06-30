
/*=======================================================================
模块名      :
文件名      :constuls.h
相关文件    :
文件实现功能:ULS与ULC共用的常量信息
作者        :
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/07/22  1.0                       创建
=======================================================================*/
#ifndef _CONST_ULS_H_
#define _CONST_ULS_H_

#include "kdvdef.h"

#define AID_APS_CENTERCTRL                  AID_ULS_BGN            //APS主控
#define AID_OLD_26_APS_ULCSSN               AID_ULS_BGN + 1        //2.6版本APS与Ulc会话   
#define AID_APS_HTTP_SSN                    AID_ULS_BGN + 2        //HTTP会话   
#define AID_APS_CONNECTSA                   AID_ULS_BGN + 3        //APS与SA会话

//侦听端口
#define APS_OSP_LISTEN_PORT                 (u16)60070
#define APS_HTTP_LISTEN_PORT                (u16)60080			 
#define APS_HTTPS_LISTEN_PORT                (u16)443		

//
#define MAXINSTNUM_ULCSSN                   (u32)5000                         //终端最大连接数
#define MAXNUM_DB_CONNECT                   (u32)6                            //数据库最大连接数



#define MAXNUM_ULSIPLIST                    (u16)255

#define MAXLEN_URL                          (u16)100
#define MAXLEN_ULS_JID                      (u16)64
#define MAXLEN_PORT                         (u16)6
#define MAXLEN_PRIVILEGE_LIST               (u16)512


//用户权限
#define USER_PRIVILEGE_CALL                 (const s8*)"enableCall"         //电话会议权限
#define USER_PRIVILEGE_MEETING              (const s8*)"enableMeeting"      //会议管理权限
#define USER_PRIVILEGE_MEETING_ADMIN        (const s8*)"meetingAdmin"       //会议管理员
#define USER_PRIVILEGE_SATELLITE            (const s8*)"enableSatellite"    //卫星线路权限
#define USER_PRIVILEGE_SATELLITE_P2P        (const s8*)"enableSatelliteP2P" //卫星点对点
#define USER_PRIVILEGE_MEETINGSMS           (const s8*)"enableMeetingSMS"   //会管短信权限
#define USER_PRIVILEGE_WEIBO                (const s8*)"enableWeibo"        //微博权限
#define USER_PRIVILEGE_WEIBO_ADMIN          (const s8*)"weiboAdmin"         //微博管理员

//用户域权限
#define USERDOMAIN_PRIVILEGE_ROAM           (const s8*)"enableRoam"         //漫游权限
#define USERDOMAIN_PRIVILEGE_CALL           (const s8*)"enableCall"         //电话会议权限
#define USERDOMAIN_PRIVILEGE_MEETING        (const s8*)"enableMeeting"      //会议管理权限
#define USERDOMAIN_PRIVILEGE_SATELLITE      (const s8*)"enableSatellite"    //卫星线路权限
#define USERDOMAIN_PRIVILEGE_MEETINGSMS     (const s8*)"enableMeetingSMS"   //会管短信权限
#define USERDOMAIN_PRIVILEGE_WEIBO          (const s8*)"enableWeibo"        //微博权限

//服务域权限
#define SERVICEDOMAIN_PRIVILEGE_PORTMEDIA   (const s8*)"enablePortMedia"    //端口媒体权限


//http的登录参数
#define HTTP_ARGUMENT_VERSION               (const s8*)"Version"
#define HTTP_ARGUMENT_USERNAME              (const s8*)"UserName"
#define HTTP_ARGUMENT_PASSWORD              (const s8*)"PassWord"
#define HTTP_ARGUMENT_OEMMARK               (const s8*)"OemMark"
#define HTTP_ARGUMENT_DEVICETYPE            (const s8*)"DeviceType"
#define HTTP_ARGUMENT_APSLOCALIP            (const s8*)"ApsLocalIp"

#define HTTP_COOKIE_CLIENT_IP               (const s8*)"X-Forwarded-For"


#define ISP_NAME_LOCAL                      (const s8*)"本地"
#define ISP_NAME_FOREIGN                    (const s8*)"海外"

#endif //end of _CONST_ULS_H_
