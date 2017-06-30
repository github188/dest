
#ifndef _UPU_WRAPPER_H_
#define _UPU_WRAPPER_H_

#include <vector>
#include "kdvtype.h"
#include "nuupudata.h"
#include "upu_struct.h"

#ifdef  UPUWRAPPER_EXPORTS
#define UPUWRAPPER_API __declspec( dllexport )
#else
#define UPUWRAPPER_API
#endif


#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

typedef void (*ConSuccessCallBack)(void);
typedef void (*ConDisconnectCallBack)(void);

class CImpl;
class RWLock;
class UPUWRAPPER_API CUpuWrapper
{
public:
    CUpuWrapper();
    virtual ~CUpuWrapper();

public:
    BOOL32 StartUpu(IN const s8* szIpAddr, IN s32 nPort);
       
	BOOL32 SetClientId(IN const s8* szCltId, IN u8 byCltIdLen);
    BOOL32 GetConnectState();
    BOOL32 IsNeedClearUpuCache();   //是否需要清理UPU缓存
    void SetClearUpuCacheState(BOOL32 bNeed);   //设置UPU缓存清理状态
	BOOL32 SetConnectCB(IN ConSuccessCallBack _ConSuccessCB, IN ConDisconnectCallBack _ConDisconnectCB);
public:
    //mt
    BOOL32 AddMt(IN const TMtInfoForUpu& tMtInfo);
    BOOL32 FindMtByE164(IN const s8* szE164NO, OUT TMtInfoForUpu& tMtInfo);
    BOOL32 DelMtByMOID(IN const TMtInfoForUpu& tMtInfo);
    BOOL32 DelMtBySrvDevID(IN const s8* szNUID);

    //conf
    BOOL32 FindConfByE164(IN const s8* szE164NO, OUT confinfo& tConfInfo);

    //nu
    BOOL32 AddNu(IN const TNUInfoForUpu&  tNuInfo);
    BOOL32 FindNuByNuID(IN const s8* szNUID, OUT TNUInfoForUpu& tNuInfo );
    BOOL32 FindNuByPlatformID(IN const s8* szPlatformID, OUT std::vector<TNUInfoForUpu>& vcNuInfo);
    BOOL32 DelNu(IN const s8* szNUID);

    //call info
    BOOL32 AddDomainCall(IN const TDomainCallInfo& tCallInfo);
    BOOL32 FindDomainCallByDomainID(IN const s8* szUserDomainID, OUT std::vector<TDomainCallInfo>& vcCallInfo);
    BOOL32 DelDomainCallByNuID(IN const s8* szNUID);

private:
    CImpl* m_pImpl;
    RWLock* m_pSyncLock;

};


#endif  //_UPU_WRAPPER_H_







