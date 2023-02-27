#include <stdio.h>

#include "dapp_list.h"

void dapp_list_init(dapp_list_t *head)
{
    head->prev = head;
    head->next = head;
}

static void __dapp_list_add(dapp_list_t *new, dapp_list_t *prev, dapp_list_t *next)
{
    prev->next = new;
    next->prev = new;
    new->prev = prev;
    new->next = next;
}

void dapp_list_add(dapp_list_t *head, dapp_list_t *new)
{
    __dapp_list_add(new, head, head->next);
}

void dapp_list_add_tail(dapp_list_t *head, dapp_list_t *new)
{
    __dapp_list_add(new, head->prev, head);
}

static void __dapp_list_del(dapp_list_t *del, dapp_list_t *prev, dapp_list_t *next)
{
    prev->next = next;
    next->prev = prev;
}

void dapp_list_del(dapp_list_t *del)
{
    __dapp_list_del(del, del->prev, del->next);

    del->next = LIST_POISON1;
    del->prev = LIST_POISON2;
}