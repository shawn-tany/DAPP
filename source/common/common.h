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
    
    DAPP_ERR_DPDK_EAL_PARAM,
    
    DAPP_ERR_MODL_TYPE,
    DAPP_ERR_MODL_TIMEOUT,
    DAPP_ERR_MODL_UNREG,
    DAPP_ERR_MODL_INIT_NULL,
    DAPP_ERR_MODL_EXEC_NULL,
    DAPP_ERR_MODL_EXIT_NULL,

    DAPP_ERR_PORT_
} STATUS;

#ifdef DAPP_DEBUG
    #define DAPP_TRACE(format, ...) printf (format, ##__VA_ARGS__)
#else
    #define DAPP_TRACE(format, ...) do { } while(0)
#endif

#define PTR_CHECK(ptr)          \
    if (!ptr)                   \
    {                           \
        return DAPP_ERR_PARAM;  \
    }

#define ITEM(a) (sizeof(a) / sizeof((a)[0]))

#define UNUSED(a) ((void)(a))

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

static inline int
dapp_atomic32_cmpset(volatile UINT32_T *dst, UINT32_T exp, UINT32_T src)
{
    UINT8_T res;

    asm volatile(
        "lock ;"
        "cmpxchgl %[src], %[dst];"
        "sete %[res];"
        : [res] "=a" (res),     /* output */
        [dst] "=m" (*dst)
        : [src] "r" (src),      /* input */
        "a" (exp),
        "m" (*dst)
        : "memory");            /* no-clobber list */
    
    return res;
}

#define CORE_NUM (64)

#endif
