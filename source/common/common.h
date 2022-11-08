#ifndef DAPP_COMMON
#define DAPP_COMMON

typedef unsigned char UINT8_T;
typedef unsigned short UINT16_T;
typedef unsigned int UINT32_T;
typedef unsigned long long UINT64_T;

typedef UINT8_T bool;

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
    DAPP_ERR_MODL_TYPE,
} STATUS;

#ifdef DAPP_DEBUG
    #define DAPP_TRACE(format, ...) printf (format, ##__VA_ARGS__)
#else
    #define DAPP_TRACE(format, ...) do { } while(0)
#endif

#define ITEM(a) (sizeof(a) / sizeof((a)[0]))

#define DAPP_CONSTRUCTOR(func, args...)             \
{                                                   \
    __attribute__((constructor)) func(args...);     \
}

#define DAPP_DESTRUCTOR(func, args...)              \
{                                                   \
    __attribute__((destructor)) func(args...);      \
}

#define DAPP_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define DAPP_MIN(a, b) (((a) > (b)) ? (b) : (a))

#endif