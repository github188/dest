
#include <sys/stat.h>
#include <assert.h>
#include <time.h>
#include <errno.h>

#ifdef WIN32         
#include <windows.h>       
#include <Tlhelp32.h>
#include <direct.h>

#ifndef PATH_MAX
#define PATH_MAX  MAX_PATH
#endif

#endif	// end WIN32

#ifdef _LINUX_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/resource.h>
#endif // end _LINUX_

#include "bossutils.h"





namespace BossUtils{

std::string GetIpString( u32 dwIp )
{	
	dwIp = htonl(dwIp);
	
    s8 achStrIP[32+1] = {0};  
	u8 *p = (u8 *)&dwIp;
	snprintf(achStrIP, sizeof(achStrIP), "%u.%u.%u.%u", p[0],p[1],p[2],p[3]);
	achStrIP[ sizeof(achStrIP) -1 ] = '\0';
	
	return achStrIP;	
}

u32 GetIpByString( const s8* achIp )
{
	u32 dwIp = inet_addr(achIp);
	dwIp = ntohl(dwIp);

	return dwIp;
}

//tcp/ip协议中，专门保留了三个IP地址区域作为私有地址，其地址范围如下：        
//10.0.0.0/8：10.0.0.0～10.255.255.255
//172.16.0.0/12：172.16.0.0～172.31.255.255
//192.168.0.0/16：192.168.0.0～192.168.255.255
BOOL32 IsLANIp(u32 dwIp)
{
    u32 dwBeginIp = GetIpByString("10.0.0.0");
    u32 dwEndIp = GetIpByString("10.255.255.255");
    if (dwIp >= dwBeginIp && dwIp <= dwEndIp)
    {
        return TRUE;
    }

    dwBeginIp = GetIpByString("172.16.0.0");
    dwEndIp = GetIpByString("172.31.255.255");
    if (dwIp >= dwBeginIp && dwIp <= dwEndIp)
    {
        return TRUE;
    }

    dwBeginIp = GetIpByString("192.168.0.0");
    dwEndIp = GetIpByString("192.168.255.255");
    if (dwIp >= dwBeginIp && dwIp <= dwEndIp)
    {
        return TRUE;
    }

    return FALSE;
}

u8 GetIpMaskLength( u32 dwIpMask )
{    
    u8 byLength = 0;
    while( 0 != (dwIpMask & 0x80000000) )
    {        
        byLength++;        
        dwIpMask = dwIpMask << 1;
    }
    
    return byLength;  
}

BOOL32 IsValidIpV4(u32 dwIP)
{
    std::string strIP = GetIpString(dwIP);
    return IsValidIpV4(strIP);
}

BOOL32 IsValidIpV4(const s8* lptrIPStr)
{
    if ( lptrIPStr == NULL ) 
    {
        return FALSE;
    }
	
    s32 nDot    = 0;                        //  已处理的字符串里的点号数
    s32 nData   = 0;                        //  已处理的字符串里的数值
    s32 nPos	= 0;						//	两点之间的间隔
    s32 nLen    = strlen( lptrIPStr );     //  字符串长度
	
    //  字符串最少7位最长15位, 且不能以'0'开头
    if ( nLen < 7 || nLen > 15 ) 
    {
        // 2004/05/19 李洪强 *.*.*.*和***.***.***.***
        return FALSE;
    }
	
    if ( lptrIPStr[0] == '0'
		|| ( lptrIPStr[0] == '2' && lptrIPStr[1] == '5'
		&& lptrIPStr[2] == '5' ) )
    {
        return FALSE;
    }
	
    if ( lptrIPStr[0] == '.' || lptrIPStr[nLen - 1] == '.' )
    {
        return FALSE;
    }
	
    for ( s32 nCount = 0; nCount < nLen; ++nCount )
    {
        if ( lptrIPStr[nCount] >= '0' && lptrIPStr[nCount] <= '9' )//遇到数字
        {
            nData = 10 * nData + lptrIPStr[nCount] - '0';     //计算数值
			++nPos;
        }
        else if ( lptrIPStr[nCount] == '.' )     //  遇到点号
        {
            nDot++;
            if ( nDot > 3 )  //  点号数大于3
            {
				return FALSE;
            }
			else if ( nPos > 3 )
			{
				return FALSE;
			}
			else if ( nData > 255 ) //数值大于255
			{
				return FALSE;
			}
            else        //  已处理的字符串合法
            {
                nData = 0;
				nPos = 0;
            }
        }
        else            //  遇到非法字符
        {
			return FALSE;
        }
    }
	
    if ( nDot != 3 )
    {
		return FALSE;
    }
    
    // 顾振华@2006.03.17 检查最后一个数字是否合法
    if ( nData > 255 ) //数值大于255
	{
		return FALSE;
	}    
	
    return TRUE;
}

BOOL32 IsValidIpV4(const std::string &csIP)
{
	return IsValidIpV4( csIP.c_str() );
}

BOOL32 IsValidIpMask(u32 dwIpMask)
{
	if ( dwIpMask == 0 )
	{
		return FALSE;
	}
	
	u32 dwTemp1 = ~dwIpMask;
	u32 dwTemp2 = dwIpMask - dwTemp1;
	return ( (dwTemp1 & dwTemp2) == 1 );
}

BOOL32 IsValidIpMask(const s8* lptrIpMask)
{
	BOOL32 bRet = IsValidIpV4( lptrIpMask );
	if ( !bRet )
	{
		return FALSE;
	}
	
	u32 dwAddr = ntohl( inet_addr( lptrIpMask ) ); 
	bRet = IsValidIpMask( dwAddr );
	return bRet;
}

BOOL32 IsValidIpMask(const std::string lptrIpMask)
{
	return IsValidIpMask( lptrIpMask.c_str() );
}




time_t GetSysTime( void )
{
    time_t rawtime;
    time ( &rawtime );

    return rawtime;        
}

std::string GetSysTimeString( void )
{
    time_t rawtime;
    time ( &rawtime );

    struct tm * timeinfo;
    timeinfo = localtime ( &rawtime );
    
    return asctime(timeinfo);
}

std::string TimeToString( time_t dwTime)
{
    struct tm * timeinfo;
    timeinfo = localtime ( &dwTime );

    return asctime(timeinfo);
}

/*=============================================================================
函 数 名： CreatDir
功    能： 创建目录(递归创建)
算法实现： 
全局变量： 
参    数： const s8* lpszDirPath
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2007/9/15   4.0			周广程                  创建
=============================================================================*/
BOOL32 CreatDir(const s8* lpszDirPath)
{
    if ( NULL == lpszDirPath )
    {
        return FALSE;
    }

    BOOL32 bSuccess = TRUE;

	s8 achDirPath[PATH_MAX+1] = {0};
	strncpy( achDirPath, lpszDirPath, sizeof(achDirPath)-1 );
	achDirPath[sizeof(achDirPath)-1] = '\0';
    
    u32 dwLen = strlen( achDirPath );
    for ( u32 dwIdx = 0; dwIdx < dwLen; dwIdx++ )
    {
        if ( achDirPath[dwIdx] == '\\' )
        {
            achDirPath[dwIdx] = '/';
        }
    }

#ifdef _LINUX_
    s32 nRet = 0;
    s8 ch = '/';
    s8 *pchDest = NULL;
    pchDest = strrchr( achDirPath, ch );
    DIR* pdir = opendir(achDirPath);
    if ( pdir != NULL )
    {
        closedir(pdir);
        chmod(achDirPath, 0777);
        return TRUE;
    }
    
    if ( pchDest == NULL || pchDest == achDirPath )
    {
        nRet = mkdir( achDirPath, S_IRWXU|S_IRWXG|S_IRWXO );
        if( 0 != nRet )
        {
            printf("make diretory<%s> Error :%s\n",achDirPath,strerror(errno)); 
            return FALSE;
        }
    }
    else
    {
        *pchDest = '\0';
        if ( CreatDir(achDirPath) )
        {
            nRet = mkdir( lpszDirPath, S_IRWXU|S_IRWXG|S_IRWXO );
            if( 0 != nRet )
            {
                printf("make diretory<%s> Error :%s\n",lpszDirPath,strerror(errno));  
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }		                
#endif
#ifdef WIN32
	bSuccess =  CreateDirectory( lpszDirPath, NULL );
    DWORD lastError = GetLastError();
    if ( ERROR_ALREADY_EXISTS == lastError )
    {
        return TRUE;
    }
	// 获取上级目录
	s8 ch = '/';
    s8* pchDest = strrchr( achDirPath, ch );
    if ( NULL != pchDest )
    {
        *pchDest = '\0';
    }

	// 如果上级目录不存在,递归创建
	bSuccess =  CreateDirectory( achDirPath, NULL );
	lastError = GetLastError();
	if ( ERROR_ALREADY_EXISTS != lastError )
	{
		CreatDir( achDirPath );
	} 

	bSuccess =  CreateDirectory( lpszDirPath, NULL );
    lastError = GetLastError();
    if ( ERROR_ALREADY_EXISTS == lastError )
    {
        bSuccess = TRUE;
    }
#endif
    return bSuccess;
}

/*=============================================================================
函 数 名： CreatFile
功    能： 创建任意路径下的文件，同时自动创建需要的目录
           如文件已存在，则直接返回TURE
算法实现： 
全局变量： 
参    数： const s8 * lpszFileFullPath  // 文件全路径，含文件名
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2008/10/8   4.0		    周广程                  创建
=============================================================================*/
/*lint -save -e438*/
BOOL32 CreatFile( const s8 * lpszFileFullPath )
{
    FILE *stream = fopen( lpszFileFullPath, "r" );
    if( stream != NULL )
    {
        fclose(stream);
        return TRUE;
    }
    
    s8 achDirPath[PATH_MAX +1] = {0};
    strncpy( achDirPath, lpszFileFullPath, sizeof(achDirPath)-1 );
	achDirPath[sizeof(achDirPath)-1] = '\0';

    u32 dwLen = strlen( achDirPath );
    for ( u32 dwIdx = 0; dwIdx < dwLen; dwIdx++ )
    {
        if ( achDirPath[dwIdx] == '\\' )
        {
            achDirPath[dwIdx] = '/';
        }
    }
    
    //获得文件目录地址
	s8 ch = '/';
    s8* pchDest = strrchr( achDirPath, ch );
    if ( NULL != pchDest )
    {
        *pchDest = '\0';
    }
    BOOL32 bCreat = CreatDir( achDirPath );// 递归创建目录
    if ( !bCreat ) 
    {
        return FALSE;
    }
    
    stream = fopen( lpszFileFullPath, "w" );
    if ( stream == NULL )
    {
        return FALSE;
    }
    fclose(stream);
    stream = NULL;
    
    return TRUE;
}
/*lint -restore*/
/*=============================================================================
函 数 名： IsProcessRunned
功    能： 进程是否已经被启动
算法实现： 
全局变量： 
参    数： const s8 * pszApplicationName
返 回 值： BOOL32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2010/7/12   4.0			周广程                  创建
=============================================================================*/
BOOL32 IsProcessRunned( const s8 * pszApplicationName )
{
    BOOL32 bFind = FALSE;

    u32 dwPID = BossUtils::GetCurrentProcessID();
    
#ifdef _LINUX_   
    DIR *dp = opendir("/proc");
    if ( NULL == dp )
    {
        return FALSE;
    }
    
    s8 sProcStatFile[64];
    s8 sReadBuf[512];
    s8 sProcName[64];
    s8 achSeps[] = " ";
    struct dirent * dirp = NULL;         
    while( ( dirp=readdir(dp) ) != NULL ) // 搜索所有的进程ID  
    {  
        memset(sProcStatFile,0,sizeof(sProcStatFile));  
        memset(sReadBuf,0,sizeof(sReadBuf));       
        if( !isdigit(dirp->d_name[0]) ) // 一个进程号对应一个在/proc目录下一个相同数字的目录
        {
            continue;
        }
        
        snprintf( sProcStatFile, sizeof(sProcStatFile), "/proc/%s/stat", dirp->d_name );
		sProcStatFile[sizeof(sProcStatFile)-1] = '\0';

        FILE * fp = fopen((const s8*)sProcStatFile,"r"); // 从/Proc/<进程文件夹>/stat文件中获取文件名
		if( NULL == fp )
		{
			continue;
		}
        
        fgets( (s8*)sReadBuf,sizeof(sReadBuf)-1,fp );  
        fclose(fp);  
        fp = NULL;
        if(strlen((const s8*)sReadBuf)==0)
        {
            continue; 
        }		
        
        s8 * pPPid = NULL;
        pPPid = strtok( (s8*)sReadBuf, (const s8*)achSeps );    // 进程ID  
        if ( NULL == pPPid ) continue;

        pid_t ppid = atoi(pPPid);                               // 同本进程，跳过
        if( ppid == dwPID )
        {
           continue;
	    }

        pPPid = strtok( NULL, (const s8*)achSeps );             // 进程名
        if ( NULL == pPPid ) continue;
        
        // 进程名在文件中以(xxx)的形式存在，需要把括号去掉
        memset( sProcName, 0, sizeof(sProcName) );
        strncpy( sProcName, pPPid+1, sizeof(sProcName)-1 );
        *(sProcName+strlen(sProcName)-1) = '\0';
        
        if ( strcmp( sProcName, pszApplicationName ) == 0 )
        {
            bFind = TRUE;
            break;
        }
    }
    closedir(dp);
#endif
#ifdef WIN32
    void * hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );  
    PROCESSENTRY32 tPPE;  
    ZeroMemory( &tPPE, sizeof(PROCESSENTRY32) );
    tPPE.dwSize = sizeof(PROCESSENTRY32); 
    
    BOOL32 bRet = Process32First( hSnapshot, &tPPE ); 
    while( bRet ) 
    {  
        if ( dwPID == tPPE.th32ProcessID )
        {
			bRet = Process32Next( hSnapshot, &tPPE );
            continue;
        }

        if( 0 == strcmp( tPPE.szExeFile, (const char*)pszApplicationName ) )  
        {  
            bFind = TRUE;
            break;
        }  
        bRet = Process32Next( hSnapshot, &tPPE );
    }    
    
    CloseHandle(hSnapshot); 
#endif
    return bFind;
}

/*=============================================================================
函 数 名： GetCurrentProcessID
功    能： 
算法实现： 
全局变量： 
参    数： void
返 回 值： u32 
----------------------------------------------------------------------
修改记录    ：
日  期		版本		修改人		走读人    修改内容
2009/9/28   4.0			周广程                  创建
=============================================================================*/
u32 GetCurrentProcessID( void )
{
    u32 dwCurrentPID = 0;
#ifdef _LINUX_
    dwCurrentPID = getpid();
#endif
#ifdef WIN32
    dwCurrentPID = GetCurrentProcessId();
#endif
    return dwCurrentPID;
}


int system_closefd(const char * cmdstring, u32 dwTimeOut)
{
#ifdef WIN32
    return system(cmdstring);
#else   //end WIN32
    u32 dwShellStartTime = (u32)time(NULL);
    pid_t pid;
    int status = FALSE;
    if(cmdstring == NULL)
    {
        printf("[system_closefd] cmdstring == NULL!\n");
        return (1);
    }

	//错误提示no child processes的解决方法：
	//system()函数前把SIGCHLD信号处理方式显式修改为SIG_DFL方式，同时记录原来的处理方式，
	//使用完system()后再设为原来的处理方式。这样我们可以屏蔽因系统升级或信号处理方式改变带来的影响。
	sighandler_t old_handler;  
	old_handler = signal(SIGCHLD, SIG_DFL);  

    pid = fork();
    if(pid < 0)
    {
		printf("[system_closefd] fork child process fail! pid < 0!\n");
        status = -1;
    }
    else if(pid == 0)
    {
        int32_t dwMaxNumFD = sysconf(_SC_OPEN_MAX);
        struct rlimit rlim;
        int nRet = getrlimit(RLIMIT_NOFILE, &rlim);
        if (0 == nRet)
        {
            dwMaxNumFD = rlim.rlim_cur;
        }

        for (int fd = 0; fd < dwMaxNumFD; ++fd)  //跳过stdin stdout stderr
        {
            if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)  
            {
                close(fd);
            }
        }

        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);
    }
    else
    {
        pid_t pr;
        do 
        {
            //防止cpu跑满，sleep 30毫秒
            usleep(30 * 1000);

            pr = waitpid(pid, &status, WNOHANG );
            if(pr == 0)
            {
                s32 nTime = abs((s32)((u32)time(NULL) - dwShellStartTime));

                if (nTime >= (s32)dwTimeOut)
                {
					printf("[system_closefd] nTime >= (s32)dwTimeOut!\n");
					signal(SIGCHLD, old_handler);
                    kill(pid, SIGKILL);
                    //					kill(pid+1, SIGKILL);
					while( waitpid(pid, NULL, WNOHANG) == 0 )
					{
						kill(pid, SIGKILL);
						usleep(30 * 1000);
					}
                    status = -1;
                    return status;
                }
            }
        }while (pr == 0);
        if (pr == pid)
        {
        }
        else
        {
            if(errno != EINTR)
            {
				printf("[nTime >= (s32)dwTimeOut] errno != EINTR!\n");
				signal(SIGCHLD, old_handler);
				while( waitpid(pid, NULL, WNOHANG) == 0 )
				{
					kill(pid, SIGKILL);
					usleep(30 * 1000);
				}
                status = -1;
				return status;
            }
        }
    }

