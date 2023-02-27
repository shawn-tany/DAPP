#include <stdio.h>
#include "modules.h"

typedef struct 
{
    
} dapp_protocol_ws_t;

static int dapp_protocol_init(void *arg)
{
    DAPP_TRACE("dapp protocol init\n");

    return DAPP_OK;
}

static int dapp_protocol_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp protocol exec\n");

    while (*running) {
        sleep(3);
    }

    DAPP_TRACE("dapp protocol exec end\n");

    return DAPP_OK;
}

static int dapp_protocol_exit(void *arg)
{
    DAPP_TRACE("dapp protocol exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_PROTOCOL, protocol, dapp_protocol_init, dapp_protocol_exec, dapp_protocol_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_PROTOCOL, protocol);

