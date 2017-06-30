
/*=======================================================================
ģ����      :
�ļ���      :constuls.h
����ļ�    :
�ļ�ʵ�ֹ���:ULS��ULC���õĳ�����Ϣ
����        :
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/07/22  1.0                       ����
=======================================================================*/
#ifndef _CONST_ULS_H_
#define _CONST_ULS_H_

#include "kdvdef.h"

#define AID_APS_CENTERCTRL                  AID_ULS_BGN            //APS����
#define AID_OLD_26_APS_ULCSSN               AID_ULS_BGN + 1        //2.6�汾APS��Ulc�Ự   
#define AID_APS_HTTP_SSN                    AID_ULS_BGN + 2        //HTTP�Ự   
#define AID_APS_CONNECTSA                   AID_ULS_BGN + 3        //APS��SA�Ự

//�����˿�
#define APS_OSP_LISTEN_PORT                 (u16)60070
#define APS_HTTP_LISTEN_PORT                (u16)60080			 
#define APS_HTTPS_LISTEN_PORT                (u16)443		

//
#define MAXINSTNUM_ULCSSN                   (u32)5000                         //�ն����������
#define MAXNUM_DB_CONNECT                   (u32)6                            //���ݿ����������



#define MAXNUM_ULSIPLIST                    (u16)255

#define MAXLEN_URL                          (u16)100
#define MAXLEN_ULS_JID                      (u16)64
#define MAXLEN_PORT                         (u16)6
#define MAXLEN_PRIVILEGE_LIST               (u16)512


//�û�Ȩ��
#define USER_PRIVILEGE_CALL                 (const s8*)"enableCall"         //�绰����Ȩ��
#define USER_PRIVILEGE_MEETING              (const s8*)"enableMeeting"      //�������Ȩ��
#define USER_PRIVILEGE_MEETING_ADMIN        (const s8*)"meetingAdmin"       //�������Ա
#define USER_PRIVILEGE_SATELLITE            (const s8*)"enableSatellite"    //������·Ȩ��
#define USER_PRIVILEGE_SATELLITE_P2P        (const s8*)"enableSatelliteP2P" //���ǵ�Ե�
#define USER_PRIVILEGE_MEETINGSMS           (const s8*)"enableMeetingSMS"   //��ܶ���Ȩ��
#define USER_PRIVILEGE_WEIBO                (const s8*)"enableWeibo"        //΢��Ȩ��
#define USER_PRIVILEGE_WEIBO_ADMIN          (const s8*)"weiboAdmin"         //΢������Ա

//�û���Ȩ��
#define USERDOMAIN_PRIVILEGE_ROAM           (const s8*)"enableRoam"         //����Ȩ��
#define USERDOMAIN_PRIVILEGE_CALL           (const s8*)"enableCall"         //�绰����Ȩ��
#define USERDOMAIN_PRIVILEGE_MEETING        (const s8*)"enableMeeting"      //�������Ȩ��
#define USERDOMAIN_PRIVILEGE_SATELLITE      (const s8*)"enableSatellite"    //������·Ȩ��
#define USERDOMAIN_PRIVILEGE_MEETINGSMS     (const s8*)"enableMeetingSMS"   //��ܶ���Ȩ��
#define USERDOMAIN_PRIVILEGE_WEIBO          (const s8*)"enableWeibo"        //΢��Ȩ��

//������Ȩ��
#define SERVICEDOMAIN_PRIVILEGE_PORTMEDIA   (const s8*)"enablePortMedia"    //�˿�ý��Ȩ��


//http�ĵ�¼����
#define HTTP_ARGUMENT_VERSION               (const s8*)"Version"
#define HTTP_ARGUMENT_USERNAME              (const s8*)"UserName"
#define HTTP_ARGUMENT_PASSWORD              (const s8*)"PassWord"
#define HTTP_ARGUMENT_OEMMARK               (const s8*)"OemMark"
#define HTTP_ARGUMENT_DEVICETYPE            (const s8*)"DeviceType"
#define HTTP_ARGUMENT_APSLOCALIP            (const s8*)"ApsLocalIp"

#define HTTP_COOKIE_CLIENT_IP               (const s8*)"X-Forwarded-For"


#define ISP_NAME_LOCAL                      (const s8*)"����"
#define ISP_NAME_FOREIGN                    (const s8*)"����"

#endif //end of _CONST_ULS_H_
