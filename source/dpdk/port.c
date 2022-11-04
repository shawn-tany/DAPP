#include <stdio.h>
#include "modules.h"

int dapp_port_init(void *arg)
{
    return DAPP_OK;
}

int dapp_port_exec(void *arg)
{
    return DAPP_OK;
}

int dapp_port_exit(void *arg)
{
    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_PORT, port, dapp_port_init, dapp_port_exec, dapp_port_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_PORT, port);

