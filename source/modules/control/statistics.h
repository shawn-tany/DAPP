#ifndef __DAPP_STATISTICS__
#define __DAPP_STATISTICS__

#include "common.h"

#define DAPP_CORE_NUM (64)

typedef struct {
    UINT64_T pkts;
    UINT64_T bytes;
} STATISTIC_DATA_T;

typedef struct {
    STATISTIC_DATA_T rx[DAPP_CORE_NUM];
    STATISTIC_DATA_T tx[DAPP_CORE_NUM];
    STATISTIC_DATA_T free[DAPP_CORE_NUM];
} STATISTIC_T;

#endif
