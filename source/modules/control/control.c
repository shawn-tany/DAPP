#include <stdio.h>
#include "modules.h"

typedef struct 
{
    
} dapp_control_ws_t;

static int dapp_control_init(void *arg)
{
    DAPP_TRACE("dapp control init\n");

    return DAPP_OK;
}

static int dapp_control_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp control exec\n");

    while (*running) {
        sleep(3);
    }

    return DAPP_OK;
}

static int dapp_control_exit(void *arg)
{
    DAPP_TRACE("dapp control exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_CONTROL, contorl, dapp_control_init, dapp_control_exec, dapp_control_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_CONTROL, contorl);