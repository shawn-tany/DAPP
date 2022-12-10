#include <stdio.h>
#include <string.h>
#include <time.h>
#include "modules.h"

#include "pcap.h"
#include "rte_ring.h"
#include "rte_mbuf.h"

#define MAX_FILE_SIZE (1024 * 1024 * 256)

#define DAPP_CACHE_PATH "/etc/DAPP/cache"

#define DAPP_RING_OBJ_NUM (8)

typedef struct 
{
    struct rte_ring *flows_ring;
} dapp_files_ws_t;

static dapp_files_ws_t files_ws;

static pcap_dumper_t *dapp_pcap_open(void)
{
    pcap_dumper_t *dumper = NULL;

    char filename[256] = {0};
    
    snprintf(filename, sizeof(filename), "%s/dapp.pcap", DAPP_CACHE_PATH);

    dumper = pcap_dump_open(pcap_open_dead(DLT_EN10MB, 1600), filename);
    if (NULL == dumper)
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

static int dapp_files_init(void *arg)
{
    DAPP_TRACE("dapp files init\n");

    files_ws.flows_ring = rte_ring_lookup("FLOWS_RING");

    if (!files_ws.flows_ring) {
        printf("Can not find ring %s!\n", "FLOWS_RING");
        return DAPP_FAIL;
    }

    return DAPP_OK;
}

static UINT64_T deq_count = 0;
static UINT64_T enq_count = 0;
static UINT64_T rls_count = 0;

static int dapp_files_exec(UINT8_T *running, void *arg)
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
        printf("dumper is NULL\n");
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

            if (0 != dapp_pcap_dump(dumper, pktbuf, mbuff[i]->data_len)) {
                printf("pcap dump fail\n");
                return -1;
            }
        
            rte_pktmbuf_free(mbuff[i]);

            rls_count++;
        }
    }

    dapp_pcap_close(dumper);

    return DAPP_OK;
}

static int dapp_files_exit(void *arg)
{
    DAPP_TRACE("dapp files exit\n");

    DAPP_TRACE("module files dequeue form ring(%s)  count = %llu\n", files_ws.flows_ring->name, deq_count);
    DAPP_TRACE("module files enqueue form ring(%s)  count = %llu\n", files_ws.flows_ring->name, enq_count);
    DAPP_TRACE("module files free mbuf count = %llu\n", rls_count);

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FILES, files, dapp_files_init, dapp_files_exec, dapp_files_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FILES, files);