	signal(SIGCHLD, old_handler);

    return status;
#endif
}

/*=============================================================================
函 数 名:GetModuleFullPath
功    能:获取模块所在文件夹全路径, 包括最后的\
参    数:无
注    意:例如 C:\
返 回 值:文件夹字符串
-------------------------------------------------------------------------------
修改纪录:
日      期  版本    修改人  修改内容
2004/10/26  3.5     王昊    创建
2006/06/07  3.9		李洪强  支持LINUX平台
=============================================================================*/
std::string GetModuleFullPath()
{
	s8 achModuleName[256] = {0};
	s32 nLen = sizeof (achModuleName);
	std::string csPath = "";

	if (GetModFilePath(achModuleName, nLen)) 
	{	
		csPath = achModuleName;
#ifdef WIN32
		csPath = csPath.substr(0, csPath.find_last_of('\\') + 1);
#endif //WIN32

#ifdef _LINUX_
		s32 nPos = csPath.find_last_of('/');
		std::string csFullPath = csPath.substr(0, nPos + 1);
		csPath = csFullPath;
#endif // _LINUX_
	}

	return csPath;
}

/*========================================================================
函 数 名：GetModFilePath
功    能：得到模块的全路径，包括文件名
参    数：s8 *lpModName		[out]模块名的空间
		  s32 &nNameLen			[in/out]in模块名空间的大小,out实际的模块名长度
返 回 值：
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2006/06/08	3.9		李洪强	创建
========================================================================*/
BOOL32 GetModFilePath(s8 *lpModName, s32 &nNameLen)
{
	if (lpModName == NULL || nNameLen < 1) 
	{
		return FALSE;
	}

	s8 achPath[PATH_MAX+1] = {0};

	s32 nLen = 0;
#ifdef WIN32
	nLen = GetModuleFileName(NULL, achPath, sizeof (achPath));
#endif // WIN32

#ifdef _LINUX_
	nLen = readlink( "/proc/self/exe", achPath, sizeof(achPath)-1);
	achPath[nLen] = 0;
#endif // _LINUX_

	if ( nLen < 1 || nLen > nNameLen)
	{
		return FALSE;
	}
	
	strncpy(lpModName, achPath, nNameLen);
	lpModName[nNameLen-1] = '\0';

	nNameLen = nLen;

	return TRUE;
}

