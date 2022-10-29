#ifndef DAPP_MODULES
#define DAPP_MODULES

#define MODULES_ITEM(a) (sizeof(a) / sizeof((a)[0]))

#define MODULES_NAME_SIZE (128)

typedef enum 
{
    MODULES_CTRL = 0,
    MODULES_PORT_TRANS,
    MODULES_PROTO_IDENTI,
    MODULES_RULE_MATCH,
    MODULES_FLOW_ISOTONIC,
    MODULES_DATA_RESTORE,
    MODULES_FILE_IDENTI,
    MODULES_TYPE_NUM,
    
} dapp_modules_type_t;

typedef struct {
    struct {
        char name[MODULES_NAME_SIZE];
        dapp_modules_type_t type;
    } modules[MODULES_TYPE_NUM];

} dapp_modules_t;

char *dapp_modules_name_get(dapp_modules_type_t type);

#endif