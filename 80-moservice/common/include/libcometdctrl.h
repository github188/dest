#pragma once

#ifdef COMETDCTRLDLL_EXPORT
#ifdef WIN32
	#define COMETDCTRLDLL_EXPORT __declspec(dllexport)
#else
	#define COMETDCTRLDLL_EXPORT
#endif
#else
#ifdef WIN32
	#define COMETDCTRLDLL_EXPORT __declspec(dllimport)
#else
	#define COMETDCTRLDLL_EXPORT
#endif
#endif


#include <string.h>
#ifdef _LINUX_
#include <dlfcn.h>
#endif _LINUX_



enum EmCometdErr
{
	emNoerr =0,
	emInitJVMerr,
	emInitClientErr,
//	emCreateClientErr,
	emClientShakeErr,
};

//��Ϣ�ص���C++
typedef struct tagCBInfo
{
	char	m_strID[33];//���͵����кţ���cometd��java�ͻ��˿��Լ�������
	bool	m_bResult;//true �ɹ�  falseʧ��
	int		m_nReason;//��ʱδ��

	tagCBInfo()
	{
		memset(m_strID, 0, sizeof(m_strID));
		m_bResult = false;
		m_nReason = 0;
	}
}TCBInfo;

///////////////////////////////////////////////
//���ã���ʼ��InitCometdCtrlLib
//������[in] bWebSocket �Ƿ���websocket
//����  ���ش����
//����         ʱ��           �޸ļ�¼ 
//����       2014/05/04        ����
///////////////////////////////////////////////////
EmCometdErr COMETDCTRLDLL_EXPORT InitCometdCtrlLib(bool bWebSocket, char *URL);

///////////////////////////////////////////////
//���ã���Cometd����������
//������[in] URL ��ʽ��"http://172.16.80.49:8080/cometdPrj/cometd"
//����  ���ش����
//����         ʱ��           �޸ļ�¼ 
//����       2014/05/04        ����
///////////////////////////////////////////////////
EmCometdErr COMETDCTRLDLL_EXPORT HandShake();

///////////////////////////////////////////////
//���ã�����CometdClient
//������[in] ��
//����  ���ش����
//��ע�� �����˳�ʱ����
//����         ʱ��           �޸ļ�¼ 
//����       2014/05/04        ����
///////////////////////////////////////////////////
EmCometdErr COMETDCTRLDLL_EXPORT DestroyCometdCtrlLib();
///////////////////////////////////////////////
//���ã���������
//������[in] pValΪָ���ַ����׵�ַ������Json����ĵ�ַ
//      [in] IsJson ����pVal�Ƿ���Json
//����  true �ɹ�,  false ʧ��
//����         ʱ��           �޸ļ�¼ 
//����       2014/05/04        ����
///////////////////////////////////////////////////
bool COMETDCTRLDLL_EXPORT SendData(void *pVal, bool IsJson=true);
///////////////////////////////////////////////
//���ã��Ƿ�������
//����  
//����         ʱ��           �޸ļ�¼ 
//����       2014/05/04        ����
///////////////////////////////////////////////////
bool COMETDCTRLDLL_EXPORT IsConnected();

typedef void (*Myresult)(TCBInfo *);
void COMETDCTRLDLL_EXPORT SetCallback(Myresult pfun);