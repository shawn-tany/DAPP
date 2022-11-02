#ifndef DAPP_MODULES
#define DAPP_MODULES

#include "common.h"

#define MODULES_ITEM(a) (sizeof(a) / sizeof((a)[0]))

#define MODULES_NAME_SIZE (128)

typedef enum 
{
    DAPP_MODULES_CTRL = 0,
    DAPP_MODULES_PORT_TRANS,
    DAPP_MODULES_PROTO_IDENTI,
    DAPP_MODULES_RULE_MATCH,
    DAPP_MODULES_FLOW_ISOTONIC,
    DAPP_MODULES_DATA_RESTORE,
    DAPP_MODULES_FILE_IDENTI,
    DAPP_MODULES_TYPE_NUM,
    
} dapp_modules_type_t;

typedef int (*dapp_module_init)(void *);
typedef int (*dapp_module_exec)(void *);
typedef int (*dapp_module_exit)(void *);

typedef struct 
{
    struct {
        /* Module Registration */
        struct {
            uint8_t reg : 1;
            uint8_t var : 7;
            char name[MODULES_NAME_SIZE];
            dapp_module_init init;
            dapp_module_exec exec;
            dapp_module_exit exit;
        } reg;

        /* Usage of module lcore */
        struct {
            uint8_t running : 1;
            uint8_t var : 7;
            uint64_t lcore_mask;
        } lcore;

        /* Module Workspace */
        struct 
        {
            void *work_data;
            uint32_t work_size;
        } ws;
    } module[DAPP_MODULES_TYPE_NUM];

    uint64_t lcore_mask;
    uint8_t lcore_num;
} __attribute((__packed__)) dapp_modules_t;

char *dapp_modules_name_get(dapp_modules_type_t type);

void dapp_module_reg(dapp_modules_type_t type, dapp_module_t *module_handle);

void dapp_module_unreg(dapp_modules_type_t type);

#endif
