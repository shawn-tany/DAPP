#include <stdio.h>
#include "modules.h"
#include "dapp_code.h"

static dapp_modules_table_t MODULES;

char *dapp_module_name_get(dapp_modules_type_t type)
{
    int i = 0;

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

dapp_module_t *dapp_module_get_by_lcore(UINT64_T lcore)
{
    int i = 0;
    UINT64_T t;

    for (i = 0; i < MODULES_ITEM(MODULES.module); ++i) {

        DAPP_MASK_TST(MODULES.module[i].lcore.lcore_mask, lcore, t);
    
        if (t) {
            return &MODULES.module[i];
        }
    }

    return NULL;
}

dapp_module_t *dapp_module_get_by_type(dapp_modules_type_t type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return NULL;
    }

    return &MODULES.module[type];
}

UINT8_T dapp_module_running(dapp_modules_type_t type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return 0;
    }

    return MODULES.module[type].lcore.running;
}

UINT16_T dapp_module_lcore_num_get_by_type(dapp_modules_type_t type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return 0;
    }

    return MODULES.module[type].lcore.lcore_num;
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