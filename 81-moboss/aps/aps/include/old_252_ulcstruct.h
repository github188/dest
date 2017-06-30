/*****************************************************************************
模块名      : UserLoginClientLib
文件名      : ulcstruct.h
创建时间    : 2011年 08月 18日
实现功能    : 
作者        : 陈文灿
版本        : 
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2011/08/18  1.0         陈文灿        创建
******************************************************************************/
#ifndef _ULC_STRUCT_H_
#define _ULC_STRUCT_H_


#include "osp.h"
#include "kdvsys.h"
#include "kdvtype.h"
#include "old_26_structulc.h"
#include "structulc.h"

#ifdef WIN32

#include <WinSock2.h>
#pragma comment( lib, "ws2_32.lib" ) 
#pragma pack( push )
#pragma pack( 1 )


#ifndef PACKED
#define PACKED
#endif

#endif	/* end WIN32 */



#ifdef _LINUX_

#ifndef PACKED
#define PACKED __attribute__ ((packed)) 
#endif

#endif  /* end _LINUX_ */

#ifdef WIN32
#ifndef vsnprintf
#define vsnprintf   _vsnprintf
#endif
#ifndef snprintf
#define snprintf    _snprintf
#endif
#endif

#define			MAX_ULSLIST_NUM					(u8)10
//#define         MAX_ULSIPLIST_NUM               (u8)255

#define			MAX_MTNAME_LEN					(u16)16
#define			MAX_MTPASSWORD_LEN				(u16)32
#define			MAX_DOMAINNAME_LEN				(u16)64


#define			ULC_CONNECTULS_TASK_PRIORITY	(u8)90
#define			ULC_CONNECTULS_TASK_STACKSIZE	(u32)(100<10)


#define			ERR_CONNECTULS_FAILED		1
#define			ERR_INVALID_E164			2

// wy[2012/08/08] add 终端登录ver，从2.5.0.2.0即2.5的SP2版本开始，对版本信息进行校验，向前兼容
// 注：之后新发版本，在Ver1后添加，在uls测进行版本识别
typedef enum tagEULSVER
{
    emVerBgn = 0,
	emVer1,      // 2.5.0.2.0即2.5的SP2版本
    emVerEnd
}EUlsVer;
// end

//wy[2012/08/13] add 服务器类型
typedef enum tagESRVTYPE
{
	emSrvTypeBgn = 0,
	emSrvTypeTui,
	emSrvTypeNma,
	emSrvTypeSus,
	emSrvTypeUls,
	emSrvTypeNts,
	emSrvTypeEnd
}ESrvType;
//end// 

//OEM标识的最大长度
#define MAXLEN_OLD252_SUS_OEM_MARK                     (u16)16

struct TOld252MtLoginInfo
{
protected:
	u16	m_wLoginType;                              // 终端类型：暂无用
	s8	m_achLoginName[MAX_MTNAME_LEN+1];          // 终端登录名称：终端E164NO
	s8	m_achPassword[MAX_MTPASSWORD_LEN+1];       // 终端登录密码：暂无用
	s8  m_achOemRemark[MAXLEN_OLD252_SUS_OEM_MARK + 1];   // 终端OEM标示：OEM版本的终端使用，为获取OEM标示的SUS
    u32 m_dwUlsIP;      //网络序

public:
	TOld252MtLoginInfo()
	{
		Clear();
	}

	void Clear( void )
	{
		m_wLoginType = 0;
		memset( m_achLoginName, 0, sizeof(m_achLoginName) );
		memset( m_achPassword, 0, sizeof(m_achPassword) );
		memset( m_achOemRemark, 0, sizeof( m_achOemRemark ) );
        m_dwUlsIP = 0;
	}

	void SetLoginType( u16 wType )
	{
		m_wLoginType = wType;
	}
	u16 GetLoginType( void )
	{
		return m_wLoginType;
	}

    const s8* GetPassword( ) const { return m_achPassword; }
	void SetPassword( const s8 * pchPassword )
	{
		memset( m_achPassword, 0, sizeof(m_achPassword) );
		if( NULL != pchPassword )
		{
			memcpy( m_achPassword, pchPassword, sizeof(m_achPassword)-1 );
			m_achPassword[sizeof(m_achPassword)-1] = '\0';
		}
		return ;
	}

