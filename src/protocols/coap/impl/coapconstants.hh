#ifndef COAPCONSTANTS_H
#define COAPCONSTANTS_H

#define VER   1
#define TYPE_CON  1
#define TYPE_ACK  2
#define TYPE_RST  3

#define METHOD_GET     1
#define METHOD_POST    2  
#define METHOD_PUT     3
#define METHOD_DELETE  4

#define BYTE_VERSION   64
#define BYTE_ACK       32


#define RES_CODE_CREATED          65
#define RES_CODE_DELETED          66
#define RES_CODE_CHANGED          68
#define RES_CODE_CONTENT          69
#define RES_CODE_UNAUTHORIZED    129
#define RES_CODE_BAD_OPTION      130
#define RES_CODE_NOT_FOUND       132
#define RES_CODE_NOT_IMPLEMENTED 161


#define HEADER_COAP_VERSION  (1 << HEADER_VER)
#define HEADER_TYPE_CON      (MSG_CON << HEADER_TYPE)
#define HEADER_TYPE_NON      (MSG_NON << HEADER_TYPE)
#define HEADER_TYPE_ACK      (MSG_ACK << HEADER_TYPE)
#define HEADER_TYPE_RST      (MSG_RST << HEADER_TYPE)
#define HEADER_OC_CON        (2 << HEADER_OC)
#define HEADER_OC_ACK        (1 << HEADER_OC)

#define OPT_CONTENT_TYPE     1
#define OPT_MAX_AGE         2
#define OPT_PROXY_URI       3
#define OPT_ETAG            4
#define OPT_URI_HOST        5
#define OPT_LOCATION_PATH   6
#define OPT_URI_PORT        7
#define OPT_LOCATION_QUERY  8
#define OPT_URI_PATH        9
#define OPT_TOKEN          11
#define OPT_URI_QUERY      15

#define TOKEN_SIZE  1

#define OC_GET 2
#define OC_PUT 2
#define OC_DEL 2


// Protocol constants Sec. 9, p. 49
#define RESPONSE_TIMEOUT 2 // 2 sec
#define MAX_RETRANSMIT   4

//constants for upper layer
#define ERR_GOTRST 1
#define ERR_SENTRST 2
#define CONF_RECV 3


#endif // COAPCONSTANTS_H
