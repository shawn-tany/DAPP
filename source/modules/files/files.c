#include <stdio.h>
#include <string.h>
#include <time.h>
#include "modules.h"

#include "pcap.h"
#include "rte_ring.h"
#include "rte_mbuf.h"

#define MAX_FILE_SIZE (1024 * 1024 * 256)

#define DAPP_CACHE_PATH "/etc/DAPP/cache"

typedef struct 
{
    struct rte_ring *flows_ring;
} dapp_files_ws_t;

static dapp_files_ws_t files_ws;

static pcap_dumper_t *dumper = NULL;

static void dapp_pcap_open(void)
{
    char filename[256] = {0};
    
    if (NULL == dumper)
    {    
        snprintf(filename, sizeof(filename), "%s/dapp.pcap", DAPP_CACHE_PATH);

        dumper = pcap_dump_open(pcap_open_dead(DLT_EN10MB, 1600), filename);
        if (NULL == dumper)
        {
            printf("dumper is NULL\n");
            return;
        }
    }
}

static void dapp_pcap_dump(UINT8_T *data, UINT32_T size)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct pcap_pkthdr hdr;
    hdr.ts.tv_sec = tv.tv_sec;
    hdr.ts.tv_usec = tv.tv_usec;
    hdr.caplen = size;
    hdr.len = size; 

    pcap_dump((UINT8_T *)dumper, &hdr, data);
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

static int dapp_files_exec(UINT8_T *running, void *arg)
{
    DAPP_TRACE("dapp files exec\n");

    UINT32_T nmsg_deq;
    struct rte_mbuf *mbuff[8];

    /*
     * Open pcap file
     */  
    dapp_pcap_open();
        
    while (*running) {

        nmsg_deq = rte_ring_dequeue_bulk(files_ws.flows_ring, (void **)mbuff, 8, NULL);

        if (0 == nmsg_deq) {
            continue;
        }

        DAPP_TRACE("dequeue %d form ring(%s)\n", nmsg_deq, "FLOWS_RING");

        /*
         * Release pkt mbuf
         */
        int i;
        for (i = 0; i < nmsg_deq; ++i) {

            char *pktbuf = rte_pktmbuf_mtod(mbuff[i], char *);

            dapp_pcap_dump(pktbuf, mbuff[i]->data_len);
        
            rte_pktmbuf_free(mbuff[i]);
        }
    }

    return DAPP_OK;
}

static int dapp_files_exit(void *arg)
{
    DAPP_TRACE("dapp files exit\n");

    return DAPP_OK;
}

DAPP_MODULE_REG_CONSTRUCTOR(DAPP_MODULE_FILES, files, dapp_files_init, dapp_files_exec, dapp_files_exit);

DAPP_MODULE_UNREG_DESTRUCTOR(DAPP_MODULE_FILES, files);

