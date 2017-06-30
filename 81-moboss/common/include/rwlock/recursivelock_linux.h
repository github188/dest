

#ifndef _RECURSIVE_LOCK_LINUX_H_
#define _RECURSIVE_LOCK_LINUX_H_

#ifdef _LINUX_

#include <pthread.h>   

/*
   * �ݹ���
   1.ͬһʱ��ֻ����һ�߳̽����ٽ���
   2.ͬһ���߳̿����ظ������ٽ����������ν��룬��������˳�
   3.�����ٽ���(����)���˳��ٽ���(����)���߱���ɶ�ʹ��
*/
class RecursiveLock
{
public:
    RecursiveLock();
    ~RecursiveLock();
    
public:
    //����
    void Lock();
    
    //���Լ���
    //@Return: true ��ǰ�ܹ�����(�Զ�������֮��Ҫ���н�������) , false��ǰ���ܼ���
    bool TryLock();
    
    //����
    void UnLock();
    
private:
    pthread_mutex_t m_RecursiveLock;
    pthread_mutexattr_t m_attr;
};

#endif /*_LINUX_*/
#endif	/*_RECURSIVE_LOCK_LINUX_H_*/





