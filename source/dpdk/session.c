#include <stdio.h>

#include "session.h"
#include "rte_malloc.h"
#include "rte_mempool.h"
#include "rte_hash.h"
#include "rte_hash_crc.h"
#include "rte_cycles.h"

int dapp_session_table_create(DAPP_SESSION_TABLE_T **session, UINT32_T priv_size, UINT32_T node_num, UINT32_T timeout)
{
    unsigned flags = MEMPOOL_F_NO_SPREAD;

    *session = rte_malloc(NULL, sizeof(DAPP_SESSION_TABLE_T), 2);

    if (!session) {
        DAPP_TRACE("ERROR : failed to create sessio, can not malloc session\n");
        return -1;
    }

    (*session)->pool = rte_mempool_create("DAPP_SESSION_POOL", node_num, sizeof(DAPP_SESSION_NODE_T), 0, 
                                          priv_size, NULL, NULL, NULL, NULL, rte_socket_id(), flags);

    if (!((*session)->pool)) {
        DAPP_TRACE("ERROR : failed to create sessio, can not create session pool\n");
        return -1;
    }

    struct rte_hash_parameters params = {
        .name = "DAPP_SESSION_HASH",
        .reserved = 0,
        .entries = node_num,
        .key_len = sizeof(DAPP_SESSION_KEY_T),
        .hash_func = rte_hash_crc,
        .hash_func_init_val = 0,
        .socket_id = rte_socket_id(),
        .extra_flag = 0
    };    

    (*session)->timeout = timeout;

    (*session)->hash = rte_hash_create(&params);

    if (!((*session)->hash)) {
        DAPP_TRACE("ERROR : failed to create sessio, can not create session hash\n");
        return -1;
    }

    return 0;
}

int dapp_session_node_create(DAPP_SESSION_TABLE_T *session, DAPP_SESSION_NODE_T **node, 
                                    DAPP_SESSION_KEY_T *session_key, void *priv_data, UINT32_T priv_size)
{
    if (!session) {
        printf("ERROR : failed to create session node, invalid param\n");
        return -1;
    }
    
    if (session->pool->private_data_size < priv_size) {
        printf("ERROR : failed to create session node, invalid priv size\n");
        return -1;
    }

    if (0 != rte_mempool_get(session->pool, (void **)node)) {
        printf("ERROR : failed to create session node, can not get pool node\n");
        return -1;
    }

    (*node)->session_key = *session_key;
    (*node)->timeout = rte_get_tsc_cycles() + session->timeout;
    rte_memcpy((*node)->data, priv_data, priv_size);

    if (0 != rte_hash_add_key_data(session->hash, (void *)session_key, *node)) {
        printf("ERROR : failed to create session node, can not add hash node\n");
        return -1;
    }

    return 0;
}

int dapp_session_node_ingress(DAPP_SESSION_TABLE_T *session, DAPP_SESSION_KEY_T *session_key, DAPP_SESSION_NODE_T **node)
{
    if (0 != rte_hash_lookup_data(session->hash, (void *)session_key, (void **)node)) {
        
    } else {
        
    }

    return 0;
}

int dapp_session_node_timeout()
{
    return 0;
}

int dapp_session_node_destory(DAPP_SESSION_TABLE_T *session, DAPP_SESSION_NODE_T *node)
{
    if (!session || !node) {
        return -1;
    }

    if (0 != rte_hash_del_key(session->hash, (void *)(&(node->session_key)))) {
        return -1;
    }

    rte_mempool_put(session->pool, (void *)node);

    return 0;
}

int dapp_session_table_destory()
{
    return 0;
}
