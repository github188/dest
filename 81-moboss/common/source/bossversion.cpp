

#include <stdio.h>
#include "bossversion.h"
#include "kdvsys.h"

/*lint -save -e652*/
#ifdef WIN32
#ifndef vsnprintf
#define vsnprintf   _vsnprintf
#endif
#ifndef snprintf
#define snprintf    _snprintf
#endif
#endif


std::string GetBossBuildVersion()
{		
	s8 szDate[32+1] = {0};
	snprintf(szDate, sizeof(szDate), "%s", __DATE__);	
	szDate[ sizeof(szDate)-1 ] = '\0';

	StrUpper(szDate);
	
	s8 szMon[16] = {0};
	u32 dwDay = 0;
	u32 dwYear = 0;
	sscanf(szDate, "%s %d %d", szMon, &dwDay, &dwYear );
	
	u32 byMonth = 0;
	if ( 0 == strcmp( szMon, "JAN") )		 
		byMonth = 1;
	else if ( 0 == strcmp( szMon, "FEB") )
		byMonth = 2;
	else if ( 0 == strcmp( szMon, "MAR") )
		byMonth = 3;
	else if ( 0 == strcmp( szMon, "APR") )		 
		byMonth = 4;
	else if ( 0 == strcmp( szMon, "MAY") )
		byMonth = 5;
	else if ( 0 == strcmp( szMon, "JUN") )
		byMonth = 6;
	else if ( 0 == strcmp( szMon, "JUL") )
		byMonth = 7;
	else if ( 0 == strcmp( szMon, "AUG") )
		byMonth = 8;
	else if ( 0 == strcmp( szMon, "SEP") )		 
		byMonth = 9;
	else if ( 0 == strcmp( szMon, "OCT") )
		byMonth = 10;
	else if ( 0 == strcmp( szMon, "NOV") )
		byMonth = 11;
	else if ( 0 == strcmp( szMon, "DEC") )
		byMonth = 12;
	else
		byMonth = 0;
	
	
	s8 szFullDate[24+1] = {0};
	snprintf(szFullDate, sizeof(szFullDate), "%04d%02d%02d", dwYear, byMonth, dwDay);  
	szFullDate[ sizeof(szFullDate)-1 ] = '\0';
	
	s8 szBossVer[MAXLEN_MO_BOSS_VER + 4] = {0};	    //·þÎñÆ÷°æ±¾ºÅ	
	snprintf(szBossVer, sizeof(szBossVer), "%s.%s", MO_BOSS_VER_PREFIX, szFullDate);
	szBossVer[ sizeof(szBossVer)-1 ] = '\0';
	
    return szBossVer;
}

std::string GetBossCopyRight()
{
	s8 szDate[32+1] = {0};
    snprintf(szDate, sizeof(szDate), "%s", __DATE__);
	szDate[ sizeof(szDate)-1 ] = '\0';
	
	s8 szMon[16+1] = {0};
    u32 dwDay = 0;
    u32 dwYear = 0;
    sscanf(szDate, "%s %d %d", szMon, &dwDay, &dwYear );
	
	s8 szCopyRightBuf[256+1] = { 0 }; 
	snprintf( szCopyRightBuf, sizeof(szCopyRightBuf), MO_BOSS_KEDA_COPYRIGHT, dwYear );
	szCopyRightBuf[ sizeof(szCopyRightBuf) -1 ] = '\0';
	
	return szCopyRightBuf;
}
/*lint -restore*/



