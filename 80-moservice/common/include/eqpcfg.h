//
//��������
//
//
#ifndef _PERIPHERY_EQUIPMENT_CONFIG_H_
#define _PERIPHERY_EQUIPMENT_CONFIG_H_

#include "mcuconst.h"
#include "kdvsys.h"
#include "commonlib.h"

#ifdef _LINUX
#include "brdwrapper.h"
#endif
#include "kdvtype.h"

#define MAXNUM_MAP    5

#define QOS_AUDIO   (s32)1  // ��ƵQos
#define QOS_VIDEO   (s32)2  // ��ƵQos
#define QOS_DATA    (s32)3  // ����Qos

#define ENCPARAMLIMITTABLE (s8*)"EncParamLimitTable"

enum EReadDefaultTableType //��Ĭ�����ʷֱ���������Ϣ������
{
	eReadTableNone = 0,
	eReadMpuTable = 1,
	eReadMpu2Table = 2,
	eRead8KETable = 3,
	eRead8KHTable = 4
};



#ifdef WIN32
    #pragma pack( push )
    #pragma pack( 1 )
#endif


struct TMAPParam
{
	u32  dwCoreSpd;     // Map�ں��ٶ�
	u32  dwMemSpd;      // �ڴ��ٶ�
	u32  dwMemSize;     // �ڴ��С
	u8   byMapId;	    // Map���
	u32   dwPort;	    // Map�˿ں�
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


//8000e����������ýṹ
struct T8keEqpCfg
{
protected:
	char  m_achAlias[ MAXLEN_ALIAS+1 ];
#ifdef _CARRIER_
    u16   m_wEqpId;           
#else
	u8    m_byEqpId;          //����ID
#endif
	u8    m_byEqpType;        //��������

	u32   m_dwLocalIP;        //����IP��ַ
	u32   m_dwConnectIP;      //����MCUIP��ַ
	u16   m_wConnectPort;     //����MCUͨ�Ŷ˿�
	u16   m_wMcuId;           //MCU id ��

    u16   m_wEqpStartPort;    //���������ʼ�˿ں�
    u16   m_wMcuStartPort;    //MCU������ʼ�˿ں�
    u32   m_dwMcuSwitchIp;    //MCUת��IP

    u8    m_byDiscHeartBeatNum;
    u16   m_wDiscHeartBeatTime;

    u8    m_byIsEmbed;        //�Ƿ���Ƕ

public:
    T8keEqpCfg(void) { memset(this, 0, sizeof(T8keEqpCfg)); }

    s8 *  GetAlias(void) const { return (s8*)m_achAlias; }
    void  SetAlias(s8 * pchAlias)
    {
        if ( NULL == pchAlias )
        {
            return;
        }
        strncpy( m_achAlias, pchAlias, MAXLEN_ALIAS );
        m_achAlias[MAXLEN_ALIAS] = '\0';
        return;
    }
    void  ClearAlias(void) { memset( m_achAlias, 0, sizeof(m_achAlias) ); }

#ifdef _CARRIER_
    u16   GetEqpId(void) const { return ntohs(m_wEqpId); };
    void  SetEqpId(u16 wEqpId) { m_wEqpId = htons(wEqpId); }
#else
    u8    GetEqpId(void) const { return m_byEqpId; };
    void  SetEqpId(u8 byEqpId) { m_byEqpId = byEqpId; }
#endif

    u8    GetEqpType(void) const { return m_byEqpType; }
    void  SetEqpType(u8 byEqpType) { m_byEqpType = byEqpType; }

    u32   GetEqpIp(void) const { return ntohl(m_dwLocalIP); }
    void  SetEqpIp(u32 dwEqpIp) { m_dwLocalIP = htonl(dwEqpIp); }

    u32   GetConnectIp(void) const { return ntohl(m_dwConnectIP); }
    void  SetConnectIp(u32 dwConnectIp) { m_dwConnectIP = htonl(dwConnectIp); }

    u16   GetConnectPort(void) const { return ntohs(m_wConnectPort); }
    void  SetConnectPort(u16 wConnectPort) { m_wConnectPort = htons(wConnectPort); }

    u16   GetMcuId(void) const { return ntohs(m_wMcuId); }
    void  SetMcuId(u16 wMcuId) { m_wMcuId = htons(wMcuId); }

    u16   GetEqpStartPort(void) const { return ntohs(m_wEqpStartPort); }
    void  SetEqpStartPort(u16 wEqpStartPort) { m_wEqpStartPort = htons(wEqpStartPort); }
    
    u16   GetMcuStartPort(void) const { return ntohs(m_wMcuStartPort); }
    void  SetMcuStartPort(u16 wMcuStartPort) { m_wMcuStartPort = htons(wMcuStartPort); }
    
    u32   GetMcuSwitchIp(void) const { return ntohl(m_dwMcuSwitchIp); }
    void  SetMcuSwitchIp(u32 dwMcuSwitchIp) { m_dwMcuSwitchIp = htonl(dwMcuSwitchIp); }

    void SetDiscHeartBeatNum( u8 byNum ) { m_byDiscHeartBeatNum = byNum; }
    u8   GetDiscHeartBeatNum( void ) const { return m_byDiscHeartBeatNum; }
    
    void SetDiscHeartBeatTime( u16 wTime ) { m_wDiscHeartBeatTime = htons(wTime); }
    u16   GetDiscHeartBeatTime( void ) const { return ntohs(m_wDiscHeartBeatTime); }

