#ifndef DAPP_SESSION
#define DAPP_SESSION

#include "common.h"
#include "dapp_list.h"

typedef struct 
{
    UINT8_T ip_version;

    union {
        UINT64_T v6[2];
        UINT32_T v4[4];
        UINT8_T addr[16];
    };
} IP_ADDR_T;

typedef struct 
{
    IP_ADDR_T sip;
    IP_ADDR_T dip;
    UINT16_T sport;
    UINT16_T dport;
    UINT32_T proto_id;
    
} DAPP_SESSION_KEY_T;

typedef struct 
{
    struct rte_hash *hash;
    struct rte_mempool *pool;
    dapp_list_t time_list;

    UINT32_T timeout;
        
} DAPP_SESSION_TABLE_T;

typedef struct 
{
    UINT32_T proto_id;
    DAPP_SESSION_KEY_T session_key;
    dapp_list_t time_node;

    UINT64_T timeout;

    UINT8_T data[0];
} DAPP_SESSION_NODE_T;

#endif
