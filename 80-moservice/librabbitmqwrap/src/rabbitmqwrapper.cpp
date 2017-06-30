#define _CRT_SECURE_NO_WARNINGS		//suppress Security Features in the CRT for  _snprintf

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <exception>
#include <time.h>
#include <string>
#include <map>
using namespace std; 

#pragma comment(lib, "winmm.lib")

#define __DLL_EXPORT__
#include "rabbitmqwrapper.h"
using namespace rabbitmqwrap;

#ifndef WIN32
#include <pthread.h>
#include <dlfcn.h>
#include <errno.h>		//for ETIMEDOUT
#endif


#ifndef snprintf
	#ifdef WIN32
		#define snprintf	_snprintf
	#else
		#define snprintf	snprintf
	#endif
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif



//------------global base in libevent
static struct event_base *g_event_base;			
static bool g_bEventInited = 0;
static bool g_bQuit = false;

//------------login info
static char s_achBrokerIp[16] = {0};
static uint16_t s_wBrokerPort = 0;
static char s_achVhost[128]={0};
static char s_achName[128] = {0};
static char s_achPwd[64] = {0};

//------------P_vector and mutex related
#ifndef WIN32
typedef struct
{
	pthread_cond_t m_tCond;
	pthread_mutex_t m_tMutex;
	unsigned int m_dwCurrentCount;
	unsigned int m_dwMaxCount;

}TMutexCond;

#define RBSEMHANDLE		pthread_cond_t*
#define RBTASKANDLE		pthread_t
#else
#define RBSEMHANDLE		HANDLE
#define RBTASKANDLE		HANDLE
#endif

static unsigned int s_dwProducerIdx = 0;
static vector<MQ*> g_vMsgQue;
static RBSEMHANDLE g_phMutex;
static RBTASKANDLE g_hEventLoopThread;


//-------------print related
static const unsigned char	RABBIT_ERROR		=	1;
static const unsigned char	RABBIT_WARNING		=	2;
static const unsigned char	RABBIT_INFO			=	3;
static const unsigned char	RABBIT_DETAIL		=	4;		//更详细的打印（包括打印时间）

static int g_nPrntLvl = 1;									//默认打印error信息

//typedef void (*PFOspPrintf)(signed int bScreen, signed int bFile, char * szFormat,...);
static PFOspPrintf  OspPrint = NULL;	//note: don't name this variable OspPrintf

typedef void (*PFOspRegCommand)(const char* szName, void* pfFunc, const char* szUsage);
PFOspRegCommand OspRegCmd = NULL;

static void WaitForThreadExit(RBTASKANDLE hTask)
{
#ifdef WIN32
	DWORD dwRtn = ::WaitForSingleObject(hTask, INFINITE);
#else
	int dwRtn = pthread_join(hTask, NULL);
#endif
}

//-------------internal functions
void RabbitmqwrapPrint(unsigned char byLevel, char*  pszFmt, ...)
{
	if( byLevel > g_nPrntLvl )
	{
		return;
	}
		
	if ( RABBIT_DETAIL == g_nPrntLvl )
	{
		
		time_t  tCurTime;
		struct tm   *ptTime;
		char achBuf[32] = {0};
		tCurTime = time(0);
		ptTime = localtime(&tCurTime);
		strftime(achBuf, sizeof(achBuf), "%H:%M:%S ", ptTime);

		if(OspPrint)
			OspPrint(1, 0, achBuf);

//		if(OspPrint)
//			OspPrint( 1, 0, "%s ", __TIME__ );
	}

	static const char * pchLevelName[] = 
	{(char *)"[ERROR]", (char *)"[WARNING]", "", ""};
	
	if( byLevel > RABBIT_INFO ) 
	{
		byLevel = RABBIT_INFO;
	}
	
	va_list tArgptr;
	char achContentStr[256] = {0};
	va_start( tArgptr, pszFmt );
	int nBufLen = vsnprintf(achContentStr, sizeof(achContentStr), pszFmt, tArgptr );	
	va_end( tArgptr );
	
	char    achPrintBuf[512] = {0};
	int nLen = snprintf( achPrintBuf, sizeof(achPrintBuf), "{Rabbitmqwrap} %s ", pchLevelName[byLevel-1] );	
		
	snprintf(achPrintBuf + nLen, sizeof(achPrintBuf) - nLen, "%s", achContentStr);
	//strncat( achPrintBuf + nLen, achContentStr, sizeof(achPrintBuf) - nLen );

	if(OspPrint)
	{
		OspPrint( 1, 0, achPrintBuf );
	}
		 
	return;
}

void* EventLoop(void* arg)
{
#ifndef WIN32
	pthread_attr_t tThreadAttr;
	pthread_t tid = pthread_self();
	pthread_getattr_np(tid, &tThreadAttr);
	//get stacksize of the thread
	size_t stacksize;
	pthread_attr_getstacksize(&tThreadAttr, &stacksize);
	int ndetachState;
	pthread_attr_getdetachstate(&tThreadAttr, &ndetachState);

	printf("[%s] stacksize of pid.%d-tid.%u is %d, detach state is %d\n", __FUNCTION__, getpid(), (unsigned int)tid, stacksize, ndetachState);
#endif 
	//NOTE: when using libevent's new version, it can be improved as a new flag provided and  while(1) can be avoided.
	while (!g_bQuit)
	{
		amqp_evbase_loop( g_event_base );
	}
	return 0;
}


bool MutexInit(RBSEMHANDLE *pphMutex)
{
	if (NULL == pphMutex)
	{
		return 0;
	}

#ifndef WIN32
	TMutexCond* ptMutexCond = NULL;
	try
	{
		ptMutexCond = ( TMutexCond* )malloc(sizeof(TMutexCond));
	}
	catch (const bad_alloc& e)
	{
		printf("bad alloc!\n");
		return 0;
	}
	
	pthread_cond_init(&ptMutexCond->m_tCond, NULL);
	pthread_mutex_init(&ptMutexCond->m_tMutex, NULL);
	ptMutexCond->m_dwCurrentCount = 1;
	ptMutexCond->m_dwMaxCount = 1;
	*pphMutex = (RBSEMHANDLE)ptMutexCond;

#else
	*pphMutex = CreateSemaphore(NULL, 1, 1, NULL);
#endif	//WIN32
	
	return 1;
}

