#include <stdio.h>
#include "modules.h"

typedef struct 
{
    
} dapp_rule_ws_t;

static int dapp_rule_init(void *arg)
{
    DAPP_TRACE("dapp rule init\n");

    return DAPP_OK;
}

static int dapp_rule_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp rule exec\n");

    while (*running) {
        sleep(3);
    }

    return DAPP_OK;
}

static int dapp_rule_exit(void *arg)
{
    DAPP_TRACE("dapp rule exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_RULE, rule, dapp_rule_init, dapp_rule_exec, dapp_rule_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_RULE, rule);

