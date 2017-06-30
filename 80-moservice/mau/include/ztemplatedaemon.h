//����DaemonInstance��zTemplate�ػ�
//������������ͨInstance��APP
//by �޿��� 2015/10/16

#pragma once

#include "osp.h"
#include "kdvtype.h"

//Ӧ��ģ��(ģ�����Ϊʵ���ļ̳����Լ����ʵ��ʵ����)
template <class I, class A, u8 maxAliasLen>
class zTemplate<I, 0, A, maxAliasLen> : public CApp, public A
{
public:
	//����ʵ������
	BOOL32 SetInstAlias(u16 insid, const char *pchAlias, u8 len);
	//���ʵ��������¼��Ϣ
	BOOL32  ClearInstAlias(u16 insid, const char *pAlias, u8 len);

private:
	//�ɾֲ�ʵ���Ų���ʵ��
	CInstance *GetInstance(u16 insid);
	//�ɱ�������ʵ��
	CInstance *FindInstByAlias(const char *pchAlias, u8 len);
	//�����ͨʵ������
	int GetInstanceNumber(void);
	//�������������
	u8 GetMaxAliasLen(void);

private:
	//daemonʵ��
	I daemIns;

	//��ʼ��ʵ��������¼��Ϣ
	BOOL32 InitAliasArray();
};

/* ������Ա������ģ�����Ա������ʵ�� */
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
	/* �ȿ��Ƿ���Daemonʵ�� */
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

/*���ʵ��������¼��Ϣ*/
template <class I, class A, u8 maxAliasLen>
BOOL32 zTemplate<I, 0, A, maxAliasLen>::ClearInstAlias(u16 insid, const char *pAlias, u8 len)
{
	return  FALSE;
}
