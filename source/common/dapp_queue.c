#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dapp_queue.h"

#define PTR_CHECK(p) {  \
    if (!p) {           \
        return -1;      \
    }                   \
}

dapp_queue_t *dapp_queue_create(int node_num, int node_size)
{
    dapp_queue_t *queue = NULL;

    queue = (dapp_queue_t *)malloc(sizeof(dapp_queue_t));

    if (!queue) {
        return NULL;
    }

    queue->addr = malloc(node_num * node_size);

    if (!queue->addr) {
        return NULL;
    }

    memset(queue->addr, 0, node_num * node_size);

    queue->prod = queue->addr;
    queue->cons = queue->addr;

    queue->total = node_num;
    queue->avail = node_num;
    queue->offset = node_size;

    return queue;
}

int dapp_enqueue(dapp_queue_t *queue, void *node, int size)
{
    PTR_CHECK(queue);
    PTR_CHECK(node);
    PTR_CHECK(queue->addr);

    if (size > queue->offset) {
        return -2;
    }

    if (!queue->avail) {
        return -3;
    }

    if (queue->prod > queue->addr + (queue->offset * queue->total) ||
        queue->prod + queue->offset > queue->addr + (queue->offset * queue->total)) {
        
        queue->prod = queue->addr;
    }

    memcpy(queue->prod, node, size);

    queue->prod = queue->prod + queue->offset;
    
    queue->avail--;

    return 0;
}

int dapp_dequeue(dapp_queue_t *queue, void *node, int size)
{
    PTR_CHECK(queue);
    PTR_CHECK(node);
    PTR_CHECK(queue->addr);

    if (queue->avail == queue->total) {
        return -4;
    }

    if (queue->offset < size) {
        return -5;
    }

    if (queue->cons > queue->addr + (queue->offset * queue->total) ||
        queue->cons + queue->offset > queue->addr + (queue->offset * queue->total)) {
        
        queue->prod = queue->addr;
    }

    memcpy(node, queue->cons, size);

    queue->cons = queue->cons + queue->offset;
    
    queue->avail++;

    return 0;
}

void dapp_queue_free(dapp_queue_t *queue)
{
    if (queue) {
        if (queue->addr) {
            free(queue->addr);
            queue->addr = NULL;
        }

        queue->prod = NULL;
        queue->cons = NULL;
        
        queue->total = 0;
        queue->avail = 0;
        queue->offset = 0;

        free(queue);
    }
}

#if 0
int main()
{
    dapp_queue_t *queue = dapp_queue_create(100, sizeof(int));

    if (!queue) {
        printf("faild to create queue\n");
        return -1;
    }

    int i;

    for (i = 0; i < 100; ++i) {
        if (dapp_enqueue(queue, &i, sizeof(i))) {
            printf("failed to enqueue\n");
            goto FAIL;
        }
    }

    int node;
    int idex = 0;
    
    while (!dapp_dequeue(queue, &node, sizeof(node))) {
        printf("dequeue[%d] : %d\n", ++idex, node);
    }

    dapp_queue_free(queue);

    return 0;

FAIL :

    dapp_queue_free(queue);

    return -1;
}
#endif
