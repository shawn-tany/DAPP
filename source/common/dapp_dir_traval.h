#ifndef DAPP_DIR_TRAVAL
#define DAPP_DIR_TRAVAL

#include "dapp_queue.h"

#define FILE_NAME_SIZE (256)

typedef struct 
{
    char d_name[FILE_NAME_SIZE];
    int is_dir;
	int depth;
} dir_node_t;

int dir_push(dapp_queue_t **queue, const char *path, int file_num);

int dir_pop(dapp_queue_t *queue, dir_node_t *node);

#endif
