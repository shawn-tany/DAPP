#include <stdio.h>
#include "modules.h"

typedef struct 
{
    
} dapp_flows_ws_t;

static void dapp_flows_loop(void)
{
    dapp_module_t *flows_module = NULL;

    flows_module = dapp_module_get_by_type(DAPP_MODULE_FLOWS);

    if (!flows_module) {
        DAPP_TRACE("invalid dapp modules type %d\n", DAPP_MODULE_FLOWS);
        return ;
    }

    while (flows_module->lcore.running) {
        DAPP_TRACE("dapp flows loop\n");
        sleep(3);
    }
}

int dapp_flows_init(void *arg)
{
    DAPP_TRACE("dapp flows init\n");

    return DAPP_OK;
}

int dapp_flows_exec(void *arg)
{
    DAPP_TRACE("dapp flows exec\n");

    dapp_flows_loop();

    return DAPP_OK;
}

int dapp_flows_exit(void *arg)
{
    DAPP_TRACE("dapp flows exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FLOWS, flows, dapp_flows_init, dapp_flows_exec, dapp_flows_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FLOWS, flows);

