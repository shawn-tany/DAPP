#include <stdio.h>
#include "config.h"

static STATUS dapp_static_conf_paser(dapp_static_conf_t *static_conf)
{
    if (!static_conf) {
        return DAPP_ERR_PARAM;
    }

    return DAPP_OK;
}

static STATUS dapp_dynamic_conf_paser(dapp_dynamic_conf_t *dynamic_conf)
{
    if (!dynamic_conf) {
        return DAPP_ERR_PARAM;
    }

    return DAPP_OK;
}

STATUS dapp_conf_paser(dapp_conf_t *config)
{
    if (!config) {
        return DAPP_ERR_PARAM;
    }

    return DAPP_OK;
}