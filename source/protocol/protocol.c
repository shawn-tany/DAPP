#include <stdio.h>
#include "modules.h"

int dapp_protocol_init(void *arg)
{
    return DAPP_OK;
}

int dapp_protocol_exec(void *arg)
{
    return DAPP_OK;
}

int dapp_protocol_exit(void *arg)
{
    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_PROTOCOL, protocol, dapp_protocol_init, dapp_protocol_exec, dapp_protocol_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_PROTOCOL, protocol);

