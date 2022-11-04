#include <stdio.h>
#include "modules.h"

int dapp_restore_init(void *arg)
{
    return DAPP_OK;
}

int dapp_restore_exec(void *arg)
{
    return DAPP_OK;
}

int dapp_restore_exit(void *arg)
{
    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_RESTORE, restore, dapp_restore_init, dapp_restore_exec, dapp_restore_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_RESTORE, restore);

