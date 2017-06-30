/*****************************************************************************
   ģ����      : Recorder
   �ļ���      : RecGenInfo.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: Rec�������װ�������ݽṹ�ṩ
   ����        : 
   �汾        : V4.0  Copyright(C) 2007-2009 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2007/08/08  4.0         �ű���      ����
******************************************************************************/
#ifndef _REC_GENINFO_H_
#define _REC_GENINFO_H_

#include "osp.h"
#define _EXCLUDE_VCCOMMON_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
typedef enum MC_STATUS
{
    MCU_MEETING_START	=	0,  //���鿪ʼ��¼��ʼ����֮�෴
    MCU_MEETING_STOP	=	1   //����ֹͣ��¼��ֹͣ����֮�෴
}MC_STATUS;
*/

enum emMediaType
{
    emMediaTypeNone     =   0,  /*δ֪ý������*/
        
    // ��Ƶ
    emMediaTypePCMU     =   1,  /*G.711 ulaw  mode 6*/
    emMediaTypePCMA     =   2,  /*G.711 Alaw  mode 5*/
    emMediaTypeG721     =   3,  /*G.721*/
    emMediaTypeG722	    =   4,  /*G.722*/
    emMediaTypeG7221    =   5,  /*G.7221*/
    emMediaTypeG7221C   =   6,  /*G722.1.C Siren14*/
    emMediaTypeG7231    =   7,  /*G.7231*/
    emMediaTypeG728     =   8,  /*G.728*/
    emMediaTypeG729     =   9,  /*G.729*/
    emMediaTypeADPCM    =   10, /*DVI4 ADPCM*/
    emMediaTypeMP3      =   11, /*mp3 mode 0-4*/
    
    // ��Ƶ
    emMediaTypeMP4      =   12, /*MPEG-4*/
    emMediaTypeH261     =   13, /*H.261*/
    emMediaTypeH262     =   14, /*H.262 (MPEG-2)*/
    emMediaTypeH263     =   15, /*H.263*/
    emMediaTypeH263PLUS =   16, /*H.263+*/
    emMediaTypeH264     =   17  /*H.264*/
};

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
    emErrCfgMcuPort         =   9

    //emOprErr: 21-50

    //emOtherException: 51+

};

#define MAXLEN_ALIAS 128

//¼�����Ҫ���յ��ϲ�������Ϣ len:140
struct TRecCfgInfo
{
    TRecCfgInfo():m_dwRecIpAddr(0),
                  m_wRecStartPort(0),
                  m_byEqpId(RECORDERID_MIN),
                  m_byRecChnNum(MAXNUM_RECORDER_CHNNL),
                  m_dwMcuIpAddr(0),
                  m_wMcuConnPort(MCU_LISTEN_PORT)
    {
        memset( m_achAlias, 0, sizeof(m_achAlias) );
    };
    ~TRecCfgInfo(){};
    
public:
    void SetRecIp(u32 dwIpAddr) { m_dwRecIpAddr = htonl(dwIpAddr);    }
    u32  GetRecIp(void) const { return ntohl(m_dwRecIpAddr);    }
    void SetRecAlias(LPCSTR pszAlias)
    {
        if ( NULL != pszAlias )
        {
            u8 byStrLen = min(strlen(pszAlias), MAXLEN_ALIAS-1);
            strncpy(m_achAlias, pszAlias, byStrLen);
            m_achAlias[byStrLen] = '\0';
        }
    }
    LPCSTR GetRecAlias(void) const { return m_achAlias;    }
    void SetRecStartPort(u16 wPort) { m_wRecStartPort = htons(wPort);    }
    u16  GetRecStartPort(void) const { return ntohs(m_wRecStartPort);    }
    void SetEqpId(u8 byEqpId) { m_byEqpId = byEqpId;    }
    u8   GetEqpId(void) const { return m_byEqpId;    }
    void SetRecChnNum(u8 byNum) { m_byRecChnNum = byNum;    }
    u8   GetRecChnNum(void) const { return m_byRecChnNum;    }
    void SetMcuIp(u32 dwMcuIp) { m_dwMcuIpAddr = htonl(dwMcuIp);    }
    u32  GetMcuIp(void) const  { return ntohl(m_dwMcuIpAddr);    }
    void SetMcuPort(u16 wMcuPort) { m_wMcuConnPort = htons(wMcuPort);    }
    u16  GetMcuPort(void) const { return ntohs(m_wMcuConnPort);    } 

private:
    u32  m_dwRecIpAddr;                  //¼���IP��ַ
    s8   m_achAlias[MAXLEN_ALIAS];       //����
    u16  m_wRecStartPort;                //¼��ͨ����ʼ�˿ں�
    u8   m_byEqpId;                      //����ID
    u8   m_byRecChnNum;                  //¼��ͨ����

    u32  m_dwMcuIpAddr;                  //MCU IP��ַ
    u16  m_wMcuConnPort;                 //MCU ͨ�Ŷ˿ں�
    
    u32  m_dwReserved1;
    u32  m_dwReserved2;    
};

//��������Խṹ��len:274
struct TConfProperty
{
    TConfProperty(): m_dwDuration(0),
                     m_bStopMeetingManual(TRUE),
                     m_dwTotalBitRate(0),
                     m_dwReserved(0)
    {
        memset(m_achConfName, 0, sizeof(m_achConfName));
    }
public:
    void SetIndex(u32 dwIdx)
    {
        m_dwIndex = dwIdx;
    }
    u32 GetIndex() const
    {
        return m_dwIndex;
    }
    void SetDuration(u32 dwDur){ m_dwDuration = htonl(dwDur);    }
    u32  GetDuration(void) const { return ntohl(m_dwDuration);    }
    void SetStopManual(BOOL bManual) { m_bStopMeetingManual = bManual;    }
    BOOL IsStopManual(void) const { return m_bStopMeetingManual;    }
    void SetTotalBitRate(u32 dwBR) { m_dwTotalBitRate = htonl(dwBR);    }
    u32  GetTotalBitRate(void) const { return ntohl(m_dwTotalBitRate);    }
    void SetConfName(LPCSTR pszAlias)
    {
        if ( NULL != pszAlias )
        {
            u8 byStrLen = min(strlen(pszAlias), MAXLEN_ALIAS-1);
            strncpy(m_achConfName, pszAlias, byStrLen);
            m_achConfName[byStrLen] = '\0';
        }
    }
    LPCSTR GetConfName(void) const { return m_achConfName;    }

private:
    u32  m_dwIndex;
	u32	 m_dwDuration;
	BOOL m_bStopMeetingManual;
	u32	 m_dwTotalBitRate;
	s8	 m_achConfName[MAXLEN_ALIAS];
	u32  m_dwReserved;
};

//¼�����Խṹ.len��5
struct TRecProperty
{
    TRecProperty():m_byPublishMode(emPublishNone),
                   m_dwReserved(0){}
public:
    void SetPublishMode(u8 byMode) { m_byPublishMode = byMode;    }
    u8   GetPublishMode(void) const { return m_byPublishMode;    }
private:
	u8  m_byPublishMode;// ������¼�ļ��ķ���ģʽ���μ� emPublishMode
	u32	m_dwReserved;   // reserved for the future
};

//֡ͷ���Խṹ.len: 36
struct TFrameHead
{
    TFrameHead():m_dwBitRate(0),
                 m_dwFormat(0),
                 m_pbyDataBuf(NULL),
                 m_dwDataBufLen(0),
                 m_dwTimeStamp(0)
    {
        memset(&m_tSParam, 0, sizeof(m_tSParam));
    }
public:
    void SetBitRate(u32 dwBR) { m_dwBitRate = htonl(dwBR);    }
    u32  GetBitRate(void) const { return ntohl(m_dwBitRate);  }
    void SetFormat(u32 dwFormat){ m_dwFormat = htonl(dwFormat);    }
    u32  GetFormat(void) const { return ntohl(m_dwFormat);    }
    void SetBufHead(u8 *pbyBufHead) { m_pbyDataBuf = pbyBufHead;    }
    u8*  GetBufHead(void) const { return m_pbyDataBuf;    }
    void SetBufLen(u32 dwLen) { m_dwDataBufLen = htonl(dwLen);    }
    u32  GetBufLen(void) const { return ntohl(m_dwDataBufLen);    }
    void SetTStamp(u32 dwTStamp) { m_dwTimeStamp = htonl(dwTStamp);    }
    u32  GetTStamp(void) const { return ntohl(m_dwTimeStamp);    }

    void SetAudSamplesPerSec(u32 dwSamples){ m_tSParam.m_tAudioParam.m_dwSamplesPerSec = htonl(dwSamples); }
    u32  GetAudSamplesPerSec(void) const { return ntohl(m_tSParam.m_tAudioParam.m_dwSamplesPerSec);    }
    void SetAudBitsPerSample(u32 dwBits){ m_tSParam.m_tAudioParam.m_dwBitsPerSample = htonl(dwBits);    }
    u32  GetAudBitsPerSample(void) const { return ntohl(m_tSParam.m_tAudioParam.m_dwBitsPerSample); }
    
