#include <stdio.h>
#include "modules.h"

#include "rte_ring.h"

typedef struct 
{
    struct rte_ring *pkts_ring;
} dapp_flows_ws_t;

static dapp_flows_ws_t flows_ws;

static void dapp_flows_loop(void)
{
    UINT32_T nmsg_deq;
    struct rte_mbuf *mbuff[8];

    while (dapp_module_running(DAPP_MODULE_FLOWS)) {
        DAPP_TRACE("dapp flows loop\n");

        nmsg_deq = rte_ring_dequeue_bulk(flows_ws.pkts_ring, (void **)mbuff, 8, NULL);

        if (0 == nmsg_deq) {
            continue;
        }

        printf("dequeue %d form ring\n", nmsg_deq);
    }
}

int dapp_flows_init(void *arg)
{
    DAPP_TRACE("dapp flows init\n");

    flows_ws.pkts_ring = rte_ring_lookup("PKTS_RING");

    if (!flows_ws.pkts_ring) {
        printf("Can not find ring %s!\n", "PKTS_RING");
        return DAPP_FAIL;
    }

    return DAPP_OK;
}

int dapp_flows_exec(void *arg)
{
    DAPP_TRACE("dapp flows exec\n");

    dapp_flows_loop();

    return DAPP_OK;
}

int dapp_flows_exit(void *arg)
{
    DAPP_TRACE("dapp flows exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FLOWS, flows, dapp_flows_init, dapp_flows_exec, dapp_flows_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FLOWS, flows);

