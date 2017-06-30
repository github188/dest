

#ifndef _RECURSIVE_LOCK_WINDOWS_H_
#define _RECURSIVE_LOCK_WINDOWS_H_

#ifdef _WIN32
#include<windows.h>

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
    //windows下的临界区、互斥器、信号量、事件等线程同步工具,天生就是递归锁
    //相比其他，临界区效率稍微好点
	CRITICAL_SECTION m_csRecursiveLock;  
};


#endif /*_WIN32*/
#endif /*_RECURSIVE_LOCK_WINDOWS_H_*/







