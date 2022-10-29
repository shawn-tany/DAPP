#ifndef DAPP_DPDK
#define DAPP_DPDK

#define DPDK_ARG_NUM  (32)
#define DPDK_ARG_SIZE (512)

typedef int (*eal_args_parse_callback)(int *, char **, void *);

unsigned dpdk_lcore_id(void);

int dpdk_init(void *arg, eal_args_parse_callback args_parse);

int dpdk_run(int (*slave_loop)(void *), void *slave_arg, int (*master_loop)(void *), void *master_arg);

void dpdk_exit(void);

#endif