bool MutexTake(RBSEMHANDLE phMutex)
{
	if (NULL == phMutex)
	{
		return 0;
	}

#ifndef WIN32

	TMutexCond* ptMutexCond = (TMutexCond* )phMutex;
	int nRet = 0;
	pthread_mutex_lock( &ptMutexCond->m_tMutex );
	while( ptMutexCond->m_dwCurrentCount <= 0 )
	{
		nRet = pthread_cond_wait( &ptMutexCond->m_tCond, &ptMutexCond->m_tMutex );
	}
	if( 0 == nRet )
    {
        if(ptMutexCond->m_dwCurrentCount > 0)
        {        
    	    ptMutexCond->m_dwCurrentCount--;
        }
    }

	pthread_mutex_unlock( &ptMutexCond->m_tMutex );
	return (0 == nRet);
#else
	return ( WAIT_FAILED != WaitForSingleObject(phMutex, INFINITE) );
#endif	//WIN32
}

bool MutexTakeByTime(RBSEMHANDLE phMutex, unsigned int dwTimeout)	//传入ms
{
	if (NULL == phMutex)
	{
		return 0;
	}

#ifndef WIN32
	
	TMutexCond* ptMutexCond = (TMutexCond* )phMutex;
	int nRet = 0;

	struct timeval now;
	struct timespec timeout;
	gettimeofday(&now, NULL);

	timeout.tv_sec = now.tv_sec + dwTimeout/1000;
	int nTime = dwTimeout%1000;
	if(now.tv_usec+nTime*1000>=1000000) 
	{
		timeout.tv_sec++;
		timeout.tv_nsec = (now.tv_usec+nTime*1000-1000000)*1000;
	}
	else
	{
		timeout.tv_nsec = (now.tv_usec + nTime*1000) * 1000;
	}

	pthread_mutex_lock( &ptMutexCond->m_tMutex );
	while( ptMutexCond->m_dwCurrentCount <= 0 )
	{
		nRet = pthread_cond_timedwait( &ptMutexCond->m_tCond, &ptMutexCond->m_tMutex, &timeout );
		if( ETIMEDOUT == nRet )
		{
			break;
		}
	}
	if( 0 == nRet )
	{
		if(ptMutexCond->m_dwCurrentCount > 0)
		{        
			ptMutexCond->m_dwCurrentCount--;
		}
	}

	pthread_mutex_unlock( &ptMutexCond->m_tMutex );
	return (0 == nRet);
#else
	return ( WAIT_FAILED != WaitForSingleObject(phMutex, dwTimeout) );
#endif	//WIN32
}

bool MutexGive(RBSEMHANDLE phMutex)
{
	if (NULL == phMutex)
	{
		return 0;
	}

#ifndef WIN32
	
	TMutexCond* ptMutexCond = (TMutexCond* )phMutex;

	int nRet = 1;
	pthread_mutex_lock( &ptMutexCond->m_tMutex );
	if( ptMutexCond->m_dwMaxCount > ptMutexCond->m_dwCurrentCount )
	{
		if( 0 == pthread_cond_signal( &ptMutexCond->m_tCond ) )
		{
			nRet = 0;
			ptMutexCond->m_dwCurrentCount ++;
		}
	}

	pthread_mutex_unlock( &ptMutexCond->m_tMutex );
	return (0 == nRet);

#else
	unsigned int previous;

	return (ReleaseSemaphore(phMutex, 1, (LPLONG)&previous) == 0) ? false: true;
#endif	//WIN32
}

bool MutexDelete(RBSEMHANDLE phMutex)
{
	if (NULL == phMutex)
	{
		return 0;
	}
#ifndef WIN32
	
	TMutexCond* ptMutexCond = (TMutexCond* )phMutex;
	pthread_cond_destroy( &ptMutexCond->m_tCond );
	pthread_mutex_destroy( &ptMutexCond->m_tMutex );
	free(ptMutexCond);
	
	return 1;

#else
	return ( CloseHandle(phMutex) == 0) ? false: true;
#endif	//WIN32
}

class CAutoMutex
{
public:
	CAutoMutex(RBSEMHANDLE pMutex)
	{
		m_pMutex = pMutex;
		MutexTake(m_pMutex);
	}
	~CAutoMutex()
	{
		MutexGive(m_pMutex);
	}
private:
	RBSEMHANDLE m_pMutex;
};

