
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

//tcp/ipЭ���У�ר�ű���������IP��ַ������Ϊ˽�е�ַ�����ַ��Χ���£�        
//10.0.0.0/8��10.0.0.0��10.255.255.255
//172.16.0.0/12��172.16.0.0��172.31.255.255
//192.168.0.0/16��192.168.0.0��192.168.255.255
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
	
    s32 nDot    = 0;                        //  �Ѵ�����ַ�����ĵ����
    s32 nData   = 0;                        //  �Ѵ�����ַ��������ֵ
    s32 nPos	= 0;						//	����֮��ļ��
    s32 nLen    = strlen( lptrIPStr );     //  �ַ�������
	
    //  �ַ�������7λ�15λ, �Ҳ�����'0'��ͷ
    if ( nLen < 7 || nLen > 15 ) 
    {
        // 2004/05/19 ���ǿ *.*.*.*��***.***.***.***
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
        if ( lptrIPStr[nCount] >= '0' && lptrIPStr[nCount] <= '9' )//��������
        {
            nData = 10 * nData + lptrIPStr[nCount] - '0';     //������ֵ
			++nPos;
        }
        else if ( lptrIPStr[nCount] == '.' )     //  �������
        {
            nDot++;
            if ( nDot > 3 )  //  ���������3
            {
				return FALSE;
            }
			else if ( nPos > 3 )
			{
				return FALSE;
			}
			else if ( nData > 255 ) //��ֵ����255
			{
				return FALSE;
			}
            else        //  �Ѵ�����ַ����Ϸ�
            {
                nData = 0;
				nPos = 0;
            }
        }
        else            //  �����Ƿ��ַ�
        {
			return FALSE;
        }
    }
	
    if ( nDot != 3 )
    {
		return FALSE;
    }
    
    // ����@2006.03.17 ������һ�������Ƿ�Ϸ�
    if ( nData > 255 ) //��ֵ����255
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
�� �� ���� CreatDir
��    �ܣ� ����Ŀ¼(�ݹ鴴��)
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� const s8* lpszDirPath
�� �� ֵ�� BOOL32 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2007/9/15   4.0			�ܹ��                  ����
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
	// ��ȡ�ϼ�Ŀ¼
	s8 ch = '/';
    s8* pchDest = strrchr( achDirPath, ch );
    if ( NULL != pchDest )
    {
        *pchDest = '\0';
    }

	// ����ϼ�Ŀ¼������,�ݹ鴴��
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
�� �� ���� CreatFile
��    �ܣ� ��������·���µ��ļ���ͬʱ�Զ�������Ҫ��Ŀ¼
           ���ļ��Ѵ��ڣ���ֱ�ӷ���TURE
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� const s8 * lpszFileFullPath  // �ļ�ȫ·�������ļ���
�� �� ֵ�� BOOL32 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2008/10/8   4.0		    �ܹ��                  ����
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
    
    //����ļ�Ŀ¼��ַ
	s8 ch = '/';
    s8* pchDest = strrchr( achDirPath, ch );
    if ( NULL != pchDest )
    {
        *pchDest = '\0';
    }
    BOOL32 bCreat = CreatDir( achDirPath );// �ݹ鴴��Ŀ¼
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
�� �� ���� IsProcessRunned
��    �ܣ� �����Ƿ��Ѿ�������
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� const s8 * pszApplicationName
�� �� ֵ�� BOOL32 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2010/7/12   4.0			�ܹ��                  ����
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
    while( ( dirp=readdir(dp) ) != NULL ) // �������еĽ���ID  
    {  
        memset(sProcStatFile,0,sizeof(sProcStatFile));  
        memset(sReadBuf,0,sizeof(sReadBuf));       
        if( !isdigit(dirp->d_name[0]) ) // һ�����̺Ŷ�Ӧһ����/procĿ¼��һ����ͬ���ֵ�Ŀ¼
        {
            continue;
        }
        
        snprintf( sProcStatFile, sizeof(sProcStatFile), "/proc/%s/stat", dirp->d_name );
		sProcStatFile[sizeof(sProcStatFile)-1] = '\0';

        FILE * fp = fopen((const s8*)sProcStatFile,"r"); // ��/Proc/<�����ļ���>/stat�ļ��л�ȡ�ļ���
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
        pPPid = strtok( (s8*)sReadBuf, (const s8*)achSeps );    // ����ID  
        if ( NULL == pPPid ) continue;

        pid_t ppid = atoi(pPPid);                               // ͬ�����̣�����
        if( ppid == dwPID )
        {
           continue;
	    }

        pPPid = strtok( NULL, (const s8*)achSeps );             // ������
        if ( NULL == pPPid ) continue;
        
        // ���������ļ�����(xxx)����ʽ���ڣ���Ҫ������ȥ��
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
�� �� ���� GetCurrentProcessID
��    �ܣ� 
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� void
�� �� ֵ�� u32 
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��		�汾		�޸���		�߶���    �޸�����
2009/9/28   4.0			�ܹ��                  ����
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

	//������ʾno child processes�Ľ��������
	//system()����ǰ��SIGCHLD�źŴ���ʽ��ʽ�޸�ΪSIG_DFL��ʽ��ͬʱ��¼ԭ���Ĵ���ʽ��
	//ʹ����system()������Ϊԭ���Ĵ���ʽ���������ǿ���������ϵͳ�������źŴ���ʽ�ı������Ӱ�졣
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

        for (int fd = 0; fd < dwMaxNumFD; ++fd)  //����stdin stdout stderr
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
            //��ֹcpu������sleep 30����
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
�� �� ��:GetModuleFullPath
��    ��:��ȡģ�������ļ���ȫ·��, ��������\
��    ��:��
ע    ��:���� C:\
�� �� ֵ:�ļ����ַ���
-------------------------------------------------------------------------------
�޸ļ�¼:
��      ��  �汾    �޸���  �޸�����
2004/10/26  3.5     ���    ����
2006/06/07  3.9		���ǿ  ֧��LINUXƽ̨
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
�� �� ����GetModFilePath
��    �ܣ��õ�ģ���ȫ·���������ļ���
��    ����s8 *lpModName		[out]ģ�����Ŀռ�
		  s32 &nNameLen			[in/out]inģ�����ռ�Ĵ�С,outʵ�ʵ�ģ��������
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2006/06/08	3.9		���ǿ	����
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
������    ��GetSysTick
����      ���õ�ϵͳ��Tick
�㷨ʵ��  ��<��ѡ��>
����˵��  ��[I/O/IO]���������壨˵�������ղ�����˳������
����ֵ˵����
-----------------------------------------------------------------
�޸ļ�¼  ��
��  ��      �汾        �޸���      �߶���        �޸ļ�¼
2012/8/13    V2.5                                   ����
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
		assert(FALSE && "�����������");
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
