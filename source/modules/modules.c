#include <stdio.h>
#include <stdarg.h>

#include "modules.h"
#include "dapp_code.h"

static dapp_modules_table_t MODULES;

#define DAPP_MODULE_WAIT_TIMES (60)

#define DAPP_MODULE_INIT_WAIT(type) {                                           \
                                                                                \
    UINT32_T times = 0;                                                         \
                                                                                \
    while (DAPP_MODULE_INIT_OK != MODULES.module[type].rely.init_status) {      \
                                                                                \
        if (DAPP_MODULE_INIT_FAIL == MODULES.module[type].rely.init_status) {   \
            return DAPP_FAIL;                                                   \
        }                                                                       \
                                                                                \
        sleep(1);                                                               \
                                                                                \
        if (++times > DAPP_MODULE_WAIT_TIMES) {                                 \
            return DAPP_ERR_MODL_TIMEOUT;                                       \
        }                                                                       \
    }                                                                           \
}

#define DAPP_MODULE_INIT_STATUS_SET(type, status) {     \
    MODULES.module[type].rely.init_status = status;     \
}                                                       \

char *dapp_modules_name_get_by_type(dapp_modules_type_t type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return "unknow";
    }

    return MODULES.module[type].reg.name;
}

UINT64_T dapp_modules_total_lcore_mask_get(void)
{
    return MODULES.lcore_mask;
}

UINT64_T dapp_module_lcore_mask_get(dapp_modules_type_t type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return 0;
    }

    return MODULES.module[type].lcore.lcore_mask;
}

UINT16_T dapp_module_lcore_num_get_by_type(dapp_modules_type_t type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return 0;
    }

    return MODULES.module[type].lcore.lcore_num;
}

dapp_modules_type_t dapp_module_type_get_by_lcore(UINT64_T lcore)
{
    int i = 0;
    UINT64_T t;

    for (i = 0; i < MODULES_ITEM(MODULES.module); ++i) {

        DAPP_MASK_TST(MODULES.module[i].lcore.lcore_mask, lcore, t);
    
        if (t) {
            return i;
        }
    }

    return DAPP_MODULE_TYPE_NUM;
}

STATUS DAPP_MODL_INIT_MACHINE(dapp_modules_type_t init_type, void *arg)
{
    int ret;

    if (DAPP_MODULE_TYPE_NUM <= init_type) {
        return DAPP_ERR_MODL_TYPE;
    }

    if (!MODULES.module[init_type].reg.reg) {
        return DAPP_ERR_MODL_UNREG;
    }
    
    if (!MODULES.module[init_type].reg.init) {
        return DAPP_ERR_MODL_INIT_NULL;
    }

    int i;

    for (i = 0; i < DAPP_MODULE_TYPE_NUM; ++i) {

        UINT64_T rely = 0;

        DAPP_MASK_TST(MODULES.module[init_type].rely.rely_mask, i, rely);

        if (rely) {
            DAPP_MODULE_INIT_WAIT(i);
        }
    }

    DAPP_MODULE_INIT_STATUS_SET(init_type, DAPP_MODULE_INIT_START);

    ret = MODULES.module[init_type].reg.init(arg);

    if (DAPP_OK != ret) {
        DAPP_MODULE_INIT_STATUS_SET(init_type, DAPP_MODULE_INIT_FAIL);
        return ret;
    }

    DAPP_MODULE_INIT_STATUS_SET(init_type, DAPP_MODULE_INIT_OK);

    return DAPP_OK;
}

STATUS DAPP_MODL_EXEC_MACHINE(dapp_modules_type_t exec_type, void *arg)
{
    int ret;

    if (DAPP_MODULE_TYPE_NUM <= exec_type) {
        return DAPP_ERR_MODL_TYPE;
    }

    if (!MODULES.module[exec_type].reg.reg) {
        return DAPP_ERR_MODL_UNREG;
    }
    
    if (!MODULES.module[exec_type].reg.exec) {
        return DAPP_ERR_MODL_EXEC_NULL;
    }

    int i;

    DAPP_MODULE_INIT_WAIT(exec_type);
    
    for (i = 0; i < DAPP_MODULE_TYPE_NUM; ++i) {

        UINT64_T rely = 0;

        DAPP_MASK_TST(MODULES.module[exec_type].rely.rely_mask, i, rely);

        if (rely) {
            DAPP_MODULE_INIT_WAIT(i);
        }
    }

    ret = MODULES.module[exec_type].reg.exec(&MODULES.module[exec_type].lcore.running, arg);

    if (DAPP_OK != ret) {
        return ret;
    }

    return DAPP_OK;
}

