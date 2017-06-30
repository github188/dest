#include "inc.h"
#include "mautest.h"
#include "vccommon.h"
#include "jsontranslate.h"
#include "mauconst.h"
#include "evmau.h"
#include "eventnumau.h"
#include "mauvc.h"
#include "upussn.h"

extern CMauVCApp g_cMauVCApp;

void SetMtReq(s8 * pchJsonBuf, u32 &dwJsonBufLen, TMtAlias *ptMtAlias, u16 *pwBitrate, u16 wMtNum)
{
	if (NULL == pchJsonBuf || NULL == ptMtAlias || 0 == pwBitrate || 0 == wMtNum)
	{
		OspPrintf(TRUE, FALSE, "[SetMtReq] Param is NULL, retur!\n");
		return;
	}

	json_error_t jReason;
	json_t_ptr pjRoot = NULL;
	if (0 != dwJsonBufLen)
	{
		pjRoot = json_loads(pchJsonBuf, JSON_DISABLE_EOF_CHECK, &jReason);
	}
	else
	{
		pjRoot = json_object();
	}

	if (NULL == pjRoot)
	{
		OspPrintf(TRUE, FALSE, "[SetMtReq] pjRoot is NULL, return\n");
		return;
	}

	json_t_ptr pjMtInfoArray = json_array();
	if (NULL == pjMtInfoArray)
	{
		OspPrintf(TRUE, FALSE, "[SetMtReq] json_array pjMtInfoArray is NULL, return!\n");
		return;
	}

	s8 achTmpBuf[256] = { 0 };

	u16 wMtIdx = 0;
	for (wMtIdx = 0; wMtIdx < wMtNum; wMtIdx++)
	{
		json_t_ptr pjMtInfo = json_object();
		if (NULL == pjMtInfo)
		{
			continue;
		}

		json_t_ptr pjMtNO = json_string(ptMtAlias[wMtIdx].m_achAlias);
		if (NULL == pjMtNO)
		{
			continue;
		}
		json_object_set_s(pjMtInfo, JSON_MTNO, pjMtNO);

		memset(achTmpBuf, 0, sizeof(achTmpBuf));
		sprintf(achTmpBuf, "%d", pwBitrate[wMtIdx]);
		json_t_ptr pjBitrate = json_string(achTmpBuf);
		if (NULL == pjBitrate)
		{
			continue;
		}
		json_object_set_s(pjMtInfo, JSON_BITRATE, pjBitrate);

		memset(achTmpBuf, 0, sizeof(achTmpBuf));
		if (mtAliasTypePartNumber == ptMtAlias[wMtIdx].m_AliasType)
		{
			sprintf(achTmpBuf, "%d", MT_TYPE_PHONE);
		}
		else
		{
			sprintf(achTmpBuf, "%d", MT_TYPE_MT);
		}

		json_t_ptr pjMtType = json_string(achTmpBuf);
		if (NULL == pjMtType)
		{
			continue;
		}
		json_object_set_s(pjMtInfo, JSON_MTTYPE, pjMtType);

		json_array_append_s(pjMtInfoArray, pjMtInfo);
	}


	json_object_set_s(pjRoot, JSON_MT, pjMtInfoArray);

	json_str pchData(pjRoot);
	if (NULL == pchData)
	{
		OspPrintf(TRUE, FALSE, "[SetMtReq] json_dumps is NULL!\n");
		return;
	}

	if (MAXLEN_JSONBUF < strlen(pchData))
	{
		OspPrintf(TRUE, FALSE, "[SetMtReq] pchData len > MAXLEN_JSONBUF, return!\n");
		return;
	}

	dwJsonBufLen = strlen(pchData);

	memcpy(pchJsonBuf, pchData, dwJsonBufLen);
}

API void initcmu()
{
	CMauMsg cServMsg;
	s32 nSourceNum = 1;
	s8  achRoutingKey[MAXLEN_ALIAS + 1] = { 0 };    // MQ 队列名
	s8	achMcuIP[20 + 1] = { 0 };		    	    // McuIp
	s8  achMcuType[MAXLEN_ALIAS + 1] = { 0 };		// mcu类型
	s8  achMcuName[MAXLEN_ALIAS + 1] = { 0 };		// mcu名
	cServMsg.SetMsgBody((u8*)&nSourceNum, sizeof(s32));

	cServMsg.CatMsgBody((u8*)"cmu.conf.k:172.16.80.104", sizeof(achRoutingKey));
	cServMsg.CatMsgBody((u8*)"172.16.80.104", sizeof(achMcuIP));
	cServMsg.CatMsgBody((u8*)achMcuType, sizeof(achMcuType));
	cServMsg.CatMsgBody((u8*)achMcuName, sizeof(achMcuName));

	::OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, CInstance::DAEMON), RESMGR_CMUSSN_CMUINFO_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
}

API void dntf()
{
	CMauMsg cServMsg;
	::OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), MAU_MCU_MTLIST_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
}

API void addconf(u8 byIdx /*= 0*/)
{
	s8 achConfE16[7] = { 0 };

	sprintf(achConfE16, "%s%d", "051999", byIdx);

	//会议信息写入UPU
	upuclient_t * ptUpuClient = g_cUpuSsnApp.m_cUpuClient.GetClient();

	if (NULL != ptUpuClient)
	{
		confinfo tUpuConfInfo = {0};
		// 		conf_set_confe164( &tUpuConfInfo, achConfE16 );
		// 		conf_set_userdomain( &tUpuConfInfo, "20303000002200000000000000000112" );
		// 		conf_set_platformid( &tUpuConfInfo, "20303000002200000000000000000110" );
		// 		conf_set_devid( &tUpuConfInfo, "12345678901234567890123456789012" );


		OspPrintf(TRUE, FALSE, "[addconf]ConfE164<%s:%s> UserGUID<%s> PlatGUID<%s> DevGUID<%s>!\n",
			achConfE16, tUpuConfInfo.m_abyConfE164, tUpuConfInfo.m_abyUserDomain, tUpuConfInfo.m_abyPlatformID, tUpuConfInfo.m_abyDevID);

		int nRet = add_conf(ptUpuClient, &tUpuConfInfo);
		if (0 != nRet)
		{
			OspPrintf(TRUE, FALSE, "[delconf]upu client add_conf() failed! ret = %d\n", nRet);
		}
	}
	else
	{
		OspPrintf(TRUE, FALSE, "[delconf]Get upu client failed!\n");
	}
}

API void delconf(u8 byIdx /*= 0*/)
{
	s8 achConfE16[7] = { 0 };

	sprintf(achConfE16, "%s%d", "051999", byIdx);

	upuclient_t * ptUpuClient = g_cUpuSsnApp.m_cUpuClient.GetClient();
	if (NULL != ptUpuClient)
	{
		confinfo tUpuConfInfo = {0};
		conf_set_confe164(&tUpuConfInfo, achConfE16);

		OspPrintf(TRUE, FALSE, "[delconf]ConfE164<%s:%s> !\n", achConfE16, tUpuConfInfo.m_abyConfE164);

		int nRet = del_conf_by_e164(ptUpuClient, &tUpuConfInfo);
		if (0 != nRet)
		{
			OspPrintf(TRUE, FALSE, "[delconf]upu client del_conf_by_e164() failed! ret = %d\n", nRet);
		}
	}
	else
	{
		OspPrintf(TRUE, FALSE, "[delconf] Get upu client for del conf failed!\n");
	}
}

API void deldev()
{
	//会议信息写入UPU
	upuclient_t * ptUpuClient = g_cUpuSsnApp.m_cUpuClient.GetClient();
	if (NULL != ptUpuClient)
	{
		confinfo tUpuConfInfo = {0};
		//tUpuConfInfo.SetDevGUID( "12345678901234567890123456789012" );
		conf_set_devid(&tUpuConfInfo, "12345678901234567890123456789012");

		int nRet = del_conf_by_devid(ptUpuClient, &tUpuConfInfo);
		if (0 != nRet)
		{
			OspPrintf(TRUE, FALSE, "[deldev]upu client add_conf() failed! ret = %d\n", nRet);
		}
	}
	else
	{
		OspPrintf(TRUE, FALSE, "[deldev]Get upu client failed!\n");
	}
}

