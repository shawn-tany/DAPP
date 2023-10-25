#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "dapp_queue.h"
#include "dapp_stack.h"
#include "dapp_dir_traval.h"

static int dir_node_enqueue(dapp_queue_t *queue, const char *fname, int is_dir, int depth)
{
    dir_node_t node;

    snprintf(node.d_name, sizeof(node.d_name), "%s", fname);
    node.is_dir = is_dir;
    node.depth = depth;

    return dapp_enqueue(queue, &node, sizeof(node));
}

static int dir_node_enstack(dapp_stack_t *stack, const char *fname, int is_dir, int depth)
{
    dir_node_t node;

    snprintf(node.d_name, sizeof(node.d_name), "%s", fname);
    node.is_dir = is_dir;
    node.depth = depth;

    return dapp_enstack(stack, &node, sizeof(node));
}

int dir_push(dapp_queue_t **queue, const char *path, int file_num)
{
    if (!path) {
        goto TRAVAL_FAILED;
    }

    dapp_stack_t *dir_stack = NULL;

    dir_node_t dir_node;

    if (access(path, F_OK))
    {
        goto TRAVAL_FAILED;
    }

    (*queue) = dapp_queue_create(file_num, sizeof(dir_node_t));

    if (!(*queue)) {
        goto TRAVAL_FAILED;
    }

    dir_stack = dapp_stack_create(file_num, sizeof(dir_node_t));

    if (!dir_stack) {
        goto TRAVAL_FAILED;
    }
    
    struct stat st;

    stat(path, &st);
    
    if (!S_ISDIR(st.st_mode)) {
        dir_node_enqueue((*queue), path, 0, 0);
    } else {
        
        DIR *pDir = NULL;
        char f_name[512] = {0};

        dir_node_enstack(dir_stack, path, 1, 0);

        while (!dapp_destack(dir_stack, &dir_node, sizeof(dir_node))) {

            dir_node_enqueue((*queue), dir_node.d_name, 1, dir_node.depth);
        
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

                if ('/' == dir_node.d_name[strlen(dir_node.d_name) - 1]) {
                    snprintf(f_name, sizeof(f_name), "%s%s", dir_node.d_name, ent->d_name);
                } else {
                    snprintf(f_name, sizeof(f_name), "%s/%s", dir_node.d_name, ent->d_name);
                }

                /*
                 * Is directory
                 */
                if (DT_DIR == ent->d_type) {
                    dir_node_enstack(dir_stack, f_name, 1, dir_node.depth + 1);
                } else {
                    dir_node_enqueue((*queue), f_name, 0, dir_node.depth + 1);
                }
            }
        }
    }

    dapp_stack_free(dir_stack);

    return 0;

TRAVAL_FAILED :

    dapp_stack_free(dir_stack);
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
