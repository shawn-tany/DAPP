#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "modules.h"
#include "statistics.h"

static pthread_mutex_t dapp_statistic_mutex;

STATISTIC_T statistic[DAPP_MODULE_TYPE_NUM];

int statistics_init()
{
    pthread_mutex_init(&dapp_statistic_mutex, NULL);
}

int statistics_clear(DAPP_MODULES_TYPE module)
{
    if (DAPP_MODULE_TYPE_NUM == module) {
        memset(statistic, 0, sizeof(statistic));
    }

    memset(&statistic[module], 0, sizeof(statistic[module]));

    return 0;
}

#define DAPP_STATISTIC_DATA_ADD(module, type, opra, step) \
    statistic[module].type.opra += step

