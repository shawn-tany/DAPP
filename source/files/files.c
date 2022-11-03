#include <stdio.h>
#include "modules.h"

int dapp_files_init(void *arg)
{
    return DAPP_OK;
}

int dapp_files_exec(void *arg)
{
    return DAPP_OK;
}

int dapp_files_exit(void *arg)
{
    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FILES, "files", dapp_files_init, dapp_files_exec, dapp_files_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FILES, "files");

