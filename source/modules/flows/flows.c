#include <stdio.h>
#include "modules.h"

#include "rte_ring.h"
#include "rte_mbuf.h"

typedef struct 
{
    struct rte_ring *pkts_ring;
    struct rte_ring *flows_ring;
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

    /*
     * Create flows ring
     */
    flows_ws.flows_ring = rte_ring_create("FLOWS_RING", 131072, rte_socket_id(), RING_F_SC_DEQ);

    if (!flows_ws.flows_ring) {
        printf("ring %s create fail!\n", "FLOWS_RING");
        return DAPP_FAIL;
    }

    return DAPP_OK;
}

static UINT64_T deq_count = 0;
static UINT64_T enq_count = 0;
static UINT64_T rls_count = 0;

static int dapp_flows_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp flows exec\n");

    UINT32_T nmsg_deq;
    UINT32_T nmsg_enq;
    struct rte_mbuf *mbuff[8];

    while (*running) {

        nmsg_deq = rte_ring_dequeue_bulk(flows_ws.pkts_ring, (void **)mbuff, 8, NULL);

        if (0 == nmsg_deq) {
            continue;
        }

        deq_count += nmsg_deq;

        nmsg_enq = rte_ring_enqueue_bulk(flows_ws.flows_ring, (void **)mbuff, nmsg_deq, NULL);

        if (0 == nmsg_enq) {

            printf("Can not enqueue bulk to ring (%s)\n", "FLOWS_RING");

            /*
             * Release pkt mbuf
             */
            int i;
            for (i = 0; i < nmsg_deq; ++i) {
                rte_pktmbuf_free(mbuff[i]);
                rls_count++;
            }
        }

        enq_count += nmsg_enq;
    }

    return DAPP_OK;
}

static int dapp_flows_exit(void *arg)
{
    DAPP_TRACE("dapp flows exit\n");

    DAPP_TRACE("module flows dequeue form ring(%s)  count = %llu\n", flows_ws.pkts_ring->name, deq_count);
    DAPP_TRACE("module flows enqueue form ring(%s)  count = %llu\n", flows_ws.flows_ring->name, enq_count);
    DAPP_TRACE("module flows free mbuf count = %llu\n", rls_count);

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FLOWS, flows, dapp_flows_init, dapp_flows_exec, dapp_flows_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FLOWS, flows);

