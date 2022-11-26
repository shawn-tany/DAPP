#include <stdio.h>

#include "dapp_list.h"

void dapp_list_init(dapp_list_head_t *head)
{
    head->prev = head;
    head->next = head;
}

void dapp_list_del()
{
    
}