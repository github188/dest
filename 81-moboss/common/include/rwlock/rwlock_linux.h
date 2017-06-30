

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
       * @Purpose: 加读锁
       */
	void ReadLock();
	/**
       * @Purpose: 尝试读
       * @Return: true 当前能够读(自动加上读锁，读后要进行解锁操作) , false 当前不能读
       */
	bool TryRead();
	/**
       * @Purpose: 解读锁
       */
	void ReadUnLock();
	/**
       * @Purpose: 加写锁
       */
	void WriteLock();
	/**
       * @Purpose: 尝试写
       * @Return: true 当前能够写(自动加上写锁，写后要进行解锁操作) , false 当前不能写
       */
	bool TryWrite();
	/**
       * @Purpose: 解写锁
       */
	void WriteUnLock();
private:

	pthread_rwlock_t m_rwLock;
};


#endif /*_LINUX_*/
#endif	/*_RW_LOCK_LINUX_H_*/





