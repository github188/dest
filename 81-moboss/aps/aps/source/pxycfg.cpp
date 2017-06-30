
#include "pxycfg.h"
#include "bossutils.h"
#include "kdvsys.h"
#include "jsonconvert.h"


using namespace BossUtils;

#ifdef WIN32
#ifndef vsnprintf
#define vsnprintf   _vsnprintf
#endif
#ifndef snprintf
#define snprintf    _snprintf
#endif
#endif


CPxyCfg g_cPxyCfg;

CPxyCfg::CPxyCfg()
{
	/*lint -save -e1566*/
	Init();
	/*lint -restore*/
}

void CPxyCfg::Init()
{
	memset(m_szCfgFileName, 0, sizeof(m_szCfgFileName));
#ifdef _PPC_
	snprintf(m_szCfgFileName, sizeof(m_szCfgFileName), "%s/%s", PXY_CFG_PATH, PXY_CFGFILE_NAME);
	m_szCfgFileName[ sizeof(m_szCfgFileName) - 1 ] = '\0';
#else
#ifdef WIN32
	snprintf(m_szCfgFileName, sizeof(m_szCfgFileName), "%s%s\\%s", BossUtils::GetModuleFullPath().c_str(), PXY_CFG_PATH, PXY_CFGFILE_NAME);
	m_szCfgFileName[ sizeof(m_szCfgFileName) - 1 ] = '\0';
#else
	snprintf(m_szCfgFileName, sizeof(m_szCfgFileName), "%s%s/%s", BossUtils::GetModuleFullPath().c_str(), PXY_CFG_PATH, PXY_CFGFILE_NAME);
	m_szCfgFileName[ sizeof(m_szCfgFileName) - 1 ] = '\0';
#endif
#endif
	m_setPxyPlatformSrvAddr.clear();
/*
	FILE *fp = fopen(m_szCfgFileName, "r");
	if ( fp )
	{
		fseek(fp,0,SEEK_END);
		if ( ftell(fp) > 0 )
		{
			printf("file: %s already exist\n",m_szCfgFileName);
		}
		fclose(fp);
		return;
	}
    //test  生产一个配置文件
    std::set<CSrvAddrInfo> setSrvAddrInfo;
    CSrvAddrInfo tAddr1;
    tAddr1.SetIpAddr(56789);
    tAddr1.SetPort(1727);
    tAddr1.SetNetDomainName("www.kedacom.com");
    
    CSrvAddrInfo tAddr2;
    tAddr2.SetIpAddr(567893);
    tAddr2.SetPort(1729);
    tAddr2.SetNetDomainName("www.moooon.com");

    setSrvAddrInfo.insert(tAddr1);
    setSrvAddrInfo.insert(tAddr2);

    CPxyIspSrvAddr tIspSrv;
    tIspSrv.SetNmsAddrInfo(setSrvAddrInfo);
    tIspSrv.SetNdsAddrInfo(setSrvAddrInfo);
    tIspSrv.SetSusAddrInfo(setSrvAddrInfo);

    tIspSrv.SetStdH323AddrInfo(setSrvAddrInfo);
    tIspSrv.SetNonH323AddrInfo(setSrvAddrInfo);
    tIspSrv.SetSipAddrInfo(setSrvAddrInfo);

    tIspSrv.SetSnsAddrInfo(setSrvAddrInfo);
    tIspSrv.SetCmcAddrInfo(setSrvAddrInfo);
    tIspSrv.SetXnsAddrInfo(setSrvAddrInfo);
    tIspSrv.SetNtpAddrInfo(setSrvAddrInfo);
    tIspSrv.SetVrsAddrInfo(setSrvAddrInfo);
    tIspSrv.SetDcsAddrInfo(setSrvAddrInfo);

    CPxyIsp tIspItem1;
    tIspItem1.SetPxyIspSrvAddr(tIspSrv);
    tIspItem1.SetIspName("中国电信");

    CPxyIsp tIspItem2;
    tIspItem2.SetPxyIspSrvAddr(tIspSrv);
    tIspItem2.SetIspName("中国联通");

    std::set<CPxyIsp> m_setIspAddr;
    m_setIspAddr.insert(tIspItem1);
    m_setIspAddr.insert(tIspItem2);

    CPxyPlatformAddr item1;
    item1.SetPlatformDomainMOID("0001");
    item1.SetPxyPlatformSrvAddr(m_setIspAddr);

    m_setPxyPlatformSrvAddr.insert(item1);


    //
    item1.SetPlatformDomainMOID("0002");
    m_setPxyPlatformSrvAddr.insert(item1);

    Json::Value tArrayObj(Json::arrayValue);
    // 注释运用 set<T>内部类型iterator时须要添加typename
    std::set< CPxyPlatformAddr >::const_iterator iter = m_setPxyPlatformSrvAddr.begin();
    for ( ; iter != m_setPxyPlatformSrvAddr.end(); ++iter )
    {           
        Json::Value tNested = iter->StructToJson();
        tArrayObj.append(tNested);
    }

    Json::Value  m_jsonObject;
    m_jsonObject["PlatformAddrList"] = tArrayObj;

    std::string strJson = m_jsonObject.toStyledString();
    FILE* pFileFd = fopen(m_szCfgFileName, "w+");
    if (NULL != pFileFd)
    {
        fwrite(strJson.c_str(), sizeof(s8), strJson.length(), pFileFd);
        fclose(pFileFd);
    }
	*/
}

