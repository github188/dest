//仅有DaemonInstance的zTemplate特化
//适用于无需普通Instance的APP
//by 邹俊龙 2015/10/16

#pragma once

#include "osp.h"
#include "kdvtype.h"

//应用模板(模板参数为实例的继承类以及最大实例实例数)
template <class I, class A, u8 maxAliasLen>
class zTemplate<I, 0, A, maxAliasLen> : public CApp, public A
{
public:
	//设置实例别名
	BOOL32 SetInstAlias(u16 insid, const char *pchAlias, u8 len);
	//清除实例别名纪录信息
	BOOL32  ClearInstAlias(u16 insid, const char *pAlias, u8 len);

private:
	//由局部实例号查找实例
	CInstance *GetInstance(u16 insid);
	//由别名查找实例
	CInstance *FindInstByAlias(const char *pchAlias, u8 len);
	//获得普通实例个数
	int GetInstanceNumber(void);
	//获得最大别名长度
	u8 GetMaxAliasLen(void);

private:
	//daemon实例
	I daemIns;

	//初始化实例别名记录信息
	BOOL32 InitAliasArray();
};

/* 内联成员函数或模板类成员函数的实现 */
template <class I, class A, u8 maxAliasLen>
BOOL32 zTemplate<I, 0, A, maxAliasLen>::InitAliasArray()
{
	return TRUE;
}

template <class I, class A, u8 maxAliasLen>
int zTemplate<I, 0, A, maxAliasLen>::GetInstanceNumber(void)
{
	return 0;
}

template <class I, class A, u8 maxAliasLen>
u8 zTemplate<I, 0, A, maxAliasLen>::GetMaxAliasLen(void)
{
	return maxAliasLen;
}

template <class I, class A, u8 maxAliasLen>
CInstance *zTemplate<I, 0, A, maxAliasLen>::GetInstance(u16 insid)
{
	if (insid == CInstance::DAEMON)
		return &daemIns;

	return NULL;
}

template <class I, class A, u8 maxAliasLen>
BOOL32 zTemplate<I, 0, A, maxAliasLen>::SetInstAlias(u16 insid, const char *pchAlias, u8 len)
{
	CInstance *pIns = GetInstance(insid);
	if (pIns != NULL && len <= maxAliasLen) {
		CInstance *pOtherIns = FindInstByAlias(pchAlias, len);
		if ((pOtherIns != NULL) && (pOtherIns->GetInsID() != pIns->GetInsID())) {
			return FALSE;
		}
		memcpy(pIns->m_alias, pchAlias, len);
		pIns->m_aliasLen = len;
		//return pIns->SetAlias(pchAlias, len);
	}
	return TRUE;
}

template <class I, class A, u8 maxAliasLen>
CInstance *zTemplate<I, 0, A, maxAliasLen>::FindInstByAlias(const char *pchAlias, u8 len)
{
	/* 先看是否是Daemon实例 */
	u8   aliasLen = 0;
	u32  dwCycTime = 0;

	if (maxAliasLen < len) {
		return NULL;
	}

	char aliasBuf[MAX_ALIAS_LEN];
	memset(aliasBuf, 0, MAX_ALIAS_LEN);

	CInstance *pIns = GetInstance(CInstance::DAEMON);
	if (pIns != NULL &&
		pIns->GetAlias(aliasBuf, maxAliasLen, &aliasLen)) {
		if ((aliasLen == len) &&
			(memcmp(aliasBuf, pchAlias, len) == 0)) {
			return pIns;
		}
	}

	return NULL;
}

/*清除实例别名纪录信息*/
template <class I, class A, u8 maxAliasLen>
BOOL32 zTemplate<I, 0, A, maxAliasLen>::ClearInstAlias(u16 insid, const char *pAlias, u8 len)
{
	return  FALSE;
}
