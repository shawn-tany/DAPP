#include "modules.h"

static dapp_modules_t modules = {
    {
        {"control",             MODULES_CTRL},
        {"port transform",      MODULES_PORT_TRANS},
        {"protocol identify",   MODULES_PROTO_IDENTI},
        {"rule match",          MODULES_RULE_MATCH},
        {"flow isotonic",       MODULES_FLOW_ISOTONIC},
        {"data restore",        MODULES_DATA_RESTORE},
        {"file identify",       MODULES_FILE_IDENTI}
    }
};

char *dapp_modules_name_get(dapp_modules_type_t type)
{
    int i = 0;

    for (i = 0; i < MODULES_ITEM(modules.modules); ++i) {
        if (type == modules.modules[i].type) {
            return modules.modules[i].name;
        }
    }

    return "unknow";
}