API void findconf(u8 byIdx /*= 0*/)
{
	s8 achConfE16[13] = { 0 };

	sprintf(achConfE16, "%s%d", "051999", byIdx);

	upuclient_t * ptUpuClient = g_cUpuSsnApp.m_cUpuClient.GetClient();

	if (NULL != ptUpuClient)
	{
		//TConfInfoForUpu tUpuConfInfo;
		//tUpuConfInfo.SetConfE164( achConfE16 );
		confinfo tUpuConfInfo = {0};
		conf_set_confe164(&tUpuConfInfo, achConfE16);

		OspPrintf(TRUE, FALSE, "[findconf]ConfE164<%s:%s> !\n", achConfE16, tUpuConfInfo.m_abyConfE164);

		int nRet = find_conf_by_e164(ptUpuClient, &tUpuConfInfo);
		if (0 != nRet)
		{
			OspPrintf(TRUE, FALSE, "[findconf]upu client del_conf_by_e164() failed! ret = %d\n", nRet);
		}
	}
	else
	{
		OspPrintf(TRUE, FALSE, "[findconf]Get upu client for del conf failed!\n");
	}
}



API void addmt()
{
	TMtAlias tMtAliasArray[MAXNUM_CONF_MT];
	s8 achJsonBuf[MAXLEN_JSONBUF] = { 0 };
	u16 awMtDialRate[MAXNUM_CONF_MT];

	u8 byMtNum = 0;
	u32 dwJsonBufLen = 0;
	memset(achJsonBuf, 0, sizeof(achJsonBuf));
	memset(awMtDialRate, 0, sizeof(awMtDialRate));
	memset(tMtAliasArray, 0, sizeof(tMtAliasArray));

	// 构造MtAlias
	tMtAliasArray[0].SetAlias(mtAliasTypeE164, (u8)13, "0512800120012");
	tMtAliasArray[1].SetAlias(mtAliasTypeE164, (u8)13, "0512800120000");
	byMtNum = 2;

	awMtDialRate[0] = 512;
	awMtDialRate[1] = 1024;
	// 	awMtDialRate[2] = 2048;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_ADDMT_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("book4");
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetConfE164ToJson(achJsonBuf, dwJsonBufLen, "05121199996");
	SetMtReq(achJsonBuf, dwJsonBufLen, tMtAliasArray, awMtDialRate, byMtNum);

	OspPrintf(TRUE, FALSE, "[rnn-test][addmt]%s.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, (u16)1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);
}

API void callmt()
{
	TMtAlias tMtAliasArray[MAXNUM_CONF_MT];
	s8 achJsonBuf[MAXLEN_JSONBUF] = { 0 };
	u16 awMtDialRate[MAXNUM_CONF_MT];

	u8 byMtNum = 0;
	u32 dwJsonBufLen = 0;
	memset(achJsonBuf, 0, sizeof(achJsonBuf));
	memset(awMtDialRate, 0, sizeof(awMtDialRate));
	memset(tMtAliasArray, 0, sizeof(tMtAliasArray));

	// 构造MtAlias
	tMtAliasArray[0].SetAlias(mtAliasTypeE164, (u8)13, "0512800120018");
	tMtAliasArray[1].SetAlias(mtAliasTypeE164, (u8)13, "0512800120017");
	tMtAliasArray[2].SetAlias(mtAliasTypeE164, (u8)13, "0512800000002");
	byMtNum = 3;

	awMtDialRate[0] = 512;
	awMtDialRate[1] = 1024;
	awMtDialRate[2] = 2048;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_CALLMT_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("book4");
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetConfE164ToJson(achJsonBuf, dwJsonBufLen, "05121199996");
	SetMtReq(achJsonBuf, dwJsonBufLen, tMtAliasArray, awMtDialRate, byMtNum);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, (u16)1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);
}

API void delmt()
{
	TMtAlias tMtAliasArray[MAXNUM_CONF_MT];
	s8 achJsonBuf[MAXLEN_JSONBUF] = { 0 };
	u16 awMtDialRate[MAXNUM_CONF_MT];

	u8 byMtNum = 0;
	u32 dwJsonBufLen = 0;
	memset(achJsonBuf, 0, sizeof(achJsonBuf));
	memset(awMtDialRate, 0, sizeof(awMtDialRate));
	memset(tMtAliasArray, 0, sizeof(tMtAliasArray));

	// 构造MtAlias
	tMtAliasArray[0].SetAlias(mtAliasTypeE164, (u8)13, "0512800120018");
	tMtAliasArray[1].SetAlias(mtAliasTypeE164, (u8)13, "0512800120017");
	tMtAliasArray[2].SetAlias(mtAliasTypeE164, (u8)13, "0512800000002");
	byMtNum = 3;

	awMtDialRate[0] = 512;
	awMtDialRate[1] = 1024;
	awMtDialRate[2] = 2048;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_DELMT_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("book4");
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetConfE164ToJson(achJsonBuf, dwJsonBufLen, "05121199996");
	SetMtReq(achJsonBuf, dwJsonBufLen, tMtAliasArray, awMtDialRate, byMtNum);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, (u16)1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);
}

API void dropmt()
{
	TMtAlias tMtAliasArray[MAXNUM_CONF_MT];
	s8 achJsonBuf[MAXLEN_JSONBUF] = { 0 };
	u16 awMtDialRate[MAXNUM_CONF_MT];

	u8 byMtNum = 0;
	u32 dwJsonBufLen = 0;
	memset(achJsonBuf, 0, sizeof(achJsonBuf));
	memset(awMtDialRate, 0, sizeof(awMtDialRate));
	memset(tMtAliasArray, 0, sizeof(tMtAliasArray));

	// 构造MtAlias
	tMtAliasArray[0].SetAlias(mtAliasTypeE164, (u8)13, "0512800120018");
	tMtAliasArray[1].SetAlias(mtAliasTypeE164, (u8)13, "0512800120017");
	tMtAliasArray[2].SetAlias(mtAliasTypeE164, (u8)13, "0512800000002");
	byMtNum = 3;

	awMtDialRate[0] = 512;
	awMtDialRate[1] = 1024;
	awMtDialRate[2] = 2048;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_DROPMT_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("book4");
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetConfE164ToJson(achJsonBuf, dwJsonBufLen, "05121199996");
	SetMtReq(achJsonBuf, dwJsonBufLen, tMtAliasArray, awMtDialRate, byMtNum);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, (u16)1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);
}

API void joinconf()
{
	//0512119999999
	//99999051211
	TReqHeadInfo tReqInfo;

	tReqInfo.SetMtType(MT_TYPE_MT);
	tReqInfo.SetSrcMtE164("0512119999999");
	tReqInfo.SetConfE164("99999051211");
	tReqInfo.SetMtReqID(56);


	OspPost(MAKEIID(AID_MAU_NUSSN_APPID, (u16)1), NU_MAU_JOINCONF_REQ, (u8*)&tReqInfo, sizeof(tReqInfo));
}

API void joinconf2()
{
	//0512119999999
	//99999051211
	TReqHeadInfo tReqInfo;

	tReqInfo.SetMtType(MT_TYPE_MT);
	tReqInfo.SetSrcMtE164("0512119999999");
	tReqInfo.SetConfE164("05121199999");
	tReqInfo.SetMtReqID(56);


	OspPost(MAKEIID(AID_MAU_NUSSN_APPID, (u16)1), NU_MAU_JOINCONF_REQ, (u8*)&tReqInfo, sizeof(tReqInfo));
}

API void getconf()
{
	TReqHeadInfo tReqInfo;

	tReqInfo.SetMtType(MT_TYPE_MT);
	tReqInfo.SetSrcMtE164("0512800120018");
	tReqInfo.SetConfE164("05121199996");


	OspPost(MAKEIID(AID_MAU_NUSSN_APPID, (u16)1), NU_MAU_GETCONFDETAIL_REQ, (u8*)&tReqInfo, sizeof(tReqInfo));
}