//---------------external API
namespace rabbitmqwrap{

RMQ_API void setrabbitmqlvl(int nlvl)
{
	g_nPrntLvl = nlvl;
	if( 0 != nlvl )
	{
		if (NULL == OspPrint)
		{
			printf( "cannot print due to inner problem: can't load OspPrintf addr in main program!\n ");
		}
		else
		{
			OspPrint(1, 0, "Ready to Print\n");
		}
	}
	
}

int RabbitmqClntInit(const char *pchSvrip, unsigned short wSvrPort/*=5672*/, const char *pchVhost, const char *pchName/*="guest"*/, const char *pchPwd/*="guest"*/)
{
	if (g_bEventInited)
	{
		printf("[%s] already initialized!\n", __FUNCTION__);
		return RT_ERROR;
	}

#ifndef WIN32
	void * handle = dlopen(NULL/*"./libosp.so"*/, RTLD_LAZY);
#else
	HINSTANCE handle = GetModuleHandle(NULL); //获取宿主程序主模块的句柄
#endif

	if (handle)
	{
#ifndef WIN32
		if ( NULL == OspPrint)
		{
			OspPrint = (PFOspPrintf)dlsym(handle, "OspPrintf");
		}
		
		if (NULL == OspRegCmd)
		{
			OspRegCmd = (PFOspRegCommand)dlsym(handle, "OspRegCommand");
			if (NULL != OspRegCmd)
			{
				printf("get OspRegCommand seccessfully!\n");
				OspRegCmd("setrabbitmqlvl", (void*)rabbitmqwrap::setrabbitmqlvl, "setrabbitmqlvl");
				OspRegCmd("showc", (void*)rabbitmqwrap::showc, "show consumer list");
				OspRegCmd("showp", (void*)rabbitmqwrap::showp, "show producer list");
			}
		}
		
#else
		if ( NULL == OspPrint)
		{
			OspPrint = (PFOspPrintf)GetProcAddress(handle, "OspPrintf");
		}

#endif
	}

	if ( NULL == OspPrint)
	{
		printf("Due to failing to load OspPrintf address, the lib does not support OspPrint \n");
	}


	do 
	{
		g_event_base = amqp_create_evbase_with_lock();
		g_bQuit = false;
		int nLen = snprintf(s_achBrokerIp,sizeof(s_achBrokerIp), "%s\0",pchSvrip);

		//fprintf( stderr, "achBrokerIp is :%.16s, len is %d, sizeof(achBrokerIp) is %d, strlen(pchSvrip) is %d, strlen(achBrokerIp) is:%d\n", 
		//	achBrokerIp, nLen, sizeof(achBrokerIp),strlen(pchSvrip), strlen(achBrokerIp) );

		s_wBrokerPort = wSvrPort;
		snprintf(s_achVhost,sizeof(s_achVhost), "%s\0",pchVhost);
		snprintf(s_achName,sizeof(s_achName), "%s\0",pchName);
		snprintf(s_achPwd,sizeof(s_achPwd), "%s\0",pchPwd);

#ifndef WIN32
		pthread_t thread;
		pthread_attr_t tThreadAttr;
		pthread_attr_init(&tThreadAttr);

		// set stack size for the new thread
		bool bSupportStacksize = 0;
		long nRetsysconf = sysconf(_SC_THREAD_ATTR_STACKSIZE);
		if(  0 > nRetsysconf)
		{
			if (-1 == nRetsysconf)
			{
				printf("[%s] not support stack size \n",__FUNCTION__);

			}
			else
			{
				//printf("support stack size modify. value is :%d\n",nRetsysconf);
				bSupportStacksize = 1;
			}
		}
		else
		{
			//printf("support stack size modify\n");
			bSupportStacksize = 1;
		}
		if(bSupportStacksize)
		{
			unsigned int dwStacksize = 100 <<10;
			pthread_attr_setstacksize(&tThreadAttr, dwStacksize);					// stack size: 100K
			printf("[%s] stack size:%d\n", __FUNCTION__, dwStacksize);
		}

		// set detach mode
		pthread_attr_setdetachstate(&tThreadAttr, PTHREAD_CREATE_DETACHED );	
		//printf("detachstate: %d\n",  PTHREAD_CREATE_DETACHED);

		int nErr = 0;
		nErr = pthread_create(&g_hEventLoopThread, &tThreadAttr, EventLoop, NULL);
		if (nErr != 0)
		{
			printf("[%s] can't create eventloop thread: %s\n", __FUNCTION__, strerror(nErr));
			break;  //return RT_ERROR;
		}
#else	//win32 

		unsigned int dwTaskID;
		unsigned int dwStacksize = 100 <<10;
		g_hEventLoopThread = CreateThread(NULL, dwStacksize, (LPTHREAD_START_ROUTINE)EventLoop, NULL, 0, (unsigned long *)&dwTaskID);
		if( NULL == g_hEventLoopThread)
		{
			printf("[%s] can't create eventloop thread\n",__FUNCTION__);
			break; //return RT_ERROR;
		}
#endif

		if ( NULL == g_phMutex)
		{
			if (!MutexInit(&g_phMutex))
			{
				MutexDelete(g_phMutex);
				printf("[%s] Create Mutex failed!\n", __FUNCTION__);
				break;//return RT_ERROR;
			}
		}

		if (g_vMsgQue.capacity() < 15)
		{
			printf("[%s] make reservation for MsgQue vector!\n", __FUNCTION__);
			g_vMsgQue.reserve(15);	//vector预留15个元素
		}

		g_bEventInited = true;

	} while (0);
	
	if (!g_bEventInited)
	{
		//indicate the initialization failed, we should release event_base
		amqp_destroy_evbase(g_event_base);
	}

	return (g_bEventInited)? RT_OK: RT_ERROR;
}

RMQ_API void RabbitmqClntQuit()
{
	CAutoMutex cAutoMutex(g_phMutex);

	if (g_hEventLoopThread)
	{
		g_bQuit = true;
		amqp_evbase_loopbreak(g_event_base);
		WaitForThreadExit(g_hEventLoopThread);
		g_hEventLoopThread = NULL;		

		vector<MQ*>::iterator queIter = g_vMsgQue.begin();
		for (; queIter != g_vMsgQue.end(); ++queIter)
		{
			MQ_ITEM* pMqItem = NULL;
			while ((pMqItem = mq_pop(*queIter)) != NULL)
			{
				mqi_free(pMqItem);
			}
			mq_deinit(*queIter);
			free(*queIter);
		}
		g_vMsgQue.clear();
	}
}

RMQ_API void SetOspPrintFP(PFOspPrintf pf)
{
	OspPrint = pf;
}

int PostMsg(IN unsigned int dwRBMq, 
			IN const char* content, IN unsigned int dwContentlen, 
			IN const char *pRkey,  
			IN const char *pXname /*= NULL*/,
			IN int nPersistant /*= 0*/, 
			IN int nRPC_mode /*= 0*/,
			IN const char *correlation_id /*= NULL*/,
			IN const char *reply_to /*= NULL */,
			IN unsigned int dwTTL)
{	
	CAutoMutex cAutoMutex(g_phMutex);

	if( dwRBMq > g_vMsgQue.size() || 0 >= dwRBMq)
	{
		RabbitmqwrapPrint( RABBIT_ERROR, "[%s] rabbit handle is invalid\n", __FUNCTION__);
		return RT_ERROR;
	}
	//MQ *pMsgQ = g_vMsgQue.at(nRBmq-1); 
	MQ *pMsgQ = g_vMsgQue[dwRBMq-1];  
	//MQ *pMsgQ = (MQ*)nRBmq;

	if ( NULL == pMsgQ )
	{
		RabbitmqwrapPrint( RABBIT_ERROR, "[%s] rabbit handle is NULL\n", __FUNCTION__);
		return RT_ERROR;
	}

	if ( NULL == content )
	{
		RabbitmqwrapPrint( RABBIT_ERROR, "[%s] content is NULL, fail to PostMsg\n", __FUNCTION__);
		return RT_ERROR;
	}

	if ( NULL == pXname && NULL == pRkey )
	{
		RabbitmqwrapPrint( RABBIT_ERROR, "[%s] both exchange name and routing key name is NULL, fail to PostMsg\n", __FUNCTION__);
		return RT_ERROR;
	}

	if ( NULL == pXname )
	{
		pXname = "";	//默认的exchange， 这里必须要赋下值，因为下面的mqi_prepare调用约束。
	}

	if (dwContentlen > MAXLEN_RBMSG )
	{
		dwContentlen = MAXLEN_RBMSG;			//500K
	}

#ifdef _LINUX_
	timeval start;
	gettimeofday(&start, NULL);

	RabbitmqwrapPrint(RABBIT_DETAIL, "[%s] mqi_prepare %ld.%ld\n", __FUNCTION__, start.tv_sec, start.tv_usec);
#endif

	//模拟构建MQ
	MQ_ITEM *mq_item = NULL;
	if (dwTTL > 0)
	{
		char achTTL[10] = { 0 };
		sprintf(achTTL, "%u", dwTTL);
		mq_item = mqi_prepare(pXname, pRkey, content, dwContentlen, nPersistant, nRPC_mode, correlation_id, reply_to, true, achTTL);
	}
	else
	{
		mq_item = mqi_prepare(pXname, pRkey, content, dwContentlen, nPersistant, nRPC_mode, correlation_id, reply_to, false, NULL);
	}
#ifdef _LINUX_
	gettimeofday(&start, NULL);
	RabbitmqwrapPrint(RABBIT_DETAIL, "[%s] mqi_push %ld.%ld\n", __FUNCTION__, start.tv_sec, start.tv_usec);
#endif
	bool bRtOk = 0;
	if ( NULL != mq_item )
	{
		bRtOk = 1;
		mq_push(pMsgQ, mq_item);
	}

#ifdef _LINUX_
	gettimeofday(&start, NULL);
	RabbitmqwrapPrint(RABBIT_DETAIL, "[%s] mqi_pushed %ld.%ld\n", __FUNCTION__, start.tv_sec, start.tv_usec);
#endif

	return (bRtOk ? RT_OK : RT_ERROR) ;
}


int CreateX(char *pXname, char *pXtype/*, int nDurable = 0*/)
{
	if ( NULL == pXname || NULL == pXtype)
	{
		RabbitmqwrapPrint( RABBIT_ERROR, "[%s] Xname is NULL or Xtype is NULL\n", __FUNCTION__);
		return RT_ERROR;
	}
	
	unsigned int dwAttr = E_DECLARE;
	//if( 0 != nDurable)
	//{
	//	dwAttr |= DURABLE;
	//}

	int nHeartbeat = 30;
	char *managertag = NULL; 
	int nRet = add_manager( g_event_base, s_achBrokerIp,  s_wBrokerPort, s_achVhost, s_achName, s_achPwd,
		NULL, pXname, pXtype, NULL, dwAttr,nHeartbeat, managertag  );

	return (nRet == 0? RT_OK: RT_ERROR);
}


int DeleteQ(char *pQname)
{
	if (NULL == pQname)
	{
		RabbitmqwrapPrint( RABBIT_ERROR, "[%s] Queue name is NULL\n", __FUNCTION__);
		return RT_ERROR;
	}

	unsigned int dwAttr = Q_DELETE;
	int nHeartbeat = 30;
	char *managertag = NULL; 
	int nRet = add_manager( g_event_base, s_achBrokerIp,  s_wBrokerPort, s_achVhost, s_achName, s_achPwd,
		pQname, NULL, NULL, NULL, dwAttr ,nHeartbeat, managertag);

	return (nRet == 0? RT_OK: RT_ERROR);
}

//------------------------safeP-------------------------
class CCreateProducerParam
{
public:
	CCreateProducerParam()
	{
		memset(m_achIp, 0, sizeof(m_achIp));
		m_wPort = 0;
		m_dwAttr = 0;
		memset(m_achVhost, 0, sizeof(m_achVhost));
		memset(m_achName, 0, sizeof(m_achName));
		memset(m_achPwd, 0, sizeof(m_achPwd));
		memset(m_achTag, 0, sizeof(m_achTag));
		m_pcMQ = NULL;
		m_fConnectionSuccCB = NULL;
		m_fConnectionFailCB = NULL;
		m_fPublisherconfirmCB = NULL;
		m_nHeartBeat = 30;
		m_pTimerEventHandle = NULL;
	}
	void SetParam(const char* pchIp,
		uint16_t wPort,
		const char* pchVhost,
		const char* pchName,
		const char* pchPwd,
		unsigned int dwAttr,
		const char* pchTag,
		MQ* pcMQ,
		ConnectionSucc_CB conn_success_cb /*= NULL*/,
		ConnectionFail_CB conn_disconnect_cb /*= NULL*/,
		PublisherConfirm_CB publisher_confirm_cb /*= NULL*/,
		int nHeartBeat /*=30*/)
	{
		strncpy(m_achIp, pchIp, sizeof(m_achIp) - 1);
		m_wPort = wPort;
		strncpy(m_achVhost, pchVhost, sizeof(m_achVhost) - 1);
		strncpy(m_achName, pchName, sizeof(m_achName) - 1);
		strncpy(m_achPwd, pchPwd, sizeof(m_achPwd) - 1);
		m_dwAttr = dwAttr;
		strncpy(m_achTag, pchTag, sizeof(m_achTag) - 1);
		m_pcMQ = pcMQ;
		m_fConnectionSuccCB = conn_success_cb;
		m_fConnectionFailCB = conn_disconnect_cb;
		m_fPublisherconfirmCB = publisher_confirm_cb;
		m_nHeartBeat = nHeartBeat;
	}
	void SetCreateProducerTimerEventHandle(struct event* pEvent)
	{
		m_pTimerEventHandle = pEvent;
	}
	struct event* GetCreateProducerTimerEventHandle() const
	{
		return m_pTimerEventHandle;
	}

