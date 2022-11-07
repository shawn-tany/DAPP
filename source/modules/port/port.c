#include <stdio.h>
#include "modules.h"

#include "rte_ethdev.h"
#include "rte_mbuf.h"

#define MAC_PRINT(mac) printf("%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5])

typedef struct 
{
    UINT16_T n_rx_ports;
    UINT16_T n_tx_ports;

    struct rte_mempool *rx_mempool;
} dapp_port_ws_t;

static struct rte_eth_conf dev_conf_default = {
    .rxmode = {
        .max_rx_pkt_len = RTE_ETHER_MAX_LEN,
    }
};

static dapp_port_ws_t port_ws;

static void dapp_port_loop(void)
{
    dapp_module_t *port_module = NULL;
    struct rte_mbuf *mbuff[8];

    port_module = dapp_module_get_by_type(DAPP_MODULE_PORT);

    if (!port_module) {
        DAPP_TRACE("invalid dapp modules type %d\n", DAPP_MODULE_PORT);
        return ;
    }

    /*
     * Port loop
     */
    while (port_module->lcore.running) {

        UINT16_T port_id;
        UINT16_T nb_rx;

        RTE_ETH_FOREACH_DEV(port_id) {
            nb_rx = rte_eth_rx_burst(port_id, 0, mbuff, 8);

            if (0 == nb_rx) {
                continue;
            }

            printf("rx pkts %d\n", nb_rx);
        }
    }
}

static int dapp_port_init(void *arg)
{
    DAPP_TRACE("dapp port init\n");

    int ret;

    UINT16_T n_ports = rte_eth_dev_count_avail();

    if (0 == n_ports) {
        printf("No ethernet device avalible!\n");
        return -1;
    }
    
    port_ws.n_rx_ports = 1;
    port_ws.n_tx_ports = 1;

    /*
     * Create rx mbuf pool
     */
    port_ws.rx_mempool = rte_pktmbuf_pool_create("DAPP_RX_MPOOL", 10240, 0, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
    if (!port_ws.rx_mempool) {
        printf("pktmbuf pool %s create fail!\n", "DAPP_RX_MPOOL");
        return -1;
    }

    /*
     * Initialize all port
     */
    UINT16_T port_id;
    
    RTE_ETH_FOREACH_DEV(port_id) {
        /*
         * Configure port
         */
        ret = rte_eth_dev_configure(port_id, port_ws.n_rx_ports, port_ws.n_tx_ports, &dev_conf_default);
        if (0 > ret) {
            printf("Can not configure device! ERR : %d\n", ret);
            return -1;
        }

        /*
         * Setup rx queue
         */
        ret = rte_eth_rx_queue_setup(port_id, 0, 128, rte_eth_dev_socket_id(port_id), NULL, port_ws.rx_mempool);
        if (0 > ret) {
            printf("Can not setup rx queue! ERR : %d\n", ret);
            return -1;
        }

        /*
         * Setup tx queue
         */
        ret = rte_eth_tx_queue_setup(port_id, 0, 128, rte_eth_dev_socket_id(port_id), NULL);
        if (0 > ret) {
            printf("Can not setup tx queue! ERR : %d\n", ret);
            return -1;
        }

        rte_eth_dev_start(port_id);

        /*
         * enable port promiscuous mode
         */
        ret = rte_eth_promiscuous_enable(port_id);
        if (0 > ret) {
            printf("Can not enable promiscuous mode\n");
            return -1;
        }

        /*
         * Show mac address of port
         */
        struct rte_ether_addr mac_addr;
        rte_eth_macaddr_get(port_id, &mac_addr);
        MAC_PRINT(mac_addr.addr_bytes);
    }

    return DAPP_OK;
}

static int dapp_port_exec(void *arg)
{
    DAPP_TRACE("dapp port exec\n");

    dapp_port_loop();

    return DAPP_OK;
}

static int dapp_port_exit(void *arg)
{
    DAPP_TRACE("dapp port exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_PORT, port, dapp_port_init, dapp_port_exec, dapp_port_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_PORT, port);