API void createconf()
{
	u32 dwJsonBufLen = 0;
	u8 byMtNum = 0;
	CMauMsg cServMsg;
	TReqHeadInfo tReqInfo;
	TMediaEncrypt tMediaEncrypt;
	TConfAttrb tConfAttrb;
	TConfInfo tConfInfo;
	TConfInfoEx tConfInfoEx;
	TConfStatus tConfStatus;
	TVideoStreamCap tMainStreamCap;
	TVideoStreamCap tDoubleStreamCap;
	TAudioTypeDesc tAudioDesc;
	s8 achJsonBuf[MAXLEN_JSONBUF] = { 0 };
	u16 awMtDialRate[MAXNUM_CONF_MT] = { 0 };
	TMtAlias tMtAliasArray[MAXNUM_CONF_MT];
	s8 achMtNO[MAXLEN_E164] = { 0 };

	memset(achJsonBuf, 0, sizeof(achJsonBuf));
	memset(awMtDialRate, 0, sizeof(awMtDialRate));
	memset(tMtAliasArray, 0, sizeof(tMtAliasArray));

	cServMsg.SetEventId(MAU_MCU_CREATECONF_REQ);

	// 构造MtAlias
	tMtAliasArray[0].SetAlias(mtAliasTypeE164, (u8)13, "0512119999999");
	tMtAliasArray[1].SetAlias(mtAliasTypeE164, (u8)13, "0512119999998");
	tMtAliasArray[2].SetAlias(mtAliasTypeE164, (u8)13, "0512119999997");
	tMtAliasArray[3].SetAlias(mtAliasTypeE164, (u8)13, "0512119999996");
	byMtNum = 4;

	// 构造TConfInfo
	tMediaEncrypt.SetEncryptMode((u8)CONF_ENCRYPTMODE_AES);
	tConfStatus.SetCallMode((u8)CONF_CALLMODE_TIMER);
	tConfStatus.SetCallInterval((u8)10);
	tConfStatus.SetCallTimes((u32)5);
	tConfAttrb.SetPortConfFlag(FALSE);
	tConfAttrb.SetDualMode((u8)CONF_DUALMODE_EACHMTATWILL);
	tConfAttrb.SetSpeakerSrc((u8)CONF_SPEAKERSRC_CHAIR);
	tConfAttrb.SetPublicConf((u8)1);
	tConfAttrb.SetOpenMode((u8)CONF_OPENMODE_OPEN);
	tConfAttrb.SetQualityPri(TRUE);

	tConfInfo.SetConfE164("05121199999");
	tConfInfo.SetConfName("TestConf");
	tConfInfo.SetDomainName("TestDomainName");
	tConfInfo.SetDomainMoid("01234567890123456789012345678901");
	tConfInfo.SetConfPwd("d41d8cd98f00b204e9800998ecf8427e");
	tConfInfo.SetDuration(240);
	tConfInfo.SetMediaKey(tMediaEncrypt);
	tConfInfo.SetChairAlias(tMtAliasArray[0]);
	tConfInfo.SetSpeakerAlias(tMtAliasArray[1]);
	tConfInfo.SetStatus(tConfStatus);
	tConfInfo.SetConfAttrb(tConfAttrb);

	// 构造TConfInfoEx
	tMainStreamCap.SetMaxBitRate((u16)2048);
	tMainStreamCap.SetMediaType((u8)MEDIA_TYPE_H264);
	tMainStreamCap.SetResolution((u8)VIDEO_FORMAT_HD720);
	tMainStreamCap.SetFrameRate((u8)30);
	tMainStreamCap.SetH264ProfileAttrb(emHpAttrb);

	tDoubleStreamCap.SetMediaType((u8)VIDEO_DSTREAM_H264);
	tDoubleStreamCap.SetResolution((u8)VIDEO_FORMAT_HD720);
	tDoubleStreamCap.SetFrameRate((u8)30);

	u8 byCapNum = 1;
	tConfInfoEx.SetMainStreamCapEx(&tMainStreamCap, byCapNum);
	tConfInfoEx.SetDoubleStreamCapEx(&tDoubleStreamCap, byCapNum);
	tConfInfoEx.SetAudioTypeDesc(&tAudioDesc, byCapNum);



	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_MAU_CMU_CREATECONF_REQ);
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqInfo);
	SetConfInfoToJson(achJsonBuf, dwJsonBufLen, tConfInfo);
	SetConfInfoExToJson(achJsonBuf, dwJsonBufLen, tConfInfoEx);
	SetMtAliasListToJson(achJsonBuf, dwJsonBufLen, tMtAliasArray, awMtDialRate, byMtNum);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, (u16)1), MAU_MCU_CREATECONF_REQ, achJsonBuf, dwJsonBufLen);

	return;
}

API void releaseconf(u16 wMeetingId)
{
	CMauMsg cServMsg;
	TReqHeadInfo tReqHeadinfo;
	s8 achJsonBuf[MAXLEN_JSONBUF] = { 0 };
	u32 dwJsonBufLen = 0;

	tReqHeadinfo.SetConfE164("05121199996");

	CMauData::SetDomainMoIdToJson(achJsonBuf, "ttz3z0iz9s455shj4x0jkz86", dwJsonBufLen);
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadinfo);
	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_MAU_MCU_RELEASECONF_CMD);
	SetMeetingIDToJson(achJsonBuf, dwJsonBufLen, wMeetingId);
	SetConfE164ToJson(achJsonBuf, dwJsonBufLen, "05121199996");

	cServMsg.SetMsgBody((u8*)&tReqHeadinfo, sizeof(tReqHeadinfo));
	cServMsg.CatMsgBody((u8*)achJsonBuf, dwJsonBufLen);

	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), CM_MAU_RELEASECONF_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	return;
}

API void mcuntf()
{
	CMauMsg cServMsg;
	s8  achRoutingKey[MAXLEN_ALIAS + 1];  // MQ 队列名

	s32 nSourceNum = 3;
	cServMsg.SetMsgBody((u8*)&nSourceNum, sizeof(s32));

	s32 nSourceIdx = 0;
	for (nSourceIdx = 0; nSourceIdx < nSourceNum; nSourceIdx++)
	{
		memset(achRoutingKey, 0, sizeof(achRoutingKey));
		sprintf(achRoutingKey, "mcu-%d", nSourceIdx);

		cServMsg.CatMsgBody((u8*)achRoutingKey, sizeof(achRoutingKey));
	}

	::OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, CInstance::DAEMON), RESMGR_CMUSSN_CMUINFO_NTF, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	return;
}

