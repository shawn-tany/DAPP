#include <stdio.h>
#include "modules.h"

int dapp_control_init(void *arg)
{
    return DAPP_OK;
}

int dapp_control_exec(void *arg)
{
    return DAPP_OK;
}

int dapp_control_exit(void *arg)
{
    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_CONTROL, "contorl", dapp_control_init, dapp_control_exec, dapp_control_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_CONTROL, "contorl");