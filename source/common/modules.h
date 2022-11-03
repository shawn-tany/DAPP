#ifndef DAPP_MODULES
#define DAPP_MODULES

#include "common.h"

#define MODULES_ITEM(a) (sizeof(a) / sizeof((a)[0]))

#define MODULES_NAME_SIZE (128)

typedef enum 
{
    DAPP_MODULE_CONTROL = 0,
    DAPP_MODULE_PORT,
    DAPP_MODULE_PROTOCOL,
    DAPP_MODULE_RULE,
    DAPP_MODULE_FLOWS,
    DAPP_MODULE_RESTORE,
    DAPP_MODULE_FILES,
    DAPP_MODULE_TYPE_NUM,
    
} dapp_modules_type_t;

typedef int (*dapp_module_init)(void *);
typedef int (*dapp_module_exec)(void *);
typedef int (*dapp_module_exit)(void *);

typedef struct 
{
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
} __attribute((__packed__)) dapp_module_t;

typedef struct 
{
    dapp_module_t module[DAPP_MODULE_TYPE_NUM];

    uint64_t lcore_mask;
    uint8_t lcore_num;
} __attribute((__packed__)) dapp_modules_table_t;

char *dapp_modules_name_get(dapp_modules_type_t type);

uint64_t dapp_module_lcore_mask_get(dapp_modules_type_t type);

uint64_t dapp_modules_total_lcore_mask_get(void);

dapp_module_t *dapp_module_get_by_lcore(uint64_t lcore);

void dapp_module_reg(dapp_modules_type_t type, 
                           const char *name, 
                           dapp_module_init init, 
                           dapp_module_exec exec, 
                           dapp_module_exit exit);

void dapp_module_unreg(dapp_modules_type_t type);

void dapp_module_lcore_init(dapp_modules_type_t type, uint16_t lcore_num);

void dapp_module_lcore_uninit(dapp_modules_type_t type);

void dapp_module_ws_init(dapp_modules_type_t type, void *data, uint32_t size);

void dapp_module_ws_uninit(dapp_modules_type_t type);

#define DAPP_MODULE_REG_CONSTRUCTOR(type, name, init, exec, exit)   \
    static __attribute__((constructor)) dapp_module_reg##_name() {  \
        dapp_module_reg(type, name, init, exec, exit);              \
    }                                                                               

#define DAPP_MODULE_UNREG_DESTRUCTOR(type, name)                    \
    static __attribute__((constructor)) dapp_module_unreg##_name() {\
            dapp_module_unreg(type);                                \
    }  

#endif
