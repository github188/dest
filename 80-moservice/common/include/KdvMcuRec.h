/*****************************************************************************
   ģ����      : KDV MCU
   �ļ���      : KdvMcuRec.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: KDV Mcu Rec COM �����װ����ӿ��ṩ
   ����        : 
   �汾        : V4.0  Copyright(C) 2007-2009 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2007/08/08  4.0         ����      ����
******************************************************************************/

#ifndef _KDVMCUREC_INTERFACE_H_
#define _KDVMCUREC_INTERFACE_H_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXLEN_ALIAS         128           // �������ơ�¼��������ĳ���
#define MAXNUM_RECCHNNL      32            // ���¼��ͨ����
#define MAXNUM_STREAM_PERREC 3             // ÿ��¼������������
#define MCU_LISTEN_PORT      60000         // Ĭ�ϵ�MCU�����˿�


enum emStreamType
{
    emStreamTypeNone    =   0,  /*δ֪������*/
    emStreamTypeAudio   =   1,  /*��Ƶ��*/
    emStreamTypeVideo   =   2,  /*��Ƶ��*/
    emStreamTypeDStream =   3   /*�ڶ�·��Ƶ*/
};

enum emPublishMode
{
    emPublishNone           =   0,  /*������*/
    emPublishImmediately    =   1,  /*��������*/
    emPublishFinished       =   2   /*¼�귢��*/
};

enum emRecState
{
    emRecStateStop = 0,             /*û����¼��*/
    emRecStateRecording = 1,        /*����¼��*/
    emRecStatePaused = 2            /*¼����ͣ��*/
};

// �Ա���̬���������궨��
enum emRecGenErr
{
    emRecSucceed            =   0,
        
    //emCfgErr: 1-20
    emErrCfgID              =   1,
    emErrCfgAlias           =   2,
    emErrCfgRecIp           =   3,
    emErrCfgRecPort         =   4,
    emErrCfgRecChnNum       =   5,
    emErrCfgPlayChnNum      =   6,
    emErrCfgTotalChnNum     =   7,
    emErrCfgMcuIp           =   8,
    emErrCfgMcuPort         =   9,
    
    //emOprErr: 21-50
    emErrSetNetRcvParam     =   21,
    emErrConfID             =   22,
    emErrStopRec            =   23,
    emErrStopWhileRecready  =   24

    //emOtherException: 51+
};


//¼�����Ҫ���յ��ϲ�������Ϣ len:140
struct TRecCfgInfo
{
    TRecCfgInfo():m_dwRecIpAddr(0),
               m_wRecStartPort(0),
               m_byEqpId(0),
               m_byRecChnNum(MAXNUM_RECCHNNL),
               m_dwMcuIpAddr(0)
    {
        memset( m_achAlias, 0, sizeof(m_achAlias) );
        SetMcuPort(MCU_LISTEN_PORT);
    };
    ~TRecCfgInfo(){};
    
public:
    void SetRecIp(unsigned long dwIpAddr) { m_dwRecIpAddr = htonl(dwIpAddr);    }
    unsigned long  GetRecIp(void) const { return ntohl(m_dwRecIpAddr);    }
    void SetRecAlias(LPCSTR pszAlias)
    {
        if ( NULL != pszAlias )
        {
            unsigned char byStrLen = min(strlen(pszAlias), MAXLEN_ALIAS-1);
            strncpy(m_achAlias, pszAlias, byStrLen);
            m_achAlias[byStrLen] = '\0';
        }
    }
    LPCSTR GetRecAlias(void) const { return m_achAlias;    }
    void SetRecStartPort(unsigned short wPort) { m_wRecStartPort = htons(wPort);    }
    unsigned short  GetRecStartPort(void) const { return ntohs(m_wRecStartPort);    }
    void SetEqpId(unsigned char byEqpId) { m_byEqpId = byEqpId;    }
    unsigned char   GetEqpId(void) const { return m_byEqpId;    }
    void SetRecChnNum(unsigned char byNum) { m_byRecChnNum = byNum;    }
    unsigned char   GetRecChnNum(void) const { return m_byRecChnNum;    }
    void SetMcuIp(unsigned long dwMcuIp) { m_dwMcuIpAddr = htonl(dwMcuIp);    }
    unsigned long  GetMcuIp(void) const  { return ntohl(m_dwMcuIpAddr);    }
    void SetMcuPort(unsigned short wMcuPort) { m_wMcuConnPort = htons(wMcuPort);    }
    unsigned short  GetMcuPort(void) const { return ntohs(m_wMcuConnPort);    } 

protected:
    unsigned long  m_dwRecIpAddr;                  //¼���IP��ַ
    char           m_achAlias[MAXLEN_ALIAS];       //����
    unsigned short m_wRecStartPort;                //¼��ͨ����ʼ�˿ں�
    unsigned char  m_byEqpId;                      //����ID
    unsigned char  m_byRecChnNum;                  //¼��ͨ����

