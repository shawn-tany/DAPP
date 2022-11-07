#include <stdio.h>
#include "modules.h"

typedef struct 
{
    
} dapp_protocol_ws_t;

static void dapp_protocol_loop(void)
{
    dapp_module_t *protocol_module = NULL;

    protocol_module = dapp_module_get_by_type(DAPP_MODULE_PROTOCOL);

    if (!protocol_module) {
        DAPP_TRACE("invalid dapp modules type %d\n", DAPP_MODULE_PROTOCOL);
        return ;
    }

    while (protocol_module->lcore.running) {
        DAPP_TRACE("dapp protocol loop\n");
        sleep(3);
    }
}

int dapp_protocol_init(void *arg)
{
    DAPP_TRACE("dapp protocol init\n");

    return DAPP_OK;
}

int dapp_protocol_exec(void *arg)
{
    DAPP_TRACE("dapp protocol exec\n");

    dapp_protocol_loop();

    return DAPP_OK;
}

int dapp_protocol_exit(void *arg)
{
    DAPP_TRACE("dapp protocol exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_PROTOCOL, protocol, dapp_protocol_init, dapp_protocol_exec, dapp_protocol_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_PROTOCOL, protocol);