STATUS DAPP_MODL_EXIT_MACHINE(dapp_modules_type_t exit_type, void *arg)
{
    int ret;

    if (DAPP_MODULE_TYPE_NUM <= exit_type) {
        return DAPP_ERR_MODL_TYPE;
    }

    if (!MODULES.module[exit_type].reg.reg) {
        return DAPP_ERR_MODL_UNREG;
    }

    if (!MODULES.module[exit_type].reg.exit) {
        return DAPP_ERR_MODL_EXIT_NULL;
    }

    ret = MODULES.module[exit_type].reg.exit(arg);

    if (DAPP_OK != ret) {
        return ret;
    }

    return DAPP_OK;
}

void dapp_module_reg(dapp_modules_type_t type, 
                         const char *name, 
                         dapp_module_init init, 
                         dapp_module_exec exec, 
                         dapp_module_exit exit)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    /*
     * initialize basic information of module
     */
    snprintf(MODULES.module[type].reg.name, sizeof(MODULES.module[type].reg.name), "%s", name);
    MODULES.module[type].reg.init = init;
    MODULES.module[type].reg.exec = exec;
    MODULES.module[type].reg.exit = exit;

    /*
     * Set module registration flag
     */
    MODULES.module[type].reg.reg = 1;
}

void dapp_module_unreg(dapp_modules_type_t type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    /*
     * Cancel module registration flag
     */
    MODULES.module[type].reg.reg = 0;
}

void dapp_module_lcore_init(dapp_modules_type_t type, UINT16_T lcore_num)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    if (!MODULES.module[type].reg.reg) {
        return ;
    }

    int i = 0;
    UINT64_T mask = 0;

    for (i = 0; i < lcore_num; ++i) {
        /*
         * Initialize Module lcore Mask
         */
        DAPP_MASK_SET(MODULES.module[type].lcore.lcore_mask, MODULES.lcore_num + i);
        /*
         * Initialize program lcore Mask
         */
        DAPP_MASK_SET(MODULES.lcore_mask, MODULES.lcore_num + i);
    }

    /*
     * Set module running flag
     */
    MODULES.module[type].lcore.running = 1;
    MODULES.module[type].lcore.lcore_num = lcore_num;
    MODULES.lcore_num += lcore_num;
}

void dapp_module_lcore_uninit(dapp_modules_type_t type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    if (!MODULES.module[type].reg.reg) {
        return ;
    }

    /*
     * Cancel module running flag
     */
    MODULES.module[type].lcore.running = 0;
}

void dapp_module_rely_init(dapp_modules_type_t type, int rely_num, ...)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    if (!MODULES.module[type].reg.reg) {
        return ;
    }

    va_list valist;

    va_start(valist, rely_num);

    dapp_modules_type_t rely_type;

    int i;

    for (i = 0; i < rely_num; ++i) {

        rely_type = va_arg(valist, dapp_modules_type_t);

        if (DAPP_MODULE_TYPE_NUM <= rely_type) {
            continue;
        }

        DAPP_MASK_SET(MODULES.module[type].rely.rely_mask, rely_type);
        MODULES.module[type].rely.rely_num++;
    }

    if (MODULES.module[type].rely.rely_num != rely_num) {
        printf("exist unknow module type\n");
    }

    va_end(valist);
}

void dapp_module_rely_uninit(dapp_modules_type_t type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    if (!MODULES.module[type].reg.reg) {
        return ;
    }

    /*
     * Cancel module relevace
     */
    MODULES.module[type].rely.rely_mask = 0;
    MODULES.module[type].rely.rely_num = 0;
}