    const s8* GetLoginName( ) const { return m_achLoginName; }
	void SetLoginName( const s8 * pchMtLoginName )
	{
		// wy[2012/08/08] add
		SetLoginType( (u16)emVer1 );
		// end

		memset( m_achLoginName, 0, sizeof(m_achLoginName) );
        if (NULL != pchMtLoginName)
        {
            memcpy( m_achLoginName, pchMtLoginName, sizeof(m_achLoginName)-1 );
            m_achLoginName[sizeof(m_achLoginName)-1] = '\0';
        }
        return;
	}

    const s8* GetOemRemark( ) const { return m_achOemRemark; }
	void SetOemRemark( const s8 * pchOemRemark )
	{
		memset( m_achOemRemark, 0, sizeof( m_achOemRemark ) );
		if ( NULL != pchOemRemark )
		{
			memcpy( m_achOemRemark, pchOemRemark, sizeof( m_achOemRemark ) - 1 );
			m_achOemRemark[sizeof(m_achOemRemark) - 1] = '\0';
		}
		return;
	}

    u32 GetUlsIP()/*网络序*/ const { return m_dwUlsIP; }
    void SetUlsIP(u32 dwUlsIP/*网络序*/) { m_dwUlsIP = dwUlsIP; }
}
PACKED
;
 
struct TOld252SrvAddrListInfo
{
protected:
	COldSrvAddrInfo m_tGkInfo;		//GK地址信息
	COldSrvAddrInfo m_tNtsInfo;		//NTS地址信息
	COldSrvAddrInfo m_tNmaInfo;		//NMA地址信息
	COldSrvAddrInfo m_tSusInfo;		//SUS地址信息
	
	u8 m_byValidUlsCount;

public:
	COldSrvAddrInfo m_atUlsListInfo[MAX_ULSLIST_NUM];

public:

	TOld252SrvAddrListInfo()
	{
		Clear();
	}

	COldSrvAddrInfo GetGkAddr( ) const { return m_tGkInfo; }
	void SetGkAddr( const COldSrvAddrInfo& tGkInfo )
	{
		m_tGkInfo = tGkInfo;
	}
    void SetGkAddr(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetGkAddr( Convert(tAddrInfo) );
    }

	COldSrvAddrInfo GetNtsAddr( ) const { return m_tNtsInfo; }
	void SetNtsAddr( const COldSrvAddrInfo& tNtsInfo )
	{
        m_tNtsInfo = tNtsInfo;
	}
    void SetNtsAddr(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetNtsAddr( Convert(tAddrInfo) );
    }

	COldSrvAddrInfo GetNmaAddr( ) const { return m_tNmaInfo; }
	void SetNmaAddr( const COldSrvAddrInfo& tNmaInfo )
	{
        m_tNmaInfo = tNmaInfo;
	}
    void SetNmaAddr(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetNmaAddr( Convert(tAddrInfo) );
    }

    //
    COldSrvAddrInfo GetSusAddr( ) { return m_tSusInfo; }
	void SetSusAddr( const COldSrvAddrInfo& tSusInfo )
	{
        m_tSusInfo = tSusInfo;
	}
    void SetSusAddr(const CSrvAddrInfo& tAddrInfo) 
    { 
        SetSusAddr( Convert(tAddrInfo) );
    }

	void SetUlsCount( u8 byUlsCount )
	{
		m_byValidUlsCount = (byUlsCount >= MAX_ULSLIST_NUM) ? MAX_ULSLIST_NUM : byUlsCount ;
	}
	u8  GetUlsCount( void )
	{
		return m_byValidUlsCount;
	}

	void Clear( void )
	{
		m_tGkInfo.Clear();
		m_tNtsInfo.Clear();
		m_tNmaInfo.Clear();
		m_tSusInfo.Clear();

		m_byValidUlsCount = 0;
		memset( m_atUlsListInfo, 0, sizeof(m_atUlsListInfo) );
	}
}
PACKED
;



//////////////////////////////////////////////////////////////////////////////////////////////////
//                      从mcustruct.h文件中 移植CServMsg
//////////////////////////////////////////////////////////////////////////////////////////////////
//会议号结构 (len:16)
#define     MAXLEN_CONFID                   16
//消息长度相关
#define      SERV_MSG_LEN					0x7000  //消息长度
#define      SERV_MSGHEAD_LEN				0x30    //消息头长度

