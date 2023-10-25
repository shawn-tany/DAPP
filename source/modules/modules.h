#ifndef DAPP_MODULES
#define DAPP_MODULES

#include <unistd.h>
#include "common.h"
#include "config.h"

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
    
} DAPP_MODULES_TYPE;

typedef enum
{
    DAPP_MODULE_INIT_START = 0,
    DAPP_MODULE_INIT_OK,
    DAPP_MODULE_INIT_WAIT,
    DAPP_MODULE_INIT_FAIL,

} DAPP_INIT_STATUS;

typedef enum 
{
    DAPP_MODULE_UNI_INIT = 0,
    DAPP_MODULE_MULTI_INIT,

} DAPP_INIT_OPTIONS;

typedef STATUS (*dapp_module_conf)(void *);
typedef STATUS (*dapp_module_init)(void *);
typedef STATUS (*dapp_module_exec)(UINT8_T *, void *);
typedef STATUS (*dapp_module_exit)(void *);

typedef struct
{
    dapp_module_conf conf;
    dapp_module_init init;
    dapp_module_exec exec;
    dapp_module_exit exit;
} __attribute((__packed__)) DAPP_MODULE_OPS;

typedef struct 
{
    /* Module Registration */
    struct {
        UINT8_T reg : 1;
        UINT8_T var : 7;
        char name[MODULES_NAME_SIZE];
        DAPP_MODULE_OPS *module_ops;
    } reg;

    /* Usage of module lcore */
    struct {
        UINT8_T running;
        UINT64_T lcore_mask;
        UINT8_T lcore_num;
    } lcore;

    /* Relevance of modules */
    struct {
        UINT8_T multi_init;
        UINT32_T has_initd;
        DAPP_INIT_STATUS init_status;
        UINT8_T rely_num;
        UINT64_T rely_mask; 
    } rely;

    /* Module Workspace */
    struct 
    {
        void *work_data;
        UINT32_T work_size;
    } ws;
} __attribute((__packed__)) DAPP_MODULE_WK;

typedef struct 
{
    DAPP_MODULE_WK module[DAPP_MODULE_TYPE_NUM];

    UINT64_T lcore_mask;
    UINT8_T lcore_num;
} __attribute((__packed__)) DAPP_MODULES_TABLE;

char *dapp_modules_name_get_by_type(DAPP_MODULES_TYPE type);

UINT64_T dapp_modules_total_lcore_mask_get(void);

UINT64_T dapp_module_lcore_mask_get(DAPP_MODULES_TYPE type);

UINT16_T dapp_module_lcore_num_get_by_type(DAPP_MODULES_TYPE type);

DAPP_MODULES_TYPE dapp_module_type_get_by_lcore(UINT64_T lcore);

void dapp_module_reg(DAPP_MODULES_TYPE type, const char *name, DAPP_MODULE_OPS *module_ops);

void dapp_module_unreg(DAPP_MODULES_TYPE type);

void dapp_module_lcore_init(DAPP_MODULES_TYPE type, UINT16_T lcore_num);

void dapp_module_lcore_uninit(DAPP_MODULES_TYPE type);

void dapp_module_rely_init(DAPP_MODULES_TYPE type, UINT8_T multi_init, int rely_num, ...);

void dapp_module_rely_uninit(DAPP_MODULES_TYPE type);

STATUS DAPP_MODL_CONF_MACHINE(const char *file_name, void *arg);

STATUS DAPP_MODL_TASK_MACHINE(UINT16_T lcore);

STATUS DAPP_MODL_INIT_MACHINE(DAPP_MODULES_TYPE init_type, void *arg);

STATUS DAPP_MODL_EXEC_MACHINE(DAPP_MODULES_TYPE exec_type, void *arg);

STATUS DAPP_MODL_EXIT_MACHINE(DAPP_MODULES_TYPE exit_type, void *arg);

/*
 * Macro of modules registration
 */
#define _DAPP_MODULE_REG(module) dapp_module_reg_##module
#define DAPP_MODULE_REG(module) _DAPP_MODULE_REG(module)

#define _DAPP_MODULE_UNREG(module) dapp_module_unreg_##module
#define DAPP_MODULE_UNREG(module) _DAPP_MODULE_UNREG(module)

#define DAPP_MODULE_REG_CONSTRUCTOR(type, name, module_ops)   \
    static void __attribute__((constructor)) DAPP_MODULE_REG(name)() {   \
        dapp_module_reg(type, #name, module_ops);             \
    }

#define DAPP_MODULE_UNREG_DESTRUCTOR(type, name)                    \
    static void __attribute__((destructor)) DAPP_MODULE_UNREG(name)() {  \
        dapp_module_unreg(type);                                    \
    }

#endif
