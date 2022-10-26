#ifndef DAPP_DPDK
#define DAPP_DPDK

#define DPDK_MAX_EAL_ARGC (32)
#define DPDK_EAL_ARGV_SIZE (2048)

typedef struct
{
	int argc;
	char argv[DPDK_MAX_EAL_ARGC][DPDK_EAL_ARGV_SIZE];
} dpdk_eal_args_t;

#endif