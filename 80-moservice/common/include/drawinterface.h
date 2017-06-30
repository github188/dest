/*****************************************************************************
   ģ����      : draw bmp
   �ļ���      : drawinterface.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ���׻�ͼ����ͷ�ļ�
   ����        : 
   �汾        : V0.9  Copyright(C) 2001-2006 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2006/03/29  4.0         john     ����
******************************************************************************/
#ifndef _DRAW_INTERFACE_H
#define _DRAW_INTERFACE_H

#ifndef  _EXCLUDE_VCCOMMON_H  // ����Vccommon.h
#define  _EXCLUDE_VCCOMMON_H
#endif

#include "kdvtype.h"
#include "mcuconst.h"
#include "osp.h"

#ifdef WIN32
    #pragma  pack (1)
#endif

#define IN     // ������������
#define OUT    // �����������

// ��ͼ����ֵ
#define  SUCCESS_DRAW   (u8)1    // success
#define  FAIL_DRAW      (u8)0    // fail

//bmp �滭ģʽ
#define  BMP_8   (u8)1
#define  BMP_24  (u8)2

// ������뷽ʽ
// ��ֱ����VerticalAlignment
#define VERTICAL_TOP    (u8)0x01  // ��ֱ������
#define VERTICAL_CENTER (u8)0x02  // ��ֱ�������
#define VERTICAL_BOTTOM (u8)0x04  // ��ֱ������
// ˮƽ����LevelAlignment
#define LEVEL_RIGHT     (u8)0x10  // ˮƽ�����Ҷ���
#define LEVEL_CENTER    (u8)0x20  // ˮƽ�������
#define LEVEL_LEFT      (u8)0x40  // ˮƽ���������

// ����ͼƬ��С������ʽ
#define AUTO_SIZE_MODE          (u8)0x01        // ��������Ӧ��С��ͼƬ���������ı����ɺ��ʴ�С��ͼƬ
#define USER_DEFINED_SIZE_MODE  (u8)0x02        // �����û��Զ����С��ͼƬ

// RGB
#undef RGB
#define RGB(r,g,b) ((u32)(((u8)(r)|((u16)((u8)(g))<<8))|(((u32)(u8)(b))<<16)))
// ��ɫ
const u32 COLOR_BLACK = RGB( 0, 0, 0 );
const u32 COLOR_WHITE = RGB( 255, 255, 255 );
const u32 COLOR_GRAY = RGB( 196, 206, 212 );
const u32 COLOR_MENUHEADER = RGB( 54,40,111 );
const u32 COLOR_MENUBODY = RGB( 124,114,124 )/*RGB( 120,120,120 )*/;
const u32 COLOR_MENUFOOT = RGB(90,90,90 );
const u32 COLOR_ITEMFONT = RGB( 42,31,85 );
const u32 COLOR_ITEMHL = RGB( 85,95,170 );
const u32 COLOR_YELLOW = RGB( 252, 226, 84 );
const u32 COLOR_BLUE = RGB( 44, 94, 180 );
const u32 COLOR_GREEN = RGB( 132, 218, 132 );
const u32 COLOR_CURSOR = RGB( 100, 202, 68 );
const u32 COLOR_RED = RGB( 255, 0, 0 );

// ͼƬ����
struct TPicParam
{
public:
    /*   Ĭ�ϲ�����BMP_8��������ɫͼ��  */  
    TPicParam() : m_byPicType( BMP_8 ),
                  m_dwPicHeight(0),
                  m_dwPicWidth(0),
                  m_dwPicSize(0)
    { 
    
    }
    void SetPicParamNull(void) { memset(this, 0, sizeof(TPicParam)); }

    u8  GetPicType( void ) { return m_byPicType; }
    u32 GetPicHeight( void ) { return m_dwPicHeight; }
    u32 GetPicWidth( void )  { return m_dwPicWidth;  }
    u32 GetPicSize( void )   { return m_dwPicSize;  }

    void SetPicType( u8 byPicType )     { m_byPicType = byPicType; }
    void SetPicHeight( u32 dwPicHeight ) { m_dwPicHeight = dwPicHeight; }
    void SetPicWidth( u32 dwPicWidth )   { m_dwPicWidth = dwPicWidth; }
    void SetPicSize( u32 dwPicSize )    { m_dwPicSize = dwPicSize; }
    
protected:
    u8  m_byPicType;        // ͼƬ����
    u32 m_dwPicHeight;      // ͼƬ�߶�
    u32 m_dwPicWidth;       // ͼƬ���
    u32 m_dwPicSize;        // ͼƬ��С
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// ͼƬ
struct TPic : public TPicParam
{
public:
    TPic() 
    { 
        memset( m_abyPic, 0, sizeof(m_abyPic) );
    }
    void SetPicNull(void) { memset(this, 0, sizeof(TPic)); }

