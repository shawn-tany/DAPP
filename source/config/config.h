#ifndef DAPP_CONFIG
#define DAPP_CONFIG

#include "common.h"

#define DAPP_PORT_NAME_SIZE 64
#define DAPP_MAX_PORT_NUM 64

typedef struct {
    UINT16_T node_size;
    UINT32_T node_num;
} dapp_mempool_conf_t;

typedef struct {
    UINT8_T port_num;
    char ports[DAPP_MAX_PORT_NUM][DAPP_PORT_NAME_SIZE];
} dapp_port_conf_t;

/*
 * init configuration, which cannot be modified at runtime
 */
typedef struct 
{
    struct {
        UINT8_T thread_num;
        dapp_mempool_conf_t mempool;
        dapp_port_conf_t port;
    } port;

    struct {
        UINT8_T thread_num;
        dapp_mempool_conf_t mempool;
        UINT32_T window;
    } flows;

    struct {
        UINT8_T thread_num;
    } rule;

    struct {
        UINT8_T thread_num;
    } protocol;
} dapp_conf_t;

/*
 * Resolve static configuration
 */
STATUS dapp_conf_parse(dapp_conf_t *conf, const char *file_name);

/*
 * Resolve dynamic configuration
 */
void dapp_conf_dump(dapp_conf_t *static_conf);

#endif
