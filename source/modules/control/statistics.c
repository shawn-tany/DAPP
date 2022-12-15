#include <stdlib.h>
#include <string.h>

#include "modules.h"
#include "statistics.h"

STATISTIC_T statistic[DAPP_MODULE_TYPE_NUM];

int statistics_clear(dapp_modules_type_t module)
{
    if (DAPP_MODULE_TYPE_NUM == module) {
        memset(statistic, 0, sizeof(statistic));
    }

    memset(&statistic[module], 0, sizeof(statistic[module]));

    return 0;
}

#define DAPP_STATISTIC_DATA_ADD(module, type, opra, step) \
    statistic[module].type.opra += step

