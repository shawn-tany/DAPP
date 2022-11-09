#include <stdio.h>
#include "modules.h"

typedef struct 
{
    
} dapp_restore_ws_t;

static int dapp_restore_init(void *arg)
{
    DAPP_TRACE("dapp restore init\n");

    return DAPP_OK;
}

static int dapp_restore_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp restore exec\n");

    while (*running) {
        DAPP_TRACE("dapp restore loop\n");
        sleep(3);
    }

    return DAPP_OK;
}

static int dapp_restore_exit(void *arg)
{
    DAPP_TRACE("dapp restore exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_RESTORE, restore, dapp_restore_init, dapp_restore_exec, dapp_restore_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_RESTORE, restore);

