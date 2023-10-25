#ifndef DAPP_CONFIG
#define DAPP_CONFIG

#include "jansson.h"
#include "modules.h"

STATUS dapp_conf_init(const char *file_name);

void dapp_conf_uinit(void);

json_t *dapp_conf_root_get(void);

#endif