#ifndef getmin
#define getmin(a,b) ((a)>(b)?(b):(a))
#endif
#ifndef getmax
#define getmax(a,b) ((a)>(b)?(a):(b))
#endif

struct CConfId
{
protected:
    u8  	m_abyConfId[MAXLEN_CONFID];

public:
    u8   GetConfId( u8   * pbyConfId, u8   byBufLen ) const; //获得会议号数组
    void CreateConfId( u8 byUsrGrpId );                               //创建会议号
    void GetConfIdString( LPSTR lpszConfId, u8   byBufLen ) const;//获得会议号字符串
    void SetConfId( const CConfId & cConfId );                  //设置会议号
    u8   SetConfId( const u8   * pbyConfId, u8   byBufLen );          //设置会议号数组
    u8   SetConfId( LPCSTR lpszConfId );                          //设置会议号数组	
    BOOL32 IsNull( void ) const;                                  //判断会议号是否为空
    void SetNull( void );                                       //设置会议号为空
    BOOL32 operator == ( const CConfId & cConfId ) const;         //操作符重载
    // 顾振华，在CConfId中内置用户组的信息
    u8   GetUsrGrpId() const 
    {
        return m_abyConfId[MAXLEN_CONFID-1];
    }
    void SetUsrGrpId(u8 byGrpId)
    {
        m_abyConfId[MAXLEN_CONFID-1] = byGrpId;
    }

    //fuxiuhua,增加会议发起方信息
    //会议ID的组成：MCU号(2Byte)/会议类型(1Byte)/会议索引值(1Byte)/会议号产生次数(4Byte)/
    //              未用(6Byte)/发起方类型(1Byte)/组号(1Byte)
    void SetConfSource( u8 byConfSource )
    {
        m_abyConfId[MAXLEN_CONFID - 2] = byConfSource;
    }
    u8   GetConfSource() const
    {
        return m_abyConfId[MAXLEN_CONFID - 2];
    }

    u8 GetConfIdx() const
    {
        return m_abyConfId[3];
    }
}
/*
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
*/
PACKED
    ;

//业务消息类，最大处理28K长度消息
class CServMsg
{
protected:
    u16     m_wSerialNO;            //流水号
    u8      m_bySrcDriId;           //源DRI板号
    u8      m_bySrcMtId;            //源终端号
    u16     m_wSrcSsnId;           //源会话号
    u8      m_byDstDriId;           //目的DRI板号
    u8      m_byDstMtId;            //目的终端号
    u8      m_byMcuId;              //MCU号
    u8      m_byChnIndex;           //通道索引号
    u32     m_dwConfIdx;            //会议索引号
    CConfId m_cConfId;              //会议号
    u16		m_wEventId;             //事件号
    u16		m_wTimer;               //定时
    u16		m_wErrorCode;           //错误码
    u16		m_wMsgBodyLen;          //消息体长度
    u8      m_byTotalPktNum;        //总包数（用于需要切包发送的消息）
    u8      m_byCurPktIdx;          //当前包索引（从0开始）
    u16     m_wDstSsnId;            //目的会话号
    u8      m_abyReserverd[10];     //保留字节
    u8      m_abyMsgBody[SERV_MSG_LEN-SERV_MSGHEAD_LEN];    //消息体
public:
    void  SetSerialNO(u16  wSerialNO){ m_wSerialNO = htons(wSerialNO);} 
    u16   GetSerialNO( void ) const { return ntohs(m_wSerialNO); }
    void  SetSrcDriId(u8   bySrcDriId){ m_bySrcDriId = bySrcDriId;} 
    u8    GetSrcDriId( void ) const { return m_bySrcDriId; }
    void  SetSrcMtId(u8   bySrcMtId){ m_bySrcMtId = bySrcMtId;} 
    u8    GetSrcMtId( void ) const { return m_bySrcMtId; }
    void  SetSrcSsnId( u16 wSrcSsnId ){ m_wSrcSsnId = htons(wSrcSsnId);} 
    u16   GetSrcSsnId( void ) const { return ntohs(m_wSrcSsnId); }
    void  SetDstSsnId( u16 wDstSsnId ){ m_wDstSsnId = htons(wDstSsnId);} 
    u16   GetDstSsnId( void ) const { return ntohs(m_wDstSsnId); }
    void  SetDstDriId(u8   byDstDriId){ m_byDstDriId = byDstDriId;} 
    u8    GetDstDriId( void ) const { return m_byDstDriId; }  
    void  SetDstMtId(u8   byDstMtId){ m_byDstMtId = byDstMtId;} 
    u8    GetDstMtId( void ) const { return m_byDstMtId; }
    void  SetMcuId(u8   byMcuId){ m_byMcuId = byMcuId;} 
    u8    GetMcuId( void ) const { return m_byMcuId; }
    void  SetChnIndex(u8   byChnIndex){ m_byChnIndex = byChnIndex;} 
    u8    GetChnIndex( void ) const { return m_byChnIndex; }
    void  SetConfIdx( u32 dwConfIdx ){ m_dwConfIdx = dwConfIdx; } 
    u32   GetConfIdx( void ) const { return m_dwConfIdx; }
    void  SetEventId(u16  wEventId){ m_wEventId = htons(wEventId);} 
    u16   GetEventId( void ) const { return ntohs(m_wEventId); }
    void  SetTimer(u16  wTimer){ m_wTimer = htons(wTimer);} 
    u16   GetTimer( void ) const { return ntohs(m_wTimer); }
    void  SetErrorCode(u16  wErrorCode){ m_wErrorCode = htons(wErrorCode);} 
    u16   GetErrorCode( void ) const { return ntohs(m_wErrorCode); }
    void  SetTotalPktNum(u8 byPktNum) { m_byTotalPktNum = byPktNum; }
    u8    GetTotalPktNum( void ) { return m_byTotalPktNum; }
    void  SetCurPktIdx(u8 byPktIdx) { m_byCurPktIdx = byPktIdx; }
    u8    GetCurPktIdx( void ) { return m_byCurPktIdx; }