API void createconfbycm()
{
	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = MAXLEN_JSONBUF;
	u32 dwLen = 0;

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetReqEventID(CM_MAU_CREATECONF_REQ);
	tReqHeadInfo.SetConfName("rnntest");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetReqSrcID(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON));

	SetMsgTypeToJson(achJsonBuf, dwLen, EV_CM_MAU_CREATECONF_REQ);
	CMauData::SetDomainMoIdToJson(achJsonBuf, tReqHeadInfo.GetDomainMOID(), dwJsonBufLen);
	SetConfNameToJson(achJsonBuf, tReqHeadInfo.GetConfName(), dwJsonBufLen);

	u8 byMtNum = 0;
	TMediaEncrypt tMediaEncrypt;
	TConfAttrb tConfAttrb;
	TConfInfo tConfInfo;
	TConfInfoEx tConfInfoEx;
	TConfStatus tConfStatus;
	TVideoStreamCap tMainStreamCap;
	TVideoStreamCap tDoubleStreamCap;
	TAudioTypeDesc tAudioDesc;
	u16 awMtDialRate[MAXNUM_CONF_MT] = { 0 };
	TMtAlias tMtAliasArray[MAXNUM_CONF_MT];
	s8 achMtNO[MAXLEN_E164] = { 0 };

	//	memset( achJsonBuf, 0, sizeof( achJsonBuf ) );
	memset(awMtDialRate, 0, sizeof(awMtDialRate));
	memset(tMtAliasArray, 0, sizeof(tMtAliasArray));

	// 构造MtAlias
	//tMtAliasArray[0].SetAlias( mtAliasTypeE164, (u8)13, "0512800120012" );
	//tMtAliasArray[1].SetAlias( mtAliasTypeE164, (u8)13, "0512800120000" );
	byMtNum = 0;

	// 构造TConfInfo
	tMediaEncrypt.SetEncryptMode((u8)CONF_ENCRYPTMODE_AES);
	tConfStatus.SetCallMode((u8)CONF_CALLMODE_TIMER);
	tConfStatus.SetCallInterval((u8)10);
	tConfStatus.SetCallTimes((u32)5);
	tConfAttrb.SetPortConfFlag(FALSE);
	tConfAttrb.SetDualMode((u8)CONF_DUALMODE_EACHMTATWILL);
	tConfAttrb.SetSpeakerSrc((u8)CONF_SPEAKERSRC_CHAIR);
	tConfAttrb.SetPublicConf((u8)1);
	tConfAttrb.SetOpenMode((u8)CONF_OPENMODE_OPEN);
	tConfAttrb.SetQualityPri(TRUE);

	tConfInfo.SetConfE164("05121199996");
	tConfInfo.SetConfName("book4");
	tConfInfo.SetDomainName(cDomain.GetWholeName());
	tConfInfo.SetDomainMoid(cDomain.GetDomainMOID());
	//	tConfInfo.SetConfPwd( "d41d8cd98f00b204e9800998ecf8427e" );
	tConfInfo.SetDuration(140);
	tConfInfo.SetMediaKey(tMediaEncrypt);
	tConfInfo.SetChairAlias(tMtAliasArray[0]);
	tConfInfo.SetSpeakerAlias(tMtAliasArray[1]);
	tConfInfo.SetStatus(tConfStatus);
	tConfInfo.SetConfAttrb(tConfAttrb);

	// 构造TConfInfoEx
	tMainStreamCap.SetMaxBitRate((u16)2048);
	tMainStreamCap.SetMediaType((u8)MEDIA_TYPE_H264);
	tMainStreamCap.SetResolution((u8)VIDEO_FORMAT_HD720);
	tMainStreamCap.SetFrameRate((u8)30);
	tMainStreamCap.SetH264ProfileAttrb(emHpAttrb);

	tDoubleStreamCap.SetMediaType((u8)VIDEO_DSTREAM_H264);
	tDoubleStreamCap.SetResolution((u8)VIDEO_FORMAT_HD720);
	tDoubleStreamCap.SetFrameRate((u8)30);

	u8 byCapNum = 1;
	tConfInfoEx.SetMainStreamCapEx(&tMainStreamCap, byCapNum);
	tConfInfoEx.SetDoubleStreamCapEx(&tDoubleStreamCap, byCapNum);
	tConfInfoEx.SetAudioTypeDesc(&tAudioDesc, byCapNum);

	SetMeetingIDToJson(achJsonBuf, dwJsonBufLen, 149);
	SetConfInfoToJson(achJsonBuf, dwJsonBufLen, tConfInfo);
	SetConfInfoExToJson(achJsonBuf, dwJsonBufLen, tConfInfoEx);
	SetMtAliasListToJson(achJsonBuf, dwJsonBufLen, tMtAliasArray, awMtDialRate, byMtNum);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	CMauMsg cServMsg;
	cServMsg.SetEventId(CM_MAU_CREATECONF_REQ);
	cServMsg.SetMsgBody((u8*)&tReqHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody((u8*)achJsonBuf, strlen(achJsonBuf));

	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), CM_MAU_CREATECONF_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	return;
}

API void createconfbycmtemp(void)
{
	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = MAXLEN_JSONBUF;
	u32 dwLen = 0;

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetReqEventID(CM_MAU_CREATECONF_REQ);
	tReqHeadInfo.SetConfName("");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetReqSrcID(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON));

	SetMsgTypeToJson(achJsonBuf, dwLen, EV_CM_MAU_CREATECONF_REQ);
	CMauData::SetDomainMoIdToJson(achJsonBuf, tReqHeadInfo.GetDomainMOID(), dwJsonBufLen);
	SetConfNameToJson(achJsonBuf, tReqHeadInfo.GetConfName(), dwJsonBufLen);

	u8 byMtNum = 0;
	TMediaEncrypt tMediaEncrypt;
	TConfAttrb tConfAttrb;
	TConfInfo tConfInfo;
	TConfInfoEx tConfInfoEx;
	TConfStatus tConfStatus;
	TVideoStreamCap tMainStreamCap;
	TVideoStreamCap tDoubleStreamCap;
	TAudioTypeDesc tAudioDesc;
	u16 awMtDialRate[MAXNUM_CONF_MT] = { 0 };
	TMtAlias tMtAliasArray[MAXNUM_CONF_MT];
	s8 achMtNO[MAXLEN_E164] = { 0 };

	//	memset( achJsonBuf, 0, sizeof( achJsonBuf ) );
	memset(awMtDialRate, 0, sizeof(awMtDialRate));
	memset(tMtAliasArray, 0, sizeof(tMtAliasArray));

	// 构造MtAlias
	tMtAliasArray[0].SetAlias(mtAliasTypeE164, (u8)13, "0512119999999");
	tMtAliasArray[1].SetAlias(mtAliasTypeE164, (u8)13, "0512119999998");
	tMtAliasArray[2].SetAlias(mtAliasTypeE164, (u8)13, "0512119999997");
	tMtAliasArray[3].SetAlias(mtAliasTypeE164, (u8)13, "0512119999996");
	byMtNum = 4;

	// 构造TConfInfo
	tMediaEncrypt.SetEncryptMode((u8)CONF_ENCRYPTMODE_AES);
	tConfStatus.SetCallMode((u8)CONF_CALLMODE_TIMER);
	tConfStatus.SetCallInterval((u8)10);
	tConfStatus.SetCallTimes((u32)5);
	tConfAttrb.SetPortConfFlag(FALSE);
	tConfAttrb.SetDualMode((u8)CONF_DUALMODE_EACHMTATWILL);
	tConfAttrb.SetSpeakerSrc((u8)CONF_SPEAKERSRC_CHAIR);
	tConfAttrb.SetPublicConf((u8)1);
	tConfAttrb.SetOpenMode((u8)CONF_OPENMODE_OPEN);
	tConfAttrb.SetQualityPri(TRUE);

	tConfInfo.SetConfE164("");
	tConfInfo.SetConfName("cmtemp2");
	tConfInfo.SetDomainName(cDomain.GetWholeName());
	tConfInfo.SetDomainMoid(cDomain.GetDomainMOID());
	tConfInfo.SetConfPwd("d41d8cd98f00b204e9800998ecf8427e");
	tConfInfo.SetDuration(140);
	tConfInfo.SetMediaKey(tMediaEncrypt);
	tConfInfo.SetChairAlias(tMtAliasArray[0]);
	tConfInfo.SetSpeakerAlias(tMtAliasArray[1]);
	tConfInfo.SetStatus(tConfStatus);
	tConfInfo.SetConfAttrb(tConfAttrb);

	// 构造TConfInfoEx
	tMainStreamCap.SetMaxBitRate((u16)2048);
	tMainStreamCap.SetMediaType((u8)MEDIA_TYPE_H264);
	tMainStreamCap.SetResolution((u8)VIDEO_FORMAT_HD720);
	tMainStreamCap.SetFrameRate((u8)30);
	tMainStreamCap.SetH264ProfileAttrb(emHpAttrb);

	tDoubleStreamCap.SetMediaType((u8)VIDEO_DSTREAM_H264);
	tDoubleStreamCap.SetResolution((u8)VIDEO_FORMAT_HD720);
	tDoubleStreamCap.SetFrameRate((u8)30);

	u8 byCapNum = 1;
	tConfInfoEx.SetMainStreamCapEx(&tMainStreamCap, byCapNum);
	tConfInfoEx.SetDoubleStreamCapEx(&tDoubleStreamCap, byCapNum);
	tConfInfoEx.SetAudioTypeDesc(&tAudioDesc, byCapNum);

	SetMeetingIDToJson(achJsonBuf, dwJsonBufLen, 133);
	SetConfInfoToJson(achJsonBuf, dwJsonBufLen, tConfInfo);
	SetConfInfoExToJson(achJsonBuf, dwJsonBufLen, tConfInfoEx);
	SetMtAliasListToJson(achJsonBuf, dwJsonBufLen, tMtAliasArray, awMtDialRate, byMtNum);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	CMauMsg cServMsg;
	cServMsg.SetEventId(CM_MAU_CREATECONF_REQ);
	cServMsg.SetMsgBody((u8*)&tReqHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody((u8*)achJsonBuf, strlen(achJsonBuf));

	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), CM_MAU_CREATECONF_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	return;
}

