#ifndef _MCUEQPSIM_H_
#define _MCUEQPSIM_H_

#include "osp.h"
#include "mcustruct.h"
#include "kdvdef.h"

#define  SAFE_DEL_PTR(p) do{ if(p){ delete p; p = NULL; } }while(0)
#define NULL_CLASS_DEF(T) class T{}

class CMcuInst : public CInstance
{
	enum ESTALE{ IDLE, NORMAL };
protected:
    u32             m_dwMcuIpAddr;
	u32				m_dwEqpIpAddr; //������յ�ַ
	u16				m_wEqpStartPort; //������ʼ�˿�
	u8				m_byEqpId;		//�豸ID��
	u8  			m_byEqpChnNum;	//��������ŵ���
	u8				m_byEqpType;    //��������
	u8				m_byEqpChnId;
	u32				m_dwEqpNode;
	u32				m_dwEqpIId;
	
	TPeriEqpStatus  m_tEqpStatus[MAXNUM_MCU_PERIEQP];   //����״̬
	CServMsg m_cServMsg[10];
protected:
	void InstanceEntry( CMessage* const pMsg );
	BOOL Init();
	void MsgRegProc(CMessage* const pMsg);
	void SetDefaultData(CMessage* const pMsg);
	void MsgDisconnectProc();
	void Disconnect();//����������
	void ProcMixNotif( CMessage *const pMsg );
	void ProcBasNotif(CMessage* const pMsg );
	void ProcPrsNotif(CMessage* const pMsg );
	void ProcTvWallNotif(CMessage* const pMsg );
	void ProcVmpNotif(CMessage* const pMsg );
    void ProcHduNotif(CMessage* const pMsg);
		
public:
    CMcuInst();
	void ClearInst();
	BOOL ProcMcuToEqpMsg( CMessage* const pMsg );
	void ProcEqpMcuNotif( const CMessage * pcMsg );
	void AddToAckBuf(CServMsg cServMsg );
	void ClearAckBuf( );
};

class McuData
{
public:
	McuData()
	{
	}
	~McuData()
	{
	}
};

typedef zTemplate< CMcuInst, MAXNUM_MCU_PERIEQP, McuData, 0 > CMcuEqpSimApp;

extern CMcuEqpSimApp	g_cMcuEqpSimSsnApp;	//����ỰӦ��ʵ��

//extern "C" void McuEqpSimuInit(void);
extern "C" void McuEqpSimuInit(u32 dwLocalIpAddr = 0x7F000001);

#endif //!_MCUEQPSIM_H_