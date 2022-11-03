#ifndef DAPP_CODE
#define DAPP_CODE

#define DAPP_MASK_SET(mask, bit)            \
{                                           \
    if ((sizeof(mask) * 8) <= bit) {        \
        printf("invalid bit\n");            \
    }                                       \
                                            \
    mask |= (((typeof(mask)) 1) << bit);  \
}

#define DAPP_MASK_CLR(mask, bit)            \
{                                           \
    if ((sizeof(mask) * 8) <= bit) {        \
        printf("invalid bit\n");            \
    }                                       \
                                            \
    typeof(mask) val = 1;                   \
    mask &= ~(val << bit);                  \
}

#define DAPP_MASK_TST(mask, bit, val)       \
{                                           \
    if ((sizeof(mask) * 8) <= bit) {        \
        printf("invalid bit\n");            \
    }                                       \
                                            \
    val = mask << ((sizeof(mask) * 8) - bit - 1);\
    val = val >> (sizeof(mask) * 8 - 1); \
}

#define DAPP_MASK_PRT(mask)                 \
{                                           \
    int mi = 0;                             \
    for (mi = (sizeof(mask) * 8) - 1; mi >= 0; --mi) {\
                                            \
        typeof(mask) mc;                    \
        DAPP_MASK_TST(mask, mi, mc);        \
                                            \
        printf("%d", mc);                   \
        if (mi != ((sizeof(mask) * 8) - 1) && !(mi % 4)) {\
            printf(" ");                    \
        }                                   \
    }                                       \
    printf("\n");                           \
}

#endif