	char m_achIp[100 + 1];
	uint16_t m_wPort;
	char m_achVhost[100 + 1];
	char m_achName[100 + 1];
	char m_achPwd[100 + 1];
	unsigned int m_dwAttr;
	char m_achTag[100 + 1];
	MQ* m_pcMQ;
	ConnectionSucc_CB m_fConnectionSuccCB;
	ConnectionFail_CB m_fConnectionFailCB;
	PublisherConfirm_CB m_fPublisherconfirmCB;
	int m_nHeartBeat;
	struct event * m_pTimerEventHandle;
};

map<string, CCreateProducerParam> g_mapTagProducerParam;

void CreateProducerCB(int fd, short event, void *params);

void SetTimerToCreateProducer(void* pProducer)
{
	struct timeval tv = { 3, 0 };
	struct event *timeout = event_new(g_event_base, -1, EV_TIMEOUT, CreateProducerCB, (void*)pProducer);
	((CCreateProducerParam*)pProducer)->SetCreateProducerTimerEventHandle(timeout);
	evtimer_add(timeout, &tv);
}

void Producer_ConFail_MQCB( amqp_connection_state_t conn, const char *expect, const char *precv )
{
	CAutoMutex cAutoMutex(g_phMutex);

	const char* pchTag = amqp_get_tag(conn);
	CCreateProducerParam& cCreateProducerParam(g_mapTagProducerParam[pchTag]);
	if (cCreateProducerParam.m_fConnectionFailCB != NULL)
	{
		//通知用户失败之后，启动定时add_producer直到成功
		cCreateProducerParam.m_fConnectionFailCB(conn, expect, precv);
	}
	SetTimerToCreateProducer((void*)&cCreateProducerParam);
}

int AddProducer(CCreateProducerParam& cCreateProducer)
{
	return add_producer(g_event_base, 
		cCreateProducer.m_achIp,
		cCreateProducer.m_wPort,
		cCreateProducer.m_achVhost,
		cCreateProducer.m_achName,
		cCreateProducer.m_achPwd,
		NULL, NULL, 				//exchange，routing key 全填空,这部分信息在MQ_ITEM中带
		cCreateProducer.m_dwAttr,				
		cCreateProducer.m_pcMQ,
		CT_JSON,
		0,
		cCreateProducer.m_nHeartBeat, 
		cCreateProducer.m_achTag,
		cCreateProducer.m_fConnectionSuccCB, 
		Producer_ConFail_MQCB, 
		cCreateProducer.m_fPublisherconfirmCB	
		);
}

void CreateProducerCB(int fd, short event, void *params)
{
	CAutoMutex cAutoMutex(g_phMutex);

	CCreateProducerParam* pCreateProducerParam = (CCreateProducerParam*)params;
	event_free(pCreateProducerParam->GetCreateProducerTimerEventHandle());
	int nRet = AddProducer(*pCreateProducerParam);
	if (nRet != 0)
	{
		RabbitmqwrapPrint(RABBIT_ERROR, "[%s]AddProducer<tag:%s> fail, sertimer to add producer util successs\n",
			__FUNCTION__, pCreateProducerParam->m_achTag);

		SetTimerToCreateProducer((void*)pCreateProducerParam);
	}
	else
	{
		RabbitmqwrapPrint(RABBIT_ERROR, "[%s]AddProducer<tag:%s> successs\n",
			__FUNCTION__, pCreateProducerParam->m_achTag);
	}
}

unsigned int SafeCreateP(ConnectionSucc_CB conn_success_cb /*= NULL*/, ConnectionFail_CB conn_disconnect_cb /*= NULL*/, PublisherConfirm_CB publisher_confirm_cb /*= NULL*/,
	int nHeartBeat )
{
	return SafeCreateProducer(s_achBrokerIp, s_wBrokerPort, s_achVhost, s_achName, s_achPwd,
		conn_success_cb, conn_disconnect_cb, publisher_confirm_cb, nHeartBeat);
}

unsigned int SafeCreateProducer(const char* pchIp, uint16_t wPort,
	const char* pchVhost, const char* pchName, const char* pchPwd,
    ConnectionSucc_CB conn_success_cb /*= NULL*/,
	ConnectionFail_CB conn_disconnect_cb /*= NULL*/,
	PublisherConfirm_CB publisher_confirm_cb /*= NULL*/,
	int nHeartBeat /*=30*/ )
{
	MQ *pMsgQ = (MQ*)malloc(sizeof(MQ));
	if ( pMsgQ == NULL )
	{
		return 0;
	}
	mq_init(pMsgQ);

	unsigned dwAttr = 0;
	if (NULL != publisher_confirm_cb)
	{
		dwAttr |= P_CONFIRM_SELECT;	//attr 置位C_CONFIRM_SELECT，表明要启用publisher confirm机制
	}

	CAutoMutex cAtuoMutex(g_phMutex);

	g_vMsgQue.push_back(pMsgQ);
	s_dwProducerIdx++;

	char achProducerID[100 + 1] = { 0 };
	snprintf(achProducerID, 100, "%u", s_dwProducerIdx);
	CCreateProducerParam cCreateProducerParam;
	cCreateProducerParam.SetParam(pchIp, wPort, pchVhost, pchName, pchPwd, dwAttr, achProducerID, pMsgQ,
		conn_success_cb, conn_disconnect_cb, publisher_confirm_cb, nHeartBeat);
	g_mapTagProducerParam[achProducerID] = cCreateProducerParam;
	int nRet = AddProducer(g_mapTagProducerParam[achProducerID]);
	if (nRet != 0)
	{
		SetTimerToCreateProducer((void*)&(g_mapTagProducerParam[achProducerID]));
	}

	return nRet == 0 ? s_dwProducerIdx : 0;
}
//------------------------safeP-end-------------------------
//------------------------safeC------------------------------------------------------
int SafeCreateC(IN const char *pchQname, IN const char *pchExchange, IN const char *pchRoutingkey /* = NULL */,
	IN ContentBodyCB body_cb /* = NULL */,
	IN ContenHeaderPropsCB header_props_cb /* = NULL */,
	IN AnonymousQueueDeclareCB anonymous_queue_declare_cb /* = NULL */,
	IN ConnectionSuccCB conn_success_cb /* = NULL */,
	IN ConnectionFailCB conn_disconnect_cb /* = NULL */,
	IN int nExclusive /* = 1 */,
	IN int nDurable /* = 0*/,
	IN int nAutoack /* = 0 */, IN int nPrefetch_count /* = 1 */,
	IN int nHeartBeat /* = 30 */,
	IN const char *pchTag /*= NULL*/)
{
	return SafeCreateConsumer(s_achBrokerIp, s_wBrokerPort, s_achVhost, s_achName, s_achPwd,
		pchQname, pchExchange, pchRoutingkey, body_cb, header_props_cb, anonymous_queue_declare_cb,
		conn_success_cb, conn_disconnect_cb, nExclusive, nDurable, nAutoack, nPrefetch_count, nHeartBeat, pchTag);
}

class CCreateConsumerParam
{
public:
	CCreateConsumerParam()
	{
		m_wPort = 0;
		m_dwAttr |= EXCLUSIVE;
		m_pairQname = make_pair(true, "");
		m_pairExchange = make_pair(true, "");
		m_pairRoutingKey = make_pair(true, "");
		m_pairUserTag = make_pair(true, "");
		m_fContentBodyCB = NULL;
		m_fContentHeaderPropsCB = NULL;
		m_fAnonymousQueueDeclareCB = NULL;
		m_fConnectionSuccCB = NULL;
		m_fConnectionFailCB = NULL;
		m_nAutoack = 0;
		m_nPrefetchCount = 1;
		m_nHeartBeat = 30;
		m_pTimerEventHandle = NULL;
	}
	