    void Init( void );
    void SetNoSrc(){ SetSrcSsnId( 0 ); }
    void SetMsgBodyLen( u16  wMsgBodyLen );
    CServMsg( void );//constructor
    CServMsg( u8   * const pbyMsg, u16  wMsgLen );//constructor
    ~CServMsg( void );//distructor
    void ClearHdr( void );//消息头清零
    CConfId GetConfId( void ) const;//获取会议号信息
    void SetConfId( const CConfId & cConfId );//设置会议号信息
    void SetNullConfId( void );//设置会议号信息为空
    u16  GetMsgBodyLen( void ) const;//获取消息体长度信息
    u16  GetMsgBody( u8   * pbyMsgBodyBuf, u16  wBufLen ) const;//获取消息体，由用户申请BUFFER，如果过小做截断处理
    u8   * const GetMsgBody( void ) const;//获取消息体指针，用户不需提供BUFFER
    void SetMsgBody( u8   * const pbyMsgBody = NULL, u16  wLen = 0 );//设置消息体
    void CatMsgBody( u8   * const pbyMsgBody, u16  wLen );//添加消息体
    u16  GetServMsgLen( void ) const;//获取整个消息长度
    u16  GetServMsg( u8   * pbyMsgBuf, u16  wBufLen ) const;//获取整个消息，由用户申请BUFFER，如果过小做截断处理
    u8   * const GetServMsg( void ) const;//获取整个消息指针，用户不需提供BUFFER
    void SetServMsg( u8   * const pbyMsg, u16  wLen );//设置整个消息
    const CServMsg & operator= ( const CServMsg & cServMsg );//操作符重载
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
    ;


/*-------------------------------------------------------------------
                               CServMsg                              
--------------------------------------------------------------------*/

/*====================================================================
    函数名      ：CServMsg
    功能        ：constructor
    算法实现    ：
    引用全局变量：
    输入参数说明：
    返回值说明  ：
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline CServMsg::CServMsg( void )
{
	Init();
}

/*====================================================================
    函数名      ：CServMsg
    功能        ：constructor
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * const pbyMsg, 要赋值的消息指针
				  u16 wMsgLen, 要赋值的消息长度，必须大于等于 SERV_MSGHEAD_LEN
    返回值说明  ：
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline CServMsg::CServMsg( u8 * const pbyMsg, u16 wMsgLen )
{
	Init();
	
	if( wMsgLen < SERV_MSGHEAD_LEN || pbyMsg == NULL )
		return;
	
	wMsgLen = getmin( wMsgLen, SERV_MSG_LEN );
	memcpy( this, pbyMsg, wMsgLen );
	
	//set length
	SetMsgBodyLen( wMsgLen - SERV_MSGHEAD_LEN );
}

/*====================================================================
    函数名      ：~CServMsg
    功能        ：distructor
    算法实现    ：
    引用全局变量：
    输入参数说明：
    返回值说明  ：
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline CServMsg::~CServMsg( void )
{

}

/*====================================================================
    函数名      ：Init
    功能        ：BUFFER初始化
    算法实现    ：
    引用全局变量：
    输入参数说明：
    返回值说明  ：
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline void CServMsg::Init( void )
{
	memset( this, 0, SERV_MSG_LEN );	//清零
}

/*====================================================================
    函数名      ：ClearHdr
    功能        ：消息头清零
    算法实现    ：
    引用全局变量：
    输入参数说明：
    返回值说明  ：
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline void CServMsg::ClearHdr( void )
{
	u16		wBodyLen = GetMsgBodyLen();

	memset( this, 0, SERV_MSGHEAD_LEN );	//清零
	SetMsgBodyLen( wBodyLen );
}

/*====================================================================
    函数名      ：GetConfId
    功能        ：获取会议号信息
    算法实现    ：
    引用全局变量：
    输入参数说明：无
    返回值说明  ：会议号，全0表示创建
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
    02/12/24    1.0         LI Yi         修改接口
====================================================================*/
inline CConfId CServMsg::GetConfId( void ) const
{
	return( m_cConfId );
}

/*====================================================================
    函数名      ：SetConfId
    功能        ：设置会议号信息
    算法实现    ：
    引用全局变量：
    输入参数说明：const CConfId & cConfId，会议号，全0表示创建
    返回值说明  ：无
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
    02/12/24    1.0         LI Yi         修改接口
====================================================================*/
inline void CServMsg::SetConfId( const CConfId & cConfId )
{
	m_cConfId = cConfId;
}

/*====================================================================
    函数名      ：SetNullConfId
    功能        ：设置会议号信息为空
    算法实现    ：
    引用全局变量：
    输入参数说明：无
    返回值说明  ：无
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    03/06/06    1.0         LI Yi         创建
====================================================================*/
inline void CServMsg::SetNullConfId( void )
{
	CConfId		cConfId;
	
	cConfId.SetNull();
	SetConfId( cConfId );
}

/*====================================================================
    函数名      ：GetMsgBodyLen
    功能        ：获取消息体长度信息
    算法实现    ：
    引用全局变量：
    输入参数说明：无
    返回值说明  ：消息体长度
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline u16 CServMsg::GetMsgBodyLen( void ) const
{
	return( ntohs( m_wMsgBodyLen ) );
}

/*====================================================================
    函数名      ：SetMsgBodyLen
    功能        ：设置消息体长度信息
    算法实现    ：
    引用全局变量：
    输入参数说明：u16 wMsgBodyLen, 消息体长度
    返回值说明  ：无
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline void CServMsg::SetMsgBodyLen( u16 wMsgBodyLen )
{
	m_wMsgBodyLen = htons( wMsgBodyLen );
}

/*====================================================================
    函数名      ：GetMsgBody
    功能        ：获取消息体指针，用户不需提供BUFFER
    算法实现    ：
    引用全局变量：
    输入参数说明：无
    返回值说明  ：u8 * const指针
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline u8 * const CServMsg::GetMsgBody( void ) const
{
	return( ( u8 * const )m_abyMsgBody );
}

/*====================================================================
    函数名      ：GetMsgBodyLen
    功能        ：获取整个消息长度
    算法实现    ：
    引用全局变量：
    输入参数说明：无
    返回值说明  ：整个消息长度
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/26    1.0         LI Yi         创建
====================================================================*/
inline u16 CServMsg::GetServMsgLen( void ) const
{
	return( GetMsgBodyLen() + SERV_MSGHEAD_LEN );
}

/*====================================================================
    函数名      ：GetMsgBody
    功能        ：获取消息体，由用户申请BUFFER，如果过小做截断处理
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * pbyMsgBodyBuf, 返回的消息体
				  u16 wBufLen, BUFFER大小
    返回值说明  ：实际返回的消息体长度
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline u16 CServMsg::GetMsgBody( u8 * pbyMsgBodyBuf, u16 wBufLen ) const
{
    u16 wActLen = getmin( GetMsgBodyLen(), wBufLen );
	memcpy( pbyMsgBodyBuf, m_abyMsgBody, wActLen );
	return wActLen;
}

/*====================================================================
    函数名      ：SetMsgBody
    功能        ：设置消息体
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * const pbyMsgBody, 传入的消息体，缺省为NULL
				  u16 wLen, 传入的消息体长度，缺省为0
    返回值说明  ：无
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline void CServMsg::SetMsgBody( u8 * const pbyMsgBody, u16 wLen )
{
    /*
    if (pbyMsgBody == NULL || wLen == 0)
    {
        SetMsgBodyLen( 0 );
        return;
    }
    */
	wLen = getmin( wLen, SERV_MSG_LEN - SERV_MSGHEAD_LEN );
	memcpy( m_abyMsgBody, pbyMsgBody, wLen );
	SetMsgBodyLen( wLen );
}

