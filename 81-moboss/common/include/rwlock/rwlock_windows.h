

#ifndef _RW_LOCK_WINDOWS_H_
#define _RW_LOCK_WINDOWS_H_

#ifdef _WIN32

#include<windows.h>

/**
   * 读写锁（读-读并发，读-写互斥，写-写互斥，基于CRITICAL_SECTION实现，简单高效，不支持进程间使用，写操作优先（即当前有线程在读，有写操作后，不允许再有新的读，直至写操作完成，避免写饿死））
   * @author  CGG
   * @version  1.0
   */
class RWLock
{
public:
	/**
       * @Purpose: 构造函数（临界区的初始化）
       */
	RWLock();
	/**
       * @Purpose: 析构函数（删除临界区，析构对象前请保证全部锁解开）
       */
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
	int m_nReaderCount; //读者计数
	CRITICAL_SECTION m_csWriteLock; //写锁
	CRITICAL_SECTION m_csReadLock; //读锁
	CRITICAL_SECTION m_csVistorReaderCount; //读者计数访问互斥锁
};


#endif /*_WIN32*/
#endif /*_RW_LOCK_WINDOWS_H_*/