	void SetParam(IN const char* pchIp, IN uint16_t wPort,
		IN const char* pchVhost, IN const char* pchName, IN const char* pchPwd,
		IN const char *pchQname, IN const char *pchExchange, IN const char *pchRoutingkey,
		unsigned int dwAttr,
		IN int nAutoack, 
		IN int nPrefetch_count,
		IN int nHeartBeat,
		IN const char *pchUserTag,
		IN const char *pchUseReconnectTag,
		IN ConnectionSuccCB conn_success_cb,
		IN ConnectionFailCB conn_disconnect_cb,
		IN ContenHeaderPropsCB header_props_cb,
		IN ContentBodyCB body_cb,
		IN AnonymousQueueDeclareCB anonymous_queue_declare_cb
		)
	{
		m_strIp = pchIp;
		m_wPort = wPort;
		m_strVhost = pchVhost;
		m_strName = pchName;
		m_strPwd = pchPwd;
		m_dwAttr = dwAttr;
		if (pchQname == NULL)
		{
			m_pairQname = make_pair(true, "");
		}
		else
		{
			m_pairQname = make_pair(false, pchQname);
		}
		if (pchExchange == NULL)
		{
			m_pairExchange = make_pair(true, "");
		}
		else
		{
			m_pairExchange = make_pair(false, pchExchange);
		}
		if (pchRoutingkey == NULL)
		{
			m_pairRoutingKey = make_pair(true, "");
		}
		else
		{
			m_pairRoutingKey = make_pair(false, pchRoutingkey);
		}
		if (pchUserTag == NULL)
		{
			m_pairUserTag = make_pair(true, "");
		}
		else
		{
			m_pairUserTag = make_pair(false, pchUserTag);
		}
		m_strUseReconnectTag = pchUseReconnectTag;
		
		m_fContentBodyCB = body_cb;
		m_fContentHeaderPropsCB = header_props_cb;
		m_fAnonymousQueueDeclareCB = anonymous_queue_declare_cb;
		m_fConnectionSuccCB = conn_success_cb;
		m_fConnectionFailCB = conn_disconnect_cb;
		m_nAutoack = nAutoack;
		m_nPrefetchCount = nPrefetch_count;
		m_nHeartBeat = nHeartBeat;
		
	}
	void SetCreateConsumerTimerEventHandle(struct event* pEvent)
	{
		m_pTimerEventHandle = pEvent;
	}
	struct event* GetCreateConsumerTimerEventHandle() const
	{
		return m_pTimerEventHandle;
	}

