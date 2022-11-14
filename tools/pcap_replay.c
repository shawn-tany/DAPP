#include <stdio.h>
#include <getopt.h>

#include "pcap.h"

#include "rte_lcore.h"
#include "rte_mbuf.h"
#include "rte_mempool.h"
#include "rte_ring.h"
#include "rte_errno.h"

typedef unsigned char UINT8_T;
typedef unsigned short UINT16_T;

static struct {
    char pcap_path[256];
    int is_dir;
    int core_num;
    struct rte_ring *pkts_ring;
    struct rte_mempool *replay_pool;
} pcap_replay_ctx;

static int pcap_dir_replay()
{
    return 0;
}

static int pcap_file_replay()
{
    pcap_t *p = NULL;
    char err_buf[PCAP_ERRBUF_SIZE] = {0};
    struct rte_mbuf *mbuf[8] = {0};
    struct pcap_pkthdr pkthdr;
    int msg_count = 0;
    int file_end = 0;

    /*
     * open pcap file
     */
    p = pcap_open_offline(pcap_replay_ctx.pcap_path, err_buf);

    if (!p) {
        printf("Faild to open pcap file %s! ERR : %s\n", pcap_replay_ctx.pcap_path, err_buf);
        return -1;
    }

    while (1) {

        const UINT8_T *pkt = pcap_next(p, &pkthdr);

        if (!pkt){
            printf("Pcap file parse over !\n");
            file_end = 1;
        }

        printf("Length: %d\n", pkthdr.len);

        if (0 > rte_pktmbuf_alloc_bulk(pcap_replay_ctx.replay_pool, mbuf, 8)) {
            printf("Can not alloc bulk mbuf from mempool %s\n", "PCAP_REPLAY_POOL");
            return -1;
        }

        /*
         * Fill packet
         */
        rte_memcpy(mbuf[msg_count]->buf_addr, pkt, pkthdr.len);
        mbuf[msg_count]->data_len = pkthdr.len;
        msg_count++;

        if (8 == msg_count || file_end) {
            if (0 == rte_ring_enqueue_bulk(pcap_replay_ctx.pkts_ring, (void **)mbuf, 8, NULL)) {
                printf("Can not enqueue mbuf to ring %s\n", "PKTS_RING");
                return -1;
            }

            msg_count = 0;
        }
        
        if (file_end) {
            return 0;
        }
    }

    pcap_close(p);

    return 0;
}

/*
 * pcap replay work
 */
int pcap_replay_work(void *argv)
{
    /*
     * init ring
     */
    pcap_replay_ctx.pkts_ring = rte_ring_lookup("PKTS_RING");

    if (!pcap_replay_ctx.pkts_ring) {
        printf("Can not find ring %s\n", "PKTS_RING");
        return 0;
    }

    pcap_replay_ctx.replay_pool = rte_mempool_lookup("PCAP_REPLAY_POOL");

    if (!pcap_replay_ctx.replay_pool) {
        
        pcap_replay_ctx.replay_pool = rte_mempool_create("PCAP_REPLAY_POOL", 10240, RTE_MBUF_DEFAULT_BUF_SIZE, 
                                                     0, 0, NULL, NULL, NULL, NULL, rte_socket_id(), 0);

        if (!pcap_replay_ctx.replay_pool) {
            printf("mempool %s create fail! ERR : %d\n", "PCAP_REPLAY_POOL",  rte_errno);        
            return -1;
        }
    }    

    if (pcap_replay_ctx.is_dir) {
        pcap_dir_replay();
    } else {
        pcap_file_replay();
    }

    rte_mempool_free(pcap_replay_ctx.replay_pool);

    return 0;
}

static struct option long_options[] = {
    {"core",    1,  0,  'c' },
    {"dir",     1,  0,  'd' },
    {"file",    1,  0,  'f' },
    {0,         0,  0,  0 }
};

static int args_paser(int argc, char *argv[ ])
{
    int opt = 0;

    while (-1 != (opt = getopt_long(argc, argv, "c:d:f:", long_options, NULL))) {
        switch (opt) {
            case 'c' :
                pcap_replay_ctx.core_num = atoi(optarg);
                break;
            case 'd' :
                snprintf(pcap_replay_ctx.pcap_path, sizeof(pcap_replay_ctx.pcap_path), "%s", strdup(optarg));
                pcap_replay_ctx.is_dir = 1;
                break;
            case 'f' :
                snprintf(pcap_replay_ctx.pcap_path, sizeof(pcap_replay_ctx.pcap_path), "%s", strdup(optarg));
                pcap_replay_ctx.is_dir = 0;
                break;
            default :
                return -1;
        }
    }

    return 0;
}

int main(int argc, char *argv[ ])
{
    if (0 != args_paser(argc, argv)) {
        return -1;
    }

    int argc1 = 0;
    char *argv1[3];
    char argvList[3][256] = {0};

    argv1[argc1++] = argv[0];
    argv1[argc1] = argvList[0];
    snprintf(argv1[argc1++], 256, "--proc-type=secondary");
    argv1[argc1] = argvList[1];
    snprintf(argv1[argc1++], 256, "-c80");

    /*
     * dpdk init
     */
    if (0 > rte_eal_init(argc1, argv1)) {
        return -1;
    }

    UINT16_T lcore_id;

    /*
     * dpdk run
     */
    RTE_LCORE_FOREACH_SLAVE(lcore_id) {
        rte_eal_remote_launch(pcap_replay_work, NULL, lcore_id);
    }

    pcap_replay_work(NULL);

    /*
     * dpdk exit
     */
    rte_eal_mp_wait_lcore();
    rte_eal_cleanup();

    return 0;
}