    unsigned long  m_dwMcuIpAddr;                  //MCU IP��ַ
    unsigned short m_wMcuConnPort;                 //MCU ͨ�Ŷ˿ں�
    
    unsigned long  m_dwReserved1;
    unsigned long  m_dwReserved2;    
};


//¼��������״̬: len: 12
struct TRecConfStatus
{
public:
    void SetIndex(unsigned long dwIdx)
    {
        m_dwChnlIndex = dwIdx;
    }
    unsigned long GetIndex() const
    {
        return m_dwChnlIndex;
    }
    void SetRecState(emRecState emState) { m_emRecState = emState; }
    emRecState GetRecState(void) const{ return m_emRecState;    }
    void SetErrCode(unsigned long dwErr) { m_dwErrCode = htonl(dwErr); }
    unsigned long GetErrCode(void) const { return ntohl(m_dwErrCode) ;  }

protected:
    unsigned long   m_dwChnlIndex;                   // ¼��ͨ���������ţ�Ψһ��ʶһ������¼��ֵ>0�������Ϊ0���ʾΪ����¼�����״̬
    emRecState      m_emRecState;                    // ����¼���״̬��emRecState
    unsigned long   m_dwErrCode;                     // ¼�����ǰ�Ĵ����루״̬)���������� emRecGenErr ����
    unsigned long   m_dwReserved;                    // ����
};


//��������Խṹ��len:276
struct TConfProperty
{
    TConfProperty(): m_dwChnlIndex(0),
                     m_dwDuration(0),
                     m_bStopMeetingManual(TRUE),
                     m_dwTotalBitRate(0),
                     m_dwReserved(0)
    {
        memset(m_achConfName, 0, sizeof(m_achConfName));
        memset(m_achRecName, 0, sizeof(m_achRecName));
    }
public:
    void SetIndex(unsigned long dwIdx)
    {
        m_dwChnlIndex = dwIdx;
    }
    unsigned long GetIndex() const
    {
        return m_dwChnlIndex;
    }
    void SetDuration(unsigned long dwDur){ m_dwDuration = htonl(dwDur);    }
    unsigned long  GetDuration(void) const { return ntohl(m_dwDuration);    }
    void SetStopManual(BOOL bManual) { m_bStopMeetingManual = bManual;    }
    BOOL IsStopManual(void) const { return m_bStopMeetingManual;    }
    void SetTotalBitRate(unsigned long dwBR) { m_dwTotalBitRate = htonl(dwBR);    }
    unsigned long  GetTotalBitRate(void) const { return ntohl(m_dwTotalBitRate);    }
    void SetConfName(LPCSTR pszAlias)
    {
        if ( NULL != pszAlias )
        {
            unsigned char byStrLen = min(strlen(pszAlias), MAXLEN_ALIAS-1);
            strncpy(m_achConfName, pszAlias, byStrLen);
            m_achConfName[byStrLen] = '\0';
        }
    }
    LPCSTR GetConfName(void) const { return m_achConfName;    }

    void SetRecName(LPCSTR pszAlias)
    {
        if ( NULL != pszAlias )
        {
            unsigned char byStrLen = min(strlen(pszAlias), MAXLEN_ALIAS-1);
            strncpy(m_achRecName, pszAlias, byStrLen);
            m_achRecName[byStrLen] = '\0';
        }
    }
    LPCSTR GetRecName(void) const { return m_achRecName;    }

protected:
    unsigned long  m_dwChnlIndex;                   // ¼��ͨ���������ţ�Ψһ��ʶһ������¼��
    char	 m_achConfName[MAXLEN_ALIAS];           // ������
    char     m_achRecName[MAXLEN_ALIAS];            // ¼����
	unsigned long	 m_dwDuration;                  // ����ʱ��
	BOOL m_bStopMeetingManual;                      // �Ƿ��ֶ�����
	unsigned long	 m_dwTotalBitRate;              // ������
	unsigned long  m_dwReserved;                    // ����
};

