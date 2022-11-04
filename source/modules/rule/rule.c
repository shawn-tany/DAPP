#include <stdio.h>
#include "modules.h"

int dapp_rule_init(void *arg)
{
    return DAPP_OK;
}

int dapp_rule_exec(void *arg)
{
    return DAPP_OK;
}

int dapp_rule_exit(void *arg)
{
    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_RULE, rule, dapp_rule_init, dapp_rule_exec, dapp_rule_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_RULE, rule);

