#ifndef DAPP_CODE
#define DAPP_CODE

#define DAPP_MASK_SET(mask, bit)            \
{                                           \
    if ((sizeof(mask) * 8) <= bit) {        \
        printf("invalid bit\n");            \
    }                                       \
                                            \
    mask |= (((typeof(mask)) 1) << bit);  \
                                            \
    mask;                                   \
}

#define DAPP_MASK_CLR(mask, bit)            \
{                                           \
    if ((sizeof(mask) * 8) <= bit) {        \
        printf("invalid bit\n");            \
    }                                       \
                                            \
    mask &= ~((typeof(mask)1) << bit);      \
                                            \
    mask;                                   \
}

#define DAPP_MASK_TST(mask, bit)            \
{                                           \
    if ((sizeof(mask) * 8) <= bit) {        \
        printf("invalid bit\n");            \
    }                                       \
                                            \
    (typeof(mask)) mask_tmp = 0;            \
                                            \
    mask_tmp |= mask << bit;                \
    mask_tmp |= mask_tmp >> (sizeof(mask) * 8 - 1)\
                                            \
    mask_tmp;                               \
}

#if 0
#define DAPP_MASK_PRT(mask)                 \
{                                           \
    int mi = 0;                             \
    for (mi = sizeof(mask) - 1; mi >= 0; --mi) {\
                                            \
        int mc = DAPP_MASK_TST(mask, mi);   \
                                            \
        printf("%d", mc);                   \
                                            \
        if (mi % 4) {                       \
            printf(" ");                    \
        }                                   \
    }                                       \
                                            \
    printf("\n");                           \
}
#endif

#endif