API void createconfbytemp(u32 dwSerialNO, u32 dwGkPreFix)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[mdftemp]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%05d%06d", dwSerialNO, dwGkPreFix);
	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = MAXLEN_JSONBUF;
	u32 dwLen = 0;

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetReqEventID(CM_MAU_CREATECONFBYTEMP_REQ);
	tReqHeadInfo.SetConfName("");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetReqSrcID(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON));

	SetMsgTypeToJson(achJsonBuf, dwLen, EV_CM_MAU_CREATECONFBYTEMP_REQ);
	CMauData::SetDomainMoIdToJson(achJsonBuf, tReqHeadInfo.GetDomainMOID(), dwJsonBufLen);
	//	g_cMauVCApp.SetConfNameToJson( achJsonBuf, tReqHeadInfo.GetConfName(), dwJsonBufLen );

	SetConfE164ToJson(achJsonBuf, dwJsonBufLen, achConfE164);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	CMauMsg cServMsg;
	cServMsg.SetEventId(CM_MAU_CREATECONFBYTEMP_REQ);
	cServMsg.SetMsgBody((u8*)&tReqHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody((u8*)achJsonBuf, strlen(achJsonBuf));

	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), CM_MAU_CREATECONFBYTEMP_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	return;
}

API void mtcreateconf(u32 dwGkPreFix, u32 dwSerialNO)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 9999999)
	{
		OspPrintf(TRUE, FALSE, "[mdftemp]Income value err, return!\n");
		return;
	}

	s8 achMtE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achMtE164, sizeof(achMtE164), "%06d%07d", dwGkPreFix, dwSerialNO);

	s8 achConfName[MAXLEN_CONFNAME + 1] = { 0 };
	s8 achConfNameEnd[30 + 1] = { 0 };
	strncpy(achConfNameEnd, "召集的会议", sizeof(achConfNameEnd));
	u32 dwConfNameLen = strlen(achMtE164) + strlen(achConfNameEnd) + 1;
	snprintf(achConfName, dwConfNameLen, "%s%s", achMtE164, achConfNameEnd);

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	TCreateConfReq tCreateConfInfo;
	TMiniConfInfo tMiniConfInfo;
	//	tMiniConfInfo.SetConfId( "00c00021000000000000210001000000" );
	tMiniConfInfo.SetDuration(120);
	tMiniConfInfo.SetBitRate(1024);
	tMiniConfInfo.SetConfName(achConfName);
	tMiniConfInfo.SetConfPwd("");
	tMiniConfInfo.SetOpenMode(1);
	tMiniConfInfo.SetReleaseMode(1);
	tMiniConfInfo.SetDiscussConf(TRUE);
	tMiniConfInfo.SetAllInitDumb(FALSE);

	TCapSupport tCapSupport;
	memset((void*)&tCapSupport, 0, sizeof(tCapSupport));
	//双流
	tCapSupport.SetDStreamSupportH239(TRUE);
	tCapSupport.SetDStreamType(VIDEO_DSTREAM_H264_H239);
	tCapSupport.SetDStreamMediaType(MEDIA_TYPE_H264);
	tCapSupport.SetDStreamResolution(VIDEO_FORMAT_CIF);			//双流分辨率
	tCapSupport.SetDStreamFrmRate(VIDEO_FPS_2997_1);			//帧率 	
	//主流
	TSimCapSet tMain;
	tMain.SetAudioMediaType(MEDIA_TYPE_G7221C);					//音频类型
	tMain.SetVideoMediaType(MEDIA_TYPE_H264);					//视频类型	
	tMain.SetVideoResolution(VIDEO_FORMAT_HD720);				//分辨率
	tMain.SetVideoFrameRate(VIDEO_FPS_2997_1);					//帧率
	//辅流
	TSimCapSet tSecond;
	tSecond.Clear();

	tCapSupport.SetMainSimCapSet(tMain);
	tCapSupport.SetSecondSimCapSet(tSecond);
	tMiniConfInfo.SetCapSupport(tCapSupport);

	TMediaEncrypt tMediaEncrypt;
	tMediaEncrypt.SetEncryptMode((u8)CONF_ENCRYPTMODE_NONE);
	tMiniConfInfo.SetMediaKey(tMediaEncrypt);

	//	TVMPParam		tVMPParam;
	// 	tVMPParam.SetVMPStyle(VMP_STYLE_DYNAMIC);							//画面合成风格
	// 	tVMPParam.SetIsAddMmbAlias(TRUE);									//显示终端别名
	// 	tVMPParam.SetVMPMode(CONF_VMPMODE_AUTO);							//2 - 自动图像复合(动态分屏与设置成员)
	// 	tVMPParam.SetVMPBrdst(1);											//1 - 合成图像是否向终端广播
	// 	tVMPParam.SetIsRimEnabled(TRUE);									//边框
	//	tMiniConfInfo.SetVmpParam(tVMPParam);

	tMiniConfInfo.SetPublicConf(1);
	tCreateConfInfo.SetConfInfo(tMiniConfInfo);
	tCreateConfInfo.SetStaDCastMode(0);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetMtType(MT_TYPE_MT);
	tReqHeadInfo.SetSrcMtE164(achMtE164);
	tReqHeadInfo.SetMtAlias(achMtE164);
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetReqEventID(NU_MAU_CREATECONF_REQ);
	tReqHeadInfo.SetConfName(achConfName);
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetReqSrcID(2);

	CMauMsg cServMsg;
	cServMsg.SetEventId(NU_MAU_CREATECONF_REQ);
	cServMsg.SetMsgBody((u8*)&tReqHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody((u8*)&tCreateConfInfo, sizeof(TCreateConfReq));

	OspPost(MAKEIID(AID_MAU_NUSSN_APPID, (u16)1), NU_MAU_CREATECONF_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	return;
}

API void mtcreateconfbytemp(u32 dwGkPreFix, u32 dwGkConfPre, u32 dwSerialNO, u32 dwTempSerialNO)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwGkConfPre > 9999 || dwSerialNO > 9999999 || dwTempSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[mdftemp]Income value err, return!\n");
		return;
	}

	s8 achMtE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achMtE164, sizeof(achMtE164), "%06d%07d", dwGkPreFix, dwSerialNO);

	s8 achTempKey[MAXLEN_CONFE164 + 1] = { 0 };
	snprintf(achTempKey, sizeof(achTempKey), "%05d%04d", dwTempSerialNO, dwGkConfPre);

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetMtType(MT_TYPE_MT);
	tReqHeadInfo.SetSrcMtE164(achMtE164);
	tReqHeadInfo.SetMtAlias(achMtE164);
	tReqHeadInfo.SetConfE164(achTempKey);
	tReqHeadInfo.SetReqEventID(NU_MAU_JOINCONF_REQ);
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetReqSrcID(2);


	CMauMsg cServMsg;
	cServMsg.SetEventId(NU_MAU_JOINCONF_REQ);
	cServMsg.SetMsgBody((u8*)&tReqHeadInfo, sizeof(TReqHeadInfo));

	OspPost(MAKEIID(AID_MAU_NUSSN_APPID, (u16)1), NU_MAU_JOINCONF_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	return;
}

