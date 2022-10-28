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
    char port[DAPP_MAX_PORT_NUM][DAPP_PORT_NAME_SIZE];
} dapp_port_list_t;

/*
 * Static configuration, which cannot be modified at runtime
 */
typedef struct 
{
    struct {
        uint8_t thread_num;
        dapp_mempool_conf_t mempool;
        dapp_port_list_t port_list;
    } port;

    struct {
        uint8_t thread_num;
        dapp_mempool_conf_t mempool;
        uint32_t window;
    } flow_iotonic;

    struct {
        uint8_t thread_num;
    } rule_match;

    struct {
        uint8_t thread_num;
    } proto_identi;
} dapp_static_conf_t;

/*
 * Dynamic configuration, which can be modified at runtime
 */
typedef struct 
{
    
} dapp_dynamic_conf_t;

/*
 * All configuration
 */
typedef struct 
{
    dapp_static_conf_t static_conf;
    dapp_dynamic_conf_t dynamic_conf;
} dapp_conf_t;

/*
 * Resolve static configuration
 */
STATUS dapp_static_conf_parse(dapp_static_conf_t *static_conf, const char *file_name);

/*
 * Resolve dynamic configuration
 */
void dapp_static_conf_dump(dapp_static_conf_t *static_conf);

/*
 * Resolve dynamic configuration
 */
STATUS dapp_dynamic_conf_parse(dapp_dynamic_conf_t *dynamic_conf, const char *file_name);

#endif