/*lint -save -e438*/
BOOL32 CPxyCfg::IsConfigFileExist() 
{    
	FILE * pStream = fopen( m_szCfgFileName , "rb" );
	if( NULL == pStream )
	{
		printf("[CPxyCfg][IsConfigFileExist] config file:%s not exist.\n", m_szCfgFileName);
		OspPrintf(TRUE, TRUE, "[CPxyCfg][IsConfigFileExist] config file:%s not exist.\n", m_szCfgFileName);
		return FALSE;
	}

    fclose(pStream);
    pStream = NULL;
	return TRUE;
}

BOOL32 CPxyCfg::ReadConfigInfo()
{
	BOOL32 bRet = FALSE;
	//原有数据清零
	Init();

	FILE * pStream = fopen( m_szCfgFileName, "rt" );
	if( NULL == pStream )
	{
		OspPrintf(TRUE, TRUE, "[CPxyCfg][ReadConfigInfo] the recipe file:%s not exist\n", m_szCfgFileName); 
		return FALSE;
	}

	//文件长度
	u32 dwDataLen = BossUtils::GetFileSize(m_szCfgFileName);
	if (0 == dwDataLen)
	{
		fclose(pStream);
		OspPrintf(TRUE, TRUE, "[CPxyCfg][ReadConfigInfo] get file size fail! file path:%s\n", m_szCfgFileName);
		return FALSE;
	}

	//读取内容
	s8* szDataBuf = new s8[dwDataLen+1];
	fread(szDataBuf, sizeof(s8), dwDataLen, pStream);
	szDataBuf[dwDataLen] = '\0';

    u16 wOffset = 0;
    u8 byBOM[] = {0xef, 0xbb, 0xbf}; //windows下的有些编辑器，utf8编码时会带上BOM头;此文件有可能被人在windows下手工编辑，兼容一下
    int bResult = memcmp(szDataBuf, byBOM, sizeof(byBOM));
    if (0 == bResult)
    {
        wOffset = sizeof(byBOM);    //带了BOM头
    }

	//json转换
	CJsonConvert toolJson;        
	bRet = toolJson.ParseJsonStr(szDataBuf+wOffset);
	if (!bRet)
	{
		//清理资源
		delete[] szDataBuf;
		szDataBuf = NULL;
		fclose(pStream);

		OspPrintf(TRUE, TRUE, "[CPxyCfg][ReadConfigInfo] JsonToStruct fail\n"); 
		return FALSE;
	}

	m_setPxyPlatformSrvAddr.clear();
	bRet = toolJson.GetNestedSetValue("PlatformAddrList", m_setPxyPlatformSrvAddr);
	if (!bRet)
	{
		//清理资源
		delete[] szDataBuf;
		szDataBuf = NULL;
		fclose(pStream);

		OspPrintf(TRUE, TRUE, "[CPxyCfg][ReadConfigInfo] tRecipe.JsonToStruct fail!tRecipe:%s \n", m_szCfgFileName);
		return FALSE;
	}

	//过滤掉LogicSrv名为空的项
	CPxyPlatformAddr temp;
	temp.SetPlatformDomainMOID("");
	m_setPxyPlatformSrvAddr.erase(temp);

	//清理资源
	delete[] szDataBuf;
	szDataBuf = NULL;
	fclose(pStream);

	return TRUE;
}




/*lint -restore*/

