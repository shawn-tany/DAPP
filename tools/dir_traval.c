#include <stdio.h>

#include "dapp_dir_traval.h"

int main(int argc, char *argv[ ])
{
    if (2 > argc) {
        printf("ERROR : Please select a directory!\n");
        return -1;
    }

    char *path = argv[1];

    dapp_queue_t *queue = NULL;

    if (dir_push(&queue, path, 102400)) {
        printf("failed to push dir\n");
        return -1;
    }

    dir_node_t node;
    int i;

    while (!dir_pop(queue, &node)) {

        for (i = 0; i < node.depth; ++i) {
            printf("    ");
        }

        if (!node.is_dir) {
            printf("|");
        } else {
            printf("+");
        }

        printf("%s\n", node.d_name);
    }
}

