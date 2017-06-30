

#ifndef _RECURSIVE_LOCK_LINUX_H_
#define _RECURSIVE_LOCK_LINUX_H_

#ifdef _LINUX_

#include <pthread.h>   

/*
   * 递归锁
   1.同一时刻只能由一线程进入临界区
   2.同一个线程可以重复进入临界区，如果多次进入，则必须多次退出
   3.进入临界区(锁定)，退出临界区(解锁)二者必须成对使用
*/
class RecursiveLock
{
public:
    RecursiveLock();
    ~RecursiveLock();
    
public:
    //加锁
    void Lock();
    
    //尝试加锁
    //@Return: true 当前能够加锁(自动加锁，之后要进行解锁操作) , false当前不能加锁
    bool TryLock();
    
    //解锁
    void UnLock();
    
private:
    pthread_mutex_t m_RecursiveLock;
    pthread_mutexattr_t m_attr;
};

#endif /*_LINUX_*/
#endif	/*_RECURSIVE_LOCK_LINUX_H_*/





