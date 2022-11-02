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

static dapp_module_t control_modules = {
    .reg.name = "control";

    .reg.init = dapp_control_init;
    .reg.exec = dapp_control_exec;
    .reg.exit = dapp_control_exit;
};

__attribute__((constructor)) dapp_module_reg(DAPP_MODULES_CTRL, &control_modules);

__attribute__((destructor)) dapp_module_unreg(DAPP_MODULES_CTRL);