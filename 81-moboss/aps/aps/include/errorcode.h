
/*****************************************************************************
   ģ����      : XAP
   �ļ���      : 
   ����ļ�    : 
   �ļ�ʵ�ֹ���: XAP���ն�ͨ�ŵĴ���������
   ����        : 
   �汾        : V1.0  Copyright(C) 2006-2010 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���        �޸�����
   2013/08/06   1.0
******************************************************************************/
#ifndef _APS_ERROR_CODE_H_
#define _APS_ERROR_CODE_H_

/*------------------------------------------------------------------------------
������
------------------------------------------------------------------------------*/

#define APS_ERRORCODE_BGN_KDVP                      (u32)(20000)

#define APS_ERR_CONNECT_FAILED                      (APS_ERRORCODE_BGN_KDVP + 1000)     //����ʧ��

#define APS_ERR_LOGIN_PARAM_ERROR                   (APS_ERRORCODE_BGN_KDVP + 2000)     //��½��������
#define APS_ERR_ACCOUNT_NAME_NOTEXIST               (APS_ERRORCODE_BGN_KDVP + 2001)     //�û�������
#define APS_ERR_ACCOUNT_PWD_ERROR                   (APS_ERRORCODE_BGN_KDVP + 2002)     //�û��������
#define APS_ERR_DEVTYPE_RESTRICT                    (APS_ERRORCODE_BGN_KDVP + 2003)     //�豸��������
#define APS_ERR_NOTEXIST                            (APS_ERRORCODE_BGN_KDVP + 2004)     //��¼������
#define APS_ERR_ASSIGN_SRV_ERROR                    (APS_ERRORCODE_BGN_KDVP + 2005)     //�������������
#define APS_ERR_NO_ROAM_PRIVILEGE                   (APS_ERRORCODE_BGN_KDVP + 2006)     //û������Ȩ��

#define APS_ERR_SERVER_INSIDE_ERR			        (APS_ERRORCODE_BGN_KDVP + 3001)     //�������ڲ�����
#define APS_ERR_DB_EXEC_FAIL				        (APS_ERRORCODE_BGN_KDVP + 3002)     //���ݿ�ִ��ʧ��
//5.1.sp2���������ն˲�֧��
#define APS_ERR_REJECT_LOGIN				        (APS_ERRORCODE_BGN_KDVP + 3003)     //ƽ̨�ܾ���¼



#define APS_ONE_TEXT(errcode,ChnStr)                        \
case errcode:                                               \
    return ChnStr;                                          \
	
static const s8* GetApsErrorStr(u32 wErrorID)
{   
    switch ( wErrorID )
    {
        APS_ONE_TEXT( APS_ERR_CONNECT_FAILED, "connect failed!");

        APS_ONE_TEXT( APS_ERR_LOGIN_PARAM_ERROR, "login param error!");
        APS_ONE_TEXT( APS_ERR_ACCOUNT_NAME_NOTEXIST, "account name notexist!");
        APS_ONE_TEXT( APS_ERR_ACCOUNT_PWD_ERROR, "pwd error!" );
        APS_ONE_TEXT( APS_ERR_DEVTYPE_RESTRICT, "devicetype restrict!" );
        APS_ONE_TEXT( APS_ERR_NOTEXIST, "recorder not exist!" );
        APS_ONE_TEXT( APS_ERR_ASSIGN_SRV_ERROR, "assign srv error!" );
        APS_ONE_TEXT( APS_ERR_NO_ROAM_PRIVILEGE, "no roam privilege!" );
		
        APS_ONE_TEXT( APS_ERR_SERVER_INSIDE_ERR, "server inside error!" );
        APS_ONE_TEXT( APS_ERR_DB_EXEC_FAIL, "db exec fail!" ); 
        APS_ONE_TEXT( APS_ERR_REJECT_LOGIN, "reject mt login!" );

    default:
        {
            return "unknown error";
        }
    }
}

#endif //end of _APS_ERROR_CODE_H_
