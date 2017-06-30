
#include "ipdb.h"
#include "bossutils.h"
#include "sqlstring.h"
#include "dbtabledef.h"
#include "debugprint.h"
#include "config.h"

CIpDB::CIpDB()
{

}

CIpDB::~CIpDB()
{

}

u16 CIpDB::GetIpISPName( IN u32 dwClientIp/*������*/, OUT std::string& strISPName )
{   
    strISPName = "";
    //������·�����ò����ݿ�
    if (BossUtils::IsLANIp(dwClientIp))
    {
        strISPName = g_cCfgFile.GetLoaclIspName();  //ֱ��д��
        return SUCCESS_DBENGINE;
    }

    //��Ϊ99.99%��IP�������Թ���,��ֱ�Ӳ�ѯ���ڵ�ip��ַ��; ����鲻�����ٲ��Ƿ������ĸ�����
    u16 wErrId = SUCCESS_DBENGINE;
    TSqlString tSqlString;

    tSqlString.Format("SELECT * FROM %s WHERE %s <= %u AND %s >= %u",
        TABLE_CITY_IP, CITYIP_START_IP, dwClientIp, CITYIP_END_IP, dwClientIp);

    wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }

    if ( m_cDBEngine.IsEof() )  //���ڹ���
    {
        return QueryForeignISPName(dwClientIp, strISPName); //������·
    }


    //�����ж�����¼,��ȡһ���Ϳ�����
    //ISP
    s8 szISP[MAXLEN_DB_VALUE+1] = {0};
    wErrId = m_cDBEngine.GetFieldValue(CITYIP_ISP, szISP, sizeof(szISP));
    if (SUCCESS_DBENGINE != wErrId)
    {
        DBOprLog(LOG_LVL_KEYSTATUS, "[CIpDB::GetIpISPName] GetFieldValue:%s fail\n", CITYIP_ISP);
        return wErrId;
    }
    strISPName = szISP;

    //ʡ��
    s8 szProvinceName[MAXLEN_DB_VALUE+1] = {0};
    wErrId = m_cDBEngine.GetFieldValue( CITYIP_PROVINCE_NAME , szProvinceName , sizeof(szProvinceName) );
    if (SUCCESS_DBENGINE != wErrId)
    {
        DBOprLog(LOG_LVL_KEYSTATUS, "[CIpDB::GetIpISPName] GetFieldValue:%s fail\n", CITYIP_PROVINCE_NAME);
        return wErrId;
    }

    s8 szCityName[MAXLEN_DB_VALUE+1] = {0};
    wErrId = m_cDBEngine.GetFieldValue( CITYIP_CITY_NAME , szCityName , sizeof(szCityName) );
    if (SUCCESS_DBENGINE != wErrId)
    {
        DBOprLog(LOG_LVL_KEYSTATUS, "[CIpDB::GetIpISPName] GetFieldValue:%s fail\n", CITYIP_CITY_NAME);
        return wErrId;
    }

    DBOprLog(LOG_LVL_KEYSTATUS, "[CIpDB::GetIpISPName] IP:%s ����:%s%s--%s!\n", BossUtils::GetIpString(dwClientIp).c_str(), szProvinceName, szCityName, szISP);
    return SUCCESS_DBENGINE;
}

u16 CIpDB::QueryForeignISPName(IN u32 dwClientIp/*������*/, OUT std::string& strISPName)
{
    u16 wErrId = SUCCESS_DBENGINE;
    TSqlString tSqlString;

    tSqlString.Format("SELECT DISTINCT B.* FROM %s A, %s B WHERE A.%s = B.%s AND A.%s <= %u AND %s >= %u",
        TABLE_COUNTRY_IP, TABLE_CODE_COUNTRY, COUNTRYIP_CODE2, CODECOUNTRY_CODE2, COUNTRYIP_START_IP, dwClientIp, COUNTRYIP_END_IP, dwClientIp);

    wErrId = m_cDBEngine.ExecSql(tSqlString.GetBuf(), TRUE);
    if (SUCCESS_DBENGINE != wErrId)
    {
        return wErrId;
    }

    if ( m_cDBEngine.IsEof() )
    {
        //���ǹ�����·
        DBOprLog( LOG_LVL_WARNING, "[CIpDB::QueryForeignISPName] IP:%s ���ǹ�����·\n", BossUtils::GetIpString(dwClientIp).c_str() );
        return ERR_DBENGINE_INTER_NO_RESULTSET;
    }
    strISPName = g_cCfgFile.GetForeignIspName();  //ֱ��д��

    //������������ֻ��Ϊ�˴�ӡ��־

    //���Ĺ�����
    s8 szCountry_cn[MAXLEN_DB_VALUE+1] = {0};
    wErrId = m_cDBEngine.GetFieldValue(CODECOUNTRY_CHN_NAME, szCountry_cn, sizeof(szCountry_cn));
    if (SUCCESS_DBENGINE != wErrId)
    {
        DBOprLog(LOG_LVL_KEYSTATUS, "[CIpDB::QueryForeignISPName] GetFieldValue:%s fail\n", CODECOUNTRY_CHN_NAME);
        return wErrId;
    }

    //Ӣ�Ĺ�����
    s8 szCountry_en[MAXLEN_DB_VALUE+1] = {0};
    wErrId = m_cDBEngine.GetFieldValue( CODECOUNTRY_ENG_NAME , szCountry_en , sizeof(szCountry_en) );
    if (SUCCESS_DBENGINE != wErrId)
    {
        DBOprLog(LOG_LVL_KEYSTATUS, "[CIpDB::QueryForeignISPName] GetFieldValue:%s fail\n", CODECOUNTRY_ENG_NAME);
        return wErrId;
    }

    DBOprLog(LOG_LVL_KEYSTATUS, "[CIpDB::QueryForeignISPName] IP:%s ����:%s(%s)!\n", BossUtils::GetIpString(dwClientIp).c_str(), szCountry_cn, szCountry_en);
    return SUCCESS_DBENGINE;
}



