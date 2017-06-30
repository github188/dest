

#ifndef _RECURSIVE_LOCK_WINDOWS_H_
#define _RECURSIVE_LOCK_WINDOWS_H_

#ifdef _WIN32
#include<windows.h>

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
    //windows�µ��ٽ��������������ź������¼����߳�ͬ������,�������ǵݹ���
    //����������ٽ���Ч����΢�õ�
	CRITICAL_SECTION m_csRecursiveLock;  
};


#endif /*_WIN32*/
#endif /*_RECURSIVE_LOCK_WINDOWS_H_*/







