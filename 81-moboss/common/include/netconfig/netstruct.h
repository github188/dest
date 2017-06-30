/*****************************************************************************
   模块名      : netconfig
   文件名      : netstruct.h
   相关文件    : 
   文件实现功能: 网络配置相关参数
   作者        : 
   版本        : V4.0  Copyright( C) 2006-2008 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人        修改内容
   2005/08/17  4.0         liuhuiyun     创建
   2006/11/02  4.0         张宝卿        数据结构优化
******************************************************************************/
#ifndef  _NET_STRUCT_H_
#define  _NET_STRUCT_H_


#include "osp.h"
#include "kdvsys.h"
#include "netconst.h"

#ifdef WIN32
#pragma pack( push )
#pragma pack( 1 )
#endif


// 由于Linux在inet_addr时保护 "0"认为是错误地址，所以需要保护一下
// 建议所有Linux相关的地址转换使用本宏。
// 本宏要求字符串以\0结尾
#define INET_ADDR( szAddr )   ( strncmp(szAddr, "0", strlen(szAddr)) == 0 ? 0 : inet_addr(szAddr) )


#ifndef getmin
#define getmin(a,b) ((a)>(b)?(b):(a))
#endif
#ifndef getmax
#define getmax(a,b) ((a)>(b)?(a):(b))
#endif





#define  SUCCESS_AGENT				(u16)0		// 正确

// 错误码定义
#define  ERR_AGENT_ERRNO_BGN        (u8)(100)

#define  ERR_AGENT_SETNODEVALUE		(u16)(ERR_AGENT_ERRNO_BGN+1)		// 设置节点错误
#define  ERR_AGENT_GETNODEVALUE		(u16)(ERR_AGENT_ERRNO_BGN+2)		// 取节点值错误
#define  ERR_AGENT_CANNOTCHANGE		(u16)(ERR_AGENT_ERRNO_BGN+3)		// 改节点不能修改
#define  ERR_AGENT_WRITEFILE		(u16)(ERR_AGENT_ERRNO_BGN+4)		// 写文件失败
#define  ERR_AGENT_SETMASKIP		(u16)(ERR_AGENT_ERRNO_BGN+5)		// 设置掩码地址失败
#define  ERR_AGENT_SETGWIP			(u16)(ERR_AGENT_ERRNO_BGN+6)		// 设置网关地址失败
#define  ERR_AGENT_SETMPCIP			(u16)(ERR_AGENT_ERRNO_BGN+7)		// 设置MpcIp地址失败
#define  ERR_AGENT_GETCPURATE		(u16)(ERR_AGENT_ERRNO_BGN+8)		// 取Cpu占用率
#define  ERR_AGENT_GETQOSIPTYPE		(u16)(ERR_AGENT_ERRNO_BGN+9)		// 取QosIp服务类型错误
#define  ERR_AGENT_GETTABLEVALUE	(u16)(ERR_AGENT_ERRNO_BGN+10)		// 取表值错误
#define  ERR_AGENT_GETPERIINFO		(u16)(ERR_AGENT_ERRNO_BGN+11)		// 取外设信息失败
#define  ERR_AGENT_SEMTAKE			(u16)(ERR_AGENT_ERRNO_BGN+12)		// 取信号量错误
#define  ERR_AGENT_VALUEBESET       (u16)(ERR_AGENT_ERRNO_BGN+13)       // 设置的参数不合法
#define  ERR_AGENT_SETLOGININFO		(u16)(ERR_AGENT_ERRNO_BGN+14)		// 设置LoginInfo错误
#define  ERR_AGENT_GETLOGININFO		(u16)(ERR_AGENT_ERRNO_BGN+15)		// 取LoginInfo错误
#define  ERR_AGENT_SETOTHERMODULEINFO   (u16)(ERR_AGENT_ERRNO_BGN+16)   // 设置TOtherModuleInfo错误
#define  ERR_AGENT_GETOTHERMODULEINFO   (u16)(ERR_AGENT_ERRNO_BGN+17)   // 取TOtherModuleInfo错误
#define  ERR_AGENT_SETSYSTIME       (u16)(ERR_AGENT_ERRNO_BGN+18)       //设置MCU系统时间失败
#define  ERR_AGENT_SETNETPARAM      (u16)(ERR_AGENT_ERRNO_BGN+19)       //设置网络参数
#define  ERR_AGENT_GETNETPARAM      (u16)(ERR_AGENT_ERRNO_BGN+20)       //取得网络参数

/*=============================================================================
模板类：
类名： Link
作用： 链表节点
说明： 如有特别初始化需要，类T应自行提供相应的无参数构造函数
=============================================================================*/
template <class T>
class ArrayNode
{
private:
    T m_Data;                                   // 用于保存结点元素的内容
    u32 m_dwNextId;                             // 指向后继结点的指针
	
public:      
    ArrayNode() { m_dwNextId = 0; }    
    ArrayNode(const T &Data, u32 dwNextId = 0)
    {
        SetData( Data );
        SetNext( dwNextId );
    }
    
    T * GetData( void ) { return &m_Data; }    
    void SetData( const T &Data ) { m_Data = Data; }    
    u32 GetNext( void ) { return ntohl(m_dwNextId); }
    void SetNext( u32 dwNextId ) { m_dwNextId = htonl(dwNextId); }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;




/*=============================================================================
模板类：
类名： LinkArray
作用： 构造一个单向链表
说明： 1、如有特别初始化需要，类T应自行提供相应的无参数构造函数
2、元素T应支持重载运算符 == : 该要求取消
3、2009-02-26：取消 IsExist 和 GetPos 两函数，因为这两个函数需要 类T 重载
运算符 == ，通用性不强
=============================================================================*/
template <class T, u32 dwArrayLen=1>
class LinkArray
{
private: 
    ArrayNode<T> m_atLinkArray[dwArrayLen];
    u32     m_dwHeadId;
    u32     m_dwTailId;                                  // 单链表的头、尾ID号, 索引号等于ID号减1

private: 
    u32     GetIdByLink( ArrayNode<T> * pCur )
    {
        u32 dwArraySize = sizeof(m_atLinkArray)/sizeof(m_atLinkArray[0]);
        u32 dwIdx = 0;
        for ( dwIdx = 0; dwIdx < dwArraySize; dwIdx++ )
        {
            if ( pCur == &(m_atLinkArray[dwIdx]) )
            {
                return (dwIdx+1);
            }
        } 
        return 0;
    }
    ArrayNode<T> * GetLinkById( u32 dwId )
    {
        if ( 0 == dwId || dwId > sizeof(m_atLinkArray)/sizeof(m_atLinkArray[0]) )
        {
            return NULL;
        }
        ArrayNode<T> * pCur = &(m_atLinkArray[dwId-1]);
        return pCur;
    }

