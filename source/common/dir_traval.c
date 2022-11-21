#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "dapp_queue.h"
#include "dir_traval.h"

static int dir_node_enqueue(dapp_queue_t *queue, const char *fname, int is_dir, int level)
{
    dir_node_t node;

    snprintf(node.d_name, sizeof(node.d_name), "%s", fname);
    node.is_dir = is_dir;
	node.level = level;

    return dapp_enqueue(queue, &node, sizeof(node));
}

int dir_push(dapp_queue_t **queue, const char *path, int file_num)
{
    if (!path) {
        goto TRAVAL_FAILED;
    }

    dapp_queue_t *dir_queue = NULL;

    dir_node_t dir_node;

    (*queue) = dapp_queue_create(file_num, sizeof(dir_node_t));

    if (!(*queue)) {
        goto TRAVAL_FAILED;
    }

    dir_queue = dapp_queue_create(file_num, sizeof(dir_node));

    if (!dir_queue) {
        goto TRAVAL_FAILED;
    }
    
    struct stat st;

    stat(path, &st);
    
    if (!S_ISDIR(st.st_mode)) {

        dir_node_enqueue((*queue), path, 0, 0);
    
        goto TRAVAL_SUCCESS;
    } else {
        
        DIR *pDir = NULL;
        char f_name[256] = {0};

        dir_node_enqueue(dir_queue, path, 1, 0);

        while (!dapp_dequeue(dir_queue, &dir_node, sizeof(dir_node))) {

            dir_node_enqueue((*queue), dir_node.d_name, 1, dir_node.level);
        
            /*
             * Open directory
             */
            pDir = opendir(dir_node.d_name);

            if (!pDir) {
                continue;
            }

            struct dirent* ent = NULL;

            /*
             * Traval directory
             */
            while ((ent = readdir(pDir)))
            {
				if (!strncmp(ent->d_name, ".", strlen(".")) || 
					!strncmp(ent->d_name, "..", strlen(".."))) {
					continue;
				}

				snprintf(f_name, sizeof(f_name), "%s/%s", dir_node.d_name, ent->d_name);
					
                /*
                 * Is directory
                 */
                if (DT_DIR == ent->d_type) {
                    dir_node_enqueue(dir_queue, f_name, 1, dir_node.level + 1);
                } else {
                    dir_node_enqueue((*queue), f_name, 0, dir_node.level + 1);
                }
            }
        }
    }

TRAVAL_SUCCESS :

    dapp_queue_free(dir_queue);

    return 0;

TRAVAL_FAILED :

    dapp_queue_free(dir_queue);
    dapp_queue_free((*queue));

    *queue = NULL;

    return -1;
}

int dir_pop(dapp_queue_t *queue, dir_node_t *node)
{
    if (!queue || !node) {
        return -1;
    }

	if (queue->avail == queue->total) {
        dapp_queue_free(queue);
		return 1;
    }

    dapp_dequeue(queue, node, sizeof(*node));

    return 0;
}

int main(int argc, char *argv[ ])
{
    char *path = argv[1];

    dapp_queue_t *queue = NULL;

    if (dir_push(&queue, path, 10240)) {
        printf("failed to push dir\n");
        return -1;
    }

    dir_node_t node;
	int i;

    while (!dir_pop(queue, &node)) {

		for (i = 0; i < node.level; ++i) {
			printf("    ");
		}
	
        if (!node.is_dir) {
            printf("file : %s\n", node.d_name);
        } else {
            printf("dir : %s\n", node.d_name);
        }
    }
}
