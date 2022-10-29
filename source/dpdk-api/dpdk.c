#include "dpdk.h"
#include "rte_eal.h"
#include "rte_debug.h"
#include "rte_lcore.h"

unsigned dpdk_lcore_id(void)
{
    return rte_lcore_id();
}

int dpdk_init(void *arg, eal_args_parse_callback args_parse)
{
    if (!arg || !args_parse) {
        return -1;
    }

    int argc = 0;
    char *argv[DPDK_ARG_NUM] = {0};

    if (0 > args_parse(&argc, argv, arg)) {
        rte_panic("Cannot parse eal param\n");
    }
    
    if (0 > rte_eal_init(argc, argv)) {
        rte_panic("Cannot init eal\n");
        return -1;
    }

    return 0;
}

int dpdk_run(int (*slave_loop)(void *), void *slave_arg, int (*master_loop)(void *), void *master_arg)
{
    unsigned lcore_id;

    if (!slave_loop) {
        return -1;
    }

    /*
     * call slave_loop() on every slave lcore
     */
    RTE_LCORE_FOREACH_SLAVE(lcore_id) {
        rte_eal_remote_launch(slave_loop, slave_arg, lcore_id);
    }

    /* 
     * call it on master lcore too
     */
    if (!master_loop) {
        slave_loop(slave_arg);
    } else {
        master_loop(master_arg);
    }
    
    return 0;
}

void dpdk_exit(void)
{
    /*
     * Wait for all child threads to end
     */
    rte_eal_mp_wait_lcore();

    /*
     * clean up the EAL
     */
    rte_eal_cleanup();
}