//������
struct TStreamParam
{
public:
    void SetStreamType(emStreamType emType)
    {
        m_emStreamType = emType;
    }
    emStreamType GetStreamType() const
    {
        return m_emStreamType;
    }

    void SetBitrate(unsigned long dwBitrate)
    {
        m_dwBitRate = dwBitrate;
    }

    unsigned long GetBitrate() const
    {
        return m_dwBitRate;
    }

    void SetAudFormat(unsigned short wFormat){ m_tSParam.m_tAudioParam.m_wTwoCC = htons(wFormat);    }
    unsigned long  GetAudFormat(void) const { return ntohs(m_tSParam.m_tAudioParam.m_wTwoCC);    }
    void SetAudSamplesPerSec(unsigned long dwSamples){ m_tSParam.m_tAudioParam.m_dwSamplesPerSec = htonl(dwSamples); }
    unsigned long  GetAudSamplesPerSec(void) const { return ntohl(m_tSParam.m_tAudioParam.m_dwSamplesPerSec);    }
    void SetAudBitsPerSample(unsigned long dwBits){ m_tSParam.m_tAudioParam.m_dwBitsPerSample = htonl(dwBits);    }
    unsigned long  GetAudBitsPerSample(void) const { return ntohl(m_tSParam.m_tAudioParam.m_dwBitsPerSample); }
    void SetAudChnlNum(unsigned char byNum){ m_tSParam.m_tAudioParam.m_byChnnlNum = byNum; }
    unsigned char   GetAudChnlNum(void) const { return  m_tSParam.m_tAudioParam.m_byChnnlNum; }
    void SetAudCodecSpecificDataSize(unsigned short wDataSize) { m_tSParam.m_tAudioParam.m_wCodeSpecificDataSize = wDataSize;    }
    unsigned short   GetAudCodecSpecificDataSize(void) const { return m_tSParam.m_tAudioParam.m_wCodeSpecificDataSize;    }
    void SetAudCodecSpecificData(unsigned char * pbyData)
    {
        m_tSParam.m_tAudioParam.m_pbyCodeSpecificData = pbyData;
    }
    unsigned char *GetAudCodecSpecificData(void) const { return m_tSParam.m_tAudioParam.m_pbyCodeSpecificData;    }
    
    void SetAudBlockAlignment(unsigned short wAlignment){ m_tSParam.m_tAudioParam.m_wBlockAlignment = wAlignment;   }
    unsigned short GetAudBlockAlignment(void) const { return m_tSParam.m_tAudioParam.m_wBlockAlignment;    }
    void SetAudAverageNumOfBytesPerSecond(unsigned long dwNPS) { m_tSParam.m_tAudioParam.m_dwAverageNumOfBytesPerSecond = dwNPS;    }
    unsigned long GetAudAverageNumOfBytesPerSecond(void) const { return m_tSParam.m_tAudioParam.m_dwAverageNumOfBytesPerSecond;    }
    

    void SetVidFormat(unsigned long dwFormat){ m_tSParam.m_tVideoParam.m_dwFourCC = htonl(dwFormat);    }
    unsigned long  GetVidFormat(void) const { return ntohl(m_tSParam.m_tVideoParam.m_dwFourCC);    }
    void SetVidFramesPerSec(unsigned long dwFrames) { m_tSParam.m_tVideoParam.m_dwFramesPerSec = htonl(dwFrames);    }
    unsigned long  GetVidFramesPerSec(void) const { return ntohl(m_tSParam.m_tVideoParam.m_dwFramesPerSec);    }
    void SetVidImageHeight(unsigned long dwHight) { m_tSParam.m_tVideoParam.m_dwImageHeight = htonl(dwHight);    }
    unsigned long  GetVidImageHeight(void) const { return ntohl(m_tSParam.m_tVideoParam.m_dwImageHeight);    }
    void SetVidImageWidth(unsigned long dwWidth) { m_tSParam.m_tVideoParam.m_dwImageWidth = htonl(dwWidth);    }
    unsigned long  GetVidImageWidth(void) const { return ntohl(m_tSParam.m_tVideoParam.m_dwImageWidth);    }
    void SetVidBitCounts(unsigned char byBC) { m_tSParam.m_tVideoParam.m_byBitCounts = byBC;    }
    unsigned char   GetVidBitCounts(void) const { return m_tSParam.m_tVideoParam.m_byBitCounts;    }
    
