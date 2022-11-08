#ifndef __DAPP_STATISTICS__
#define __DAPP_STATISTICS__

#define DAPP_CORE_NUM (64)

typedef struct {
    UINT64_T pkts;
    UINT64_T bytes;
} dapp_port_statistic_t;

typedef struct {
    dapp_port_statistic_t rx[DAPP_CORE_NUM];
    dapp_port_statistic_t tx[DAPP_CORE_NUM];
} dapp_port_statistics_t;

#endif