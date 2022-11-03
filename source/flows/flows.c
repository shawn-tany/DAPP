#include <stdio.h>
#include "modules.h"

int dapp_flows_init(void *arg)
{
    return DAPP_OK;
}

int dapp_flows_exec(void *arg)
{
    return DAPP_OK;
}

int dapp_flows_exit(void *arg)
{
    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FLOWS, "flows", dapp_flows_init, dapp_flows_exec, dapp_flows_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FLOWS, "flows");