    void SetVidImageSize(unsigned long dwSize) { m_tSParam.m_tVideoParam.m_dwImageSize = dwSize;    }
    unsigned long GetVidImageSize(void) const { return m_tSParam.m_tVideoParam.m_dwImageSize;    }
    void SetVidHorizontalPixelsPerMeter(signed long nHPPM) { m_tSParam.m_tVideoParam.m_nHorizontalPixelsPerMeter = nHPPM;    }
    signed long GetVidHorizontalPixelsPerMeter(void) const { return m_tSParam.m_tVideoParam.m_nHorizontalPixelsPerMeter;    }
    void SetVidVerticalPixelsPerMeter(signed long nVPPM) { m_tSParam.m_tVideoParam.m_nVerticalPixelsPerMeter = nVPPM;   }
    signed long GetVidVerticalPixelsPerMeter(void) const { return m_tSParam.m_tVideoParam.m_nVerticalPixelsPerMeter;    }
    void SetVidColorsUsedCount(unsigned long dwCount) { m_tSParam.m_tVideoParam.m_dwColorsUsedCount = dwCount;    }
    unsigned long GetVidColorsUsedCount(void) const { return m_tSParam.m_tVideoParam.m_dwColorsUsedCount;    }
    void SetVidImportantColorsCount(unsigned long dwCount) { m_tSParam.m_tVideoParam.m_dwImportantColorsCount = dwCount;   }
    unsigned long GetVidImportantColorsCount(void) const { return m_tSParam.m_tVideoParam.m_dwImportantColorsCount;    }

protected:
    emStreamType  m_emStreamType;        // �����ͣ��μ� emStreamType
    unsigned long m_dwBitRate;           // �����������ʣ���λ��bps
    union UTSParam
    {
        struct TAudioStreamParam
        {        
            unsigned long  m_dwSamplesPerSec;       // ÿ�������
            unsigned long  m_dwBitsPerSample;       // ÿ��������λ��
            unsigned short m_wTwoCC;                // ��Ƶ����
            unsigned char  m_byChnnlNum;            // ������
            unsigned short m_wCodeSpecificDataSize; // ��ƵCodec��ʶ
            unsigned char *m_pbyCodeSpecificData;   // ��ƵCodec��ʶ
            unsigned short m_wBlockAlignment;       // ���ֽڶ�����
            unsigned long  m_dwAverageNumOfBytesPerSecond;  // ƽ������ Bytes / Second
        }m_tAudioParam;
        
        struct TVideoStreamParam
        {
            unsigned long  m_dwFourCC;          //��Ƶ����
            unsigned long  m_dwFramesPerSec;    //֡�ʣ�the number of frames per second
            unsigned long  m_dwImageWidth;      //ͼ���Specifies the width of the bitmap, in pixels
            unsigned long  m_dwImageHeight;     //ͼ��ߣ�Specifies the height of the bitmap, in pixels
            unsigned char  m_byBitCounts;       //λ�Specifies the number of bits per pixel.
            unsigned long  m_dwImageSize;					// ����ߴ�
            signed long    m_nHorizontalPixelsPerMeter;		// ˮƽ�ֱ���
            signed long    m_nVerticalPixelsPerMeter;		// ��ֱ����ֱ���
            unsigned long  m_dwColorsUsedCount;				// ��ɫ��
            unsigned long  m_dwImportantColorsCount;        // ��Ҫ��ɫ����
            
        }m_tVideoParam;
    }m_tSParam;
};