    u32     GetHeadId( void ) const { return ntohl(m_dwHeadId); }
    void    SetHeadId( u32 dwHeadId ) { m_dwHeadId = htonl(dwHeadId); }

    u32     GetTailId( void ) const { return ntohl(m_dwTailId); }
    void    SetTailId( u32 dwTailId ) { m_dwTailId = htonl(dwTailId); }

    // 获得空闲存储空间ID
    u32 GetIdleId(void)
    {
        u32 dwArraySize = sizeof(m_atLinkArray)/sizeof(m_atLinkArray[0]);
        u32 dwIdx = 0;
        for ( dwIdx = 0; dwIdx < dwArraySize; dwIdx++ )
        {
            if ( 0 == m_atLinkArray[dwIdx].GetNext() &&
                GetTailId() != (dwIdx+1) )
            {
                return (dwIdx+1);
            }
        }
        return 0;
    }
     // 返回线性表指向第p个元素的指针值
    ArrayNode<T> * GetLink(const u32 &dwPos)   
    {
        if ( dwPos < 0 || dwPos >= Length() )
        {
            return NULL;
        }
        
        u32 dwCount = 0;
        ArrayNode<T> *pCur = GetLinkById( GetHeadId() );
        while ( pCur != NULL && dwCount < dwPos )
        {
            u32 dwNextId = pCur->GetNext();
            pCur = GetLinkById(dwNextId);
            dwCount++;
        }       
        return pCur;
    }   

public: 
    LinkArray() { Clear(); }         // 构造函数
    ~LinkArray() { Clear(); }        // 析构函数
    // 判断链表是否为空
    BOOL32 IsEmpty() const { return ( 0 == m_dwHeadId || 0 == m_dwTailId ); }
    // 将链表存储的内容清除，成为空表
    void   Clear()                                    
    {
        m_dwHeadId = 0;
        m_dwTailId = 0;
        u32 dwArraySize = sizeof(m_atLinkArray)/sizeof(m_atLinkArray[0]);
        u32 dwIdx = 0;
        for ( dwIdx = 0; dwIdx < dwArraySize; dwIdx++ )
        {
            m_atLinkArray[dwIdx].SetNext( 0 );
        }
    }
    // 返回此顺序表的当前实际长度
    u32 Length()
    {
        ArrayNode<T> *pCur = GetLinkById( GetHeadId() );
        u32 dwCount = 0;
        
        while ( pCur != NULL ) 
        {
            u32 dwNextId = pCur->GetNext();
            pCur = GetLinkById(dwNextId);
            dwCount++;
        }
        return dwCount;
    }    
    // 在表尾添加一个元素value，表的长度增1
    BOOL32 Append(const T &Value)                      
    {
        u32 dwIdleId = GetIdleId();
        ArrayNode<T> *pCur = GetLinkById( dwIdleId );
        if ( NULL == pCur )
        {
            return FALSE;
        }       
        pCur->SetData( Value );
        pCur->SetNext( 0 );
        
        u32 dwOldTailId = GetTailId();
        SetTailId(dwIdleId);

        ArrayNode<T> * pTail = GetLinkById( dwOldTailId );
        if ( NULL != pTail )
        {
            pTail->SetNext( dwIdleId );
        }
        else
        {
            SetHeadId(dwIdleId);
        }        
        return TRUE;
    }
    // 在第dwPos个位置插入数据内容为Value的新结点
    BOOL32 Insert(const u32 &dwPos, const T &Value)    
    {
        ArrayNode<T> *pCur = NULL;
        ArrayNode<T> *pNext = NULL;
        
        u32 dwIdleId = GetIdleId();
        pNext = GetLinkById( dwIdleId ); 
        if ( NULL == pNext )
        {
            return FALSE;
        }
        
        if ( dwPos == 0 )
        {
            pNext->SetData( Value );
            pNext->SetNext( GetHeadId() );
            SetHeadId( dwIdleId );
            if ( 0 == GetTailId() )
            {
                // 原来的表为空
                SetTailId( dwIdleId );
            }
        }
        else 
        {
            if ( (pCur = GetLink(dwPos-1) ) == NULL) 
            {                                                     
                return FALSE;
            }
            pNext->SetData( Value );
            pNext->SetNext( pCur->GetNext() );
            pCur->SetNext( dwIdleId );
            u32 dwPreId = GetIdByLink(pCur);
            if ( dwPreId == GetTailId() )                                          
            {
                SetTailId( dwIdleId );
            }
        }    
        return TRUE;
    }
    // 删除位置i上的元素，表的长度减 1
    BOOL32 Delete(const u32 &dwPos)                    
    {
        ArrayNode<T> *pCur = NULL;
        ArrayNode<T> *pPre = NULL; 
        
        pCur = GetLink( dwPos );
        if ( NULL == pCur )
        {
            return FALSE;
        }
        if ( dwPos > 0 )
        {
            pPre = GetLink( dwPos-1 );
            if ( NULL == pPre )
            {
                return FALSE;
            }
        }

        u32 dwNext = pCur->GetNext();
        pCur->SetNext( 0 );
        if ( NULL == pPre )
        {                           
            SetHeadId( dwNext );                  
        }
        else
        {
            pPre->SetNext( dwNext );         
        }
        if ( GetTailId() == GetIdByLink(pCur) )
        {
            if ( dwNext != 0 )
            {
                SetTailId( dwNext );
            }
            else
            {
                SetTailId( GetIdByLink(pPre) );
            }
        } 
        return TRUE;
    }

