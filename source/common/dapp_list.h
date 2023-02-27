#ifndef DAPP_LIST
#define DAPP_LIST

#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

typedef struct dapp_list_head
{
    struct dapp_list_head *prev;
    struct dapp_list_head *next;
} dapp_list_t;


#define offsetof(type, member) ((size_t) &(((type *)0)->member))


#define container_of(list, type, member)                    \
({                                                          \
    const typeof(((type *)0)->member)*_ptr = (list);        \
    (type *)((char *)_ptr - offsetof(type, member));        \
})


#define dapp_list_entry(list, type, member)                  \
    container_of(list, type, member)


#define dapp_list_for_each_entry(pos, head, member)                         \
    for (pos = dapp_list_entry((head)->next, typeof(*pos), member);         \
         &pos->member != (head);                                            \
         pos = dapp_list_entry(pos->member.next, typeof(*pos), member))


#define dapp_list_for_each_entry_safe(pos, next, head, member)              \
    for (pos = dapp_list_entry((head)->next, typeof(*pos), member),         \
         next = dapp_list_entry(pos->member.next, typeof(*pos), member);    \
         &pos->member != (head);                                            \
         pos = next, next = dapp_list_entry(next->member.next, typeof(*pos), member))

#define dapp_LIST_EMPTY(list) ((list)->prev == (list) && (list)->next == (list))


#endif
