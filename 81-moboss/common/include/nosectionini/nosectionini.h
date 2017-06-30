
/*=======================================================================
模块名      : 
文件名      : nosectionini.h
相关文件    : 
文件实现功能: 对没有[setion]的配置文件，进行配置工作

作者        : 
版本        : V1.0  Copyright(C) 2005-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/12/10  1.0                     创建
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


// 文件路径的最大长度
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
    // 打开配置文件
    BOOL32 OpenCfgFile(IN const s8* szCfgFilePath, IN BOOL32 bCreate = FALSE);

    //获取文件路径
    const s8* GetCfgFilePath() const;

    // 从配置文件中读取一字符型值
    BOOL32 GetKeyString(IN const s8* szKeyName, OUT s8* szReturnBuf, IN u32 dwBufSize);
    BOOL32 GetKeyString(IN const s8* szKeyName, OUT std::string& strReturn);

    // 从配置文件中读取一整数型值
    BOOL32 GetKeyInt(IN const s8* szKeyName, OUT s32 &nReturnValue);

    // 往配置文件中写入一新的字符串型的值，如果不存在则自动创建; 如果szNewValue==NULL则表示删除改行
    BOOL32 SetKeyString(IN const s8* szKeyName, IN const s8* szNewValue, PrintLogCallBack funPrintCallBack = NULL);

    // 往配置文件中写入一新的字符串型的值，如果不存在则自动创建
    BOOL32 SetKeyInt(IN const s8* szKeyName, IN s32 nNewValue, PrintLogCallBack funPrintCallBack = NULL);

    // 从配置文件中删除一行Key
    BOOL32 DeleteKey(IN const s8* szKeyName);

    // 获取所有的Key Value
    BOOL32 GetAllKeyValue(OUT std::map<std::string, std::string>& mapKeyValue, PrintLogCallBack funPrintCallBack = NULL);

private:
    //创建任意路径下的文件，同时自动创建需要的目录
    BOOL32 CreatFile(const s8* szFileFullPath) const;

    //创建目录
    BOOL32 CreatDir(const s8* szDirPath) const;

    //将"szKeyName=szNewValue" 写入文件的当前行
    BOOL32 WriteKeyValueString(const s8*szKeyName, const s8*szNewValue, u32 dwOffsetCurLine, u32 dwOffsetNextLine, PrintLogCallBack funPrintCallBack = NULL);
    
private:    
    s8 m_szCfgFilePath[MAXLEN_FILE_PATH_NOSECTION+1];
    FILE* m_pFile;
};

NoSetionIniAPI string FilterSpace(const s8* szOriginal);




#endif // _NO_SECTION_INI_H_
// END FILE



