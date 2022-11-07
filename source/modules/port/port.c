#include <stdio.h>
#include "modules.h"
#include "rte_ethdev.h"

typedef struct 
{
    
} dapp_port_ws_t;

static void dapp_port_loop(void)
{
    dapp_module_t *port_module = NULL;

    port_module = dapp_module_get_by_type(DAPP_MODULE_PORT);

    if (!port_module) {
        DAPP_TRACE("invalid dapp modules type %d\n", DAPP_MODULE_PORT);
        return ;
    }

    /*
     * Port loop
     */
    while (port_module->lcore.running) {
        DAPP_TRACE("dapp port loop\n");
        sleep(3);
    }
}

int dapp_port_init(void *arg)
{
    DAPP_TRACE("dapp port init\n");

    return DAPP_OK;
}

int dapp_port_exec(void *arg)
{
    DAPP_TRACE("dapp port exec\n");

    dapp_port_loop();

    return DAPP_OK;
}

int dapp_port_exit(void *arg)
{
    DAPP_TRACE("dapp port exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_PORT, port, dapp_port_init, dapp_port_exec, dapp_port_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_PORT, port);

