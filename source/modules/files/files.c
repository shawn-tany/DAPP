#include <stdio.h>
#include "modules.h"

typedef struct 
{
    struct rte_ring *files_ring;
} dapp_files_ws_t;

static dapp_files_ws_t files_ws;

static int dapp_files_init(void *arg)
{
    DAPP_TRACE("dapp files init\n");

    return DAPP_OK;
}

static int dapp_files_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp files exec\n");
    
    while (*running) {
        DAPP_TRACE("dapp files loop\n");
        sleep(3);
    }

    return DAPP_OK;
}

static int dapp_files_exit(void *arg)
{
    DAPP_TRACE("dapp files exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FILES, files, dapp_files_init, dapp_files_exec, dapp_files_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FILES, files);

