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
    DAPP_FAIL,
    DAPP_ERR_PARAM,
    DAPP_ERR_MEM,
} STATUS;

#endif