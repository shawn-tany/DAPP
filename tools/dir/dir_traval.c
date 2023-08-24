#include <stdio.h>
#include <unistd.h>

#include "dapp_dir_traval.h"

int main(int argc, char *argv[ ])
{
    char *path = NULL;
    
    if (2 > argc) {
        path = ".";
    }
    else
    {
        path = argv[1];
    }

    if (access(path, F_OK))
    {
        printf("ERROR \"%s\": no such file or directory!\n", path);
        return -1;
    }

    dapp_queue_t *queue = NULL;

    if (dir_push(&queue, path, 102400)) {
        printf("ERROR : failed to push dir!\n");
        return -1;
    }

    dir_node_t node;
    int i;

    while (!dir_pop(queue, &node)) {

        for (i = 0; i < node.depth; ++i) {
            printf("  ");
        }

        if (!node.is_dir) {
            printf("|");
        } else {
            printf("+");
        }

        printf("%s\n", node.d_name);
    }
}

