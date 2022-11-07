#include <stdio.h>
#include "modules.h"

typedef struct 
{
    
} dapp_control_ws_t;

static void dapp_control_loop(void)
{
    dapp_module_t *control_module = NULL;

    control_module = dapp_module_get_by_type(DAPP_MODULE_CONTROL);

    if (!control_module) {
        DAPP_TRACE("invalid dapp modules type %d\n", DAPP_MODULE_CONTROL);
        return ;
    }

    while (control_module->lcore.running) {
        DAPP_TRACE("dapp control loop\n");
        sleep(3);
    }
}

int dapp_control_init(void *arg)
{
    DAPP_TRACE("dapp control init\n");

    return DAPP_OK;
}

int dapp_control_exec(void *arg)
{
    DAPP_TRACE("dapp control exec\n");

    dapp_control_loop();

    return DAPP_OK;
}

int dapp_control_exit(void *arg)
{
    DAPP_TRACE("dapp control exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_CONTROL, contorl, dapp_control_init, dapp_control_exec, dapp_control_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_CONTROL, contorl);