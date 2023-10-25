#include <stdio.h>
#include "modules.h"

typedef struct 
{
    UINT8_T  lcore_num;    
} dapp_control_conf_t;

typedef struct 
{
    dapp_control_conf_t config;
} dapp_control_ws_t;

static dapp_control_ws_t control_ws;

static STATUS dapp_control_conf(void *args)
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
        if (!(conf_obj = json_object_get(obj_arr, "control"))) {
            return DAPP_ERR_JSON_CONF;
        }

        /*
         * Get the configuration through the json object
         */
        if (!(sub_obj = json_object_get(conf_obj, "thread_num"))) {
            return DAPP_ERR_JSON_CONF;
        }
        control_ws.config.lcore_num = json_integer_value(sub_obj);
    }

    dapp_module_lcore_init(DAPP_MODULE_CONTROL, control_ws.config.lcore_num);

    return DAPP_OK;
}

static STATUS dapp_control_init(void *arg)
{
    DAPP_TRACE("dapp control init\n");

    return DAPP_OK;
}

static STATUS dapp_control_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp control exec\n");

    /* io多路复用，命令行支持线程 */

    /* 支持线程 */

    while (*running) {    
        sleep(3);
    }

    return DAPP_OK;
}

static STATUS dapp_control_exit(void *arg)
{
    DAPP_TRACE("dapp control exit\n");

    return DAPP_OK;
}

static DAPP_MODULE_OPS control_ops = {
    .conf = dapp_control_conf,
    .init = dapp_control_init,
    .exec = dapp_control_exec,
    .exit = dapp_control_exit   
};

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_CONTROL, contorl, &control_ops);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_CONTROL, contorl);