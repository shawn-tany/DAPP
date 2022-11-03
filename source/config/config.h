#ifndef DAPP_CONFIG
#define DAPP_CONFIG

#include "common.h"

#define DAPP_PORT_NAME_SIZE 64
#define DAPP_MAX_PORT_NUM 64

typedef struct {
    uint16_t node_size;
    uint32_t node_num;
} dapp_mempool_conf_t;

typedef struct {
    uint8_t port_num;
    char ports[DAPP_MAX_PORT_NUM][DAPP_PORT_NAME_SIZE];
} dapp_port_conf_t;

/*
 * init configuration, which cannot be modified at runtime
 */
typedef struct 
{
    struct {
        uint8_t thread_num;
        dapp_mempool_conf_t mempool;
        dapp_port_conf_t port;
    } port;

    struct {
        uint8_t thread_num;
        dapp_mempool_conf_t mempool;
        uint32_t window;
    } flows;

    struct {
        uint8_t thread_num;
    } rule;

    struct {
        uint8_t thread_num;
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