API void delayconf(u32 dwGkPreFix, u32 dwSerialNO, u32 dwDelayTime)
{
	if (dwGkPreFix > 9999 || dwSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[delayconf]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%04d%05d", dwGkPreFix, dwSerialNO);

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_MAU_DELAYCONF_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);
	SetDelayToJson(achJsonBuf, dwJsonBufLen, dwDelayTime);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void confsafe(u32 dwGkPreFix, u32 dwSerialNO, u32 dwConfOpen)
{
	if (dwGkPreFix > 9999 || dwSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[confsafe]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_CONFE164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%04d%05d", dwGkPreFix, dwSerialNO);

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_SETCONFSAFE_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);
	SetConfSafeToJson(achJsonBuf, dwJsonBufLen, dwConfOpen);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void addtemp(s8 *pchtempname)
{
	s8 achTempName[MAXLEN_CONFNAME + 1] = { 0 };
	u32 dwTempNameLen = 0;
	dwTempNameLen = MAXLEN_CONFNAME > strlen(pchtempname) ? strlen(pchtempname) : MAXLEN_CONFNAME;
	memcpy(achTempName, pchtempname, dwTempNameLen);

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = MAXLEN_JSONBUF;
	u32 dwLen = 0;

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetReqEventID(CM_MAU_CREATETEMPLATE_REQ);
	tReqHeadInfo.SetConfName(achTempName);
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetReqSrcID(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON));

	SetMsgTypeToJson(achJsonBuf, dwLen, EV_CM_MAU_CREATETEMPLATE_REQ);
	CMauData::SetDomainMoIdToJson(achJsonBuf, tReqHeadInfo.GetDomainMOID(), dwJsonBufLen);
	SetConfNameToJson(achJsonBuf, tReqHeadInfo.GetConfName(), dwJsonBufLen);
	SetTempInfoToJson(achJsonBuf, dwJsonBufLen);

	OspPrintf(TRUE, TRUE, "[rnn-test]temp info.%s.\n", achJsonBuf);

	CMauMsg cServMsg;
	cServMsg.SetEventId(CM_MAU_CREATETEMPLATE_REQ);
	cServMsg.SetMsgBody((u8*)&tReqHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody((u8*)achJsonBuf, strlen(achJsonBuf));

	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), CM_MAU_CREATETEMPLATE_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
}

API void mdftemp(u32 dwSerialNO, u32 dwGkPreFix, s8 *pchtempname)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[mdftemp]Income value err, return!\n");
		return;
	}

	s8 achTempKey[MAXLEN_E164 + 1] = { 0 };
	snprintf(achTempKey, sizeof(achTempKey), "%05d%06d", dwSerialNO, dwGkPreFix);

	s8 achTempName[MAXLEN_CONFNAME + 1] = { 0 };
	u32 dwTempNameLen = 0;
	dwTempNameLen = MAXLEN_CONFNAME > strlen(pchtempname) ? strlen(pchtempname) : MAXLEN_CONFNAME;
	memcpy(achTempName, pchtempname, dwTempNameLen);

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = MAXLEN_JSONBUF;
	u32 dwLen = 0;

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);


	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetReqEventID(CM_MAU_MDFTEMPLATE_REQ);
	tReqHeadInfo.SetConfName(achTempName);
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetReqSrcID(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON));

	SetMsgTypeToJson(achJsonBuf, dwLen, EV_CM_MAU_MDFTEMPLATE_REQ);
	CMauData::SetDomainMoIdToJson(achJsonBuf, tReqHeadInfo.GetDomainMOID(), dwJsonBufLen);
	SetConfNameToJson(achJsonBuf, tReqHeadInfo.GetConfName(), dwJsonBufLen);

	SetTempInfoToJson(achJsonBuf, dwJsonBufLen);
	SetConfE164ToJson(achJsonBuf, dwJsonBufLen, achTempKey);

	OspPrintf(TRUE, TRUE, "[rnn-test]after mdf temp info.%s.\n", achJsonBuf);

	CMauMsg cServMsg;
	cServMsg.SetEventId(CM_MAU_MDFTEMPLATE_REQ);
	cServMsg.SetMsgBody((u8*)&tReqHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody((u8*)achJsonBuf, strlen(achJsonBuf));

	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), CM_MAU_MDFTEMPLATE_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());
}

API void deltemp(void)
{
	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = MAXLEN_JSONBUF;
	u32 dwLen = 0;

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);


	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetReqEventID(CM_MAU_DELTEMPLATE_REQ);
	tReqHeadInfo.SetConfName("rnntest");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetReqSrcID(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON));

	SetMsgTypeToJson(achJsonBuf, dwLen, EV_CM_MAU_DELTEMPLATE_REQ);
	CMauData::SetDomainMoIdToJson(achJsonBuf, tReqHeadInfo.GetDomainMOID(), dwJsonBufLen);
	SetConfNameToJson(achJsonBuf, tReqHeadInfo.GetConfName(), dwJsonBufLen);

	const s8* achDelTempList = "999990512119999805121199997051211";


	SetDelTempListToJson(achJsonBuf, dwJsonBufLen, achDelTempList);

	OspPrintf(TRUE, TRUE, "[rnn-test]del temp info.%s.\n", achJsonBuf);

	CMauMsg cServMsg;
	cServMsg.SetEventId(CM_MAU_DELTEMPLATE_REQ);
	cServMsg.SetMsgBody((u8*)&tReqHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody((u8*)achJsonBuf, strlen(achJsonBuf));

	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), CM_MAU_DELTEMPLATE_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	return;
}

API void addbook(s8 *pchbookname)
{
	s8 achBookName[MAXLEN_CONFNAME + 1] = { 0 };
	u32 dwBookNameLen = 0;
	dwBookNameLen = MAXLEN_CONFNAME > strlen(pchbookname) ? strlen(pchbookname) : MAXLEN_CONFNAME;
	memcpy(achBookName, pchbookname, dwBookNameLen);

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = MAXLEN_JSONBUF;
	u32 dwLen = 0;

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetReqEventID(CM_MAU_ADDBOOKCONF_REQ);
	tReqHeadInfo.SetConfName(achBookName);
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetReqSrcID(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON));

	SetMsgTypeToJson(achJsonBuf, dwLen, EV_CM_MAU_ADDBOOKCONF_REQ);
	CMauData::SetDomainMoIdToJson(achJsonBuf, tReqHeadInfo.GetDomainMOID(), dwJsonBufLen);
	SetConfNameToJson(achJsonBuf, tReqHeadInfo.GetConfName(), dwJsonBufLen);

	u8 byMtNum = 0;
	TMediaEncrypt tMediaEncrypt;
	TConfAttrb tConfAttrb;
	TConfInfo tConfInfo;
	TConfInfoEx tConfInfoEx;
	TConfStatus tConfStatus;
	TVideoStreamCap tMainStreamCap;
	TVideoStreamCap tDoubleStreamCap;
	TAudioTypeDesc tAudioDesc;
	u16 awMtDialRate[MAXNUM_CONF_MT] = { 0 };
	TMtAlias tMtAliasArray[MAXNUM_CONF_MT];
	s8 achMtNO[MAXLEN_E164] = { 0 };

	memset(awMtDialRate, 0, sizeof(awMtDialRate));
	memset(tMtAliasArray, 0, sizeof(tMtAliasArray));

	// 构造MtAlias
	// 	tMtAliasArray[0].SetAlias( mtAliasTypeE164, (u8)13, "0512119999999" );
	// 	tMtAliasArray[1].SetAlias( mtAliasTypeE164, (u8)13, "0512119999998" );
	// 	tMtAliasArray[2].SetAlias( mtAliasTypeE164, (u8)13, "0512119999997" );
	// 	tMtAliasArray[3].SetAlias( mtAliasTypeE164, (u8)13, "0512119999996" );
	byMtNum = 0;

	// 构造TConfInfo
	tMediaEncrypt.SetEncryptMode((u8)CONF_ENCRYPTMODE_AES);
	tConfStatus.SetCallMode((u8)CONF_CALLMODE_TIMER);
	tConfStatus.SetCallInterval((u8)10);
	tConfStatus.SetCallTimes((u32)5);
	tConfAttrb.SetPortConfFlag(FALSE);
	tConfAttrb.SetDualMode((u8)CONF_DUALMODE_EACHMTATWILL);
	tConfAttrb.SetSpeakerSrc((u8)CONF_SPEAKERSRC_CHAIR);
	tConfAttrb.SetPublicConf((u8)1);
	tConfAttrb.SetOpenMode((u8)CONF_OPENMODE_OPEN);
	tConfAttrb.SetQualityPri(TRUE);

	tConfInfo.SetConfName(tReqHeadInfo.GetConfName());
	tConfInfo.SetDomainName(cDomain.GetWholeName());
	tConfInfo.SetDomainMoid(cDomain.GetDomainMOID());
	tConfInfo.SetConfPwd("d41d8cd98f00b204e9800998ecf8427e");
	tConfInfo.SetDuration(240);
	tConfInfo.SetMediaKey(tMediaEncrypt);
	tConfInfo.SetChairAlias(tMtAliasArray[0]);
	tConfInfo.SetSpeakerAlias(tMtAliasArray[1]);
	tConfInfo.SetStatus(tConfStatus);
	tConfInfo.SetConfAttrb(tConfAttrb);

	// 构造TConfInfoEx
	tMainStreamCap.SetMaxBitRate((u16)2048);
	tMainStreamCap.SetMediaType((u8)MEDIA_TYPE_H264);
	tMainStreamCap.SetResolution((u8)VIDEO_FORMAT_HD720);
	tMainStreamCap.SetFrameRate((u8)30);
	tMainStreamCap.SetH264ProfileAttrb(emHpAttrb);

	tDoubleStreamCap.SetMediaType((u8)VIDEO_DSTREAM_H264);
	tDoubleStreamCap.SetResolution((u8)VIDEO_FORMAT_HD720);
	tDoubleStreamCap.SetFrameRate((u8)30);

	u8 byCapNum = 1;
	tConfInfoEx.SetMainStreamCapEx(&tMainStreamCap, byCapNum);
	tConfInfoEx.SetDoubleStreamCapEx(&tDoubleStreamCap, byCapNum);
	tConfInfoEx.SetAudioTypeDesc(&tAudioDesc, byCapNum);

	SetConfInfoToJson(achJsonBuf, dwJsonBufLen, tConfInfo);
	SetConfInfoExToJson(achJsonBuf, dwJsonBufLen, tConfInfoEx);
	SetMtAliasListToJson(achJsonBuf, dwJsonBufLen, tMtAliasArray, awMtDialRate, byMtNum);

	//115051211999972014-10-22 19:00:002014-10-22 21:00:00119051211999962014-10-23 19:00:002014-10-23 21:00:00
	//107051211999972014-10-22 17:00:002014-10-22 19:20:00111051211999962014-10-26 17:00:002014-10-26 19:20:00
	const s8* achBookList = "115051211999972014-11-08 19:00:002014-11-08 21:00:00119051211999962014-10-23 19:00:002014-10-23 21:00:00";


	SetBookListToJson(achJsonBuf, dwJsonBufLen, achBookList);

	OspPrintf(TRUE, TRUE, "[rnn-test]book info.%s.\n", achJsonBuf);

	CMauMsg cServMsg;
	cServMsg.SetEventId(CM_MAU_ADDBOOKCONF_REQ);
	cServMsg.SetMsgBody((u8*)&tReqHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody((u8*)achJsonBuf, strlen(achJsonBuf));

	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), CM_MAU_ADDBOOKCONF_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	return;
}

