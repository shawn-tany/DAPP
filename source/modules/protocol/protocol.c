#include <stdio.h>
#include "modules.h"

typedef struct 
{
    
} dapp_protocol_ws_t;

int dapp_protocol_init(void *arg)
{
    int ret;

    /*
     * Wait port moudle initialized
     */
    ret = dapp_module_init_wait(2, DAPP_MODULE_PORT, DAPP_MODULE_FLOWS);

    if (DAPP_OK != ret) {
        printf("module %s wait fail! ERR : %d\n", dapp_modules_name_get_by_type(DAPP_MODULE_PROTOCOL), ret);
        return ret;
    }

    DAPP_TRACE("dapp protocol init\n");

    return DAPP_OK;
}

int dapp_protocol_exec(void *arg)
{
    DAPP_TRACE("dapp protocol exec\n");

    while (dapp_module_running(DAPP_MODULE_PROTOCOL)) {
        
        sleep(3);
    }

    return DAPP_OK;
}

int dapp_protocol_exit(void *arg)
{
    DAPP_TRACE("dapp protocol exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_PROTOCOL, protocol, dapp_protocol_init, dapp_protocol_exec, dapp_protocol_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_PROTOCOL, protocol);

