#include <stdio.h>
#include "modules.h"

typedef struct 
{
    
} dapp_rule_ws_t;

static void dapp_rule_loop(void)
{
    dapp_module_t *rule_module = NULL;

    rule_module = dapp_module_get_by_type(DAPP_MODULE_RULE);

    if (!rule_module) {
        DAPP_TRACE("invalid dapp modules type %d\n", DAPP_MODULE_RULE);
        return ;
    }

    while (rule_module->lcore.running) {

        sleep(3);
    }
}

int dapp_rule_init(void *arg)
{
    DAPP_TRACE("dapp rule init\n");

    return DAPP_OK;
}

int dapp_rule_exec(void *arg)
{
    DAPP_TRACE("dapp rule exec\n");

    dapp_rule_loop();

    return DAPP_OK;
}

int dapp_rule_exit(void *arg)
{
    DAPP_TRACE("dapp rule exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_RULE, rule, dapp_rule_init, dapp_rule_exec, dapp_rule_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_RULE, rule);

