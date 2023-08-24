#include <stdio.h>
#include "modules.h"
#include "port.h"

#include "rte_ethdev.h"
#include "rte_mbuf.h"
#include "rte_lcore.h"
#include "rte_mempool.h"
#include "rte_ring.h"
#include "rte_errno.h"

#define MAC_PRINT(mac) printf("%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5])
#define DAPP_RX_MBUFS (8)

typedef struct 
{
    UINT16_T n_rx_queue;
    UINT16_T n_tx_queue;

    struct rte_mempool *rx_mempool;
    struct rte_ring *pkts_ring;
} dapp_port_ws_t;

static struct rte_eth_conf dev_conf_default = {
    .rxmode = {
        .max_rx_pkt_len = RTE_ETHER_MAX_LEN,
    }
};

static dapp_port_ws_t port_ws;

static int dapp_port_init(void *arg)
{
    DAPP_TRACE("dapp port init start\n");

    int ret;

    DAPP_TRACE("initialize port module in lcore(%d)\n", rte_lcore_id());

    /*
     * Create pkts ring
     */
    port_ws.pkts_ring = rte_ring_create("PKTS_RING", 131072, rte_socket_id(), RING_F_SC_DEQ);

    if (!port_ws.pkts_ring) {
        printf("ERROR : ring %s create fail! ERR : %s\n", "PKTS_RING", rte_strerror(rte_errno));
        return DAPP_FAIL;
    }

    /*
     * Get available port number
     */
    UINT16_T n_ports = rte_eth_dev_count_avail();

    if (0 == n_ports) {
        printf("WARNING : No ethernet device avalible!\n");
        return DAPP_OK;
    }
    
    /*
     * Create rx mbuf pool
     */
    port_ws.rx_mempool = rte_pktmbuf_pool_create("DAPP_RX_MPOOL", 102400, RTE_CACHE_LINE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());

    if (!port_ws.rx_mempool) {
        printf("ERROR : pktmbuf pool %s create fail! ERR : %s\n", "DAPP_RX_MPOOL", rte_strerror(rte_errno));
        return DAPP_FAIL;
    }

    /*
     * Initialize all port
     */
    UINT16_T port_id;
    
    RTE_ETH_FOREACH_DEV(port_id) {

        /*
         * Get ethernet device info
         */
        struct rte_eth_dev_info dev_info;

        ret = rte_eth_dev_info_get(port_id, &dev_info);

        if (0 != ret) {
            printf("ERROR : Can not get ethernet device info! ERR : %s\n", rte_strerror(ret));
            return DAPP_FAIL;
        }

        /*
         * Set rx/tx queue number
         */
        port_ws.n_rx_queue = dapp_module_lcore_num_get_by_type(DAPP_MODULE_PORT);
        port_ws.n_tx_queue = dapp_module_lcore_num_get_by_type(DAPP_MODULE_PORT);

        port_ws.n_rx_queue = DAPP_MIN(port_ws.n_rx_queue, dev_info.max_rx_queues);
        port_ws.n_tx_queue = DAPP_MIN(port_ws.n_tx_queue, dev_info.max_tx_queues);
        
        /*
         * Configure port
         */
        ret = rte_eth_dev_configure(port_id, port_ws.n_rx_queue, port_ws.n_tx_queue, &dev_conf_default);

        if (0 != ret) {
            printf("ERROR : Can not configure device! ERR : %s\n", rte_strerror(ret));
            return DAPP_FAIL;
        }
        
        /*
         * Setup rx queue
         */
        int queue_id;
        for (queue_id = 0; queue_id < port_ws.n_rx_queue; ++queue_id) {
            ret = rte_eth_rx_queue_setup(port_id, queue_id, 128, rte_eth_dev_socket_id(port_id), NULL, port_ws.rx_mempool);

            if (0 != ret) {
                printf("ERROR : Can not setup rx queue(%d)! ERR : %s\n", queue_id, rte_strerror(ret));
                return DAPP_FAIL;
            }
        }

        /*
         * Setup tx queue
         */
        for (queue_id = 0; queue_id < port_ws.n_tx_queue; ++queue_id) {
            ret = rte_eth_tx_queue_setup(port_id, queue_id, 128, rte_eth_dev_socket_id(port_id), NULL);

            if (0 != ret) {
                printf("ERROR : Can not setup tx queue! ERR : %s\n", rte_strerror(ret));
                return DAPP_FAIL;
            }
        }

        /*
         * Start the ethernet device
         */
        rte_eth_dev_start(port_id);

        /*
         * enable port promiscuous mode
         */
        ret = rte_eth_promiscuous_enable(port_id);

        if (0 != ret) {
            printf("ERROR : Can not enable promiscuous mode! ERR : %s\n", rte_strerror(ret));
            return DAPP_FAIL;
        }

        /*
         * Show mac address of port
         */
        struct rte_ether_addr mac_addr;
        rte_eth_macaddr_get(port_id, &mac_addr);
        MAC_PRINT(mac_addr.addr_bytes);
    }
    
    DAPP_TRACE("dapp port init end\n");

    return DAPP_OK;
}

static int dapp_port_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp port exec start\n");

    int ret;

    struct rte_mbuf *mbuff[DAPP_RX_MBUFS];
    
    UINT16_T lcore_id = rte_lcore_id();

    if (!port_ws.n_rx_queue && !port_ws.n_tx_queue) {
        return DAPP_OK;
    }

    UINT16_T rx_queue_id = lcore_id % port_ws.n_rx_queue;
    UINT16_T tx_queue_id = lcore_id % port_ws.n_tx_queue;
    
    /*
     * Port loop
     */
    while (*running) {

        UINT16_T port_id;
        UINT32_T npkts_rx;
        UINT32_T nmsg_enq;

        RTE_ETH_FOREACH_DEV(port_id) {
        
            /*
             * RX pkts
             */
            npkts_rx = rte_eth_rx_burst(port_id, rx_queue_id, mbuff, DAPP_RX_MBUFS);

            if (0 == npkts_rx) {
                
                struct rte_eth_stats stats;
                
                if (0 > (ret = rte_eth_stats_get(port_id, &stats))) {
                    printf("ERROR : Can not get ethernet device info! ERR : %d\n", ret);
                    return -1;
                }

                if (stats.imissed || stats.ierrors || stats.rx_nombuf) {
                    printf("WARNING : dpdk rx fail! imissed = %lu ierrors = %lu rx_nombuf = %lu\n", stats.imissed, stats.ierrors, stats.rx_nombuf);
                }

                if (!rte_mempool_avail_count(port_ws.rx_mempool)) {
                    printf("WARNING : none avail in rx pktmbuf pool!\n");
                }
                
                continue;
            }

            /*
             * forward ring
             */
            nmsg_enq = rte_ring_enqueue_bulk(port_ws.pkts_ring, (void **)mbuff, npkts_rx, NULL);
            
            if (0 == nmsg_enq) {
                printf("ERROR : Can not enqueue bulk to ring (%s)\n", "PKTS_RING");

                /*
                 * Release pkt mbuf
                 */
                int i;
                for (i = 0; i < nmsg_enq; ++i) {
                    rte_pktmbuf_free(mbuff[i]);
                }
            
                return DAPP_OK;
            }
        }
    }

    DAPP_TRACE("dapp port exec end\n");

    UNUSED(tx_queue_id);

    return DAPP_OK;
}

static int dapp_port_exit(void *arg)
{
    DAPP_TRACE("dapp port exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_PORT, port, dapp_port_init, dapp_port_exec, dapp_port_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_PORT, port);

