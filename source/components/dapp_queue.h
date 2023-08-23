#ifndef DAPP_QUEUE
#define DAPP_QUEUE

typedef struct 
{
    void *addr;
    void *prod;
    void *cons;
    int offset;
    int avail;
    int total;
} dapp_queue_t;

dapp_queue_t *dapp_queue_create(int node_num, int node_size);

void dapp_queue_free(dapp_queue_t *queue);

int dapp_enqueue(dapp_queue_t *queue, void *node, int size);

int dapp_dequeue(dapp_queue_t *queue, void *node, int size);

#endif