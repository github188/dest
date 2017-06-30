
#include "rediswrap.h"
#include "acl_cpp/lib_acl.hpp"

using namespace std;

static BOOL32 WaitForThreadExit(TASKHANDLE hTask)
{
#ifdef WIN32
	DWORD dwRtn = ::WaitForSingleObject(hTask, INFINITE);
#else
	s32 dwRtn = pthread_join(hTask, NULL);
#endif
	return dwRtn == 0 ? TRUE : FALSE;
}

//--CRedisResultImpl
class CRedisResult::CRedisResultImpl
{
public:
	CRedisResultImpl() : m_nResVal(0){};
	~CRedisResultImpl() {}
	s32 GetInteger() const
	{
		return m_nResVal;
	}
	const s8* GetString() const
	{
		return m_strRes.c_str();
	}
	void SetInteger(const s32 nVal)
	{
		m_nResVal = nVal;
	}
	void SetString(const s8* pchStr)
	{
		if (pchStr != NULL)
		{
			m_strRes = pchStr;
		}
	}
private:
	s32 m_nResVal;
	string m_strRes;
};

//--CRedisResult
CRedisResult::CRedisResult()
{
	m_pRedisResultImpl = new CRedisResultImpl;
}
CRedisResult::~CRedisResult()
{
	delete m_pRedisResultImpl;
}

s32 CRedisResult::GetInteger() const
{
	return m_pRedisResultImpl->GetInteger();
}

const s8* CRedisResult::GetString() const
{
	return m_pRedisResultImpl->GetString();
}

void CRedisResult::SetInteger(const s32 nVal)
{
	m_pRedisResultImpl->SetInteger(nVal);
}

void CRedisResult::SetString(const s8* pchStr)
{
	m_pRedisResultImpl->SetString(pchStr);
}

//--redissubscribe
//	-sem(CRedisSubscriberImpl::Quit, cleanup)
class CRedisSubscriberImpl
{
public:
	CRedisSubscriberImpl(): m_hMessageLoop(NULL)
	{
		OspSemBCreate(&m_hQuit);
		m_pcRedis = new acl::redis();
	}
	~CRedisSubscriberImpl()
	{
		Quit();
		OspSemDelete(m_hQuit);
	}
	BOOL32 Init(const s8* pchIp, u16 wPort)
	{
		AUTOLOCK;
		acl::acl_cpp_init();

		s8 achAddr[30] = { 0 };
		sprintf(achAddr, "%s:%d", pchIp, wPort);
		m_pcRedis->set_client(new acl::redis_client(achAddr));
		return TRUE;
	}
	void Quit()
	{
		AUTOLOCK;
		m_strChannel.erase();

		//取消已有订阅
		if (m_pcRedis)
		{
			m_pcRedis->get_client()->close();
			m_pcRedis->quit();
		}

		if (m_hMessageLoop)
		{
			// 发送退出信号量
			OspSemGive(m_hQuit);
			// 等待线程退出
			WaitForThreadExit(m_hMessageLoop);
			m_hMessageLoop = NULL;
		}

		if (m_pcRedis)
		{
			delete m_pcRedis->get_client();
			delete m_pcRedis;
			m_pcRedis = NULL;
		}
	}
	BOOL32 Subscribe(const s8* pchChannel)
	{
		AUTOLOCK;
		if (!m_pcRedis)	return FALSE;

		m_strChannel = pchChannel;
		OspSemTake(m_hQuit);
		m_hMessageLoop =
			::OspTaskCreate(&CRedisSubscriberImpl::MessageLoop, "RedisMessageLoop", 90, 1024 << 8, (u32)(this), 0);
		if (!m_hMessageLoop)
		{
			return FALSE;
		}
		return TRUE;
	}
	void SetCallBack(PFunc_SubscribeCallBack pfCallBack)
	{
		m_pfCallBack = pfCallBack;
	}

