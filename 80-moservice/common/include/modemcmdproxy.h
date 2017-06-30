#ifndef _MODEM_CMD_PROXY_H
#define _MODEM_CMD_PROXY_H

#define MCMD_OK                 0               //�ɹ�
#define MCMD_FAIL               -1              //ʧ��
#define MCMD_UNSURE             -2              //����ִ����

#if defined(_MSC_VER)       // for Microsoft c++
#define API extern "C"  __declspec(dllexport)
#else                       // for gcc
#define API extern "C"
#endif

#define MOD_CLKSRC_RCV					3								//���ͣ���ʱ��Դ
#define MOD_CLKSRC_INTER				0								//���ͣ���ʱ��Դ
#define DEM_CLKSRC_RCV					0								//���գ���ʱ��Դ
#define DEM_CLKSRC_INTER				1

enum TModemType
{
    MODEM_TYPE_70M_70M,              
    MODEM_TYPE_70M_LBAND,            
    MODEM_TYPE_LBAND_70M,            
    MODEM_TYPE_LBAND_LBAND,          
};

class ModemCmdProxy
{
public:
    ModemCmdProxy(u32 ip = 0, u16 port = 0);

    int SetDemBitRate(u32 rate, u8 trynum = 0);                //���ý�������
    int CheckDemBitRate(u32 &rate);
    int QueryDemBitRate(u8 trynum = 0);
    
    int SetDemClkSrc(u32 src, u8 trynum = 0);                  //���ý���ʱ��Դ
    int CheckDemClkSrc(u32 &src);
	int QueryDemClkSrc(u8 trynum = 0);
    
    int SetDemFreq(u32 freq, u8 trynum = 0);                   //���ý���Ƶ��
    int CheckDemFreq(u32 &freq);
	int QueryDemFreq(u8 trynum = 0);
    
    int SetDemBrFr(u32 rate, u32 freq, u8 trynum = 0);         //���ý������ʺ�Ƶ��
    int CheckDemBrFr(u32 &rate, u32 &freq);

    int SetModBitRate(u32 rate, u8 trynum = 0);                //���÷�������
    int CheckModBitRate(u32 &rate);
	int QueryModBitRate(u8 trynum = 0);
    
    int SetModClkSrc(u32 src, u8 trynum = 0);                  //���÷���ʱ��Դ
    int CheckModClkSrc(u32 &src);
	int QueryModClkSrc(u8 trynum = 0);
    
    int SetModFreq(u32 freq, u8 trynum = 0);                   //���÷���Ƶ��
    int CheckModFreq(u32 &freq);
	int QueryModFreq(u8 trynum = 0);
    
    int SetModOutput(BOOL32 enable, BOOL32 reset = FALSE, u8 trynum = 0); //���÷����Ƿ�ʼ
    int QueryModOutput(u8 trynum = 0);                         //��ѯ�����Ƿ�ʼ
    int CheckModOutput(BOOL32 &enable);
    
    int SetModBrFr(u32 rate, u32 freq, u8 trynum = 0);		   //���÷������ʺ�Ƶ��
    int CheckModBrFr(u32 &rate, u32 &freq);

    void SetModemIp(u32 ip, u16 port); 	                       //����IP��Port
    void SetModemType(u8 type);

    

private:
    ModemCmdProxy(ModemCmdProxy&) {};
    bool isvalid();                                            //������в����Ƿ�����
    u32 m_dwIp;
    u16 m_wPort;
    u8  m_byTxType;                                            // �������ͣ�1 - 70M; 2 - LBAND
    u8  m_byRxType;
    u32 m_dwTxDefault;                                         // Ĭ��Ƶ��
};


API int  MdcomInit		(void);								   //����ģ��
API void MdcomTerm		(void);								   //ֹͣģ��
API void MdcomReport	(void);								   //״̬����
API void MdcomMsgLog	(void);							       //��Ϣ����
API int  MdcomCommand	(const char* cmd, const char* ip, u16 port, u32 value); 
API int  MdcomCheck		(const char* cmd, const char* ip, u16 port);

s8 * strOfIp( u32 dwIP );
s8 * strOfCmd( u32 dwCmd );

#endif