    void SetVidFramesPerSec(u32 dwFrames) { m_tSParam.m_tVideoParam.m_dwFramesPerSec = htonl(dwFrames);    }
    u32  GetVidFramesPerSec(void) const { return ntohl(m_tSParam.m_tVideoParam.m_dwFramesPerSec);    }
    void SetVidImageHeight(u32 dwHight) { m_tSParam.m_tVideoParam.m_dwImageHeight = htonl(dwHight);    }
    u32  GetVidImageHeight(void) const { return ntohl(m_tSParam.m_tVideoParam.m_dwImageHeight);    }
    void SetVidImageWidth(u32 dwWidth) { m_tSParam.m_tVideoParam.m_dwImageWidth = htonl(dwWidth);    }
    u32  GetVidImageWidth(void) const { return ntohl(m_tSParam.m_tVideoParam.m_dwImageWidth);    }
    void SetIsKeyFrame(BOOL bKeyFrame) { m_tSParam.m_tVideoParam.m_bKeyFrame = bKeyFrame;    }
    BOOL GetIsKeyFrame(void) const { return m_tSParam.m_tVideoParam.m_bKeyFrame;    }
    void SetBitCounts(u8 byBC) { m_tSParam.m_tVideoParam.m_byBitCounts = byBC;    }
    u8   GetBitCounts(void) const { return m_tSParam.m_tVideoParam.m_byBitCounts;    }

private:
    u32 m_dwBitRate;    //��֡���ݶ�Ӧ������������
	u32 m_dwFormat;     //��֡���ݶ�Ӧ��������ʽ���μ�emMediaType�궨��
    u8  *m_pbyDataBuf;  //���ݻ���
    u32 m_dwDataBufLen; //���ݻ�������С
    u32 m_dwTimeStamp;  //ʱ�����Time Stamp
    union TStreamParam
    {
        struct TAudioStreamParam
        {
            u32 m_dwSamplesPerSec;    // ÿ�������
            u32 m_dwBitsPerSample;    // ÿ��������λ��
        }m_tAudioParam;

        struct TVideoStreamParam
        {
            u32  m_dwFramesPerSec;    //֡�ʣ�the number of frames per second
            u32  m_dwImageWidth;      //ͼ���Specifies the width of the bitmap, in pixels
            u32  m_dwImageHeight;     //ͼ��ߣ�Specifies the height of the bitmap, in pixels
            BOOL m_bKeyFrame;         //�Ƿ�ؼ�֡
            u8	 m_byBitCounts;       //λ�Specifies the number of bits per pixel.  
            //u32 m_dwMaxKeyFrameSpace;   //�ؼ�֡��������ṩ
        }m_tVideoParam;
        
    }m_tSParam;         //��֡��Ӧ��������
    u32 dwReserved;
};

//��ͨ�������ṹ.len: 50
struct TStreamProperty
{
    TStreamProperty():m_byTotalStreamNum(0),
                      m_byStreamType(emStreamTypeNone),
                      m_byChannel(0),
                      m_dwReserved(0){}
public:
    void SetConfIndex(u32 dwIdx)
    {
        m_dwConfIndex = dwIdx;
    }
    u32 GetConfIndex() const
    {
        return m_dwConfIndex;
    }
    void SetTotalStreamNum(u8 byNum) { m_byTotalStreamNum = byNum;    }
    u8   GetTotalStreamNum(void) const { return m_byTotalStreamNum;   }
    void SetStreamType(u8 byType) { m_byStreamType = byType;    }
    u8   GetStreamType(void) const { return m_byStreamType;    }
    void SetChannel(u8 byChannel) { m_byChannel = byChannel;    }
    u8   GetChannel(void) const { return m_byChannel;    }
    void SetFrameHead(TFrameHead &tFrameHead) { m_tFrameHead = tFrameHead;    }
    TFrameHead * GetFrameHead(void) { return &m_tFrameHead;    }
private:
    u32 m_dwConfIndex;          // ���ڻ���������
    u8  m_byChannel;            // �������ڵ�ͨ������
    u8  m_byTotalStreamNum;     // �뱾��ͬʱ�����������
    u8  m_byStreamType;         // �����ͣ��μ� emStreamType    
    TFrameHead m_tFrameHead;    // ����֡ͷ���Խṹ
    u32 m_dwReserved;
};


#pragma pack()

#endif
// !_REC_GENINFO_H_
