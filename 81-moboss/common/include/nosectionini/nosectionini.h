
/*=======================================================================
ģ����      : 
�ļ���      : nosectionini.h
����ļ�    : 
�ļ�ʵ�ֹ���: ��û��[setion]�������ļ����������ù���

����        : 
�汾        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/12/10  1.0                     ����
=======================================================================*/

#ifndef _NO_SECTION_INI_H_
#define _NO_SECTION_INI_H_

#pragma warning( disable : 4786 )

#include "kdvtype.h"
#include <map>
#include <string.h>
#include <string>
#include <stdlib.h>

using namespace std;


#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif


// �ļ�·������󳤶�
#ifndef MAXLEN_FILE_PATH_NOSECTION
#define MAXLEN_FILE_PATH_NOSECTION                (u16)512
#endif


#ifdef  NOSECTION_EXPORT
#define NoSetionIniAPI __declspec( dllexport )
#else
#define NoSetionIniAPI
#endif


#define LOG_LEVEL_CRITICAL			(u8)0
#define LOG_LEVEL_ERROR				(u8)1
#define LOG_LEVEL_WARNING			(u8)2
#define LOG_LEVEL_INFO				(u8)3
#define LOG_LEVEL_DETAIL			(u8)4

typedef void (*PrintLogCallBack)( s32 nPrtLevel, const s8* fmt, ... );


class NoSetionIniAPI CNoSectionIni
{
public:
    CNoSectionIni();
    CNoSectionIni(IN const s8* szCfgFilePath, IN BOOL32 bCreate = FALSE);
    ~CNoSectionIni();

public:
    // �������ļ�
    BOOL32 OpenCfgFile(IN const s8* szCfgFilePath, IN BOOL32 bCreate = FALSE);

    //��ȡ�ļ�·��
    const s8* GetCfgFilePath() const;

    // �������ļ��ж�ȡһ�ַ���ֵ
    BOOL32 GetKeyString(IN const s8* szKeyName, OUT s8* szReturnBuf, IN u32 dwBufSize);
    BOOL32 GetKeyString(IN const s8* szKeyName, OUT std::string& strReturn);

    // �������ļ��ж�ȡһ������ֵ
    BOOL32 GetKeyInt(IN const s8* szKeyName, OUT s32 &nReturnValue);

    // �������ļ���д��һ�µ��ַ����͵�ֵ��������������Զ�����; ���szNewValue==NULL���ʾɾ������
    BOOL32 SetKeyString(IN const s8* szKeyName, IN const s8* szNewValue, PrintLogCallBack funPrintCallBack = NULL);

    // �������ļ���д��һ�µ��ַ����͵�ֵ��������������Զ�����
    BOOL32 SetKeyInt(IN const s8* szKeyName, IN s32 nNewValue, PrintLogCallBack funPrintCallBack = NULL);

    // �������ļ���ɾ��һ��Key
    BOOL32 DeleteKey(IN const s8* szKeyName);

    // ��ȡ���е�Key Value
    BOOL32 GetAllKeyValue(OUT std::map<std::string, std::string>& mapKeyValue, PrintLogCallBack funPrintCallBack = NULL);

private:
    //��������·���µ��ļ���ͬʱ�Զ�������Ҫ��Ŀ¼
    BOOL32 CreatFile(const s8* szFileFullPath) const;

    //����Ŀ¼
    BOOL32 CreatDir(const s8* szDirPath) const;

    //��"szKeyName=szNewValue" д���ļ��ĵ�ǰ��
    BOOL32 WriteKeyValueString(const s8*szKeyName, const s8*szNewValue, u32 dwOffsetCurLine, u32 dwOffsetNextLine, PrintLogCallBack funPrintCallBack = NULL);
    
private:    
    s8 m_szCfgFilePath[MAXLEN_FILE_PATH_NOSECTION+1];
    FILE* m_pFile;
};

NoSetionIniAPI string FilterSpace(const s8* szOriginal);




#endif // _NO_SECTION_INI_H_
// END FILE