    // 返回位置i的元素值 
    T * GetValue(const u32 &dwPos)           
    {
        ArrayNode<T> *p = GetLink(dwPos);
        if ( p == NULL )
        {
            return NULL;
        }
        else
        {
            return p->GetData();
        }
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


 
//网卡信息结构
struct TMINIMCUNetParam
{
public:
    enum enNetworkType
    {
        enInvalid = 0,
            enWAN = 1,
            enLAN = 2
    };
protected:
    u8  m_byAdapterIndex;       // 网卡序号
	u32 m_dwIpAddr;		        // 网络序
	u32 m_dwIpMask;		        // 网络序
	u32 m_dwGatewayIp;          // 网络序
	s8  m_asServerAlias[MAXLEN_PWD]; // 运营商别名
    u8  m_byNetworkType;    // 网络地址类型，LAN or WAN    
public:
	TMINIMCUNetParam():m_dwIpAddr(0),
					   m_dwIpMask(0),
					   m_dwGatewayIp(0),
                       m_byNetworkType(enInvalid)
	{
		memset(	m_asServerAlias, 0, sizeof(m_asServerAlias) );
	}

public:
	void SetNetParam( u8 byNetworkType,  u32 dwIp, u32 dwIpMask, u32 dwGW, LPCSTR lpszAlias )
	{
		m_byNetworkType = byNetworkType;
		m_dwIpAddr = htonl(dwIp);
		m_dwIpMask = htonl(dwIpMask); 
		m_dwGatewayIp = htonl(dwGW);

		if (NULL != lpszAlias)
		{
			strncpy(m_asServerAlias, lpszAlias, MAXLEN_PWD-1);
			m_asServerAlias[MAXLEN_PWD-1] = 0;
		}
		else
		{
			memset(	m_asServerAlias, 0, sizeof(m_asServerAlias) );
		}
	}
    // 设置为公网地址
    void SetWanParam( u32 dwIp, u32 dwIpMask, u32 dwGW, LPCSTR lpszAlias )
    {
        m_byNetworkType = enWAN;
        m_dwIpAddr = htonl(dwIp);
        m_dwIpMask = htonl(dwIpMask); 
        m_dwGatewayIp = htonl(dwGW);

        if (NULL != lpszAlias)
        {
            strncpy(m_asServerAlias, lpszAlias, MAXLEN_PWD-1);
            m_asServerAlias[MAXLEN_PWD-1] = 0;
        }
        else
        {
            memset(	m_asServerAlias, 0, sizeof(m_asServerAlias) );
        }
    }

    // 设置为局域网地址
    void SetLanParam( u32 dwIp, u32 dwMask, u32 dwGW ,LPCSTR lpszAlias = NULL )
    {
        m_byNetworkType = enLAN;
        m_dwIpAddr = htonl(dwIp);
        m_dwIpMask = htonl(dwMask);
        m_dwGatewayIp = htonl(dwGW);

        // 局域网不需要别名
		 if (NULL != lpszAlias)
        {
            strncpy(m_asServerAlias, lpszAlias, MAXLEN_PWD-1);
            m_asServerAlias[MAXLEN_PWD-1] = 0;
        }
        else
        {
            memset(	m_asServerAlias, 0, sizeof(m_asServerAlias) );
        }
    }

    BOOL32 operator == (const TMINIMCUNetParam & tNetParam)
    {
        if ( this->GetIpAddr() == tNetParam.GetIpAddr() &&
             this->GetIpMask() == tNetParam.GetIpMask() &&
             this->GetGatewayIp() == tNetParam.GetGatewayIp() &&
             this->GetAdapterIdx() == tNetParam.GetAdapterIdx() &&
             this->GetNetworkType() == tNetParam.GetNetworkType() &&
             ( 0 == strcmp( this->GetServerAlias(), tNetParam.GetServerAlias() ) ) )
        {
            return TRUE;
        }
        return FALSE;
    }

    BOOL32 IsValid() const
    {
        if ( 0 == m_dwIpAddr || enInvalid == m_byNetworkType || m_byAdapterIndex >= MCU_MAXNUM_ADAPTER )
        {
            return FALSE;
        }
        return TRUE;
    }

    // 获取网络类型
    u8   GetNetworkType() const
    {
        return m_byNetworkType;
    }

    u8 IsWan() const
    {
        return (m_byNetworkType == enWAN);
    }
    u8 IsLan() const
    {
        return (m_byNetworkType == enLAN);
    }

    // 获取地址参数
    u32  GetIpAddr(void) const { return ntohl(m_dwIpAddr); }
	u32  GetIpMask(void) const{ return ntohl(m_dwIpMask); }
    u32  GetGatewayIp(void) const{ return ntohl(m_dwGatewayIp); }

    // 获取别名
    const s8 * GetServerAlias(void) const{ return m_asServerAlias; }

    void SetAdapterIdx( u8 byIdx ) { m_byAdapterIndex = byIdx; }
    u8   GetAdapterIdx( void ) const { return m_byAdapterIndex; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
 
 
 
struct TMINIMCUNetParamAll
{
public:
    LinkArray< TMINIMCUNetParam, MCU_MAXNUM_ADAPTER_IP*MCU_MAXNUM_ADAPTER > m_tLinkNetParam;

public:
	TMINIMCUNetParamAll()
	{
		memset( &m_tLinkNetParam, 0, sizeof(m_tLinkNetParam) );
	}

	BOOL32 IsValid()
	{
		return ( m_tLinkNetParam.Length() != 0 );
	}

    BOOL32 IsNetParamIn(TMINIMCUNetParam &tNetParam)
    {
        if ( !tNetParam.IsValid() )
        {
            return FALSE;
        }

        u32 dwLength = m_tLinkNetParam.Length();
        for ( u32 dwIdx = 0; dwIdx < dwLength; dwIdx++ )
        {
            TMINIMCUNetParam * ptNetParam = m_tLinkNetParam.GetValue(dwIdx);
            if ( NULL != ptNetParam )
            {
                TMINIMCUNetParam tLocalParam = *ptNetParam;
                if ( tLocalParam == tNetParam )
                {
                    return TRUE;
                }
            }
        }

        return FALSE;
    }

	// dwIpInfo 为主机序
	BOOL32 IsIpIn( u32 dwIpInfo )
	{
		if( 0 == dwIpInfo )
		{
			return FALSE;
		}

		u32 dwLength = m_tLinkNetParam.Length();
		for( u32 dwIdx = 0 ; dwIdx < dwLength; dwIdx++ )
		{
			TMINIMCUNetParam * ptNetParam = m_tLinkNetParam.GetValue( dwIdx );
			if( NULL != ptNetParam )
			{
				if( ptNetParam->GetIpAddr() == dwIpInfo )
				{
					return TRUE;
				}
			}
		}

		return FALSE;
	}

    TMINIMCUNetParamAll & operator = (TMINIMCUNetParamAll & tNewNetParamAll)
    {
        if ( this != &tNewNetParamAll )
        {
            Clear();
            u32 dwIdx = 0;
            for ( dwIdx = 0; dwIdx < tNewNetParamAll.m_tLinkNetParam.Length(); dwIdx++ )
            {
                TMINIMCUNetParam * ptNetParam = tNewNetParamAll.m_tLinkNetParam.GetValue(dwIdx);
                if ( NULL != ptNetParam )
                {
                    TMINIMCUNetParam tNewNetParam = *ptNetParam;
                    this->m_tLinkNetParam.Append( tNewNetParam );
                }
            }
        }
        return *this;
    }

    BOOL32 operator == (TMINIMCUNetParamAll & tNewNetParamAll)
    {
        u32 dwNewIdx = 0;
        u32 dwLocalIdx = 0;
        u32 dwLocalLength = this->m_tLinkNetParam.Length();
        u32 dwNewLength = tNewNetParamAll.m_tLinkNetParam.Length();
        
        if ( dwLocalLength != dwNewLength )
        {
            return FALSE;
        }

        // new的都在local里
        for ( dwNewIdx = 0; dwNewIdx < dwNewLength; dwNewIdx++ )
        {
            TMINIMCUNetParam * ptNewNetParam = tNewNetParamAll.m_tLinkNetParam.GetValue(dwNewIdx);
            if ( NULL != ptNewNetParam )
            {
                TMINIMCUNetParam tNewNetParam = *ptNewNetParam;
                if ( !this->IsNetParamIn(tNewNetParam) )
                {
                    return FALSE;
                }
            }
        }

        //local都在new里
        for ( dwLocalIdx = 0; dwLocalIdx < dwLocalLength; dwLocalIdx++ )
        {
            TMINIMCUNetParam * ptLocalNetParam = this->m_tLinkNetParam.GetValue(dwLocalIdx);
            if ( NULL != ptLocalNetParam )
            {
                TMINIMCUNetParam tLocalNetParam = *ptLocalNetParam;
                if ( !tNewNetParamAll.IsNetParamIn(tLocalNetParam) )
                {
                    return FALSE;
                }
            }
        }
        return TRUE;
    }

    u8 GetNetParamNum(void)
    {
        return (u8)m_tLinkNetParam.Length();
    }

    u8 GetLANParamNum(void)
    {
        u8 byNum = 0;
        u8 byParamNum = GetNetParamNum();
        for ( u8 byLop = 0; byLop < byParamNum; byLop++ )
        {
            TMINIMCUNetParam * ptMcuNetParam = m_tLinkNetParam.GetValue( (u32)byLop );
            if ( NULL == ptMcuNetParam )
            {
                continue;
            }
            if ( ptMcuNetParam->IsLan() )
            {
                byNum++;
            }
        }
        return byNum;
    }
    BOOL32 AddOneNetParam( TMINIMCUNetParam &tNetParam )
    {
        if ( !tNetParam.IsValid() )
        {
            return FALSE;
        }
        if ( IsNetParamIn(tNetParam) )
        {
            return FALSE;
        }
        m_tLinkNetParam.Append(tNetParam);
        return TRUE;
    }
    BOOL32 InsertOneNetParam( const u32 &dwPos, TMINIMCUNetParam &tNetParam )
    {
//        if ( !tNetParam.IsValid() )
//        {
//            return FALSE;
//        }
        if ( IsNetParamIn(tNetParam) )
        {
            return FALSE;
        }
        m_tLinkNetParam.Insert( dwPos, tNetParam);        
        return TRUE;
    }

    BOOL32 GetNetParambyIdx ( u8 byIdx, TMINIMCUNetParam &tNetParam )
    {
        memset( &tNetParam, 0, sizeof(TMINIMCUNetParam) );
        if ( byIdx >= m_tLinkNetParam.Length() )
        {
            return FALSE;
        }
        TMINIMCUNetParam * ptMcuNetParam = m_tLinkNetParam.GetValue(byIdx);
        if ( NULL == ptMcuNetParam )
        {
            return FALSE;
        }
        memcpy( &tNetParam, ptMcuNetParam, sizeof(TMINIMCUNetParam) );
        return TRUE;
    }
    BOOL32 DelNetParamByIdx(u8 byIdx)
    {
        BOOL32 bRet = m_tLinkNetParam.Delete(byIdx);   
        return bRet;
    }
    BOOL32 IsEqualTo( TMINIMCUNetParamAll &tNewNetParamAll )
    {
        u32 dwNewIdx = 0;
        u32 dwLocalIdx = 0;
        u32 dwLocalLength = this->m_tLinkNetParam.Length();
        u32 dwNewLength = tNewNetParamAll.m_tLinkNetParam.Length();
        
        if ( dwLocalLength != dwNewLength )
        {
            return FALSE;
        }
        
        // new的都在local里
        for ( dwNewIdx = 0; dwNewIdx < dwNewLength; dwNewIdx++ )
        {
            TMINIMCUNetParam * ptNewNetParam = tNewNetParamAll.m_tLinkNetParam.GetValue(dwNewIdx);
            if ( NULL != ptNewNetParam )
            {
                TMINIMCUNetParam tNewNetParam = *ptNewNetParam;
                if ( !this->IsNetParamIn(tNewNetParam) )
                {
                    return FALSE;
                }
            }
        }
        
        //local都在new里
        for ( dwLocalIdx = 0; dwLocalIdx < dwLocalLength; dwLocalIdx++ )
        {
            TMINIMCUNetParam * ptLocalNetParam = this->m_tLinkNetParam.GetValue(dwLocalIdx);
            if ( NULL != ptLocalNetParam )
            {
                TMINIMCUNetParam tLocalNetParam = *ptLocalNetParam;
                if ( !tNewNetParamAll.IsNetParamIn(tLocalNetParam) )
                {
                    return FALSE;
                }
            }
        }
        return TRUE;
	}
	
	void Clear(void)
	{
		memset( this, 0, sizeof(TMINIMCUNetParamAll) );
	}
    
    void Print(void)
    {
        u32 dwLength = m_tLinkNetParam.Length();
        for ( u32 dwIdx = 0; dwIdx < dwLength; dwIdx++ )
        {
            TMINIMCUNetParam * ptNetParam = m_tLinkNetParam.GetValue(dwIdx);
            if ( NULL != ptNetParam )
            {
                OspPrintf( TRUE, FALSE, "McuNetParam.%d --> AdapterIdx.%d, NetType.%d, IpAddr.0x%x, IpMask.0x%x, Gateway.0x%x, ServerAlias.%s\n",
                           dwIdx,
                           ptNetParam->GetAdapterIdx(),
                           ptNetParam->GetNetworkType(),
                           ptNetParam->GetIpAddr(),
                           ptNetParam->GetIpMask(),
                           ptNetParam->GetGatewayIp(),
                           ptNetParam->GetServerAlias() );
            }
        }
        return;
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

 

struct TMINIMCUNetParam_Mcu : public TMINIMCUNetParam
{
protected:
    s8 m_achAdapterName[MCU_MAX_ADAPTER_NAME_LENGTH + 4];	//网络适配器名称

public:
    TMINIMCUNetParam_Mcu()
    {
        memset( m_achAdapterName, 0, sizeof(m_achAdapterName) );
    }

    void SetAdapterName( const s8 * achAdapterName )
    {
        if ( NULL == achAdapterName )
        {
            memset( m_achAdapterName, 0, sizeof(m_achAdapterName) );
        }
        else
        {
            strncpy( m_achAdapterName, achAdapterName, sizeof(m_achAdapterName) );
        }
        return;
    }
    const s8 * GetAdapterName( void )
    {
        return m_achAdapterName;
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMINIMCUNetParamAll_Mcu
{
public:
    LinkArray< TMINIMCUNetParam_Mcu, MCU_MAXNUM_ADAPTER_IP*MCU_MAXNUM_ADAPTER > m_tLinkNetParam;

public:
	TMINIMCUNetParamAll_Mcu()
	{
		memset( &m_tLinkNetParam, 0, sizeof(m_tLinkNetParam) );
	}

	BOOL32 IsValid()
	{
		return ( m_tLinkNetParam.Length() != 0 );
	}

    BOOL32 IsNetParamIn(TMINIMCUNetParam_Mcu &tNetParam)
    {
        if ( !tNetParam.IsValid() )
        {
            return FALSE;
        }

        u32 dwLength = m_tLinkNetParam.Length();
        for ( u32 dwIdx = 0; dwIdx < dwLength; dwIdx++ )
        {
            TMINIMCUNetParam_Mcu * ptNetParam = m_tLinkNetParam.GetValue(dwIdx);
            if ( NULL != ptNetParam )
            {
                TMINIMCUNetParam tLocalParam = (TMINIMCUNetParam)*ptNetParam;
                if ( tLocalParam == (TMINIMCUNetParam)tNetParam &&
                     0 == strcmp(tNetParam.GetAdapterName(), ptNetParam->GetAdapterName()) )
                {
                    return TRUE;
                }
            }
        }

        return FALSE;
    }

    TMINIMCUNetParamAll_Mcu & operator = (TMINIMCUNetParamAll_Mcu & tNewNetParamAll)
    {
        if ( this != &tNewNetParamAll )
        {
            Clear();
            u32 dwIdx = 0;
            for ( dwIdx = 0; dwIdx < tNewNetParamAll.m_tLinkNetParam.Length(); dwIdx++ )
            {
                TMINIMCUNetParam_Mcu * ptNetParam = tNewNetParamAll.m_tLinkNetParam.GetValue(dwIdx);
                if ( NULL != ptNetParam )
                {
                    TMINIMCUNetParam_Mcu tNewNetParam = *ptNetParam;
                    this->m_tLinkNetParam.Append( tNewNetParam );
                }
            }
        }
        return *this;
    }

    BOOL32 operator == (TMINIMCUNetParamAll_Mcu & tNewNetParamAll)
    {
        u32 dwNewIdx = 0;
        u32 dwLocalIdx = 0;
        u32 dwLocalLength = this->m_tLinkNetParam.Length();
        u32 dwNewLength = tNewNetParamAll.m_tLinkNetParam.Length();
        
        if ( dwLocalLength != dwNewLength )
        {
            return FALSE;
        }

        // new的都在local里
        for ( dwNewIdx = 0; dwNewIdx < dwNewLength; dwNewIdx++ )
        {
            TMINIMCUNetParam_Mcu * ptNewNetParam = tNewNetParamAll.m_tLinkNetParam.GetValue(dwNewIdx);
            if ( NULL != ptNewNetParam )
            {
                TMINIMCUNetParam_Mcu tNewNetParam = *ptNewNetParam;
                if ( !this->IsNetParamIn(tNewNetParam) )
                {
                    return FALSE;
                }
            }
        }

        //local都在new里
        for ( dwLocalIdx = 0; dwLocalIdx < dwLocalLength; dwLocalIdx++ )
        {
            TMINIMCUNetParam_Mcu * ptLocalNetParam = this->m_tLinkNetParam.GetValue(dwLocalIdx);
            if ( NULL != ptLocalNetParam )
            {
                TMINIMCUNetParam_Mcu tLocalNetParam = *ptLocalNetParam;
                if ( !tNewNetParamAll.IsNetParamIn(tLocalNetParam) )
                {
                    return FALSE;
                }
            }
        }
        return TRUE;
    }

    u32 GetLANParamNum(void)
    {
        u32 dwNum = 0;
        u32 dwParamNum = m_tLinkNetParam.Length();
        for ( u32 dwLop = 0; dwLop < dwParamNum; dwLop++ )
        {
            TMINIMCUNetParam_Mcu * ptMcuNetParam = m_tLinkNetParam.GetValue( (u32)dwLop );
            if ( NULL == ptMcuNetParam )
            {
                continue;
            }
            if ( ptMcuNetParam->IsLan() )
            {
                dwNum++;
            }
        }
        return dwNum;
    }

    u32 GetAdapterNum(void)
    {
        u32 dwNum = 0;
        u32 dwParamNum = m_tLinkNetParam.Length();
        for ( u32 dwLop = 0; dwLop < dwParamNum; dwLop++ )
        {
            TMINIMCUNetParam_Mcu * ptMcuNetParam = m_tLinkNetParam.GetValue( (u32)dwLop );
            if ( NULL == ptMcuNetParam )
            {
                continue;
            }
            const s8 * pchName = ptMcuNetParam->GetAdapterName();
            if ( dwLop == 0 )
            {
                dwNum++;
                continue;
            }
            u32 dwLopBack = dwLop;
            for ( ; dwLopBack > 0; dwLopBack-- )
            {
                TMINIMCUNetParam_Mcu * ptMcuNetParamBack = m_tLinkNetParam.GetValue( (u32)dwLopBack-1 );
                if ( NULL == ptMcuNetParamBack )
                {
                    continue;
                }
                if ( 0 == strcmp( ptMcuNetParamBack->GetAdapterName(), pchName ) )
                {
                    break;
                }
            }
            if ( 0 == dwLopBack )
            {
                dwNum++;
            }
        }
        return dwNum;
    }
    	
	void Clear(void)
	{
		memset( this, 0, sizeof(TMINIMCUNetParamAll_Mcu) );
	}
    
    void Print(void)
    {
        u32 dwLength = m_tLinkNetParam.Length();
        for ( u32 dwIdx = 0; dwIdx < dwLength; dwIdx++ )
        {
            TMINIMCUNetParam_Mcu * ptNetParam = m_tLinkNetParam.GetValue(dwIdx);
            if ( NULL != ptNetParam )
            {
                OspPrintf( TRUE, FALSE, "McuNetParam.%d --> AdapterName.%s, AdapterIdx.%d, NetType.%d, IpAddr.0x%x, IpMask.0x%x, Gateway.0x%x, ServerAlias.%s\n",
                           dwIdx,
                           ptNetParam->GetAdapterName(),
                           ptNetParam->GetAdapterIdx(),
                           ptNetParam->GetNetworkType(),
                           ptNetParam->GetIpAddr(),
                           ptNetParam->GetIpMask(),
                           ptNetParam->GetGatewayIp(),
                           ptNetParam->GetServerAlias() );
            }
        }
        return;
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TNetParam
{
protected:   
    u32 m_dwIpAddr;		// 网络序
    u32 m_dwIpMask;		// 网络序
    u32 m_dwGateway;
	
public:
    TNetParam()
    {
        SetNull();
    }
    
public:
    void SetNetParam( u32 dwIp, u32 dwIpMask )    {
        m_dwIpAddr = htonl(dwIp);
        m_dwIpMask = htonl(dwIpMask);
        
    }
	void SetGateway(u32 dwGateway){m_dwGateway = htonl(dwGateway); }
    void SetNull(void)
    {
        m_dwIpAddr = 0;
        m_dwIpMask = 0;
    }
    
    BOOL32 IsValid() const
    {
        return ( 0 == m_dwIpAddr ) ? FALSE : TRUE ;
    }
    
    // 获取地址参数
    u32  GetIpAddr(void) const { return ntohl(m_dwIpAddr); }
    u32  GetIpMask(void) const{ return ntohl(m_dwIpMask); }
    u32  GetIpGW(void) const{ return ntohl(m_dwGateway); }
	
    void Print(void)
    {
        u32 dwIp = GetIpAddr();
        u32 dwMask = GetIpMask();
        OspPrintf( TRUE, FALSE, "%u.%u.%u.%u/%u.%u.%u.%u\n", 
            (dwIp>>24)&0xff, (dwIp>>16)&0xff,(dwIp>>8)&0xff, dwIp&0xff,
            (dwMask>>24)&0xff, (dwMask>>16)&0xff, (dwMask>>8)&0xff, dwMask&0xff );
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


//网络适配信息
struct TNetAdaptInfo
{
protected:
    s8  m_achAdapterName[MCU_MAX_ADAPTER_NAME_LENGTH + 4];			//网络适配器名称
    s8  m_achDescription[MCU_MAX_ADAPTER_DESCRIPTION_LENGTH + 4];	//网络适配器描述
    u32 m_dwMacAddressLength;										//mac地址长度
    u8  m_abyMacAddress[MCU_MAX_ADAPTER_ADDRESS_LENGTH];			//mac地址
    u32 m_dwIndex;													//网络适配器索引
    u32 m_dwType;													//网络适配器类型

    TNetParam m_tCurrentIpAddress;

public:
    LinkArray<TNetParam, MCU_MAXNUM_ADAPTER_IP> m_tLinkIpAddr;          // 本网卡IP地址
    LinkArray<TNetParam, MCU_MAXNUM_ADAPTER_GW> m_tLinkDefaultGateway;  // 本网卡默认网关

protected:
    void SetMacAddressLength( u32 dwMacLength ) { m_dwMacAddressLength = htonl( dwMacLength ); }
              
public:
    
    TNetAdaptInfo(void) { Clear(); }
    ~TNetAdaptInfo(void) { Clear(); }

    void Clear( void )
    {
        memset( m_achAdapterName, '\0', sizeof(m_achAdapterName) );
        memset( m_achDescription, '\0', sizeof(m_achDescription) );
        memset( m_abyMacAddress, 0, sizeof(m_abyMacAddress) );
        m_dwMacAddressLength = 0;
        m_dwIndex = 0;
        m_dwType = 0;
        
        m_tCurrentIpAddress.SetNull();
        m_tLinkIpAddr.Clear();
        m_tLinkDefaultGateway.Clear();
    }
    void Print( void )
    {
        u32 dwIdx = 0;
        OspPrintf( TRUE, FALSE, "AdapterName: %s\n", GetAdapterName() );
        OspPrintf( TRUE, FALSE, "Description: %s\n", GetDescription() );
        OspPrintf( TRUE, FALSE, "MacAddress : ");
        for ( dwIdx = 0; dwIdx < GetMacAddressLength(); dwIdx++ )
        {
            if ( dwIdx < GetMacAddressLength()-1)
            {
                OspPrintf( TRUE, FALSE, "%02x-", m_abyMacAddress[dwIdx]);
            }
            else
            {
                OspPrintf( TRUE, FALSE, "%02x\n", m_abyMacAddress[dwIdx]);
            }
        }
        OspPrintf( TRUE, FALSE, "AdaptIdx: 0x%x\n", GetAdaptIdx() );
        OspPrintf( TRUE, FALSE, "AdaptType: %d ", GetAdaptType() );
        switch ( GetAdaptType() )    //适配器类型
        {
        case MCU_MIB_IF_TYPE_OTHER:
            OspPrintf( TRUE, FALSE, "Other\n");
            break;
        case MCU_MIB_IF_TYPE_ETHERNET:
            OspPrintf( TRUE, FALSE, "Ethernet\n");
            break;
        case MCU_MIB_IF_TYPE_TOKENRING:
            OspPrintf( TRUE, FALSE, "Tokenring\n");
            break;
        case MCU_MIB_IF_TYPE_FDDI:
            OspPrintf( TRUE, FALSE, "FDDI\n");
            break;            
        case MCU_MIB_IF_TYPE_PPP:
            OspPrintf( TRUE, FALSE, "PPP\n");
            break;
        case MCU_MIB_IF_TYPE_LOOPBACK:
            OspPrintf( TRUE, FALSE, "LoopBack\n");
            break;
        case MCU_MIB_IF_TYPE_SLIP:
            OspPrintf( TRUE, FALSE, "Slip\n");
            break;    
        default:
            OspPrintf( TRUE, FALSE, "Unknow\n");
            break;
        }
        OspPrintf( TRUE, FALSE, "CurrentIpAddress: Ip - 0x%x, Mask - 0x%x\n", 
            m_tCurrentIpAddress.GetIpAddr(),
            m_tCurrentIpAddress.GetIpMask() );
            for ( dwIdx = 0; dwIdx < m_tLinkIpAddr.Length(); dwIdx++ )
            {
                OspPrintf( TRUE, FALSE, "NetAddress[%d]: Ip - 0x%x, Mask - 0x%x\n", dwIdx,
                m_tLinkIpAddr.GetValue( dwIdx )->GetIpAddr(), 
                m_tLinkIpAddr.GetValue( dwIdx )->GetIpMask() );
            }
            for ( dwIdx = 0; dwIdx < m_tLinkDefaultGateway.Length(); dwIdx++ )
            {
                OspPrintf( TRUE, FALSE, "DefaultGW[%d]: Ip - 0x%x\n", dwIdx,
                    m_tLinkDefaultGateway.GetValue( dwIdx )->GetIpAddr() );
            }
    }

    // 网络适配器别名操作
    void SetAdapterName( const s8 * pchAdapterName )
    {
        if ( NULL != pchAdapterName )
        {
            strncpy( m_achAdapterName, pchAdapterName, sizeof(m_achAdapterName)-4 );
            m_achAdapterName[sizeof(m_achAdapterName)-4] = '\0';
        }
        else
        {
            memset( m_achAdapterName, '\0', sizeof(m_achAdapterName) );
        }
        return;
    }
    s8 * GetAdapterName( void )
    {
        return m_achAdapterName; 
    }

    // 网络适配器MAC地址操作
    void SetMacAddress( u8 *pbyMacAddress, u32 dwLength )
    {
        if ( NULL != pbyMacAddress )
        {
            u8 *pbyMac = pbyMacAddress;
            u32 dwLop = 0;
            for ( dwLop = 0; dwLop < dwLength && dwLop < MCU_MAX_ADAPTER_ADDRESS_LENGTH; dwLop++ )
            {
                m_abyMacAddress[dwLop] = *pbyMac;
                pbyMac++;
            }
            SetMacAddressLength( dwLop );
        }
        else
        {
            memset( m_abyMacAddress, 0, sizeof(m_abyMacAddress) );
            SetMacAddressLength( 0 );
        }
        return;
    }
    u8 * GetMacAddress( void ) { return m_abyMacAddress; }

    u32  GetMacAddressLength( void ) const { return ntohl(m_dwMacAddressLength); }

    // 网络适配器描述操作
    void SetDescription( s8 * pchDescription )
    {
        if ( NULL != pchDescription )
        {
            strncpy( m_achDescription, pchDescription, sizeof(m_achDescription)-4 );
            m_achDescription[sizeof(m_achDescription)-4] = '\0';
        }
        else
        {
            memset( m_achDescription, '\0', sizeof(m_achDescription) );
        }
        return;
    }
    s8 * GetDescription( void ) { return m_achDescription; }

    // 网络适配器索引号操作
    void SetAdaptIdx( u32 dwIdx ) {m_dwIndex = htonl(dwIdx);}
    u32  GetAdaptIdx( void ) const { return ntohl(m_dwIndex); }

    // 网络适配器类型
    void SetAdaptType( u32 dwType ) { m_dwType = htonl(dwType); }
    u32  GetAdaptType( void ) const { return ntohl(m_dwType); }

    void SetCurretnIpAddr( const TNetParam &tCurIp ) 
    {
        m_tCurrentIpAddress = tCurIp; 
    }
    TNetParam GetCurrentIpAddr(void) const 
    { 
        return m_tCurrentIpAddress; 
    }

    s32 GetIpAddrPos( u32 dwIpAddr )
    {
        u32 dwIpNum = m_tLinkIpAddr.Length();
        for ( u32 dwIdx = 0; dwIdx < dwIpNum; dwIdx++ )
        {
            TNetParam tLinkParam = *(m_tLinkIpAddr.GetValue(dwIdx) );
            if ( tLinkParam.GetIpAddr() == dwIpAddr )
            {
                return dwIdx;
            }
        }
        return -1;
    }

    s32 GetDefaultGWPos( u32 dwGWAddr )
    {
        u32 dwIpNum = m_tLinkDefaultGateway.Length();
        for ( u32 dwIdx = 0; dwIdx < dwGWAddr; dwIdx++ )
        {
            TNetParam tLinkParam = *(m_tLinkDefaultGateway.GetValue(dwIdx) );
            if ( tLinkParam.GetIpAddr() == dwGWAddr )
            {
                return dwIdx;
            }
        }
        return -1;
    }
    
    TNetAdaptInfo & operator = (TNetAdaptInfo tNetAdapt)
    {
        if ( this != &tNetAdapt )
        {
            this->Clear();
            this->SetAdapterName( tNetAdapt.GetAdapterName() );
            this->SetAdaptIdx( tNetAdapt.GetAdaptIdx() );
            this->SetAdaptType( tNetAdapt.GetAdaptType() );
            this->SetCurretnIpAddr( tNetAdapt.GetCurrentIpAddr() );
            this->SetDescription( tNetAdapt.GetDescription() );
            this->SetMacAddress( tNetAdapt.GetMacAddress(), tNetAdapt.GetMacAddressLength() );

            u32 dwLop = 0;
            u32 dwArrayLen = 0;
            // Ip array
            TNetParam *ptNetAddr = NULL;
            dwArrayLen = tNetAdapt.m_tLinkIpAddr.Length();
            for ( dwLop = 0; dwLop < dwArrayLen; dwLop++ )
            {
                ptNetAddr = tNetAdapt.m_tLinkIpAddr.GetValue(dwLop);
                if ( NULL == ptNetAddr )
                {
                    continue;
                }
                if ( !m_tLinkIpAddr.Append( *ptNetAddr ) )
                {
                    break;
                }
            }
            // GW
            TNetParam *ptGW = NULL;
            dwArrayLen = tNetAdapt.m_tLinkDefaultGateway.Length();
            for ( dwLop = 0; dwLop < dwArrayLen; dwLop++ )
            {
                ptGW = tNetAdapt.m_tLinkDefaultGateway.GetValue(dwLop);
                if ( NULL == ptGW )
                {
                    continue;
                }
                if ( !m_tLinkDefaultGateway.Append( *ptGW ) )
                {
                    break;
                }
            }
        }
        return *this;
    }

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;




struct TNetAdaptInfoAll
{
public:
    LinkArray<TNetAdaptInfo, MCU_MAXNUM_ADAPTER> m_tLinkNetAdapt;
    
public:
    TNetAdaptInfoAll(void) { Clear(); }
    ~TNetAdaptInfoAll(void) { Clear(); }
    
    void Clear(void)
    {
        u32 dwAdaptNum = m_tLinkNetAdapt.Length();
        for ( u32 dwIdx = 0; dwIdx < dwAdaptNum; dwIdx++ )
        {
            TNetAdaptInfo * ptAdaptInfo = m_tLinkNetAdapt.GetValue(dwIdx);
            ptAdaptInfo->Clear();
        }
        m_tLinkNetAdapt.Clear();
    }
    
    TNetAdaptInfoAll & operator = (TNetAdaptInfoAll & tNetAdaptInfoAll)
    {
        if ( this != &tNetAdaptInfoAll )
        {
            this->Clear();
            u32 dwLop = 0;
            u32 dwArrayLen = tNetAdaptInfoAll.m_tLinkNetAdapt.Length();
            TNetAdaptInfo * ptNetAdapt = NULL;
            for ( dwLop = 0; dwLop < dwArrayLen; dwLop++ )
            {
                ptNetAdapt = tNetAdaptInfoAll.m_tLinkNetAdapt.GetValue( dwLop );
                if ( NULL == ptNetAdapt )
                {
                    continue;
                }
                if ( !this->m_tLinkNetAdapt.Append(*ptNetAdapt) )
                {
                    break;
                }
            }
        }
        return *this;
    }
    
    BOOL32 IsValid(void)
    {
        u32 dwAdapterNum = m_tLinkNetAdapt.Length();
        u32 dwIdx = 0;
        TNetAdaptInfo * ptAdapter = NULL;
        for ( dwIdx = 0; dwIdx < dwAdapterNum; dwIdx++ )
        {
            ptAdapter = m_tLinkNetAdapt.GetValue( dwIdx );
            if ( ptAdapter->m_tLinkIpAddr.Length() == 0 )
            {
                return FALSE;
            }
        }
        return TRUE;
    }

    u32  GetIpNum(void)
    {
        u32 dwIpNum = 0;
        u32 dwArrayLen = m_tLinkNetAdapt.Length();
        TNetAdaptInfo * ptNetAdapt = NULL;
        for ( u32 dwLop = 0; dwLop < dwArrayLen; dwLop++ )
        {
            ptNetAdapt = m_tLinkNetAdapt.GetValue( dwLop );
            if ( NULL == ptNetAdapt )
            {
                continue;
            }
            dwIpNum += ptNetAdapt->m_tLinkIpAddr.Length();
        }
        return dwIpNum;
    }

    BOOL32 GetAllNetParam( TNetParam * ptNetParam /*IN/OUT*/, u32 &dwNetParamSize/*IN/OUT*/ )
    {
        if ( NULL == ptNetParam || 0 == dwNetParamSize || dwNetParamSize < GetIpNum() )
        {
            dwNetParamSize = GetIpNum();
            return FALSE;
        }

        TNetParam *ptTemp = ptNetParam;
        u32 dwArrayLen = m_tLinkNetAdapt.Length();
        TNetAdaptInfo * ptNetAdapt = NULL;
        u32 dwLop = 0;
        u32 dwIpNum = 0;
        for ( dwLop = 0; dwLop < dwArrayLen; dwLop++ )
        {
            ptNetAdapt = m_tLinkNetAdapt.GetValue( dwLop );
            if ( NULL == ptNetAdapt )
            {
                continue;
            }
            u32 dwIpLen = ptNetAdapt->m_tLinkIpAddr.Length();
            u32 dwIpIdx = 0;
            for ( dwIpIdx = 0; dwIpIdx < dwIpLen; dwIpIdx++ )
            {
                *ptTemp = *(ptNetAdapt->m_tLinkIpAddr.GetValue( dwIpIdx ));
                ptTemp++;
                dwIpNum++;
            }
        }
        dwNetParamSize = dwIpNum;
        return TRUE;
    }
    
    void Print(void)
    {
        u32 dwAdapterNum = m_tLinkNetAdapt.Length();
        u32 dwIdx = 0;
        TNetAdaptInfo * ptAdapter = NULL;
        for ( dwIdx = 0; dwIdx < dwAdapterNum; dwIdx++ )
        {
            ptAdapter = m_tLinkNetAdapt.GetValue( dwIdx );
            ptAdapter->Print();
        }
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;



struct TRoute
{
protected:
    TNetParam m_tDstNet;
    u32 m_dwGateway;
    u32 m_dwMetric;     //跃点数
	
public:
    TRoute(void) { SetNull(); }
	
    TNetParam GetDstNet(void) const { return m_tDstNet; }
    void    SetDstNet( const TNetParam &tDstNet ) { m_tDstNet = tDstNet; }
	
    u32 GetGateway(void) const { return ntohl(m_dwGateway); }
    void SetGateway( u32 dwGateway ) { m_dwGateway = htonl(dwGateway); }
	
    u32 GetMetric(void) const { return ntohl(m_dwGateway); }
    void SetMetric( u32 dwMetric ) { m_dwMetric = htonl(dwMetric); }
	
    void SetNull(void) 
    { 
        m_tDstNet.SetNull();
        m_dwGateway = 0;
        m_dwMetric = 0;
    }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


class CSemOpt{
public:
    CSemOpt( SEMHANDLE &hSem, u32 dwTime = WAIT_SEM_TIMEOUT )
    {
        m_bTake = FALSE;
        m_hSem = hSem;
		
        m_bTake = OspSemTakeByTime( m_hSem, dwTime );
        if( !m_bTake )
        {
            OspPrintf( TRUE, FALSE, "semTake error accord to handle<0x%x>\n", hSem );
        }
    }
    CSemOpt()
    {
        m_bTake = FALSE;
    }
    ~CSemOpt( )
    {
        OspSemGive( m_hSem );
        m_bTake = FALSE;
    }
public:
    BOOL32 SemTake( SEMHANDLE &hSem, u32 dwTime = WAIT_SEM_TIMEOUT )
    {
        m_bTake = FALSE;
        m_hSem = hSem;
        
        m_bTake = OspSemTakeByTime( m_hSem, dwTime );
        if( !m_bTake )
        {
            OspPrintf( TRUE, FALSE, "semTake error accord to handle<0x%x>\n", hSem );
        }
        return m_bTake;
    }
	
    BOOL32 IsSemTake(void)
    {
        return m_bTake;
    }
	
private:
    SEMHANDLE m_hSem;
    BOOL32    m_bTake;   
};



#ifdef WIN32
#pragma pack( pop )
#endif

#endif  // _NET_STRUCT_H_