	static void* MessageLoop(void *pcContent)
	{
		CRedisSubscriberImpl* pRedisSubscrber = (CRedisSubscriberImpl*)pcContent;
		BOOL32 bSuccess = FALSE;

		while (!OspSemTakeByTime(pRedisSubscrber->m_hQuit, 1))
		{
			if (!bSuccess)
			{
				OspPrintf(TRUE, FALSE, "[REDIS]Resubscribe %s\n", pRedisSubscrber->m_strChannel.c_str());

				vector<const s8*> vecChannels;
				vecChannels.push_back(pRedisSubscrber->m_strChannel.c_str());
				s32 dwNum = pRedisSubscrber->m_pcRedis->subscribe(vecChannels);
				if (dwNum == 0)
				{
					OspPrintf(TRUE, FALSE, "[REDIS]Subscribe Failed\n");
					continue;
				}
			}

			OspPrintf(TRUE, FALSE, "[REDIS]Get Message\n", pRedisSubscrber->m_strChannel.c_str());

			acl::string strChannel, strMessage;
			bSuccess = pRedisSubscrber->m_pcRedis->get_message(strChannel, strMessage);
			if (bSuccess)
			{
				OspPrintf(TRUE, FALSE, "[REDIS]Receive Redis Publish Message:\n");
				OspPrintf(TRUE, FALSE, "%s\n", strMessage.c_str());
				if (pRedisSubscrber->m_pfCallBack)
				{
					(*pRedisSubscrber->m_pfCallBack)(strChannel.c_str(), strMessage.c_str());
				}
			}
			else
			{
				OspPrintf(TRUE, FALSE, "[REDIS]Receive Redis Msg Failed:\n");
			}
		}
		return 0;
	}
private:
	OspLock m_cLock;
	SEMHANDLE m_hQuit;
	acl::redis* m_pcRedis;
	TASKHANDLE m_hMessageLoop;
	std::string m_strChannel;
	PFunc_SubscribeCallBack m_pfCallBack;
};

CRedisSubscriber::CRedisSubscriber() 
{
	m_pcSubsriberImpl = new CRedisSubscriberImpl;
}

CRedisSubscriber::~CRedisSubscriber()
{
	delete m_pcSubsriberImpl;
}

BOOL32 CRedisSubscriber::Init(const s8* achIp, u16 wPort)
{
	return m_pcSubsriberImpl->Init(achIp, wPort);
}

void CRedisSubscriber::Quit()
{
	m_pcSubsriberImpl->Quit();
}

BOOL32 CRedisSubscriber::Subscribe(const s8* pchChannel)
{
	return m_pcSubsriberImpl->Subscribe(pchChannel);
}

void CRedisSubscriber::SetCallBack(PFunc_SubscribeCallBack pfCallBack)
{
	m_pcSubsriberImpl->SetCallBack(pfCallBack);
}

//--subscribe-end---

class CRedisImpl
{
public:
	CRedisImpl()
	{
		m_pcRedis = new acl::redis();
	}
	~CRedisImpl()
	{
		if (m_pcRedis->get_client() != NULL)
		{
			delete m_pcRedis->get_client();
		}
		delete m_pcRedis;
	}

	BOOL32 Init(const s8* pchIp, u16 wPort, const s8* pchPwd = NULL)
	{
		AUTOLOCK;
		acl::acl_cpp_init();

		s8 achAddr[30] = { 0 };
		sprintf(achAddr, "%s:%d", pchIp, wPort);
		acl::redis_client* pRedisClient = new acl::redis_client(achAddr);
		if (pchPwd != NULL)
		{
			pRedisClient->set_password(pchPwd);
		}
		m_pcRedis->set_client(pRedisClient);
		return TRUE;
	}

	BOOL32 IsConnected() 
	{
		AUTOLOCK;
		return m_pcRedis->ping();
	}

	BOOL32 LoadAllScript(const s8** pchScriptPathArry, const u32 dwScriptNum,
		vector<string>* pvecFailedScript = NULL)
	{
		u32 i = 0;
		for (; i < dwScriptNum; ++i)
		{
			if (!LoadScript(pchScriptPathArry[i]) && pvecFailedScript != NULL)
			{
				pvecFailedScript->push_back(pchScriptPathArry[i]);
			}
		}
		return TRUE;
	}

	BOOL32 LoadAllScript(const std::vector<std::string>& vecScriptPath,
		vector<string>* pvecFailedScript = NULL)
	{
		vector<string>::const_iterator iterScriptPath = vecScriptPath.begin();
		for (; iterScriptPath != vecScriptPath.end(); ++iterScriptPath)
		{
			if (!LoadScript(iterScriptPath->c_str()) && pvecFailedScript != NULL)
			{
				pvecFailedScript->push_back(*iterScriptPath);
			}
		}
		return TRUE;
	}

	BOOL32 LoadScript(const s8* achFileName)
	{
		AUTOLOCK;

		if (!m_pcRedis) return FALSE;

		acl::string strFile;
		bool bResult = acl::ifstream::load(achFileName, &strFile);
		if (!bResult)
		{
			OspPrintf(TRUE, FALSE, "[%s]Can't Open Script File %s!\n", __FUNCTION__, achFileName);
			return bResult;
		}

		acl::string strSHA1;
		bResult = m_pcRedis->script_load(strFile, strSHA1);
		if (!bResult)
		{
			OspPrintf(TRUE, FALSE, "[%s]Load Script File %s Failed!\n", __FUNCTION__, achFileName);
			return bResult;
		}

		m_mapFilePathScriptSHA[achFileName] = strSHA1.c_str();
		OspPrintf(TRUE, FALSE, "[%s]Load Script File %s Success, SHA:%s\n", __FUNCTION__,
			achFileName, strSHA1.c_str());
		return bResult;
	}

