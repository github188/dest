

#ifndef _RW_LOCK_LINUX_H_
#define _RW_LOCK_LINUX_H_

#ifdef _LINUX_

#include <pthread.h>   

class RWLock
{
public:
	RWLock();
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

	pthread_rwlock_t m_rwLock;
};


#endif /*_LINUX_*/
#endif	/*_RW_LOCK_LINUX_H_*/





