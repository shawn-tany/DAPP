#include <stdio.h>
#include "modules.h"
#include "flows.h"

#include "rte_ring.h"
#include "rte_mbuf.h"

#define DAPP_RING_OBJ_NUM (8)

typedef struct 
{
    UINT8_T  lcore_num;    
} dapp_flows_conf_t;

typedef struct 
{
    struct rte_ring *pkts_ring;
    struct rte_ring *flows_ring;

    dapp_flows_conf_t config;
} dapp_flows_ws_t;

static dapp_flows_ws_t flows_ws;

static STATUS dapp_flows_conf(void *args)
{
    json_t *root = dapp_conf_root_get();    
    json_t *obj_arr = NULL;
    json_t *conf_obj = NULL;    
    json_t *sub_obj = NULL;    
    UINT32_T i = 0;
    UINT32_T array_size = 0;

    array_size = json_array_size(root);

    for (i = 0; i < array_size; ++i) {
        if (!(obj_arr = json_array_get(root, i))) {
            return DAPP_ERR_JSON_FMT;
        }

        /* port parse */
        if (!(conf_obj = json_object_get(obj_arr, "flows"))) {
            return DAPP_ERR_JSON_CONF;
        }

        /*
         * Get the configuration through the json object
         */
        if (!(sub_obj = json_object_get(conf_obj, "thread_num"))) {
            return DAPP_ERR_JSON_CONF;
        }
        flows_ws.config.lcore_num = json_integer_value(sub_obj);
    }

    dapp_module_lcore_init(DAPP_MODULE_FLOWS, flows_ws.config.lcore_num);
    
    dapp_module_rely_init(DAPP_MODULE_FLOWS, DAPP_MODULE_UNI_INIT, 1, DAPP_MODULE_PORT);

    return DAPP_OK;
}


static STATUS dapp_flows_init(void *arg)
{
    DAPP_TRACE("dapp flows init\n");

    flows_ws.pkts_ring = rte_ring_lookup("PKTS_RING");

    if (!flows_ws.pkts_ring) {
        printf("ERROR : Can not find ring %s!\n", "PKTS_RING");
        return DAPP_FAIL;
    }

    /*
     * Create flows ring
     */
    flows_ws.flows_ring = rte_ring_create("FLOWS_RING", 131072, rte_socket_id(), RING_F_SC_DEQ);

    if (!flows_ws.flows_ring) {
        printf("ERROR : ring %s create fail!\n", "FLOWS_RING");
        return DAPP_FAIL;
    }

    return DAPP_OK;
}

static UINT64_T deq_count = 0;
static UINT64_T enq_count = 0;
static UINT64_T rls_count = 0;

static STATUS dapp_flows_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp flows exec\n");

    UINT32_T nmsg_deq;
    UINT32_T nmsg_enq;
    struct rte_mbuf *mbuff[8];

    while (*running) {

        nmsg_deq = rte_ring_dequeue_bulk(flows_ws.pkts_ring, (void **)mbuff, DAPP_RING_OBJ_NUM, NULL);

        if (0 == nmsg_deq) {
            continue;
        }

        /* business */

        

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

static STATUS dapp_flows_exit(void *arg)
{
    DAPP_TRACE("dapp flows exit\n");

    DAPP_TRACE("module flows dequeue form ring(%s)  count = %llu\n", flows_ws.pkts_ring->name, deq_count);
    DAPP_TRACE("module flows enqueue form ring(%s)  count = %llu\n", flows_ws.flows_ring->name, enq_count);
    DAPP_TRACE("module flows free mbuf count = %llu\n", rls_count);

    return DAPP_OK;
}

static DAPP_MODULE_OPS flows_ops = {
    .conf = dapp_flows_conf,
    .init = dapp_flows_init,
    .exec = dapp_flows_exec,
    .exit = dapp_flows_exit   
};

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FLOWS, flows, &flows_ops);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FLOWS, flows);