    void  SetIsEmbed( BOOL32 bIsEmbed ) { m_byIsEmbed = bIsEmbed ? 1 : 0; }
    BOOL32 IsEmbed(void) const { return (0 != m_byIsEmbed) ? TRUE: FALSE; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct T8keAudioMixerCfg : public T8keEqpCfg
{
	u8    m_byMaxMixGroupCount;
	u8    m_byMaxChannelInGroup;

public:
    u8    GetMaxMixGrpCount(void) const { return m_byMaxMixGroupCount; }
    void  SetMaxMixGrpCount(u8 byMaxCount) { m_byMaxMixGroupCount = byMaxCount; }
    u8    GetMaxChnnlInGrp(void) const { return m_byMaxChannelInGroup; }
    void  SetMaxChnnlInGrp(u8 byMaxChnnl) { m_byMaxChannelInGroup = byMaxChnnl; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct T8keVmpCfg : public T8keEqpCfg
{
protected:
    u8	m_byEncodeNum;
	u32 m_dwIframeInterval;				//[nizhijun 2011/03/21] VMP��ʱ���ؼ�֡����ʱ��(��λ������)
public:
    void SetEncodeNum(u8 byEncodeNum) { m_byEncodeNum = byEncodeNum; }
    u8   GetEncodeNum(void) { return m_byEncodeNum; }

	void  SetIframeInterval(u32 dwIframeInterval) { m_dwIframeInterval = dwIframeInterval; }
    u32   GetIframeInterval(void) { return m_dwIframeInterval; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct T8keBasCfg : public T8keEqpCfg
{
protected:
	u32 m_dwIframeInterval;				//[nizhijun 2011/03/21] BAS��ʱ���ؼ�֡����ʱ��(��λ������)
public:
	void  SetIframeInterval(u32 dwIframeInterval) { m_dwIframeInterval = dwIframeInterval; }
    u32   GetIframeInterval(void) { return m_dwIframeInterval; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


//����������ýṹ(��map��Ϣ)
struct TEqpBasicCfg
{
	char  achAlias[ MAXLEN_ALIAS+1 ];
	u8    byEqpId;          //����ID
	u8    byEqpType;        //��������
	
	u32   dwLocalIP;        //����IP��ַ
	u16   wLocalLisPort;    //���������˿�
	u32   dwConnectIP;      //����MCUIP��ַ
	u16   wConnectPort;     //����MCU ͨ�Ŷ˿�
	u16   wMcuId;           //MCU id ��
	u16   wRcvStartPort;    //������ʼ�˿ں�

	
    u32 dwConnectIpB;
    u16 wConnectPortB;
    u16 wRcvStartPortB;
    TEqpBasicCfg(void) { memset(this, 0, sizeof(TEqpBasicCfg)); }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

//����������ýṹ
struct TEqpCfg:public TEqpBasicCfg
{

	u16   wMAPCount;        //ʹ�õ�MAP����
	TMAPParam m_atMap[MAXNUM_MAP];	//map��Ϣ

    TEqpCfg(void) { memset(this, 0, sizeof(TEqpCfg)); }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TAudioMixerCfg : public TEqpCfg
{
	u8    byMaxMixGroupCount;
	u8    byMaxChannelInGroup;
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
struct TEapuMixerCfg : TEqpCfg
{
	u8  m_byMixerMemberNum;
	BOOL32 m_bIsMultiFormat;
	TEapuMixerCfg()
	{
		m_byMixerMemberNum = 0;
		m_bIsMultiFormat = FALSE;
	}
	void printf(void) const
	{
		OspPrintf(TRUE,FALSE,"achAlias = [%s] EqpId = [%d] LocalIP = [%x] RcvStartPort = [%d] ConnectIP = [%x] MixerMemberNum = [%d]\n",
			achAlias,byEqpId,dwLocalIP,wRcvStartPort,dwConnectIP,m_byMixerMemberNum);
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
struct TEapuCfg
{
	BOOL32  m_bIsProductTest;
	u8		m_byMixerNum;
	TEapuMixerCfg m_tEapuMixerCfg[MAXNUM_EAPU_MIXER];
	u8 m_byIsSimuAPU;
	TEapuCfg(void)
	{
		m_bIsProductTest = FALSE;//Ĭ�Ϸ���������
		m_byMixerNum	= 0;
		m_byIsSimuAPU = 0;
	}
	BOOL32 IsProductTest( void ){ return m_bIsProductTest; }
	void SetProductTest( BOOL32 bProductTestest ){ m_bIsProductTest = bProductTestest; }
	void printf(void) const
	{
		for ( u8 byIndex = 0 ;byIndex < m_byMixerNum ; byIndex++)
		{
			m_tEapuMixerCfg[byIndex].printf();
		}
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
BOOL32 eapuInit(TEapuCfg * ptEapuMixerCfg);
//��DVMP�����𲽷ϳ��ýṹ��// xliang [5/27/2009] 
struct TVmpCfg : public TEqpCfg
{
	u8   by_VmpStyle;
	u8   byDbVid;
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TPrsCfg:public TEqpCfg
{
    u16  m_wFirstTimeSpan;	    //��һ���ش�����(ms)
	u16  m_wSecondTimeSpan;     //�ڶ����ش�����(ms)
	u16  m_wThirdTimeSpan;      //�������ش�����(ms)
	u16  m_wRejectTimeSpan;     //���ڶ�����ʱ����(ms)
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;
//4.6 �¼Ӱ汾 jlb
struct THduModePort
{
public:
	THduModePort()
	{
		memset(this, 0x0, sizeof(THduModePort));
	}
	u8 GetZoomRate() const { return byZoomRate; }
	void SetZoomRate(u8 val) { byZoomRate = val; }
	
    u8   GetOutPortType() const { return byOutPortType; }
    void SetOutPortType(u8 val) { byOutPortType = val; }
	
    u8   GetOutModeType() const { return byOutModeType; }
    void SetOutModeType(u8 val) { byOutModeType = val; }

	u8 GetScalingMode() const { return (reserved & 0x03);}
	void SetScalingMode(u8 byMode) { reserved = (reserved | 0x03) & (byMode | 0xFC); }

	LPCSTR GetOutTypeStr()
	{
		switch(byOutPortType) 
		{
		case HDU_OUTPUT_YPbPr:
			return "YPbPr";
		case HDU_OUTPUT_VGA:
			return "VGA";
		case HDU_OUTPUT_DVI:
			return "DVI";
		case HDU_OUTPUT_HDMI:
			return "HDMI";
		case HDU_OUTPUT_SDI:
			return "SDI";
		default:
			return "UNKOWN";
		}
	}
	LPCSTR GetOutModeStr()
	{
		switch(byOutModeType) 
		{
		case HDU_YPbPr_1080P_24fps:
			return "1080P_24fps";
		case HDU_YPbPr_1080P_25fps:
			return "1080P_25fps";
		case HDU_YPbPr_1080P_30fps:
			return "1080P_30fps";
		case HDU_YPbPr_1080P_50fps:
			return "1080P_50fps";
		case HDU_YPbPr_1080P_60fps:
			return "1080P_60fps";
		case HDU_YPbPr_1080i_50HZ:
			return "1080i_50HZ";
		case HDU_YPbPr_1080i_60HZ:
			return "1080i_60HZ";
		case HDU_YPbPr_720P_60fps:
			return "720P_60fps";
		case HDU_YPbPr_576i_50HZ:
			return "576i_50HZ";
		case HDU_YPbPr_480i_60HZ:
			return "480i_60HZ";
		case HDU_VGA_SXGA_60HZ:
			return "SXGA_60HZ";
		case HDU_VGA_XGA_60HZ:
			return "XGA_60HZ";
		case HDU_VGA_XGA_75HZ:
			return "XGA_75HZ";
		case HDU_VGA_SVGA_60HZ:
			return "SVGA_60HZ";
		case HDU_VGA_SVGA_75HZ:
			return "SVGA_75HZ";
		case HDU_VGA_VGA_60HZ:
			return "VGA_60HZ";
		case HDU_VGA_VGA_75HZ:
			return "VGA_75HZ";
		case HDU_VGA_SXGA_75HZ:
			return "SXGA_75HZ";
		case HDU_YPbPr_720P_50fps:
			return "720P_50fps";
		case HDU_VGA_WXGA_1280_768_60HZ:
			return "WXGA_1280_768_60HZ";
		case HDU_VGA_WXGA_1280_800_60HZ:
			return "WXGA_1280_800_60HZ";
		case HDU_VGA_WSXGA_60HZ:
			return "WSXGA_60HZ";
		case HDU_VGA_SXGAPlus_60HZ:
			return "SXGAPlus_60HZ";
		case HDU_VGA_UXGA_60HZ:
			return "UXGA_60HZ";
		default:
			return "UNKOWN";
		}
	}
	LPCSTR GetZoomRateStr()
	{
		switch(byZoomRate) 
		{
		case HDU_ZOOMRATE_4_3:
			return "4:3";
		case HDU_ZOOMRATE_16_9:
			return "16:9";
		default:
			return "UNKOWN";
		}
	}
	LPCSTR GetScalingModeStr()
	{
		switch(reserved) 
		{
		case HDU2_SHOWMODE_NONGEOMETRIC:
			return "NONGEOMETRIC";
		case HDU2_SHOWMODE_CUTEDGEGEOME:
			return "CUTEDGEGEOME";
		case HDU2_SHOWMODE_BLACKEDGEGEOME:
			return "BLACKEDGEGEOME";
		default:
			return "UNKOWN";
		}
	}
private:
    u8 byOutPortType;
    u8 byOutModeType;
    u8 byZoomRate;

	// 1)���ֶε�2λ��ʹ�ã����ڱ�ʶScalingMode [11/29/2011 chendaiwei]
    u8 reserved;
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct THduCfg : public TEqpCfg
{
protected:
	BOOL32  m_bAutoTest;
	u8   byReserved[4];
    THduModePort atHduModePort[MAXNUM_HDU_CHANNEL];
	u8 m_byHduSubType; //HDU����������(HDU_SUBTYPE_HDU_M...)
public:
	THduCfg(void)
	{
        memset(atHduModePort, 0x0, sizeof(atHduModePort));
		m_byHduSubType = 0;
	}
	void GetHduModePort( u8 byNum, THduModePort &tHduModePortType )
	{
        tHduModePortType.SetOutModeType(atHduModePort[byNum].GetOutModeType());
		tHduModePortType.SetOutPortType(atHduModePort[byNum].GetOutPortType());
		tHduModePortType.SetZoomRate(atHduModePort[byNum].GetZoomRate());
		tHduModePortType.SetScalingMode(atHduModePort[byNum].GetScalingMode());
	}
	void SetHduModePort( u8 byNum, THduModePort &tHduModePortType )
	{
        atHduModePort[byNum].SetOutModeType(tHduModePortType.GetOutModeType());
		atHduModePort[byNum].SetOutPortType(tHduModePortType.GetOutPortType());
		atHduModePort[byNum].SetZoomRate(tHduModePortType.GetZoomRate());
		atHduModePort[byNum].SetScalingMode(tHduModePortType.GetScalingMode());
	}
	u8   GetEqpId( void ){ return byEqpId; }
	u8   GetEqpType( void ){ return byEqpType; }
    u16   GetMcuId( void ){ return wMcuId; }
    
	BOOL32 GetAutoTest( void ){ return m_bAutoTest; }
	void SetAutoTest( BOOL32 bAutoTest ){ m_bAutoTest = bAutoTest; }

	u8 GetHduSubType( void ){return m_byHduSubType;}
	void SetHduSubType (u8 bySubType){m_byHduSubType = bySubType;}

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

struct TMpuCfg : public TEqpCfg
{
protected:
	u8		m_byVmpStyle;
	u8		m_byDbVid;
	u8		m_byReserved1;
	u8		m_byReserved2;
	BOOL32  m_bAutoTest;
public:
	TMpuCfg(void)
	{
		m_bAutoTest = FALSE;
		m_byVmpStyle = 0;
		m_byDbVid	= 0;
	}

	BOOL32 IsAutoTest( void ){ return m_bAutoTest; }
	void SetAutoTest( BOOL32 bAutoTest ){ m_bAutoTest = bAutoTest; }
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


#define MAXNUM_MPU2_EQPNUM 2
struct TMpu2Cfg 
{
protected:
	u8		 m_byMpu2EqpNum;                        //mpu2��ʵ�����õ��������
	u8       m_byWorkMode;                         //mpu2�������ҵ��
	TEqpBasicCfg  m_atMpu2EqpCfg[MAXNUM_MPU2_EQPNUM];   //mpu2��������������������Ϣ
	BOOL32   m_bAutoTest;						 //TRUE���������ԣ�FALSE������������
public:
	TMpu2Cfg(void)
	{
		m_byMpu2EqpNum	= 0;
		m_byWorkMode = 0;
		m_bAutoTest = FALSE;
		memset(m_atMpu2EqpCfg,0,sizeof(m_atMpu2EqpCfg));
	}
	void SetIsAutoTest(BOOL32 bAutoTest){ m_bAutoTest = bAutoTest; }
	BOOL32 GetIsAutoTest()const{return m_bAutoTest;}
	void SetMpu2EqpNum(u8 byEqpNum){m_byMpu2EqpNum = byEqpNum;}
	u8   GetMpu2EqpNum()const{return m_byMpu2EqpNum; }
	void SetMpu2WorkMode(u8 byWorkMode){m_byWorkMode = byWorkMode;}
	u8   GetMpu2WorkMode()const{return m_byWorkMode; }
	BOOL32 SetMpu2EqpCfg(u8 byEqpIdx, const TEqpBasicCfg& tEqpCfg)
	{
		if ( byEqpIdx >= MAXNUM_MPU2_EQPNUM )
		{
			return FALSE;
		}
		m_atMpu2EqpCfg[byEqpIdx] = tEqpCfg;
		return TRUE;
	}
	TEqpBasicCfg GetMpu2EqpCfg(u8 byEqpIdx)const
	{
		TEqpCfg tCfg;
		if (byEqpIdx >= MAXNUM_MPU2_EQPNUM)
		{
			return tCfg;
		}
		return m_atMpu2EqpCfg[byEqpIdx];
	}
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;




struct TResBitrateLimit  //�ֱ�����������������Ϣ
{
protected:
	u8 m_byMediaType;   //��Ӧ��ý�����ͣ�h264,mpeg4...��
	u8 m_byVideoRes;	//��Ӧ�ķֱ��� ��1080p,4cif...��
	u8 m_byFrameRate;   //��Ӧ��֡��
	u8 m_byPorfileType;  //��Ӧ��hp/bp����
	u16 m_wMinBitRate;  //��Ӧý�����ͣ�profiletype���ֱ��ʣ�֡���µ���С���ʷ�ֵ��С�ڸ�����Ӧ�ý��ֱ��ʻ�֡��

public:
	TResBitrateLimit(void)
	{
		clear();
	}

	void clear()
	{
		m_byMediaType = MEDIA_TYPE_NULL;
		m_byVideoRes = VIDEO_FORMAT_INVALID;
		m_byFrameRate = 0;
		m_byPorfileType = emBpAttrb;
		m_wMinBitRate = 0;
	}
	void SetMeidaType(const u8& byMediaType)
	{
		m_byMediaType = byMediaType;
	}
	u8 GetMediaType()const
	{
		return m_byMediaType;
	}
	void SetVideoRes(const u8& byVideoRes)
	{
		m_byVideoRes = byVideoRes;
	}
	u8 GetVideoRes()const
	{
		return m_byVideoRes;
	}
	void SetFrameRate(const u8& byFrameRate)
	{
		m_byFrameRate = byFrameRate;
	}
	u8 GetFrameRate()const
	{
		return m_byFrameRate;
	}
	void SePorfileType(const u8& byPorfileType)
	{
		m_byPorfileType = byPorfileType;
	}
	u8 GetPorfileType()const
	{
		return m_byPorfileType;
	}
	void SetMinBitrate(const u16& wMinBitrate)
	{
		m_wMinBitRate = wMinBitrate;
	}
	u16 GetMinBitrate()const
	{
		return m_wMinBitRate;
	}
	void Print()
	{
		StaticLog("MediaType:%d,VideoRes:%d,FrameRate:%d,ProfileType:%d,MinBitrate:%d\n",
			m_byMediaType,m_byVideoRes,m_byFrameRate,m_byPorfileType,m_wMinBitRate);
	}

}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;


#define FREE_MEM(p, n)				\
{									\
	for( u32 dwInnerLoop = 0; dwInnerLoop < n; dwInnerLoop++ )			\
{															\
	if( p && NULL != p[dwInnerLoop] )			\
{															\
				free( p[dwInnerLoop] );							\
				p[dwInnerLoop] = NULL;							\
}															\
}															\
	free( p );											\
	p = NULL;											\
}





const u16 MpuEncResBrLimit[ ][5]=
{
	//  ��Ƶ��ʽ			�ֱ���   				֡��  	��С���ʣ�K��		�Ƿ�HP
	MEDIA_TYPE_H264,		VIDEO_FORMAT_UXGA,		60,			2382,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_UXGA,		30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_UXGA,	  	20,			0	,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SXGA,	  	60,			2382,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SXGA,	  	30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SXGA,	  	20,			0	,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_WXGA,	  	60,			2382,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_WXGA,	  	30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_WXGA,	  	20,			0	,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_VGA,	  	60,			2382,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_VGA,	  	30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_VGA,	  	20,			0	,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SVGA,	  	60,			2382,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SVGA,	  	30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SVGA,	  	20,			0	,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_XGA,	  	60,			2382,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_XGA,	  	30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_XGA,	  	20,			0	,				0,
	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,	50,			3072,				0,			
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,	60,			3072,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,	24,			2560,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,	25,			2560,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,	30,			2560,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD720,		50,			2560,				0,		
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD720,		60,			2560,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD720,		30,			1536,				0,				
	MEDIA_TYPE_H264,		VIDEO_FORMAT_4CIF,	  	25,			1024,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_4CIF,	  	30,			1024,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_CIF,	  	25,			128	,				0,		
	
	MEDIA_TYPE_H263,		VIDEO_FORMAT_CIF,	  	25,			128	,				0,
	MEDIA_TYPE_MP4,			VIDEO_FORMAT_CIF,	  	25,			128	,				0,	
	MEDIA_TYPE_MP4,			VIDEO_FORMAT_4CIF,	  	25,			3072,				0,	
	MEDIA_TYPE_MP4,			VIDEO_FORMAT_2CIF,	  	25,			1536,				0

	
};


const u16 Mpu2EncResBrLimit[ ][5]=
{
	//  ��Ƶ��ʽ			�ֱ���   					֡��  	��С���ʣ�K��			�Ƿ�HP
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,		50,			1536,				1,			
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,		60,			1536,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,		24,			1024,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,		25,			1024,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,		30,			1024,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD720,			50,			832,				1,		
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD720,			60,			832,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD720,			30,			512,				1,				
	MEDIA_TYPE_H264,		VIDEO_FORMAT_4CIF,	  		25,			128,				1,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_CIF,	  		25,			128,				1,	
			
	MEDIA_TYPE_H264,		VIDEO_FORMAT_VGA,	  		60,			744,				1,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_VGA,	  		30,			372,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_VGA,	  		10,			232,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_VGA,	  		5,			0,					1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SVGA,	  		60,			744,				1,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SVGA,	  		30,			372,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SVGA,	  		10,			232,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SVGA,	  		5,			0,					1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_XGA,	  		60,			744,				1,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_XGA,	  		30,			372,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_XGA,	  		10,			232,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_XGA,	  		5,			0,					1,
			
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SXGA,	  		60,			1153,				1,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SXGA,	  		30,			744,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SXGA,	  		20,			372,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SXGA,	  		10,			232,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SXGA,	  		5,			0,					1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_WXGA,	  		60,			1153,				1,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_WXGA,	  		30,			744,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_WXGA,	  		20,			372,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_WXGA,	  		10,			232,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_WXGA,	  		5,			0,					1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_UXGA,	  		60,			1153,				1,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_UXGA,	  		30,			744,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_UXGA,	  		20,			372,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_UXGA,	  		10,			232,				1,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_UXGA,	  		5,			0,					1,

	MEDIA_TYPE_H264,		VIDEO_FORMAT_UXGA,			60,			2382,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_UXGA,			30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_UXGA,	  		20,			0,					0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SXGA,	  		60,			2382,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SXGA,	  		30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SXGA,	  		20,			0,					0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_WXGA,	  		60,			2382,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_WXGA,	  		30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_WXGA,	  		20,			0,					0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_VGA,	  		60,			2382,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_VGA,	  		30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_VGA,	  		20,			0,					0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SVGA,	  		60,			2382,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SVGA,	  		30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_SVGA,	  		20,			0,					0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_XGA,	  		60,			2382,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_XGA,	  		30,			1153,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_XGA,	  		20,			0,					0,
	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,		50,			3072,				0,			
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,		60,			3072,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,		24,			2560,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,		25,			2560,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD1080,		30,			2560,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD720,			50,			2560,				0,		
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD720,			60,			2560,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_HD720,			30,			1536,				0,				
	MEDIA_TYPE_H264,		VIDEO_FORMAT_4CIF,	  		25,			1024,				0,
	MEDIA_TYPE_H264,		VIDEO_FORMAT_4CIF,	  		30,			1024,				0,	
	MEDIA_TYPE_H264,		VIDEO_FORMAT_CIF,	  		25,			128	,				0,		
	
	MEDIA_TYPE_H263,		VIDEO_FORMAT_CIF,	  			25,			128	,				0,
	MEDIA_TYPE_MP4,			VIDEO_FORMAT_CIF,	  			25,			128	,				0,	
	MEDIA_TYPE_MP4,			VIDEO_FORMAT_4CIF,	  			25,			3072,				0,	
	MEDIA_TYPE_MP4,			VIDEO_FORMAT_2CIF,	  			25,			1536,				0	
		
		
};

//TODO:�ȴ�8kh��8ke�ı���
 const u16 _8KHEncResBrLimit[ ][5]=
 {
	 //  ��Ƶ��ʽ			�ֱ���   				֡��  	��С���ʣ�K��		�Ƿ�HP
	MEDIA_TYPE_NULL,0,0,0,0
 };

 const u16 _8KEEncResBrLimit[ ][5]=
 {
	 //  ��Ƶ��ʽ			�ֱ���   				֡��  	��С���ʣ�K��		�Ƿ�HP
	MEDIA_TYPE_NULL,0,0,0,0
 };


#define MAXNUM_RESBITLIMIT 100
struct TResBrLimitTable 
{
protected:
	u8		 m_byResBrLimitNum;                        //�ֱ����������Ʊ���Ŀ��
	TResBitrateLimit  m_atResBitrateLimit[MAXNUM_RESBITLIMIT];   //�ֱ����������Ʊ���Ϣ

public:
	TResBrLimitTable(void)
	{
		m_byResBrLimitNum = 0;
	}

	//�����ʷֱ�����Ϣ��,mpu/mpu2�贫brdtype����������mpu2ģ��mpu����8ke��8kh����
	BOOL32 ReadResBrLimitTable(u8 byReadBrdType = 0 )
	{
		
		s8 szFileName[KDV_MAX_PATH]={0}; 
		sprintf( szFileName, "%s/mcueqp.ini", DIR_CONFIG );
		s8 szResLimitSectionName[MAX_SECTIONNAME_LEN]={0};
		sprintf(szResLimitSectionName,"%s",ENCPARAMLIMITTABLE);
		BOOL32 bReadFromFile = ReadResLimitFromFile(szFileName,szResLimitSectionName);
		
		//��������ɹ����߱���û������
		if (FALSE == bReadFromFile || 0 == m_byResBrLimitNum )
		{
			EReadDefaultTableType eReadType = eReadTableNone;
#ifdef _8KH_
			eReadType = eRead8KHTable;
#elif defined(_8KE_)
			eReadType = eRead8KETable;
#else  //mpu ,mpu2�ô�������brdtype��ʼ������Ϊ����mpu2ģ��mpu
			
			switch ( byReadBrdType )
			{
			case BRD_TYPE_MPU:
				eReadType = eReadMpuTable;
				break;
			case BRD_TYPE_MPU2:
				eReadType = eReadMpu2Table;
				break;
			default:
				break;
			}
#endif
			
			return ReadDefaultResLimitTable(eReadType);

		}

		return  bReadFromFile;

		
	}

	//���ݷֱ�������������Ϣ������ǰ��������£��ֱ��ʣ�֡���Ƿ���Ҫ�����������ص�����ķֱ��ʺ�֡��
	BOOL32 GetRealResFrameRatebyBitrate(u8 byMediaType,u8& byVideRes,u8& byFramerate,u16 wBitrate,u8 byProfileType = emBpAttrb)
	{
		BOOL32 bRet = FALSE;
		//1������ж�Ӧý�����ͣ��ֱ��ʣ�֡�ʺ�profiletype����С����
		m_byResBrLimitNum = getmin( m_byResBrLimitNum, MAXNUM_RESBITLIMIT);//ȷ�����鲻��Խ��
		BOOL32 bNeedAdjRes = FALSE;
		u8 byLoop = 0;
		u8 byResIndx = 0;
		for (byLoop = 0; byLoop < m_byResBrLimitNum; byLoop++)
		{
			if (m_atResBitrateLimit[byLoop].GetMediaType() == byMediaType &&
				m_atResBitrateLimit[byLoop].GetVideoRes() == byVideRes && 
				m_atResBitrateLimit[byLoop].GetFrameRate() == byFramerate &&
				m_atResBitrateLimit[byLoop].GetPorfileType() == byProfileType
				)
			{
				u16 byMinBitrate = m_atResBitrateLimit[byLoop].GetMinBitrate();
				//������ʴ��ڶ�Ӧ����С���ʣ�����ԭ�ֱ��ʺ�֡�ʣ�����
				if ( byMinBitrate <= wBitrate ) 
				{
					return TRUE;
				}
				else
				{
					//��Ҫ�����ֱ��ʻ�֡��
					bNeedAdjRes = TRUE;
					byResIndx = byLoop;
					break;
				}
			}
		}
		//�������Ҳ�����Ӧ�������FALSE
		if ( bNeedAdjRes == FALSE )
		{
			//����ԭ����
			return FALSE;
		}
		//2�����Ѿ��ź���(����)���������������ң��ҵ���һ�����ϵľ�������Ҫ���ķֱ��ʺ�֡��
		for ( byLoop = byResIndx; byLoop < m_byResBrLimitNum; byLoop++)
		{
			
			//ȡ�������ʵ����֡��
			if ( IsDstSrcResMatched(m_atResBitrateLimit[byLoop].GetVideoRes(),byVideRes) && //�ֱ�������Ҫ����,1080p�����ҵ�vga��ȥ
				m_atResBitrateLimit[byLoop].GetMediaType() == byMediaType &&
				m_atResBitrateLimit[byLoop].GetPorfileType() == byProfileType && 
				m_atResBitrateLimit[byLoop].GetMinBitrate() <= wBitrate &&
				m_atResBitrateLimit[byLoop].GetFrameRate() <= byFramerate
				)
			{
				//�ӱ��������ܷ��ظ���ķֱ���
				if (IsResBigger(m_atResBitrateLimit[byLoop].GetVideoRes() ,byVideRes) )
				{
					//����ԭ����
					return FALSE;
				}
				if ( byFramerate == 50 && m_atResBitrateLimit[byLoop].GetFrameRate() == 30 )
				{
					//�磺1080 50�ҵ�720 30�������������Ҹ�720 25�ġ�60�����ҵ�25����Ϊ30��25��������һ���Ķ�30��ǰ
					continue;
				}
				byFramerate = m_atResBitrateLimit[byLoop].GetFrameRate();
				byVideRes = m_atResBitrateLimit[byLoop].GetVideoRes();
				return TRUE;
			}
		}
		//�Ҳ����ɵ��ķֱ��ʺ�֡�ʣ�����ԭ����
		return bRet;
	}


	//���ݷֱ�������������Ϣ������ǰ��������£��ֱ��ʣ�֡���Ƿ���Ҫ�����������ص�����ķֱ��ʺ�֡��
	BOOL32 GetRealResFrameRatebyBitrate(u8 byMediaType,u16& wVideWidth,u16 &wVideHeight,u8& byFramerate,u16 wBitrate,u8 byProfileType = emBpAttrb)
	{
		BOOL32 bRet = FALSE;
		//1������ж�Ӧý�����ͣ��ֱ��ʣ�֡�ʺ�profiletype����С����
		m_byResBrLimitNum = getmin( m_byResBrLimitNum, MAXNUM_RESBITLIMIT);//ȷ�����鲻��Խ��
		BOOL32 bNeedAdjRes = FALSE;
		u8 byLoop = 0;
		u8 byResIndx = 0;
		u8 byVideRes = GetResViaWH(wVideWidth,wVideHeight);
		for (byLoop = 0; byLoop < m_byResBrLimitNum; byLoop++)
		{
			if (m_atResBitrateLimit[byLoop].GetMediaType() == byMediaType &&
				m_atResBitrateLimit[byLoop].GetVideoRes() == byVideRes && 
				m_atResBitrateLimit[byLoop].GetFrameRate() == byFramerate &&
				m_atResBitrateLimit[byLoop].GetPorfileType() == byProfileType
				)
			{
				u16 byMinBitrate = m_atResBitrateLimit[byLoop].GetMinBitrate();
				//������ʴ��ڶ�Ӧ����С���ʣ�����ԭ�ֱ��ʺ�֡�ʣ�����
				if ( byMinBitrate <= wBitrate ) 
				{
					return TRUE;
				}
				else
				{
					//��Ҫ�����ֱ��ʻ�֡��
					bNeedAdjRes = TRUE;
					byResIndx = byLoop;
					break;
				}
			}
		}
		//�������Ҳ�����Ӧ�������FALSE
		if ( bNeedAdjRes == FALSE )
		{
			//����ԭ����
			return FALSE;
		}
		//2�����Ѿ��ź����������������ң��ҵ���һ�����ϵľ�������Ҫ���ķֱ��ʺ�֡��
		for ( byLoop = byResIndx; byLoop < m_byResBrLimitNum; byLoop++)
		{
			
			//ȡ�������ʵ����֡��
			if ( IsDstSrcResMatched(m_atResBitrateLimit[byLoop].GetVideoRes(),byVideRes) && //�ֱ�������Ҫ����,1080p�����ҵ�vga��ȥ
				m_atResBitrateLimit[byLoop].GetMediaType() == byMediaType &&
				m_atResBitrateLimit[byLoop].GetPorfileType() == byProfileType && 
				m_atResBitrateLimit[byLoop].GetMinBitrate() <= wBitrate&&
				m_atResBitrateLimit[byLoop].GetFrameRate() <= byFramerate
				)
			{
				//�ӱ��������ܷ��ظ���ķֱ���
				if (IsResBigger(m_atResBitrateLimit[byLoop].GetVideoRes() ,byVideRes) )
				{
					//����ԭ����
					return FALSE;
				}
				if (byFramerate == 50 && m_atResBitrateLimit[byLoop].GetFrameRate() == 30 )
				{
					//�磺1080 50�ҵ�720 30�������������Ҹ�720 25�ġ�60�����ҵ�25����Ϊ30��25��������һ���Ķ�30��ǰ
					continue;
				}
				byFramerate = m_atResBitrateLimit[byLoop].GetFrameRate();
				byVideRes = m_atResBitrateLimit[byLoop].GetVideoRes();
				GetWHViaRes(byVideRes,wVideWidth,wVideHeight);
				return TRUE;
			}
		}
		//�Ҳ����ɵ��ķֱ��ʺ�֡��,����ԭ����
		return FALSE;
	}

	void printTable()
	{
		for (u8 byLoop = 0; byLoop < m_byResBrLimitNum;byLoop++)
		{
			m_atResBitrateLimit[byLoop].Print();
		}
		
	}
protected:
	//���ļ��е�������Ϣ��������������
	BOOL32 ReadResLimitFromFile(const s8* pszFileName,const s8* pszResLimitSectionName)
	{
		if (NULL == pszResLimitSectionName || NULL == pszFileName )
		{
			OspPrintf(TRUE,FALSE,"[ReadResLimitFromFile] invalid filename or ResLimitSectionName\n");
			return FALSE;
		}

		BOOL32  bRet = FALSE;
		s32  nEntryNum = 0;
		s8   chSeps[] = " \t"; //space or tab as seperators
		s8  *pchToken;
		u32  dwLoop = 0;
		u32  dwValidNum = 0;
		s8 **lpszTable;
		
    
		//get the number of entry
		bRet = GetRegKeyInt( pszFileName, pszResLimitSectionName, "EntryNum", 0, &nEntryNum );
		if( FALSE == bRet ) 
		{
			OspPrintf(TRUE,FALSE,"[ReadResLimitFromFile] read EntryNum failed\n");
			return bRet;
		}
		if( nEntryNum < 0 )
		{
			OspPrintf(TRUE,FALSE,"[ReadResLimitFromFile] read EntryNum <0 \n");
			return bRet;
		}

		if( nEntryNum > MAXNUM_RESBITLIMIT )
		{
			nEntryNum = MAXNUM_RESBITLIMIT;
		}
		m_byResBrLimitNum = nEntryNum;

		//alloc memory
		lpszTable = (s8 **)malloc( m_byResBrLimitNum * sizeof( s8* ) );
		
		if ( NULL == lpszTable )
		{
			OspPrintf(TRUE,FALSE,"ReadResLimitFromFile Err while malloc for lpszTable\n");
			return bRet;
		}

		for( dwLoop = 0; dwLoop < (u32)m_byResBrLimitNum; dwLoop++ )
		{
			lpszTable[dwLoop] = (s8 *)malloc( MAX_VALUE_LEN+1 );
		}
		
		//get the table string
		u32 dwEntryNum = m_byResBrLimitNum;
		bRet = GetRegKeyStringTable( pszFileName, pszResLimitSectionName, "fail", lpszTable, &dwEntryNum, (MAX_VALUE_LEN+1) );
		if( FALSE == bRet ) 
		{
			printf( "ReadResLimitFromFile Err while reading %s table!\n", pszResLimitSectionName );
			OspPrintf(TRUE,FALSE, "ReadResLimitFromFile Err while reading %s table!\n", pszResLimitSectionName );
			//  �ͷ��ڴ�
			FREE_MEM(lpszTable, m_byResBrLimitNum);	
			return bRet;
		}
		m_byResBrLimitNum = (u8)getmin(dwEntryNum,m_byResBrLimitNum); //���ܶ�������ʵ��entry����ǰ�����entrynumС
		for( dwLoop = 0; dwLoop < m_byResBrLimitNum; dwLoop++ )
		{
        
			pchToken = NULL;
			if (lpszTable && lpszTable[dwValidNum])
			{
				pchToken = strtok( lpszTable[dwValidNum], chSeps );
			}		

			//mediatype		
			if( NULL == pchToken )
			{
				printf( "ReadResLimitFromFile Err while reading %s entryID.%d!\n", "mediatype", dwLoop );
				OspPrintf(TRUE,FALSE, "ReadResLimitFromFile Err while reading %s entryID.%d!\n", "mediatype", dwLoop );
				continue;
			}
			else
			{
				m_atResBitrateLimit[dwValidNum].SetMeidaType( (u8)atoi(pchToken) ) ;
				
			}

			//resolution
			pchToken = strtok( NULL, chSeps );
			if( NULL == pchToken )
			{
				printf( "ReadResLimitFromFile Err while reading %s entryID.%d!\n", "resolution", dwLoop );
				OspPrintf(TRUE,FALSE, "ReadResLimitFromFile Err while reading %s entryID.%d!\n", "resolution", dwLoop );
				continue;
			}
			else
			{
				m_atResBitrateLimit[dwValidNum].SetVideoRes( (u8)atoi(pchToken) );
			}
			//framerate
			pchToken = strtok( NULL, chSeps );
			if( NULL == pchToken )
			{
				printf( "ReadResLimitFromFile Err while reading %s entryID.%d!\n", "framerate", dwLoop );
				OspPrintf(TRUE,FALSE, "ReadResLimitFromFile Err while reading %s entryID.%d!\n", "framerate", dwLoop );
				continue;
			}
			else
			{
				m_atResBitrateLimit[dwValidNum].SetFrameRate( (u8)atoi(pchToken) );
			}
			
			//minbitrate
			pchToken = strtok( NULL, chSeps );
			if( NULL == pchToken )
			{
				printf( "ReadResLimitFromFile Err while reading %s entryID.%d!\n", "minbitrate", dwLoop );
				OspPrintf(TRUE,FALSE, "ReadResLimitFromFile Err while reading %s entryID.%d!\n", "minbitrate", dwLoop );
				continue;
			}
			else
			{
				m_atResBitrateLimit[dwValidNum].SetMinBitrate( (u16)atoi(pchToken) );
			}
			//hp/bp
			pchToken = strtok( NULL, chSeps );
			if( NULL == pchToken )
			{
				printf( "ReadResLimitFromFile Err while reading %s entryID.%d!\n", "profiletype", dwLoop );
				OspPrintf(TRUE,FALSE, "ReadResLimitFromFile Err while reading %s entryID.%d!\n", "profiletype", dwLoop );
				continue;
			}
			else
			{
				m_atResBitrateLimit[dwValidNum].SePorfileType( (atoi(pchToken) == 1 ? emHpAttrb:emBpAttrb ) );
			}
        
			dwValidNum++;
		}

		FREE_MEM(lpszTable, m_byResBrLimitNum);
    
		m_byResBrLimitNum = (u8)dwValidNum;
		if ( m_byResBrLimitNum == 0 )
		{
			OspPrintf(TRUE,FALSE,"[ReadResLimitFromFile]error:no related tableinfo\n");
			return FALSE;
		}
		if ( m_byResBrLimitNum > 1)
		{
			//�������������źã������Ļ���Ҫ����ʱ��ֻҪindex++�����ҵ������һ�����ϵľͿ���
			QuickSortTableInfo(0,m_byResBrLimitNum - 1 );
		}
		bRet = TRUE;
		return bRet;
	}


	BOOL32 ReadDefaultResLimitTable(EReadDefaultTableType eReadTableType)
	{
		if (eReadTableType == eReadTableNone)
		{
			OspPrintf(TRUE,FALSE,"[ReadDefaultResLimitTable] error :invalid param!\n");
			return FALSE;
		}

		u8 byResBrLimitNum = 0;
		switch (eReadTableType)
		{
		case eReadMpuTable:
			byResBrLimitNum = sizeof(MpuEncResBrLimit)/(sizeof(u16)*5);//ÿ5�����ݱ�ʾһ������
			break;
		case eReadMpu2Table:
			byResBrLimitNum = sizeof(Mpu2EncResBrLimit)/(sizeof(u16)*5);//ÿ5�����ݱ�ʾһ������
			break;
		case eRead8KHTable:
			byResBrLimitNum = sizeof(_8KHEncResBrLimit)/(sizeof(u16)*5);//ÿ5�����ݱ�ʾһ������
			break;
		case eRead8KETable:
			byResBrLimitNum = sizeof(_8KEEncResBrLimit)/(sizeof(u16)*5);//ÿ5�����ݱ�ʾһ������
			break;
		default:
			break;
		}
		u8 byValidNum = 0;
		for (u8 byLoop = 0; byLoop < byResBrLimitNum; byLoop++)
		{

			u8 byMediaType = MEDIA_TYPE_NULL;
			u8 byRes = VIDEO_FORMAT_INVALID;
			u8 byFrameRate = 0;
			u16 wMinBitrate = 0;
			u8 byProfileType = emBpAttrb;
			switch (eReadTableType)
			{
			case eReadMpuTable:
				byMediaType = (u8)MpuEncResBrLimit[byLoop][0];
				byRes = (u8)MpuEncResBrLimit[byLoop][1];
				byFrameRate = (u8)MpuEncResBrLimit[byLoop][2];
				wMinBitrate = MpuEncResBrLimit[byLoop][3];
				byProfileType = (u8)MpuEncResBrLimit[byLoop][4];
				break;
			case eReadMpu2Table:
				byMediaType = (u8)Mpu2EncResBrLimit[byLoop][0];
				byRes = (u8)Mpu2EncResBrLimit[byLoop][1];
				byFrameRate = (u8)Mpu2EncResBrLimit[byLoop][2];
				wMinBitrate = Mpu2EncResBrLimit[byLoop][3];
				byProfileType = (u8)Mpu2EncResBrLimit[byLoop][4];
				break;
			case eRead8KHTable:
 				byMediaType = (u8)_8KHEncResBrLimit[byLoop][0];
 				byRes = (u8)_8KHEncResBrLimit[byLoop][1];
 				byFrameRate = (u8)_8KHEncResBrLimit[byLoop][2];
 				wMinBitrate = _8KHEncResBrLimit[byLoop][3];
 				byProfileType = (u8)_8KHEncResBrLimit[byLoop][4];
 				break;
 			case eRead8KETable:
 				byMediaType = (u8)_8KEEncResBrLimit[byLoop][0];
 				byRes = (u8)_8KEEncResBrLimit[byLoop][1];
 				byFrameRate = (u8)_8KEEncResBrLimit[byLoop][2];
 				wMinBitrate = _8KEEncResBrLimit[byLoop][3];
 				byProfileType = (u8)_8KEEncResBrLimit[byLoop][4];
				break;
			default:
				break;
			}
			if ( MEDIA_TYPE_NULL == byMediaType )
			{
				continue;
			}
			
			//mediatype		
			m_atResBitrateLimit[byValidNum].SetMeidaType( byMediaType ) ;
			//res
			m_atResBitrateLimit[byValidNum].SetVideoRes( byRes  );
			//framerate
			m_atResBitrateLimit[byValidNum].SetFrameRate( byFrameRate );
			//minbitrate
			m_atResBitrateLimit[byValidNum].SetMinBitrate(wMinBitrate );
			//profiletype
			m_atResBitrateLimit[byValidNum].SePorfileType( byProfileType );
			byValidNum++;
		}

		m_byResBrLimitNum = byValidNum;
		if ( m_byResBrLimitNum == 0 )
		{
			OspPrintf(TRUE,FALSE,"[ReadDefaultResLimitTable]error:no related defaulttable\n");
			return FALSE;
		}
		if ( m_byResBrLimitNum > 1)
		{
			//�������������źã������Ļ���Ҫ����ʱ��ֻҪindex++�����ҵ������һ�����ϵľͿ���
			QuickSortTableInfo(0,m_byResBrLimitNum - 1 );
		}
		

		return TRUE;
	}

	//������profiletype,ý�����ͣ��ֱ��ʣ�֡������,����
	void QuickSortTableInfo( u8 byStart,u8 byEnd )
	{
		//profiletype 1->0  ý�����ͣ�106(h264)->97(mpeg4)->34(h263)
		//�ֱ��ʣ�IsResBigger���Ƚ� ֡�ʣ�����ֵ�ɴ�С
		if (byStart >= byEnd)
		{
			return;
		}
		u8 byLeft = byStart,byRight = byEnd;
		TResBitrateLimit tMidLimit = m_atResBitrateLimit[byStart];
		while (byLeft < byRight)
		{
			while (byLeft < byRight && !IsTableInfoBigger(m_atResBitrateLimit[byRight] ,tMidLimit))
			{
				byRight--;
			}
			m_atResBitrateLimit[byLeft] = m_atResBitrateLimit[byRight];
			while(byLeft < byRight && IsTableInfoBigger(m_atResBitrateLimit[byLeft],tMidLimit))
			{

				byLeft++;
			}
			m_atResBitrateLimit[byRight] = m_atResBitrateLimit[byLeft]; 
		}
		m_atResBitrateLimit[byLeft] = tMidLimit;
		QuickSortTableInfo(byStart,byRight - 1);
		QuickSortTableInfo(byRight+1,byEnd);
		
	}

	BOOL32 IsMediaTypeG(const u8& byFstMediaType,const u8& bySecMediaType)
	{	
		const u8 abyMediaType[] = { MEDIA_TYPE_H263,
			MEDIA_TYPE_MP4,
			MEDIA_TYPE_H264,
		};
		
		u8 byFstIdx = 0xff;
		u8 bySecIdx = 0xff;
		u8 byLop = 0;
		
		// ��¼�ڷֱ��������е�λ�ã����ڱȽϷֱ��ʴ�С
		for ( byLop = 0; byLop < sizeof(abyMediaType)/sizeof(u8); byLop++ )
		{
			if ( abyMediaType[byLop] == byFstMediaType )
			{
				if ( byFstIdx == 0xff )
				{
					byFstIdx = byLop;    
				}
			}
			if ( abyMediaType[byLop] == bySecMediaType )
			{
				if ( bySecIdx == 0xff )
				{
					bySecIdx = byLop;
				}
			}
			if ( byFstIdx != 0xff && bySecIdx != 0xff )
			{
				break;
			}
		}

		if (byFstIdx == 0xff || bySecIdx == 0xff)
		{
			
			return FALSE;
		}
		
		if (byFstIdx <= bySecIdx)
		{
			return FALSE;
		}
		return TRUE;
	}

	BOOL32 IsTableInfoBigger(TResBitrateLimit tFirst,TResBitrateLimit tSecond)
	{
		//�ȱ�hp/bp,�ٱ�ý�����ͣ��ٱȷֱ��ʣ��ٱ�֡��
		if (tFirst.GetPorfileType() > tSecond.GetPorfileType())
		{
			return TRUE;
		}
		else if (tFirst.GetPorfileType() == tSecond.GetPorfileType())
		{
			//ͬprofiletype����ý������
			if (IsMediaTypeG(tFirst.GetMediaType(),tSecond.GetMediaType()))
			{
				return TRUE;
			}
			else if ( tFirst.GetMediaType() == tSecond.GetMediaType() )
			{
				//ͬprofiletype,medaitype ,�ȷֱ���
				if ( IsResBigger(tFirst.GetVideoRes(),tSecond.GetVideoRes()) )
				{
					return TRUE;
				}
				else if ( tFirst.GetVideoRes() == tSecond.GetVideoRes() )
				{
					//���϶���ͬ���ȷֱ���
					if (tFirst.GetFrameRate() > tSecond.GetFrameRate() )
					{
						return TRUE;
					}
				}
			}
		}
		//�������������false
		return FALSE;
	}


	
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

#ifdef WIN32
    #pragma pack( pop )
#endif

#endif // _PERIPHERY_EQUIPMENT_CONFIG_H_
