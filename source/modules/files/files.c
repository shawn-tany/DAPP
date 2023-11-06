#include <stdio.h>
#include <string.h>
#include <time.h>
#include "modules.h"

#include "pcap.h"
#include "rte_ring.h"
#include "rte_mbuf.h"

#define MAX_FILE_SIZE (1024 * 1024 * 256)

#define DAPP_DAFAULT_CACHE_PATH "/home/dapp/cache"

#define DAPP_RING_OBJ_NUM (8)

typedef struct 
{
    UINT8_T  lcore_num;    
} dapp_files_conf_t;

typedef struct 
{
    struct rte_ring *flows_ring;
    dapp_files_conf_t config;
} dapp_files_ws_t;

static dapp_files_ws_t files_ws;

static pcap_dumper_t *dapp_pcap_open(void)
{
    pcap_dumper_t *dumper = NULL;

    char cachepath[256] = {0};
    char filename[300] = {0};
    
#ifdef DAPP_CACHE_PATH
    snprintf(cachepath, sizeof(cachepath), "%s", DAPP_CACHE_PATH);
#else
    snprintf(cachepath, sizeof(cachepath), "%s/%s", DAPP_DAFAULT_CACHE_PATH);
#endif
    
    DAPP_TRACE("dapp cache : %s\n", cachepath);

    snprintf(filename, sizeof(filename), "%s/%s", cachepath, "dapp.pcap");

    DAPP_TRACE("dapp pcap dumper open : %s\n", filename);

    dumper = pcap_dump_open(pcap_open_dead(DLT_EN10MB, 1600), filename);
    if (!dumper)
    {
        return NULL;
    }

    return dumper;
}

static int dapp_pcap_dump(pcap_dumper_t *dumper, UINT8_T *data, UINT32_T size)
{
    if (!dumper || !data) {
        return -1;
    }

    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct pcap_pkthdr hdr;
    hdr.ts.tv_sec = tv.tv_sec;
    hdr.ts.tv_usec = tv.tv_usec;
    hdr.caplen = size;
    hdr.len = size; 

    pcap_dump((UINT8_T *)dumper, &hdr, data);

    return 0;
}

static void dapp_pcap_close(pcap_dumper_t *dumper)
{
    if (dumper) {
        pcap_dump_close(dumper);
    }
}

static STATUS dapp_files_conf(void *args)
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
        if (!(conf_obj = json_object_get(obj_arr, "files"))) {
            return DAPP_ERR_JSON_CONF;
        }

        /*
         * Get the configuration through the json object
         */
        if (!(sub_obj = json_object_get(conf_obj, "thread_num"))) {
            return DAPP_ERR_JSON_CONF;
        }
        files_ws.config.lcore_num = json_integer_value(sub_obj);
    }

    dapp_module_lcore_init(DAPP_MODULE_FILES, files_ws.config.lcore_num);
    
    dapp_module_rely_init(DAPP_MODULE_FILES, DAPP_MODULE_UNI_INIT, 1, DAPP_MODULE_FLOWS);

    return DAPP_OK;
}


static STATUS dapp_files_init(void *arg)
{
    DAPP_TRACE("dapp files init\n");

    files_ws.flows_ring = rte_ring_lookup("FLOWS_RING");

    if (!files_ws.flows_ring) {
        printf("ERROR : Can not find ring %s!\n", "FLOWS_RING");
        return DAPP_FAIL;
    }

    return DAPP_OK;
}

static UINT64_T deq_count = 0;
static UINT64_T rls_count = 0;

static STATUS dapp_files_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp files exec\n");

    UINT32_T nmsg_deq;
    struct rte_mbuf *mbuff[8];
    char *pktbuf;
    pcap_dumper_t *dumper = NULL;

    /*
     * Open pcap file
     */  
    if (!(dumper = dapp_pcap_open())) {
        printf("ERROR : dumper is NULL\n");
        return -1;
    }
    
    while (*running) {

        nmsg_deq = rte_ring_dequeue_bulk(files_ws.flows_ring, (void **)mbuff, DAPP_RING_OBJ_NUM, NULL);

        if (0 == nmsg_deq) {
            continue;
        }

        deq_count += nmsg_deq;

        /*
         * Release pkt mbuf
         */
        int i;
        for (i = 0; i < nmsg_deq; ++i) {

            pktbuf = rte_pktmbuf_mtod(mbuff[i], char *);

            if (!mbuff[i]->data_len) {
                continue;
            }

            if (0 != dapp_pcap_dump(dumper, (uint8_t *)pktbuf, mbuff[i]->data_len)) {
                printf("ERROR : pcap dump fail\n");
                return -1;
            }
        
            rte_pktmbuf_free(mbuff[i]);

            rls_count++;
        }
    }

    dapp_pcap_close(dumper);

    return DAPP_OK;
}

static STATUS dapp_files_exit(void *arg)
{
    DAPP_TRACE("dapp files exit\n");

    DAPP_TRACE("module files dequeue form ring(%s)  count = %llu\n", files_ws.flows_ring->name, deq_count);
    DAPP_TRACE("module files free mbuf count = %llu\n", rls_count);

    return DAPP_OK;
}

static DAPP_MODULE_OPS files_ops = {
    .conf = dapp_files_conf,
    .init = dapp_files_init,
    .exec = dapp_files_exec,
    .exit = dapp_files_exit   
};

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FILES, files, &files_ops);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FILES, files);

