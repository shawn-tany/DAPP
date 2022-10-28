#include "dpdk.h"
#include "rte_eal.h"

int dpdk_init(dpdk_eal_args_t *dpdk_eal_args)
{
    return rte_eal_init(dpdk_eal_args->argc, dpdk_eal_args->argv);
}

int dpdk_run()
{
    return 0;
}

int dpdk_exit()
{
    return 0;
}