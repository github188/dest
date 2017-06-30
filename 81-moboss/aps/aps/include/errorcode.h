
/*****************************************************************************
   模块名      : XAP
   文件名      : 
   相关文件    : 
   文件实现功能: XAP与终端通信的错误码描述
   作者        : 
   版本        : V1.0  Copyright(C) 2006-2010 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人        修改内容
   2013/08/06   1.0
******************************************************************************/
#ifndef _APS_ERROR_CODE_H_
#define _APS_ERROR_CODE_H_

/*------------------------------------------------------------------------------
错误码
------------------------------------------------------------------------------*/

#define APS_ERRORCODE_BGN_KDVP                      (u32)(20000)

#define APS_ERR_CONNECT_FAILED                      (APS_ERRORCODE_BGN_KDVP + 1000)     //连接失败

#define APS_ERR_LOGIN_PARAM_ERROR                   (APS_ERRORCODE_BGN_KDVP + 2000)     //登陆参数错误
#define APS_ERR_ACCOUNT_NAME_NOTEXIST               (APS_ERRORCODE_BGN_KDVP + 2001)     //用户不存在
#define APS_ERR_ACCOUNT_PWD_ERROR                   (APS_ERRORCODE_BGN_KDVP + 2002)     //用户密码错误
#define APS_ERR_DEVTYPE_RESTRICT                    (APS_ERRORCODE_BGN_KDVP + 2003)     //设备类型限制
#define APS_ERR_NOTEXIST                            (APS_ERRORCODE_BGN_KDVP + 2004)     //记录不存在
#define APS_ERR_ASSIGN_SRV_ERROR                    (APS_ERRORCODE_BGN_KDVP + 2005)     //分配服务器错误
#define APS_ERR_NO_ROAM_PRIVILEGE                   (APS_ERRORCODE_BGN_KDVP + 2006)     //没有漫游权限

#define APS_ERR_SERVER_INSIDE_ERR			        (APS_ERRORCODE_BGN_KDVP + 3001)     //服务器内部错误
#define APS_ERR_DB_EXEC_FAIL				        (APS_ERRORCODE_BGN_KDVP + 3002)     //数据库执行失败
//5.1.sp2新增，老终端不支持
#define APS_ERR_REJECT_LOGIN				        (APS_ERRORCODE_BGN_KDVP + 3003)     //平台拒绝登录



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