BOOL32 IsFileExist( const s8* szFilePath )
{
    if ( NULL == szFilePath )
    {
        return FALSE;
    }

    struct stat sTimebuf;
	memset(&sTimebuf, 0, sizeof(sTimebuf));
    if ( 0 != stat( szFilePath, &sTimebuf ) )
    {
        return FALSE;
    }

    return TRUE;
}

u32 GetFileSize(const s8* szFilePath)
{
    if (NULL == szFilePath)
    {
        return 0;
    }
    
    struct stat tInfo;  
	memset(&tInfo, 0, sizeof(tInfo));
    stat(szFilePath, &tInfo);  

    return tInfo.st_size; 
}

/*==============================================================
函数名    ：GetSysTick
功能      ：得到系统的Tick
算法实现  ：<可选项>
参数说明  ：[I/O/IO]参数：含义（说明：按照参数的顺序排序）
返回值说明：
-----------------------------------------------------------------
修改记录  ：
日  期      版本        修改人      走读人        修改记录
2012/8/13    V2.5                                   创建
===============================================================*/
u32 GetSysTick()
{
	u32 dwTickCount = 0;
#ifdef WIN32
	dwTickCount = GetTickCount();
#endif // WIN32
	
#ifdef _LINUX_
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	dwTickCount = tv.tv_usec / 1000;
#endif // _LINUX_
	
	return dwTickCount;
}

u64 strtou64(/*IN*/ const s8 *szU64Str) 
{
	u64 lwValue = 0 ;
	if (NULL != szU64Str)
	{
#ifdef _LINUX_
		lwValue = strtoull(szU64Str, 0, 10);   //_LINUX_
#else
		lwValue = _atoi64(szU64Str);           //windows        
#endif
	}

	return lwValue;
}

BOOL32 u64tostr(/*IN*/ u64 lwValue, /*IN OUT*/ s8* strBuf, /*IN*/ u16 wMaxStrBufSize)
{
	if (NULL == strBuf || 0 >= wMaxStrBufSize )
	{
		assert(FALSE && "传入参数有误");
		return FALSE;
	}

#ifdef _LINUX_
	snprintf(strBuf, wMaxStrBufSize, "%llu", lwValue);		//_LINUX_ 
#else
	snprintf(strBuf, wMaxStrBufSize, "%I64u", lwValue);		//windows   
#endif

	return TRUE;    
}

} // end namespace
