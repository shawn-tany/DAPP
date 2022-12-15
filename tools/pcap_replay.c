#include <stdio.h>
#include <getopt.h>
#include <signal.h>

#include "pcap.h"

#include "rte_lcore.h"
#include "rte_mbuf.h"
#include "rte_mempool.h"
#include "rte_ring.h"
#include "rte_errno.h"

#include "dapp_dir_traval.h"

#define DAPP_RING_OBJ_NUM (8)

typedef unsigned char UINT8_T;
typedef unsigned short UINT16_T;
typedef unsigned long UINT32_T;
typedef unsigned long long UINT64_T;

static struct {
    char pcap_path[256];
    int is_dir;
    int core_num;
    struct rte_ring *pkts_ring;
    struct rte_mempool *replay_pool;
} pcap_replay_ctx;

static UINT8_T running = 1;

void signal_handle(int sig)
{
    if (SIGINT == sig) {
        printf("pcap replay exit!\n");
        running = 0;
    }
}

static struct option long_options[] = {
    {"core",    1,  0,  'c' },
    {"dir",     1,  0,  'd' },
    {"file",    1,  0,  'f' },
    {"help",    0,  0,  'h' },
    {0,         0,  0,  0 }
};

static void pcap_replay_options_print(char *program)
{
    printf("%s OPTIONS :\n"
           "    -c, --core, set core num\n"
           "    -d, --dir,  select a replay directory\n"
           "    -f, --file, select a replay file\n"
           "    -h, --help, show %s options\n", 
           program, program);
}

static int args_paser(int argc, char *argv[ ])
{
    int opt = 0;

    pcap_replay_ctx.core_num = 1;

    while (-1 != (opt = getopt_long(argc, argv, "c:d:f:h", long_options, NULL))) {
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
            case 'h' :
                pcap_replay_options_print(argv[0]);
                break;
            default :
                printf("invalid options\n");
                pcap_replay_options_print(argv[0]);
                return -1;
        }
    }

    return 0;
}

static int args_check(void)
{
    if (!pcap_replay_ctx.core_num) {
        printf("invalid core num = %d\n", pcap_replay_ctx.core_num);
        return 0;
    }

    if (!strlen(pcap_replay_ctx.pcap_path)) {
        printf("please select a directory or file\n");
        return 0;
    }

    return 1;
}

static UINT64_T deq_count = 0;
static UINT64_T enq_count = 0;
static UINT64_T rls_count = 0;

static int pcap_file_replay(const char *path)
{
    int ret = 0;
    int i = 0;
    int j = 0;
    pcap_t *p = NULL;
    char err_buf[PCAP_ERRBUF_SIZE] = {0};
    struct rte_mbuf *mbuf[8] = {0};
    struct pcap_pkthdr pkthdr;
    int msg_count = 0;
    int file_end = 0;
    UINT32_T nenq_msg = 0;

    /*
     * open pcap file
     */
    p = pcap_open_offline(path, err_buf);

    if (!p) {
        printf("Faild to open pcap file %s! ERR : %s\n", path, err_buf);
        return -1;
    }

    printf("mempool avail count: %d\n", rte_mempool_avail_count(pcap_replay_ctx.replay_pool));

    while (running) {
        
        /*
         * Step1 alloc pktmbuf bulk
         */
        if (0 != (ret = rte_pktmbuf_alloc_bulk(pcap_replay_ctx.replay_pool, mbuf, DAPP_RING_OBJ_NUM))) {
            printf("Can not alloc bulk mbuf from mempool %s! ERR : %d\n", "DAPP_PCAP_REPLAY_POOL", ret);
            break;
        }

        /*
         * Step2 fill pktmbuf bulk
         */
        for (i = 0; i < DAPP_RING_OBJ_NUM; ++i) {
        
            const UINT8_T *pkt = pcap_next(p, &pkthdr);

            if (!pkt){
                file_end = 1;
                break;
            }

            /*
             * Fill packet
             */            
            memcpy(mbuf[i]->buf_addr + mbuf[i]->data_off + mbuf[i]->data_len, pkt, pkthdr.len);
            mbuf[i]->data_len += pkthdr.len;
            mbuf[i]->pkt_len += pkthdr.len;
        }

        /*
         * Step3 enqueue ring
         */
        nenq_msg = rte_ring_enqueue_bulk(pcap_replay_ctx.pkts_ring, (void **)mbuf, i, NULL);
    
        if (i && 0 == nenq_msg) {
            printf("Can not enqueue mbuf to ring %s\n", "PKTS_RING");

            for (j = 0; j < i; ++j) {
                rte_pktmbuf_free(mbuf[j]);
            }
        }

        enq_count += nenq_msg;
        
        if (file_end) {
            printf("Pcap file parse over !\n");
            break;
        }
    }

    printf("tx packet count = %llu !\n", enq_count);

    pcap_close(p);

    return 0;
}

static int pcap_dir_replay(const char *path)
{
    dapp_queue_t *queue = NULL;
    dir_node_t node;

    if (dir_push(&queue, path, 10240)) {
        printf("failed to push dir\n");
        return -1;
    }

    while (!dir_pop(queue, &node)) {
        if (node.is_dir) {
            printf("replay dir %s\n", node.d_name);
        } else {
            printf("replay file %s\n", node.d_name);

            pcap_file_replay(node.d_name);
        }
    }

    return 0;
}

/*
 * pcap replay work
 */
int pcap_replay_work(void *argv)
{
    printf("pcap_replay...\n");

    /*
     * init ring
     */
    pcap_replay_ctx.pkts_ring = rte_ring_lookup("PKTS_RING");

    if (!pcap_replay_ctx.pkts_ring) {
        printf("Can not find ring %s\n", "PKTS_RING");
        return 0;
    }

    pcap_replay_ctx.replay_pool = rte_mempool_lookup("DAPP_PCAP_REPLAY_POOL");

    if (!pcap_replay_ctx.replay_pool) {
        
        pcap_replay_ctx.replay_pool = rte_mempool_create("DAPP_PCAP_REPLAY_POOL", 102400, RTE_MBUF_DEFAULT_BUF_SIZE, 
                                                     RTE_CACHE_LINE_SIZE, 0, rte_pktmbuf_pool_init, NULL, 
                                                     rte_pktmbuf_init, NULL, rte_socket_id(), 0);

        if (!pcap_replay_ctx.replay_pool) {
            printf("pktmbuf pool %s create fail! ERR : %s\n", "DAPP_PCAP_REPLAY_POOL",  rte_strerror(rte_errno));        
            return -1;
        }
    }
        
    if (pcap_replay_ctx.is_dir) {
        pcap_dir_replay(pcap_replay_ctx.pcap_path);
    } else {
        pcap_file_replay(pcap_replay_ctx.pcap_path);
    }

    return 0;
}

int main(int argc, char *argv[ ])
{
    if (0 != args_paser(argc, argv)) {
        return -1;
    }

    if (!args_check()) {
        return -1;
    }

    int argc1 = 0;
    char *argv1[3];
    char argvList[3][256] = {0};

    signal(SIGINT, signal_handle);

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