    LPCSTR GetPic( void ) { return m_abyPic; }
    void SetPic( LPCSTR lpszPic )
    { 
        memcpy( m_abyPic, lpszPic, sizeof(m_abyPic) );
        m_abyPic[ sizeof( m_abyPic ) - 1 ] = '\0';
    }
protected:
    s8 m_abyPic[MAX_BMPFILE_SIZE+1];    // ͼƬBUF
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// �ı�����
struct TTextParam
{
public:
    /*   Ĭ�ϲ��������塢32�š���ֱ���С���ɫ����  */    
    TTextParam() : m_byFontType( FONT_KAI ),
                   m_byFontSize( FONT_SIZE_32 ),
                   m_wTextAlignment( VERTICAL_CENTER | LEVEL_CENTER ),
                   m_dwTextColor( COLOR_BLACK )
    { 
        memset( m_achText, 0, sizeof(m_achText) );
    }
    void SetTextParamNull(void) { memset(this, 0, sizeof(TTextParam)); }

    u8  GetFontType( void)       { return m_byFontType; }
    u8  GetFontSize( void )      { return m_byFontSize; }
    u8  GetTextAlignment( void ) { return m_wTextAlignment; }
    u32 GetTextColor( void )     { return m_dwTextColor; }
    LPCSTR GetText( void )       { return m_achText; }
    
    void SetFontType( u8 byFontType )            { m_byFontType = byFontType; }
    void SetFontSize( u8 byFontSize )            { m_byFontSize = byFontSize; }
    void SetTextAlignment( u8 byTextlAlignment ) { m_wTextAlignment = byTextlAlignment; }
    void SetTextColor( u32 dwTextColor )         { m_dwTextColor = dwTextColor; }
    void SetText( LPCSTR lpszText)
    {
        memcpy( m_achText, lpszText, sizeof(m_achText) );
        m_achText[ sizeof( m_achText ) - 1 ] = '\0';
	}

protected:
    u8  m_byFontType;       // ��������
    u8  m_byFontSize;       // �����С
    u8  m_wTextAlignment;   // �ı����뷽ʽ�����÷�ʽ: VerticalAlignment | LevelAlignment����ֱ���뷽ʽ λ�� ˮƽ���뷽ʽ��
    u32 m_dwTextColor;      // �ı���ɫ
	s8  m_achText[DEFAULT_TEXT_LEN+1]; // �ı�����
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

// ��ͼ����
/*
ͨ������m_byDrawMode�ж���ͼģʽ��

����AUTO_SIZE_MODEģʽ�������ñ���������
��TextParam ��m_byFontType,m_byFontSize,m_wTextAlignment,m_dwTextColor,m_achText[DEFAULT_TEXT_LEN+1]T
��TPicParam ��m_byPicType
��TDrawParam��m_dwDrawBkColor

����USER_DEFINED_SIZE_MODEģʽ�������ñ���������
��TextParam ��m_byFontType,m_byFontSize,m_wTextAlignment,m_dwTextColor,m_achText[DEFAULT_TEXT_LEN+1]
��TPicParam ��m_byPicType,m_wPicHeight�����𲿷֣�,m_wPicWidth�����𲿷֣�
��TDrawParam��m_dwDrawBkColor
*/
struct TDrawParam : public TPicParam, public TTextParam
{
public:
    /*   Ĭ�ϲ�����AUTO_SIZE_MODEģʽ����ɫ����  */ 
    TDrawParam() : m_byDrawMode( AUTO_SIZE_MODE ),
                   m_dwDrawBkColor( COLOR_WHITE )
    {

    }
    void SetDrawParamNull(void) { memset(this, 0, sizeof(TDrawParam)); }

    u8  GetDrawMode( void ) { return m_byDrawMode; }
    u32 GetDrawBkColor( void ) { return m_dwDrawBkColor; }    
    
    void SetDrawMode( u8 byDrawMode )     { m_byDrawMode = byDrawMode; }
    void SetDrawBkColor( u32 dwDrawBkColor ) { m_dwDrawBkColor = dwDrawBkColor; }

protected:
    u8  m_byDrawMode;           // ��ͼģʽ: AUTO_SIZE_MODE ���� USER_DEFINED_SIZE_MODE
    u32 m_dwDrawBkColor;        // ��ͼ����ɫ
}
#ifndef WIN32
__attribute__ ( (packed) ) 
#endif
;

class CKdvGDI;

class CDrawInterface
{	
public :
	CDrawInterface();
	virtual ~CDrawInterface();

    // ��ͼ
	BOOL32 DrawBmp( OUT TPic& tPic, IN TDrawParam& tDrawParam );

private:
	CKdvGDI *m_pDrawGdi;
};

#endif // _DRAW_INTERFACE_H

