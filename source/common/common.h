#ifndef DAPP_COMMON
#define DAPP_COMMON

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef uint8_t bool;

#ifndef TRUE
    #define TRUE (1)
#endif

#ifndef FALSE
    #define FALSE (0)
#endif

typedef enum 
{
    DAPP_OK = 0,
    DAPP_FAIL = 1001,
    DAPP_ERR_PARAM,
    DAPP_ERR_OPTION,
    DAPP_ERR_MEM,
    DAPP_ERR_FILE,
    DAPP_ERR_JSON_FMT,
    DAPP_ERR_JSON_CONF,
    DAPP_ERR_EAL_PARAM,
} STATUS;

#ifdef DAPP_DEBUG
    #define DAPP_TRACE(format, ...) printf (format, ##__VA_ARGS__)
#else
    #define DAPP_TRACE(format, ...) do { } while(0)
#endif

#define ITEM(a) (sizeof(a) / sizeof((a)[0]))

#endif