	string m_strIp;
	uint16_t m_wPort;
	string m_strVhost;
	string m_strName;
	string m_strPwd;
	unsigned int m_dwAttr;
	typedef bool TYPE_ISNULL ;
	pair<TYPE_ISNULL, string> m_pairExchange;
	pair<TYPE_ISNULL, string> m_pairRoutingKey;
	pair<TYPE_ISNULL, string> m_pairQname;
	ContentBodyCB m_fContentBodyCB;
	ContenHeaderPropsCB m_fContentHeaderPropsCB;
	AnonymousQueueDeclareCB m_fAnonymousQueueDeclareCB;
	ConnectionSuccCB m_fConnectionSuccCB;
	ConnectionFailCB m_fConnectionFailCB;
	int m_nAutoack;
	int m_nPrefetchCount;
	int m_nHeartBeat;
	string m_strUseReconnectTag;
	pair<TYPE_ISNULL, string> m_pairUserTag;
	struct event* m_pTimerEventHandle;
};

map<string, CCreateConsumerParam> g_mapConsumerParam;

void Consumer_ConFail_MQCB(amqp_connection_state_t conn, const char *expect, const char *precv);
void Consumer_ConSuc_MQCB(amqp_connection_state_t conn, char *desc);
void AnonymousQueueDeclare_MQCB(amqp_connection_state_t conn, void *queueName, size_t queueNameLen);
void ContenHeaderProps_MQCB(amqp_connection_state_t conn, void *correlation_id, size_t idLen, void *replyTo, size_t replyToLen);
void ContentBody_MQCB(amqp_connection_state_t conn, void *pbuf, size_t len, response_type *prsp_type);
void CreateConsumerCB(int fd, short event, void *params);

int AddConsumer(const CCreateConsumerParam& cCreateConsumer)
{
	return add_consumer(g_event_base,
		cCreateConsumer.m_strIp.c_str(),
		cCreateConsumer.m_wPort,
		cCreateConsumer.m_strVhost.c_str(),
		cCreateConsumer.m_strName.c_str(),
		cCreateConsumer.m_strPwd.c_str(),
		cCreateConsumer.m_pairQname.first == true ? NULL : cCreateConsumer.m_pairQname.second.c_str(),
		cCreateConsumer.m_pairExchange.first == true ? NULL : cCreateConsumer.m_pairExchange.second.c_str(),
		cCreateConsumer.m_pairRoutingKey.first == true ? NULL : cCreateConsumer.m_pairRoutingKey.second.c_str(),
		cCreateConsumer.m_dwAttr,
		NULL,
		cCreateConsumer.m_nAutoack,
		cCreateConsumer.m_nPrefetchCount,
		cCreateConsumer.m_nHeartBeat,
		cCreateConsumer.m_strUseReconnectTag.c_str(),
		(cCreateConsumer.m_fConnectionSuccCB == NULL ? NULL : Consumer_ConSuc_MQCB),
		Consumer_ConFail_MQCB,
		(cCreateConsumer.m_fContentHeaderPropsCB == NULL ? NULL : ContenHeaderProps_MQCB),
		(cCreateConsumer.m_fContentBodyCB == NULL ? NULL : ContentBody_MQCB),
		(cCreateConsumer.m_fAnonymousQueueDeclareCB == NULL ? NULL : AnonymousQueueDeclare_MQCB));
}

void SetTimerToAddConsumer(void* pConsumer)
{
	struct timeval tv = { 3, 0 };
	struct event *timeout = event_new(g_event_base, -1, EV_TIMEOUT, CreateConsumerCB, (void*)pConsumer);
	((CCreateConsumerParam*)pConsumer)->SetCreateConsumerTimerEventHandle(timeout);
	evtimer_add(timeout, &tv);
}

void CreateConsumerCB(int fd, short event, void *params)
{
	CAutoMutex cAutoMutex(g_phMutex);

	CCreateConsumerParam* pCreateConsumer = (CCreateConsumerParam*)params;
	event_free(pCreateConsumer->GetCreateConsumerTimerEventHandle());
	int nRet = AddConsumer(*pCreateConsumer);
	if (nRet != 0)
	{
		RabbitmqwrapPrint(RABBIT_ERROR, "[%s]AddConsumer<ID:%s,QName:%s> fail, set timer add until success\n",
			__FUNCTION__, pCreateConsumer->m_strUseReconnectTag.c_str(),
			pCreateConsumer->m_pairQname.second.c_str());

		SetTimerToAddConsumer(pCreateConsumer);
	}
	else
	{
		RabbitmqwrapPrint(RABBIT_ERROR, "[%s]AddConsumer<ID:%s,QName:%s> success\n",
			__FUNCTION__, pCreateConsumer->m_strUseReconnectTag.c_str(),
			pCreateConsumer->m_pairQname.second.c_str());
	}
}

CCreateConsumerParam& GetCreateConsumerParam(amqp_connection_state_t conn)
{
	const char* pchUseReconnectTag = amqp_get_tag(conn);
	return g_mapConsumerParam[pchUseReconnectTag];
}

void Consumer_ConFail_MQCB(amqp_connection_state_t conn, const char *expect, const char *precv)
{
	CAutoMutex cAutoMutex(g_phMutex);

	CCreateConsumerParam& cCreateConsumerParam(GetCreateConsumerParam(conn));
	if (cCreateConsumerParam.m_fConnectionFailCB != NULL)
	{
		const char* pchUserTag = (cCreateConsumerParam.m_pairUserTag.first == true ?
			NULL : cCreateConsumerParam.m_pairUserTag.second.c_str());
		(cCreateConsumerParam.m_fConnectionFailCB)(conn, expect, precv, (char*)pchUserTag);
	}
	SetTimerToAddConsumer((void*)&cCreateConsumerParam);
}

void Consumer_ConSuc_MQCB(amqp_connection_state_t conn, char *desc)
{
	CAutoMutex cAutoMutex(g_phMutex);

	CCreateConsumerParam& cCreateConsumerParam(GetCreateConsumerParam(conn));
	const char* pchUserTag = (cCreateConsumerParam.m_pairUserTag.first == true ? NULL : cCreateConsumerParam.m_pairUserTag.second.c_str());
	(cCreateConsumerParam.m_fConnectionSuccCB)(conn, desc, (char*)pchUserTag);
}

void AnonymousQueueDeclare_MQCB(amqp_connection_state_t conn, void *queueName, size_t queueNameLen)
{
	CAutoMutex cAutoMutex(g_phMutex);

	CCreateConsumerParam& cCreateConsumerParam(GetCreateConsumerParam(conn));
	const char* pchUserTag = (cCreateConsumerParam.m_pairUserTag.first == true ? NULL : cCreateConsumerParam.m_pairUserTag.second.c_str());
	(cCreateConsumerParam.m_fAnonymousQueueDeclareCB)(conn, queueName, queueNameLen, (char*)pchUserTag);
}

void ContenHeaderProps_MQCB(amqp_connection_state_t conn, void *correlation_id, size_t idLen,
	void *replyTo, size_t replyToLen)
{
	CAutoMutex cAutoMutex(g_phMutex);

	CCreateConsumerParam& cCreateConsumerParam(GetCreateConsumerParam(conn));
	const char* pchUserTag = (cCreateConsumerParam.m_pairUserTag.first == true ? NULL : cCreateConsumerParam.m_pairUserTag.second.c_str());
	(cCreateConsumerParam.m_fContentHeaderPropsCB)(conn, correlation_id, idLen, replyTo, replyToLen, (char*)pchUserTag);
}

void ContentBody_MQCB(amqp_connection_state_t conn, void *pbuf, size_t len, response_type *prsp_type)
{
	CAutoMutex cAutoMutex(g_phMutex);

	CCreateConsumerParam& cCreateConsumerParam(GetCreateConsumerParam(conn));
	const char* pchUserTag = (cCreateConsumerParam.m_pairUserTag.first == true ? NULL : cCreateConsumerParam.m_pairUserTag.second.c_str());
	(cCreateConsumerParam.m_fContentBodyCB)(conn, pbuf, len, prsp_type, (char*)pchUserTag);
}

int SafeCreateConsumer(IN const char* pchIp, IN uint16_t wPort,
	IN const char* pchVhost, IN const char* pchName, IN const char* pchPwd,
	IN const char *pchQname, IN const char *pchExchange, IN const char *pchRoutingkey /* = NULL */,
	IN ContentBodyCB body_cb /* = NULL */,
	IN ContenHeaderPropsCB header_props_cb /* = NULL */,
	IN AnonymousQueueDeclareCB anonymous_queue_declare_cb /* = NULL */,
	IN ConnectionSuccCB conn_success_cb /* = NULL */,
	IN ConnectionFailCB conn_disconnect_cb /* = NULL */,
	IN int nExclusive /* = 1 */,
	IN int nDurable /* = 0*/,
	IN int nAutoack /* = 0 */, IN int nPrefetch_count /* = 1 */,
	IN int nHeartBeat /* = 30 */,
	IN const char *pchTag /*= NULL*/)
{
	unsigned int dwAttr = Q_DECLARE/*|Q_BIND*/|B_QOS|B_CONSUME;
	if ( NULL != pchExchange )
	{
		dwAttr |= Q_BIND;		//设置了exchange则要进行Q_BIND		
	}

	if (0 != nExclusive)
	{
		printf("CreatC------------EXCLUSIVE\n");
		dwAttr |= EXCLUSIVE;
	}
	if ( 0 != nDurable )
	{
		printf("CreatC------------Durable\n");
		dwAttr |= DURABLE;
	}

	CAutoMutex cAutoMutex(g_phMutex);

	unsigned int dwConsumerID = g_mapConsumerParam.size();
	char achConsumerID[100 + 1] = { 0 };
	snprintf(achConsumerID, 100, "%u", dwConsumerID);
	CCreateConsumerParam cCreate;
	cCreate.SetParam(pchIp, wPort, pchVhost, pchName, pchPwd, pchQname,pchExchange, pchRoutingkey,
		dwAttr, nAutoack, nPrefetch_count, nHeartBeat, pchTag, achConsumerID, 
		conn_success_cb, conn_disconnect_cb, header_props_cb, body_cb, anonymous_queue_declare_cb);
	g_mapConsumerParam[achConsumerID] = cCreate;
	int nRet = AddConsumer(g_mapConsumerParam[achConsumerID]);
	if (nRet != 0)
	{
		SetTimerToAddConsumer((void*)(&g_mapConsumerParam[achConsumerID]));
	}
	return nRet == 0 ? RT_OK : RT_ERROR;
}

//-------------------------------------------safec-end------------------------------------------------------
RMQ_API void showp()
{
	RabbitmqwrapPrint(RABBIT_ERROR, "producer num: %u\n", g_mapTagProducerParam.size());
	RabbitmqwrapPrint(RABBIT_ERROR, "=====================\n");
	map<string, CCreateProducerParam>::iterator pIter = g_mapTagProducerParam.begin();
	for (; pIter != g_mapTagProducerParam.end(); ++pIter)
	{
		RabbitmqwrapPrint(RABBIT_ERROR, "Tag: %s\n", pIter->first.c_str());
		RabbitmqwrapPrint(RABBIT_ERROR, "ip: %s\n", pIter->second.m_achIp);
		RabbitmqwrapPrint(RABBIT_ERROR, "port: %u\n", pIter->second.m_wPort);
		RabbitmqwrapPrint(RABBIT_ERROR, "vhost: %s\n", pIter->second.m_achVhost);
		RabbitmqwrapPrint(RABBIT_ERROR, "name: %s\n", pIter->second.m_achName);
		RabbitmqwrapPrint(RABBIT_ERROR, "pwd: %s\n", pIter->second.m_achPwd);
		RabbitmqwrapPrint(RABBIT_ERROR, "attr: %u\n", pIter->second.m_dwAttr);
		RabbitmqwrapPrint(RABBIT_ERROR, "tag: %s\n", pIter->second.m_achTag);
		RabbitmqwrapPrint(RABBIT_ERROR, "mq*: %s\n", pIter->second.m_pcMQ == NULL ? "NULL" : "NOT NULL");
		RabbitmqwrapPrint(RABBIT_ERROR, "succCB: %s\n", pIter->second.m_fConnectionSuccCB == NULL ? "NULL" : "NOT NULL");
		RabbitmqwrapPrint(RABBIT_ERROR, "failCB: %s\n", pIter->second.m_fConnectionFailCB == NULL ? "NULL" : "NOT NULL");
		RabbitmqwrapPrint(RABBIT_ERROR, "ConfirmCB: %s\n", pIter->second.m_fPublisherconfirmCB == NULL ? "NULL" : "NOT NULL");
		RabbitmqwrapPrint(RABBIT_ERROR, "HeartBeat: %u\n", pIter->second.m_nHeartBeat);
		RabbitmqwrapPrint(RABBIT_ERROR, "---------------------\n");
	}
}

RMQ_API void showc()
{
	RabbitmqwrapPrint(RABBIT_ERROR, "consumer num: %u\n", g_mapConsumerParam.size());
	RabbitmqwrapPrint(RABBIT_ERROR, "=====================\n");
	map<string, CCreateConsumerParam>::iterator cIter = g_mapConsumerParam.begin();
	for (; cIter != g_mapConsumerParam.end(); ++cIter)
	{
		RabbitmqwrapPrint(RABBIT_ERROR, "Tag: %s\n", cIter->first.c_str());
		RabbitmqwrapPrint(RABBIT_ERROR, "ip: %s\n", cIter->second.m_strIp.c_str());
		RabbitmqwrapPrint(RABBIT_ERROR, "port: %u\n", cIter->second.m_wPort);
		RabbitmqwrapPrint(RABBIT_ERROR, "vhost: %s\n", cIter->second.m_strVhost.c_str());
		RabbitmqwrapPrint(RABBIT_ERROR, "name: %s\n", cIter->second.m_strName.c_str());
		RabbitmqwrapPrint(RABBIT_ERROR, "pwd: %s\n", cIter->second.m_strPwd.c_str());
		RabbitmqwrapPrint(RABBIT_ERROR, "attr: %u\n", cIter->second.m_dwAttr);
		RabbitmqwrapPrint(RABBIT_ERROR, "Qname: %s\n", cIter->second.m_pairQname.first == true ? "NULL"
			: cIter->second.m_pairQname.second.c_str());
		RabbitmqwrapPrint(RABBIT_ERROR, "RoutingKey: %s\n", cIter->second.m_pairRoutingKey.first == true ? "NULL" 
			: cIter->second.m_pairRoutingKey.second.c_str());
		RabbitmqwrapPrint(RABBIT_ERROR, "Exchange: %s\n", cIter->second.m_pairExchange.first == true ? "NULL" 
			: cIter->second.m_pairExchange.second.c_str());
		RabbitmqwrapPrint(RABBIT_ERROR, "contentCB: %s\n", cIter->second.m_fConnectionFailCB == NULL ? "NULL" : "NOT NULL");
		RabbitmqwrapPrint(RABBIT_ERROR, "contentHeaderPropsCB: %s\n", cIter->second.m_fContentHeaderPropsCB == NULL ? "NULL" : "NOT NULL");
		RabbitmqwrapPrint(RABBIT_ERROR, "anonymousQueueDeclareCB: %s\n", cIter->second.m_fAnonymousQueueDeclareCB == NULL ? "NULL" : "NOT NULL");
		RabbitmqwrapPrint(RABBIT_ERROR, "succCB: %s\n", cIter->second.m_fConnectionSuccCB == NULL ? "NULL" : "NOT NULL");
		RabbitmqwrapPrint(RABBIT_ERROR, "failCB: %s\n", cIter->second.m_fConnectionFailCB == NULL ? "NULL" : "NOT NULL");
		RabbitmqwrapPrint(RABBIT_ERROR, "tag: %s\n", cIter->second.m_pairUserTag.first == true ? "NULL" 
			: cIter->second.m_pairUserTag.second.c_str());
		RabbitmqwrapPrint(RABBIT_ERROR, "ID: %s\n", cIter->second.m_strUseReconnectTag.c_str());
		RabbitmqwrapPrint(RABBIT_ERROR, "HeartBeat: %u\n", cIter->second.m_nHeartBeat);
		RabbitmqwrapPrint(RABBIT_ERROR, "AutoAck: %u\n", cIter->second.m_nAutoack);
		RabbitmqwrapPrint(RABBIT_ERROR, "PrefetchCount: %u\n", cIter->second.m_nPrefetchCount);
		RabbitmqwrapPrint(RABBIT_ERROR, "---------------------\n");
	}
}

}	// end of namespace "rabbitmqwrap"


