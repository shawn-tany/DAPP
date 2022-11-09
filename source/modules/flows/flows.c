#include <stdio.h>
#include "modules.h"

#include "rte_ring.h"
#include "rte_mbuf.h"

typedef struct 
{
    struct rte_ring *pkts_ring;
} dapp_flows_ws_t;

static dapp_flows_ws_t flows_ws;

static int dapp_flows_init(void *arg)
{
    int ret;
    
    DAPP_TRACE("dapp flows init\n");

    flows_ws.pkts_ring = rte_ring_lookup("PKTS_RING");

    if (!flows_ws.pkts_ring) {
        printf("Can not find ring %s!\n", "PKTS_RING");
        return DAPP_FAIL;
    }

    return DAPP_OK;
}

static int dapp_flows_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp flows exec\n");

    UINT32_T nmsg_deq;
    struct rte_mbuf *mbuff[8];

    while (*running) {

        nmsg_deq = rte_ring_dequeue_bulk(flows_ws.pkts_ring, (void **)mbuff, 8, NULL);

        if (0 == nmsg_deq) {
            continue;
        }

        DAPP_TRACE("dequeue %d form ring\n", nmsg_deq);

        /*
         * Release pkt mbuf
         */
        int i;
        for (i = 0; i < nmsg_deq; ++i) {
            rte_pktmbuf_free(mbuff[i]);
        }
    }

    return DAPP_OK;
}

static int dapp_flows_exit(void *arg)
{
    DAPP_TRACE("dapp flows exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FLOWS, flows, dapp_flows_init, dapp_flows_exec, dapp_flows_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FLOWS, flows);