API void mdfbook(s8 *pchbookname)
{
	s8 achBookName[MAXLEN_CONFNAME + 1] = { 0 };
	u32 dwBookNameLen = 0;
	dwBookNameLen = MAXLEN_CONFNAME > strlen(pchbookname) ? strlen(pchbookname) : MAXLEN_CONFNAME;
	memcpy(achBookName, pchbookname, dwBookNameLen);

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = MAXLEN_JSONBUF;
	u32 dwLen = 0;

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetReqEventID(CM_MAU_MDFBOOKCONF_REQ);
	tReqHeadInfo.SetConfName(achBookName);
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetReqSrcID(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON));

	SetMsgTypeToJson(achJsonBuf, dwLen, EV_CM_MAU_MDFBOOKCONF_REQ);
	CMauData::SetDomainMoIdToJson(achJsonBuf, tReqHeadInfo.GetDomainMOID(), dwJsonBufLen);
	SetConfNameToJson(achJsonBuf, tReqHeadInfo.GetConfName(), dwJsonBufLen);

	u8 byMtNum = 0;
	TMediaEncrypt tMediaEncrypt;
	TConfAttrb tConfAttrb;
	TConfInfo tConfInfo;
	TConfInfoEx tConfInfoEx;
	TConfStatus tConfStatus;
	TVideoStreamCap tMainStreamCap;
	TVideoStreamCap tDoubleStreamCap;
	TAudioTypeDesc tAudioDesc;
	u16 awMtDialRate[MAXNUM_CONF_MT] = { 0 };
	TMtAlias tMtAliasArray[MAXNUM_CONF_MT];
	s8 achMtNO[MAXLEN_E164] = { 0 };

	memset(awMtDialRate, 0, sizeof(awMtDialRate));
	memset(tMtAliasArray, 0, sizeof(tMtAliasArray));

	// 构造MtAlias
	tMtAliasArray[0].SetAlias(mtAliasTypeE164, (u8)13, "0512119999999");
	tMtAliasArray[1].SetAlias(mtAliasTypeE164, (u8)13, "0512119999998");
	tMtAliasArray[2].SetAlias(mtAliasTypeE164, (u8)13, "0512119999997");
	tMtAliasArray[3].SetAlias(mtAliasTypeE164, (u8)13, "0512119999996");
	byMtNum = 4;

	// 构造TConfInfo
	tMediaEncrypt.SetEncryptMode((u8)CONF_ENCRYPTMODE_AES);
	tConfStatus.SetCallMode((u8)CONF_CALLMODE_TIMER);
	tConfStatus.SetCallInterval((u8)10);
	tConfStatus.SetCallTimes((u32)5);
	tConfAttrb.SetPortConfFlag(FALSE);
	tConfAttrb.SetDualMode((u8)CONF_DUALMODE_EACHMTATWILL);
	tConfAttrb.SetSpeakerSrc((u8)CONF_SPEAKERSRC_CHAIR);
	tConfAttrb.SetPublicConf((u8)1);
	tConfAttrb.SetOpenMode((u8)CONF_OPENMODE_OPEN);
	tConfAttrb.SetQualityPri(TRUE);

	tConfInfo.SetConfName(tReqHeadInfo.GetConfName());
	tConfInfo.SetDomainName(cDomain.GetWholeName());
	tConfInfo.SetDomainMoid(cDomain.GetDomainMOID());
	tConfInfo.SetConfPwd("d41d8cd98f00b204e9800998ecf8427e");
	tConfInfo.SetDuration(240);
	tConfInfo.SetMediaKey(tMediaEncrypt);
	tConfInfo.SetChairAlias(tMtAliasArray[0]);
	tConfInfo.SetSpeakerAlias(tMtAliasArray[1]);
	tConfInfo.SetStatus(tConfStatus);
	tConfInfo.SetConfAttrb(tConfAttrb);

	// 构造TConfInfoEx
	tMainStreamCap.SetMaxBitRate((u16)2048);
	tMainStreamCap.SetMediaType((u8)MEDIA_TYPE_H264);
	tMainStreamCap.SetResolution((u8)VIDEO_FORMAT_HD720);
	tMainStreamCap.SetFrameRate((u8)30);
	tMainStreamCap.SetH264ProfileAttrb(emHpAttrb);

	tDoubleStreamCap.SetMediaType((u8)VIDEO_DSTREAM_H264);
	tDoubleStreamCap.SetResolution((u8)VIDEO_FORMAT_HD720);
	tDoubleStreamCap.SetFrameRate((u8)30);

	u8 byCapNum = 1;
	tConfInfoEx.SetMainStreamCapEx(&tMainStreamCap, byCapNum);
	tConfInfoEx.SetDoubleStreamCapEx(&tDoubleStreamCap, byCapNum);
	tConfInfoEx.SetAudioTypeDesc(&tAudioDesc, byCapNum);

	SetConfInfoToJson(achJsonBuf, dwJsonBufLen, tConfInfo);
	SetConfInfoExToJson(achJsonBuf, dwJsonBufLen, tConfInfoEx);
	SetMtAliasListToJson(achJsonBuf, dwJsonBufLen, tMtAliasArray, awMtDialRate, byMtNum);

	const s8* achBookList = "382051211999982014-10-28 21:00:002014-10-28 23:20:00398051211999972014-10-29 21:00:002014-10-29 23:20:00";

	SetBookListToJson(achJsonBuf, dwJsonBufLen, achBookList);

	OspPrintf(TRUE, TRUE, "[rnn-test]book info.%s.\n", achJsonBuf);

	CMauMsg cServMsg;
	cServMsg.SetEventId(CM_MAU_MDFBOOKCONF_REQ);
	cServMsg.SetMsgBody((u8*)&tReqHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody((u8*)achJsonBuf, strlen(achJsonBuf));

	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), CM_MAU_MDFBOOKCONF_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	return;
}

