#ifndef __COMMONDEF__
#define __COMMONDEF__

//app id
#define  AID_MCU_PERIEQPSSN		27
#define  AID_MPU				127

//max limit value
#define  MAXNUM_MCU_PERIEQP		2
#define  MAXLEN_ALIAS			128		
#define  MAXNUM_MSG				300		// just for test

//inner event define
#define		INNER_EVBASE				10000
#define		POWERON						INNER_EVBASE
#define		INNER_PRE_REQ				(INNER_EVBASE+1)
#define		INNER_SNDMSG_CMD			(INNER_EVBASE+10)
#define		INNER_RCVMSG_NTF			(INNER_EVBASE+20)

#ifndef snprintf
#ifdef WIN32
#define snprintf	_snprintf
#else
#define snprintf	snprintf
#endif
#endif


//struct
struct TMsgHead
{
	unsigned short wEventid;
	char achEventDes[64];
	//other info such as dst

	//construct
	TMsgHead(){ memset(this, 0, sizeof(TMsgHead)); }
};

#endif
