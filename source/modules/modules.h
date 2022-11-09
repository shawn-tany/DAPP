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

typedef enum
{
    DAPP_MODULE_INIT_START = 1,
    DAPP_MODULE_INIT_OK,
    DAPP_MODULE_INIT_FAIL,

} DAPP_INIT_STATUS;

typedef int (*dapp_module_init)(void *);
typedef int (*dapp_module_exec)(void *);
typedef int (*dapp_module_exit)(void *);

typedef struct 
{
    /* Module Registration */
    struct {
        UINT8_T reg : 1;
        UINT8_T var : 7;
        char name[MODULES_NAME_SIZE];
        dapp_module_init init;
        dapp_module_exec exec;
        dapp_module_exit exit;
    } reg;

    /* Usage of module lcore */
    struct {
        UINT8_T running : 1;
        UINT8_T init_status : 2;
        UINT8_T var : 5;
        UINT64_T lcore_mask;
        UINT8_T lcore_num;
    } lcore;

    /* Module Workspace */
    struct 
    {
        void *work_data;
        UINT32_T work_size;
    } ws;
} __attribute((__packed__)) dapp_module_t;

typedef struct 
{
    dapp_module_t module[DAPP_MODULE_TYPE_NUM];

    UINT64_T lcore_mask;
    UINT8_T lcore_num;
} __attribute((__packed__)) dapp_modules_table_t;

char *dapp_modules_name_get_by_type(dapp_modules_type_t type);

UINT64_T dapp_module_lcore_mask_get(dapp_modules_type_t type);

UINT64_T dapp_modules_total_lcore_mask_get(void);

dapp_module_t *dapp_module_get_by_lcore(UINT64_T lcore);

dapp_module_t *dapp_module_get_by_type(dapp_modules_type_t type);

void dapp_module_init_status_set(dapp_modules_type_t type, DAPP_INIT_STATUS status);

STATUS dapp_module_init_wait(dapp_modules_type_t type);

void dapp_module_reg(dapp_modules_type_t type, 
                           const char *name, 
                           dapp_module_init init, 
                           dapp_module_exec exec, 
                           dapp_module_exit exit);

void dapp_module_unreg(dapp_modules_type_t type);

void dapp_module_lcore_init(dapp_modules_type_t type, UINT16_T lcore_num);

void dapp_module_lcore_uninit(dapp_modules_type_t type);

void dapp_module_ws_init(dapp_modules_type_t type, void *data, UINT32_T size);

void dapp_module_ws_uninit(dapp_modules_type_t type);

/*
 * Macro of modules registration
 */
#define _DAPP_MODULE_REG(module) dapp_module_reg_##module
#define DAPP_MODULE_REG(module) _DAPP_MODULE_REG(module)

#define _DAPP_MODULE_UNREG(module) dapp_module_unreg_##module
#define DAPP_MODULE_UNREG(module) _DAPP_MODULE_UNREG(module)

#define DAPP_MODULE_REG_CONSTRUCTOR(type, name, init, exec, exit)   \
    static __attribute__((constructor)) DAPP_MODULE_REG(name)() {   \
        dapp_module_reg(type, #name, init, exec, exit);             \
    }

#define DAPP_MODULE_UNREG_DESTRUCTOR(type, name)                    \
    static __attribute__((destructor)) DAPP_MODULE_UNREG(name)() {  \
        dapp_module_unreg(type);                                    \
    }

#endif
