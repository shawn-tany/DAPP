#include <stdio.h>
#include "modules.h"

typedef struct 
{
    
} dapp_files_ws_t;

static void dapp_files_loop(void)
{
    dapp_module_t *files_module = NULL;

    files_module = dapp_module_get_by_type(DAPP_MODULE_FILES);

    if (!files_module) {
        DAPP_TRACE("invalid dapp modules type %d\n", DAPP_MODULE_FILES);
        return ;
    }

    while (files_module->lcore.running) {
        DAPP_TRACE("dapp files loop\n");
        sleep(3);
    }
}

int dapp_files_init(void *arg)
{
    DAPP_TRACE("dapp files init\n");

    return DAPP_OK;
}

int dapp_files_exec(void *arg)
{
    DAPP_TRACE("dapp files exec\n");

    dapp_files_loop();

    return DAPP_OK;
}

int dapp_files_exit(void *arg)
{
    DAPP_TRACE("dapp files exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FILES, files, dapp_files_init, dapp_files_exec, dapp_files_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FILES, files);

