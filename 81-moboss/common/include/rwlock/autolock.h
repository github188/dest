
#ifndef _AUTO_LOCK_H_
#define _AUTO_LOCK_H_


#include "rwlock_windows.h"
#include "rwlock_linux.h"

#include "recursivelock_windows.h"
#include "recursivelock_linux.h"



//Ð´Ëø
class CAutoWriteLock
{
public:
    explicit CAutoWriteLock( RWLock &tLock );
    ~CAutoWriteLock( );

	void UnLock( );

private:
	void Lock();
//	bool TryLock();	

private:
    RWLock* m_pWRLock; 
	bool m_bState;
};


//¶ÁËø
class CAutoReadLock
{
public:
    explicit CAutoReadLock( RWLock &tLock );
    ~CAutoReadLock( );
	
	void UnLock( );
private:
	void Lock()	;
//	bool TryLock();
	
private:
    RWLock* m_pWRLock; 
	bool m_bState;
};

//µÝ¹éËø
class CAutoRecursiveLock
{
public:
    explicit CAutoRecursiveLock( RecursiveLock &tLock );
    ~CAutoRecursiveLock( );
    
    void UnLock( );
    
private:
    void Lock();
    //	bool TryLock();	
    
private:
    RecursiveLock* m_pRecursiveLock; 
    bool m_bState;
};

#endif /*_AUTO_LOCK_H_*/




