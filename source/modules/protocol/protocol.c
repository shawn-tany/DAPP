#include <stdio.h>
#include "modules.h"

typedef struct 
{
    UINT8_T  lcore_num;    
} dapp_protocol_conf_t;

typedef struct 
{
    dapp_protocol_conf_t config;
} dapp_protocol_ws_t;

static dapp_protocol_ws_t protocol_ws;

static STATUS dapp_protocol_conf(void *args)
{
    json_t *root = dapp_conf_root_get();    
    json_t *obj_arr = NULL;
    json_t *conf_obj = NULL;    
    json_t *sub_obj = NULL;    
    UINT32_T i = 0;
    UINT32_T array_size = 0;

    array_size = json_array_size(root);

    for (i = 0; i < array_size; ++i) {
        if (!(obj_arr = json_array_get(root, i))) {
            return DAPP_ERR_JSON_FMT;
        }

        /* port parse */
        if (!(conf_obj = json_object_get(obj_arr, "protocol"))) {
            return DAPP_ERR_JSON_CONF;
        }

        /*
         * Get the configuration through the json object
         */
        if (!(sub_obj = json_object_get(conf_obj, "thread_num"))) {
            return DAPP_ERR_JSON_CONF;
        }
        protocol_ws.config.lcore_num = json_integer_value(sub_obj);
    }

    dapp_module_lcore_init(DAPP_MODULE_PROTOCOL, protocol_ws.config.lcore_num);

    dapp_module_rely_init(DAPP_MODULE_PROTOCOL, DAPP_MODULE_UNI_INIT, 1, DAPP_MODULE_FLOWS);

    return DAPP_OK;
}

static STATUS dapp_protocol_init(void *arg)
{
    DAPP_TRACE("dapp protocol init\n");

    return DAPP_OK;
}

static STATUS dapp_protocol_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp protocol exec\n");

    while (*running) {
        sleep(3);
    }

    DAPP_TRACE("dapp protocol exec end\n");

    return DAPP_OK;
}

static STATUS dapp_protocol_exit(void *arg)
{
    DAPP_TRACE("dapp protocol exit\n");

    return DAPP_OK;
}

static DAPP_MODULE_OPS protocol_ops = {
    .conf = dapp_protocol_conf,
    .init = dapp_protocol_init,
    .exec = dapp_protocol_exec,
    .exit = dapp_protocol_exit   
};

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_PROTOCOL, protocol, &protocol_ops);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_PROTOCOL, protocol);

