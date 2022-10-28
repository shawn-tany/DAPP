#ifndef DAPP_DPDK
#define DAPP_DPDK

#define DPDK_MAX_EAL_ARGC (32)
#define DPDK_EAL_ARGV_SIZE (2048)

typedef struct
{
    int argc;
    char *argv[DPDK_MAX_EAL_ARGC];
} dpdk_eal_args_t;

unsigned dpdk_lcore_id(void);

int dpdk_init(dpdk_eal_args_t *dpdk_eal_args);

int dpdk_run(int (*slave_loop)(void *), void *slave_arg, int (*master_loop)(void *), void *master_arg);

void dpdk_exit(void);

#endif