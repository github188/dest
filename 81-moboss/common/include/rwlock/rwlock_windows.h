

#ifndef _RW_LOCK_WINDOWS_H_
#define _RW_LOCK_WINDOWS_H_

#ifdef _WIN32

#include<windows.h>

/**
   * ��д������-����������-д���⣬д-д���⣬����CRITICAL_SECTIONʵ�֣��򵥸�Ч����֧�ֽ��̼�ʹ�ã�д�������ȣ�����ǰ���߳��ڶ�����д�����󣬲����������µĶ���ֱ��д������ɣ�����д��������
   * @author  CGG
   * @version  1.0
   */
class RWLock
{
public:
	/**
       * @Purpose: ���캯�����ٽ����ĳ�ʼ����
       */
	RWLock();
	/**
       * @Purpose: ����������ɾ���ٽ�������������ǰ�뱣֤ȫ�����⿪��
       */
	~RWLock();
	/**
       * @Purpose: �Ӷ���
       */
	void ReadLock();
	/**
       * @Purpose: ���Զ�
       * @Return: true ��ǰ�ܹ���(�Զ����϶���������Ҫ���н�������) , false ��ǰ���ܶ�
       */
	bool TryRead();
	/**
       * @Purpose: �����
       */
	void ReadUnLock();
	/**
       * @Purpose: ��д��
       */
	void WriteLock();
	/**
       * @Purpose: ����д
       * @Return: true ��ǰ�ܹ�д(�Զ�����д����д��Ҫ���н�������) , false ��ǰ����д
       */
	bool TryWrite();
	/**
       * @Purpose: ��д��
       */
	void WriteUnLock();
private:
	int m_nReaderCount; //���߼���
	CRITICAL_SECTION m_csWriteLock; //д��
	CRITICAL_SECTION m_csReadLock; //����
	CRITICAL_SECTION m_csVistorReaderCount; //���߼������ʻ�����
};


#endif /*_WIN32*/
#endif /*_RW_LOCK_WINDOWS_H_*/







