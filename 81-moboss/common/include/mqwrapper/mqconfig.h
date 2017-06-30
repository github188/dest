
#ifndef _MQ_CONFIG_
#define _MQ_CONFIG_

#include <stdio.h>
#include <string.h>
#include "kdvtype.h"
#include "constmqwrapper.h"

#ifdef WIN32
#ifndef vsnprintf
#define vsnprintf   _vsnprintf
#endif
#ifndef snprintf
#define snprintf    _snprintf
#endif
#endif


// rabbitmq��Ϣ
class CMqConfig
{
public:
    CMqConfig( void )
    {
        Init();
    }

    //�ÿ�
    void Init()
    {
        memset(m_szMqIP, 0, sizeof(m_szMqIP));
        m_wMqPort = 0;
        memset(m_szVhost, 0, sizeof(m_szVhost));
        memset(m_szUserName, 0, sizeof(m_szUserName));
        memset(m_szUserPwd, 0, sizeof(m_szUserPwd));
        memset(m_szBindingkey, 0, sizeof(m_szBindingkey));
    }

    const s8* GetIP() const { return m_szMqIP; }
    void SetIP(const s8* szIp) 
    {
        if (NULL == szIp)
        {
            memset(m_szMqIP, 0, sizeof(m_szMqIP));
            return;
        }

        strncpy(m_szMqIP, szIp, sizeof(m_szMqIP)-1);
        return; 
    }
    void SetDBIP(u32 dwDBIp/*������*/) 
    {
        u8 *p = (u8 *)&dwDBIp;
        snprintf(m_szMqIP, sizeof(m_szMqIP), "%u.%u.%u.%u", p[0],p[1],p[2],p[3]);
        m_szMqIP[ sizeof(m_szMqIP) -1 ] = '\0';

        return; 
    }

    u16 GetPort() const { return m_wMqPort; }
    void SetPort(u16 wPort) { m_wMqPort = wPort; }

    const s8* GetVHost() const { return m_szVhost; }
    void SetVhost(const s8* szHost) 
    {
        if (NULL == szHost)
        {
            memset(m_szVhost, 0, sizeof(m_szVhost));
            return;
        }

        strncpy(m_szVhost, szHost, sizeof(m_szVhost)-1);
        return; 
    }

    const s8* GetUserPwd() const { return m_szUserPwd; }
    void SetUserPwd(const s8* szPwd)
    { 
        if (NULL == szPwd)
        {
            memset(m_szUserPwd, 0, sizeof(m_szUserPwd));
            return;
        }

        strncpy(m_szUserPwd, szPwd, sizeof(m_szUserPwd)-1);
        return; 
    }

    const s8* GetUsername() const {return m_szUserName; }
    void SetUsername(const s8* szUsername) 
    { 
        if (NULL == szUsername)
        {
            memset(m_szUserName, 0, sizeof(m_szUserName));
            return;
        }

        strncpy(m_szUserName, szUsername, sizeof(m_szUserName)-1);
        return; 
    }

    const s8* GetExchangeName() const { return m_szExchangeName; }
    void SetExchangeName(const s8* szName) 
    {
        if (NULL == szName)
        {
            memset(m_szExchangeName, 0, sizeof(m_szExchangeName));
            return;
        }

        strncpy(m_szExchangeName, szName, sizeof(m_szExchangeName)-1);
        return; 
    }

    const s8* GetQueueName() const { return m_szQueueName; }
    void SetQueueName(const s8* szName) 
    { 
        if (NULL == szName)
        {
            memset(m_szQueueName, 0, sizeof(m_szQueueName));
            return;
        }

        strncpy(m_szQueueName, szName, sizeof(m_szQueueName)-1);
        return; 
    }

    const s8* GetBindingkey() const { return m_szBindingkey; }
    void SetBindingkey(const s8* szKey) 
    { 
        if (NULL == szKey)
        {
            memset(m_szBindingkey, 0, sizeof(m_szBindingkey));
            return;
        }

        strncpy(m_szBindingkey, szKey, sizeof(m_szBindingkey)-1);
        return;
    }

private:
    s8 m_szMqIP[MQWRAPPER_MAXLEN_IP+1];   //MQ������IP
    u16 m_wMqPort;       //MQ�������˿�
    s8 m_szVhost[MQWRAPPER_MAXLEN_VHOST+1];  //��������
    s8 m_szUserName[MQWRAPPER_MAXLEN_USER_NAME+1]; //�û���
    s8 m_szUserPwd[MQWRAPPER_MAXLEN_USER_PWD+1];  //����
    s8 m_szExchangeName[MQWRAPPER_MAXLEN_EXCHANGE_NAME+1];  //Exchange��    
    s8 m_szQueueName[MQWRAPPER_MAXLEN_QUEUE_NAME+1];  //Queue��(�����߲�����)
    s8 m_szBindingkey[MQWRAPPER_MAXLEN_ROUTINGKEY+1];  //Bindingkey(�����߲��Ҷ�����ҪBindʱ������)
};

//�������ߵ�MqConfig������Ϣ
class CMqCfgWithProducer:public CMqConfig
{
public:
	CMqCfgWithProducer()
	{

	}

	virtual ~CMqCfgWithProducer()
	{

	}

	BOOL32 IsUseProducer()const
	{
		return strlen(m_szRoutingKey) > 0? TRUE: FALSE;
	}
	void SetRoutintKey(s8 *pszRoutingKey)
	{
		if ( pszRoutingKey )
		{
			u32 dwLen = strlen(pszRoutingKey);
			if ( dwLen > MQWRAPPER_MAXLEN_ROUTINGKEY )
			{
				dwLen = MQWRAPPER_MAXLEN_ROUTINGKEY;
			}
			memcpy(m_szRoutingKey, pszRoutingKey, dwLen);
			m_szRoutingKey[dwLen] = 0;
		}
	}

	const s8 *GetRoutingKey() const
	{
		return (const s8*)m_szRoutingKey;
	}

	void SetPostBuf(s8 *pszPostBuf)
	{
		if ( pszPostBuf )
		{
			u32 dwLen = strlen(pszPostBuf);
			if ( dwLen > MQWRAPPER_MAXLEN_POSTBUF )
			{
				dwLen = MQWRAPPER_MAXLEN_POSTBUF-1;
			}
			memcpy(m_szPostBuf, pszPostBuf, dwLen);
			m_szPostBuf[dwLen] = 0;
		}
	}
	
	const s8 *GetPostBuf() const
	{
		return (const s8*)m_szPostBuf;
	}

	u32 GetPostBufLen() const
	{
		return strlen(m_szPostBuf);
	}
private:
	s8 m_szRoutingKey[MQWRAPPER_MAXLEN_ROUTINGKEY+1];   //����Ŀ�ĵ�RoutingKey��ֻҪ������ͱ���Ҫ��������
	s8 m_szPostBuf[MQWRAPPER_MAXLEN_POSTBUF];           //����Ŀ�ĵ����ݣ���ҵ����
};

#endif  //_MQ_CONFIG_