//¼�����Խṹ.len��
struct TRecProperty
{
    TRecProperty():m_byPublishMode(emPublishNone),
                   m_byTotalStreamNum(0),
                   m_dwReserved(0)
    {
        memset(&m_atStream, 0, sizeof(m_atStream));
    }
public:
    void SetPublishMode(unsigned char byMode) { m_byPublishMode = byMode;    }
    unsigned char   GetPublishMode(void) const { return m_byPublishMode;    }
    void SetTotalStreamNum(unsigned char byNum) { m_byTotalStreamNum = byNum;    }
    unsigned char   GetTotalStreamNum(void) const { return m_byTotalStreamNum;   }
    void SetStreamParam(unsigned char byStreamIdx, TStreamParam &tStreamParam)
    {
        if ( byStreamIdx > 2 )
        {
            OspPrintf( TRUE, FALSE, "[TRecProperty] SetStreamParam SIdx.%d, ignore\n", byStreamIdx );
            return;
        }
        m_atStream[byStreamIdx] = tStreamParam;
    }
    TStreamParam *GetStreamParam(unsigned char byStreamIdx)
    {
        if ( byStreamIdx > 2 )
        {
            OspPrintf( TRUE, FALSE, "[TRecProperty] GetStreamParam SIdx.%d, ignore\n", byStreamIdx );
            return NULL;
        }        
        return &m_atStream[byStreamIdx];
    }

protected:
    unsigned char  m_byPublishMode;        // ������¼�ļ��ķ���ģʽ���μ� emPublishMode
    unsigned char  m_byTotalStreamNum;     // ¼���а�����������
    TStreamParam   m_atStream[MAXNUM_STREAM_PERREC];    //ÿ�����Ĳ������±�Ϊ����-1��
	unsigned long  m_dwReserved;           // ����
};

//֡ͷ���Խṹ.len: 36+xx
struct TFrameHead
{
    TFrameHead():m_pbyDataBuf(NULL),
                 m_dwDataBufLen(0),
                 m_dwTimeStamp(0),
                 m_bKeyFrame(FALSE)
    {
    }
public:
    
    void SetBufHead(unsigned char *pbyBufHead) { m_pbyDataBuf = pbyBufHead;    }
    unsigned char*  GetBufHead(void) const { return m_pbyDataBuf;    }
    void SetBufLen(unsigned long dwLen) { m_dwDataBufLen = htonl(dwLen);    }
    unsigned long  GetBufLen(void) const { return ntohl(m_dwDataBufLen);    }
    void SetTStamp(unsigned long dwTStamp) { m_dwTimeStamp = htonl(dwTStamp);    }
    unsigned long  GetTStamp(void) const { return ntohl(m_dwTimeStamp);    }

    void SetIsKeyFrame(BOOL bKeyFrame) { m_bKeyFrame = bKeyFrame;    }
    BOOL GetIsKeyFrame(void) const { m_bKeyFrame;    }

protected:        
    unsigned char  *m_pbyDataBuf; //֡���ݻ���
    unsigned long m_dwDataBufLen; //֡���ݻ�������С
    unsigned long m_dwTimeStamp;  //ʱ�����Time Stamp
    BOOL          m_bKeyFrame;    //������Ƶ��Ч���Ƿ��ǹؼ�֡
    unsigned long dwReserved;
};

//ͨ�������ṹ.len: 46+xx
struct TStreamProperty
{
    TStreamProperty():m_dwConfIndex(0),
                      m_byStreamIdx(0),
                      m_dwReserved(0){}
public:
    void SetConfIndex(unsigned long dwIdx)
    {
        m_dwConfIndex = dwIdx;
    }
    unsigned long GetConfIndex() const
    {
        return m_dwConfIndex;
    }
    void SetStreamIndex(unsigned char byChannel) { m_byStreamIdx = byChannel;    }
    unsigned char   GetStreamIndex(void) const { return m_byStreamIdx;    }
    
    void SetFrameHead(TFrameHead &tFrameHead) { m_tFrameHead = tFrameHead;    }
    TFrameHead * GetFrameHead(void) { return &m_tFrameHead;    }

    void SetNull(){ memset(this, 0, sizeof(TStreamProperty)); }
    
protected:
    unsigned long m_dwConfIndex;          // ��������¼��ͨ��������
    unsigned char  m_byStreamIdx;         // ����������        
    TFrameHead m_tFrameHead;              // ����֡ͷ���Խṹ
    unsigned long m_dwReserved;           // ����
};


#endif // _KDVMCUREC_INTERFACE_H_