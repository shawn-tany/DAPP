#ifndef DAPP_DIR_TRAVAL
#define DAPP_DIR_TRAVAL

#define FILE_NAME_SIZE (256)

typedef struct 
{
    char d_name[FILE_NAME_SIZE];
    int is_dir;
} dir_node_t;

#endif