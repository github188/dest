
/*****************************************************************************
   ģ����      : SusMgr
   �ļ���      : dbtabledef.h
   ����ļ�    :
   �ļ�ʵ�ֹ���: ap-���ݿ�������ݱ��Ķ���
   ����        :
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/07/08  1.0                     ����
******************************************************************************/
#ifndef _DBTABLE_DEFINE_ULS_H_
#define _DBTABLE_DEFINE_ULS_H_

/************************************************************************/
/*							AP���ݿ�                   
/************************************************************************/

//#define TABLE_SERVICE_DOMAIN_INFO		    (LPCSTR)"service_domain"            // ��������Ϣ
#define TABLE_PLATFOEM_DOMAIN_INFO		    "platform_domain"           // ƽ̨����Ϣ
#define TABLE_USER_DOMAIN_INFO			    (LPCSTR)"user_domain"               // �û�����Ϣ
#define TABLE_USERDOMAIN_PRIVILEGE_DATA		(LPCSTR)"user_domain_privilege_data"// �û���Ȩ���б�

#define TABLE_SERVER_DEVICE_INFO			(LPCSTR)"server_info"		        // �������豸��Ϣ

#define TABLE_USER_INFO						(LPCSTR)"user_info"					// �û���Ϣ
#define TABLE_USER_PROFILE				    (LPCSTR)"user_profile"				// �û�����
#define TABLE_USER_PRIVILEGE_DATA			(LPCSTR)"user_privilege_data"		// �û�Ȩ���б�

#define TABLE_SYS_CFG           		    (LPCSTR)"t_sys_cfg"                 // ϵͳ������Ϣ(ƽ̨������������Ϣ)

//2. �ֶ���
// �������(service_domain)
//#define SERVICE_DOMAIN_MOID				    (LPCSTR)"service_domain_moid"       //MOID
//#define SERVICE_SELFBUILT_PLATFORM          (LPCSTR)"self_built_platform"       //�Ƿ��Խ�ƽ̨����������ޣ�

// ƽ̨���(platform_domain)
#define PLATFOEM_DOMAIN_MOID				"platform_domain_moid"      //MOID
#define PLATFOEM_DOMAIN_NAME				(LPCSTR)"platform_domain_name"      //����
#define PLATFOEM_DOMAIN_SERVICEDOMAIN_MOID  (LPCSTR)"service_domain_moid"       //����������
#define PLATFORM_DOMAIN_DISASTER_BACKUP     "disaster_backup"           //�ֱ����ã�0����رգ�1�����ֶ���2�����Զ���
#define PLATFORM_DOMAIN_USE_BACKUP_PLATFORM "use_backup_platform"       //�Ƿ�ʹ�ñ�ƽ̨��1�����ǣ�0�����

//platform_domain_disaster���ֱ����ù�ϵ��
#define TABLE_PLATFORM_DOMAIN_DISASTER      "platform_domain_disaster"  
#define BACKUP_PLATFORM_DOMAIN_MOID         "backup_platform_domain_moid"

// �û����(user_domain)
#define USERDOMAIN_MOID					    (LPCSTR)"user_domain_moid"      //MOID
#define USERDOMAIN_NAME				        (LPCSTR)"user_domain_name"      //����
#define USERDOMAIN_SERVICEDOMAIN_MOID       (LPCSTR)"service_domain_moid"   //����������
#define USERDOMAIN_PLATFOEMDOMAIN_MOID		(LPCSTR)"platform_domain_moid"  //����ע���ƽ̨��
#define USERDOMAIN_USED_FLAG                (LPCSTR)"used_flag"             //����ͣ�ñ�ʶ
#define USERDOMAIN_MAXNUM_CONF              (LPCSTR)"concurrent_number"     //��󲢷������ٿ���
#define USERDOMAIN_MAXNUM_ROAM              (LPCSTR)"roam_call_count"       //��󲢷���������
#define USERDOMAIN_MAXNUM_OUT               (LPCSTR)"out_call_count"        //��󲢷���������
#define USERDOMAIN_MAXNUM_INCOMING          (LPCSTR)"incoming_call_count"   //��󲢷��������
#define USERDOMAIN_MAXNUM_PHONECALL         (LPCSTR)"concurrent_phone_call_count"//��󲢷��绰��������
#define USERDOMAIN_PHONECONF_NUMBER         (LPCSTR)"phone_conf_num"        //�绰�������
#define USERDOMAIN_MAXNUM_SATELLITE_UPLOAD  (LPCSTR)"satellite_meeting_max_number"//���ǻ�������ϴ�·��
#define USERDOMAIN_SATELLITE_MULTICAST_IP   (LPCSTR)"multicast_ip"          //�����鲥��ַ

// �û���Ȩ���б�(user_domain_privilege_data)
#define USERDOMAIN_PRIVILEGE_MOID			(LPCSTR)"user_domain_moid"	    // �û���moid
#define USERDOMAIN_PRIVILEGE_KEY			(LPCSTR)"privilege_key"	        // �û���ӵ�е�Ȩ��


// �������豸��(server_device_info)
#define SRV_DEVICE_MOID						(LPCSTR)"server_moid"               //MOID
#define SRV_DEVICE_GUID						(LPCSTR)"device_guid"               //GUID
#define SRV_DEVICE_SERVER_TYPE				(LPCSTR)"server_type"               //����������
#define SRV_DEVICE_USED_FLAG				(LPCSTR)"enable"                    //����ͣ�ñ�ʶ
#define SRV_DEVICE_USERDOMAIN_MOID          (LPCSTR)"user_domain_moid"          //�����û���
#define SRV_DEVICE_PLATFORMDOMAIN_MOID      (LPCSTR)"platform_domain_moid"      //����ƽ̨��
#define SRV_DEVICE_IPADDR					(LPCSTR)"device_ip"                 //IP
#define SRV_DEVICE_NET_DOMAIN_NAME			(LPCSTR)"network_domain"            //��������
#define SRV_DEVICE_OEM_MARK					(LPCSTR)"oem_remark"	            //SUS OEM��ʶ


// �û���Ϣ��(user_info)
#define USER_MOID							(LPCSTR)"moid"	              //�˺�ȫ��ΨһID           	         
#define USER_E164_NO						(LPCSTR)"e164"	              //E164�� 
#define USER_ACCOUNT						(LPCSTR)"account"	          //�û��Զ����˺� 
#define USER_MOBILE						    (LPCSTR)"mobile"	          //�ֻ��� 
#define USER_JID							(LPCSTR)"jid"                 //xmpp�ʺ�	    
#define USER_EMAIL							(LPCSTR)"email"	              //ע������
#define USER_PASSWORD						(LPCSTR)"password"            //�˺ż��ܺ�����
#define USER_USERDOMAIN_MOID			    (LPCSTR)"user_domain_moid"	  //�����û���ID
#define USER_BINDED							(LPCSTR)"binded"              //�Ƿ�󶨱�־	    
#define USER_USED_FLAG						(LPCSTR)"enable"              //�˺����ý��ñ�־	    
#define USER_PAS_MOID						(LPCSTR)"nu_server_id"        //�û����ȵ�¼��PAS    

// �û�����(user_profile)
#define USER_PROFILE_MOID					(LPCSTR)"moid"	              //�˺�ȫ��ΨһID           	         
#define USER_PROFILE_RESTRICT				(LPCSTR)"restrict_used_on"	  //��½���� 

// �û�Ȩ���б�(user_privilege_data)
#define USER_PRIVILEGE_MOID			        (LPCSTR)"moid"			       // �û�moid
#define USER_PRIVILEGE_KEY					(LPCSTR)"privilege_key"	       // �û�ӵ�е�Ȩ��


//ϵͳ������Ϣ(t_sys_cfg)
#define SYSCFG_KEY					        (LPCSTR)"cfg_key"               //key	    
#define SYSCFG_VALUE						(LPCSTR)"cfg_value"             //value   

#define SYSCFG_SYSTEM_MODE                  (LPCSTR)"system.mode"           //ϵͳģʽ (1-�Խ���0-����)

/************************************************************************/
/*							IP���ݿ�                   
/************************************************************************/

#define TABLE_CITY_IP				        (LPCSTR)"city_ip"				// ����IP��ַ�ĳ��й���
#define TABLE_COUNTRY_IP			        (LPCSTR)"country_ip"		    // IP��ַ��������
#define TABLE_CODE_COUNTRY			        (LPCSTR)"code_country"		    // �����ַ�����

// ��ַ�ĳ��й���(city_ip)
#define CITYIP_START_IP					    (LPCSTR)"start_ip"	            //��ʼIP           	         
#define CITYIP_END_IP				        (LPCSTR)"end_ip"	            //����IP  
#define CITYIP_PROVINCE_NAME				(LPCSTR)"province"	            //����ʡ  
#define CITYIP_CITY_NAME				    (LPCSTR)"city"	            //����ʡ  
#define CITYIP_ISP				            (LPCSTR)"isp"	                //������Ӫ��

// ��ַ��������(country_ip)
#define COUNTRYIP_START_IP					(LPCSTR)"start_ip"	            //��ʼIP           	         
#define COUNTRYIP_END_IP				    (LPCSTR)"end_ip"	            //����IP  
#define COUNTRYIP_CODE2				        (LPCSTR)"code2"	                //���ҵ�2λ����

// �����ַ�����(code_country)
#define CODECOUNTRY_CODE2					(LPCSTR)"c2code"	            //���ҵ�2λ����          	         
#define CODECOUNTRY_CHN_NAME				(LPCSTR)"country_cn"	        //���Ĺ����� 
#define CODECOUNTRY_ENG_NAME				(LPCSTR)"country_en"	        //Ӣ�Ĺ����� 

#endif //end of _DBTABLE_DEFINE_ULS_H_
