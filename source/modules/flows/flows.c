#include <stdio.h>
#include "modules.h"

#include "rte_ring.h"
#include "rte_mbuf.h"

typedef struct 
{
    struct rte_ring *pkts_ring;
} dapp_flows_ws_t;

static dapp_flows_ws_t flows_ws;

int dapp_flows_init(void *arg)
{
    int ret;

    /*
     * Wait port moudle initialized
     */
    ret = dapp_module_init_wait(1, DAPP_MODULE_PORT);

    if (DAPP_OK != ret) {
        printf("module %s wait fail! ERR : %d\n", dapp_modules_name_get_by_type(DAPP_MODULE_PORT), ret);
        return ret;
    }
    
    DAPP_TRACE("dapp flows init\n");

    dapp_module_init_status_set(DAPP_MODULE_FLOWS, DAPP_MODULE_INIT_START);

    flows_ws.pkts_ring = rte_ring_lookup("PKTS_RING");

    if (!flows_ws.pkts_ring) {
        printf("Can not find ring %s!\n", "PKTS_RING");
        return DAPP_FAIL;
    }

    dapp_module_init_status_set(DAPP_MODULE_FLOWS, DAPP_MODULE_INIT_OK);

    return DAPP_OK;
}

int dapp_flows_exec(void *arg)
{
    DAPP_TRACE("dapp flows exec\n");

    UINT32_T nmsg_deq;
    struct rte_mbuf *mbuff[8];

    while (dapp_module_running(DAPP_MODULE_FLOWS)) {

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

int dapp_flows_exit(void *arg)
{
    DAPP_TRACE("dapp flows exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FLOWS, flows, dapp_flows_init, dapp_flows_exec, dapp_flows_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FLOWS, flows);

