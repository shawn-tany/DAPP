#include <stdio.h>
#include <string.h>
#include "common.h"
#include "config.h"

#define STATIC_CONF_FILE "../install/config/static.conf"

int main(int argc, char *argv[])
{
    dapp_static_conf_t static_conf;
    memset(&static_conf, 0, sizeof(static_conf));

    if (DAPP_OK != dapp_static_conf_parse(&static_conf, STATIC_CONF_FILE)) {
        printf("dapp_static_conf_parse fail\n");
        return DAPP_ERR_JSON_CONF;
    }

    dapp_static_conf_dump(&static_conf);

    return DAPP_OK;
}