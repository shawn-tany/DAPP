#ifndef DAPP_LIST
#define DAPP_LIST

typedef struct dapp_list_head
{
    struct dapp_list_head *prev;
    struct dapp_list_head *next;
} dapp_list_head_t;

#endif