/*====================================================================
    函数名      ：CatMsgBody
    功能        ：添加消息体
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * const pbyMsgBody, 传入的消息体，缺省为NULL
				  u16 wLen, 传入的消息体长度，缺省为0
    返回值说明  ：无
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/11/07    1.0         Liaoweijiang  创建
====================================================================*/
inline void CServMsg::CatMsgBody( u8 * const pbyMsgBody, u16 wLen )
{
    /*
    if (pbyMsgBody == NULL || wLen == 0)
    {
        return;
    }
    */
	wLen = getmin( wLen, SERV_MSG_LEN - SERV_MSGHEAD_LEN - GetMsgBodyLen() );
	memcpy( m_abyMsgBody + GetMsgBodyLen(), pbyMsgBody, wLen );
	SetMsgBodyLen( GetMsgBodyLen() + wLen );
}

/*====================================================================
    函数名      ：GetMsgBody
    功能        ：获取整个消息指针，用户不需提供BUFFER
    算法实现    ：
    引用全局变量：
    输入参数说明：无
    返回值说明  ：u8 * const指针
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/26    1.0         LI Yi         创建
====================================================================*/
inline u8 * const CServMsg::GetServMsg( void ) const
{
	return( ( u8 * const )( this ) );
}

/*====================================================================
    函数名      ：GetServMsg
    功能        ：获取整个消息，由用户申请BUFFER，如果过小做截断处理
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * pbyMsgBuf, 返回的消息
				  u16 wBufLen, BUFFER大小
    返回值说明  ：实际返回的消息长度
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline u16 CServMsg::GetServMsg( u8 * pbyMsgBuf, u16 wBufLen ) const
{
	wBufLen = getmin(SERV_MSG_LEN,wBufLen);
	memcpy( pbyMsgBuf, this, wBufLen );
	return( getmin( GetMsgBodyLen() + SERV_MSGHEAD_LEN, wBufLen ) );
}

/*====================================================================
    函数名      ：SetServMsg
    功能        ：设置整个消息
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * const pbyMsg, 传入的整个消息
				  u16 wMsgLen, 传入的消息长度
    返回值说明  ：无
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/15    1.0         LI Yi         创建
====================================================================*/
inline void CServMsg::SetServMsg( u8 * const pbyMsg, u16 wMsgLen )
{
	if( wMsgLen < SERV_MSGHEAD_LEN )
	{
		OspPrintf( TRUE, FALSE, "CServMsg: SetServMsg() Exception -- invalid MsgLen!\n" );
		return;
	}

	wMsgLen = getmin( wMsgLen, SERV_MSG_LEN );
	memcpy( this, pbyMsg, wMsgLen );
	SetMsgBodyLen( wMsgLen - SERV_MSGHEAD_LEN );
}

/*====================================================================
    函数名      ：operator=
    功能        ：操作符重载
    算法实现    ：
    引用全局变量：
    输入参数说明：CServMsg & cServMsg, 赋值的消息引用
    返回值说明  ：CServMsg对象应用
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/07/26    1.0         LI Yi         创建
====================================================================*/
inline const CServMsg & CServMsg::operator= ( const CServMsg & cServMsg )
{
	u16	wLen = cServMsg.GetServMsgLen();

	memcpy( this, cServMsg.GetServMsg(), wLen );
	return( *this );
}



#ifdef WIN32
#pragma pack( pop )
#endif


#endif

