#include <stdio.h>
#include "modules.h"

typedef struct 
{
    
} dapp_restore_ws_t;

static void dapp_restore_loop(void)
{
    dapp_module_t *restore_module = NULL;

    restore_module = dapp_module_get_by_type(DAPP_MODULE_RESTORE);

    if (!restore_module) {
        DAPP_TRACE("invalid dapp modules type %d\n", DAPP_MODULE_RESTORE);
        return ;
    }

    while (restore_module->lcore.running) {
        DAPP_TRACE("dapp restore loop\n");
        sleep(3);
    }
}

int dapp_restore_init(void *arg)
{
    DAPP_TRACE("dapp restore init\n");

    return DAPP_OK;
}

int dapp_restore_exec(void *arg)
{
    DAPP_TRACE("dapp restore exec\n");

    dapp_restore_loop();

    return DAPP_OK;
}

int dapp_restore_exit(void *arg)
{
    DAPP_TRACE("dapp restore exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_RESTORE, restore, dapp_restore_init, dapp_restore_exec, dapp_restore_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_RESTORE, restore);

