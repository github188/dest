/*=======================================================================
ģ����      :
�ļ���      : readlicense.h
����ļ�    :
�ļ�ʵ�ֹ��� : ��ȡlicense�ļ�������
����        : ���ƶ�
�汾        : KCMV1.0  Copyright(C) 2003-2008 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2007/01/09  KCM1.0      ���ƶ�      ����
2009/04/01  kdv1.0      zhanghb	    Add  writelicense Interface for 8000E
=======================================================================*/
#ifndef _READLICENSE_H_
#define _READLICENSE_H_

#include <string.h>

///////////////////////////////////////////////////////////////////
//	Linux ����ϵͳͷ�ļ�
///////////////////////////////////////////////////////////////////
#if defined(_LINUX_)
#ifdef PWLIB_SUPPORT
#include <ptlib.h>
#endif

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include <malloc.h>

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/times.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <termios.h>
#include <signal.h>
#include "syslog.h"

#endif //_LINUX_


#ifdef _MSC_VER

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#ifdef WINVER
#undef WINVER
#endif

/* vc is vs2005 and later */
#if (_MSC_VER >= 1400)
#define _WIN32_WINNT 0x0501 //support only winxp and above
#define WINVER 0x0501 //support only winxp and above
#else
#define _WIN32_WINNT 0x400
#define WINVER 0x0400
#endif

#include <malloc.h>
#ifdef _AFXDLL
#include <afx.h>
#include <afxwin.h>
#include <winsock2.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#endif

#include <assert.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib, "User32.lib")

#endif	// _MSC_VER

#include "kdvtype.h"

#define  KDC_CALL	extern "C"

typedef struct tagKey_Value 
{
	tagKey_Value()
	{
		memset(achKey, 0, sizeof(achKey));
		memset(achValue, 0, sizeof(achValue));
	}
	s8 achKey[1024];
	s8 achValue[1024];
}TKeyValue;

/*========================================================================
�� �� ����ReadLicenseFile
��    �ܣ���ȡLicense�ļ�
��    ����s8 *pchPath					[in]License�ļ���(ȫ·����������'\0'��β)
		  
�� �� ֵ��u16 ������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2007/01/09	KCM1.0	���ƶ�	����
========================================================================*/
KDC_CALL u16 ReadLicenseFile(const s8 *pchPath);

/*========================================================================
�� �� ����WriteLicenseFile
��    �ܣ�дLicense�ļ�
��    ����BOOL32 bToUsb = FALSE             [in]    �Ƿ�д��USB�豸
�� �� ֵ��u16 ������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2007/01/09	KCM1.0	���ƶ�	����
2007/06/12  1.0     ���    ����д��USB�豸
========================================================================*/
KDC_CALL u16 WriteLicenseFile(BOOL32 bToUsb = FALSE);

/*========================================================================
�� �� ����GetKeyValueNum
��    �ܣ���ȡLicense�ļ����ж���Key��valueֵ��
��    ����
�� �� ֵ��Key��valueֵ����
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2007/01/23	KCM1.0	���ƶ�	����
========================================================================*/
KDC_CALL u16 GetKeyValueNum();
/*========================================================================
�� �� ����EnumKeyList
��    �ܣ�ö��License�ļ����ж���Keyֵ
��    ����TKeyValue *pchKeyValueList	[out]Key-Valueֵ�б�
		  s32 &nListLen					[in/out]Key-Valueֵ�б�ռ��С/Key-Valueֵ��
�� �� ֵ��u16 ������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2007/01/09	KCM1.0	���ƶ�	����
========================================================================*/
KDC_CALL u16 EnumKeyList(TKeyValue *pchKeyValueList, s32 &nListLen);
/*========================================================================
�� �� ����GetValue
��    �ܣ�����Keyֵ��ȡ��Ӧ��ֵ
��    ����s8 *pchKey					[in]Keyֵ
		  s8 *pchValue					[out]ֵ
		  s32 &nValueLen				[in/out]ֵ��Ч����
�� �� ֵ��u16 ������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2007/01/09	KCM1.0	���ƶ�	����
========================================================================*/
KDC_CALL u16 GetValue(const s8 *pchKey, s8 *pchValue, s32 &nValueLen);
/*========================================================================
�� �� ����GetErrorCodeDescription
��    �ܣ���ȡ���ò����Ĵ����������
��    ����u16 wErrorCode				[in]������
		  s8 *pchDescription			[out]����������
		  s32 &nLen						[in/out]������������Ч����
		  s8 *pchIniFilePath			[in]�����������ļ�ȫ·��(�����ļ���)������'\0'��β
�� �� ֵ��BOOL32 TRUE : �ɹ�   FALSE : ʧ��
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2007/01/09	KCM1.0	���ƶ�	����
========================================================================*/
KDC_CALL BOOL32 GetErrorCodeDescription(u16 wErrorCode, s8 *pchDescription, s32 &nLen, s8 *pchIniFilePath);

/*=============================================================================
�� �� ��: SetValue
��    ��: ���ڴ���������
��    ��: LPCSTR lpszKey                    [in]    ����
          LPCSTR lpszValue                  [in]    ��ֵ
ע    ��: ��ˢ�¶�ȡ���������ڴ�
�� �� ֵ: ������
-------------------------------------------------------------------------------
�޸ļ�¼:
��      ��  �汾    �޸���  �޸�����
2007/06/12  1.0     ���    ����
=============================================================================*/
KDC_CALL u16 SetValue(LPCSTR lpszKey, LPCSTR lpszValue);
/*=============================================================================
�� �� ��: GenEncodeData
��    ��: ����
��    ��: s8* ptrInBuf,                  [in]    ���ӽ�������
          u16 wInBufLen                  [in]    ���ݳ���(���ֽڼ���)
          BOOL32 bDirect                 [in]    ����: TRUE; ����: FALSE
          s8* ptrOutBuf                  [out]   �ӽ��ܺ������
ע    ��: ��
�� �� ֵ: ������
-------------------------------------------------------------------------------
�޸ļ�¼:
��      ��  �汾    �޸���  �޸�����
2009/04/02  1.0     zhanghb    ����
=============================================================================*/
KDC_CALL void GenEncodeData(s8* ptrInBuf, u16 wInBufLen, BOOL32 bDirect,s8* ptrOutBuf);
/*=============================================================================
�� �� ��: GetKeyValue
��    ��: �õ�key��value
��    ��: 
ע    ��: ��
�� �� ֵ: ������
-------------------------------------------------------------------------------
�޸ļ�¼:
��      ��  �汾    �޸���  �޸�����
2009/04/02  1.0     zhanghb    ����
=============================================================================*/
u16 GetKeyValue(s8 *pchBuf, s8 *pchKey, s32 &nKeyLen, s8 *pchValue, s32 &nValueLen);
/*=============================================================================
�� �� ��: CreateLicenseFile
��    ��: ����license�ļ�
��    ��: 
ע    ��: 
�� �� ֵ: ������
-------------------------------------------------------------------------------
�޸ļ�¼:
��      ��  �汾    �޸���  �޸�����
2009/04/02  1.0     zhanghb    ����
=============================================================================*/

#ifdef WIN32
KDC_CALL u16 CreateLicenseFile( BSTR pbyAuthCode,BSTR pbyLicenseKey,BSTR pbyLicenseContent);
#endif


#endif // _READLICENSE_H_