API void delbook(void)
{
	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = MAXLEN_JSONBUF;
	u32 dwLen = 0;

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetReqEventID(CM_MAU_DELBOOKCONF_REQ);
	tReqHeadInfo.SetConfName("");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetReqSrcID(MAKEIID(AID_MAU_CMMAUSSN_APPID, CInstance::DAEMON));

	SetMsgTypeToJson(achJsonBuf, dwLen, EV_CM_MAU_DELBOOKCONF_REQ);
	CMauData::SetDomainMoIdToJson(achJsonBuf, tReqHeadInfo.GetDomainMOID(), dwJsonBufLen);
	SetConfNameToJson(achJsonBuf, tReqHeadInfo.GetConfName(), dwJsonBufLen);

	const s8* achBookList = "382051211999982014-10-28 21:00:002014-10-28 23:20:00398051211999972014-10-29 21:00:002014-10-29 23:20:00";

	SetBookListToJson(achJsonBuf, dwJsonBufLen, achBookList);

	OspPrintf(TRUE, TRUE, "[rnn-test]book info.%s.\n", achJsonBuf);

	CMauMsg cServMsg;
	cServMsg.SetEventId(CM_MAU_DELBOOKCONF_REQ);
	cServMsg.SetMsgBody((u8*)&tReqHeadInfo, sizeof(TReqHeadInfo));
	cServMsg.CatMsgBody((u8*)achJsonBuf, strlen(achJsonBuf));

	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), CM_MAU_DELBOOKCONF_REQ, cServMsg.GetServMsg(), cServMsg.GetServMsgLen());

	return;
}

API void forcebrd(u32 dwGkPreFix, u32 dwSerialNO, u32 dwForcebrd)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[forcebrd]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_FORCEBRD_CMD);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);
	SetForceBrdReqToJson(achJsonBuf, dwJsonBufLen, dwForcebrd);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void confmute(u32 dwGkPreFix, u32 dwSerialNO)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[forcebrd]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_CONFMUTE_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void confnotmute(u32 dwGkPreFix, u32 dwSerialNO)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[forcebrd]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_CANCELCONFMUTE_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void confsilence(u32 dwGkPreFix, u32 dwSerialNO)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[forcebrd]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_CONFSILENCE_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void confnotsilence(u32 dwGkPreFix, u32 dwSerialNO)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[forcebrd]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_CANCELCONFSILENCE_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void setchair(u32 dwGkPreFix, u32 dwSerialNO, u32 dwMt1, u32 dwMt2)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999 || dwMt1 > MAXVALUE_GK_PREFIX || dwMt2 > 9999999)
	{
		OspPrintf(TRUE, FALSE, "[setchair]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	s8 achMtNo[MAXLEN_E164 + 1] = { 0 };
	snprintf(achMtNo, sizeof(achMtNo), "%06d%07d", dwMt1, dwMt2);


	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_SETCHAIRMAN_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);
	SetMtNOInfoToJson(achJsonBuf, dwJsonBufLen, achMtNo);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void setspeaker(u32 dwGkPreFix, u32 dwSerialNO, u32 dwMt1, u32 dwMt2)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999 || dwMt1 > MAXVALUE_GK_PREFIX || dwMt2 > 9999999)
	{
		OspPrintf(TRUE, FALSE, "[setspeaker]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	s8 achMtNo[MAXLEN_E164 + 1] = { 0 };
	snprintf(achMtNo, sizeof(achMtNo), "%06d%07d", dwMt1, dwMt2);


	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_SETSPEAKER_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);
	SetMtNOInfoToJson(achJsonBuf, dwJsonBufLen, achMtNo);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void cancelchair(u32 dwGkPreFix, u32 dwSerialNO)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[cancelchair]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_CANCELCHAIRMAN_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void cancelspeaker(u32 dwGkPreFix, u32 dwSerialNO)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999)
	{
		OspPrintf(TRUE, FALSE, "[cancelspeaker]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_CANCELSPEAKER_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void setmtmute(u32 dwGkPreFix, u32 dwSerialNO, u32 dwMt1, u32 dwMt2)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999 || dwMt1 > MAXVALUE_GK_PREFIX || dwMt2 > 9999999)
	{
		OspPrintf(TRUE, FALSE, "[setmtmute]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	s8 achMtNo[MAXLEN_E164 + 1] = { 0 };
	snprintf(achMtNo, sizeof(achMtNo), "%06d%07d", dwMt1, dwMt2);


	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_MTMUTE_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);
	SetMtNOInfoToJson(achJsonBuf, dwJsonBufLen, achMtNo);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void setmtsilence(u32 dwGkPreFix, u32 dwSerialNO, u32 dwMt1, u32 dwMt2)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999 || dwMt1 > MAXVALUE_GK_PREFIX || dwMt2 > 9999999)
	{
		OspPrintf(TRUE, FALSE, "[setmtsilence]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	s8 achMtNo[MAXLEN_E164 + 1] = { 0 };
	snprintf(achMtNo, sizeof(achMtNo), "%06d%07d", dwMt1, dwMt2);


	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_MTSILENCE_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);
	SetMtNOInfoToJson(achJsonBuf, dwJsonBufLen, achMtNo);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void cancelmtmute(u32 dwGkPreFix, u32 dwSerialNO, u32 dwMt1, u32 dwMt2)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999 || dwMt1 > MAXVALUE_GK_PREFIX || dwMt2 > 9999999)
	{
		OspPrintf(TRUE, FALSE, "[cancelmtmute]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	s8 achMtNo[MAXLEN_E164 + 1] = { 0 };
	snprintf(achMtNo, sizeof(achMtNo), "%06d%07d", dwMt1, dwMt2);


	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_CANCELMTMUTE_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);
	SetMtNOInfoToJson(achJsonBuf, dwJsonBufLen, achMtNo);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void cancelmtsilence(u32 dwGkPreFix, u32 dwSerialNO, u32 dwMt1, u32 dwMt2)
{
	if (dwGkPreFix > MAXVALUE_GK_PREFIX || dwSerialNO > 99999 || dwMt1 > MAXVALUE_GK_PREFIX || dwMt2 > 9999999)
	{
		OspPrintf(TRUE, FALSE, "[cancelmtsilence]Income value err, return!\n");
		return;
	}

	s8 achConfE164[MAXLEN_E164 + 1] = { 0 };
	snprintf(achConfE164, sizeof(achConfE164), "%06d%05d", dwGkPreFix, dwSerialNO);

	s8 achMtNo[MAXLEN_E164 + 1] = { 0 };
	snprintf(achMtNo, sizeof(achMtNo), "%06d%07d", dwMt1, dwMt2);


	CKdvDomainInfo cKey;
	CKdvDomainInfo cDomain;
	cKey.SetDomainGUID("20303000000100000000000000000033");
	cKey.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	g_cMauVCApp.GetDomainInfo(cKey.GetDomainMOID(), cDomain);

	OspPrintf(TRUE, TRUE, "[rnn-test]domain info:\n");
	cDomain.Print();

	s8 achJsonBuf[MAXLEN_JSONBUF + 1] = { 0 };
	u32 dwJsonBufLen = 0;

	SetMsgTypeToJson(achJsonBuf, dwJsonBufLen, EV_CM_CMU_CANCELMTSILENCE_REQ);

	TReqHeadInfo tReqHeadInfo;
	tReqHeadInfo.SetDomainMOID("ttz3z0iz9s455shj4x0jkz86");
	tReqHeadInfo.SetConfName("333333333333333");
	tReqHeadInfo.SetDomainMOID(cDomain.GetDomainMOID());
	tReqHeadInfo.SetMQName("meeting.cmu.reply.queue");   // 此处借用MqName字段类存储Rpc
	tReqHeadInfo.SetMQCorrelationId("59b7a944-1cbe-4f2e-b2b4-54bfca890dbc");	//存储rpc模式id
	SetReqHeadToJson(achJsonBuf, dwJsonBufLen, tReqHeadInfo);

	SetTestReqToJson(achJsonBuf, dwJsonBufLen, achConfE164);
	SetMtNOInfoToJson(achJsonBuf, dwJsonBufLen, achMtNo);

	OspPrintf(TRUE, TRUE, "[rnn-test]conf info.%s by cm.\n", achJsonBuf);

	OspPost(MAKEIID(AID_MAU_CMUSSN_APPID, 1), CM_CMU_CONFOPR_NTF, achJsonBuf, dwJsonBufLen);

	return;
}

API void enablecmu(u16 wMpcId)
{
	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_MAU_ENABLE_CMU_CMD, &wMpcId, sizeof(wMpcId));
}

API void disablecmu(u16 wMpcId)
{
	OspPost(MAKEIID(AID_MAU_MAUVC_APPID, CInstance::DAEMON), MAU_MAU_DISABLE_CMU_CMD, &wMpcId, sizeof(wMpcId));
}
