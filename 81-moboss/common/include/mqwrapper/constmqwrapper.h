

#ifndef _CONST_MQ_WRAPPER_H_
#define _CONST_MQ_WRAPPER_H_

#include "kdvtype.h"


#define MQWRAPPER_MAXLEN_IP                             (u16)32
#define MQWRAPPER_MAXLEN_VHOST                          (u16)32
#define MQWRAPPER_MAXLEN_USER_NAME						(u16)64
#define MQWRAPPER_MAXLEN_USER_PWD						(u16)32
#define MQWRAPPER_MAXLEN_EXCHANGE_NAME				    (u16)32
#define MQWRAPPER_MAXLEN_QUEUE_NAME					    (u16)64
#define MQWRAPPER_MAXLEN_ROUTINGKEY					    (u16)64
// 生产者发送内容最大长度 [1/6/2016 pengguofeng]
#define MQWRAPPER_MAXLEN_POSTBUF                        (u16)2048

#endif  //_CONST_MQ_WRAPPER_H_