	BOOL32 EvalScript(const s8* pchFilePath,
		const std::vector<const s8*>& vecKeys,
		const std::vector<const s8*>& vecArgs,
		CRedisResult* pResult = NULL,
		BOOL32 bReload = TRUE)
	{
		// 如果发现sha1为空
		const acl::redis_result* pResultTemp = m_pcRedis->evalsha(
			m_mapFilePathScriptSHA[pchFilePath].c_str(), vecKeys, vecArgs);
		if (!pResultTemp)
		{
			OspPrintf(TRUE, FALSE, "[%s]Run Script %d Failed, No Result\n",
				__FUNCTION__, pchFilePath);
			return FALSE;
		}

		if (pResultTemp->get_type() == acl::REDIS_RESULT_ERROR)
		{
			// 如果发现不存在，则尝试重新注册脚本
			if (bReload && strstr(pResultTemp->get_error(), "NOSCRIPT"))
			{
				map<string, string>::const_iterator iterFilePath = m_mapFilePathScriptSHA.begin();
				for (; iterFilePath != m_mapFilePathScriptSHA.end(); ++iterFilePath)
				{
					LoadScript(iterFilePath->first.c_str());
				}
				return EvalScript(pchFilePath, vecKeys, vecArgs, pResult, FALSE);
			}
			else
			{
				OspPrintf(TRUE, FALSE, "[%s]Run Script %d Failed, Reason:%s\n",
					__FUNCTION__, pchFilePath, pResultTemp->get_error());
				return FALSE;
			}
		}

		if (pResult)
		{
			if (pResultTemp->get_type() == acl::REDIS_RESULT_INTEGER)
			{
				bool bSuccess = false;
				pResult->SetInteger(pResultTemp->get_integer(&bSuccess));
				if (!bSuccess)
				{
					OspPrintf(TRUE, FALSE, "[%s]Run Script %d,ResType is int, "
						"but get integer failed\n",
						__FUNCTION__, pchFilePath);
				}
				return bSuccess;
			}
			else if (pResultTemp->get_type() == acl::REDIS_RESULT_STRING)
			{
				const s8* pchResStr = pResultTemp->get(0);
				if (pchResStr != NULL)
				{
					pResult->SetString(pchResStr);
					return TRUE;
				}
			}
			else
			{
				OspPrintf(TRUE, FALSE, "[%s]Run Script %d ResType is unsupport\n",
					__FUNCTION__, pchFilePath);
				return FALSE;
			}
		}

		return TRUE;
	}

private:
	OspLock m_cLock;
	acl::redis* m_pcRedis;
	std::map<std::string, std::string> m_mapFilePathScriptSHA;
};

//--CRedisResult

CRedisWrap::CRedisWrap() 
{
	m_pcRedisImplement = new CRedisImpl;
}

CRedisWrap::~CRedisWrap()
{
	delete m_pcRedisImplement;
}

BOOL32 CRedisWrap::Init(const s8* achIp, u16 wPort, const s8* pchPwd/*=NULL*/)
{
	return m_pcRedisImplement->Init(achIp, wPort, pchPwd);
}

BOOL32 CRedisWrap::IsConnected()
{
	return m_pcRedisImplement->IsConnected();
}

BOOL32 CRedisWrap::LoadAllScript(const s8** pchScriptPathArry, const u32 dwScriptNum,
	vector<string>* pvecFailedScript/*=NULL*/)
{
	return m_pcRedisImplement->LoadAllScript(pchScriptPathArry, dwScriptNum, pvecFailedScript);
}

BOOL32 CRedisWrap::LoadAllScript(const vector<string>& vecScriptPath, 
	vector<string>* pvecFailedScript/*=NULL*/)
{
	return m_pcRedisImplement->LoadAllScript(vecScriptPath, pvecFailedScript);
}

BOOL32 CRedisWrap::LoadScript(const s8* achFileName)
{
	return m_pcRedisImplement->LoadScript(achFileName);
}

BOOL32 CRedisWrap::EvalScript(const s8* pchFilePath, const std::vector<const s8*>& vecKeys,
	const std::vector<const s8*>& vecArgs, CRedisResult* pResult, BOOL32 bReload)
{
	return m_pcRedisImplement->EvalScript(pchFilePath, vecKeys, vecArgs, pResult, bReload);
}
