#ifndef DAPP_CODE
#define DAPP_CODE

#define DAPP_MASK_SET(mask, bit)        (mask |= (1 << bit))

#define DAPP_MASK_CLR(mask, bit)        (mask &= ~(1 << bit))

#define DAPP_MASK_TST(mask, bit)        (mask & (1 << bit))

#define DAPP_MASK_SHOW(mask)                \
{                                           \
    int mi = 0;                             \
    for (mi = (sizeof(mask) * 8) - 1; mi >= 0; --mi) {      \
        printf("%d : %d\n", mi, DAPP_MASK_TST(mask, mi) ? 1 : 0);       \
        if (mi != ((sizeof(mask) * 8) - 1) && !(mi % 4)) { \
            printf(" ");                    \
        }                                   \
    }                                       \
    printf("\n");                           \
}

#endif